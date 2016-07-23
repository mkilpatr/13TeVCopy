#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/Utilities/interface/PartonMatching.h"


using namespace std;
using namespace ucsbsusy;

//#define TESTMODE

class Copier : public TreeCopierManualBranches {
public:
  Copier(TString fileName, TString treeName, TString outFileName, size rSeed, bool isMCTree, cfgSet::ConfigSet * pars) : TreeCopierManualBranches(fileName,treeName,outFileName,rSeed,isMCTree,pars),
  i_process (0),
  i_sigType (0),
  i_mass1   (0),
  i_mass2   (0),
  i_mass3   (0),
  i_stdCorr (0),
  i_upCorr  (0),
  i_downCorr(0),
  i_njets(0)
  {
  };
  virtual ~Copier() {};

  virtual void loadVariables() override{
    load(cfgSet::EVTINFO);
    load(cfgSet::AK4JETS,JetReader::LOADRECO | JetReader::FILLOBJ | JetReader::LOADGEN | JetReader::LOADTOPASSOC);
    load(cfgSet::GENPARTICLES, GenParticleReader::LOADPARTONDECAY | GenParticleReader::FILLOBJ);
  }

  int getNISRJets(const BaseTreeAnalyzer * ana) const {
    //hard coded to match derivation
    const double minJetPT = 30;
    const double maxJetETA = 2.4;

    int nISRJets = 0;
    //Please forgive me for what I am about to do
    auto& recoJetReader = *const_cast<JetReader*>(&ana->ak4Reader);
    auto& genPartReader = *const_cast<GenParticleReader*>(&ana->genParticleReader);

    std::vector<GenJetF*> genJets;
    std::vector<ucsbsusy::RecoJetF*> recoJets;
    std::vector<PartonMatching::DecayID> decays;

    recoJets.clear();
    decays.clear();
    std::vector<int> genIDX;
    for(auto& j : recoJetReader.recoJets){
      if(j.pt() < minJetPT) continue;
      if(j.absEta() > maxJetETA) continue;
      if(ana->process != defaults::SIGNAL && !j.looseid()) continue;
      recoJets.push_back(&j);
      genIDX.push_back(-1);
      if(j.genJet() == 0)  continue;
      genIDX.back() = genJets.size();
      genJets.push_back(j.genJet());
    }

    PartonMatching::PartonEvent * partonEvent = new PartonMatching::PartonEvent(genPartReader,recoJetReader,genJets);
    partonEvent->processSubtractedJetPTs();

    for(unsigned int iJ = 0; iJ < recoJets.size(); ++iJ){
      if(genIDX[iJ] < 0 ){
        nISRJets++;
        continue;
      }
      double newGenJETPT = partonEvent->subtractedJetPTs[genIDX[iJ]];
      double originalGENJetPT = genJets[genIDX[iJ]]->pt();
      double recoCF = originalGENJetPT == 0 ? 1 : newGenJETPT/originalGENJetPT;
      if(recoJets[iJ]->pt()*recoCF >= minJetPT) nISRJets++;
    }


    for(unsigned int iJ = 0; iJ < recoJets.size(); ++iJ){

      double newJetPT = recoJets[iJ]->pt();
      if(genIDX[iJ] >= 0){
        double newGenJETPT = partonEvent->subtractedJetPTs[genIDX[iJ]];
        double originalGENJetPT = genJets[genIDX[iJ]]->pt();
        double recoCF = originalGENJetPT == 0 ? 1 : newGenJETPT/originalGENJetPT;
        newJetPT*= recoCF;
      }
      cout << recoJets[iJ]->p4() <<" " << newJetPT <<" "<< (genIDX[iJ] < 0 ? 0.0 : genJets[genIDX[iJ]]->pt() )
    <<" "<< (genIDX[iJ] < 0 ? 0.0 : partonEvent->subtractedJetPTs[genIDX[iJ]] ) <<endl;
    }

    delete partonEvent;
    return nISRJets;


  }


