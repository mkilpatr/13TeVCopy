//--------------------------------------------------------------------------------------------------
// 
// Types
// 
// Class with definitions of various types.
// 
// Types.h created on Tue Oct 21 21:34:00 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_UTILITIES_TYPES_H
#define ANALYSISTOOLS_UTILITIES_TYPES_H

#include <map>
#include <iostream>

#include "Rtypes.h"
#include "Math/PtEtaPhiE4D.h"
#include "Math/PtEtaPhiM4D.h"
#include "Math/LorentzVector.h"


namespace ucsbsusy {

  typedef   unsigned int                  size;
  typedef   unsigned char                 multiplicity;
  typedef   std::pair<double,int>         RankedIndex;
  typedef   std::pair<int,int>            CountedIndex;


  typedef ROOT::Math::PtEtaPhiM4D<double>				CylLorentzCoord;
  typedef ROOT::Math::PtEtaPhiM4D<float> 				CylLorentzCoordF;
  typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> >	XYZTLorentzVectorF;
  typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> >	PtEtaPhiMLorentzVectorD;
  typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> >	PtEtaPhiMLorentzVectorF;
  typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> >	CartLorentzVector;
  typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> >	CylLorentzVector;
  typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> >	CylLorentzVectorF;

  typedef std::map<std::string, unsigned long>	TauIdMap;

  enum TaggableType	{ TAGGABLE_MONO_B, TAGGABLE_MULTI_B, TAGGABLE_CHARM, TAGGABLE_LIGHT, TAGGABLE_GLUON, TAGGABLE_NONB, numTaggableTypes };

  enum TauIdFlags {
    kDecayMode		= 1UL<<0,
    kDecayModeNew	= 1UL<<1,
    kVLooseIsoDB3Hits	= 1UL<<2,
    kLooseIsoDB3Hits	= 1UL<<3,
    kMediumIsoDB3Hits	= 1UL<<4,
    kTightIsoDB3Hits	= 1UL<<5,
    kVLooseIsoMVAnoLT	= 1UL<<6,
    kLooseIsoMVAnoLT	= 1UL<<7,
    kMediumIsoMVAnoLT	= 1UL<<8,
    kTightIsoMVAnoLT	= 1UL<<9,
    kVTightIsoMVAnoLT	= 1UL<<10,
    kVVTightIsoMVAnoLT	= 1UL<<11,
    kVLooseIsoMVALT	= 1UL<<12,
    kLooseIsoMVALT	= 1UL<<13,
    kMediumIsoMVALT	= 1UL<<14,
    kTightIsoMVALT	= 1UL<<15,
    kVTightIsoMVALT	= 1UL<<16,
    kVVTightIsoMVALT	= 1UL<<17,
    kLooseEle		= 1UL<<18,
    kMediumEle		= 1UL<<19,
    kTightEle		= 1UL<<20,
    kVLooseEleMVA	= 1UL<<21,
    kLooseEleMVA	= 1UL<<22,
    kMediumEleMVA	= 1UL<<23,
    kTightEleMVA	= 1UL<<24,
    kVTightEleMVA	= 1UL<<25,
    kLooseMu		= 1UL<<26,
    kMediumMu		= 1UL<<27,
    kTightMu		= 1UL<<28,
    kLooseMu2		= 1UL<<29,
    kMediumMu2		= 1UL<<30,
    kTightMu2		= 1UL<<31,
    kLooseMu3		= 1UL<<32,
    kTightMu3		= 1UL<<33,
    kLooseMuMVA		= 1UL<<34,
    kMediumMuMVA	= 1UL<<35,
    kTightMuMVA		= 1UL<<36
  };


}

#endif
