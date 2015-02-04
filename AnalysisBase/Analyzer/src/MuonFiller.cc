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
  iMVAiso_       = data.addMulti<float>(branchName_,"MVAiso",0);
  iismedium_     = data.addMulti<bool >(branchName_,"isMedium",false);

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
  string base = getenv("CMSSW_BASE");
  string muonisomva=base+"/src/AnalysisTools/ObjectSelection/data/Muons/muon_sefsip3drhoiso_training.root_BDTG.weights.xml";
  muMVAiso = new LeptonMVA();
  muMVAiso->initialize(muonisomva);

}

//--------------------------------------------------------------------------------------------------
void MuonFiller::load(const edm::Event& iEvent)
{
  reset();
  FileUtilities::enforceGet(iEvent, muonTag_,muons_,true);
  iEvent.getByLabel("lsfSubJets","LSFJets3",lsfSubJets3);
  FileUtilities::enforceGet(iEvent,"fixedGridRhoFastjetAll",rho_,true);
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

    float lsf3Iso = 9.; float lsf3IsoDR = 9.;
    LorentzVector mu_;
    mu_ = mu.p4();
    LorentzVectorCollection lsfSubJets3_;
    for (LorentzVectorCollection::const_iterator jt = lsfSubJets3->begin(); jt != lsfSubJets3->end(); jt++) {
      LorentzVector tmpVec;
      tmpVec.SetPxPyPzE(jt->px(),jt->py(),jt->pz(),jt->energy());
      lsfSubJets3_.push_back(tmpVec);
    }
    calculateLSFIso(mu_,lsfSubJets3_,&lsf3Iso,&lsf3IsoDR);
    double rhoiso=calculateRhoIso(mu.eta(),mu.pfIsolationR04().sumChargedHadronPt,mu.pfIsolationR04().sumNeutralHadronEt,mu.pfIsolationR04().sumPhotonEt,*rho_);
    double sip3d=fabs(mu.dB(mu.PV3D) / mu.edB(mu.PV3D));
    data.fillMulti<float>(iMVAiso_,muMVAiso->evaluateMVA(mu.pt(), lsf3Iso, sip3d, rhoiso));
    data.fillMulti<bool >(iismedium_,mediumID(mu.isLooseMuon(), mu.pt() ,dbiso ,-1.*mu.muonBestTrack()->dxy(evtInfoFiller_->primaryVertex()) ,mu.muonBestTrack()->dz(evtInfoFiller_->primaryVertex()),mu.isGlobalMuon() ,mu.globalTrack().isNonnull() ? mu.normChi2() : 0.0,  mu.combinedQuality().trkKink, mu.combinedQuality().chi2LocalPosition , mu.innerTrack().isNonnull() ? mu.innerTrack()->validFraction() : 0.0, mu.segmentCompatibility()));

  }
  isFilled_ = true;

}

void MuonFiller::calculateLSFIso(LorentzVector mu_,LorentzVectorCollection lsfSubJets_, float *lsfIso_, float *lsfIsoDR_) {

  float LSFDR_  = 999.;
  float LSF_ = -9.;
  for (LorentzVectorCollection::const_iterator jt = lsfSubJets_.begin(); jt != lsfSubJets_.end(); jt++) {
    LorentzVector jt_;
    jt_.SetPxPyPzE(jt->px(),jt->py(),jt->pz(),jt->e());
    float dRtmp_  = deltaR(mu_,jt_);
    if (dRtmp_<LSFDR_) {
      LSFDR_  = dRtmp_;
      LSF_ = mu_.pt()/jt->pt();
    }
  } // end of looping over the subjets jets

  *lsfIso_   = LSF_;
  *lsfIsoDR_ = LSFDR_;

}

float MuonFiller::calculateRhoIso(double eta, double pfchargediso, double pfneutraliso, double pfphotoniso, float rho) {

  double EA=0.1177;
  if(fabs(eta)<0.8) EA=0.0913;
  else if(fabs(eta)<1.3) EA=0.0765;
  else if(fabs(eta)<2.0) EA=0.0546;
  else if(fabs(eta)<2.3) EA=0.0728;
  return pfchargediso+TMath::Max(pfneutraliso+pfphotoniso-rho*EA,0.0);

}
bool MuonFiller::mediumID(bool isLoose, double pt , double pfdbetaiso, double d0, double dz, bool isGlobal, double nChi2, double trkKink, double chi2Local, double validFrac, double segComp) {

  if(isLoose==kFALSE) return kFALSE;
  if(pfdbetaiso/pt>0.4) return kFALSE;
  if(fabs(d0)>0.05)  return kFALSE;
  if(fabs(dz)>0.2)  return kFALSE;
  bool goodglobal=kFALSE;
  if(isGlobal && nChi2<3 && trkKink<20 && chi2Local<12) goodglobal=kTRUE;
  double segmentcut=0.451;
  if(goodglobal) segmentcut=0.303;
  if(!(validFrac>=0.8 && segComp>=segmentcut))  return kFALSE;
  return kTRUE;
}
