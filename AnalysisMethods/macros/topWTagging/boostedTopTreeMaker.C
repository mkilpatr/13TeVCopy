#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"
#include "AnalysisTools/Utilities/interface/PartonMatching.h"

#include <algorithm>

using namespace std;
using namespace ucsbsusy;

//#define TESTMODE

class Copier : public TreeCopierManualBranches {
public:
  Copier(TString fileName, TString treeName, TString outFileName, size rSeed, bool isMCTree, cfgSet::ConfigSet * pars) : TreeCopierManualBranches(fileName,treeName,outFileName,rSeed,isMCTree,pars),
  i_process          (0),
  i_sigType          (0),
  i_mass1            (0),
  i_mass2            (0),
  i_mass3            (0),
  i_weight           (0),

  i_njets            (0),
  i_met              (0),

  i_top_pt           (0),
  i_top_eta          (0),
  i_top_beta         (0),
  i_top_maxDR        (0),
  i_top_b_pt         (0),
  i_top_W_pt         (0),
  i_top_W_j_1_pt     (0),
  i_top_W_j_2_pt     (0),

  i_top_fj_maxDR     (0),
  i_fj_mindr_p2      (0),

  i_fj_pt            (0),
  i_fj_eta           (0),
  i_fj_mass          (0),
  i_fj_prunedMass    (0),
  i_fj_sdMass        (0),
  i_fj_tau1          (0),
  i_fj_tau2          (0),
  i_fj_tau3          (0),
  i_fj_csv           (0),
  i_fj_drsj          (0),
  i_fj_sj1_pt        (0),
  i_fj_sj1_mass      (0),
  i_fj_sj1_csv       (0),
  i_fj_sj2_pt        (0),
  i_fj_sj2_mass      (0),
  i_fj_sj2_csv       (0),
  i_fj_puppi_pt      (0),
  i_fj_puppi_eta     (0),
  i_fj_puppi_mass    (0),
  i_fj_puppi_sdMass  (0),
  i_fj_puppi_tau1    (0),
  i_fj_puppi_tau2    (0),
  i_fj_puppi_tau3    (0),
  i_fj_puppi_drsj    (0),
  i_fj_puppi_sj1_pt  (0),
  i_fj_puppi_sj1_mass(0),
  i_fj_puppi_sj1_csv (0),
  i_fj_puppi_sj2_pt  (0),
  i_fj_puppi_sj2_mass(0),
  i_fj_puppi_sj2_csv (0)

  {
  };
  virtual ~Copier() {};

  virtual void loadVariables() override{
    load(cfgSet::EVTINFO);
    load(cfgSet::AK4JETS,JetReader::LOADRECO | JetReader::FILLOBJ | JetReader::LOADGEN | JetReader::LOADTOPASSOC);
    load(cfgSet::GENPARTICLES, GenParticleReader::LOADPARTONDECAY | GenParticleReader::FILLOBJ);

    load(cfgSet::ELECTRONS);
    load(cfgSet::MUONS);
    load(cfgSet::PFCANDS);
    load(cfgSet::AK8FATJETS, FatJetReader::LOADRECO | FatJetReader::LOADPUPPI | FatJetReader::FILLOBJ);

  }

  //--------------------------------------------------------------------------------------------------
  virtual BaseEventAnalyzer * setupEventAnalyzer() override {return new CopierFillYourselfEventAnalyzer();}

  bool passEventSel() const {
    if(met->pt() < 250) return false;
    if(nJets < 5) return false;
    size nlbjets = 0;
    for(const auto* j : jets) {
      if(j->csv() > defaults::CSV_LOOSE)  nlbjets++;
    }
    if(nlbjets < 2 || nBJets < 1) return false;
    if(nSelLeptons || nVetoedTracks) return false;

    return true;
  }

  template<class FatJet>
  int getMatchedJet(const PartonMatching::TopDecay *  top, const std::vector<FatJet>& fatJets, float& minDR) const {
    int idx = -1;
    float minMaxDR2 = -99;
    for(unsigned int iFJ = 0; iFJ < fatJets.size(); ++iFJ){
      float maxDR2 = -99;
      for(unsigned int iP = 0; iP < top->hadronicPartons.size(); ++iP){
        maxDR2 = std::max(double(maxDR2),PhysicsUtilities::deltaR2(*(top->hadronicPartons[iP]->parton),fatJets[iFJ]));
      }
      if(maxDR2 < 0) continue;
      if(minMaxDR2 < 0 || maxDR2 <  minMaxDR2  ) {
        idx = iFJ;
        minMaxDR2 = maxDR2;
      }
    }
    minDR = TMath::Sqrt(minMaxDR2);
    return idx;
  }

