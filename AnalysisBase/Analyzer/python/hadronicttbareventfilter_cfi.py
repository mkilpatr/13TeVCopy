import FWCore.ParameterSet.Config as cms

from AnalysisBase.Analyzer.analyzer_configuration_cfi import physicsAnalyzer_configuration
                            
hadronicTTBarEventFilter = cms.EDFilter('HadronicTTBarEventFilter',
  physicsAnalyzer_configuration,                                  
  Gen = cms.untracked.PSet(
    isFilled            = cms.untracked.bool(True),
    genEventInfoSource  = cms.InputTag (''),                                                                                
    packedGenParticles  = cms.InputTag('packedGenParticles'),
    prunedGenParticles  = cms.InputTag('prunedGenParticles'),
    saveAllGenParticles = cms.untracked.bool(False),
    savePartonHadronization = cms.untracked.bool(False),
)
)