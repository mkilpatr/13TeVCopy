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
  iistight_      = data.addMulti<bool >(branchName_,"isTight",false);
  iispf_         = data.addMulti<bool >(branchName_,"isPF",false);
  iisglobal_     = data.addMulti<bool >(branchName_,"isGlobal",false);
  iistracker_    = data.addMulti<bool >(branchName_,"isTracker",false);
  iisstandalone_ = data.addMulti<bool >(branchName_,"isStandAlone",false);
  iMVAiso_       = data.addMulti<float>(branchName_,"MVAiso",0);
  iismedium_     = data.addMulti<bool >(branchName_,"isMedium",false);
  iminiiso_      = data.addMulti<float>(branchName_,"miniiso",0);
  iptrel_        = data.addMulti<float>(branchName_,"ptrel",0);
  iptratio_      = data.addMulti<float>(branchName_,"ptratio",0);
  isip3d_        = data.addMulti<float>(branchName_,"sip3d",0);

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
  iLSFIso_   = data.addMulti<float>(branchName_,"lsfIso",0);
  string base = getenv("CMSSW_BASE");
  string muonisomva=base+"/src/data/Muons/muon_sefsip3drhoiso_training.root_BDTG.weights.xml";
  muMVAiso = new LeptonMVA();
  muMVAiso->initialize(muonisomva);
 
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
void MuonFiller::load(const edm::Event& iEvent)
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

    LorentzVector mu_;
    mu_ = mu.p4();
    data.fillMulti<float>(iLSFIso_  ,LSF(mu_,ca8jets_));
    double rhoiso=calculateRhoIso(mu.eta(),mu.pfIsolationR04().sumChargedHadronPt,mu.pfIsolationR04().sumNeutralHadronEt,mu.pfIsolationR04().sumPhotonEt,*rho_);
    double sip3d=fabs(mu.dB(mu.PV3D) / mu.edB(mu.PV3D));
    data.fillMulti<float>(iMVAiso_,muMVAiso->evaluateMVA(mu.pt(), LSF(mu_,ca8jets_), sip3d, rhoiso));
    data.fillMulti<bool >(iismedium_,mediumID(mu.isLooseMuon(), mu.pt() ,dbiso ,mu.innerTrack().isNonnull() ? -1.*mu.innerTrack()->dxy(evtInfoFiller_->primaryVertex()):0 , mu.innerTrack().isNonnull() ? mu.innerTrack()->dz(evtInfoFiller_->primaryVertex()):0 ,mu.isGlobalMuon() ,mu.globalTrack().isNonnull() ? mu.normChi2() : 0.0,  mu.combinedQuality().trkKink, mu.combinedQuality().chi2LocalPosition , mu.innerTrack().isNonnull() ? mu.innerTrack()->validFraction() : 0.0, mu.segmentCompatibility()));
    data.fillMulti<float>(iminiiso_,getPFIsolation(pfcands_, mu, 0.05, 0.2, 10., false, false));
    data.fillMulti<float>(iptrel_,getLeptonPtRel( ak4jets_, mu ));
    data.fillMulti<float>(iptratio_,getLeptonPtRatio( ak4jets_, mu ));
    data.fillMulti<float>(isip3d_, sip3d);

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
  if(fabs(d0)>0.02)  return kFALSE;
  if(fabs(dz)>0.1)  return kFALSE;
  bool goodglobal=kFALSE;
  if(isGlobal && nChi2<3 && trkKink<20 && chi2Local<12) goodglobal=kTRUE;
  double segmentcut=0.451;
  if(goodglobal) segmentcut=0.303;
  if(!(validFrac>=0.8 && segComp>=segmentcut))  return kFALSE;
  return kTRUE;
}

