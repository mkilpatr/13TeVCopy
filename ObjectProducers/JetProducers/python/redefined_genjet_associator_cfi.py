import FWCore.ParameterSet.Config as cms

redGenAssoc        = cms.EDFilter('RedefinedGenJetAssociator'
                                    , recoJetsSrc      = cms.InputTag('slimmedJets')
                                    , redRecoJetsSrc   = cms.InputTag('redAK4','')
                                    , redGenJetsSrc    = cms.InputTag('redAK4','Gen')
                                    , redGenJetsPtrSrc = cms.InputTag('redAK4','GenPtr')
                                    , redGenJetTag     = cms.string('GenPtr')
)
