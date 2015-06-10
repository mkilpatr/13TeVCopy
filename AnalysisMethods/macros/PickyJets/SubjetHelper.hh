#ifndef SUBJETHELPER_HH
#define SUBJETHELPER_HH
#include "TopDecayHelper.hh"
#include "AnalysisTools/TreeReader/interface/TreeReader.h"
#include "AnalysisTools/Utilities/interface/TreeWriterData.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"

using namespace ucsbsusy;
using namespace topdecays;

struct SubjetTreeLoader {

  enum SplitAnalysis {EMPTY_SPLIT,
                      STOPPED_SPLIT,
                      SINGLE_SPLIT_TO_FIRST,
                      SINGLE_SPLIT_TO_FIRST_FAILS_PTCUT,
                      SINGLE_SPLIT_TO_SECOND,
                      SINGLE_SPLIT_TO_SECOND_FAILS_PTCUT,
                      SINGLE_DONOT_SPLIT,
                      MULTI_SPLIT_TO_BOTH,
                      MULTI_SPLIT_TO_FIRST,
                      MULTI_SPLIT_TO_SECOND,
                      MULTI_GOOD_CONTAINMENT_IN_BOTH,
                      MULTI_GOOD_CONTAINMENT_IN_BOTH_LOST_PARTON_CONTENT,
                      MULTI_GOOD_CONTAINMENT_IN_FIRST,
                      MULTI_GOOD_CONTAINMENT_IN_FIRST_LOST_PARTON_CONTENT_TO_SECOND,
                      MULTI_GOOD_CONTAINMENT_IN_FIRST_LOST_PARTON_CONTENT,
                      MULTI_GOOD_CONTAINMENT_IN_SECOND,
                      MULTI_GOOD_CONTAINMENT_IN_SECOND_LOST_PARTON_CONTENT_TO_FIRST,
                      MULTI_GOOD_CONTAINMENT_IN_SECOND_LOST_PARTON_CONTENT,
                      LOST_PARTONS,
                      UNDEFINED};

  SubjetTreeLoader() {
    gentop_pt = 0;
    gentop_eta = 0;
    gentop_phi = 0;
    gentop_mass = 0;
    parton_topindex = 0;
    parton_pt = 0;
    parton_eta = 0;
    parton_phi = 0;
    parton_energy = 0;
    parton_hade = 0;
    superJet_prtIndex = 0;
    superJet_purity = 0;
    subJet_1_prtIndex = 0;
    subJet_1_purity = 0;
    subJet_2_prtIndex = 0;
    subJet_2_purity = 0;
    partons.reserve(6);
    tops.reserve(2);
    superJet = 0;
    subJet1 = 0;
    subJet2 = 0;
    splitAnalysis = UNDEFINED;
    shouldStopSplitting = false;
  }

  vector<AssocParton*>    partons;
  vector<TopPartonAssoc*> tops;
  AssocJet*               superJet;
  AssocJet*               subJet1;
  AssocJet*               subJet2;
  SplitAnalysis           splitAnalysis;
  bool                    shouldStopSplitting;
  vector<int>             reiterations;

  unsigned int  run;
  unsigned int  lumi;
  unsigned int  event;
  size8         nVert;
  size8         nGoodPartons;
  bool          isGen;
  vector<float> * gentop_pt;
  vector<float> * gentop_eta;
  vector<float> * gentop_phi;
  vector<float> * gentop_mass;
  vector<int  > * parton_topindex;
  vector<float> * parton_pt;
  vector<float> * parton_eta;
  vector<float> * parton_phi;
  vector<float> * parton_energy;
  vector<float> * parton_hade;
  size8         splitResult;
  size8         numSplits;
  size8         splitDecision;
  float         superJet_pt;
  float         superJet_eta;
  float         superJet_phi;
  float         superJet_mass;
  float         superJet_gene;
  vector<int  > * superJet_prtIndex;
  vector<float> * superJet_purity;
  float         subJet_1_pt;
  float         subJet_1_eta;
  float         subJet_1_phi;
  float         subJet_1_mass;
  float         subJet_1_gene;
  vector<int  > * subJet_1_prtIndex;
  vector<float> * subJet_1_purity;
  float         subJet_2_pt;
  float         subJet_2_eta;
  float         subJet_2_phi;
  float         subJet_2_mass;
  float         subJet_2_gene;
  vector<int  > * subJet_2_prtIndex;
  vector<float> * subJet_2_purity;

