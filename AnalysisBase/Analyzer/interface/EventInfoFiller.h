//--------------------------------------------------------------------------------------------------
// 
// EventInfoFiller
// 
// Simple class to write some event info into a TTree. To be tested with MiniAOD for now. Sophisticated data management methods to be added later :-)
// 
// EventInfoFiller.h created on Mon Aug 11 10:23:46 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISBASE_ANALYZER_EVENTINFOFILLER_H
#define ANALYSISBASE_ANALYZER_EVENTINFOFILLER_H

#include "TFile.h"
#include "TH1F.h"
#include "TTree.h"
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

    protected:
      TFile *outFile;
      TH1F  *nevtHist;
      TTree *evtTree;

    private:
      void beginJob();
      void analyze(const edm::Event&, const edm::EventSetup&);
      void endJob();

      edm::EDGetTokenT<reco::VertexCollection> vtxToken_;
      edm::EDGetTokenT<pat::METCollection> metToken_;
      std::string outputName_;
      int nEvts_;
      EventInfo *evtInfo_;

  };
}

#endif
