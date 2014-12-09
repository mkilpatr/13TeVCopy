import FWCore.ParameterSet.Config as cms

process = cms.Process('run')
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100

from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing('analysis')

options.outputFile = 'evttree.root'
options.inputFiles = '/store/mc/Phys14DR/ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/000D3972-D973-E411-B12E-001E67398142.root'

options.maxEvents = -1

options.parseArguments()

process.TFileService = cms.Service('TFileService',
    fileName = cms.string(options.outputFile)
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )

process.source = cms.Source('PoolSource',
    fileNames = cms.untracked.vstring (options.inputFiles)
)

from AnalysisBase.Analyzer.analyzer_configuration_cfi import nominal_configuration

process.TestAnalyzer = cms.EDFilter('TestAnalyzer',
  nominal_configuration
)

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
for idmod in my_id_modules:
    setupAllVIDIdsInModule(process,idmod,setupVIDElectronSelection)

# Set ID tags
process.TestAnalyzer.Electrons.vetoId   = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-PHYS14-PU20bx25-V0-miniAOD-standalone-veto")
process.TestAnalyzer.Electrons.looseId  = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-PHYS14-PU20bx25-V0-miniAOD-standalone-loose")
process.TestAnalyzer.Electrons.mediumId = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-PHYS14-PU20bx25-V0-miniAOD-standalone-medium")
process.TestAnalyzer.Electrons.tightId  = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-PHYS14-PU20bx25-V0-miniAOD-standalone-tight")

# Custom settings: jets
process.TestAnalyzer.Jets.fillReGenJets = True
process.TestAnalyzer.Jets.minJetPt      = 20.0

process.TestAnalyzer.TrimmedJets.isFilled = True                        # set to True if trimmed jets are desired
process.TestAnalyzer.PuppiJets.isFilled   = True                        # set to True if puppi jets are desired

process.load('ObjectProducers.JetProducers.redefined_jet_producers_cfi')
process.redAK4.ghostArea = -1

process.load('ObjectProducers.JetProducers.redefined_genjet_associator_cfi')

process.load('ObjectProducers.Puppi.puppiJetProducer_cff')
process.load('ObjectProducers.JetProducers.trimmedJets_cfi')

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.GlobalTag.globaltag = process.TestAnalyzer.globalTag

process.p = cms.Path(process.redAK4 * process.redGenAssoc * 
                     process.puppiJetFullSequence * process.redAK4Puppi * process.redPuppiGenAssoc * 
                     process.trimmedJetFullSequence * process.redAK8CHS * process.redAK8TrimmedGenAssoc * 
                     process.egmGsfElectronIDSequence * 
                     process.TestAnalyzer)

# If producing puppi jets with JECs: 
if process.TestAnalyzer.PuppiJets.isFilled and process.TestAnalyzer.PuppiJets.applyJEC :
    process.TestAnalyzer.PuppiJets.jets          = cms.InputTag('correctedAK4PFJetsPuppi')
    process.ak4PuppiImpactParameterTagInfos.jets = cms.InputTag('correctedAK4PFJetsPuppi')

    process.TestAnalyzer.PuppiJets.fillReGenJets = False                             # switch off redefined genjets for now
 
    process.p = cms.Path(process.redAK4 * process.redGenAssoc * 
                         process.puppiCorrJetFullSequence * 
                         process.trimmedJetFullSequence * process.redAK8CHS * process.redAK8TrimmedGenAssoc * 
                         process.egmGsfElectronIDSequence * 
                         process.TestAnalyzer)

