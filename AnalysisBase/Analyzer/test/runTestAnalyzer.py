import FWCore.ParameterSet.Config as cms
import re

process = cms.Process('run')

process.options = cms.untracked.PSet(
    allowUnscheduled=cms.untracked.bool(True),
    wantSummary=cms.untracked.bool(False)
)

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger.suppressWarning = cms.untracked.vstring('ak8PFJetsPuppiSoftDrop', 'ak8PFJetsPuppiPruned')

# Valgrind
# process.ProfilerService = cms.Service (
#       "ProfilerService",
#       firstEvent = cms.untracked.int32(3),
#       lastEvent = cms.untracked.int32(12),
#       paths = cms.untracked.vstring('p')
# )

from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing('analysis')

options.outputFile = 'evttree.root'
#options.inputFiles = '/store/mc/RunIISummer16MiniAODv2/TTToSemilepton_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/120000/0023AF2C-D7CD-E611-9247-002590E7D7CE.root'
#options.inputFiles = '/store/mc/RunIISummer16MiniAODv2/GJets_DR-0p4_HT-40To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUMoriond17_qcut19_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/120000/047C04F9-11BF-E611-B984-E41D2D08DDC0.root'
#options.inputFiles = '/store/mc/RunIISpring16MiniAODv2/SMS-T2tt_mStop-400to1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUSpring16Fast_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/10000/00212097-BA34-E611-A687-003048F35112.root'
options.inputFiles = '/store/data/Run2016G/SinglePhoton/MINIAOD/23Sep2016-v1/50000/005B4B20-4787-E611-989B-008CFAF75356.root'
#options.inputFiles = '/store/data/Run2016H/SingleMuon/MINIAOD/PromptReco-v3/000/284/036/00000/0E02D50E-989F-E611-A962-FA163EE15C80.root'

options.maxEvents = -1

options.register('skipEvents',
                 0,
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.int,
                 "Number of events to skip in processing")

options.register('inputDataset',
                 None,
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.string,
                 "Input dataset")

options.register('eventsToProcess',
                  '',
                  VarParsing.multiplicity.list,
                  VarParsing.varType.string,
                  "Events to process")

options.parseArguments()

process.TFileService = cms.Service('TFileService',
    fileName=cms.string(options.outputFile)
)

process.maxEvents = cms.untracked.PSet(input=cms.untracked.int32(options.maxEvents))

process.source = cms.Source('PoolSource',
    fileNames=cms.untracked.vstring (options.inputFiles),
    skipEvents=cms.untracked.uint32(options.skipEvents)
)
if options.eventsToProcess:
    process.source.eventsToProcess = cms.untracked.VEventRange(options.eventsToProcess)

#==============================================================================================================================#
from AnalysisBase.Analyzer.analyzer_configuration_cfi import nominal_configuration

process.TestAnalyzer = cms.EDFilter('TestAnalyzer',
  nominal_configuration
)

# dataset name
runCRAB = True if options.inputDataset else False
DatasetName = options.inputDataset if options.inputDataset else options.inputFiles[0]
print DatasetName

# MC theory systematic weights only saved for specific samples
if 'TTZ' in DatasetName or 'TTW' in DatasetName or 'ST_' in DatasetName or 'tZq' in DatasetName or 'SMS' in DatasetName or 'T2bW' in DatasetName or 'TTJets' in DatasetName or 'WJets' in DatasetName or 'WW' in DatasetName or 'WZ' in DatasetName or 'ZZ' in DatasetName:
    print 'Adding theory weights'
    process.TestAnalyzer.EventInfo.saveSystematicWeights = cms.untracked.bool(True)
    if 'T2tt_mStop-500_mLSP-325' in DatasetName or 'T2tt_mStop-425_mLSP-325' in DatasetName or 'T2tt_mStop-850_mLSP-100' in DatasetName:
        process.TestAnalyzer.EventInfo.lheEvtInfo = cms.InputTag('source')
else :
    process.TestAnalyzer.EventInfo.saveSystematicWeights = cms.untracked.bool(False)

# Turn on saving number of genjets for QCD patching
if 'QCD' in DatasetName:
    process.TestAnalyzer.EventInfo.fillNumStdGenJets = cms.untracked.bool(True)

