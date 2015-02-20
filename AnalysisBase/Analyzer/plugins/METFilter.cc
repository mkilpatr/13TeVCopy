#include "AnalysisBase/Analyzer/interface/PhysicsAnalyzer.h"


using namespace ucsbsusy;
using namespace std;

class METFilter : public PhysicsAnalyzer {
  const double minMET;
  public:
    FILTER_MODE
    METFilter(const edm::ParameterSet &cfg) :PhysicsAnalyzer(cfg), minMET(cfg.getParameter<double>("minMET"))
    {
      initialize(cfg, "EventInfo", EVTINFO);
    }

    ~METFilter() {}

    bool filter() override {
      if(eventInfo->met()->pt() < minMET) return false;
      return true;
    }

};

DEFINE_FWK_MODULE(METFilter);