  void load(TreeReader* reader) {
    reader->setBranchAddress("","run",			&run);
    reader->setBranchAddress("","lumi",			&lumi);
    reader->setBranchAddress("","event",		&event);
    reader->setBranchAddress("","top_pt",		&gentop_pt);
    reader->setBranchAddress("","top_eta",		&gentop_eta);
    reader->setBranchAddress("","top_phi",		&gentop_phi);
    reader->setBranchAddress("","top_mass",		&gentop_mass);
    reader->setBranchAddress("","parton_topindex",	&parton_topindex);
    reader->setBranchAddress("","parton_pt",		&parton_pt);
    reader->setBranchAddress("","parton_eta",		&parton_eta);
    reader->setBranchAddress("","parton_phi",		&parton_phi);
    reader->setBranchAddress("","parton_energy",	&parton_energy);
    reader->setBranchAddress("","parton_hade",		&parton_hade);
    reader->setBranchAddress("","nVert",		&nVert);
    reader->setBranchAddress("","nGoodPartons",		&nGoodPartons);
    reader->setBranchAddress("","isGen",		&isGen);
    reader->setBranchAddress("","splitResult",		&splitResult);
    reader->setBranchAddress("","numSplits",		&numSplits);
    reader->setBranchAddress("","superJet_pt",		&superJet_pt);
    reader->setBranchAddress("","superJet_eta",		&superJet_eta);
    reader->setBranchAddress("","superJet_phi",		&superJet_phi);
    reader->setBranchAddress("","superJet_mass",	&superJet_mass);
    reader->setBranchAddress("","superJet_gene",	&superJet_gene);
    reader->setBranchAddress("","superJet_prtIndex",	&superJet_prtIndex);
    reader->setBranchAddress("","superJet_purity",	&superJet_purity);
    reader->setBranchAddress("","subJet_1_pt",		&subJet_1_pt);
    reader->setBranchAddress("","subJet_1_eta",		&subJet_1_eta);
    reader->setBranchAddress("","subJet_1_phi",		&subJet_1_phi);
    reader->setBranchAddress("","subJet_1_mass",	&subJet_1_mass);
    reader->setBranchAddress("","subJet_1_gene",	&subJet_1_gene);
    reader->setBranchAddress("","subJet_1_prtIndex",	&subJet_1_prtIndex);
    reader->setBranchAddress("","subJet_1_purity",	&subJet_1_purity);
    reader->setBranchAddress("","subJet_2_pt",		&subJet_2_pt);
    reader->setBranchAddress("","subJet_2_eta",		&subJet_2_eta);
    reader->setBranchAddress("","subJet_2_phi",		&subJet_2_phi);
    reader->setBranchAddress("","subJet_2_mass",	&subJet_2_mass);
    reader->setBranchAddress("","subJet_2_gene",	&subJet_2_gene);
    reader->setBranchAddress("","subJet_2_prtIndex",	&subJet_2_prtIndex);
    reader->setBranchAddress("","subJet_2_purity",	&subJet_2_purity);
    reader->setBranchAddress("","splitDecision",	&splitDecision);
  }

  void refresh() {
    partons.clear();
    tops.clear();

    assert(gentop_pt->size() == 2);
    tops.push_back(new TopPartonAssoc(gentop_pt->at(0), gentop_eta->at(0), gentop_phi->at(0), gentop_mass->at(0)));
    tops.push_back(new TopPartonAssoc(gentop_pt->at(1), gentop_eta->at(1), gentop_phi->at(1), gentop_mass->at(1)));

    for(unsigned int ip = 0; ip < parton_pt->size(); ++ip)
      partons.push_back(new AssocParton(parton_pt->at(ip), parton_eta->at(ip), parton_phi->at(ip), parton_energy->at(ip), parton_topindex->at(ip)));

    refreshJets();

  }

