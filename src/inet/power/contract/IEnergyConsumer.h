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

#ifndef __INET_IENERGYCONSUMER_H
#define __INET_IENERGYCONSUMER_H

#include "inet/power/base/PowerDefs.h"

namespace inet {

namespace power {

class IEnergySource;

/**
 * This class is a base interface that must be implemented by energy consumer
 * models to integrate with other parts of the power model. This interface is
 * extended by various energy consumer interfaces. Actual energy consumer
 * implementations should implement one of the derived interfaces.
 *
 * See the corresponding NED file for more details.
 *
 */
class INET_API IEnergyConsumer
{
  public:
    virtual ~IEnergyConsumer() {}

    /**
     * Returns the energy source that provides energy for this energy consumer.
     * This function never returns nullptr.
     */
    virtual IEnergySource *getEnergySource() const = 0;
};

} // namespace power

} // namespace inet

#endif

