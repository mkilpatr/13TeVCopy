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

#include "DataFormats/PatCandidates/interface/Tau.h"

#include "AnalysisTools/Utilities/interface/Types.h"

#include "AnalysisBase/Analyzer/interface/BaseFiller.h"
#include "AnalysisBase/Analyzer/interface/EventInfoFiller.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"

namespace ucsbsusy {

  class prodIsoTrksFiller : public BaseFiller {

    public :
      prodIsoTrksFiller(const edm::ParameterSet& cfg, edm::ConsumesCollector && cc, const int options, const string branchName, EventInfoFiller * evtInfoFiller);
      ~prodIsoTrksFiller() {}

      enum Options {
                    NULLOPT      = 0,
		    LOADPATFILTERS = (1 << 0), // load extra filters produced in "PAT" step (for data the MET filters are produced in "RECO" step by default; MC is always "PAT")
                   };
      static const int defaultOptions = NULLOPT;

      void load(const edm::Event& iEvent, const edm::EventSetup &iSetup);
      void fill();
      void initTriggerNames();

    private :
      edm::EDGetTokenT<edm::TriggerResults>  triggerBitToken_;
      const edm::TriggerNames *              triggerNames_;
      edm::EDGetTokenT<edm::TriggerResults>  patTriggerBitToken_;
      const edm::TriggerNames *              patTriggerNames_;
      const EventInfoFiller * evtInfoFiller_;
      size itrig_bit_pass;

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

      edm::Handle<edm::TriggerResults> triggerBits_;
      edm::Handle<edm::TriggerResults> patTriggerBits_;

  };

}

#endif
