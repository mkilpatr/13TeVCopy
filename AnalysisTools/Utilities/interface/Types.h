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
    kHLT_IsoMu24_eta2p1 = 1UL<<0,
    kHLT_IsoMu27 = 1UL<<1,
    kHLT_IsoTkMu24_eta2p1 = 1UL<<2,
    kHLT_IsoTkMu27 = 1UL<<3,
    kHLT_Ele27_eta2p1_WPLoose_Gsf = 1UL<<4,
    kHLT_Ele27_eta2p1_WP75_Gsf = 1UL<<5,
    kHLT_Ele32_eta2p1_WPLoose_Gsf = 1UL<<6,
    kHLT_Ele32_eta2p1_WP75_Gsf = 1UL<<7,
    kHLT_DiCentralPFJet55_PFMET110_NoiseCleaned = 1UL<<8,
    kHLT_DiCentralPFJet70_PFMET120_NoiseCleaned = 1UL<<9,
    kHLT_PFHT350_PFMET100_NoiseCleaned = 1UL<<10,
    kHLT_PFHT350_PFMET120_NoiseCleaned = 1UL<<11,
    kHLT_PFMET170_NoiseCleaned = 1UL<<12,
    kHLT_PFMET120_NoiseCleaned_BTagCSV07 = 1UL<<13,
    kHLT_PFMET120_NoiseCleaned_BTagCSV072 = 1UL<<14,
    kHLT_PFHT300 = 1UL<<15,
    kHLT_PFHT350 = 1UL<<16,
    kHLT_PFHT400 = 1UL<<17,
    kHLT_PFHT475 = 1UL<<18,
    kHLT_PFHT600 = 1UL<<19,
    kHLT_PFHT650 = 1UL<<20,
    kHLT_PFHT800 = 1UL<<21,
    kHLT_PFHT900 = 1UL<<22,
    kHLT_Photon120_R9Id90_HE10_IsoM = 1UL<<23,
    kHLT_Photon165_R9Id90_HE10_IsoM = 1UL<<24,
    kHLT_Photon165_HE10 = 1UL<<25,
    kHLT_Photon175 = 1UL<<26,
    kHLT_DiPFJetAve100_HFJEC = 1UL<<27,
    kHLT_DiPFJetAve140 = 1UL<<28,
    kHLT_DiPFJetAve160_HFJEC = 1UL<<29,
    kHLT_DiPFJetAve200 = 1UL<<30,
    kHLT_DiPFJetAve220_HFJEC = 1UL<<31,
    kHLT_DiPFJetAve260 = 1UL<<32,
    kHLT_DiPFJetAve300_HFJEC = 1UL<<33,
    kHLT_DiPFJetAve320 = 1UL<<34,
    kHLT_DiPFJetAve400 = 1UL<<35,
    kHLT_DiPFJetAve500 = 1UL<<36,
    kHLT_DiPFJetAve60_HFJEC = 1UL<<37,
    kHLT_DiPFJetAve60 = 1UL<<38,
    kHLT_DiPFJetAve80_HFJEC = 1UL<<39,
    kHLT_DiPFJetAve80 = 1UL<<40,
    kHLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ = 1UL<<41,
    kHLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ = 1UL<<42,
    kHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ = 1UL<<43,
    kHLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ = 1UL<<44,
    kHLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL = 1UL<<45,
    kHLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL = 1UL<<46,
    kHLT_IsoMu22 = 1UL<<47,
    kHLT_IsoTkMu22 = 1UL<<48,
    kHLT_Ele22_eta2p1_WPLoose_Gsf = 1UL<<49,
    kHLT_Ele22_eta2p1_WP75_Gsf = 1UL<<50,
    kHLT_Ele22_eta2p1_WPTight_Gsf = 1UL<<51,
    kHLT_Ele27_eta2p1_WPTight_Gsf = 1UL<<52,
    kHLT_Ele32_eta2p1_WPTight_Gsf = 1UL<<53,
    kHLT_PFHT350_PFMET100_JetIdCleaned = 1UL<<54,
    kHLT_PFMET170_JetIdCleaned = 1UL<<55,
    kHLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL = 1UL<<56,
    kHLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL = 1UL<<57,
    kHLT_Ele23_WPLoose_Gsf = 1UL<<58,
    kHLT_Ele27_WPLoose_Gsf = 1UL<<59,
    kHLT_IsoMu20 = 1UL<<60,
    kHLT_PFMET90_PFMHT90_IDTight = 1UL<<61,
    kHLT_PFMET100_PFMHT100_IDTight = 1UL<<62
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
    kSingleEle32 = 1UL<<10,
    kSingleIsoMu22 = 1UL<<11,
    kSingleIsoTkMu22 = 1UL<<12,
    kSingleEle22 = 1UL<<13,
    kSingleEle22WPTight = 1UL<<14,
    kSingleEle27 = 1UL<<15,
    kSingleEle27WPTight = 1UL<<16,
    kSingleEle32WPTight = 1UL<<17,
    kMu17Ele12MuLeg = 1UL<<18,
    kMu17Ele12EleLeg = 1UL<<19,
    kMu23Ele12MuLeg = 1UL<<20,
    kMu23Ele12EleLeg = 1UL<<21,
    kMu8Ele17MuLeg = 1UL<<22,
    kMu8Ele17EleLeg = 1UL<<23,
    kMu8Ele23MuLeg = 1UL<<24,
    kMu8Ele23EleLeg = 1UL<<25,
    kSingleEle27NoER = 1UL<<26,
    kSingleEle23NoER = 1UL<<27,
    kSingleIsoMu20 = 1UL<<28
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
