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
MuonFiller::MuonFiller(const int options,
  const string branchName,
  const EventInfoFiller * evtInfoFiller,
  const edm::InputTag muonTag,
  const bool requireLoose,
  const double muptMin) :
  BaseFiller(options, branchName),
  evtInfoFiller_(evtInfoFiller),
  muonTag_(muonTag),
  requireLoose_(requireLoose),
  muptMin_(muptMin)
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
  iistight_      = data.addMulti<bool >(branchName_,"isTight",false);
  iispf_         = data.addMulti<bool >(branchName_,"isPF",false);
  iisglobal_     = data.addMulti<bool >(branchName_,"isGlobal",false);
  iistracker_    = data.addMulti<bool >(branchName_,"isTracker",false);
  iisstandalone_ = data.addMulti<bool >(branchName_,"isStandAlone",false);

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

}

//--------------------------------------------------------------------------------------------------
void MuonFiller::load(const edm::Event& iEvent)
{
  reset();
  FileUtilities::enforceGet(iEvent, muonTag_,muons_,true);
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
    data.fillMulti<float>(id0_, -1.*mu.muonBestTrack()->dxy(evtInfoFiller_->primaryVertex()));
    data.fillMulti<float>(idz_, mu.muonBestTrack()->dz(evtInfoFiller_->primaryVertex()));

    float dbiso = mu.pfIsolationR04().sumChargedHadronPt + max(0.0 , mu.pfIsolationR04().sumNeutralHadronEt + mu.pfIsolationR04().sumPhotonEt - 0.5 * mu.pfIsolationR04().sumPUPt);
    data.fillMulti<float>(ipfdbetaiso_, dbiso);

    data.fillMulti<bool >(iisloose_, mu.isLooseMuon());
    data.fillMulti<bool >(iistight_, mu.isTightMuon(reco::Vertex(evtInfoFiller_->primaryVertex(), reco::Vertex::Error())));
    data.fillMulti<bool >(iispf_, mu.isPFMuon());
    data.fillMulti<bool >(iisglobal_, mu.isGlobalMuon());
    data.fillMulti<bool >(iistracker_, mu.isTrackerMuon());
    data.fillMulti<bool >(iisstandalone_, mu.isStandAloneMuon());

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

  }
  isFilled_ = true;

}
