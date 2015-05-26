# Photon ID: egm_id_74X_v1

import FWCore.ParameterSet.Config as cms

egmPhotonIDs = cms.EDProducer(
    "VersionedPhotonIdProducer",
    physicsObjectSrc = cms.InputTag('slimmedPhotons'),
    physicsObjectIDs = cms.VPSet( )
)