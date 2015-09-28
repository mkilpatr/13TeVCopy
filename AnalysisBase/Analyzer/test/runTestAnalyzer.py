import FWCore.ParameterSet.Config as cms

process = cms.Process('run')

process.options = cms.untracked.PSet( 
    allowUnscheduled = cms.untracked.bool(True),
    wantSummary = cms.untracked.bool(False) 
)

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100

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
options.inputFiles = '/store/mc/RunIISpring15DR74/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/Asympt50ns_MCRUN2_74_V9A-v1/00000/0066F143-F8FD-E411-9A0B-D4AE526A0D2E.root'
#options.inputFiles = '/store/mc/RunIISpring15DR74/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/Asympt25ns_MCRUN2_74_V9-v2/00000/06B5178E-F008-E511-A2CF-00261894390B.root'
#options.inputFiles = '/store/mc/RunIISpring15DR74/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/Asympt25ns_MCRUN2_74_V9-v3/10000/009D49A5-7314-E511-84EF-0025905A605E.root'
#options.inputFiles = '/store/data/Run2015B/BTagCSV/MINIAOD/17Jul2015-v1/40000/C88E0BCD-972E-E511-B231-0025905B85F6.root'
#options.inputFiles = '/store/mc/RunIISpring15DR74/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/Asympt25ns_MCRUN2_74_V9-v1/00000/022B08C4-C702-E511-9995-D4856459AC30.root'
#options.inputFiles = '/store/mc/RunIISpring15DR74/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/MINIAODSIM/Asympt25ns_MCRUN2_74_V9-v1/30000/60087A61-9134-E511-B0C6-0025905B855E.root'
#options.inputFiles = '/store/mc/RunIISpring15DR74/GJets_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/Asympt25ns_MCRUN2_74_V9-v1/00000/2AC9FDED-4319-E511-AAF9-02163E011C20.root'
#options.inputFiles = '/store/mc/RunIISpring15DR74/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/MINIAODSIM/Asympt25ns_MCRUN2_74_V9-v1/00000/906D9FB3-4906-E511-9C81-0025905A6056.root'
#options.inputFiles = '/store/mc/RunIISpring15DR74/QCD_Pt-20to30_EMEnriched_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/Asympt25ns_MCRUN2_74_V9-v1/10000/028A8588-3A03-E511-B296-0025905B858A.root'
#options.inputFiles = '/store/data/Run2015B/MET/MINIAOD/PromptReco-v1/000/251/244/00000/5E425D83-6B27-E511-98E0-02163E01345F.root'
#options.inputFiles = '/store/data/Run2015C/HTMHT/MINIAOD/PromptReco-v1/000/253/620/00000/1E9DB47F-1640-E511-B7EB-02163E014450.root'

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


from AnalysisBase.Analyzer.analyzer_configuration_cfi import nominal_configuration

process.TestAnalyzer = cms.EDFilter('TestAnalyzer',
  nominal_configuration
)

# only look at LHEEventProduct info if it's available
if 'madgraph' in options.inputFiles[0] or 'powheg' in options.inputFiles[0] or 'amcatnlo' in options.inputFiles[0]:
   process.TestAnalyzer.EventInfo.saveSystematicWeights = cms.untracked.bool(True)
else : 
   process.TestAnalyzer.EventInfo.saveSystematicWeights = cms.untracked.bool(False)

# global Tag for 50ns MC
if '50ns' in options.inputFiles[0] :
    process.TestAnalyzer.globalTag = cms.string('74X_mcRun2_startup_v2')

ISDATA = False

if '/store/data' in options.inputFiles[0] :
    ISDATA = True
    process.TestAnalyzer.isData = cms.int32(1)
    process.TestAnalyzer.globalTag = cms.string('74X_dataRun2_v2')
    process.TestAnalyzer.Jets.fillJetGenInfo = cms.untracked.bool(False)
    process.TestAnalyzer.Muons.fillMuonGenInfo = cms.untracked.bool(False)
    process.TestAnalyzer.Electrons.fillElectronGenInfo = cms.untracked.bool(False)
    if '17Jul2015' in options.inputFiles[0] :
        process.TestAnalyzer.METFilters.bits = cms.InputTag('TriggerResults','','PAT')
    else :
        process.TestAnalyzer.METFilters.bits = cms.InputTag('TriggerResults','','RECO')
        process.TestAnalyzer.EventInfo.metsOOB = cms.InputTag('slimmedMETs','','RECO')

