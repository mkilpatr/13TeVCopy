#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"
#include "AnalysisTools/Utilities/interface/PartonMatching.h"


using namespace std;
using namespace ucsbsusy;

//#define TESTMODE

class Copier : public TreeCopierManualBranches {
public:
  Copier(TString fileName, TString treeName, TString outFileName, size rSeed, bool isMCTree, cfgSet::ConfigSet * pars) : TreeCopierManualBranches(fileName,treeName,outFileName,rSeed,isMCTree,pars),
  i_process        (0),
  i_sigType        (0),
  i_mass1          (0),
  i_mass2          (0),
  i_mass3          (0),
  i_weight         (0),

  i_njets          (0),
  i_ht             (0),
  i_met            (0),
  i_nlbjets        (0),
  i_nmbjets        (0),

  i_top_1_had      (0),
  i_top_1_pt       (0),
  i_top_1_maxDR    (0),
  i_top_1_minWbDR  (0),
  i_top_1_b_pt     (0),
  i_top_1_W_pt     (0),
  i_top_1_W_DR     (0),
  i_top_1_W_j_1_pt (0),
  i_top_1_W_j_2_pt (0),

  i_top_2_had      (0),
  i_top_2_pt       (0),
  i_top_2_maxDR    (0),
  i_top_2_minWbDR  (0),
  i_top_2_b_pt     (0),
  i_top_2_W_pt     (0),
  i_top_2_W_DR     (0),
  i_top_2_W_j_1_pt (0),
  i_top_2_W_j_2_pt (0)

  {
  };
  virtual ~Copier() {};

  virtual void loadVariables() override{
    load(cfgSet::EVTINFO);
    load(cfgSet::AK4JETS,JetReader::LOADRECO | JetReader::FILLOBJ | JetReader::LOADGEN | JetReader::LOADTOPASSOC);
    load(cfgSet::GENPARTICLES, GenParticleReader::LOADPARTONDECAY | GenParticleReader::FILLOBJ);
  }

  //--------------------------------------------------------------------------------------------------
  virtual BaseEventAnalyzer * setupEventAnalyzer() override {return new CopierFillYourselfEventAnalyzer();}

  virtual bool fillEvent() override {

    std::vector<GenJetF*> filteredGenJes;
    for(auto * j : jets){ if(j->genJet()) filteredGenJes.push_back(j->genJet());}
    PartonMatching::PartonEvent * partonEvent = new PartonMatching::PartonEvent(genParticleReader,*defaultJets,filteredGenJes);
    const PartonMatching::TopDecay * top1 = partonEvent->topDecays.size() > 0 ? &partonEvent->topDecays[0] : 0 ;
    const PartonMatching::TopDecay * top2 = partonEvent->topDecays.size() > 1 ? &partonEvent->topDecays[1] : 0 ;

    //filter
    if(top1->isLeptonic || top2->isLeptonic) return false;
    if(met->pt() < 250) return false;


    data.fill<unsigned int>(i_process,process);
    data.fill<unsigned int>(  i_sigType,size(evtInfoReader.signalType));
    data.fill<float>(  i_mass1 ,evtInfoReader.massparams->size() > 0 ? evtInfoReader.massparams->at(0) : 0 );
    data.fill<float>(  i_mass2 ,evtInfoReader.massparams->size() > 1 ? evtInfoReader.massparams->at(1) : 0);
    data.fill<float>(  i_mass3 ,evtInfoReader.massparams->size() > 2 ? evtInfoReader.massparams->at(2) : 0);
    data.fill<float>(  i_weight ,weight);

    size nlbjets = 0;
    for(const auto* j : jets) {
      if(j->csv() > defaults::CSV_LOOSE)  nlbjets++;
    }



    data.fill<unsigned int>(  i_njets,size(nJets));
     data.fill<float>(i_ht            , JetKinematics::ht(jets));
     data.fill<float>(i_met           ,met->pt());
     data.fill<unsigned int>(  i_nlbjets,nlbjets);
     data.fill<unsigned int>(  i_nmbjets,size(nBJets));


     auto getTopDR = [](const PartonMatching::TopDecay * top)->float {
       return  std::max(PhysicsUtilities::deltaR(*top->b->parton,*top->W_dau1->parton),
                        std::max(PhysicsUtilities::deltaR(*top->b->parton,*top->W_dau2->parton),
                                 PhysicsUtilities::deltaR(*top->W_dau2->parton,*top->W_dau1->parton)
                                )
                       );
     };
     auto getMinWbDR = [](const PartonMatching::TopDecay * top)->float {
       return  std::min(PhysicsUtilities::deltaR(*top->b->parton,*top->W_dau2->parton),
                            PhysicsUtilities::deltaR(*top->b->parton,*top->W_dau1->parton)
                       );
     };
     auto getWDR = [](const PartonMatching::TopDecay * top)->float {
       return  PhysicsUtilities::deltaR(*top->W_dau1->parton,*top->W_dau2->parton);
     };


     data.fill<bool >(i_top_1_had     ,!top1 ? 0 : !top1->isLeptonic);
     data.fill<float>(i_top_1_pt      ,!top1 ? 0 : top1->top->pt());
     data.fill<float>(i_top_1_maxDR   ,!top1 ? 0 : getTopDR(top1));
     data.fill<float>(i_top_1_minWbDR ,!top1 ? 0 : getMinWbDR(top1));
     data.fill<float>(i_top_1_b_pt    ,!top1 ? 0 : top1->b->parton->pt());
     data.fill<float>(i_top_1_W_pt    ,!top1 ? 0 : top1->W->pt());
     data.fill<float>(i_top_1_W_DR    ,!top1 ? 0 : getWDR(top1));
     data.fill<float>(i_top_1_W_j_1_pt,!top1 ? 0 : top1->W_dau1->parton->pt());
     data.fill<float>(i_top_1_W_j_2_pt,!top1 ? 0 : top1->W_dau2->parton->pt());

     data.fill<bool >(i_top_2_had     ,!top2 ? 0 : !top2->isLeptonic);
     data.fill<float>(i_top_2_pt      ,!top2 ? 0 : top2->top->pt());
     data.fill<float>(i_top_2_maxDR   ,!top2 ? 0 : getTopDR(top2));
     data.fill<float>(i_top_2_minWbDR ,!top2 ? 0 : getMinWbDR(top2));
     data.fill<float>(i_top_2_b_pt    ,!top2 ? 0 : top2->b->parton->pt());
     data.fill<float>(i_top_2_W_pt    ,!top2 ? 0 : top2->W->pt());
     data.fill<float>(i_top_2_W_DR    ,!top2 ? 0 : getWDR(top2));
     data.fill<float>(i_top_2_W_j_1_pt,!top2 ? 0 : top2->W_dau1->parton->pt());
     data.fill<float>(i_top_2_W_j_2_pt,!top2 ? 0 : top2->W_dau2->parton->pt());


    fillFillingTree();
    return true;
  }

