/*
 * PhotonFiller.cc
 *
 *  Created on: May 12, 2015
 *      Author: hqu
 */
#include "AnalysisBase/Analyzer/interface/PhotonFiller.h"

using namespace ucsbsusy;

//--------------------------------------------------------------------------------------------------
PhotonFiller::PhotonFiller(const edm::ParameterSet& cfg, edm::ConsumesCollector && cc, const int options, const string branchName) :
  BaseFiller(options, branchName),
  photonToken_  (cc.consumes<pat::PhotonCollection>(cfg.getParameter<edm::InputTag>("photons"))),
  looseIdToken_ (cc.consumes<edm::ValueMap<bool> > (cfg.getParameter<edm::InputTag>("looseId"))),
  mediumIdToken_(cc.consumes<edm::ValueMap<bool> > (cfg.getParameter<edm::InputTag>("mediumId"))),
  tightIdToken_ (cc.consumes<edm::ValueMap<bool> > (cfg.getParameter<edm::InputTag>("tightId"))),
  rhoToken_     (cc.consumes<double>               (cfg.getParameter<edm::InputTag>("rho"))),
  vtxToken_     (cc.consumes<reco::VertexCollection>(cfg.getParameter<edm::InputTag>("vertices"))),
  electronToken_ (cc.consumes<pat::ElectronCollection>(cfg.getParameter<edm::InputTag>("electrons"))),
  muonToken_     (cc.consumes<pat::MuonCollection>  (cfg.getParameter<edm::InputTag>("muons"))),
  jetToken_     (cc.consumes<pat::JetCollection>  (cfg.getParameter<edm::InputTag>("jets"))),
  pfcandidateToken_     (cc.consumes<pat::PackedCandidateCollection>  (cfg.getParameter<edm::InputTag>("pfcandidates"))),
  phoptMin_     (cfg.getUntrackedParameter<double> ("minPhotonPt")),
  phoptMinRC_     (cfg.getUntrackedParameter<double> ("minPhotonPtRC")),
  jetptMinRC_     (cfg.getUntrackedParameter<double> ("minJetPtRC")),
  leptonptMinRC_     (cfg.getUntrackedParameter<double> ("minLeptonPtRC"))
{
    ipt_         = data.addMulti<float>(branchName_,"pt",0);
    ieta_        = data.addMulti<float>(branchName_,"eta",0);
    iSCeta_      = data.addMulti<float>(branchName_,"SCeta",0);
    iphi_        = data.addMulti<float>(branchName_,"phi",0);
    imass_       = data.addMulti<float>(branchName_,"mass",0);
    ir9_         = data.addMulti<float>(branchName_,"r9",0);
    ilooseid_    = data.addMulti<bool >(branchName_,"looseid",false);
    imediumid_   = data.addMulti<bool >(branchName_,"mediumid",false);
    itightid_    = data.addMulti<bool >(branchName_,"tightid",false);
    ipfdbetaiso_ = data.addMulti<float>(branchName_,"pfdbetaiso",0);
    ipassElectronVeto_   = data.addMulti<bool >(branchName_,"passElectronVeto",false);
    ihasPixelSeed_       = data.addMulti<bool >(branchName_,"hasPixelSeed",false);
    isigietaieta_        = data.addMulti<float>(branchName_,"sigietaieta",0);
    ifull5x5sigietaieta_ = data.addMulti<float>(branchName_,"full5x5sigietaieta",0);
    ihOverE_             = data.addMulti<float>(branchName_,"hOverE",0);
    ipfchargedHadronIso_ = data.addMulti<float>(branchName_,"pfchargediso",0);
    ipfneutralHadronIso_ = data.addMulti<float>(branchName_,"pfneutraliso",0);
    ipfphotonIso_  = data.addMulti<float>(branchName_,"pfphotoniso",0);
    ipfpuiso_      = data.addMulti<float>(branchName_,"pfpuiso",0);

    if(options_ & FILLIDVARS) {
      irhoPFchargedHadronIso_ = data.addMulti<float>(branchName_,"rhoPFchargedHadronIso",0);
      irhoPFneutralHadronIso_ = data.addMulti<float>(branchName_,"rhoPFneutralHadronIso",0);
      irhoPFphotonIso_        = data.addMulti<float>(branchName_,"rhoPFphotonIso",0);
      ietaRC_                 = data.addMulti<float>(branchName_,"etaRC",0);
      iphiRC_                 = data.addMulti<float>(branchName_,"phiRC",0);
      irhoPFchargedHadronIsoRC_ = data.addMulti<float>(branchName_,"rhoPFchargedHadronIsoRC",0);
      irhoPFneutralHadronIsoRC_ = data.addMulti<float>(branchName_,"rhoPFneutralHadronIsoRC",0);
      irhoPFphotonIsoRC_        = data.addMulti<float>(branchName_,"rhoPFphotonIsoRC",0);
    }

    if(options_ & FILLISOVARS) {
      itrackiso_     = data.addMulti<float>(branchName_,"trackiso",0);
      iecaliso_      = data.addMulti<float>(branchName_,"ecaliso",0);
      ihcaliso_      = data.addMulti<float>(branchName_,"hcaliso",0);
    }
}

