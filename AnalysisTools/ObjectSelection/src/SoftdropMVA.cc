/*
 * SoftdropMVA.cc
 *
 *  Created on: Nov 5, 2016
 *      Author: patterson
 */

#include "AnalysisTools/ObjectSelection/interface/SoftdropMVA.h"
#include "AnalysisTools/TreeReader/interface/Defaults.h"

using namespace ucsbsusy;


SoftdropMVA::SoftdropMVA(TString weightfile, TString mvaname) :mvaReader(weightfile, mvaname) {
  initSoftdropMVA();
}

SoftdropMVA::~SoftdropMVA() {
}

bool SoftdropMVA::isPreselected(const FatJetF* fatjet){
  bool isPreselected =
          fatjet->pt()>200
          && fabs(fatjet->eta())<2.4
          && fatjet->softDropMass()>50
          && fatjet->nSubjets()==2
          && fatjet->subJet(0).pt()>20 && fatjet->subJet(1).pt()>20;
  return isPreselected;
}
float SoftdropMVA::getSoftdropMVAScore(const FatJetF* fatjet){

  if(!isPreselected(fatjet)) return -9.;

  auto varMap = calcSoftdropMVAVars(fatjet,false);
  for (const auto &v: varsF){
    mvaReader.setValue(v, varMap.at(v));
  }
  for (const auto &v: varsI){
    mvaReader.setValue(v, varMap.at(v));
  }
  // return 0.501; // dummy before training
  return mvaReader.eval();
}

// give me ana->fatJets and SoftdropMVA::WP_LOOSE or WP_TIGHT
std::vector<const FatJetF*> SoftdropMVA::getSoftdropMVATops(const std::vector<const FatJetF*> &fatjets, double WP) {
  std::vector<const FatJetF*> SoftdropMVATops;
  for(const FatJetF * fj : fatjets){
    if(getSoftdropMVAScore(fj) > WP){
      SoftdropMVATops.push_back(fj);
    }
  }
  return SoftdropMVATops;
}

void SoftdropMVA::initSoftdropMVA() {
  // variable order must match the order in XML file

  varsF = {
     "ak8_sdmass",
     "ak8_ptDR",
     "ak8_tau21",
     "ak8_tau32",
     "ak8_rel_ptdiff",
     "ak8_csv1_mass",
     "ak8_csv1_csv",
     "ak8_csv1_ptD",
     "ak8_csv1_axis1",
     "ak8_csv2_mass",
     "ak8_csv2_ptD",
     "ak8_csv2_axis1",
  };

  varsI = {
     "ak8_csv1_mult",
     "ak8_csv2_mult",
  };

  mvaReader.addVariables(varsF);
  mvaReader.addVariables(varsI);
}

