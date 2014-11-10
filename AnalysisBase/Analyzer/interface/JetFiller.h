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
#include "AnalysisTools/Utilities/interface/TreeWriterData.h"

namespace ucsbsusy {
class EventInfoFiller;
class QuarkGluonTagInterface;
class QuarkGluonTaggingVariables;

  class JetFiller : public BaseFiller {
  public:
  enum  Options           {
                            NULLOPT         = 0
                          , LOADGEN         = (1 <<  0)   ///< load gen jets
                          , LOADJETSHAPE    = (1 <<  1)   ///< load jet shap variables
  };
  static const int defaultOptions;


    public:
      JetFiller(const int options, const string branchName, const EventInfoFiller * evtInfoFille
          , const edm::InputTag jetTag
          , const edm::InputTag reGenJetTag
          , const edm::InputTag stdGenJetTag
          , const double jptMin
          );
      ~JetFiller() {}

      void		load(const edm::Event& iEvent);
      void		fill();

      reco::GenJetRef	getReGenJet(const pat::Jet& jet)  const;
      reco::GenJetRef	getStdGenJet(const pat::Jet& jet) const;

      static const std::string	REGENJET;   // userClass label for the redefined genJet of the given jet

    private:
      const EventInfoFiller     * evtInfofiller_;

    private:
      // Input from the config file
      const edm::InputTag	jetTag_;
      const edm::InputTag	reGenJetTag_;
      const edm::InputTag	stdGenJetTag_;
//      const edm::InputTag	genParticleTag_;
      const double		    jptMin_;

      // Members to hold info to be filled in the tree (for now; this implementation is to be updated)
      size jetpt_       ;
      size jeteta_      ;
      size jetphi_      ;
      size jetmass_     ;
      size jetptraw_    ;
      size jetpuId_     ;
      size jetcsv_      ;
      size jetflavor_   ;
      // For genjets matched to reco jets
      size genjetpt_    ;
      size genjeteta_   ;
      size genjetphi_   ;
      size genjetmass_  ;
      size genjetflavor_;
      // For jetShape info
      size jetbetaStar_;
      size jetqgl_     ;
      size jetptD_     ;
      size jetaxis1_   ;
      size jetaxis2_   ;
      size jetMult_    ;
      size genjetptD_   ;
      size genjetaxis1_ ;
      size genjetaxis2_ ;
      size genjetMult_  ;

    private:
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
