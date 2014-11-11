import FWCore.ParameterSet.Config as cms

redefinedAK4Producer = cms.EDProducer('RedefinedJetProducer',
                                    isRealData = cms.int32(0),
                                    src = cms.InputTag("packedPFCandidates"),                                    
                                    produceGen      = cms.bool(True),
                                    producePU       = cms.bool(False),
                                    genSrc      = cms.InputTag('packedGenParticles'),
                                    jetAlgorithm      = cms.string('AntiKt'),
                                    rParameter      = cms.double(.4),
                                    jetPtMin       = cms.double(10),
                                    minParticlePT     = cms.double          (0),
                                    maxParticleEta    = cms.double          (5),
                                    ghostArea         = cms.double          (0.001)
)

redefinedCA1Producer = cms.EDProducer('RedefinedJetProducer',
                                    isRealData = cms.int32(0),
                                    src = cms.InputTag("packedPFCandidates"),                                    
                                    produceGen      = cms.bool(True),
                                    producePU       = cms.bool(False),
                                    genSrc      = cms.InputTag('packedGenParticles'),
                                    jetAlgorithm      = cms.string('CambridgeAachen'),
                                    rParameter      = cms.double(1),
                                    jetPtMin          = cms.double(20),
                                    minParticlePT     = cms.double          (0),
                                    maxParticleEta    = cms.double          (3),
                                    ghostArea         = cms.double          (0.001)
)
