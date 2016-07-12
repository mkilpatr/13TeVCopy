//--------------------------------------------------------------------------------------------------
// 
// PFCandidateFiller
// 
// Class to fill PF candidate information.
// 
// PFCandidateFiller.cc created on Fri Jan 9 13:43:01 CET 2015 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisBase/Analyzer/interface/PFCandidateFiller.h"

using namespace ucsbsusy;

PFCandidateFiller::PFCandidateFiller(const edm::ParameterSet& cfg, edm::ConsumesCollector && cc, const int options, const string branchName, EventInfoFiller * evtInfoFiller) :
  BaseFiller(options, branchName),
  evtInfoFiller_(evtInfoFiller),
  pfCandToken_  (cc.consumes<pat::PackedCandidateCollection>(cfg.getParameter<edm::InputTag>("pfcands"))),
  jetToken_     (cc.consumes<pat::JetCollection>            (cfg.getParameter<edm::InputTag>("jets"))),
  tauToken_     (cc.consumes<pat::TauCollection>            (cfg.getParameter<edm::InputTag>("taus"))),
  candptMin_    (cfg.getUntrackedParameter<double>          ("minCandPt")),
  candetaMax_   (cfg.getUntrackedParameter<double>          ("maxCandEta")),
  taudiscMin_   (cfg.getUntrackedParameter<double>          ("minTauDisc"))
{

  string base = getenv("CMSSW_BASE") + string("/src/data/taus/");
  string tauMVAFileName_MtPresel = cfg.getUntrackedParameter<string>("tauMVAFileName_MtPresel");
//  string tauMVAFileName_DphiPresel = cfg.getUntrackedParameter<string>("tauMVAFileName_DphiPresel");
  string tauMVAName = cfg.getUntrackedParameter<string>("tauMVAName");

  tauMVA_MtPresel_   = new TauMVA(base+tauMVAFileName_MtPresel, tauMVAName);
//  tauMVA_DphiPresel_ = new TauMVA(base+tauMVAFileName_DphiPresel, tauMVAName);

  ipt_     = data.addMulti<float>(branchName_,"pt",0);
  ieta_    = data.addMulti<float>(branchName_,"eta",0);
  iphi_    = data.addMulti<float>(branchName_,"phi",0);
  imass_   = data.addMulti<float>(branchName_,"mass",0);
  iq_      = data.addMulti<int  >(branchName_,"q",0);
  ipdgid_  = data.addMulti<int  >(branchName_,"pdgid",0);
  id0_     = data.addMulti<float>(branchName_,"d0",0);
  idz_     = data.addMulti<float>(branchName_,"dz",0);
  ifromPV_ = data.addMulti<int  >(branchName_,"fromPV",-1);
  ichiso0p1_     = data.addMulti<float>(branchName_,"chiso0p1",0);
  ichiso0p2_     = data.addMulti<float>(branchName_,"chiso0p2",0);
  ichiso0p3_     = data.addMulti<float>(branchName_,"chiso0p3",0);
  ichiso0p4_     = data.addMulti<float>(branchName_,"chiso0p4",0);
  itotiso0p1_    = data.addMulti<float>(branchName_,"totiso0p1",0);
  itotiso0p2_    = data.addMulti<float>(branchName_,"totiso0p2",0);
  itotiso0p3_    = data.addMulti<float>(branchName_,"totiso0p3",0);
  itotiso0p4_    = data.addMulti<float>(branchName_,"totiso0p4",0);
  itrackiso_     = data.addMulti<float>(branchName_,"trackiso",0);
  inearphopt_    = data.addMulti<float>(branchName_,"nearphopt",0);
  inearphoeta_   = data.addMulti<float>(branchName_,"nearphoeta",0);
  inearphophi_   = data.addMulti<float>(branchName_,"nearphophi",0);
  inearestTrkDR_ = data.addMulti<float>(branchName_,"nearestTrkDR",0.0);
  icontJetIndex_ = data.addMulti<int  >(branchName_,"contJetIndex",-1);
  icontJetDR_    = data.addMulti<float>(branchName_,"contJetDR",-1.0);
  icontJetCSV_   = data.addMulti<float>(branchName_,"contJetCSV",-1.0);
  itaudisc_mtpresel_      = data.addMulti<float>(branchName_,"taudisc_mtpresel",-10.0);
  itaudisc_dphipresel_    = data.addMulti<float>(branchName_,"taudisc_dphipresel",-10.0);
}


int PFCandidateFiller::getContainingJetIndex(const pat::PackedCandidate* pfc) const
{

  int index = -1;

  for (unsigned int ijet = 0; ijet < jets_->size(); ijet++) {
    const pat::Jet &j = (*jets_)[ijet];
    for(unsigned int id = 0; id < j.numberOfDaughters(); id++) {
      const pat::PackedCandidate* dau = dynamic_cast<const pat::PackedCandidate*>(j.daughter(id));
      if(pfc == dau) {
        index = ijet;
        break;
      }
    }
  }


  return index;

}

