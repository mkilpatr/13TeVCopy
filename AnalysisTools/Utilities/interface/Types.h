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
#include "TString.h"
#include "Math/PtEtaPhiE4D.h"
#include "Math/PtEtaPhiM4D.h"
#include "Math/LorentzVector.h"


namespace ucsbsusy {

//integer types used for storage
  typedef   unsigned char        size8 ; // 8 bit 0->255
  typedef   unsigned short int   size16; //16 bit 0->65536
  typedef   unsigned int         size  ; //32 bit 0->4294967296
  typedef   char                 int8  ;
  typedef   short int            int16 ;

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

  //Conversion function to check validity
  template<typename Target, typename Source>
  Target convertTo(Source source, const char name[], bool lenient = false, bool* good = 0)
  {
    Target            converted = static_cast<Target>(source);

    if (static_cast<Source>(converted) != source) {
      const Target    lowest    = !std::numeric_limits<Target>::is_signed
                                ? 0
                                : std::numeric_limits<Target>::has_infinity
                                ? -std::numeric_limits<Target>::infinity()
                                :  std::numeric_limits<Target>::min()
                                ;

      std::string problem = "convertTo(): Source value " + std::to_string((double)  source) + " outside of target range "
                           +"["+std::to_string((double)  lowest)+","+std::to_string((double)  std::numeric_limits<Target>::max())+"]"
                           + " for '" + name +"'";

      if (good)      *good      = false;
      if (lenient) {
        std::cerr << "WARNING: " << problem << std::endl;
        return  ( source > static_cast<Source>(std::numeric_limits<Target>::max())
                ? std::numeric_limits<Target>::max()
                : lowest
                );
      }
      throw std::range_error( problem);
    }

    return converted;
  }

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

  enum TriggerBits {
    kHLT_IsoMu24_eta2p1_v1 = 1UL<<0,
    kHLT_IsoMu27_v1 = 1UL<<1,
    kHLT_IsoTkMu24_eta2p1_v1 = 1UL<<2,
    kHLT_IsoTkMu27_v1 = 1UL<<3,
    kHLT_Ele32_eta2p1_WPLoose_Gsf_v1 = 1UL<<4,
    kHLT_Ele32_eta2p1_WP75_Gsf_v1 = 1UL<<5,
    kHLT_DiCentralPFJet55_PFMET110_NoiseCleaned_v1 = 1UL<<6,
    kHLT_DiCentralPFJet70_PFMET120_NoiseCleaned_v1 = 1UL<<7,
    kHLT_PFHT350_PFMET100_NoiseCleaned_v1 = 1UL<<8,
    kHLT_PFHT350_PFMET120_NoiseCleaned_v1 = 1UL<<9,
    kHLT_PFMET170_NoiseCleaned_v1 = 1UL<<10,
    kHLT_PFMET170_NoiseCleaned_v2 = 1UL<<11,
    kHLT_PFMET120_NoiseCleaned_BTagCSV07_v1 = 1UL<<12,
    kHLT_PFMET120_NoiseCleaned_BTagCSV072_v1 = 1UL<<13,
    kHLT_PFMET120_NoiseCleaned_BTagCSV072_v2 = 1UL<<14,
    kHLT_PFHT300_v1 = 1UL<<15,
    kHLT_PFHT350_v1 = 1UL<<16,
    kHLT_PFHT350_v2 = 1UL<<17,
    kHLT_PFHT400_v1 = 1UL<<18,
    kHLT_PFHT475_v1 = 1UL<<19,
    kHLT_PFHT600_v1 = 1UL<<20,
    kHLT_PFHT600_v2 = 1UL<<21,
    kHLT_PFHT650_v1 = 1UL<<22,
    kHLT_PFHT800_v1 = 1UL<<23,
    kHLT_PFHT900_v1 = 1UL<<24,
    kHLT_IsoMu24_eta2p1_v2 = 1UL<<25,
    kHLT_IsoMu27_v2 = 1UL<<26,
    kHLT_IsoTkMu24_eta2p1_v2 = 1UL<<27,
    kHLT_IsoTkMu27_v2 = 1UL<<28,
    kHLT_Photon120_R9Id90_HE10_IsoM_v2 = 1UL<<29,
    kHLT_Photon165_R9Id90_HE10_IsoM_v2 = 1UL<<30,
    kHLT_Photon165_HE10_v2 = 1UL<<31,
    kHLT_Photon175_v2 = 1UL<<32
  };

  enum TriggerFilters {
    kPFMET100 = 1UL<<0,
    kPFMET110 = 1UL<<1,
    kPFMET120 = 1UL<<2,
    kPFHT350 = 1UL<<3,
    kDiCentralPFJet55 = 1UL<<4,
    kDiCentralPFJet70 = 1UL<<5,
    kSingleIsoMu24 = 1UL<<6,
    kSingleIsoMu27 = 1UL<<7,
    kSingleIsoTkMu24 = 1UL<<8,
    kSingleIsoTkMu27 = 1UL<<9,
    kSingleEle32 = 1UL<<10
  };

  enum METFilterBits {
    fFlag_trackingFailureFilter = 1UL<<0,
    fFlag_goodVertices = 1UL<<1,
    fFlag_CSCTightHaloFilter = 1UL<<2,
    fFlag_trkPOGFilters = 1UL<<3,
    fFlag_trkPOG_logErrorTooManyClusters = 1UL<<4,
    fFlag_EcalDeadCellTriggerPrimitiveFilter = 1UL<<5,
    fFlag_ecalLaserCorrFilter = 1UL<<6,
    fFlag_trkPOG_manystripclus53X = 1UL<<7,
    fFlag_eeBadScFilter = 1UL<<8,
    fFlag_METFilters = 1UL<<9,
    fFlag_HBHENoiseFilter = 1UL<<10,
    fFlag_trkPOG_toomanystripclus53X = 1UL<<11,
    fFlag_hcalLaserEventFilter = 1UL<<12
  };

}

#endif
