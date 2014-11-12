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
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "AnalysisBase/Analyzer/interface/BaseFiller.h"

#include "AnalysisTools/ObjectSelection/interface/EGammaMvaEleEstimatorCSA14.h"
#include "AnalysisTools/ObjectSelection/interface/LeptonId.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"

namespace ucsbsusy {

  class ElectronFiller : public BaseFiller {

    public :
      ElectronFiller(const edm::ParameterSet &cfg, const bool isMC);
      ~ElectronFiller() {}

      void		initMVA();
      void		book(TreeWriter& tW);
      void		reset();
      void		load(const edm::Event& iEvent);
      void		fill();

    private :
      // Stored information can be customized
      const bool    fillIDVars_;
      const bool    fillIsoVars_;
      const bool    evaluatePOGMVA_;
      const bool    fillGenInfo_;
      // Input from the config file
      const edm::InputTag	electronTag_;
      const edm::InputTag	vtxTag_;
      const edm::InputTag	genParticleTag_;
      const double		eleptMin_;
      const int		bunchSpacing_;
      const bool		printIds_;

      // Evaluate POG MVA ID
      EGammaMvaEleEstimatorCSA14*	eleMVANonTrig;
      EGammaMvaEleEstimatorCSA14*	eleMVATrig;
      LeptonId*				eleIdCuts;

      // Members to hold info to be filled in the tree (for now; this implementation is to be updated)
      // List of stored information to be updated according to our needs
      // ID and iso information not useful yet. Have a look at https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgammaIDRecipesRun2 for the implementation of POG IDs.
      vector<float>	ele_pt_;
      vector<float>	ele_eta_;
      vector<float>	ele_SCeta_;
      vector<float>	ele_phi_;
      vector<float>	ele_mass_;
      vector<int>	ele_q_;
      vector<float>	ele_r9_;
      vector<float>	ele_d0_;
      vector<float>	ele_dz_;
      // ID variables, needed for evaluation of cut-based ID
      vector<float>	ele_ecalE_;
      vector<float>	ele_eOverPIn_;
      vector<float>	ele_sigietaieta_;
      vector<float>	ele_sigiphiiphi_;
      vector<float>	ele_full5x5sigietaieta_;
      vector<float>	ele_dEtaIn_;
      vector<float>	ele_dPhiIn_;
      vector<float>	ele_hOverE_;
      vector<float>	ele_fBrem_;
      vector<int>	ele_nExpHitsInner_;
      vector<int>	ele_nLostHitsInner_;
      vector<bool>	ele_passConvVeto_;
      // Isolation quantities
      vector<float>	ele_trackiso_;
      vector<float>	ele_ecaliso_;
      vector<float>	ele_hcaliso_;
      vector<float>	ele_pfchargediso_;
      vector<float>	ele_pfneutraliso_;
      vector<float>	ele_pfphotoniso_;
      vector<float>	ele_pfpuiso_;
      vector<float>	ele_pfdbetaiso_;
      // CSA14 MVA ID (can be evalated for either 25ns or 50ns scenarios, determined by bunchSpacing_ parameter)
      vector<float>	ele_mvaidnontrig_;
      vector<float>	ele_mvaidtrig_;
      // Id flags: cut-based electron ID flags stored for pat::Electrons. Q.: are these meaningful at all?
      // Value map: 0: fails, 1: passes electron ID only, 2: passes electron Isolation only, 3: passes electron ID and Isolation only, 4: passes conversion rejection, 5: passes conversion rejection and ID, 6: passes conversion rejection and Isolation, 7: passes the whole selection
      vector<bool>	ele_vetoid_;
      vector<bool>	ele_looseid_;
      vector<bool>	ele_mediumid_;
      vector<bool>	ele_tightid_;
      // MC truth match information
      vector<float>	ele_genpt_;
      vector<float>	ele_geneta_;
      vector<float>	ele_genphi_;
      vector<float>	ele_genmass_;
      vector<int>	ele_genpdgid_;
      vector<int>	ele_genstatus_;
      vector<int>	ele_genmotherpdgid_;
      vector<int>	ele_genmotherstatus_;

    public :
      // Data members
      edm::Handle<pat::ElectronCollection>	electrons_;
      edm::Handle<reco::VertexCollection>	vertices_;
      edm::Handle<pat::PackedGenParticleCollection>	genParticles_;

  };

}

#endif
