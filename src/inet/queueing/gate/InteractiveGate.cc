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

#include "inet/queueing/gate/InteractiveGate.h"

namespace inet {
namespace queueing {

Define_Module(InteractiveGate);

void InteractiveGate::initialize(int stage)
{
    PacketGateBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL)
        isOpen_ = par("open");
}

void InteractiveGate::handleParameterChange(const char *name)
{
    if (name != nullptr) {
        if (!strcmp(name, "open")) {
            if (par("open"))
                open();
            else
                close();
        }
    }
}

} // namespace queueing
} // namespace inet

