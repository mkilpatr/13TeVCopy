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

  trigIds_["HLT_IsoMu20_v1"] = kHLT_IsoMu20;
  trigIds_["HLT_IsoMu20_v2"] = kHLT_IsoMu20;
  trigIds_["HLT_IsoMu20_v3"] = kHLT_IsoMu20;
  trigIds_["HLT_IsoMu20_v4"] = kHLT_IsoMu20;
  trigIds_["HLT_IsoTkMu20_v1"] = kHLT_IsoTkMu20;
  trigIds_["HLT_IsoTkMu20_v2"] = kHLT_IsoTkMu20;
  trigIds_["HLT_IsoTkMu20_v3"] = kHLT_IsoTkMu20;
  trigIds_["HLT_IsoTkMu20_v4"] = kHLT_IsoTkMu20;
  trigIds_["HLT_IsoTkMu20_v5"] = kHLT_IsoTkMu20;
  trigIds_["HLT_IsoTkMu20_v6"] = kHLT_IsoTkMu20;
  trigIds_["HLT_IsoMu22_v1"] = kHLT_IsoMu22;
  trigIds_["HLT_IsoMu22_v2"] = kHLT_IsoMu22;
  trigIds_["HLT_IsoMu22_v3"] = kHLT_IsoMu22;
  trigIds_["HLT_IsoTkMu22_v1"] = kHLT_IsoTkMu22;
  trigIds_["HLT_IsoTkMu22_v2"] = kHLT_IsoTkMu22;
  trigIds_["HLT_IsoTkMu22_v3"] = kHLT_IsoTkMu22;
  trigIds_["HLT_IsoTkMu22_v4"] = kHLT_IsoTkMu22;
  trigIds_["HLT_IsoMu24_eta2p1_v1"] = kHLT_IsoMu24_eta2p1;
  trigIds_["HLT_IsoMu24_eta2p1_v2"] = kHLT_IsoMu24_eta2p1;
  trigIds_["HLT_IsoTkMu24_eta2p1_v1"] = kHLT_IsoTkMu24_eta2p1;
  trigIds_["HLT_IsoTkMu24_eta2p1_v2"] = kHLT_IsoTkMu24_eta2p1;
  trigIds_["HLT_Ele22_eta2p1_WPLoose_Gsf_v1"] = kHLT_Ele22_eta2p1_WPLoose_Gsf;
  trigIds_["HLT_Ele22_eta2p1_WPLoose_Gsf_v2"] = kHLT_Ele22_eta2p1_WPLoose_Gsf;
  trigIds_["HLT_Ele22_eta2p1_WPLoose_Gsf_v3"] = kHLT_Ele22_eta2p1_WPLoose_Gsf;
  trigIds_["HLT_Ele22_eta2p1_WPLoose_Gsf_v4"] = kHLT_Ele22_eta2p1_WPLoose_Gsf;
  trigIds_["HLT_Ele22_eta2p1_WP75_Gsf_v1"] = kHLT_Ele22_eta2p1_WP75_Gsf;
  trigIds_["HLT_Ele22_eta2p1_WPTight_Gsf_v1"] = kHLT_Ele22_eta2p1_WPTight_Gsf;
  trigIds_["HLT_Ele22_eta2p1_WPTight_Gsf_v2"] = kHLT_Ele22_eta2p1_WPTight_Gsf;
  trigIds_["HLT_Ele22_eta2p1_WPTight_Gsf_v3"] = kHLT_Ele22_eta2p1_WPTight_Gsf;
  trigIds_["HLT_Ele23_WPLoose_Gsf_v1"] = kHLT_Ele23_WPLoose_Gsf;
  trigIds_["HLT_Ele23_WPLoose_Gsf_v2"] = kHLT_Ele23_WPLoose_Gsf;
  trigIds_["HLT_Ele23_WPLoose_Gsf_v3"] = kHLT_Ele23_WPLoose_Gsf;
  trigIds_["HLT_Ele23_WPLoose_Gsf_v4"] = kHLT_Ele23_WPLoose_Gsf;
  trigIds_["HLT_Ele23_WPLoose_Gsf_v5"] = kHLT_Ele23_WPLoose_Gsf;
  trigIds_["HLT_Ele23_WPLoose_Gsf_v6"] = kHLT_Ele23_WPLoose_Gsf;
  trigIds_["HLT_Ele25_WPTight_Gsf_v1"] = kHLT_Ele25_WPTight_Gsf;
  trigIds_["HLT_Ele25_WPTight_Gsf_v2"] = kHLT_Ele25_WPTight_Gsf;
  trigIds_["HLT_Ele25_WPTight_Gsf_v3"] = kHLT_Ele25_WPTight_Gsf;
  trigIds_["HLT_Ele25_WPTight_Gsf_v4"] = kHLT_Ele25_WPTight_Gsf;
  trigIds_["HLT_Ele27_WPLoose_Gsf_v1"] = kHLT_Ele27_WPLoose_Gsf;
  trigIds_["HLT_Ele27_WPLoose_Gsf_v2"] = kHLT_Ele27_WPLoose_Gsf;
  trigIds_["HLT_Ele27_WPLoose_Gsf_v3"] = kHLT_Ele27_WPLoose_Gsf;
  trigIds_["HLT_Ele27_WPLoose_Gsf_v4"] = kHLT_Ele27_WPLoose_Gsf;
  trigIds_["HLT_Ele25_eta2p1_WPLoose_Gsf_v1"] = kHLT_Ele25_eta2p1_WPLoose_Gsf;
  trigIds_["HLT_Ele25_eta2p1_WPLoose_Gsf_v2"] = kHLT_Ele25_eta2p1_WPLoose_Gsf;
  trigIds_["HLT_Ele25_eta2p1_WPLoose_Gsf_v3"] = kHLT_Ele25_eta2p1_WPLoose_Gsf;
  trigIds_["HLT_Ele25_eta2p1_WPLoose_Gsf_v4"] = kHLT_Ele25_eta2p1_WPLoose_Gsf;
  trigIds_["HLT_Ele25_eta2p1_WPTight_Gsf_v1"] = kHLT_Ele25_eta2p1_WPTight_Gsf;
  trigIds_["HLT_Ele25_eta2p1_WPTight_Gsf_v2"] = kHLT_Ele25_eta2p1_WPTight_Gsf;
  trigIds_["HLT_Ele25_eta2p1_WPTight_Gsf_v3"] = kHLT_Ele25_eta2p1_WPTight_Gsf;
  trigIds_["HLT_Ele25_eta2p1_WPTight_Gsf_v4"] = kHLT_Ele25_eta2p1_WPTight_Gsf;
  trigIds_["HLT_Ele27_eta2p1_WPLoose_Gsf_v1"] = kHLT_Ele27_eta2p1_WPLoose_Gsf;
  trigIds_["HLT_Ele27_eta2p1_WPLoose_Gsf_v2"] = kHLT_Ele27_eta2p1_WPLoose_Gsf;
  trigIds_["HLT_Ele27_eta2p1_WPLoose_Gsf_v3"] = kHLT_Ele27_eta2p1_WPLoose_Gsf;
  trigIds_["HLT_Ele27_eta2p1_WPLoose_Gsf_v4"] = kHLT_Ele27_eta2p1_WPLoose_Gsf;
  trigIds_["HLT_Ele27_eta2p1_WPLoose_Gsf_v5"] = kHLT_Ele27_eta2p1_WPLoose_Gsf;
  trigIds_["HLT_Ele27_eta2p1_WPTight_Gsf_v1"] = kHLT_Ele27_eta2p1_WPTight_Gsf;
  trigIds_["HLT_Ele27_eta2p1_WPTight_Gsf_v2"] = kHLT_Ele27_eta2p1_WPTight_Gsf;
  trigIds_["HLT_Ele27_eta2p1_WPTight_Gsf_v3"] = kHLT_Ele27_eta2p1_WPTight_Gsf;
  trigIds_["HLT_Ele27_eta2p1_WPTight_Gsf_v4"] = kHLT_Ele27_eta2p1_WPTight_Gsf;
  trigIds_["HLT_Ele27_eta2p1_WPTight_Gsf_v5"] = kHLT_Ele27_eta2p1_WPTight_Gsf;
  trigIds_["HLT_Ele27_eta2p1_WP75_Gsf_v1"] = kHLT_Ele27_eta2p1_WP75_Gsf;
  trigIds_["HLT_DiCentralPFJet55_PFMET110_NoiseCleaned_v1"] = kHLT_DiCentralPFJet55_PFMET110_NoiseCleaned;
  trigIds_["HLT_DiCentralPFJet55_PFMET110_JetIdCleaned_v1"] = kHLT_DiCentralPFJet55_PFMET110_NoiseCleaned;
  trigIds_["HLT_DiCentralPFJet55_PFMET110_JetIdCleaned_v2"] = kHLT_DiCentralPFJet55_PFMET110_NoiseCleaned;
  trigIds_["HLT_DiCentralPFJet55_PFMET110_v1"] = kHLT_DiCentralPFJet55_PFMET110_NoiseCleaned;
  trigIds_["HLT_DiCentralPFJet55_PFMET110_v2"] = kHLT_DiCentralPFJet55_PFMET110_NoiseCleaned;
  trigIds_["HLT_DiCentralPFJet55_PFMET110_v3"] = kHLT_DiCentralPFJet55_PFMET110_NoiseCleaned;
  trigIds_["HLT_DiCentralPFJet70_PFMET120_NoiseCleaned_v1"] = kHLT_DiCentralPFJet70_PFMET120_NoiseCleaned;
  trigIds_["HLT_PFHT350_PFMET100_NoiseCleaned_v1"] = kHLT_PFHT350_PFMET100_NoiseCleaned;
  trigIds_["HLT_PFHT350_PFMET100_JetIdCleaned_v1"] = kHLT_PFHT350_PFMET100_JetIdCleaned;
  trigIds_["HLT_PFHT350_PFMET100_JetIdCleaned_v2"] = kHLT_PFHT350_PFMET100_JetIdCleaned;
  trigIds_["HLT_PFHT350_PFMET100_v1"] = kHLT_PFHT350_PFMET100_JetIdCleaned;
  trigIds_["HLT_PFHT350_PFMET120_NoiseCleaned_v1"] = kHLT_PFHT350_PFMET120_NoiseCleaned;
  trigIds_["HLT_PFHT300_PFMET100_v1"] = kHLT_PFHT300_PFMET100;
  trigIds_["HLT_PFHT300_PFMET100_v2"] = kHLT_PFHT300_PFMET100;
  trigIds_["HLT_PFHT300_PFMET100_v3"] = kHLT_PFHT300_PFMET100;
  trigIds_["HLT_PFHT300_PFMET110_v1"] = kHLT_PFHT300_PFMET110;
  trigIds_["HLT_PFHT300_PFMET110_v2"] = kHLT_PFHT300_PFMET110;
  trigIds_["HLT_PFHT300_PFMET110_v3"] = kHLT_PFHT300_PFMET110;
  trigIds_["HLT_PFMET170_NoiseCleaned_v1"] = kHLT_PFMET170_NoiseCleaned;
  trigIds_["HLT_PFMET170_NoiseCleaned_v2"] = kHLT_PFMET170_NoiseCleaned;
  trigIds_["HLT_PFMET170_NoiseCleaned_v3"] = kHLT_PFMET170_NoiseCleaned;
  trigIds_["HLT_PFMET170_NoiseCleaned_v4"] = kHLT_PFMET170_NoiseCleaned;
  trigIds_["HLT_PFMET170_JetIdCleaned_v1"] = kHLT_PFMET170_JetIdCleaned;
  trigIds_["HLT_PFMET170_JetIdCleaned_v2"] = kHLT_PFMET170_JetIdCleaned;
  trigIds_["HLT_PFMET170_JetIdCleaned_v3"] = kHLT_PFMET170_JetIdCleaned;
  trigIds_["HLT_PFMET170_v1"] = kHLT_PFMET170_NoiseCleaned;
  trigIds_["HLT_PFMET170_v2"] = kHLT_PFMET170_NoiseCleaned;
  trigIds_["HLT_PFMET90_PFMHT90_IDTight_v1"] = kHLT_PFMET90_PFMHT90_IDTight;
  trigIds_["HLT_PFMET90_PFMHT90_IDTight_v2"] = kHLT_PFMET90_PFMHT90_IDTight;
  trigIds_["HLT_PFMET90_PFMHT90_IDTight_v3"] = kHLT_PFMET90_PFMHT90_IDTight;
  trigIds_["HLT_PFMET90_PFMHT90_IDTight_v4"] = kHLT_PFMET90_PFMHT90_IDTight;
  trigIds_["HLT_PFMET100_PFMHT100_IDTight_v1"] = kHLT_PFMET100_PFMHT100_IDTight;
  trigIds_["HLT_PFMET100_PFMHT100_IDTight_v2"] = kHLT_PFMET100_PFMHT100_IDTight;
  trigIds_["HLT_PFMET100_PFMHT100_IDTight_v3"] = kHLT_PFMET100_PFMHT100_IDTight;
  trigIds_["HLT_PFMET100_PFMHT100_IDTight_v4"] = kHLT_PFMET100_PFMHT100_IDTight;
  trigIds_["HLT_PFMET110_PFMHT110_IDTight_v1"] = kHLT_PFMET110_PFMHT110_IDTight;
  trigIds_["HLT_PFMET110_PFMHT110_IDTight_v2"] = kHLT_PFMET110_PFMHT110_IDTight;
  trigIds_["HLT_PFMET110_PFMHT110_IDTight_v3"] = kHLT_PFMET110_PFMHT110_IDTight;
  trigIds_["HLT_PFMET110_PFMHT110_IDTight_v4"] = kHLT_PFMET110_PFMHT110_IDTight;
  trigIds_["HLT_PFMETNoMu90_JetIdCleaned_PFMHTNoMu90_IDTight_v1"] = kHLT_PFMETNoMu90_PFMHTNoMu90_IDTight;
  trigIds_["HLT_PFMETNoMu90_JetIdCleaned_PFMHTNoMu90_IDTight_v2"] = kHLT_PFMETNoMu90_PFMHTNoMu90_IDTight;
  trigIds_["HLT_PFMETNoMu90_JetIdCleaned_PFMHTNoMu90_IDTight_v3"] = kHLT_PFMETNoMu90_PFMHTNoMu90_IDTight;
  trigIds_["HLT_PFMETNoMu90_PFMHTNoMu90_IDTight_v1"] = kHLT_PFMETNoMu90_PFMHTNoMu90_IDTight;
  trigIds_["HLT_PFMETNoMu90_PFMHTNoMu90_IDTight_v2"] = kHLT_PFMETNoMu90_PFMHTNoMu90_IDTight;
  trigIds_["HLT_PFMETNoMu90_PFMHTNoMu90_IDTight_v3"] = kHLT_PFMETNoMu90_PFMHTNoMu90_IDTight;
  trigIds_["HLT_PFMETNoMu90_PFMHTNoMu90_IDTight_v4"] = kHLT_PFMETNoMu90_PFMHTNoMu90_IDTight;
  trigIds_["HLT_PFMETNoMu100_PFMHTNoMu100_IDTight_v1"] = kHLT_PFMETNoMu100_PFMHTNoMu100_IDTight;
  trigIds_["HLT_PFMETNoMu100_PFMHTNoMu100_IDTight_v2"] = kHLT_PFMETNoMu100_PFMHTNoMu100_IDTight;
  trigIds_["HLT_PFMETNoMu100_PFMHTNoMu100_IDTight_v3"] = kHLT_PFMETNoMu100_PFMHTNoMu100_IDTight;
  trigIds_["HLT_PFMETNoMu100_PFMHTNoMu100_IDTight_v4"] = kHLT_PFMETNoMu100_PFMHTNoMu100_IDTight;
  trigIds_["HLT_PFMETNoMu110_PFMHTNoMu110_IDTight_v1"] = kHLT_PFMETNoMu110_PFMHTNoMu110_IDTight;
  trigIds_["HLT_PFMETNoMu110_PFMHTNoMu110_IDTight_v2"] = kHLT_PFMETNoMu110_PFMHTNoMu110_IDTight;
  trigIds_["HLT_PFMETNoMu110_PFMHTNoMu110_IDTight_v3"] = kHLT_PFMETNoMu110_PFMHTNoMu110_IDTight;
  trigIds_["HLT_PFMETNoMu110_PFMHTNoMu110_IDTight_v4"] = kHLT_PFMETNoMu110_PFMHTNoMu110_IDTight;
  trigIds_["HLT_PFHT300_v1"] = kHLT_PFHT300;
  trigIds_["HLT_PFHT300_v2"] = kHLT_PFHT300;
  trigIds_["HLT_PFHT300_v3"] = kHLT_PFHT300;
  trigIds_["HLT_PFHT300_v4"] = kHLT_PFHT300;
  trigIds_["HLT_PFHT350_v1"] = kHLT_PFHT350;
  trigIds_["HLT_PFHT350_v2"] = kHLT_PFHT350;
  trigIds_["HLT_PFHT350_v3"] = kHLT_PFHT350;
  trigIds_["HLT_PFHT350_v4"] = kHLT_PFHT350;
  trigIds_["HLT_PFHT350_v5"] = kHLT_PFHT350;
  trigIds_["HLT_PFHT400_v1"] = kHLT_PFHT400;
  trigIds_["HLT_PFHT400_v2"] = kHLT_PFHT400;
  trigIds_["HLT_PFHT400_v3"] = kHLT_PFHT400;
  trigIds_["HLT_PFHT400_v4"] = kHLT_PFHT400;
  trigIds_["HLT_PFHT475_v1"] = kHLT_PFHT475;
  trigIds_["HLT_PFHT475_v2"] = kHLT_PFHT475;
  trigIds_["HLT_PFHT475_v3"] = kHLT_PFHT475;
  trigIds_["HLT_PFHT475_v4"] = kHLT_PFHT475;
  trigIds_["HLT_PFHT600_v1"] = kHLT_PFHT600;
  trigIds_["HLT_PFHT600_v2"] = kHLT_PFHT600;
  trigIds_["HLT_PFHT600_v3"] = kHLT_PFHT600;
  trigIds_["HLT_PFHT600_v4"] = kHLT_PFHT600;
  trigIds_["HLT_PFHT600_v5"] = kHLT_PFHT600;
  trigIds_["HLT_PFHT650_v1"] = kHLT_PFHT650;
  trigIds_["HLT_PFHT650_v2"] = kHLT_PFHT650;
  trigIds_["HLT_PFHT650_v3"] = kHLT_PFHT650;
  trigIds_["HLT_PFHT650_v4"] = kHLT_PFHT650;
  trigIds_["HLT_PFHT650_v5"] = kHLT_PFHT650;
  trigIds_["HLT_PFHT800_v1"] = kHLT_PFHT800;
  trigIds_["HLT_PFHT800_v2"] = kHLT_PFHT800;
  trigIds_["HLT_PFHT800_v3"] = kHLT_PFHT800;
  trigIds_["HLT_PFHT800_v4"] = kHLT_PFHT800;
  trigIds_["HLT_PFHT900_v1"] = kHLT_PFHT900;
  trigIds_["HLT_PFHT900_v2"] = kHLT_PFHT900;
  trigIds_["HLT_PFHT900_v3"] = kHLT_PFHT900;
  trigIds_["HLT_Photon165_R9Id90_HE10_IsoM_v1"] = kHLT_Photon165_R9Id90_HE10_IsoM;
  trigIds_["HLT_Photon165_R9Id90_HE10_IsoM_v2"] = kHLT_Photon165_R9Id90_HE10_IsoM;
  trigIds_["HLT_Photon165_R9Id90_HE10_IsoM_v3"] = kHLT_Photon165_R9Id90_HE10_IsoM;
  trigIds_["HLT_Photon165_R9Id90_HE10_IsoM_v4"] = kHLT_Photon165_R9Id90_HE10_IsoM;
  trigIds_["HLT_Photon165_R9Id90_HE10_IsoM_v5"] = kHLT_Photon165_R9Id90_HE10_IsoM;
  trigIds_["HLT_Photon165_R9Id90_HE10_IsoM_v6"] = kHLT_Photon165_R9Id90_HE10_IsoM;
  trigIds_["HLT_Photon165_HE10_v1"] = kHLT_Photon165_HE10;
  trigIds_["HLT_Photon165_HE10_v2"] = kHLT_Photon165_HE10;
  trigIds_["HLT_Photon165_HE10_v3"] = kHLT_Photon165_HE10;
  trigIds_["HLT_Photon165_HE10_v4"] = kHLT_Photon165_HE10;
  trigIds_["HLT_Photon165_HE10_v5"] = kHLT_Photon165_HE10;
  trigIds_["HLT_Photon165_HE10_v6"] = kHLT_Photon165_HE10;
  trigIds_["HLT_Photon175_v1"] = kHLT_Photon175;
  trigIds_["HLT_Photon175_v2"] = kHLT_Photon175;
  trigIds_["HLT_Photon175_v3"] = kHLT_Photon175;
  trigIds_["HLT_Photon175_v4"] = kHLT_Photon175;
  trigIds_["HLT_Photon175_v5"] = kHLT_Photon175;
  trigIds_["HLT_Photon175_v6"] = kHLT_Photon175;
  // double electron
  trigIds_["HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v1"] = kHLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ;
  trigIds_["HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v2"] = kHLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ;
  trigIds_["HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v3"] = kHLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ;
  trigIds_["HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v4"] = kHLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ;
  trigIds_["HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v5"] = kHLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ;
  trigIds_["HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v6"] = kHLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ;
  trigIds_["HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v1"] = kHLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ;
  trigIds_["HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v2"] = kHLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ;
  trigIds_["HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v3"] = kHLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ;
  trigIds_["HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v4"] = kHLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ;
  trigIds_["HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v5"] = kHLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ;
  trigIds_["HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v6"] = kHLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ;
  // double muon
  trigIds_["HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v1"] = kHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ;
  trigIds_["HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v2"] = kHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ;
  trigIds_["HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v3"] = kHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ;
  trigIds_["HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v4"] = kHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ;
  trigIds_["HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v1"] = kHLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ;
  trigIds_["HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v2"] = kHLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ;
  trigIds_["HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v3"] = kHLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ;
  // electron + muon
  trigIds_["HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v1"] = kHLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL;
  trigIds_["HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v2"] = kHLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL;
  trigIds_["HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v3"] = kHLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL;
  trigIds_["HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v4"] = kHLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL;
  trigIds_["HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v5"] = kHLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL;
  trigIds_["HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v6"] = kHLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL;
  trigIds_["HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v1"] = kHLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL;
  trigIds_["HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v2"] = kHLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL;
  trigIds_["HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v3"] = kHLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL;
  trigIds_["HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v4"] = kHLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL;
  trigIds_["HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v5"] = kHLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL;
  trigIds_["HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v6"] = kHLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL;
  trigIds_["HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v1"] = kHLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL;
  trigIds_["HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v2"] = kHLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL;
  trigIds_["HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v3"] = kHLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL;
  trigIds_["HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v4"] = kHLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL;
  trigIds_["HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v5"] = kHLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL;
  trigIds_["HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v6"] = kHLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL;
  trigIds_["HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v1"] = kHLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL;
  trigIds_["HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v2"] = kHLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL;
  trigIds_["HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v3"] = kHLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL;
  trigIds_["HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v4"] = kHLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL;
  trigIds_["HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v5"] = kHLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL;
  trigIds_["HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v6"] = kHLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL;
  // lep x ht triggers
  trigIds_["HLT_Ele15_IsoVVVL_PFHT350_PFMET50_v1"] = kHLT_Ele15_IsoVVVL_PFHT350_PFMET50;
  trigIds_["HLT_Ele15_IsoVVVL_PFHT350_PFMET50_v2"] = kHLT_Ele15_IsoVVVL_PFHT350_PFMET50;
  trigIds_["HLT_Ele15_IsoVVVL_PFHT350_PFMET50_v3"] = kHLT_Ele15_IsoVVVL_PFHT350_PFMET50;
  trigIds_["HLT_Ele15_IsoVVVL_PFHT350_PFMET50_v4"] = kHLT_Ele15_IsoVVVL_PFHT350_PFMET50;
  trigIds_["HLT_Ele15_IsoVVVL_PFHT350_PFMET50_v5"] = kHLT_Ele15_IsoVVVL_PFHT350_PFMET50;
  trigIds_["HLT_Ele15_IsoVVVL_PFHT400_PFMET50_v1"] = kHLT_Ele15_IsoVVVL_PFHT400_PFMET50;
  trigIds_["HLT_Ele15_IsoVVVL_PFHT400_PFMET50_v2"] = kHLT_Ele15_IsoVVVL_PFHT400_PFMET50;
  trigIds_["HLT_Ele15_IsoVVVL_PFHT400_PFMET50_v3"] = kHLT_Ele15_IsoVVVL_PFHT400_PFMET50;
  trigIds_["HLT_Ele15_IsoVVVL_PFHT350_v1"]         = kHLT_Ele15_IsoVVVL_PFHT350;
  trigIds_["HLT_Ele15_IsoVVVL_PFHT350_v2"]         = kHLT_Ele15_IsoVVVL_PFHT350;
  trigIds_["HLT_Ele15_IsoVVVL_PFHT350_v3"]         = kHLT_Ele15_IsoVVVL_PFHT350;
  trigIds_["HLT_Ele15_IsoVVVL_PFHT350_v4"]         = kHLT_Ele15_IsoVVVL_PFHT350;
  trigIds_["HLT_Ele15_IsoVVVL_PFHT350_v5"]         = kHLT_Ele15_IsoVVVL_PFHT350;
  trigIds_["HLT_Ele15_IsoVVVL_PFHT400_v1"]         = kHLT_Ele15_IsoVVVL_PFHT400;
  trigIds_["HLT_Ele15_IsoVVVL_PFHT400_v2"]         = kHLT_Ele15_IsoVVVL_PFHT400;
  trigIds_["HLT_Ele15_IsoVVVL_PFHT400_v3"]         = kHLT_Ele15_IsoVVVL_PFHT400;
  trigIds_["HLT_Ele15_IsoVVVL_PFHT600_v1"]         = kHLT_Ele15_IsoVVVL_PFHT600;
  trigIds_["HLT_Ele15_IsoVVVL_PFHT600_v2"]         = kHLT_Ele15_IsoVVVL_PFHT600;
  trigIds_["HLT_Ele15_IsoVVVL_PFHT600_v3"]         = kHLT_Ele15_IsoVVVL_PFHT600;
  trigIds_["HLT_Ele15_IsoVVVL_PFHT600_v4"]         = kHLT_Ele15_IsoVVVL_PFHT600;
  trigIds_["HLT_Ele15_IsoVVVL_PFHT600_v5"]         = kHLT_Ele15_IsoVVVL_PFHT600;
  trigIds_["HLT_Ele15_IsoVVVL_PFHT600_v6"]         = kHLT_Ele15_IsoVVVL_PFHT600;
  trigIds_["HLT_Mu15_IsoVVVL_PFHT350_PFMET50_v1"]  = kHLT_Mu15_IsoVVVL_PFHT350_PFMET50;
  trigIds_["HLT_Mu15_IsoVVVL_PFHT350_PFMET50_v2"]  = kHLT_Mu15_IsoVVVL_PFHT350_PFMET50;
  trigIds_["HLT_Mu15_IsoVVVL_PFHT350_PFMET50_v3"]  = kHLT_Mu15_IsoVVVL_PFHT350_PFMET50;
  trigIds_["HLT_Mu15_IsoVVVL_PFHT350_PFMET50_v4"]  = kHLT_Mu15_IsoVVVL_PFHT350_PFMET50;
  trigIds_["HLT_Mu15_IsoVVVL_PFHT400_PFMET50_v1"]  = kHLT_Mu15_IsoVVVL_PFHT400_PFMET50;
  trigIds_["HLT_Mu15_IsoVVVL_PFHT400_PFMET50_v2"]  = kHLT_Mu15_IsoVVVL_PFHT400_PFMET50;
  trigIds_["HLT_Mu15_IsoVVVL_PFHT350_v1"]          = kHLT_Mu15_IsoVVVL_PFHT350;
  trigIds_["HLT_Mu15_IsoVVVL_PFHT350_v2"]          = kHLT_Mu15_IsoVVVL_PFHT350;
  trigIds_["HLT_Mu15_IsoVVVL_PFHT350_v3"]          = kHLT_Mu15_IsoVVVL_PFHT350;
  trigIds_["HLT_Mu15_IsoVVVL_PFHT350_v4"]          = kHLT_Mu15_IsoVVVL_PFHT350;
  trigIds_["HLT_Mu15_IsoVVVL_PFHT400_v1"]          = kHLT_Mu15_IsoVVVL_PFHT400;
  trigIds_["HLT_Mu15_IsoVVVL_PFHT400_v2"]          = kHLT_Mu15_IsoVVVL_PFHT400;
  trigIds_["HLT_Mu15_IsoVVVL_PFHT600_v1"]          = kHLT_Mu15_IsoVVVL_PFHT600;
  trigIds_["HLT_Mu15_IsoVVVL_PFHT600_v2"]          = kHLT_Mu15_IsoVVVL_PFHT600;
  trigIds_["HLT_Mu15_IsoVVVL_PFHT600_v3"]          = kHLT_Mu15_IsoVVVL_PFHT600;
  trigIds_["HLT_Mu15_IsoVVVL_PFHT600_v4"]          = kHLT_Mu15_IsoVVVL_PFHT600;
  trigIds_["HLT_Mu15_IsoVVVL_PFHT600_v5"]          = kHLT_Mu15_IsoVVVL_PFHT600;

  trigFilterIds_["hltPFMET100Filter"] = kPFMET100;
  trigFilterIds_["hltPFMET110Filter"] = kPFMET110;
  trigFilterIds_["hltPFMET120Filter"] = kPFMET120;
  trigFilterIds_["hltPFHT350Filter"] = kPFHT350;
  trigFilterIds_["hltDiCentralPFJet55"] = kDiCentralPFJet55;
  trigFilterIds_["hltDiCentralPFJet70"] = kDiCentralPFJet70;
  trigFilterIds_["hltL3crIsoL1sMu16L1f0L2f10QL3f20QL3trkIsoFiltered0p09"] = kSingleIsoMu20;
  trigFilterIds_["hltL3crIsoL1sMu18L1f0L2f10QL3f20QL3trkIsoFiltered0p09"] = kSingleIsoMu20;
  trigFilterIds_["hltL3crIsoL1sMu20L1f0L2f10QL3f22QL3trkIsoFiltered0p09"] = kSingleIsoMu22;
  trigFilterIds_["hltL3crIsoL1sMu20Eta2p1L1f0L2f10QL3f24QL3trkIsoFiltered0p09"] = kSingleIsoMu24;
  trigFilterIds_["hltL3fL1sMu16L1f0Tkf20QL3trkIsoFiltered0p09"] = kSingleIsoTkMu20;
  trigFilterIds_["hltL3fL1sMu18L1f0Tkf20QL3trkIsoFiltered0p09"] = kSingleIsoTkMu20;
  trigFilterIds_["hltL3fL1sMu20L1f0Tkf22QL3trkIsoFiltered0p09"] = kSingleIsoTkMu22;
  trigFilterIds_["hltL3fL1sMu20Eta2p1L1f0Tkf24QL3trkIsoFiltered0p09"] = kSingleIsoTkMu24;
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
  if( ! isFastSim_ ) {
    iEvent.getByToken(triggerPrescaleToken_, triggerPrescales_);
  }
  triggerNames_ = &iEvent.triggerNames(*triggerBits_);
  isLoaded_ = true;

}

