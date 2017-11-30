#include "AnalysisBase/Analyzer/interface/prodIsoTrksFiller.h"


using namespace ucsbsusy;

//--------------------------------------------------------------------------------------------------
prodIsoTrksFiller::prodIsoTrksFiller(const edm::ParameterSet& cfg, edm::ConsumesCollector && cc, const int options, const string branchName, EventInfoFiller * evtInfoFiller) :
  BaseFiller(options, branchName),
  evtInfoFiller_(evtInfoFiller)
  tauToken_     (cc.consumes<pat::TauCollection>            (cfg.getParameter<edm::InputTag>("taus"))),
  candptMin_    (cfg.getUntrackedParameter<double>          ("minCandPt")),
  candetaMax_   (cfg.getUntrackedParameter<double>          ("maxCandEta")),
{

  string base = getenv("CMSSW_BASE") + string("/src/data/taus/");

  itrkIsoVeto_lVec_                = data.addMulti<float>(branchName_,"trkIsoVeto_lVec",0);
  itrkIsoVeto_charge_              = data.addMulti<float>(branchName_,"trkIsoVeto_charge",0);
  itrkIsoVeto_dz_                  = data.addMulti<float>(branchName_,"trkIsoVeto_dz",0);
  itrkIsoVeto_pdgId_               = data.addMulti<float>(branchName_,"trkIsoVeto_pdgId",0);
  itrkIsoVeto_idx_                 = data.addMulti<float>(branchName_,"trkIsoVeto_idx",0);
  itrkIsoVeto_iso_                 = data.addMulti<float>(branchName_,"trkIsoVeto_iso",0);
  itrkIsoVeto_pfActivity_          = data.addMulti<float>(branchName_,"trkIsoVeto_pfActivity",0);

  ilooseIsoTrks_lVec_              = data.addMulti<float>(branchName_,"looseIsoTrks_lVec",0);
  ilooseIsoTrks_charge_            = data.addMulti<float>(branchName_,"looseIsoTrks_charge",0);
  ilooseIsoTrks_dz_                = data.addMulti<float>(branchName_,"looseIsoTrks_dz",0);
  ilooseIsoTrks_pdgId_             = data.addMulti<float>(branchName_,"looseIsoTrks_pdgId",0);
  ilooseIsoTrks_idx_               = data.addMulti<float>(branchName_,"looseIsoTrks_idx",0);
  ilooseIsoTrks_iso_               = data.addMulti<float>(branchName_,"looseIsoTrks_iso",0);
  ilooseIsoTrks_mtw_               = data.addMulti<float>(branchName_,"looseIsoTrks_mtw",0);
  ilooseIsoTrks_pfActivity_        = data.addMulti<float>(branchName_,"looseIsoTrks_pfActivity",0);

  iforVetoIsoTrks_idx_             = data.addMulti<float>(branchName_,"forVetoIsoTrks_idx",0);

  iloosenIsoTrks_		   = data.addMulti<float>(branchName_,"loosenIsoTrks",0);
  inIsoTrksForVeto_		   = data.addMulti<float>(branchName_,"nIsoTrksForVeto",0);

}

//prodIsoTrks Functions from Alpha group
bool prodIsoTrksFiller::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  if( !iEvent.isRealData() ) isData_ = false;

  std::unique_ptr<std::vector<TLorentzVector> > trksForIsoVetoLVec(new std::vector<TLorentzVector>());
  std::unique_ptr<std::vector<double> > trksForIsoVeto_charge(new std::vector<double>());
  std::unique_ptr<std::vector<double> > trksForIsoVeto_dz(new std::vector<double>());
  std::unique_ptr<std::vector<int> > trksForIsoVeto_pdgId(new std::vector<int>());
  std::unique_ptr<std::vector<int> > trksForIsoVeto_idx(new std::vector<int>());
  std::unique_ptr<std::vector<double> > trksForIsoVeto_iso(new std::vector<double>());
  std::unique_ptr<std::vector<double> > trksForIsoVeto_pfActivity(new std::vector<double>());

  std::unique_ptr<std::vector<TLorentzVector> > loose_isoTrksLVec(new std::vector<TLorentzVector>());
  std::unique_ptr<std::vector<double> > loose_isoTrks_charge(new std::vector<double>());
  std::unique_ptr<std::vector<double> > loose_isoTrks_dz(new std::vector<double>());
  std::unique_ptr<std::vector<int> > loose_isoTrks_pdgId(new std::vector<int>());
  std::unique_ptr<std::vector<int> > loose_isoTrks_idx(new std::vector<int>());
  std::unique_ptr<std::vector<double> > loose_isoTrks_iso(new std::vector<double>());
  std::unique_ptr<std::vector<double> > loose_isoTrks_mtw(new std::vector<double>());
  std::unique_ptr<std::vector<double> > loose_isoTrks_pfActivity(new std::vector<double>());

  std::unique_ptr<std::vector<int> > forVetoIsoTrks_idx(new std::vector<int>());