  template<class FatJet>
  float getMinSecondDRToFJ(const FatJet& fj, const PartonMatching::TopDecay * top1,const PartonMatching::TopDecay * top2){

    auto secondDR2FromTop = [](const FatJet& fj, const PartonMatching::TopDecay * top) {
      std::vector<float> drs; drs.reserve(3);
      for(const auto * p : top->hadronicPartons){
        drs.push_back(PhysicsUtilities::deltaR2(fj,*p->parton));
      }
      std::sort(drs.begin(),drs.end(),std::less<float>());
      return drs.size() > 1 ? TMath::Sqrt(drs[1]) : -99;
    };

    double dr1 = top1 ? secondDR2FromTop(fj,top1) : -99;
    double dr2 = top2 ? secondDR2FromTop(fj,top2) : -99;

    if((dr1 >= 0) && (dr2 >= 0))
      return dr1 < dr2 ? dr1 : dr2;
    return (dr1 >= 0 ? dr1 : dr2);
  }




  void fillEventProperties() {
    data.fill<unsigned int>(i_process,process);
    data.fill<unsigned int>( i_sigType,size(evtInfoReader.signalType));
    data.fill<float>(  i_mass1 ,evtInfoReader.massparams->size() > 0 ? evtInfoReader.massparams->at(0) : 0 );
    data.fill<float>(  i_mass2 ,evtInfoReader.massparams->size() > 1 ? evtInfoReader.massparams->at(1) : 0);
    data.fill<float>(  i_mass3 ,evtInfoReader.massparams->size() > 2 ? evtInfoReader.massparams->at(2) : 0);
    data.fill<float>(  i_weight ,weight);
    data.fill<unsigned int>(  i_njets,size(nJets));
    data.fill<float>(i_met           ,met->pt());
  }

  void fillGenInfo(const PartonMatching::TopDecay * top){

    auto getTopDR = [](const PartonMatching::TopDecay * top)->float {
      return  std::max(PhysicsUtilities::deltaR(*top->b->parton,*top->W_dau1->parton),
                       std::max(PhysicsUtilities::deltaR(*top->b->parton,*top->W_dau2->parton),
                                PhysicsUtilities::deltaR(*top->W_dau2->parton,*top->W_dau1->parton)
                               )
                      );
    };
    if(top && top->isLeptonic) top = 0; //only look at hadronic tops
    data.fill<float>(i_top_pt      ,!top ? 0 : top->top->pt());
    data.fill<float>(i_top_eta     ,!top ? 0 : TMath::Abs(top->top->eta()));
    data.fill<float>(i_top_beta    ,!top ? 0 : TMath::Abs(top->b->parton->eta()));
    data.fill<float>(i_top_maxDR   ,!top ? 0 : getTopDR(top));
    data.fill<float>(i_top_b_pt    ,!top ? 0 : top->b->parton->pt());
    data.fill<float>(i_top_W_pt    ,!top ? 0 : top->W->pt());
    data.fill<float>(i_top_W_j_1_pt,!top ? 0 : top->W_dau1->parton->pt());
    data.fill<float>(i_top_W_j_2_pt,!top ? 0 : top->W_dau2->parton->pt());
  }