int PFCandidateFiller::getHPSTauIndex(const unsigned int pfInd)
{

  int index = -1;

  for (unsigned int itau = 0; itau < taus_->size(); itau++) {
    const pat::Tau &t = (*taus_)[itau];
    for(unsigned int ic = 0; ic < t.signalCands().size(); ic++) {
      if(t.signalCands()[ic].key() == pfInd) {
        index = itau;
        break;
      }
    }
  }


  return index;

}

int PFCandidateFiller::getNearPhotonIndex(const pat::PackedCandidate* pfc)
{

  const double minPhotonPt = 0.5;
  const double maxPhotonDR = 0.2;

  int photonInd = -1;
  double maxPhotonPT = 0.0;

  for(unsigned int ic = 0; ic < pfcands_->size(); ic++) {
    const pat::PackedCandidate* c = &pfcands_->at(ic);
    if(!ParticleInfo::isA(ParticleInfo::p_gamma, c)) continue;
    if(c->pt() < minPhotonPt) continue;
    float dr = PhysicsUtilities::deltaR(c->eta(), c->phi(), pfc->eta(), pfc->phi());
    if(dr > maxPhotonDR) continue;
    if(c->pt() > maxPhotonPT) {
      maxPhotonPT = c->pt();
      photonInd = ic;
    }
  }

  return photonInd;

}

float PFCandidateFiller::getDRNearestTrack(const pat::PackedCandidate* particle, const float minTrackPt)
{

  float minDR = 10.0;

  for(unsigned int ic = 0; ic < pfcands_->size(); ic++) {
    const pat::PackedCandidate* cand = &pfcands_->at(ic);
    if(particle == cand) continue;

    if(cand->charge() == 0) continue;
    if(cand->pt() < minTrackPt) continue;

    const float dR = PhysicsUtilities::deltaR(*particle, *cand);
    if(dR > minDR) continue;
    minDR = dR;
  }

  return minDR;

}

float PFCandidateFiller::computeMT(const pat::PackedCandidate* pfc, const pat::MET* met)
{

  int photonInd = getNearPhotonIndex(pfc);

  reco::Candidate::PolarLorentzVector candP4 = pfc->polarP4();
  if(photonInd > -1) candP4+=(*pfcands_)[photonInd].polarP4();
  MomentumF* cand = new MomentumF(candP4);

  return JetKinematics::transverseMass(*cand, *met);

}

float PFCandidateFiller::TrackIso(const pat::PackedCandidate* particle, const float maxDR, const float deltaZCut)
{

  float absIso=0;

  for (unsigned int ipf = 0; ipf < pfcands_->size(); ipf++) {
    const pat::PackedCandidate* cand = &pfcands_->at(ipf);
    if(particle == cand) continue;
    if(cand->charge() == 0) continue; // skip neutrals
    const float dR = PhysicsUtilities::deltaR(particle->eta(), particle->phi(), cand->eta(), cand->phi());
    if(dR > maxDR) continue;
    if( cand->pt()>=0.0 && fabs(cand->dz()) < deltaZCut) absIso += cand->pt();
  }

  return absIso;
}

// next round: move isolation computations to IsolationVariables
float PFCandidateFiller::computePFIsolation(const pat::PackedCandidate* particle, const float minDR, const float maxDR, const unsigned int isotype, const float minNeutralPt, const float minPUPt, const float dBeta)
{

  float chargedIso = 0.0;
  float neutralIso = 0.0;
  float puIso = 0.0;

  for(unsigned int ic = 0; ic < pfcands_->size(); ic++) {
    const pat::PackedCandidate* cand = &pfcands_->at(ic);
    if(particle == cand) continue;

    const float dR = PhysicsUtilities::deltaR(particle->eta(), particle->phi(), cand->eta(), cand->phi());
    if(dR < minDR || dR > maxDR) continue;

    if(cand->charge() == 0) {
      if(cand->pt() > minNeutralPt) neutralIso += cand->pt();
    } else if(cand->fromPV() > 1) {
      chargedIso += cand->pt();
    } else {
      if(cand->pt() > minPUPt) puIso += cand->pt();
    }
  }

  float isolation = 0.0;

  switch(isotype) {
    case 0 : {
      isolation = chargedIso;
      break;
    }
    case 1 : {
      isolation = (chargedIso + std::max(float(0.0), (neutralIso - (dBeta*puIso))));
      break;
    }
    default : {
      printf("Isolation type not known!\n");
    }
  }

  return isolation;

}

void PFCandidateFiller::load(const edm::Event& iEvent, const edm::EventSetup &iSetup)
{
  reset();
  iEvent.getByToken(pfCandToken_, pfcands_);
  iEvent.getByToken(jetToken_, jets_);
  iEvent.getByToken(tauToken_, taus_);
  isLoaded_ = true;
}