# Settings: default for FullSim MC
ISDATA = False
ISFASTSIM = False
runMetCorrAndUnc = True
updateJECs = True
usePrivateSQlite = True
JECUNCFILE = 'data/JEC/Spring16_23Sep2016V2_MC_Uncertainty_AK4PFchs.txt' # not used
updateBTagging = True

# FastSim samples
if 'FastAsympt25ns' in DatasetName or 'RunIISpring15FSPremix' in DatasetName or 'PUSpring16Fast' in DatasetName :
    print 'Running on FastSim'
    ISFASTSIM = True
    runMetCorrAndUnc = True
    updateJECs = True
    usePrivateSQlite = True
    JECUNCFILE = 'data/JEC/Spring16_FastSimV1_MC_Uncertainty_AK4PFchs.txt'
    process.TestAnalyzer.globalTag = cms.string('80X_mcRun2_asymptotic_2016_miniAODv2_v1')
    process.TestAnalyzer.getGenLumiHeader = cms.untracked.bool(True)
    process.TestAnalyzer.Triggers.isFastSim = cms.untracked.bool(True)
    process.TestAnalyzer.EventInfo.lheEvtInfo = cms.InputTag('source')
    process.TestAnalyzer.EventInfo.isFastSim = cms.untracked.bool(True)
    if 'SMS' in DatasetName or 'T2bW' in DatasetName :
        print 'SMS: will save masses'
        process.TestAnalyzer.EventInfo.isMassScan = cms.untracked.bool(True)

# Specific to data
if '/store/data' in DatasetName or re.match(r'^/[a-zA-Z]+/Run[0-9]{4}[A-Z]', DatasetName):
    ISDATA = True
    runMetCorrAndUnc = True
    updateJECs = True
    usePrivateSQlite = True
#     JECUNCFILE = 'data/JEC/Spring16_23Sep2016BCDV2_DATA_Uncertainty_AK4PFchs.txt' #FIXME: IOV dependence - not used
    import FWCore.PythonUtilities.LumiList as LumiList
    import os
    jsonFile = os.path.expandvars("$CMSSW_BASE/src/data/JSON/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt")
    process.source.lumisToProcess = LumiList.LumiList(filename=jsonFile).getVLuminosityBlockRange()
    process.TestAnalyzer.isData = cms.int32(1)
    process.TestAnalyzer.globalTag = cms.string('80X_dataRun2_2016SeptRepro_v4')
    if 'PromptReco' in DatasetName:
        process.TestAnalyzer.globalTag = cms.string('80X_dataRun2_Prompt_v14')
    process.TestAnalyzer.Jets.fillJetGenInfo = cms.untracked.bool(False)
    process.TestAnalyzer.Muons.fillMuonGenInfo = cms.untracked.bool(False)
    process.TestAnalyzer.Electrons.fillElectronGenInfo = cms.untracked.bool(False)
    process.TestAnalyzer.METFilters.bits = cms.InputTag('TriggerResults', '', 'RECO')

# Import of standard configurations
process.load("Configuration.EventContent.EventContent_cff")
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = process.TestAnalyzer.globalTag

#==============================================================================================================================#
# badMuon and badChargedHadron Filters
#from RecoMET.METFilters.chargedHadronTrackResolutionFilter_cfi import *
process.load('RecoMET.METFilters.BadChargedCandidateFilter_cfi')
process.BadChargedCandidateFilter.muons = cms.InputTag("slimmedMuons")
process.BadChargedCandidateFilter.PFCandidates = cms.InputTag("packedPFCandidates")
process.BadChargedCandidateFilter.taggingMode = cms.bool(True)

process.load('RecoMET.METFilters.BadPFMuonFilter_cfi')
process.BadPFMuonFilter.muons = cms.InputTag("slimmedMuons")
process.BadPFMuonFilter.PFCandidates = cms.InputTag("packedPFCandidates")
process.BadPFMuonFilter.taggingMode = cms.bool(True)


#==============================================================================================================================#
# Electron ID, following prescription in
# https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationRun2:
# set up everything that is needed to compute electron IDs and
# add the ValueMaps with ID decisions into the event data stream

# Load tools and function definitions
from PhysicsTools.SelectorUtils.tools.vid_id_tools import *

switchOnVIDElectronIdProducer(process, DataFormat.MiniAOD)

