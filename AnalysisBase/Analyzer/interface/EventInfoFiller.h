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
      void			load(edm::Event& iEvent, bool storeOnlyPtr = false, bool isMC = false);
      void			fill(TreeWriter& tW, const int& numAnalyzed);

      const math::XYZPoint&	primaryVertex()	  const	{ return primaryVertex_;	}

      const pat::MET*		met()		  const	{ return met_;			};

    private:
      // Input from the config file
      edm::InputTag		vtxTag_;
      edm::InputTag		metTag_;

      // Members to hold info to be filled in the tree (for now; this implementation is to be updated)
      int			nVertices_;
      float			pvx_;
      float			pvy_;
      float			pvz_;
      float			metpt_;
      float			metphi_;
      float			metsumEt_;

    public:
      // Data members
      edm::Handle<reco::VertexCollection>	vertices_;
      math::XYZPoint				primaryVertex_;
      edm::Handle<pat::METCollection>		mets_;
      const pat::MET				*met_;

  };

}

#endif
