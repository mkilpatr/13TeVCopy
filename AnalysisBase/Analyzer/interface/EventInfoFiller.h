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
#include "AnalysisTools/DataFormats/interface/EventInfo.h"


namespace ucsbsusy {

  class EventInfoFiller : public BaseFiller {
    public:
      EventInfoFiller(const edm::ParameterSet &cfg);
      ~EventInfoFiller();

      void load(edm::Event& iEvent, bool storeOnlyPtr = false);
      void fill(Planter& plant, int& bookMark);
      EventInfo& operator->(){return *obj;}

      //--------------------------------------------------------------------------------------------------
      // Input from the config file
      //--------------------------------------------------------------------------------------------------
      edm::InputTag vtxTag_;
      edm::InputTag metTag_;

      //--------------------------------------------------------------------------------------------------
      // Data Members
      //--------------------------------------------------------------------------------------------------
      EventInfo                    * obj;
      const reco::VertexCollection * verticies_;
      const reco::Vertex           * primaryVerex_;
      const pat::METCollection     * mets_;
      const pat::MET               * met_;
  };
}

#endif