# Define which IDs we want to produce
# my_id_modules = ['AnalysisTools.ObjectSelection.cutBasedElectronID_Spring15_25ns_V1_cff',
#                  'AnalysisTools.ObjectSelection.mvaElectronID_Spring15_25ns_nonTrig_V1_cff',
#                  'AnalysisTools.ObjectSelection.mvaElectronID_Spring15_25ns_Trig_V1_cff']
my_id_modules = ['RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Spring15_25ns_V1_cff',
                 'RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Spring15_25ns_nonTrig_V1_cff',
                 'RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Spring15_25ns_Trig_V1_cff']

# Add them to the VID producer
if process.TestAnalyzer.Electrons.isFilled:
    for idmod in my_id_modules:
        setupAllVIDIdsInModule(process, idmod, setupVIDElectronSelection)

# Set ID tags
process.TestAnalyzer.Electrons.vetoId = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-veto")
process.TestAnalyzer.Electrons.looseId = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-loose")
process.TestAnalyzer.Electrons.mediumId = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-medium")
process.TestAnalyzer.Electrons.tightId = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-tight")
process.TestAnalyzer.Electrons.mvanontrigwp90Id = cms.InputTag("egmGsfElectronIDs:mvaEleID-Spring15-25ns-nonTrig-V1-wp90")
process.TestAnalyzer.Electrons.mvanontrigwp80Id = cms.InputTag("egmGsfElectronIDs:mvaEleID-Spring15-25ns-nonTrig-V1-wp80")
process.TestAnalyzer.Electrons.mvanontrigValuesMap = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Spring15NonTrig25nsV1Values")
process.TestAnalyzer.Electrons.mvanontrigCategoriesMap = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Spring15NonTrig25nsV1Categories")
process.TestAnalyzer.Electrons.mvatrigwp90Id = cms.InputTag("egmGsfElectronIDs:mvaEleID-Spring15-25ns-Trig-V1-wp90")
process.TestAnalyzer.Electrons.mvatrigwp80Id = cms.InputTag("egmGsfElectronIDs:mvaEleID-Spring15-25ns-Trig-V1-wp80")
process.TestAnalyzer.Electrons.mvatrigValuesMap = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Spring15Trig25nsV1Values")
process.TestAnalyzer.Electrons.mvatrigCategoriesMap = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Spring15Trig25nsV1Categories")

#==============================================================================================================================#
# Photon ID, following prescription in
# https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedPhotonIdentificationRun2

switchOnVIDPhotonIdProducer(process, DataFormat.MiniAOD)

# Define which IDs we want to produce
my_photon_id_modules = ['RecoEgamma.PhotonIdentification.Identification.cutBasedPhotonID_Spring16_V2p2_cff']

# Add them to the VID producer
if process.TestAnalyzer.Photons.isFilled:
    for idmod in my_photon_id_modules:
        setupAllVIDIdsInModule(process, idmod, setupVIDPhotonSelection)

# Set ID tags
process.TestAnalyzer.Photons.looseId = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring16-V2p2-loose")
process.TestAnalyzer.Photons.mediumId = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring16-V2p2-medium")
process.TestAnalyzer.Photons.tightId = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring16-V2p2-tight")

if not 'Photon' in DatasetName and not 'GJets' in DatasetName and not 'DYToEE' in DatasetName and not 'QCD' in DatasetName :
    process.TestAnalyzer.Photons.fillPhotonIDVars = cms.untracked.bool(False)
    process.TestAnalyzer.Photons.fillPhotonIsoVars = cms.untracked.bool(False)

#==============================================================================================================================#
# Jets and quark-gluon tagging
process.load('ObjectProducers.JetProducers.jet_producer_sequences_cfi')
process.load('ObjectProducers.JetProducers.jet_qgtagging_cfi')

if ISDATA :
    process.ak4Jets.produceGen = cms.bool(False)
    process.ak4Jets.producePartonJets = cms.bool(False)
    delattr(process, 'ak4FlvAssoc')
    delattr(process, 'redGenAssoc')

process.TestAnalyzer.Jets.jetCorrInputFile = cms.untracked.FileInPath(JECUNCFILE)

#==============================================================================================================================#
# Custom METs
# Configurable options
runOnData = ISDATA  # data/MC switch
useHFCandidates = True  # create an additionnal NoHF slimmed MET collection if the option is set to false
applyResiduals = True  # application of residual corrections.

