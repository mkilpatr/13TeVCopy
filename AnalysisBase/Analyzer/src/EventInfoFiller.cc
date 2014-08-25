//--------------------------------------------------------------------------------------------------
// 
// EventInfoFiller
// 
// Simple class to write some event info into a TTree.
// 
// EventInfoFiller.cc created on Mon Aug 11 10:23:46 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisBase/Analyzer/interface/EventInfoFiller.h"

using namespace ucsbsusy;
using namespace std;

//--------------------------------------------------------------------------------------------------
EventInfoFiller::EventInfoFiller(const edm::ParameterSet &cfg) :
  vtxTag_(cfg.getParameter<edm::InputTag>("vertices")),
  metTag_(cfg.getParameter<edm::InputTag>("mets")),
  obj(0),
  verticies_(0),
  primaryVerex_(0),
  mets_(0),
  met_(0)
{
  EventInfo::Class()->IgnoreTObjectStreamer();
}

//--------------------------------------------------------------------------------------------------
EventInfoFiller::~EventInfoFiller()
{
  delete obj;
}

//--------------------------------------------------------------------------------------------------
void EventInfoFiller::load(edm::Event& iEvent, bool storeOnlyPtr ){
  enforceGet(iEvent,vtxTag_,verticies_,true);
  if(verticies_->size() != 0)
    primaryVerex_ = &verticies_->front();


  enforceGet(iEvent,metTag_,mets_,true);
  met_ = &mets_->front();

  if(storeOnlyPtr) return;
  if(!obj) obj = new EventInfo;

  obj->setRunNum(iEvent.id().run());
  obj->setLumiSec(iEvent.luminosityBlock());
  obj->setEvtNum(iEvent.id().event());
  obj->setIsMC(!iEvent.isRealData());

  obj->setNPV(verticies_->size());
  obj->setPVx(primaryVerex_ ? primaryVerex_->x() : -99999);
  obj->setPVy(primaryVerex_ ? primaryVerex_->y() : -99999);
  obj->setPVz(primaryVerex_ ? primaryVerex_->z() : -99999);

  obj->setMET(met_->pt());
  obj->setMETPhi(met_->phi());
  obj->setMETSumET(met_->sumEt());
  obj->setGenMET(met_->genMET()->pt());
  obj->setMETUp(met_->shiftedPt(pat::MET::JetEnUp));
  obj->setMETDown(met_->shiftedPt(pat::MET::JetEnDown));
}



//--------------------------------------------------------------------------------------------------
void EventInfoFiller::fill(Planter& plant, int& bookMark)
{
  plant.fill(*obj,"EventInfo");
  bookMark++;
}
