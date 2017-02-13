/*
  updated feb 10 2017

  resolved mva tops
  treefiller for resolved mva training variables (+ some others, such as lepton contamination)

  use in ZeroLeptonTreeHelper:
    ResolvedMVAFiller resolvedMVAFiller;
    resolvedMVAFiller.bookTrain()
    resolvedMVAFiller.fillTrainInfo()

  filling of variables is PER-OBJECT for all resolved candidates (WP ALL, any pT, Ak4 are cleaned wrt merged objects)
    AnalysisTools/ObjectSelection/src/ResolvedTopMVA.cc requires (passMassW, passMassTop) range 40 GeV from W mass, 80 from Top mass

  all calculations for variables done centrally in AnalysisTools/ObjectSelection/src/ResolvedTopMVA.cc
  and for gen matching use nMatchedSubjets in AnalysisTools/ObjectSelection/interface/ResolvedTopMVA.hh
*/

#ifndef RESOLVEDMVA_HH
#define RESOLVEDMVA_HH

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
#include "AnalysisTools/ObjectSelection/interface/ResolvedTopMVA.h"
#include "MVACommon.hh"

using namespace ucsbsusy;

struct ResolvedMVAFiller {

  ResolvedMVAFiller() {}

  size i_res_eta;
  size i_res_nmatchedsubjets;
  size i_res_mva;

  size i_res_gen_b_nearleppt;
  size i_res_gen_b_nearlepeta;
  size i_res_gen_b_nearlepdr;
  size i_res_gen_b_nearlepid;

  size i_res_gen_j_nearleppt;
  size i_res_gen_j_nearlepeta;
  size i_res_gen_j_nearlepdr;
  size i_res_gen_j_nearlepid;

  size i_res_b_pt;
  size i_res_b_mass;
  size i_res_b_csv;
  size i_res_b_cvsl;
  size i_res_b_qgl;
  size i_res_b_ptD;
  size i_res_b_axis1;
  size i_res_b_axis2;
  size i_res_b_mult;

  size i_res_j2_pt;
  size i_res_j2_mass;
  size i_res_j2_csv;
  size i_res_j2_cvsl;
  size i_res_j2_qgl;
  size i_res_j2_ptD;
  size i_res_j2_axis1;
  size i_res_j2_axis2;
  size i_res_j2_mult;

  size i_res_j3_pt;
  size i_res_j3_mass;
  size i_res_j3_csv;
  size i_res_j3_cvsl;
  size i_res_j3_qgl;
  size i_res_j3_ptD;
  size i_res_j3_axis1;
  size i_res_j3_axis2;
  size i_res_j3_mult;

  size i_res_topcand_mass;
  size i_res_topcand_pt;
  size i_res_b_wcand_deltaR;
  size i_res_topcand_ptDR;

  size i_res_wcand_mass;
  size i_res_wcand_pt;
  size i_res_wcand_deltaR;
  size i_res_wcand_ptDR;

  size i_res_sd_0;
  size i_res_sd_n1p5;
  size i_res_sd_n2;

  size i_res_b_j2_mass;
  size i_res_b_j3_mass;