# import of standard configurations
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
                 'AnalysisTools.ObjectSelection.cutBasedElectronID_Spring15_50ns_V2_cff',
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

process.load("RecoEgamma.PhotonIdentification.egmPhotonIDs_cfi")

# Overwrite collection name
process.egmPhotonIDs.physicsObjectSrc = cms.InputTag('slimmedPhotons')

# Load the producer module to build full 5x5 cluster shapes and whatever 
# else is needed for IDs
process.load('RecoEgamma.PhotonIdentification.PhotonIDValueMapProducer_cfi')

process.egmPhotonIDSequence = cms.Sequence(process.photonIDValueMapProducer * process.egmPhotonIDs)

# Define which IDs we want to produce
my_photon_id_modules = ['AnalysisTools.ObjectSelection.cutBasedPhotonID_Spring15_50ns_V1_cff']

# Add them to the VID producer
if process.TestAnalyzer.Photons.isFilled:
    for idmod in my_photon_id_modules:
        setupAllVIDIdsInModule(process,idmod,setupVIDPhotonSelection)

# Set ID tags
#process.TestAnalyzer.Photons.vetoId     = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring15-50ns-V1-standalone-veto")
process.TestAnalyzer.Photons.looseId    = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring15-50ns-V1-standalone-loose")
process.TestAnalyzer.Photons.mediumId   = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring15-50ns-V1-standalone-medium")
process.TestAnalyzer.Photons.tightId    = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring15-50ns-V1-standalone-tight")


#==============================================================================================================================#
# ======================== HCAL_Noise_Filter ===================================
process.load('CommonTools.RecoAlgos.HBHENoiseFilterResultProducer_cfi')
process.HBHENoiseFilterResultProducer.minZeros = cms.int32(99999)

process.ApplyBaselineHBHENoiseFilter = cms.EDFilter('BooleanFlagFilter',
   inputLabel = cms.InputTag('HBHENoiseFilterResultProducer','HBHENoiseFilterResult'),
   reverseDecision = cms.bool(False)
)
process.ApplyBaselineHBHENoiseFilterRun2Loose = process.ApplyBaselineHBHENoiseFilter.clone(
   inputLabel = cms.InputTag('HBHENoiseFilterResultProducer','HBHENoiseFilterResultRun2Loose'),
)
process.ApplyBaselineHBHENoiseFilterRun2Tight = process.ApplyBaselineHBHENoiseFilter.clone(
   inputLabel = cms.InputTag('HBHENoiseFilterResultProducer','HBHENoiseFilterResultRun2Tight'),
)
# ================================ end of HCAL NOISE FILTER =====================

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

#==============================================================================================================================#
# Custom METs
# Configurable options =======================================================================
runOnData=ISDATA        #data/MC switch
usePrivateSQlite=False  #use external JECs (sqlite file)
useHFCandidates=False   #create an additionnal NoHF slimmed MET collection if the option is set to false
applyResiduals=True     #application of residual corrections. Have to be set to True once the 13 TeV residual corrections are available. False to be kept meanwhile. Can be kept to False later for private tests or for analysis checks and developments (not the official recommendation!).

# For adding NoHF MET
if not useHFCandidates:
    process.noHFCands = cms.EDFilter("CandPtrSelector",
                                     src=cms.InputTag("packedPFCandidates"),
                                     cut=cms.string("abs(pdgId)!=1 && abs(pdgId)!=2 && abs(eta)<3.0")
                                     )

