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
#include "CORRALHelper.h"

using namespace std;
using namespace ucsbsusy;

class Copier : public TreeCopierAllBranches {
public:
  Copier(string fileName, string treeName, string outFileName, bool isMCTree, ConfigPars * pars = 0) : TreeCopierAllBranches(fileName,treeName,outFileName,isMCTree,pars) {
  };
  virtual ~Copier() {};


  void loadVariables(){
    load(EVTINFO);
    load(PICKYJETS,JetReader::FILLOBJ | JetReader::LOADGEN | JetReader::LOADRECO | JetReader::LOADTOPASSOC | JetReader::LOADJETSHAPE );
    load(GENPARTICLES, GenParticleReader::FILLOBJ | GenParticleReader::LOADPARTONDECAY);
  }


  virtual bool fillEvent() {
//    ParticleInfo::printGenInfo(genParticleReader.genParticles,-1);
    auto * jetReader = &pickyJetReader;
        std::vector<RecoJetF*> recoJets;
        std::vector<TopDecayEvent::DecayID> decays;
        if(!setup(&genParticleReader,jetReader, recoJets,decays)) return false;

    return true;
  }

  void book() {
  }

};


#endif


void CORRALTrainingSkimmer(string fileName, string treeName = "TestAnalyzer/Events", string outPostfix ="skimmed") {
  //get the output name
  TString prefix(fileName);
  prefix.Remove(0,prefix.Last('/') + 1);
  if(prefix.First('.') >= 0) prefix.Resize(prefix.First('.'));
  TString outName = TString::Format("%s_%s.root",prefix.Data(),outPostfix.c_str());
  Copier a(fileName,treeName,outName.Data(),true);
//a.reader.eventNumber = 6430000;
  a.analyze();
}
