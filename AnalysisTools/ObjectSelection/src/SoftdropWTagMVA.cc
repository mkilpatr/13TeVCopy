/*
 * SoftdropWTagMVA.cc
 *
 *  Created on: Nov 9, 2016
 *      Author: hqu
 */

#include "AnalysisTools/ObjectSelection/interface/SoftdropWTagMVA.h"

using namespace ucsbsusy;

SoftdropWTagMVA::SoftdropWTagMVA(TString weightfile, TString mvaname) :mvaReader(weightfile, mvaname) {
  initSoftdropWTagMVA();
}

SoftdropWTagMVA::~SoftdropWTagMVA() {
}

bool SoftdropWTagMVA::isPreselected(const FatJetF* fatjet){
  bool isPreselected =
          fatjet->pt()>200
          && fabs(fatjet->eta())<2.4
          && fatjet->softDropMass()>50
          && fatjet->nSubjets()==2
          && fatjet->subJet(0).pt()>20 && fatjet->subJet(1).pt()>20;
  return isPreselected;
}

float SoftdropWTagMVA::getSoftdropWTagMVAScore(const FatJetF* fatjet){

  if(!isPreselected(fatjet)) return -9.;

  auto varMap = calcSoftdropWTagMVAVars(fatjet,false);
  for (const auto &v: varsF){
    mvaReader.setValue(v, varMap.at(v));
  }
  for (const auto &v: varsI){
    mvaReader.setValue(v, varMap.at(v));
  }
  return mvaReader.eval();
}

void SoftdropWTagMVA::initSoftdropWTagMVA() {
  // variable order must match the order in XML file

  varsF = {
     "var_fj_sdmass",
     "var_fj_ptDR",
     "var_fj_tau21",
     "var_fj_rel_ptdiff",
     "var_sj1_ptD",
     "var_sj1_axis1",
     "var_sj2_ptD",
     "var_sj2_axis1",
     "var_sjmax_csv",
     "var_sd_n2",
  };

  varsI = {
     "var_sj1_mult",
     "var_sj2_mult",
  };

  mvaReader.addVariables(varsF);
  mvaReader.addVariables(varsI);
}