//--------------------------------------------------------------------------------------------------
void TriggerFiller::fill()
{

  for(unsigned int i = 0; i < triggerBits_->size(); ++i) {
    if(options_ & PRINTTRIGGERNAMES) cout << triggerNames_->triggerName(i) << endl;
    auto trigindex = trigIds_.find(triggerNames_->triggerName(i));

    if(trigindex != trigIds_.end()) {
      data.fillMulti<unsigned long>(itrig_bit_flag, trigindex->second);
      data.fillMulti<bool         >(itrig_bit_pass, triggerBits_->accept(i));
      data.fillMulti<unsigned int >(itrig_bit_prescale, isFastSim_ ? 1 : triggerPrescales_->getPrescaleForIndex(i));
    }
  }

  for (pat::TriggerObjectStandAlone obj : *triggerObjects_) {
    obj.unpackPathNames(*triggerNames_);
    unsigned long trigflags = 0;
    unsigned long trigfilterflags = 0;
    for (unsigned h = 0; h < obj.filterLabels().size(); ++h) {
      auto filterIndex = trigFilterIds_.find(obj.filterLabels()[h]);
      if(filterIndex != trigFilterIds_.end()) {
        trigfilterflags |= filterIndex->second;
        for(auto trigindex : trigIds_) {
          if(obj.hasPathName(trigindex.first)) {
            trigflags |= trigindex.second;
          }
        }
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
