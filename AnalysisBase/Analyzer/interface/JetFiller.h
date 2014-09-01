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

      void load(edm::Event& iEvent, bool storeOnlyPtr = false, bool isMC = false);
      void fill(Planter& plant, int& bookMark, const int& numAnalyzed);
      RecoJetFCollection& operator->(){return jets;}

      reco::GenJetRef getReGenJet(const pat::Jet& jet)  const;
      reco::GenJetRef getStdGenJet(const pat::Jet& jet) const;

      static const std::string    REGENJET        ;   ///< userClass label for the redefined genJet of the given jet

    private:
      //--------------------------------------------------------------------------------------------------
      // Input from the config file
      //--------------------------------------------------------------------------------------------------
      edm::InputTag jetTag_;
      edm::InputTag reGenJetTag_;
      edm::InputTag stdGenJetTag_;
      edm::InputTag genParticleTag_;
      double        jptMin_;
      bool          fillGenInfo;

    public:
      //--------------------------------------------------------------------------------------------------
      // Data Members
      //--------------------------------------------------------------------------------------------------
      edm::Handle<pat::JetCollection>         jets_;
      edm::Handle<reco::GenJetCollection>     reGenJets_;
      edm::Handle<reco::GenJetCollection>     stdGenJets_;

      RecoJetFCollection                      jets;
      GenJetFCollection                       genJets;

      vector<float>                           v_csv;
      vector<JetFlavorMatching::TaggableType> v_flavor;




  };
}

#endif
