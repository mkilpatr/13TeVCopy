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
#include "AnalysisBase/Analyzer/interface/BaseAnalyzer.h"
#include "AnalysisTools/DataFormats/interface/EventInfo.h"


namespace ucsbsusy {

  class EventInfoFiller : public BaseAnalyzer {
    public:
      EventInfoFiller(const edm::ParameterSet &cfg);
      ~EventInfoFiller();

      void book(TTree &t);
      void fill(const edm::Event &e);

    private:
      edm::InputTag vtxTag_;
      edm::InputTag metTag_;
      EventInfo *evtInfo_;

  };
}

#endif
