//
// Copyright (C) 2020 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//

#ifndef __INET_IENERGYSTORAGE_H
#define __INET_IENERGYSTORAGE_H

#include "inet/power/contract/IEnergySink.h"
#include "inet/power/contract/IEnergySource.h"

namespace inet {

namespace power {

/**
 * This class is a base interface that must be implemented by energy storage
 * models to integrate with other parts of the power model. This interface is
 * extended by various energy storage interfaces. Actual energy storage
 * implementations should implement one of the derived interfaces.
 *
 * See the corresponding NED file for more details.
 *
 */
class INET_API IEnergyStorage : public virtual IEnergySource, public virtual IEnergySink
{
};

} // namespace power

} // namespace inet

#endif

