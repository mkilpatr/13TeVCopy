# Photon ID: egm_id_74X_v1

import FWCore.ParameterSet.Config as cms

from PhysicsTools.SelectorUtils.tools.vid_id_tools import *
from PhysicsTools.SelectorUtils.centralIDRegistry import central_id_registry

# taken from: 
# https://github.com/ikrav/cmssw/blob/egm_id_74X_v1/PhysicsTools/SelectorUtils/python/tools/vid_id_tools.py

def setupVIDPhotonSelection(process,cutflow,patProducer=None):
    if not hasattr(process,'egmPhotonIDs'):
        raise Exception('VIDProducerNotAvailable','egmPhotonIDs producer not available in process!')
    setupVIDSelection(process.egmPhotonIDs,cutflow)
    #add to PAT photon producer if available or specified
    if hasattr(process,'patPhotons') or patProducer is not None:
        if patProducer is None:
            patProducer = process.patPhotons
        idName = cutflow.idName.value()
        addVIDSelectionToPATProducer(patProducer,'egmPhotonIDs',idName)
        
