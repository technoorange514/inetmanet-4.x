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

#ifndef __INET_TOKENBUCKETMETERMIXIN_H
#define __INET_TOKENBUCKETMETERMIXIN_H

#include "inet/common/packet/Packet.h"
#include "inet/queueing/common/LabelsTag_m.h"

namespace inet {
namespace queueing {

template<typename T>
class INET_API TokenBucketMeterMixin : public T
{
  protected:
    double tokenConsumptionPerPacket = NaN;
    double tokenConsumptionPerBit = NaN;
    const char *defaultLabel = nullptr;
    std::vector<std::string> labelPriority;

  protected:
    virtual void initialize(int stage) override
    {
        T::initialize(stage);
        if (stage == INITSTAGE_LOCAL) {
            tokenConsumptionPerPacket = T::par("tokenConsumptionPerPacket");
            tokenConsumptionPerBit = T::par("tokenConsumptionPerBit");
            defaultLabel = T::par("defaultLabel");
            labelPriority = check_and_cast<cValueArray*>(T::par("labelPriority").objectValue())->asStringVector();
        }
    }

    virtual void labelPacket(Packet *packet, const char *label)
    {
        auto labelsTag = packet->addTagIfAbsent<LabelsTag>();
        if (labelPriority.empty())
            labelsTag->insertLabels(label);
        else {
            int newPriority = std::find(labelPriority.begin(), labelPriority.end(), label) - labelPriority.begin();
            for (int i = 0; i < labelsTag->getLabelsArraySize(); i++) {
                auto it = std::find(labelPriority.begin(), labelPriority.end(), labelsTag->getLabels(i));
                if (it != labelPriority.end()) {
                    int oldPriority = it - labelPriority.begin();
                    if (oldPriority < newPriority)
                        labelsTag->eraseLabels(i);
                    else
                        return;
                }
            }
            labelsTag->insertLabels(label);
        }
    }

    virtual double getNumPacketTokens(Packet *packet) const
    {
        return b(packet->getDataLength()).get() * tokenConsumptionPerBit + tokenConsumptionPerPacket;
    }
};

} // namespace queueing
} // namespace inet

#endif

