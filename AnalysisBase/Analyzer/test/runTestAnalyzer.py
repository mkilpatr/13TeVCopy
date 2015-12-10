import FWCore.ParameterSet.Config as cms

process = cms.Process('run')

process.options = cms.untracked.PSet( 
    allowUnscheduled = cms.untracked.bool(True),
    wantSummary = cms.untracked.bool(False) 
)

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger.suppressWarning = cms.untracked.vstring('ak8PFJetsPuppiSoftDrop','ak8PFJetsPuppiPruned')

# Valgrind
#process.ProfilerService = cms.Service (
#       "ProfilerService",
#       firstEvent = cms.untracked.int32(3),
#       lastEvent = cms.untracked.int32(12),
#       paths = cms.untracked.vstring('p') 
#)

from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing('analysis')

options.outputFile = 'evttree.root'
#options.inputFiles = '/store/data/Run2015D/MuonEG/MINIAOD/05Oct2015-v2/60000/00D43A12-C573-E511-8F4C-0025905A60E0.root'
#options.inputFiles = '/store/data/Run2015D/HTMHT/MINIAOD/PromptReco-v4/000/258/159/00000/42D9839F-DC6B-E511-82B0-02163E0136EC.root'
#options.inputFiles = '/store/mc/RunIISpring15MiniAODv2/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v3/60000/00181849-176A-E511-8B11-848F69FD4C94.root'
#options.inputFiles = '/store/mc/RunIISpring15MiniAODv2/GJets_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/10000/18F237DA-CF6D-E511-B4A3-00221981B410.root'
#options.inputFiles = '/store/mc/RunIISpring15MiniAODv2/SMS-T1tttt_mGluino-1500_mLSP-100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/80000/38C49928-8D72-E511-94A6-001E67579188.root'
#options.inputFiles = '/store/mc/RunIISpring15MiniAODv2/TT_TuneCUETP8M1_13TeV-powheg-pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/40000/FEE1CE52-216E-E511-9B5A-0025905A60B8.root'
#options.inputFiles = '/store/mc/RunIISpring15MiniAODv2/TT_TuneCUETP8M1_13TeV-powheg-pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2_ext3-v1/10000/003964D7-D06E-E511-A8DA-001517F7F524.root'
options.inputFiles = '/store/mc/RunIISpring15MiniAODv2/SMS-T1tttt_mGluino-1500to1525_mLSP-50to1125_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/FastAsympt25ns_74X_mcRun2_asymptotic_v2-v1/20000/0A5EEA32-037C-E511-9870-002590596486.root'
#options.inputFiles = '/store/mc/RunIISpring15FSPremix/SMS-T2tt_mStop-600-950_mLSP-1to450_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/MCRUN2_74_V9-v1/00000/00667B2C-6F9B-E511-978E-02163E013DDA.root'


options.maxEvents = -1

options.register('skipEvents',
                 0,
                 VarParsing.multiplicity.singleton,
                 VarParsing.varType.int,
                 "Number of events to skip in processing")

options.parseArguments()

process.TFileService = cms.Service('TFileService',
    fileName = cms.string(options.outputFile)
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )

process.source = cms.Source('PoolSource',
    fileNames = cms.untracked.vstring (options.inputFiles),
    skipEvents = cms.untracked.uint32(options.skipEvents)
)

#==============================================================================================================================#
from AnalysisBase.Analyzer.analyzer_configuration_cfi import nominal_configuration

process.TestAnalyzer = cms.EDFilter('TestAnalyzer',
  nominal_configuration
)

# MC theory systematic weights only saved for specific samples
if 'TTZ' in options.inputFiles[0] or 'TTW' in options.inputFiles[0] or 'tZq' in options.inputFiles[0] or 'SMS' in options.inputFiles[0] :
    print 'Adding theory weights'
    process.TestAnalyzer.EventInfo.saveSystematicWeights = cms.untracked.bool(True)
    if 'SMS' in options.inputFiles[0] :
        process.TestAnalyzer.EventInfo.lheEvtInfo = cms.InputTag('source')
else : 
    process.TestAnalyzer.EventInfo.saveSystematicWeights = cms.untracked.bool(False)

# Turn on jet shape info for specific datasets
if 'Photon' in options.inputFiles[0] or 'JetHT' in options.inputFiles[0] or 'DoubleMu' in options.inputFiles[0] or 'GJets' in options.inputFiles[0] or 'QCD' in options.inputFiles[0] or 'DYJets' in options.inputFiles[0] :
    print 'Adding jet shape info'
    process.TestAnalyzer.Jets.fillJetShapeInfo = cms.untracked.bool(True)

