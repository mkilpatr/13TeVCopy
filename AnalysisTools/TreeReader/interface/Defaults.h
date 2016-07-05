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
  const std::string BRANCH_CASUBJETS = "subjetsca";
  enum Process {DATA, QCD, TTBAR, SINGLE_W, SINGLE_Z, SINGLE_G, SINGLE_T, TTZ, TTW, DIBOSON, SIGNAL, DATA_SINGLEEL, DATA_SINGLEMU, DATA_SINGLEPHO, DATA_DOUBLEEG, DATA_DOUBLEMU, DATA_MUEG, DATA_MET, DATA_JETHT, DATA_HTMHT, NUMPROCESSES };

  const std::string PROCESS_NAMES[]   = {"data","qcd","ttbar","w","z","g","t","ttZ","ttW","diboson","signal","singleel","singlemu","singlepho","doubleeg","doublemu","mueg","met","jetht","htmht",""};

  enum DataReco {MC, PROMPT_50NS, JUL17_2015_50NS, PROMPT_25NS_2015C, PROMPT_25NS_2015D, OCT05_2015_2015D, OCT05_2015_2015C, PROMPT_25NS_2016B, PROMPT_25NS_2016C};

  const std::string DATA_RECO_NAMES[] = {"","2015b-pr","2015b-17jul15","2015c-pr","2015d-pr","2015d-05oct15","2015c-05oct15","2016b-pr","2016c-pr","invalid"};

  const std::string BRANCH_PUPPIJETS  = "ak4puppi";
  const std::string BRANCH_CA8JETS    = "ca8";
  const std::string BRANCH_ELECTRONS  = "ele";
  const std::string BRANCH_MUONS      = "mu";
  const std::string BRANCH_TAUS       = "tau";
  const std::string BRANCH_PHOTONS    = "pho";
  const std::string BRANCH_PFCANDS    = "pfcand";
  const std::string BRANCH_GENPARTS   = "gen";
  const std::string BRANCH_CMSTOPS    = "ctt";
  const std::string BRANCH_CORRAL     = "corral";
  const std::string BRANCH_AK8FATJETS = "ak8";
  const std::string BRANCH_AK8PUPPIFATJETS = "ak8puppi";
  const std::string BRANCH_TRIGGERS   = "trig";
  const std::string BRANCH_METFILTERS = "metfilter";
  const std::string BRANCH_SV         = "sv";

  enum CSVWPs {CSVL, CSVM, CSVT};
  const double CSVValues[] = {0.460,0.800,0.935};

  //Will be depreciated in next run
  const double CSV_LOOSE        = 0.460;
  const double CSV_MEDIUM       = 0.800;
  const double CSV_TIGHT        = 0.935;
  ///

  const double TAU_MVA_VETO_MTPRESEL_MEDIUM = 0.71;

  const double TAU_MVA_VETO_DPHIPRESEL_MEDIUM = 0.53;
  const double TAU_MTCUT_VETO = 100.;
  const double TAU_DPHICUT_VETO = 1.34;
}


#endif
