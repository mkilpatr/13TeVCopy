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
  triggerNames_        (0),
  isFastSim_(cfg.getUntrackedParameter<bool>("isFastSim"))
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

  trigIds_["HLT_IsoMu24"] = kHLT_IsoMu24;
  trigIds_["HLT_IsoTkMu24"] = kHLT_IsoTkMu24;
  trigIds_["HLT_Mu50"] = kHLT_Mu50;
  trigIds_["HLT_IsoMu22_eta2p1"] = kHLT_IsoMu22_eta2p1;
  trigIds_["HLT_IsoTkMu22_eta2p1"] = kHLT_IsoTkMu22_eta2p1;
  trigIds_["HLT_IsoMu24_eta2p1"] = kHLT_IsoMu24_eta2p1;
  trigIds_["HLT_IsoTkMu24_eta2p1"] = kHLT_IsoTkMu24_eta2p1;

  trigIds_["HLT_Ele27_WPTight_Gsf"] = kHLT_Ele27_WPTight_Gsf;
  trigIds_["HLT_Ele25_eta2p1_WPTight_Gsf"] = kHLT_Ele25_eta2p1_WPTight_Gsf;
  trigIds_["HLT_Ele27_eta2p1_WPTight_Gsf"] = kHLT_Ele27_eta2p1_WPTight_Gsf;

  trigIds_["HLT_PFHT300_PFMET110"] = kHLT_PFHT300_PFMET110;
  trigIds_["HLT_PFMET170_HBHECleaned"] = kHLT_PFMET170_HBHECleaned;
  trigIds_["HLT_PFMET110_PFMHT110_IDTight"] = kHLT_PFMET110_PFMHT110_IDTight;
  trigIds_["HLT_PFMETNoMu110_PFMHTNoMu110_IDTight"] = kHLT_PFMETNoMu110_PFMHTNoMu110_IDTight;
  trigIds_["HLT_PFMET120_PFMHT120_IDTight"] = kHLT_PFMET120_PFMHT120_IDTight;
  trigIds_["HLT_PFMETNoMu120_PFMHTNoMu120_IDTight"] = kHLT_PFMETNoMu120_PFMHTNoMu120_IDTight;

  trigIds_["HLT_PFHT400"] = kHLT_PFHT400;
  trigIds_["HLT_PFHT600"] = kHLT_PFHT600;
  trigIds_["HLT_PFHT900"] = kHLT_PFHT900;

  trigIds_["HLT_CaloJet500_NoJetID"] = kHLT_CaloJet500_NoJetID;
  trigIds_["HLT_ECALHT800"] = kHLT_ECALHT800;
  trigIds_["HLT_Photon165_R9Id90_HE10_IsoM"] = kHLT_Photon165_R9Id90_HE10_IsoM;
  trigIds_["HLT_Photon165_HE10"] = kHLT_Photon165_HE10;
  trigIds_["HLT_Photon175"] = kHLT_Photon175;

  // double electron
  trigIds_["HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ"] = kHLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ;
  trigIds_["HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_MW"] = kHLT_DoubleEle33_CaloIdL_GsfTrkIdVL_MW;
  trigIds_["HLT_Ele115_CaloIdVT_GsfTrkIdT"] = kHLT_Ele115_CaloIdVT_GsfTrkIdT;
  // double muon
  trigIds_["HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ"] = kHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ;
  trigIds_["HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ"] = kHLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ;
  trigIds_["HLT_TkMu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ"] = kHLT_TkMu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ;
  trigIds_["HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL"] = kHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL;
  trigIds_["HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL"] = kHLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL;
  trigIds_["HLT_TkMu17_TrkIsoVVL_TkMu8_TrkIsoVVL"] = kHLT_TkMu17_TrkIsoVVL_TkMu8_TrkIsoVVL;
  trigIds_["HLT_Mu30_TkMu11"] = kHLT_Mu30_TkMu11;
  trigIds_["HLT_Mu50"] = kHLT_Mu50;
  trigIds_["HLT_TkMu50"] = kHLT_Mu50;
  // electron + muon
  trigIds_["HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_DZ"] = kHLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_DZ;
  trigIds_["HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ"] = kHLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_DZ;
  trigIds_["HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL"] = kHLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL;
  trigIds_["HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL"] = kHLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL;
  // lep x ht triggers
  trigIds_["HLT_Ele15_IsoVVVL_PFHT350_PFMET50"] = kHLT_Ele15_IsoVVVL_PFHT350_PFMET50;
  trigIds_["HLT_Ele15_IsoVVVL_PFHT400_PFMET50"] = kHLT_Ele15_IsoVVVL_PFHT400_PFMET50;
  trigIds_["HLT_Ele15_IsoVVVL_PFHT350"]         = kHLT_Ele15_IsoVVVL_PFHT350;
  trigIds_["HLT_Ele15_IsoVVVL_PFHT400"]         = kHLT_Ele15_IsoVVVL_PFHT400;
  trigIds_["HLT_Ele15_IsoVVVL_PFHT600"]         = kHLT_Ele15_IsoVVVL_PFHT600;
  trigIds_["HLT_Ele50_IsoVVVL_PFHT400"]         = kHLT_Ele50_IsoVVVL_PFHT400;

  trigIds_["HLT_Mu15_IsoVVVL_PFHT350_PFMET50"]  = kHLT_Mu15_IsoVVVL_PFHT350_PFMET50;
  trigIds_["HLT_Mu15_IsoVVVL_PFHT400_PFMET50"]  = kHLT_Mu15_IsoVVVL_PFHT400_PFMET50;
  trigIds_["HLT_Mu15_IsoVVVL_PFHT350"]          = kHLT_Mu15_IsoVVVL_PFHT350;
  trigIds_["HLT_Mu15_IsoVVVL_PFHT400"]          = kHLT_Mu15_IsoVVVL_PFHT400;
  trigIds_["HLT_Mu15_IsoVVVL_PFHT600"]          = kHLT_Mu15_IsoVVVL_PFHT600;
  trigIds_["HLT_Mu50_IsoVVVL_PFHT400"]          = kHLT_Mu50_IsoVVVL_PFHT400;

  trigFilterIds_["hltPFMET100Filter"] = kPFMET100;
  trigFilterIds_["hltPFMET110Filter"] = kPFMET110;
  trigFilterIds_["hltPFMET120Filter"] = kPFMET120;
  trigFilterIds_["hltPFHT350Filter"] = kPFHT350;
  trigFilterIds_["hltDiCentralPFJet55"] = kDiCentralPFJet55;
  trigFilterIds_["hltDiCentralPFJet70"] = kDiCentralPFJet70;
  trigFilterIds_["hltSingleCaloJet500"] = kCaloJet500;
  trigFilterIds_["hltL3crIsoL1sMu16L1f0L2f10QL3f20QL3trkIsoFiltered0p09"] = kSingleIsoMu20;
  trigFilterIds_["hltL3crIsoL1sMu18L1f0L2f10QL3f20QL3trkIsoFiltered0p09"] = kSingleIsoMu20;
  trigFilterIds_["hltL3crIsoL1sMu20L1f0L2f10QL3f22QL3trkIsoFiltered0p09"] = kSingleIsoMu22;
  trigFilterIds_["hltL3crIsoL1sMu22L1f0L2f10QL3f24QL3trkIsoFiltered0p09"] = kSingleIsoMu24;
  trigFilterIds_["hltL3fL1sMu16L1f0Tkf20QL3trkIsoFiltered0p09"] = kSingleIsoTkMu20;
  trigFilterIds_["hltL3fL1sMu18L1f0Tkf20QL3trkIsoFiltered0p09"] = kSingleIsoTkMu20;
  trigFilterIds_["hltL3fL1sMu20L1f0Tkf22QL3trkIsoFiltered0p09"] = kSingleIsoTkMu22;
  trigFilterIds_["hltL3fL1sMu22L1f0Tkf24QL3trkIsoFiltered0p09"] = kSingleIsoTkMu24;
  trigFilterIds_["hltSingleEle22WPLooseGsfTrackIsoFilter"] = kSingleEle22;
  trigFilterIds_["hltSingleEle22WP75GsfTrackIsoFilter"] = kSingleEle22;
  trigFilterIds_["hltSingleEle22WPTightGsfTrackIsoFilter"] = kSingleEle22WPTight;
  trigFilterIds_["hltEle23WPLooseGsfTrackIsoFilter"] = kSingleEle23NoER;
  trigFilterIds_["hltEle25WPTightGsfTrackIsoFilter"] = kSingleEle25WPTightNoER;
  trigFilterIds_["hltEle25erWPLooseGsfTrackIsoFilter"] = kSingleEle25WPLoose;
  trigFilterIds_["hltEle25erWPTightGsfTrackIsoFilter"] = kSingleEle25WPTight;
  trigFilterIds_["hltEle27noerWPLooseGsfTrackIsoFilter"] = kSingleEle27NoER;
  trigFilterIds_["hltEle27WPLooseGsfTrackIsoFilter"] = kSingleEle27;
  trigFilterIds_["hltEle27erWPLooseGsfTrackIsoFilter"] = kSingleEle27;
  trigFilterIds_["hltEle27WP75GsfTrackIsoFilter"] = kSingleEle27;
  trigFilterIds_["hltEle27WPTightGsfTrackIsoFilter"] = kSingleEle27WPTight;
  trigFilterIds_["hltEle27erWPTightGsfTrackIsoFilter"] = kSingleEle27WPTight;
  trigFilterIds_["hltEle15VVVLGsfTrackIsoFilter"] = kEle15IsoVVVL;
  trigFilterIds_["hltL3MuVVVLIsoFIlter"] = kMu15IsoVVVL;
  trigFilterIds_["hltMu17TrkIsoVVLEle12CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered17"] = kMu17Ele12MuLeg;
  trigFilterIds_["hltMu17TrkIsoVVLEle12CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter"] = kMu17Ele12EleLeg;
  trigFilterIds_["hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered23"] = kMu23Ele12MuLeg;
  trigFilterIds_["hltMu23TrkIsoVVLEle12CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter"] = kMu23Ele12EleLeg;
  trigFilterIds_["hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered8"] = kMu8Ele23MuLeg;
  trigFilterIds_["hltMu8TrkIsoVVLEle23CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter"] = kMu8Ele23EleLeg;
  trigFilterIds_["hltMu8TrkIsoVVLEle17CaloIdLTrackIdLIsoVLMuonlegL3IsoFiltered8"] = kMu8Ele17MuLeg;
  trigFilterIds_["hltMu8TrkIsoVVLEle17CaloIdLTrackIdLIsoVLElectronlegTrackIsoFilter"] = kMu8Ele17EleLeg;
}

