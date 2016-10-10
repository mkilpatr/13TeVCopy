import FWCore.ParameterSet.Config as cms
from RecoJets.JetProducers.PFJetParameters_cfi import *
from RecoJets.JetProducers.AnomalousCellParameters_cfi import *

#Name  
def HTTJets(process, seq, name, radius) :
    setattr( process, name, 
        cms.EDProducer(
                "FastjetJetProducer",
                PFJetParameters,
                AnomalousCellParameters,
                jetAlgorithm = cms.string("CambridgeAachen"),
                rParam       = cms.double(radius))
    )
    getattr(process, name).src = cms.InputTag("packedPFCandidates")
    getattr(process, name).jetPtMin = cms.double(100.)
    
    seq += getattr(process, name)
    
    softdrop_name          = name + "SoftdropZ2B1"
    
    setattr( process, softdrop_name,
            getattr(process, name).clone(
            useSoftDrop = cms.bool(True),
            zcut = cms.double(0.2),
            beta = cms.double(1.),
            R0 = cms.double(radius),
            useExplicitGhosts = cms.bool(True),
            writeCompound = cms.bool(False), # Also write subjets
#             jetCollInstanceName=cms.string("SubJets"),            
        )
    )
    seq += getattr(process, softdrop_name)
    
    softdrop_subjetti_name          = softdrop_name + "njettiness"
    
    setattr( process, softdrop_subjetti_name,  cms.EDProducer("NjettinessAdder",
                                                                         src=cms.InputTag(softdrop_name),
                                                                         Njets = cms.vuint32(1,2,3,4),
                                                                         # variables for measure definition : 
                                                                         measureDefinition = cms.uint32( 0 ), # CMS default is normalized measure
                                                                         beta = cms.double(1.0),              # CMS default is 1
                                                                         R0 = cms.double(radius),                # CMS default is jet cone size
                                                                         Rcutoff = cms.double( 999.0),       # not used by default
                                                                         # variables for axes definition :
                                                                         axesDefinition = cms.uint32( 6 ),    # CMS default is 1-pass KT axes
                                                                         nPass = cms.int32(999),             # not used by default
                                                                         akAxesR0 = cms.double(-999.0)        # not used by default
        )
    )
    seq += getattr(process, softdrop_subjetti_name)
    
    htt_name          = name + "HTT"
    
    setattr( process, htt_name,
             cms.EDProducer(
       "HTTTopJetProducer",
       PFJetParameters.clone(
           src               = cms.InputTag("packedPFCandidates"),
           doAreaFastjet     = cms.bool(True),
           doRhoFastjet      = cms.bool(False),
           jetPtMin          = cms.double(100.0)
           ),
       AnomalousCellParameters,
       useExplicitGhosts = cms.bool(True),
       algorithm           = cms.int32(1),
       jetAlgorithm        = cms.string("CambridgeAachen"),
       rParam              = cms.double(radius),
       optimalR            = cms.bool(True),
       qJets               = cms.bool(False),
       minFatjetPt         = cms.double(100.),
       minSubjetPt         = cms.double(0.),
       minCandPt           = cms.double(0.),
       maxFatjetAbsEta     = cms.double(99.),
       subjetMass          = cms.double(30.),
       muCut               = cms.double(0.8),
       filtR               = cms.double(0.3),
       filtN               = cms.int32(5),
       mode                = cms.int32(4),
       minCandMass         = cms.double(0.),
       maxCandMass         = cms.double(999999.),
       massRatioWidth      = cms.double(999999.),
       minM23Cut           = cms.double(0.),
       minM13Cut           = cms.double(0.),
       maxM13Cut           = cms.double(999999.),
       writeCompound       = cms.bool(True),
       jetCollInstanceName = cms.string("SubJets")
       )
    )
    seq += getattr(process, htt_name)
    
    #btagging
    jetAlgo = "CambridgeAachen"
    subjet_label = "SubJets"
    fatjet_label = ""
    
    # Define the module names
    impact_info_name          = htt_name + "ImpactParameterTagInfos"
    isv_info_name             = htt_name + "pfInclusiveSecondaryVertexFinderTagInfos"        
    csvv2_computer_name       = htt_name + "combinedSecondaryVertexV2Computer"
    csvv2ivf_name             = htt_name + "pfCombinedInclusiveSecondaryVertexV2BJetTags"        

    # Setup the modules
    # IMPACT PARAMETER
    setattr(process, 
            impact_info_name, 
            process.pfImpactParameterTagInfos.clone(
                primaryVertex = cms.InputTag("offlineSlimmedPrimaryVertices"),
                candidates = cms.InputTag("packedPFCandidates"),
                computeGhostTrack = cms.bool(True),
                computeProbabilities = cms.bool(True),
                maxDeltaR = cms.double(0.4),
                jets = cms.InputTag(htt_name, subjet_label),
            ))
    getattr(process, impact_info_name).explicitJTA = cms.bool(True)
    seq += getattr(process, htt_name)

    # ISV
    setattr(process,
            isv_info_name,                
            process.pfInclusiveSecondaryVertexFinderTagInfos.clone(
               extSVCollection               = cms.InputTag('slimmedSecondaryVertices'),
               trackIPTagInfos               = cms.InputTag(impact_info_name),                
            ))
    seq += getattr(process, isv_info_name)

    getattr(process, isv_info_name).useSVClustering = cms.bool(True)
    getattr(process, isv_info_name).rParam = cms.double(radius)
    getattr(process, isv_info_name).extSVDeltaRToJet = cms.double(0.3)
    getattr(process, isv_info_name).trackSelection.jetDeltaRMax = cms.double(0.3)
    getattr(process, isv_info_name).vertexCuts.maxDeltaRToJetAxis = cms.double(0.4)
    getattr(process, isv_info_name).jetAlgorithm = cms.string(jetAlgo)
    getattr(process, isv_info_name).fatJets  =  cms.InputTag(name)
    getattr(process, isv_info_name).groomedFatJets  =  cms.InputTag(htt_name, fatjet_label)

    # CSV V2 COMPUTER
    setattr(process,
            csvv2_computer_name,
            process.candidateCombinedSecondaryVertexV2Computer.clone())

    # CSV IVF V2
    setattr(process,
            csvv2ivf_name,
            process.pfCombinedInclusiveSecondaryVertexV2BJetTags.clone(
                tagInfos = cms.VInputTag(cms.InputTag(impact_info_name),
                                         cms.InputTag(isv_info_name)),
                jetTagComputer = cms.string(csvv2_computer_name,)
            ))
    seq += getattr(process,csvv2ivf_name)