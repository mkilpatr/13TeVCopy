//--------------------------------------------------------------------------------------------------
// 
// prodIsoTrksFiller
// 
// Class to fill hadronic tau information in a TTree. NB: MiniAOD stores taus from hpsPFprodIsoTrksProducer with pT > 18 GeV, and passing the basic decayModeFinding id.
// 
// prodIsoTrksFiller.h created on Wed Oct 22 14:12:52 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISBASE_ANALYZER_PRODISOTRKSFILLER_H
#define ANALYSISBASE_ANALYZER_PRODISOTRKSFILLER_H

#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

#include "AnalysisBase/Analyzer/interface/BaseFiller.h"
#include "AnalysisBase/Analyzer/interface/EventInfoFiller.h"
#include "AnalysisTools/Utilities/interface/TreeWriterData.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisTools/DataFormats/interface/Momentum.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"

#include <memory>
#include <algorithm>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "DataFormats/JetReco/interface/BasicJet.h"
#include "DataFormats/JetReco/interface/BasicJetCollection.h"

#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/GenMET.h"

#include "DataFormats/PatCandidates/interface/Jet.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include "TLorentzVector.h"

// additional headers
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/Provenance/interface/EventAuxiliary.h"
#include "TString.h"
#include "TTree.h"
#include <TFile.h>
#include <vector>
#include "Math/GenVector/LorentzVector.h"
#include <DataFormats/Math/interface/deltaR.h>
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/Candidate/interface/Candidate.h"
//#include "DataFormats/PatCandidates/interface/IsolatedTrack.h"
#include "DataFormats/PatCandidates/interface/PFIsolation.h"

#include "AnalysisBase/Analyzer/interface/PhysicsAnalyzer.h"

namespace ucsbsusy {

  class prodIsoTrksFiller : public BaseFiller {

    public :
      prodIsoTrksFiller(const edm::ParameterSet& cfg, edm::ConsumesCollector && cc, const int options, const string branchName, EventInfoFiller * evtInfoFiller);
      ~prodIsoTrksFiller() {}

      enum Options {
                    NULLOPT      = 0,
		    SAVEALLCANDS = (1 << 0), // load extra filters produced in "PAT" step (for data the MET filters are produced in "RECO" step by default; MC is always "PAT")
		    FILLPRODISOTRKS = (1 << 1), //whether or not to store the prodIsoTrks Variables
                   };
      static const int defaultOptions = NULLOPT;

      void load(const edm::Event& iEvent, const edm::EventSetup &iSetup);
      void fill();
      //double GetTrackActivity(const edm::Handle<pat::PackedCandidateCollection> & other_pfcands, const pat::PackedCandidate* track);

    private :
      const EventInfoFiller * evtInfoFiller_;
      edm::EDGetTokenT< std::vector<reco::Vertex> >VtxTok_;
      edm::EDGetTokenT<edm::View<reco::MET> >MetTok_;
      edm::EDGetTokenT<pat::PackedCandidateCollection> Loose_IsoTrksHandle_Tok_;
      edm::EDGetTokenT<pat::PackedCandidateCollection> ForVetoIsoTrks_Tok_;
      edm::EDGetTokenT<std::vector<double> >  Loose_Isotrk_IsoVecHandle_Tok_;
      edm::EDGetTokenT<std::vector<double> >Loose_Isotrk_DzpvVecHandle_Tok_;

      // Members to hold indices of tree data
      size ilooseIsoTrks_pt_;             
      size ilooseIsoTrks_eta_;             
      size ilooseIsoTrks_phi_;             
      size ilooseIsoTrks_mass_;             
      size ilooseIsoTrks_charge_;           
      size ilooseIsoTrks_dz_;               
      size ilooseIsoTrks_pdgId_;            
      size ilooseIsoTrks_idx_;              
      size ilooseIsoTrks_iso_;              
      size ilooseIsoTrks_mtw_;              
      size ilooseIsoTrks_pfActivity_;       
      
      size iforVetoIsoTrks_idx_;           
      
      size iloosenIsoTrks_;                
      size inIsoTrksForVeto_;              

      std::vector<int> exclPdgIdVec_;
      double dR_, dzcut_;
      double minPt_, isoCut_;
      unsigned int loose_nIsoTrks, nIsoTrksForVeto;

    public :
      // Data members
      edm::InputTag pfCandSrc_, loose_isoTrkSrc_, loose_isotrk_isoVecSrc_, loose_isotrk_dzpvVecSrc_;
      edm::Handle< std::vector<reco::Vertex> > vertices;
      edm::Handle<edm::View<reco::MET> > met;
      edm::Handle<std::vector<double> >  loose_isotrk_isoVecHandle, loose_isotrk_dzpvVecHandle; 
 
      bool debug_;
  
      int find_idx(const reco::Candidate & target);
      int find_idx(int genIdx, const std::vector<int> &genDecayIdxVec);
  
      bool isData_;
   
      edm::Handle<pat::PackedCandidateCollection> loose_isoTrksHandle_;
      edm::Handle<pat::PackedCandidateCollection> forVetoIsoTrks_; 

  };

}

#endif
