import FWCore.ParameterSet.Config as cms

process = cms.Process("run")

process.load("FWCore.MessageService.MessageLogger_cfi")

from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing("analysis")

options.outputFile = "evttree.root"
options.inputFiles = "root://xrootd.unl.edu//store/relval/CMSSW_7_2_0_pre3/RelValTTbar_13/MINIAODSIM/PU50ns_POSTLS172_V4-v1/00000/549EACFF-E41D-E411-8B8F-02163E00A081.root"
options.maxEvents = -1

options.parseArguments()

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring (options.inputFiles)
)

from AnalysisBase.Analyzer.analyzer_configuration_cfi import *
process.TestAnalyzer = cms.EDFilter('TestAnalyzer',
  nominal_configuration
)



process.p = cms.Path(process.TestAnalyzer)
