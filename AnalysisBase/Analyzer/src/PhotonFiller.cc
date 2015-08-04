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
  vetoIdToken_  (cc.consumes<edm::ValueMap<bool> > (cfg.getParameter<edm::InputTag>("vetoId"))),
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
	  ivetoid_     = data.addMulti<bool >(branchName_,"vetoid",false);
	  ilooseid_    = data.addMulti<bool >(branchName_,"looseid",false);
	  imediumid_   = data.addMulti<bool >(branchName_,"mediumid",false);
	  itightid_    = data.addMulti<bool >(branchName_,"tightid",false);
	  ipfdbetaiso_ = data.addMulti<float>(branchName_,"pfdbetaiso",0);

	  if(options_ & FILLIDVARS) {
	    isigietaieta_        = data.addMulti<float>(branchName_,"sigietaieta",0);
	    ipassElectronVeto_   = data.addMulti<bool >(branchName_,"passConvVeto",false);
	    ifull5x5sigietaieta_ = data.addMulti<float>(branchName_,"full5x5sigietaieta",0);
	    ihOverE_             = data.addMulti<float>(branchName_,"hOverE",0);
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
	    ipfchargedHadronIso_ = data.addMulti<float>(branchName_,"pfchargediso",0);
	    ipfneutralHadronIso_ = data.addMulti<float>(branchName_,"pfneutraliso",0);
	    ipfphotonIso_  = data.addMulti<float>(branchName_,"pfphotoniso",0);
	    ipfpuiso_      = data.addMulti<float>(branchName_,"pfpuiso",0);
	  }
}

//--------------------------------------------------------------------------------------------------
void PhotonFiller::load(const edm::Event& iEvent, const edm::EventSetup &iSetup)
{
  reset();
  iEvent.getByToken(photonToken_,photons_);
  iEvent.getByToken(vetoIdToken_,veto_id_decisions_);
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
    if(!pho->passElectronVeto()) continue;
    
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
      PFIsolation_struct FPR_out = remover.PFIsolation(pho->superCluster(),  VtxPtr);
      //float photonIsoFPRChargedVtx0 = FPR_out.chargediso_primvtx;//remover.PFIsolation("charged",it->superCluster(),0);
      //float photonIsoFPRCharged = FPR_out.chargediso;//remover.PFIsolation("charged",it->superCluster(),0);
      //float photonIsoFPRNeutral = FPR_out.neutraliso;//remover.PFIsolation("neutral",it->superCluster());
      //float photonIsoFPRPhoton  = FPR_out.photoniso;//remover.PFIsolation("photon",it->superCluster());
      
      photonIsoFPRRandomConeChargedVtx0 = FPR_out.chargediso_primvtx_rcone; //remover.RandomConeIsolation("charged",it->superCluster(),0);
      photonIsoFPRRandomConeNeutral = FPR_out.neutraliso_rcone; //remover.RandomConeIsolation("neutral",it->superCluster());
      photonIsoFPRRandomConePhoton  = FPR_out.photoniso_rcone; //remover.RandomConeIsolation("photon",it->superCluster());
      photonIsoFPRRandomConeEta     = FPR_out.eta_rcone;
      photonIsoFPRRandomConePhi     = FPR_out.phi_rcone; // if RC is not ok, everything is -999
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
    bool passveto = (*veto_id_decisions_)[ phoPtr ];
    bool passloose = (*loose_id_decisions_)[ phoPtr ];
    bool passmedium = (*medium_id_decisions_)[ phoPtr ];
    bool passtight = (*tight_id_decisions_)[ phoPtr ];
    data.fillMulti<bool >(ivetoid_, passveto);
    data.fillMulti<bool >(ilooseid_, passloose);
    data.fillMulti<bool >(imediumid_, passmedium);
    data.fillMulti<bool >(itightid_, passtight);
    if(options_ & FILLIDVARS) {
      data.fillMulti<float>(isigietaieta_, pho->sigmaIetaIeta());
      data.fillMulti<bool >(ipassElectronVeto_, pho->passElectronVeto());
      data.fillMulti<float>(ifull5x5sigietaieta_, pho->full5x5_sigmaIetaIeta());
      data.fillMulti<float>(ihOverE_, pho->hadronicOverEm());
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
      data.fillMulti<float>(ipfchargedHadronIso_, pho->chargedHadronIso());
      data.fillMulti<float>(ipfneutralHadronIso_, pho->neutralHadronIso());
      data.fillMulti<float>(ipfphotonIso_, pho->photonIso());
      data.fillMulti<float>(ipfpuiso_, pho->sumPUPt());
    }

  }
  isFilled_ = true;

}

float PhotonFiller::calculateRhoIso(double eta, double pfiso, double rho, ISO_TYPE isotype) {
  // Effective Areas:
  // https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedPhotonIdentificationRun2
  const double EA[3][7] = {
      {0.0234, 0.0189, 0.0171, 0.0129, 0.0110, 0.0074, 0.0035}, // PFchargedHadronIso
      {0.0053, 0.0103, 0.0057, 0.0070, 0.0152, 0.0232, 0.1709}, // PFneutralHadronIso
      {0.0780, 0.0629, 0.0264, 0.0462, 0.0740, 0.0924, 0.1484}  // PFphotonIso
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

