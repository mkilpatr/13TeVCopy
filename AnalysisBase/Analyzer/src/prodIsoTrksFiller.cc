#include "AnalysisBase/Analyzer/interface/prodIsoTrksFiller.h"


using namespace ucsbsusy;

//--------------------------------------------------------------------------------------------------
prodIsoTrksFiller::prodIsoTrksFiller(const edm::ParameterSet& cfg, edm::ConsumesCollector && cc, const int options, const string branchName, EventInfoFiller * evtInfoFiller) :
  BaseFiller(options, branchName),
  evtInfoFiller_(evtInfoFiller),
  VtxTok_				(cc.consumes<std::vector<reco::Vertex>>	    (cfg.getParameter<edm::InputTag>("vtxSrc"))),
  MetTok_       			(cc.consumes<edm::View<reco::MET>>          (cfg.getParameter<edm::InputTag>("metSrc"))),
  Loose_IsoTrksHandle_Tok_		(cc.consumes<pat::PackedCandidateCollection>(cfg.getParameter<edm::InputTag>("loose_isoTrkSrc"))),
  ForVetoIsoTrks_Tok_			(cc.consumes<pat::PackedCandidateCollection>(cfg.getParameter<edm::InputTag>("forVetoIsoTrkSrc"))),
  exclPdgIdVec_				(cfg.getParameter<std::vector<int>>	    ("exclPdgIdVec")),
  isotrk_dR_				(cfg.getParameter<double>          	    ("isotrk_dR")),
  isotrk_dz_				(cfg.getParameter<double>          	    ("isotrk_dz")),
  debug_				(cfg.getParameter<bool>			    ("debug"))
