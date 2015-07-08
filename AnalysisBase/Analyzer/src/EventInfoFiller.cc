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
  const edm::ParameterSet& cfg,
  edm::ConsumesCollector && cc,
  const int options
) :
  BaseFiller(options, ""),
  vtxToken_          (cc.consumes<reco::VertexCollection>(cfg.getParameter<edm::InputTag>("vertices"))),
  rhoToken_          (cc.consumes<double>                (cfg.getParameter<edm::InputTag>("rho"))),
  metToken_          (cc.consumes<pat::METCollection>    (cfg.getParameter<edm::InputTag>("mets"))),
  genEvtInfoToken_   (cc.consumes<GenEventInfoProduct>   (cfg.getParameter<edm::InputTag>("genEvtInfo"))),
  lheEvtInfoToken_   (cc.consumes<LHEEventProduct>       (cfg.getParameter<edm::InputTag>("lheEvtInfo"))),
  systWgtIndices_    (cfg.getUntrackedParameter<std::vector<unsigned int> >              ("whichSystematicWeights")),
  primaryVertexIndex_(-1),
  met_(0)
{
  irun_           =  data.add<unsigned int>(branchName_,"run"       ,"i",0);
  ilumi_          =  data.add<unsigned int>(branchName_,"lumi"      ,"i",0);
  ievent_         =  data.add<unsigned int>(branchName_,"event"     ,"i",0);
  inpv_           =  data.add<unsigned int>(branchName_,"npv"       ,"i",0);
  irho_           =  data.add<float>       (branchName_,"rho"       ,"F",0);
  ipvx_           =  data.add<float>       (branchName_,"pv_x"      ,"F",0);
  ipvy_           =  data.add<float>       (branchName_,"pv_y"      ,"F",0);
  ipvz_           =  data.add<float>       (branchName_,"pv_z"      ,"F",0);
  imetpt_         =  data.add<float>       (branchName_,"met_pt"    ,"F",0);
  imetphi_        =  data.add<float>       (branchName_,"met_phi"   ,"F",0);
  imetsumEt_      =  data.add<float>       (branchName_,"met_sumEt" ,"F",0);
  igenmetpt_      =  data.add<float>       (branchName_,"genmet_pt" ,"F",0);
  igenmetphi_     =  data.add<float>       (branchName_,"genmet_phi","F",0);
  igoodvertex_    =  data.add<bool>        (branchName_,"goodvertex","O",false);
  igenwgt_        =  data.add<float>       (branchName_,"genweight" ,"F",1);
  igenqscale_     =  data.add<float>       (branchName_,"genqscale" ,"F",1);
  inmeparts_      =  data.add<int>         (branchName_,"nmepartons","I",0);
  inmefiltparts_  =  data.add<int>         (branchName_,"nmefiltpartons"  ,"I",0);
  ilhecentralwgt_ =  data.add<float>       (branchName_,"lhecentralweight","F",1);
  isystwgts_      =  data.addMulti<float>  (branchName_,"systweights",0);

}

//--------------------------------------------------------------------------------------------------
void EventInfoFiller::load(const edm::Event& iEvent)
{
  reset();

  iEvent.getByToken(vtxToken_, vertices_);
  iEvent.getByToken(rhoToken_, rho_);
  iEvent.getByToken(metToken_, mets_);
  if(options_ & LOADGEN)
   iEvent.getByToken(genEvtInfoToken_, genEvtInfo_);
  if(options_ & LOADLHE)
    iEvent.getByToken(lheEvtInfoToken_, lheEvtInfo_);

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
  bool hasgoodvtx = false;
  if(!(*vertices_)[primaryVertexIndex_].isFake() && 
      (*vertices_)[primaryVertexIndex_].ndof() > 4. &&
      (*vertices_)[primaryVertexIndex_].position().Rho() <= 2.0 &&
      fabs((*vertices_)[primaryVertexIndex_].position().Z())<=24.0)
    hasgoodvtx = true;

  data.fill<unsigned int>(irun_       ,eventCoords.run);
  data.fill<unsigned int>(ilumi_      ,eventCoords.lumi);
  data.fill<unsigned int>(ievent_     ,eventCoords.event);
  data.fill<unsigned int>(inpv_       ,vertices_->size());
  data.fill<float>       (irho_       ,(*rho_));
  data.fill<float>       (ipvx_       ,(*vertices_)[primaryVertexIndex_].x());
  data.fill<float>       (ipvy_       ,(*vertices_)[primaryVertexIndex_].y());
  data.fill<float>       (ipvz_       ,(*vertices_)[primaryVertexIndex_].z());
  data.fill<float>       (imetpt_     ,met_->pt());
  data.fill<float>       (imetphi_    ,met_->phi());
  data.fill<float>       (imetsumEt_  ,met_->sumEt());
  data.fill<float>       (igenmetpt_  ,met_->genMET()->pt());
  data.fill<float>       (igenmetphi_ ,met_->genMET()->phi());
  data.fill<bool>        (igoodvertex_,hasgoodvtx );

  if(options_ & LOADGEN) {
    data.fill<float>     (igenwgt_      ,genEvtInfo_->weight());
    data.fill<float>     (igenqscale_   ,genEvtInfo_->qScale());
    data.fill<int>       (inmeparts_    ,genEvtInfo_->nMEPartons());
    data.fill<int>       (inmefiltparts_,genEvtInfo_->nMEPartonsFiltered());
  }
  if(options_ & LOADLHE) {
    for(auto index : systWgtIndices_) {
      data.fillMulti<float>(isystwgts_, lheEvtInfo_->weights()[index].wgt);
    }
    data.fill<float>     (ilhecentralwgt_,lheEvtInfo_->originalXWGTUP());
  }

  isFilled_ = true;
}