# Run using external JECs ... doesn't work at the moment
if usePrivateSQlite:
    from CondCore.DBCommon.CondDBSetup_cfi import *
    import os
    era="Summer15_50nsV2_MC"
    dBFile = os.path.expandvars("$CMSSW_BASE/src/PhysicsTools/PatAlgos/test/"+era+".db")
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
            )
                               )
    process.es_prefer_jec = cms.ESPrefer("PoolDBESSource",'jec')

# Jets are rebuilt from those candidates by the tools, no need to do anything else
from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD

# Default configuration for miniAOD reprocessing, change the isData flag to run on data
# For a full met computation, remove the pfCandColl input
JECUNCFILE = 'PhysicsTools/PatUtils/data/Summer15_25nsV2_MC_UncertaintySources_AK4PFchs.txt' if '25ns' in options.inputFiles[0] else 'PhysicsTools/PatUtils/data/Summer15_50nsV5_MC_UncertaintySources_AK4PFchs.txt'
runMetCorAndUncFromMiniAOD(process,
                           isData=runOnData,
                           jecUncFile=JECUNCFILE
                           )

if not useHFCandidates:
    runMetCorAndUncFromMiniAOD(process,
                               isData=runOnData,
                               jecUncFile=JECUNCFILE,
                               pfCandColl=cms.InputTag("noHFCands"),
                               postfix="NoHF"
                               )

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
# Also update jets with different JECs
updateJECs = True

if updateJECs:
    from PhysicsTools.PatAlgos.producersLayer1.jetUpdater_cff import patJetCorrFactorsUpdated
    process.patJetCorrFactorsReapplyJEC = patJetCorrFactorsUpdated.clone(
        src = cms.InputTag("slimmedJets"),
        levels = ['L1FastJet', 
                  'L2Relative', 
                  'L3Absolute'],
        payload = 'AK4PFchs' ) # Make sure to choose the appropriate levels and payload here!
    if ISDATA and applyResiduals:
        process.patJetCorrFactorsReapplyJEC.levels = ['L1FastJet','L2Relative','L3Absolute','L2L3Residual']

    from PhysicsTools.PatAlgos.producersLayer1.jetUpdater_cff import patJetsUpdated
    process.patJetsReapplyJEC = patJetsUpdated.clone(
        jetSource = cms.InputTag("slimmedJets"),
        jetCorrFactorsSource = cms.VInputTag(cms.InputTag("patJetCorrFactorsReapplyJEC"))
    )
    process.TestAnalyzer.Jets.jets = cms.InputTag('patJetsReapplyJEC')
    process.TestAnalyzer.Muons.jets = cms.InputTag('patJetsReapplyJEC')
    process.TestAnalyzer.Electrons.jets = cms.InputTag('patJetsReapplyJEC')
    process.TestAnalyzer.Photons.jets = cms.InputTag('patJetsReapplyJEC')
    process.TestAnalyzer.PFCandidates.jets = cms.InputTag('patJetsReapplyJEC')
    process.QGTagger.srcJets = cms.InputTag('patJetsReapplyJEC')
    if not ISDATA :
        process.redGenAssoc.recoJetsSrc = cms.InputTag('patJetsReapplyJEC')

    process.p = cms.Path(process.patJetCorrFactorsReapplyJEC *
                         process.patJetsReapplyJEC        *
                         process.ak4PatAssocSeq           * 
                         process.ca8JetsSeq               *
                         process.egmGsfElectronIDSequence * 
                         process.egmPhotonIDSequence      *
                         process.QGTagger                 *
                         process.HBHENoiseFilterResultProducer *
                         process.TestAnalyzer)

else :
    process.p = cms.Path(process.ak4PatAssocSeq           * 
                         process.ca8JetsSeq               *
                         process.egmGsfElectronIDSequence * 
                         process.egmPhotonIDSequence      *
                         process.QGTagger                 *
                         process.HBHENoiseFilterResultProducer *
                         process.TestAnalyzer)

if ISDATA :
    process.p.remove(process.ak4PatAssocSeq)

if not process.TestAnalyzer.Electrons.isFilled :
    process.p.remove(process.egmGsfElectronIDSequence)

if not process.TestAnalyzer.Photons.isFilled :
    process.p.remove(process.egmPhotonIDSequence)

