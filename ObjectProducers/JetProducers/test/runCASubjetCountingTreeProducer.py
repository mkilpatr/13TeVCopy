import FWCore.ParameterSet.Config as cms

process = cms.Process('run')
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100

from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing('analysis')

options.outputFile = 'evttree.root'

options.inputFiles = (
    '/store/mc/Phys14DR/SMS-T2tt_2J_mStop-850_mLSP-100_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/563CD412-C16B-E411-ACE1-C4346BC8E730.root',
    '/store/mc/Phys14DR/SMS-T2tt_2J_mStop-850_mLSP-100_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/10000/D83EC823-A66B-E411-80CA-00266CFFC4C4.root',
    '/store/mc/Phys14DR/SMS-T2tt_2J_mStop-850_mLSP-100_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/10000/F414852E-A66B-E411-B879-1CC1DE046F78.root'
)

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

process.Analyzer = cms.EDFilter('CASubjetCountingTreeProducer',
  nominal_configuration,
  doSplitting = cms.bool(True),
  splittingAlgo = cms.string("Picky"),
  #somewhat optimized cuts
  mCut = cms.double(20.0),
  rMin = cms.double(0.10),
  yCut = cms.double(0.05),
  pickyMVAFileName    = cms.string("$CMSSW_BASE/src/data/pickyJets/pickymva_nopuppi_reco.root"),
  pickyMVAName        = cms.string("picky_nopuppi_reco_0"),
  beta                = cms.double(1),
  rCutoff             = cms.double(10000),
  nSubAxes            = cms.string("1passKT"),
  pickyMaxSplits      = cms.int32(4)
)

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.GlobalTag.globaltag = process.Analyzer.globalTag

process.load('AnalysisBase.Analyzer.eventfilters_cfi')

from ObjectProducers.JetProducers.jet_clustering_cfi import *
from ObjectProducers.JetProducers.redefined_genjet_associator_cfi import *
from ObjectProducers.JetProducers.jet_flavor_associator_cfi import *

process.ca10 = ak4Jets.clone(rParameter = cms.double(1.0), jetAlgorithm = cms.string('CambridgeAachen'))
process.ca10FlvAssoc = ak4FlvAssoc.clone(genJetsSrc = cms.InputTag('ca10','Gen'),
                                         recoJetSrc = cms.InputTag('ca10'),
                                         partonJetSrc = cms.InputTag('ca10','parton'),
                                         recoToGenTag = cms.InputTag('ca10','GenPtr'),
                                         recoToPartonTag = cms.InputTag('ca10','partonPtr'))

process.Analyzer.PickyJets.jets = cms.InputTag('ca10')
process.Analyzer.PickyJets.fillCustomBtagInfo = False
process.Analyzer.PickyJets.fillJetShapeInfo   = False
process.Analyzer.PickyJets.reGenJets          = cms.InputTag('ca10','Gen')
process.Analyzer.PickyJets.reGenJetAssoc      = cms.InputTag('ca10:GenPtr')
process.Analyzer.PickyJets.flvAssoc           = cms.InputTag('ca10FlvAssoc','flavors')

process.Analyzer.Electrons.isFilled = False
process.Analyzer.Muons.isFilled = False
process.Analyzer.Taus.isFilled = False
process.Analyzer.Photons.isFilled = False
process.Analyzer.PFCandidates.isFilled = False
process.Analyzer.Jets.isFilled = False
process.Analyzer.CASubJets.isFilled = False
process.Analyzer.CMSTops.isFilled = False
process.Analyzer.AK8FatJets.isFilled = False

process.p = cms.Path( 
                     process.hadronicTTBarEventFilter *
                     process.ca10   *
                     process.ca10FlvAssoc *
                     process.Analyzer
                     )
