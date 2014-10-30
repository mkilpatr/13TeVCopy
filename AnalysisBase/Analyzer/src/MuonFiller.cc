//--------------------------------------------------------------------------------------------------
// 
// MuonFiller
// 
// Muon filler.
// 
// MuonFiller.cc created on Fri Oct 17 12:11:09 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisBase/Analyzer/interface/MuonFiller.h"

using namespace ucsbsusy;

//--------------------------------------------------------------------------------------------------
MuonFiller::MuonFiller(const edm::ParameterSet &cfg) :
  muonTag_(cfg.getParameter<edm::InputTag>("muons")),
  vtxTag_(cfg.getParameter<edm::InputTag>("vertices")),
  genParticleTag_(cfg.getParameter<edm::InputTag>("packedGenParticles")),
  muptMin_(cfg.getUntrackedParameter<double>("minMuonPt", 5.)),
  requireLoose_(cfg.getUntrackedParameter<bool>("requireLooseMuon", true)),
  fillIDVars_(cfg.getUntrackedParameter<bool>("fillMuonIDVars", false)),
  fillIsoVars_(cfg.getUntrackedParameter<bool>("fillMuonIsoVars", false)),
  fillGenInfo_(cfg.getUntrackedParameter<bool>("fillMuonGenInfo",false))
{

}

//--------------------------------------------------------------------------------------------------
void MuonFiller::book(TreeWriter& tW)
{
  tW.book("mu_pt", mu_pt_);
  tW.book("mu_eta", mu_eta_);
  tW.book("mu_phi", mu_phi_);
  tW.book("mu_mass", mu_mass_);
  tW.book("mu_q", mu_q_);
  tW.book("mu_d0", mu_d0_);
  tW.book("mu_dz", mu_dz_);
  if(fillIDVars_) {
    tW.book("mu_nChi2", mu_nChi2_);
    tW.book("mu_nValidHits", mu_nValidHits_);
    tW.book("mu_nMatch", mu_nMatch_);
    tW.book("mu_nPixHits", mu_nPixHits_);
    tW.book("mu_nTrackLayers", mu_nTrackLayers_);
  }
  if(fillIsoVars_) {
    tW.book("mu_trackiso", mu_trackiso_);
    tW.book("mu_ecaliso", mu_ecaliso_);
    tW.book("mu_hcaliso", mu_hcaliso_);
    tW.book("mu_pfchargediso", mu_pfchargediso_);
    tW.book("mu_pfneutraliso", mu_pfneutraliso_);
    tW.book("mu_pfphotoniso", mu_pfphotoniso_);
    tW.book("mu_pfpuiso", mu_pfpuiso_);
  }
  tW.book("mu_pfdbetaiso", mu_pfdbetaiso_);
  tW.book("mu_isLoose", mu_isloose_);
  tW.book("mu_isTight", mu_istight_);
  tW.book("mu_isPF", mu_ispf_);
  tW.book("mu_isGlobal", mu_isglobal_);
  tW.book("mu_isTracker", mu_istracker_);
  tW.book("mu_isStandAlone", mu_isstandalone_);
  if(fillGenInfo_) {
    tW.book("mu_genpt", mu_genpt_);
    tW.book("mu_geneta", mu_geneta_);
    tW.book("mu_genphi", mu_genphi_);
    tW.book("mu_genmass", mu_genmass_);
    tW.book("mu_genpdgid", mu_genpdgid_);
    tW.book("mu_genstatus", mu_genstatus_);
    tW.book("mu_genmotherpdgid", mu_genmotherpdgid_);
    tW.book("mu_genmotherstatus", mu_genmotherstatus_);
  }
}

//--------------------------------------------------------------------------------------------------
void MuonFiller::reset()
{
  mu_pt_.resize(0);
  mu_eta_.resize(0);
  mu_phi_.resize(0);
  mu_mass_.resize(0);
  mu_q_.resize(0);
  mu_d0_.resize(0);
  mu_dz_.resize(0);
  if(fillIDVars_) {
    mu_nChi2_.resize(0);
    mu_nValidHits_.resize(0);
    mu_nMatch_.resize(0);
    mu_nPixHits_.resize(0);
    mu_nTrackLayers_.resize(0);
  }
  if(fillIsoVars_) {
    mu_trackiso_.resize(0);
    mu_ecaliso_.resize(0);
    mu_hcaliso_.resize(0);
    mu_pfchargediso_.resize(0);
    mu_pfneutraliso_.resize(0);
    mu_pfphotoniso_.resize(0);
    mu_pfpuiso_.resize(0);
  }
  mu_pfdbetaiso_.resize(0);
  mu_isloose_.resize(0);
  mu_istight_.resize(0);
  mu_ispf_.resize(0);
  mu_isglobal_.resize(0);
  mu_istracker_.resize(0);
  mu_isstandalone_.resize(0);
  if(fillGenInfo_) {
    mu_genpt_.resize(0);
    mu_geneta_.resize(0);
    mu_genphi_.resize(0);
    mu_genmass_.resize(0);
    mu_genpdgid_.resize(0);
    mu_genstatus_.resize(0);
    mu_genmotherpdgid_.resize(0);
    mu_genmotherstatus_.resize(0);
  }
}

