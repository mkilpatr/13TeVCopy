//--------------------------------------------------------------------------------------------------
//
// SecondaryVertexFiller
//
// Muon filler.
//
// SecondaryVertexFiller.cc created on Fri Oct 17 12:11:09 CEST 2014
//
//--------------------------------------------------------------------------------------------------

#include "AnalysisBase/Analyzer/interface/SecondaryVertexFiller.h"
#include "MagneticField/UniformEngine/src/UniformMagneticField.h"
#include "MagneticField/ParametrizedEngine/src/OAEParametrizedMagneticField.h"
#include "TrackingTools/PatternTools/interface/TwoTrackMinimumDistance.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/IPTools/interface/IPTools.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "RecoVertex/VertexTools/interface/VertexDistance3D.h"
#include "RecoVertex/VertexTools/interface/VertexDistanceXY.h"
#include <vector>
#include <TMath.h>
#include <Math/SVector.h>

#include "AnalysisTools/DataFormats/interface/SV.h"
#include "AnalysisTools/ObjectSelection/interface/IVFMVA.h"
#include "AnalysisTools/TreeReader/interface/Defaults.h"


MagneticField *SecondaryVertexFiller::paramField_ = 0;

using namespace ucsbsusy;



//--------------------------------------------------------------------------------------------------
SecondaryVertexFiller::SecondaryVertexFiller(const edm::ParameterSet& cfg, edm::ConsumesCollector && cc, const int options, const string branchName) :
  BaseFiller(options, branchName),
  PVtoken_(cc.consumes<reco::VertexCollection>(cfg.getParameter<edm::InputTag>("primaryvertices"))),
  SVtoken_(cc.consumes<reco::VertexCompositePtrCandidateCollection>(cfg.getParameter<edm::InputTag>("secondaryvertices")))
{
  sv_pt_           = data.addMulti<float>(branchName_,"sv_pt",0);
  sv_eta_          = data.addMulti<float>(branchName_,"sv_eta",0);
  sv_phi_          = data.addMulti<float>(branchName_,"sv_phi",0);
  sv_mass_         = data.addMulti<float>(branchName_,"sv_mass",0);
  sv_ntracks_      = data.addMulti<int>(branchName_,"sv_ntracks",0);
  sv_chi2_         = data.addMulti<float>(branchName_,"sv_chi2",0);
  sv_ndf_          = data.addMulti<float>(branchName_,"sv_ndf",0);
  //  sv_ip2d_         = data.addMulti<float>(branchName_,"sv_ip2d",0);
  //  sv_sip2d_        = data.addMulti<float>(branchName_,"sv_sip2d",0);
  //  sv_ip3d_         = data.addMulti<float>(branchName_,"sv_ip3d",0);
  //  sv_sip3d_        = data.addMulti<float>(branchName_,"sv_sip3d",0);
  sv_dxy_          = data.addMulti<float>(branchName_,"sv_dxy",0);
  sv_dxyerr_       = data.addMulti<float>(branchName_,"sv_dxyerr",0);
  sv_d3d_          = data.addMulti<float>(branchName_,"sv_d3d",0);
  sv_d3derr_       = data.addMulti<float>(branchName_,"sv_d3derr",0);
  sv_costhetasvpv_ = data.addMulti<float>(branchName_,"sv_costhetasvpv",0);

  if (options_ & LOADMVA){
    ivfMVA_lpt_b = new IVFMVA(defaults::MVAWEIGHT_IVF_LOWPT_B);
    ivfMVA_lpt_e = new IVFMVA(defaults::MVAWEIGHT_IVF_LOWPT_E);
    ivfMVA_hpt_b = new IVFMVA(defaults::MVAWEIGHT_IVF_HIGHPT_B);
    ivfMVA_hpt_e = new IVFMVA(defaults::MVAWEIGHT_IVF_HIGHPT_E);

    sv_mva_          = data.addMulti<float>(branchName_,"sv_mva",-9);
  }
}