# For adding NoHF MET
if not useHFCandidates:
    print 'Producing NoHF candidate collection'
    process.noHFCands = cms.EDFilter("CandPtrSelector",
                                     src=cms.InputTag("packedPFCandidates"),
                                     cut=cms.string("abs(pdgId)!=1 && abs(pdgId)!=2 && abs(eta)<3.0")
                                     )

# Run using external JECs if required
if usePrivateSQlite:
    from CondCore.DBCommon.CondDBSetup_cfi import *
    import os
    era = "Summer16_23Sep2016AllV3_DATA" if ISDATA else "Summer16_23Sep2016V3_MC"
    if ISFASTSIM :
        era = "Spring16_25nsFastSimMC_V1"
    dBFile = era+'.db' if runCRAB else os.path.expandvars("$CMSSW_BASE/src/data/JEC/" + era + ".db")
    print 'Using sqlite file ', dBFile, ' for JECs'
    process.jec = cms.ESSource("PoolDBESSource", CondDBSetup,
                               connect=cms.string("sqlite:///" + dBFile),
                               toGet=cms.VPSet(
            cms.PSet(
                record=cms.string("JetCorrectionsRecord"),
                tag=cms.string("JetCorrectorParametersCollection_" + era + "_AK4PF"),
                label=cms.untracked.string("AK4PF")
                ),
            cms.PSet(
                record=cms.string("JetCorrectionsRecord"),
                tag=cms.string("JetCorrectorParametersCollection_" + era + "_AK4PFchs"),
                label=cms.untracked.string("AK4PFchs")
                ),
            cms.PSet(
                record=cms.string("JetCorrectionsRecord"),
                tag=cms.string("JetCorrectorParametersCollection_" + era + "_AK8PFchs"),
                label=cms.untracked.string("AK8PFchs")
                )
            )
                               )
    process.es_prefer_jec = cms.ESPrefer("PoolDBESSource", "jec")

# Jets are rebuilt from those candidates by the tools, no need to do anything else
if runMetCorrAndUnc :
    print 'Adding MET corrections'
    from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD

    # Default configuration for miniAOD reprocessing, change the isData flag to run on data
    # For a full met computation, remove the pfCandColl input
    runMetCorAndUncFromMiniAOD(process,
                               isData=runOnData
                               )
    #process.shiftedPatJetEnDown.jetCorrUncertaintyTag = cms.string("")
    #process.shiftedPatJetEnUp.jetCorrUncertaintyTag = cms.string("")
    process.TestAnalyzer.EventInfo.mets = cms.InputTag('slimmedMETs', '', 'run')

    if not useHFCandidates:
        runMetCorAndUncFromMiniAOD(process,
                               isData=runOnData,
                               pfCandColl=cms.InputTag("noHFCands"),
                               reclusterJets=True,  # needed for NoHF
                               recoMetFromPFCs=True,  # needed for NoHF
                               postfix="NoHF"
                               )
        process.shiftedPatJetEnDownNoHF.jetCorrUncertaintyTag = cms.string("")
        process.shiftedPatJetEnUpNoHF.jetCorrUncertaintyTag = cms.string("")
        process.TestAnalyzer.EventInfo.metsNoHF = cms.InputTag('slimmedMETsNoHF', '', 'run')

    # The lines below remove the L2L3 residual corrections when processing data
    if not applyResiduals:
        process.patPFMetT1T2Corr.jetCorrLabelRes = cms.InputTag("L3Absolute")
        process.patPFMetT1T2SmearCorr.jetCorrLabelRes = cms.InputTag("L3Absolute")
        process.patPFMetT2Corr.jetCorrLabelRes = cms.InputTag("L3Absolute")
        process.patPFMetT2SmearCorr.jetCorrLabelRes = cms.InputTag("L3Absolute")
        process.shiftedPatJetEnDown.jetCorrLabelUpToL3Res = cms.InputTag("ak4PFCHSL1FastL2L3Corrector")
        process.shiftedPatJetEnUp.jetCorrLabelUpToL3Res = cms.InputTag("ak4PFCHSL1FastL2L3Corrector")

        if not useHFCandidates:
              process.patPFMetT1T2CorrNoHF.jetCorrLabelRes = cms.InputTag("L3Absolute")
              process.patPFMetT1T2SmearCorrNoHF.jetCorrLabelRes = cms.InputTag("L3Absolute")
              process.patPFMetT2CorrNoHF.jetCorrLabelRes = cms.InputTag("L3Absolute")
              process.patPFMetT2SmearCorrNoHF.jetCorrLabelRes = cms.InputTag("L3Absolute")
              process.shiftedPatJetEnDownNoHF.jetCorrLabelUpToL3Res = cms.InputTag("ak4PFCHSL1FastL2L3Corrector")
              process.shiftedPatJetEnUpNoHF.jetCorrLabelUpToL3Res = cms.InputTag("ak4PFCHSL1FastL2L3Corrector")