  void test() {

    cout <<" -------  "<<endl;
    ParticleInfo::printGenInfo(genParticleReader.genParticles);
    int nISRJets = getNISRJets(this);
    float corr = isrCorrections.getISRCorrector()->getCorrFactor(NOMINAL,nISRJets);
    float corrUp = isrCorrections.getISRCorrector()->getCorrFactor(UP,nISRJets);
    float corrDown = isrCorrections.getISRCorrector()->getCorrFactor(DOWN,nISRJets);
    cout << evtInfoReader.signalType<<  " "<< nISRJets<<" "<<corr<<" "<<corrUp<<" "<<corrDown<<endl;
  }


  //--------------------------------------------------------------------------------------------------
  virtual BaseEventAnalyzer * setupEventAnalyzer() override {return new CopierFillYourselfEventAnalyzer();}

  virtual bool fillEvent() override {
//    test();

    int nISRJets = isrCorrections.getNumberOfISRJets();
    float corr = isrCorrections.getISRCorrector()->getCorrFactor(NOMINAL,nISRJets);
    float corrUp = isrCorrections.getISRCorrector()->getCorrFactor(UP,nISRJets);
    float corrDown = isrCorrections.getISRCorrector()->getCorrFactor(DOWN,nISRJets);


    data.fill<unsigned int>(i_process,process);
    data.fill<unsigned int>(  i_sigType,size(evtInfoReader.signalType));
    data.fill<unsigned int>(  i_njets,size(nISRJets));
    data.fill<float>(  i_mass1 ,evtInfoReader.massparams->size() > 0 ? evtInfoReader.massparams->at(0) : 0 );
    data.fill<float>(  i_mass2 ,evtInfoReader.massparams->size() > 1 ? evtInfoReader.massparams->at(1) : 0);
    data.fill<float>(  i_mass3 ,evtInfoReader.massparams->size() > 2 ? evtInfoReader.massparams->at(2) : 0);
    data.fill<float>(  i_stdCorr,corr);
    data.fill<float>(  i_upCorr,corrUp);
    data.fill<float>(  i_downCorr,corrDown);

    fillFillingTree();
    return true;
  }

  void book() override {
    cout << "a";
    i_process       = data.add<unsigned int>("","process"              ,"i",0);
    i_sigType       = data.add<unsigned int>("","sigType"              ,"i",0);
    i_mass1         = data.add<float>("","mass1"                       ,"F",0);
    i_mass2         = data.add<float>("","mass2"                       ,"F",0);
    i_mass3         = data.add<float>("","mass3"                       ,"F",0);
    i_stdCorr       = data.add<float>("","corrWeight"              ,"F",0);
    i_upCorr        = data.add<float>("","upCorrWeight"            ,"F",0);
    i_downCorr      = data.add<float>("","downCorrWeight"          ,"F",0);
    i_njets         = data.add<unsigned int>("","njets"              ,"i",0);
  }


  //event level info
  size i_process  ;
  size i_sigType  ;
  size i_mass1    ;
  size i_mass2    ;
  size i_mass3    ;
  size i_stdCorr  ;
  size i_upCorr   ;
  size i_downCorr ;
  size i_njets;

};


#endif

void ISRTreeMaker(TString sname = "T2bW",
                             const int fileindex = -1,
                             const bool isMC = true,
                             const TString fname = "/store/user/lpcstop/noreplica/13TeV/120716/signals/merged/T2bW_1000_650_ntuple_postproc.root",
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


    TString treeName = "Events";
    Copier a(fullname,treeName,outfilename.Data(),fileindex +2,isMC, &cfg);
    a.analyze();

}


/*
 * Running
 * nohup root -b -q '$CMSSW_BASE/src/AnalysisMethods/macros/JetMETStudies/BtagEffTreeMaker.C+("/eos/uscms/store/user/hqu/13TeV/290915/merged/ttbar-powheg-ext_10_ntuple_postproc.root"              )' &
 * For every QCD file
 */
