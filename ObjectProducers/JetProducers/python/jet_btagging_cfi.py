import FWCore.ParameterSet.Config as cms

# b-tagging: not yet the final version
#from Configuration.StandardSequences.Geometry_cff import *
#from Configuration.StandardSequences.MagneticField_38T_cff import *
from TrackingTools.TransientTrack.TransientTrackBuilder_cfi import *
from RecoBTag.Configuration.RecoBTag_cff import *

IPTagInfos =  pfImpactParameterTagInfos.clone();
IPTagInfos.primaryVertex = cms.InputTag('offlineSlimmedPrimaryVertices')
IPTagInfos.candidates = cms.InputTag('packedPFCandidates')
IPTagInfos.explicitJTA = cms.bool(False)
IPTagInfos.maxDeltaR = cms.double(0.4)
IPTagInfos.jets = cms.InputTag("ak4Jets")

subjetIPTagInfos = IPTagInfos.clone(explicitJTA = cms.bool(True))


ISVTagInfos = pfSecondaryVertexTagInfos.clone()
ISVTagInfos.useExternalSV = cms.bool(True)
ISVTagInfos.vertexCuts.fracPV = 0.79 ## 4 out of 5 is discarded
ISVTagInfos.vertexCuts.distSig2dMin = 2.0
ISVTagInfos.extSVCollection = cms.InputTag('slimmedSecondaryVertices')
ISVTagInfos.trackIPTagInfos = cms.InputTag('IPTagInfos')

subjetISVTagInfos = ISVTagInfos.clone()
subjetISVTagInfos.trackIPTagInfos = cms.InputTag('subjetIPTagInfos')
subjetISVTagInfos.useSVClustering = cms.bool(True)
subjetISVTagInfos.jetAlgorithm = cms.string('CambridgeAachen')
subjetISVTagInfos.rParam = cms.double(1)
subjetISVTagInfos.fatJets = cms.InputTag("pickyJets","")
subjetISVTagInfos.groomedFatJets = cms.InputTag("pickyJets","Super")



CSVIV2 = cms.EDProducer("JetTagProducer",
    jetTagComputer = cms.string('candidateCombinedSecondaryVertexV2Computer'),
    tagInfos = cms.VInputTag(cms.InputTag("IPTagInfos"), cms.InputTag("ISVTagInfos"))
)
