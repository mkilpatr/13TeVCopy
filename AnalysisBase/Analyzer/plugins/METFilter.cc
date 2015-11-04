#include "AnalysisBase/Analyzer/interface/PhysicsAnalyzer.h"


using namespace ucsbsusy;
using namespace std;

class METFilter : public PhysicsAnalyzer {
  const double minMET;
  const double maxMET;
  const bool verbose;
  public:
    FILTER_MODE
    METFilter(const edm::ParameterSet &cfg) :PhysicsAnalyzer(cfg), minMET(cfg.getParameter<double>("minMET")), maxMET(cfg.getParameter<double>("maxMET")), verbose(cfg.getParameter<bool>("verbose"))
    {
      initialize(cfg, "EventInfo", EVTINFO);
    }

    ~METFilter() {}

    bool filter() override {

      bool pass = true;

      if(minMET > 0 && eventInfo->met()->pt() < minMET) pass = false;
      if(maxMET > 0 && eventInfo->met()->pt() > maxMET) pass = false;

      if(verbose && !pass) clog << " ++   ERROR!!!! You have a MET of "<< eventInfo->met()->pt() <<"! The event will be skipped!" << endl;

      return pass;
    }

};

DEFINE_FWK_MODULE(METFilter);
