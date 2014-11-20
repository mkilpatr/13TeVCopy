//--------------------------------------------------------------------------------------------------
// 
// TestAnalyzer
// 
// Test class to try out event analysis modules. The information you want to analyze and store in the output TTree is configurable and implemented by means of the different Filler analyzer classes. Filtering functionality to be added. To be tested with MiniAOD for now. Sophisticated data management methods to be added later :-)
// 
// TestAnalyzer.cc created on Fri Aug 15 14:57:13 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisBase/Analyzer/interface/PhysicsAnalyzer.h"


using namespace ucsbsusy;
using namespace std;

class TestAnalyzer : public PhysicsAnalyzer {

  public:
    ANALYZER_MODE
    TestAnalyzer(const edm::ParameterSet &cfg) : PhysicsAnalyzer(cfg)
    {
      addFiller(cfg, "EventInfo", EVTINFO);
      addFiller(cfg, "Jets", AK4JETS);
      addFiller(cfg, "PuppiJets", PUPPIJETS);
      addFiller(cfg, "Electrons", ELECTRONS);
      addFiller(cfg, "Muons", MUONS);
      addFiller(cfg, "Taus", TAUS);
      if(isMC()) addFiller(cfg, "Gen", GENPARTICLES);
      book();
    }

    ~TestAnalyzer() {}

    // check for a named parameter set in cfg and activate corresponding filler
    void addFiller(const edm::ParameterSet &cfg, const string psetName, VarType type, int options=-1, string branchName="") {
      if(cfg.existsAs<edm::ParameterSet>(psetName, false)) {
        edm::ParameterSet fcfg(cfg.getUntrackedParameter<edm::ParameterSet>(psetName));   // parameter set for this filler
        bool fill = fcfg.getUntrackedParameter<bool>("isFilled");                         // to fill or not to fill
        if(fill) initialize(fcfg, type, options, branchName);
      }
    }

    void analyze() {
      fill();
    }

};

DEFINE_FWK_MODULE(TestAnalyzer);
