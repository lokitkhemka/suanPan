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
/**
 * @class Tchamwa
 * @brief A Tchamwa class defines a solver using Tchamwa algorithm.
 *
 * @author tlc
 * @date 03/12/2022
 * @version 0.1.0
 * @file Tchamwa.h
 * @addtogroup Integrator
 * @{
 */

#ifndef TCHAMWA_H
#define TCHAMWA_H

#include "Integrator.h"

class Tchamwa final : public ExplicitIntegrator {
    const double PHI;
    double DT{0.};

public:
    Tchamwa(unsigned, double);

    void assemble_resistance() override;
    void assemble_matrix() override;

    int update_trial_status() override;

    void update_parameter(double) override;

    void print() override;
};

#endif

//! @}
