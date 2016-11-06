/*
  Softdrop MVA Tagger

  Diffs of SoftdropMVA.hh from HTTMVA.hh
    variables filling and calculation
    matching/exclusion radii 0.8/0.8 instead of ropt+0.1/ropt
    preselectedFatJets filling

  Struct:
    filler SoftdropMVAFiller
  Methods:
    book / fillBasicInfo for basics (# loose, # tight, pt, eta, sd mass, mva)
    bookTrain / fillTrainInfo for everything
  Filling scheme:
    fillMulti is used to fill every variable for every preselected fatjet in the event (pt 200, eta 2.4, softdrop mass 50, min subjet pt 20)
    thus if there are 5 preselected fatjets in the event every variable is filled exactly 5 times
    defaults are -9 -9. or false
    all csv of <=0 ( from failure of csv algo, few percent ) are set to 0 to save the MVA
    all eta are NOT abs(eta)
    a matching hadr. gen top quark is looked for only if process is ttbar(0l,1l) or signal. if one is found matches_gen = true and gen variables are non-default (>=0)
    preselected fatjets are sorted by descending MVA score, thus so are all variables. use [0] to flatten all variables to best MVA scoring fatjet
  Matching scheme:
    for each preselected fatjet if the process is ttbar(0l,1l) or signal we look for a top quark satisfying
      "matching" : gen_maxdrqfj < 0.8, meaning this top quark's b/w1/w2 quarks are contained within the fatjet 0.8 cone
        --> gen_maxdrqfj is defined as MAX dR( the b/w1/w2 quarks from this hadr. top quark decay, the preselected fatjet to which this hadr. top quark is matched )
      "exclusion": gen_mindrtq > 0.8, meaning no b/w1/w2 quarks from other hadr. top quark decays in the 0.8 cone
        --> gen_mindrtq is defined as min dR( this hadr. top quark , the b/w1/w2 quarks from other hadr. top quark decays )
    if both are satisfied, matches_gen is filled with true. if not, false.
  Variables:
    for gen properties see *gen* and combine with matches_gen (or else you'll get the default value), eg Scan("gen_top_pt", "met>200 && matches_gen")
    for preselected fat jet properties *fj*, their subjet properties *bydef* (by default method) and *bycsv* (by csv method), nick variables *nick*
    if wonky quickly check that vector variables are all filling correctly in parallel:
      tree->Scan("event:fj_pt:matches_gen:gen_toppt")
                   1 : 100 : 0 : -9
                   1 : 150 : 1 : 160
                   2 : ...
      using two sets of vector variables, say if leptonpt were fillMulti and you did "fj_pt>400 && leptonpt>20", can cause shit to hit the fan
*/

#ifndef SOFTDROPMVA_HH
#define SOFTDROPMVA_HH

#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisTools/DataFormats/interface/CMSTop.h"
#include "AnalysisTools/Utilities/interface/JetFlavorInfo.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include <vector>
#include "Math/LorentzVector.h"
#include "Math/VectorUtil.h"
#include "AnalysisTools/Utilities/interface/PartonMatching.h"
#include "AnalysisTools/ObjectSelection/src/SoftdropMVA.cc"
#include "MVACommon.hh"

using namespace ucsbsusy;

struct SoftdropMVAFiller {

  SoftdropMVAFiller() {}

  std::vector<const FatJetF*> preselectedFatjets; // those fatjets passing the preselection defined in preparePreselectedFatJets
  std::vector<pair<const PartonMatching::TopDecay*, const FatJetF*> matchPairs; // exactly one pair for every preselectedFatjet. first entry is the gen top if match found.
  std::vector<const PartonMatching::TopDecay*> gentops; // hadronic gen tops

  //// basic variables
  size i_nLooseSoftdropVA;
  size i_nTightSoftdropMVA;
  size i_basic_ak8_matches_gen;
  size i_basic_ak8_pt;
  size i_basic_ak8_eta;
  size i_basic_ak8_sdmass;
  size i_basic_ak8_mva;
  size i_basic_ak8_matches_gen;


  //// train variables
  size i_ak8_matches_gen;
 
