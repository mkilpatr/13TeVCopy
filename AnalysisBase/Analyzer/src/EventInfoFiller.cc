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
EventInfoFiller::EventInfoFiller(
    const edm::InputTag vtxTag,
    const edm::InputTag rhoTag,
    const edm::InputTag metTag
) :
  vtxTag_(vtxTag),
  rhoTag_ (rhoTag),
  metTag_(metTag),
  primaryVertexIndex_(-1),
  mets_(0),
  met_(0)
{
  run_      = data.add<int  >(branchName_,"run"      ,"i",0);
  lumi_     = data.add<int  >(branchName_,"lumi"     ,"i",0);
  event_    = data.add<int  >(branchName_,"event"    ,"i",0);
  nVertices_= data.add<int  >(branchName_,"npv"      ,"I",0);
  rho_      = data.add<float>(branchName_,"pv_x"     ,"F",0);
  pvx_      = data.add<float>(branchName_,"pv_y"     ,"F",0);
  pvy_      = data.add<float>(branchName_,"pv_z"     ,"F",0);
  pvz_      = data.add<float>(branchName_,"rho"      ,"F",0);
  metpt_    = data.add<float>(branchName_,"met_pt"   ,"F",0);
  metphi_   = data.add<float>(branchName_,"met_phi"  ,"F",0);
  metsumEt_ = data.add<float>(branchName_,"met_sumEt","F",0);

}

//--------------------------------------------------------------------------------------------------
void EventInfoFiller::load(const edm::Event& iEvent)
{
  reset();

  FileUtilities::enforceGet(iEvent,vtxTag_,vertices_,true);
  FileUtilities::enforceGet(iEvent,metTag_,mets_,true);
  FileUtilities::enforceGet(iEvent,rhoTag_,rhoHandle_,true);

  if(vertices_->size() > 0)
    primaryVertexIndex_ = 0;

  met_ = &mets_->front();
  eventCoords.run = iEvent.run();
  eventCoords.lumi = iEvent.luminosityBlock();
  eventCoords.event = iEvent.id().event();
  isLoaded_ = true;
}

//--------------------------------------------------------------------------------------------------
void EventInfoFiller::fill()
{
  data.fill<int  >(run_       ,eventCoords.run);
  data.fill<int  >(lumi_      ,eventCoords.lumi);
  data.fill<int  >(event_     ,eventCoords.event);
  data.fill<int  >(nVertices_ ,vertices_->size());
  data.fill<int  >(pvx_       ,(*vertices_)[primaryVertexIndex_].x());
  data.fill<int  >(pvy_       ,(*vertices_)[primaryVertexIndex_].y());
  data.fill<int  >(pvz_       ,(*vertices_)[primaryVertexIndex_].z());
  data.fill<float>(rho_       ,(*rhoHandle_));
  data.fill<float>(metpt_     ,met_->pt());
  data.fill<float>(metphi_    ,met_->phi());
  data.fill<float>(metsumEt_  ,met_->sumEt());
  data.fill<float>(isFilled_  ,true);
}