double MuonFiller::getPFIsolation(edm::Handle<pat::PackedCandidateCollection> pfcands, const pat::Muon ptcl, double r_iso_min, double r_iso_max, double kt_scale, bool use_pfweight, bool charged_only) {
   if (ptcl.pt()<5.) return 99999.;
   double deadcone_nh(0.), deadcone_ch(0.), deadcone_ph(0.), deadcone_pu(0.);
   deadcone_ch = 0.0001; deadcone_pu = 0.01; deadcone_ph = 0.01;deadcone_nh = 0.01;
   double iso_nh(0.); double iso_ch(0.);
   double iso_ph(0.); double iso_pu(0.);
   double ptThresh(0.5);
   double r_iso = max(r_iso_min,min(r_iso_max, kt_scale/ptcl.pt()));
   for (const pat::PackedCandidate &pfc : *pfcands) {
     if (abs(pfc.pdgId())<7) continue;
     double dr = deltaR(pfc, ptcl);
     if (dr > r_iso) continue;
     ////////////////// NEUTRALS /////////////////////////                                                                                                                                                                                  
     if (pfc.charge()==0){
       if (pfc.pt()>ptThresh) {
         double wpf(1.);
         if (use_pfweight){
           double wpv(0.), wpu(0.);
           for (const pat::PackedCandidate &jpfc : *pfcands) {
             double jdr = deltaR(pfc, jpfc);
             if (pfc.charge()!=0 || jdr<0.00001) continue;
             double jpt = jpfc.pt();
             if (pfc.fromPV()>1) wpv *= jpt/jdr;
             else wpu *= jpt/jdr;
           }
           wpv = log(wpv);
           wpu = log(wpu);
           wpf = wpv/(wpv+wpu);
         }
         /////////// PHOTONS ////////////                                                                                                                                                                                                   
         if (abs(pfc.pdgId())==22) {
           if(dr < deadcone_ph) continue;
           iso_ph += wpf*pfc.pt();
           /////////// NEUTRAL HADRONS ////////////                                                                                                                                                                                         
         } else if (abs(pfc.pdgId())==130) {
           if(dr < deadcone_nh) continue;
           iso_nh += wpf*pfc.pt();
         }
       }
       ////////////////// CHARGED from PV /////////////////////////                                                                                                                                                                         
     } else if (pfc.fromPV()>1){
       if (abs(pfc.pdgId())==211) {
         if(dr < deadcone_ch) continue;
         iso_ch += pfc.pt();
       }
       ////////////////// CHARGED from PU /////////////////////////                                                                                                                                                                         
     } else {
       if (pfc.pt()>ptThresh){
         if(dr < deadcone_pu) continue;
         iso_pu += pfc.pt();
       }
     }
   }
   double iso(0.);
   if (charged_only){
     iso = iso_ch;
   } else {
     iso = iso_ph + iso_nh;
     if (!use_pfweight) iso -= 0.5*iso_pu;
     if (iso>0) iso += iso_ch;
     else iso = iso_ch;
   }
   iso = iso/ptcl.pt();
   return iso;
 }



double MuonFiller::getLeptonPtRel(edm::Handle<pat::JetCollection> jets, const pat::Muon lepton) {
  const pat::Jet *closestJet = 0;
  double minDR = 9999;
  for (const pat::Jet &j : *jets) {
    if (j.pt() < 20) continue;
    double tmpDR = deltaR(j.eta(),j.phi(),lepton.eta(),lepton.phi());
    if (tmpDR < minDR) {
      minDR = tmpDR;
      closestJet = &j;
    }
  }

  //if no jet was found nearby, return some large default value
  if (!closestJet) return 9999;
  TLorentzVector closestJetFourVector(closestJet->px(),closestJet->py(),closestJet->pz(),closestJet->energy());
  for (unsigned int i = 0, n = closestJet->numberOfSourceCandidatePtrs(); i < n; ++i) {
    const pat::PackedCandidate &candidate = dynamic_cast<const pat::PackedCandidate &>(*(closestJet->sourceCandidatePtr(i)));
    bool isPartOfLepton = false;
      // muon candidate pointers to the PF candidate is null in miniAOD.
      // we will match by relative pt difference and deltaR. thresholds at 0.1% and 0.001 in DR were tuned by eye
      if (abs(candidate.pdgId()) == 13
	  && fabs(candidate.pt() - lepton.pt()) / lepton.pt() < 0.001
	  && deltaR(candidate.eta() , candidate.phi(), lepton.eta() , lepton.phi()) < 0.001
	  ) isPartOfLepton = true;
    //if the PF candidate is part of the muon, subtract its momentum from the jet momentum
    if (isPartOfLepton) {
      closestJetFourVector.SetPxPyPzE( closestJetFourVector.Px() - candidate.px(),
				       closestJetFourVector.Py() - candidate.py(),
				       closestJetFourVector.Pz() - candidate.pz(),
				       closestJetFourVector.E() - candidate.energy());
    }
  }
  TLorentzVector lepFourVector(lepton.px(),lepton.py(),lepton.pz(),lepton.energy());
  return lepFourVector.Perp(closestJetFourVector.Vect());
}


