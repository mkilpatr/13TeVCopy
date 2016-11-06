/*
 * HTTMVA.cc
 *
 *  Created on: Nov 5, 2016
 *      Author: patterson, nmcoll, hqu
 */

#include "AnalysisTools/ObjectSelection/interface/HTTMVA.h"
#include "AnalysisTools/TreeReader/interface/Defaults.h"

using namespace ucsbsusy;

HTTMVA::HTTMVA(TString weightfile, TString trainversion, TString mvaname) :mvaReader(weightfile, mvaname) {
  initHTTMVA();
  nickVersion = trainversion.Contains("nick", TString::kIgnoreCase);
}

HTTMVA::~HTTMVA() {
}

bool HTTMVA::isPreselected(const HTTFatJetF* fatjet){
  bool isPreselected =
          fatjet->pt()>200
          && abs(fatjet->eta())<2.4
          && fatjet->softDropMass()>50
          && fatjet->nSubjets()>=3
          && fatjet->bSubjet().pt()>20 && fatjet->w1Subjet().pt()>20 && fatjet->w2Subjet().pt()>20;
  return isPreselected;
}

float HTTMVA::getHTTMVAScore(const HTTFatJetF* fatjet){

  if(!isPreselected(fatjet)) return -9.;

  auto varMap = (!nickVersion ? calcHTTMVAVars(fatjet,false) : calcHTTNickMVAVars(fatjet,false));

  for (const auto &v: varsF){
    mvaReader.setValue(v, varMap.at(v));
  }
  for (const auto &v: varsI){
    mvaReader.setValue(v, varMap.at(v));
  }
  return mvaReader.eval();
}

// give me ana->httTops and HTTMVA::WP_LOOSE or WP_TIGHT
std::vector<const HTTFatJetF*> HTTMVA::getHTTMVATops(const std::vector<const HTTFatJetF*> &fatjets, double WP) {
  std::vector<const HTTFatJetF*> HTTMVATops;
  for(const HTTFatJetF * fj : fatjets){
    if(getHTTMVAScore(fj) > WP){
      HTTMVATops.push_back(fj);
    }
  }
  return HTTMVATops;
}

void HTTMVA::initHTTMVA() {
  // variable order must match the order in XML file

  if(!nickVersion) { // alex training
    varsF = {
       "htt_sdmass",
       "htt_ropt_mass",
       "htt_ropt_tau21",
       "htt_ropt_tau32",
       "htt_ropt_tau31",
       "htt_ropt",
       "htt_ropt_deltaRopt",
       "htt_ropt_ptDR",
       "htt_ropt_frec",
       "htt_bycsv_b_pt",
       "htt_bycsv_b_squaredaxis",
       "htt_bycsv_w1_squaredaxis",
       "htt_bycsv_w2_squaredaxis",
       "htt_bycsv_w_mass",
       "htt_bycsv_w_ptDR",
    };
    varsI = {};

  }else{ // nick training
    varsF = {
       "htt_nick_subjet_pt",
       "htt_nick_tau1",
       "htt_nick_tau2",
       "htt_nick_tau3",
       "htt_nick_ropt",
       "htt_nick_subjet_mass",
       "htt_nick_subjet_w_mass",
       "htt_nick_subjet_w_pt",
       "htt_nick_b_pt",
       "htt_nick_b_csv",
       "htt_nick_b_mass",
       "htt_nick_subjet_w_bycsv_mass",
       "htt_nick_subjet_w_bycsv_pt",
       "htt_nick_subjet_b_bycsv_pt",
       "htt_nick_subjet_b_bycsv_csv",
       "htt_nick_subjet_b_bycsv_mass",
    };
    varsI = {};
  }

  mvaReader.addVariables(varsF);
  mvaReader.addVariables(varsI);
}

