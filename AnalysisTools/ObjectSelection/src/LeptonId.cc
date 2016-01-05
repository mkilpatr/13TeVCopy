#include "AnalysisTools/ObjectSelection/interface/LeptonId.h"

//-------------- Print-outs ------------------------------//
namespace LeptonSelection {
std::ostream& operator<<(std::ostream& os, const Electron& a){
  os << "  min Pt = "<< a.minPT <<std::endl;
  os << "  max eta = "<< a.maxETA <<std::endl;
  os << "  max D0 = "<< a.maxD0 <<std::endl;
  os << "  max Dz = "<< a.maxDz <<std::endl;
  os << "  type = ";
  switch(a.type){
    case ZL_SEL_ELE: os <<"ZL_SEL_ELE"; break;
    case ZL_CTR_ELE: os <<"ZL_CTR_ELE"; break;
    case SL_SEL_ELE: os <<"SL_SEL_ELE"; break;
    case SL_SEL_SEC_ELE: os <<"SL_SEL_SEC_ELE"; break;
    default: os << "NONE_ELE";
  }
  os <<std::endl;
  return os;
}
std::ostream& operator<<(std::ostream& os, const Muon& a){
  os << "  min Pt = "<< a.minPT <<std::endl;
  os << "  max eta = "<< a.maxETA <<std::endl;
  os << "  max D0 = "<< a.maxD0 <<std::endl;
  os << "  max Dz = "<< a.maxDz <<std::endl;
  os << "  type = ";
  switch(a.type){
    case ZL_SEL_MU: os <<"ZL_SEL_MU"; break;
    case ZL_CTR_MU: os <<"ZL_CTR_MU"; break;
    case SL_SEL_MU: os <<"SL_SEL_MU"; break;
    case SL_SEL_SEC_MU: os <<"SL_SEL_SEC_MU"; break;
    default: os << "NONE_MU";
  }
  os <<std::endl;
  return os;
}
}

//-------------- Zero lepton Electrons ------------------------------//
namespace LeptonSelection {
  Electron zl_sel_electrons (
                              5,
                             2.4,
                             -1,
                             -1,
                             &ElectronID::vetoNoIso,
                             &ElectronISO::medMini,
                             ZL_SEL_ELE);
  Electron zl_ctr_sLep_electrons (
                              30,
                              2.1,
                              0.02,
                              0.1,
                              &ElectronID ::mediumNoIso,
                              &ElectronISO::medMini,
                              ZL_CTR_ELE);
  Electron zl_ctr_sLep_sec_electrons (
                               5,
                             2.4,
                             0.02,
                             0.1,
                             &ElectronID ::mediumNoIso,
                             &ElectronISO::medMini,
                             ZL_CTR_ELE);
  Electron zl_ctr_diLep_electrons(
                              20,
                              2.4,
                              0.02,
                              0.1,
                              &ElectronID ::mediumNoIso,
                              &ElectronISO::medMini,
                              ZL_CTR_ELE);
  Electron zl_ctr_diLep_sec_electrons(
                              15,
                              2.4,
                              0.02,
                              0.1,
                              &ElectronID ::mediumNoIso,
                              &ElectronISO::medMini,
                              ZL_CTR_ELE);
}

//-------------- Zero lepton Muons ------------------------------//
namespace LeptonSelection {
Muon zl_sel_muons(
                             5,
                            2.4,
                            0.1,
                            0.5,
                            &MuonID ::loose,
                            &MuonISO::looseMini,
                            ZL_SEL_MU);
Muon zl_ctr_sLep_muons(
                             30,
                            2.4,
                            0.02,
                            0.1,
                            &MuonID ::medium,
                            &MuonISO::medMini,
                            ZL_CTR_MU);
Muon zl_ctr_sLep_sec_muons(
                             5,
                            2.4,
                            0.02,
                            0.1,
                            &MuonID ::medium,
                            &MuonISO::medMini,
                            ZL_CTR_MU);


Muon zl_ctr_diLep_muons(
                             20,
                            2.4,
                            0.02,
                            0.1,
                            &MuonID ::medium,
                            &MuonISO::medMini,
                            ZL_CTR_MU);
Muon zl_ctr_diLep_sec_muons(
                             10,
                            2.4,
                            0.02,
                            0.1,
                            &MuonID ::medium,
                            &MuonISO::medMini,
                            ZL_CTR_MU);
}

//-------------- Single lepton Muons + Electrons ------------------------------//
namespace LeptonSelection {
  Electron sl_sel_electrons (
                             30,
                             1.4442,
                             -1,
                             -1,
                             &ElectronID ::mediumNoIso,
                             &ElectronISO::medMini,
                             NONE_ELE);
  Electron sl_sel_sec_electrons (
                               5,
                             2.4,
                              -1,
                              -1,
                              &ElectronID ::vetoNoIso,
                              &ElectronISO::looseMini,
                              NONE_ELE);
  Muon sl_sel_muons(
                               20,
                              2.1,
                              0.02,
                              0.1,
                              &MuonID ::medium,
                              &MuonISO::medMini,
                              NONE_MU);
  Muon sl_sel_sec_muons(
                               5,
                              2.4,
                              0.1,
                              0.5,
                              &MuonID ::loose,
                              &MuonISO::looseMini,
                              NONE_MU);

}