#==============================================================================================================================#
# Filter to remove super large MET events
from AnalysisBase.Analyzer.eventfilters_cfi import met131TeVFilter

process.met131TeVFilter = met131TeVFilter
process.met131TeVFilter.globalTag = process.TestAnalyzer.globalTag
process.met131TeVFilter.isData = process.TestAnalyzer.isData
process.met131TeVFilter.EventInfo.mets = cms.InputTag('slimmedMETs', processName=cms.InputTag.skipCurrentProcess())
process.met131TeVFilter.EventInfo.metsOOB = cms.InputTag('slimmedMETs', processName=cms.InputTag.skipCurrentProcess())
process.met131TeVFilter.EventInfo.metsNoHF = cms.InputTag('slimmedMETsNoHF', processName=cms.InputTag.skipCurrentProcess())

#==============================================================================================================================#
# Get puppi corrected ak8 jets using jettoolbox
# from JMEAnalysis.JetToolbox.jetToolbox_cff import *
# 
# JETCorrLevels = ['L1FastJet','L2Relative', 'L3Absolute']
# if ISDATA: JETCorrLevels.append('L2L3Residual')
# 
# # add CTT
# jetToolbox(process, 'ca8', 'dummy', 'out', JETCorrPayload = 'AK8PFchs', JETCorrLevels = JETCorrLevels, miniAOD=True, runOnMC=(not ISDATA), addCMSTopTagger=True)

# add HTTv2
# runHTT = False
# if runHTT:
#     from ObjectProducers.JetProducers.htt_cfg import *
#     process.httseq = cms.Sequence()
#     HTTJets(process,process.httseq,"CA15HTT",1.5)

# add subjet b/c-tagging
# runSubjetCTagging = False
# if runSubjetCTagging:
#     process.TestAnalyzer.AK8FatJets.fillSubjetCTag = True
#     bTagDiscriminators=['pfCombinedInclusiveSecondaryVertexV2BJetTags','pfCombinedMVAV2BJetTags','pfCombinedCvsLJetTags','pfCombinedCvsBJetTags']
#  
#     jetToolbox(process, 'ak8', 'dummy', 'out', PUMethod = 'CHS',   JETCorrPayload = 'AK8PFchs',   JETCorrLevels = JETCorrLevels, miniAOD=True, runOnMC=(not ISDATA), addSoftDrop=True, addSoftDropSubjets=True, bTagDiscriminators=bTagDiscriminators)
#     process.TestAnalyzer.AK8FatJets.sdSubjets = cms.InputTag('selectedPatJetsAK8PFCHSSoftDropPacked')
#  
#     jetToolbox(process, 'ak8', 'dummy', 'out', PUMethod = 'Puppi', JETCorrPayload = 'AK8PFPuppi', JETCorrLevels = JETCorrLevels, miniAOD=True, runOnMC=(not ISDATA), addSoftDrop=True, addSoftDropSubjets=True, bTagDiscriminators=bTagDiscriminators)
#     process.TestAnalyzer.AK8FatJets.puppiSubjets = cms.InputTag('selectedPatJetsAK8PFPuppiSoftDropPacked')

#==============================================================================================================================#
# Also update jets with different JECs if needed
if updateJECs or updateBTagging:
    from JMEAnalysis.JetToolbox.jetToolbox_cff import jetToolbox

    print 'Adding sequence to update JECs'
    JETCorrLevels = ['L1FastJet','L2Relative','L3Absolute']
    if ISDATA and applyResiduals: JETCorrLevels.append('L2L3Residual')

    bTagDiscriminators = None
    if updateBTagging:
        print 'Adding sequence to update b-tagging'
        bTagDiscriminators = [
            'deepFlavourJetTags:probudsg',
            'deepFlavourJetTags:probb',
            'deepFlavourJetTags:probc',
            'deepFlavourJetTags:probbb',
            'deepFlavourJetTags:probcc',
            'deepFlavourCMVAJetTags:probudsg',
            'deepFlavourCMVAJetTags:probb',
            'deepFlavourCMVAJetTags:probc',
            'deepFlavourCMVAJetTags:probbb',
            'deepFlavourCMVAJetTags:probcc',
        ]

    jetToolbox(process, 'ak4', 'jetSequence', 'out', PUMethod = 'CHS', updateCollection='slimmedJets', JETCorrPayload = 'AK4PFchs', JETCorrLevels = JETCorrLevels, runOnMC=(not ISDATA), bTagDiscriminators=bTagDiscriminators)
    jetToolbox(process, 'ak8', 'jetSequence', 'out', PUMethod = 'CHS', updateCollection='slimmedJetsAK8', JETCorrPayload = 'AK8PFchs', JETCorrLevels = JETCorrLevels, runOnMC=(not ISDATA))

