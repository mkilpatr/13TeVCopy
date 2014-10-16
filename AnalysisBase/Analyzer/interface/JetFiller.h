//--------------------------------------------------------------------------------------------------
// 
// JetFiller
// 
// Class to fill some basic jet information for testing. To be developed as needed.
// 
// JetFiller.h created on Thu Aug 14 11:10:55 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISBASE_ANALYZER_JETFILLER_H
#define ANALYSISBASE_ANALYZER_JETFILLER_H

#include "DataFormats/PatCandidates/interface/Jet.h"

#include "AnalysisBase/Analyzer/interface/BaseFiller.h"
#include "AnalysisTools/DataFormats/interface/Jet.h"

namespace ucsbsusy {

  class JetFiller : public BaseFiller {

    public:
      JetFiller(const edm::ParameterSet &cfg);
      ~JetFiller() {}

      void		book(TreeWriter& tW);
      void		reset();
      void		load(edm::Event& iEvent, bool storeOnlyPtr = false, bool isMC = false);
      void		fill(TreeWriter& tW, const int& numAnalyzed);

      RecoJetF&		operator[] (const int& index)		{ return recoJets[index];	}

      reco::GenJetRef	getReGenJet(const pat::Jet& jet)  const;
      reco::GenJetRef	getStdGenJet(const pat::Jet& jet) const;

      static const std::string	REGENJET;   // userClass label for the redefined genJet of the given jet

    private:
      // Input from the config file
      edm::InputTag	jetTag_;
      edm::InputTag	reGenJetTag_;
      edm::InputTag	stdGenJetTag_;
      edm::InputTag	genParticleTag_;
      double		jptMin_;
      bool		fillGenInfo_;

      // Members to hold info to be filled in the tree (for now; this implementation is to be updated)
      vector<float>	jetpt_;
      vector<float>	jeteta_;
      vector<float>	jetphi_;
      vector<float>	jetmass_;
      vector<float>	jetptraw_;
      vector<float>	jetpuId_;
      vector<float>	jetcsv_;

    public:
      // Data members
      edm::Handle<pat::JetCollection>		jets_;
      edm::Handle<reco::GenJetCollection>	reGenJets_;
      edm::Handle<reco::GenJetCollection>	stdGenJets_;

      RecoJetFCollection			recoJets;
      GenJetFCollection				genJets;

      vector<float>				v_csv;
      vector<JetFlavorMatching::TaggableType>	v_flavor;

  };

}

#endif
