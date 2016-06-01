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
      initialize(cfg, "EventInfo", EVTINFO);
      if(isMC()) initialize(cfg, "Gen", GENPARTICLES);
      initialize(cfg, "Jets", AK4JETS);
      initialize(cfg, "PuppiJets", PUPPIJETS);
      initialize(cfg, "PickyJets", PICKYJETS);
      initialize(cfg, "CASubJets", CASUBJETS);
      initialize(cfg, "Electrons", ELECTRONS);
      initialize(cfg, "Muons", MUONS);
      initialize(cfg, "Taus", TAUS);
      initialize(cfg, "Photons", PHOTONS);
      initialize(cfg, "PFCandidates", PFCANDS);
      initialize(cfg, "CMSTops", CMSTOPS);
      initialize(cfg, "AK8FatJets", AK8FATJETS);
      initialize(cfg, "AK8PuppiFatJets", AK8PUPPIFATJETS);
      initialize(cfg, "Triggers", TRIGGERS);
      initialize(cfg, "METFilters", METFILTERS);
      initialize(cfg, "SVs", SV);
      book();
    }

    ~TestAnalyzer() {}

    void analyze() {
      fill();
    }

};

DEFINE_FWK_MODULE(TestAnalyzer);
