import FWCore.ParameterSet.Config as cms

from ObjectProducers.JetProducers.redefined_jet_producers_cfi import *
from ObjectProducers.JetProducers.redefined_genjet_associator_cfi import *
from ObjectProducers.JetProducers.jet_flavor_associator_cfi import *
from ObjectProducers.JetProducers.jet_btagging_cfi import *

ak4PatAssocSeq = cms.Sequence(redAK4 * redGenAssoc * redAK4FlvAssoc)
ca8AssocSeq = cms.Sequence(redCA8)
trimmedJetSeq = cms.Sequence(redAK8Trimmed*redAK8TrimmedFlvAssoc*trimmedIPTagInfos*trimmedISVFinderTagInfos*trimmedCSVIVF)
trimmedPuppiJetSeq = cms.Sequence(redAK8TrimmedPuppi*redAK8TrimmedPuppiFlvAssoc*trimmedPuppiIPTagInfos*trimmedPuppiISVFinderTagInfos*trimmedPuppiCSVIVF)
ak4PuppiJetSeq = cms.Sequence(redAK4Puppi * redAK4PuppiFlvAssoc * ak4PuppiIPTagInfos *ak4PuppiISVFinderTagInfos*ak4PuppiCSVIVF )

from JetMETCorrections.Configuration.JetCorrectionServices_cff import *
ak4PFPuppiL2L3Corrector = cms.ESProducer(
    'JetCorrectionESChain',
    correctors = cms.vstring("ak4PFL2L3")
)
redAK4PuppiL2L3 = cms.EDProducer('PFJetCorrectionProducer',
    src = cms.InputTag("redAK4Puppi",""),
    correctors = cms.vstring("ak4PFPuppiL2L3Corrector")
)
ak4PuppiL2L3IPTagInfos = customIPTagInfos.clone(jets = cms.InputTag('redAK4PuppiL2L3',''))
ak4PuppiL2L3ISVFinderTagInfos = customISVFinderTagInfos.clone(trackIPTagInfos = cms.InputTag('ak4PuppiL2L3IPTagInfos'))
ak4PuppiL2L3CSVIVF = customCSVIVF.clone(tagInfos = cms.VInputTag(
                                    cms.InputTag('ak4PuppiL2L3IPTagInfos'),
                                    cms.InputTag('ak4PuppiL2L3ISVFinderTagInfos'))
                                   )
ak4PuppiL2L3JetSeq = cms.Sequence(redAK4Puppi * redAK4PuppiFlvAssoc  *redAK4PuppiL2L3 * ak4PuppiL2L3IPTagInfos *ak4PuppiL2L3ISVFinderTagInfos *ak4PuppiL2L3CSVIVF * ca8AssocSeq)
