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
  puSummaryToken_    (cc.consumes<std::vector<PileupSummaryInfo> >(cfg.getParameter<edm::InputTag>("pileupSummaryInfos"))),
  metToken_          (cc.consumes<pat::METCollection>    (cfg.getParameter<edm::InputTag>("mets"))),
  metOOBToken_       (cc.consumes<pat::METCollection>    (cfg.getParameter<edm::InputTag>("metsOOB"))),
  metNoHFToken_      (cc.consumes<pat::METCollection>    (cfg.getParameter<edm::InputTag>("metsNoHF"))),
  puppimetToken_     (cc.consumes<pat::METCollection>    (cfg.getParameter<edm::InputTag>("puppimets"))),
  genEvtInfoToken_   (cc.consumes<GenEventInfoProduct>   (cfg.getParameter<edm::InputTag>("genEvtInfo"))),
  lheEvtInfoToken_   (cc.consumes<LHEEventProduct>       (cfg.getParameter<edm::InputTag>("lheEvtInfo"))),
  systWgtIndices_    (cfg.getUntrackedParameter<std::vector<unsigned int> >              ("whichSystematicWeights")),
  stdGenJetToken_    (cc.consumes<reco::GenJetCollection>(cfg.getParameter<edm::InputTag>("stdGenJets"))),
  metEGCleanToken_   (cc.consumes<pat::METCollection>    (cfg.getParameter<edm::InputTag>("metsEGClean"))),
  metMuCleanToken_   (cc.consumes<pat::METCollection>    (cfg.getParameter<edm::InputTag>("metsMuClean"))),
  metNoCleanToken_   (cc.consumes<pat::METCollection>    (cfg.getParameter<edm::InputTag>("metsNoClean"))),
  primaryVertexIndex_(-1),
  met_(0),
  metOOB_(0),
  metNoHF_(0),
  puppimet_(0),
  isFastSim_(false)
{
  irun_           =  data.add<unsigned int>(branchName_,"run"       ,"i",0);
  ilumi_          =  data.add<unsigned int>(branchName_,"lumi"      ,"i",0);
  ievent_         =  data.add<unsigned int>(branchName_,"event"     ,"i",0);
  ifastsim_       =  data.add<bool>        (branchName_,"isfastsim" ,"O",0);
  inpv_           =  data.add<unsigned int>(branchName_,"npv"       ,"i",0);
  irho_           =  data.add<float>       (branchName_,"rho"       ,"F",0);
  ipvx_           =  data.add<float>       (branchName_,"pv_x"      ,"F",0);
  ipvy_           =  data.add<float>       (branchName_,"pv_y"      ,"F",0);
  ipvz_           =  data.add<float>       (branchName_,"pv_z"      ,"F",0);
  ibunchspacing_  =  data.add<int>         (branchName_,"bunch_spacing","I",0);
  intruepuints_   =  data.add<float>       (branchName_,"nputrue"   ,"F",0);
  inpuverts_      =  data.add<int>         (branchName_,"npu"       ,"I",0);
  inpuvertsbx1_   =  data.add<int>         (branchName_,"npu_bx1"   ,"I",0);
  inpuvertsbxm1_  =  data.add<int>         (branchName_,"npu_bxm1"  ,"I",0);
  imetpt_         =  data.add<float>       (branchName_,"met_pt"    ,"F",0);
  imetphi_        =  data.add<float>       (branchName_,"met_phi"   ,"F",0);
  imetsumEt_      =  data.add<float>       (branchName_,"met_sumEt" ,"F",0);
  imetsignificance_ =  data.add<float>     (branchName_,"met_significance","F",0);
  irawmetpt_      =  data.add<float>       (branchName_,"rawmet_pt"    ,"F",0);
  irawmetphi_     =  data.add<float>       (branchName_,"rawmet_phi"   ,"F",0);
  irawmetsumEt_   =  data.add<float>       (branchName_,"rawmet_sumEt" ,"F",0);
  icalometpt_     =  data.add<float>       (branchName_,"calomet_pt"   ,"F",0);
  icalometphi_    =  data.add<float>       (branchName_,"calomet_phi"  ,"F",0);
  icalometsumEt_  =  data.add<float>       (branchName_,"calomet_sumEt","F",0);
  imetnohfpt_     =  data.add<float>       (branchName_,"metnohf_pt"   ,"F",0);
  imetnohfphi_    =  data.add<float>       (branchName_,"metnohf_phi"  ,"F",0);
  imetnohfsumEt_  =  data.add<float>       (branchName_,"metnohf_sumEt","F",0);
  ipuppimetpt_    =  data.add<float>       (branchName_,"puppimet_pt"   ,"F",0);
  ipuppimetphi_   =  data.add<float>       (branchName_,"puppimet_phi"  ,"F",0);
  ipuppimetsumEt_ =  data.add<float>       (branchName_,"puppimet_sumEt","F",0);
  igenmetpt_      =  data.add<float>       (branchName_,"genmet_pt" ,"F",0);
  igenmetphi_     =  data.add<float>       (branchName_,"genmet_phi","F",0);
  igoodvertex_    =  data.add<bool>        (branchName_,"goodvertex","O",false);
  igenwgt_        =  data.add<float>       (branchName_,"genweight" ,"F",1);
  igenqscale_     =  data.add<float>       (branchName_,"genqscale" ,"F",1);
  inmeparts_      =  data.add<int>         (branchName_,"nmepartons","I",0);
  inmefiltparts_  =  data.add<int>         (branchName_,"nmefiltpartons"  ,"I",0);
  ilhecentralwgt_ =  data.add<float>       (branchName_,"lhecentralweight","F",1);
  isystwgts_      =  data.addMulti<float>  (branchName_,"systweights",0);
  instdgenjets_   =  data.add<int>         (branchName_,"nstdgenjets","I",0);
  imetunclustrun2uppt_ =  data.add<float>       (branchName_,"metunclustrun2up_pt"    ,"F",0);
  imetunclustrun2dnpt_ =  data.add<float>       (branchName_,"metunclustrun2dn_pt"    ,"F",0);
  if(options_ & LOADLHE && options_ & SAVEMASSES)
    imasspar_     =  data.addMulti<size16> (branchName_,"massparams",0);

  if(options_ & LOADEXTRAMETS){
    imetEGCleanpt_         =  data.add<float>       (branchName_,"metEGClean_pt"    ,"F",0);
    imetEGCleanphi_        =  data.add<float>       (branchName_,"metEGClean_phi"   ,"F",0);
    imetEGCleansumEt_      =  data.add<float>       (branchName_,"metEGClean_sumEt" ,"F",0);
    imetMuCleanpt_         =  data.add<float>       (branchName_,"metMuClean_pt"    ,"F",0);
    imetMuCleanphi_        =  data.add<float>       (branchName_,"metMuClean_phi"   ,"F",0);
    imetMuCleansumEt_      =  data.add<float>       (branchName_,"metMuClean_sumEt" ,"F",0);
    imetNoCleanpt_         =  data.add<float>       (branchName_,"metNoClean_pt"    ,"F",0);
    imetNoCleanphi_        =  data.add<float>       (branchName_,"metNoClean_phi"   ,"F",0);
    imetNoCleansumEt_      =  data.add<float>       (branchName_,"metNoClean_sumEt" ,"F",0);
  }

}

