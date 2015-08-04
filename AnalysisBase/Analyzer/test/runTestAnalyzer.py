import FWCore.ParameterSet.Config as cms

process = cms.Process('run')
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

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
#options.inputFiles = '/store/mc/RunIISpring15DR74/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/Asympt25ns_MCRUN2_74_V9-v2/00000/06B5178E-F008-E511-A2CF-00261894390B.root'
options.inputFiles = '/store/mc/RunIISpring15DR74/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/Asympt25ns_MCRUN2_74_V9-v1/00000/022B08C4-C702-E511-9995-D4856459AC30.root'
#options.inputFiles = '/store/mc/RunIISpring15DR74/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/MINIAODSIM/Asympt25ns_MCRUN2_74_V9-v1/30000/60087A61-9134-E511-B0C6-0025905B855E.root'
#options.inputFiles = '/store/mc/RunIISpring15DR74/GJets_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/Asympt25ns_MCRUN2_74_V9-v1/00000/2AC9FDED-4319-E511-AAF9-02163E011C20.root'
#options.inputFiles = '/store/mc/RunIISpring15DR74/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/MINIAODSIM/Asympt25ns_MCRUN2_74_V9-v1/00000/906D9FB3-4906-E511-9C81-0025905A6056.root'
#options.inputFiles = '/store/mc/RunIISpring15DR74/QCD_Pt-20to30_EMEnriched_TuneCUETP8M1_13TeV_pythia8/MINIAODSIM/Asympt25ns_MCRUN2_74_V9-v1/10000/028A8588-3A03-E511-B296-0025905B858A.root'
#options.inputFiles = '/store/data/Run2015B/MET/MINIAOD/PromptReco-v1/000/251/244/00000/5E425D83-6B27-E511-98E0-02163E01345F.root'

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

ISDATA = False

if '/store/data' in options.inputFiles[0] :
    ISDATA = True
    process.TestAnalyzer.isData = cms.int32(1)
    process.TestAnalyzer.globalTag = cms.string('GR_P_V56')
    process.TestAnalyzer.Jets.fillJetGenInfo = cms.untracked.bool(False)
    process.TestAnalyzer.Muons.fillMuonGenInfo = cms.untracked.bool(False)
    process.TestAnalyzer.Electrons.fillElectronGenInfo = cms.untracked.bool(False)
    process.TestAnalyzer.METFilters.bits = cms.InputTag('TriggerResults','','RECO')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
#from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

process.GlobalTag.globaltag = process.TestAnalyzer.globalTag

#==============================================================================================================================#

# Electron ID, following prescription in
# https://twiki.cern.ch/twiki/bin/viewauth/CMS/CutBasedElectronIdentificationRun2:
# set up everything that is needed to compute electron IDs and
# add the ValueMaps with ID decisions into the event data stream

# Load tools and function definitions
from PhysicsTools.SelectorUtils.tools.vid_id_tools import *

process.load("RecoEgamma.ElectronIdentification.egmGsfElectronIDs_cfi")

# Overwrite collection name
process.egmGsfElectronIDs.physicsObjectSrc = cms.InputTag('slimmedElectrons')

from PhysicsTools.SelectorUtils.centralIDRegistry import central_id_registry
process.egmGsfElectronIDSequence = cms.Sequence(process.egmGsfElectronIDs)

# Define which IDs we want to produce
my_id_modules = ['AnalysisTools.ObjectSelection.cutBasedElectronID_PHYS14_PU20bx25_V0_miniAOD_cff']

# Add them to the VID producer
if process.TestAnalyzer.Electrons.isFilled:
    for idmod in my_id_modules:
        setupAllVIDIdsInModule(process,idmod,setupVIDElectronSelection)

# Set ID tags
process.TestAnalyzer.Electrons.vetoId   = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-PHYS14-PU20bx25-V0-miniAOD-standalone-veto")
process.TestAnalyzer.Electrons.looseId  = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-PHYS14-PU20bx25-V0-miniAOD-standalone-loose")
process.TestAnalyzer.Electrons.mediumId = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-PHYS14-PU20bx25-V0-miniAOD-standalone-medium")
process.TestAnalyzer.Electrons.tightId  = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-PHYS14-PU20bx25-V0-miniAOD-standalone-tight")

#==============================================================================================================================#

# Photon ID, following prescription in
# https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedPhotonIdentificationRun2
#from AnalysisTools.ObjectSelection.vid_id_tools_photon import *

#process.load('AnalysisTools.ObjectSelection.egmPhotonIDs_cfi')
process.load("RecoEgamma.PhotonIdentification.egmPhotonIDs_cfi")

# Overwrite collection name
process.egmPhotonIDs.physicsObjectSrc = cms.InputTag('slimmedPhotons')

# Load the producer module to build full 5x5 cluster shapes and whatever 
# else is needed for IDs
process.load('AnalysisTools.ObjectSelection.PhotonIDValueMapProducer_cfi')

process.egmPhotonIDSequence = cms.Sequence(process.photonIDValueMapProducer * process.egmPhotonIDs)

#switchOnVIDPhotonIdProducer(process, DataFormat.MiniAOD)

# Define which IDs we want to produce
my_photon_id_modules = ['AnalysisTools.ObjectSelection.cutBasedPhotonID_PHYS14_PU20bx25_V2_cff']

# Add them to the VID producer
if process.TestAnalyzer.Photons.isFilled:
    for idmod in my_photon_id_modules:
        setupAllVIDIdsInModule(process,idmod,setupVIDPhotonSelection)

# Set ID tags
process.TestAnalyzer.Photons.vetoId     = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-PHYS14-PU20bx25-V2-standalone-veto")
process.TestAnalyzer.Photons.looseId    = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-PHYS14-PU20bx25-V2-standalone-loose")
process.TestAnalyzer.Photons.mediumId   = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-PHYS14-PU20bx25-V2-standalone-medium")
process.TestAnalyzer.Photons.tightId    = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-PHYS14-PU20bx25-V2-standalone-tight")

#==============================================================================================================================#

process.load('ObjectProducers.JetProducers.jet_producer_sequences_cfi')
process.load('ObjectProducers.JetProducers.jet_qgtagging_cfi')
process.load('ObjectProducers.LSFJetProducer.CfiFile_cfi')

process.load('Dummy.Puppi.Puppi_cff')
process.puppi.PuppiName      = cms.untracked.string("")

if ISDATA :
    process.redCA8.produceGen = cms.bool(False)
    process.redCA8.producePartonJets = cms.bool(False)

process.p = cms.Path(#process.puppi*
                     process.ak4PatAssocSeq           * 
                     #process.ak4PuppiJetSeq           * 
                     process.ca8JetsSeq               *
                     #process.lsfSubJets               *
                     process.egmGsfElectronIDSequence * 
                     process.egmPhotonIDSequence      *
                     #process.pickyJetSeq              *
                     #process.subjetscaJetSeq          *
                     process.QGTagger                 *
                     process.TestAnalyzer)

if ISDATA :
    process.p.remove(process.ak4PatAssocSeq)

if not process.TestAnalyzer.Electrons.isFilled :
    process.p.remove(process.egmGsfElectronIDSequence)

if not process.TestAnalyzer.Photons.isFilled :
    process.p.remove(process.egmPhotonIDSequence)
