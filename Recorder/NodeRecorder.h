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
 * @class NodeRecorder
 * @brief A NodeRecorder class.
 *
 * @author tlc
 * @date 24/08/2017
 * @version 0.1.0
 * @file NodeRecorder.h
 * @addtogroup Recorder
 * @{
 */

#ifndef NODERECORDER_H
#define NODERECORDER_H

#include <Recorder/Recorder.h>

class NodeRecorder : public Recorder {
public:
    using Recorder::Recorder;

    void initialize(const shared_ptr<DomainBase>&) override;

    void record(const shared_ptr<DomainBase>&) override;

    void print() override;
};

#endif

//! @}
