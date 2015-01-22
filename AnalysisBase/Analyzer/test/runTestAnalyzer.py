import FWCore.ParameterSet.Config as cms

process = cms.Process('run')
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100

from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing('analysis')

options.outputFile = 'evttree_fullStat_QCD_PU20bx25.root'
#options.outputFile = 'evttree_lowStat_QCD_testoftest.root'
options.inputFiles = (
                       # Znunu 
                       #'root://xrootd.unl.edu//store/mc/Phys14DR/ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/000D3972-D973-E411-B12E-001E67398142.root',
                       #'root://xrootd.unl.edu//store/mc/Phys14DR/ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/02FB0226-A571-E411-8991-002590A88804.root',
                       #'root://xrootd.unl.edu//store/mc/Phys14DR/ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/046B6DB0-B072-E411-8229-002590A887F8.root',
                       #'root://xrootd.unl.edu//store/mc/Phys14DR/ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/0AC8495E-AA72-E411-AAAA-0025B3E06394.root',
                       #'root://xrootd.unl.edu//store/mc/Phys14DR/ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/0EDBA177-A972-E411-A314-001E67396CFC.root',
                       #'root://xrootd.unl.edu//store/mc/Phys14DR/ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/104EFABF-C171-E411-8E61-001E672CC1E7.root',
                       #'root://xrootd.unl.edu//store/mc/Phys14DR/ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/14F025FF-AB71-E411-A028-001E67397E13.root',
                       #'root://xrootd.unl.edu//store/mc/Phys14DR/ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/000D3972-D973-E411-B12E-001E67398142.root',
                       #'root://xrootd.unl.edu//store/mc/Phys14DR/ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/1E36A850-AB71-E411-9138-001E67397E54.root',
                       #'root://xrootd.unl.edu//store/mc/Phys14DR/ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/1E978152-AB71-E411-806C-001E67397238.root',
                       #'root://xrootd.unl.edu//store/mc/Phys14DR/ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/201A610B-D471-E411-B134-001E67396707.root',
                       #'root://xrootd.unl.edu//store/mc/Phys14DR/ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/22E495AE-C171-E411-AF3B-001E673973E1.root',
                       #'root://xrootd.unl.edu//store/mc/Phys14DR/ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/26309E58-AB71-E411-8F99-001E67396E0A.root',
                       #'root://xrootd.unl.edu//store/mc/Phys14DR/ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/2E825ABB-A571-E411-A94C-001E6739723D.root',
                       #'root://xrootd.unl.edu//store/mc/Phys14DR/ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/3807195B-AD72-E411-803C-002590A3711E.root',
                       #'root://xrootd.unl.edu//store/mc/Phys14DR/ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/3E54FF01-AF72-E411-B40A-002590A887F8.root',
                       #'root://xrootd.unl.edu//store/mc/Phys14DR/ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/4A6FB452-AB71-E411-8327-001E6739689C.root',
                       #'root://xrootd.unl.edu//store/mc/Phys14DR/ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/4C7B3482-AB72-E411-8DF7-002590A50046.root'
                       # QCD_HT  PU20  bx25      dataset=/QCD_HT*/Phys14DR-PU*bx*_PHYS14_*/MINIAODSIM
                       'root://xrootd.unl.edu//store/mc/Phys14DR/QCD_HT-500To1000_13TeV-madgraph/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/481A6155-916F-E411-BA52-00266CFFCAF0.root',
                       'root://xrootd.unl.edu//store/mc/Phys14DR/QCD_HT-500To1000_13TeV-madgraph/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/60893A5E-916F-E411-B2D9-00266CFBE43C.root',
                       'root://xrootd.unl.edu//store/mc/Phys14DR/QCD_HT-500To1000_13TeV-madgraph/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/7A37A45E-916F-E411-94F2-00266CFFBEB4.root',
                       'root://xrootd.unl.edu//store/mc/Phys14DR/QCD_HT-500To1000_13TeV-madgraph/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/7A67B658-916F-E411-AE98-1CC1DE1CED22.root',
                       'root://xrootd.unl.edu//store/mc/Phys14DR/QCD_HT-500To1000_13TeV-madgraph/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/827D2760-916F-E411-8F4C-AC162DABAF78.root',
                       'root://xrootd.unl.edu//store/mc/Phys14DR/QCD_HT-500To1000_13TeV-madgraph/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/86182C53-916F-E411-9DA7-00266CFFBE14.root',
                       'root://xrootd.unl.edu//store/mc/Phys14DR/QCD_HT-500To1000_13TeV-madgraph/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/CCE6D44F-916F-E411-A0E3-AC162DABBBA0.root',
                       'root://xrootd.unl.edu//store/mc/Phys14DR/QCD_HT-500To1000_13TeV-madgraph/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/E8DF5154-916F-E411-937A-00266CFFBF88.root',
                       'root://xrootd.unl.edu//store/mc/Phys14DR/QCD_HT-500To1000_13TeV-madgraph/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/10000/284DE32F-956F-E411-B06D-C4346BC8E730.root',
                       'root://xrootd.unl.edu//store/mc/Phys14DR/QCD_HT-500To1000_13TeV-madgraph/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/10000/2C7E1B90-956F-E411-82F1-1CC1DE056008.root'
                       )

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

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.GlobalTag.globaltag = process.TestAnalyzer.globalTag

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
process.TestAnalyzer.Jets.minJetPt = 20.0

