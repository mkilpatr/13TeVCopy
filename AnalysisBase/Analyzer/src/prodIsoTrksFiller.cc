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
  dR_					(cfg.getParameter<double>          	    ("dR_ConeSize")),
  dzcut_				(cfg.getParameter<double>          	    ("dz_CutValue")),
  minPt_				(cfg.getParameter<double>		    ("minPt_PFCandidate")),
  isoCut_				(cfg.getParameter<double>		    ("isoCut")),	
  debug_				(cfg.getParameter<bool>			    ("debug"))
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

//--------------------------------------------------------------------------------------------------
void prodIsoTrksFiller::load(const edm::Event& iEvent, const edm::EventSetup &iSetup)
{
  reset();
  iEvent.getByToken(VtxTok_, vertices);
  iEvent.getByToken(MetTok_, met);
  iEvent.getByToken(Loose_IsoTrksHandle_Tok_, loose_isoTrksHandle_); 
  iEvent.getByToken(ForVetoIsoTrks_Tok_, forVetoIsoTrks_); 
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

  if( vertices->size() > 0) {
    if( debug_ ) std::cout<<"\nloose_nIsoTrks : "<<loose_nIsoTrks<<"  nIsoTrksForVeto : "<<nIsoTrksForVeto<<std::endl;
    
    for(unsigned int is=0; is<loose_nIsoTrks; is++){
       const pat::PackedCandidate isoTrk = (*loose_isoTrksHandle_)[is];

       if( isoTrk.charge() == 0 ) continue;
       if( std::isnan(isoTrk.pt()) || std::isinf(isoTrk.pt()) ) continue;
       if( isoTrk.pt() < minPt_ ) continue;

       if(isoTrk.charge() != 0){
         double trkiso = 0.0;
         for(unsigned int iP=0; iP<loose_nIsoTrks; iP++){
            const pat::PackedCandidate isoTrk_other = (*loose_isoTrksHandle_)[iP];

            // don't count the PFCandidate in its own isolation sum
            if( is == iP ) continue;
            if( isoTrk_other.charge() == 0 ) continue;

            // cut on dR between the PFCandidates
            double dR = PhysicsUtilities::deltaR(isoTrk.eta(), isoTrk.phi(), isoTrk_other.eta(), isoTrk_other.phi());
            if( dR > dR_ ) continue;

            // cut on the PFCandidate dz
            double dz_other = isoTrk_other.dz();
            if( fabs(dz_other) > dzcut_ ) continue;
            if( std::find( exclPdgIdVec_.begin(), exclPdgIdVec_.end(), isoTrk_other.pdgId() ) != exclPdgIdVec_.end() ) continue;

            trkiso += isoTrk_other.pt();
         }

         float pat::PFIsolation isolate = isoTrk.chargedHadronIso();
	 if(!((isoTrk.pt()>5 && (fabs(isoTrk.pdgId()) == 11 || fabs(isoTrk.pdgId()) == 13)) || isoTrk.pt() > 10)) continue; 
         if(!(fabs(isoTrk.pdgId()) < 15 || fabs(isoTrk.eta()) < 2.5)) continue;
         if(!(fabs(isoTrk.dxy() < 0.2))) continue;
         std::cout << "chargedHadronIso: " << isoTrk.chargedHadronIso() << std::endl;
	 //if(!((pat::IsolatedTrack::pfIsolationDR03().chargedHadronIso < 5 && isoTrk.pt() < 25) || pat::IsolatedTrack::pfIsolationDR03().chargedHadronIso/isoTrk.pt() < 0.2)) continue;
	 if( trkiso/isoTrk.pt() > isoCut_ ) continue;
         if( std::abs(isoTrk.dz()) > dzcut_ ) continue;

         double mtw = sqrt( 2*( (*met)[0].pt()*isoTrk.pt() -( (*met)[0].px()*isoTrk.px() + (*met)[0].py()*isoTrk.py() ) ) );

         loose_isoTrks_pt.push_back(isoTrk.pt());
         loose_isoTrks_eta.push_back(isoTrk.eta());
         loose_isoTrks_phi.push_back(isoTrk.phi());
         loose_isoTrks_mass.push_back(isoTrk.energy());
         loose_isoTrks_charge.push_back(isoTrk.charge());
         loose_isoTrks_dz.push_back(isoTrk.dz());
         loose_isoTrks_pdgId.push_back(isoTrk.pdgId());
         loose_isoTrks_iso.push_back(trkiso);
         loose_isoTrks_mtw.push_back(mtw);

         if( debug_ ){
            std::cout<<"  --> is : "<<is<<"  pt/eta/phi/chg : "<<isoTrk.pt()<<"/"<<isoTrk.eta()<<"/"<<isoTrk.phi()<<"/"<<isoTrk.charge()<<"  mtw : "<<mtw<<"  pdgId : "<<(*loose_isoTrksHandle_)[is].pdgId()<<"  dz : " << isoTrk.dz()<<"  iso/pt : "<<trkiso/isoTrk.pt()<<std::endl;
         }
       }else{
             //neutral particle, set trkiso and dzpv to 9999
       }
    }
    if( debug_ ) std::cout<<std::endl;
    for (unsigned int i = 0; i != loose_isoTrks_dz.size(); i++){
      data.fillMulti<float>(ilooseIsoTrks_pt_, loose_isoTrks_pt[i]);
      data.fillMulti<float>(ilooseIsoTrks_eta_, loose_isoTrks_eta[i]);
      data.fillMulti<float>(ilooseIsoTrks_phi_, loose_isoTrks_phi[i]);
      data.fillMulti<float>(ilooseIsoTrks_mass_, loose_isoTrks_mass[i]);
      data.fillMulti<double>(ilooseIsoTrks_charge_, loose_isoTrks_charge[i]);
      data.fillMulti<double>(ilooseIsoTrks_dz_, loose_isoTrks_dz[i]);
      data.fillMulti<int>(ilooseIsoTrks_pdgId_, loose_isoTrks_pdgId[i]);
      data.fillMulti<double>(ilooseIsoTrks_iso_, loose_isoTrks_iso[i]);
      data.fillMulti<double>(ilooseIsoTrks_mtw_, loose_isoTrks_mtw[i]);
      if(i == 0) data.fill<unsigned int>(iloosenIsoTrks_, loose_nIsoTrks);
      if(i == 0) data.fill<unsigned int>(inIsoTrksForVeto_, nIsoTrksForVeto);
    }
    isFilled_ = true;
  }
}
