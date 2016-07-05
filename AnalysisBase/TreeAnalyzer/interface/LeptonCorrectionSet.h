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

  };

  class TnPCorr : public Correction {
    public:
      enum LEPSEL {MT2VETO, GOODPOG};
      TnPCorr(TString corrName,
              const LeptonSelection::Electron elSel, const LeptonSelection::Electron secElSel,
              const LeptonSelection::Muon     muSel, const LeptonSelection::Muon     secMuSel);
      virtual ~TnPCorr();
      float getLepWeight(LeptonF* lep, CORRTYPE elCorrType, CORRTYPE muCorrType ) const;
      float getGenLepWeight(const GenParticleF* lep, CORRTYPE muCorrType ) const;
      float getEvtWeight(const std::vector<LeptonF*>& allLeptons, const std::vector<LeptonF*>& selectedLeptons, const std::vector<GenParticleF*> genParts,
                         CORRTYPE elCorrType, CORRTYPE muCorrType) const;
      //float getLepWeight(LeptonF* lep, CORRTYPE elIdCorrType, CORRTYPE elIsoCorrType, CORRTYPE muIdCorrType, CORRTYPE muIsoCorrType ) const;
      //float getGenLepWeight(const GenParticleF* lep, CORRTYPE muIdCorrType ) const;
      //float getEvtWeight(const std::vector<LeptonF*>& allLeptons, const std::vector<LeptonF*>& selectedLeptons, const std::vector<GenParticleF*> genParts,
      //                   CORRTYPE elIdCorrType, CORRTYPE elIsoCorrType, CORRTYPE muIdCorrType, CORRTYPE muIsoCorrType) const;
      virtual float pickBin(int bin, int nBins) const { if(bin<1)     return 1;
                                                        if(bin>nBins) return nBins;
                                                        return bin; }
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
      virtual float getElMCIdEffValue(float x, float y) const { return HistMCVetoIdEffEl->GetBinContent(pickBin(HistMCVetoIdEffEl->GetXaxis()->FindFixBin(x), HistMCVetoIdEffEl->GetNbinsX())
                                                                                                       ,pickBin(HistMCVetoIdEffEl->GetYaxis()->FindFixBin(y), HistMCVetoIdEffEl->GetNbinsY()) ); }
      virtual float getMuMCIdEffValue(float x, float y) const { return HistMCVetoIdEffMu->GetBinContent(pickBin(HistMCVetoIdEffMu->GetXaxis()->FindFixBin(x), HistMCVetoIdEffMu->GetNbinsX())
                                                                                                       ,pickBin(HistMCVetoIdEffMu->GetYaxis()->FindFixBin(y), HistMCVetoIdEffMu->GetNbinsY()) ); }
      virtual float getElMCIdEffError(float x, float y) const { return HistMCVetoIdEffEl->GetBinError  (pickBin(HistMCVetoIdEffEl->GetXaxis()->FindFixBin(x), HistMCVetoIdEffEl->GetNbinsX())
                                                                                                       ,pickBin(HistMCVetoIdEffEl->GetYaxis()->FindFixBin(y), HistMCVetoIdEffEl->GetNbinsY()) ); }
      virtual float getMuMCIdEffError(float x, float y) const { return HistMCVetoIdEffMu->GetBinError  (pickBin(HistMCVetoIdEffMu->GetXaxis()->FindFixBin(x), HistMCVetoIdEffMu->GetNbinsX())
                                                                                                       ,pickBin(HistMCVetoIdEffMu->GetYaxis()->FindFixBin(y), HistMCVetoIdEffMu->GetNbinsY()) ); }
      virtual float getElMCIsoEffValue(float x, float y) const { return HistMCVetoIsoEffEl->GetBinContent(pickBin(HistMCVetoIsoEffEl->GetXaxis()->FindFixBin(x), HistMCVetoIsoEffEl->GetNbinsX())
                                                                                                         ,pickBin(HistMCVetoIsoEffEl->GetYaxis()->FindFixBin(y), HistMCVetoIsoEffEl->GetNbinsY()) ); }
      virtual float getMuMCIsoEffValue(float x, float y) const { return HistMCVetoIsoEffMu->GetBinContent(pickBin(HistMCVetoIsoEffMu->GetXaxis()->FindFixBin(x), HistMCVetoIsoEffMu->GetNbinsX())
                                                                                                         ,pickBin(HistMCVetoIsoEffMu->GetYaxis()->FindFixBin(y), HistMCVetoIsoEffMu->GetNbinsY()) ); }
      virtual float getElMCIsoEffError(float x, float y) const { return HistMCVetoIsoEffEl->GetBinError  (pickBin(HistMCVetoIsoEffEl->GetXaxis()->FindFixBin(x), HistMCVetoIsoEffEl->GetNbinsX())
                                                                                                         ,pickBin(HistMCVetoIsoEffEl->GetYaxis()->FindFixBin(y), HistMCVetoIsoEffEl->GetNbinsY()) ); }
      virtual float getMuMCIsoEffError(float x, float y) const { return HistMCVetoIsoEffMu->GetBinError  (pickBin(HistMCVetoIsoEffMu->GetXaxis()->FindFixBin(x), HistMCVetoIsoEffMu->GetNbinsX())
                                                                                                         ,pickBin(HistMCVetoIsoEffMu->GetYaxis()->FindFixBin(y), HistMCVetoIsoEffMu->GetNbinsY()) ); }
      virtual float getError(float a, float b, float c=0) const { return sqrt(a*a+b*b+c*c); }
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
      TFile* fileMCVetoIdEffEl;
      TFile* fileMCVetoIdEffMu;
      TFile* fileMCVetoIsoEffEl;
      TFile* fileMCVetoIsoEffMu;
      TH2F*  HistIdEl;
      TH2F*  HistIdMu;
      TH2F*  HistIsoEl;
      TH2F*  HistIsoMu;
      TH2F*  HistMCVetoIdEffEl;
      TH2F*  HistMCVetoIdEffMu;
      TH2F*  HistMCVetoIsoEffEl;
      TH2F*  HistMCVetoIsoEffMu;
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
        USE_HPSTAUS   = (1 << 10)
      }; 

      LeptonCorrectionSet() : lepCorr(0), tnpCorr(0), vetoLepWeight(1), selLepWeight(1), useHPS(false), tnpEvtWeight(1) {}
      virtual ~LeptonCorrectionSet() {}
      virtual void load(TString fileName,
                        const LeptonSelection::Electron elSel, const LeptonSelection::Electron secElSel,
                        const LeptonSelection::Muon     muSel, const LeptonSelection::Muon     secMuSel,
                        int correctionOptions = NULLOPT);
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