# Global Tag for 50ns MC
if '50ns' in options.inputFiles[0] :
    process.TestAnalyzer.globalTag = cms.string('74X_mcRun2_startup_v2')

# Settings
ISDATA = False
ISFASTSIM = False
ISMINIAODV1 = False
runMetCorrAndUnc = True
updateJECs = True
JECUNCFILE = 'data/JEC/Summer15_25nsV6_MC_Uncertainty_AK4PFchs.txt'

# FastSim samples
if 'FastAsympt25ns' in options.inputFiles[0] or 'RunIISpring15FSPremix' in options.inputFiles[0] :
    print 'Running on FastSim'
    ISFASTSIM = True
    process.TestAnalyzer.METFilters.bits = cms.InputTag('TriggerResults','','HLT')
    process.TestAnalyzer.METFilters.isFastSim = cms.untracked.bool(True)
    if 'RunIISpring15FSPremix' in options.inputFiles[0] :
        print 'Running on miniAODv1'
        ISMINIAODV1 = True
        process.TestAnalyzer.EventInfo.pileupSummaryInfos = cms.InputTag('addPileupInfo')
    if 'SMS' in options.inputFiles[0] :
        print 'SMS: will save masses'
        process.TestAnalyzer.EventInfo.isMassScan = cms.untracked.bool(True)

# Specific to data
if '/store/data' in options.inputFiles[0] :
    ISDATA = True
    JECUNCFILE = 'data/JEC/Summer15_25nsV6_DATA_Uncertainty_AK4PFchs.txt'
    import FWCore.PythonUtilities.LumiList as LumiList
    import os
    jsonFile = os.path.expandvars("$CMSSW_BASE/src/data/JSON/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_Silver.txt")
    process.source.lumisToProcess = LumiList.LumiList(filename = jsonFile).getVLuminosityBlockRange()
    process.TestAnalyzer.isData = cms.int32(1)
    process.TestAnalyzer.globalTag = cms.string('74X_dataRun2_v5')
    process.TestAnalyzer.Jets.fillJetGenInfo = cms.untracked.bool(False)
    process.TestAnalyzer.Muons.fillMuonGenInfo = cms.untracked.bool(False)
    process.TestAnalyzer.Electrons.fillElectronGenInfo = cms.untracked.bool(False)
    process.TestAnalyzer.METFilters.bits = cms.InputTag('TriggerResults','','RECO')

# Import of standard configurations
process.load("Configuration.EventContent.EventContent_cff")
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.GlobalTag.globaltag = process.TestAnalyzer.globalTag


#==============================================================================================================================#
# Electron ID, following prescription in
# https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationRun2:
# set up everything that is needed to compute electron IDs and
# add the ValueMaps with ID decisions into the event data stream

# Load tools and function definitions
from PhysicsTools.SelectorUtils.tools.vid_id_tools import *

process.load("RecoEgamma.ElectronIdentification.egmGsfElectronIDs_cfi")
process.load("AnalysisTools.ObjectSelection.ElectronMVAValueMapProducer_cfi")

# Overwrite collection name
process.egmGsfElectronIDs.physicsObjectSrc = cms.InputTag('slimmedElectrons')

from PhysicsTools.SelectorUtils.centralIDRegistry import central_id_registry
process.egmGsfElectronIDSequence = cms.Sequence(process.electronMVAValueMapProducer * process.egmGsfElectronIDs)

# Define which IDs we want to produce
my_id_modules = ['AnalysisTools.ObjectSelection.cutBasedElectronID_Spring15_25ns_V1_cff',
                 'AnalysisTools.ObjectSelection.mvaElectronID_Spring15_25ns_nonTrig_V1_cff',
                 'AnalysisTools.ObjectSelection.mvaElectronID_Spring15_25ns_Trig_V1_cff']


# Add them to the VID producer
if process.TestAnalyzer.Electrons.isFilled:
    for idmod in my_id_modules:
        setupAllVIDIdsInModule(process,idmod,setupVIDElectronSelection)

