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

class TopCand{
public:
  TopCand() {}
  TopCand(const RecoJetF *inB, const RecoJetF *inJ2, const RecoJetF *inJ3) : b(inB) {
    if (inJ2->pt()>inJ3->pt()){
      j2 = inJ2; j3 = inJ3;
    }else {
      j2 = inJ3; j3 = inJ2;
    }
    wcand = j2->p4() + j3->p4();
    topcand = b->p4() + wcand.p4();
  }

  bool passMassW(double range=40)   const { return std::abs(wcand.mass()-80)    <= range; }
  bool passMassTop(double range=80) const { return std::abs(topcand.mass()-175) <= range; }

  friend std::ostream &operator<<(std::ostream &os, const TopCand &c){
    os << "top (disc=" << c.disc << "): " << c.topcand << endl
        << " -- b : " << c.b->p4() << "csv = " << c.b->csv() << endl
        << " -- j2: " << c.j2->p4() << endl
        << " -- j3: " << c.j3->p4() << endl;
    return os;
  }

  bool sameAs(const TopCand &c) const {
    return b==c.b && j2==c.j2 && j3==c.j3;
  }

  bool overlaps(const TopCand &c) const {
    return b ==c.b || b ==c.j2 || b ==c.j3
        || j2==c.b || j2==c.j2 || j2==c.j3
        || j3==c.b || j3==c.j2 || j3==c.j3;
  }

public:
  const RecoJetF *b  = nullptr;
  const RecoJetF *j2 = nullptr;
  const RecoJetF *j3 = nullptr;
  MomentumF wcand, topcand;

  double disc = -9;

};


class Copier : public TreeCopierManualBranches {
public:
  Copier(TString fileName, TString treeName, TString outfileName, size randomSeed, bool isMCTree, cfgSet::ConfigSet *pars) :
    TreeCopierManualBranches(fileName, treeName, outfileName, randomSeed, isMCTree, pars) { initTopMVA(); }
  virtual ~Copier() {}

  virtual void loadVariables() override{
    load(cfgSet::EVTINFO);
    load(cfgSet::AK4JETS, JetReader::defaultOptions | JetReader::LOADGEN | JetReader::LOADTOPASSOC | JetReader::LOADJETSHAPE);
    load(cfgSet::ELECTRONS);
    load(cfgSet::MUONS);
    load(cfgSet::PFCANDS);
    if(isMC()) load(cfgSet::GENPARTICLES);
  }

  void initTopMVA(){
    varsF = {
 //     "var_b_pt",
      "var_b_csv",

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
      "var_topcand_pt",
      "var_b_wcand_deltaR",

      "var_wcand_mass",
      "var_wcand_ptDR",
      "var_wcand_pt",
      "var_wcand_deltaR",

 //     "var_b_j2_mass",
 //     "var_b_j3_mass",

 //     "var_top_radius",

 //     "var_sd_0",
 //     "var_sd_0p5",
 //     "var_sd_1",
 //     "var_sd_1p5",
 //     "var_sd_n0p5",
 //     "var_sd_n1",
      "var_sd_n1p5",

    };

    varsI = {
      "var_j2_mult",
      "var_j3_mult",
 //     "var_n_extra_jets",
    };

    topMVA = new TMVAReader(TString(cfgSet::CMSSW_BASE)+"/src/data/resTop/TMVAClassification_BDTG.weights.xml", "BDTG");
    topMVA->addVariables(varsF);
    topMVA->addVariables(varsI);
  }

