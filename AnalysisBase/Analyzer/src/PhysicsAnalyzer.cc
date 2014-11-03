#include <iostream>

#include "AnalysisBase/Analyzer/interface/PhysicsAnalyzer.h"
#include "AnalysisBase/Analyzer/interface/FileUtilities.h"

using namespace ucsbsusy;
using namespace std;


//--------------------------------------------------------------------------------------------------
PhysicsAnalyzer::PhysicsAnalyzer(const edm::ParameterSet& iConfig)
: BaseAnalyzer(iConfig)
, event_(0)
, genEventInfoSource_ (iConfig.getParameter<edm::InputTag  >("genEventInfoSource"      ))
, eventWeight_        (1)
, isRealData          (iConfig.getParameter<int             >("isData"                 ))
, globalTag           (iConfig.getParameter<string          >("globalTag"       ).data())
, process             (iConfig.getParameter<string          >("process"         ).data())
, dataset             (iConfig.getParameter<string          >("dataset"         ).data())
, crossSection        (iConfig.getParameter<double          >("crossSection"           ))
, totalEvents         (iConfig.getParameter<int             >("totalEvents"            ))
, crossSectionScaling (iConfig.getParameter<double          >("crossSectionScaling"    ))
// ---- Configure event information
, eventInfo           (iConfig)
, jets                (iConfig, isMC())
, muons               (iConfig, isMC())
, electrons           (iConfig, isMC())
, taus                (iConfig)

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

//--------------------------------------------------------------------------------------------------
PhysicsAnalyzer::~PhysicsAnalyzer() {}

//--------------------------------------------------------------------------------------------------
void PhysicsAnalyzer::beginJob() {}

//--------------------------------------------------------------------------------------------------
bool PhysicsAnalyzer::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  static bool               hasEventInfo  = true;
  eventWeight_              = FileUtilities::tryToGet(iEvent,genEventInfoSource_, genEventInfo,numAnalyzed(),hasEventInfo)
                            ? genEventInfo->weight()
                            : 1
                            ;
  bool                      canBeNegative = eventWeight_ < 0 ? true : false;

  if (crossSectionScaling > 0 && isMC()) {
    eventWeight_         *= 1000 * crossSectionScaling * crossSection / totalEvents;

    if(!canBeNegative) assert(eventWeight_ > 0);
  }

  event_ = &iEvent;

  return BaseAnalyzer::filter(iEvent,iSetup);
}


//--------------------------------------------------------------------------------------------------
bool PhysicsAnalyzer::isData() const
{
  if (isRealData < 0)
    throw cms::Exception("PhysicsAnalyzer.isData()", "isRealData not set -- This should only be used if the sample information has been loaded from the database, or when processing the first event.");
  return  isRealData;
}

//--------------------------------------------------------------------------------------------------
bool PhysicsAnalyzer::isMC() const
{
  if (isRealData < 0)
    throw cms::Exception("PhysicsAnalyzer.isData()", "isRealData not set -- This should only be used if the sample information has been loaded from the database, or when processing the first event.");
  return !isRealData;
}

//--------------------------------------------------------------------------------------------------
void PhysicsAnalyzer::book(BaseFiller* filler)
{
  filler->book(*treeWriter());
}

//--------------------------------------------------------------------------------------------------
void PhysicsAnalyzer::loadObj(BaseFiller* filler)
{
  filler->load(*event_);
}

//--------------------------------------------------------------------------------------------------
void PhysicsAnalyzer::fillObj(BaseFiller* filler)
{
  filler->fill(*treeWriter(), numAnalyzed());
}
