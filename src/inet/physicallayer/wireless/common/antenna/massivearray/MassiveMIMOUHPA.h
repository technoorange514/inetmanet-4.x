#pragma once
#ifndef __INET_MassiveMIMOUHPA_H
#define __INET_MassiveMIMOUHPA_H

#include <iostream>
#include <vector>
#include "inet/physicallayer/wireless/common/base/packetlevel/AntennaBase.h"
#include "inet/physicallayer/wireless/common/contract/packetlevel/IRadio.h"
#include "inet/power/contract/IEnergySource.h"
#include "inet/power/contract/IEnergyStorage.h"
#include "inet/power/storage/SimpleEpEnergyStorage.h"
#include "inet/physicallayer/wireless/common/medium/RadioMedium.h"
#include "inet/physicallayer/wireless/ieee80211/packetlevel/Ieee80211ScalarReceiver.h"
#include "inet/physicallayer/wireless/common/antenna/massivearray/MassiveArray.h"

#include <tuple>
#include <vector>
namespace inet {

namespace physicallayer {

using std::cout;
//extern double risInt;
class INET_API MassiveMIMOUHPA : public MassiveArray
{
    protected:
    class AntennaGain : public IAntennaGain
     {
       protected:
         m length;
         int M = -1;
         double  phiz;
         double freq;
         double distance;
         double risInt;
         ModuleRefByPar<power::IEnergySource> energySource;
         // IEnergySource *energySource = nullptr;
         int numAntennas;
         // internal state
         int energyConsumerId;
         double newConfigurtion = 0;
         W actualConsumption = W(0);
         MassiveMIMOUHPA *ourpa;
         IRadio *radio = nullptr;

       public:
         AntennaGain(m length, int M, double phiz, double freq, double distance, double risInt, MassiveMIMOUHPA *ourpa, IRadio *radio):
             length(length),
             M(M),
             phiz(phiz),
             freq(freq),
             distance (distance),
             risInt(risInt),
             ourpa(ourpa),
             radio(radio){}
         virtual m getLength() const {return length;}
         virtual double getMinGain() const override {return 0;}
         virtual double getMaxGain() const override;
         virtual double computeGain(const Quaternion &direction) const override;
         virtual double getAngolo(Coord p1, Coord p2)const;
         virtual double getPhizero() {return phiz; }
         virtual void setPhizero(double o) {phiz = o; }
         virtual double computeRecGain(const rad &direction) const;
     };

    protected:
         Ptr<AntennaGain> gain;
         // internal state
         int energyConsumerId;
         bool pendingConfiguration = false;
         double newConfigurtion = 0;
         W actualConsumption = W(0);

         double nextValue = NaN;
  protected:
    virtual void initialize(int stage) override;
    double computeIntegral();
    virtual void receiveSignal(cComponent *source, simsignal_t signalID, double d, cObject *details) override;
    virtual void receiveSignal(cComponent *source, simsignal_t signalID, intval_t d, cObject *details) override;

  public:
    MassiveMIMOUHPA();
    ~MassiveMIMOUHPA() {

    }
    virtual Ptr<const IAntennaGain> getGain() const override { return gain; }
    virtual std::ostream& printToStream(std::ostream& stream, int level, int evFlags = 0) const override;
    virtual void setDirection(const double &angle) override;
   // Consumption methods
//   virtual W getPowerConsumption() const override {return actualConsumption;}
/*   virtual W getPowerConsumption() const {return actualConsumption;}
   virtual void setConsumption()
   {
       if (energySource)
           energySource->setPowerConsumption(energyConsumerId,getPowerConsumption());
   }*/


};


} // namespace physicallayer

} // namespace inet

#endif // ifndef __INET_MassiveMIMOUHPA_H
