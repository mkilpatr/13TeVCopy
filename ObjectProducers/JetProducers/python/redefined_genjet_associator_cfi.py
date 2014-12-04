import FWCore.ParameterSet.Config as cms

redGenAssoc          = cms.EDFilter('RedefinedGenJetAssociator'
                                    , recoJetsSrc      = cms.InputTag('slimmedJets')
                                    , redRecoJetsSrc   = cms.InputTag('redAK4','')
                                    , redGenJetsSrc    = cms.InputTag('redAK4','Gen')
                                    , redGenJetsPtrSrc = cms.InputTag('redAK4','GenPtr')
                                    , redGenJetTag     = cms.string('GenPtr')
)

redPuppiGenAssoc     = cms.EDProducer('RecoGenJetAssociator'
                                      , recoJetsSrc      = cms.InputTag('ak4PFJetsPuppi')
                                      , redRecoJetsSrc   = cms.InputTag('redAK4Puppi','')
                                      , redGenJetsSrc    = cms.InputTag('redAK4Puppi','Gen')
                                      , redGenJetsPtrSrc = cms.InputTag('redAK4Puppi','GenPtr')
                                      , redGenJetTag     = cms.string('GenPtr')
)

redAK8TrimmedGenAssoc = cms.EDProducer('RecoGenJetAssociator'
                                       , recoJetsSrc      = cms.InputTag('ak8PFJetsCHSTrimmedr0p1ptf0p03')
                                       , redRecoJetsSrc   = cms.InputTag('redAK8CHS','')
                                       , redGenJetsSrc    = cms.InputTag('redAK8CHS','Gen')
                                       , redGenJetsPtrSrc = cms.InputTag('redAK8CHS','GenPtr')
                                       , redGenJetTag     = cms.string('GenPtr')
)