  void fillFatJetInfo(const FatJetF * fj){
    data.fill<float>(i_fj_pt            ,!fj ? 0 : fj->pt());
    data.fill<float>(i_fj_eta           ,!fj ? 0 : TMath::Abs(fj->eta()));
    data.fill<float>(i_fj_mass          ,!fj ? 0 : fj->mass());
    data.fill<float>(i_fj_prunedMass    ,!fj ? 0 : fj->prunedMass());
    data.fill<float>(i_fj_sdMass        ,!fj ? 0 : fj->softDropMass());
    data.fill<float>(i_fj_tau1          ,!fj ? 0 : fj->tau1());
    data.fill<float>(i_fj_tau2          ,!fj ? 0 : fj->tau2());
    data.fill<float>(i_fj_tau3          ,!fj ? 0 : fj->tau3());
    data.fill<float>(i_fj_csv           ,!fj ? 0 : fj->csv());
    data.fill<float>(i_fj_drsj          ,fj && fj->nSubjets() > 1 ? PhysicsUtilities::deltaR(fj->subJet(0),fj->subJet(1)) : 0) ;
    data.fill<float>(i_fj_sj1_pt        ,fj && fj->nSubjets() > 0 ? fj->subJet(0).pt()  : 0);
    data.fill<float>(i_fj_sj1_mass      ,fj && fj->nSubjets() > 0 ? fj->subJet(0).mass(): 0);
    data.fill<float>(i_fj_sj1_csv       ,fj && fj->nSubjets() > 0 ? fj->subJet(0).csv() : 0);
    data.fill<float>(i_fj_sj2_pt        ,fj && fj->nSubjets() > 1 ? fj->subJet(1).pt()  : 0);
    data.fill<float>(i_fj_sj2_mass      ,fj && fj->nSubjets() > 1 ? fj->subJet(1).mass(): 0);
    data.fill<float>(i_fj_sj2_csv       ,fj && fj->nSubjets() > 1 ? fj->subJet(1).csv() : 0);
    data.fill<float>(i_fj_puppi_pt      ,!fj ? 0 : fj->puppi_mom().pt());
    data.fill<float>(i_fj_puppi_eta     ,!fj ? 0 : TMath::Abs(fj->puppi_mom().eta()));
    data.fill<float>(i_fj_puppi_mass    ,!fj ? 0 : fj->puppi_mom().mass());
    data.fill<float>(i_fj_puppi_sdMass  ,!fj ? 0 : fj->puppi_softDropMass());
    data.fill<float>(i_fj_puppi_tau1    ,!fj ? 0 : fj->puppi_tau1());
    data.fill<float>(i_fj_puppi_tau2    ,!fj ? 0 : fj->puppi_tau2());
    data.fill<float>(i_fj_puppi_tau3    ,!fj ? 0 : fj->puppi_tau3());
    data.fill<float>(i_fj_puppi_drsj    ,fj && fj->puppi_nSubjets() > 1 ? PhysicsUtilities::deltaR(fj->puppi_subJet(0),fj->puppi_subJet(1)) : 0) ;
    data.fill<float>(i_fj_puppi_sj1_pt  ,fj && fj->puppi_nSubjets() > 0 ? fj->puppi_subJet(0).pt()  : 0);
    data.fill<float>(i_fj_puppi_sj1_mass,fj && fj->puppi_nSubjets() > 0 ? fj->puppi_subJet(0).mass(): 0);
    data.fill<float>(i_fj_puppi_sj1_csv ,fj && fj->puppi_nSubjets() > 0 ? fj->puppi_subJet(0).csv() : 0);
    data.fill<float>(i_fj_puppi_sj2_pt  ,fj && fj->puppi_nSubjets() > 1 ? fj->puppi_subJet(1).pt()  : 0);
    data.fill<float>(i_fj_puppi_sj2_mass,fj && fj->puppi_nSubjets() > 1 ? fj->puppi_subJet(1).mass(): 0);
    data.fill<float>(i_fj_puppi_sj2_csv ,fj && fj->puppi_nSubjets() > 1 ? fj->puppi_subJet(1).csv() : 0);
  }

