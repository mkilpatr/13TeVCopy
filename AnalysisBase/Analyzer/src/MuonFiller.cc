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
MuonFiller::MuonFiller(const edm::ParameterSet& cfg, edm::ConsumesCollector && cc, const int options, const string branchName, EventInfoFiller * evtInfoFiller) :
  BaseFiller(options, branchName),
  evtInfoFiller_(evtInfoFiller),
  muonToken_    (cc.consumes<pat::MuonCollection>           (cfg.getParameter<edm::InputTag>("muons"))),
  ca8jetToken_  (cc.consumes<reco::PFJetCollection>         (cfg.getParameter<edm::InputTag>("ca8jets"))),
  rhoToken_     (cc.consumes<double>                        (cfg.getParameter<edm::InputTag>("rho"))),
  jetToken_     (cc.consumes<pat::JetCollection>            (cfg.getParameter<edm::InputTag>("jets"))),
  pfcandToken_  (cc.consumes<pat::PackedCandidateCollection>(cfg.getParameter<edm::InputTag>("pfcands"))),
  requireLoose_ (cfg.getUntrackedParameter<bool>            ("requireLooseMuon")),
  muptMin_      (cfg.getUntrackedParameter<double>          ("minMuonPt"))
{

  ipt_           = data.addMulti<float>(branchName_,"pt",0);
  ieta_          = data.addMulti<float>(branchName_,"eta",0);
  iphi_          = data.addMulti<float>(branchName_,"phi",0);
  imass_         = data.addMulti<float>(branchName_,"mass",0);
  iq_            = data.addMulti<int  >(branchName_,"q",0);
  id0_           = data.addMulti<float>(branchName_,"d0",0);
  idz_           = data.addMulti<float>(branchName_,"dz",0);
  ipfdbetaiso_   = data.addMulti<float>(branchName_,"pfdbetaiso",0);
  iisloose_      = data.addMulti<bool >(branchName_,"isLoose",false);
  iismedium_     = data.addMulti<bool >(branchName_,"isMedium",false);
  iistight_      = data.addMulti<bool >(branchName_,"isTight",false);
  iispf_         = data.addMulti<bool >(branchName_,"isPF",false);
  iisglobal_     = data.addMulti<bool >(branchName_,"isGlobal",false);
  iistracker_    = data.addMulti<bool >(branchName_,"isTracker",false);
  iisstandalone_ = data.addMulti<bool >(branchName_,"isStandAlone",false);
  iMVAiso_       = data.addMulti<float>(branchName_,"MVAiso",0);
  iminiiso_      = data.addMulti<float>(branchName_,"miniiso",0);
  iannulus_      = data.addMulti<float>(branchName_,"annulus",0);
  iptrel_        = data.addMulti<float>(branchName_,"ptrel",0);
  iptratio_      = data.addMulti<float>(branchName_,"ptratio",0);
  isip3d_        = data.addMulti<float>(branchName_,"sip3d",0);
  irhoiso_       = data.addMulti<float>(branchName_,"rhoiso",0);
  iLSFIso_       = data.addMulti<float>(branchName_,"lsfIso",0);

  if(options_ & FILLIDVARS) {
    inChi2_        = data.addMulti<float>(branchName_,"nChi2",0);
    inValidHits_   = data.addMulti<int  >(branchName_,"nValidHits",0);
    inMatch_       = data.addMulti<int  >(branchName_,"nMatch",0);
    inPixHits_     = data.addMulti<int  >(branchName_,"nPixHits",0);
    inTrackLayers_ = data.addMulti<int  >(branchName_,"nTrackLayers",0);
  }

  if(options_ & FILLISOVARS) {
    itrackiso_     = data.addMulti<float>(branchName_,"trackiso",0);
    iecaliso_      = data.addMulti<float>(branchName_,"ecaliso",0);
    ihcaliso_      = data.addMulti<float>(branchName_,"hcaliso",0);
    ipfchargediso_ = data.addMulti<float>(branchName_,"pfchargediso",0);
    ipfneutraliso_ = data.addMulti<float>(branchName_,"pfneutraliso",0);
    ipfphotoniso_  = data.addMulti<float>(branchName_,"pfphotoniso",0);
    ipfpuiso_      = data.addMulti<float>(branchName_,"pfpuiso",0);
  }
 
  if(options_ & LOADGEN) {
    igenpt_           = data.addMulti<float>(branchName_, "gen_pt", 0);
    igeneta_          = data.addMulti<float>(branchName_, "gen_eta", 0);
    igenphi_          = data.addMulti<float>(branchName_, "gen_phi", 0);
    igenmass_         = data.addMulti<float>(branchName_, "gen_mass", 0);
    igenstatus_       = data.addMulti<int  >(branchName_, "gen_status", 0);
    igenpdgid_        = data.addMulti<int  >(branchName_, "gen_pdgid", 0);
    igenmotherstatus_ = data.addMulti<int  >(branchName_, "genmother_status", 0);
    igenmotherpdgid_  = data.addMulti<int  >(branchName_, "genmother_pdgid", 0);
  }

}

//--------------------------------------------------------------------------------------------------
void MuonFiller::load(const edm::Event& iEvent, const edm::EventSetup &iSetup)
{
  reset();
  iEvent.getByToken(muonToken_,muons_);
  iEvent.getByToken(ca8jetToken_,ca8jets_);
  iEvent.getByToken(rhoToken_,rho_);
  iEvent.getByToken(jetToken_,ak4jets_);
  iEvent.getByToken(pfcandToken_,pfcands_);
  isLoaded_ = true;
}