/*
  if( !isData_ ){
     iEvent.getByToken(W_EmVec_Tok_, W_emuVec_);
     iEvent.getByToken(W_Tau_EmuVec_Tok_, W_tau_emuVec_);
     iEvent.getByToken(W_Tau_ProngsVec_Tok_, W_tau_prongsVec_);
     iEvent.getByToken(GenDecayLVec_Tok_, genDecayLVec_);
  }
*/
  edm::Handle< std::vector<reco::Vertex> > vertices;
  iEvent.getByToken(VtxTok_, vertices);
//  reco::Vertex::Point vtxpos = (vertices->size() > 0 ? (*vertices)[0].position() : reco::Vertex::Point());

  edm::Handle<edm::View<reco::MET> > met;
  iEvent.getByToken(MetTok_, met);

  iEvent.getByToken(Loose_IsoTrksHandle_Tok_, loose_isoTrksHandle_); 
  if( loose_isoTrksHandle_.isValid() ) loose_nIsoTrks = loose_isoTrksHandle_->size(); else loose_nIsoTrks =0;
  iEvent.getByToken(ForVetoIsoTrks_Tok_, forVetoIsoTrks_); 
  if( forVetoIsoTrks_.isValid() ) nIsoTrksForVeto = forVetoIsoTrks_->size(); else nIsoTrksForVeto =0;

  ///iEvent.getByToken(Ref_All_IsoTrksHandle_Tok_, ref_all_isoTrksHandle_); 
  //iEvent.getByToken(Ref_All_IsoTrks_IsoVecHandle_Tok_, ref_all_isoTrks_isoVecHandle_); 

  edm::Handle<std::vector<double> >  loose_isotrk_isoVecHandle, loose_isotrk_dzpvVecHandle;
  iEvent.getByToken(Loose_Isotrk_IsoVecHandle_Tok_, loose_isotrk_isoVecHandle);
  iEvent.getByToken(Loose_Isotrk_DzpvVecHandle_Tok_, loose_isotrk_dzpvVecHandle);
  if( loose_isoTrksHandle_.isValid() && loose_isotrk_isoVecHandle.isValid() && loose_isotrk_dzpvVecHandle.isValid() ){
     if( loose_nIsoTrks != loose_isotrk_isoVecHandle->size() || loose_nIsoTrks != loose_isotrk_dzpvVecHandle->size() ){
        std::cout<<"ERROR ... mis-matching between loose_nIsoTrks : "<<loose_nIsoTrks<<"  loose_isotrk_isoVecHandle->size : "<<loose_isotrk_isoVecHandle->size()<<"  loose_isotrk_dzpvVecHandle->size : "<<loose_isotrk_dzpvVecHandle->size()<<std::endl;
     }
  }

  if( debug_ ) std::cout<<"\nloose_nIsoTrks : "<<loose_nIsoTrks<<"  nIsoTrksForVeto : "<<nIsoTrksForVeto<<std::endl;
  for(unsigned int is=0; is<loose_nIsoTrks; is++){
     const pat::PackedCandidate isoTrk = (*loose_isoTrksHandle_)[is];
     double isoTrkpt = isoTrk.pt(), isoTrketa = isoTrk.eta(), isoTrkphi = isoTrk.phi(), isoTrkenergy = isoTrk.energy();
     double isoTrkcharge = isoTrk.charge();

     TLorentzVector perIsoTrkLVec;
     perIsoTrkLVec.SetPtEtaPhiE(isoTrkpt, isoTrketa, isoTrkphi, isoTrkenergy);
     loose_isoTrksLVec->push_back(perIsoTrkLVec);

     double mtw = sqrt( 2*( (*met)[0].pt()*(*loose_isoTrksHandle_)[is].pt() -( (*met)[0].px()*(*loose_isoTrksHandle_)[is].px() + (*met)[0].py()*(*loose_isoTrksHandle_)[is].py() ) ) );

     loose_isoTrks_charge->push_back(isoTrkcharge);
     loose_isoTrks_dz->push_back((*loose_isotrk_dzpvVecHandle)[is]);
     loose_isoTrks_pdgId->push_back((*loose_isoTrksHandle_)[is].pdgId());
     loose_isoTrks_iso->push_back((*loose_isotrk_isoVecHandle)[is]);
     loose_isoTrks_mtw->push_back(mtw);

     if( debug_ ){
        std::cout<<"  --> is : "<<is<<"  pt/eta/phi/chg : "<<isoTrkpt<<"/"<<isoTrketa<<"/"<<isoTrkphi<<"/"<<isoTrkcharge<<"  mtw : "<<mtw<<"  pdgId : "<<(*loose_isoTrksHandle_)[is].pdgId()<<"  dz : "<<(*loose_isotrk_dzpvVecHandle)[is]<<"  iso/pt : "<<(*loose_isotrk_isoVecHandle)[is]/isoTrkpt<<std::endl;
     }
  }
  if( debug_ ) std::cout<<std::endl;

  iEvent.getByToken(PfCandTok_, pfCandHandle_);
  // store in the event

  std::unique_ptr<int>  loose_nIsoTrksPtr(new int);
  *loose_nIsoTrksPtr = loose_nIsoTrks;

  std::unique_ptr<int> nIsoTrksForVetoPtr(new int);
  *nIsoTrksForVetoPtr = nIsoTrksForVeto;
   
  //iEvent.put(std::move(trksForIsoVetoLVec), "trksForIsoVetoLVec");
  //iEvent.put(std::move(trksForIsoVeto_charge), "trksForIsoVetocharge");
  //iEvent.put(std::move(trksForIsoVeto_dz), "trksForIsoVetodz");
  //iEvent.put(std::move(trksForIsoVeto_pdgId), "trksForIsoVetopdgId");
  //iEvent.put(std::move(trksForIsoVeto_idx), "trksForIsoVetoidx");
  //iEvent.put(std::move(trksForIsoVeto_iso), "trksForIsoVetoiso");
  //iEvent.put(std::move(trksForIsoVeto_pfActivity), "trksForIsoVetopfActivity");

  //iEvent.put(std::move(loose_isoTrksLVec), "looseisoTrksLVec");
  //iEvent.put(std::move(loose_isoTrks_charge), "looseisoTrkscharge");
  //iEvent.put(std::move(loose_isoTrks_dz), "looseisoTrksdz");
  //iEvent.put(std::move(loose_isoTrks_pdgId), "looseisoTrkspdgId");
  //iEvent.put(std::move(loose_isoTrks_idx), "looseisoTrksidx");
  //iEvent.put(std::move(loose_isoTrks_iso), "looseisoTrksiso");
  //iEvent.put(std::move(loose_isoTrks_mtw), "looseisoTrksmtw");
  //iEvent.put(std::move(loose_isoTrks_pfActivity), "looseisoTrkspfActivity");

  //iEvent.put(std::move(forVetoIsoTrks_idx), "forVetoIsoTrksidx");

  //iEvent.put(std::move(loose_nIsoTrksPtr), "loosenIsoTrks");
  //iEvent.put(std::move(nIsoTrksForVetoPtr), "nIsoTrksForVeto");

  data.fill<TLorentzVector>(itrkIsoVeto_lVec_, std::move(trksForIsoVetoLVec));
  data.fill<double>(itrkIsoVeto_charge_, std::move(trksForIsoVetocharge));
  data.fill<double>(itrkIsoVeto_dz_, std::move(trksForIsoVetodz));
  data.fill<int>(itrkIsoVeto_pdgId_, std::move(trksForIsoVetopdgId));
  data.fill<int>(itrkIsoVeto_idx_, std::move(trksForIsoVetoidx));
  data.fill<double>(itrkIsoVeto_iso_, std::move(trksForIsoVetoiso));
  data.fill<double>(itrkIsoVeto_pfActivity_, std::move(trksForIsoVetopfActivity));
    
  data.fill<TLorentzVector>(ilooseIsoTrks_lVec_, std::move(looseisoTrksLVec));
  data.fill<double>(ilooseIsoTrks_charge_, std::move(looseisoTrkscharge));
  data.fill<double>(ilooseIsoTrks_dz_, std::move(looseisoTrksdz));
  data.fill<int>(ilooseIsoTrks_pdgId_, std::move(looseisoTrkspdgId));
  data.fill<int>(ilooseIsoTrks_idx_, std::move(looseisoTrksidx));
  data.fill<double>(ilooseIsoTrks_iso_, std::move(looseisoTrksiso));
  data.fill<double>(ilooseIsoTrks_mtw_, std::move(looseisoTrksmtw));
  data.fill<double>(ilooseIsoTrks_pfActivity_, std::move(looseisoTrkspfActivity));
   
  data.fill<int>(iforVetoIsoTrks_idx_, std::move(forVetoIsoTrksidx));
  
  data.fill<int>(iloosenIsoTrks_, std::move(loosenIsoTrks));
  data.fill<int>(inIsoTrksForVeto_, std::move(nIsoTrksForVeto));
  
  return true;
}

double prodIsoTrksFiller::GetTrackActivity(const edm::Handle<pat::PackedCandidateCollection> & other_pfcands, const pat::PackedCandidate* track) {
  if (track->pt()<5.) return -1.0;
  double trkiso(0.); 
  double r_iso = 0.3;
  for (const pat::PackedCandidate &other_pfc : *other_pfcands) {
      if (other_pfc.charge()==0) continue;
      double dr = deltaR(other_pfc, *track);
      if (dr < r_iso || dr > 0.4) continue; // activity annulus
      float dz_other = other_pfc.dz();
      if( fabs(dz_other) > 0.1 ) continue;
      trkiso += other_pfc.pt();
    }
    double activity = trkiso/track->pt();
    return activity;
}

//--------------------------------------------------------------------------------------------------
void prodIsoTrksFiller::load(const edm::Event& iEvent, const edm::EventSetup &iSetup)
{
  reset();
  iEvent.getByToken(pfCandToken_, pfcands_);
  iEvent.getByToken(tauToken_, taus_);
  isLoaded_ = true;
}

//--------------------------------------------------------------------------------------------------
void prodIsoTrksFiller::fill()
{
  
  filter(iEvent, iSetup);
  isFilled_ = true;
}
