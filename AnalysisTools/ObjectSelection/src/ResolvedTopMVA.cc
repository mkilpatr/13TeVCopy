/*
 * ResolvedTopMVA.cc
 *
 *  Created on: Oct 6, 2016
 *      Author: hqu
 */

#include "AnalysisTools/ObjectSelection/interface/ResolvedTopMVA.h"
#include "AnalysisTools/TreeReader/interface/Defaults.h"

using namespace ucsbsusy;


ResolvedTopMVA::ResolvedTopMVA(TString weightfile, TString mvaname) :mvaReader(weightfile, mvaname) {
  initTopMVA();
}

ResolvedTopMVA::~ResolvedTopMVA() {
}

std::vector<TopCand> ResolvedTopMVA::getTopCandidates(const std::vector<RecoJetF*> &jets, double WP) {
  // need at least three jets to form a top
  if(jets.size()<3) return std::vector<TopCand>();

  // sort jets by CSV
  std::vector<RecoJetF*> csvJets(jets);
  std::sort(csvJets.begin(), csvJets.end(), [](RecoJetF *a, RecoJetF *b){return a->csv() > b->csv();});

  std::vector<TopCand> allCands;
  for (unsigned iB=0; iB<2; ++iB){
//    if(csvJets.at(iB)->csv() < defaults::CSV_LOOSE) break; // b must pass CSVL

    for (unsigned i2=0; i2<csvJets.size()-1; ++i2){
      if (i2==iB) continue;
      for (unsigned i3=i2+1; i3<csvJets.size(); ++i3){
        if (i3==iB) continue;
        TopCand tmpCand(csvJets.at(iB), csvJets.at(i2), csvJets.at(i3));
        if (tmpCand.passMassW() && tmpCand.passMassTop()){
          auto varMap = calcTopCandVars(&tmpCand);
          for (const auto &v: varsF){
            mvaReader.setValue(v, varMap.at(v));
          }
          for (const auto &v: varsI){
            mvaReader.setValue(v, varMap.at(v));
          }
          tmpCand.disc = mvaReader.eval();
          allCands.push_back(tmpCand);
        }
      }
    }
  }
  auto cands = removeOverlap(allCands, WP);
  return cands;
}

void ResolvedTopMVA::initTopMVA() {
  // variable order must match the order in XML file

  varsF = {
    "var_b_mass",
    "var_b_csv",
//      "var_b_pt",

//     "var_j2_pt",
    "var_j2_csv",
    "var_j2_cvsl",
    "var_j2_ptD",
    "var_j2_axis1",
//     "var_j2_axis2",

//     "var_j3_pt",
    "var_j3_csv",
    "var_j3_cvsl",
    "var_j3_ptD",
    "var_j3_axis1",
//     "var_j3_axis2",

    "var_topcand_mass",
    "var_topcand_ptDR",
//      "var_topcand_pt",
//      "var_b_wcand_deltaR",

    "var_wcand_mass",
    "var_wcand_ptDR",
//      "var_wcand_pt",
//      "var_wcand_deltaR",

    "var_b_j2_mass",
    "var_b_j3_mass",

    "var_sd_n2",

  };

  varsI = {
    "var_j2_mult",
    "var_j3_mult",
  };

  mvaReader.addVariables(varsF);
  mvaReader.addVariables(varsI);

}