//--------------------------------------------------------------------------------------------------
void TriggerFiller::load(const edm::Event& iEvent, const edm::EventSetup &iSetup)
{
  reset();
  iEvent.getByToken(triggerBitToken_, triggerBits_);
  iEvent.getByToken(triggerObjToken_, triggerObjects_);
  iEvent_ = &iEvent;
  if( ! isFastSim_ ) {
    iEvent.getByToken(triggerPrescaleToken_, triggerPrescales_);
  }
  triggerNames_ = &iEvent.triggerNames(*triggerBits_);
  isLoaded_ = true;

}

//--------------------------------------------------------------------------------------------------
void TriggerFiller::fill()
{

  TrigIdMap verTrigIds; // trigId map w/ version number

  for(unsigned int i = 0; i < triggerBits_->size(); ++i) {
    const auto &trigname = triggerNames_->triggerName(i);
    if(options_ & PRINTTRIGGERNAMES) cout << trigname << endl;
    cout << trigname << endl;

    for (const auto &trigindex : trigIds_){
      if(trigname.find(trigindex.first+"_v") != std::string::npos){
        data.fillMulti<unsigned long>(itrig_bit_flag, trigindex.second);
        data.fillMulti<bool         >(itrig_bit_pass, triggerBits_->accept(i));
        data.fillMulti<unsigned int >(itrig_bit_prescale, isFastSim_ ? 1 : triggerPrescales_->getPrescaleForIndex(i));
        verTrigIds[trigname] = trigindex.second;
      }
    }

  }

  for (pat::TriggerObjectStandAlone obj : *triggerObjects_) {
    obj.unpackPathNames(*triggerNames_);
    obj.unpackFilterLabels(*iEvent_, *triggerBits_);
    unsigned long trigflags = 0;
    unsigned long trigfilterflags = 0;
    for (unsigned h = 0; h < obj.filterLabels().size(); ++h) {
      auto filterIndex = trigFilterIds_.find(obj.filterLabels()[h]);
      if(filterIndex != trigFilterIds_.end()) {
        trigfilterflags |= filterIndex->second;
        for(auto trigindex : verTrigIds) {
          if(obj.hasPathName(trigindex.first)) {
            trigflags |= trigindex.second;
          }
        }
      }
    }
	  cout << trigflags << endl;
	  cout << trigfilterflags << endl;
    data.fillMulti<float>(itrig_obj_pt, obj.pt());
    data.fillMulti<float>(itrig_obj_eta, obj.eta());
    data.fillMulti<float>(itrig_obj_phi, obj.phi());
    data.fillMulti<float>(itrig_obj_mass, obj.mass());
    data.fillMulti<unsigned long>(itrig_obj_pathflags, trigflags);
    data.fillMulti<unsigned long>(itrig_obj_filterflags, trigfilterflags);
  }

}