  size i_ak8_gen_top_pt;
  size i_ak8_gen_w_pt;
  size i_ak8_gen_b_pt;
  size i_ak8_gen_mindrtt;
  size i_ak8_gen_mindrtq;
  size i_ak8_gen_mindrqq;
  size i_ak8_gen_maxdrqfj;
  size i_ak8_gen_quarkalarmdr;
  size i_ak8_gen_quarkalarmpt;

  size i_ak8_mva;
  size i_ak8_passCutBasedSd;
  size i_ak8_passCutBased;
  size i_ak8_minsubjetpt;
  size i_ak8_nsubjets;
  size i_ak8_pt;
  size i_ak8_eta;
  size i_ak8_sdmass;
  size i_ak8_tau1;
  size i_ak8_tau2;
  size i_ak8_tau3;
  size i_ak8_tau21;
  size i_ak8_tau32;
  size i_ak8_tau31;
  size i_ak8_deltaR;
  size i_ak8_ptDR;
  size i_ak8_rel_ptdiff;

  // subjet variables: pt ranked
  size i_ak8_sj1_pt;
  size i_ak8_sj1_mass;
  size i_ak8_sj1_csv;
  size i_ak8_sj1_ptD;
  size i_ak8_sj1_axis1;
  size i_ak8_sj1_axis2;
  size i_ak8_sj1_mult;

  size i_ak8_sj2_pt;
  size i_ak8_sj2_mass;
  size i_ak8_sj2_csv;
  size i_ak8_sj2_ptD;
  size i_ak8_sj2_axis1;
  size i_ak8_sj2_axis2;
  size i_ak8_sj2_mult;

  // subjet variables: CSV ranked
  size i_ak8_csv1_pt;
  size i_ak8_csv1_mass;
  size i_ak8_csv1_csv;
  size i_ak8_csv1_ptD;
  size i_ak8_csv1_axis1;
  size i_ak8_csv1_axis2;
  size i_ak8_csv1_mult;

  size i_ak8_csv2_pt;
  size i_ak8_csv2_mass;
  size i_ak8_csv2_csv;
  size i_ak8_csv2_ptD;
  size i_ak8_csv2_axis1;
  size i_ak8_csv2_axis2;
  size i_ak8_csv2_mult;

  // subjet variables: mass ranked
  size i_ak8_mass1_pt;
  size i_ak8_mass1_mass;
  size i_ak8_mass1_csv;
  size i_ak8_mass1_ptD;
  size i_ak8_mass1_axis1;
  size i_ak8_mass1_axis2;
  size i_ak8_mass1_mult;

  size i_ak8_mass2_pt;
  size i_ak8_mass2_mass;
  size i_ak8_mass2_csv;
  size i_ak8_mass2_ptD;
  size i_ak8_mass2_axis1;
  size i_ak8_mass2_axis2;
  size i_ak8_mass2_mult;

  size i_ak8_sd_0;
  size i_ak8_sd_n0p5;
  size i_ak8_sd_n1;
  size i_ak8_sd_n1p5;
  size i_ak8_sd_n2;

  void book(TreeWriterData* data) {
    i_nLooseSoftdropMVA    = data.add<int  >("","nLooseSoftdropMVA",-9);
    i_nTightSoftdropMVA    = data.add<int  >("","nTightSoftdropMVA",-9);
    i_basic_ak8_matches_gen= data.addMulti<bool >("","basic_ak8_matches_gen",false);
    i_basic_ak8_pt          = data.addMulti<float>("","basic_ak8_pt",-9);
    i_basic_ak8_eta         = data.addMulti<float>("","basic_ak8_eta",-9);
    i_basic_ak8_sdmass      = data.addMulti<float>("","basic_ak8_sdmass",-9);
    i_basic_ak8_roptmass    = data.addMulti<float>("","basic_ak8_roptmass",-9);
    i_basic_ak8_mva         = data.addMulti<float>("","basic_ak8_mva",-9);
    i_basic_ak8_matches_gen = data.addMulti<bool >("","basic_ak8_matches_gen",false);
  }

