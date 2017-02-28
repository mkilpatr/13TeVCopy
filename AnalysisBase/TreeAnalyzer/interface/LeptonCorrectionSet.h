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
#include "TH3.h"

namespace ucsbsusy {

  class LepCorr : public Correction {

    public:
      LepCorr();
      virtual ~LepCorr();
      void getEvtWeight(const BaseTreeAnalyzer * ana, CORRTYPE tauCorrType, float &outVetoLepWeightLM, float &outSelLepWeightLM, float &outVetoLepWeightHM, float &outSelLepWeightHM, bool useHPS=false);
      void countRecoLepsAndGenTaus(const BaseTreeAnalyzer * ana, int &nPromptGenTaus, int &nSelectedElectrons, int &nSelectedMuons);

      TFile * fileLepLM;
      TFile * fileLepHM;

      TH1F * histLepLM;
      TH1F * histLepHM;
  };

  class TnPCorr : public Correction {
    public:
      TnPCorr(const LeptonSelection::Electron elSel, const LeptonSelection::Electron secElSel,
              const LeptonSelection::Muon     muSel, const LeptonSelection::Muon     secMuSel);
      virtual ~TnPCorr();
      float getLepWeight(LeptonF* lep, CORRTYPE elCorrType, CORRTYPE muCorrType, bool isGenMuon, bool isLM, bool isFastsim, bool isSR, int nPV );
      float getGenLepWeight(const GenParticleF* lep, CORRTYPE elCorrType, CORRTYPE muCorrType, bool isLM, bool isFastsim, bool isSR, int nPV );
      float getEvtWeight(const std::vector<LeptonF*>& allLeptons, const std::vector<LeptonF*>& selectedLeptons, const std::vector<GenParticleF*> genParts,
                         CORRTYPE elCorrType, CORRTYPE muCorrType, bool isLM, bool isFastsim, bool isSR, int nPV);

      bool isCR; // do configs indicate 1-lepton control region? (NOT inverted veto ... looking for ZL_CTR_X configs)

      // histo getter fxns ('getbin's avoid under/overflow bins)
      template<typename H> int getbinx(float xvalue,  H * hist) {return std::min(std::max(hist->GetXaxis()->FindFixBin(xvalue),1),hist->GetNbinsX());  }
      template<typename H> int getbiny(float yvalue,  H * hist) {return std::min(std::max(hist->GetYaxis()->FindFixBin(yvalue),1),hist->GetNbinsY());  }
      template<typename H> int getbinz(float zvalue,  H * hist) {return std::min(std::max(hist->GetZaxis()->FindFixBin(zvalue),1),hist->GetNbinsZ());  }
      float getgraphcontent1d(float value,  TGraphAsymmErrors * graph) {return graph->Eval(value); }
      float getbincontent1d(float value,  TH1F * hist) {return hist->GetBinContent(getbinx(value,hist)); }
      float getbinerror1d(  float value,  TH1F * hist) {return hist->GetBinError(getbinx(value,hist)); }
      float getbincontent2d(float xvalue, float yvalue,  TH2F * hist) {return hist->GetBinContent(getbinx(xvalue,hist),getbiny(yvalue,hist)); }
      float getbinerror2d(  float xvalue, float yvalue,  TH2F * hist) {return hist->GetBinError(getbinx(xvalue,hist),getbiny(yvalue,hist)); }
      template<typename H> float getbincontent3d(float xvalue, float yvalue, float zvalue, H * hist) {return hist->GetBinContent(getbinx(xvalue,hist),getbiny(yvalue,hist),getbinz(zvalue,hist)); }

    protected:
      // electron / muon configs
      LeptonSelection::Electron elConf;
      LeptonSelection::Muon     muConf;
      LeptonSelection::Electron elConfNoIso;
      LeptonSelection::Muon     muConfNoIso;
      LeptonSelection::Electron elConfKin;
      LeptonSelection::Muon     muConfKin;

      // electron files
      TFile * fileElSf;
      TFile * fileElSfTracker;
      TFile * fileElSfFullFastId;
      TFile * fileElSfFullFastIso;
      TFile * fileElMCEffsLM;
      TFile * fileElMCEffsHM;
      //TFile * fileElMCEffsLMId;
      //TFile * fileElMCEffsLMIso;
      //TFile * fileElMCEffsHMId;
      //TFile * fileElMCEffsHMIso;

