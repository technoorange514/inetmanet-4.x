//
// Copyright (C) 2013 OpenSim Ltd.
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

#ifndef __INET_ISIGNALBITMODEL_H
#define __INET_ISIGNALBITMODEL_H

#include "inet/physicallayer/wireless/common/contract/bitlevel/IFecCoder.h"
#include "inet/physicallayer/wireless/common/contract/bitlevel/IInterleaver.h"
#include "inet/physicallayer/wireless/common/contract/bitlevel/IScrambler.h"
#include "inet/physicallayer/wireless/common/contract/packetlevel/IModulation.h"

namespace inet {
namespace physicallayer {

/**
 * This purely virtual interface provides an abstraction for different radio
 * signal models in the bit domain.
 */
class INET_API ISignalBitModel : public IPrintableObject
{
  public:
    virtual b getHeaderLength() const = 0;
    /**
     * Returns the gross (physical) bitrate of the PHY frame header.
     */
    virtual bps getHeaderBitRate() const = 0;
    virtual b getDataLength() const = 0;

    /**
     * Returns the gross (physical) bitrate of the PHY frame data.
     */
    virtual bps getDataBitRate() const = 0;
    virtual const BitVector *getBits() const = 0;
};

class INET_API ITransmissionBitModel : public virtual ISignalBitModel
{
  public:
    virtual const IForwardErrorCorrection *getForwardErrorCorrection() const = 0;
    virtual const IScrambling *getScrambling() const = 0;
    virtual const IInterleaving *getInterleaving() const = 0;
};

class INET_API IReceptionBitModel : public virtual ISignalBitModel
{
  public:
    virtual double getBitErrorRate() const = 0;
};

} // namespace physicallayer
} // namespace inet

#endif

