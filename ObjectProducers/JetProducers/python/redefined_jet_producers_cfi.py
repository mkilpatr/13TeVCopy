import FWCore.ParameterSet.Config as cms



redAK4             = cms.EDProducer('PackedReJetProducer',
                                    isRealData = cms.int32(0),
                                    produceGen      = cms.bool(True),
                                    ignoreBosonInv  = cms.bool(True),
                                    ignoreBSMInv    = cms.bool(True),
                                    producePU       = cms.bool(False),                                    
                                    src             = cms.InputTag("packedPFCandidates"),                                    
                                    genMotherSrc    = cms.InputTag('prunedGenParticles'),
                                    genSrc          = cms.InputTag('packedGenParticles'),                                    
                                    jetAlgorithm    = cms.string('AntiKt'),
                                    rParameter      = cms.double(.4),
                                    jetPtMin        = cms.double(3),
                                    minParticlePT   = cms.double          (0),
                                    maxParticleEta  = cms.double          (5.0),
                                    ghostArea       = cms.double          (.01)
)


redAK4Puppi        = cms.EDProducer('PFReJetProducer',
                                    isRealData = cms.int32(0),
                                    produceGen      = cms.bool(True),
                                    ignoreBosonInv  = cms.bool(True),
                                    ignoreBSMInv    = cms.bool(True),
                                    src             = cms.InputTag('puppi','Puppi'),                                
                                    genMotherSrc    = cms.InputTag('prunedGenParticles'),
                                    genSrc          = cms.InputTag('packedGenParticles'),                                    
                                    jetAlgorithm    = cms.string('AntiKt'),
                                    rParameter      = cms.double(.4),
                                    jetPtMin        = cms.double(3),
                                    minParticlePT   = cms.double          (0),
                                    maxParticleEta  = cms.double          (5.0),
                                    ghostArea       = cms.double          (.01)
)


redCA1 = redAK4.clone(
                      jetAlgorithm      = cms.string('CambridgeAachen'),
                      rParameter      = cms.double(1),
                      jetPtMin          = cms.double(20),
                      maxParticleEta    = cms.double          (3),
                      )

redCA1Puppi = redAK4Puppi.clone(
                      jetAlgorithm      = cms.string('CambridgeAachen'),
                      rParameter      = cms.double(1),
                      jetPtMin          = cms.double(20),
                      maxParticleEta    = cms.double          (3),
                      )