//--------------------------------------------------------------------------------------------------
void SecondaryVertexFiller::load(const edm::Event& iEvent, const edm::EventSetup &iSetup)
{
  reset();
  iEvent.getByToken(PVtoken_, pvs_);
  iEvent.getByToken(SVtoken_, svs_);
  isLoaded_ = true;
}

//--------------------------------------------------------------------------------------------------
void SecondaryVertexFiller::fill()
{


  int primaryVertexIndex_ = 0;
  bool hasgoodvtx = false;
  if (pvs_->size()==0 ) { hasgoodvtx = false; }
  else if (!(*pvs_)[primaryVertexIndex_].isFake() &&
	   (*pvs_)[primaryVertexIndex_].ndof() > 4. &&
	   (*pvs_)[primaryVertexIndex_].position().Rho() <= 2.0 &&
	   fabs((*pvs_)[primaryVertexIndex_].position().Z())<=24.0) { hasgoodvtx = true; }


  if (hasgoodvtx) {

    for (unsigned int i0 = 0; i0<svs_->size(); ++i0) {

      const reco::VertexCompositePtrCandidate &sv = (*svs_)[i0];
      const reco::Vertex & pv = (*pvs_)[0];

      if ( ((vertexDxy(sv,pv).value())>4.) || (vertexDdotP(sv,pv)<0.9) ) { continue; }

      data.fillMulti<float>(sv_pt_          , sv.pt() );
      data.fillMulti<float>(sv_eta_         , sv.eta() );
      data.fillMulti<float>(sv_phi_         , sv.phi() );
      data.fillMulti<float>(sv_mass_        , sv.mass() );
      data.fillMulti<int  >(sv_ntracks_     , sv.numberOfDaughters() );
      data.fillMulti<float>(sv_chi2_        , sv.vertexChi2() );
      data.fillMulti<float>(sv_ndf_         , sv.vertexNdof() );
      data.fillMulti<float>(sv_dxy_         , vertexDxy(sv,pv).value() );
      data.fillMulti<float>(sv_dxyerr_      , vertexDxy(sv,pv).error() );
      data.fillMulti<float>(sv_d3d_         , vertexD3d(sv,pv).value() );
      data.fillMulti<float>(sv_d3derr_      , vertexD3d(sv,pv).error() );
      data.fillMulti<float>(sv_costhetasvpv_, vertexDdotP(sv,pv) );


      if (options_ & LOADMVA){
        SVF tmpSVObj(CylLorentzVectorF(sv.pt(), sv.eta(), sv.phi(), sv.mass()), -1,
            sv.numberOfDaughters(),
            sv.vertexChi2(),
            sv.vertexNdof(),
            vertexDxy(sv,pv).value(),
            vertexDxy(sv,pv).error(),
            vertexD3d(sv,pv).value(),
            vertexD3d(sv,pv).error(),
            vertexDdotP(sv,pv));

        float mva_ = -9;
        if (sv.pt()<15.  && fabs(sv.eta())<1.2 ) { mva_ = ivfMVA_lpt_b->getIVFCandScore(tmpSVObj); }
        if (sv.pt()<15.  && fabs(sv.eta())>=1.2) { mva_ = ivfMVA_lpt_e->getIVFCandScore(tmpSVObj); }
        if (sv.pt()>=15. && fabs(sv.eta())<1.2 ) { mva_ = ivfMVA_hpt_b->getIVFCandScore(tmpSVObj); }
        if (sv.pt()>=15. && fabs(sv.eta())>=1.2) { mva_ = ivfMVA_hpt_e->getIVFCandScore(tmpSVObj); }

        data.fillMulti<float>(sv_mva_, mva_);
      }

    }

  } //end of checking if there is a good PV

  isFilled_ = true;

}


Measurement1D SecondaryVertexFiller::IP2D(const reco::Track &tk, const reco::Vertex &vtx) const {
  if (paramField_ == 0) paramField_ = new OAEParametrizedMagneticField("3_8T");
  reco::TransientTrack ttk(tk,paramField_);
  return IPTools::absoluteTransverseImpactParameter(ttk,vtx).second;
}

