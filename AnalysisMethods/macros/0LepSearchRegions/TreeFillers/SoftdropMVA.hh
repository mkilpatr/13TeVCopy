/*
  updated feb 10 2017

  softdrop mva tops and ws
  treefiller for mva variables, either just a few basic variables or all variables used in training

  use in ZeroLeptonTreeHelper:
    SoftdropMVAFiller softdropMVAFiller;
    softdropMVAFiller.book() for basics or bookTrain() for all
    softdropMVAFiller.fillBasicInfo() or fillTrainInfo()

  filling of variables is PER-JET for all fatjets passing our standard PRESELECTION fxns
    AnalysisTools/ObjectSelection/src/SoftdropTopMVA.cc / SoftdropWMVA.cc:
    SoftdropTopMVA::isPreselected(fatjet) selects fatjets with pt 200, abseta 2.4, ==2 subjets, softdrop mass 50, subjet pt 20
    SoftdropWMVA::isPreselected(fatjet) selects fatjets with same --

  all calculations for Softdrop MVA variables are done centrally in AnalysisTools/ObjectSelection/src/SoftdropMVA.hh to avoid bugs
  all calculations for gen matching are done centrally in https://github.com/UCSBSusy/13TeVAnalysis/blob/80X_Moriond17/AnalysisBase/TreeAnalyzer/src/BaseTreeAnalyzer.cc#L560
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
#include "AnalysisTools/ObjectSelection/interface/SoftdropTopMVA.h"
#include "MVACommon.hh"

using namespace ucsbsusy;

struct SoftdropMVAFiller {

  SoftdropMVAFiller() {}

  std::vector<const FatJetF*> preselectedFatjets; // those fatjets passing the preselection defined in preparePreselectedFatJets
  //std::vector<pair<const PartonMatching::TopDecay*, const FatJetF*> > matchPairs; // exactly one pair for every preselectedFatjet. first entry is the gen top if match found.
  std::vector<const PartonMatching::TopDecay*> gentops; // hadronic gen tops
  PartonMatching::PartonEvent * partonEvent;

  //// basic variables
  size i_nLooseSoftdropMVA;
  size i_nMediumSoftdropMVA;
  size i_nTightSoftdropMVA;
  size i_basic_ak8_matches_gen;
  size i_basic_ak8_pt;
  size i_basic_ak8_eta;
  size i_basic_ak8_sdmass;
  size i_basic_ak8_top_mva;
  size i_basic_ak8_w_mva;

  //// train variables

  size i_ak8_recocat_istop;
  size i_ak8_recocat_isw;
  size i_ak8_gencat_istop;
  size i_ak8_gencat_isw;
  size i_ak8_gencat_iscontainedtop;
  size i_ak8_gencat_iscontainedw;

  size i_ak8_gen_nearleppt;
  size i_ak8_gen_nearlepeta;
  size i_ak8_gen_nearlepdr;
  size i_ak8_gen_nearlepid;

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
  size i_ak8_passCutBasedICHEP;
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
    i_nLooseSoftdropMVA    = data->add<int  >("","nLooseSoftdropMVA","I",-9);
    i_nMediumSoftdropMVA   = data->add<int  >("","nMediumSoftdropMVA","I",-9);
    i_nTightSoftdropMVA    = data->add<int  >("","nTightSoftdropMVA","I",-9);
    i_basic_ak8_matches_gen= data->addMulti<bool >("","basic_ak8_matches_gen",false);
    i_basic_ak8_pt          = data->addMulti<float>("","basic_ak8_pt",-9);
    i_basic_ak8_eta         = data->addMulti<float>("","basic_ak8_eta",-9);
    i_basic_ak8_sdmass      = data->addMulti<float>("","basic_ak8_sdmass",-9);
    i_basic_ak8_top_mva         = data->addMulti<float>("","basic_ak8_top_mva",-9);
    i_basic_ak8_w_mva         = data->addMulti<float>("","basic_ak8_w_mva",-9);
  }

  void bookTrain(TreeWriterData* data) {

    // cats    
    i_ak8_recocat_istop            = data->addMulti<bool>("","ak8_recocat_istop",false);
    i_ak8_recocat_isw              = data->addMulti<bool>("","ak8_recocat_isw",false);
    i_ak8_gencat_istop             = data->addMulti<bool>("","ak8_gencat_istop",false);
    i_ak8_gencat_isw               = data->addMulti<bool>("","ak8_gencat_isw",false);
    i_ak8_gencat_iscontainedtop    = data->addMulti<bool>("","ak8_gencat_iscontainedtop",false);
    i_ak8_gencat_iscontainedw      = data->addMulti<bool>("","ak8_gencat_iscontainedw",false);

    i_ak8_gen_nearleppt            = data->addMulti<float>("","ak8_gen_nearleppt",-9);
    i_ak8_gen_nearlepeta           = data->addMulti<float>("","ak8_gen_nearlepeta",-9);
    i_ak8_gen_nearlepdr            = data->addMulti<float>("","ak8_gen_nearlepdr",-9);
    i_ak8_gen_nearlepid            = data->addMulti<int  >("","ak8_gen_nearlepid",-9);

    // of gen top quark, if match found
    i_ak8_gen_top_pt           = data->addMulti<float>("","ak8_gen_top_pt",-9);
    i_ak8_gen_w_pt             = data->addMulti<float>("","ak8_gen_w_pt"  ,-9);
    i_ak8_gen_b_pt             = data->addMulti<float>("","ak8_gen_b_pt"  ,-9);

    i_ak8_gen_mindrtt          = data->addMulti<float>("","ak8_gen_mindrtt",-9);
    i_ak8_gen_mindrtq          = data->addMulti<float>("","ak8_gen_mindrtq",-9);
    i_ak8_gen_mindrqq          = data->addMulti<float>("","ak8_gen_mindrqq",-9);
    i_ak8_gen_maxdrqfj         = data->addMulti<float>("","ak8_gen_maxdrqfj",-9);
    i_ak8_gen_quarkalarmpt     = data->addMulti<float>("","ak8_gen_quarkalarmpt",-9);
    i_ak8_gen_quarkalarmdr     = data->addMulti<float>("","ak8_gen_quarkalarmdr",-9);

    // is preselected fatjet matched to gen top quark?
    i_ak8_matches_gen          = data->addMulti<bool >("","ak8_matches_gen",false);

    // of preselected fatjets
    i_ak8_mva                  = data->addMulti<float>("","ak8_mva",-9);
    i_ak8_passCutBasedSd       = data->addMulti<bool >("","ak8_passCutBasedSd",false);
    i_ak8_passCutBasedICHEP    = data->addMulti<bool >("","ak8_passCutBasedICHEP",false);
    i_ak8_minsubjetpt          = data->addMulti<float>("","ak8_minsubjetpt",-9);
    i_ak8_nsubjets             = data->addMulti<int>("","ak8_nsubjets",-9);
    i_ak8_pt          = data->addMulti<float>("","ak8_pt",-9);
    i_ak8_eta         = data->addMulti<float>("","ak8_eta",-9);
    i_ak8_sdmass      = data->addMulti<float>("","ak8_sdmass",-9);
    i_ak8_tau1        = data->addMulti<float>("","ak8_tau1",-9);
    i_ak8_tau2        = data->addMulti<float>("","ak8_tau2",-9);
    i_ak8_tau3        = data->addMulti<float>("","ak8_tau3",-9);
    i_ak8_tau21       = data->addMulti<float>("","ak8_tau21",-9);
    i_ak8_tau32       = data->addMulti<float>("","ak8_tau32",-9);
    i_ak8_tau31       = data->addMulti<float>("","ak8_tau31",-9);
    i_ak8_deltaR      = data->addMulti<float>("","ak8_deltaR",-9);
    i_ak8_ptDR        = data->addMulti<float>("","ak8_ptDR",-9);
    i_ak8_rel_ptdiff  = data->addMulti<float>("","ak8_rel_ptdiff",-9);

    // by pt
    i_ak8_sj1_pt         = data->addMulti<float>("","ak8_sj1_pt",  -9);
    i_ak8_sj1_mass       = data->addMulti<float>("","ak8_sj1_mass",  -9);
    i_ak8_sj1_csv        = data->addMulti<float>("","ak8_sj1_csv",  -9);
    i_ak8_sj1_ptD        = data->addMulti<float>("","ak8_sj1_ptD",  -9);
    i_ak8_sj1_axis1      = data->addMulti<float>("","ak8_sj1_axis1",  -9);
    i_ak8_sj1_axis2      = data->addMulti<float>("","ak8_sj1_axis2",  -9);
    i_ak8_sj1_mult       = data->addMulti<int>  ("","ak8_sj1_mult",  -9);

    i_ak8_sj2_pt         = data->addMulti<float>("","ak8_sj2_pt",  -9);
    i_ak8_sj2_mass       = data->addMulti<float>("","ak8_sj2_mass",  -9);
    i_ak8_sj2_csv        = data->addMulti<float>("","ak8_sj2_csv",  -9);
    i_ak8_sj2_ptD        = data->addMulti<float>("","ak8_sj2_ptD",  -9);
    i_ak8_sj2_axis1      = data->addMulti<float>("","ak8_sj2_axis1",  -9);
    i_ak8_sj2_axis2      = data->addMulti<float>("","ak8_sj2_axis2",  -9);
    i_ak8_sj2_mult       = data->addMulti<int>  ("","ak8_sj2_mult",  -9);

    // by csv
    i_ak8_csv1_pt         = data->addMulti<float>("","ak8_csv1_pt",  -9);
    i_ak8_csv1_mass       = data->addMulti<float>("","ak8_csv1_mass",  -9);
    i_ak8_csv1_csv        = data->addMulti<float>("","ak8_csv1_csv",  -9);
    i_ak8_csv1_ptD        = data->addMulti<float>("","ak8_csv1_ptD",  -9);
    i_ak8_csv1_axis1      = data->addMulti<float>("","ak8_csv1_axis1",  -9);
    i_ak8_csv1_axis2      = data->addMulti<float>("","ak8_csv1_axis2",  -9);
    i_ak8_csv1_mult       = data->addMulti<int>  ("","ak8_csv1_mult",  -9);

    i_ak8_csv2_pt         = data->addMulti<float>("","ak8_csv2_pt",  -9);
    i_ak8_csv2_mass       = data->addMulti<float>("","ak8_csv2_mass",  -9);
    i_ak8_csv2_csv        = data->addMulti<float>("","ak8_csv2_csv",  -9);
    i_ak8_csv2_ptD        = data->addMulti<float>("","ak8_csv2_ptD",  -9);
    i_ak8_csv2_axis1      = data->addMulti<float>("","ak8_csv2_axis1",  -9);
    i_ak8_csv2_axis2      = data->addMulti<float>("","ak8_csv2_axis2",  -9);
    i_ak8_csv2_mult       = data->addMulti<int>  ("","ak8_csv2_mult",  -9);

    // by mass
    i_ak8_mass1_pt         = data->addMulti<float>("","ak8_mass1_pt",  -9);
    i_ak8_mass1_mass       = data->addMulti<float>("","ak8_mass1_mass",  -9);
    i_ak8_mass1_csv        = data->addMulti<float>("","ak8_mass1_csv",  -9);
    i_ak8_mass1_ptD        = data->addMulti<float>("","ak8_mass1_ptD",  -9);
    i_ak8_mass1_axis1      = data->addMulti<float>("","ak8_mass1_axis1",  -9);
    i_ak8_mass1_axis2      = data->addMulti<float>("","ak8_mass1_axis2",  -9);
    i_ak8_mass1_mult       = data->addMulti<int>  ("","ak8_mass1_mult",  -9);

    i_ak8_mass2_pt         = data->addMulti<float>("","ak8_mass2_pt",  -9);
    i_ak8_mass2_mass       = data->addMulti<float>("","ak8_mass2_mass",  -9);
    i_ak8_mass2_csv        = data->addMulti<float>("","ak8_mass2_csv",  -9);
    i_ak8_mass2_ptD        = data->addMulti<float>("","ak8_mass2_ptD",  -9);
    i_ak8_mass2_axis1      = data->addMulti<float>("","ak8_mass2_axis1",  -9);
    i_ak8_mass2_axis2      = data->addMulti<float>("","ak8_mass2_axis2",  -9);
    i_ak8_mass2_mult       = data->addMulti<int>  ("","ak8_mass2_mult",  -9);

    i_ak8_sd_0           = data->addMulti<float>("","ak8_sd_0",-9);
    i_ak8_sd_n0p5        = data->addMulti<float>("","ak8_sd_n0p5",-9);
    i_ak8_sd_n1          = data->addMulti<float>("","ak8_sd_n1",-9);
    i_ak8_sd_n1p5        = data->addMulti<float>("","ak8_sd_n1p5",-9);
    i_ak8_sd_n2          = data->addMulti<float>("","ak8_sd_n2",-9);

  }

  void fillCats(TreeWriterData* data, const FatJetF *fatjet){
    // AnalysisTools/DataFormats/interface/FatJet.h
    data->fillMulti<bool>(i_ak8_recocat_istop, fatjet->recoCategory() & FatJetRecoCategory::SDMVATOP);
    data->fillMulti<bool>(i_ak8_recocat_isw,   fatjet->recoCategory() & FatJetRecoCategory::SDMVAW);
    data->fillMulti<bool>(i_ak8_gencat_istop, fatjet->genCategory() & FatJetGenCategory::GENTOP);
    data->fillMulti<bool>(i_ak8_gencat_isw,   fatjet->genCategory() & FatJetGenCategory::GENW);
    data->fillMulti<bool>(i_ak8_gencat_iscontainedtop, fatjet->genCategory() & FatJetGenCategory::GENTOP_CONTAINED);
    data->fillMulti<bool>(i_ak8_gencat_iscontainedw,   fatjet->genCategory() & FatJetGenCategory::GENW_CONTAINED);
  }


  void fillFatJetTrainInfo(TreeWriterData* data, const FatJetF *fatjet, bool matchesGen){
    bool dbg = false;
    if(dbg) std::cout << "[Sd MVA]     fillFatJetTrainInfo for fatjet pt " << fatjet->pt() << std::endl;
    assert(fatjet->nSubjets()==2); // preselected fatjet. if not, MUST fill all vars with default values, or else have vector vars of different lengths.
    data->fillMulti<bool> (i_ak8_matches_gen, matchesGen);
    //if(dbg) std::cout << "[Sd MVA]     fillFatJetTrainInfo pass cutbasedsd, ichep " << cfgSet::isSoftDropTagged(fatjet, 400, 110, 210, 0.69, 1e9) << " " << cfgSet::isSoftDropTagged(fatjet, 400, 105, 220, 0.67, 1e9, 0.46) << std::endl;
    //if(dbg) std::cout << "[Sd MVA]     fillFatJetTrainInfo pt, sd mass, tau3/2 " << fatjet->pt() << " " << fatjet->softDropMass() << " " << fatjet->tau3()/fatjet->tau2() << std::endl;
    data->fillMulti<bool >(i_ak8_passCutBasedSd,     cfgSet::isSoftDropTagged(fatjet, 400, 110, 210, 0.69, 1e9));
    data->fillMulti<bool >(i_ak8_passCutBasedICHEP,  cfgSet::isSoftDropTagged(fatjet, 400, 105, 220, 0.67, 1e9, 0.46));

    // all other vars use ONE set of code located in AnalysisTools/ObjectSelection/src/SoftdropMVA.cc
    auto varMap = SoftdropTopMVA::calcSoftdropMVAVars(fatjet, true);

    data->fillMulti<int  >(i_ak8_nsubjets,          int(varMap["ak8_nsubjets"]));
    data->fillMulti<float>(i_ak8_minsubjetpt,       varMap["ak8_minsubjetpt"]);
    data->fillMulti<float>(i_ak8_pt,                varMap["ak8_pt"]);
    data->fillMulti<float>(i_ak8_eta,               varMap["ak8_eta"]);
    data->fillMulti<float>(i_ak8_sdmass,            varMap["ak8_sdmass"]);
    data->fillMulti<float>(i_ak8_tau1,              varMap["ak8_tau1"]);
    data->fillMulti<float>(i_ak8_tau2,              varMap["ak8_tau2"]);
    data->fillMulti<float>(i_ak8_tau3,              varMap["ak8_tau3"]);
    data->fillMulti<float>(i_ak8_tau21,             varMap["ak8_tau21"]);
    data->fillMulti<float>(i_ak8_tau32,             varMap["ak8_tau32"]);
    data->fillMulti<float>(i_ak8_tau31,             varMap["ak8_tau31"]);

    // varialbes involve both subjets
    data->fillMulti<float>(i_ak8_deltaR,            varMap["ak8_deltaR"]);
    data->fillMulti<float>(i_ak8_ptDR,              varMap["ak8_ptDR"]);
    data->fillMulti<float>(i_ak8_rel_ptdiff,        varMap["ak8_rel_ptdiff"]);

    // pt ranked
    data->fillMulti<float>(i_ak8_sj1_pt,            varMap["ak8_sj1_pt"]);
    data->fillMulti<float>(i_ak8_sj1_mass,          varMap["ak8_sj1_mass"]);
    data->fillMulti<float>(i_ak8_sj1_csv,           varMap["ak8_sj1_csv"]);
    data->fillMulti<float>(i_ak8_sj1_ptD,           varMap["ak8_sj1_ptD"]);
    data->fillMulti<float>(i_ak8_sj1_axis1,         varMap["ak8_sj1_axis1"]);
    data->fillMulti<float>(i_ak8_sj1_axis2,         varMap["ak8_sj1_axis2"]);
    data->fillMulti<int>  (i_ak8_sj1_mult,          int(varMap["ak8_sj1_mult"]));

    data->fillMulti<float>(i_ak8_sj2_pt,            varMap["ak8_sj2_pt"]);
    data->fillMulti<float>(i_ak8_sj2_mass,          varMap["ak8_sj2_mass"]);
    data->fillMulti<float>(i_ak8_sj2_csv,           varMap["ak8_sj2_csv"]);
    data->fillMulti<float>(i_ak8_sj2_ptD,           varMap["ak8_sj2_ptD"]);
    data->fillMulti<float>(i_ak8_sj2_axis1,         varMap["ak8_sj2_axis1"]);
    data->fillMulti<float>(i_ak8_sj2_axis2,         varMap["ak8_sj2_axis2"]);
    data->fillMulti<int>  (i_ak8_sj2_mult,          int(varMap["ak8_sj2_mult"]));

    // sd vars
    data->fillMulti<float>(i_ak8_sd_0,              varMap["ak8_sd_0"]);
    data->fillMulti<float>(i_ak8_sd_n0p5,           varMap["ak8_sd_n0p5"]);
    data->fillMulti<float>(i_ak8_sd_n1,             varMap["ak8_sd_n1"]);
    data->fillMulti<float>(i_ak8_sd_n1p5,           varMap["ak8_sd_n1p5"]);
    data->fillMulti<float>(i_ak8_sd_n2,             varMap["ak8_sd_n2"]);


    // CSV ranked

    data->fillMulti<float>(i_ak8_csv1_pt,            varMap["ak8_csv1_pt"]);
    data->fillMulti<float>(i_ak8_csv1_mass,          varMap["ak8_csv1_mass"]);
    data->fillMulti<float>(i_ak8_csv1_csv,           varMap["ak8_csv1_csv"]);
    data->fillMulti<float>(i_ak8_csv1_ptD,           varMap["ak8_csv1_ptD"]);
    data->fillMulti<float>(i_ak8_csv1_axis1,         varMap["ak8_csv1_axis1"]);
    data->fillMulti<float>(i_ak8_csv1_axis2,         varMap["ak8_csv1_axis2"]);
    data->fillMulti<int>  (i_ak8_csv1_mult,          int(varMap["ak8_csv1_mult"]));

    data->fillMulti<float>(i_ak8_csv2_pt,            varMap["ak8_csv2_pt"]);
    data->fillMulti<float>(i_ak8_csv2_mass,          varMap["ak8_csv2_mass"]);
    data->fillMulti<float>(i_ak8_csv2_csv,           varMap["ak8_csv2_csv"]);
    data->fillMulti<float>(i_ak8_csv2_ptD,           varMap["ak8_csv2_ptD"]);
    data->fillMulti<float>(i_ak8_csv2_axis1,         varMap["ak8_csv2_axis1"]);
    data->fillMulti<float>(i_ak8_csv2_axis2,         varMap["ak8_csv2_axis2"]);
    data->fillMulti<int>  (i_ak8_csv2_mult,          int(varMap["ak8_csv2_mult"]));

/* // not used for training...
    // mass ranked

    data->fillMulti<float>(i_ak8_mass1_pt,            varMap["ak8_mass1_pt"]);
    data->fillMulti<float>(i_ak8_mass1_mass,          varMap["ak8_mass1_mass"]);
    data->fillMulti<float>(i_ak8_mass1_csv,           varMap["ak8_mass1_csv"]);
    data->fillMulti<float>(i_ak8_mass1_ptD,           varMap["ak8_mass1_ptD"]);
    data->fillMulti<float>(i_ak8_mass1_axis1,         varMap["ak8_mass1_axis1"]);
    data->fillMulti<float>(i_ak8_mass1_axis2,         varMap["ak8_mass1_axis2"]);
    data->fillMulti<int>  (i_ak8_mass1_mult,          int(varMap["ak8_mass1_mult"]));

    data->fillMulti<float>(i_ak8_mass2_pt,            varMap["ak8_mass2_pt"]);
    data->fillMulti<float>(i_ak8_mass2_mass,          varMap["ak8_mass2_mass"]);
    data->fillMulti<float>(i_ak8_mass2_csv,           varMap["ak8_mass2_csv"]);
    data->fillMulti<float>(i_ak8_mass2_ptD,           varMap["ak8_mass2_ptD"]);
    data->fillMulti<float>(i_ak8_mass2_axis1,         varMap["ak8_mass2_axis1"]);
    data->fillMulti<float>(i_ak8_mass2_axis2,         varMap["ak8_mass2_axis2"]);
    data->fillMulti<int>  (i_ak8_mass2_mult,          int(varMap["ak8_mass2_mult"]));
*/
  }

  template<class FatJet>
  void fillGenInfo(TreeWriterData* data, const PartonMatching::TopDecay *top, const vector<const PartonMatching::TopDecay*> tops, const FatJet& fatjet){
    bool dbg = false;
    if(dbg) std::cout << "[Sd MVA]     fillGenInfo. gen top?  " << (top ? top->top->pt() : 0) << std::endl;
    data->fillMulti<float>(i_ak8_gen_top_pt,  (top ? top->top->pt() : -9));
    data->fillMulti<float>(i_ak8_gen_w_pt,    (top ? top->W->pt() : -9));
    data->fillMulti<float>(i_ak8_gen_b_pt,    (top ? top->b->parton->pt() : -9));

    float mindrtq = 20., mindrqq = 20., mindrtt = 20., quarkalarmdr = 20., quarkalarmpt = -9.;
    if(dbg) std::cout << "[Sd MVA]     fillGenInfo calling calcGenInfo ----start" << std::endl;
    if(top) MVACommon::calcGenInfo(top, tops, mindrtq, mindrqq, mindrtt, quarkalarmdr, quarkalarmpt, 0.8);
    if(dbg) std::cout << "[Sd MVA]     fillGenInfo quarkalarm filling dr, pt ---end: " << quarkalarmdr << " " << quarkalarmpt << std::endl;
    data->fillMulti<float>(i_ak8_gen_mindrtt, (top ? mindrtt : -9));
    data->fillMulti<float>(i_ak8_gen_mindrtq, (top ? mindrtq : -9));
    data->fillMulti<float>(i_ak8_gen_mindrqq, (top ? mindrqq : -9));
    data->fillMulti<float>(i_ak8_gen_maxdrqfj,(top ? MVACommon::getmaxdrqfj(top, fatjet) : -9));
    data->fillMulti<float>(i_ak8_gen_quarkalarmdr, (top ? quarkalarmdr : -9));
    data->fillMulti<float>(i_ak8_gen_quarkalarmpt, (top ? quarkalarmpt : -9));
  }

  void fillGenLepInfo(TreeWriterData* data, BaseTreeAnalyzer * ana, const FatJetF *fatjet){
    // find nearby prompt gen leps
    if(!ana->isMC()) return;

    float mindr = 99.;
    GenParticleF* closestlep = 0;
    for(auto* p : ana->genParts) {
      float dr = PhysicsUtilities::deltaR(fatjet->p4(), p->p4());
      //if(dr > 0.8) continue;
      if (p->numberOfMothers()==0) continue;
      const auto *genPartMom = p->mother(0);

      bool isLep = ( ParticleInfo::isA(ParticleInfo::p_eminus, p) || 
                     ParticleInfo::isA(ParticleInfo::p_muminus, p) || 
                     ParticleInfo::isA(ParticleInfo::p_tauminus, p) );
      bool isPrompt    = ( ParticleInfo::isA(ParticleInfo::p_Z0, genPartMom) || 
                           ParticleInfo::isA(ParticleInfo::p_Wplus, genPartMom) || 
                           ParticleInfo::isA(ParticleInfo::p_tauminus, genPartMom) );

      if(!isLep || !isPrompt) continue;
      if(dr < mindr) { closestlep = p; mindr = dr; }
    }
    float genpt = -9., geneta = -9., gendr = -9.;
    int absid = -1;
    if(closestlep){
      genpt = closestlep->pt();
      geneta = closestlep->eta();
      gendr = PhysicsUtilities::deltaR(fatjet->p4(), closestlep->p4());
      absid = abs(closestlep->pdgId());
    }

    data->fillMulti<float>(i_ak8_gen_nearleppt, genpt);
    data->fillMulti<float>(i_ak8_gen_nearlepeta, geneta);
    data->fillMulti<float>(i_ak8_gen_nearlepdr, mindr);
    data->fillMulti<int  >(i_ak8_gen_nearlepid, absid);
  }

  void preparePreselectedFatjets(BaseTreeAnalyzer* ana) {
    bool dbg = false;
    if(dbg) std::cout << "[Sd MVA] Preparing preselected fatjets" << std::endl;

    preselectedFatjets.clear();
    for (const auto *fatjet : ana->fatJets){
      if(dbg) std::cout << "[Sd MVA]     sd Top has pt, sd, raw masses: " << fatjet->pt() << " " << fatjet->softDropMass() << " " <<  fatjet->p4().mass() << std::endl;
      if(dbg) std::cout << "[Sd MVA]             top/W  MVA score, isPreselected? " << fatjet->top_mva() << " " << fatjet->w_mva() << " " << SoftdropTopMVA::isPreselected(fatjet) << std::endl;
      if(SoftdropTopMVA::isPreselected(fatjet)) {
        preselectedFatjets.push_back(fatjet);
      }
    }

    //if(dbg) std::cout << "[Sd MVA]     Sorting by MVA score: " << gentops.size() << std::endl;

    // sort by descending MVA score
    //std::sort(preselectedFatjets.begin(), preselectedFatjets.end(), [&](const FatJetF *a, const FatJetF *b){ return a->mva() > b->mva(); });
  }

  void prepareGenTops(BaseTreeAnalyzer* ana){
    bool dbg = false;
    if(dbg) std::cout << "[Sd MVA] Preparing gen tops" << std::endl;
    gentops.clear();

    std::vector<GenJetF*> filteredGenJets;
    for(auto * j : ana->jets){ if(j->genJet()) filteredGenJets.push_back(j->genJet()); }
    partonEvent = new PartonMatching::PartonEvent(ana->genParticleReader,*ana->defaultJets,filteredGenJets);
    for(unsigned int i = 0 ; i < partonEvent->topDecays.size() ; i++){
      PartonMatching::TopDecay* top = &partonEvent->topDecays[i];
      if(top->isLeptonic) continue;
      gentops.push_back(top); // Note: don't condense this loop or else bugs from &X->Y[i] resolving as &X ?
    }
    if(dbg) std::cout << "[Sd MVA]   Found # gen tops: " << gentops.size() << std::endl;
  }

  void fillBasicInfo(TreeWriterData* data, BaseTreeAnalyzer* ana) {
    bool dbg = false;

    bool topProcess =  ana->process==defaults::TTBAR || ana->process==defaults::SIGNAL;
    preparePreselectedFatjets(ana);
    prepareGenTops(ana);

    //data->fill<int  >(i_nLooseSoftdropMVA,  std::count_if( preselectedFatjets.begin(), preselectedFatjets.end(), [](const FatJetF* fatjet){ return fatjet->mva() > SoftdropTopMVA::WP_LOOSE; }));
    //data->fill<int  >(i_nMediumSoftdropMVA, std::count_if( preselectedFatjets.begin(), preselectedFatjets.end(), [](const FatJetF* fatjet){ return fatjet->mva() > SoftdropTopMVA::WP_MEDIUM; }));
    //data->fill<int  >(i_nTightSoftdropMVA,  std::count_if( preselectedFatjets.begin(), preselectedFatjets.end(), [](const FatJetF* fatjet){ return fatjet->mva() > SoftdropTopMVA::WP_TIGHT; }));
    for(const auto &fatjet : preselectedFatjets) {
      const PartonMatching::TopDecay * top = (topProcess ? MVACommon::getTopCand(fatjet, gentops, 0.8, 0.8) : 0); // if not top process don't try to match
      data->fillMulti<bool >(i_basic_ak8_matches_gen, (top ? true : false));
      data->fillMulti<float>(i_basic_ak8_pt, fatjet->pt());
      data->fillMulti<float>(i_basic_ak8_eta, fatjet->eta());
      data->fillMulti<float>(i_basic_ak8_sdmass, fatjet->softDropMass());

      data->fillMulti<float>(i_basic_ak8_top_mva, fatjet->top_mva());
      data->fillMulti<float>(i_basic_ak8_w_mva, fatjet->w_mva());

    }
    delete partonEvent;
  }//fillBasicInfo

  void fillTrainInfo(TreeWriterData* data, BaseTreeAnalyzer* ana) {
    bool dbg = false;
    if(dbg) std::cout << "[Sd MVA] Filling training info" << std::endl;

    bool topProcess =  ana->process==defaults::TTBAR || ana->process==defaults::SIGNAL;
    preparePreselectedFatjets(ana);
    prepareGenTops(ana);

    //int ngenhadronictops = 0;
    //for(auto top : gentops) { if( !top->isLeptonic) ngenhadronictops++; }
    //data->fill<int>(i_ngenhadronictops, ngenhadronictops);

    // fill for EVERY preselected fatjet. first entry 0 if unmatched --> match_gen variable.
    for(const auto &fatjet : preselectedFatjets) {
      if(dbg) std::cout << "[Sd MVA]   Matching presel fj with pt     : " << fatjet->pt() << " " << std::endl;
      const PartonMatching::TopDecay * top = (topProcess ? MVACommon::getTopCand(fatjet, gentops, 0.8, 0.8) : 0); // if not top process don't try to match
      if(dbg) std::cout << "[Sd MVA]     return of getTopCand gen pt, process: " << (top ? top->top->pt() : 0) << " " << topProcess << " " << std::endl;
      //matchPairs.emplace_back(top,fatjet);
      //fillGenInfo(data, top, gentops, fatjet);
      fillGenLepInfo(data, ana, fatjet);
      fillCats(data, fatjet);
      fillFatJetTrainInfo(data, fatjet, (top ? true : false));
    }
    delete partonEvent;
  }//fillTrainInfo

};//SoftdropMVAFiller

#endif