//--------------------------------------------------------------------------------------------------
void EventInfoFiller::load(const edm::Event& iEvent, const edm::EventSetup &iSetup)
{
  reset();

  iEvent.getByToken(vtxToken_, vertices_);
  iEvent.getByToken(rhoToken_, rho_);
  if(options_ & LOADPUINFO)
    iEvent.getByToken(puSummaryToken_, puSummaryInfos_);
  iEvent.getByToken(metToken_, mets_);
  iEvent.getByToken(metOOBToken_, metsOOB_);
  //iEvent.getByToken(metNoHFToken_, metsNoHF_);
  iEvent.getByToken(puppimetToken_, puppimets_);
  if(options_ & LOADGEN)
   iEvent.getByToken(genEvtInfoToken_, genEvtInfo_);
  if(options_ & LOADLHE && !(options_ & SAVEMASSES))
    iEvent.getByToken(lheEvtInfoToken_, lheEvtInfo_);
  if(options_ & LOADGENJETS)
    iEvent.getByToken(stdGenJetToken_, stdGenJets_);
  if(options_ & LOADEXTRAMETS){
    iEvent.getByToken(metEGCleanToken_, metsEGClean_);
    iEvent.getByToken(metMuCleanToken_, metsMuClean_);
    iEvent.getByToken(metNoCleanToken_, metsNoClean_);
  }

  if(vertices_->size() > 0)
    primaryVertexIndex_ = 0;

  met_ = &mets_->front();
  metOOB_ = &metsOOB_->front();
  //metNoHF_ = &metsNoHF_->front();
  puppimet_ = &puppimets_->front();
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
  data.fill<bool>        (ifastsim_   ,isFastSim_);
  data.fill<unsigned int>(inpv_       ,vertices_->size());
  data.fill<bool>        (igoodvertex_,hasgoodvtx );
  data.fill<float>       (irho_       ,(*rho_));
  data.fill<float>       (ipvx_       ,(*vertices_)[primaryVertexIndex_].x());
  data.fill<float>       (ipvy_       ,(*vertices_)[primaryVertexIndex_].y());
  data.fill<float>       (ipvz_       ,(*vertices_)[primaryVertexIndex_].z());

  if(options_ & LOADPUINFO) {
    int   bunch_spacing = 0;
    float num_true_interactions = 0;
    int   num_pu_verts = 0, num_pu_verts_bx1 = 0, num_pu_verts_bxm1 = 0;
    for( const auto& psu : *puSummaryInfos_ ) {
      if(psu.getBunchCrossing() == 0) {
        bunch_spacing = psu.getBunchSpacing();
        num_true_interactions = psu.getTrueNumInteractions();
        num_pu_verts = psu.getPU_NumInteractions();
      } else if (psu.getBunchCrossing() == 1) {
        num_pu_verts_bx1 = psu.getPU_NumInteractions();
      } else if (psu.getBunchCrossing() == -1) {
        num_pu_verts_bxm1 = psu.getPU_NumInteractions();
      } else {
        continue;
      }
    }
    data.fill<int>       (ibunchspacing_  ,bunch_spacing);
    data.fill<float>     (intruepuints_   ,num_true_interactions);
    data.fill<int>       (inpuverts_      ,num_pu_verts);
    data.fill<int>       (inpuvertsbx1_   ,num_pu_verts_bx1);
    data.fill<int>       (inpuvertsbxm1_  ,num_pu_verts_bxm1);
  }


  data.fill<float>       (imetpt_            ,met_->pt());
  data.fill<float>       (imetphi_           ,met_->phi());
  data.fill<float>       (imetsumEt_         ,met_->sumEt());
  data.fill<float>       (imetsignificance_  ,met_->significance());
  data.fill<float>       (irawmetpt_         ,metOOB_->uncorPt());
  data.fill<float>       (irawmetphi_        ,metOOB_->uncorPhi());
  data.fill<float>       (irawmetsumEt_      ,metOOB_->uncorSumEt());
  data.fill<float>       (icalometpt_        ,metOOB_->caloMETPt());
  data.fill<float>       (icalometphi_       ,metOOB_->caloMETPhi());
  data.fill<float>       (icalometsumEt_     ,metOOB_->caloMETSumEt());
  //data.fill<float>       (imetnohfpt_        ,metNoHF_->pt());
  //data.fill<float>       (imetnohfphi_       ,metNoHF_->phi());
  //data.fill<float>       (imetnohfsumEt_     ,metNoHF_->sumEt());
  data.fill<float>       (ipuppimetpt_       ,puppimet_->pt());
  data.fill<float>       (ipuppimetphi_      ,puppimet_->phi());
  data.fill<float>       (ipuppimetsumEt_    ,puppimet_->sumEt());
  data.fill<float>       (imetunclustrun2uppt_, met_->shiftedPt(pat::MET::UnclusteredEnUp));
  data.fill<float>       (imetunclustrun2dnpt_, met_->shiftedPt(pat::MET::UnclusteredEnDown));

  if(options_ & LOADEXTRAMETS){
    data.fill<float>       (imetEGCleanpt_            ,metsEGClean_->front().pt());
    data.fill<float>       (imetEGCleanphi_           ,metsEGClean_->front().phi());
    data.fill<float>       (imetEGCleansumEt_         ,metsEGClean_->front().sumEt());
    data.fill<float>       (imetMuCleanpt_            ,metsMuClean_->front().pt());
    data.fill<float>       (imetMuCleanphi_           ,metsMuClean_->front().phi());
    data.fill<float>       (imetMuCleansumEt_         ,metsMuClean_->front().sumEt());
    data.fill<float>       (imetNoCleanpt_            ,metsNoClean_->front().pt());
    data.fill<float>       (imetNoCleanphi_           ,metsNoClean_->front().phi());
    data.fill<float>       (imetNoCleansumEt_         ,metsNoClean_->front().sumEt());
  }

  if(options_ & LOADGEN) {
    data.fill<float>     (igenmetpt_    ,metOOB_->genMET()->pt());
    data.fill<float>     (igenmetphi_   ,metOOB_->genMET()->phi());
    data.fill<float>     (igenwgt_      ,genEvtInfo_->weight());
    data.fill<float>     (igenqscale_   ,genEvtInfo_->qScale());
    data.fill<int>       (inmeparts_    ,genEvtInfo_->nMEPartons());
    data.fill<int>       (inmefiltparts_,genEvtInfo_->nMEPartonsFiltered());
  }
  if(options_ & LOADLHE) {
    // save all of them if no specific weights are specified
    if(options_ & SAVEMASSES) {
      if(!systWgtIndices_.size()) {
        for(auto weight : genEvtInfo_->weights()) {
          data.fillMulti<float>(isystwgts_   ,weight);
        }
      } else {
        for(auto index : systWgtIndices_) {
          data.fillMulti<float>(isystwgts_   ,genEvtInfo_->weights()[index]);
        }
      }
      TString massstring(modelString_);
      // masstring examples:
      // T2tt_dM-10to80_genHT-160_genMET-80_400_350
      // T2tt_525_325
      while(massstring.Index("_") != TString::kNPOS) {
        massstring.Remove(0,massstring.Index("_")+1);
        TString mstr;
        if(massstring.Index("_") != TString::kNPOS)
          mstr = TString(massstring(0,massstring.First('_')));
        else
          mstr = TString(massstring(0,massstring.Length()));
        if(!mstr.IsDigit()) continue;
        if(options_ & SAVEMASSES)
          data.fillMulti<size16>(imasspar_, convertTo<size16>(mstr.Atoi(),"EventInfoFiller::massparams"));
      }
    } else {
      if(!systWgtIndices_.size()) {
        for(auto weight : lheEvtInfo_->weights()) {
          data.fillMulti<float>(isystwgts_   ,weight.wgt);
        }
      } else {
        for(auto index : systWgtIndices_) {
          data.fillMulti<float>(isystwgts_   ,lheEvtInfo_->weights()[index].wgt);
        }
      }
      data.fill<float>     (ilhecentralwgt_,lheEvtInfo_->originalXWGTUP());
    }
  }
  if(options_ & LOADGENJETS) {
    int ngenj = 0;
    for (const auto &j : *stdGenJets_){
      if (j.pt()>20 && fabs(j.eta())<2.4) ++ngenj;
    }
    data.fill<int>       (instdgenjets_    ,ngenj);
  }

  isFilled_ = true;
}