std::map<TString, float> ResolvedTopMVA::calcTopCandVars(const TopCand* topcand) {
  std::map<TString, float> vars;

  vars["var_b_pt"]         = topcand->b->pt();
  vars["var_b_mass"]       = topcand->b->mass();
  vars["var_b_csv"]        = topcand->b->csv();
  vars["var_b_cvsb"]       = topcand->b->cvsb();
  vars["var_b_qgl"]        = topcand->b->qgl();
  vars["var_b_ptD"]        = topcand->b->ptD();
  vars["var_b_axis1"]      = topcand->b->axis1();
  vars["var_b_axis2"]      = topcand->b->axis2();
  vars["var_b_mult"]       = topcand->b->jetMult();

  vars["var_j2_pt"]        = topcand->j2->pt();
  vars["var_j2_csv"]       = topcand->j2->csv();
  vars["var_j2_cvsl"]      = topcand->j2->cvsl();
  vars["var_j2_cvsb"]      = topcand->j2->cvsb();
  vars["var_j2_qgl"]       = topcand->j2->qgl();
  vars["var_j2_ptD"]       = topcand->j2->ptD();
  vars["var_j2_axis1"]     = topcand->j2->axis1();
  vars["var_j2_axis2"]     = topcand->j2->axis2();
  vars["var_j2_mult"]      = topcand->j2->jetMult();

  vars["var_j3_pt"]        = topcand->j3->pt();
  vars["var_j3_csv"]       = topcand->j3->csv();
  vars["var_j3_cvsl"]      = topcand->j3->cvsl();
  vars["var_j3_cvsb"]      = topcand->j3->cvsb();
  vars["var_j3_qgl"]       = topcand->j3->qgl();
  vars["var_j3_ptD"]       = topcand->j3->ptD();
  vars["var_j3_axis1"]     = topcand->j3->axis1();
  vars["var_j3_axis2"]     = topcand->j3->axis2();
  vars["var_j3_mult"]      = topcand->j3->jetMult();

  vars["var_topcand_mass"] = topcand->topcand.mass();
  vars["var_topcand_pt"]   = topcand->topcand.pt();
  vars["var_b_wcand_deltaR"] = PhysicsUtilities::deltaR(*topcand->b, topcand->wcand);
  vars["var_topcand_ptDR"] = vars.at("var_topcand_pt") * vars.at("var_b_wcand_deltaR");

  vars["var_wcand_mass"]   = topcand->wcand.mass();
  vars["var_wcand_pt"]     = topcand->wcand.pt();
  vars["var_wcand_deltaR"] = PhysicsUtilities::deltaR(*topcand->j2, *topcand->j3);
  vars["var_wcand_ptDR"]   = vars.at("var_wcand_pt") * vars.at("var_wcand_deltaR");

  vars["var_sd_0"]         = topcand->j3->pt()/(topcand->j2->pt()+topcand->j3->pt());
  vars["var_sd_n1p5"]      = vars.at("var_sd_0")/std::pow(vars.at("var_wcand_deltaR"), -1.5);
  vars["var_sd_n2"]        = vars.at("var_sd_0")/std::pow(vars.at("var_wcand_deltaR"), -2);

  vars["var_b_j2_mass"]    = (topcand->b->p4() + topcand->j2->p4()).mass();
  vars["var_b_j3_mass"]    = (topcand->b->p4() + topcand->j3->p4()).mass();

//  double top_radius = -1;
//  for (const auto *j : {topcand->b, topcand->j2, topcand->j3}){
//    double dR = PhysicsUtilities::deltaR(*j, topcand->topcand);
//    if(dR > top_radius) top_radius = dR;
//  }
//  int n_extra_jets = 0;
//  for (const auto *j : jets){
//    if (PhysicsUtilities::deltaR(topcand->topcand, *j) < top_radius){
//      if (j!=topcand->b && j!=topcand->j2 && j!=topcand->j3) ++n_extra_jets;
//    }
//  }
//
//  vars["var_top_radius"]   = top_radius;
//  vars["var_n_extra_jets"] = n_extra_jets;

  return vars;

}

std::vector<TopCand> ResolvedTopMVA::removeOverlap(std::vector<TopCand>& cands, double threshold) {
  std::sort(cands.begin(), cands.end(), [](const TopCand &a, const TopCand &b){ return a.disc>b.disc; });

  std::vector<TopCand> cleanedCands;
  for (const auto &c : cands){
    if (c.disc<threshold) break;
    bool isOverlap = false;
    for (const auto &cleaned : cleanedCands){
      if(c.overlaps(cleaned)) {
        isOverlap = true; break;
      }
    }
    if(!isOverlap) cleanedCands.push_back(c);
  }

  return cleanedCands;

}
