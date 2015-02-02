import FWCore.ParameterSet.Config as cms

ak4FlvAssoc                        = cms.EDProducer('JetFlavorAssociator'
                                    , genJetsSrc       = cms.InputTag('ak4Jets','Gen')
                                    , genMotherSrc     = cms.InputTag('prunedGenParticles')
                                    , genSrc           = cms.InputTag('packedGenParticles')   
                                    , coneSize         = cms.double(.4)
)


noRadFlvAssoc = ak4FlvAssoc.clone(coneSize         = cms.double(-1))