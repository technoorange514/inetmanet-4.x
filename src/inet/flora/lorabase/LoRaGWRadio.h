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
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef LORA_LORAGWRADIO_H_
#define LORA_LORAGWRADIO_H_

#include "inet/flora/lorabase/LoRaMacFrame_m.h"
#include "inet/flora/loraphy/LoRaMedium.h"
#include "inet/flora/loraphy/LoRaReceiver.h"
#include "inet/flora/loraphy/LoRaReception.h"
#include "inet/flora/loraphy/LoRaTransmission.h"
#include "inet/flora/loraphy/LoRaTransmitter.h"
#include "inet/physicallayer/wireless/common/base/packetlevel/NarrowbandRadioBase.h"
#include "inet/physicallayer/wireless/common//medium/RadioMedium.h"
#include "inet/common/LayeredProtocolBase.h"

namespace inet {

namespace flora {
using namespace physicallayer;
class LoRaGWRadio : public NarrowbandRadioBase {
private:
    void completeRadioModeSwitch(RadioMode newRadioMode);
protected:
    void initialize(int stage) override;
    virtual void finish() override;
    virtual void handleSelfMessage(cMessage *message) override;
    virtual void handleUpperPacket(Packet *packet) override;
    void handleSignal(WirelessSignal *radioFrame) override;

    bool iAmTransmiting;
    virtual bool isTransmissionTimer(const cMessage *message) const;
    virtual void handleTransmissionTimer(cMessage *message) override;
    virtual void startTransmission(Packet *macFrame, IRadioSignal::SignalPart part) override;
    virtual void continueTransmission(cMessage *timer);
    virtual void endTransmission(cMessage *timer);

    virtual bool isReceptionTimer(const cMessage *message) const override;
    virtual void startReception(cMessage *timer, IRadioSignal::SignalPart part) override;
    virtual void continueReception(cMessage *timer) override;
    virtual void endReception(cMessage *timer) override;
    virtual void abortReception(cMessage *timer) override;


public:
    LoRaGWRadio() { }
    bool iAmGateway;

    std::list<cMessage *>concurrentReceptions;
    std::list<cMessage *>concurrentTransmissions;

    long LoRaGWRadioReceptionStarted_counter;
    long LoRaGWRadioReceptionFinishedCorrect_counter;
    simsignal_t LoRaGWRadioReceptionStarted;
    simsignal_t LoRaGWRadioReceptionFinishedCorrect;


    virtual void setPower(W newPower);
    virtual void setBitrate(bps newBitrate);

};

}

}

#endif /* LORA_LORAGWRADIO_H_ */
