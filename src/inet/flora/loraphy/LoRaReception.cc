/*
 * LoRaReception.cpp
 *
 *  Created on: Feb 17, 2017
 *      Author: slabicm1
 */

#include "inet/flora/loraphy/LoRaReception.h"

namespace inet {

namespace flora {

LoRaReception::LoRaReception(const IRadio *radio, const ITransmission *transmission, const simtime_t startTime, const simtime_t endTime, const Coord startPosition, const Coord endPosition, const Quaternion startOrientation, const Quaternion endOrientation, Hz LoRaCF, Hz LoRaBW, W receivedPower, int LoRaSF, int LoRaCR) :
        ScalarReception(radio, transmission, startTime, endTime, startPosition, endPosition, startOrientation, endOrientation, LoRaCF, LoRaBW, receivedPower),
        LoRaSF(LoRaSF),
        LoRaCR(LoRaCR),
        receivedPower(receivedPower)
{
}

W LoRaReception::computeMinPower(simtime_t startTime, simtime_t endTime) const
{
    return receivedPower;
}

}

}
