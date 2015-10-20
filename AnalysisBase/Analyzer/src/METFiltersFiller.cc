//--------------------------------------------------------------------------------------------------
// 
// METFiltersFiller
// 
// Filler for trigger decisions
// 
// METFiltersFiller.cc created on Thu Jul 2 14:13:32 CEST 2015 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisBase/Analyzer/interface/METFiltersFiller.h"

using namespace ucsbsusy;

//--------------------------------------------------------------------------------------------------
METFiltersFiller::METFiltersFiller(const edm::ParameterSet& cfg, edm::ConsumesCollector && cc, const int options, const string branchName) :
  BaseFiller(options, branchName),
  triggerBitToken_            (cc.consumes<edm::TriggerResults> (cfg.getParameter<edm::InputTag>("bits"))),
  hNoiseResultToken_          (cc.consumes<bool>                (cfg.getParameter<edm::InputTag>("hbhe"))),
  hNoiseResultRun2LooseToken_ (cc.consumes<bool>                (cfg.getParameter<edm::InputTag>("hbherun2loose"))),
  hNoiseResultRun2TightToken_ (cc.consumes<bool>                (cfg.getParameter<edm::InputTag>("hbherun2tight"))),
  triggerNames_(0),
  isFastSim_(cfg.getUntrackedParameter<bool>("isFastSim"))
{

  initTriggerNames();

  itrig_bit_flag        = data.addMulti<unsigned long>(branchName_,"bit_flag",0);
  itrig_bit_pass        = data.addMulti<bool         >(branchName_,"bit_pass",0);
  hbheFilterFix_        = data.addMulti<bool         >(branchName_,"hbheFilterFix",0);
  hbheFilterRun2Loose_  = data.addMulti<bool         >(branchName_,"hbheFilterRun2Loose",0);
  hbheFilterRun2Tight_  = data.addMulti<bool         >(branchName_,"hbheFilterRun2Tight",0);
}

//--------------------------------------------------------------------------------------------------
void METFiltersFiller::initTriggerNames()
{
  
  trigIds_["Flag_trackingFailureFilter"]              = fFlag_trackingFailureFilter;
  trigIds_["Flag_goodVertices"]                       = fFlag_goodVertices;
  trigIds_["Flag_CSCTightHaloFilter"]                 = fFlag_CSCTightHaloFilter;
  trigIds_["Flag_trkPOGFilters"]                      = fFlag_trkPOGFilters;
  trigIds_["Flag_trkPOG_logErrorTooManyClusters"]     = fFlag_trkPOG_logErrorTooManyClusters;
  trigIds_["Flag_EcalDeadCellTriggerPrimitiveFilter"] = fFlag_EcalDeadCellTriggerPrimitiveFilter;
  trigIds_["Flag_ecalLaserCorrFilter"]                = fFlag_ecalLaserCorrFilter;
  trigIds_["Flag_trkPOG_manystripclus53X"]            = fFlag_trkPOG_manystripclus53X;
  trigIds_["Flag_eeBadScFilter"]                      = fFlag_eeBadScFilter;
  trigIds_["Flag_METFilters"]                         = fFlag_METFilters;
  trigIds_["Flag_HBHENoiseFilter"]                    = fFlag_HBHENoiseFilter;
  trigIds_["Flag_trkPOG_toomanystripclus53X"]         = fFlag_trkPOG_toomanystripclus53X;
  trigIds_["Flag_hcalLaserEventFilter"]               = fFlag_hcalLaserEventFilter;
}

//--------------------------------------------------------------------------------------------------
void METFiltersFiller::load(const edm::Event& iEvent, const edm::EventSetup &iSetup)
{
  reset();
  iEvent.getByToken(triggerBitToken_, triggerBits_);
  if( ! isFastSim_ ) {
    iEvent.getByToken(hNoiseResultToken_, hNoiseResult_);
    iEvent.getByToken(hNoiseResultRun2LooseToken_, hNoiseResultRun2Loose_);
    iEvent.getByToken(hNoiseResultRun2TightToken_, hNoiseResultRun2Tight_);
  }
  triggerNames_ = &iEvent.triggerNames(*triggerBits_);
  isLoaded_ = true;

}

//--------------------------------------------------------------------------------------------------
void METFiltersFiller::fill()
{
  data.fillMulti<bool>(hbheFilterFix_,       isFastSim_ ? true : *hNoiseResult_);
  data.fillMulti<bool>(hbheFilterRun2Loose_, isFastSim_ ? true : *hNoiseResultRun2Loose_);
  data.fillMulti<bool>(hbheFilterRun2Tight_, isFastSim_ ? true : *hNoiseResultRun2Tight_);

  for(unsigned int i = 0; i < triggerBits_->size(); ++i) {
    auto trigindex = trigIds_.find(triggerNames_->triggerName(i));
    if(trigindex != trigIds_.end()) {
      data.fillMulti<unsigned long>(itrig_bit_flag, trigindex->second);
      data.fillMulti<bool         >(itrig_bit_pass, triggerBits_->accept(i));
    }
  }

} // end of METFiltersFiller::fill()
