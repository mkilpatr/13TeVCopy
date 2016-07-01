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
  triggerNames_(0),
  badChCandToken_  (cc.consumes<bool>(cfg.getParameter<edm::InputTag>("badchcandfilter"))),
  badPFMuonToken_  (cc.consumes<bool>(cfg.getParameter<edm::InputTag>("badpfmuonfilter")))
{

  initTriggerNames();
  itrig_bit_pass        = data.add<size>(branchName_,"int_bit_pass","i",0);
  ibadchcand_           =  data.add<bool>(branchName_,"badchcand"     ,"O",false);
  ibadpfmuon_           =  data.add<bool>(branchName_,"badpfmuon"     ,"O",false);
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
  trigIds_["Flag_globalTightHalo2016Filter"]          = fFlag_globalTightHalo2016Filter;

}

//--------------------------------------------------------------------------------------------------
void METFiltersFiller::load(const edm::Event& iEvent, const edm::EventSetup &iSetup)
{
  reset();
  iEvent.getByToken(triggerBitToken_, triggerBits_);
  triggerNames_ = &iEvent.triggerNames(*triggerBits_);
  iEvent.getByToken(badChCandToken_, badChCand_);
  iEvent.getByToken(badPFMuonToken_, badPFMuon_);
  isLoaded_ = true;

}

//--------------------------------------------------------------------------------------------------
void METFiltersFiller::fill()
{
  size trigPass = 0;
  for(unsigned int i = 0; i < triggerBits_->size(); ++i) {
    auto trigindex = trigIds_.find(triggerNames_->triggerName(i));
    if(trigindex != trigIds_.end() && triggerBits_->accept(i)) {
      trigPass |= trigindex->second;
    }
  }

  bool  filterbadChCandidate = *badChCand_;
  bool  filterbadPFMuon = *badPFMuon_;
  data.fill<bool> (ibadchcand_, filterbadChCandidate);
  data.fill<bool> (ibadpfmuon_, filterbadPFMuon);
  data.fill<size>     (itrig_bit_pass    ,trigPass);
} // end of METFiltersFiller::fill()