  virtual bool fillEvent() override {

    if(!passEventSel()) return false;

    std::vector<GenJetF*> filteredGenJes;
    for(auto * j : jets){ if(j->genJet()) filteredGenJes.push_back(j->genJet());}
    PartonMatching::PartonEvent * partonEvent = new PartonMatching::PartonEvent(genParticleReader,*defaultJets,filteredGenJes);
    const PartonMatching::TopDecay * top1 = partonEvent->topDecays.size() > 0 ? &partonEvent->topDecays[0] : 0 ;
    const PartonMatching::TopDecay * top2 = partonEvent->topDecays.size() > 1 ? &partonEvent->topDecays[1] : 0 ;

    //Only look at fully hadronic signal
    if(process == defaults::SIGNAL) {
      if(top2 == 0) return false;
      if(top1->isLeptonic || top2->isLeptonic) return false;
    }

    //Don't look at leptonic signal in any case
    if(top1 && top1->isLeptonic) top1 = 0;
    if(top2 && top2->isLeptonic) top2 = 0;


    float minDR1 = -99;
    float minDR2 = -99;
    int matchedFJ1 = -1;
    int matchedFJ2 = -1;
    if(top1) matchedFJ1 = getMatchedJet(top1,fatJetReader.fatJets, minDR1);
    if(top2) matchedFJ2 = getMatchedJet(top1,fatJetReader.fatJets, minDR2);

    //Fill tops with no matching jets...should be rare
    auto fillTopNoFJ = [this](const PartonMatching::TopDecay * top) {
      fillGenInfo(top);
      data.fill<float>(i_top_fj_maxDR          , 99);
      data.fill<float>(i_fj_mindr_p2           , 99);
      fillFatJetInfo(0);
      fillFillingTree();
    };

    fillEventProperties();

    if(top1 && matchedFJ1 < 0) fillTopNoFJ(top1); //If there was no match
    if(top2 && matchedFJ2 < 0) fillTopNoFJ(top2); //If there was no match

    for(unsigned int iFJ = 0; iFJ < fatJetReader.fatJets.size(); ++iFJ){
      const auto& fj = fatJetReader.fatJets[iFJ];

      if(matchedFJ1 == iFJ){
        fillGenInfo(top1);
        data.fill<float>(i_top_fj_maxDR         ,minDR1);
      } else if(matchedFJ2 == iFJ){
        fillGenInfo(top2);
        data.fill<float>(i_top_fj_maxDR         ,minDR2);
      } else{
        fillGenInfo(0);
        data.fill<float>(i_top_fj_maxDR         ,99);
      }

      float minDR = getMinSecondDRToFJ(fj,top1,top2);
      data.fill<float>(i_fj_mindr_p2            , minDR  < 0 ? float(99.0) : minDR);

      fillFatJetInfo(&fj);
      fillFillingTree();
    }
    return true;
  }

  void book() override {
    i_process              = data.add<unsigned int>("","process"   ,"i",0);
    i_sigType              = data.add<unsigned int>("","sigType"   ,"i",0);
    i_mass1                = data.add<float>("","mass1"            ,"F",0);
    i_mass2                = data.add<float>("","mass2"            ,"F",0);
    i_mass3                = data.add<float>("","mass3"            ,"F",0);
    i_weight               = data.add<float>("","weight"           ,"F",0);

    i_njets                = data.add<unsigned int>("","njets"     ,"i",0);
    i_met                  = data.add<float>("","met"              ,"F",0);

    i_top_pt               = data.add<float>("","top_pt"           ,"F",0);
    i_top_eta              = data.add<float>("","top_eta"          ,"F",0);
    i_top_beta             = data.add<float>("","top_beta"         ,"F",0);
    i_top_maxDR            = data.add<float>("","top_maxDR"        ,"F",0);
    i_top_b_pt             = data.add<float>("","top_b_pt"         ,"F",0);
    i_top_W_pt             = data.add<float>("","top_W_pt"         ,"F",0);
    i_top_W_j_1_pt         = data.add<float>("","top_W_j_1_pt"     ,"F",0);
    i_top_W_j_2_pt         = data.add<float>("","top_W_j_2_pt"     ,"F",0);

    i_top_fj_maxDR        = data.add<float>("","top_fj_maxDR"      ,"F",0);
    i_fj_mindr_p2         = data.add<float>("","fj_mindr_p2"       ,"F",0);

    i_fj_pt               = data.add<float>("","fj_pt"             ,"F",0);
    i_fj_eta              = data.add<float>("","fj_eta"            ,"F",0);
    i_fj_mass             = data.add<float>("","fj_mass"           ,"F",0);
    i_fj_prunedMass       = data.add<float>("","fj_prunedMass"     ,"F",0);
    i_fj_sdMass           = data.add<float>("","fj_sdMass"         ,"F",0);
    i_fj_tau1             = data.add<float>("","fj_tau1"           ,"F",0);
    i_fj_tau2             = data.add<float>("","fj_tau2"           ,"F",0);
    i_fj_tau3             = data.add<float>("","fj_tau3"           ,"F",0);
    i_fj_csv              = data.add<float>("","fj_csv"            ,"F",0);
    i_fj_drsj             = data.add<float>("","fj_drsj"           ,"F",0);
    i_fj_sj1_pt           = data.add<float>("","fj_sj1_pt"         ,"F",0);
    i_fj_sj1_mass         = data.add<float>("","fj_sj1_mass"       ,"F",0);
    i_fj_sj1_csv          = data.add<float>("","fj_sj1_csv"        ,"F",0);
    i_fj_sj2_pt           = data.add<float>("","fj_sj2_pt"         ,"F",0);
    i_fj_sj2_mass         = data.add<float>("","fj_sj2_mass"       ,"F",0);
    i_fj_sj2_csv          = data.add<float>("","fj_sj2_csv"        ,"F",0);
    i_fj_puppi_drsj       = data.add<float>("","fj_puppi_drsj"     ,"F",0);
    i_fj_puppi_pt         = data.add<float>("","fj_puppi_pt"       ,"F",0);
    i_fj_puppi_eta        = data.add<float>("","fj_puppi_eta"      ,"F",0);
    i_fj_puppi_mass       = data.add<float>("","fj_puppi_mass"     ,"F",0);
    i_fj_puppi_sdMass     = data.add<float>("","fj_puppi_sdMass"   ,"F",0);
    i_fj_puppi_tau1       = data.add<float>("","fj_puppi_tau1"     ,"F",0);
    i_fj_puppi_tau2       = data.add<float>("","fj_puppi_tau2"     ,"F",0);
    i_fj_puppi_tau3       = data.add<float>("","fj_puppi_tau3"     ,"F",0);
    i_fj_puppi_sj1_pt     = data.add<float>("","fj_puppi_sj1_pt"   ,"F",0);
    i_fj_puppi_sj1_mass   = data.add<float>("","fj_puppi_sj1_mass" ,"F",0);
    i_fj_puppi_sj1_csv    = data.add<float>("","fj_puppi_sj1_csv"  ,"F",0);
    i_fj_puppi_sj2_pt     = data.add<float>("","fj_puppi_sj2_pt"   ,"F",0);
    i_fj_puppi_sj2_mass   = data.add<float>("","fj_puppi_sj2_mass" ,"F",0);
    i_fj_puppi_sj2_csv    = data.add<float>("","fj_puppi_sj2_csv"  ,"F",0);


  }


