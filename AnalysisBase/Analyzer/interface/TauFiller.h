//--------------------------------------------------------------------------------------------------
// 
// TauFiller
// 
// Class to fill hadronic tau information in a TTree. NB: MiniAOD stores taus from hpsPFTauProducer with pT > 18 GeV, and passing the basic decayModeFinding id.
// 
// TauFiller.h created on Wed Oct 22 14:12:52 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISBASE_ANALYZER_TAUFILLER_H
#define ANALYSISBASE_ANALYZER_TAUFILLER_H

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

#include "AnalysisTools/Utilities/interface/Types.h"

#include "AnalysisBase/Analyzer/interface/BaseFiller.h"

namespace ucsbsusy {

  class TauFiller : public BaseFiller {

    public :
      TauFiller(const edm::ParameterSet &cfg);
      ~TauFiller() {}

      void		initTauIdNames();
      void		book(TreeWriter& tW);
      void		reset();
      void		load(edm::Event& iEvent);
      void		fill(TreeWriter& tW, const int& numAnalyzed);

    private :
      // Whether or not to store the raw HPS discriminator outputs
      const bool    fillRawDiscs_;
      // Input from the config file
      edm::InputTag	tauTag_;
      edm::InputTag	vtxTag_;
      double		tauptMin_;
      bool		printIds_;
      // Map of HPS discriminator names to flags
      TauIdMap		hpsIds_;


      // Members to hold info to be filled in the tree
      // List of stored information to be updated according to our needs
      vector<float>	tau_pt_;
      vector<float>	tau_eta_;
      vector<float>	tau_phi_;
      vector<float>	tau_mass_;
      vector<int>	tau_q_;
      vector<float>	tau_dxy_;
      vector<float>	tau_dxyerr_;
      vector<float>	tau_dxysig_;
      vector<float>	tau_leadcand_pt_;
      vector<float>	tau_leadcand_eta_;
      vector<float>	tau_leadcand_phi_;
      vector<float>	tau_leadcand_mass_;
      vector<int>	tau_leadcand_q_;
      vector<float>	tau_leadchargedcand_pt_;
      vector<float>	tau_leadchargedcand_eta_;
      vector<float>	tau_leadchargedcand_phi_;
      vector<float>	tau_leadchargedcand_mass_;
      vector<int>	tau_leadchargedcand_q_;
      vector<float>	tau_leadchargedcand_d0_;
      vector<float>	tau_leadchargedcand_dz_;
      vector<unsigned long>	tau_idflags_;
      // Raw discriminator values
      vector<float>	tau_isodb3hitsraw_;
      vector<float>	tau_isomvanoltraw_;
      vector<float>	tau_isomvaltraw_;
      vector<float>	tau_antielemvaraw_;
      vector<int>	tau_antielemvacat_;
      vector<float>	tau_antimumvaraw_;
      vector<int>	tau_antimumvacat_;

    public :
      // Data members
      edm::Handle<pat::TauCollection>		taus_;
      edm::Handle<reco::VertexCollection>	vertices_;

  };

}

#endif
