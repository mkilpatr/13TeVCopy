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
      enum LEPSEL {MT2VETO, GOODPOG};
      TnPCorr(TString corrName, LEPSEL lepSel);
      virtual ~TnPCorr();
      void getLepWeight(float &wt, float &vetoWt, LeptonF* lep, CORRTYPE elCorrType, CORRTYPE muCorrType ) const;
      float getEvtWeight(const std::vector<LeptonF*>& allLeptons, const std::vector<LeptonF*>& selectedLeptons, const std::vector<GenParticleF*> genParts, CORRTYPE elCorrType, CORRTYPE muCorrType) const;
      virtual float pickBin(int bin, int nBins) const { if(bin<1)     return 1;
                                                        if(bin>nBins) return nBins;
                                                        return bin; }
      virtual float getElIDValue(float pt, float eta) const { return HistIDEl->GetBinContent(pickBin(HistIDEl->GetXaxis()->FindFixBin(pt) , HistIDEl->GetNbinsX())
                                                                                            ,pickBin(HistIDEl->GetYaxis()->FindFixBin(eta), HistIDEl->GetNbinsY()) ); }
      virtual float getMuIDValue(float pt, float eta) const { return HistIDMu->GetBinContent(pickBin(HistIDMu->GetXaxis()->FindFixBin(pt) , HistIDMu->GetNbinsX())
                                                                                            ,pickBin(HistIDMu->GetYaxis()->FindFixBin(eta), HistIDMu->GetNbinsY()) ); }
      virtual float getElIDError(float pt, float eta) const { return HistIDEl->GetBinError  (pickBin(HistIDEl->GetXaxis()->FindFixBin(pt) , HistIDEl->GetNbinsX())
                                                                                            ,pickBin(HistIDEl->GetYaxis()->FindFixBin(eta), HistIDEl->GetNbinsY()) ); }
      virtual float getMuIDError(float pt, float eta) const { return HistIDMu->GetBinError  (pickBin(HistIDMu->GetXaxis()->FindFixBin(pt) , HistIDMu->GetNbinsX())
                                                                                            ,pickBin(HistIDMu->GetYaxis()->FindFixBin(eta), HistIDMu->GetNbinsY()) ); }
      virtual float getElIsoValue(float pt, float eta) const { return HistIsoEl->GetBinContent(pickBin(HistIsoEl->GetXaxis()->FindFixBin(pt) , HistIsoEl->GetNbinsX())
                                                                                              ,pickBin(HistIsoEl->GetYaxis()->FindFixBin(eta), HistIsoEl->GetNbinsY()) ); }
      virtual float getMuIsoValue(float pt, float eta) const { return HistIsoMu->GetBinContent(pickBin(HistIsoMu->GetXaxis()->FindFixBin(pt) , HistIsoMu->GetNbinsX())
                                                                                              ,pickBin(HistIsoMu->GetYaxis()->FindFixBin(eta), HistIsoMu->GetNbinsY()) ); }
      virtual float getElIsoError(float pt, float eta) const { return HistIsoEl->GetBinError  (pickBin(HistIsoEl->GetXaxis()->FindFixBin(pt) , HistIsoEl->GetNbinsX())
                                                                                              ,pickBin(HistIsoEl->GetYaxis()->FindFixBin(eta), HistIsoEl->GetNbinsY()) ); }
      virtual float getMuIsoError(float pt, float eta) const { return HistIsoMu->GetBinError  (pickBin(HistIsoMu->GetXaxis()->FindFixBin(pt) , HistIsoMu->GetNbinsX())
                                                                                              ,pickBin(HistIsoMu->GetYaxis()->FindFixBin(eta), HistIsoMu->GetNbinsY()) ); }
      virtual float getElMCEffValue(float pt, float eta) const { return HistMCVetoEffEl->GetBinContent(pickBin(HistMCVetoEffEl->GetXaxis()->FindFixBin(pt)  , HistMCVetoEffEl->GetNbinsX())
                                                                                                      ,pickBin(HistMCVetoEffEl->GetYaxis()->FindFixBin(eta), HistMCVetoEffEl->GetNbinsY()) ); }
      virtual float getMuMCEffValue(float pt, float eta) const { return HistMCVetoEffMu->GetBinContent(pickBin(HistMCVetoEffMu->GetXaxis()->FindFixBin(pt) , HistMCVetoEffMu->GetNbinsX())
                                                                                                      ,pickBin(HistMCVetoEffMu->GetYaxis()->FindFixBin(eta), HistMCVetoEffMu->GetNbinsY()) ); }
      virtual float getElMCEffError(float pt, float eta) const { return HistMCVetoEffEl->GetBinError  (pickBin(HistMCVetoEffEl->GetXaxis()->FindFixBin(pt) , HistMCVetoEffEl->GetNbinsX())
                                                                                                      ,pickBin(HistMCVetoEffEl->GetYaxis()->FindFixBin(eta), HistMCVetoEffEl->GetNbinsY()) ); }
      virtual float getMuMCEffError(float pt, float eta) const { return HistMCVetoEffMu->GetBinError  (pickBin(HistMCVetoEffMu->GetXaxis()->FindFixBin(pt) , HistMCVetoEffMu->GetNbinsX())
                                                                                                      ,pickBin(HistMCVetoEffMu->GetYaxis()->FindFixBin(eta), HistMCVetoEffMu->GetNbinsY()) ); }
      virtual float getError(float a, float b) const { return sqrt(a*a+b*b); }
    protected:
      TFile* fileIdIsoEl;
      TFile* fileIdMu;
      TFile* fileIsoMu;
      TFile* fileMCVetoEffEl;
      TFile* fileMCVetoEffMu;
      TH2F*  HistIDEl;
      TH2F*  HistIDMu;
      TH2F*  HistIsoEl;
      TH2F*  HistIsoMu;
      TH2F*  HistMCVetoEffEl;
      TH2F*  HistMCVetoEffMu;
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
      virtual void load(TString fileName, TnPCorr::LEPSEL tnpLepSel, int correctionOptions = NULLOPT);
      virtual void processCorrection(const BaseTreeAnalyzer * ana);

      float getVetoLepWeight()     const { return vetoLepWeight; }
      float getSelLepWeight()      const { return selLepWeight;  }
      void  setUseHPSTaus(bool setHPS)   { useHPS = setHPS;      }
      float getTnPLepWeight()      const { return tnpEvtWeight;  }

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