  //Process description
  size i_process  ;
  size i_sigType  ;
  size i_mass1    ;
  size i_mass2    ;
  size i_mass3    ;
  size i_weight   ;
  //Event properties
  size i_njets   ;
  size i_met     ;

  //gen properties
  size i_top_pt          ;
  size i_top_eta         ;
  size i_top_beta        ;
  size i_top_maxDR       ;
  size i_top_b_pt        ;
  size i_top_W_pt        ;
  size i_top_W_j_1_pt    ;
  size i_top_W_j_2_pt    ;

  //matching details
  size i_top_fj_maxDR    ;
  size i_fj_mindr_p2     ;

  //fj properties
  size i_fj_pt            ;
  size i_fj_eta           ;
  size i_fj_mass          ;
  size i_fj_prunedMass    ;
  size i_fj_sdMass        ;
  size i_fj_tau1          ;
  size i_fj_tau2          ;
  size i_fj_tau3          ;
  size i_fj_csv           ;
  size i_fj_drsj          ;
  size i_fj_sj1_pt        ;
  size i_fj_sj1_mass      ;
  size i_fj_sj1_csv       ;
  size i_fj_sj2_pt        ;
  size i_fj_sj2_mass      ;
  size i_fj_sj2_csv       ;
  size i_fj_puppi_pt      ;
  size i_fj_puppi_eta     ;
  size i_fj_puppi_mass    ;
  size i_fj_puppi_sdMass  ;
  size i_fj_puppi_tau1    ;
  size i_fj_puppi_tau2    ;
  size i_fj_puppi_tau3    ;
  size i_fj_puppi_drsj    ;
  size i_fj_puppi_sj1_pt  ;
  size i_fj_puppi_sj1_mass;
  size i_fj_puppi_sj1_csv ;
  size i_fj_puppi_sj2_pt  ;
  size i_fj_puppi_sj2_mass;
  size i_fj_puppi_sj2_csv ;

};


#endif

void boostedTopTreeMaker(TString sname = "T2tt",
                             const int fileindex = -1,
                             const bool isMC = true,
                             const TString fname = "/store/user/lpcstop/noreplica/13TeV/120716/signals/merged/T2tt_1000_1_ntuple_postproc.root",
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
