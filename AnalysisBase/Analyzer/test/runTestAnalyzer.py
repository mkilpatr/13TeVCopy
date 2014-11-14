import FWCore.ParameterSet.Config as cms

process = cms.Process("run")
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing("analysis")

options.outputFile = "evttree.root"
#options.inputFiles = "file:/uscms/home/nmccoll/nobackup/2011-04-15-susyra2/CMSSW_7_2_0_pre4/src/7E02B380-1528-E411-907B-00248C55CC7F.root"
# options.inputFiles = "root://cmsxrootd.fnal.gov//store/cmst3/user/gpetrucc/miniAOD/v1/TT_Tune4C_13TeV-pythia8-tauola_PU_S14_PAT.root" # if running at CERN
options.inputFiles = ("root://xrootd.unl.edu///store/mc/Spring14miniaod/ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_POSTLS170_V5-v2/20000/0450EA80-C427-E411-8AD8-02163E008CCC.root","root://xrootd.unl.edu///store/mc/Spring14miniaod/ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_POSTLS170_V5-v2/20000/06103A79-C427-E411-92F1-02163E00F109.root","root://xrootd.unl.edu///store/mc/Spring14miniaod/ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_POSTLS170_V5-v2/20000/08287178-C427-E411-B3A8-02163E008EAB.root","root://xrootd.unl.edu///store/mc/Spring14miniaod/ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_POSTLS170_V5-v2/20000/0A183879-C427-E411-B5B9-02163E00F109.root","root://xrootd.unl.edu///store/mc/Spring14miniaod/ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_POSTLS170_V5-v2/20000/14BBC57A-C427-E411-AFDB-02163E008CE6.root")


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

process.TestAnalyzer.fillReGenJets = True
process.TestAnalyzer.minJetPt = 30.0

from ObjectProducers.JetProducers.redefined_jet_producers_cfi import *
process.redAK4 = redAK4
process.redAK4.ghostArea       = -1

from ObjectProducers.JetProducers.redefined_genjet_associator_cfi import *
process.redGenAssoc = redGenAssoc

process.p = cms.Path(process.redAK4 * process.redGenAssoc * process.TestAnalyzer)
