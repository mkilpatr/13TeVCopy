#include <iostream>

#include "AnalysisBase/Analyzer/interface/PhysicsAnalyzer.h"

using namespace ucsbsusy;
using namespace std;


PhysicsAnalyzer::PhysicsAnalyzer(const edm::ParameterSet& iConfig)
: BaseAnalyzer(iConfig)
, eventInfoSource      (iConfig.getParameter<edm::InputTag  >("eventInfoSource"        ))
, eventWeight_        (1)
, isRealData          (iConfig.getParameter<int             >("isData"                 ))
, globalTag           (iConfig.getParameter<string          >("globalTag"       ).data())
, process             (iConfig.getParameter<string          >("process"         ).data())
, dataset             (iConfig.getParameter<string          >("dataset"         ).data())
, crossSection        (iConfig.getParameter<double          >("crossSection"           ))
, totalEvents         (iConfig.getParameter<int             >("totalEvents"            ))
, crossSectionScaling (iConfig.getParameter<double          >("crossSectionScaling"    ))

{

  //-- Dataset info -----------------------------------------------------------
  if(globalTag.Length())
  clog << " ++  globalTag           = " << globalTag                      << endl;
  clog << " ++  dataset             = ";
  if (dataset.Length()) {
    clog << dataset << " (" << process << ")  -->  cross-section = ";
    if      (isRealData != 0)     clog << "(n/a)";
    else if (crossSection < 0)    clog << "(unknown)";
    else                          clog << crossSection << " pb";
  }
  else clog << "(n/a)";
  if      (isRealData == 1)       clog << "  ...  \033[31mDATA\033[0m";
  else if (isRealData == 0)       clog << "  ...  \033[1;34mMC\033[0m";
  clog << endl;

  clog << " ++  totalEvents         = " << totalEvents                  << endl;
  clog << " ++  crossSectionScaling = ";
  if (isRealData != 0) {
    const_cast<double&>( crossSectionScaling )  = -9;
    clog << "(n/a)";
  }
  else if (crossSectionScaling > 0) {
    clog << TString::Format("%.4g/fb  -->  ", crossSectionScaling);
    clog << TString::Format("%.4g", 1000 * crossSectionScaling * crossSection / totalEvents);
  } else                          clog << "(n/a)";
  clog << endl;

}

bool PhysicsAnalyzer::filter(edm::Event& iEvent, const edm::EventSetup& iSetup){
  static bool               hasEventInfo  = true;
  eventWeight_              = tryToGet(iEvent,eventInfoSource, eventInfo,numAnalyzed(),hasEventInfo)
                            ? eventInfo->weight()
                            : 1
                            ;
  bool                      canBeNegative = eventWeight_ < 0 ? true : false;

  if (crossSectionScaling > 0 && isMC()) {
    eventWeight_         *= 1000 * crossSectionScaling * crossSection / totalEvents;

    if(!canBeNegative) assert(eventWeight_ > 0);
  }

  return BaseAnalyzer::filter(iEvent,iSetup);
}


//_____________________________________________________________________________
bool PhysicsAnalyzer::isData() const
{
  if (isRealData < 0)
    throw cms::Exception("PhysicsAnalyzer.isData()", "isRealData not set -- This should only be used if the sample information has been loaded from the database, or when processing the first event.");
  return  isRealData;
}

//_____________________________________________________________________________
bool PhysicsAnalyzer::isMC() const
{
  if (isRealData < 0)
    throw cms::Exception("PhysicsAnalyzer.isData()", "isRealData not set -- This should only be used if the sample information has been loaded from the database, or when processing the first event.");
  return !isRealData;
}