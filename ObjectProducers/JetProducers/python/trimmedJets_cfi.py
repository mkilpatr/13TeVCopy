import FWCore.ParameterSet.Config as cms

from RecoJets.JetProducers.ak4PFJetsTrimmed_cfi import *

ak8PFJetsTrimmedr0p2ptf0p03 = ak4PFJetsTrimmed.clone(
    src           = cms.InputTag('packedPFCandidates'),
    doAreaFastjet = True,
    rParam        = 0.8,
    jetPtMin      = 15.0,
    rFilt         = 0.2,
    trimPtFracMin = 0.03
)

ak8PFJetsTrimmedr0p2ptf0p05 = ak8PFJetsTrimmedr0p2ptf0p03.clone(trimPtFracMin = 0.05)
ak8PFJetsTrimmedr0p1ptf0p03 = ak8PFJetsTrimmedr0p2ptf0p03.clone(rFilt         = 0.1)
ak8PFJetsTrimmedr0p3ptf0p03 = ak8PFJetsTrimmedr0p2ptf0p03.clone(rFilt         = 0.3)

pfCHS = cms.EDFilter('CandPtrSelector',
    src = cms.InputTag('packedPFCandidates'),
    cut = cms.string('fromPV')
)

ak8PFJetsCHSTrimmedr0p2ptf0p03 = ak8PFJetsTrimmedr0p2ptf0p03.clone(src = cms.InputTag('pfCHS'))
ak8PFJetsCHSTrimmedr0p2ptf0p05 = ak8PFJetsTrimmedr0p2ptf0p05.clone(src = cms.InputTag('pfCHS'))
ak8PFJetsCHSTrimmedr0p1ptf0p03 = ak8PFJetsTrimmedr0p1ptf0p03.clone(src = cms.InputTag('pfCHS'))
ak8PFJetsCHSTrimmedr0p3ptf0p03 = ak8PFJetsTrimmedr0p3ptf0p03.clone(src = cms.InputTag('pfCHS'))

from ObjectProducers.Puppi.Puppi_cff import *

ak8PFJetsPuppiTrimmedr0p2ptf0p03 = ak8PFJetsTrimmedr0p2ptf0p03.clone(src = cms.InputTag('puppi','Puppi'))
ak8PFJetsPuppiTrimmedr0p2ptf0p05 = ak8PFJetsTrimmedr0p2ptf0p05.clone(src = cms.InputTag('puppi','Puppi'))
ak8PFJetsPuppiTrimmedr0p1ptf0p03 = ak8PFJetsTrimmedr0p1ptf0p03.clone(src = cms.InputTag('puppi','Puppi'))
ak8PFJetsPuppiTrimmedr0p3ptf0p03 = ak8PFJetsTrimmedr0p3ptf0p03.clone(src = cms.InputTag('puppi','Puppi'))

trimmedJetsCHSr0p2ptf0p03 = cms.Sequence(pfCHS * ak8PFJetsCHSTrimmedr0p2ptf0p03)
trimmedJetsCHSr0p2ptf0p05 = cms.Sequence(pfCHS * ak8PFJetsCHSTrimmedr0p2ptf0p05)
trimmedJetsCHSr0p1ptf0p03 = cms.Sequence(pfCHS * ak8PFJetsCHSTrimmedr0p1ptf0p03)
trimmedJetsCHSr0p3ptf0p03 = cms.Sequence(pfCHS * ak8PFJetsCHSTrimmedr0p3ptf0p03)

trimmedJetsPuppir0p2ptf0p03 = cms.Sequence(puppi * ak8PFJetsPuppiTrimmedr0p2ptf0p03)
trimmedJetsPuppir0p2ptf0p05 = cms.Sequence(puppi * ak8PFJetsPuppiTrimmedr0p2ptf0p05)
trimmedJetsPuppir0p1ptf0p03 = cms.Sequence(puppi * ak8PFJetsPuppiTrimmedr0p1ptf0p03)
trimmedJetsPuppir0p3ptf0p03 = cms.Sequence(puppi * ak8PFJetsPuppiTrimmedr0p3ptf0p03)

# b-tagging: not yet the final version
from Configuration.StandardSequences.Geometry_cff import *
from Configuration.StandardSequences.MagneticField_38T_cff import *
from TrackingTools.TransientTrack.TransientTrackBuilder_cfi import *
from RecoBTag.Configuration.RecoBTag_cff import *

trimmedImpactParameterTagInfos = pfImpactParameterTagInfos.clone()
trimmedImpactParameterTagInfos.primaryVertex = cms.InputTag('offlineSlimmedPrimaryVertices')
trimmedImpactParameterTagInfos.jets = cms.InputTag('ak8PFJetsCHSTrimmedr0p1ptf0p03')
trimmedImpactParameterTagInfos.candidates = cms.InputTag('packedPFCandidates')
trimmedImpactParameterTagInfos.maxDeltaR = 0.1

trimmedInclusiveSecondaryVertexFinderTagInfos = pfSecondaryVertexTagInfos.clone()
trimmedInclusiveSecondaryVertexFinderTagInfos.extSVDeltaRToJet    = cms.double(0.3)
trimmedInclusiveSecondaryVertexFinderTagInfos.useExternalSV = cms.bool(True)
trimmedInclusiveSecondaryVertexFinderTagInfos.vertexCuts.fracPV = 0.79 ## 4 out of 5 is discarded
trimmedInclusiveSecondaryVertexFinderTagInfos.vertexCuts.distSig2dMin = 2.0
trimmedInclusiveSecondaryVertexFinderTagInfos.trackIPTagInfos = cms.InputTag('trimmedImpactParameterTagInfos')
trimmedInclusiveSecondaryVertexFinderTagInfos.extSVCollection = cms.InputTag('slimmedSecondaryVertices')

trimmedCSVIVF = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('candidateCombinedSecondaryVertex'), #V2 not yet available in global tag?
    tagInfos = cms.VInputTag(cms.InputTag('trimmedImpactParameterTagInfos'),
                             cms.InputTag('trimmedInclusiveSecondaryVertexFinderTagInfos'))
)

trimmedJetFullSequence = cms.Sequence(trimmedJetsCHSr0p1ptf0p03 * trimmedImpactParameterTagInfos * trimmedInclusiveSecondaryVertexFinderTagInfos * trimmedCSVIVF)
