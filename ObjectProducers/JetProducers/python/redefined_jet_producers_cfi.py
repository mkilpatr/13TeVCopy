import FWCore.ParameterSet.Config as cms

pickyjet_configuration = cms.PSet(
                                    pickyMVAFileName    = cms.string("support/pickymva_nopuppi_reco.root")
                                  , pickyMVAName        = cms.string("picky_nopuppi_reco_0")                            
                                  , beta                = cms.double(1)                                                                                             
                                  , rCutoff             = cms.double(10000)                                                                                         
                                  , nSubAxes            = cms.string("1passKT")
                                  , pickyMaxSplits      = cms.int32(4)
                                  , doPickyJets         = cms.bool(False)
                                    )

redAK4             = cms.EDProducer('PackedReJetProducer',
                                    pickyjet_configuration,
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
                                    useTrimmedSubjets = cms.bool(False),
                                    rFilt           = cms.double(.1),
                                    trimPtFracMin   = cms.double(.03),                                    
)


redAK4Puppi        = cms.EDProducer('PFReJetProducer',
                                    pickyjet_configuration,
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
                                    useTrimmedSubjets = cms.bool(False),
                                    rFilt           = cms.double(.1),
                                    trimPtFracMin   = cms.double(.03),
)
redCA8                 = redAK4.clone(jetAlgorithm = cms.string('CambridgeAachen'),rParameter      = cms.double(.8) )

trimmedJets            = redAK4.clone(rParameter      = cms.double(1), useTrimming = cms.bool(True),useTrimmedSubjets = cms.bool(True),rFilt=cms.double(.2))
pickyJets              = redAK4.clone(rParameter = cms.double(1), jetAlgorithm    = cms.string('CambridgeAachen'),doPickyJets = cms.bool(True))