#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"


using namespace std;
using namespace ucsbsusy;

#define TESTMODE

class Copier : public TreeCopierManualBranches {
public:
  Copier(string fileName, string treeName, string outFileName, bool isMCTree, cfgSet::ConfigSet * pars) : TreeCopierManualBranches(fileName,treeName,outFileName,isMCTree,pars),
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
  void analyze(int reportFrequency = 10000, int numEvents = -1)
  {
    loadVariables();
    isLoaded_ = true;
    setupTree();
    book();
    data.book(treeWriter_);
    while(reader.nextEvent(reportFrequency)){
      isProcessed_ = false;
      if(numEvents >= 0 && getEventNumber() >= numEvents) return;
      processVariables();
      data.reset();
      fillEvent();
    }
  }

  virtual bool fillEvent() {

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
    if(nJets < 2 || jets[1]->pt() < 75 ) passBaseline = false;
    if(nJets < 5) passBaseline = false;

    data.fill<unsigned int>(i_process,process);
    data.fill<bool>(i_passBaseline,passBaseline);
    data.fill<float>(i_weight,weight);
    data.fill<float>(i_puWeight,eventCorrections.getPUWeight());


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
      data.fill<unsigned int>(i_flavor  ,(j->genJet() && j->genJet()->pt() > 10)  ? j->genJet()->flavor() : JetFlavorInfo::numJetFlavors );
      outFile_->cd();
      treeWriter_->fill();
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


};


#endif

void BtagEffTreeMaker(string fileName,  string treeName = "Events", string outPostfix ="btagEff", bool isMC = true) {

  cfgSet::loadDefaultConfigurations();
  cfgSet::ConfigSet cfg = cfgSet::zl_search_set;

  #ifdef TESTMODE
  cfg.corrections.bTagCorrections = BTagCorrectionSet::BYEVTWEIGHT;
  cfg.corrections.lightBTagCorrType = DOWN;
  cfg.corrections.heavyBTagCorrType = DOWN;
  #endif


  //get the output name
  TString prefix(fileName);
  prefix.Remove(0,prefix.Last('/') + 1);
  if(prefix.First('.') >= 0) prefix.Resize(prefix.First('.'));
  TString outName = TString::Format("%s_%s.root",prefix.Data(),outPostfix.c_str());

  Copier a(fileName,treeName,outName.Data(),isMC, &cfg);

  a.analyze();
}


/*
 * Running
 * nohup root -b -q '$CMSSW_BASE/src/AnalysisMethods/macros/JetMETStudies/BtagEffTreeMaker.C+("/eos/uscms/store/user/hqu/13TeV/290915/merged/ttbar-powheg-ext_10_ntuple_postproc.root"              )' &
 * For every QCD file
 */
