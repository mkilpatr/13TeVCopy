import FWCore.ParameterSet.Config as cms

process = cms.Process("run")

process.load("FWCore.MessageService.MessageLogger_cfi")

from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing("analysis")

options.outputFile = "evttree.root"
options.inputFiles = "file:////uscms/home/nmccoll/nobackup/2011-04-15-susyra2/CMSSW_7_2_0_pre4/src/7E02B380-1528-E411-907B-00248C55CC7F.root"
#options.inputFiles = "/store/cmst3/user/gpetrucc/miniAOD/v1/TT_Tune4C_13TeV-pythia8-tauola_PU_S14_PAT.root" # if running at CERN
options.maxEvents = -1

options.parseArguments()

process.TFileService = cms.Service("TFileService",
    fileName = cms.string(options.outputFile)
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring (options.inputFiles)
)

from AnalysisBase.Analyzer.analyzer_configuration_cfi import *
process.TestAnalyzer = cms.EDFilter('TestAnalyzer',
  nominal_configuration
)

process.p = cms.Path(process.TestAnalyzer)