  void refreshJets() {
    if(numSplits == 0) {
      shouldStopSplitting = false;
      reiterations.clear();
    }
    superJet = new AssocJet(superJet_pt, superJet_eta, superJet_phi, superJet_mass, superJet_gene, int(numSplits));
    superJet->attachPartons(partons);
    for(unsigned int ip = 0; ip < superJet_prtIndex->size(); ++ip)
      if(superJet_purity->at(ip)*superJet_gene > minpartonrele*partons[superJet_prtIndex->at(ip)]->energy)
        superJet->setPurity(superJet_prtIndex->at(ip), superJet_purity->at(ip));

    subJet1 = new AssocJet(subJet_1_pt, subJet_1_eta, subJet_1_phi, subJet_1_mass, subJet_1_gene);
    subJet1->attachPartons(partons);
    for(unsigned int ip = 0; ip < subJet_1_prtIndex->size(); ++ip)
      if(subJet_1_purity->at(ip)*subJet_1_gene > minpartonrele*partons[subJet_1_prtIndex->at(ip)]->energy)
        subJet1->setPurity(subJet_1_prtIndex->at(ip), subJet_1_purity->at(ip));

    subJet2 = new AssocJet(subJet_2_pt, subJet_2_eta, subJet_2_phi, subJet_2_mass, subJet_2_gene);
    subJet2->attachPartons(partons);
    for(unsigned int ip = 0; ip < subJet_2_prtIndex->size(); ++ip)
      if(subJet_2_purity->at(ip)*subJet_2_gene > minpartonrele*partons[subJet_2_prtIndex->at(ip)]->energy)
        subJet2->setPurity(subJet_2_prtIndex->at(ip), subJet_2_purity->at(ip));
  }

  float subJetDR() const { return PhysicsUtilities::deltaR(subJet_1_eta, subJet_1_phi, subJet_2_eta, subJet_2_phi); }

  const char* printSplitAnalysis() {
    switch(splitAnalysis) {
      case EMPTY_SPLIT                        : { return "empty split"; break; }
      case STOPPED_SPLIT                      : { return "stopped splitting on a previous iteration"; break; }
      case SINGLE_SPLIT_TO_FIRST              : { return "single parton, split to first subjet"; break; }
      case SINGLE_SPLIT_TO_FIRST_FAILS_PTCUT  : { return "single parton, should split to first subjet but it's below threshold"; break; }
      case SINGLE_SPLIT_TO_SECOND             : { return "single parton, split to second subjet"; break; }
      case SINGLE_SPLIT_TO_SECOND_FAILS_PTCUT : { return "single parton, should split to second subjet but it's below threshold"; break; }
      case SINGLE_DONOT_SPLIT                 : { return "single parton, don't split"; break; }
      case MULTI_SPLIT_TO_BOTH                : { return "multiple partons, split to both subjets"; break; }
      case MULTI_SPLIT_TO_FIRST               : { return "multiple partons, split to first subjet"; break; }
      case MULTI_SPLIT_TO_SECOND              : { return "multiple partons, split to second subjet"; break; }
      case MULTI_GOOD_CONTAINMENT_IN_BOTH     : { return "multiple partons, good fraction of energy kept in first and second subjet for all partons"; break; }
      case MULTI_GOOD_CONTAINMENT_IN_BOTH_LOST_PARTON_CONTENT   : { return "multiple partons, good fraction of energy kept in first and second subjet but lost some parton content"; break; }
      case MULTI_GOOD_CONTAINMENT_IN_FIRST    : { return "multiple partons, good fraction of energy kept in first subjet for all partons"; break; }
      case MULTI_GOOD_CONTAINMENT_IN_FIRST_LOST_PARTON_CONTENT_TO_SECOND  : { return "multiple partons, good fraction of energy kept in first subjet for some partons but lost significant parton content to second subjet"; break; }
      case MULTI_GOOD_CONTAINMENT_IN_FIRST_LOST_PARTON_CONTENT  : { return "multiple partons, good fraction of energy kept in first subjet for some partons but lost some parton content"; break; }
      case MULTI_GOOD_CONTAINMENT_IN_SECOND   : { return "multiple partons, good fraction of energy kept in second subjet for all partons"; break; }
      case MULTI_GOOD_CONTAINMENT_IN_SECOND_LOST_PARTON_CONTENT_TO_FIRST  : { return "multiple partons, good fraction of energy kept in second subjet for some partons but lost significant parton content to first subjet"; break; }
      case MULTI_GOOD_CONTAINMENT_IN_SECOND_LOST_PARTON_CONTENT : { return "multiple partons, good fraction of energy kept in second subjet for some partons but lost some parton content"; break; }
      case LOST_PARTONS : { return "lost partons! where did they go?"; break; }
      case UNDEFINED    : { return "split analysis undefined"; break; }
      default           : { return "unknown"; break; }
    }
  }