  void bookTrain(TreeWriterData* data) {
    i_res_eta  = data->addMulti<float>("","res_eta", -9);
    i_res_mva  = data->addMulti<float>("","res_mva",-9);
    i_res_nmatchedsubjets= data->addMulti<int>("","res_nmatchedsubjets",-1);

    i_res_gen_b_nearleppt     = data->addMulti<float>("","res_gen_b_nearleppt",-9);
    i_res_gen_b_nearlepeta    = data->addMulti<float>("","res_gen_b_nearlepeta",-9);
    i_res_gen_b_nearlepdr     = data->addMulti<float>("","res_gen_b_nearlepdr",-9);
    i_res_gen_b_nearlepid     = data->addMulti<float>("","res_gen_b_nearlepid",-9);

    i_res_gen_j_nearleppt     = data->addMulti<float>("","res_gen_j_nearleppt",-9);
    i_res_gen_j_nearlepeta    = data->addMulti<float>("","res_gen_j_nearlepeta",-9);
    i_res_gen_j_nearlepdr     = data->addMulti<float>("","res_gen_j_nearlepdr",-9);
    i_res_gen_j_nearlepid     = data->addMulti<float>("","res_gen_j_nearlepid",-9);

    i_res_b_pt      = data->addMulti<float>("","res_b_pt", -9);
    i_res_b_mass    = data->addMulti<float>("","res_b_mass", -9);
    i_res_b_csv     = data->addMulti<float>("","res_b_csv", -9);
    i_res_b_cvsl    = data->addMulti<float>("","res_b_cvsl", -9);
    i_res_b_qgl     = data->addMulti<float>("","res_b_qgl", -9);
    i_res_b_ptD     = data->addMulti<float>("","res_b_ptD", -9);
    i_res_b_axis1   = data->addMulti<float>("","res_b_axis1", -9);
    i_res_b_axis2   = data->addMulti<float>("","res_b_axis2", -9);
    i_res_b_mult    = data->addMulti<int>("","res_b_mult", -9);

    i_res_j2_pt     = data->addMulti<float>("","res_j2_pt", -9);
    i_res_j2_mass   = data->addMulti<float>("","res_j2_mass", -9);
    i_res_j2_csv    = data->addMulti<float>("","res_j2_csv", -9);
    i_res_j2_cvsl   = data->addMulti<float>("","res_j2_cvsl", -9);
    i_res_j2_qgl    = data->addMulti<float>("","res_j2_qgl", -9);
    i_res_j2_ptD    = data->addMulti<float>("","res_j2_ptD", -9);
    i_res_j2_axis1  = data->addMulti<float>("","res_j2_axis1", -9);
    i_res_j2_axis2  = data->addMulti<float>("","res_j2_axis2", -9);
    i_res_j2_mult   = data->addMulti<int>("","res_j2_mult", -9);

    i_res_j3_pt     = data->addMulti<float>("","res_j3_pt", -9);
    i_res_j3_mass   = data->addMulti<float>("","res_j3_mass", -9);
    i_res_j3_csv    = data->addMulti<float>("","res_j3_csv", -9);
    i_res_j3_cvsl   = data->addMulti<float>("","res_j3_cvsl", -9);
    i_res_j3_qgl    = data->addMulti<float>("","res_j3_qgl", -9);
    i_res_j3_ptD    = data->addMulti<float>("","res_j3_ptD", -9);
    i_res_j3_axis1  = data->addMulti<float>("","res_j3_axis1", -9);
    i_res_j3_axis2  = data->addMulti<float>("","res_j3_axis2", -9);
    i_res_j3_mult   = data->addMulti<int>("","res_j3_mult", -9);

    i_res_topcand_mass   = data->addMulti<float>("","res_topcand_mass", -9);
    i_res_topcand_pt     = data->addMulti<float>("","res_topcand_pt", -9);
    i_res_b_wcand_deltaR = data->addMulti<float>("","res_b_wcand_deltaR", -9);
    i_res_topcand_ptDR   = data->addMulti<float>("","res_topcand_ptDR", -9);

    i_res_wcand_mass     = data->addMulti<float>("","res_wcand_mass", -9);
    i_res_wcand_pt       = data->addMulti<float>("","res_wcand_pt", -9);
    i_res_wcand_deltaR   = data->addMulti<float>("","res_wcand_deltaR", -9);
    i_res_wcand_ptDR     = data->addMulti<float>("","res_wcand_ptDR", -9);

    i_res_sd_0           = data->addMulti<float>("","res_sd_0", -9);
    i_res_sd_n1p5        = data->addMulti<float>("","res_sd_n1p5", -9);
    i_res_sd_n2          = data->addMulti<float>("","res_sd_n2", -9);

    i_res_b_j2_mass      = data->addMulti<float>("","res_b_j2_mass", -9);
    i_res_b_j3_mass      = data->addMulti<float>("","res_b_j3_mass", -9);

  }

