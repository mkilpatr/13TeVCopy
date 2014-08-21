//--------------------------------------------------------------------------------------------------
// 
// TestAnalyzer
// 
// Test class to try out event analysis modules. The information you want to analyze and store in the output TTree is configurable and implemented by means of the different Filler analyzer classes. Filtering functionality to be added. To be tested with MiniAOD for now. Sophisticated data management methods to be added later :-)
// 
// TestAnalyzer.cc created on Fri Aug 15 14:57:13 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisBase/Analyzer/interface/TestAnalyzer.h"

using namespace ucsbsusy;
using namespace std;

//--------------------------------------------------------------------------------------------------
TestAnalyzer::TestAnalyzer(const edm::ParameterSet &cfg) :
  outtreeName_(cfg.getUntrackedParameter<string>("treename")),
  outputName_(cfg.getUntrackedParameter<string>("filename")),
  analyzerList_(cfg.getUntrackedParameter<vector<string> >("analyzers")),
  nEvts_(0)
{
  // Constructor

  for(UInt_t iana = 0; iana < analyzerList_.size(); iana++) {
    const string type(analyzerList_.at(iana));
    if(type.compare("EventInfoFiller")==0) {
      EventInfoFiller *eventInfoFiller = new EventInfoFiller(cfg);
      addAnalyzer(eventInfoFiller);
      continue;
    }
    if(type.compare("JetFiller")==0) {
      JetFiller *jetFiller = new JetFiller(cfg);
      addAnalyzer(jetFiller);
      continue;
    }
  }

}

//--------------------------------------------------------------------------------------------------
TestAnalyzer::~TestAnalyzer()
{
  // Destructor

}

//--------------------------------------------------------------------------------------------------
void TestAnalyzer::addAnalyzer(BaseAnalyzer *ana)
{
  // Add analyzer to the list

  analyzers.push_back(ana);

}

//--------------------------------------------------------------------------------------------------
void TestAnalyzer::beginJob()
{
  // Book branches for each analysis component

  outFile = new TFile(outputName_.c_str(),"RECREATE");
  nevtHist = new TH1F("NEvents","",1,0,1);
  evtTree = new TTree(outtreeName_.c_str(),outtreeName_.c_str());

  for(vector<BaseAnalyzer*>::iterator iana = analyzers.begin(); iana != analyzers.end(); ++iana) {
    (*iana)->book(*evtTree);
  }

}

//--------------------------------------------------------------------------------------------------
void TestAnalyzer::analyze(const edm::Event &event, const edm::EventSetup &setup)
{

  nEvts_++;
  nevtHist->Fill(0);

  for(vector<BaseAnalyzer*>::iterator iana = analyzers.begin(); iana != analyzers.end(); ++iana) {
    (*iana)->fill(event);
  }

  evtTree->Fill();

}

//--------------------------------------------------------------------------------------------------
void TestAnalyzer::endJob()
{

  nevtHist->Write();
  outFile->Write();
  outFile->Close();

  delete outFile;
  delete nevtHist;
  delete evtTree;

  outputName_="";
  outtreeName_="";
  nEvts_=0;

  for(vector<BaseAnalyzer*>::iterator iana = analyzers.begin(); iana != analyzers.end(); ++iana) {
    delete *iana;
  }

}
