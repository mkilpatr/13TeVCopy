import FWCore.ParameterSet.Config as cms

redAK4FlvAssoc          = cms.EDProducer('JetFlavorAssociator'
                                    , genJetsSrc       = cms.InputTag('redAK4','Gen')
                                    , genMotherSrc     = cms.InputTag('prunedGenParticles')
                                    , genSrc           = cms.InputTag('packedGenParticles')   
                                    , coneSize         = cms.double(.4)
)

redPuppiAssoc           = redAK4FlvAssoc.clone(genJetsSrc = cms.InputTag('redAK4Puppi','Gen'))
redAK8TrimmedAssoc      = redAK4FlvAssoc.clone(genJetsSrc = cms.InputTag('redAK8CHS','Gen'))