# Set ID tags
process.TestAnalyzer.Electrons.vetoId   = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-veto")
process.TestAnalyzer.Electrons.looseId  = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-loose")
process.TestAnalyzer.Electrons.mediumId = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-medium")
process.TestAnalyzer.Electrons.tightId  = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-tight")
process.TestAnalyzer.Electrons.mvanontrigwp90Id        = cms.InputTag("egmGsfElectronIDs:mvaEleID-Spring15-25ns-nonTrig-V1-wp90")
process.TestAnalyzer.Electrons.mvanontrigwp80Id        = cms.InputTag("egmGsfElectronIDs:mvaEleID-Spring15-25ns-nonTrig-V1-wp80")
process.TestAnalyzer.Electrons.mvanontrigValuesMap     = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Spring15NonTrigv225nsV1Values")
process.TestAnalyzer.Electrons.mvanontrigCategoriesMap = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Spring15NonTrigv225nsV1Categories")
process.TestAnalyzer.Electrons.mvatrigwp90Id        = cms.InputTag("egmGsfElectronIDs:mvaEleID-Spring15-25ns-Trig-V1-wp90")
process.TestAnalyzer.Electrons.mvatrigwp80Id        = cms.InputTag("egmGsfElectronIDs:mvaEleID-Spring15-25ns-Trig-V1-wp80")
process.TestAnalyzer.Electrons.mvatrigValuesMap     = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Spring15Trig25nsV1Values")
process.TestAnalyzer.Electrons.mvatrigCategoriesMap = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Spring15Trig25nsV1Categories")

#==============================================================================================================================#
# Photon ID, following prescription in
# https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedPhotonIdentificationRun2

switchOnVIDPhotonIdProducer(process, DataFormat.MiniAOD)

# Define which IDs we want to produce
my_photon_id_modules = ['AnalysisTools.ObjectSelection.cutBasedPhotonID_Spring15_25ns_V1_cff']

# Add them to the VID producer
if process.TestAnalyzer.Photons.isFilled:
    for idmod in my_photon_id_modules:
        setupAllVIDIdsInModule(process,idmod,setupVIDPhotonSelection)

# Set ID tags
process.TestAnalyzer.Photons.looseId    = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring15-25ns-V1-standalone-loose")
process.TestAnalyzer.Photons.mediumId   = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring15-25ns-V1-standalone-medium")
process.TestAnalyzer.Photons.tightId    = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring15-25ns-V1-standalone-tight")

if not 'Photon' in options.inputFiles[0] and not 'GJets' in options.inputFiles[0] and not 'QCD' in options.inputFiles[0] :
    process.TestAnalyzer.Photons.fillPhotonIDVars = cms.untracked.bool(False)
    process.TestAnalyzer.Photons.fillPhotonIsoVars = cms.untracked.bool(False)

#==============================================================================================================================#
# HCAL Noise Filter 
if not ISFASTSIM :
    process.load('CommonTools.RecoAlgos.HBHENoiseFilterResultProducer_cfi')
    process.HBHENoiseFilterResultProducer.minZeros = cms.int32(99999)
    process.HBHENoiseFilterResultProducer.IgnoreTS4TS5ifJetInLowBVRegion=cms.bool(False) 
    
    process.ApplyBaselineHBHENoiseFilter = cms.EDFilter('BooleanFlagFilter',
       inputLabel = cms.InputTag('HBHENoiseFilterResultProducer','HBHENoiseFilterResult'),
       reverseDecision = cms.bool(False)
    )
    process.ApplyBaselineHBHEIsoNoiseFilter = cms.EDFilter('BooleanFlagFilter',
       inputLabel = cms.InputTag('HBHENoiseFilterResultProducer','HBHEIsoNoiseFilterResult'),
       reverseDecision = cms.bool(False)
    )
    process.ApplyBaselineHBHENoiseFilterRun2Loose = process.ApplyBaselineHBHENoiseFilter.clone(
       inputLabel = cms.InputTag('HBHENoiseFilterResultProducer','HBHENoiseFilterResultRun2Loose'),
    )
    process.ApplyBaselineHBHENoiseFilterRun2Tight = process.ApplyBaselineHBHENoiseFilter.clone(
       inputLabel = cms.InputTag('HBHENoiseFilterResultProducer','HBHENoiseFilterResultRun2Tight'),
    )

#==============================================================================================================================#
# Jets and quark-gluon tagging
process.load('ObjectProducers.JetProducers.jet_producer_sequences_cfi')
process.load('ObjectProducers.JetProducers.jet_qgtagging_cfi')

if ISDATA :
    process.redCA8.produceGen = cms.bool(False)
    process.redCA8.producePartonJets = cms.bool(False)
    process.ak4Jets.produceGen = cms.bool(False)
    process.ak4Jets.producePartonJets = cms.bool(False)
    delattr(process,'ak4FlvAssoc')
    delattr(process,'redGenAssoc')

process.TestAnalyzer.Jets.jetCorrInputFile = cms.untracked.FileInPath(JECUNCFILE)

