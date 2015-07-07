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
  phoptMin_     (cfg.getUntrackedParameter<double> ("minPhotonPt"))
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

	  if(options_ & FILLIDVARS) {
	    isigietaieta_        = data.addMulti<float>(branchName_,"sigietaieta",0);
	    ipassElectronVeto_   = data.addMulti<bool >(branchName_,"passConvVeto",false);
      ifull5x5sigietaieta_ = data.addMulti<float>(branchName_,"full5x5sigietaieta",0);
      ihOverE_             = data.addMulti<float>(branchName_,"hOverE",0);
      irhoPFchargedHadronIso_ = data.addMulti<float>(branchName_,"rhoPFchargedHadronIso",0);
      irhoPFneutralHadronIso_ = data.addMulti<float>(branchName_,"rhoPFneutralHadronIso",0);
      irhoPFphotonIso_        = data.addMulti<float>(branchName_,"rhoPFphotonIso",0);
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
void PhotonFiller::load(const edm::Event& iEvent)
{
  reset();
  iEvent.getByToken(photonToken_,photons_);
  iEvent.getByToken(looseIdToken_,loose_id_decisions_);
  iEvent.getByToken(mediumIdToken_,medium_id_decisions_);
  iEvent.getByToken(tightIdToken_,tight_id_decisions_);
  iEvent.getByToken(rhoToken_,rho_);
  isLoaded_ = true;
}

//--------------------------------------------------------------------------------------------------
void PhotonFiller::fill()
{

  for (auto pho = photons_->begin(); pho != photons_->end(); pho++) {
    if (pho->pt() < phoptMin_) continue;

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

    if(options_ & FILLIDVARS) {
      data.fillMulti<float>(isigietaieta_, pho->sigmaIetaIeta());
      data.fillMulti<bool >(ipassElectronVeto_, pho->passElectronVeto());
      data.fillMulti<float>(ifull5x5sigietaieta_, pho->full5x5_sigmaIetaIeta());
      data.fillMulti<float>(ihOverE_, pho->hadronicOverEm());
      data.fillMulti<float>(irhoPFchargedHadronIso_, calculateRhoIso(pho->eta(), pho->chargedHadronIso(), *rho_, CHARGED));
      data.fillMulti<float>(irhoPFneutralHadronIso_, calculateRhoIso(pho->eta(), pho->neutralHadronIso(), *rho_, NEUTRAL));
      data.fillMulti<float>(irhoPFphotonIso_,        calculateRhoIso(pho->eta(), pho->photonIso(),        *rho_, PHOTON));
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

  return max(pfiso - rho*EA[isotype][bin], 0.);

}

