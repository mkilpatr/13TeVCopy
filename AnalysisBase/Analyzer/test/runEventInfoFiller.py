import FWCore.ParameterSet.Config as cms

process = cms.Process("run")

process.load("FWCore.MessageService.MessageLogger_cfi")

from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing("analysis")

options.outputFile = "evttree.root"
options.inputFiles = "/store/cmst3/user/gpetrucc/miniAOD/v1/TT_Tune4C_13TeV-pythia8-tauola_PU_S14_PAT.root"
options.maxEvents = -1

options.parseArguments()

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring (options.inputFiles)
)

process.load("AnalysisBase.Analyzer.EventInfoFiller_cfi")

process.EventInfoFiller.filename = cms.untracked.string(options.outputFile)

process.p = cms.Path(process.EventInfoFiller)
