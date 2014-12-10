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
                                    minParticlePT   = cms.double(0),
                                    maxParticleEta  = cms.double(5.0),
                                    ghostArea       = cms.double(.01),
                                    useTrimming     = cms.bool(False),
                                    rFilt           = cms.double(.1),
                                    trimPtFracMin   = cms.double(.03),
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
                                    minParticlePT   = cms.double(0),
                                    maxParticleEta  = cms.double(5.0),
                                    ghostArea       = cms.double(.01),
                                    useTrimming     = cms.bool(False),
                                    rFilt           = cms.double(.1),
                                    trimPtFracMin   = cms.double(.03),
)

redAK8                 = redAK4.clone(rParameter = cms.double(0.8))
redAK8Trimmed          = redAK8.clone(useTrimming = cms.bool(True))

redAK8Puppi            = redAK4Puppi.clone(rParameter = cms.double(0.8))
redAK8TrimmedPuppi     = redAK8Puppi.clone(useTrimming = cms.bool(True))