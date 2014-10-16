//--------------------------------------------------------------------------------------------------
// 
// ElectronFiller
// 
// Class to fill electron information in a TTree.
// 
// ElectronFiller.h created on Thu Oct 16 12:21:38 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISBASE_ANALYZER_ELECTRONFILLER_H
#define ANALYSISBASE_ANALYZER_ELECTRONFILLER_H

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "AnalysisBase/Analyzer/interface/BaseFiller.h"

namespace ucsbsusy {

  class ElectronFiller : public BaseFiller {

    public :
      ElectronFiller(const edm::ParameterSet &cfg);
      ~ElectronFiller() {}

      void		book(TreeWriter& tW);
      void		reset();
      void		load(edm::Event& iEvent, bool storeOnlyPtr = false, bool isMC = false);
      void		fill(TreeWriter& tW, const int& numAnalyzed);

    private :
      // Input from the config file
      edm::InputTag	electronTag_;
      edm::InputTag	vtxTag_;
      double		eleptMin_;
      bool		printIds_;
      // Stored information can be customized
      bool		fillIDVars_;
      bool		fillIsoVars_;

      // Members to hold info to be filled in the tree (for now; this implementation is to be updated)
      // List of stored information to be updated according to our needs
      // ID and iso information not useful yet. Have a look at https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgammaIDRecipesRun2 for the implementation of POG IDs.
      vector<float>	ele_pt_;
      vector<float>	ele_eta_;
      vector<float>	ele_SCeta_;
      vector<float>	ele_phi_;
      vector<float>	ele_mass_;
      vector<float>	ele_r9_;
      vector<float>	ele_d0_;
      vector<float>	ele_dz_;
      vector<float>	ele_sigietaieta_;
      vector<float>	ele_sigiphiiphi_;
      vector<float>	ele_hOverE_;
      vector<float>	ele_fBrem_;
      vector<int>	ele_nExpHitsInner_;
      vector<int>	ele_nLostHitsInner_;
      vector<bool>	ele_passConvVeto_;
      vector<float>	ele_trackiso_;
      vector<float>	ele_ecaliso_;
      vector<float>	ele_hcaliso_;
      vector<float>	ele_pfchargediso_;
      vector<float>	ele_pfneutraliso_;
      vector<float>	ele_pfphotoniso_;
      vector<float>	ele_pfpuiso_;
      // Id flags: cut-based electron ID flags stored for pat::Electrons. Q.: are these meaningful at all?
      // Add new Run2 electron IDs as appropriate
      // Value map: 0: fails, 1: passes electron ID only, 2: passes electron Isolation only, 3: passes electron ID and Isolation only, 4: passes conversion rejection, 5: passes conversion rejection and ID, 6: passes conversion rejection and Isolation, 7: passes the whole selection
      vector<float>	ele_looseid_;
      vector<float>	ele_tightid_;
      vector<float>	ele_robustlooseid_;
      vector<float>	ele_robusttightid_;

    public :
      // Data members
      edm::Handle<pat::ElectronCollection>	electrons_;
      edm::Handle<reco::VertexCollection>	vertices_;

  };

}

#endif