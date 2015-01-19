import FWCore.ParameterSet.Config as cms

# b-tagging: not yet the final version
from Configuration.StandardSequences.Geometry_cff import *
from Configuration.StandardSequences.MagneticField_38T_cff import *
from TrackingTools.TransientTrack.TransientTrackBuilder_cfi import *
from RecoBTag.Configuration.RecoBTag_cff import *

customIPTagInfos =  pfImpactParameterTagInfos.clone();
customIPTagInfos.primaryVertex = cms.InputTag('offlineSlimmedPrimaryVertices')
customIPTagInfos.candidates = cms.InputTag('packedPFCandidates')
customIPTagInfos.maxDeltaR = 0.1


customISVFinderTagInfos = pfSecondaryVertexTagInfos.clone()
customISVFinderTagInfos.extSVDeltaRToJet    = cms.double(0.3)
customISVFinderTagInfos.useExternalSV = cms.bool(True)
customISVFinderTagInfos.vertexCuts.fracPV = 0.79 ## 4 out of 5 is discarded
customISVFinderTagInfos.vertexCuts.distSig2dMin = 2.0
customISVFinderTagInfos.extSVCollection = cms.InputTag('slimmedSecondaryVertices')

customCSVIVF = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('candidateCombinedSecondaryVertex'), #V2 not yet available in global tag?
    tagInfos = cms.VInputTag(cms.InputTag('customIPTagInfos'),
                             cms.InputTag('customISVFinderTagInfos'))
)


trimmedJetIPTagInfos = customIPTagInfos.clone(jets = cms.InputTag('trimmedJets',''))
trimmedJetISVFinderTagInfos = customISVFinderTagInfos.clone(trackIPTagInfos = cms.InputTag('trimmedJetIPTagInfos'))
trimmedJetCSVIVF = customCSVIVF.clone(tagInfos = cms.VInputTag(
                                    cms.InputTag('trimmedJetIPTagInfos'),
                                    cms.InputTag('trimmedJetISVFinderTagInfos'))
                                   )


ak4PuppiIPTagInfos = customIPTagInfos.clone(jets = cms.InputTag('redAK4Puppi',''))
ak4PuppiISVFinderTagInfos = customISVFinderTagInfos.clone(trackIPTagInfos = cms.InputTag('ak4PuppiIPTagInfos'))
ak4PuppiCSVIVF = customCSVIVF.clone(tagInfos = cms.VInputTag(
                                    cms.InputTag('ak4PuppiIPTagInfos'),
                                    cms.InputTag('ak4PuppiISVFinderTagInfos'))
                                   )

pickyJetIPTagInfos = customIPTagInfos.clone(jets = cms.InputTag('pickyJets',''))
pickyJetISVFinderTagInfos = customISVFinderTagInfos.clone(trackIPTagInfos = cms.InputTag('pickyJetIPTagInfos'))
pickyJetCSVIVF = customCSVIVF.clone(tagInfos = cms.VInputTag(
                                    cms.InputTag('pickyJetIPTagInfos'),
                                    cms.InputTag('pickyJetISVFinderTagInfos'))
                                   )