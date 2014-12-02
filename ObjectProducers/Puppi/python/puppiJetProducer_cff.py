import FWCore.ParameterSet.Config as cms

from ObjectProducers.Puppi.Puppi_cff import *

from RecoJets.JetProducers.ak4PFJets_cfi import ak4PFJets

ak4PFJetsPuppi = ak4PFJets.clone(
    src      = cms.InputTag('puppi','Puppi'),
    jetPtMin = cms.double(20)
    )

from JetMETCorrections.Configuration.JetCorrectionServices_cff import *

# use AK4PFCHS JEC for now
ak4PFPuppiL1FastL2L3 = cms.ESProducer(
    'JetCorrectionESChain',
    correctors = cms.vstring("ak4PFCHSL1FastL2L3")
)

correctedAK4PFJetsPuppi = cms.EDProducer('PFJetCorrectionProducer',
    src = cms.InputTag("ak4PFJetsPuppi"),
    correctors = cms.vstring("ak4PFPuppiL1FastL2L3")
)

puppiJetSequence = cms.Sequence(puppi * ak4PFJetsPuppi)
puppiCorrJetSequence = cms.Sequence(puppiJetSequence * correctedAK4PFJetsPuppi)

# to be added later : btagging
