#if !defined(__CINT__) || defined(__MAKECINT__)

#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"
#include "AnalysisTools/Utilities/interface/PartonMatching.h"

#include "AnalysisBase/TreeAnalyzer/interface/TMVAReader.h"

using namespace std;
using namespace ucsbsusy;

class Copier : public TreeCopierManualBranches {
public:
  Copier(TString fileName, TString treeName, TString outfileName, size randomSeed, bool isMCTree, cfgSet::ConfigSet *pars) :
    TreeCopierManualBranches(fileName, treeName, outfileName, randomSeed, isMCTree, pars) { initTopMVA(); }
  virtual ~Copier() {}

  virtual void loadVariables() override{
    load(cfgSet::EVTINFO);
    load(cfgSet::AK4JETS, JetReader::defaultOptions | JetReader::LOADJETSHAPE | (isMC() ? JetReader::LOADGEN | JetReader::LOADTOPASSOC : JetReader::NULLOPT));
    load(cfgSet::ELECTRONS);
    load(cfgSet::MUONS);
    load(cfgSet::PFCANDS);
    if(isMC()) load(cfgSet::GENPARTICLES);
  }

  void initTopMVA(){
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

 //     "var_top_radius",

 //     "var_sd_0",
 //     "var_sd_0p5",
 //     "var_sd_1",
 //     "var_sd_1p5",
 //     "var_sd_n0p5",
 //     "var_sd_n1",
//      "var_sd_n1p5",
      "var_sd_n2",

    };

    varsI = {
      "var_j2_mult",
      "var_j3_mult",
 //     "var_n_extra_jets",
    };

