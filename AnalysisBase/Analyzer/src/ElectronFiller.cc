//--------------------------------------------------------------------------------------------------
// 
// ElectronFiller
// 
// Class to fill electron information in a TTree.
// 
// ElectronFiller.cc created on Thu Oct 16 12:21:38 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisBase/Analyzer/interface/ElectronFiller.h"

using namespace ucsbsusy;

//--------------------------------------------------------------------------------------------------
ElectronFiller::ElectronFiller(const edm::ParameterSet &cfg) :
  electronTag_(cfg.getParameter<edm::InputTag>("electrons")),
  vtxTag_(cfg.getParameter<edm::InputTag>("vertices")),
  eleptMin_(cfg.getUntrackedParameter<double>("minElectronPt", 5.)),
  printIds_(cfg.getUntrackedParameter<bool>("printElectronIDs", false)),
  fillIDVars_(cfg.getUntrackedParameter<bool>("fillElectronIDVars", false)),
  fillIsoVars_(cfg.getUntrackedParameter<bool>("fillElectronIsoVars", false))
{

}

//--------------------------------------------------------------------------------------------------
void ElectronFiller::book(TreeWriter& tW)
{
  tW.book("ele_pt", ele_pt_);
  tW.book("ele_eta", ele_eta_);
  tW.book("ele_SCeta", ele_SCeta_);
  tW.book("ele_phi", ele_phi_);
  tW.book("ele_mass", ele_mass_);
  tW.book("ele_r9", ele_r9_);
  tW.book("ele_d0", ele_d0_);
  tW.book("ele_dz", ele_dz_);
  if(fillIDVars_) {
    tW.book("ele_sigietaieta", ele_sigietaieta_);
    tW.book("ele_sigiphiiphi", ele_sigiphiiphi_);
    tW.book("ele_hOverE", ele_hOverE_);
    tW.book("ele_fBrem", ele_fBrem_);
    tW.book("ele_nExpHitsInner", ele_nExpHitsInner_);
    tW.book("ele_nLostHitsInner", ele_nLostHitsInner_);
    tW.book("ele_passConvVeto", ele_passConvVeto_);
  }
  if(fillIsoVars_) {
    tW.book("ele_trackiso", ele_trackiso_);
    tW.book("ele_ecaliso", ele_ecaliso_);
    tW.book("ele_hcaliso", ele_hcaliso_);
    tW.book("ele_pfchargediso", ele_pfchargediso_);
    tW.book("ele_pfneutraliso", ele_pfneutraliso_);
    tW.book("ele_pfphotoniso", ele_pfphotoniso_);
    tW.book("ele_pfpuiso", ele_pfpuiso_);
  }
  tW.book("ele_looseid", ele_looseid_);
  tW.book("ele_tightid", ele_tightid_);
  tW.book("ele_robustlooseid", ele_robustlooseid_);
  tW.book("ele_robusttightid", ele_robusttightid_);
}

//--------------------------------------------------------------------------------------------------
void ElectronFiller::reset()
{
  ele_pt_.resize(0);
  ele_eta_.resize(0);
  ele_SCeta_.resize(0);
  ele_phi_.resize(0);
  ele_mass_.resize(0);
  ele_r9_.resize(0);
  ele_d0_.resize(0);
  ele_dz_.resize(0);
  if(fillIDVars_) {
    ele_sigietaieta_.resize(0);
    ele_sigiphiiphi_.resize(0);
    ele_hOverE_.resize(0);
    ele_fBrem_.resize(0);
    ele_nExpHitsInner_.resize(0);
    ele_nLostHitsInner_.resize(0);
    ele_passConvVeto_.resize(0);
  }
  if(fillIsoVars_) {
    ele_trackiso_.resize(0);
    ele_ecaliso_.resize(0);
    ele_hcaliso_.resize(0);
    ele_pfchargediso_.resize(0);
    ele_pfneutraliso_.resize(0);
    ele_pfphotoniso_.resize(0);
    ele_pfpuiso_.resize(0);
  }
  ele_looseid_.resize(0);
  ele_tightid_.resize(0);
  ele_robustlooseid_.resize(0);
  ele_robusttightid_.resize(0);
}

//--------------------------------------------------------------------------------------------------
void ElectronFiller::load(edm::Event& iEvent, bool storeOnlyPtr, bool isMC )
{
  reset();
  enforceGet(iEvent, electronTag_,electrons_,true);
  // or just pass PV from EventInfoFiller to this class, that would be easier
  enforceGet(iEvent,vtxTag_,vertices_,true);
}

//--------------------------------------------------------------------------------------------------
void ElectronFiller::fill(TreeWriter& tW, const int& numAnalyzed)
{

  const reco::Vertex &PV = vertices_->front();

  for (const pat::Electron &el : *electrons_) {
    if (el.pt() < eleptMin_) continue;
    ele_pt_.push_back(el.pt());
    ele_eta_.push_back(el.eta());
    ele_SCeta_.push_back(el.superCluster()->eta());
    ele_phi_.push_back(el.phi());
    ele_mass_.push_back(el.mass());
    ele_r9_.push_back(el.r9());
    ele_d0_.push_back(-1.*el.gsfTrack()->dxy(PV.position())); // check if this is right!
    ele_dz_.push_back(el.gsfTrack()->dz(PV.position()));
    if(fillIDVars_) {
      ele_sigietaieta_.push_back(el.sigmaIetaIeta());
      ele_sigiphiiphi_.push_back(el.sigmaIphiIphi());
      ele_hOverE_.push_back(el.hadronicOverEm());
      ele_fBrem_.push_back(el.fbrem());
      ele_nExpHitsInner_.push_back(el.gsfTrack()->trackerExpectedHitsInner().numberOfHits());
      ele_nLostHitsInner_.push_back(el.gsfTrack()->trackerExpectedHitsInner().numberOfLostHits());
      ele_passConvVeto_.push_back(el.passConversionVeto());
    }
    if(fillIsoVars_) {
      ele_trackiso_.push_back(el.trackIso());
      ele_ecaliso_.push_back(el.ecalIso());
      ele_hcaliso_.push_back(el.hcalIso());
      // is this implementation correct?
      ele_pfchargediso_.push_back(el.pfIsolationVariables().sumChargedHadronPt);
      ele_pfneutraliso_.push_back(el.pfIsolationVariables().sumNeutralHadronEt);
      ele_pfphotoniso_.push_back(el.pfIsolationVariables().sumPhotonEt);
      ele_pfpuiso_.push_back(el.pfIsolationVariables().sumPUPt);
    }
    ele_looseid_.push_back(el.electronID("eidLoose"));
    ele_tightid_.push_back(el.electronID("eidTight"));
    ele_robustlooseid_.push_back(el.electronID("eidRobustLoose"));
    ele_robusttightid_.push_back(el.electronID("eidRobustTight"));
    if(printIds_) {
      for (std::vector<pat::Electron::IdPair>::const_iterator it = el.electronIDs().begin(), ed = el.electronIDs().end(); it != ed; ++it) {
      std::cout << "Electron ID name: " << it->first << "; value: " << it->second << endl;
      }
    }
  }

}
