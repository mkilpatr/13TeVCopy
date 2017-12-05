#include "AnalysisBase/Analyzer/interface/prodIsoTrksFiller.h"


using namespace ucsbsusy;

//--------------------------------------------------------------------------------------------------
prodIsoTrksFiller::prodIsoTrksFiller(const edm::ParameterSet& cfg, edm::ConsumesCollector && cc, const int options, const string branchName, EventInfoFiller * evtInfoFiller) :
  BaseFiller(options, branchName),
  evtInfoFiller_(evtInfoFiller),
  MetTok_       			(cc.consumes<edm::View<reco::MET>>          (cfg.getParameter<edm::InputTag>("metSrc"))),
  Loose_IsoTrksHandle_Tok_		(cc.consumes<pat::PackedCandidateCollection>(cfg.getParameter<edm::InputTag>("loose_isoTrkSrc"))),
  ForVetoIsoTrks_Tok_			(cc.consumes<pat::PackedCandidateCollection>(cfg.getParameter<edm::InputTag>("forVetoIsoTrkSrc"))),
  Loose_Isotrk_IsoVecHandle_Tok_	(cc.consumes<std::vector<double>>(cfg.getParameter<edm::InputTag>("loose_isotrk_isoVecSrc"))),
  Loose_Isotrk_DzpvVecHandle_Tok_	(cc.consumes<std::vector<double>>(cfg.getParameter<edm::InputTag>("loose_isotrk_dzpvVecSrc")))
{

  ilooseIsoTrks_pt_              = data.addMulti<float>(branchName_,"looseIsoTrks_pt",0);
  ilooseIsoTrks_eta_              = data.addMulti<float>(branchName_,"looseIsoTrks_eta",0);
  ilooseIsoTrks_phi_              = data.addMulti<float>(branchName_,"looseIsoTrks_phi",0);
  ilooseIsoTrks_mass_              = data.addMulti<float>(branchName_,"looseIsoTrks_mass",0);
  ilooseIsoTrks_charge_            = data.addMulti<float>(branchName_,"looseIsoTrks_charge",0);
  ilooseIsoTrks_dz_                = data.addMulti<float>(branchName_,"looseIsoTrks_dz",0);
  ilooseIsoTrks_pdgId_             = data.addMulti<float>(branchName_,"looseIsoTrks_pdgId",0);
  ilooseIsoTrks_idx_               = data.addMulti<float>(branchName_,"looseIsoTrks_idx",0);
  ilooseIsoTrks_iso_               = data.addMulti<float>(branchName_,"looseIsoTrks_iso",0);
  ilooseIsoTrks_mtw_               = data.addMulti<float>(branchName_,"looseIsoTrks_mtw",0);
  ilooseIsoTrks_pfActivity_        = data.addMulti<float>(branchName_,"looseIsoTrks_pfActivity",0);

  iforVetoIsoTrks_idx_             = data.addMulti<float>(branchName_,"forVetoIsoTrks_idx",0);

  iloosenIsoTrks_		   = data.add<unsigned int>(branchName_,"loosenIsoTrks","i",0);
  inIsoTrksForVeto_		   = data.add<unsigned int>(branchName_,"nIsoTrksForVeto","i",0);

}


//double prodIsoTrksFiller::GetTrackActivity(const edm::Handle<pat::PackedCandidateCollection> & other_pfcands, const pat::PackedCandidate* track) {
//  if (track->pt()<5.) return -1.0;
//  double trkiso(0.); 
//  double r_iso = 0.3;
//  for (const pat::PackedCandidate &other_pfc : *other_pfcands) {
//      if (other_pfc.charge()==0) continue;
//      double dr = deltaR(other_pfc, *track);
//      if (dr < r_iso || dr > 0.4) continue; // activity annulus
//      float dz_other = other_pfc.dz();
//      if( fabs(dz_other) > 0.1 ) continue;
//      trkiso += other_pfc.pt();
//    }
//    double activity = trkiso/track->pt();
//    return activity;
//}

//--------------------------------------------------------------------------------------------------
void prodIsoTrksFiller::load(const edm::Event& iEvent, const edm::EventSetup &iSetup)
{
  reset();
  iEvent.getByToken(MetTok_, met);
  iEvent.getByToken(Loose_IsoTrksHandle_Tok_, loose_isoTrksHandle_); 
  iEvent.getByToken(ForVetoIsoTrks_Tok_, forVetoIsoTrks_); 
  iEvent.getByToken(Loose_Isotrk_IsoVecHandle_Tok_, loose_isotrk_isoVecHandle);
  iEvent.getByToken(Loose_Isotrk_DzpvVecHandle_Tok_, loose_isotrk_dzpvVecHandle);
  isLoaded_ = true;
}