std::map<TString, float> SoftdropMVA::calcSoftdropMVAVars(const FatJetF* fatjet, bool fullMonty) {
  bool dbg = false;
  if(dbg) std::cout << "[ObjectSelection/SoftdropMVA] calcSoftdropMVAVars fatjet pt " << fatjet->pt() << std::endl;
  std::map<TString, float> vars;

  assert(fatjet->nSubjets() == 2); // done in fatjet preselection. otherwise MUST fill below vector vars with default values or else vector vars will be of different lengths

  if(!fullMonty) { // only calculate vars necessary for evaluation. MUST INCLUDE ALL OF VARSF / VARSI ABOVE

    vars["ak8_sdmass"] = fatjet->softDropMass();
    vars["ak8_tau21"] =  fatjet->tau1() > 0 ? fatjet->tau2()/fatjet->tau1() : 1e9;
    vars["ak8_tau32"] =  fatjet->tau2() > 0 ? fatjet->tau3()/fatjet->tau2() : 1e9;

    // filling subjet variables
    {
      const auto *sj1 = &fatjet->subJet(0);
      const auto *sj2 = &fatjet->subJet(1);

      double fj_deltaR = PhysicsUtilities::deltaR(*sj1, *sj2);
      vars["ak8_ptDR"] =       fj_deltaR*fatjet->pt();
      vars["ak8_rel_ptdiff"] = fabs(sj1->pt()-sj2->pt())/fatjet->pt();
      if(sj1->csv() < sj2->csv()) std::swap(sj1,sj2);
      vars["ak8_csv1_mass"] =  sj1->mass();
      vars["ak8_csv1_csv"] =   (sj1->csv() > 0 ? sj1->csv() : 0.);
      vars["ak8_csv1_ptD"] =   sj1->ptD();
      vars["ak8_csv1_axis1"] = sj1->axis1();
      vars["ak8_csv1_mult"] =  sj1->multiplicity();
      vars["ak8_csv2_mass"] =  sj2->mass();
      vars["ak8_csv2_ptD"] =   sj2->ptD();
      vars["ak8_csv2_axis1"] = sj2->axis1();
      vars["ak8_csv2_mult"] =  sj2->multiplicity();
    }
    return vars;

  }else{ // fullMonty, calculate every var for training -- we're being called by an outside method.

    if(dbg) std::cout << "[ObjectSelection/SoftdropMVA] calcSoftdropMVAVars -- fullMonty " << std::endl;
    vars["ak8_nsubjets"] = fatjet->nSubjets();
    vars["ak8_minsubjetpt"] = (fatjet->nSubjets()==2 ? TMath::Min(fatjet->subJet(0).pt(),fatjet->subJet(1).pt()) : -9.);
    vars["ak8_pt"] =     fatjet->pt();
    vars["ak8_eta"] =    fatjet->eta();
    vars["ak8_sdmass"] = fatjet->softDropMass();
    vars["ak8_tau1"] =   fatjet->tau1();
    vars["ak8_tau2"] =   fatjet->tau2();
    vars["ak8_tau3"] =   fatjet->tau3();
    vars["ak8_tau21"] =  fatjet->tau1() > 0 ? fatjet->tau2()/fatjet->tau1() : 1e9;
    vars["ak8_tau32"] =  fatjet->tau2() > 0 ? fatjet->tau3()/fatjet->tau2() : 1e9;
    vars["ak8_tau31"] =  fatjet->tau1() > 0 ? fatjet->tau3()/fatjet->tau1() : 1e9;

    // filling subjet variables
    {
      const auto *sj1 = &fatjet->subJet(0);
      const auto *sj2 = &fatjet->subJet(1);

      // varialbes involve both subjets
      double fj_deltaR = PhysicsUtilities::deltaR(*sj1, *sj2);
      vars["ak8_deltaR"] = fj_deltaR;
      vars["ak8_ptDR"] =   fj_deltaR*fatjet->pt();
      vars["ak8_rel_ptdiff"] = fabs(sj1->pt()-sj2->pt())/fatjet->pt();

      // pt ranked
      vars["ak8_sj1_pt"] =    sj1->pt();
      vars["ak8_sj1_mass"] =  sj1->mass();
      vars["ak8_sj1_csv"] =   (sj1->csv() > 0 ? sj1->csv() : 0.);
      vars["ak8_sj1_ptD"] =   sj1->ptD();
      vars["ak8_sj1_axis1"] = sj1->axis1();
      vars["ak8_sj1_axis2"] = sj1->axis2();
      vars["ak8_sj1_mult"] =  sj1->multiplicity();

      vars["ak8_sj2_pt"] =    sj2->pt();
      vars["ak8_sj2_mass"] =  sj2->mass();
      vars["ak8_sj2_csv"] =   (sj2->csv() > 0 ? sj2->csv() : 0.);
      vars["ak8_sj2_ptD"] =   sj2->ptD();
      vars["ak8_sj2_axis1"] = sj2->axis1();
      vars["ak8_sj2_axis2"] = sj2->axis2();
      vars["ak8_sj2_mult"] =  sj2->multiplicity();

      // sd vars
      double var_sd_0 = sj2->pt()/(sj1->pt()+sj2->pt());
      vars["ak8_sd_0"] =       var_sd_0;
      vars["ak8_sd_n0p5"] =    var_sd_0/std::pow(fj_deltaR, -0.5);
      vars["ak8_sd_n1"] =      var_sd_0/std::pow(fj_deltaR, -1);
      vars["ak8_sd_n1p5"] =    var_sd_0/std::pow(fj_deltaR, -1.5);
      vars["ak8_sd_n2"] =      var_sd_0/std::pow(fj_deltaR, -2);

      // CSV ranked
      if(sj1->csv() < sj2->csv()) std::swap(sj1,sj2);
      vars["ak8_csv1_pt"] =    sj1->pt();
      vars["ak8_csv1_mass"] =  sj1->mass();
      vars["ak8_csv1_csv"] =   (sj1->csv() > 0 ? sj1->csv() : 0.);
      vars["ak8_csv1_ptD"] =   sj1->ptD();
      vars["ak8_csv1_axis1"] = sj1->axis1();
      vars["ak8_csv1_axis2"] = sj1->axis2();
      vars["ak8_csv1_mult"] =  sj1->multiplicity();

      vars["ak8_csv2_pt"] =    sj2->pt();
      vars["ak8_csv2_mass"] =  sj2->mass();
      vars["ak8_csv2_csv"] =   (sj2->csv() > 0 ? sj2->csv() : 0.);
      vars["ak8_csv2_ptD"] =   sj2->ptD();
      vars["ak8_csv2_axis1"] = sj2->axis1();
      vars["ak8_csv2_axis2"] = sj2->axis2();
      vars["ak8_csv2_mult"] =  sj2->multiplicity();

      // mass ranked
      if(sj1->mass() < sj2->mass()) std::swap(sj1, sj2);
      vars["ak8_mass1_pt"] =    sj1->pt();
      vars["ak8_mass1_mass"] =  sj1->mass();
      vars["ak8_mass1_csv"] =   (sj1->csv() > 0 ? sj1->csv() : 0.);
      vars["ak8_mass1_ptD"] =   sj1->ptD();
      vars["ak8_mass1_axis1"] = sj1->axis1();
      vars["ak8_mass1_axis2"] = sj1->axis2();
      vars["ak8_mass1_mult"] =  sj1->multiplicity();

      vars["ak8_mass2_pt"] =    sj2->pt();
      vars["ak8_mass2_mass"] =  sj2->mass();
      vars["ak8_mass2_csv"] =   (sj2->csv() > 0 ? sj2->csv() : 0.);
      vars["ak8_mass2_ptD"] =   sj2->ptD();
      vars["ak8_mass2_axis1"] = sj2->axis1();
      vars["ak8_mass2_axis2"] = sj2->axis2();
      vars["ak8_mass2_mult"] =  sj2->multiplicity();
    }

    return vars;
  }//endif

}//calcSoftdropMVAvars