#==============================================================================================================================#
# Custom METs
# Configurable options
runOnData=ISDATA        #data/MC switch
usePrivateSQlite=False  #use external JECs (sqlite file)
useHFCandidates=False   #create an additionnal NoHF slimmed MET collection if the option is set to false
applyResiduals=True     #application of residual corrections.

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
    era="Summer15_25nsV6_DATA" if ISDATA else "Summer15_25nsV6_MC"
    dBFile = os.path.expandvars("$CMSSW_BASE/src/data/JEC/"+era+".db")
    print 'Using sqlite file ', dBFile, ' for JECs'
    process.jec = cms.ESSource("PoolDBESSource",CondDBSetup,
                               connect = cms.string( "sqlite_file://"+dBFile ),
                               toGet =  cms.VPSet(
            cms.PSet(
                record = cms.string("JetCorrectionsRecord"),
                tag = cms.string("JetCorrectorParametersCollection_"+era+"_AK4PF"),
                label= cms.untracked.string("AK4PF")
                ),
            cms.PSet(
                record = cms.string("JetCorrectionsRecord"),
                tag = cms.string("JetCorrectorParametersCollection_"+era+"_AK4PFchs"),
                label= cms.untracked.string("AK4PFchs")
                ),
            cms.PSet(
                record = cms.string("JetCorrectionsRecord"),
                tag = cms.string("JetCorrectorParametersCollection_"+era+"_AK8PFchs"),
                label= cms.untracked.string("AK8PFchs")
                ),
            )
                               )
    process.es_prefer_jec = cms.ESPrefer("PoolDBESSource",'jec')

# Jets are rebuilt from those candidates by the tools, no need to do anything else
if runMetCorrAndUnc :
    print 'Adding MET corrections'
    from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD

    # Default configuration for miniAOD reprocessing, change the isData flag to run on data
    # For a full met computation, remove the pfCandColl input
    if ISMINIAODV1 :
        runMetCorAndUncFromMiniAOD(process,
                               isData=runOnData,
                               jecUncFile=JECUNCFILE,
                               repro74X=True
                               )
    else :
        runMetCorAndUncFromMiniAOD(process,
                               isData=runOnData,
                               jecUncFile=JECUNCFILE
                               )
    process.shiftedPatJetEnDown.jetCorrUncertaintyTag = cms.string("")
    process.shiftedPatJetEnUp.jetCorrUncertaintyTag = cms.string("")
    process.TestAnalyzer.EventInfo.mets = cms.InputTag('slimmedMETs','','run')

    if not useHFCandidates:
        if ISMINIAODV1 :
            runMetCorAndUncFromMiniAOD(process,
                                   isData=runOnData,
                                   jecUncFile=JECUNCFILE,
                                   pfCandColl=cms.InputTag("noHFCands"),
                                   postfix="NoHF",
                                   repro74X=True,
                                   reclusterJets=True
                                   )
        else :
            runMetCorAndUncFromMiniAOD(process,
                                   isData=runOnData,
                                   jecUncFile=JECUNCFILE,
                                   pfCandColl=cms.InputTag("noHFCands"),
                                   postfix="NoHF"
                                   )
        process.shiftedPatJetEnDownNoHF.jetCorrUncertaintyTag = cms.string("")
        process.shiftedPatJetEnUpNoHF.jetCorrUncertaintyTag = cms.string("")
        process.TestAnalyzer.EventInfo.metsNoHF = cms.InputTag('slimmedMETsNoHF','','run')

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
if ISMINIAODV1 :
    process.met131TeVFilter.EventInfo.metsNoHF = cms.InputTag('slimmedMETsNoHF','','run')

#==============================================================================================================================#
# Get puppi corrected ak8 jets using jettoolbox
from JMEAnalysis.JetToolbox.jetToolbox_cff import *

process.load('CommonTools.PileupAlgos.Puppi_cff')

process.puppi.useExistingWeights = True
process.puppi.candName = cms.InputTag( 'packedPFCandidates' )
process.puppi.vertexName = cms.InputTag( 'offlineSlimmedPrimaryVertices' )

if ISDATA and applyResiduals:
    jetToolbox(process, 'ak8', 'dummy', 'out', PUMethod = 'Puppi', JETCorrPayload = 'AK8PFPuppi', JETCorrLevels = ['L1FastJet', 'L2Relative', 'L3Absolute','L2L3Residual'], miniAOD=True, runOnMC=False, addPruning=True, addSoftDrop=True, addNsub=True, newPFCollection=True, nameNewPFCollection='puppi')