### old updateJEC ###
#     from PhysicsTools.PatAlgos.producersLayer1.jetUpdater_cff import updatedPatJetCorrFactors
#     process.patJetCorrFactorsReapplyJEC = updatedPatJetCorrFactors.clone(
#         src=cms.InputTag("slimmedJets"),
#         levels=['L1FastJet',
#                   'L2Relative',
#                   'L3Absolute'],
#         payload='AK4PFchs')  # Make sure to choose the appropriate levels and payload here!
#     process.patJetCorrFactorsReapplyJECAK8 = updatedPatJetCorrFactors.clone(
#         src=cms.InputTag("slimmedJetsAK8"),
#         levels=['L1FastJet',
#                   'L2Relative',
#                   'L3Absolute'],
#         payload='AK8PFchs')  # Make sure to choose the appropriate levels and payload here!
#     if ISDATA and applyResiduals:
#         process.patJetCorrFactorsReapplyJEC.levels = ['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual']
#         process.patJetCorrFactorsReapplyJECAK8.levels = ['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual']
# 
#     from PhysicsTools.PatAlgos.producersLayer1.jetUpdater_cff import updatedPatJets
#     process.patJetsReapplyJEC = updatedPatJets.clone(
#         jetSource=cms.InputTag("slimmedJets"),
#         jetCorrFactorsSource=cms.VInputTag(cms.InputTag("patJetCorrFactorsReapplyJEC"))
#     )
#     process.patJetsAK8ReapplyJEC = updatedPatJets.clone(
#         jetSource=cms.InputTag("slimmedJetsAK8"),
#         jetCorrFactorsSource=cms.VInputTag(cms.InputTag("patJetCorrFactorsReapplyJECAK8"))
#     )

    process.TestAnalyzer.Jets.jets = cms.InputTag('selectedUpdatedPatJetsAK4PFCHS')
    process.TestAnalyzer.AK8FatJets.fatJets = cms.InputTag('selectedUpdatedPatJetsAK8PFCHS')
    process.TestAnalyzer.Muons.jets = cms.InputTag('selectedUpdatedPatJetsAK4PFCHS')
    process.TestAnalyzer.Electrons.jets = cms.InputTag('selectedUpdatedPatJetsAK4PFCHS')
    process.TestAnalyzer.Photons.jets = cms.InputTag('selectedUpdatedPatJetsAK4PFCHS')
    process.TestAnalyzer.PFCandidates.jets = cms.InputTag('selectedUpdatedPatJetsAK4PFCHS')
    #process.TestAnalyzer.EventInfo.mets = cms.InputTag('slimmedMETsNewJEC')
    process.QGTagger.srcJets = cms.InputTag('selectedUpdatedPatJetsAK4PFCHS')
    if not ISDATA :
        process.redGenAssoc.recoJetsSrc = cms.InputTag('selectedUpdatedPatJetsAK4PFCHS')


process.seq = cms.Sequence(process.met131TeVFilter *
                           process.ak4PatAssocSeq *
#                            process.httseq  *
                           process.egmGsfElectronIDSequence *
                           process.egmPhotonIDSequence *
                           process.BadChargedCandidateFilter *
                           process.BadPFMuonFilter)


process.p = cms.Path(process.seq * process.TestAnalyzer)

if ISDATA :
    process.p.remove(process.ak4PatAssocSeq)

if not process.TestAnalyzer.Electrons.isFilled :
    process.p.remove(process.egmGsfElectronIDSequence)

if not process.TestAnalyzer.Photons.isFilled :
    process.p.remove(process.egmPhotonIDSequence)

