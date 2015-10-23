//--------------------------------------------------------------------------------------------------
// 
// TauFiller
// 
// Class to fill hadronic tau information in a TTree.
// 
// TauFiller.cc created on Wed Oct 22 14:12:52 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisBase/Analyzer/interface/TauFiller.h"

using namespace ucsbsusy;

//--------------------------------------------------------------------------------------------------
TauFiller::TauFiller(const edm::ParameterSet& cfg, edm::ConsumesCollector && cc, const int options, const string branchName, EventInfoFiller * evtInfoFiller) :
  BaseFiller(options, branchName),
  evtInfoFiller_(evtInfoFiller),
  tauToken_     (cc.consumes<pat::TauCollection>(cfg.getParameter<edm::InputTag>("taus"))),
  tauptMin_     (cfg.getUntrackedParameter<double>("minTauPt"))
{

  initTauIdNames();

  ipt_                   = data.addMulti<float>(branchName_,"pt",0);
  ieta_                  = data.addMulti<float>(branchName_,"eta",0);
  iphi_                  = data.addMulti<float>(branchName_,"phi",0);
  imass_                 = data.addMulti<float>(branchName_,"mass",0);
  iq_                    = data.addMulti<int  >(branchName_,"q",0);
  idxy_                  = data.addMulti<float>(branchName_,"dxy",0);
  idxyerr_               = data.addMulti<float>(branchName_,"dxyerr",0);
  idxysig_               = data.addMulti<float>(branchName_,"dxysig",0);
  iidflags_              = data.addMulti<unsigned long>(branchName_,"idflags",0);

  if(options_ & FILLCANDINFO) {
    ileadcand_pt_          = data.addMulti<float>(branchName_,"leadcand_pt",0);
    ileadcand_eta_         = data.addMulti<float>(branchName_,"leadcand_eta",0);
    ileadcand_phi_         = data.addMulti<float>(branchName_,"leadcand_phi",0);
    ileadcand_mass_        = data.addMulti<float>(branchName_,"leadcand_mass",0);
    ileadcand_q_           = data.addMulti<int  >(branchName_,"leadcand_q",0);
    ileadchargedcand_pt_   = data.addMulti<float>(branchName_,"leadchargedcand_pt",0);
    ileadchargedcand_eta_  = data.addMulti<float>(branchName_,"leadchargedcand_eta",0);
    ileadchargedcand_phi_  = data.addMulti<float>(branchName_,"leadchargedcand_phi",0);
    ileadchargedcand_mass_ = data.addMulti<float>(branchName_,"leadchargedcand_mass",0);
    ileadchargedcand_q_    = data.addMulti<int  >(branchName_,"leadchargedcand_q",0);
    ileadchargedcand_d0_   = data.addMulti<float>(branchName_,"leadchargedcand_d0",0);
    ileadchargedcand_dz_   = data.addMulti<float>(branchName_,"leadchargedcand_dz",0);
  }

  if(options_ & FILLRAWDISCS) {
    iisodb3hitsraw_ = data.addMulti<float>(branchName_,"isodb3hitsraw",0);
    iisomvanoltraw_ = data.addMulti<float>(branchName_,"isomvanoltraw",0);
    iisomvaltraw_   = data.addMulti<float>(branchName_,"isomvaltraw",0);
    iantielemvaraw_ = data.addMulti<float>(branchName_,"antielemvaraw",0);
    iantielemvacat_ = data.addMulti<int  >(branchName_,"antielemvacat",0);
    iantimumvaraw_  = data.addMulti<float>(branchName_,"antimumvaraw",0);
    iantimumvacat_  = data.addMulti<int  >(branchName_,"antimumvacat",0);
  }

}


