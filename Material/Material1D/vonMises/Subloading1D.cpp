/*******************************************************************************
 * Copyright (C) 2017-2024 Theodore Chang
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

#include "Subloading1D.h"

const double Subloading1D::rate_bound = -log(z_bound);

vec2 Subloading1D::yield_ratio(const double z) {
    if(z < z_bound) return {rate_bound, 0.};

    return {-log(z), -1. / z};
}

Subloading1D::Subloading1D(const unsigned T, DataSubloading1D&& D, const double R)
    : DataSubloading1D{std::move(D)}
    , Material1D(T, R) {}

int Subloading1D::initialize(const shared_ptr<DomainBase>&) {
    trial_stiffness = current_stiffness = initial_stiffness = elastic;

    initialize_history(4);

    return SUANPAN_SUCCESS;
}

unique_ptr<Material> Subloading1D::get_copy() { return make_unique<Subloading1D>(*this); }

int Subloading1D::update_trial_status(const vec& t_strain) {
    incre_strain = (trial_strain = t_strain) - current_strain;

    if(fabs(incre_strain(0)) <= datum::eps) return SUANPAN_SUCCESS;

    trial_stress = current_stress + (trial_stiffness = initial_stiffness) * incre_strain;

    trial_history = current_history;
    const auto& current_alpha = current_history(0);
    const auto& current_d = current_history(1);
    const auto& current_q = current_history(2);
    const auto& current_z = current_history(3);
    auto& alpha = trial_history(0);
    auto& d = trial_history(1);
    auto& q = trial_history(2);
    auto& z = trial_history(3);

    auto gamma = 0., ref_error = 0.;

    vec2 residual, incre;
    mat22 jacobian;

    const auto current_ratio = yield_ratio(current_z);

    auto counter = 0u;
    while(true) {
        if(max_iteration == ++counter) {
            suanpan_error("Cannot converge within {} iterations.\n", max_iteration);
            return SUANPAN_FAIL;
        }

        q = current_q + gamma;

        const auto exp_iso = saturation_iso * exp(-m_iso * q);
        auto y = initial_iso + saturation_iso + k_iso * q - exp_iso;
        auto dy = k_iso + m_iso * exp_iso;
        if(y < 0.) y = dy = 0.;

        const auto exp_kin = saturation_kin * exp(-m_kin * q);
        auto a = initial_kin + saturation_kin + k_kin * q - exp_kin;
        auto da = k_kin + m_kin * exp_kin;
        if(a < 0.) a = da = 0.;

        const auto n = trial_stress(0) - current_alpha / (1. + be * gamma) + (z - 1.) * y * current_d / (1. + ce * gamma) > 0. ? 1. : -1.;

        auto top = be * gamma * a * n + current_alpha;
        auto bottom = 1. + be * gamma;

        alpha = top / bottom;
        const auto dalpha = (be * n * (a + gamma * da) - top * be / bottom) / bottom;

        top = ce * ze * gamma * n + current_d;
        bottom = 1. + ce * gamma;

        d = top / bottom;
        const auto dd = (ce * ze * n - top * ce / bottom) / bottom;

        const auto trial_ratio = yield_ratio(z);
        const auto avg_rate = u * .5 * (current_ratio(0) + trial_ratio(0));

        residual(0) = fabs(trial_stress(0) - elastic * gamma * n - alpha + (z - 1.) * y * d) - z * y;

        if(1u == counter && residual(0) < 0.) {
            z = ((trial_stress(0) - alpha) / y - d) / (n - d);
            return SUANPAN_SUCCESS;
        }

        residual(1) = z - current_z - gamma * avg_rate;

        jacobian(0, 0) = n * ((z - 1.) * (dd * y + dy * d) - dalpha) - elastic - z * dy;
        jacobian(0, 1) = n * y * d - y;

        jacobian(1, 0) = -avg_rate;
        jacobian(1, 1) = 1. - u * gamma * .5 * trial_ratio(1);

        if(!solve(incre, jacobian, residual)) return SUANPAN_FAIL;

        const auto error = inf_norm(incre);
        if(1u == counter) ref_error = error;
        suanpan_debug("Local iteration error: {:.5E}.\n", error);
        if(error < tolerance * ref_error || ((error < tolerance || inf_norm(residual) < tolerance) && counter > 5u)) {
            trial_stress -= elastic * gamma * n;
            trial_stiffness += elastic / det(jacobian) * elastic * jacobian(1, 1);

            return SUANPAN_SUCCESS;
        }

        gamma -= incre(0);
        z -= incre(1);
        if(z > 1.) z = 1. - datum::eps;
    }
}

int Subloading1D::clear_status() {
    current_strain.zeros();
    current_stress.zeros();
    current_history = initial_history;
    current_stiffness = initial_stiffness;
    return reset_status();
}

int Subloading1D::commit_status() {
    current_strain = trial_strain;
    current_stress = trial_stress;
    current_history = trial_history;
    current_stiffness = trial_stiffness;
    return SUANPAN_SUCCESS;
}

int Subloading1D::reset_status() {
    trial_strain = current_strain;
    trial_stress = current_stress;
    trial_history = current_history;
    trial_stiffness = current_stiffness;
    return SUANPAN_SUCCESS;
}

void Subloading1D::print() {
    suanpan_info("A uniaxial combined hardening material using subloading surface model.\n");
    Material1D::print();
}