Measurement1D SecondaryVertexFiller::IP2D(const reco::Track &tk, const reco::VertexCompositePtrCandidate &sv) const {
  reco::Vertex::CovarianceMatrix csv; sv.fillVertexCovariance(csv);
  reco::Vertex svtx(sv.vertex(), csv);
  return IP2D(tk, svtx);
}

Measurement1D SecondaryVertexFiller::IP3D(const reco::Track &tk, const reco::Vertex &vtx) const {
  if (paramField_ == 0) paramField_ = new OAEParametrizedMagneticField("3_8T");
  reco::TransientTrack ttk(tk,paramField_);
  return IPTools::absoluteImpactParameter3D(ttk,vtx).second;
}

Measurement1D SecondaryVertexFiller::IP3D(const reco::Track &tk, const reco::VertexCompositePtrCandidate &sv) const {
  reco::Vertex::CovarianceMatrix csv; sv.fillVertexCovariance(csv);
  reco::Vertex svtx(sv.vertex(), csv);
  return IP3D(tk, svtx);
}

Measurement1D SecondaryVertexFiller::signedIP2D(const reco::Track &tk, const reco::Vertex &vtx, const reco::Track::Vector jetdir) const {
  if (paramField_ == 0) paramField_ = new OAEParametrizedMagneticField("3_8T");
  reco::TransientTrack ttk(tk,paramField_);
  return IPTools::signedTransverseImpactParameter(ttk, GlobalVector(jetdir.X(),jetdir.Y(),jetdir.Z()), vtx).second;
}

Measurement1D SecondaryVertexFiller::signedIP2D(const reco::Track &tk, const reco::VertexCompositePtrCandidate &sv, const reco::Track::Vector jetdir) const {
  reco::Vertex::CovarianceMatrix csv; sv.fillVertexCovariance(csv);
  reco::Vertex svtx(sv.vertex(), csv);
  return signedIP2D(tk, svtx, jetdir);
}

Measurement1D SecondaryVertexFiller::signedIP3D(const reco::Track &tk, const reco::Vertex &vtx, const reco::Track::Vector jetdir) const {
  if (paramField_ == 0) paramField_ = new OAEParametrizedMagneticField("3_8T");
  reco::TransientTrack ttk(tk,paramField_);
  return IPTools::signedImpactParameter3D(ttk, GlobalVector(jetdir.X(),jetdir.Y(),jetdir.Z()), vtx).second;
}

Measurement1D SecondaryVertexFiller::signedIP3D(const reco::Track &tk, const reco::VertexCompositePtrCandidate &sv, const reco::Track::Vector jetdir) const {
  reco::Vertex::CovarianceMatrix csv; sv.fillVertexCovariance(csv);
  reco::Vertex svtx(sv.vertex(), csv);
  return signedIP3D(tk, svtx, jetdir);
}

Measurement1D SecondaryVertexFiller::vertexD3d(const reco::VertexCompositePtrCandidate &svcand, const reco::Vertex &pv) const {
  VertexDistance3D dist;
  reco::Vertex::CovarianceMatrix csv; svcand.fillVertexCovariance(csv);
  reco::Vertex svtx(svcand.vertex(), csv);
  return dist.distance(svtx, pv);
}

Measurement1D SecondaryVertexFiller::vertexDxy(const reco::VertexCompositePtrCandidate &svcand, const reco::Vertex &pv) const {
  VertexDistanceXY dist;
  reco::Vertex::CovarianceMatrix csv; svcand.fillVertexCovariance(csv);
  reco::Vertex svtx(svcand.vertex(), csv);
  return dist.distance(svtx, pv);
}

float SecondaryVertexFiller::vertexDdotP(const reco::VertexCompositePtrCandidate &sv, const reco::Vertex &pv) const {
  reco::Candidate::Vector p = sv.momentum();
  reco::Candidate::Vector d(sv.vx() - pv.x(), sv.vy() - pv.y(), sv.vz() - pv.z());
  return p.Unit().Dot(d.Unit());
}
