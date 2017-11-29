import FWCore.ParameterSet.Config as cms
from AnalysisBase.Analyzer.analyzer_configuration_cfi import nominal_configuration

prodIsoTrks = cms.EDFilter(
  "TestAnalyzer",
  nominal_configuration,
  vtxSrc = cms.InputTag('offlineSlimmedPrimaryVertices'),#prodGoodVertices'),#goodVertices'),
  metSrc = cms.InputTag('slimmedMETs'),
  forVetoIsoTrkSrc = cms.InputTag("trackIsolation"),
  pfCandSrc = cms.InputTag("packedPFCandidates"),
  isotrk_dR = cms.double(0.5),
  isotrk_dz = cms.double(0.15),
  loose_isoTrkSrc = cms.InputTag("loosetrackIsolation"),
  loose_isotrk_isoVecSrc = cms.InputTag("loosetrackIsolation:pfcandstrkiso"),
  loose_isotrk_dzpvVecSrc = cms.InputTag("loosetrackIsolation:pfcandsdzpv"),
  #W_emuVec = cms.InputTag("prodGenInfo:WemuVec"),
  #W_tau_emuVec = cms.InputTag("prodGenInfo:WtauemuVec"),
  #W_tau_prongsVec = cms.InputTag("prodGenInfo:WtauprongsVec"),
  #genDecayLVec = cms.InputTag("prodGenInfo:genDecayLVec"),
  debug  = cms.bool(False),
  #ref_all_isoTrkSrc = cms.InputTag("refalltrackIsolation"),
  #ref_all_isoTrk_isoVecSrc = cms.InputTag("refalltrackIsolation:pfcandstrkiso"),

  # Name of the output tree
  TreeName          = cms.string('AUX'),
  ## might help if something isn working wilil produce couts
  # List of InputTags for Float_t variables (Float_t) stored in
  # optional names to store in the tree can be defined  not only will have an effect if the number of input tags for variable is exactly the same as number of optional names!!
  varsDouble        = cms.VInputTag(),
  varsDoubleNamesInTree = cms.vstring(),
  # List of InputTags for Int variables (Int) stored in
  # optional names to store in the tree can be defined  not only will have an effect if the number of input tags for variable is exactly the same as number of optional names!!
  varsInt        = cms.VInputTag(),
  varsIntNamesInTree = cms.vstring(),
  # List of InputTags for bool variables (bool) stored in
  # optional names to store in the tree can be defined  not only will have an effect if the number of input tags for variable is exactly the same as number of optional names!!
  varsBool        = cms.VInputTag(),
  varsBoolNamesInTree = cms.vstring(),
  
  varsString      = cms.VInputTag(),
  varsStringNamesInTree = cms.vstring(),
  
  # List of InputTags for TLorentz variables (TLorentz) stored in
  # optional names to store in the tree can be defined  not only will have an effect if the number of input tags for variable is exactly the same as number of optional names!!
  varsTLorentzVector        = cms.VInputTag(),
  varsTLorentzVectorNamesInTree = cms.vstring(),
  
  vectorDouble              = cms.VInputTag(),
  vectorDoubleNamesInTree   = cms.vstring(),
  
  vectorInt                 = cms.VInputTag(),
  vectorIntNamesInTree      = cms.vstring(),
  
  vectorBool                = cms.VInputTag(),
  vectorBoolNamesInTree     = cms.vstring(),
  
  vectorString              = cms.VInputTag(),
  vectorStringNamesInTree   = cms.vstring(),
  
  vectorTLorentzVector        = cms.VInputTag(),
  vectorTLorentzVectorNamesInTree = cms.vstring(),
)