  map<TString, float> calcTopCandVars(const TopCand *topcand){
    map<TString, float> vars;

    vars["var_b_pt"]         = topcand->b->pt();
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
    data.fill<float>(i_met, met->pt());
    data.fill<int  >(i_npv, nPV);
    data.fill<int  >(i_nvetolep, nSelLeptons);
    data.fill<int  >(i_nvetotau, nVetoedTracks);
    data.fill<float>(i_ht,  JetKinematics::ht(jets));
    data.fill<int  >(i_njets,    nJets);
    data.fill<int  >(i_nbjets,   nBJets);
    data.fill<int  >(i_nlbjets,  std::count_if(jets.begin(), jets.end(), [](const RecoJetF *j){ return j->csv() > defaults::CSV_LOOSE; }));
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
    if(nJets < 5)   return false;
    if(met->pt() < 200) return false;
    if(nBJets< 1)   return false;
    int nLBJets = std::count_if(jets.begin(), jets.end(), [](const RecoJetF *j){ return j->csv() > defaults::CSV_LOOSE; });
    if (nLBJets<2)  return false;

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

    auto countTops = [&cands](double threshold){
      return std::count_if(cands.begin(), cands.end(), [threshold](const TopCand &t){return t.disc>threshold;});
    };

    data.fill<int>(i_ntopVL, countTops(0.0));
    data.fill<int>(i_ntopL, countTops(0.2));
    data.fill<int>(i_ntopM, countTops(0.5));
    data.fill<int>(i_ntopT, countTops(0.8));

    auto passCutSel = [](const TopCand &t){
      return t.passMassW(20) && t.passMassTop(50)
          && abs(t.topcand.pt()*PhysicsUtilities::deltaR(*t.b, t.wcand)/260-1)<0.3
          && abs(t.wcand.pt()  *PhysicsUtilities::deltaR(*t.j2, *t.j3)/160-1)<0.3;
    };

    data.fill<int>(i_ntopC, std::count_if(cands.begin(), cands.end(), [&passCutSel](const TopCand &t){return passCutSel(t);}));

    if(process==defaults::TTBAR || process==defaults::SIGNAL){

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
      if (matchedCands.empty()) return false;

      for (auto &c : allCands){
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

    }else {
      // bkg samples
      if(!allCands.empty()){
        const auto cand = allCands.at(randGen->Uniform(0, allCands.size()));
        data.fill<float>(i_cand_pt,   cand.topcand.pt());
        data.fill<float>(i_cand_disc, cand.disc);
      }
    }

    return true;
  }


  void book() override {
    i_event          = data.add<unsigned int>("","event","i",0);
    i_weight         = data.add<float>("","weight","F",0);

    i_met            = data.add<float>("","met","F",0);
    i_npv            = data.add<int>("","npv","I",0);
    i_nvetolep       = data.add<int>("","nvetolep","I",0);
    i_nvetotau       = data.add<int>("","nvetotau","I",0);
    i_ht             = data.add<float>("","ht","F",0);
    i_njets          = data.add<int>("","njets","I",0);
    i_nbjets         = data.add<int>("","nbjets","I",0);
    i_nlbjets        = data.add<int>("","nlbjets","I",0);

    i_ntopC          = data.add<int>("","ntopC","I",0);
    i_ntopVL         = data.add<int>("","ntopVL","I",0);
    i_ntopL          = data.add<int>("","ntopL","I",0);
    i_ntopM          = data.add<int>("","ntopM","I",0);
    i_ntopT          = data.add<int>("","ntopT","I",0);

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

  }


  //Process description
  size i_weight   ;
  size i_event    ;

  //Event properties
  size i_met       ;
  size i_npv       ;
  size i_nvetolep  ;
  size i_nvetotau  ;
  size i_ht      ;
  size i_njets   ;
  size i_nbjets  ;
  size i_nlbjets ;

  //mvaTop
  size i_ntopC;
  size i_ntopVL;
  size i_ntopL;
  size i_ntopM;
  size i_ntopT;

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

  TMVAReader *topMVA = nullptr;
  std::vector<TString> varsF;
  std::vector<TString> varsI;

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
  cfgSet::ConfigSet cfg = cfgSet::zl_search_set;

  // disable JetID for signal samples
  if (sname.Contains("T2tt") || sname.Contains("T2tb") || sname.Contains("T2bW") || sname.Contains("T2fbd") || sname.Contains("T2cc")) cfg.jets.applyJetID = false;

  TString treeName = "Events";
  Copier a(fullname,treeName,outfilename.Data(),fileindex +2,isMC, &cfg);
  a.analyze();

}
