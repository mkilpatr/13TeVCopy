import FWCore.ParameterSet.Config as cms

from ObjectProducers.Puppi.Puppi_cff import *

from RecoJets.JetProducers.ak4PFJets_cfi import ak4PFJets

ak4PFJetsPuppi = ak4PFJets.clone(
    src      = cms.InputTag('puppi','Puppi'),
    jetPtMin = cms.double(20)
    )

from JetMETCorrections.Configuration.JetCorrectionServices_cff import *

# use AK4PFCHS JEC for now
ak4PFPuppiL1Fastjet = ak5PFCHSL1Fastjet.clone(algorithm='AK4PFchs')
ak4PFPuppiL2Relative = ak5PFCHSL2Relative.clone(algorithm='AK4PFchs')
ak4PFPuppiL3Absolute = ak5PFCHSL3Absolute.clone(algorithm='AK4PFchs')
ak4PFPuppiL1FastL2L3 = cms.ESProducer(
    'JetCorrectionESChain',
     correctors = cms.vstring('ak4PFPuppiL1Fastjet','ak4PFPuppiL2Relative','ak4PFPuppiL3Absolute')
     )

correctedAK4PFJetsPuppi = cms.EDProducer('PFJetCorrectionProducer',
    src = cms.InputTag("ak4PFJetsPuppi"),
    correctors = cms.vstring("ak4PFPuppiL1FastL2L3")
)

puppiJetSequence = cms.Sequence(puppi * ak4PFJetsPuppi)
puppiCorrJetSequence = cms.Sequence(puppiJetSequence * correctedAK4PFJetsPuppi)

# to be added later : btagging
