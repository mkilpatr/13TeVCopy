import FWCore.ParameterSet.Config as cms

redAK4FlvAssoc          = cms.EDProducer('JetFlavorAssociator'
                                    , genJetsSrc       = cms.InputTag('redAK4','Gen')
                                    , genMotherSrc     = cms.InputTag('prunedGenParticles')
                                    , genSrc           = cms.InputTag('packedGenParticles')   
                                    , coneSize         = cms.double(.4)
)

redAK4PuppiFlvAssoc        = redAK4FlvAssoc.clone(genJetsSrc = cms.InputTag('redAK4Puppi','Gen'))
trimmedJetFlvAssoc         = redAK4FlvAssoc.clone(genJetsSrc = cms.InputTag('trimmedJets','Gen'), coneSize = cms.double(.2))
pickyJetFlvAssoc           = redAK4FlvAssoc.clone(genJetsSrc       = cms.InputTag('pickyJets','Gen') , coneSize  = cms.double(-1) )