import FWCore.ParameterSet.Config as cms
pickyjet_configuration = cms.PSet(
                                    pickyMVAFileName    = cms.string("$CMSSW_BASE/src/data/pickyJets/pickymva_nopuppi_reco.root")
                                  , pickyMVAName        = cms.string("picky_nopuppi_reco_0")                            
                                  , beta                = cms.double(1)                                                                                             
                                  , rCutoff             = cms.double(10000)                                                                                         
                                  , nSubAxes            = cms.string("1passKT")
                                  , pickyMaxSplits      = cms.int32(4)
                                  , doPickyJets         = cms.bool(False)
                                    )
trimmedjet_configuration = cms.PSet (
                                    useTrimming     = cms.bool(False),
                                    useTrimmedSubjets = cms.bool(False),
                                    rFilt           = cms.double(.1),
                                    trimPtFracMin   = cms.double(.03)     
                                     )
subjetcounting_configuration = cms.PSet (
                                        useSubjetCountingCA = cms.bool(False),
                                        ptCut   = cms.double(20.0),
                                        mCutoff = cms.double(20.0),
                                        yCut    = cms.double(0.05),
                                        rMin    = cms.double(0.10)
                                        )
jet_configuration      = cms.PSet(
                                    pickyjet_configuration,
                                    trimmedjet_configuration,
                                    subjetcounting_configuration,
                                    isRealData = cms.int32(0),
                                    produceGen      = cms.bool(True),
                                    producePartonJets      = cms.bool(True),
                                    ignoreBosonInv  = cms.bool(True),
                                    ignoreBSMInv    = cms.bool(True),
                                    src             = cms.InputTag("packedPFCandidates"), 
                                    genMotherSrc    = cms.InputTag('prunedGenParticles'),
                                    genSrc          = cms.InputTag('packedGenParticles'),                                    
                                    jetAlgorithm    = cms.string('AntiKt'),
                                    rParameter      = cms.double(.4),
                                    jetPtMin        = cms.double(3),
                                    minParticlePT   = cms.double(0),
                                    maxParticleEta  = cms.double(5.0),
                                    ghostArea       = cms.double(.01),
                                    outputSuperJets = cms.bool(False)
                                  ) 



ak4Jets                 = cms.EDProducer('PackedReJetProducer',
                                    jet_configuration, 
                                    producePU       = cms.bool(False),                                                                                                                                      
)


#ak4PuppiJets            = cms.EDProducer('PFReJetProducer',
#                                    jet_configuration,                                                                    
#)
#ak4PuppiJets.src = cms.InputTag('puppi')
