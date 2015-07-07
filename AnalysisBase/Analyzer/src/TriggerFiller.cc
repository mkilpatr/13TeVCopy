//--------------------------------------------------------------------------------------------------
// 
// TriggerFiller
// 
// Filler for trigger decisions
// 
// TriggerFiller.cc created on Thu Jul 2 14:13:32 CEST 2015 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisBase/Analyzer/interface/TriggerFiller.h"

using namespace ucsbsusy;

//--------------------------------------------------------------------------------------------------
TriggerFiller::TriggerFiller(const edm::ParameterSet& cfg, edm::ConsumesCollector && cc, const int options, const string branchName) :
  BaseFiller(options, branchName),
  triggerBitToken_     (cc.consumes<edm::TriggerResults>                   (cfg.getParameter<edm::InputTag>("bits"))),
  triggerObjToken_     (cc.consumes<pat::TriggerObjectStandAloneCollection>(cfg.getParameter<edm::InputTag>("objects"))),
  triggerPrescaleToken_(cc.consumes<pat::PackedTriggerPrescales>           (cfg.getParameter<edm::InputTag>("prescales"))),
  triggerNames_        (0)
{

  initTriggerNames();

  itrig_bit_flag        = data.addMulti<unsigned long>(branchName_,"bit_flag",0);
  itrig_bit_pass        = data.addMulti<bool         >(branchName_,"bit_pass",0);
  itrig_bit_prescale    = data.addMulti<unsigned int >(branchName_,"bit_prescale",0);
  itrig_obj_pt          = data.addMulti<float>(branchName_,"obj_pt",0);
  itrig_obj_eta         = data.addMulti<float>(branchName_,"obj_eta",0);
  itrig_obj_phi         = data.addMulti<float>(branchName_,"obj_phi",0);
  itrig_obj_mass        = data.addMulti<float>(branchName_,"obj_mass",0);
  itrig_obj_pathflags   = data.addMulti<unsigned long>(branchName_,"obj_pathflags",0);
  itrig_obj_filterflags = data.addMulti<unsigned long>(branchName_,"obj_filterflags",0);

}

//--------------------------------------------------------------------------------------------------
void TriggerFiller::initTriggerNames()
{

  trigIds_["HLT_IsoMu24_eta2p1_v1"] = kHLT_IsoMu24_eta2p1_v1;
  trigIds_["HLT_IsoMu27_v1"] = kHLT_IsoMu27_v1;
  trigIds_["HLT_IsoTkMu24_eta2p1_v1"] = kHLT_IsoTkMu24_eta2p1_v1;
  trigIds_["HLT_IsoTkMu27_v1"] = kHLT_IsoTkMu27_v1;
  trigIds_["HLT_Ele32_eta2p1_WPLoose_Gsf_v1"] = kHLT_Ele32_eta2p1_WPLoose_Gsf_v1;
  trigIds_["HLT_DiCentralPFJet70_PFMET120_NoiseCleaned_v1"] = kHLT_DiCentralPFJet70_PFMET120_NoiseCleaned_v1;
  trigIds_["HLT_PFHT350_PFMET120_NoiseCleaned_v1"] = kHLT_PFHT350_PFMET120_NoiseCleaned_v1;
  trigIds_["HLT_PFMET170_NoiseCleaned_v1"] = kHLT_PFMET170_NoiseCleaned_v1;
  trigIds_["HLT_PFMET170_NoiseCleaned_v1"] = kHLT_PFMET170_NoiseCleaned_v1;
  trigIds_["HLT_PFMET120_NoiseCleaned_BTagCSV07_v1"] = kHLT_PFMET120_NoiseCleaned_BTagCSV07_v1;
  trigIds_["HLT_PFHT350_v1"] = kHLT_PFHT350_v1;
  trigIds_["HLT_PFHT600_v1"] = kHLT_PFHT600_v1;
  trigIds_["HLT_PFHT650_v1"] = kHLT_PFHT650_v1;
  trigIds_["HLT_PFHT900_v1"] = kHLT_PFHT900_v1;

  trigFilterIds_["hltPFMET120Filter"] = kPFMET120;
  trigFilterIds_["hltDiCentralPFJet70"] = kDiCentralPFJet70;
  trigFilterIds_["hltL3crIsoL1sMu20Eta2p1L1f0L2f10QL3f24QL3trkIsoFiltered0p09"] = kSingleIsoMu;
  trigFilterIds_["hltEle32WPLooseGsfTrackIsoFilter"] = kSingleEle;

}

//--------------------------------------------------------------------------------------------------
void TriggerFiller::load(const edm::Event& iEvent)
{
  reset();
  iEvent.getByToken(triggerBitToken_, triggerBits_);
  iEvent.getByToken(triggerObjToken_, triggerObjects_);
  iEvent.getByToken(triggerPrescaleToken_, triggerPrescales_);
  triggerNames_ = &iEvent.triggerNames(*triggerBits_);
  isLoaded_ = true;

}

//--------------------------------------------------------------------------------------------------
void TriggerFiller::fill()
{

  for(unsigned int i = 0; i < triggerBits_->size(); ++i) {
    auto trigindex = trigIds_.find(triggerNames_->triggerName(i));
    if(trigindex != trigIds_.end()) {
      data.fillMulti<unsigned long>(itrig_bit_flag, trigindex->second);
      data.fillMulti<bool         >(itrig_bit_pass, triggerBits_->accept(i));
      data.fillMulti<unsigned int >(itrig_bit_prescale, triggerPrescales_->getPrescaleForIndex(i));
    }
  }

  for (pat::TriggerObjectStandAlone obj : *triggerObjects_) {
    obj.unpackPathNames(*triggerNames_);
    for (unsigned h = 0; h < obj.filterLabels().size(); ++h) {
      auto filterIndex = trigFilterIds_.find(obj.filterLabels()[h]);
      unsigned long trigflags = 0;
      unsigned long trigfilterflags = 0;
      if(filterIndex != trigFilterIds_.end()) {
        trigfilterflags |= filterIndex->second;
        for(auto trigindex : trigIds_) {
          if(obj.hasPathName(trigindex.first)) {
            trigflags |= trigindex.second;
          }
        }
        data.fillMulti<float>(itrig_obj_pt, obj.pt());
        data.fillMulti<float>(itrig_obj_eta, obj.eta());
        data.fillMulti<float>(itrig_obj_phi, obj.phi());
        data.fillMulti<float>(itrig_obj_mass, obj.mass());
        data.fillMulti<unsigned long>(itrig_obj_pathflags, trigflags);
        data.fillMulti<unsigned long>(itrig_obj_filterflags, trigfilterflags);
      }
    }
  }

}
