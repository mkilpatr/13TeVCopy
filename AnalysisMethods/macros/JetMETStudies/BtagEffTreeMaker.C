#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"


using namespace std;
using namespace ucsbsusy;

//#define TESTMODE

class Copier : public TreeCopierManualBranches {
public:
  Copier(TString fileName, TString treeName, TString outFileName, size rSeed, bool isMCTree, cfgSet::ConfigSet * pars) : TreeCopierManualBranches(fileName,treeName,outFileName,rSeed,isMCTree,pars),
  i_process     (0),
  i_passBaseline(0),
  i_weight      (0),
  i_puWeight    (0),
  i_pt          (0),
  i_absEta         (0),
  i_csv (0),
  i_flavor      (0)
  {
  };
  virtual ~Copier() {};

  virtual void loadVariables(){
    load(cfgSet::EVTINFO);
    load(cfgSet::AK4JETS,JetReader::LOADRECO | JetReader::LOADGEN | JetReader::FILLOBJ);
    load(cfgSet::GENPARTICLES);
  }

  //--------------------------------------------------------------------------------------------------
  virtual BaseEventAnalyzer * setupEventAnalyzer() override {return new CopierFillYourselfEventAnalyzer();}

  virtual bool fillEvent() {

    if(applyCHFFilter && !cfgSet::passCHFFilter(jets)) return false;

    //If the event has gen leptons get a list so we can veto any jets near them
    //should make the efficiencies more pure
    vector<CylLorentzVectorF> genLeptons;
    for(const auto* p : genParts ){
      int pdgID = p->absPdgId();
      if(!ParticleInfo::isLepton(pdgID)) continue;
      if(!ParticleInfo::isLastInChain(p)) continue;
      if(pdgID == ParticleInfo::p_eminus || pdgID == ParticleInfo::p_muminus){
        genLeptons.push_back(p->p4());
        continue;
      }
      CylLorentzVectorF visMom;
      for(unsigned int iD = 0; iD < p->numberOfDaughters(); ++iD){
        if(ParticleInfo::isInvisible(p->daughterRef(iD)->pdgId())) continue;
        visMom += p->daughterRef(iD)->p4();
      }
      genLeptons.push_back(visMom);
    }


    bool passBaseline = true;
    if(met->pt() < 175) passBaseline = false;
    if(nJets < 4) passBaseline = false;
    if(!passBaseline) return false;

    data.fill<unsigned int>(i_process,process);
    data.fill<bool>(i_passBaseline,passBaseline);
    data.fill<float>(i_weight,weight);
    data.fill<float>(i_puWeight,eventCorrections.getTruePUWeight());


    for(const auto* j : jets ){

#ifdef TESTMODE
      const auto * bCorr = bTagCorrections.getBTagByEvtWeightCorrector();
      bool isTTBARLike = false;
      if(process == defaults::TTBAR || process == defaults::SINGLE_T || process == defaults::TTZ || process == defaults::TTW )
        isTTBARLike = true;

      JetFlavorInfo::JetFlavor flv = j->genJet() ? JetFlavorInfo::JetFlavor(j->genJet()->flavor()) : JetFlavorInfo::numJetFlavors;

      CORRTYPE corrType =  (flv == JetFlavorInfo::c_jet || flv == JetFlavorInfo::b_jet ) ?  configSet.corrections.heavyBTagCorrType : configSet.corrections.lightBTagCorrType;

//      cout << TString::Format("%.1f, %.2f, %s, %4f ",j->pt(),j->eta(),JetFlavorInfo::jetFlavorName(flv).Data(),j->csv());
//      for(unsigned int iT = defaults::CSVL; iT <= defaults::CSVT; ++iT){
//        cout << TString::Format("(%f,%f)  ",
//        bCorr->getJetEff(j->pt(),j->eta(),flv, defaults::CSVWPs(iT), isTTBARLike),
//        bCorr->getJetEffSF(j->pt(),j->eta(),flv, defaults::CSVWPs(iT),corrType));
//      }
//      cout <<         bCorr->getJetWeight(j, configSet.corrections.lightBTagCorrType,configSet.corrections.heavyBTagCorrType,isTTBARLike ) << endl;
#endif

      bool vetoJet = false;
      for(const auto& l: genLeptons){
        if(PhysicsUtilities::deltaR2(l,j->p4()) < .16) vetoJet = true;
      }
      if(vetoJet) continue;


      data.fill<float>(i_pt  ,j->pt() );
      data.fill<float>(i_absEta  ,j->absEta() );
      data.fill<float>(i_csv  ,j->csv() );
      data.fill<unsigned int>(i_flavor  ,(j->genJet())  ? j->genJet()->flavor() : JetFlavorInfo::numJetFlavors );
      fillFillingTree();
    }

#ifdef TESTMODE
    bool isTTBARLike = false;
    if(process == defaults::TTBAR || process == defaults::SINGLE_T || process == defaults::TTZ || process == defaults::TTW )
      isTTBARLike = true;
    cout << endl <<" ----------  "<< bTagCorrections.getBTagByEvtWeightCorrector()->getEvtWeight(jets,configSet.corrections.lightBTagCorrType,configSet.corrections.heavyBTagCorrType,isTTBARLike) <<" "<<bTagCorrections.getBTagByEvtWeight()<<" ----------  "<<endl;
#endif

    return true;
  }

  void book() {
    i_process       = data.add<unsigned int>("","process"                      ,"i",0);
    i_passBaseline  = data.add<bool>("","passBaseline"                      ,"O",0);
    i_weight        = data.add<float>("","weight"                      ,"F",0);
    i_puWeight      = data.add<float>("","puWeight"                      ,"F",0);
    i_pt            = data.add<float>("","pt"                      ,"F",0);
    i_absEta        = data.add<float>("","absEta"                      ,"F",0);
    i_csv           = data.add<float>("","csv"                      ,"F",0);
    i_flavor        = data.add<unsigned int>("","flavor"                      ,"i",0);
  }


  //event level info
  size i_process     ;
  size i_passBaseline;
  size i_weight      ;
  size i_puWeight    ;
  size i_pt          ;
  size i_absEta      ;
  size i_csv         ;
  size i_flavor      ;

  bool   applyCHFFilter = false ;
};


#endif

void BtagEffTreeMaker(TString sname = "T2tt_750_100",
                             const int fileindex = -1,
                             const bool isMC = true,
                             const TString fname = "/store/user/apatters/13TeV/271115/merged/T2tt_750_100_ntuple_postproc.root",
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

   cfg.corrections.ttbarCorrections |= ucsbsusy::TtbarCorrectionSet::TOPPAIRPT;

   // disable JetID for signal samples
    if (sname.Contains("T2tt") || sname.Contains("T2tb") || sname.Contains("T2bW") || sname.Contains("T2fbd") || sname.Contains("T2cc")) cfg.jets.applyJetID = false;

    TString treeName = "Events";
    Copier a(fullname,treeName,outfilename.Data(),fileindex +2,isMC, &cfg);


    // CHF filter for FastSim
    if (sname.Contains("T2tt") || sname.Contains("T2tb") || sname.Contains("T2bW") || sname.Contains("T2fbd") || sname.Contains("T2cc")) a.applyCHFFilter = true;



    a.analyze();

}


/*
 * Running
 * nohup root -b -q '$CMSSW_BASE/src/AnalysisMethods/macros/JetMETStudies/BtagEffTreeMaker.C+("/eos/uscms/store/user/hqu/13TeV/290915/merged/ttbar-powheg-ext_10_ntuple_postproc.root"              )' &
 * For every QCD file
 */
