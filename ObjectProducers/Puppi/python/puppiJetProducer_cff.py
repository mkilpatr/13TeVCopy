import FWCore.ParameterSet.Config as cms

from ObjectProducers.Puppi.Puppi_cff import *

from RecoJets.JetProducers.ak4PFJets_cfi import ak4PFJets

ak4PFJetsPuppi = ak4PFJets.clone(
    src      = cms.InputTag('puppi','Puppi'),
    doAreaFastjet = True,
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

# b-tagging: not yet the final version
from Configuration.StandardSequences.Geometry_cff import *
from Configuration.StandardSequences.MagneticField_38T_cff import *
from TrackingTools.TransientTrack.TransientTrackBuilder_cfi import *
from RecoBTag.Configuration.RecoBTag_cff import *
#from RecoJets.Configuration.RecoJetAssociations_cff import *
#from PhysicsTools.PatAlgos.slimming.unpackedTracksAndVertices_cfi import *

# doesn't work because of missing track links for puppi PF candidates
#ak4PuppiJetTracksAssociatorExplicit = ak4JetTracksAssociatorExplicit.clone()
#ak4PuppiJetTracksAssociatorExplicit.jets = cms.InputTag('ak4PFJetsPuppi')
#ak4PuppiJetTracksAssociatorExplicit.tracks = cms.InputTag('unpackedTracksAndVertices')
#
#ak4PuppiImpactParameterTagInfos = impactParameterTagInfos.clone()
#ak4PuppiImpactParameterTagInfos.primaryVertex = cms.InputTag('unpackedTracksAndVertices','')
#ak4PuppiImpactParameterTagInfos.jetTracks = cms.InputTag('ak4PuppiJetTracksAssociatorExplicit')
#
#ak4PuppiInclusiveSecondaryVertexFinderTagInfos = inclusiveSecondaryVertexFinderTagInfos.clone()
#ak4PuppiInclusiveSecondaryVertexFinderTagInfos.trackIPTagInfos = cms.InputTag('ak4PuppiImpactParameterTagInfos')
#ak4PuppiInclusiveSecondaryVertexFinderTagInfos.extSVCollection = cms.InputTag('unpackedTracksAndVertices','secondary')
#
#ak4PuppiCSVIVFV2 = combinedInclusiveSecondaryVertexV2BJetTags.clone()
#ak4PuppiCSVIVFV2.tagInfos = cms.VInputTag(cms.InputTag('ak4PuppiImpactParameterTagInfos'),
#                                          cms.InputTag('ak4PuppiInclusiveSecondaryVertexFinderTagInfos'))


ak4PuppiImpactParameterTagInfos = pfImpactParameterTagInfos.clone()
ak4PuppiImpactParameterTagInfos.primaryVertex = cms.InputTag('offlineSlimmedPrimaryVertices')
ak4PuppiImpactParameterTagInfos.jets = cms.InputTag('ak4PFJetsPuppi')
ak4PuppiImpactParameterTagInfos.candidates = cms.InputTag('packedPFCandidates')

ak4PuppiInclusiveSecondaryVertexFinderTagInfos = pfSecondaryVertexTagInfos.clone()
ak4PuppiInclusiveSecondaryVertexFinderTagInfos.extSVDeltaRToJet    = cms.double(0.3)
ak4PuppiInclusiveSecondaryVertexFinderTagInfos.useExternalSV = cms.bool(True)
ak4PuppiInclusiveSecondaryVertexFinderTagInfos.vertexCuts.fracPV = 0.79 ## 4 out of 5 is discarded
ak4PuppiInclusiveSecondaryVertexFinderTagInfos.vertexCuts.distSig2dMin = 2.0
ak4PuppiInclusiveSecondaryVertexFinderTagInfos.trackIPTagInfos = cms.InputTag('ak4PuppiImpactParameterTagInfos')
ak4PuppiInclusiveSecondaryVertexFinderTagInfos.extSVCollection = cms.InputTag('slimmedSecondaryVertices')

ak4PuppiCSVIVF = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('candidateCombinedSecondaryVertex'), #V2 not yet available in global tag?
    tagInfos = cms.VInputTag(cms.InputTag('ak4PuppiImpactParameterTagInfos'),
                             cms.InputTag('ak4PuppiInclusiveSecondaryVertexFinderTagInfos'))
)

puppiJetFullSequence = cms.Sequence(puppiJetSequence * ak4PuppiImpactParameterTagInfos * ak4PuppiInclusiveSecondaryVertexFinderTagInfos * ak4PuppiCSVIVF)
puppiCorrJetFullSequence = cms.Sequence(puppiCorrJetSequence * ak4PuppiImpactParameterTagInfos * ak4PuppiInclusiveSecondaryVertexFinderTagInfos * ak4PuppiCSVIVF)