//--------------------------------------------------------------------------------------------------
void TauFiller::initTauIdNames()
{

  hpsIds_.insert(pair<string, unsigned long>("decayModeFinding", kDecayMode));
  hpsIds_.insert(pair<string, unsigned long>("decayModeFindingnewDMs", kDecayModeNew));
  hpsIds_.insert(pair<string, unsigned long>("byVLooseCombinedIsolationDeltaBetaCorr3Hits", kVLooseIsoDB3Hits));
  hpsIds_.insert(pair<string, unsigned long>("byLooseCombinedIsolationDeltaBetaCorr3Hits", kLooseIsoDB3Hits));
  hpsIds_.insert(pair<string, unsigned long>("byMediumCombinedIsolationDeltaBetaCorr3Hits", kMediumIsoDB3Hits));
  hpsIds_.insert(pair<string, unsigned long>("byTightCombinedIsolationDeltaBetaCorr3Hits", kTightIsoDB3Hits));
  hpsIds_.insert(pair<string, unsigned long>("byVLooseIsolationMVA3newDMwoLT", kVLooseIsoMVAnoLT));
  hpsIds_.insert(pair<string, unsigned long>("byLooseIsolationMVA3newDMwoLT", kLooseIsoMVAnoLT));
  hpsIds_.insert(pair<string, unsigned long>("byMediumIsolationMVA3newDMwoLT", kMediumIsoMVAnoLT));
  hpsIds_.insert(pair<string, unsigned long>("byTightIsolationMVA3newDMwoLT", kTightIsoMVAnoLT));
  hpsIds_.insert(pair<string, unsigned long>("byVTightIsolationMVA3newDMwoLT", kVTightIsoMVAnoLT));
  hpsIds_.insert(pair<string, unsigned long>("byVVTightIsolationMVA3newDMwoLT", kVVTightIsoMVAnoLT));
  hpsIds_.insert(pair<string, unsigned long>("byVLooseIsolationMVA3newDMwLT", kVLooseIsoMVALT));
  hpsIds_.insert(pair<string, unsigned long>("byLooseIsolationMVA3newDMwLT", kLooseIsoMVALT));
  hpsIds_.insert(pair<string, unsigned long>("byMediumIsolationMVA3newDMwLT", kMediumIsoMVALT));
  hpsIds_.insert(pair<string, unsigned long>("byTightIsolationMVA3newDMwLT", kTightIsoMVALT));
  hpsIds_.insert(pair<string, unsigned long>("byVTightIsolationMVA3newDMwLT", kVTightIsoMVALT));
  hpsIds_.insert(pair<string, unsigned long>("byVVTightIsolationMVA3newDMwLT", kVVTightIsoMVALT));
  hpsIds_.insert(pair<string, unsigned long>("againstElectronLoose", kLooseEle));
  hpsIds_.insert(pair<string, unsigned long>("againstElectronMedium", kMediumEle));
  hpsIds_.insert(pair<string, unsigned long>("againstElectronTight", kTightEle));
  hpsIds_.insert(pair<string, unsigned long>("againstElectronVLooseMVA5", kVLooseEleMVA));
  hpsIds_.insert(pair<string, unsigned long>("againstElectronLooseMVA5", kLooseEleMVA));
  hpsIds_.insert(pair<string, unsigned long>("againstElectronMediumMVA5", kMediumEleMVA));
  hpsIds_.insert(pair<string, unsigned long>("againstElectronTightMVA5", kTightEleMVA));
  hpsIds_.insert(pair<string, unsigned long>("againstElectronVTightMVA5", kVTightEleMVA));
  hpsIds_.insert(pair<string, unsigned long>("againstMuonLoose", kLooseMu));
  hpsIds_.insert(pair<string, unsigned long>("againstMuonMedium", kMediumMu));
  hpsIds_.insert(pair<string, unsigned long>("againstMuonTight", kTightMu));
  hpsIds_.insert(pair<string, unsigned long>("againstMuonLoose2", kLooseMu2));
  hpsIds_.insert(pair<string, unsigned long>("againstMuonMedium2", kMediumMu2));
  hpsIds_.insert(pair<string, unsigned long>("againstMuonTight2", kTightMu2));
  hpsIds_.insert(pair<string, unsigned long>("againstMuonLoose3", kLooseMu3));
  hpsIds_.insert(pair<string, unsigned long>("againstMuonTight3", kTightMu3));
  hpsIds_.insert(pair<string, unsigned long>("againstMuonLooseMVA", kLooseMuMVA));
  hpsIds_.insert(pair<string, unsigned long>("againstMuonMediumMVA", kMediumMuMVA));
  hpsIds_.insert(pair<string, unsigned long>("againstMuonTightMVA", kTightMuMVA));

}

//--------------------------------------------------------------------------------------------------
void TauFiller::load(const edm::Event& iEvent, const edm::EventSetup &iSetup)
{
  reset();
  iEvent.getByToken(tauToken_,taus_);
  isLoaded_ = true;
}

