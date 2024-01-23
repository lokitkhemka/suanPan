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

#include "Amplitude.h"

Amplitude::Amplitude(const unsigned T, const unsigned ST)
    : Tag(T)
    , start_step(ST) {}

void Amplitude::initialize(const shared_ptr<DomainBase>&) {}

double Amplitude::get_amplitude(const double) { return 0.; }

void Amplitude::set_start_step(const unsigned T) { start_step = T; }

unsigned Amplitude::get_start_step() const { return start_step; }

void Amplitude::set_start_time(const double T) { start_time = T; }

double Amplitude::set_start_time() const { return start_time; }
