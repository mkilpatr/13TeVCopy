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
#include "AnalysisBase/TreeAnalyzer/interface/LeptonCorrectionSet.h"


namespace ucsbsusy {

  class LepCorr : public HistogramCorrection {

    public:
      LepCorr(TString fileNameLM, TString fileNameHM)  : HistogramCorrection("LEP",fileNameLM,fileNameHM) {}

      static const unsigned int defaultBin       = 1;
      static const unsigned int muCorrBinLowPt   = 2;
      static const unsigned int muCorrBinHighPt  = 3;
      static const unsigned int eleCorrBinLowPt  = 4;
      static const unsigned int eleCorrBinHighPt = 5;
      static const unsigned int tauCorrBinLowPt  = 6;
      static const unsigned int tauCorrBinHighPt = 7;
      static const unsigned int hpsTauCorrBin    = 8;
      static const unsigned int fakeBin          = 9;

      static const std::vector<double> eleCorrPtBins;
      static const std::vector<double> muCorrPtBins;
      static const std::vector<double> tauCorrPtBins;

  };

  class TnPCorr : public Correction {
    public:
      enum LEPSEL {MT2VETO, GOODPOG};
      TnPCorr(TString corrName,
              const LeptonSelection::Electron elSel, const LeptonSelection::Electron secElSel,
              const LeptonSelection::Muon     muSel, const LeptonSelection::Muon     secMuSel);
      virtual ~TnPCorr();
      float getLepWeight(LeptonF* lep, CORRTYPE elCorrType, CORRTYPE muCorrType, TString region ) const;
      float getGenLepWeight(const GenParticleF* lep, CORRTYPE muCorrType, TString region ) const;
      float getEvtWeight(const std::vector<LeptonF*>& allLeptons, const std::vector<LeptonF*>& selectedLeptons, const std::vector<GenParticleF*> genParts,
                         CORRTYPE elCorrType, CORRTYPE muCorrType, TString region) const;
      //float getLepWeight(LeptonF* lep, CORRTYPE elIdCorrType, CORRTYPE elIsoCorrType, CORRTYPE muIdCorrType, CORRTYPE muIsoCorrType ) const;
      //float getGenLepWeight(const GenParticleF* lep, CORRTYPE muIdCorrType ) const;
      //float getEvtWeight(const std::vector<LeptonF*>& allLeptons, const std::vector<LeptonF*>& selectedLeptons, const std::vector<GenParticleF*> genParts,
      //                   CORRTYPE elIdCorrType, CORRTYPE elIsoCorrType, CORRTYPE muIdCorrType, CORRTYPE muIsoCorrType) const;

      // should change this so TnPCorr : Histogram2DCorrection, then use setXAxisNoUnderOver etc functions as getters
      virtual float getError(float a, float b, float c=0) const { return sqrt(a*a+b*b+c*c); }
      virtual float pickBin(int bin, int nBins) const { if(bin<1)     return 1;
                                                        if(bin>nBins) return nBins;
                                                        return bin; }
      virtual float getGenericValue2D(float x, float y, TH2F* hist) const {
        return hist->GetBinContent(pickBin(hist->GetXaxis()->FindFixBin(x), hist->GetNbinsX()),
                                   pickBin(hist->GetYaxis()->FindFixBin(y), hist->GetNbinsY()));
      }
      virtual float getGenericError2D(float x, float y, TH2F* hist) const {
        return hist->GetBinError  (pickBin(hist->GetXaxis()->FindFixBin(x), hist->GetNbinsX()),
                                   pickBin(hist->GetYaxis()->FindFixBin(y), hist->GetNbinsY()));
      }
      virtual float getGenericValue1D(float x, TH1F* hist) const {
        return hist->GetBinContent(pickBin(hist->GetXaxis()->FindFixBin(x), hist->GetNbinsX()));
      }
      virtual float getGenericError1D(float x, TH1F* hist) const {
        return hist->GetBinError  (pickBin(hist->GetXaxis()->FindFixBin(x), hist->GetNbinsX()));
      }

