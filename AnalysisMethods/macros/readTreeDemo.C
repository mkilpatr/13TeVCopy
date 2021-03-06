#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#endif

using namespace std;
using namespace ucsbsusy;

// Like a CMSSW filter, all of the processing has been setup for you if you inherit from BaseTreeAnalyzer
// you can override te three functions shown below if you want to change the default behavior
// no matter what, you have to define a runEvent function....to do what you want to do
class Analyze : public BaseTreeAnalyzer{
public:
  Analyze(TString fname, string treeName, bool isMCTree, cfgSet::ConfigSet * cfg) : BaseTreeAnalyzer(fname,treeName, isMCTree, cfg)
  {}

//  Override this function to change the way the analyzer loads default variable types
//  void  load(VarType type, int options = -1, std::string branchName = "" )
//  {}

//  Override this function to choose a different set of variables to load (run once per job)
//  void loadVariables()
//  {}

//  Override this function to change the default way that variables are processed (run once every event)
//  void processVariables()
//  {}

///Analysis code ran in every event is put here .... must be defined
  void runEvent(){
    cout << "MET= " << met->pt() << ": number of vertices = " << nPV << endl;
    for(const auto* jet : (jets)){
      cout << "Jet " << ": pt = " << jet->pt() << "; eta = " << jet->eta() << "; phi = " << jet->phi()<<endl;
    }
    for(const auto* lepton : (selectedLeptons)){
      cout << "Lepton " << ": pt = " << lepton->pt() << "; eta = " << lepton->eta() << "; phi = " << lepton->phi() << endl;
    }

    /*for(const auto* tau : (taus)){
            bool passantimutight = (tau->hpsid() & kTightMuMVA);
            bool passantielemedium = (tau->hpsid() & kMediumEleMVA);
            bool passmvaloose = (tau->hpsid() & kLooseIsoMVALT);
            cout << "Tau " <<": pt = " << tau->pt() << "; eta = " << tau->eta() << "; phi = " << tau->phi() <<  "; dxysig = " << tau->dxysig() << "; anti-muon tight rejection = " << passantimutight << "; anti-tau medium rejection = " << passantielemedium << "; pass mva iso (with lifetime) loose = " << passmvaloose << "; passes POG selection? " << tau->isgoodpogtau()  << endl;
    }*/
  }
};


// This part is just a wrapper, you just need to construct your class and
// call the analyze() function
void readTreeDemo(string fname = "$CMSSW_BASE/src/AnalysisBase/Analyzer/test/evttree_numEvent1000.root", string treeName = "TestAnalyzer/Events", bool isMCTree = false) {
  cfgSet::loadDefaultConfigurations();
  cfgSet::ConfigSet cfg = cfgSet::ol_search_set;
  Analyze a(fname, treeName, isMCTree,&cfg);
  a.analyze();
}
