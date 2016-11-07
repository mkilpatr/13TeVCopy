#if !defined(__CINT__) || defined(__MAKECINT__)

#include <algorithm>
#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"
#include "AnalysisTools/Utilities/interface/PartonMatching.h"

using namespace std;
using namespace ucsbsusy;

class Copier : public TreeCopierManualBranches {
public:
  Copier(TString fileName, TString treeName, TString outfileName, size randomSeed, bool isMCTree, cfgSet::ConfigSet *pars) :
    TreeCopierManualBranches(fileName, treeName, outfileName, randomSeed, isMCTree, pars) {}
  virtual ~Copier() {}

  virtual BaseEventAnalyzer * setupEventAnalyzer() override {return new CopierFillYourselfEventAnalyzer();}

  virtual void loadVariables() override{
    load(cfgSet::EVTINFO);
    load(cfgSet::AK4JETS, JetReader::defaultOptions | JetReader::LOADGEN | JetReader::LOADTOPASSOC | JetReader::LOADJETSHAPE);
    load(cfgSet::ELECTRONS);
    load(cfgSet::MUONS);
    load(cfgSet::PFCANDS);
    if(isMC()) load(cfgSet::GENPARTICLES);
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

  void addTopCandInfo(const TopCand *topcand, bool isSignal){
    data.fill<bool> (i_flag_signal, isSignal);

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

  void addGenTopInfo(const PartonMatching::TopDecay *topDecay){
    data.fill<float>(i_gen_top_pt, topDecay->top->pt());
    data.fill<float>(i_gen_w_pt,   topDecay->W->pt());
    data.fill<int>  (i_gen_b_diag, topDecay->b->diag);
    data.fill<int>  (i_gen_w_diag, topDecay->W_decay->diag);
    data.fill<bool> (i_gen_top_resolved, topDecay->diag == PartonMatching::RESOLVED);
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

    //      cout << "-----\n~~b:\n" << topDecay->b->parton->p4() << "\n" << b_->p4();
    //      cout <<      "\n~~w:\n" << topDecay->W_dau1->parton->p4() << "\n" << j2_->p4() << "\n"
    //                              << topDecay->W_dau2->parton->p4() << "\n" << j3_->p4() << "\n";

    bool ptMatched = isCompatible(topDecay->b->parton->pt(), b_->pt())
        && isCompatible(topDecay->W_dau1->parton->pt(), j2_->pt())
        && isCompatible(topDecay->W_dau2->parton->pt(), j3_->pt());
    if(!ptMatched) return nullptr;

    return new TopCand(b_, j2_, j3_);
  }


  void prepareEventToFill() {

    if(!goodvertex) return;
    if(nJets < 5)   return;
    if(met->pt() < 100) return;
    if(nBJets< 1)   return;
    auto nLBJets = std::count_if(jets.begin(), jets.end(), [](const RecoJetF *j){ return j->csv() > defaults::CSV_LOOSE; });
    if(nLBJets<2) return;

    // jets sorted by CSV
    vector<RecoJetF*> csvJets(jets);
    cfgSet::sortByCSV(csvJets);

    // fill event vars
    addEventInfo();

    // ------------------------------
    // form all possible candidates
    vector<TopCand> cands;
    for (unsigned iB=0; iB<2; ++iB){
      vector<RecoJetF*> tmpJets(csvJets);
      tmpJets.erase(tmpJets.begin()+iB);
      for (unsigned i2=0; i2<tmpJets.size()-1; ++i2){
        for (unsigned i3=i2+1; i3<tmpJets.size(); ++i3){
          TopCand tmpCand(csvJets.at(iB), tmpJets.at(i2), tmpJets.at(i3));
          if (tmpCand.passMassW() && tmpCand.passMassTop()){
            cands.push_back(tmpCand);
          }
        }
      }
    }
    if(cands.empty()) return;

    // ------------------------------
    // treat signal and bkg differently
    if(process==defaults::TTBAR || process==defaults::SIGNAL){
      vector<TopCand> matchedCands;

      std::vector<GenJetF*> filteredGenJets;
      for(auto * j : jets){ if(j->genJet()) filteredGenJets.push_back(j->genJet());}
      PartonMatching::PartonEvent * partonEvent = new PartonMatching::PartonEvent(genParticleReader,*defaultJets,filteredGenJets);
      for (const auto &topDecay : partonEvent->topDecays){
        if(topDecay.isLeptonic) continue;
        const auto topcand = getTopCand(&topDecay);
        if(!topcand) continue;

        int b_csv_rank = std::distance(csvJets.begin() , std::find(csvJets.begin(), csvJets.end(), topcand->b));
        if (b_csv_rank>1) continue; // if matched jet is not among the two highest CSV jets
        if(!topcand->passMassTop() || !topcand->passMassW()) continue;

        matchedCands.push_back(*topcand);
        addGenTopInfo(&topDecay);
        addTopCandInfo(topcand, true);
        fillFillingTree();
        delete topcand;
      }

      vector<const TopCand*> fakeCands;
      for (const auto &c : cands){
        bool isMatched = false;
        for (const auto &top : matchedCands){
          if (c.b==top.b && c.j2==top.j2 && c.j3==top.j3) {
            isMatched = true; break;
          }
        }
        if (!isMatched) fakeCands.push_back(&c);
      }

      std::random_shuffle(fakeCands.begin(), fakeCands.end());
      for (unsigned i=0; i<2 && i<fakeCands.size(); ++i){
        addTopCandInfo(fakeCands.at(i), false);
        fillFillingTree();
      }

    }else{

//      auto dist = [](const TopCand &t) {
//        return std::pow((t.wcand.mass()-80)/10, 2) + std::pow((t.topcand.mass()-175)/20, 2);
//      };
//      std::sort(cands.begin(), cands.end(), [&dist](const TopCand &a, const TopCand &b){ return dist(a)<dist(b); });
//      unsigned randIndx = 0;

      unsigned randIndx = randGen->Uniform(0, cands.size());
      addTopCandInfo(&cands.at(randIndx), false);
      fillFillingTree();

      for (unsigned i=0; i<2 && cands.size()>1; ++i){
        cands.erase(cands.begin()+randIndx);
        randIndx = randGen->Uniform(0, cands.size());
        addTopCandInfo(&cands.at(randIndx), false);
        fillFillingTree();
      }

    }

  }


  virtual bool fillEvent() override {
    if (event%3==0) return false; // Use 2/3 of the events for training
    prepareEventToFill();
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

    i_flag_signal    = data.add<bool>("","flag_signal","O",0);

    i_gen_top_pt     = data.add<float>("","gen_top_pt","F",-1);
    i_gen_w_pt       = data.add<float>("","gen_w_pt","F",-1);
    i_gen_b_diag     = data.add<int>  ("","gen_b_diag","I",-1);
    i_gen_w_diag     = data.add<int>  ("","gen_w_diag","I",-1);
    i_gen_top_resolved = data.add<bool> ("","gen_top_resolved","O",0);

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

  //Event properties
  size i_met       ;
  size i_npv       ;
  size i_nvetolep  ;
  size i_nvetotau  ;
  size i_ht      ;
  size i_njets   ;
  size i_nbjets  ;
  size i_nlbjets ;

  //gen properties
  size i_gen_top_pt;
  size i_gen_w_pt;
  size i_gen_b_diag;
  size i_gen_w_diag;
  size i_gen_top_resolved;

  size i_flag_signal;

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

};


#endif

void ResolvedTopMVATreeMaker(TString sname = "T2tt",
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
