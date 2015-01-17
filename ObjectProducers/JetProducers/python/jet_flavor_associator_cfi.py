import FWCore.ParameterSet.Config as cms

redAK4FlvAssoc          = cms.EDProducer('JetFlavorAssociator'
                                    , genJetsSrc       = cms.InputTag('redAK4','Gen')
                                    , genMotherSrc     = cms.InputTag('prunedGenParticles')
                                    , genSrc           = cms.InputTag('packedGenParticles')   
                                    , coneSize         = cms.double(.4)
)

redAK4PuppiFlvAssoc        = redAK4FlvAssoc.clone(genJetsSrc = cms.InputTag('redAK4Puppi','Gen'))
redAK8TrimmedFlvAssoc      = redAK4FlvAssoc.clone(genJetsSrc = cms.InputTag('redAK8Trimmed','Gen'), coneSize = cms.double(1))
redAK8TrimmedPuppiFlvAssoc = redAK4FlvAssoc.clone(genJetsSrc = cms.InputTag('redAK8TrimmedPuppi','Gen'), coneSize = cms.double(1))
pickyJetFlvAssoc           = redAK4FlvAssoc.clone(genJetsSrc       = cms.InputTag('pickyJets','Gen') , coneSize  = cms.double(-1) )