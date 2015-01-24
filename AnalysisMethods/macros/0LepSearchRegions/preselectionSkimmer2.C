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

class Copier : public TreeCopierLoadedBranches {
public:
  ParamatrixMVA* paramRec;
  Copier(string fileName, string treeName, string outFileName, bool isMCTree, ConfigPars* pars) : TreeCopierLoadedBranches(fileName,treeName,outFileName,isMCTree,pars) {
    TFile* recoFile = TFile::Open("/uscms/home/mullin/nobackup/stuff2015/quarkGluonTagger/QGDisc_reco_puppi.root", "READ");
    paramRec = dynamic_cast<ParamatrixMVA*>(recoFile->Get("QG_0"));
    delete recoFile;

  };
  virtual ~Copier() {};

  virtual void loadVariables(){
    load(EVTINFO);
    load(AK4JETS,JetReader::LOADRECO | JetReader::LOADGEN | JetReader::LOADTOPASSOC | JetReader::LOADJETSHAPE | JetReader::FILLOBJ);
    load(ELECTRONS);
    load(MUONS);
    load(TAUS);
    load(GENPARTICLES);
    setDefaultJets(AK4JETS);
  }

  double getRecoMVA(const RecoJetF& jet, const JetReader * reader){
    const Panvariate* recoMVA = paramRec->get(jet.pt(),TMath::Abs(jet.eta()));
    if(!recoMVA) return -1;
    static const int reco_ak4pfchs_jet_ptD     = recoMVA->findVariable("ak4pfpuppi_jet_ptD"    );
    static const int reco_ak4pfchs_jet_axis1   = recoMVA->findVariable("ak4pfpuppi_jet_axis1"  );
    static const int reco_ak4pfchs_jet_axis2   = recoMVA->findVariable("ak4pfpuppi_jet_axis2"  );
    static const int reco_ak4pfchs_jet_jetMult = recoMVA->findVariable("ak4pfpuppi_jet_jetMult");
    recoMVA->setVariable(reco_ak4pfchs_jet_ptD    ,reader->jetptD_->at(jet.index())  );
    recoMVA->setVariable(reco_ak4pfchs_jet_axis1  ,reader->jetaxis1_->at(jet.index()));
    recoMVA->setVariable(reco_ak4pfchs_jet_axis2  ,reader->jetaxis2_->at(jet.index()));
    recoMVA->setVariable(reco_ak4pfchs_jet_jetMult,reader->jetMult_->at(jet.index()));
    return recoMVA->evaluateMethod(0);
  } //

  virtual bool fillEvent() {
    if(nLeptons == 0) return false;
    if(nLeptons > 2) return false;
    if(nJets < 4) return false;

    if(nLeptons == 2){
      met->setP4( met->p4() + leptons[1]->p4());
      evtInfoReader.met_phi = met->phi();
      evtInfoReader.met_pt = met->pt();
    }
    if(met->pt() < 150) return false;

    if(PhysicsUtilities::absDeltaPhi(*leptons[0], *met) <= 1) return false;




//    //lepton veto
//    if(nLeptons != 1) return false;
//
//    //Trigger requirements
//    if(met->pt() < 150) return false;
//
//    //5 jet requirement
//    if(nJets < 3) return false;
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

void preselectionSkimmer2(string fileName, string processName, double crossSection, double lumi = 5, double nEvents = -1,  string treeName = "TestAnalyzer/Events", string outPostfix ="skimmedPuppi") {

  ConfigPars pars;
  pars.cleanJetsvLeptons_ = true;
  pars.cleanJetsvTaus_ = true;


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
  a.weight = lumi * 1000*crossSection / ( nEvents > 0 ? nEvents : float(a.getEntries())  );
  a.process = process;

  clog << "Skimming "<< a.getEntries() <<" events of type " <<  defaults::PROCESS_NAMES[a.process] <<" and weight "<< a.weight <<" into file "<< outName << endl;
  a.analyze();
}