  void book() override {
    i_process          = data.add<unsigned int>("","process"      ,"i",0);
    i_sigType          = data.add<unsigned int>("","sigType"      ,"i",0);
    i_mass1            = data.add<float>("","mass1"               ,"F",0);
    i_mass2            = data.add<float>("","mass2"               ,"F",0);
    i_mass3            = data.add<float>("","mass3"               ,"F",0);
    i_weight           = data.add<float>("","weight"              ,"F",0);

    i_njets            = data.add<unsigned int>("","njets"        ,"i",0);
    i_ht               = data.add<float>("","ht"                  ,"F",0);
    i_met              = data.add<float>("","met"                 ,"F",0);
    i_nlbjets            = data.add<unsigned int>("","nlbjets"        ,"i",0);
    i_nmbjets            = data.add<unsigned int>("","nmbjets"        ,"i",0);

    i_top_1_had        = data.add<bool> ("","top_1_had"        ,"O",0);
    i_top_1_pt         = data.add<float>("","top_1_pt"         ,"F",0);
    i_top_1_maxDR      = data.add<float>("","top_1_maxDR"      ,"F",0);
    i_top_1_minWbDR    = data.add<float>("","top_1_minWbDR"    ,"F",0);
    i_top_1_b_pt       = data.add<float>("","top_1_b_pt"       ,"F",0);
    i_top_1_W_pt       = data.add<float>("","top_1_W_pt"       ,"F",0);
    i_top_1_W_DR       = data.add<float>("","top_1_W_DR"       ,"F",0);
    i_top_1_W_j_1_pt   = data.add<float>("","top_1_W_j_1_pt"   ,"F",0);
    i_top_1_W_j_2_pt   = data.add<float>("","top_1_W_j_2_pt"   ,"F",0);

    i_top_2_had        = data.add<bool> ("","top_2_had"        ,"O",0);
    i_top_2_pt         = data.add<float>("","top_2_pt"         ,"F",0);
    i_top_2_maxDR      = data.add<float>("","top_2_maxDR"      ,"F",0);
    i_top_2_minWbDR    = data.add<float>("","top_2_minWbDR"    ,"F",0);
    i_top_2_b_pt       = data.add<float>("","top_2_b_pt"       ,"F",0);
    i_top_2_W_pt       = data.add<float>("","top_2_W_pt"       ,"F",0);
    i_top_2_W_DR       = data.add<float>("","top_2_W_DR"       ,"F",0);
    i_top_2_W_j_1_pt   = data.add<float>("","top_2_W_j_1_pt"   ,"F",0);
    i_top_2_W_j_2_pt   = data.add<float>("","top_2_W_j_2_pt"   ,"F",0);
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
  size i_ht      ;
  size i_met     ;
  size i_nlbjets ;
  size i_nmbjets ;

  //gen properties
  size i_top_1_had         ;
  size i_top_1_pt          ;
  size i_top_1_maxDR       ;
  size i_top_1_minWbDR     ;
  size i_top_1_b_pt        ;
  size i_top_1_W_pt        ;
  size i_top_1_W_DR        ;
  size i_top_1_W_j_1_pt    ;
  size i_top_1_W_j_2_pt    ;

  size i_top_2_had         ;
  size i_top_2_pt          ;
  size i_top_2_maxDR       ;
  size i_top_2_minWbDR     ;
  size i_top_2_b_pt        ;
  size i_top_2_W_pt        ;
  size i_top_2_W_DR        ;
  size i_top_2_W_j_1_pt    ;
  size i_top_2_W_j_2_pt    ;

};


#endif

void genPropertiesTreeMaker(TString sname = "T2tt",
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