  void analyzeSplit() {
    auto index = find(reiterations.begin(), reiterations.end(), int(numSplits));
    if(index != reiterations.end()) {
      shouldStopSplitting = false;
      reiterations.erase(index);
    }

    if(shouldStopSplitting) {
      splitAnalysis = STOPPED_SPLIT;
      return;
    }

    if(!superJet || !subJet1 || !subJet2) {
      splitAnalysis = UNDEFINED;
      return;
    }

    if(superJet->isEmpty()) {
      splitAnalysis = EMPTY_SPLIT;
      shouldStopSplitting = true;
      return;
    }

    unsigned int goodsplit[2]      = {0,0};
    unsigned int containssigpartenergy[2] = {0,0};
    unsigned int keptgoodenergy[2] = {0,0};
    unsigned int keptfullenergy[2] = {0,0};
    for(auto jp : superJet->purity) {
      bool goodSplitToFirst = false, goodSplitToSecond = false;
      bool sigEnergyInFirst = false, sigEnergyInSecond = false;
      bool goodEnergyInFirst = false, goodEnergyInSecond = false;
      bool fullEnergyInFirst = false, fullEnergyInSecond = false;
      for(auto sjp : subJet1->purity) {
        if(sjp.second != jp.second) continue;
        if(sjp.first*subJet1->genE > minjetretainment*jp.first*superJet->genE) {
          goodEnergyInFirst = true;
          if(sjp.first*subJet1->genE == jp.first*superJet->genE) fullEnergyInFirst = true;
          if(sjp.first >= jp.first) goodSplitToFirst = true;
        } else if(sjp.first*subJet1->genE > minsigpartonrele*partons[sjp.second]->energy) {
          sigEnergyInFirst = true;
        }
      }
      for(auto sjp : subJet2->purity) {
        if(sjp.second != jp.second) continue;
        if(sjp.first*subJet2->genE > minjetretainment*jp.first*superJet->genE) {
          goodEnergyInSecond = true;
          if(sjp.first*subJet2->genE == jp.first*superJet->genE) fullEnergyInSecond = true;
          if(sjp.first >= jp.first) goodSplitToSecond = true;
        } else if(sjp.first*subJet2->genE > minsigpartonrele*partons[sjp.second]->energy) {
          sigEnergyInSecond = true;
        }
      }
      if(goodSplitToFirst) goodsplit[0]++;
      else if(goodSplitToSecond) goodsplit[1]++;
      if(goodEnergyInFirst) keptgoodenergy[0]++;
      else if(goodEnergyInSecond) keptgoodenergy[1]++;
      if(fullEnergyInFirst) keptfullenergy[0]++;
      else if(fullEnergyInSecond) keptfullenergy[1]++;
      if(sigEnergyInFirst)  containssigpartenergy[0]++;
      if(sigEnergyInSecond)  containssigpartenergy[1]++;
    }

    if(superJet->isSingle()) {
      if(goodsplit[0] && keptfullenergy[0]) {
        if(subJet1->pt() > ptcut) {
          reiterations.push_back(int(numSplits)+1);
          splitAnalysis = SINGLE_SPLIT_TO_FIRST;
        } else {
          splitAnalysis = SINGLE_SPLIT_TO_FIRST_FAILS_PTCUT;
          shouldStopSplitting = true;
        }
      }
      else if(goodsplit[1] && keptfullenergy[1]) {
        if(subJet2->pt() > ptcut) {
          reiterations.push_back(int(numSplits)+1);
          splitAnalysis = SINGLE_SPLIT_TO_SECOND;
        } else {
          splitAnalysis = SINGLE_SPLIT_TO_SECOND_FAILS_PTCUT;
          shouldStopSplitting = true;
        }
      }
      else {
        splitAnalysis = SINGLE_DONOT_SPLIT;
        shouldStopSplitting = true;
      }
    }
    else if(superJet->isMulti()) {
      if(goodsplit[0] && goodsplit[1] && (goodsplit[0] + goodsplit[1]) == superJet->purity.size()) {
        splitAnalysis = MULTI_SPLIT_TO_BOTH;
        if(subJet1->pt() > ptcut) reiterations.push_back(int(numSplits)+1);
        if(subJet2->pt() > ptcut) reiterations.push_back(int(numSplits)+1);
      }
      else if(goodsplit[0] == superJet->purity.size()) {
        splitAnalysis = MULTI_SPLIT_TO_FIRST;
        if(subJet1->pt() > ptcut) reiterations.push_back(int(numSplits)+1);
      }
      else if(goodsplit[1] == superJet->purity.size()) {
        splitAnalysis = MULTI_SPLIT_TO_SECOND;
        if(subJet2->pt() > ptcut) reiterations.push_back(int(numSplits)+1);
      }
      else if(keptgoodenergy[0] && keptgoodenergy[1] && (keptgoodenergy[0] + keptgoodenergy[1]) >= superJet->purity.size()) {
        splitAnalysis = MULTI_GOOD_CONTAINMENT_IN_BOTH;
        if(subJet1->pt() > ptcut) reiterations.push_back(int(numSplits)+1);
        if(subJet2->pt() > ptcut) reiterations.push_back(int(numSplits)+1);
      }
      else if(keptgoodenergy[0] && keptgoodenergy[1]) {
        splitAnalysis = MULTI_GOOD_CONTAINMENT_IN_BOTH_LOST_PARTON_CONTENT;
        if(subJet1->pt() > ptcut) reiterations.push_back(int(numSplits)+1);
        if(subJet2->pt() > ptcut) reiterations.push_back(int(numSplits)+1);
      }
      else if(keptgoodenergy[0] >= superJet->purity.size()) {
        splitAnalysis = MULTI_GOOD_CONTAINMENT_IN_FIRST;
        if(subJet1->pt() > ptcut) reiterations.push_back(int(numSplits)+1);
      }
      else if(keptgoodenergy[0] && containssigpartenergy[1] && (keptgoodenergy[0] + containssigpartenergy[1]) >= superJet->purity.size()) {
        splitAnalysis = MULTI_GOOD_CONTAINMENT_IN_FIRST_LOST_PARTON_CONTENT_TO_SECOND;
        if(subJet1->pt() > ptcut) reiterations.push_back(int(numSplits)+1);
        if(subJet2->pt() > ptcut) reiterations.push_back(int(numSplits)+1);
      }
      else if(keptgoodenergy[0]) {
        splitAnalysis = MULTI_GOOD_CONTAINMENT_IN_FIRST_LOST_PARTON_CONTENT;
        if(subJet1->pt() > ptcut) reiterations.push_back(int(numSplits)+1);
      }
      else if(keptgoodenergy[1] >= superJet->purity.size()) {
        splitAnalysis = MULTI_GOOD_CONTAINMENT_IN_SECOND;
        if(subJet2->pt() > ptcut) reiterations.push_back(int(numSplits)+1);
      }
      else if(keptgoodenergy[1] && containssigpartenergy[0] && (keptgoodenergy[1] + containssigpartenergy[0]) >= superJet->purity.size()) {
        splitAnalysis = MULTI_GOOD_CONTAINMENT_IN_SECOND_LOST_PARTON_CONTENT_TO_FIRST;
        if(subJet1->pt() > ptcut) reiterations.push_back(int(numSplits)+1);
        if(subJet2->pt() > ptcut) reiterations.push_back(int(numSplits)+1);
      }
      else if(keptgoodenergy[1]) {
        splitAnalysis = MULTI_GOOD_CONTAINMENT_IN_SECOND_LOST_PARTON_CONTENT;
        if(subJet2->pt() > ptcut) reiterations.push_back(int(numSplits)+1);
      }
      else splitAnalysis = LOST_PARTONS;
    }

  }

};

