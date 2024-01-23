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

#include "Axisymmetric.h"
#include <Domain/DomainBase.h>

const uvec Axisymmetric::F{0, 1, 2, 3};

Axisymmetric::Axisymmetric(const unsigned T, const unsigned BT)
    : Material2D(T, PlaneType::A, 0.)
    , base_tag(BT) {}

int Axisymmetric::initialize(const shared_ptr<DomainBase>& D) {
    base = D->initialized_material_copy(base_tag);

    if(nullptr == base || base->get_material_type() != MaterialType::D3) {
        suanpan_error("A valid 3D host material is required.\n");
        return SUANPAN_FAIL;
    }

    access::rw(density) = base->get_density();

    current_stiffness = trial_stiffness = initial_stiffness = base->get_initial_stiffness()(F, F);

    return SUANPAN_SUCCESS;
}

double Axisymmetric::get_parameter(const ParameterType P) const { return base->get_parameter(P); }

unique_ptr<Material> Axisymmetric::get_copy() { return make_unique<Axisymmetric>(*this); }

int Axisymmetric::update_trial_status(const vec& t_strain) {
    vec full_strain(6, fill::zeros);

    full_strain(F) = trial_strain = t_strain;

    if(SUANPAN_SUCCESS != base->update_trial_status(full_strain)) return SUANPAN_FAIL;

    trial_stress = base->get_trial_stress()(F);

    trial_stiffness = base->get_trial_stiffness()(F, F);

    return SUANPAN_SUCCESS;
}

int Axisymmetric::clear_status() {
    trial_strain = current_strain.zeros();
    trial_stress = current_stress.zeros();
    trial_stiffness = current_stiffness = initial_stiffness;
    return base->clear_status();
}

int Axisymmetric::commit_status() {
    current_strain = trial_strain;
    current_stress = trial_stress;
    current_stiffness = trial_stiffness;
    return base->commit_status();
}

int Axisymmetric::reset_status() {
    trial_strain = current_strain;
    trial_stress = current_stress;
    trial_stiffness = current_stiffness;
    return base->reset_status();
}

vector<vec> Axisymmetric::record(const OutputType P) { return base->record(P); }

void Axisymmetric::print() {
    suanpan_info("An axisymmetric wrapper.\n");
    if(base) base->print();
}
