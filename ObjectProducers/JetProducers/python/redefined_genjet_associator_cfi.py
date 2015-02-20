import FWCore.ParameterSet.Config as cms

redGenAssoc          = cms.EDFilter('RedefinedGenJetAssociator'
                                    , recoJetsSrc      = cms.InputTag('slimmedJets')
                                    , redRecoJetsSrc   = cms.InputTag('ak4Jets','')
                                    , redGenJetsSrc    = cms.InputTag('ak4Jets','Gen')
                                    , redGenJetsPtrSrc = cms.InputTag('ak4Jets','GenPtr')
                                    , redGenJetTag     = cms.string('GenPtr')
)