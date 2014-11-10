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


namespace ucsbsusy {
class EventInfoFiller;
class QuarkGluonTagInterface;
class QuarkGluonTaggingVariables;

  class JetFiller : public BaseFiller {

    public:
      JetFiller(const edm::ParameterSet &cfg, bool isMC = false,  const EventInfoFiller * evtInfoFiller = 0);
      ~JetFiller() {}

      void		book(TreeWriter& tW);
      void		reset();
      void    reserve();
      void		load(edm::Event& iEvent);
      void		fill(TreeWriter& tW, const int& numAnalyzed);

      reco::GenJetRef	getReGenJet(const pat::Jet& jet)  const;
      reco::GenJetRef	getStdGenJet(const pat::Jet& jet) const;

      static const std::string	REGENJET;   // userClass label for the redefined genJet of the given jet

    private:
      // Input from the config file
      const bool          fillGenInfo_;
      const bool          fillJetShapeInfo_;
      const edm::InputTag	jetTag_;
      const edm::InputTag	reGenJetTag_;
      const edm::InputTag	stdGenJetTag_;
      const edm::InputTag	genParticleTag_;
      const double		    jptMin_;
      const string		    jetsName_;          // used as prefix in branch names to specify the type of jets filled (e.g., ak4)

      // Members to hold info to be filled in the tree (for now; this implementation is to be updated)
      vector<float>	jetpt_;
      vector<float>	jeteta_;
      vector<float>	jetphi_;
      vector<float>	jetmass_;
      vector<float>	jetptraw_;
      vector<float>	jetpuId_;
      vector<float>	jetcsv_;
      vector<int> 	jetflavor_;
      // For genjets matched to reco jets
      vector<float>	genjetpt_;
      vector<float>	genjeteta_;
      vector<float>	genjetphi_;
      vector<float>	genjetmass_;
      vector<int>	  genjetflavor_;
      vector<float> genjetptD_  ;
      vector<float> genjetaxis1_;
      vector<float> genjetaxis2_;
      vector<int>   genjetMult_ ;
      // For jetShape info
      vector<float> jetbetaStar_;
      vector<float> jetqgl_     ;
      vector<float> jetptD_     ;
      vector<float> jetaxis1_   ;
      vector<float> jetaxis2_   ;
      vector<int>   jetMult_    ;

    private:
      const EventInfoFiller     * evtInfofiller_;
      QuarkGluonTagInterface    * qglInterface_;
      QuarkGluonTaggingVariables* qgTaggingVar_;


    public:
      // Data members
      edm::Handle<pat::JetCollection>		  jets_;
      edm::Handle<reco::GenJetCollection>	reGenJets_;
      edm::Handle<reco::GenJetCollection>	stdGenJets_;

  };

}

#endif
