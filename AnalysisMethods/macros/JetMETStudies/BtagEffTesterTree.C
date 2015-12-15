#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"


using namespace std;
using namespace ucsbsusy;


class Copier : public TreeCopierManualBranches {
public:
  Copier(string fileName, string treeName, string outFileName, bool isMCTree, cfgSet::ConfigSet * pars) : TreeCopierManualBranches(fileName,treeName,outFileName,isMCTree,pars),
  i_process     (0),
  i_passBaseline(0),
  i_weight      (0),
  i_puWeight    (0),
  i_btWeight    (0),
  i_btUlWeight  (0),
  i_btDlWeight  (0),
  i_btUHWeight  (0),
  i_btDHWeight  (0),
  i_nBL         (0),
  i_nBM         (0),
  i_nBT         (0)
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

  void rankedByCSV(vector<RecoJetF*> inJets, vector<RecoJetF*>& outJets) {
    outJets.clear();
    outJets.resize(inJets.size());
    vector<pair<double,int> > rankedJets(inJets.size());

    for(unsigned int iJ =0; iJ < inJets.size(); ++iJ){
      rankedJets[iJ].first = inJets[iJ]->csv();
      rankedJets[iJ].second = iJ;
    }

    std::sort(rankedJets.begin(),rankedJets.end(),PhysicsUtilities::greaterFirst<double,int>());
    for(unsigned int iJ =0; iJ < inJets.size(); ++iJ) {
      outJets[iJ] = inJets[rankedJets[iJ].second];
    }
  }

  virtual bool fillEvent() {
    if(!goodvertex) return false;

    bool passBaseline = true;
    if(met->pt() <= 200) passBaseline = false;
    if(nJets < 2 || jets[1]->pt() < 75 ) passBaseline = false;
    if(nJets < 5) passBaseline = false;
    if(nBJets < 1) passBaseline = false;
    if(nVetoedTracks > 0) passBaseline = false;
    if(nSecondaryLeptons > 0) passBaseline = false;

    float dphij1met = 0.0, dphij2met = 0.0, dphij12met = 0.0, dphij3met = 0.0;
    if(jets.size() > 0) {
      dphij1met = fabs(PhysicsUtilities::deltaPhi(*jets[0], *met));
    }
    if(jets.size() > 1) {
      dphij2met = fabs(PhysicsUtilities::deltaPhi(*jets[1], *met));
    }
    dphij12met = std::min(dphij1met,dphij2met);
    if(jets.size() > 2) {
      dphij3met = fabs(PhysicsUtilities::deltaPhi(*jets[2], *met));
    }
    if(dphij12met <= 1.0) passBaseline = false;
    if(dphij3met <= .5) passBaseline = false;

    int nlbjets = 0;
    for(auto* j : jets) {
      if(j->csv() > defaults::CSVValues[defaults::CSVL])  nlbjets++;
    }
    if(nlbjets < 2) passBaseline = false;

    vector<RecoJetF*> jetsCSVranked;
    rankedByCSV(jets,jetsCSVranked);
    float mtcsv1met = 0.0, mtcsv2met = 0.0;
    if(jetsCSVranked.size() > 0) {
      mtcsv1met = JetKinematics::transverseMass(*jetsCSVranked[0], *met);
    }
    if (jetsCSVranked.size() > 1) {
      mtcsv2met = JetKinematics::transverseMass(*jetsCSVranked[1], *met);
    }
    if(min(mtcsv1met,mtcsv2met) <= 175 ) passBaseline = false;


    data.fill<unsigned int>(i_process,process);
    data.fill<bool>(i_passBaseline,passBaseline);
    data.fill<float>(i_weight,weight);
    data.fill<float>(i_puWeight,eventCorrections.getPUWeight());
    data.fill<float>(i_btWeight,bTagCorrections.getBTagByEvtWeight());

    bool isTTBARLike = false;
    if(process == defaults::TTBAR || process == defaults::SINGLE_T || process == defaults::TTZ || process == defaults::TTW )
      isTTBARLike = true;

    data.fill<float>(i_btUlWeight,bTagCorrections.getBTagByEvtWeightCorrector()->getEvtWeight(jets,UP,configSet.corrections.heavyBTagCorrType,isTTBARLike));
    data.fill<float>(i_btDlWeight,bTagCorrections.getBTagByEvtWeightCorrector()->getEvtWeight(jets,DOWN,configSet.corrections.heavyBTagCorrType,isTTBARLike));
    data.fill<float>(i_btUHWeight,bTagCorrections.getBTagByEvtWeightCorrector()->getEvtWeight(jets,configSet.corrections.lightBTagCorrType,UP,isTTBARLike));
    data.fill<float>(i_btDHWeight,bTagCorrections.getBTagByEvtWeightCorrector()->getEvtWeight(jets,configSet.corrections.lightBTagCorrType,DOWN,isTTBARLike));

    int nBL = 0;
    int nBM = 0;
    int nBT = 0;

    for(const auto * j : jets){
      if(j->csv() > defaults::CSVValues[defaults::CSVL]) nBL++;
      if(j->csv() > defaults::CSVValues[defaults::CSVM]) nBM++;
      if(j->csv() > defaults::CSVValues[defaults::CSVT]) nBT++;
    }


    data.fill<int>(i_nBL,nBL);
    data.fill<int>(i_nBM,nBM);
    data.fill<int>(i_nBT,nBT);

    outFile_->cd();
    treeWriter_->fill();

    return true;
  }

  void book() {
    i_process       = data.add<unsigned int>("","process"                      ,"i",0);
    i_passBaseline  = data.add<bool>("","passBaseline"                      ,"O",0);
    i_weight        = data.add<float>("","weight"                      ,"F",0);
    i_puWeight      = data.add<float>("","puWeight"                      ,"F",0);
    i_btWeight      = data.add<float>("","btWeight"                      ,"F",0);
    i_btUlWeight    = data.add<float>("","btUlWeight"                      ,"F",0);
    i_btDlWeight    = data.add<float>("","btDlWeight"                      ,"F",0);
    i_btUHWeight    = data.add<float>("","btUHWeight"                      ,"F",0);
    i_btDHWeight    = data.add<float>("","btDHWeight"                      ,"F",0);
    i_nBL           = data.add<int>("","nBL"                      ,"I",0);
    i_nBM           = data.add<int>("","nBM"                      ,"I",0);
    i_nBT           = data.add<int>("","nBT"                      ,"I",0);



  }


  //event level info
  size i_process     ;
  size i_passBaseline;
  size i_weight      ;
  size i_puWeight    ;
  size i_btWeight    ;
  size i_btUlWeight    ;
  size i_btDlWeight    ;
  size i_btUHWeight    ;
  size i_btDHWeight    ;
  size i_nBL         ;
  size i_nBM         ;
  size i_nBT         ;


};


#endif

void BtagEffTesterTree(string fileName,  string treeName = "Events", string outPostfix ="btagEff", bool isMC = true) {

  cfgSet::loadDefaultConfigurations();
  cfgSet::ConfigSet cfg = cfgSet::zl_search_set;

  cfg.corrections.bTagCorrections = BTagCorrectionSet::BYEVTWEIGHT;
  cfg.corrections.lightBTagCorrType = NOMINAL;
  cfg.corrections.heavyBTagCorrType = NOMINAL;
  cfg.corrections.eventCorrectionFile =  TString::Format("%s/src/data/corrections/eventCorr_allData.root",cfgSet::CMSSW_BASE);


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