double MuonFiller::getLeptonPtRatio(edm::Handle<pat::JetCollection> jets, const pat::Muon lepton) {
  const pat::Jet *closestJet = 0;
  double minDR = 9999;
  for (const pat::Jet &j : *jets) {
    if (j.pt() < 10) continue;
    double tmpDR = deltaR(j.eta(),j.phi(),lepton.eta(),lepton.phi());
    if (tmpDR < minDR) {
      minDR = tmpDR;
      closestJet = &j;
    }
  }

  //if no jet was found nearby, return some large default value
  if (!closestJet) return 9999;

  TLorentzVector closestJetP4(closestJet->px(),closestJet->py(),closestJet->pz(),closestJet->energy());
  TLorentzVector lepP4(lepton.px(),lepton.py(),lepton.pz(),lepton.energy());
  double lepOvJetPt = lepP4.Pt()/closestJetP4.Pt();

  return lepOvJetPt;
}


double MuonFiller::LSF(LorentzVector lep,edm::Handle<std::vector<reco::PFJet>> ca8jets) {
  double ptmin = 5.0;
  double deltaRjet=100;
  int jetnumber=0;
//Find closest jet
  for(unsigned int iJ = 0; iJ < ca8jets->size(); ++iJ){
    const reco::PFJet& j = ca8jets->at(iJ);
    if(j.pt() < ptmin) continue;
    LorentzVector jet;
    jet.SetPxPyPzE(j.px(),j.py(),j.pz(),j.energy());
    if(deltaR(lep,jet)<deltaRjet) {
      deltaRjet=deltaR(lep,jet);
      jetnumber=iJ;
    }
  }
  double deltaRsub=100;
  double subjetpt=-1;
  //Create subjets and find the closest one.
  if(deltaRjet<10){
    const reco::PFJet& jet = ca8jets->at(jetnumber);
    std::vector<fastjet::PseudoJet> FJparticles;
    for (unsigned int k = 0; k < jet.numberOfDaughters(); k++)
      {
        const pat::PackedCandidate* this_constituent = dynamic_cast<const pat::PackedCandidate*>(jet.daughter(k));
        FJparticles.push_back( fastjet::PseudoJet( this_constituent->px(), this_constituent->py(), this_constituent->pz(), this_constituent->energy() ) );
      }
    fastjet::JetDefinition jet_def_kt2(fastjet::kt_algorithm, 0.2);
    fastjet::ClusterSequence clust_seq_kt(FJparticles, jet_def_kt2);
    std::vector<fastjet::PseudoJet> subjets_kt = sorted_by_pt(clust_seq_kt.inclusive_jets(ptmin));
    int num_cust_subs_kt = subjets_kt.size();
    for(int iSJ=0;iSJ<num_cust_subs_kt;iSJ++){
      LorentzVector subjet;
      subjet.SetPxPyPzE(subjets_kt[iSJ].px(),subjets_kt[iSJ].py(),subjets_kt[iSJ].pz(),subjets_kt[iSJ].e());
      if(deltaR(lep,subjet)<deltaRsub) {
        deltaRsub=deltaR(lep,subjet);
        subjetpt=subjets_kt[iSJ].pt();
      }
    }
  }
  return lep.Pt()/subjetpt;
}

