/*
 * SoftdropWTopMulticlassMVA.cc
 *
 *  Created on: Nov 18, 2016
 *      Author: hqu
 */

#include "AnalysisTools/ObjectSelection/interface/SoftdropWTopMulticlassMVA.h"

using namespace ucsbsusy;

SoftdropWTopMulticlassMVA::SoftdropWTopMulticlassMVA(TString weightfile, TString mvaname) :mvaReader(weightfile, mvaname) {
  initMVA();
}

SoftdropWTopMulticlassMVA::~SoftdropWTopMulticlassMVA() {
}

bool SoftdropWTopMulticlassMVA::isPreselected(const FatJetF* fatjet){
  bool isPreselected =
          fatjet->pt()>200
          && fabs(fatjet->eta())<2.4
          && fatjet->softDropMass()>50
          && fatjet->nSubjets()==2
          && fatjet->subJet(0).pt()>10 && fatjet->subJet(1).pt()>10;
  return isPreselected;
}

std::vector<float> SoftdropWTopMulticlassMVA::getMVAScore(const FatJetF* fatjet){

  if(!isPreselected(fatjet)) return std::vector<float>{1, 0, 0};

  auto varMap = calcMVAVars(fatjet);
  for (const auto &v: varsF){
    mvaReader.setValue(v, varMap.at(v));
  }
  for (const auto &v: varsI){
    mvaReader.setValue(v, varMap.at(v));
  }
  return mvaReader.evalMulti();
}

SoftdropWTopMulticlassMVA::WTopLabel SoftdropWTopMulticlassMVA::getMVADecision(const FatJetF* fatjet){

  if(!isPreselected(fatjet)) return FAKE;

  auto varMap = calcMVAVars(fatjet);
  for (const auto &v: varsF){
    mvaReader.setValue(v, varMap.at(v));
  }
  for (const auto &v: varsI){
    mvaReader.setValue(v, varMap.at(v));
  }
  auto rlt = mvaReader.evalMulti();
  auto tag = static_cast<WTopLabel>(std::distance(rlt.begin(), std::max_element(rlt.begin(), rlt.end())));
  return tag;
}


void SoftdropWTopMulticlassMVA::initMVA() {
  // variable order must match the order in XML file

  varsF = {
      "var_fj_sdmass",
      "var_fj_tau21",
      "var_fj_tau32",
      "var_fj_rel_ptdiff",
      "var_fj_ptDR",
      "var_csv1_mass",
      "var_csv1_csv",
      "var_csv1_ptD",
      "var_csv1_axis1",
      "var_csv2_mass",
      "var_csv2_ptD",
      "var_csv2_axis1",
      "var_sd_n2",
  };

  varsI = {
      "var_csv1_mult",
      "var_csv2_mult",
  };

  mvaReader.addVariables(varsF);
  mvaReader.addVariables(varsI);
}

std::map<TString, float> SoftdropWTopMulticlassMVA::calcMVAVars(const FatJetF* fatjet) {
  bool dbg = false;
  if(dbg) std::cout << "[ObjectSelection/SoftdropWTopMulticlassMVA] calcSoftdropWTopMulticlassMVAVars fatjet pt " << fatjet->pt() << std::endl;
  std::map<TString, float> vars;

  assert(fatjet->nSubjets() == 2); // done in fatjet preselection. otherwise MUST fill below vector vars with default values or else vector vars will be of different lengths

  const auto *sj1 = &fatjet->subJet(0);
  const auto *sj2 = &fatjet->subJet(1);

  vars["var_fj_pt"]         = fatjet->pt();
  vars["var_fj_sdmass"]     = fatjet->softDropMass();
  vars["var_fj_tau21"]      = fatjet->tau1() > 0 ? fatjet->tau2()/fatjet->tau1() : 1.001;
  vars["var_fj_tau32"]      = fatjet->tau2() > 0 ? fatjet->tau3()/fatjet->tau2() : 1.001;

  // varialbes involve both subjets
  double fj_deltaR = PhysicsUtilities::deltaR(*sj1, *sj2);
  vars["var_fj_ptDR"]       = fj_deltaR*vars["var_fj_pt"];
  vars["var_fj_rel_ptdiff"] = std::abs(sj1->pt()-sj2->pt())/vars["var_fj_pt"];

  // sd vars
  double var_sd_0 = sj2->pt()/(sj1->pt()+sj2->pt());
  vars["var_sd_n2"]       = var_sd_0/std::pow(fj_deltaR,-2);

  // CSV ranked
  if(sj1->csv() < sj2->csv()) std::swap(sj1, sj2);
  vars["var_csv1_mass"]     = sj1->mass();
  vars["var_csv1_csv"]      = sj1->csv();
  vars["var_csv1_ptD"]      = sj1->ptD();
  vars["var_csv1_axis1"]    = sj1->axis1();
  vars["var_csv1_mult"]     = sj1->multiplicity();

  vars["var_csv2_mass"]     = sj2->mass();
  vars["var_csv2_ptD"]      = sj2->ptD();
  vars["var_csv2_axis1"]    = sj2->axis1();
  vars["var_csv2_mult"]     = sj2->multiplicity();

  return vars;


}//calcSoftdropWTopMulticlassMVAvars