std::map<TString, float> SoftdropWTagMVA::calcSoftdropWTagMVAVars(const FatJetF* fatjet, bool fullMonty) {
  bool dbg = false;
  if(dbg) std::cout << "[ObjectSelection/SoftdropWTagMVA] calcSoftdropWTagMVAVars fatjet pt " << fatjet->pt() << std::endl;
  std::map<TString, float> vars;

  assert(fatjet->nSubjets() == 2); // done in fatjet preselection. otherwise MUST fill below vector vars with default values or else vector vars will be of different lengths

  if(!fullMonty) { // only calculate vars necessary for evaluation. MUST INCLUDE ALL OF VARSF / VARSI ABOVE

    vars["var_fj_sdmass"]   = fatjet->softDropMass();
    vars["var_fj_tau21"]    = fatjet->tau1() > 0 ? fatjet->tau2()/fatjet->tau1() : 1e9;

    // filling subjet variables
    const auto *sj1 = &fatjet->subJet(0);
    const auto *sj2 = &fatjet->subJet(1);

    double fj_deltaR = PhysicsUtilities::deltaR(*sj1, *sj2);
    vars["var_fj_ptDR"]     = fj_deltaR*fatjet->pt();
    vars["var_fj_rel_ptdiff"]= std::abs(sj1->pt()-sj2->pt())/fatjet->pt();
    vars["var_sj1_ptD"]     = sj1->ptD();
    vars["var_sj1_axis1"]   = sj1->axis1();
    vars["var_sj1_mult"]    = sj1->multiplicity();
    vars["var_sj2_ptD"]     = sj2->ptD();
    vars["var_sj2_axis1"]   = sj2->axis1();
    vars["var_sj2_mult"]    = sj2->multiplicity();
    vars["var_sjmax_csv"]   = std::max({sj1->csv(),sj2->csv(),0.f});

    double var_sd_0 = sj2->pt()/(sj1->pt()+sj2->pt());
    vars["var_sd_n2"]       = var_sd_0/std::pow(fj_deltaR,-2);

  }else{ // fullMonty, calculate every var for training -- we're being called by an outside method.

    vars["var_fj_pt"]       = fatjet->pt();
    vars["var_fj_csv"]      = fatjet->csv() >0 ? fatjet->csv() : 0;
    vars["var_fj_sdmass"]   = fatjet->softDropMass();
    vars["var_fj_tau1"]     = fatjet->tau1();
    vars["var_fj_tau2"]     = fatjet->tau2();
    vars["var_fj_tau3"]     = fatjet->tau3();
    vars["var_fj_tau21"]    = fatjet->tau1() > 0 ? fatjet->tau2()/fatjet->tau1() : 1e9;
    vars["var_fj_tau32"]    = fatjet->tau2() > 0 ? fatjet->tau3()/fatjet->tau2() : 1e9;

    const auto *sj1 = &fatjet->subJet(0);
    const auto *sj2 = &fatjet->subJet(1);

    // varialbes involve both subjets
    double fj_deltaR = PhysicsUtilities::deltaR(*sj1, *sj2);
    vars["var_fj_deltaR"]   = fj_deltaR;
    vars["var_fj_ptDR"]     = fj_deltaR*fatjet->pt();
    vars["var_fj_rel_ptdiff"]= std::abs(sj1->pt()-sj2->pt())/fatjet->pt();

    // pt ranked
    vars["var_sj1_pt"]      = sj1->pt();
    vars["var_sj1_mass"]    = sj1->mass();
    vars["var_sj1_csv"]     = sj1->csv() > 0 ? sj1->csv() : 0;
    vars["var_sj1_cmva"]    = sj1->cmva();
    vars["var_sj1_cvsl"]    = sj1->cvsl();
    vars["var_sj1_cvsb"]    = sj1->cvsb();
    vars["var_sj1_ptD"]     = sj1->ptD();
    vars["var_sj1_axis1"]   = sj1->axis1();
    vars["var_sj1_axis2"]   = sj1->axis2();
    vars["var_sj1_mult"]    = sj1->multiplicity();

    vars["var_sj2_pt"]      = sj2->pt();
    vars["var_sj2_mass"]    = sj2->mass();
    vars["var_sj2_csv"]     = sj2->csv() > 0 ? sj2->csv() : 0.;
    vars["var_sj2_cmva"]    = sj2->cmva();
    vars["var_sj2_cvsl"]    = sj2->cvsl();
    vars["var_sj2_cvsb"]    = sj2->cvsb();
    vars["var_sj2_ptD"]     = sj2->ptD();
    vars["var_sj2_axis1"]   = sj2->axis1();
    vars["var_sj2_axis2"]   = sj2->axis2();
    vars["var_sj2_mult"]    = sj2->multiplicity();

    vars["var_sjmax_csv"]   = std::max({sj1->csv(),sj2->csv(),0.f});
    vars["var_sjmax_cmva"]  = std::max(sj1->cmva(),sj2->cmva());

    vars["var_sjmin_csv"]   = std::max(std::min(sj1->csv(),sj2->csv()), 0.f);
    vars["var_sjmin_cmva"]  = std::min(sj1->cmva(),sj2->cmva());

    // sd vars
    double var_sd_0 = sj2->pt()/(sj1->pt()+sj2->pt());
    vars["var_sd_0"]        = var_sd_0;
    vars["var_sd_n0p5"]     = var_sd_0/std::pow(fj_deltaR,-0.5);
    vars["var_sd_n1"]       = var_sd_0/std::pow(fj_deltaR,-1);
    vars["var_sd_n1p5"]     = var_sd_0/std::pow(fj_deltaR,-1.5);
    vars["var_sd_n2"]       = var_sd_0/std::pow(fj_deltaR,-2);
    vars["var_sd_n2p5"]     = var_sd_0/std::pow(fj_deltaR,-2.5);
    vars["var_sd_n3"]       = var_sd_0/std::pow(fj_deltaR,-3);

  }//endif

  return vars;


}//calcSoftdropWTagMVAvars