      // SF getters
      virtual float getElIDValue (float x, float y) const { return getGenericValue2D(x,y,HistIdEl); }
      virtual float getElIDError (float x, float y) const { return getGenericError2D(x,y,HistIdEl); }
      virtual float getElIsoValue(float x, float y) const { return getGenericValue2D(x,y,HistIsoEl); }
      virtual float getElIsoError(float x, float y) const { return getGenericError2D(x,y,HistIsoEl); }

      virtual float getMuIDValue (float x, float y) const { return getGenericValue2D(x,y,HistIdMu); }
      virtual float getMuIDError (float x, float y) const { return getGenericError2D(x,y,HistIdMu); }
      virtual float getMuIsoValue(float x, float y) const { return getGenericValue2D(x,y,HistIsoMu); }
      virtual float getMuIsoError(float x, float y) const { return getGenericError2D(x,y,HistIsoMu); }

      virtual float getMuTrackerPtg10Value(float x) const { return getGenericValue1D(x,HistTrackerPtg10Mu); }
      virtual float getMuTrackerPtg10Error(float x) const { return getGenericError1D(x,HistTrackerPtg10Mu); }
      virtual float getMuTrackerPtl10Value(float x) const { return getGenericValue1D(x,HistTrackerPtl10Mu); }
      virtual float getMuTrackerPtl10Error(float x) const { return getGenericError1D(x,HistTrackerPtl10Mu); }

      virtual float getElTrackerValue(float x, float y) const { return getGenericValue2D(x,y,HistTrackerEl); }
      virtual float getElTrackerError(float x, float y) const { return getGenericError2D(x,y,HistTrackerEl); }

      virtual float getMuIP2DValue(float x, float y) const { return getGenericValue2D(x,y,HistIP2DMu); }
      virtual float getMuIP2DError(float x, float y) const { return getGenericError2D(x,y,HistIP2DMu); }

      // MC eff getters for LM and HM regions
      virtual float getElMCIdEffValue  (float x, float y, TString region) const { return region == "LM" ? getGenericValue2D(x,y,HistMCVetoLMIdEffEl) : getGenericValue2D(x,y,HistMCVetoHMIdEffEl);}
      virtual float getElMCIdEffError  (float x, float y, TString region) const { return region == "LM" ? getGenericError2D(x,y,HistMCVetoLMIdEffEl) : getGenericError2D(x,y,HistMCVetoHMIdEffEl);}
      virtual float getElMCIsoEffValue (float x, float y, TString region) const { return region == "LM" ? getGenericValue2D(x,y,HistMCVetoLMIsoEffEl) : getGenericValue2D(x,y,HistMCVetoHMIsoEffEl);}
      virtual float getElMCIsoEffError (float x, float y, TString region) const { return region == "LM" ? getGenericError2D(x,y,HistMCVetoLMIsoEffEl) : getGenericError2D(x,y,HistMCVetoHMIsoEffEl);}

      virtual float getMuMCIdEffValue  (float x, float y, TString region) const { return region == "LM" ? getGenericValue2D(x,y,HistMCVetoLMIdEffMu) : getGenericValue2D(x,y,HistMCVetoHMIdEffMu);}
      virtual float getMuMCIdEffError  (float x, float y, TString region) const { return region == "LM" ? getGenericError2D(x,y,HistMCVetoLMIdEffMu) : getGenericError2D(x,y,HistMCVetoHMIdEffMu);}
      virtual float getMuMCIsoEffValue (float x, float y, TString region) const { return region == "LM" ? getGenericValue2D(x,y,HistMCVetoLMIsoEffMu) : getGenericValue2D(x,y,HistMCVetoHMIsoEffMu);}
      virtual float getMuMCIsoEffError (float x, float y, TString region) const { return region == "LM" ? getGenericError2D(x,y,HistMCVetoLMIsoEffMu) : getGenericError2D(x,y,HistMCVetoHMIsoEffMu);}