  void bookTrain(TreeWriterData* data) {

    // of gen top quark, if match found
    i_ak8_gen_top_pt           = data.addMulti<float>("","ak8_gen_top_pt",-9);
    i_ak8_gen_w_pt             = data.addMulti<float>("","ak8_gen_w_pt"  ,-9);
    i_ak8_gen_b_pt             = data.addMulti<float>("","ak8_gen_b_pt"  ,-9);

    i_ak8_gen_mindrtt          = data.addMulti<float>("","ak8_gen_mindrtt",-9);
    i_ak8_gen_mindrtq          = data.addMulti<float>("","ak8_gen_mindrtq",-9);
    i_ak8_gen_mindrqq          = data.addMulti<float>("","ak8_gen_mindrqq",-9);
    i_ak8_gen_maxdrqfj         = data.addMulti<float>("","ak8_gen_maxdrqfj",-9);
    i_ak8_gen_quarkalarmpt     = data.addMulti<float>("","ak8_gen_quarkalarmpt",-9);
    i_ak8_gen_quarkalarmdr     = data.addMulti<float>("","ak8_gen_quarkalarmdr",-9);

    // is preselected fatjet matched to gen top quark?
    i_ak8_matches_gen          = data.addMulti<bool >("","ak8_matches_gen",false);

    // of preselected fatjets
    i_ak8_mva                  = data.addMulti<float>("","ak8_mva",-9);
    i_ak8_passCutBasedSd       = data.addMulti<bool >("","ak8_passCutBasedSd",false);
    i_ak8_passCutBasedICHEP    = data.addMulti<bool >("","ak8_passCutBasedICHEP",false);
    i_ak8_minsubjetpt          = data.addMulti<float>("","ak8_minsubjetpt",-9);
    i_ak8_nsubjets             = data.addMulti<int>("","ak8_nsubjets",-9);
    i_ak8_pt          = data.addMulti<float>("","var_ak8_pt",-9);
    i_ak8_eta         = data.addMulti<float>("","var_ak8_eta",-9);
    i_ak8_sdmass      = data.addMulti<float>("","var_ak8_sdmass",-9);
    i_ak8_tau1        = data.addMulti<float>("","var_ak8_tau1",-9);
    i_ak8_tau2        = data.addMulti<float>("","var_ak8_tau2",-9);
    i_ak8_tau3        = data.addMulti<float>("","var_ak8_tau3",-9);
    i_ak8_tau21       = data.addMulti<float>("","var_ak8_tau21",-9);
    i_ak8_tau32       = data.addMulti<float>("","var_ak8_tau32",-9);
    i_ak8_deltaR      = data.addMulti<float>("","var_ak8_deltaR",-9);
    i_ak8_ptDR        = data.addMulti<float>("","var_ak8_ptDR",-9);
    i_ak8_rel_ptdiff  = data.addMulti<float>("","var_ak8_rel_ptdiff",-9);

    // by pt
    i_ak8_sj1_pt         = data.addMulti<float>("","var_ak8_sj1_pt",  -9);
    i_ak8_sj1_mass       = data.addMulti<float>("","var_ak8_sj1_mass",  -9);
    i_ak8_sj1_csv        = data.addMulti<float>("","var_ak8_sj1_csv",  -9);
    i_ak8_sj1_ptD        = data.addMulti<float>("","var_ak8_sj1_ptD",  -9);
    i_ak8_sj1_axis1      = data.addMulti<float>("","var_ak8_sj1_axis1",  -9);
    i_ak8_sj1_axis2      = data.addMulti<float>("","var_ak8_sj1_axis2",  -9);
    i_ak8_sj1_mult       = data.addMulti<int>  ("","var_ak8_sj1_mult",  -9);

    i_ak8_sj2_pt         = data.addMulti<float>("","var_ak8_sj2_pt",  -9);
    i_ak8_sj2_mass       = data.addMulti<float>("","var_ak8_sj2_mass",  -9);
    i_ak8_sj2_csv        = data.addMulti<float>("","var_ak8_sj2_csv",  -9);
    i_ak8_sj2_ptD        = data.addMulti<float>("","var_ak8_sj2_ptD",  -9);
    i_ak8_sj2_axis1      = data.addMulti<float>("","var_ak8_sj2_axis1",  -9);
    i_ak8_sj2_axis2      = data.addMulti<float>("","var_ak8_sj2_axis2",  -9);
    i_ak8_sj2_mult       = data.addMulti<int>  ("","var_ak8_sj2_mult",  -9);

    // by csv
    i_ak8_csv1_pt         = data.addMulti<float>("","var_ak8_csv1_pt",  -9);
    i_ak8_csv1_mass       = data.addMulti<float>("","var_ak8_csv1_mass",  -9);
    i_ak8_csv1_csv        = data.addMulti<float>("","var_ak8_csv1_csv",  -9);
    i_ak8_csv1_ptD        = data.addMulti<float>("","var_ak8_csv1_ptD",  -9);
    i_ak8_csv1_axis1      = data.addMulti<float>("","var_ak8_csv1_axis1",  -9);
    i_ak8_csv1_axis2      = data.addMulti<float>("","var_ak8_csv1_axis2",  -9);
    i_ak8_csv1_mult       = data.addMulti<int>  ("","var_ak8_csv1_mult",  -9);

    i_ak8_csv2_pt         = data.addMulti<float>("","var_ak8_csv2_pt",  -9);
    i_ak8_csv2_mass       = data.addMulti<float>("","var_ak8_csv2_mass",  -9);
    i_ak8_csv2_csv        = data.addMulti<float>("","var_ak8_csv2_csv",  -9);
    i_ak8_csv2_ptD        = data.addMulti<float>("","var_ak8_csv2_ptD",  -9);
    i_ak8_csv2_axis1      = data.addMulti<float>("","var_ak8_csv2_axis1",  -9);
    i_ak8_csv2_axis2      = data.addMulti<float>("","var_ak8_csv2_axis2",  -9);
    i_ak8_csv2_mult       = data.addMulti<int>  ("","var_ak8_csv2_mult",  -9);

    // by mass
    i_ak8_mass1_pt         = data.addMulti<float>("","var_ak8_mass1_pt",  -9);
    i_ak8_mass1_mass       = data.addMulti<float>("","var_ak8_mass1_mass",  -9);
    i_ak8_mass1_csv        = data.addMulti<float>("","var_ak8_mass1_csv",  -9);
    i_ak8_mass1_ptD        = data.addMulti<float>("","var_ak8_mass1_ptD",  -9);
    i_ak8_mass1_axis1      = data.addMulti<float>("","var_ak8_mass1_axis1",  -9);
    i_ak8_mass1_axis2      = data.addMulti<float>("","var_ak8_mass1_axis2",  -9);
    i_ak8_mass1_mult       = data.addMulti<int>  ("","var_ak8_mass1_mult",  -9);

    i_ak8_mass2_pt         = data.addMulti<float>("","var_ak8_mass2_pt",  -9);
    i_ak8_mass2_mass       = data.addMulti<float>("","var_ak8_mass2_mass",  -9);
    i_ak8_mass2_csv        = data.addMulti<float>("","var_ak8_mass2_csv",  -9);
    i_ak8_mass2_ptD        = data.addMulti<float>("","var_ak8_mass2_ptD",  -9);
    i_ak8_mass2_axis1      = data.addMulti<float>("","var_ak8_mass2_axis1",  -9);
    i_ak8_mass2_axis2      = data.addMulti<float>("","var_ak8_mass2_axis2",  -9);
    i_ak8_mass2_mult       = data.addMulti<int>  ("","var_ak8_mass2_mult",  -9);

    i_ak8_sd_0           = data.addMulti<float>("","var_ak8_sd_0",-9);
    i_ak8_sd_n0p5        = data.addMulti<float>("","var_ak8_sd_n0p5",-9);
    i_ak8_sd_n1          = data.addMulti<float>("","var_ak8_sd_n1",-9);
    i_ak8_sd_n1p5        = data.addMulti<float>("","var_ak8_sd_n1p5",-9);
    i_ak8_sd_n2          = data.addMulti<float>("","var_ak8_sd_n2",-9);

  }

