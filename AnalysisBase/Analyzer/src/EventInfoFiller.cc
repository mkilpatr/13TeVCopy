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
  run_(0),
  lumi_(0),
  event_(0),
  nVertices_(0),
  pvx_(0),
  pvy_(0),
  pvz_(0),
  metpt_(0),
  metphi_(0),
  metsumEt_(0),
  vertices_(0),
  mets_(0),
  met_(0)
{

}

//--------------------------------------------------------------------------------------------------
void EventInfoFiller::book(TreeWriter& tW)
{

  tW.book("run",run_,"i");
  tW.book("lumi",lumi_,"i");
  tW.book("event",event_,"i");
  tW.book("npv",nVertices_,"I");
  tW.book("pv_x",pvx_,"F");
  tW.book("pv_y",pvy_,"F");
  tW.book("pv_z",pvz_,"F");
  tW.book("met",metpt_,"F");
  tW.book("met_phi",metphi_,"F");
  tW.book("met_sumEt",metsumEt_,"F");

}

//--------------------------------------------------------------------------------------------------
void EventInfoFiller::reset()
{
  run_ = 0;
  lumi_ = 0;
  event_ = 0;
  nVertices_ = 0;
  pvx_ = 0;
  pvy_ = 0;
  pvz_ = 0;
  metpt_ = 0;
  metphi_ = 0;
  metsumEt_= 0;
}

//--------------------------------------------------------------------------------------------------
void EventInfoFiller::load(edm::Event& iEvent, bool storeOnlyPtr, bool isMC)
{
  reset();

  enforceGet(iEvent,vtxTag_,vertices_,true);
  enforceGet(iEvent,metTag_,mets_,true);

  if(vertices_->size() > 0)
    primaryVertex_ = vertices_->front().position();
  else
    primaryVertex_.SetXYZ(0,0,0);

  met_ = &mets_->front();
  run_ = iEvent.run();
  lumi_ = iEvent.luminosityBlock();
  event_ = iEvent.id().event();
}

//--------------------------------------------------------------------------------------------------
void EventInfoFiller::fill(TreeWriter& tW, const int& numAnalyzed)
{
  nVertices_ = vertices_->size();
  pvx_ = vertices_->front().x();
  pvy_ = vertices_->front().y();
  pvz_ = vertices_->front().z();
  metpt_ = met_->pt();
  metphi_ = met_->phi();
  metsumEt_ = met_->sumEt();
}