    protected:
      LeptonSelection::Electron elConf;
      LeptonSelection::Muon     muConf;
      LeptonSelection::Electron elConfNoIso;
      LeptonSelection::Muon     muConfNoIso;
      LeptonSelection::Electron elConfKin;
      LeptonSelection::Muon     muConfKin;
      TFile* fileEl;
      TFile* fileIdMu;
      TFile* fileIsoMu;
      TFile* fileMCVetoLMIdEffEl;
      TFile* fileMCVetoLMIdEffMu;
      TFile* fileMCVetoLMIsoEffEl;
      TFile* fileMCVetoLMIsoEffMu;
      TFile* fileMCVetoHMIdEffEl;
      TFile* fileMCVetoHMIdEffMu;
      TFile* fileMCVetoHMIsoEffEl;
      TFile* fileMCVetoHMIsoEffMu;
      TFile* fileTrackerMu;
      TFile* fileTrackerEl;
      TFile* fileIP2DMu;
      TH2F*  HistIdEl;
      TH2F*  HistIdMu;
      TH2F*  HistIsoEl;
      TH2F*  HistIsoMu;
      TH2F*  HistMCVetoHMIdEffEl;
      TH2F*  HistMCVetoHMIdEffMu;
      TH2F*  HistMCVetoHMIsoEffEl;
      TH2F*  HistMCVetoHMIsoEffMu;
      TH2F*  HistMCVetoLMIdEffEl;
      TH2F*  HistMCVetoLMIdEffMu;
      TH2F*  HistMCVetoLMIsoEffEl;
      TH2F*  HistMCVetoLMIsoEffMu;
      TH1F*  HistTrackerPtg10Mu;
      TH1F*  HistTrackerPtl10Mu;
      TH2F*  HistTrackerEl;
      TH2F*  HistIP2DMu;
  };

  class LeptonCorrectionSet : public CorrectionSet {

    public :
      enum CorrectionOptions {
        NULLOPT       = 0,        // no correction
        LEP           = (1 << 0), // apply lepton corrections
        TNP           = (1 << 1), // apply tnp lepton corrections
        LEP_VARY_UP   = (1 << 2), // systematic variation up, all flavours
        LEP_VARY_DOWN = (1 << 3), // systematic variation down, all flavours
        ELE_VARY_UP   = (1 << 4), // systematic variation up, specific flavour
        ELE_VARY_DOWN = (1 << 5), // systematic variation down, specific flavour
        MU_VARY_UP    = (1 << 6), //
        MU_VARY_DOWN  = (1 << 7), //
        TAU_VARY_UP   = (1 << 8), //
        TAU_VARY_DOWN = (1 << 9), //
        USE_HPSTAUS   = (1 << 10), //
        MULTI_PT_BINS = (1 << 11)
      }; 

      LeptonCorrectionSet() : lepCorr(0), tnpCorr(0), vetoLepWeightLM(1), vetoLepWeightHM(1), selLepWeightLM(1), selLepWeightHM(1), useHPS(false), multiPtBins(true), tnpEvtWeightLM(1), tnpEvtWeightHM(1) {}
      virtual ~LeptonCorrectionSet() {}
      virtual void load(TString fileNameLM, TString fileNameHM, //for LepCorrs
                        const LeptonSelection::Electron elSel, const LeptonSelection::Electron secElSel,
                        const LeptonSelection::Muon     muSel, const LeptonSelection::Muon     secMuSel,
                        int correctionOptions = NULLOPT);
      virtual void processCorrection(const BaseTreeAnalyzer * ana);

      float getVetoLepWeightLM()     const { return vetoLepWeightLM; }
      float getSelLepWeightLM()      const { return selLepWeightLM;  }
      float getVetoLepWeightHM()     const { return vetoLepWeightHM; }
      float getSelLepWeightHM()      const { return selLepWeightHM;  }
      void  setUseHPSTaus(bool setHPS)   { useHPS = setHPS;      }
      void  setMultiPtBins(bool setBins) { multiPtBins = setBins; }
      float getTnPLepWeightLM()      const { return tnpEvtWeightLM;  }
      float getTnPLepWeightHM()      const { return tnpEvtWeightHM;  }

    private :
      LepCorr* lepCorr;
      TnPCorr* tnpCorr;

      float vetoLepWeightLM;
      float vetoLepWeightHM;
      float selLepWeightLM;
      float selLepWeightHM;
      bool  useHPS;
      bool  multiPtBins;
      float tnpEvtWeightLM;
      float tnpEvtWeightHM;
  };

} /* namespace ucsbsusy */

#endif /* ANALYSISBASE_TREEANALYZER_INTERFACE_LEPTONCORRECTIONSET_H_ */
