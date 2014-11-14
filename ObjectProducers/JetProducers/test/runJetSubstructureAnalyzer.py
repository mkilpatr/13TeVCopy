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
    "root://eoscms.cern.ch//eos/cms/store/mc/Spring14miniaod/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_POSTLS170_V5-v2/00000/004C6DA7-FB03-E411-96BD-0025905A497A.root",
    "root://eoscms.cern.ch//eos/cms/store/mc/Spring14miniaod/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_POSTLS170_V5-v2/00000/00566B8C-FA03-E411-B790-0025905A60CE.root",
    "root://eoscms.cern.ch//eos/cms/store/mc/Spring14miniaod/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_POSTLS170_V5-v2/00000/00600DE1-F903-E411-98B8-0025905A608C.root",
    "root://eoscms.cern.ch//eos/cms/store/mc/Spring14miniaod/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_POSTLS170_V5-v2/00000/006F31B7-FA03-E411-A101-0025905A60D2.root",
    "root://eoscms.cern.ch//eos/cms/store/mc/Spring14miniaod/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_POSTLS170_V5-v2/00000/0088CFB8-FB03-E411-8995-002618943869.root",
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

SplittinessConfig = cms.PSet( mvaVsDirtyDecluster = cms.string("data/decluster_pruning_discriminator_bdt.root:MVAvsDirty_m_dij_z")
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


from AnalysisBase.Analyzer.analyzer_configuration_cfi import *
process.analyzer = cms.EDFilter('JetSubstructureAnalyzer',
  SplittinessConfig,
  nominal_configuration
)
process.analyzer.minJetPt          = 30.0

from ObjectProducers.JetProducers.redefined_jet_producers_cfi import *
process.redCA1 = redCA1
process.redCA1.producePU       = True
process.p = cms.Path(process.redCA1 *  process.analyzer)
