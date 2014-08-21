//--------------------------------------------------------------------------------------------------
// 
// TestAnalyzer
// 
// Test class to try out event analysis modules. The information you want to analyze and store in the output TTree is configurable and implemented by means of the different Filler analyzer classes. Filtering functionality to be added. To be tested with MiniAOD for now. Sophisticated data management methods to be added later :-)
// 
// TestAnalyzer.h created on Fri Aug 15 14:57:13 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISBASE_ANALYZER_TESTANALYZER_H
#define ANALYSISBASE_ANALYZER_TESTANALYZER_H

#include <TFile.h>
#include <TH1F.h>
#include <TTree.h>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "AnalysisBase/Analyzer/interface/EventInfoFiller.h"
#include "AnalysisBase/Analyzer/interface/JetFiller.h"

namespace ucsbsusy {
  class BaseAnalyzer;

  class TestAnalyzer : public edm::EDAnalyzer {
    public:
      TestAnalyzer(const edm::ParameterSet&);
      ~TestAnalyzer();

      void beginJob();
      void analyze(const edm::Event&, const edm::EventSetup&);
      void endJob();

    protected:
      void addAnalyzer(BaseAnalyzer *ana);

      TFile *outFile;
      TH1F  *nevtHist;
      TTree *evtTree;

    private:
      std::vector<BaseAnalyzer*> analyzers;
      std::string outtreeName_;
      std::string outputName_;
      std::vector<std::string> analyzerList_;
      int nEvts_;

      edm::EDGetTokenT<reco::VertexCollection> vtxToken_;
      edm::EDGetTokenT<pat::METCollection> metToken_;
      edm::EDGetTokenT<pat::JetCollection> jetToken_;

      void book();

  };
}

#endif