//--------------------------------------------------------------------------------------------------
void PhotonFiller::load(const edm::Event& iEvent, const edm::EventSetup &iSetup)
{
  reset();
  iEvent.getByToken(photonToken_,photons_);
  iEvent.getByToken(looseIdToken_,loose_id_decisions_);
  iEvent.getByToken(mediumIdToken_,medium_id_decisions_);
  iEvent.getByToken(tightIdToken_,tight_id_decisions_);
  iEvent.getByToken(rhoToken_,rho_);
  iEvent.getByToken(vtxToken_, vertices_);
  iEvent.getByToken(muonToken_,muons_);
  iEvent.getByToken(electronToken_, electrons_);
  iEvent.getByToken(jetToken_, jets_);
  iEvent.getByToken(pfcandidateToken_, pfcandidates_);

  edm::ESHandle<CaloGeometry> geometry ;
  iSetup.get<CaloGeometryRecord>().get(geometry);
  barrelGeometry = (CaloSubdetectorGeometry*)(geometry->getSubdetectorGeometry(DetId::Ecal, EcalBarrel));
  endcapGeometry = (CaloSubdetectorGeometry*)(geometry->getSubdetectorGeometry(DetId::Ecal, EcalEndcap));
  edm::ESHandle<MagneticField> magneticField;
  iSetup.get<IdealMagneticFieldRecord>().get(magneticField);
  magField = (MagneticField*)(magneticField.product());
  isLoaded_ = true;
}

