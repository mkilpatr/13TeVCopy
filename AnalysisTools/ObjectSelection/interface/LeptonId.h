#ifndef ANALYSISTOOLS_OBJECTSELECTION_LEPTONID_H
#define ANALYSISTOOLS_OBJECTSELECTION_LEPTONID_H

#include "AnalysisTools/DataFormats/interface/Electron.h"
#include "AnalysisTools/DataFormats/interface/Muon.h"

//-------------- ID and ISO evaluating functions ------------------------------//
namespace ElectronID {
  inline bool inclusive      (const ucsbsusy::ElectronF* lep) {return true;}
  inline bool vetoNoIso      (const ucsbsusy::ElectronF* lep) {return lep->isvetoid();}
  inline bool looseNoIso     (const ucsbsusy::ElectronF* lep) {return lep->islooseid();}
  inline bool mediumNoIso    (const ucsbsusy::ElectronF* lep) {return lep->ismediumid();}
  inline bool veto           (const ucsbsusy::ElectronF* lep) {return lep->isvetoelectron();}
  inline bool loose          (const ucsbsusy::ElectronF* lep) {return lep->islooseelectron();}
  inline bool medium         (const ucsbsusy::ElectronF* lep) {return lep->ismediumelectron();}
  inline bool tight          (const ucsbsusy::ElectronF* lep) {return lep->istightelectron();}
}

namespace ElectronISO {
  inline bool inclusive     (const ucsbsusy::ElectronF* lep) {return true;}
  inline bool looseMini     (const ucsbsusy::ElectronF* lep) {return lep->miniiso()<0.2;}
  inline bool medMini       (const ucsbsusy::ElectronF* lep) {return lep->miniiso()<0.1;}
  inline bool tightRel      (const ucsbsusy::ElectronF* lep) {return lep->pfdbetaiso()/lep->pt() < 0.15;}
}

namespace MuonID {
  inline bool inclusive      (const ucsbsusy::MuonF* lep) {return true;}
  inline bool loose          (const ucsbsusy::MuonF* lep) {return lep->isloosemuon();}
  inline bool medium         (const ucsbsusy::MuonF* lep) {return lep->ismediummuon();}
  inline bool tight          (const ucsbsusy::MuonF* lep) {return lep->istightmuon();}
}

namespace MuonISO {
  inline bool inclusive     (const ucsbsusy::MuonF* lep) {return true;}
  inline bool looseMini     (const ucsbsusy::MuonF* lep) {return lep->miniiso()<0.2;}
  inline bool medMini       (const ucsbsusy::MuonF* lep) {return lep->miniiso()<0.1;}
  inline bool medRel        (const ucsbsusy::MuonF* lep) {return lep->pfdbetaiso()/lep->pt() < 0.20;}
}


//-------------- Base classes to set full selection settings ------------------------------//
namespace LeptonSelection {

  //Specific names that correspond to ID+ISO+DZ/0 combinations that can be used elsewhere
  enum ElectronType {NONE_ELE, ZL_SEL_ELE,ZL_CTR_ELE,SL_SEL_ELE, SL_SEL_SEC_ELE};
  enum MuonType     {NONE_MU, ZL_SEL_MU,ZL_CTR_MU,SL_SEL_MU, SL_SEL_SEC_MU};

  struct Electron {
    float minPT;
    float maxETA;
    float maxD0;
    float maxDz;
    bool (*passID)(const ucsbsusy::ElectronF* lep);
    bool (*passISO)(const ucsbsusy::ElectronF* lep);
    ElectronType type;
    bool isConfig;
    Electron() : minPT(-1),maxETA(-1),maxD0(-1),maxDz(-1),passID(&ElectronID::inclusive),passISO(&ElectronISO::inclusive),type(NONE_ELE),isConfig(false){}
    Electron(float minPT, float maxETA, float maxD0, float maxDz, bool (*passID)(const ucsbsusy::ElectronF* lep),
        bool (*passISO)(const ucsbsusy::ElectronF* lep), ElectronType type = NONE_ELE ) :
          minPT(minPT),maxETA(maxETA),maxD0(maxD0),maxDz(maxDz),passID(passID),passISO(passISO),type(type),isConfig(true){}
    friend std::ostream& operator<<(std::ostream& os, const Electron& a);
  };
  struct Muon {
    float minPT;
    float maxETA;
    float maxD0;
    float maxDz;
    bool (*passID)(const ucsbsusy::MuonF* lep);
    bool (*passISO)(const ucsbsusy::MuonF* lep);
    MuonType type;
    bool isConfig;
    Muon() : minPT(-1),maxETA(-1),maxD0(-1),maxDz(-1),passID(&MuonID::inclusive),passISO(&MuonISO::inclusive), type(NONE_MU),isConfig(false){}
    Muon(float minPT, float maxETA, float maxD0, float maxDz, bool (*passID)(const ucsbsusy::MuonF* lep),
        bool (*passISO)(const ucsbsusy::MuonF* lep), MuonType type = NONE_MU ) :
          minPT(minPT),maxETA(maxETA),maxD0(maxD0),maxDz(maxDz),passID(passID),passISO(passISO),type(type),isConfig(true){}
    friend std::ostream& operator<<(std::ostream& os, const Muon& a);
  };
}

//-------------- Implementation of specific selections ------------------------------//
namespace LeptonSelection {

  extern Electron zl_sel_electrons ;
  extern Electron zl_ctr_sLep_electrons;
  extern Electron zl_ctr_sLep_sec_electrons;
  extern Electron zl_ctr_diLep_electrons;
  extern Electron zl_ctr_diLep_sec_electrons;
  extern Electron sl_sel_electrons ;
  extern Electron sl_sel_sec_electrons;

  extern Muon zl_sel_muons ;
  extern Muon zl_ctr_sLep_muons;
  extern Muon zl_ctr_sLep_sec_muons;
  extern Muon zl_ctr_diLep_muons;
  extern Muon zl_ctr_diLep_sec_muons;
  extern Muon sl_sel_muons ;
  extern Muon sl_sel_sec_muons;
}





#endif
