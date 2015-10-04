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
ElectronFiller::ElectronFiller(const edm::ParameterSet& cfg, edm::ConsumesCollector && cc, const int options, const string branchName, EventInfoFiller * evtInfoFiller) :
  BaseFiller(options, branchName),
  evtInfoFiller_(evtInfoFiller),
  electronToken_(cc.consumes<pat::ElectronCollection>        (cfg.getParameter<edm::InputTag>("electrons"))),
  vetoIdToken_  (cc.consumes<edm::ValueMap<bool> >           (cfg.getParameter<edm::InputTag>("vetoId"))),
  looseIdToken_ (cc.consumes<edm::ValueMap<bool> >           (cfg.getParameter<edm::InputTag>("looseId"))),
  mediumIdToken_(cc.consumes<edm::ValueMap<bool> >           (cfg.getParameter<edm::InputTag>("mediumId"))),
  tightIdToken_ (cc.consumes<edm::ValueMap<bool> >           (cfg.getParameter<edm::InputTag>("tightId"))),
  looseIdFullInfoMapToken_(cc.consumes<edm::ValueMap<vid::CutFlowResult> > (cfg.getParameter<edm::InputTag>("looseId"))),
  mediumIdFullInfoMapToken_(cc.consumes<edm::ValueMap<vid::CutFlowResult> >(cfg.getParameter<edm::InputTag>("mediumId"))),
  mvanontrigMediumIdToken_(cc.consumes<edm::ValueMap<bool> > (cfg.getParameter<edm::InputTag>("mvanontrigwp90Id"))),
  mvanontrigTightIdToken_ (cc.consumes<edm::ValueMap<bool> > (cfg.getParameter<edm::InputTag>("mvanontrigwp80Id"))),
  mvanontrigMVAValueToken_(cc.consumes<edm::ValueMap<float> >(cfg.getParameter<edm::InputTag>("mvanontrigValuesMap"))),
  mvanontrigMVACatToken_(cc.consumes<edm::ValueMap<int> >    (cfg.getParameter<edm::InputTag>("mvanontrigCategoriesMap"))),
  mvatrigMediumIdToken_(cc.consumes<edm::ValueMap<bool> >    (cfg.getParameter<edm::InputTag>("mvatrigwp90Id"))),
  mvatrigTightIdToken_ (cc.consumes<edm::ValueMap<bool> >    (cfg.getParameter<edm::InputTag>("mvatrigwp80Id"))),
  mvatrigMVAValueToken_(cc.consumes<edm::ValueMap<float> >   (cfg.getParameter<edm::InputTag>("mvatrigValuesMap"))),
  mvatrigMVACatToken_(cc.consumes<edm::ValueMap<int> >       (cfg.getParameter<edm::InputTag>("mvatrigCategoriesMap"))),
  ca8jetToken_  (cc.consumes<reco::PFJetCollection>          (cfg.getParameter<edm::InputTag>("ca8jets"))),
  rhoToken_     (cc.consumes<double>                         (cfg.getParameter<edm::InputTag>("rho"))),
  jetToken_     (cc.consumes<pat::JetCollection>             (cfg.getParameter<edm::InputTag>("jets"))),
  pfcandToken_  (cc.consumes<pat::PackedCandidateCollection> (cfg.getParameter<edm::InputTag>("pfcands"))),
  eleptMin_     (cfg.getUntrackedParameter<double>           ("minElectronPt"))
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
  imvanontrigmediumid_ = data.addMulti<bool >(branchName_,"mvanontrigmediumid",false);
  imvanontrigtightid_  = data.addMulti<bool >(branchName_,"mvanontrigtightid",false);
  imvatrigmediumid_    = data.addMulti<bool >(branchName_,"mvanontrigmediumid",false);
  imvatrigtightid_     = data.addMulti<bool >(branchName_,"mvanontrigtightid",false);
  ipfdbetaiso_ = data.addMulti<float>(branchName_,"pfdbetaiso",0);
  iminiiso_    = data.addMulti<float>(branchName_,"miniiso",0);
  iannulus_    = data.addMulti<float>(branchName_,"annulus",0);
  iptrel_      = data.addMulti<float>(branchName_,"ptrel",0);
  iptratio_    = data.addMulti<float>(branchName_,"ptratio",0);
  irhoiso_     = data.addMulti<float>(branchName_,"rhoiso",0);
  isip3d_      = data.addMulti<float>(branchName_,"sip3d",0);
  iLSFIso_     = data.addMulti<float>(branchName_,"lsfIso",0);
  inExpHitsInner_      = data.addMulti<int  >(branchName_,"nExpHitsInner",0);
  inLostHitsInner_     = data.addMulti<int  >(branchName_,"nLostHitsInner",0);
  ipassConvVeto_       = data.addMulti<bool >(branchName_,"passConvVeto",false);
  iPassMediumIDOnly_   = data.addMulti<bool >(branchName_,"passMediumIDOnly",0);
  iPassLooseIDOnly_    = data.addMulti<bool >(branchName_,"passLooseIDOnly",0);


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
    imvaidnontrig_    = data.addMulti<float>(branchName,"mvaidnontrig",0);
    imvaidcatnontrig_ = data.addMulti<float>(branchName,"mvaidcatnontrig",0);
    imvaidtrig_       = data.addMulti<float>(branchName,"mvaidtrig",0);
    imvaidcattrig_    = data.addMulti<float>(branchName,"mvaidcattrig",0);
  }

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
void ElectronFiller::load(const edm::Event& iEvent, const edm::EventSetup &iSetup)
{
  reset();
  iEvent.getByToken(electronToken_,electrons_);
  iEvent.getByToken(vetoIdToken_,veto_id_decisions_);
  iEvent.getByToken(looseIdToken_,loose_id_decisions_);
  iEvent.getByToken(mediumIdToken_,medium_id_decisions_);
  iEvent.getByToken(tightIdToken_,tight_id_decisions_);
  iEvent.getByToken(looseIdFullInfoMapToken_,loose_id_cutflow_);
  iEvent.getByToken(mediumIdFullInfoMapToken_,medium_id_cutflow_);
  iEvent.getByToken(mvanontrigMediumIdToken_,mvanontrig_medium_id_decisions_);
  iEvent.getByToken(mvanontrigTightIdToken_,mvanontrig_tight_id_decisions_);
  iEvent.getByToken(mvanontrigMVAValueToken_,mvanontrig_value_map_);
  iEvent.getByToken(mvanontrigMVACatToken_,mvanontrig_category_map_);
  iEvent.getByToken(mvatrigMediumIdToken_,mvatrig_medium_id_decisions_);
  iEvent.getByToken(mvatrigTightIdToken_,mvatrig_tight_id_decisions_);
  iEvent.getByToken(mvatrigMVAValueToken_,mvatrig_value_map_);
  iEvent.getByToken(mvatrigMVACatToken_,mvatrig_category_map_);
  iEvent.getByToken(ca8jetToken_,ca8jets_);
  iEvent.getByToken(rhoToken_,rho_);
  iEvent.getByToken(jetToken_,ak4jets_);
  iEvent.getByToken(pfcandToken_,pfcands_);
  isLoaded_ = true;
}