//--------------------------------------------------------------------------------------------------
void MuonFiller::load(edm::Event& iEvent, bool storeOnlyPtr, bool isMC )
{
  reset();
  enforceGet(iEvent, muonTag_,muons_,true);
  // or just pass PV from EventInfoFiller to this class, that would be easier
  enforceGet(iEvent,vtxTag_,vertices_,true);

  fillGenInfo_ = fillGenInfo_ && isMC;
  if(fillGenInfo_) {
    enforceGet(iEvent,genParticleTag_,genParticles_,true);
  }
}

//--------------------------------------------------------------------------------------------------
void MuonFiller::fill(TreeWriter& tW, const int& numAnalyzed)
{

  const reco::Vertex &PV = vertices_->front();

  for (const pat::Muon &mu : *muons_) {
    if (mu.pt() < muptMin_) continue;
    if (requireLoose_ && !mu.isLooseMuon()) continue;
    mu_pt_.push_back(mu.pt());
    mu_eta_.push_back(mu.eta());
    mu_phi_.push_back(mu.phi());
    mu_mass_.push_back(mu.mass());
    mu_q_.push_back(mu.charge());
    mu_d0_.push_back(-1.*mu.muonBestTrack()->dxy(PV.position()));
    mu_dz_.push_back(mu.muonBestTrack()->dz(PV.position()));
    if(fillIDVars_) {
      if(mu.globalTrack().isNonnull()) {
	mu_nChi2_.push_back(mu.normChi2());
      } else {
	mu_nChi2_.push_back(0.0);
      }
      mu_nValidHits_.push_back(mu.numberOfValidHits());
      mu_nMatch_.push_back(mu.numberOfMatchedStations());
      if(mu.innerTrack().isNonnull()) {
	mu_nPixHits_.push_back(mu.innerTrack()->hitPattern().numberOfValidPixelHits());
	mu_nTrackLayers_.push_back(mu.innerTrack()->hitPattern().trackerLayersWithMeasurement());
      } else {
	mu_nPixHits_.push_back(0);
	mu_nTrackLayers_.push_back(0);
      }
    }
    if(fillIsoVars_) {
      mu_trackiso_.push_back(mu.trackIso());
      mu_ecaliso_.push_back(mu.ecalIso());
      mu_hcaliso_.push_back(mu.hcalIso());
      mu_pfchargediso_.push_back(mu.pfIsolationR04().sumChargedHadronPt);
      mu_pfneutraliso_.push_back(mu.pfIsolationR04().sumNeutralHadronEt);
      mu_pfphotoniso_.push_back(mu.pfIsolationR04().sumPhotonEt);
      mu_pfpuiso_.push_back(mu.pfIsolationR04().sumPUPt);
    }
    mu_pfdbetaiso_.push_back(mu.pfIsolationR04().sumChargedHadronPt + max(0.0 , mu.pfIsolationR04().sumNeutralHadronEt + mu.pfIsolationR04().sumPhotonEt - 0.5 * mu.pfIsolationR04().sumPUPt));
    mu_isloose_.push_back(mu.isLooseMuon());
    mu_istight_.push_back(mu.isTightMuon(PV));
    mu_ispf_.push_back(mu.isPFMuon());
    mu_isglobal_.push_back(mu.isGlobalMuon());
    mu_istracker_.push_back(mu.isTrackerMuon());
    mu_isstandalone_.push_back(mu.isStandAloneMuon());

    if(fillGenInfo_ && mu.genParticle() != nullptr) {
      mu_genpt_.push_back(mu.genParticle()->pt());
      mu_geneta_.push_back(mu.genParticle()->eta());
      mu_genphi_.push_back(mu.genParticle()->phi());
      mu_genmass_.push_back(mu.genParticle()->mass());
      mu_genpdgid_.push_back(mu.genParticle()->pdgId());
      mu_genstatus_.push_back(mu.genParticle()->status());
      const reco::Candidate* match = mu.genParticle();
      // check this!!
      bool foundmom = false;
      for(size_t j=0; j<genParticles_->size();j++){
	const reco::Candidate* mother = (*genParticles_)[j].mother(0);
	if(mother != nullptr && MCTruth::isAncestor(mother, match)) {
	  foundmom = true;
	  mu_genmotherpdgid_.push_back(mother->pdgId());
	  mu_genmotherstatus_.push_back(mother->status());
	  break;
	}
      }
      if(!foundmom) {
	mu_genmotherpdgid_.push_back(-99);
	mu_genmotherstatus_.push_back(-99);
      }
    } else {
      mu_genpt_.push_back(-99.0);
      mu_geneta_.push_back(-99.0);
      mu_genphi_.push_back(-99.0);
      mu_genmass_.push_back(-99.0);
      mu_genpdgid_.push_back(-99);
      mu_genstatus_.push_back(-99);
      mu_genmotherpdgid_.push_back(-99);
      mu_genmotherstatus_.push_back(-99);
    }

  }

}
