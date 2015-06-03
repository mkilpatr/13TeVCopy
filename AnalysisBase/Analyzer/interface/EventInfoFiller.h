//--------------------------------------------------------------------------------------------------
// 
// EventInfoFiller
// 
// Simple class to write some event info into a TTree.
// 
// EventInfoFiller.h created on Mon Aug 11 10:23:46 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISBASE_ANALYZER_EVENTINFOFILLER_H
#define ANALYSISBASE_ANALYZER_EVENTINFOFILLER_H

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/MET.h"

#include "AnalysisBase/Analyzer/interface/BaseFiller.h"

namespace ucsbsusy {

  struct EventCoords {
    EventCoords() : run(0),lumi(0),event(0) {}
    unsigned int run;
    unsigned int lumi;
    unsigned int event;
  };

  class EventInfoFiller : public BaseFiller {

    public:
      EventInfoFiller(
          const edm::InputTag vtxTag,
          const edm::InputTag rhoTag,
          const edm::InputTag metTag
          );

      ~EventInfoFiller() {};

      void load(const edm::Event& iEvent);
      void fill();

      reco::Vertex::Point primaryVertex() const { return primaryVertexIndex_ >= 0 ? (*vertices_)[primaryVertexIndex_].position() : reco::Vertex::Point(); }
      const pat::MET*     met()           const { return met_; }

    private:
      // Input from the config file
      const edm::InputTag vtxTag_;
      const edm::InputTag rhoTag_;
      const edm::InputTag metTag_;

      // Members to hold index of most recently filled tree data
      size irun_      ;
      size ilumi_     ;
      size ievent_    ;
      size inpv_      ;
      size irho_      ;
      size ipvx_      ;
      size ipvy_      ;
      size ipvz_      ;
      size imetpt_    ;
      size imetphi_   ;
      size imetsumEt_ ;
      size igenmetpt_ ;
      size igenmetphi_;
      size igoodvertex_;
    public:
      // Data members
      EventCoords    eventCoords;
      int            primaryVertexIndex_;
      const pat::MET *met_;
      edm::Handle<reco::VertexCollection> vertices_;
      edm::Handle<double>                 rho_;
      edm::Handle<pat::METCollection>     mets_;

  };

}

#endif