//--------------------------------------------------------------------------------------------------
void prodIsoTrksFiller::fill()
{
  
  std::vector<float>  loose_isoTrks_pt;
  std::vector<float>  loose_isoTrks_eta;
  std::vector<float>  loose_isoTrks_phi;
  std::vector<float>  loose_isoTrks_mass;
  std::vector<double>  loose_isoTrks_charge;
  std::vector<double>  loose_isoTrks_dz;
  std::vector<int>  loose_isoTrks_pdgId;
  std::vector<int>  loose_isoTrks_idx;
  std::vector<double>  loose_isoTrks_iso;
  std::vector<double>  loose_isoTrks_mtw;
  std::vector<double>  loose_isoTrks_pfActivity;

  std::vector<int>  forVetoIsoTrks_idx;

  if( loose_isoTrksHandle_.isValid() ) loose_nIsoTrks = loose_isoTrksHandle_->size(); else loose_nIsoTrks =0;
  if( forVetoIsoTrks_.isValid() ) nIsoTrksForVeto = forVetoIsoTrks_->size(); else nIsoTrksForVeto =0;

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

     //TLorentzVector perIsoTrkLVec;
     //perIsoTrkLVec.SetPtEtaPhiE(isoTrkpt, isoTrketa, isoTrkphi, isoTrkenergy);
     //loose_isoTrksLVec.push_back(perIsoTrkLVec);
     loose_isoTrks_pt.push_back(isoTrkpt);
     loose_isoTrks_eta.push_back(isoTrketa);
     loose_isoTrks_phi.push_back(isoTrkphi);
     loose_isoTrks_mass.push_back(isoTrkenergy);

     double mtw = sqrt( 2*( (*met)[0].pt()*(*loose_isoTrksHandle_)[is].pt() -( (*met)[0].px()*(*loose_isoTrksHandle_)[is].px() + (*met)[0].py()*(*loose_isoTrksHandle_)[is].py() ) ) );

     loose_isoTrks_charge.push_back(isoTrkcharge);
     loose_isoTrks_dz.push_back((*loose_isotrk_dzpvVecHandle)[is]);
     loose_isoTrks_pdgId.push_back((*loose_isoTrksHandle_)[is].pdgId());
     loose_isoTrks_iso.push_back((*loose_isotrk_isoVecHandle)[is]);
     loose_isoTrks_mtw.push_back(mtw);

     if( debug_ ){
        std::cout<<"  --> is : "<<is<<"  pt/eta/phi/chg : "<<isoTrkpt<<"/"<<isoTrketa<<"/"<<isoTrkphi<<"/"<<isoTrkcharge<<"  mtw : "<<mtw<<"  pdgId : "<<(*loose_isoTrksHandle_)[is].pdgId()<<"  dz : "<<(*loose_isotrk_dzpvVecHandle)[is]<<"  iso/pt : "<<(*loose_isotrk_isoVecHandle)[is]/isoTrkpt<<std::endl;
     }
  }
  if( debug_ ) std::cout<<std::endl;
  // store in the event

  //std::unique_ptr<int>  loose_nIsoTrksPtr(new int);
  //unsigned int loose_nIsoTrksPtr = loose_nIsoTrks;

  //std::unique_ptr<int> nIsoTrksForVetoPtr(new int);
  //unsigned int nIsoTrksForVetoPtr = nIsoTrksForVeto;
  
  for (unsigned int i = 0; i != loose_isoTrks_pt.size(); i++){
    data.fillMulti<float>(ilooseIsoTrks_pt_, loose_isoTrks_pt[i]);
    data.fillMulti<float>(ilooseIsoTrks_eta_, loose_isoTrks_eta[i]);
    data.fillMulti<float>(ilooseIsoTrks_phi_, loose_isoTrks_phi[i]);
    data.fillMulti<float>(ilooseIsoTrks_mass_, loose_isoTrks_mass[i]);
    data.fillMulti<double>(ilooseIsoTrks_charge_, loose_isoTrks_charge[i]);
    data.fillMulti<double>(ilooseIsoTrks_dz_, loose_isoTrks_dz[i]);
    data.fillMulti<int>(ilooseIsoTrks_pdgId_, loose_isoTrks_pdgId[i]);
    data.fillMulti<int>(ilooseIsoTrks_idx_, loose_isoTrks_idx[i]);
    data.fillMulti<double>(ilooseIsoTrks_iso_, loose_isoTrks_iso[i]);
    data.fillMulti<double>(ilooseIsoTrks_mtw_, loose_isoTrks_mtw[i]);
    data.fillMulti<double>(ilooseIsoTrks_pfActivity_, loose_isoTrks_pfActivity[i]);
     
    data.fillMulti<int>(iforVetoIsoTrks_idx_, forVetoIsoTrks_idx[i]);
  }
  data.fill<unsigned int>(iloosenIsoTrks_, loose_nIsoTrks);
  data.fill<unsigned int>(inIsoTrksForVeto_, nIsoTrksForVeto);
  
  isFilled_ = true;
}
