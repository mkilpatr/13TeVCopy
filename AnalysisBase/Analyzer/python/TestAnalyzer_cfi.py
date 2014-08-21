import FWCore.ParameterSet.Config as cms

TestAnalyzer = cms.EDAnalyzer('TestAnalyzer',
  filename   = cms.untracked.string ('test.root'),
  treename   = cms.untracked.string ('Events'),
  vertices = cms.InputTag('offlineSlimmedPrimaryVertices'),
  mets = cms.InputTag('slimmedMETs'),
  jets = cms.InputTag('slimmedJets'),
  minjetpt = cms.untracked.double(15.0),
  maxnjets = cms.untracked.uint32(20)
)

