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

      // helper functions, to be replaced by the correction FW's helper functions (functionally identical)
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

      virtual float getMuIDValue (float x, float y) const { return 1.; } // waiting on MuID SFs
      virtual float getMuIDError (float x, float y) const { return 0.; } // --
      //virtual float getMuIDValue (float x, float y) const { return getGenericValue2D(x,y,HistIdMu); }
      //virtual float getMuIDError (float x, float y) const { return getGenericError2D(x,y,HistIdMu); }
      virtual float getMuIsoValue(float x, float y) const { return getGenericValue2D(x,y,HistIsoMu); }
      virtual float getMuIsoError(float x, float y) const { return getGenericError2D(x,y,HistIsoMu); }

      virtual float getMuTrackerPtg10Value(float x) const { return getGenericValue1D(x,HistMuTrackerPtg10); }
      virtual float getMuTrackerPtg10Error(float x) const { return getGenericError1D(x,HistMuTrackerPtg10); }
      virtual float getMuTrackerPtl10Value(float x) const { return getGenericValue1D(x,HistMuTrackerPtl10); }
      virtual float getMuTrackerPtl10Error(float x) const { return getGenericError1D(x,HistMuTrackerPtl10); }

      // MC eff getters for LM and HM regions
      virtual float getElMCIdEffValue  (float x, float y, TString region) const { return region == "LM" ? getGenericValue2D(x,y,HistMCVetoLMIdEffEl) : getGenericValue2D(x,y,HistMCVetoHMIdEffEl);}
      virtual float getElMCIdEffError  (float x, float y, TString region) const { return region == "LM" ? getGenericError2D(x,y,HistMCVetoLMIdEffEl) : getGenericError2D(x,y,HistMCVetoHMIdEffEl);}
      virtual float getElMCIsoEffValue (float x, float y, TString region) const { return region == "LM" ? getGenericValue2D(x,y,HistMCVetoLMIsoEffEl) : getGenericValue2D(x,y,HistMCVetoHMIsoEffEl);}
      virtual float getElMCIsoEffError (float x, float y, TString region) const { return region == "LM" ? getGenericError2D(x,y,HistMCVetoLMIsoEffEl) : getGenericError2D(x,y,HistMCVetoHMIsoEffEl);}

      virtual float getMuMCIdEffValue  (float x, float y, TString region) const { return region == "LM" ? getGenericValue2D(x,y,HistMCVetoLMIdEffMu) : getGenericValue2D(x,y,HistMCVetoHMIdEffMu);}
      virtual float getMuMCIdEffError  (float x, float y, TString region) const { return region == "LM" ? getGenericError2D(x,y,HistMCVetoLMIdEffMu) : getGenericError2D(x,y,HistMCVetoHMIdEffMu);}
      virtual float getMuMCIsoEffValue (float x, float y, TString region) const { return region == "LM" ? getGenericValue2D(x,y,HistMCVetoLMIsoEffMu) : getGenericValue2D(x,y,HistMCVetoHMIsoEffMu);}
      virtual float getMuMCIsoEffError (float x, float y, TString region) const { return region == "LM" ? getGenericError2D(x,y,HistMCVetoLMIsoEffMu) : getGenericError2D(x,y,HistMCVetoHMIsoEffMu);}