process.TestAnalyzer.PuppiJets.isFilled = True                        # set to True if puppi jets are desired

from ObjectProducers.JetProducers.redefined_jet_producers_cfi import *
process.redAK4 = redAK4
process.redAK4.ghostArea = -1

from ObjectProducers.JetProducers.redefined_genjet_associator_cfi import *
process.redGenAssoc = redGenAssoc

process.p = cms.Path(process.redAK4 * process.redGenAssoc * process.egmGsfElectronIDSequence * process.TestAnalyzer)

# If producing puppi jets: 
if process.TestAnalyzer.PuppiJets.isFilled :
    process.load('Configuration.StandardSequences.Services_cff')
    process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
    process.GlobalTag.globaltag = process.TestAnalyzer.globalTag
    process.GlobalTag.toGet = cms.VPSet(
        cms.PSet(record = cms.string("JetCorrectionsRecord"),
                 tag = cms.string("JetCorrectorParametersCollection_CSA14_V4_MC_AK4PFchs"),
                 connect = cms.untracked.string("frontier://FrontierProd/CMS_CONDITIONS"),
                 label = cms.untracked.string("AK4PFchs")
        ),
    )

    process.load('ObjectProducers.Puppi.puppiJetProducer_cff')
    #process.p = cms.Path(process.redAK4 * process.redGenAssoc * process.puppiCorrJetSequence * process.egmGsfElectronIDSequence * process.TestAnalyzer)
    process.ak4PFJetsPuppi = redAK4Puppi
    process.ak4PFJetsPuppi.ghostArea = -1
    process.tempPuppiSeq = cms.Sequence(process.puppi * process.ak4PFJetsPuppi);
    process.TestAnalyzer.PuppiJets.jetsPuppi         = cms.InputTag('ak4PFJetsPuppi','')
    process.TestAnalyzer.PuppiJets.reGenJetsPuppi    = cms.InputTag('ak4PFJetsPuppi','Gen')
    process.TestAnalyzer.PuppiJets.fillJetGenInfo    = cms.untracked.bool(True)
    process.TestAnalyzer.PuppiJets.fillJetShapeInfo  = cms.untracked.bool(True)
    process.TestAnalyzer.PuppiJets.fillReGenJetsPuppi= cms.untracked.bool(True)
    process.p = cms.Path(process.redAK4 * process.redGenAssoc * process.tempPuppiSeq * process.egmGsfElectronIDSequence * process.TestAnalyzer)
    
process.TestAnalyzer.Electrons.tightId  = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-PHYS14-PU20bx25-V0-miniAOD-standalone-tight")


process.load('ObjectProducers.JetProducers.jet_producer_sequences_cfi')
process.load('ObjectProducers.Puppi.Puppi_cff')

process.load('ObjectProducers.LSFJetProducer.CfiFile_cfi')

process.p = cms.Path(process.puppi *
                     process.ak4PatAssocSeq * 
                     process.ak4PuppiJetSeq * 
                     process.ca8AssocSeq    *
                     process.lsfSubJets     *
                     process.egmGsfElectronIDSequence * 
                     process.pickyJetSeq    *
                     process.TestAnalyzer)

#process.lsfSubJets     *

# dont use for now
# # If producing puppi jets with JECs:
# if process.TestAnalyzer.PuppiJets.isFilled and process.TestAnalyzer.PuppiJets.applyJEC :
#     process.TestAnalyzer.PuppiJets.fillReGenJets = False                             # switch off redefined genjets for now
#  
#     process.p = cms.Path(process.ak4PatAssocSeq *
#                          process.ak4PuppiJetSeq * 
#                          process.trimmedJetSeq  *
#                          process.egmGsfElectronIDSequence * 
#                          process.TestAnalyzer)