std::map<TString, float> HTTMVA::calcHTTMVAVars(const HTTFatJetF* fatjet, bool fullMonty) {
  std::map<TString, float> vars;

  assert(fatjet->nSubjets() > 2); // done in fatjet preselection. otherwise MUST fill below vector vars with default values or else vector vars will be of different lengths

  if(!fullMonty) { // only calculate vars necessary for evaluation. MUST INCLUDE ALL OF VARSF / VARSI ABOVE
    vars["htt_sdmass"] =         fatjet->softDropMass();
    vars["htt_ropt_mass"] =      fatjet->ropt_mom().mass();
    vars["htt_tau21"] =          fatjet->tau1() > 0 ? fatjet->tau2()/fatjet->tau1() : 1e9;
    vars["htt_tau32"] =          fatjet->tau2() > 0 ? fatjet->tau3()/fatjet->tau2() : 1e9;
    vars["htt_tau31"] =          fatjet->tau1() > 0 ? fatjet->tau3()/fatjet->tau1() : 1e9;
    vars["htt_ropt"] =           fatjet->ropt();
    vars["htt_ropt_deltaRopt"] = fatjet->ropt()-fatjet->roptcalc();
    vars["htt_ropt_ptDR"] =      fatjet->ropt()*fatjet->ropt_mom().pt();
    vars["htt_ropt_frec"] =      fatjet->frec();

    // by csv method. pt(w1) > pt(w2).
    {
      std::vector<SubJetF> subjetsByCSV(fatjet->subjets);
      std::sort(subjetsByCSV.begin(), subjetsByCSV.end(), [](const SubJetF &a, const SubJetF &b){return a.csv()>b.csv();});
      if(subjetsByCSV[1].pt()<subjetsByCSV[2].pt()) std::swap(subjetsByCSV[1], subjetsByCSV[2]);

      const auto &b_  = subjetsByCSV.at(0);
      const auto &w1_ = subjetsByCSV.at(1);
      const auto &w2_ = subjetsByCSV.at(2);
      const auto wcand = w1_.p4() + w2_.p4();
      //const auto tcand = b_.p4() + wcand;

      double var_w_deltaR = PhysicsUtilities::deltaR(w1_, w2_);
      vars["htt_bycsv_b_pt"] =            b_.pt();
      vars["htt_bydef_b_squaredaxis"] =   b_.axis1()*b_.axis1()+b_.axis2()*b_.axis2();
      vars["htt_bydef_w1_squaredaxis"] =  w1_.axis1()*w1_.axis1()+w1_.axis2()*w1_.axis2();
      vars["htt_bydef_w2_squaredaxis"] =  w2_.axis1()*w2_.axis1()+w2_.axis2()*w2_.axis2();
      vars["htt_bycsv_w_mass"] =          wcand.mass();
      vars["htt_bycsv_w_ptDR"] =          var_w_deltaR*wcand.pt();

    }//bycsvscope

    return vars;

  }else{ // fullMonty, calculate every var for training -- we're being called by an outside method.

    vars["htt_nsubjets"] =      fatjet->nSubjets();
    vars["htt_minsubjetpt"] =   TMath::Min(TMath::Min(fatjet->bSubjet().pt(),fatjet->w1Subjet().pt()),fatjet->w2Subjet().pt()) ;
    vars["htt_pt"] =            fatjet->pt();
    vars["htt_eta"] =           fatjet->eta();
    vars["htt_rawmass"] =       fatjet->p4().mass();
    vars["htt_sdmass"] =        fatjet->softDropMass();
    vars["htt_tau1"] =          fatjet->tau1();
    vars["htt_tau2"] =          fatjet->tau2();
    vars["htt_tau3"] =          fatjet->tau3();
    vars["htt_tau21"] =         fatjet->tau1() > 0 ? fatjet->tau2()/fatjet->tau1() : 1e9;
    vars["htt_tau32"] =         fatjet->tau2() > 0 ? fatjet->tau3()/fatjet->tau2() : 1e9;
    vars["htt_tau31"] =         fatjet->tau1() > 0 ? fatjet->tau3()/fatjet->tau1() : 1e9;

    vars["htt_ropt_pt"] =       fatjet->ropt_mom().pt();
    vars["htt_ropt_mass"] =     fatjet->ropt_mom().mass();
    vars["htt_ropt_tau1"] =     fatjet->ropt_tau1();
    vars["htt_ropt_tau2"] =     fatjet->ropt_tau2();
    vars["htt_ropt_tau3"] =     fatjet->ropt_tau3();
    vars["htt_ropt_tau21"] =    fatjet->ropt_tau1() > 0 ? fatjet->ropt_tau2()/fatjet->ropt_tau1() : 1e9;
    vars["htt_ropt_tau32"] =    fatjet->ropt_tau2() > 0 ? fatjet->ropt_tau3()/fatjet->ropt_tau2() : 1e9;
    vars["htt_ropt_tau31"] =    fatjet->ropt_tau1() > 0 ? fatjet->ropt_tau3()/fatjet->ropt_tau1() : 1e9;
    vars["htt_ropt"] =          fatjet->ropt();
    vars["htt_ropt_ptDR"] =     fatjet->ropt()*fatjet->ropt_mom().pt();
    vars["htt_ropt_calc"] =     fatjet->roptcalc();
    vars["htt_ropt_deltaRopt"] =     fatjet->ropt()-fatjet->roptcalc();
    vars["htt_ropt_deltaRopt_pt"] =  (fatjet->ropt()-fatjet->roptcalc()) * fatjet->ropt_mom().pt();
    vars["htt_ropt_frec"] =           fatjet->frec();

    // by default method
    {
      const auto &b_  = fatjet->bSubjet();
      const auto &w1_ = fatjet->w1Subjet();
      const auto &w2_ = fatjet->w2Subjet();
      const auto wcand = w1_.p4() + w2_.p4();
      const auto tcand = b_.p4() + wcand;

      vars["htt_bydef_b_pt"] =            b_.pt();
      vars["htt_bydef_b_mass"] =          b_.mass();
      vars["htt_bydef_b_csv"] =           (b_.csv() > 0 ? b_.csv() : 0.);
      vars["htt_bydef_b_ptD"] =           b_.ptD();
      vars["htt_bydef_b_axis1"] =         b_.axis1();
      vars["htt_bydef_b_axis2"] =         b_.axis2();
      vars["htt_bydef_b_squaredaxis"] =   b_.axis1()*b_.axis1()+b_.axis2()*b_.axis2();
      vars["htt_bydef_b_mult"] =          b_.multiplicity();

      vars["htt_bydef_w1_mass"] =         w1_.mass();
      vars["htt_bydef_w1_pt"] =           w1_.pt();
      vars["htt_bydef_w1_csv"] =          (w1_.csv() > 0 ? w1_.csv() : 0.);
      vars["htt_bydef_w1_ptD"] =          w1_.ptD();
      vars["htt_bydef_w1_axis1"] =        w1_.axis1();
      vars["htt_bydef_w1_axis2"] =        w1_.axis2();
      vars["htt_bydef_w1_squaredaxis"] =  w1_.axis1()*w1_.axis1()+w1_.axis2()*w1_.axis2();
      vars["htt_bydef_w1_mult"] =         w1_.multiplicity();

      vars["htt_bydef_w2_mass"] =         w2_.mass();
      vars["htt_bydef_w2_pt"] =           w2_.pt();
      vars["htt_bydef_w2_csv"] =          (w2_.csv() > 0 ? w2_.csv() : 0.);
      vars["htt_bydef_w2_ptD"] =          w2_.ptD();
      vars["htt_bydef_w2_axis1"] =        w2_.axis1();
      vars["htt_bydef_w2_axis2"] =        w2_.axis2();
      vars["htt_bydef_w2_squaredaxis"] =  w2_.axis1()*w2_.axis1()+w2_.axis2()*w2_.axis2();
      vars["htt_bydef_w2_mult"] =         w2_.multiplicity();

      double var_w_deltaR = PhysicsUtilities::deltaR(w1_, w2_);
      vars["htt_bydef_w_pt"] =            wcand.pt();
      vars["htt_bydef_w_mass"] =          wcand.mass();
      vars["htt_bydef_w_deltaR"] =        var_w_deltaR;
      vars["htt_bydef_w_ptDR"] =          var_w_deltaR*wcand.pt();

      double var_t_deltaR = PhysicsUtilities::deltaR(b_, wcand);
      vars["htt_bydef_t_deltaR"] =        var_t_deltaR;
      vars["htt_bydef_t_ptDR"] =          var_t_deltaR*tcand.pt();

      vars["htt_bydef_b_w1_mass"] =       (b_.p4()+w1_.p4()).mass();
      vars["htt_bydef_b_w2_mass"] =       (b_.p4()+w2_.p4()).mass();
    }

    // by csv method. pt(w1) > pt(w2).
    {
      std::vector<SubJetF> subjetsByCSV(fatjet->subjets);
      std::sort(subjetsByCSV.begin(), subjetsByCSV.end(), [](const SubJetF &a, const SubJetF &b){return a.csv()>b.csv();});
      if(subjetsByCSV[1].pt()<subjetsByCSV[2].pt()) std::swap(subjetsByCSV[1], subjetsByCSV[2]);

      const auto &b_  = subjetsByCSV.at(0);
      const auto &w1_ = subjetsByCSV.at(1);
      const auto &w2_ = subjetsByCSV.at(2);
      const auto wcand = w1_.p4() + w2_.p4();
      const auto tcand = b_.p4() + wcand;

      vars["htt_bycsv_b_pt"] =            b_.pt();
      vars["htt_bycsv_b_mass"] =          b_.mass();
      vars["htt_bycsv_b_csv"] =           (b_.csv() > 0 ? b_.csv() : 0.);
      vars["htt_bycsv_b_ptD"] =           b_.ptD();
      vars["htt_bycsv_b_axis1"] =         b_.axis1();
      vars["htt_bycsv_b_axis2"] =         b_.axis2();
      vars["htt_bycsv_b_mult"] =          b_.multiplicity();

      vars["htt_bycsv_w1_pt"] =           w1_.pt();
      vars["htt_bycsv_w1_csv"] =          (w1_.csv() > 0 ? w1_.csv() : 0.);
      vars["htt_bycsv_w1_ptD"] =          w1_.ptD();
      vars["htt_bycsv_w1_axis1"] =        w1_.axis1();
      vars["htt_bycsv_w1_axis2"] =        w1_.axis2();
      vars["htt_bycsv_w1_mult"] =         w1_.multiplicity();

      vars["htt_bycsv_w2_pt"] =           w2_.pt();
      vars["htt_bycsv_w2_csv"] =          (w2_.csv() > 0 ? w2_.csv() : 0.);
      vars["htt_bycsv_w2_ptD"] =          w2_.ptD();
      vars["htt_bycsv_w2_axis1"] =        w2_.axis1();
      vars["htt_bycsv_w2_axis2"] =        w2_.axis2();
      vars["htt_bycsv_w2_mult"] =         w2_.multiplicity();

      double var_w_deltaR = PhysicsUtilities::deltaR(w1_, w2_);
      vars["htt_bycsv_w_pt"] =            wcand.pt();
      vars["htt_bycsv_w_mass"] =          wcand.mass();
      vars["htt_bycsv_w_deltaR"] =        var_w_deltaR;
      vars["htt_bycsv_w_ptDR"] =          var_w_deltaR*wcand.pt();

      double var_t_deltaR = PhysicsUtilities::deltaR(b_, wcand);
      vars["htt_bycsv_t_deltaR"] =        var_t_deltaR;
      vars["htt_bycsv_t_ptDR"] =          var_t_deltaR*tcand.pt();

      vars["htt_bycsv_b_w1_mass"] =       (b_.p4()+w1_.p4()).mass();
      vars["htt_bycsv_b_w2_mass"] =       (b_.p4()+w2_.p4()).mass();

    }

    return vars;
  }//endif

}//calcHTTMVAVars


