//--------------------------------------------------------------------------------------------------
// 
// EventInfoFiller
// 
// Simple class to write some event info into a TTree. To be tested with MiniAOD for now. Sophisticated data management methods to be added later :-)
// 
// EventInfoFiller.cc created on Mon Aug 11 10:23:46 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisBase/Analyzer/interface/EventInfoFiller.h"

using namespace ucsbsusy;
using namespace std;

EventInfoFiller::EventInfoFiller(const edm::ParameterSet &cfg):
  BaseAnalyzer(cfg),
  vtxToken_(consumes<reco::VertexCollection>(cfg.getParameter<edm::InputTag>("vertices"))),
  metToken_(consumes<pat::METCollection>(cfg.getParameter<edm::InputTag>("mets"))),
  outputName_(cfg.getUntrackedParameter<string>("filename")),
  nEvts_(0),
  evtInfo_(new EventInfo())
{
  // Constructor

  EventInfo::Class()->IgnoreTObjectStreamer();

}

EventInfoFiller::~EventInfoFiller()
{
  // Destructor

  delete evtInfo_;

}

// called at the beginning of the job
void EventInfoFiller::beginJob()
{

  outFile = new TFile(outputName_.c_str(),"RECREATE");
  nevtHist = new TH1F("NEvents","",1,0,1);
  evtTree = new TTree("Events","Events");

  evtTree->Branch("EventInfo", &evtInfo_);

}

// called for each event
void EventInfoFiller::analyze(const edm::Event &event, const edm::EventSetup &setup)
{

  nEvts_++;
  nevtHist->Fill(0);

  edm::Handle<reco::VertexCollection> vertices;
  event.getByToken(vtxToken_, vertices);
  if (vertices->empty()) return; // skip if no PV found
  const reco::Vertex &PV = vertices->front();

  edm::Handle<pat::METCollection> mets;
  event.getByToken(metToken_, mets);
  const pat::MET &met = mets->front();

  evtInfo_->setRunNum(event.id().run());
  evtInfo_->setLumiSec(event.luminosityBlock());
  evtInfo_->setEvtNum(event.id().event());
  evtInfo_->setIsMC(!event.isRealData());

  evtInfo_->setNPV(vertices->size());
  evtInfo_->setPVx(PV.x());
  evtInfo_->setPVy(PV.y());
  evtInfo_->setPVz(PV.z());

  evtInfo_->setMET(met.pt());
  evtInfo_->setMETPhi(met.phi());
  evtInfo_->setMETSumET(met.sumEt());
  evtInfo_->setGenMET(met.genMET()->pt());
  evtInfo_->setMETUp(met.shiftedPt(pat::MET::JetEnUp));
  evtInfo_->setMETDown(met.shiftedPt(pat::MET::JetEnDown));

  evtTree->Fill();

}

// called at the end of the job
void EventInfoFiller::endJob()
{

  nevtHist->Write();
  outFile->Write();
  outFile->Close();

  delete outFile;
  delete nevtHist;
  delete evtTree;

  outputName_="";
  nEvts_=0;

}
