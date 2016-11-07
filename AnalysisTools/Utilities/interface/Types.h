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
    kDecayModeOld	= 1UL<<1,
    kDecayModeNew	= 1UL<<2,
    kVLooseIsoDB3Hits	= 1UL<<3,
    kLooseIsoDB3Hits	= 1UL<<4,
    kMediumIsoDB3Hits	= 1UL<<5,
    kTightIsoDB3Hits	= 1UL<<6,
    kVLooseIsoMVAnoLT	= 1UL<<7,
    kLooseIsoMVAnoLT	= 1UL<<8,
    kMediumIsoMVAnoLT	= 1UL<<9,
    kTightIsoMVAnoLT	= 1UL<<10,
    kVTightIsoMVAnoLT	= 1UL<<11,
    kVVTightIsoMVAnoLT	= 1UL<<12,
    kVLooseIsoMVALT	= 1UL<<13,
    kLooseIsoMVALT	= 1UL<<14,
    kMediumIsoMVALT	= 1UL<<15,
    kTightIsoMVALT	= 1UL<<16,
    kVTightIsoMVALT	= 1UL<<17,
    kVVTightIsoMVALT	= 1UL<<18,
    kLooseEle		= 1UL<<19,
    kMediumEle		= 1UL<<20,
    kTightEle		= 1UL<<21,
    kVLooseEleMVA	= 1UL<<22,
    kLooseEleMVA	= 1UL<<23,
    kMediumEleMVA	= 1UL<<24,
    kTightEleMVA	= 1UL<<25,
    kVTightEleMVA	= 1UL<<26,
    kLooseMu		= 1UL<<27,
    kMediumMu		= 1UL<<28,
    kTightMu		= 1UL<<29,
    kLooseMu2		= 1UL<<30,
    kMediumMu2		= 1UL<<31,
    kTightMu2		= 1UL<<32,
    kLooseMu3		= 1UL<<33,
    kTightMu3		= 1UL<<34,
    kLooseMuMVA		= 1UL<<35,
    kMediumMuMVA	= 1UL<<36,
    kTightMuMVA		= 1UL<<37,
    kVLooseIsoMVARun2newDMwLT     = 1UL<<38,
    kLooseIsoMVARun2newDMwLT      = 1UL<<39,
    kMediumIsoMVARun2newDMwLT     = 1UL<<40,
    kTightIsoMVARun2newDMwLT      = 1UL<<41,
    kVTightIsoMVARun2newDMwLT     = 1UL<<42,
    kLooseIsoMVARun2dR03oldDMwLT  = 1UL<<43,
    kMediumIsoMVARun2dR03oldDMwLT = 1UL<<44,
    kTightIsoMVARun2dR03oldDMwLT  = 1UL<<45,
    kVTightIsoMVARun2dR03oldDMwLT = 1UL<<46,
    kLooseIsoDB3HitsdR03          = 1UL<<47,
    kMediumIsoDB3HitsdR03         = 1UL<<48,
    kTightIsoDB3HitsdR03          = 1UL<<49,
  };

  enum TriggerBits {

    kHLT_PFHT300_PFMET110 = 1UL<<11,
    kHLT_PFMET170_HBHECleaned = 1UL<<12,
    kHLT_PFMET110_PFMHT110_IDTight = 1UL<<13,
    kHLT_PFMETNoMu110_PFMHTNoMu110_IDTight = 1UL<<14,
    kHLT_PFMET120_PFMHT120_IDTight = 1UL<<15,
    kHLT_PFMETNoMu120_PFMHTNoMu120_IDTight = 1UL<<16,
    kHLT_PFHT400 = 1UL<<17,
    kHLT_PFHT600 = 1UL<<18,
    kHLT_PFHT900 = 1UL<<19,
    kHLT_Mu50_IsoVVVL_PFHT400 = 1UL << 20,
    kHLT_Ele50_IsoVVVL_PFHT400 = 1UL << 21,
    kHLT_ECALHT800 = 1UL<<22,
    kHLT_CaloJet500_NoJetID = 1UL<<23,
    kHLT_Photon165_R9Id90_HE10_IsoM = 1UL<<24,
    kHLT_Photon165_HE10 = 1UL<<25,
    kHLT_Photon175 = 1UL<<26,
    kHLT_Ele25_eta2p1_WPTight_Gsf = 1UL<<27,
    kHLT_Ele27_eta2p1_WPTight_Gsf = 1UL<<28,
    kHLT_Ele27_WPTight_Gsf = 1UL<<29,
    kHLT_Ele15_IsoVVVL_PFHT350_PFMET50 = 1UL<<30,
    kHLT_Ele15_IsoVVVL_PFHT400_PFMET50 = 1UL<<31,
    kHLT_Ele15_IsoVVVL_PFHT350 = 1UL<<32,
    kHLT_Ele15_IsoVVVL_PFHT400 = 1UL<<33,
    kHLT_Ele15_IsoVVVL_PFHT600 = 1UL<<34,
    kHLT_Mu15_IsoVVVL_PFHT350_PFMET50 = 1UL<<35,
    kHLT_Mu15_IsoVVVL_PFHT400_PFMET50 = 1UL<<36,
    kHLT_Mu15_IsoVVVL_PFHT350 = 1UL<<37,
    kHLT_Mu15_IsoVVVL_PFHT400 = 1UL<<38,
    kHLT_Mu15_IsoVVVL_PFHT600 = 1UL<<39,
    kHLT_Ele115_CaloIdVT_GsfTrkIdT = 1UL<<40,
    kHLT_DoubleEle33_CaloIdL_GsfTrkIdVL_MW = 1UL<<41,
    kHLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ = 1UL<<42,
    kHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ = 1UL<<43,
    kHLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ = 1UL<<44,
    kHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL = 1UL<<45,
    kHLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL = 1UL<<46,
    kHLT_TkMu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ = 1UL<<47,
    kHLT_TkMu17_TrkIsoVVL_TkMu8_TrkIsoVVL = 1UL<<48,
    kHLT_Mu30_TkMu11 = 1UL<<49,
    kHLT_IsoMu24 = 1UL<<50,
    kHLT_IsoTkMu24 = 1UL<<51,
    kHLT_Mu50 = 1UL<<52,
    kHLT_IsoMu24_eta2p1 = 1UL<<53,
    kHLT_IsoMu22_eta2p1 = 1UL<<54,
    kHLT_IsoTkMu24_eta2p1 = 1UL<<55,
    kHLT_IsoTkMu22_eta2p1 = 1UL<<56,
    kHLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_DZ = 1UL<<57,
    kHLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL = 1UL<<58,
    kHLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ = 1UL<<59,
    kHLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL = 1UL<<60

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
    kSingleIsoMu20 = 1UL<<28,
    kSingleIsoTkMu20 = 1UL<<29,
    kSingleEle25WPTightNoER = 1UL<<30,
    kSingleEle25WPLoose = 1UL<<31,
    kSingleEle25WPTight = 1UL<<32,
    kEle15IsoVVVL = 1UL<<33,
    kMu15IsoVVVL = 1UL<<34,
    kCaloJet500 = 1UL<<35
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
    fFlag_hcalLaserEventFilter = 1UL<<12,
    fFlag_CSCTightHalo2015Filter = 1UL<<13,
    fFlag_HBHENoiseIsoFilter     = 1UL<<14,
    fFlag_globalTightHalo2016Filter = 1UL<<15
  };


  enum METFilterBits_VectorVersion {
     fFlag_idx_HBHENoiseFilter                    = 0,
     fFlag_idx_HBHENoiseIsoFilter                 = 1,
     fFlag_idx_CSCTightHaloFilter                 = 2,
     fFlag_idx_CSCTightHalo2015Filter             = 3,
     fFlag_idx_globalTightHalo2016Filter          = 4,
     fFlag_idx_hcalLaserEventFilter               = 5,
     fFlag_idx_EcalDeadCellTriggerPrimitiveFilter = 6,
     fFlag_idx_goodVertices                      = 7,
     fFlag_idx_eeBadScFilter                     = 8,
     fFlag_idx_ecalLaserCorrFilter               = 9,
     fFlag_idx_trkPOGFilters                     = 10,
     fFlag_idx_trkPOG_manystripclus53X           = 11,
     fFlag_idx_trkPOG_toomanystripclus53X        = 12,
     fFlag_idx_trkPOG_logErrorTooManyClusters    = 13,
     fFlag_idx_METFilters                        = 14


  };

}

#endif
