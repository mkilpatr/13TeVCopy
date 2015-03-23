import FWCore.ParameterSet.Config as cms

from RecoJets.JetProducers.QGTagger_cfi import *
QGTagger.srcJets          = cms.InputTag('slimmedJets') 
QGTagger.jetsLabel        = cms.string('QGL_AK4PFchs')