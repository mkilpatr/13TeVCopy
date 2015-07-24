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
  trigIds_["HLT_IsoMu24_eta2p1_v2"] = kHLT_IsoMu24_eta2p1_v2;
  trigIds_["HLT_IsoMu27_v2"] = kHLT_IsoMu27_v2;
  trigIds_["HLT_IsoTkMu24_eta2p1_v2"] = kHLT_IsoTkMu24_eta2p1_v2;
  trigIds_["HLT_IsoTkMu27_v2"] = kHLT_IsoTkMu27_v2;
  trigIds_["HLT_Ele32_eta2p1_WPLoose_Gsf_v1"] = kHLT_Ele32_eta2p1_WPLoose_Gsf_v1;
  trigIds_["HLT_Ele32_eta2p1_WP75_Gsf_v1"] = kHLT_Ele32_eta2p1_WP75_Gsf_v1;
  trigIds_["HLT_DiCentralPFJet55_PFMET110_NoiseCleaned_v1"] = kHLT_DiCentralPFJet55_PFMET110_NoiseCleaned_v1;
  trigIds_["HLT_DiCentralPFJet70_PFMET120_NoiseCleaned_v1"] = kHLT_DiCentralPFJet70_PFMET120_NoiseCleaned_v1;
  trigIds_["HLT_PFHT350_PFMET100_NoiseCleaned_v1"] = kHLT_PFHT350_PFMET100_NoiseCleaned_v1;
  trigIds_["HLT_PFHT350_PFMET120_NoiseCleaned_v1"] = kHLT_PFHT350_PFMET120_NoiseCleaned_v1;
  trigIds_["HLT_PFMET170_NoiseCleaned_v1"] = kHLT_PFMET170_NoiseCleaned_v1;
  trigIds_["HLT_PFMET170_NoiseCleaned_v2"] = kHLT_PFMET170_NoiseCleaned_v2;
  trigIds_["HLT_PFMET120_NoiseCleaned_BTagCSV07_v1"] = kHLT_PFMET120_NoiseCleaned_BTagCSV07_v1;
  trigIds_["HLT_PFMET120_NoiseCleaned_BTagCSV072_v1"] = kHLT_PFMET120_NoiseCleaned_BTagCSV072_v1;
  trigIds_["HLT_PFMET120_NoiseCleaned_BTagCSV072_v2"] = kHLT_PFMET120_NoiseCleaned_BTagCSV072_v2;
  trigIds_["HLT_PFHT300_v1"] = kHLT_PFHT300_v1;
  trigIds_["HLT_PFHT350_v1"] = kHLT_PFHT350_v1;
  trigIds_["HLT_PFHT350_v2"] = kHLT_PFHT350_v2;
  trigIds_["HLT_PFHT400_v1"] = kHLT_PFHT400_v1;
  trigIds_["HLT_PFHT475_v1"] = kHLT_PFHT475_v1;
  trigIds_["HLT_PFHT600_v1"] = kHLT_PFHT600_v1;
  trigIds_["HLT_PFHT600_v2"] = kHLT_PFHT600_v2;
  trigIds_["HLT_PFHT650_v1"] = kHLT_PFHT650_v1;
  trigIds_["HLT_PFHT800_v1"] = kHLT_PFHT800_v1;
  trigIds_["HLT_PFHT900_v1"] = kHLT_PFHT900_v1;
  trigIds_["HLT_Photon120_R9Id90_HE10_IsoM_v2"] = kHLT_Photon120_R9Id90_HE10_IsoM_v2;
  trigIds_["HLT_Photon165_R9Id90_HE10_IsoM_v2"] = kHLT_Photon165_R9Id90_HE10_IsoM_v2;
  trigIds_["HLT_Photon165_HE10_v2"] = kHLT_Photon165_HE10_v2;
  trigIds_["HLT_Photon175_v2"] = kHLT_Photon175_v2;

  trigFilterIds_["hltPFMET100Filter"] = kPFMET100;
  trigFilterIds_["hltPFMET110Filter"] = kPFMET110;
  trigFilterIds_["hltPFMET120Filter"] = kPFMET120;
  trigFilterIds_["hltPFHT350Filter"] = kPFHT350;
  trigFilterIds_["hltDiCentralPFJet55"] = kDiCentralPFJet55;
  trigFilterIds_["hltDiCentralPFJet70"] = kDiCentralPFJet70;
  trigFilterIds_["hltL3crIsoL1sMu20Eta2p1L1f0L2f10QL3f24QL3trkIsoFiltered0p09"] = kSingleIsoMu24;
  trigFilterIds_["hltL3crIsoL1sMu25L1f0L2f10QL3f27QL3trkIsoFiltered0p09"] = kSingleIsoMu27;
  trigFilterIds_["hltL3fL1sMu20Eta2p1L1f0Tkf24QL3trkIsoFiltered0p09"] = kSingleIsoTkMu24;
  trigFilterIds_["hltL3fL1sMu25L1f0Tkf27QL3trkIsoFiltered0p09"] = kSingleIsoTkMu27;
  trigFilterIds_["hltEle32WPLooseGsfTrackIsoFilter"] = kSingleEle32;
  trigFilterIds_["hltEle32WP75GsfTrackIsoFilter"] = kSingleEle32;

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
