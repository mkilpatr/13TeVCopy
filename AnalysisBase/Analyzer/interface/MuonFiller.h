//--------------------------------------------------------------------------------------------------
// 
// MuonFiller
// 
// Class to fill muon information in a TTree.
// 
// MuonFiller.h created on Fri Oct 17 12:11:09 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISBASE_ANALYZER_MUONFILLER_H
#define ANALYSISBASE_ANALYZER_MUONFILLER_H

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"

#include "AnalysisBase/Analyzer/interface/BaseFiller.h"
#include "AnalysisTools/Utilities/interface/MCTruth.h"

namespace ucsbsusy {

  class MuonFiller : public BaseFiller {

    public :
      MuonFiller(const edm::ParameterSet &cfg);
      ~MuonFiller() {}

      void		book(TreeWriter& tW);
      void		reset();
      void		load(edm::Event& iEvent, bool storeOnlyPtr = false, bool isMC = false);
      void		fill(TreeWriter& tW, const int& numAnalyzed);

    private :
      // Input from the config file
      edm::InputTag	muonTag_;
      edm::InputTag	vtxTag_;
      edm::InputTag	genParticleTag_;
      double		muptMin_;
      // Stored information can be customized
      bool		requireLoose_;         // only store muons which pass "Loose " ID
      bool		fillIDVars_;
      bool		fillIsoVars_;
      bool		fillGenInfo_;

      // Members to hold info to be filled in the tree (for now; this implementation is to be updated)
      // List of stored information to be updated according to our needs
      vector<float>	mu_pt_;
      vector<float>	mu_eta_;
      vector<float>	mu_phi_;
      vector<float>	mu_mass_;
      vector<int>	mu_q_;
      vector<float>	mu_d0_;
      vector<float>	mu_dz_;
      vector<float>	mu_nChi2_;
      vector<int>	mu_nValidHits_;
      vector<int>	mu_nMatch_;
      vector<int>	mu_nPixHits_;
      vector<int>	mu_nTrackLayers_;
      vector<float>	mu_trackiso_;
      vector<float>	mu_ecaliso_;
      vector<float>	mu_hcaliso_;
      vector<float>	mu_pfchargediso_;
      vector<float>	mu_pfneutraliso_;
      vector<float>	mu_pfphotoniso_;
      vector<float>	mu_pfpuiso_;
      vector<float>	mu_pfdbetaiso_;
      vector<bool>	mu_isloose_;
      vector<bool>	mu_istight_;
      vector<bool>	mu_ispf_;
      vector<bool>	mu_isglobal_;
      vector<bool>	mu_istracker_;
      vector<bool>	mu_isstandalone_;
      // MC truth match information
      vector<float>	mu_genpt_;
      vector<float>	mu_geneta_;
      vector<float>	mu_genphi_;
      vector<float>	mu_genmass_;
      vector<int>	mu_genpdgid_;
      vector<int>	mu_genstatus_;
      vector<int>	mu_genmotherpdgid_;
      vector<int>	mu_genmotherstatus_;

    public :
      // Data members
      edm::Handle<pat::MuonCollection>		muons_;
      edm::Handle<reco::VertexCollection>	vertices_;
      edm::Handle<pat::PackedGenParticleCollection>	genParticles_;

  };

}

#endif
