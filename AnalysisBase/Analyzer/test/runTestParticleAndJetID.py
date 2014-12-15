import FWCore.ParameterSet.Config as cms

process = cms.Process("run")
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing("analysis")

options.outputFile = "evttree.root"
# options.inputFiles = "file:/uscms/home/nmccoll/nobackup/2011-04-15-susyra2/CMSSW_7_2_0_pre4/src/7E02B380-1528-E411-907B-00248C55CC7F.root"
options.inputFiles = (
                      'root://xrootd.unl.edu//store/mc/Phys14DR/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/00C90EFC-3074-E411-A845-002590DB9262.root'
#   'root://xrootd.unl.edu//store/mc/Phys14DR/TTZJets_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/188E610D-9871-E411-BABD-002481E15008.root'
)
# options.inputFiles = (
#    "root://xrootd.unl.edu//store/mc/Spring14miniaod/SMS-T2tt_2J_mStop-850_mLSP-100_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_PLS170_V7AN1-v1/00000/B07E9D6A-AF5B-E411-86D3-AC162DA87230.root",
#    "root://xrootd.unl.edu//store/mc/Spring14miniaod/SMS-T2tt_2J_mStop-850_mLSP-100_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_PLS170_V7AN1-v1/00000/C0FD1E46-AF5B-E411-965F-AC162DACC3E8.root",
#    "root://xrootd.unl.edu//store/mc/Spring14miniaod/SMS-T2tt_2J_mStop-850_mLSP-100_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_PLS170_V7AN1-v1/00000/C8F24342-AF5B-E411-8E7F-78E7D1E4B6E8.root",
# )          
options.maxEvents = -1

options.parseArguments()

process.TFileService = cms.Service("TFileService",
    fileName = cms.string(options.outputFile)
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring (options.inputFiles)
)


from AnalysisBase.Analyzer.analyzer_configuration_cfi import nominal_configuration
process.analyzer = cms.EDFilter('TestParticleAndJetID',
  nominal_configuration
)

process.load('ObjectProducers.JetProducers.jet_producer_sequences_cfi')

process.p = cms.Path(process.ak4PatAssocSeq  * process.analyzer)