      // electron histos
      TH2F * histElSfId;
      TH2F * histElSfIso;
      TH2F * histElSfTracker;
      TH2F * histElSfFullFastId;
      TH2F * histElSfFullFastIso;
      TH2F * histElMCEffsLMId;
      TH2F * histElMCEffsLMIso;
      TH2F * histElMCEffsHMId;
      TH2F * histElMCEffsHMIso;

      // muon files
      TFile * fileMuSfId;
      TFile * fileMuSfIso;
      TFile * fileMuSfIp2d;
      TFile * fileMuSfTracker;
      TFile * fileMuSfFullFastId;
      TFile * fileMuSfFullFastIso;
      TFile * fileMuSfFullFastIp2d;
      TFile * fileMuMCEffsLM;
      TFile * fileMuMCEffsHM;
      //TFile * fileMuMCEffsLMId;
      //TFile * fileMuMCEffsLMIso;
      //TFile * fileMuMCEffsHMId;
      //TFile * fileMuMCEffsHMIso;

      // muon histos;
      TH2F * histMuSfId;
      TH2F * histMuSfIso;
      TH2F * histMuSfIp2d;
      TGraphAsymmErrors * histMuSfTrackerGt10;
      TGraphAsymmErrors * histMuSfTrackerLt10;
      TH2F * histMuSfFullFastId;
      TH2F * histMuSfFullFastIso;
      TH2F * histMuSfFullFastIp2d;
      TH2F * histMuMCEffsLMId;
      TH2F * histMuMCEffsLMIso;
      TH2F * histMuMCEffsHMId;
      TH2F * histMuMCEffsHMIso;
  };

  class LeptonCorrectionSet : public CorrectionSet {

    public :
      enum CorrectionOptions {
        NULLOPT       = 0,        // no correction
        LEP           = (1 << 0), // apply lepton corrections
        TNP           = (1 << 1), // apply tnp lepton corrections
        LEP_VARY_UP   = (1 << 2), // systematic variation up, all flavours
        LEP_VARY_DOWN = (1 << 3), // systematic variation down, all flavours
        //ELE_VARY_UP   = (1 << 4), // systematic variation up, specific flavour
        //ELE_VARY_DOWN = (1 << 5), // systematic variation down, specific flavour
        //MU_VARY_UP    = (1 << 6), //
        //MU_VARY_DOWN  = (1 << 7), //
        TAU_VARY_UP   = (1 << 8), //
        TAU_VARY_DOWN = (1 << 9), //
        USE_HPSTAUS   = (1 << 10), //
      }; 

      LeptonCorrectionSet() : lepCorr(0), tnpCorr(0), vetoLepWeightLM(1), vetoLepWeightHM(1), selLepWeightLM(1), selLepWeightHM(1), useHPS(false), multiPtBins(true), tnpEvtWeightLM(1), tnpEvtWeightHM(1) {}
      virtual ~LeptonCorrectionSet() {}
      virtual void load(const LeptonSelection::Electron elSel, const LeptonSelection::Electron secElSel,
                        const LeptonSelection::Muon     muSel, const LeptonSelection::Muon     secMuSel,
                        int correctionOptions = NULLOPT);
      virtual void processCorrection(const BaseTreeAnalyzer * ana);

      // lep (tau) correction accessors
      float getVetoLepWeightLM()     const { return vetoLepWeightLM; }
      float getVetoLepWeightHM()     const { return vetoLepWeightHM; }

      float getSelLepWeightLM()      const { return selLepWeightLM;  }
      float getSelLepWeightHM()      const { return selLepWeightHM;  }

      // systematics accessors // logical OR with ucsbsusy::LeptonCorrectionSet::LEP/TAU_VARY_UP/DOWN. Default = LEP | TNP
      float getLepWeightAny(const BaseTreeAnalyzer * ana, CORRTYPE tauCorrType, bool isLM, bool isVetoWeight) const;

      void  setUseHPSTaus(bool setHPS)   { useHPS = setHPS;      }
      bool  getUseHPSTaus()              { return useHPS ? true : false;        }
      //void  setMultiPtBins(bool setBins) { multiPtBins = setBins; }

      // tnp (el/mu) correction accessors
      float getTnPLepWeightLM()      const { return tnpEvtWeightLM;  }
      float getTnPLepWeightHM()      const { return tnpEvtWeightHM;  }

      // systematics accessors
      float getTnPWeightAny(const BaseTreeAnalyzer * ana, CORRTYPE elCorrType, CORRTYPE muCorrType, bool isLM) const;

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
