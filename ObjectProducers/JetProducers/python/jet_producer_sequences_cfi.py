import FWCore.ParameterSet.Config as cms

from ObjectProducers.JetProducers.jet_clustering_cfi import *
from ObjectProducers.JetProducers.redefined_genjet_associator_cfi import *
from ObjectProducers.JetProducers.jet_flavor_associator_cfi import *
from ObjectProducers.JetProducers.jet_btagging_cfi import *


ak4PatAssocSeq = cms.Sequence(ak4Jets * redGenAssoc * ak4FlvAssoc)

redCA8 = ak4Jets.clone(jetAlgorithm = cms.string('CambridgeAachen'),rParameter      = cms.double(.8) )
ca8JetsSeq = cms.Sequence(redCA8)


#ak4PuppiFlvAssoc        = ak4FlvAssoc.clone(genJetsSrc = cms.InputTag('ak4PuppiJets','Gen'),
#                                            recoJetSrc = cms.InputTag('ak4PuppiJets'),partonJetSrc = cms.InputTag('ak4PuppiJets','parton'),
#                                            recoToGenTag = cms.InputTag('ak4PuppiJets','GenPtr'),recoToPartonTag = cms.InputTag('ak4PuppiJets','partonPtr'))
#ak4PuppiIPTagInfos      = IPTagInfos.clone(jets = cms.InputTag("ak4PuppiJets"))
#ak4PuppiISVTagInfos     = ISVTagInfos.clone(trackIPTagInfos = cms.InputTag("ak4PuppiIPTagInfos"))
#ak4PuppiCSVIV2          = CSVIV2.clone(tagInfos = cms.VInputTag(cms.InputTag("ak4PuppiIPTagInfos"), cms.InputTag("ak4PuppiISVTagInfos")))
#ak4PuppiJetSeq = cms.Sequence(ak4PuppiJets * ak4PuppiFlvAssoc * ak4PuppiIPTagInfos *ak4PuppiISVTagInfos*ak4PuppiCSVIV2 )
#
#pickyJets            = ak4Jets.clone(rParameter = cms.double(1), jetAlgorithm    = cms.string('CambridgeAachen'),doPickyJets = cms.bool(True),outputSuperJets = cms.bool(True))
#pickyFlvAssoc        = ak4FlvAssoc.clone(genJetsSrc = cms.InputTag('pickyJets','Gen'),
#                                            recoJetSrc = cms.InputTag('pickyJets'),partonJetSrc = cms.InputTag('pickyJets','parton'),
#                                            recoToGenTag = cms.InputTag('pickyJets','GenPtr'),recoToPartonTag = cms.InputTag('pickyJets','partonPtr'))
#pickyIPTagInfos      = subjetIPTagInfos.clone(jets = cms.InputTag("pickyJets"))
#pickyISVTagInfos     = ISVTagInfos.clone(trackIPTagInfos = cms.InputTag("pickyIPTagInfos"))
#pickyCSVIV2          = CSVIV2.clone(tagInfos = cms.VInputTag(cms.InputTag("pickyIPTagInfos"), cms.InputTag("pickyISVTagInfos")))
#pickyJetSeq          = cms.Sequence(pickyJets * pickyFlvAssoc * pickyIPTagInfos *pickyISVTagInfos*pickyCSVIV2 )
#
#subjetsca                = ak4Jets.clone(rParameter = cms.double(1.0), useSubjetCountingCA = cms.bool(True), outputSuperJets = cms.bool(True))
#subjetscaFlvAssoc        = ak4FlvAssoc.clone(genJetsSrc = cms.InputTag('subjetsca','Gen'),
#                                             recoJetSrc = cms.InputTag('subjetsca'),partonJetSrc = cms.InputTag('subjetsca','parton'),
#                                             recoToGenTag = cms.InputTag('subjetsca','GenPtr'),recoToPartonTag = cms.InputTag('subjetsca','partonPtr'))
#subjetscaIPTagInfos      = subjetIPTagInfos.clone(jets = cms.InputTag("subjetsca"))
#subjetscaISVTagInfos     = ISVTagInfos.clone(trackIPTagInfos = cms.InputTag("subjetscaIPTagInfos"))
#subjetscaCSVIV2          = CSVIV2.clone(tagInfos = cms.VInputTag(cms.InputTag("subjetscaIPTagInfos"), cms.InputTag("subjetscaISVTagInfos")))
#subjetscaJetSeq          = cms.Sequence(subjetsca * subjetscaFlvAssoc * subjetscaIPTagInfos *subjetscaISVTagInfos*subjetscaCSVIV2 )

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
