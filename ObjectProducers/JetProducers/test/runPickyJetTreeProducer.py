import FWCore.ParameterSet.Config as cms

process = cms.Process('run')
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100

from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing('analysis')

options.outputFile = 'evttree.root'
# options.inputFiles = '/store/mc/Phys14DR/ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/000D3972-D973-E411-B12E-001E67398142.root'

options.inputFiles = (
#   'root://xrootd.unl.edu//store/mc/Phys14DR/TTZJets_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/188E610D-9871-E411-BABD-002481E15008.root'
#   'root://xrootd.unl.edu//store/mc/Phys14DR/SMS-T2tt_2J_mStop-850_mLSP-100_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/563CD412-C16B-E411-ACE1-C4346BC8E730.root'
  'root://xrootd.unl.edu//store/mc/Phys14DR/SMS-T2tt_2J_mStop-425_mLSP-325_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/305C0F68-516F-E411-BD5B-0025901D4932.root'
#   'root://xrootd.unl.edu//store/mc/Phys14DR/SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU40bx25_PHYS14_25_V1-v1/00000/2281F34C-8475-E411-9E7D-00259073E450.root'
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

process.TestAnalyzer = cms.EDFilter('PickyJetTreeProducer',
  nominal_configuration,
  runOnPUPPI = cms.untracked.bool(False),
  
    mvaVsDirtyDecluster = cms.string("data/decluster_pruning_discriminator_bdt.root:MVAvsDirty_m_dij_z")                            
  , mvaVsMixedDecluster = cms.string("data/decluster_splitting_discriminator_bdt.root:MVAvsMixed_m_t1_t2_t3_t4_dij_cE_sE")          
  , mvaVsDirtyNsubjetti = cms.string("data/nsubjettiness_pruning_discriminator_bdt.root:MVAvsDirty_pu2_pt2_hP_lP_c_dr")             
  , mvaVsMixedNsubjetti = cms.string("data/nsubjettiness_splitting_discriminator_bdt.root:MVAvsMixed_m_hP_lP_min_lL_hL_mL_t1_t2_dr")
  , mvaVsJunkSubjets    = cms.string("data/subjet_trimming_discriminator_bdt.root:MVAtoTrim_nSub_sDR_sM_rPT_pt")                    
  , partonSpreadFile    = cms.string("data/parton_spread_dr.root")                                                                  
  , puRhoDependence     = cms.string("data/rho_eta_dependence.root:pileUpDistribution")                                             
                                                                                                                                    
  , beta                = cms.double(1)                                                                                             
  , rCutoff             = cms.double(10000)                                                                                         
  , nSubAxes            = cms.string("1passKT")                                                                                     
  
)

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.GlobalTag.globaltag = process.TestAnalyzer.globalTag

process.load('AnalysisBase.Analyzer.hadronicttbareventfilter_cfi')

from ObjectProducers.JetProducers.jet_flavor_associator_cfi import *
process.CA1FlvAssoc         = redAK4FlvAssoc.clone(genJetsSrc       = cms.InputTag('CA1','Gen') , coneSize         = cms.double(1) )

process.TestAnalyzer.TrimmedJets.jets = "CA1"
process.TestAnalyzer.TrimmedJets.fillCustomBtagInfo = False
process.TestAnalyzer.TrimmedJets.reGenJets            = cms.InputTag('CA1','Gen')
process.TestAnalyzer.TrimmedJets.reGenJetAssoc        = cms.InputTag('CA1:GenPtr')
process.TestAnalyzer.TrimmedJets.flvAssoc             = cms.InputTag('CA1FlvAssoc','flavors')


from ObjectProducers.JetProducers.redefined_jet_producers_cfi import *
#no puppi
# process.CA1                 = redAK4.clone(rParameter = cms.double(1), jetAlgorithm    = cms.string('CambridgeAachen')  )
# process.p = cms.Path( 
#                      process.hadronicTTBarEventFilter *
#                       process.CA1   *
#                       process.CA1FlvAssoc *
#                       process.TestAnalyzer
#                      )
# puppi
process.load('ObjectProducers.Puppi.Puppi_cff')
process.CA1                 = redAK4Puppi.clone(rParameter = cms.double(1), jetAlgorithm    = cms.string('CambridgeAachen')  )
process.TestAnalyzer.runOnPUPPI = True
process.p = cms.Path(                      
                     process.hadronicTTBarEventFilter *
                     process.puppi *
                      process.CA1   *
                      process.CA1FlvAssoc *
                      process.TestAnalyzer
                     )