/* // old method - keep in case
      virtual float getElIDValue(float x, float y) const { return HistIdEl->GetBinContent(pickBin(HistIdEl->GetXaxis()->FindFixBin(x), HistIdEl->GetNbinsX())
                                                                                         ,pickBin(HistIdEl->GetYaxis()->FindFixBin(y), HistIdEl->GetNbinsY()) ); }
      virtual float getMuIDValue(float x, float y) const { return HistIdMu->GetBinContent(pickBin(HistIdMu->GetXaxis()->FindFixBin(x), HistIdMu->GetNbinsX())
                                                                                         ,pickBin(HistIdMu->GetYaxis()->FindFixBin(y), HistIdMu->GetNbinsY()) ); }
      virtual float getElIDError(float x, float y) const { return HistIdEl->GetBinError  (pickBin(HistIdEl->GetXaxis()->FindFixBin(x), HistIdEl->GetNbinsX())
                                                                                         ,pickBin(HistIdEl->GetYaxis()->FindFixBin(y), HistIdEl->GetNbinsY()) ); }
      virtual float getMuIDError(float x, float y) const { return HistIdMu->GetBinError  (pickBin(HistIdMu->GetXaxis()->FindFixBin(x), HistIdMu->GetNbinsX())
                                                                                         ,pickBin(HistIdMu->GetYaxis()->FindFixBin(y), HistIdMu->GetNbinsY()) ); }
      virtual float getElIsoValue(float x, float y) const { return HistIsoEl->GetBinContent(pickBin(HistIsoEl->GetXaxis()->FindFixBin(x), HistIsoEl->GetNbinsX())
                                                                                           ,pickBin(HistIsoEl->GetYaxis()->FindFixBin(y), HistIsoEl->GetNbinsY()) ); }
      virtual float getMuIsoValue(float x, float y) const { return HistIsoMu->GetBinContent(pickBin(HistIsoMu->GetXaxis()->FindFixBin(x), HistIsoMu->GetNbinsX())
                                                                                           ,pickBin(HistIsoMu->GetYaxis()->FindFixBin(y), HistIsoMu->GetNbinsY()) ); }
      virtual float getElIsoError(float x, float y) const { return HistIsoEl->GetBinError  (pickBin(HistIsoEl->GetXaxis()->FindFixBin(x), HistIsoEl->GetNbinsX())
                                                                                           ,pickBin(HistIsoEl->GetYaxis()->FindFixBin(y), HistIsoEl->GetNbinsY()) ); }
      virtual float getMuIsoError(float x, float y) const { return HistIsoMu->GetBinError  (pickBin(HistIsoMu->GetXaxis()->FindFixBin(x), HistIsoMu->GetNbinsX())
                                                                                           ,pickBin(HistIsoMu->GetYaxis()->FindFixBin(y), HistIsoMu->GetNbinsY()) ); }
      virtual float getMuTrackerPtg10Value(float x) const { return HistMuTrackerPtg10->GetBinContent(pickBin(HistMuTrackerPtg10->GetXaxis()->FindFixBin(x), HistMuTrackerPtg10->GetNbinsX()));}
      virtual float getMuTrackerPtg10Error(float x) const { return HistMuTrackerPtg10->GetBinError  (pickBin(HistMuTrackerPtg10->GetXaxis()->FindFixBin(x), HistMuTrackerPtg10->GetNbinsX()));}
      virtual float getMuTrackerPtl10Value(float x) const { return HistMuTrackerPtl10->GetBinContent(pickBin(HistMuTrackerPtl10->GetXaxis()->FindFixBin(x), HistMuTrackerPtl10->GetNbinsX()));}
      virtual float getMuTrackerPtl10Error(float x) const { return HistMuTrackerPtl10->GetBinError  (pickBin(HistMuTrackerPtl10->GetXaxis()->FindFixBin(x), HistMuTrackerPtl10->GetNbinsX()));}
      virtual float getElMCIdEffValue(float x, float y, TString region) const { return HistMCVetoIdEffEl->GetBinContent(pickBin(HistMCVetoIdEffEl->GetXaxis()->FindFixBin(x), HistMCVetoIdEffEl->GetNbinsX())
                                                                                                       ,pickBin(HistMCVetoIdEffEl->GetYaxis()->FindFixBin(y), HistMCVetoIdEffEl->GetNbinsY()) ); }
      // waiting on muon ID SFs
      virtual float getMuMCIdEffValue(float x, float y, TString region) const { return 1.;}
      //virtual float getMuMCIdEffValue(float x, float y) const { return HistMCVetoIdEffMu->GetBinContent(pickBin(HistMCVetoIdEffMu->GetXaxis()->FindFixBin(x), HistMCVetoIdEffMu->GetNbinsX())
      //                                                                                                 ,pickBin(HistMCVetoIdEffMu->GetYaxis()->FindFixBin(y), HistMCVetoIdEffMu->GetNbinsY()) ); }
      virtual float getElMCIdEffError(float x, float y, TString region) const { return HistMCVetoIdEffEl->GetBinError  (pickBin(HistMCVetoIdEffEl->GetXaxis()->FindFixBin(x), HistMCVetoIdEffEl->GetNbinsX())
                                                                                                       ,pickBin(HistMCVetoIdEffEl->GetYaxis()->FindFixBin(y), HistMCVetoIdEffEl->GetNbinsY()) ); }
      // waiting on muon ID SFs
      virtual float getMuMCIdEffError(float x, float y, TString region) const { return 0.;}
      //virtual float getMuMCIdEffError(float x, float y) const { return HistMCVetoIdEffMu->GetBinError  (pickBin(HistMCVetoIdEffMu->GetXaxis()->FindFixBin(x), HistMCVetoIdEffMu->GetNbinsX())
      //                                                                                                 ,pickBin(HistMCVetoIdEffMu->GetYaxis()->FindFixBin(y), HistMCVetoIdEffMu->GetNbinsY()) ); }
      virtual float getElMCIsoEffValue(float x, float y, TString region) const { return HistMCVetoIsoEffEl->GetBinContent(pickBin(HistMCVetoIsoEffEl->GetXaxis()->FindFixBin(x), HistMCVetoIsoEffEl->GetNbinsX())
                                                                                                         ,pickBin(HistMCVetoIsoEffEl->GetYaxis()->FindFixBin(y), HistMCVetoIsoEffEl->GetNbinsY()) ); }
      virtual float getMuMCIsoEffValue(float x, float y, TString region) const { return HistMCVetoIsoEffMu->GetBinContent(pickBin(HistMCVetoIsoEffMu->GetXaxis()->FindFixBin(x), HistMCVetoIsoEffMu->GetNbinsX())
                                                                                                         ,pickBin(HistMCVetoIsoEffMu->GetYaxis()->FindFixBin(y), HistMCVetoIsoEffMu->GetNbinsY()) ); }
      virtual float getElMCIsoEffError(float x, float y, TString region) const { return HistMCVetoIsoEffEl->GetBinError  (pickBin(HistMCVetoIsoEffEl->GetXaxis()->FindFixBin(x), HistMCVetoIsoEffEl->GetNbinsX())
                                                                                                         ,pickBin(HistMCVetoIsoEffEl->GetYaxis()->FindFixBin(y), HistMCVetoIsoEffEl->GetNbinsY()) ); }
      virtual float getMuMCIsoEffError(float x, float y, TString region) const { return HistMCVetoIsoEffMu->GetBinError  (pickBin(HistMCVetoIsoEffMu->GetXaxis()->FindFixBin(x), HistMCVetoIsoEffMu->GetNbinsX())
                                                                                                         ,pickBin(HistMCVetoIsoEffMu->GetYaxis()->FindFixBin(y), HistMCVetoIsoEffMu->GetNbinsY()) ); }
*/

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
      TH1F*  HistMuTrackerPtg10;
      TH1F*  HistMuTrackerPtl10;
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

      LeptonCorrectionSet() : lepCorr(0), tnpCorr(0), vetoLepWeight(1), selLepWeight(1), useHPS(false), multiPtBins(false), tnpEvtWeightLM(1), tnpEvtWeightHM(1) {}
      virtual ~LeptonCorrectionSet() {}
      virtual void load(TString fileName,
                        const LeptonSelection::Electron elSel, const LeptonSelection::Electron secElSel,
                        const LeptonSelection::Muon     muSel, const LeptonSelection::Muon     secMuSel,
                        int correctionOptions = NULLOPT);
      virtual void processCorrection(const BaseTreeAnalyzer * ana);

      float getVetoLepWeight()     const { return vetoLepWeight; }
      float getSelLepWeight()      const { return selLepWeight;  }
      void  setUseHPSTaus(bool setHPS)   { useHPS = setHPS;      }
      void  setMultiPtBins(bool setBins) { multiPtBins = setBins; }
      float getTnPLepWeightLM()      const { return tnpEvtWeightLM;  }
      float getTnPLepWeightHM()      const { return tnpEvtWeightHM;  }

    private :
      LepCorr* lepCorr;
      TnPCorr* tnpCorr;

      float vetoLepWeight;
      float selLepWeight;
      bool  useHPS;
      bool  multiPtBins;
      float tnpEvtWeightLM;
      float tnpEvtWeightHM;
  };

} /* namespace ucsbsusy */

#endif /* ANALYSISBASE_TREEANALYZER_INTERFACE_LEPTONCORRECTIONSET_H_ */
