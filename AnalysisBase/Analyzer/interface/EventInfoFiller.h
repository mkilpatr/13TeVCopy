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

namespace ucsbsusy {

  class EventInfoFiller : public BaseFiller {

    public:
      EventInfoFiller(const edm::ParameterSet &cfg);
      ~EventInfoFiller() {};

      void			book(TreeWriter& tW);
      void			reset();
      void			load(edm::Event& iEvent);
      void			fill(TreeWriter& tW, const int& numAnalyzed);

      reco::Vertex::Point	primaryVertex()	  const	{return primaryVertexIndex_ >= 0 ? (*vertices_)[primaryVertexIndex_].position() : reco::Vertex::Point();	}
      const pat::MET*		met()		  const	{ return met_;			};

    private:
      // Input from the config file
      const edm::InputTag		vtxTag_;
      const edm::InputTag   rhoTag_;
      const edm::InputTag		metTag_;

      // Members to hold info to be filled in the tree (for now; this implementation is to be updated)
      unsigned int		run_;
      unsigned int		lumi_;
      unsigned int		event_;
      int			nVertices_;
      float     rho_;
      float			pvx_;
      float			pvy_;
      float			pvz_;
      float			metpt_;
      float			metphi_;
      float			metsumEt_;

    public:
      // Data members
      edm::Handle<reco::VertexCollection>	vertices_;
      int primaryVertexIndex_;
      edm::Handle<double>                 rhoHandle_;
      edm::Handle<pat::METCollection>		mets_;
      const pat::MET				*met_;

  };

}

#endif