struct SubjetTreeFiller {

  SubjetTreeFiller() {}

  size i_subjet_dr;
  size i_fatjet_pt;
  size i_fatjet_eta;
  size i_fatjet_phi;
  size i_fatjet_mass;
  size i_fatjet_purity;
  size i_goodsplit;
  size i_splitAnalysis;
  size i_newSplitDecision;

  vector<size> i_ngoodparts;
  vector<size> i_nassocparts;
  vector<size> i_nsplitparts;
  vector<size> i_nmergedparts;
  vector<size> i_nresolvedparts;
  vector<size> i_top_isgood;
  vector<size> i_top_isassoc;
  vector<size> i_top_isnotassoc;
  vector<size> i_top_isnosplit;
  vector<size> i_top_issplit;
  vector<size> i_top_ismerged;
  vector<size> i_top_iscontaminated;
  vector<size> i_top_isresolved;
  vector<size> i_top_pt;
  vector<size> i_goodtop_pt;
  vector<size> i_assoctop_pt;
  vector<size> i_notassoctop_pt;
  vector<size> i_nosplittop_pt;
  vector<size> i_splittop_pt;
  vector<size> i_mergedtop_pt;
  vector<size> i_contaminatedtop_pt;
  vector<size> i_resolvedtop_pt;
  vector<size> i_jet_pt;
  vector<size> i_jet_assocparton_pt;
  vector<size> i_jet_eta;
  vector<size> i_jet_phi;
  vector<size> i_jet_mass;
  vector<size> i_jet_nsplits;
  vector<size> i_jet_purity;
  vector<size> i_jet_fatjet_pt;
  vector<size> i_jet_fatjet_eta;
  vector<size> i_jet_fatjet_phi;
  vector<size> i_jet_fatjet_mass;

