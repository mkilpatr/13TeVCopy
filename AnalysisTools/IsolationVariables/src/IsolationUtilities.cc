/*
 * IsolationUtilities.cc
 *
 *  Created on: Sep 24, 2015
 *      Author: hqu
 */
#include "AnalysisTools/IsolationVariables/interface/IsolationUtilities.h"

Isolation::MiniPFIsoResult Isolation::miniPFIso(const pat::Electron& el, const pat::PackedCandidateCollection& pfcands) {
  using namespace ElectronIso;
  static MiniPFIsolation iso(MIN_CONESIZE, MAX_CONESIZE, KTSCALE, ACT_CONESIZE);
  iso.setDeadCone(DEADCONE_CH_EB, DEADCONE_NH_EB, DEADCONE_PH_EB, DEADCONE_PU_EB);
  iso.setDeadConeEE(DEADCONE_CH_EE, DEADCONE_NH_EE, DEADCONE_PH_EE, DEADCONE_PU_EE);
  iso.setObjectMinPt(MIN_PT);
  iso.setPtThreshold(PF_PT_THRESHOLD);

  iso.compute(el, pfcands);
  return Isolation::MiniPFIsoResult(iso.getMiniIso(), iso.getActivity());
}

Isolation::MiniPFIsoResult Isolation::miniPFIso(const pat::Muon& mu, const pat::PackedCandidateCollection& pfcands) {
  using namespace MuonIso;
  static MiniPFIsolation iso(MIN_CONESIZE, MAX_CONESIZE, KTSCALE, ACT_CONESIZE);
  iso.setDeadCone(DEADCONE_CH_EB, DEADCONE_NH_EB, DEADCONE_PH_EB, DEADCONE_PU_EB);
  iso.setDeadConeEE(DEADCONE_CH_EE, DEADCONE_NH_EE, DEADCONE_PH_EE, DEADCONE_PU_EE);
  iso.setObjectMinPt(MIN_PT);
  iso.setPtThreshold(PF_PT_THRESHOLD);

  iso.compute(mu, pfcands);
  return Isolation::MiniPFIsoResult(iso.getMiniIso(), iso.getActivity());
}

double Isolation::rhoIso(const pat::Electron& el, double rho){
  double EA = getEA(el.eta(), ElectronIso::EA_ETA, ElectronIso::EA_VALUE);
  auto iso = el.pfIsolationVariables();
  return calcRhoIso(iso.sumChargedHadronPt, iso.sumNeutralHadronEt, iso.sumPhotonEt, rho, EA);
}

double Isolation::rhoIso(const pat::Muon& mu, double rho){
  double EA = getEA(mu.eta(), MuonIso::EA_ETA, MuonIso::EA_VALUE);
  auto iso = mu.pfIsolationR04();
  return calcRhoIso(iso.sumChargedHadronPt, iso.sumNeutralHadronEt, iso.sumPhotonEt, rho, EA);
}