  void fillResolvedTrainInfo(TreeWriterData* data, BaseTreeAnalyzer * ana, const TopCand* topcand){
    bool dbg = false;
    if(dbg) std::cout << "[res]     fillFatJetTrainInfo for cand pt " << topcand->topcand.pt() << std::endl;

    data->fillMulti<float>(i_res_mva,            topcand->disc);
    data->fillMulti<float>(i_res_eta,            topcand->topcand.eta());
    data->fillMulti<int>(i_res_nmatchedsubjets,  topcand->nMatchedSubjets(ana->hadronicGenTops));

    auto varMap = ResolvedTopMVA::calcTopCandVars(topcand);

    data->fillMulti<float>(i_res_b_pt,            varMap["var_b_pt"]);
    data->fillMulti<float>(i_res_b_mass,          varMap["var_b_mass"]);
    data->fillMulti<float>(i_res_b_csv,           varMap["var_b_csv"]);
    data->fillMulti<float>(i_res_b_cvsl,          varMap["var_b_cvsl"]);
    data->fillMulti<float>(i_res_b_qgl,           varMap["var_b_qgl"]);
    data->fillMulti<float>(i_res_b_ptD,           varMap["var_b_ptD"]);
    data->fillMulti<float>(i_res_b_axis1,         varMap["var_b_axis1"]);
    data->fillMulti<float>(i_res_b_axis2,         varMap["var_b_axis2"]);
    data->fillMulti<int>(i_res_b_mult,            varMap["var_b_mult"]);

    data->fillMulti<float>(i_res_j2_pt,            varMap["var_j2_pt"]);
    data->fillMulti<float>(i_res_j2_mass,          varMap["var_j2_mass"]);
    data->fillMulti<float>(i_res_j2_csv,           varMap["var_j2_csv"]);
    data->fillMulti<float>(i_res_j2_cvsl,          varMap["var_j2_cvsl"]);
    data->fillMulti<float>(i_res_j2_qgl,           varMap["var_j2_qgl"]);
    data->fillMulti<float>(i_res_j2_ptD,           varMap["var_j2_ptD"]);
    data->fillMulti<float>(i_res_j2_axis1,         varMap["var_j2_axis1"]);
    data->fillMulti<float>(i_res_j2_axis2,         varMap["var_j2_axis2"]);
    data->fillMulti<int>(i_res_j2_mult,            varMap["var_j2_mult"]);

    data->fillMulti<float>(i_res_j3_pt,            varMap["var_j3_pt"]);
    data->fillMulti<float>(i_res_j3_mass,          varMap["var_j3_mass"]);
    data->fillMulti<float>(i_res_j3_csv,           varMap["var_j3_csv"]);
    data->fillMulti<float>(i_res_j3_cvsl,          varMap["var_j3_cvsl"]);
    data->fillMulti<float>(i_res_j3_qgl,           varMap["var_j3_qgl"]);
    data->fillMulti<float>(i_res_j3_ptD,           varMap["var_j3_ptD"]);
    data->fillMulti<float>(i_res_j3_axis1,         varMap["var_j3_axis1"]);
    data->fillMulti<float>(i_res_j3_axis2,         varMap["var_j3_axis2"]);
    data->fillMulti<int>(i_res_j3_mult,            varMap["var_j3_mult"]);

    data->fillMulti<float>(i_res_topcand_mass,     varMap["var_topcand_mass"]);
    data->fillMulti<float>(i_res_topcand_pt,       varMap["var_topcand_pt"]);
    data->fillMulti<float>(i_res_b_wcand_deltaR,   varMap["var_b_wcand_deltaR"]);
    data->fillMulti<float>(i_res_topcand_ptDR,     varMap["var_topcand_ptDR"]);

    data->fillMulti<float>(i_res_wcand_mass,       varMap["var_wcand_mass"]);
    data->fillMulti<float>(i_res_wcand_pt,         varMap["var_wcand_pt"]);
    data->fillMulti<float>(i_res_wcand_deltaR,     varMap["var_wcand_deltaR"]);
    data->fillMulti<float>(i_res_wcand_ptDR,       varMap["var_wcand_ptDR"]);

    data->fillMulti<float>(i_res_sd_0,             varMap["var_sd_0"]);
    data->fillMulti<float>(i_res_sd_n1p5,          varMap["var_sd_n1p5"]);
    data->fillMulti<float>(i_res_sd_n2,            varMap["var_sd_n2"]);

    data->fillMulti<float>(i_res_b_j2_mass,        varMap["var_b_j2_mass"]);
    data->fillMulti<float>(i_res_b_j3_mass,        varMap["var_b_j3_mass"]);

  }

