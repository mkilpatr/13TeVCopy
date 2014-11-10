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
      initilize(cfg,EVTINFO);
      initilize(cfg,AK4JETS);
      initilize(cfg,ELECTRONS);
      initilize(cfg,MUONS);
      initilize(cfg,TAUS);
      book();
    }

    ~TestAnalyzer() {}

    void analyze() {
      fill();
    }

};

DEFINE_FWK_MODULE(TestAnalyzer);
