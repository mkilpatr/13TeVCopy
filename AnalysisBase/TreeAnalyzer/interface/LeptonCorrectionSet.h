/*
 * CorrectionSet.h
 *
 *  Created on: Aug 4, 2015
 *      Author: nmccoll
 */

#ifndef ANALYSISBASE_TREEANALYZER_INTERFACE_LEPTONCORRECTIONSET_H_
#define ANALYSISBASE_TREEANALYZER_INTERFACE_LEPTONCORRECTIONSET_H_

#include "AnalysisBase/TreeAnalyzer/interface/CorrectionSet.h"
#include "AnalysisTools/DataFormats/interface/Lepton.h"
#include "AnalysisTools/DataFormats/interface/GenParticle.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"


namespace ucsbsusy {

  class LepCorr : public HistogramCorrection {

    public:
      LepCorr(TFile * file)  : HistogramCorrection("LEP",file) {}

      static const unsigned int defaultBin       = 1;
      static const unsigned int muCorrBinLowPt   = 2;
      static const unsigned int muCorrBinHighPt  = 3;
      static const unsigned int eleCorrBinLowPt  = 4;
      static const unsigned int eleCorrBinHighPt = 5;
      static const unsigned int tauCorrBinLowPt  = 6;
      static const unsigned int tauCorrBinHighPt = 7;
      static const unsigned int hpsTauCorrBin    = 8;
      static const unsigned int fakeBin          = 9;

  };

  class TnPCorr : public Correction {
    public:
      TnPCorr(TString corrName, TString tnpElFileName, TString tnpMuFileName);
      virtual ~TnPCorr();
      double getLepWeight(LeptonF* lep, CORRTYPE elCorrType, CORRTYPE muCorrType ) const;
      double getEvtWeight(const std::vector<LeptonF*>& leptons, const std::vector<GenParticleF*> genParts, CORRTYPE elCorrType, CORRTYPE muCorrType ) const;
      virtual float getElValue(float pt, float eta) const { return corrHistEl->GetBinContent(corrHistEl->GetXaxis()->FindFixBin(pt)
                                                                                            ,corrHistEl->GetYaxis()->FindFixBin(eta)); }
      virtual float getMuValue(float pt, float eta) const { return corrHistMu->GetBinContent(corrHistMu->GetXaxis()->FindFixBin(pt)
                                                                                            ,corrHistMu->GetYaxis()->FindFixBin(eta)); }
      virtual float getElError(float pt, float eta) const { return corrHistEl->GetBinError  (corrHistEl->GetXaxis()->FindFixBin(pt)
                                                                                            ,corrHistEl->GetYaxis()->FindFixBin(eta)); }
      virtual float getMuError(float pt, float eta) const { return corrHistMu->GetBinError  (corrHistMu->GetXaxis()->FindFixBin(pt)
                                                                                            ,corrHistMu->GetYaxis()->FindFixBin(eta)); }
    protected:
      TFile* fileEl;
      TFile* fileMu;
      TH2F*  corrHistEl;
      TH2F*  corrHistMu;
  };

  class LeptonCorrectionSet : public CorrectionSet {

    public :
      enum CorrectionOptions {
        NULLOPT       = 0,        // no correction
        LEP           = (1 << 0), // apply lepton corrections
        TNP           = (1 << 1), // apply tnp lepton corrections
        LEP_VARY_UP   = (1 << 2), // systematic variation up
        LEP_VARY_DOWN = (1 << 3), // systematic variation down
        USE_HPSTAUS   = (1 << 4)
      }; 

      LeptonCorrectionSet() : lepCorr(0), tnpCorr(0), vetoLepWeight(1), selLepWeight(1), useHPS(false), tnpEvtWeight(1) {}
      virtual ~LeptonCorrectionSet() {}
      virtual void load(TString fileName, TString tnpElFileName, TString tnpMuFileName, int correctionOptions = NULLOPT);
      virtual void processCorrection(const BaseTreeAnalyzer * ana);

      float getVetoLepWeight()     const { return vetoLepWeight; }
      float getSelLepWeight()      const { return selLepWeight;  }
      void  setUseHPSTaus(bool setHPS)   { useHPS = setHPS;      }
      float getTnPLepWeight()      const { return tnpEvtWeight; }

    private :
      LepCorr* lepCorr;
      TnPCorr* tnpCorr;

      float vetoLepWeight;
      float selLepWeight;
      bool  useHPS;
      float tnpEvtWeight;
  };

} /* namespace ucsbsusy */

#endif /* ANALYSISBASE_TREEANALYZER_INTERFACE_LEPTONCORRECTIONSET_H_ */
