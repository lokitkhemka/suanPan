/*******************************************************************************
 * Copyright (C) 2017-2022 Theodore Chang
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
 * @class NM3D3
 * @brief A NM3D3 class.
 * @author tlc
 * @date 26/11/2021
 * @version 0.1.0
 * @file NM3D3.h
 * @addtogroup Section-NM
 * @ingroup Section
 * @{
 */

#ifndef NM3D3_H
#define NM3D3_H

#include "SurfaceNM3D.h"
#include "VAFNM.h"

class NM3D3 final : protected SurfaceNM3D, public VAFNM {
protected:
    [[nodiscard]] double compute_f(const vec&, const vec&) const override;
    [[nodiscard]] vec compute_df(const vec&, const vec&) const override;
    [[nodiscard]] mat compute_ddf(const vec&, const vec&) const override;

public:
    NM3D3(unsigned, // tag
          double,   // EA
          double,   // EIS
          double,   // EIW
          double,   // NP
          double,   // MPS
          double,   // MPW
          double,   // c
          double,   // h
          double,   // h
          double,   // h
          vec&&,    // k
          vec&&,    // k
          double,   // linear density
          mat&& = {});

    unique_ptr<Section> get_copy() override;

    void print() override;
};

#endif

//! @}