  void fillFatJetTrainInfo(const FatJetF *fatjet, bool matchesGen){
    assert(fatjet->nSubjets()==2); // preselected fatjet. if not, MUST fill all vars with default values, or else have vector vars of different lengths.

    data.fillMulti<bool> (i_ak8_matches_gen, matchesGen);
    data.fillMulti<float>(i_ak8_mva,         fatjet->mva());
    data.fillMulti<bool >(i_ak8_passCutBasedSd,     cfgSet::isSoftDropTagged(fatjet, 400, 110, 210, 0.69, 1e9));
    data.fillMulti<bool >(i_ak8_passCutBasedICHEP,  cfgSet::isSoftDropTagged(fatjet, 400, 105, 220, 0.67, 1e9, 0.46));

    // all other vars use ONE set of code located in AnalysisTools/ObjectSelection/src/SoftdropMVA.cc
    auto varMap = SoftdropMVA::calcSoftdropMVAVars(fatjet, true);

    data.fillMulti<int  >(i_ak8_nsubjets,          int(varMap["ak8_nsubjets"]));
    data.fillMulti<float>(i_ak8_minsubjetpt,       varMap["ak8_minsubjetpt"]);
    data.fillMulti<float>(i_ak8_pt,                varMap["ak8_pt"]);
    data.fillMulti<float>(i_ak8_eta,               varMap["ak8_eta"]);
    data.fillMulti<float>(i_ak8_sdmass,            varMap["ak8_sdmass"]);
    data.fillMulti<float>(i_ak8_tau1,              varMap["ak8_tau1"]);
    data.fillMulti<float>(i_ak8_tau2,              varMap["ak8_tau2"]);
    data.fillMulti<float>(i_ak8_tau3,              varMap["ak8_tau3"]);
    data.fillMulti<float>(i_ak8_tau21,             varMap["ak8_tau21"]);
    data.fillMulti<float>(i_ak8_tau32,             varMap["ak8_tau32"]);
    data.fillMulti<float>(i_ak8_tau31,             varMap["ak8_tau31"]);

    // varialbes involve both subjets
    data.fillMulti<float>(i_ak8_deltaR,            varMap["ak8_deltaR"]);
    data.fillMulti<float>(i_ak8_ptDR,              varMap["ak8_ptDR"]);
    data.fillMulti<float>(i_ak8_rel_ptdiff,        varMap["ak8_rel_ptdiff"]);

    // pt ranked
    data.fillMulti<float>(i_ak8_sj1_pt,            varMap["ak8_sj1_pt"]);
    data.fillMulti<float>(i_ak8_sj1_mass,          varMap["ak8_sj1_mass"]);
    data.fillMulti<float>(i_ak8_sj1_csv,           varMap["ak8_sj1_csv"]);
    data.fillMulti<float>(i_ak8_sj1_ptD,           varMap["ak8_sj1_ptD"]);
    data.fillMulti<float>(i_ak8_sj1_axis1,         varMap["ak8_sj1_axis1"]);
    data.fillMulti<float>(i_ak8_sj1_axis2,         varMap["ak8_sj1_axis2"]);
    data.fillMulti<int>  (i_ak8_sj1_mult,          int(varMap["ak8_sj1_mult"]));

    data.fillMulti<float>(i_ak8_sj2_pt,            varMap["ak8_sj2_pt"]);
    data.fillMulti<float>(i_ak8_sj2_mass,          varMap["ak8_sj2_mass"]);
    data.fillMulti<float>(i_ak8_sj2_csv,           varMap["ak8_sj2_csv"]);
    data.fillMulti<float>(i_ak8_sj2_ptD,           varMap["ak8_sj2_ptD"]);
    data.fillMulti<float>(i_ak8_sj2_axis1,         varMap["ak8_sj2_axis1"]);
    data.fillMulti<float>(i_ak8_sj2_axis2,         varMap["ak8_sj2_axis2"]);
    data.fillMulti<int>  (i_ak8_sj2_mult,          int(varMap["ak8_sj2_mult"]));

    // sd vars
    data.fillMulti<float>(i_ak8_sd_0,              varMap["ak8_sd_0"]);
    data.fillMulti<float>(i_ak8_sd_n0p5,           varMap["ak8_sd_n0p5"]);
    data.fillMulti<float>(i_ak8_sd_n1,             varMap["ak8_sd_n1"]);
    data.fillMulti<float>(i_ak8_sd_n1p5,           varMap["ak8_sd_n1p5"]);
    data.fillMulti<float>(i_ak8_sd_n2,             varMap["ak8_sd_n2"]);

    // CSV ranked

    data.fillMulti<float>(i_ak8_csv1_pt,            varMap["ak8_csv1_pt"]);
    data.fillMulti<float>(i_ak8_csv1_mass,          varMap["ak8_csv1_mass"]);
    data.fillMulti<float>(i_ak8_csv1_csv,           varMap["ak8_csv1_csv"]);
    data.fillMulti<float>(i_ak8_csv1_ptD,           varMap["ak8_csv1_ptD"]);
    data.fillMulti<float>(i_ak8_csv1_axis1,         varMap["ak8_csv1_axis1"]);
    data.fillMulti<float>(i_ak8_csv1_axis2,         varMap["ak8_csv1_axis2"]);
    data.fillMulti<int>  (i_ak8_csv1_mult,          int(varMap["ak8_csv1_mult"]));

    data.fillMulti<float>(i_ak8_csv2_pt,            varMap["ak8_csv2_pt"]);
    data.fillMulti<float>(i_ak8_csv2_mass,          varMap["ak8_csv2_mass"]);
    data.fillMulti<float>(i_ak8_csv2_csv,           varMap["ak8_csv2_csv"]);
    data.fillMulti<float>(i_ak8_csv2_ptD,           varMap["ak8_csv2_ptD"]);
    data.fillMulti<float>(i_ak8_csv2_axis1,         varMap["ak8_csv2_axis1"]);
    data.fillMulti<float>(i_ak8_csv2_axis2,         varMap["ak8_csv2_axis2"]);
    data.fillMulti<int>  (i_ak8_csv2_mult,          int(varMap["ak8_csv2_mult"]));

    // mass ranked

    data.fillMulti<float>(i_ak8_mass1_pt,            varMap["ak8_mass1_pt"]);
    data.fillMulti<float>(i_ak8_mass1_mass,          varMap["ak8_mass1_mass"]);
    data.fillMulti<float>(i_ak8_mass1_csv,           varMap["ak8_mass1_csv"]);
    data.fillMulti<float>(i_ak8_mass1_ptD,           varMap["ak8_mass1_ptD"]);
    data.fillMulti<float>(i_ak8_mass1_axis1,         varMap["ak8_mass1_axis1"]);
    data.fillMulti<float>(i_ak8_mass1_axis2,         varMap["ak8_mass1_axis2"]);
    data.fillMulti<int>  (i_ak8_mass1_mult,          int(varMap["ak8_mass1_mult"]));

    data.fillMulti<float>(i_ak8_mass2_pt,            varMap["ak8_mass2_pt"]);
    data.fillMulti<float>(i_ak8_mass2_mass,          varMap["ak8_mass2_mass"]);
    data.fillMulti<float>(i_ak8_mass2_csv,           varMap["ak8_mass2_csv"]);
    data.fillMulti<float>(i_ak8_mass2_ptD,           varMap["ak8_mass2_ptD"]);
    data.fillMulti<float>(i_ak8_mass2_axis1,         varMap["ak8_mass2_axis1"]);
    data.fillMulti<float>(i_ak8_mass2_axis2,         varMap["ak8_mass2_axis2"]);
    data.fillMulti<int>  (i_ak8_mass2_mult,          int(varMap["ak8_mass2_mult"]));

  }

