//
// Copyright (C) 2018 OpenSim Ltd.
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

#include "inet/networklayer/rsvpte/RsvpProtocolDissector.h"

#include "inet/common/packet/dissector/ProtocolDissectorRegistry.h"
#include "inet/networklayer/rsvpte/RsvpPacket_m.h"

namespace inet {

Register_Protocol_Dissector(&Protocol::rsvpTe, RsvpProtocolDissector);

void RsvpProtocolDissector::dissect(Packet *packet, const Protocol *protocol, ICallback& callback) const
{
    auto header = packet->popAtFront<RsvpMessage>();
    callback.startProtocolDataUnit(&Protocol::rsvpTe);
    callback.visitChunk(header, &Protocol::rsvpTe);
    ASSERT(packet->getDataLength() == B(0));
    callback.endProtocolDataUnit(&Protocol::rsvpTe);
}

} // namespace inet

