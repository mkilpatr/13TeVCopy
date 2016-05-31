//--------------------------------------------------------------------------------------------------
// 
// MuonFiller
// 
// Class to fill muon information in a TTree.
// 
// MuonFiller.h created on Fri Oct 17 12:11:09 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISBASE_ANALYZER_SECONDARYVERTEXFILLER_H
#define ANALYSISBASE_ANALYZER_SECONDARYVERTEXFILLER_H

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "AnalysisBase/Analyzer/interface/BaseFiller.h"
#include "DataFormats/Candidate/interface/VertexCompositePtrCandidate.h"
#include "DataFormats/GeometryCommonDetAlgo/interface/Measurement1D.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "TLorentzVector.h"

#if defined( __GXX_EXPERIMENTAL_CXX0X__)
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#endif


struct MagneticField;


namespace ucsbsusy {
  
  class SecondaryVertexFiller : public BaseFiller {

  public :
    SecondaryVertexFiller(const edm::ParameterSet& cfg, edm::ConsumesCollector && cc, const int options, const string branchName);
    ~SecondaryVertexFiller() {}

    void load(const edm::Event& iEvent, const edm::EventSetup &iSetup);
    void fill();

  private :
    edm::EDGetTokenT<reco::VertexCollection>                      PVtoken_;
    edm::EDGetTokenT<reco::VertexCompositePtrCandidateCollection> SVtoken_;

    // Members to hold indices of tree data
    size sv_pt_;
    size sv_eta_;
    size sv_phi_;
    size sv_mass_;
    size sv_ntracks_;
    size sv_chi2_;
    size sv_ndf_;
    size sv_ip2d_;
    size sv_sip2d_;
    size sv_ip3d_;
    size sv_sip3d_;
    size sv_dxy_;
    size sv_d3d_;
    size sv_costhetasvpv_;

    static MagneticField *paramField_;
    Measurement1D IP2D(const reco::Track &tk, const reco::Vertex &vtx) const ;
    Measurement1D IP2D(const reco::Track &tk, const reco::VertexCompositePtrCandidate &sv) const ;
    Measurement1D IP3D(const reco::Track &tk, const reco::Vertex &vtx) const ;
    Measurement1D IP3D(const reco::Track &tk, const reco::VertexCompositePtrCandidate &sv) const ;
    Measurement1D signedIP3D(const reco::Track &tk, const reco::Vertex &vtx, const reco::Track::Vector jetdir) const ;
    Measurement1D signedIP3D(const reco::Track &tk, const reco::VertexCompositePtrCandidate &vtx, const reco::Track::Vector jetdir) const ;
    Measurement1D signedIP2D(const reco::Track &tk, const reco::Vertex &vtx, const reco::Track::Vector jetdir) const ;
    Measurement1D signedIP2D(const reco::Track &tk, const reco::VertexCompositePtrCandidate &vtx, const reco::Track::Vector jetdir) const ;
    Measurement1D vertexD3d(const reco::VertexCompositePtrCandidate &sv, const reco::Vertex &pv) const ;
    Measurement1D vertexDxy(const reco::VertexCompositePtrCandidate &sv, const reco::Vertex &pv) const ;
    float vertexDdotP(const reco::VertexCompositePtrCandidate &sv, const reco::Vertex &pv) const ;

  public :
    // Data members
    edm::Handle<reco::VertexCollection> pvs_;
    edm::Handle<reco::VertexCompositePtrCandidateCollection> svs_;
 
  };

}

#endif
