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
  vtxTag_(cfg.getParameter<edm::InputTag>("vertices")),
  metTag_(cfg.getParameter<edm::InputTag>("mets")),
  vertices_(0),
  nVertices_(0),
  mets_(0),
  met_(0)
{}
//--------------------------------------------------------------------------------------------------
void EventInfoFiller::load(edm::Event& iEvent, bool storeOnlyPtr, bool isMC){
  enforceGet(iEvent,vtxTag_,vertices_,true);
  nVertices_ = vertices_->size();
  if(nVertices_ > 0)
    primaryVertex_ = vertices_->front().position();
  else
    primaryVertex_.SetXYZ(0,0,0);

  enforceGet(iEvent,metTag_,mets_,true);
  met_ = &mets_->front();
}
//--------------------------------------------------------------------------------------------------
void EventInfoFiller::fill(Planter& plant, int& bookMark, const int& numAnalyzed)
{
  plant.fill(float( primaryVertex_.x()                    ), "pv_x"         , "x(primary vertex)"              );
  plant.fill(float( primaryVertex_.y()                    ), "pv_y"         , "y(primary vertex)"              );
  plant.fill(float( primaryVertex_.z()                    ), "pv_z"         , "z(primary vertex)"              );
  plant.fill(convertTo<multiplicity>( nVertices_, "num_vertices"          ), "num_vertices"         , "N. primary vertices"      );

  plant.fill(float( met_->pt  () ),"reco_met_pt"      , "|#slash{E}_{T}|"    );
  plant.fill(float( met_->phi () ),"reco_met_phi"     , "#phi(#slash{E}_{T})");

  bookMark++;
}

