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

#ifndef __INET_PACKETDROPPERFUNCTION_H
#define __INET_PACKETDROPPERFUNCTION_H

#include "inet/queueing/contract/IPacketDropperFunction.h"

namespace inet {
namespace queueing {

typedef Packet *(*PacketDropperFunction)(IPacketCollection *packets);

class INET_API CPacketDropperFunction : public cObject, public virtual IPacketDropperFunction
{
  protected:
    PacketDropperFunction packetDropperFunction;

  public:
    CPacketDropperFunction(PacketDropperFunction packetDropperFunction) : packetDropperFunction(packetDropperFunction) {}

    virtual Packet *selectPacket(IPacketCollection *collection) const override;
};

#define Register_Packet_Dropper_Function(name, function) \
    class INET_API name : public ::inet::queueing::CPacketDropperFunction { \
      public: name() : CPacketDropperFunction(function) {} \
    }; \
    Register_Class(name)

} // namespace queueing
} // namespace inet

#endif

