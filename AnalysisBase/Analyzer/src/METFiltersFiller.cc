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
  triggerNames_(0)
{

  initTriggerNames();

  itrig_bit_pass        = data.addMulti<bool>(branchName_,"bit_pass",0);
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
  trigIds_["Flag_CSCTightHalo2015Filter"]             = fFlag_CSCTightHalo2015Filter;
  trigIds_["Flag_HBHENoiseIsoFilter"]                 = fFlag_HBHENoiseIsoFilter;
}

//--------------------------------------------------------------------------------------------------
void METFiltersFiller::load(const edm::Event& iEvent, const edm::EventSetup &iSetup)
{
  reset();
  iEvent.getByToken(triggerBitToken_, triggerBits_);
  triggerNames_ = &iEvent.triggerNames(*triggerBits_);
  isLoaded_ = true;

}

//--------------------------------------------------------------------------------------------------
void METFiltersFiller::fill()
{
  for(unsigned int i = 0; i < triggerBits_->size(); ++i) {
    auto trigindex = trigIds_.find(triggerNames_->triggerName(i));
    if(trigindex != trigIds_.end()) {
      data.fillMulti<bool>(itrig_bit_pass, triggerBits_->accept(i));
    }
  }

} // end of METFiltersFiller::fill()
