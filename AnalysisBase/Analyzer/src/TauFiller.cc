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
TauFiller::TauFiller(const edm::ParameterSet &cfg) :
  tauTag_(cfg.getParameter<edm::InputTag>("taus")),
  vtxTag_(cfg.getParameter<edm::InputTag>("vertices")),
  tauptMin_(cfg.getUntrackedParameter<double>("minTauPt", 18.)),
  printIds_(cfg.getUntrackedParameter<bool>("printTauIDs", false)),
  fillRawDiscs_(cfg.getUntrackedParameter<bool>("fillRawTauDiscriminators", false))
{

  initTauIdNames();

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
void TauFiller::book(TreeWriter& tW)
{
  tW.book("tau_pt", tau_pt_);
  tW.book("tau_eta", tau_eta_);
  tW.book("tau_phi", tau_phi_);
  tW.book("tau_mass", tau_mass_);
  tW.book("tau_q", tau_q_);
  tW.book("tau_dxy", tau_dxy_);
  tW.book("tau_dxyerr", tau_dxyerr_);
  tW.book("tau_dxysig", tau_dxysig_);
  tW.book("tau_leadcand_pt", tau_leadcand_pt_);
  tW.book("tau_leadcand_eta", tau_leadcand_eta_);
  tW.book("tau_leadcand_phi", tau_leadcand_phi_);
  tW.book("tau_leadcand_mass", tau_leadcand_mass_);
  tW.book("tau_leadcand_q", tau_leadcand_q_);
  tW.book("tau_leadchargedcand_pt", tau_leadchargedcand_pt_);
  tW.book("tau_leadchargedcand_eta", tau_leadchargedcand_eta_);
  tW.book("tau_leadchargedcand_phi", tau_leadchargedcand_phi_);
  tW.book("tau_leadchargedcand_mass", tau_leadchargedcand_mass_);
  tW.book("tau_leadchargedcand_q", tau_leadchargedcand_q_);
  tW.book("tau_leadchargedcand_d0", tau_leadchargedcand_d0_);
  tW.book("tau_leadchargedcand_dz", tau_leadchargedcand_dz_);
  tW.book("tau_idflags", tau_idflags_);
  if(fillRawDiscs_) {
    tW.book("tau_isodb3hitsraw", tau_isodb3hitsraw_);
    tW.book("tau_isomvanoltraw", tau_isomvanoltraw_);
    tW.book("tau_isomvaltraw", tau_isomvaltraw_);
    tW.book("tau_antielemvaraw", tau_antielemvaraw_);
    tW.book("tau_antielemvacat", tau_antielemvacat_);
    tW.book("tau_antimumvaraw", tau_antimumvaraw_);
    tW.book("tau_antimumvacat", tau_antimumvacat_);
  }
}

//--------------------------------------------------------------------------------------------------
void TauFiller::reset()
{
  tau_pt_.resize(0);
  tau_eta_.resize(0);
  tau_phi_.resize(0);
  tau_mass_.resize(0);
  tau_q_.resize(0);
  tau_dxy_.resize(0);
  tau_dxyerr_.resize(0);
  tau_dxysig_.resize(0);
  tau_leadcand_pt_.resize(0);
  tau_leadcand_eta_.resize(0);
  tau_leadcand_phi_.resize(0);
  tau_leadcand_mass_.resize(0);
  tau_leadcand_q_.resize(0);
  tau_leadchargedcand_pt_.resize(0);
  tau_leadchargedcand_eta_.resize(0);
  tau_leadchargedcand_phi_.resize(0);
  tau_leadchargedcand_mass_.resize(0);
  tau_leadchargedcand_q_.resize(0);
  tau_leadchargedcand_d0_.resize(0);
  tau_leadchargedcand_dz_.resize(0);
  tau_idflags_.resize(0);
  if(fillRawDiscs_) {
    tau_isodb3hitsraw_.resize(0);
    tau_isomvanoltraw_.resize(0);
    tau_isomvaltraw_.resize(0);
    tau_antielemvaraw_.resize(0);
    tau_antielemvacat_.resize(0);
    tau_antimumvaraw_.resize(0);
    tau_antimumvacat_.resize(0);
  }
}

//--------------------------------------------------------------------------------------------------
void TauFiller::load(edm::Event& iEvent, bool storeOnlyPtr, bool isMC )
{
  reset();
  FileUtilities::enforceGet(iEvent, tauTag_,taus_,true);
  // or just pass PV from EventInfoFiller to this class, that would be easier
  FileUtilities::enforceGet(iEvent,vtxTag_,vertices_,true);
}

//--------------------------------------------------------------------------------------------------
void TauFiller::fill(TreeWriter& tW, const int& numAnalyzed)
{

  const reco::Vertex &PV = vertices_->front();

  for (const pat::Tau &tau : *taus_) {
    if (tau.pt() < tauptMin_) continue;
    tau_pt_.push_back(tau.pt());
    tau_eta_.push_back(tau.eta());
    tau_phi_.push_back(tau.phi());
    tau_mass_.push_back(tau.mass());
    tau_q_.push_back(tau.charge());
    tau_dxy_.push_back(tau.dxy());
    tau_dxyerr_.push_back(tau.dxy_error());
    tau_dxysig_.push_back(tau.dxy_Sig());
    tau_leadcand_pt_.push_back(tau.leadCand()->pt());
    tau_leadcand_eta_.push_back(tau.leadCand()->eta());
    tau_leadcand_phi_.push_back(tau.leadCand()->phi());
    tau_leadcand_mass_.push_back(tau.leadCand()->mass());
    tau_leadcand_q_.push_back(tau.leadCand()->charge());
    if(tau.leadChargedHadrCand().isNonnull()) {
      tau_leadchargedcand_pt_.push_back(tau.leadChargedHadrCand()->pt());
      tau_leadchargedcand_eta_.push_back(tau.leadChargedHadrCand()->eta());
      tau_leadchargedcand_phi_.push_back(tau.leadChargedHadrCand()->phi());
      tau_leadchargedcand_mass_.push_back(tau.leadChargedHadrCand()->mass());
      tau_leadchargedcand_q_.push_back(tau.leadChargedHadrCand()->charge());
      const reco::Track* leadtrk = 0;
      if(tau.leadTrack().isNonnull()) leadtrk = tau.leadTrack().get();
      // this is broken, need to figure out how this should be implemented
      tau_leadchargedcand_d0_.push_back(leadtrk ? -1.*leadtrk->dxy(PV.position()) : 0.0);
      tau_leadchargedcand_dz_.push_back(leadtrk ? leadtrk->dz(PV.position()) : 0.0);
    } else {
      tau_leadchargedcand_pt_.push_back(0.0);
      tau_leadchargedcand_eta_.push_back(0.0);
      tau_leadchargedcand_phi_.push_back(0.0);
      tau_leadchargedcand_mass_.push_back(0.0);
      tau_leadchargedcand_q_.push_back(0);
      tau_leadchargedcand_d0_.push_back(0.0);
      tau_leadchargedcand_dz_.push_back(0.0);
    }
    if(printIds_) {
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
    tau_idflags_.push_back(tauIdFlags);

    tau_isodb3hitsraw_.push_back(tau.isTauIDAvailable("byCombinedIsolationDeltaBetaCorrRaw3Hits") ? tau.tauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") : 0.0);
    tau_isomvanoltraw_.push_back(tau.isTauIDAvailable("byIsolationMVA3newDMwoLTraw") ? tau.tauID("byIsolationMVA3newDMwoLTraw") : 0.0);
    tau_isomvaltraw_.push_back(tau.isTauIDAvailable("byIsolationMVA3newDMwLTraw") ? tau.tauID("byIsolationMVA3newDMwLTraw") : 0.0);
    tau_antielemvaraw_.push_back(tau.isTauIDAvailable("againstElectronMVA5raw") ? tau.tauID("againstElectronMVA5raw") : 0.0);
    tau_antielemvacat_.push_back(tau.isTauIDAvailable("againstElectronMVA5category") ? tau.tauID("againstElectronMVA5category") : 0.0);
    tau_antimumvaraw_.push_back(tau.isTauIDAvailable("againstMuonMVAraw") ? tau.tauID("againstMuonMVAraw") : 0.0);
    tau_antimumvacat_.push_back(tau.isTauIDAvailable("againstMuonMVAcategory") ? tau.tauID("againstMuonMVAcategory") : 0.0);

  }

}