//--------------------------------------------------------------------------------------------------
void TauFiller::fill()
{

  for (const pat::Tau &tau : *taus_) {
    if (tau.pt() < tauptMin_) continue;

    data.fillMulti<float>(ipt_, tau.pt());
    data.fillMulti<float>(ieta_, tau.eta());
    data.fillMulti<float>(iphi_, tau.phi());
    data.fillMulti<float>(imass_, tau.mass());
    data.fillMulti<int  >(iq_, tau.charge());
    data.fillMulti<float>(idxy_, tau.dxy());
    data.fillMulti<float>(idxyerr_, tau.dxy_error());
    data.fillMulti<float>(idxysig_, tau.dxy_Sig());
    if(options_ & FILLCANDINFO) {
      data.fillMulti<float>(ileadcand_pt_, tau.leadCand()->pt());
      data.fillMulti<float>(ileadcand_eta_, tau.leadCand()->eta());
      data.fillMulti<float>(ileadcand_phi_, tau.leadCand()->phi());
      data.fillMulti<float>(ileadcand_mass_, tau.leadCand()->mass());
      data.fillMulti<int  >(ileadcand_q_, tau.leadCand()->charge());
  
      if(tau.leadChargedHadrCand().isNonnull()) {
        data.fillMulti<float>(ileadchargedcand_pt_, tau.leadChargedHadrCand()->pt());
        data.fillMulti<float>(ileadchargedcand_eta_, tau.leadChargedHadrCand()->eta());
        data.fillMulti<float>(ileadchargedcand_phi_, tau.leadChargedHadrCand()->phi());
        data.fillMulti<float>(ileadchargedcand_mass_, tau.leadChargedHadrCand()->mass());
        data.fillMulti<int  >(ileadchargedcand_q_, tau.leadChargedHadrCand()->charge());
        const reco::Track* leadtrk = 0;
        if(tau.leadTrack().isNonnull()) leadtrk = tau.leadTrack().get();
        assert(evtInfoFiller_->isLoaded());
        // this is broken, need to figure out how this should be implemented
        data.fillMulti<float>(ileadchargedcand_d0_, leadtrk ? -1.*leadtrk->dxy(evtInfoFiller_->primaryVertex()) : 0.0);
        data.fillMulti<float>(ileadchargedcand_dz_, leadtrk ? leadtrk->dz(evtInfoFiller_->primaryVertex()) : 0.0);
      } else {
        data.fillMulti<float>(ileadchargedcand_pt_, 0.0);
        data.fillMulti<float>(ileadchargedcand_eta_, 0.0);
        data.fillMulti<float>(ileadchargedcand_phi_, 0.0);
        data.fillMulti<float>(ileadchargedcand_mass_, 0.0);
        data.fillMulti<int  >(ileadchargedcand_q_, 0.0);
        data.fillMulti<float>(ileadchargedcand_d0_, 0.0);
        data.fillMulti<float>(ileadchargedcand_dz_, 0.0);
      }
    }

    if(options_ & PRINTIDS) {
      for (vector<pat::Tau::IdPair>::const_iterator it = tau.tauIDs().begin(), ed = tau.tauIDs().end(); it != ed; ++it) {
      cout << "Tau ID name: " << it->first << "; value: " << it->second << endl;
      }
    }

    // HPS Tau ID Discriminators
    unsigned long tauIdFlags = 0;

    map<string, unsigned long>::iterator myit = hpsIds_.begin();

    for(; myit != hpsIds_.end(); ++myit) {
      if(tau.isTauIDAvailable(myit->first)) {
	if(tau.tauID(myit->first)) tauIdFlags |=  myit->second;
      }
    }
    data.fillMulti<unsigned long>(iidflags_, tauIdFlags);

    if(options_ & FILLRAWDISCS) {
      data.fillMulti<float>(iisodb3hitsraw_, tau.isTauIDAvailable("byCombinedIsolationDeltaBetaCorrRaw3Hits") ? tau.tauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") : 0.0);
      data.fillMulti<float>(iisomvanoltraw_, tau.isTauIDAvailable("byIsolationMVA3newDMwoLTraw") ? tau.tauID("byIsolationMVA3newDMwoLTraw") : 0.0);
      data.fillMulti<float>(iisomvaltraw_, tau.isTauIDAvailable("byIsolationMVA3newDMwLTraw") ? tau.tauID("byIsolationMVA3newDMwLTraw") : 0.0);
      data.fillMulti<float>(iantielemvaraw_, tau.isTauIDAvailable("againstElectronMVA5raw") ? tau.tauID("againstElectronMVA5raw") : 0.0);
      data.fillMulti<int  >(iantielemvacat_, tau.isTauIDAvailable("againstElectronMVA5category") ? tau.tauID("againstElectronMVA5category") : 0.0);
      data.fillMulti<float>(iantimumvaraw_, tau.isTauIDAvailable("againstMuonMVAraw") ? tau.tauID("againstMuonMVAraw") : 0.0);
      data.fillMulti<int  >(iantimumvacat_, tau.isTauIDAvailable("againstMuonMVAcategory") ? tau.tauID("againstMuonMVAcategory") : 0.0);
    }

  }
  isFilled_ = true;
}