//--------------------------------------------------------------------------------------------------
void MuonFiller::fill()
{

  for (const pat::Muon &mu : *muons_) {
    if (mu.pt() < muptMin_) continue;
    if (requireLoose_ && !mu.isLooseMuon()) continue;

    data.fillMulti<float>(ipt_, mu.pt());
    data.fillMulti<float>(ieta_, mu.eta());
    data.fillMulti<float>(iphi_, mu.phi());
    data.fillMulti<float>(imass_, mu.mass());
    data.fillMulti<int  >(iq_, mu.charge());

    assert(evtInfoFiller_->isLoaded());
    data.fillMulti<float>(id0_, mu.innerTrack().isNonnull() ? -1.*mu.innerTrack()->dxy(evtInfoFiller_->primaryVertex()):0);
    data.fillMulti<float>(idz_, mu.innerTrack().isNonnull() ? mu.innerTrack()->dz(evtInfoFiller_->primaryVertex()):0);

    float dbiso = mu.pfIsolationR04().sumChargedHadronPt + max(0.0 , mu.pfIsolationR04().sumNeutralHadronEt + mu.pfIsolationR04().sumPhotonEt - 0.5 * mu.pfIsolationR04().sumPUPt);
    data.fillMulti<float>(ipfdbetaiso_, dbiso);

    data.fillMulti<bool >(iisloose_, mu.isLooseMuon());
    data.fillMulti<bool >(iismedium_,mu.isMediumMuon());
    data.fillMulti<bool >(iistight_, mu.isTightMuon(reco::Vertex(evtInfoFiller_->primaryVertex(), reco::Vertex::Error())));
    data.fillMulti<bool >(iispf_, mu.isPFMuon());
    data.fillMulti<bool >(iisglobal_, mu.isGlobalMuon());
    data.fillMulti<bool >(iistracker_, mu.isTrackerMuon());
    data.fillMulti<bool >(iisstandalone_, mu.isStandAloneMuon());

    double LSF = Isolation::LSF(mu, *ca8jets_);
    data.fillMulti<float>(iLSFIso_  ,LSF);
    double rhoiso=Isolation::rhoIso(mu, *rho_);
    data.fillMulti<float>(irhoiso_  ,rhoiso);
    double sip3d=fabs(mu.dB(mu.PV3D) / mu.edB(mu.PV3D));
    data.fillMulti<float>(isip3d_, sip3d);

    const auto& miniIsoRlt = Isolation::miniPFIso(mu, *pfcands_);
    data.fillMulti<float>(iminiiso_,miniIsoRlt.miniIso);
    data.fillMulti<float>(iannulus_,miniIsoRlt.activity);

    data.fillMulti<float>(iptrel_, Isolation::leptonPtRel(mu, *ak4jets_));
    data.fillMulti<float>(iptratio_,Isolation::leptonPtRatio(mu, *ak4jets_));

    if(options_ & FILLIDVARS) {
      data.fillMulti<float>(inChi2_, mu.globalTrack().isNonnull() ? mu.normChi2() : 0.0);
      data.fillMulti<int  >(inValidHits_, mu.numberOfValidHits());
      data.fillMulti<int  >(inMatch_, mu.numberOfMatchedStations());
      data.fillMulti<int  >(inPixHits_, mu.innerTrack().isNonnull() ? mu.innerTrack()->hitPattern().numberOfValidPixelHits() : 0);
      data.fillMulti<int  >(inTrackLayers_, mu.innerTrack().isNonnull() ? mu.innerTrack()->hitPattern().trackerLayersWithMeasurement() : 0);
    }

    if(options_ & FILLISOVARS) {
      data.fillMulti<float>(itrackiso_, mu.trackIso());
      data.fillMulti<float>(iecaliso_, mu.ecalIso());
      data.fillMulti<float>(ihcaliso_, mu.hcalIso());
      data.fillMulti<float>(ipfchargediso_, mu.pfIsolationR04().sumChargedHadronPt);
      data.fillMulti<float>(ipfneutraliso_, mu.pfIsolationR04().sumNeutralHadronEt);
      data.fillMulti<float>(ipfphotoniso_, mu.pfIsolationR04().sumPhotonEt);
      data.fillMulti<float>(ipfpuiso_, mu.pfIsolationR04().sumPUPt);
    }

    if(options_ & LOADGEN) {
      const reco::GenParticle* genMu = mu.genParticle();
      if(genMu) {
        data.fillMulti<float>(igenpt_, genMu->pt());
        data.fillMulti<float>(igeneta_, genMu->eta());
        data.fillMulti<float>(igenphi_, genMu->phi());
        data.fillMulti<float>(igenmass_, genMu->mass());
        data.fillMulti<int  >(igenstatus_, genMu->status());
        data.fillMulti<int  >(igenpdgid_, genMu->pdgId());
        if(genMu->numberOfMothers()) {
          const auto* mother = genMu->mother(0);
          data.fillMulti<int  >(igenmotherstatus_, mother->status());
          data.fillMulti<int  >(igenmotherpdgid_, mother->pdgId());
        } else {
          data.fillMulti<int  >(igenmotherstatus_, -1);
          data.fillMulti<int  >(igenmotherpdgid_, -1);
        }
      }
      else {
        data.fillMulti<float>(igenpt_, -1);
        data.fillMulti<float>(igeneta_, -1);
        data.fillMulti<float>(igenphi_, -1);
        data.fillMulti<float>(igenmass_, -1);
        data.fillMulti<int  >(igenstatus_, -1);
        data.fillMulti<int  >(igenpdgid_, -1);
        data.fillMulti<int  >(igenmotherstatus_, -1);
        data.fillMulti<int  >(igenmotherpdgid_, -1);
      }
    }

  }
  isFilled_ = true;

}
