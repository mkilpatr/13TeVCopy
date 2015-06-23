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
  iMVAiso_     = data.addMulti<float>(branchName_,"MVAiso",0);
  iminiiso_    = data.addMulti<float>(branchName_,"miniiso",0);
  iptrel_      = data.addMulti<float>(branchName_,"ptrel",0);
  iptratio_    = data.addMulti<float>(branchName_,"ptratio",0);
  isip3d_      = data.addMulti<float>(branchName_,"sip3d",0);

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
    imvaidnontrig_    = data.addMulti<float>(branchName,"mvaidnontrig",0);
    imvaidtrig_    = data.addMulti<float>(branchName,"mvaidtrig",0);
  } else {
    eleIdCuts = new LeptonId();
  }

  //  iLSF2Iso_   = data.addMulti<float>(branchName_,"lsf2Iso",0);
   iLSF3Iso_   = data.addMulti<float>(branchName_,"lsf3Iso",0);
   // iLSF4Iso_   = data.addMulti<float>(branchName_,"lsf4Iso",0);
   //  iLSF2IsoDR_ = data.addMulti<float>(branchName_,"lsf2IsoDR",0);
   //  iLSF3IsoDR_ = data.addMulti<float>(branchName_,"lsf3IsoDR",0);
   //  iLSF4IsoDR_ = data.addMulti<float>(branchName_,"lsf4IsoDR",0);

  string base = getenv("CMSSW_BASE");
  string electronisomva=base+"/src/AnalysisTools/ObjectSelection/data/Electrons/electron_sefsip3drhoiso_training.root_BDTG.weights.xml";
  eleMVAiso = new LeptonMVA();
  eleMVAiso->initialize(electronisomva);

  //  iPassTriggerLID_ = data.addMulti<bool>(branchName_,"passTriggerLID",0);
  iPassCutBaseNonIsoMID_ = data.addMulti<bool>(branchName_,"passCutBaseNonIsoMID",0);
  iPassLooseIDOnly_ = data.addMulti<bool>(branchName_,"passLooseIDOnly",0);

  //  iPassCutBaseTID_ = data.addMulti<bool>(branchName_,"passCutBaseTID",0);

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
void ElectronFiller::initMVA()
{

  string base = getenv("CMSSW_BASE");
  vector<string> eleNonTrigIDMVAWeights;
  vector<string> eleTrigIDMVAWeights;
  vector<string> eleNonTrigIDMVAPhys14Weights;

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
    eleNonTrigIDMVAPhys14Weights.push_back(base+"/src/AnalysisTools/ObjectSelection/data/Phys14/EIDmva_EB1_5_oldscenario2phys14_BDT.weights.xml");
    eleNonTrigIDMVAPhys14Weights.push_back(base+"/src/AnalysisTools/ObjectSelection/data/Phys14/EIDmva_EB2_5_oldscenario2phys14_BDT.weights.xml");
    eleNonTrigIDMVAPhys14Weights.push_back(base+"/src/AnalysisTools/ObjectSelection/data/Phys14/EIDmva_EE_5_oldscenario2phys14_BDT.weights.xml");
    eleNonTrigIDMVAPhys14Weights.push_back(base+"/src/AnalysisTools/ObjectSelection/data/Phys14/EIDmva_EB1_10_oldscenario2phys14_BDT.weights.xml");
    eleNonTrigIDMVAPhys14Weights.push_back(base+"/src/AnalysisTools/ObjectSelection/data/Phys14/EIDmva_EB2_10_oldscenario2phys14_BDT.weights.xml");
    eleNonTrigIDMVAPhys14Weights.push_back(base+"/src/AnalysisTools/ObjectSelection/data/Phys14/EIDmva_EE_10_oldscenario2phys14_BDT.weights.xml");
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

  eleMVANonTrigPhys14 = new EGammaMvaEleEstimatorCSA14();
  eleMVANonTrigPhys14->initialize("BDTSimpleCat",
				  EGammaMvaEleEstimatorCSA14::kNonTrigPhys14,
				  true,
				  eleNonTrigIDMVAPhys14Weights);



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
  FileUtilities::enforceGet(iEvent, "redCA8",ca8jets,true);
  iEvent.getByLabel("lsfSubJets","LSFJets2",lsfSubJets2);
  iEvent.getByLabel("lsfSubJets","LSFJets3",lsfSubJets3);
  iEvent.getByLabel("lsfSubJets","LSFJets4",lsfSubJets4);
  FileUtilities::enforceGet(iEvent,"fixedGridRhoFastjetAll",rho_,true);
  FileUtilities::enforceGet(iEvent,"slimmedJets",ak4jets_,true);
  FileUtilities::enforceGet(iEvent,"packedPFCandidates",pfcands,true);
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
      data.fillMulti<float>(imvaidnontrig_, eleMVANonTrigPhys14->mvaValue(*el, false));
      data.fillMulti<float>(imvaidtrig_, eleMVATrig->mvaValue(*el, false));
    }

    
    // calculate cut-based electrons ids
    bool tmp_iPassCutBaseNonIsoMID_ = false; //cout << "eta: " << el->superCluster()->eta() << "\n";
    bool tmp_iPassLooseIDOnly_ = false;
    if (fabs(el->superCluster()->eta()) <= 1.479) {
      //      cout << "EB\n";      
      if (
	  fabs(el->deltaEtaSuperClusterTrackAtVtx())                                          <  0.008925  &&
	  fabs(el->deltaPhiSuperClusterTrackAtVtx())                                          <  0.035973 &&
	  el->full5x5_sigmaIetaIeta()                                                         <  0.009996  &&
	  el->hadronicOverEm()                                                                <  0.050537 &&
	  fabs(-1.*el->gsfTrack()->dxy(evtInfoFiller_->primaryVertex()))                      <  0.012235 &&
	  fabs((el->gsfTrack()->dz(evtInfoFiller_->primaryVertex())))                         <  0.042020   &&	
	  fabs(1.0/el->ecalEnergy() - el->eSuperClusterOverP()/el->ecalEnergy())              <  0.091942 &&
	  el->passConversionVeto()                                                                      &&
	  el->gsfTrack()->hitPattern().numberOfLostHits(reco::HitPattern::MISSING_INNER_HITS) <= 1
	  ) { tmp_iPassCutBaseNonIsoMID_ = true; }
      
      else { tmp_iPassCutBaseNonIsoMID_ = false; }
      if(
        fabs(el->deltaEtaSuperClusterTrackAtVtx())                                          <  0.013625  &&
	fabs(el->deltaPhiSuperClusterTrackAtVtx())                                          <  0.230374  &&
	el->full5x5_sigmaIetaIeta()                                                         <  0.011586  &&
	el->hadronicOverEm()                                                                <  0.181130  &&
	fabs(-1.*el->gsfTrack()->dxy(evtInfoFiller_->primaryVertex()))                      <  0.094095  &&
	fabs((el->gsfTrack()->dz(evtInfoFiller_->primaryVertex())))                         <  0.713070 &&
	fabs(1.0/el->ecalEnergy() - el->eSuperClusterOverP()/el->ecalEnergy())              <  0.295751  &&
	el->passConversionVeto()                                                                      &&
	el->gsfTrack()->hitPattern().numberOfLostHits(reco::HitPattern::MISSING_INNER_HITS) <= 2
	) { tmp_iPassLooseIDOnly_ = true; }

    else { tmp_iPassLooseIDOnly_ = false; }


    } 
    
    else if ((fabs(el->superCluster()->eta()) > 1.479) && fabs(el->superCluster()->eta())<2.5 ) {

      if (
	  fabs(el->deltaEtaSuperClusterTrackAtVtx())                                          <  0.007429  &&
	  fabs(el->deltaPhiSuperClusterTrackAtVtx())                                          <  0.067879  &&
	  el->full5x5_sigmaIetaIeta()                                                         <  0.030135  &&
	  el->hadronicOverEm()                                                                <  0.086782   &&
	  fabs(-1.*el->gsfTrack()->dxy(evtInfoFiller_->primaryVertex()))                      <  0.036719  &&
	  fabs((el->gsfTrack()->dz(evtInfoFiller_->primaryVertex())))                         <  0.138142  &&	
	  fabs(1.0/el->ecalEnergy() - el->eSuperClusterOverP()/el->ecalEnergy())              <  0.100683 &&
	  el->passConversionVeto()                                                                      &&
	  el->gsfTrack()->hitPattern().numberOfLostHits(reco::HitPattern::MISSING_INNER_HITS) <= 1
	  ) { 
	//	std::cout<<"Electron passed"<<std::endl;
	tmp_iPassCutBaseNonIsoMID_ = true; }
      else { tmp_iPassCutBaseNonIsoMID_ = false; }
      if(
	 fabs(el->deltaEtaSuperClusterTrackAtVtx())                                          <  0.011932   &&
	 fabs(el->deltaPhiSuperClusterTrackAtVtx())                                          <  0.255450   &&
	 el->full5x5_sigmaIetaIeta()                                                         <  0.031849   &&
	 el->hadronicOverEm()                                                                <  0.223870   &&
	 fabs(-1.*el->gsfTrack()->dxy(evtInfoFiller_->primaryVertex()))                      <  0.342293  &&
	 fabs((el->gsfTrack()->dz(evtInfoFiller_->primaryVertex())))                         <  0.953461  &&
	 fabs(1.0/el->ecalEnergy() - el->eSuperClusterOverP()/el->ecalEnergy())              <  0.155501  &&
	 el->passConversionVeto()                                                                      &&
	 el->gsfTrack()->hitPattern().numberOfLostHits(reco::HitPattern::MISSING_INNER_HITS) <= 3
	 ) { tmp_iPassLooseIDOnly_ = true; }

      else { tmp_iPassLooseIDOnly_ = false; }



    }
    
    data.fillMulti<bool>(iPassCutBaseNonIsoMID_,tmp_iPassCutBaseNonIsoMID_);
    data.fillMulti<bool>(iPassLooseIDOnly_,tmp_iPassLooseIDOnly_);
    data.fillMulti<float>(iminiiso_,getPFIsolation(pfcands, *elPtr, 0.05, 0.2, 10., false, false));
    data.fillMulti<float>(iptrel_,getLeptonPtRel( ak4jets_, *elPtr));
    data.fillMulti<float>(iptratio_,getLeptonPtRatio( ak4jets_, *elPtr ));

    //    float lsf2Iso = 9.; float lsf2IsoDR = 9.;
    //    float lsf3Iso = 9.; float lsf3IsoDR = 9.;
    //    float lsf4Iso = 9.; float lsf4IsoDR = 9.;

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

    //    calculateLSFIso(el_,lsfSubJets2_,&lsf2Iso,&lsf2IsoDR);
    //    calculateLSFIso(el_,lsfSubJets3_,&lsf3Iso,&lsf3IsoDR);
    //    calculateLSFIso(el_,lsfSubJets4_,&lsf4Iso,&lsf4IsoDR);
    
    //    data.fillMulti<float>(iLSF2Iso_  ,LSF(el_,ca8jets));
    //    data.fillMulti<float>(iLSF2IsoDR_,lsf2IsoDR);
    data.fillMulti<float>(iLSF3Iso_  ,LSF(el_,ca8jets));
  //    data.fillMulti<float>(iLSF3IsoDR_,lsf3IsoDR);
  //    data.fillMulti<float>(iLSF4Iso_  ,lsf4Iso);
  //    data.fillMulti<float>(iLSF4IsoDR_,lsf4IsoDR);


    double rhoiso=calculateRhoIso(el->eta(),el->pfIsolationVariables().sumChargedHadronPt,el->pfIsolationVariables().sumNeutralHadronEt,el->pfIsolationVariables().sumPhotonEt,*rho_);
    double sip3d=fabs(el->dB(el->PV3D) / el->edB(el->PV3D));
    data.fillMulti<float>(iMVAiso_,eleMVAiso->evaluateMVA(el->pt(), LSF(el_,ca8jets) , sip3d, rhoiso));
    data.fillMulti<float>(isip3d_, sip3d);

    if(options_ & LOADGEN) {
      const reco::GenParticle* genEle = el->genParticle();
      if(genEle) {
        data.fillMulti<float>(igenpt_, genEle->pt());
        data.fillMulti<float>(igeneta_, genEle->eta());
        data.fillMulti<float>(igenphi_, genEle->phi());
        data.fillMulti<float>(igenmass_, genEle->mass());
        data.fillMulti<int  >(igenstatus_, genEle->status());
        data.fillMulti<int  >(igenpdgid_, genEle->pdgId());
        if(genEle->numberOfMothers()) {
          const auto* mother = genEle->mother(0);
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
float ElectronFiller::calculateRhoIso(double eta, double pfchargediso, double pfneutraliso, double pfphotoniso, float rho) {

     double EA=0.1530;
     if(fabs(eta)<0.8) EA=0.1013;
     else if(fabs(eta)<1.3) EA=0.0988;
     else if(fabs(eta)<2.0) EA=0.0572;
     else if(fabs(eta)<2.3) EA=0.0842;
     return pfchargediso+TMath::Max(pfneutraliso+pfphotoniso-rho*EA,0.0);

}

double ElectronFiller::getPFIsolation(edm::Handle<pat::PackedCandidateCollection> pfcands, const pat::Electron ptcl, double r_iso_min, double r_iso_max, double kt_scale, bool use_pfweight, bool charged_only) {
   if (ptcl.pt()<5.) return 99999.;
   double deadcone_nh(0.), deadcone_ch(0.), deadcone_ph(0.), deadcone_pu(0.);
   //   if(ptcl.isElectron()) {
   if (fabs(ptcl.superCluster()->eta())>1.479) {
      deadcone_ch = 0.015; deadcone_pu = 0.015; deadcone_ph = 0.08;}
   double iso_nh(0.); double iso_ch(0.);
   double iso_ph(0.); double iso_pu(0.);
   double ptThresh(0.5);
   //   if(ptcl.isElectron()) 
   ptThresh = 0;
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

double ElectronFiller::getLeptonPtRel(edm::Handle<pat::JetCollection> jets, const pat::Electron lepton) {
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
    for (auto itr : lepton.associatedPackedPFCandidates()) {
      if ( &(*itr) == &candidate) {
	isPartOfLepton = true;
	break;
      } 
    }
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


double ElectronFiller::getLeptonPtRatio(edm::Handle<pat::JetCollection> jets, const pat::Electron lepton) {
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

double ElectronFiller::LSF(LorentzVector lep,edm::Handle<std::vector<reco::PFJet>> ca8jets) {
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

