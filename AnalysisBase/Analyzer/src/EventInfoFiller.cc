//--------------------------------------------------------------------------------------------------
// 
// EventInfoFiller
// 
// Simple class to write some event info into a TTree.
// 
// EventInfoFiller.cc created on Mon Aug 11 10:23:46 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisBase/Analyzer/interface/EventInfoFiller.h"

using namespace ucsbsusy;
using namespace std;

//--------------------------------------------------------------------------------------------------
EventInfoFiller::EventInfoFiller(const edm::ParameterSet &cfg) :
  BaseAnalyzer(cfg),
  vtxTag_(cfg.getParameter<edm::InputTag>("vertices")),
  metTag_(cfg.getParameter<edm::InputTag>("mets")),
  evtInfo_(new EventInfo())
{
  // Constructor

  EventInfo::Class()->IgnoreTObjectStreamer();

}
//--------------------------------------------------------------------------------------------------
EventInfoFiller::~EventInfoFiller()
{
  // Destructor

  delete evtInfo_;

}

//--------------------------------------------------------------------------------------------------
void EventInfoFiller::book(TTree &t)
{
 // Add branch for this filler

  t.Branch("EventInfo", &evtInfo_);

}

//--------------------------------------------------------------------------------------------------
void EventInfoFiller::fill(const edm::Event &event)
{
  // Called for each event

  edm::Handle<reco::VertexCollection> vertices;
  event.getByLabel(vtxTag_, vertices);
  if (vertices->empty()) return; // skip if no PV found
  const reco::Vertex &PV = vertices->front();

  edm::Handle<pat::METCollection> mets;
  event.getByLabel(metTag_, mets);
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

}
