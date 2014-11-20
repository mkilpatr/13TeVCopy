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
ElectronFiller::ElectronFiller(const int options,
  const string branchName,
  const EventInfoFiller * evtInfoFiller,
  const edm::InputTag electronTag,
  const int bunchSpacing,
  const double eleptMin) :
  BaseFiller(options, branchName),
  evtInfoFiller_(evtInfoFiller),
  electronTag_(electronTag),
  bunchSpacing_(bunchSpacing),
  eleptMin_(eleptMin)
{

  ipt_         = data.addMulti<float>(branchName_,"pt",0);
  ieta_        = data.addMulti<float>(branchName_,"eta",0);
  iSCeta_      = data.addMulti<float>(branchName_,"SCeta",0);
  iphi_        = data.addMulti<float>(branchName_,"phi",0);
  imass_       = data.addMulti<float>(branchName_,"mass",0);
  iq_          = data.addMulti<int  >(branchName_,"q",0);
  ir9_         = data.addMulti<float>(branchName_,"r9",0);
  id0_         = data.addMulti<float>(branchName_,"d0",0);
  idz_         = data.addMulti<float>(branchName_,"dz",0);
  ivetoid_     = data.addMulti<bool >(branchName_,"vetoid",false);
  ilooseid_    = data.addMulti<bool >(branchName_,"looseid",false);
  imediumid_   = data.addMulti<bool >(branchName_,"mediumid",false);
  itightid_    = data.addMulti<bool >(branchName_,"tightid",false);
  ipfdbetaiso_ = data.addMulti<float>(branchName_,"pfdbetaiso",0);

  if(options_ & FILLIDVARS) {
    iecalE_              = data.addMulti<float>(branchName_,"ecalE",0);
    ieOverPIn_           = data.addMulti<float>(branchName_,"eOverPIn",0);
    isigietaieta_        = data.addMulti<float>(branchName_,"sigietaieta",0);
    isigiphiiphi_        = data.addMulti<float>(branchName_,"sigiphiiphi",0);
    ifull5x5sigietaieta_ = data.addMulti<float>(branchName_,"full5x5sigietaieta",0);
    idEtaIn_             = data.addMulti<float>(branchName_,"dEtaIn",0);
    idPhiIn_             = data.addMulti<float>(branchName_,"dPhiIn",0);
    ihOverE_             = data.addMulti<float>(branchName_,"hOverE",0);
    ifBrem_              = data.addMulti<float>(branchName_,"fBrem",0);
    inExpHitsInner_      = data.addMulti<int  >(branchName_,"nExpHitsInner",0);
    inLostHitsInner_     = data.addMulti<int  >(branchName_,"nLostHitsInner",0);
    ipassConvVeto_       = data.addMulti<bool >(branchName_,"passConvVeto",false);
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

  if(options_ & FILLPOGMVA) {
    initMVA();
    eleIdCuts = new LeptonId(true, bunchSpacing_);
    imvaidnontrig_ = data.addMulti<float>(branchName,"mvaidnontrig",0);
    imvaidtrig_    = data.addMulti<float>(branchName,"mvaidtrig",0);
  } else {
    eleIdCuts = new LeptonId();
  }

}

//--------------------------------------------------------------------------------------------------
void ElectronFiller::initMVA()
{

  vector<string> eleNonTrigIDMVAWeights;
  vector<string> eleTrigIDMVAWeights;

  string base = getenv("CMSSW_BASE");

  if(bunchSpacing_ == 50) {
    eleNonTrigIDMVAWeights.push_back(base+"/src/AnalysisTools/ObjectSelection/data/CSA14/EIDmva_EB_5_50ns_BDT.weights.xml");
    eleNonTrigIDMVAWeights.push_back(base+"/src/AnalysisTools/ObjectSelection/data/CSA14/EIDmva_EE_5_50ns_BDT.weights.xml");
    eleNonTrigIDMVAWeights.push_back(base+"/src/AnalysisTools/ObjectSelection/data/CSA14/EIDmva_EB_10_50ns_BDT.weights.xml");
    eleNonTrigIDMVAWeights.push_back(base+"/src/AnalysisTools/ObjectSelection/data/CSA14/EIDmva_EE_10_50ns_BDT.weights.xml");
    eleTrigIDMVAWeights.push_back(base+"/src/AnalysisTools/ObjectSelection/data/CSA14/TrigIDMVA_50ns_EB_BDT.weights.xml");
    eleTrigIDMVAWeights.push_back(base+"/src/AnalysisTools/ObjectSelection/data/CSA14/TrigIDMVA_50ns_EE_BDT.weights.xml");
  } else {
    eleNonTrigIDMVAWeights.push_back(base+"/src/AnalysisTools/ObjectSelection/data/CSA14/EIDmva_EB_5_25ns_BDT.weights.xml");
    eleNonTrigIDMVAWeights.push_back(base+"/src/AnalysisTools/ObjectSelection/data/CSA14/EIDmva_EE_5_25ns_BDT.weights.xml");
    eleNonTrigIDMVAWeights.push_back(base+"/src/AnalysisTools/ObjectSelection/data/CSA14/EIDmva_EB_10_25ns_BDT.weights.xml");
    eleNonTrigIDMVAWeights.push_back(base+"/src/AnalysisTools/ObjectSelection/data/CSA14/EIDmva_EE_10_25ns_BDT.weights.xml");
    eleTrigIDMVAWeights.push_back(base+"/src/AnalysisTools/ObjectSelection/data/CSA14/TrigIDMVA_25ns_EB_BDT.weights.xml");
    eleTrigIDMVAWeights.push_back(base+"/src/AnalysisTools/ObjectSelection/data/CSA14/TrigIDMVA_25ns_EE_BDT.weights.xml");
  }
    
  eleMVANonTrig = new EGammaMvaEleEstimatorCSA14();
  eleMVANonTrig->initialize("BDT",
                          EGammaMvaEleEstimatorCSA14::kNonTrig,
                          true,
                          eleNonTrigIDMVAWeights);

  eleMVATrig = new EGammaMvaEleEstimatorCSA14();
  eleMVATrig->initialize("BDT",
                          EGammaMvaEleEstimatorCSA14::kTrig,
                          true,
                          eleTrigIDMVAWeights);

}

//--------------------------------------------------------------------------------------------------
void ElectronFiller::load(const edm::Event& iEvent)
{
  reset();
  FileUtilities::enforceGet(iEvent, electronTag_,electrons_,true);
  isLoaded_ = true;
}

//--------------------------------------------------------------------------------------------------
void ElectronFiller::fill()
{

  for (const pat::Electron &el : *electrons_) {
    if (el.pt() < eleptMin_) continue;

    data.fillMulti<float>(ipt_, el.pt());
    data.fillMulti<float>(ieta_, el.eta());
    data.fillMulti<float>(iSCeta_, el.superCluster()->eta());
    data.fillMulti<float>(iphi_, el.phi());
    data.fillMulti<float>(imass_, el.mass());
    data.fillMulti<int  >(iq_, el.charge());
    data.fillMulti<float>(ir9_, el.r9());

    assert(evtInfoFiller_->isLoaded());
    data.fillMulti<float>(id0_, -1.*el.gsfTrack()->dxy(evtInfoFiller_->primaryVertex()));
    data.fillMulti<float>(idz_, el.gsfTrack()->dz(evtInfoFiller_->primaryVertex()));

    float dbiso = el.pfIsolationVariables().sumChargedHadronPt + max(0.0 , el.pfIsolationVariables().sumNeutralHadronEt + el.pfIsolationVariables().sumPhotonEt - 0.5 * el.pfIsolationVariables().sumPUPt);
    data.fillMulti<float>(ipfdbetaiso_, dbiso);

    bool passveto = eleIdCuts->passEleIdCSA14CutBased(el.pt(), el.superCluster()->eta(), el.deltaEtaSuperClusterTrackAtVtx(), el.deltaPhiSuperClusterTrackAtVtx(), el.full5x5_sigmaIetaIeta(), el.hadronicOverEm(), (-1.*el.gsfTrack()->dxy(evtInfoFiller_->primaryVertex())), el.gsfTrack()->dz(evtInfoFiller_->primaryVertex()), el.ecalEnergy(), el.eSuperClusterOverP(), dbiso, el.passConversionVeto(), el.gsfTrack()->trackerExpectedHitsInner().numberOfLostHits(), LeptonId::VETO); 
    bool passloose = eleIdCuts->passEleIdCSA14CutBased(el.pt(), el.superCluster()->eta(), el.deltaEtaSuperClusterTrackAtVtx(), el.deltaPhiSuperClusterTrackAtVtx(), el.full5x5_sigmaIetaIeta(), el.hadronicOverEm(), (-1.*el.gsfTrack()->dxy(evtInfoFiller_->primaryVertex())), el.gsfTrack()->dz(evtInfoFiller_->primaryVertex()), el.ecalEnergy(), el.eSuperClusterOverP(), dbiso, el.passConversionVeto(), el.gsfTrack()->trackerExpectedHitsInner().numberOfLostHits(), LeptonId::LOOSE); 
    bool passmedium = eleIdCuts->passEleIdCSA14CutBased(el.pt(), el.superCluster()->eta(), el.deltaEtaSuperClusterTrackAtVtx(), el.deltaPhiSuperClusterTrackAtVtx(), el.full5x5_sigmaIetaIeta(), el.hadronicOverEm(), (-1.*el.gsfTrack()->dxy(evtInfoFiller_->primaryVertex())), el.gsfTrack()->dz(evtInfoFiller_->primaryVertex()), el.ecalEnergy(), el.eSuperClusterOverP(), dbiso, el.passConversionVeto(), el.gsfTrack()->trackerExpectedHitsInner().numberOfLostHits(), LeptonId::MEDIUM); 
    bool passtight = eleIdCuts->passEleIdCSA14CutBased(el.pt(), el.superCluster()->eta(), el.deltaEtaSuperClusterTrackAtVtx(), el.deltaPhiSuperClusterTrackAtVtx(), el.full5x5_sigmaIetaIeta(), el.hadronicOverEm(), (-1.*el.gsfTrack()->dxy(evtInfoFiller_->primaryVertex())), el.gsfTrack()->dz(evtInfoFiller_->primaryVertex()), el.ecalEnergy(), el.eSuperClusterOverP(), dbiso, el.passConversionVeto(), el.gsfTrack()->trackerExpectedHitsInner().numberOfLostHits(), LeptonId::TIGHT); 

    data.fillMulti<bool >(ivetoid_, passveto);
    data.fillMulti<bool >(ilooseid_, passloose);
    data.fillMulti<bool >(imediumid_, passmedium);
    data.fillMulti<bool >(itightid_, passtight);

    if(options_ & FILLIDVARS) {
      data.fillMulti<float>(iecalE_, el.ecalEnergy());
      data.fillMulti<float>(ieOverPIn_, el.eSuperClusterOverP());
      data.fillMulti<float>(isigietaieta_, el.sigmaIetaIeta());
      data.fillMulti<float>(isigiphiiphi_, el.sigmaIphiIphi());
      data.fillMulti<float>(ifull5x5sigietaieta_, el.full5x5_sigmaIetaIeta());
      data.fillMulti<float>(idEtaIn_, el.deltaEtaSuperClusterTrackAtVtx());
      data.fillMulti<float>(idPhiIn_, el.deltaPhiSuperClusterTrackAtVtx());
      data.fillMulti<float>(ihOverE_, el.hadronicOverEm());
      data.fillMulti<float>(ifBrem_, el.fbrem());
      data.fillMulti<int  >(inExpHitsInner_, el.gsfTrack()->trackerExpectedHitsInner().numberOfHits());
      data.fillMulti<int  >(inLostHitsInner_, el.gsfTrack()->trackerExpectedHitsInner().numberOfLostHits());
      data.fillMulti<bool >(ipassConvVeto_, el.passConversionVeto());
    }

    if(options_ & FILLISOVARS) {
      data.fillMulti<float>(itrackiso_, el.trackIso());
      data.fillMulti<float>(iecaliso_, el.ecalIso());
      data.fillMulti<float>(ihcaliso_, el.hcalIso());
      data.fillMulti<float>(ipfchargediso_, el.pfIsolationVariables().sumChargedHadronPt);
      data.fillMulti<float>(ipfneutraliso_, el.pfIsolationVariables().sumNeutralHadronEt);
      data.fillMulti<float>(ipfphotoniso_, el.pfIsolationVariables().sumPhotonEt);
      data.fillMulti<float>(ipfpuiso_, el.pfIsolationVariables().sumPUPt);
    }

    if(options_ & FILLPOGMVA) {
      data.fillMulti<float>(imvaidnontrig_, eleMVANonTrig->mvaValue(el, false));
      data.fillMulti<float>(imvaidtrig_, eleMVATrig->mvaValue(el, false));
    }

  }
  isFilled_ = true;

}
