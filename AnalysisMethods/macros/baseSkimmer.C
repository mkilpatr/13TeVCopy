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

using namespace std;
using namespace ucsbsusy;

class Copier : public TreeCopierLoadedBranches {
public:
  Copier(string fileName, string treeName, string outFileName, bool isMCTree) : TreeCopierLoadedBranches(fileName,treeName,outFileName,isMCTree) {};
  virtual ~Copier() {};

  virtual void loadVariables(){
    load(EVTINFO);
    load(AK4JETS,JetReader::LOADRECO | JetReader::LOADGEN | JetReader::LOADJETSHAPE | JetReader::FILLOBJ);
    load(GENPARTICLES);
  }

  virtual bool fillEvent() {

    //Require one and only one boson
    std::vector<const GenParticleF*> bosons;
    for(const auto* p : genParts ){
      if(ParticleInfo::isEWKBoson(p->pdgId()))
        bosons.push_back(p);
    }
    assert(bosons.size() == 1);

    //Require boson pT to be >= 150 GeV
    if(bosons.front()->pt() < 150) return false;

    //Require at least two jets, with pT > 170 GeV, that are > .4 away from the boson
    int nJ70 = 0;
    for(const auto* j : jets){
      if(j->pt() < 70 || TMath::Abs(j->eta()) > 2.4) continue;
      if(PhysicsUtilities::deltaR2(*bosons.front(),*j) < .16 ) continue;
      nJ70++;
    }
    if(nJ70 < 2) return false;

    //Don't need to set every event because the values never change...just use the defaults!
//    iWeight      = data.add<float>       ("","weight"      ,"F",0);
//    iProcess     = data.add<size8>       ("","process"     ,"b",0);

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

void baseSkimmer(string fileName, string processName, double crossSection, double lumi = 5, double nEvents = -1,  string treeName = "TestAnalyzer/Events", string outPostfix ="skimmed") {

  //get the output name
  TString prefix(fileName);
  prefix.Remove(0,prefix.Last('/') + 1);
  if(prefix.First('.') >= 0) prefix.Resize(prefix.First('.'));
  TString outName = TString::Format("%s_%s.root",prefix.Data(),outPostfix.c_str());

  //get the process
  defaults::Process process = defaults::NUMPROCESSES;
  for(unsigned int iP = 0; defaults::PROCESS_NAMES[iP][0]; ++iP) if(defaults::PROCESS_NAMES[iP] == processName) process = static_cast<defaults::Process>(iP);
  if(process == defaults::NUMPROCESSES) throw std::invalid_argument("Did not provide a valid process name (see defaults::PROCESS_NAMES)");

  Copier a(fileName,treeName,outName.Data(),process != defaults::DATA);
  //set weight and process
  a.weight = lumi * crossSection / ( nEvents > 0 ? nEvents : float(a.getEntries())  );
  a.process = process;

  clog << "Skimming "<< a.getEntries() <<" events of type " <<  defaults::PROCESS_NAMES[a.process] <<" and weight "<< a.weight <<" into file "<< outName << endl;
  a.analyze();
}