  void bookJets(TreeWriterData* data) {
    i_subjet_dr 	= data->add<float>("","subJet_dr","F",0);
    i_fatjet_pt 	= data->add<float>("","fatJet_pt","F",0);
    i_fatjet_eta 	= data->add<float>("","fatJet_eta","F",0);
    i_fatjet_phi 	= data->add<float>("","fatJet_phi","F",0);
    i_fatjet_mass 	= data->add<float>("","fatJet_mass","F",0);
    i_fatjet_purity 	= data->add<float>("","fatJet_purity","F",0);
    i_goodsplit 	= data->add<bool >("","goodSplit","O",false);
    i_splitAnalysis 	= data->add<unsigned int>("","splitAnalysis","i",0);
    i_newSplitDecision 	= data->add<unsigned int>("","newSplitDecision","i",0);
  }

  void bookEvents(TreeWriterData* data, string label) {
    i_ngoodparts.push_back		(data->add<int>(label,"ngoodpartons","I",0));
    i_nassocparts.push_back		(data->add<int>(label,"nassocpartons","I",0));
    i_nsplitparts.push_back		(data->add<int>(label,"nsplitpartons","I",0));
    i_nmergedparts.push_back		(data->add<int>(label,"nmergedpartons","I",0));
    i_nresolvedparts.push_back		(data->add<int>(label,"nresolvedpartons","I",0));
    i_top_isgood.push_back		(data->addMulti<bool>(label,"top_isgood",false));
    i_top_isassoc.push_back		(data->addMulti<bool>(label,"top_isassoc",false));
    i_top_isnotassoc.push_back		(data->addMulti<bool>(label,"top_isnotassoc",false));
    i_top_isnosplit.push_back		(data->addMulti<bool>(label,"top_isnosplit",false));
    i_top_issplit.push_back		(data->addMulti<bool>(label,"top_issplit",false));
    i_top_ismerged.push_back		(data->addMulti<bool>(label,"top_ismerged",false));
    i_top_iscontaminated.push_back	(data->addMulti<bool>(label,"top_iscontaminated",false));
    i_top_isresolved.push_back		(data->addMulti<bool>(label,"top_isresolved",false));
    i_top_pt.push_back			(data->addMulti<float>(label,"top_pt",0));
    i_goodtop_pt.push_back		(data->addMulti<float>(label,"goodtop_pt",0));
    i_assoctop_pt.push_back		(data->addMulti<float>(label,"assoctop_pt",0));
    i_notassoctop_pt.push_back		(data->addMulti<float>(label,"notassoctop_pt",0));
    i_nosplittop_pt.push_back		(data->addMulti<float>(label,"nosplittop_pt",0));
    i_splittop_pt.push_back		(data->addMulti<float>(label,"splittop_pt",0));
    i_mergedtop_pt.push_back		(data->addMulti<float>(label,"mergedtop_pt",0));
    i_contaminatedtop_pt.push_back	(data->addMulti<float>(label,"contaminatedtop_pt",0));
    i_resolvedtop_pt.push_back		(data->addMulti<float>(label,"resolvedtop_pt",0));
    i_jet_pt.push_back			(data->addMulti<float>(label,"jet_pt",0));
    i_jet_assocparton_pt.push_back	(data->addMulti<float>(label,"jet_assocparton_pt",0));
    i_jet_eta.push_back			(data->addMulti<float>(label,"jet_eta",0));
    i_jet_phi.push_back			(data->addMulti<float>(label,"jet_phi",0));
    i_jet_mass.push_back		(data->addMulti<float>(label,"jet_mass",0));
    i_jet_nsplits.push_back		(data->addMulti<int  >(label,"jet_nsplits",0));
    i_jet_purity.push_back		(data->addMulti<float>(label,"jet_purity",0));
    i_jet_fatjet_pt.push_back		(data->addMulti<float>(label,"jet_fatjet_pt",0));
    i_jet_fatjet_eta.push_back		(data->addMulti<float>(label,"jet_fatjet_eta",0));
    i_jet_fatjet_phi.push_back		(data->addMulti<float>(label,"jet_fatjet_phi",0));
    i_jet_fatjet_mass.push_back		(data->addMulti<float>(label,"jet_fatjet_mass",0));
  }

