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
  const edm::InputTag vetoIdTag,
  const edm::InputTag looseIdTag,
  const edm::InputTag mediumIdTag,
  const edm::InputTag tightIdTag,
  const int bunchSpacing,
  const double eleptMin) :
  BaseFiller(options, branchName),
  evtInfoFiller_(evtInfoFiller),
  electronTag_(electronTag),
  vetoIdTag_(vetoIdTag),
  looseIdTag_(looseIdTag),
  mediumIdTag_(mediumIdTag),
  tightIdTag_(tightIdTag),
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

  iLSF2Iso_   = data.addMulti<float>(branchName_,"lsf2Iso",0);
  iLSF3Iso_   = data.addMulti<float>(branchName_,"lsf3Iso",0);
  iLSF4Iso_   = data.addMulti<float>(branchName_,"lsf4Iso",0);
  iLSF2IsoDR_ = data.addMulti<float>(branchName_,"lsf2IsoDR",0);
  iLSF3IsoDR_ = data.addMulti<float>(branchName_,"lsf3IsoDR",0);
  iLSF4IsoDR_ = data.addMulti<float>(branchName_,"lsf4IsoDR",0);

  //  iPassTriggerLID_ = data.addMulti<bool>(branchName_,"passTriggerLID",0);
  iPassCutBaseNonIsoMID_ = data.addMulti<bool>(branchName_,"passCutBaseNonIsoMID",0);
  //  iPassCutBaseTID_ = data.addMulti<bool>(branchName_,"passCutBaseTID",0);

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
  FileUtilities::enforceGet(iEvent, vetoIdTag_,veto_id_decisions_,true);
  FileUtilities::enforceGet(iEvent, looseIdTag_,loose_id_decisions_,true);
  FileUtilities::enforceGet(iEvent, mediumIdTag_,medium_id_decisions_,true);
  FileUtilities::enforceGet(iEvent, tightIdTag_,tight_id_decisions_,true);
  //  FileUtilities::enforceGet(iEvent, "ak8PFJetsCHSTrimmedr0p1ptf0p03",trimmedJets,true);
  FileUtilities::enforceGet(iEvent, "redCA8",ca8Jets,true);
  iEvent.getByLabel("lsfSubJets","LSFJets2",lsfSubJets2);
  iEvent.getByLabel("lsfSubJets","LSFJets3",lsfSubJets3);
  iEvent.getByLabel("lsfSubJets","LSFJets4",lsfSubJets4);

  isLoaded_ = true;
}

