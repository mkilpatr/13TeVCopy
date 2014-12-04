import FWCore.ParameterSet.Config as cms

from RecoJets.JetProducers.ak4PFJetsTrimmed_cfi import *

ak8PFJetsTrimmedr0p2ptf0p03 = ak4PFJetsTrimmed.clone(
    src           = cms.InputTag('packedPFCandidates'),
    rParam        = 0.8,
    jetPtMin      = 15.0,
    rFilt         = 0.2,
    trimPtFracMin = 0.03
)

ak8PFJetsTrimmedr0p2ptf0p05 = ak8PFJetsTrimmedr0p2ptf0p03.clone(trimPtFracMin = 0.05)
ak8PFJetsTrimmedr0p1ptf0p03 = ak8PFJetsTrimmedr0p2ptf0p03.clone(rFilt         = 0.1)
ak8PFJetsTrimmedr0p3ptf0p03 = ak8PFJetsTrimmedr0p2ptf0p03.clone(rFilt         = 0.3)

pfCHS = cms.EDFilter('CandPtrSelector',
    src = cms.InputTag('packedPFCandidates'),
    cut = cms.string('fromPV')
)

ak8PFJetsCHSTrimmedr0p2ptf0p03 = ak8PFJetsTrimmedr0p2ptf0p03.clone(src = cms.InputTag('pfCHS'))
ak8PFJetsCHSTrimmedr0p2ptf0p05 = ak8PFJetsTrimmedr0p2ptf0p05.clone(src = cms.InputTag('pfCHS'))
ak8PFJetsCHSTrimmedr0p1ptf0p03 = ak8PFJetsTrimmedr0p1ptf0p03.clone(src = cms.InputTag('pfCHS'))
ak8PFJetsCHSTrimmedr0p3ptf0p03 = ak8PFJetsTrimmedr0p3ptf0p03.clone(src = cms.InputTag('pfCHS'))

from ObjectProducers.Puppi.Puppi_cff import *

ak8PFJetsPuppiTrimmedr0p2ptf0p03 = ak8PFJetsTrimmedr0p2ptf0p03.clone(src = cms.InputTag('puppi','Puppi'))
ak8PFJetsPuppiTrimmedr0p2ptf0p05 = ak8PFJetsTrimmedr0p2ptf0p05.clone(src = cms.InputTag('puppi','Puppi'))
ak8PFJetsPuppiTrimmedr0p1ptf0p03 = ak8PFJetsTrimmedr0p1ptf0p03.clone(src = cms.InputTag('puppi','Puppi'))
ak8PFJetsPuppiTrimmedr0p3ptf0p03 = ak8PFJetsTrimmedr0p3ptf0p03.clone(src = cms.InputTag('puppi','Puppi'))

trimmedJetsCHSr0p2ptf0p03 = cms.Sequence(pfCHS * ak8PFJetsCHSTrimmedr0p2ptf0p03)
trimmedJetsCHSr0p2ptf0p05 = cms.Sequence(pfCHS * ak8PFJetsCHSTrimmedr0p2ptf0p05)
trimmedJetsCHSr0p1ptf0p03 = cms.Sequence(pfCHS * ak8PFJetsCHSTrimmedr0p1ptf0p03)
trimmedJetsCHSr0p3ptf0p03 = cms.Sequence(pfCHS * ak8PFJetsCHSTrimmedr0p3ptf0p03)

trimmedJetsPuppir0p2ptf0p03 = cms.Sequence(puppi * ak8PFJetsPuppiTrimmedr0p2ptf0p03)
trimmedJetsPuppir0p2ptf0p05 = cms.Sequence(puppi * ak8PFJetsPuppiTrimmedr0p2ptf0p05)
trimmedJetsPuppir0p1ptf0p03 = cms.Sequence(puppi * ak8PFJetsPuppiTrimmedr0p1ptf0p03)
trimmedJetsPuppir0p3ptf0p03 = cms.Sequence(puppi * ak8PFJetsPuppiTrimmedr0p3ptf0p03)