  template<class Jet>
  void findGenLep(BaseTreeAnalyzer* ana, const Jet * jet, float &genpt, float &geneta, float &gendr, int &absid){
    genpt = -9, geneta = -9, gendr = -9, absid=-9;
    if(!jet) return;

    float mindr = 99.;
    GenParticleF* closestlep = 0;
    for(auto* p : ana->genParts) {
      float dr = PhysicsUtilities::deltaR(jet->p4(), p->p4());
      //if(dr > 0.8) continue; // close leps
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

    if(closestlep){
      genpt = closestlep->pt();
      geneta = closestlep->eta();
      gendr = PhysicsUtilities::deltaR(jet->p4(), closestlep->p4());
      absid = abs(closestlep->pdgId());
    }

  }


  void fillGenLepInfo(TreeWriterData* data, BaseTreeAnalyzer* ana,const TopCand * topcand){
    // find nearby prompt gen leps
    if(!ana->isMC()) return;

    // nearest lep to b
    float genpt, geneta, gendr;
    int absid;
    findGenLep(ana, topcand->b, genpt, geneta, gendr, absid);
    data->fillMulti<float>(i_res_gen_b_nearleppt,genpt);
    data->fillMulti<float>(i_res_gen_b_nearlepeta,geneta);
    data->fillMulti<float>(i_res_gen_b_nearlepdr,gendr);
    data->fillMulti<float>(i_res_gen_b_nearlepid,absid);

    genpt = -9, geneta = -9, gendr = -9, absid = -9;
    // nearest leps to j2,j3
    float j2genpt, j2geneta, j2gendr;
    int j2absid;
    float j3genpt, j3geneta, j3gendr;
    int j3absid;
    findGenLep(ana, topcand->j2, j2genpt, j2geneta, j2gendr, j2absid);
    findGenLep(ana, topcand->j3, j3genpt, j3geneta, j3gendr, j3absid);

    // for j2,j3 take closest lep to either
    genpt = (j2gendr < j3gendr) ? j2genpt : j3genpt;
    geneta = (j2gendr < j3gendr) ? j2geneta : j3geneta;
    gendr = (j2gendr < j3gendr) ? j2gendr : j3gendr;
    absid = (j2gendr < j3gendr) ? j2absid : j3absid;
    data->fillMulti<float>(i_res_gen_j_nearleppt, genpt);
    data->fillMulti<float>(i_res_gen_j_nearlepeta,geneta);
    data->fillMulti<float>(i_res_gen_j_nearlepdr, gendr);
    data->fillMulti<float>(i_res_gen_j_nearlepid, absid);
  }

  void fillTrainInfo(TreeWriterData* data, BaseTreeAnalyzer* ana) {
    bool dbg = false;
    if(dbg) std::cout << "[res] Filling training info" << std::endl;

    // fill: tag status, pt, eta, mass, n matched subjets, gen lep info, train variables fxn

    for (auto &cand : ana->resMVATopCands) {
      TopCand * tcand = &cand;
      fillGenLepInfo(data, ana, tcand);
      fillResolvedTrainInfo(data, ana,tcand);
    }

  }//fillTrainInfo

};

#endif