  void fillJets(TreeWriterData* data, SubjetTreeLoader* vars, AssocJet* fatjet) {
    data->fill<float>(i_subjet_dr,     vars->subJetDR());
    data->fill<float>(i_fatjet_pt,     fatjet->pt());
    data->fill<float>(i_fatjet_eta,    fatjet->eta());
    data->fill<float>(i_fatjet_phi,    fatjet->phi());
    data->fill<float>(i_fatjet_mass,   fatjet->mass());
    data->fill<float>(i_fatjet_purity, fatjet->purity.size() ? fatjet->purity[0].first : 0.0);
    data->fill<bool >(i_goodsplit,     !(vars->splitResult==1 || vars->splitResult==6 || vars->splitResult==7));
    data->fill<unsigned int>(i_splitAnalysis, vars->splitAnalysis);
  }

  void fillEvent(TreeWriterData* data, CachedEventInfo* info, unsigned int index) {
    // fill partons
    int ngoodparts = 0, nassocparts = 0, nsplitparts = 0, nmergedparts = 0, nresolvedparts = 0;
    for(const auto* p : info->partons) {
      if(p->diag > AssocParton::OUT_OF_RANGE && p->diag != AssocParton::EMPTY) ngoodparts++;
      if(p->diag > AssocParton::NO_JET && p->diag != AssocParton::EMPTY)       nassocparts++;
      if(p->diag == AssocParton::SPLIT)    nsplitparts++;
      if(p->diag == AssocParton::MERGED)   nmergedparts++;
      if(p->diag == AssocParton::RESOLVED) nresolvedparts++;
    }
    data->fill<int>(i_ngoodparts[index],     ngoodparts);
    data->fill<int>(i_nassocparts[index],    nassocparts);
    data->fill<int>(i_nsplitparts[index],    nsplitparts);
    data->fill<int>(i_nmergedparts[index],   nmergedparts);
    data->fill<int>(i_nresolvedparts[index], nresolvedparts);
  
    // fill tops
    for(auto* top : info->tops) {
      data->fillMulti<bool >(i_top_isgood[index],         top->diag > TopPartonAssoc::BAD_PARTON);
      data->fillMulti<bool >(i_top_isassoc[index],        top->diag > TopPartonAssoc::MISSING_JET);
      data->fillMulti<bool >(i_top_isnotassoc[index],     top->diag == TopPartonAssoc::MISSING_JET);
      data->fillMulti<bool >(i_top_isnosplit[index],      top->diag > TopPartonAssoc::SPLIT_PARTON);
      data->fillMulti<bool >(i_top_issplit[index],        top->diag == TopPartonAssoc::SPLIT_PARTON);
      data->fillMulti<bool >(i_top_ismerged[index],       top->diag == TopPartonAssoc::MERGED_PARTONS);
      data->fillMulti<bool >(i_top_iscontaminated[index], top->diag == TopPartonAssoc::CONTAMINATED);
      data->fillMulti<bool >(i_top_isresolved[index],     top->diag == TopPartonAssoc::RESOLVED);
      data->fillMulti<float>(i_top_pt[index],             top->pt);
      if(top->diag > TopPartonAssoc::BAD_PARTON)      data->fillMulti<float>(i_goodtop_pt[index],         top->pt);
      if(top->diag > TopPartonAssoc::MISSING_JET)     data->fillMulti<float>(i_assoctop_pt[index],        top->pt);
      if(top->diag == TopPartonAssoc::MISSING_JET)    data->fillMulti<float>(i_notassoctop_pt[index],     top->pt);
      if(top->diag > TopPartonAssoc::SPLIT_PARTON)    data->fillMulti<float>(i_nosplittop_pt[index],      top->pt);
      if(top->diag == TopPartonAssoc::SPLIT_PARTON)   data->fillMulti<float>(i_splittop_pt[index],        top->pt);
      if(top->diag == TopPartonAssoc::MERGED_PARTONS) data->fillMulti<float>(i_mergedtop_pt[index],       top->pt);
      if(top->diag == TopPartonAssoc::CONTAMINATED)   data->fillMulti<float>(i_contaminatedtop_pt[index], top->pt);
      if(top->diag == TopPartonAssoc::RESOLVED)       data->fillMulti<float>(i_resolvedtop_pt[index],     top->pt);
    }
  
    // fill jets
    for(auto* jet : info->finaljets) {
      data->fillMulti<float>(i_jet_pt[index],     jet->pt());
      if(jet->purity.size())
        data->fillMulti<float>(i_jet_assocparton_pt[index],     jet->pt());
      data->fillMulti<float>(i_jet_eta[index],    jet->eta());
      data->fillMulti<float>(i_jet_phi[index],    jet->phi());
      data->fillMulti<float>(i_jet_mass[index],   jet->mass());
      data->fillMulti<int  >(i_jet_nsplits[index],jet->nsplits);
      data->fillMulti<float>(i_jet_purity[index], jet->purity.size() ? jet->purity[0].first : 0.0);
      data->fillMulti<float>(i_jet_fatjet_pt[index],   jet->fatjetpt());
      data->fillMulti<float>(i_jet_fatjet_eta[index],  jet->fatjeteta());
      data->fillMulti<float>(i_jet_fatjet_phi[index],  jet->fatjetphi());
      data->fillMulti<float>(i_jet_fatjet_mass[index], jet->fatjetmass());
    }
  }

};

#endif