//  Loose_Isotrk_IsoVecHandle_Tok_	(cc.consumes<std::vector<double>>(cfg.getParameter<edm::InputTag>("loose_isotrk_isoVecSrc")))
//  Loose_Isotrk_DzpvVecHandle_Tok_	(cc.consumes<std::vector<double>>(cfg.getParameter<edm::InputTag>("loose_isotrk_dzpvVecSrc")))
{

  ilooseIsoTrks_pt_              = data.addMulti<float>(branchName_,"looseIsoTrks_pt",0);
  ilooseIsoTrks_eta_              = data.addMulti<float>(branchName_,"looseIsoTrks_eta",0);
  ilooseIsoTrks_phi_              = data.addMulti<float>(branchName_,"looseIsoTrks_phi",0);
  ilooseIsoTrks_mass_              = data.addMulti<float>(branchName_,"looseIsoTrks_mass",0);
  ilooseIsoTrks_charge_            = data.addMulti<double>(branchName_,"looseIsoTrks_charge",0);
  ilooseIsoTrks_dz_                = data.addMulti<double>(branchName_,"looseIsoTrks_dz",0);
  ilooseIsoTrks_pdgId_             = data.addMulti<int>(branchName_,"looseIsoTrks_pdgId",0);
  ilooseIsoTrks_idx_               = data.addMulti<int>(branchName_,"looseIsoTrks_idx",0);
  ilooseIsoTrks_iso_               = data.addMulti<double>(branchName_,"looseIsoTrks_iso",0);
  ilooseIsoTrks_mtw_               = data.addMulti<double>(branchName_,"looseIsoTrks_mtw",0);
  ilooseIsoTrks_pfActivity_        = data.addMulti<double>(branchName_,"looseIsoTrks_pfActivity",0);

  iforVetoIsoTrks_idx_             = data.addMulti<int>(branchName_,"forVetoIsoTrks_idx",0);

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
  iEvent.getByToken(VtxTok_, vertices);
  iEvent.getByToken(MetTok_, met);
  iEvent.getByToken(Loose_IsoTrksHandle_Tok_, loose_isoTrksHandle_); 
  iEvent.getByToken(ForVetoIsoTrks_Tok_, forVetoIsoTrks_); 
//  iEvent.getByToken(Loose_Isotrk_IsoVecHandle_Tok_, loose_isotrk_isoVecHandle);
//  iEvent.getByToken(Loose_Isotrk_DzpvVecHandle_Tok_, loose_isotrk_dzpvVecHandle);
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

  if( loose_isoTrksHandle_.isValid() ){
     //if( loose_nIsoTrks != loose_isotrk_isoVecHandle->size() || loose_nIsoTrks != loose_isotrk_dzpvVecHandle->size() ){
     //   std::cout<<"ERROR ... mis-matching between loose_nIsoTrks : "<<loose_nIsoTrks<<"  loose_isotrk_isoVecHandle->size : "<<loose_isotrk_isoVecHandle->size()<<"  loose_isotrk_dzpvVecHandle->size : "<<loose_isotrk_dzpvVecHandle->size()<<std::endl;
     //}
  }

  if( debug_ ) std::cout<<"\nloose_nIsoTrks : "<<loose_nIsoTrks<<"  nIsoTrksForVeto : "<<nIsoTrksForVeto<<std::endl;
  for(unsigned int is=0; is<loose_nIsoTrks; is++){
     const pat::PackedCandidate isoTrk = (*loose_isoTrksHandle_)[is];
     double isoTrkpt = isoTrk.pt(), isoTrketa = isoTrk.eta(), isoTrkphi = isoTrk.phi(), isoTrkenergy = isoTrk.energy();
     double isoTrkcharge = isoTrk.charge();
     double isoTrkdz = isoTrk.dz();

     double trkiso = 0.0;
     for(unsigned int iP=0; iP<loose_nIsoTrks; iP++){
     //for( pat::PackedCandidate::const_iterator isoTrk_other = isoTrk.begin(); isoTrk_other != isoTrk.end(); isoTrk_other++ ) {
        const pat::PackedCandidate isoTrk_other = (*loose_isoTrksHandle_)[iP];

        // don't count the PFCandidate in its own isolation sum
        if( is == iP ) continue;

        // require the PFCandidate to be charged
        if( isoTrk_other.charge() == 0 ) continue;

        // cut on dR between the PFCandidates
        double dR = deltaR(isoTrk.eta(), isoTrk.phi(), isoTrk_other.eta(), isoTrk_other.phi());
        if( dR > isotrk_dR_ ) continue;

        // cut on the PFCandidate dz
        double dz_other = isoTrk_other.dz();

        if( fabs(dz_other) > isotrk_dz_ ) continue;

        if( std::find( exclPdgIdVec_.begin(), exclPdgIdVec_.end(), isoTrk_other.pdgId() ) != exclPdgIdVec_.end() ) continue;

        trkiso += isoTrk_other.pt();
     }

     //TLorentzVector perIsoTrkLVec;
     //perIsoTrkLVec.SetPtEtaPhiE(isoTrkpt, isoTrketa, isoTrkphi, isoTrkenergy);
     //loose_isoTrksLVec.push_back(perIsoTrkLVec);
     loose_isoTrks_pt.push_back(isoTrkpt);
     loose_isoTrks_eta.push_back(isoTrketa);
     loose_isoTrks_phi.push_back(isoTrkphi);
     loose_isoTrks_mass.push_back(isoTrkenergy);

     double mtw = sqrt( 2*( (*met)[0].pt()*isoTrk.pt() -( (*met)[0].px()*isoTrk.px() + (*met)[0].py()*isoTrk.py() ) ) );

     loose_isoTrks_charge.push_back(isoTrkcharge);
     loose_isoTrks_dz.push_back(isoTrkdz);
     loose_isoTrks_pdgId.push_back(isoTrk.pdgId());
     loose_isoTrks_iso.push_back(trkiso);
     loose_isoTrks_mtw.push_back(mtw);

     if( debug_ ){
        std::cout<<"  --> is : "<<is<<"  pt/eta/phi/chg : "<<isoTrkpt<<"/"<<isoTrketa<<"/"<<isoTrkphi<<"/"<<isoTrkcharge<<"  mtw : "<<mtw<<"  pdgId : "<<(*loose_isoTrksHandle_)[is].pdgId()<<"  dz : " << isoTrkdz<<"  iso/pt : "<<trkiso/isoTrkpt<<std::endl;
     }
  }
  if( debug_ ) std::cout<<std::endl;
  // store in the event

  //std::unique_ptr<int>  loose_nIsoTrksPtr(new int);
  //unsigned int loose_nIsoTrksPtr = loose_nIsoTrks;

  //std::unique_ptr<int> nIsoTrksForVetoPtr(new int);
  //unsigned int nIsoTrksForVetoPtr = nIsoTrksForVeto;
  
  for (unsigned int i = 0; i != loose_nIsoTrks; i++){
    std::cout << "Fill float" << endl;
    data.fillMulti<float>(ilooseIsoTrks_pt_, loose_isoTrks_pt[i]);
    data.fillMulti<float>(ilooseIsoTrks_eta_, loose_isoTrks_eta[i]);
    data.fillMulti<float>(ilooseIsoTrks_phi_, loose_isoTrks_phi[i]);
    data.fillMulti<float>(ilooseIsoTrks_mass_, loose_isoTrks_mass[i]);
    std::cout << "Fill double" << endl;
    data.fillMulti<double>(ilooseIsoTrks_charge_, loose_isoTrks_charge[i]);
    data.fillMulti<double>(ilooseIsoTrks_dz_, loose_isoTrks_dz[i]);
    std::cout << "Fill int" << endl;
    data.fillMulti<int>(ilooseIsoTrks_pdgId_, loose_isoTrks_pdgId[i]);
    //data.fillMulti<int>(ilooseIsoTrks_idx_, loose_isoTrks_idx[i]);
    std::cout << "Fill double" << endl;
    data.fillMulti<double>(ilooseIsoTrks_iso_, loose_isoTrks_iso[i]);
    data.fillMulti<double>(ilooseIsoTrks_mtw_, loose_isoTrks_mtw[i]);
    //data.fillMulti<double>(ilooseIsoTrks_pfActivity_, loose_isoTrks_pfActivity[i]);
     
    std::cout << "Fill int" << endl;
    //data.fillMulti<int>(iforVetoIsoTrks_idx_, forVetoIsoTrks_idx[i]);
    std::cout << "Fill prodIsoTrks vector" << endl;
  }
  data.fill<unsigned int>(iloosenIsoTrks_, loose_nIsoTrks);
  data.fill<unsigned int>(inIsoTrksForVeto_, nIsoTrksForVeto);
 
  std::cout << "end of filling" << endl; 
  isFilled_ = true;
}
