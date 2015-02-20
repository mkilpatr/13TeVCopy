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

PFCandidateFiller::PFCandidateFiller(const int options,
  const string branchName,
  const EventInfoFiller * evtInfoFiller,
  const edm::InputTag pfCandTag,
  const edm::InputTag jetTag,
  const edm::InputTag tauTag,
  const double candptMin,
  const double candetaMax,
  const double taudiscMin,
  const string tauMVAFileName,
  const string tauMVAName) :
  BaseFiller(options, branchName),
  evtInfoFiller_(evtInfoFiller),
  pfCandTag_(pfCandTag),
  jetTag_(jetTag),
  tauTag_(tauTag),
  candptMin_(candptMin),
  candetaMax_(candetaMax),
  taudiscMin_(taudiscMin)
{

  string base = getenv("CMSSW_BASE") + string("/src/");

  tauMVA_ = new TauMVA(base+tauMVAFileName, tauMVAName);

  ipt_     = data.addMulti<float>(branchName_,"pt",0);
  ieta_    = data.addMulti<float>(branchName_,"eta",0);
  iphi_    = data.addMulti<float>(branchName_,"phi",0);
  imass_   = data.addMulti<float>(branchName_,"mass",0);
  iq_      = data.addMulti<int  >(branchName_,"q",0);
  ipdgid_  = data.addMulti<int  >(branchName_,"pdgid",0);
  id0_     = data.addMulti<float>(branchName_,"d0",0);
  idz_     = data.addMulti<float>(branchName_,"dz",0);
  ifromPV_ = data.addMulti<int  >(branchName_,"fromPV",-1);
  imt_     = data.addMulti<float>(branchName_,"mttrkplusphoton",0);
  ichiso0p1_     = data.addMulti<float>(branchName_,"chiso0p1",0);
  ichiso0p2_     = data.addMulti<float>(branchName_,"chiso0p2",0);
  ichiso0p3_     = data.addMulti<float>(branchName_,"chiso0p3",0);
  ichiso0p4_     = data.addMulti<float>(branchName_,"chiso0p4",0);
  itotiso0p1_    = data.addMulti<float>(branchName_,"totiso0p1",0);
  itotiso0p2_    = data.addMulti<float>(branchName_,"totiso0p2",0);
  itotiso0p3_    = data.addMulti<float>(branchName_,"totiso0p3",0);
  itotiso0p4_    = data.addMulti<float>(branchName_,"totiso0p4",0);
  inearestTrkDR_ = data.addMulti<float>(branchName_,"nearestTrkDR",0.0);
  icontJetIndex_ = data.addMulti<int  >(branchName_,"contJetIndex",-1);
  icontJetDR_    = data.addMulti<float>(branchName_,"contJetDR",-1.0);
  icontJetCSV_   = data.addMulti<float>(branchName_,"contJetCSV",-1.0);
  icontTauIndex_ = data.addMulti<int  >(branchName_,"contTauIndex",-1);
  itaudisc_      = data.addMulti<float>(branchName_,"taudisc",-10.0);
}

int PFCandidateFiller::getContainingJetIndex(const pat::PackedCandidate* pfc)
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

float PFCandidateFiller::computeMT(const pat::PackedCandidate* pfc)
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

  reco::Candidate::PolarLorentzVector candP4 = pfc->polarP4();
  if(photonInd > -1) candP4+=(*pfcands_)[photonInd].polarP4();
  pat::PackedCandidate* cand = new pat::PackedCandidate();
  cand->setP4(candP4);

  const pat::MET* met = evtInfoFiller_->met();

  return JetKinematics::transverseMass(*cand, *met);

}

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

void PFCandidateFiller::load(const edm::Event& iEvent)
{
  reset();
  FileUtilities::enforceGet(iEvent, pfCandTag_, pfcands_, true);
  FileUtilities::enforceGet(iEvent, jetTag_, jets_, true);
  FileUtilities::enforceGet(iEvent, tauTag_, taus_, true);
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

    float nearesttrkdr = getDRNearestTrack(pfc);

    int jetIndex = getContainingJetIndex(pfc);
    const pat::Jet* jet = jetIndex > -1 ? &jets_->at(jetIndex) : 0;
    bool jetmatch = jetIndex > -1 && jet->pt() > 30.0 && fabs(jet->eta()) < 2.4;

    float contjetdr = jetmatch ? PhysicsUtilities::deltaR(*pfc, *jet) : -1.0;
    float contjetcsv = jetmatch ? jet->bDiscriminator("combinedInclusiveSecondaryVertexV2BJetTags") : -1.0;

    float taumva = tauMVA_->evaluateMVA(pfc->pt(), pfc->eta(), pfc->dz(), chiso0p1, chiso0p2, chiso0p3, chiso0p4, totiso0p1, totiso0p2, totiso0p3, totiso0p4, nearesttrkdr, contjetdr, contjetcsv);

    if(taumva < taudiscMin_) continue;

    data.fillMulti<float>(ipt_, pfc->pt());
    data.fillMulti<float>(ieta_, pfc->eta());
    data.fillMulti<float>(iphi_, pfc->phi());
    data.fillMulti<float>(imass_, pfc->mass());
    data.fillMulti<int  >(iq_, pfc->charge());
    data.fillMulti<int  >(ipdgid_, pfc->pdgId());
    data.fillMulti<float>(id0_, -1.*pfc->dxy());
    data.fillMulti<float>(idz_, pfc->dz());
    data.fillMulti<int  >(ifromPV_, pfc->fromPV());
    data.fillMulti<float>(imt_, computeMT(pfc));
    data.fillMulti<float>(ichiso0p1_, chiso0p1);
    data.fillMulti<float>(ichiso0p2_, chiso0p2);
    data.fillMulti<float>(ichiso0p3_, chiso0p3);
    data.fillMulti<float>(ichiso0p4_, chiso0p4);
    data.fillMulti<float>(itotiso0p1_, totiso0p1);
    data.fillMulti<float>(itotiso0p2_, totiso0p2);
    data.fillMulti<float>(itotiso0p3_, totiso0p3);
    data.fillMulti<float>(itotiso0p4_, totiso0p4);
    data.fillMulti<float>(inearestTrkDR_, nearesttrkdr);
    data.fillMulti<int  >(icontJetIndex_, jetIndex);
    data.fillMulti<float>(icontJetDR_, contjetdr);
    data.fillMulti<float>(icontJetCSV_, contjetcsv);
    data.fillMulti<int  >(icontTauIndex_, getHPSTauIndex(ic));
    data.fillMulti<float>(itaudisc_, taumva);
  }

  isFilled_ = true;
}  
