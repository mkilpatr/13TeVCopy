//--------------------------------------------------------------------------------------------------
// 
// BaseReader
// 
// Class to help with reading branches from TTrees.
// 
// BaseReader.h created on Tue Oct 21 15:33:32 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_TREEREADER_DEFAULTS_H
#define ANALYSISTOOLS_TREEREADER_DEFAULTS_H

#include <string>

namespace defaults {
  const std::string BRANCH_AK4JETS   = "ak4";
  const std::string BRANCH_PICKYJETS = "picky";
  enum Process {DATA, QCD, TTBAR, SINGLE_W, SINGLE_Z, SINGLE_G,SINGLE_T, TTZ, TTW,DIBOSON,SIGNAL, NUMPROCESSES };
  const std::string PROCESS_NAMES[] = {"data","qcd","ttbar","w","z","g","t","ttZ","ttW","diboson","signal",""};
  const std::string BRANCH_PUPPIJETS = "ak4puppi";
  const std::string BRANCH_CA8JETS ="ca8";
  const std::string BRANCH_ELECTRONS = "ele";
  const std::string BRANCH_MUONS     = "mu";
  const std::string BRANCH_TAUS      = "tau";
  const std::string BRANCH_PFCANDS   = "pfcand";
  const std::string BRANCH_GENPARTS  = "gen";
  const std::string BRANCH_CMSTOPS   = "ctt";
  const std::string BRANCH_CORRAL    = "corral";
  const double CSV_LOOSE      = 0.423;
  const double CSV_MEDIUM     = 0.814;
  const double CSV_TIGHT      = 0.941;
  const double CSV_OLD_LOOSE  = 0.244;
  const double CSV_OLD_MEDIUM = 0.679;
  const double CSV_OLD_TIGHT  = 0.898;
  const double TAU_MVA_VETO_MTPRESEL_LOOSE = 0.65;
  const double TAU_MVA_VETO_MTPRESEL_MEDIUM = 0.56;
  const double TAU_MVA_VETO_MTPRESEL_TIGHT = 0.45;
  const double TAU_MVA_VETO_DPHIPRESEL_LOOSE = 0.66;
  const double TAU_MVA_VETO_DPHIPRESEL_MEDIUM = 0.53;
  const double TAU_MVA_VETO_DPHIPRESEL_TIGHT = 0.44;
  const double TAU_MTCUT_VETO = 100.;
  const double TAU_DPHICUT_VETO = 1.34;
}


#endif