//--------------------------------------------------------------------------------------------------
void ElectronFiller::fill()
{

  //  cout << "\n NEW EVENT ; Mel = " << electrons_->size() << "\n";
  //  std::vector<reco::CandidatePtr> footprint_el;
  for (pat::ElectronCollection::const_iterator el = electrons_->begin(); el != electrons_->end(); el++) {
    if (el->pt() < eleptMin_) continue;

    data.fillMulti<float>(ipt_, el->pt());
    data.fillMulti<float>(ieta_, el->eta());
    data.fillMulti<float>(iSCeta_, el->superCluster()->eta());
    data.fillMulti<float>(iphi_, el->phi());
    data.fillMulti<float>(imass_, el->mass());
    data.fillMulti<int  >(iq_, el->charge());
    data.fillMulti<float>(ir9_, el->r9());

    assert(evtInfoFiller_->isLoaded());
    data.fillMulti<float>(id0_, -1.*el->gsfTrack()->dxy(evtInfoFiller_->primaryVertex()));
    data.fillMulti<float>(idz_, el->gsfTrack()->dz(evtInfoFiller_->primaryVertex()));

    float dbiso = el->pfIsolationVariables().sumChargedHadronPt + max(0.0 , el->pfIsolationVariables().sumNeutralHadronEt + el->pfIsolationVariables().sumPhotonEt - 0.5 * el->pfIsolationVariables().sumPUPt);
    data.fillMulti<float>(ipfdbetaiso_, dbiso);

    const edm::Ptr<pat::Electron> elPtr(electrons_, el - electrons_->begin() );
    bool passveto = (*veto_id_decisions_)[ elPtr ];
    bool passloose = (*loose_id_decisions_)[ elPtr ];
    bool passmedium = (*medium_id_decisions_)[ elPtr ];
    bool passtight = (*tight_id_decisions_)[ elPtr ];
    data.fillMulti<bool >(ivetoid_, passveto);
    data.fillMulti<bool >(ilooseid_, passloose);
    data.fillMulti<bool >(imediumid_, passmedium);
    data.fillMulti<bool >(itightid_, passtight);

    if(options_ & FILLIDVARS) {
      data.fillMulti<float>(iecalE_, el->ecalEnergy());
      data.fillMulti<float>(ieOverPIn_, el->eSuperClusterOverP());
      data.fillMulti<float>(isigietaieta_, el->sigmaIetaIeta());
      data.fillMulti<float>(isigiphiiphi_, el->sigmaIphiIphi());
      data.fillMulti<float>(ifull5x5sigietaieta_, el->full5x5_sigmaIetaIeta());
      data.fillMulti<float>(idEtaIn_, el->deltaEtaSuperClusterTrackAtVtx());
      data.fillMulti<float>(idPhiIn_, el->deltaPhiSuperClusterTrackAtVtx());
      data.fillMulti<float>(ihOverE_, el->hadronicOverEm());
      data.fillMulti<float>(ifBrem_, el->fbrem());
      data.fillMulti<int  >(inExpHitsInner_, el->gsfTrack()->hitPattern().numberOfHits(reco::HitPattern::MISSING_INNER_HITS));
      data.fillMulti<int  >(inLostHitsInner_, el->gsfTrack()->hitPattern().numberOfLostHits(reco::HitPattern::MISSING_INNER_HITS));
      data.fillMulti<bool >(ipassConvVeto_, el->passConversionVeto());
    }

    if(options_ & FILLISOVARS) {
      data.fillMulti<float>(itrackiso_, el->trackIso());
      data.fillMulti<float>(iecaliso_, el->ecalIso());
      data.fillMulti<float>(ihcaliso_, el->hcalIso());
      data.fillMulti<float>(ipfchargediso_, el->pfIsolationVariables().sumChargedHadronPt);
      data.fillMulti<float>(ipfneutraliso_, el->pfIsolationVariables().sumNeutralHadronEt);
      data.fillMulti<float>(ipfphotoniso_, el->pfIsolationVariables().sumPhotonEt);
      data.fillMulti<float>(ipfpuiso_, el->pfIsolationVariables().sumPUPt);
    }

    if(options_ & FILLPOGMVA) {
      data.fillMulti<float>(imvaidnontrig_, eleMVANonTrig->mvaValue(*el, false));
      data.fillMulti<float>(imvaidtrig_, eleMVATrig->mvaValue(*el, false));
    }

    
    // calculate cut-based electrons ids
    bool tmp_iPassCutBaseNonIsoMID_ = false; //cout << "eta: " << el->superCluster()->eta() << "\n";
    if (fabs(el->superCluster()->eta()) <= 1.479) {
      //      cout << "EB\n";      
      if (
	  fabs(el->deltaEtaSuperClusterTrackAtVtx())                                          <  0.0106 &&
	  fabs(el->deltaPhiSuperClusterTrackAtVtx())                                          <  0.0323 &&
	  el->full5x5_sigmaIetaIeta()                                                         <  0.0107 &&
	  el->hadronicOverEm()                                                                <  0.067  &&
	  fabs(-1.*el->gsfTrack()->dxy(evtInfoFiller_->primaryVertex()))                      <  0.0131 &&
	  fabs((el->gsfTrack()->dz(evtInfoFiller_->primaryVertex())))                         <  0.2231 &&	
	  fabs(1.0/el->ecalEnergy() - el->eSuperClusterOverP()/el->ecalEnergy())              <  0.1043 &&
	  el->passConversionVeto()                                                                      &&
	  el->gsfTrack()->hitPattern().numberOfLostHits(reco::HitPattern::MISSING_INNER_HITS) <= 1
	  ) { tmp_iPassCutBaseNonIsoMID_ = true; }
      
      else { tmp_iPassCutBaseNonIsoMID_ = false; }

    } 
    
    else if ((fabs(el->superCluster()->eta()) > 1.479) && fabs(el->superCluster()->eta())<2.5 ) {

      if (
	  fabs(el->deltaEtaSuperClusterTrackAtVtx())                                          <  0.0108 &&
	  fabs(el->deltaPhiSuperClusterTrackAtVtx())                                          <  0.0455 &&
	  el->full5x5_sigmaIetaIeta()                                                         <  0.0318 &&
	  el->hadronicOverEm()                                                                <  0.097  &&
	  fabs(-1.*el->gsfTrack()->dxy(evtInfoFiller_->primaryVertex()))                      <  0.0845 &&
	  fabs((el->gsfTrack()->dz(evtInfoFiller_->primaryVertex())))                         <  0.7523 &&	
	  fabs(1.0/el->ecalEnergy() - el->eSuperClusterOverP()/el->ecalEnergy())              <  0.1201 &&
	  el->passConversionVeto()                                                                      &&
	  el->gsfTrack()->hitPattern().numberOfLostHits(reco::HitPattern::MISSING_INNER_HITS) <= 1
	  ) { tmp_iPassCutBaseNonIsoMID_ = true; }

      else { tmp_iPassCutBaseNonIsoMID_ = false; }

    }
    
    data.fillMulti<bool>(iPassCutBaseNonIsoMID_,tmp_iPassCutBaseNonIsoMID_);
    

    float lsf2Iso = 9.; float lsf2IsoDR = 9.;
    float lsf3Iso = 9.; float lsf3IsoDR = 9.;
    float lsf4Iso = 9.; float lsf4IsoDR = 9.;

    LorentzVector el_;
    el_ = el->p4();

    LorentzVectorCollection lsfSubJets2_;
    for (LorentzVectorCollection::const_iterator jt = lsfSubJets2->begin(); jt != lsfSubJets2->end(); jt++) {
      LorentzVector tmpVec;
      tmpVec.SetPxPyPzE(jt->px(),jt->py(),jt->pz(),jt->energy());
      lsfSubJets2_.push_back(tmpVec);
    }

    LorentzVectorCollection lsfSubJets3_;
    for (LorentzVectorCollection::const_iterator jt = lsfSubJets3->begin(); jt != lsfSubJets3->end(); jt++) {
      LorentzVector tmpVec;
      tmpVec.SetPxPyPzE(jt->px(),jt->py(),jt->pz(),jt->energy());
      lsfSubJets3_.push_back(tmpVec);
    }

    LorentzVectorCollection lsfSubJets4_;
    for (LorentzVectorCollection::const_iterator jt = lsfSubJets4->begin(); jt != lsfSubJets4->end(); jt++) {
      LorentzVector tmpVec;
      tmpVec.SetPxPyPzE(jt->px(),jt->py(),jt->pz(),jt->energy());
      lsfSubJets4_.push_back(tmpVec);
    }

    calculateLSFIso(el_,lsfSubJets2_,&lsf2Iso,&lsf2IsoDR);
    calculateLSFIso(el_,lsfSubJets3_,&lsf3Iso,&lsf3IsoDR);
    calculateLSFIso(el_,lsfSubJets4_,&lsf4Iso,&lsf4IsoDR);
    
    data.fillMulti<float>(iLSF2Iso_  ,lsf2Iso);
    data.fillMulti<float>(iLSF2IsoDR_,lsf2IsoDR);
    data.fillMulti<float>(iLSF3Iso_  ,lsf3Iso);
    data.fillMulti<float>(iLSF3IsoDR_,lsf3IsoDR);
    data.fillMulti<float>(iLSF4Iso_  ,lsf4Iso);
    data.fillMulti<float>(iLSF4IsoDR_,lsf4IsoDR);

  }
  isFilled_ = true;


}


void ElectronFiller::calculateLSFIso(LorentzVector el_,LorentzVectorCollection lsfSubJets_, float *lsfIso_, float *lsfIsoDR_) {

  float LSFDR_  = 999.;
  float LSF_ = -9.;
  for (LorentzVectorCollection::const_iterator jt = lsfSubJets_.begin(); jt != lsfSubJets_.end(); jt++) {
      LorentzVector jt_;
      jt_.SetPxPyPzE(jt->px(),jt->py(),jt->pz(),jt->e());
      float dRtmp_  = deltaR(el_,jt_);
      if (dRtmp_<LSFDR_) {
	LSFDR_  = dRtmp_;
	LSF_ = el_.pt()/jt->pt();
      }
    } // end of looping over the subjets jets

  *lsfIso_   = LSF_;
  *lsfIsoDR_ = LSFDR_;

}
