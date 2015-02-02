import FWCore.ParameterSet.Config as cms

from ObjectProducers.JetProducers.jet_clustering_cfi import *
from ObjectProducers.JetProducers.redefined_genjet_associator_cfi import *
from ObjectProducers.JetProducers.jet_flavor_associator_cfi import *
from ObjectProducers.JetProducers.jet_btagging_cfi import *


ak4PatAssocSeq = cms.Sequence(ak4Jets * redGenAssoc * ak4FlvAssoc)

redCA8 = ak4Jets.clone(jetAlgorithm = cms.string('CambridgeAachen'),rParameter      = cms.double(.8) )
ca8JetsSeq = cms.Sequence(redCA8)


ak4PuppiFlvAssoc        = ak4FlvAssoc.clone(genJetsSrc = cms.InputTag('ak4PuppiJets','Gen'))
ak4PuppiIPTagInfos      = IPTagInfos.clone(jets = cms.InputTag("ak4PuppiJets"))
ak4PuppiISVTagInfos     = ISVTagInfos.clone(trackIPTagInfos = cms.InputTag("ak4PuppiIPTagInfos"))
ak4PuppiCSVIV2          = CSVIV2.clone(tagInfos = cms.VInputTag(cms.InputTag("ak4PuppiIPTagInfos"), cms.InputTag("ak4PuppiISVTagInfos")))
ak4PuppiJetSeq = cms.Sequence(ak4PuppiJets * ak4PuppiFlvAssoc * ak4PuppiIPTagInfos *ak4PuppiISVTagInfos*ak4PuppiCSVIV2 )

pickyJets            = ak4Jets.clone(rParameter = cms.double(1), jetAlgorithm    = cms.string('CambridgeAachen'),doPickyJets = cms.bool(True))
pickyFlvAssoc        = noRadFlvAssoc.clone(genJetsSrc = cms.InputTag('pickyJets','Gen'))
pickyIPTagInfos      = subjetIPTagInfos.clone(jets = cms.InputTag("pickyJets"))
pickyISVTagInfos     = ISVTagInfos.clone(trackIPTagInfos = cms.InputTag("pickyIPTagInfos")) #still need to fix this so that picky jets output the fat hets
pickyCSVIV2          = CSVIV2.clone(tagInfos = cms.VInputTag(cms.InputTag("pickyIPTagInfos"), cms.InputTag("pickyISVTagInfos")))
pickyJetSeq          = cms.Sequence(pickyJets * pickyFlvAssoc * pickyIPTagInfos *pickyISVTagInfos*pickyCSVIV2 )





# trimmedJetSeq = cms.Sequence(trimmedJets*trimmedJetFlvAssoc*trimmedJetIPTagInfos*trimmedJetISVFinderTagInfos*trimmedJetCSVIVF)

# ak4PuppiJetSeq = cms.Sequence(redAK4Puppi * redAK4PuppiFlvAssoc  )

# from JetMETCorrections.Configuration.JetCorrectionServices_cff import *
# ak4PFPuppiL2L3Corrector = cms.ESProducer(
#     'JetCorrectionESChain',
#     correctors = cms.vstring("ak4PFL2L3")
# )
# redAK4PuppiL2L3 = cms.EDProducer('PFJetCorrectionProducer',
#     src = cms.InputTag("redAK4Puppi",""),
#     correctors = cms.vstring("ak4PFPuppiL2L3Corrector")
# )
# ak4PuppiL2L3IPTagInfos = customIPTagInfos.clone(jets = cms.InputTag('redAK4PuppiL2L3',''))
# ak4PuppiL2L3ISVFinderTagInfos = customISVFinderTagInfos.clone(trackIPTagInfos = cms.InputTag('ak4PuppiL2L3IPTagInfos'))
# ak4PuppiL2L3CSVIVF = customCSVIVF.clone(tagInfos = cms.VInputTag(
#                                     cms.InputTag('ak4PuppiL2L3IPTagInfos'),
#                                     cms.InputTag('ak4PuppiL2L3ISVFinderTagInfos'))
#                                    )
# ak4PuppiL2L3JetSeq = cms.Sequence(redAK4Puppi * redAK4PuppiFlvAssoc  *redAK4PuppiL2L3 * ak4PuppiL2L3IPTagInfos *ak4PuppiL2L3ISVFinderTagInfos *ak4PuppiL2L3CSVIVF )