//--------------------------------------------------------------------------------------------------
void ElectronFiller::fill()
{

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
    data.fillMulti<bool >(ivetoid_, (*veto_id_decisions_)[ elPtr ]);
    data.fillMulti<bool >(ilooseid_, (*loose_id_decisions_)[ elPtr ]);
    data.fillMulti<bool >(imediumid_, (*medium_id_decisions_)[ elPtr ]);
    data.fillMulti<bool >(itightid_, (*tight_id_decisions_)[ elPtr ]);
    data.fillMulti<bool >(imvanontrigmediumid_, (*mvanontrig_medium_id_decisions_)[ elPtr ]);
    data.fillMulti<bool >(imvanontrigtightid_, (*mvanontrig_tight_id_decisions_)[ elPtr ]);
    data.fillMulti<bool >(imvatrigmediumid_, (*mvatrig_medium_id_decisions_)[ elPtr ]);
    data.fillMulti<bool >(imvatrigtightid_, (*mvatrig_tight_id_decisions_)[ elPtr ]);

    const auto& miniIsoRlt = Isolation::miniPFIso(*elPtr, *pfcands_);
    data.fillMulti<float>(iminiiso_,miniIsoRlt.miniIso);
    data.fillMulti<float>(iannulus_,miniIsoRlt.activity);

    data.fillMulti<float>(iptrel_, Isolation::leptonPtRel(*elPtr, *ak4jets_));
    data.fillMulti<float>(iptratio_,Isolation::leptonPtRatio(*elPtr, *ak4jets_));

    double LSF = Isolation::LSF(*elPtr, *ca8jets_);
    data.fillMulti<float>(iLSFIso_  ,LSF);

    double rhoiso=Isolation::rhoIso(*elPtr, *rho_);
    data.fillMulti<float>(irhoiso_, rhoiso);
 
    double sip3d=fabs(el->dB(el->PV3D) / el->edB(el->PV3D));
    data.fillMulti<float>(isip3d_, sip3d);

    data.fillMulti<int  >(inExpHitsInner_, el->gsfTrack()->hitPattern().numberOfHits(reco::HitPattern::MISSING_INNER_HITS));
    data.fillMulti<int  >(inLostHitsInner_, el->gsfTrack()->hitPattern().numberOfLostHits(reco::HitPattern::MISSING_INNER_HITS));
    data.fillMulti<bool >(ipassConvVeto_, el->passConversionVeto());

    vid::CutFlowResult mediumIdIsoMasked = (*medium_id_cutflow_)[ elPtr ].getCutFlowResultMasking("GsfEleEffAreaPFIsoCut_0");
    data.fillMulti<bool>(iPassMediumIDOnly_, mediumIdIsoMasked.cutFlowPassed());

    vid::CutFlowResult looseIdIsoMasked = (*loose_id_cutflow_)[ elPtr ].getCutFlowResultMasking("GsfEleEffAreaPFIsoCut_0");
    data.fillMulti<bool>(iPassLooseIDOnly_, looseIdIsoMasked.cutFlowPassed());

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
      data.fillMulti<float>(imvaidnontrig_, (*mvanontrig_value_map_)[ elPtr ]);
      data.fillMulti<int  >(imvaidcatnontrig_, (*mvanontrig_category_map_)[ elPtr ]);
      data.fillMulti<float>(imvaidtrig_, (*mvatrig_value_map_)[ elPtr ]);
      data.fillMulti<int  >(imvaidcattrig_, (*mvatrig_category_map_)[ elPtr ]);
    }

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
