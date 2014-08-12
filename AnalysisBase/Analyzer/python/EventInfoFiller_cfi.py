import FWCore.ParameterSet.Config as cms

EventInfoFiller = cms.EDAnalyzer("EventInfoFiller",
    vertices = cms.InputTag("offlineSlimmedPrimaryVertices"),
    mets = cms.InputTag("slimmedMETs"),
    filename = cms.untracked.string("test.root")
)

