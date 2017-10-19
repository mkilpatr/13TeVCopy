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
  triggerBitToken_ (cc.consumes<edm::TriggerResults> (cfg.getParameter<edm::InputTag>("bits"))),
  triggerNames_    (0),
  badChCandToken_  (cc.consumes<bool>(cfg.getParameter<edm::InputTag>("badchcandfilter"))),
  badPFMuonToken_  (cc.consumes<bool>(cfg.getParameter<edm::InputTag>("badpfmuonfilter"))),
  patTriggerBitToken_(cc.consumes<edm::TriggerResults>(cfg.getParameter<edm::InputTag>("patTriggerBits"))),
  patTriggerNames_   (nullptr),
  dupECALClustersToken_(cc.consumes<bool>(edm::InputTag("particleFlowEGammaGSFixed", "dupECALClusters", "PAT"))),
  hitsNotReplacedToken_(cc.consumes<edm::EDCollection<DetId>>(edm::InputTag("ecalMultiAndGSGlobalRecHitEB", "hitsNotReplaced", "PAT")))
{

  initTriggerNames();
  itrig_bit_pass        = data.add<size>(branchName_,"int_bit_pass","i",0);
  ibadchcand_           =  data.add<bool>(branchName_,"badchcand"     ,"O",false);
  ibadpfmuon_           =  data.add<bool>(branchName_,"badpfmuon"     ,"O",false);
  idupECALClusters_     =  data.add<bool>(branchName_,"dupECALClusters","O",false);
  ihaveHitsNotReplaced_ =  data.add<bool>(branchName_,"haveHitsNotReplaced","O",false);
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
  trigIds_["Flag_globalSuperTightHalo2016Filter"]     = fFlag_globalSuperTightHalo2016Filter;
  // bad/duplicate muons: https://twiki.cern.ch/twiki/bin/view/CMSPublic/ReMiniAOD03Feb2017Notes#Muons_and_PF_Candidates
  trigIds_["Flag_badMuons"]                           = fFlag_badMuons;       // the event contained at least one PF muon of pT > 20 GeV that is flagged as bad
  trigIds_["Flag_duplicateMuons"]                     = fFlag_duplicateMuons; // the event contained at least one PF muon of pT > 20 GeV that is flagged as duplicate
  trigIds_["Flag_noBadMuons"]                         = fFlag_noBadMuons;     // the event does not contain any PF muon of pT > 20 GeV flagged as bad or duplicate (i.e. the event is safe)

}

//--------------------------------------------------------------------------------------------------
void METFiltersFiller::load(const edm::Event& iEvent, const edm::EventSetup &iSetup)
{
  reset();
  iEvent.getByToken(triggerBitToken_, triggerBits_);
  triggerNames_ = &iEvent.triggerNames(*triggerBits_);
  iEvent.getByToken(badChCandToken_, badChCand_);
  iEvent.getByToken(badPFMuonToken_, badPFMuon_);
  if(options_ & LOADPATFILTERS){
    iEvent.getByToken(patTriggerBitToken_, patTriggerBits_);
    patTriggerNames_ = &iEvent.triggerNames(*patTriggerBits_);
  }
  if(options_ & LOADECALFIXFLAGS){
    iEvent.getByToken(dupECALClustersToken_, dupECALClusters_);
    iEvent.getByToken(hitsNotReplacedToken_, hitsNotReplaced_);
  }
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

  if (options_ & LOADPATFILTERS){
    for(unsigned int i = 0; i < patTriggerBits_->size(); ++i) {
      auto trigindex = trigIds_.find(patTriggerNames_->triggerName(i));
      if(trigindex != trigIds_.end() && patTriggerBits_->accept(i)) {
        trigPass |= trigindex->second;
      }
    }
  }
  if (options_ & LOADECALFIXFLAGS){
    data.fill<bool> (idupECALClusters_, &dupECALClusters_);
    data.fill<bool> (ihaveHitsNotReplaced_, !hitsNotReplaced_->empty());
  }

  bool  filterbadChCandidate = *badChCand_;
  bool  filterbadPFMuon = *badPFMuon_;
  data.fill<bool> (ibadchcand_, filterbadChCandidate);
  data.fill<bool> (ibadpfmuon_, filterbadPFMuon);
  data.fill<size>     (itrig_bit_pass    ,trigPass);
} // end of METFiltersFiller::fill()
