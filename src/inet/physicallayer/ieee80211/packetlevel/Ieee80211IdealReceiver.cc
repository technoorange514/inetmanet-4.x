//
// Copyright (C) 2013 OpenSim Ltd.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#include "inet/mobility/contract/IMobility.h"
#include "inet/physicallayer/contract/packetlevel/IRadio.h"
#include "inet/physicallayer/contract/packetlevel/RadioControlInfo_m.h"
#include "inet/physicallayer/unitdisk/UnitDiskNoise.h"
#include "inet/physicallayer/unitdisk/UnitDiskReception.h"
#include "inet/physicallayer/unitdisk/UnitDiskTransmission.h"
#include "inet/physicallayer/ieee80211/packetlevel/Ieee80211ControlInfo_m.h"
#include "inet/physicallayer/ieee80211/packetlevel/Ieee80211IdealReceiver.h"
#include "inet/physicallayer/ieee80211/packetlevel/Ieee80211Tag_m.h"
#include "inet/physicallayer/ieee80211/packetlevel/Ieee80211TransmissionBase.h"

namespace inet {

namespace physicallayer {

Define_Module(Ieee80211IdealReceiver);

Ieee80211IdealReceiver::Ieee80211IdealReceiver() :
    UnitDiskReceiver()
{
}

void Ieee80211IdealReceiver::initialize(int stage)
{
    if (stage == INITSTAGE_LOCAL) {
    }
}

std::ostream& Ieee80211IdealReceiver::printToStream(std::ostream& stream, int level) const
{
    stream << "Ieee80211IdealReceiver";
    return UnitDiskReceiver::printToStream(stream, level);
}

const IReceptionResult *Ieee80211IdealReceiver::computeReceptionResult(const IListening *listening, const IReception *reception, const IInterference *interference, const ISnir *snir, const std::vector<const IReceptionDecision *> *decisions) const
{
    auto transmission = check_and_cast<const Ieee80211TransmissionBase *>(reception->getTransmission());
    auto receptionResult = UnitDiskReceiver::computeReceptionResult(listening, reception, interference, snir, decisions);
    auto modeInd = const_cast<Packet *>(receptionResult->getPacket())->ensureTag<Ieee80211ModeInd>();
    modeInd->setMode(transmission->getMode());
    auto channelInd = const_cast<Packet *>(receptionResult->getPacket())->ensureTag<Ieee80211ChannelInd>();
    channelInd->setChannel(transmission->getChannel());
    return receptionResult;
}

} // namespace physicallayer

} // namespace inet

