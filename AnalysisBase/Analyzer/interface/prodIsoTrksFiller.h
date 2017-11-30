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
#include "AnalysisTools/ObjectSelection/interface/TauMVA.h"

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
      double GetTrackActivity(const edm::Handle<pat::PackedCandidateCollection> & other_pfcands, const pat::PackedCandidate* track);
      virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);

    private :
      const EventInfoFiller * evtInfoFiller_;
      edm::EDGetTokenT<pat::TauCollection>             tauToken_;
      const double        candptMin_;
      const double        candetaMax_;

      // Members to hold indices of tree data
      size itrkIsoVeto_lVec_;               
      size itrkIsoVeto_charge_;             
      size itrkIsoVeto_dz_;                 
      size itrkIsoVeto_pdgId_;              
      size itrkIsoVeto_idx_;                
      size itrkIsoVeto_iso_;                
      size itrkIsoVeto_pfActivity_;         
      
      size ilooseIsoTrks_lVec_;             
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

    public :
      // Data members

      edm::InputTag vtxSrc_, metSrc_;

      edm::InputTag forVetoIsoTrkSrc_;
      edm::Handle<pat::PackedCandidateCollection> forVetoIsoTrks_;
  
      double isotrk_dR_, isotrk_dz_;
  
      edm::InputTag pfCandSrc_, loose_isoTrkSrc_, loose_isotrk_isoVecSrc_, loose_isotrk_dzpvVecSrc_;
      edm::Handle<pat::PackedCandidateCollection> pfCandHandle_, loose_isoTrksHandle_;
  
      //edm::InputTag ref_all_isoTrkSrc_, ref_all_isoTrk_isoVecSrc_;
      //edm::Handle<pat::PackedCandidateCollection> ref_all_isoTrksHandle_;
      //edm::Handle<std::vector<double> > ref_all_isoTrks_isoVecHandle_;
  
      //edm::InputTag W_emuVec_Src_, W_tau_emuVec_Src_, W_tau_prongsVec_Src_;
      //edm::Handle<std::vector<int> > W_emuVec_, W_tau_emuVec_, W_tau_prongsVec_;
  
      //edm::InputTag genDecayLVec_Src_;
      //edm::Handle<std::vector<TLorentzVector> > genDecayLVec_;
  
      //edm::EDGetTokenT<std::vector<int> >W_EmVec_Tok_;
      //edm::EDGetTokenT<std::vector<int> >W_Tau_EmuVec_Tok_;
      //edm::EDGetTokenT<std::vector<int> >W_Tau_ProngsVec_Tok_;
      //edm::EDGetTokenT<std::vector<TLorentzVector> >GenDecayLVec_Tok_;
      edm::EDGetTokenT<pat::PackedCandidateCollection>PfCandTok_;
      edm::EDGetTokenT<pat::PackedCandidateCollection> Loose_IsoTrksHandle_Tok_;
      //edm::EDGetTokenT<pat::PackedCandidateCollection> Ref_All_IsoTrksHandle_Tok_;
      //edm::EDGetTokenT<std::vector<double> > Ref_All_IsoTrks_IsoVecHandle_Tok_;
      edm::EDGetTokenT< std::vector<reco::Vertex> >VtxTok_;
      edm::EDGetTokenT<edm::View<reco::MET> >MetTok_;
      edm::EDGetTokenT<std::vector<double> >  Loose_Isotrk_IsoVecHandle_Tok_;
      edm::EDGetTokenT<std::vector<double> >Loose_Isotrk_DzpvVecHandle_Tok_;
      edm::EDGetTokenT<pat::PackedCandidateCollection> ForVetoIsoTrks_Tok_;
  
      unsigned int loose_nIsoTrks, nIsoTrksForVeto;
  
      bool debug_;
  
      int find_idx(const reco::Candidate & target);
      int find_idx(int genIdx, const std::vector<int> &genDecayIdxVec);
  
      bool isData_;
      edm::Handle<std::vector<TLorentzVector>> trksForIsoVetoLVec_;
      edm::Handle<std::vector<double>> trksForIsoVetocharge_;
      edm::Handle<std::vector<double>> trksForIsoVetodz_;
      edm::Handle<std::vector<int>> trksForIsoVetopdgId_;
      edm::Handle<std::vector<int>> trksForIsoVetoidx_;
      edm::Handle<std::vector<double>> trksForIsoVetoiso_;
      edm::Handle<std::vector<double>> trksForIsoVetopfActivity_;
    
      edm::Handle<std::vector<TLorentzVector>> looseisoTrksLVec_;
      edm::Handle<std::vector<double>> looseisoTrkscharge_;
      edm::Handle<std::vector<double>> looseisoTrksdz_;
      edm::Handle<std::vector<int>> looseisoTrkspdgId_;
      edm::Handle<std::vector<int>> looseisoTrksidx_;
      edm::Handle<std::vector<double>> looseisoTrksiso_;
      edm::Handle<std::vector<double>> looseisoTrksmtw_;
      edm::Handle<std::vector<double>> looseisoTrkspfActivity_;
    
      edm::Handle<std::vector<int>> forVetoIsoTrksidx_;
    
      edm::Handle<int> loosenIsoTrks_;
      edm::Handle<int> nIsoTrksForVeto_;

      edm::Handle<pat::TauCollection> taus_;

  };

}

#endif