  template<class FatJet>
  void fillGenInfo(const PartonMatching::topDecay *top, const vector<const PartonMatching::TopDecay*> tops, const FatJet& fj){
    data.fillMulti<float>(i_ak8_gen_top_pt,  (top ? top->top->pt() : -9));
    data.fillMulti<float>(i_ak8_gen_w_pt,    (top ? top->W->pt() : -9));
    data.fillMulti<float>(i_ak8_gen_b_pt,    (top ? top->b->parton->pt() : -9));

    float mindrtq = 99., mindrqq = 99., mindrtt = 99., quarkalarmdr = 99., quarkalarmpt = -9.;
    if(top) MVACommon::calcGenInfo(top, tops, mindrtq, mindrqq, mindrtt, quarkalarmdr, quarkalarmpt, 0.8);
    data.fillMulti<float>(i_ak8_gen_mindrtt, (top ? mindrtt : -9));
    data.fillMulti<float>(i_ak8_gen_mindrtq, (top ? mindrtq : -9));
    data.fillMulti<float>(i_ak8_gen_mindrqq, (top ? mindrqq : -9));
    data.fillMulti<float>(i_ak8_gen_maxdrqfj,(top ? MVACommon::getmaxdrqfj(top, fj) : -9));
    data.fillMulti<float>(i_ak8_gen_quarkalarmdr, (top ? quarkalarmdr : -9));
    data.fillMulti<float>(i_ak8_gen_quarkalarmpt, (top ? quarkalarmpt : -9));
  }