else :
    jetToolbox(process, 'ak8', 'dummy', 'out', PUMethod = 'Puppi', JETCorrPayload = 'AK8PFPuppi', JETCorrLevels = ['L1FastJet', 'L2Relative', 'L3Absolute'], miniAOD=True, runOnMC=(ISDATA != True), addPruning=True, addSoftDrop=True, addNsub=True, newPFCollection=True, nameNewPFCollection='puppi')

#==============================================================================================================================#
# Also update jets with different JECs if needed
if updateJECs:
    print 'Adding sequence to update JECs'
    from PhysicsTools.PatAlgos.producersLayer1.jetUpdater_cff import patJetCorrFactorsUpdated
    process.patJetCorrFactorsReapplyJEC = patJetCorrFactorsUpdated.clone(
        src = cms.InputTag("slimmedJets"),
        levels = ['L1FastJet', 
                  'L2Relative', 
                  'L3Absolute'],
        payload = 'AK4PFchs' ) # Make sure to choose the appropriate levels and payload here!
    process.patJetCorrFactorsReapplyJECAK8 = patJetCorrFactorsUpdated.clone(
        src = cms.InputTag("slimmedJetsAK8"),
        levels = ['L1FastJet', 
                  'L2Relative', 
                  'L3Absolute'],
        payload = 'AK8PFchs' ) # Make sure to choose the appropriate levels and payload here!
    if ISDATA and applyResiduals:
        process.patJetCorrFactorsReapplyJEC.levels = ['L1FastJet','L2Relative','L3Absolute','L2L3Residual']
        process.patJetCorrFactorsReapplyJECAK8.levels = ['L1FastJet','L2Relative','L3Absolute','L2L3Residual']

    from PhysicsTools.PatAlgos.producersLayer1.jetUpdater_cff import patJetsUpdated
    process.patJetsReapplyJEC = patJetsUpdated.clone(
        jetSource = cms.InputTag("slimmedJets"),
        jetCorrFactorsSource = cms.VInputTag(cms.InputTag("patJetCorrFactorsReapplyJEC"))
    )
    process.patJetsAK8ReapplyJEC = patJetsUpdated.clone(
        jetSource = cms.InputTag("slimmedJetsAK8"),
        jetCorrFactorsSource = cms.VInputTag(cms.InputTag("patJetCorrFactorsReapplyJECAK8"))
    )
    process.TestAnalyzer.Jets.jets = cms.InputTag('patJetsReapplyJEC')
    process.TestAnalyzer.AK8FatJets.fatJets = cms.InputTag('patJetsAK8ReapplyJEC')
    process.TestAnalyzer.Muons.jets = cms.InputTag('patJetsReapplyJEC')
    process.TestAnalyzer.Electrons.jets = cms.InputTag('patJetsReapplyJEC')
    process.TestAnalyzer.Photons.jets = cms.InputTag('patJetsReapplyJEC')
    process.TestAnalyzer.PFCandidates.jets = cms.InputTag('patJetsReapplyJEC')
    process.QGTagger.srcJets = cms.InputTag('patJetsReapplyJEC')
    if not ISDATA :
        process.redGenAssoc.recoJetsSrc = cms.InputTag('patJetsReapplyJEC')

    process.seq = cms.Sequence(process.patJetCorrFactorsReapplyJEC *
                               process.patJetCorrFactorsReapplyJEC *
                               process.patJetCorrFactorsReapplyJECAK8 *
                               process.patJetsReapplyJEC        *
                               process.patJetsAK8ReapplyJEC     *
                               process.ak4PatAssocSeq           * 
                               process.met131TeVFilter          *
                               process.ca8JetsSeq               *
                               process.egmGsfElectronIDSequence * 
                               process.egmPhotonIDSequence)


else :
    process.seq = cms.Sequence(process.met131TeVFilter *
                               process.ak4PatAssocSeq  * 
                               process.ca8JetsSeq      *
                               process.egmGsfElectronIDSequence * 
                               process.egmPhotonIDSequence *
                               process.QGTagger)

if ISFASTSIM :
    process.p = cms.Path(process.seq * process.TestAnalyzer)
else :
    process.p = cms.Path(process.seq * process.HBHENoiseFilterResultProducer * process.TestAnalyzer)

if ISDATA :
    process.p.remove(process.ak4PatAssocSeq)

if not process.TestAnalyzer.Electrons.isFilled :
    process.p.remove(process.egmGsfElectronIDSequence)

if not process.TestAnalyzer.Photons.isFilled :
    process.p.remove(process.egmPhotonIDSequence)