void PFCandidateFiller::fill()
{
  for (unsigned int ic = 0; ic < pfcands_->size(); ic++) {
    const pat::PackedCandidate* pfc = &pfcands_->at(ic);
    if (pfc->pt() < candptMin_ || fabs(pfc->eta()) > candetaMax_) continue;

   if(!(options_ & SAVEALLCANDS) && !ParticleInfo::isA(ParticleInfo::p_piplus, pfc)) continue; // only save charged hadrons unless otherwise specified

    float chiso0p1 = computePFIsolation(pfc, 0.0, 0.1, 0);
    float chiso0p2 = computePFIsolation(pfc, 0.0, 0.2, 0);
    float chiso0p3 = computePFIsolation(pfc, 0.0, 0.3, 0);
    float chiso0p4 = computePFIsolation(pfc, 0.0, 0.4, 0);
    float totiso0p1 = computePFIsolation(pfc, 0.0, 0.1, 1);
    float totiso0p2 = computePFIsolation(pfc, 0.0, 0.2, 1);
    float totiso0p3 = computePFIsolation(pfc, 0.0, 0.3, 1);
    float totiso0p4 = computePFIsolation(pfc, 0.0, 0.4, 1);
    float trackiso = TrackIso(pfc,0.3,0.1);
    int photonIndex = getNearPhotonIndex(pfc);
    float nearesttrkdr = getDRNearestTrack(pfc);

    int jetIndex = getContainingJetIndex(pfc);
    const pat::Jet* jet = jetIndex > -1 ? &jets_->at(jetIndex) : 0;
    bool jetmatch = jetIndex > -1 && jet->pt() > 30.0 && fabs(jet->eta()) < 2.4;

    float contjetdr = jetmatch ? PhysicsUtilities::deltaR(*pfc, *jet) : -1.0;
    float contjetcsv = jetmatch ? jet->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags") : -1.0;

    float taumva_mtpresel   = tauMVA_MtPresel_  ->evaluateMVA(pfc->pt(), pfc->eta(), pfc->dz(), chiso0p1, chiso0p2, chiso0p3, chiso0p4, totiso0p1, totiso0p2, totiso0p3, totiso0p4, nearesttrkdr, contjetdr, contjetcsv);
//    float taumva_dphipresel = tauMVA_DphiPresel_->evaluateMVA(pfc->pt(), pfc->eta(), pfc->dz(), chiso0p1, chiso0p2, chiso0p3, chiso0p4, totiso0p1, totiso0p2, totiso0p3, totiso0p4, nearesttrkdr, contjetdr, contjetcsv);

    data.fillMulti<float>(ipt_, pfc->pt());
    data.fillMulti<float>(ieta_, pfc->eta());
    data.fillMulti<float>(iphi_, pfc->phi());
    data.fillMulti<float>(imass_, pfc->mass());
    data.fillMulti<int  >(iq_, pfc->charge());
    data.fillMulti<int  >(ipdgid_, pfc->pdgId());
    data.fillMulti<float>(id0_, -1.*pfc->dxy());
    data.fillMulti<float>(idz_, pfc->dz());
    data.fillMulti<int  >(ifromPV_, pfc->fromPV());
    data.fillMulti<float>(ichiso0p1_, chiso0p1);
    data.fillMulti<float>(ichiso0p2_, chiso0p2);
    data.fillMulti<float>(ichiso0p3_, chiso0p3);
    data.fillMulti<float>(ichiso0p4_, chiso0p4);
    data.fillMulti<float>(itotiso0p1_, totiso0p1);
    data.fillMulti<float>(itotiso0p2_, totiso0p2);
    data.fillMulti<float>(itotiso0p3_, totiso0p3);
    data.fillMulti<float>(itotiso0p4_, totiso0p4);
    data.fillMulti<float>(itrackiso_, trackiso);
    data.fillMulti<float>(inearphopt_, photonIndex > -1 ? pfcands_->at(photonIndex).pt() : -1.0);
    data.fillMulti<float>(inearphoeta_, photonIndex > -1 ? pfcands_->at(photonIndex).eta() : -1.0);
    data.fillMulti<float>(inearphophi_, photonIndex > -1 ? pfcands_->at(photonIndex).phi() : -1.0);
    data.fillMulti<float>(inearestTrkDR_, nearesttrkdr);
    data.fillMulti<int  >(icontJetIndex_, jetIndex);

    //Store in the tree jet pt -> 20 so that in the next round we can go that low
    jetmatch = jetIndex > -1 && jet->pt() >= 20.0 && fabs(jet->eta()) < 2.4;
    contjetdr = jetmatch ? PhysicsUtilities::deltaR(*pfc, *jet) : -1.0;
    contjetcsv = jetmatch ? jet->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags") : -1.0;


    data.fillMulti<float>(icontJetDR_, contjetdr);
    data.fillMulti<float>(icontJetCSV_, contjetcsv);
    data.fillMulti<float>(itaudisc_mtpresel_, taumva_mtpresel);
//    data.fillMulti<float>(itaudisc_dphipresel_, taumva_dphipresel);
  }

  isFilled_ = true;
}  