    topMVA = new TMVAReader(TString(cfgSet::CMSSW_BASE)+"/src/data/resTop/resTop_xGBoost_v0.weights.xml", "BDT");
    topMVA->addVariables(varsF);
    topMVA->addVariables(varsI);
  }

  map<TString, float> calcTopCandVars(const TopCand *topcand){
    map<TString, float> vars;

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

    double top_radius = -1;
    for (const auto *j : {topcand->b, topcand->j2, topcand->j3}){
      double dR = PhysicsUtilities::deltaR(*j, topcand->topcand);
      if(dR > top_radius) top_radius = dR;
    }
    int n_extra_jets = 0;
    for (const auto *j : jets){
      if (PhysicsUtilities::deltaR(topcand->topcand, *j) < top_radius){
        if (j!=topcand->b && j!=topcand->j2 && j!=topcand->j3) ++n_extra_jets;
      }
    }

    vars["var_top_radius"]   = top_radius;
    vars["var_n_extra_jets"] = n_extra_jets;

    return vars;
  }

  void addEventInfo(){
    data.fill<unsigned int>(i_event, event);
    data.fill<float>(i_weight, weight);
    data.fill<bool >(i_ismc,   isMC());
    data.fill<float>(i_truePUWeight,       eventCorrections.getTruePUWeight());
    data.fill<float>(i_btagWeight,         bTagCorrections.getBTagByEvtWeight());
    data.fill<float>(i_lepvetoweightHM,    leptonCorrections.getVetoLepWeightHM());
    data.fill<float>(i_leptnpweightHM,     leptonCorrections.getTnPLepWeightHM());
    data.fill<bool>(i_passjson,            isMC() || (hasJSONFile() && passesLumiMask()));
    data.fill<float>(i_j1chEnFrac,         jets.front()->chHadFrac());  // take the leading jet after cleaning/jetid/etc.

    const auto &evt = evtInfoReader;
    bool passmetfilters = evt.HBHENoiseFilter && evt.HBHENoiseIsoFilter && evt.globalTightHalo2016Filter && evt.EcalDeadCellTriggerPrimitiveFilter && evt.goodVertices && evt.eeBadScFilter && evt.badChCand && evt.badPFMuon;
    data.fill<bool>(i_passmetfilters,  passmetfilters);

    data.fill<float>(i_met, met->pt());
    data.fill<int  >(i_npv, nPV);
    data.fill<int  >(i_nvetolep, nSelLeptons);
    data.fill<int  >(i_nvetotau, nVetoedTracks);
    data.fill<float>(i_ht,  JetKinematics::ht(jets));
    data.fill<int  >(i_njets,    nJets);
    data.fill<int  >(i_nbjets,   nBJets);
    data.fill<int  >(i_nlbjets,  std::count_if(jets.begin(), jets.end(), [](const RecoJetF *j){ return j->csv() > defaults::CSV_LOOSE; }));

    if (selectedLepton){
      data.fill<float>(i_leptonpt,     selectedLepton->pt());
      data.fill<int>  (i_leptonpdgid,    selectedLepton->pdgid());
    }

  }

  void addTopCandInfo(const TopCand *topcand){

    data.fill<float>(i_var_b_pt,    topcand->b->pt());
    data.fill<float>(i_var_b_mass,  topcand->b->mass());
    data.fill<float>(i_var_b_csv,   topcand->b->csv());
    data.fill<float>(i_var_b_cvsb,  topcand->b->cvsb());
    data.fill<float>(i_var_b_qgl,   topcand->b->qgl());
    data.fill<float>(i_var_b_ptD,   topcand->b->ptD());
    data.fill<float>(i_var_b_axis1, topcand->b->axis1());
    data.fill<float>(i_var_b_axis2, topcand->b->axis2());
    data.fill<int>  (i_var_b_mult,  topcand->b->jetMult());

    data.fill<float>(i_var_j2_pt,    topcand->j2->pt());
    data.fill<float>(i_var_j2_csv,   topcand->j2->csv());
    data.fill<float>(i_var_j2_cvsl,  topcand->j2->cvsl());
    data.fill<float>(i_var_j2_cvsb,  topcand->j2->cvsb());
    data.fill<float>(i_var_j2_qgl,   topcand->j2->qgl());
    data.fill<float>(i_var_j2_ptD,   topcand->j2->ptD());
    data.fill<float>(i_var_j2_axis1, topcand->j2->axis1());
    data.fill<float>(i_var_j2_axis2, topcand->j2->axis2());
    data.fill<int>  (i_var_j2_mult,  topcand->j2->jetMult());

    data.fill<float>(i_var_j3_pt,    topcand->j3->pt());
    data.fill<float>(i_var_j3_csv,   topcand->j3->csv());
    data.fill<float>(i_var_j3_cvsl,  topcand->j3->cvsl());
    data.fill<float>(i_var_j3_cvsb,  topcand->j3->cvsb());
    data.fill<float>(i_var_j3_qgl,   topcand->j3->qgl());
    data.fill<float>(i_var_j3_ptD,   topcand->j3->ptD());
    data.fill<float>(i_var_j3_axis1, topcand->j3->axis1());
    data.fill<float>(i_var_j3_axis2, topcand->j3->axis2());
    data.fill<int>  (i_var_j3_mult,  topcand->j3->jetMult());

    double var_b_wcand_deltaR = PhysicsUtilities::deltaR(*topcand->b, topcand->wcand);
    data.fill<float>(i_var_b_wcand_deltaR, var_b_wcand_deltaR);
    data.fill<float>(i_var_topcand_pt,    topcand->topcand.pt());
    data.fill<float>(i_var_topcand_ptDR,  topcand->topcand.pt()*var_b_wcand_deltaR);
    data.fill<float>(i_var_topcand_mass,  topcand->topcand.mass());

    double var_wcand_deltaR = PhysicsUtilities::deltaR(*topcand->j2, *topcand->j3);
    data.fill<float>(i_var_wcand_deltaR,  var_wcand_deltaR);
    data.fill<float>(i_var_wcand_pt,      topcand->wcand.pt());
    data.fill<float>(i_var_wcand_ptDR,    topcand->wcand.pt()*var_wcand_deltaR);
    data.fill<float>(i_var_wcand_mass,    topcand->wcand.mass());

    auto calcChi2 = [](const TopCand &c){
      double m_top, m_w, sigma_top, sigma_w;
      if (c.topcand.pt()<250)       { m_top=161; sigma_top=18; m_w=79; sigma_w=11; }
      else if (c.topcand.pt()<350)  { m_top=167; sigma_top=16; m_w=81; sigma_w=10; }
      else                          { m_top=172; sigma_top=16; m_w=84; sigma_w=10; }
      return std::pow((c.topcand.mass()-m_top)/sigma_top, 2)+std::pow((c.wcand.mass()-m_w)/sigma_w, 2);
    };
    data.fill<float>(i_var_chi2,      calcChi2(*topcand));

    double var_sd_0 = topcand->j3->pt()/(topcand->j2->pt()+topcand->j3->pt());
    data.fill<float>(i_var_sd_0,      var_sd_0);
    data.fill<float>(i_var_sd_0p5,    var_sd_0/std::pow(var_wcand_deltaR, 0.5));
    data.fill<float>(i_var_sd_1,      var_sd_0/var_wcand_deltaR);
    data.fill<float>(i_var_sd_1p5,    var_sd_0/std::pow(var_wcand_deltaR, 1.5));
    data.fill<float>(i_var_sd_n0p5,   var_sd_0/std::pow(var_wcand_deltaR, -0.5));
    data.fill<float>(i_var_sd_n1,     var_sd_0/std::pow(var_wcand_deltaR, -1));
    data.fill<float>(i_var_sd_n1p5,   var_sd_0/std::pow(var_wcand_deltaR, -1.5));
    data.fill<float>(i_var_sd_n2,     var_sd_0/std::pow(var_wcand_deltaR, -2));

    data.fill<float>(i_var_b_j2_mass, (topcand->b->p4() + topcand->j2->p4()).mass());
    data.fill<float>(i_var_b_j3_mass, (topcand->b->p4() + topcand->j3->p4()).mass());

    double top_radius = -1;
    for (const auto *j : {topcand->b, topcand->j2, topcand->j3}){
      double dR = PhysicsUtilities::deltaR(*j, topcand->topcand);
      if(dR > top_radius) top_radius = dR;
    }
    int n_extra_jets = 0;
    for (const auto *j : jets){
      if (PhysicsUtilities::deltaR(topcand->topcand, *j) < top_radius){
        if (j!=topcand->b && j!=topcand->j2 && j!=topcand->j3) ++n_extra_jets;
      }
    }

    data.fill<float>(i_var_top_radius, top_radius);
    data.fill<int>  (i_var_n_extra_jets, n_extra_jets);

  }


  TopCand *getTopCand(const PartonMatching::TopDecay *topDecay){
    const RecoJetF *b_, *j2_, *j3_;
    auto isCompatible = [](double a, double b){ double r=(a>b)?(b/a):(a/b); return r>0.5; };

    vector<RecoJetF*> tmpJets(jets);
    // first match the b
    double nearestDR = 999;
    int bIndx = PhysicsUtilities::findNearestDRDeref(*topDecay->b->parton, tmpJets, nearestDR, 0.4);
    if (bIndx<0) return nullptr; // if b-quark not matched to any reco jet
    b_ = tmpJets.at(bIndx);

    // next match the W daughters
    tmpJets.erase(tmpJets.begin()+bIndx);
    int j2Indx = PhysicsUtilities::findNearestDRDeref(*topDecay->W_dau1->parton, tmpJets, nearestDR, 0.4);
    if (j2Indx<0) return nullptr;
    j2_ = tmpJets.at(j2Indx);

    tmpJets.erase(tmpJets.begin()+j2Indx);
    int j3Indx = PhysicsUtilities::findNearestDRDeref(*topDecay->W_dau2->parton, tmpJets, nearestDR, 0.4);
    if (j3Indx<0) return nullptr;
    j3_ = tmpJets.at(j3Indx);

    bool ptMatched = isCompatible(topDecay->b->parton->pt(), b_->pt())
        && isCompatible(topDecay->W_dau1->parton->pt(), j2_->pt())
        && isCompatible(topDecay->W_dau2->parton->pt(), j3_->pt());
    if(!ptMatched) return nullptr;

    return new TopCand(b_, j2_, j3_);
  }

  vector<TopCand> removeOverlap(vector<TopCand> &cands){
    std::sort(cands.begin(), cands.end(), [](const TopCand &a, const TopCand &b){ return a.disc>b.disc; });

    vector<TopCand> cleanedCands;
    for (const auto &c : cands){
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

  virtual bool fillEvent() override {

    if(!goodvertex) return false;
    if(nBJets< 1)   return false;
    int nLBJets = std::count_if(jets.begin(), jets.end(), [](const RecoJetF *j){ return j->csv() > defaults::CSV_LOOSE; });
    if (nLBJets<2)  return false;

    // for performance studies
//    if (nJets<5 || met->pt()<200) return false;

    // for data/MC
    if(nSelLeptons==0){
      // 0L: fake enriched
      if (nJets<3 || JetKinematics::ht(jets)<1000) return false;
    }else{
      // 1L: ttbar enriched
      if (nJets<3 || met->pt()<100) return false;
    }

    addEventInfo();

    // jets sorted by CSV
    vector<RecoJetF*> csvJets(jets);
    cfgSet::sortByCSV(csvJets);


    vector<TopCand> allCands;
    for (unsigned iB=0; iB<2; ++iB){
      vector<RecoJetF*> tmpJets(csvJets);
      tmpJets.erase(tmpJets.begin()+iB);
      for (unsigned i2=0; i2<tmpJets.size()-1; ++i2){
        for (unsigned i3=i2+1; i3<tmpJets.size(); ++i3){
          TopCand tmpCand(csvJets.at(iB), tmpJets.at(i2), tmpJets.at(i3));
          if (tmpCand.passMassW() && tmpCand.passMassTop()){
            auto varMap = calcTopCandVars(&tmpCand);
            for (const auto &v: varsF){
              topMVA->setValue(v, varMap.at(v));
            }
            for (const auto &v: varsI){
              topMVA->setValue(v, varMap.at(v));
            }
            tmpCand.disc = topMVA->eval();
            allCands.push_back(tmpCand);
          }
        }
      }
    }
    auto cands = removeOverlap(allCands);
//    std::sort(cands.begin(), cands.end(),  [](const TopCand &a, const TopCand &b){ return a.topcand.pt()>b.topcand.pt(); });

    if(cands.size()>0){
      data.fill<float>(i_topcand1_pt, cands.at(0).topcand.pt());
      data.fill<float>(i_topcand1_disc, cands.at(0).disc);
      addTopCandInfo(&cands.at(0));
    }
    if(cands.size()>1){
      data.fill<float>(i_topcand2_pt, cands.at(1).topcand.pt());
      data.fill<float>(i_topcand2_disc, cands.at(1).disc);
    }


    auto countTops = [&cands](double threshold){
      return std::count_if(cands.begin(), cands.end(), [threshold](const TopCand &t){return t.disc>threshold;});
    };

    data.fill<int>(i_ntopVL, countTops(0.0));
    data.fill<int>(i_ntopL, countTops(0.2));
    data.fill<int>(i_ntopM, countTops(0.5));
    data.fill<int>(i_ntopT, countTops(0.8));
    data.fill<int>(i_ntopWP6, countTops(0.6));
    data.fill<int>(i_ntopWP7, countTops(0.7));
    data.fill<int>(i_ntopWP974, countTops(0.974));

    auto passCutSel = [](const TopCand &t){
      return t.passMassW(20) && t.passMassTop(50)
          && abs(t.topcand.pt()*PhysicsUtilities::deltaR(*t.b, t.wcand)/260-1)<0.3
          && abs(t.wcand.pt()  *PhysicsUtilities::deltaR(*t.j2, *t.j3)/160-1)<0.3;
    };

    data.fill<int>(i_ntopC, std::count_if(cands.begin(), cands.end(), [&passCutSel](const TopCand &t){return passCutSel(t);}));

    if((process==defaults::TTBAR && !isTTbar2L) || process==defaults::SIGNAL){

      // candidates formed by matching ak4 jets to the hadronic gen-tops
      vector<pair<const PartonMatching::TopDecay*, const TopCand*>> matchedCands;

      // reconstructed candidates after cleaning, excluding those w/ all 3 jets matched to gen. had. tops
      vector<const TopCand*> unmatchedCands;

      std::vector<GenJetF*> filteredGenJets;
      for(auto * j : jets){ if(j->genJet()) filteredGenJets.push_back(j->genJet());}
      PartonMatching::PartonEvent * partonEvent = new PartonMatching::PartonEvent(genParticleReader,*defaultJets,filteredGenJets);
      for (const auto &t : partonEvent->topDecays){
        if(t.isLeptonic) continue;
        const auto *tCand = getTopCand(&t); // if tCand is null : gen-matching failed
        if(tCand){
          for(const auto &c : cands){
            if(tCand->sameAs(c)) {
              // get the disc. value if tCand is in cleaned cands collection - otherwise disc = -9 (default)
              delete tCand;
              tCand = &c; break;
            }
          }
        }

        // (1) tCand is null: gen-top cannot be resolved to 3 ak4 jets
        // (2) tCand.disc = -9: tCand not reconstructed by the algorithm
        // (3) gen-matched and reconstructed
        matchedCands.emplace_back(&t, tCand);
      }

      // matchedCands is non-empty if there exist hadronic top
      if (matchedCands.empty()) return true;

      for (auto &c : cands){
        bool matched = false;
        for (const auto &pr : matchedCands){
          if (pr.second && pr.second->sameAs(c)){
            matched = true; break;
          }
        }
        if(!matched) unmatchedCands.push_back(&c);
      }

      // fill gen-top info
      const auto tPair = matchedCands.at(randGen->Uniform(0, matchedCands.size()));
      const auto topDecay = tPair.first;
      data.fill<float>(i_gen_top_pt, topDecay->top->pt());
      data.fill<float>(i_gen_w_pt,   topDecay->W->pt());
      data.fill<int>  (i_gen_b_diag, topDecay->b->diag);
      data.fill<int>  (i_gen_w_diag, topDecay->W_decay->diag);
      data.fill<bool> (i_gen_top_resolved, topDecay->diag == PartonMatching::RESOLVED);

      // fill matched reco-top info (for efficiency)
      const auto tCand = tPair.second;
      if(tCand){
        data.fill<bool>(i_flag_trijet_matched, true);

        int b_csv_rank = std::distance(csvJets.begin() , std::find(csvJets.begin(), csvJets.end(), tCand->b));
        data.fill<bool>(i_flag_b_pass_csv, b_csv_rank==0 || b_csv_rank==1);

        data.fill<float>(i_top_disc, tCand->disc);
      }

      // fill unmatched reco-top (for fake rate)
      if (!unmatchedCands.empty()){
        const auto fake = unmatchedCands.at(randGen->Uniform(0, unmatchedCands.size()));
        data.fill<float>(i_cand_pt,     fake->topcand.pt());
        data.fill<float>(i_cand_disc,   fake->disc);

        bool hasTrueB = false;
        for (const auto &pr : matchedCands){
          if (pr.second && fake->b == pr.second->b){
            hasTrueB = true; break;
          }
        }
        data.fill<bool>(i_cand_true_b, hasTrueB);
      }

      // fill if the (<=2) cands are truth matched
      if(cands.size()>0){
        for (const auto &tP : matchedCands){
          if (tP.second && tP.second->sameAs(cands.at(0))){
            data.fill<bool> (i_topcand1_true, true);
            break;
          }
        }
      }
      if(cands.size()>1){
        for (const auto &tP : matchedCands){
          if (tP.second && tP.second->sameAs(cands.at(1))){
            data.fill<bool> (i_topcand2_true, true);
            break;
          }
        }
      }
    }else {
      // bkg samples
      if(!cands.empty()){
        const auto cand = cands.at(randGen->Uniform(0, cands.size()));
        data.fill<float>(i_cand_pt,   cand.topcand.pt());
        data.fill<float>(i_cand_disc, cand.disc);
      }
    }

    return true;
  }


  void book() override {
    i_event          = data.add<unsigned int>("","event","i",0);
    i_weight         = data.add<float>("","weight","F",0);
    i_ismc           = data.add<bool >("","ismc","O",0);
    i_truePUWeight   = data.add<float>("","truePUWeight","F",0);
    i_btagWeight     = data.add<float>("","btagWeight","F",0);
    i_lepvetoweightHM  = data.add<float>("","lepvetoweightHM","F",0);
    i_leptnpweightHM = data.add<float>("","leptnpweightHM","F",0);
    i_passjson       = data.add<bool>("","passjson","O",0);
    i_j1chEnFrac     = data.add<float>("","j1chEnFrac","F",2);
    i_passmetfilters = data.add<bool>("","passmetfilters","O",0);

    i_met            = data.add<float>("","met","F",0);
    i_npv            = data.add<int>("","npv","I",0);
    i_nvetolep       = data.add<int>("","nvetolep","I",0);
    i_nvetotau       = data.add<int>("","nvetotau","I",0);
    i_ht             = data.add<float>("","ht","F",0);
    i_njets          = data.add<int>("","njets","I",0);
    i_nbjets         = data.add<int>("","nbjets","I",0);
    i_nlbjets        = data.add<int>("","nlbjets","I",0);
    i_leptonpt       = data.add<float>("","leptonpt","F",0);
    i_leptonpdgid    = data.add<int>("","leptonpdgid","I",0);

    i_ntopC          = data.add<int>("","ntopC","I",0);
    i_ntopVL         = data.add<int>("","ntopVL","I",0);
    i_ntopL          = data.add<int>("","ntopL","I",0);
    i_ntopM          = data.add<int>("","ntopM","I",0);
    i_ntopT          = data.add<int>("","ntopT","I",0);
    i_ntopWP6        = data.add<int>("","ntopWP6","I",0);
    i_ntopWP7        = data.add<int>("","ntopWP7","I",0);
    i_ntopWP974      = data.add<int>("","ntopWP974","I",0);

    i_gen_top_pt     = data.add<float>("","gen_top_pt","F",-1);
    i_gen_w_pt       = data.add<float>("","gen_w_pt","F",-1);
    i_gen_b_diag     = data.add<int>  ("","gen_b_diag","I",-1);
    i_gen_w_diag     = data.add<int>  ("","gen_w_diag","I",-1);
    i_gen_top_resolved = data.add<bool> ("","gen_top_resolved","O",0);

    i_cand_pt        = data.add<float>("","cand_pt","F",-1);
    i_cand_disc      = data.add<float>("","cand_disc","F",-9);
    i_cand_true_b    = data.add<bool> ("","cand_true_b","O",0);

    i_flag_trijet_matched  = data.add<bool> ("","flag_trijet_matched","O",0);
    i_flag_b_pass_csv      = data.add<bool> ("","flag_b_pass_csv","O",0);
    i_top_disc       = data.add<float>("","top_disc","F",-9);

    i_topcand1_pt    = data.add<float>("","topcand1_pt","F",-1);
    i_topcand1_disc  = data.add<float>("","topcand1_disc","F",-1);
    i_topcand1_true  = data.add<bool> ("","topcand1_true","O",0);

    i_topcand2_pt    = data.add<float>("","topcand2_pt","F",-1);
    i_topcand2_disc  = data.add<float>("","topcand2_disc","F",-1);
    i_topcand2_true  = data.add<bool> ("","topcand2_true","O",0);

    i_var_b_pt       = data.add<float>("","var_b_pt","F",-1);
    i_var_b_mass     = data.add<float>("","var_b_mass","F",-1);
    i_var_b_csv      = data.add<float>("","var_b_csv","F",-1);
    i_var_b_cvsb     = data.add<float>("","var_b_cvsb","F",-1);
    i_var_b_qgl      = data.add<float>("","var_b_qgl","F",-1);
    i_var_b_ptD      = data.add<float>("","var_b_ptD","F",-1);
    i_var_b_axis1    = data.add<float>("","var_b_axis1","F",-1);
    i_var_b_axis2    = data.add<float>("","var_b_axis2","F",-1);
    i_var_b_mult     = data.add<int>  ("","var_b_mult","I",-1);

    i_var_j2_pt       = data.add<float>("","var_j2_pt","F",-1);
    i_var_j2_csv      = data.add<float>("","var_j2_csv","F",-1);
    i_var_j2_cvsl     = data.add<float>("","var_j2_cvsl","F",-1);
    i_var_j2_cvsb     = data.add<float>("","var_j2_cvsb","F",-1);
    i_var_j2_qgl      = data.add<float>("","var_j2_qgl","F",-1);
    i_var_j2_ptD      = data.add<float>("","var_j2_ptD","F",-1);
    i_var_j2_axis1    = data.add<float>("","var_j2_axis1","F",-1);
    i_var_j2_axis2    = data.add<float>("","var_j2_axis2","F",-1);
    i_var_j2_mult     = data.add<int>  ("","var_j2_mult","I",-1);

    i_var_j3_pt       = data.add<float>("","var_j3_pt","F",-1);
    i_var_j3_csv      = data.add<float>("","var_j3_csv","F",-1);
    i_var_j3_cvsl     = data.add<float>("","var_j3_cvsl","F",-1);
    i_var_j3_cvsb     = data.add<float>("","var_j3_cvsb","F",-1);
    i_var_j3_qgl      = data.add<float>("","var_j3_qgl","F",-1);
    i_var_j3_ptD      = data.add<float>("","var_j3_ptD","F",-1);
    i_var_j3_axis1    = data.add<float>("","var_j3_axis1","F",-1);
    i_var_j3_axis2    = data.add<float>("","var_j3_axis2","F",-1);
    i_var_j3_mult     = data.add<int>  ("","var_j3_mult","I",-1);

    i_var_topcand_mass   = data.add<float>("","var_topcand_mass","F",-1);
    i_var_topcand_ptDR   = data.add<float>("","var_topcand_ptDR","F",-1);
    i_var_topcand_pt     = data.add<float>("","var_topcand_pt","F",-1);
    i_var_b_wcand_deltaR = data.add<float>("","var_b_wcand_deltaR","F",-1);

    i_var_wcand_mass     = data.add<float>("","var_wcand_mass","F",-1);
    i_var_wcand_ptDR     = data.add<float>("","var_wcand_ptDR","F",-1);
    i_var_wcand_pt       = data.add<float>("","var_wcand_pt","F",-1);
    i_var_wcand_deltaR   = data.add<float>("","var_wcand_deltaR","F",-1);

    i_var_chi2           = data.add<float>("","var_chi2","F",-1);

    i_var_sd_0           = data.add<float>("","var_sd_0","F",-1);
    i_var_sd_0p5         = data.add<float>("","var_sd_0p5","F",-1);
    i_var_sd_1           = data.add<float>("","var_sd_1","F",-1);
    i_var_sd_1p5         = data.add<float>("","var_sd_1p5","F",-1);
    i_var_sd_n0p5        = data.add<float>("","var_sd_n0p5","F",-1);
    i_var_sd_n1          = data.add<float>("","var_sd_n1","F",-1);
    i_var_sd_n1p5        = data.add<float>("","var_sd_n1p5","F",-1);
    i_var_sd_n2          = data.add<float>("","var_sd_n2","F",-1);


    i_var_b_j2_mass      = data.add<float>("","var_b_j2_mass","F",-1);
    i_var_b_j3_mass      = data.add<float>("","var_b_j3_mass","F",-1);

    i_var_top_radius  = data.add<float>("","var_top_radius","F",-1);
    i_var_n_extra_jets = data.add<int> ("","var_n_extra_jets","I",0);

}


  //Process description
  size i_weight   ;
  size i_event    ;
  size i_ismc      ;
  size i_truePUWeight;
  size i_btagWeight;
  size i_lepvetoweightHM;
  size i_leptnpweightHM;
  size i_passjson    ;
  size i_j1chEnFrac  ;
  size i_passmetfilters;

  //Event properties
  size i_met       ;
  size i_npv       ;
  size i_nvetolep  ;
  size i_nvetotau  ;
  size i_ht      ;
  size i_njets   ;
  size i_nbjets  ;
  size i_nlbjets ;
  size i_leptonpt  ;
  size i_leptonpdgid;

  //mvaTop
  size i_ntopC;
  size i_ntopVL;
  size i_ntopL;
  size i_ntopM;
  size i_ntopT;
  size i_ntopWP6;
  size i_ntopWP7;
  size i_ntopWP974;

  //gen properties
  size i_gen_top_pt;
  size i_gen_w_pt;
  size i_gen_b_diag;
  size i_gen_w_diag;
  size i_gen_top_resolved;

  // cands for fake rate
  size i_cand_pt;
  size i_cand_disc;
  size i_cand_true_b;

  // reco-top for efficiency
  size i_flag_trijet_matched;
  size i_flag_b_pass_csv;
  size i_top_disc;

  // the (<=2) cands after cleaning
  size i_topcand1_pt;
  size i_topcand1_disc;
  size i_topcand1_true;

  size i_topcand2_pt;
  size i_topcand2_disc;
  size i_topcand2_true;

  // input vars for cand1: for data/MC
  size i_var_b_pt;
  size i_var_b_mass;
  size i_var_b_csv;
  size i_var_b_cvsb;
  size i_var_b_qgl;
  size i_var_b_ptD;
  size i_var_b_axis1;
  size i_var_b_axis2;
  size i_var_b_mult;

  size i_var_j2_pt;
  size i_var_j2_csv;
  size i_var_j2_cvsl;
  size i_var_j2_cvsb;
  size i_var_j2_qgl;
  size i_var_j2_ptD;
  size i_var_j2_axis1;
  size i_var_j2_axis2;
  size i_var_j2_mult;

  size i_var_j3_pt;
  size i_var_j3_csv;
  size i_var_j3_cvsl;
  size i_var_j3_cvsb;
  size i_var_j3_qgl;
  size i_var_j3_ptD;
  size i_var_j3_axis1;
  size i_var_j3_axis2;
  size i_var_j3_mult;

  size i_var_topcand_mass;
  size i_var_topcand_ptDR;
  size i_var_topcand_pt;
  size i_var_b_wcand_deltaR;

  size i_var_wcand_mass;
  size i_var_wcand_ptDR;
  size i_var_wcand_pt;
  size i_var_wcand_deltaR;

  size i_var_chi2;

  size i_var_sd_0;
  size i_var_sd_0p5;
  size i_var_sd_1;
  size i_var_sd_1p5;
  size i_var_sd_n0p5;
  size i_var_sd_n1;
  size i_var_sd_n1p5;
  size i_var_sd_n2;

  size i_var_b_j2_mass;
  size i_var_b_j3_mass;

  size i_var_top_radius;
  size i_var_n_extra_jets;


  TMVAReader *topMVA = nullptr;
  std::vector<TString> varsF;
  std::vector<TString> varsI;

  bool isTTbar2L = false;
};


#endif

void ResolvedTopMVATester(TString sname = "T2tt",
    const int fileindex = -1,
    const bool isMC = true,
    const TString fname = "/store/user/lpcstop/noreplica/13TeV/120716/signals/merged/T2tt_850_100_ntuple_postproc.root",
    const TString outputdir = "trees",
    const TString fileprefix = "root://cmseos:1094/",
    const TString json=TString::Format("%s/src/data/JSON/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_v2.txt",getenv("CMSSW_BASE")))
{

  printf("Processing file %d of %s sample\n", (fileindex > -1 ? fileindex : 0), sname.Data());

  if(fileindex > -1)
    sname += TString::Format("_%d",fileindex);

  TString fullname = fileprefix+fname;

  gSystem->mkdir(outputdir,true);
  TString outfilename = outputdir+"/"+sname+"_tree.root";
  cfgSet::loadDefaultConfigurations();
  cfgSet::setJSONFile(json);
  cfgSet::ConfigSet cfg = cfgSet::zl_search_set;

  // disable JetID for signal samples
  if (sname.Contains("T2tt") || sname.Contains("T2tb") || sname.Contains("T2bW") || sname.Contains("T2fbd") || sname.Contains("T2cc")) cfg.jets.applyJetID = false;

  TString treeName = "Events";
  Copier a(fullname,treeName,outfilename.Data(),fileindex +2,isMC, &cfg);
  if(sname.Contains("ttbar-2l")) a.isTTbar2L = true;
  a.analyze();

}