//--------------------------------------------------------------------------------------------------
void PhotonFiller::fill()
{
  //bool hasgoodvtx = false;
  int primaryVertexIndex_ = 0;
  //if(vertices_->size() > 0){
    
  //if(!(*vertices_)[primaryVertexIndex_].isFake() && 
  //(*vertices_)[primaryVertexIndex_].ndof() > 4. &&
  //(*vertices_)[primaryVertexIndex_].position().Rho() <= 2.0 &&
  //fabs((*vertices_)[primaryVertexIndex_].position().Z())<=24.0)
  //hasgoodvtx = true;
  //}

  reco::VertexRef myVtxRef(vertices_, primaryVertexIndex_);
  edm::Ptr<reco::Vertex>  VtxPtr(vertices_,primaryVertexIndex_);

  
  for (auto pho = photons_->begin(); pho != photons_->end(); pho++) {
    if (pho->pt() < phoptMin_) continue;
    if(options_ & APPLYELEVETO && !pho->passElectronVeto()) continue;
    
    //MP ISO
    SuperClusterFootprintRemoval remover(electrons_,muons_,photons_,jets_,pfcandidates_,barrelGeometry,endcapGeometry,magField,phoptMinRC_,jetptMinRC_,leptonptMinRC_,edm::ParameterSet()); 
    
    float photonIsoFPRRandomConeChargedVtx0 = -99;
    float photonIsoFPRRandomConeNeutral = -99;
    float photonIsoFPRRandomConePhoton  = -99;
    float photonIsoFPRRandomConeEta     = -99;
    float photonIsoFPRRandomConePhi =-99;
    
    bool passminiaodpresel=false;

    if(pho->r9()>0.8 || pho->chargedHadronIso()<20 || pho->chargedHadronIso()<(pho->pt()*0.3 )){
      passminiaodpresel=true;
      int numberOfClusters =  pho->superCluster()->clusters().size();
      bool missing_clusters = false;
      if( numberOfClusters > 0 ) missing_clusters = !pho->superCluster()->clusters()[numberOfClusters-1].isAvailable();
      int numberOfPSClusters =  pho->superCluster()->preshowerClusters().size();
      bool missing_PSclusters = false;
      if( numberOfPSClusters > 0 ) missing_PSclusters = !pho->superCluster()->preshowerClusters()[numberOfPSClusters-1].isAvailable();
      if( !missing_clusters && !missing_PSclusters ) {
        PFIsolation_struct FPR_out = remover.PFIsolation(pho->superCluster(),  VtxPtr);
        photonIsoFPRRandomConeChargedVtx0 = FPR_out.chargediso_primvtx_rcone; 
        photonIsoFPRRandomConeNeutral = FPR_out.neutraliso_rcone; 
        photonIsoFPRRandomConePhoton  = FPR_out.photoniso_rcone; 
        photonIsoFPRRandomConeEta     = FPR_out.eta_rcone;
        photonIsoFPRRandomConePhi     = FPR_out.phi_rcone; // if RC is not ok, everything is 999
      }
    }
    data.fillMulti<float>(ipt_, pho->pt());
    data.fillMulti<float>(ieta_, pho->eta());
    data.fillMulti<float>(iSCeta_, pho->superCluster()->eta());
    data.fillMulti<float>(iphi_, pho->phi());
    data.fillMulti<float>(imass_, pho->mass());
    data.fillMulti<float>(ir9_, pho->r9());
    float dbiso = pho->chargedHadronIso() + max(0.0 , pho->neutralHadronIso() + pho->photonIso() - 0.5 * pho->sumPUPt());
    data.fillMulti<float>(ipfdbetaiso_, dbiso);

    const edm::Ptr<pat::Photon> phoPtr(photons_, pho - photons_->begin() );
    bool passloose = (*loose_id_decisions_)[ phoPtr ];
    bool passmedium = (*medium_id_decisions_)[ phoPtr ];
    bool passtight = (*tight_id_decisions_)[ phoPtr ];
    data.fillMulti<bool >(ilooseid_, passloose);
    data.fillMulti<bool >(imediumid_, passmedium);
    data.fillMulti<bool >(itightid_, passtight);
    data.fillMulti<bool >(ipassElectronVeto_, pho->passElectronVeto());
    data.fillMulti<bool >(ihasPixelSeed_, pho->hasPixelSeed());
    data.fillMulti<float>(isigietaieta_, pho->sigmaIetaIeta());
    data.fillMulti<float>(ifull5x5sigietaieta_, pho->full5x5_sigmaIetaIeta());
    data.fillMulti<float>(ihOverE_, pho->hadronicOverEm());
    data.fillMulti<float>(ipfchargedHadronIso_, pho->chargedHadronIso());
    data.fillMulti<float>(ipfneutralHadronIso_, pho->neutralHadronIso());
    data.fillMulti<float>(ipfphotonIso_, pho->photonIso());
    data.fillMulti<float>(ipfpuiso_, pho->sumPUPt());
    if(options_ & FILLIDVARS) {
      data.fillMulti<float>(irhoPFchargedHadronIso_, calculateRhoIso(pho->eta(), pho->chargedHadronIso(), *rho_, CHARGED));
      data.fillMulti<float>(irhoPFneutralHadronIso_, calculateRhoIso(pho->eta(), pho->neutralHadronIso(), *rho_, NEUTRAL));
      data.fillMulti<float>(irhoPFphotonIso_,        calculateRhoIso(pho->eta(), pho->photonIso(),        *rho_, PHOTON));
      data.fillMulti<float>(ietaRC_, photonIsoFPRRandomConeEta);
      data.fillMulti<float>(iphiRC_, photonIsoFPRRandomConePhi);
      if(passminiaodpresel){
        data.fillMulti<float>(irhoPFchargedHadronIsoRC_, calculateRhoIso(photonIsoFPRRandomConeEta, photonIsoFPRRandomConeChargedVtx0, *rho_, CHARGED));
        data.fillMulti<float>(irhoPFneutralHadronIsoRC_, calculateRhoIso(photonIsoFPRRandomConeEta, photonIsoFPRRandomConeNeutral, *rho_, NEUTRAL));
        data.fillMulti<float>(irhoPFphotonIsoRC_,        calculateRhoIso(photonIsoFPRRandomConeEta, photonIsoFPRRandomConePhoton, *rho_, PHOTON));
      }else{//if miniaod info for photons not saved, not a real good photon for RC -> put then default values of -99 everywhere
        data.fillMulti<float>(irhoPFchargedHadronIsoRC_, photonIsoFPRRandomConeEta);
        data.fillMulti<float>(irhoPFneutralHadronIsoRC_, photonIsoFPRRandomConeEta);
        data.fillMulti<float>(irhoPFphotonIsoRC_,        photonIsoFPRRandomConeEta);
      }
    }
    
    if(options_ & FILLISOVARS) {
      data.fillMulti<float>(itrackiso_, pho->trackIso());
      data.fillMulti<float>(iecaliso_, pho->ecalIso());
      data.fillMulti<float>(ihcaliso_, pho->hcalIso());
    }

  }
  isFilled_ = true;

}

float PhotonFiller::calculateRhoIso(double eta, double pfiso, double rho, ISO_TYPE isotype) {
  // Effective Areas:
  // https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedPhotonIdentificationRun2
  const double EA[3][7] = {
      {0.0157, 0.0143, 0.0115, 0.0094, 0.0095, 0.0068, 0.0053}, // PFchargedHadronIso
      {0.0143, 0.0210, 0.0148, 0.0082, 0.0124, 0.0186, 0.0320}, // PFneutralHadronIso
      {0.0725, 0.0604, 0.0320, 0.0512, 0.0766, 0.0949, 0.1160}  // PFphotonIso
  };

  int bin;
  if(fabs(eta)<1.0)         bin=0;
  else if (fabs(eta)<1.479) bin=1;
  else if (fabs(eta)<2.0)   bin=2;
  else if (fabs(eta)<2.2)   bin=3;
  else if (fabs(eta)<2.3)   bin=4;
  else if (fabs(eta)<2.4)   bin=5;
  else                      bin=6;
  return (pfiso - rho*EA[isotype][bin]);
  //we apply an upper cut in the isolation considerations later only, so negative values don't play any role
  //shape information on negative side important for template fits
}