std::map<TString, float> HTTMVA::calcHTTNickMVAVars(const HTTFatJetF* fatjet, bool fullMonty) {
  std::map<TString, float> vars;

  assert(fatjet->nSubjets() > 2); // done in fatjet preselection. otherwise MUST fill below vector vars with default$

  vars["htt_nick_ropt"]      = fatjet->ropt();
  vars["htt_nick_tau1"]      = fatjet->tau1();
  vars["htt_nick_tau2"]      = fatjet->tau2();
  vars["htt_nick_tau3"]      = fatjet->tau3();

  ROOT::Math::LorentzVector<CylLorentzCoordF> sjMom;
  ROOT::Math::LorentzVector<CylLorentzCoordF> sjWMom;
  ROOT::Math::LorentzVector<CylLorentzCoordF> sjWMom2;
  int iBCSV = -1;
  float minPT = -9;

  for(unsigned int iS = 0; iS < fatjet->nSubjets(); ++iS){
    sjMom += fatjet->subJet(iS).p4();
    if(minPT <= 0 || fatjet->subJet(iS).pt() < minPT) minPT = fatjet->subJet(iS).pt();
  }

  sjWMom = fatjet->subJet(0).p4() + fatjet->subJet(1).p4();
  if(fatjet->subJet(0).csv() > fatjet->subJet(1).csv() && fatjet->subJet(0).csv() > fatjet->subJet(2).csv()){
    sjWMom2 = fatjet->subJet(1).p4() + fatjet->subJet(2).p4();
    iBCSV = 0;
  } else if(fatjet->subJet(1).csv() > fatjet->subJet(0).csv() && fatjet->subJet(1).csv() > fatjet->subJet(2).csv()){
    sjWMom2 = fatjet->subJet(0).p4() + fatjet->subJet(2).p4();
    iBCSV = 1;
  } else {
    sjWMom2 = fatjet->subJet(0).p4() + fatjet->subJet(1).p4();
    iBCSV = 2;
  }

  vars["htt_nick_b_pt"]                = fatjet->nSubjets() > 0 ? fatjet->subJet(2).pt()  : 0;
  vars["htt_nick_b_mass"]              = fatjet->nSubjets() > 0 ? fatjet->subJet(2).mass(): 0;
  vars["htt_nick_b_csv"]               = fatjet->nSubjets() > 0 ? (fatjet->subJet(2).csv() > 0 ? fatjet->subJet(2).csv() : 0.) : 0;
  vars["htt_nick_subjet_pt"]           = sjMom.pt();
  vars["htt_nick_subjet_mass"]         = sjMom.mass();
  vars["htt_nick_subjet_w_pt"]         = sjWMom.pt();
  vars["htt_nick_subjet_w_mass"]       = sjWMom.mass();
  vars["htt_nick_subjet_w_bycsv_pt"]   = sjWMom2.pt();
  vars["htt_nick_subjet_w_bycsv_mass"] = sjWMom2.mass();
  vars["htt_nick_subjet_b_bycsv_mass"] = iBCSV < 0 ? -9 :fatjet->subJet(iBCSV).mass();
  vars["htt_nick_subjet_b_bycsv_pt"]   = iBCSV < 0 ? -9 :fatjet->subJet(iBCSV).pt();
  vars["htt_nick_subjet_b_bycsv_csv"]  = iBCSV < 0 ? -9 :(fatjet->subJet(iBCSV).csv() > 0 ? fatjet->subJet(iBCSV).csv() : 0.);

  if(fullMonty){ // calculate the rest, we're being called by a training function
    vars["htt_nick_subjet_min_pt"]       = minPT;
    vars["htt_nick_frec"]                = fatjet->frec();
    vars["htt_nick_roptcalc"]            = fatjet->roptcalc();
    vars["htt_nick_ptforopt"]            = fatjet->ptforopt();
    vars["htt_nick_ropt_pt"]             = fatjet->ropt_mom().pt();
    vars["htt_nick_ropt_mass"]           = fatjet->ropt_mom().mass();
    vars["htt_nick_w1_pt"]               = fatjet->nSubjets() > 0 ? fatjet->subJet(0).pt()  : 0;
    vars["htt_nick_w1_mass"]             = fatjet->nSubjets() > 0 ? fatjet->subJet(0).mass(): 0;
    vars["htt_nick_w1_csv"]              = fatjet->nSubjets() > 0 ? (fatjet->subJet(0).csv() > 0 ? fatjet->subJet(0).csv() : 0.) : 0;
    vars["htt_nick_w2_pt"]               = fatjet->nSubjets() > 2 ? fatjet->subJet(1).pt()  : 0;
    vars["htt_nick_w2_mass"]             = fatjet->nSubjets() > 2 ? fatjet->subJet(1).mass(): 0;
    vars["htt_nick_w2_csv"]              = fatjet->nSubjets() > 2 ? (fatjet->subJet(1).csv() > 0 ? fatjet->subJet(1).csv() : 0.) : 0;
  }
  return vars;

}//calcHTTNickMVAVars
