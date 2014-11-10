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
  rhoTag_          (cfg.getParameter<edm::InputTag>("rho")),
  metTag_(cfg.getParameter<edm::InputTag>("mets")),
  run_(0),
  lumi_(0),
  event_(0),
  nVertices_(0),
  rho_(0),
  pvx_(0),
  pvy_(0),
  pvz_(0),
  metpt_(0),
  metphi_(0),
  metsumEt_(0),
  primaryVertexIndex_(-1),
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
  tW.book("rho",rho_,"F");
  tW.book("met_pt",metpt_,"F");
  tW.book("met_phi",metphi_,"F");
  tW.book("met_sumEt",metsumEt_,"F");

}

//--------------------------------------------------------------------------------------------------
void EventInfoFiller::reset()
{
  isLoaded_ = false;
  isFilled_ = false;

  run_ = 0;
  lumi_ = 0;
  event_ = 0;
  nVertices_ = 0;
  pvx_ = 0;
  pvy_ = 0;
  pvz_ = 0;
  primaryVertexIndex_ = -1;
  rho_ = 0;
  metpt_ = 0;
  metphi_ = 0;
  metsumEt_= 0;
}

//--------------------------------------------------------------------------------------------------
void EventInfoFiller::load(edm::Event& iEvent)
{
  reset();

  FileUtilities::enforceGet(iEvent,vtxTag_,vertices_,true);
  FileUtilities::enforceGet(iEvent,metTag_,mets_,true);
  FileUtilities::enforceGet(iEvent,rhoTag_,rhoHandle_,true);

  if(vertices_->size() > 0)
    primaryVertexIndex_ = 0;

  met_ = &mets_->front();
  run_ = iEvent.run();
  lumi_ = iEvent.luminosityBlock();
  event_ = iEvent.id().event();
  isLoaded_ = true;
}

//--------------------------------------------------------------------------------------------------
void EventInfoFiller::fill(TreeWriter& tW, const int& numAnalyzed)
{
  nVertices_ = vertices_->size();
  pvx_ = (*vertices_)[primaryVertexIndex_].x();
  pvy_ = (*vertices_)[primaryVertexIndex_].y();
  pvz_ = (*vertices_)[primaryVertexIndex_].z();
  rho_ = (*rhoHandle_);
  metpt_ = met_->pt();
  metphi_ = met_->phi();
  metsumEt_ = met_->sumEt();
  isFilled_ = true;
}
