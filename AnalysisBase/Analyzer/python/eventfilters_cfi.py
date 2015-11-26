import FWCore.ParameterSet.Config as cms

from AnalysisBase.Analyzer.analyzer_configuration_cfi import physicsAnalyzer_configuration
from AnalysisBase.Analyzer.analyzer_configuration_cfi import nominal_configuration
                            
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


met200Filter = cms.EDFilter('METFilter',
  nominal_configuration,
  minMET = cms.double(200),
  maxMET = cms.double(-1),
  verbose = cms.bool(False)
)

met131TeVFilter = met200Filter.clone( minMET = cms.double(-1), maxMET = cms.double(131000),verbose = cms.bool(True))