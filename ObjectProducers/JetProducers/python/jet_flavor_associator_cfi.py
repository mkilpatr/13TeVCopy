import FWCore.ParameterSet.Config as cms

ak4FlvAssoc                        = cms.EDProducer('JetFlavorAssociator'
                                    , genJetsSrc       = cms.InputTag('ak4Jets','Gen')
                                    , genMotherSrc     = cms.InputTag('prunedGenParticles')
                                    , genSrc           = cms.InputTag('packedGenParticles')   
                                    , recoJetSrc       = cms.InputTag('ak4Jets')
                                    , partonJetSrc     = cms.InputTag('ak4Jets','parton')
                                    , recoToGenTag     = cms.InputTag('ak4Jets','GenPtr')
                                    , recoToPartonTag  = cms.InputTag('ak4Jets','partonPtr')
)