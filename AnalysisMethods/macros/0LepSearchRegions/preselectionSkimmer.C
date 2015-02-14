/*
 * Uses the tree copier to copy over a tree!
 *
 * We use the loaded branches option so that we make the tree smaller
 *
 * Use this guy as a base class or an example!
 *
 */
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisTools/TreeReader/interface/Defaults.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/Parang/interface/Panvariate.h"

using namespace std;
using namespace ucsbsusy;

class Copier : public TreeCopierAllBranches {
public:
  ParamatrixMVA* paramRec;
  bool jetNoLep;
  Copier(string fileName, string treeName, string outFileName, bool isMCTree, ConfigPars * pars) : TreeCopierAllBranches(fileName,treeName,outFileName,isMCTree,pars) {
    TFile* recoFile = TFile::Open("/uscms/home/mullin/nobackup/stuff2015/quarkGluonTagger/QGDisc_reco_ak4.root", "READ");
    paramRec = dynamic_cast<ParamatrixMVA*>(recoFile->Get("QG_0"));
    delete recoFile;
  };
  virtual ~Copier() {};

  virtual void loadVariables(){
    load(EVTINFO);
    load(AK4JETS,JetReader::LOADRECO | JetReader::LOADJETSHAPE | JetReader::FILLOBJ);
    load(PICKYJETS,JetReader::LOADRECO | JetReader::LOADJETSHAPE | JetReader::FILLOBJ);
  }

  double getRecoMVA(const RecoJetF& jet, const JetReader * reader){
    const Panvariate* recoMVA = paramRec->get(jet.pt(),TMath::Abs(jet.eta()));
    if(!recoMVA) return -1;
    static const int reco_ak4pfchs_jet_ptD     = recoMVA->findVariable("ak4pfchs_jet_ptD"    );
    static const int reco_ak4pfchs_jet_axis1   = recoMVA->findVariable("ak4pfchs_jet_axis1"  );
    static const int reco_ak4pfchs_jet_axis2   = recoMVA->findVariable("ak4pfchs_jet_axis2"  );
    static const int reco_ak4pfchs_jet_jetMult = recoMVA->findVariable("ak4pfchs_jet_jetMult");
    recoMVA->setVariable(reco_ak4pfchs_jet_ptD    ,reader->jetptD_->at(jet.index())  );
    recoMVA->setVariable(reco_ak4pfchs_jet_axis1  ,reader->jetaxis1_->at(jet.index()));
    recoMVA->setVariable(reco_ak4pfchs_jet_axis2  ,reader->jetaxis2_->at(jet.index()));
    recoMVA->setVariable(reco_ak4pfchs_jet_jetMult,reader->jetMult_->at(jet.index()));
    return recoMVA->evaluateMethod(0);
  } //

  virtual bool fillEvent() {

    //Trigger requirements
    if(PhysicsUtilities::countObjectsDeref(jets,90,2.4) < 2) return false;
    if(met->pt() < 200) return false;

    //Tight b-tag requirement
    int nTAK4   = 0;
    int nTPICKY = 0;
    for(auto* j : bJets) if(isTightBJet(*j)) nTAK4++;

    vector<RecoJetF*> pickyJets;
    vector<RecoJetF*> pickyBJets;
    vector<RecoJetF*> pickyNonBJets;
    cleanJets(&pickyJetReader,pickyJets,&pickyBJets,&pickyNonBJets);
    for(auto* j : pickyBJets) if(isTightBJet(*j)) nTPICKY++;

    if(nTPICKY + nTAK4 == 0) return false;

    //5 jet requirement
    if(nJets < 4 && pickyJets.size() < 4) return false;

    //replace old qgl with new one
    for(unsigned int iJ = 0; iJ < ak4Reader.recoJets.size(); ++iJ){
      const auto& j =  ak4Reader.recoJets[iJ];
      ak4Reader.jetqgl_->at(j.index()) = getRecoMVA(j,&ak4Reader);
    }

    return true;
  }

  void book() {
    iWeight      = data.add<float>       ("","weight"      ,"F",weight);
    iProcess     = data.add<size8>       ("","process"     ,"b",process);
  }
  size iWeight;
  size iProcess;

  defaults::Process process;
  float weight;

};


#endif

/*
 * process -> string from defaults::PROCESS_NAMES
 * crossSection -> processCrossSection
 * lumi -> luminosity that you wish to scale for
 * nEvents -> number of events in sample (if < 0 take it from the tree)
 *
 */

void preselectionSkimmer(string fileName, string processName, double crossSection, double lumi = 4, double nEvents = -1,  string treeName = "TestAnalyzer/Events", string outPostfix ="skimmed") {
  BaseTreeAnalyzer::ConfigPars pars;
  pars.cleanJetsvSelectedLeptons_ = false;
  pars.leptonSelection = BaseTreeAnalyzer::SEL_0_LEP;
  pars.defaultJetCollection = BaseTreeAnalyzer::AK4JETS;

  //get the output name
  TString prefix(fileName);
  prefix.Remove(0,prefix.Last('/') + 1);
  if(prefix.First('.') >= 0) prefix.Resize(prefix.First('.'));
  TString outName = TString::Format("%s_%s.root",prefix.Data(),outPostfix.c_str());

  //get the process
  defaults::Process process = defaults::NUMPROCESSES;
  for(unsigned int iP = 0; defaults::PROCESS_NAMES[iP][0]; ++iP) if(defaults::PROCESS_NAMES[iP] == processName) process = static_cast<defaults::Process>(iP);
  if(process == defaults::NUMPROCESSES) throw std::invalid_argument("Did not provide a valid process name (see defaults::PROCESS_NAMES)");

  Copier a(fileName,treeName,outName.Data(),process != defaults::DATA, &pars);
  //set weight and process
  a.weight = lumi * crossSection *1000 / ( nEvents > 0 ? nEvents : float(a.getEntries())  );
  a.process = process;

  clog << "Skimming "<< a.getEntries() <<" events of type " <<  defaults::PROCESS_NAMES[a.process] <<" and weight "<< a.weight <<" into file "<< outName << endl;
  a.analyze();
}