  void preparePreselectedFatjets() {

    preselectedFatjets.clear();
    for (const auto *fj : fatJets){
      if(SoftdropMVA::isPreselected(fj) {
        preselectedFatjets.push_back(fj);
      }
    }

    // sort by descending MVA score
    std::sort(preselectedFatjets.begin(), preselectedFatjets.end(), [&](const FatJetF *a, const FatJetF *b){ return a->mva() > b->mva(); });
  }

  void prepareGenTops(){
    gentops.clear();

    std::vector<GenJetF*> filteredGenJets;
    for(auto * j : jets){ if(j->genJet()) filteredGenJets.push_back(j->genJet()); }
    PartonMatching::PartonEvent * partonEvent = new PartonMatching::PartonEvent(genParticleReader,*defaultJets,filteredGenJets);
    for(unsigned int i = 0 ; i < partonEvent->topDecays.size() ; i++){
      PartonMatching::TopDecay* top = &partonEvent->topDecays[i];
      if(top->isLeptonic) continue;
      gentops.push_back(top); // Note: don't condense this loop or else bugs from &X->Y[i] resolving as &X ?
    }
  }

  void fillBasicInfo(TreeWriterData* data, BaseTreeAnalyzer* ana) {
    bool dbg = false;

    bool topProcess =  (process==defaults::TTBAR && !isTTbar2L) || process==defaults::SIGNAL;
    preparePreselectedFatjets();
    prepareGenTops();

    data.fill<int  >(i_nLooseSoftdropMVA, std::count_if( preselectedFatjets.begin(), preselectedFatjets.end(), [](const FatJetF* fj){ return j->mva() > 0.50; }));
    data.fill<int  >(i_nTightSoftdropMVA, std::count_if( preselectedFatjets.begin(), preselectedFatjets.end(), [](const FatJetF* fj){ return j->mva() > 0.75; }));
    for(const auto &fj : preselectedFatjets) {
      const PartonMatching::TopDecay * top = (topProcess ? MVACommon::getTopCand(fj, gentops, 0.8, 0.8) : 0); // if not top process don't try to match
      data.fillMulti<bool >(i_basic_ak8_matches_gen, (top ? true : false));
      data.fillMulti<float>(i_basic_ak8_pt, fj.pt());
      data.fillMulti<float>(i_basic_ak8_eta, fj.eta1());
      data.fillMulti<float>(i_basic_ak8_sdmass, fj.softDropMass());
      data.fillMulti<float>(i_basic_ak8_roptmass, fj.ropt_mom().mass());
      data.fillMulti<float>(i_basic_ak8_mva, fj.mva());
    }
  }//fillBasicInfo

  void fillTrainInfo(TreeWriterData* data, BaseTreeAnalyzer* ana) {
    bool dbg = false;

    bool topProcess =  (process==defaults::TTBAR && !isTTbar2L) || process==defaults::SIGNAL;
    preparePreselectedFatjets();
    prepareGenTops();

    //int ngenhadronictops = 0;
    //for(auto top : gentops) { if( !top->isLeptonic) ngenhadronictops++; }
    //data.fill<int>(i_ngenhadronictops, ngenhadronictops);

    // fill for EVERY preselected fatjet. first entry 0 if unmatched --> match_gen variable.
    for(const auto &fj : preselectedFatjets) {
      const PartonMatching::TopDecay * top = (topProcess ? MVACommon::getTopCand(fj, gentops, 0.8, 0.8) : 0); // if not top process don't try to match
      matchPairs.emplace_back(top,*fj);
      fillGenInfo(top, gentops, fatjet);
      fillFatJetTrainInfo(fatjet, (top ? true : false));
    }

  }//fillTrainInfo

};//SoftdropMVAFiller

#endif
