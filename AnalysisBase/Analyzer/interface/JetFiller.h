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
      JetFiller(const int options, const string branchName, const string genJetsBranchName, const EventInfoFiller * evtInfoFiller
          , const edm::InputTag jetTag
          , const edm::InputTag reGenJetTag
          , const edm::InputTag stdGenJetTag
          , const bool fillReGenJets
          , const double jptMin
          );
      ~JetFiller() {}

      void load(const edm::Event& iEvent);
      void fill();

      reco::GenJetRef getReGenJet(const pat::Jet& jet, const bool enforce = false)  const;
      reco::GenJetRef getStdGenJet(const pat::Jet& jet) const;
      reco::CandidatePtr getRecoJet(const size iGen, bool redefined) const;

      static const std::string REGENJET;   // userClass label for the redefined genJet of the given jet

    private:
      const EventInfoFiller * evtInfofiller_;

    private:
      // Input from the config file
      const edm::InputTag jetTag_;
      const edm::InputTag reGenJetTag_;
      const edm::InputTag stdGenJetTag_;
//      const edm::InputTag genParticleTag_;
      const double        jptMin_;
      const bool          fillReGenJets_;

      // Members to hold index of most recently filled tree data
      // For standard genjets
      // For reco jets
      size ijetpt_       ;
      size ijeteta_      ;
      size ijetphi_      ;
      size ijetmass_     ;
      size ijetptraw_    ;
      size ijetpuId_     ;
      size ijetcsv_      ;
      size ijetflavor_   ;
      size ijetgenindex_ ;
      // For genjets matched to reco jets
      size igenjetpt_    ;
      size igenjeteta_   ;
      size igenjetphi_   ;
      size igenjetmass_  ;
      // For jetShape info
      size ijetbetaStar_;
      size ijetqgl_     ;
      size ijetptD_     ;
      size ijetaxis1_   ;
      size ijetaxis2_   ;
      size ijetMult_    ;
      size igenjetptD_   ;
      size igenjetaxis1_ ;
      size igenjetaxis2_ ;
      size igenjetMult_  ;

    private:
      QuarkGluonTagInterface    * qglInterface_;
      QuarkGluonTaggingVariables* qgTaggingVar_;

      mutable std::vector<reco::CandidatePtr>  reGenRecoRef_ ;
      mutable std::vector<reco::CandidatePtr>  stdGenRecoRef_ ;


    public:
      // Data members
      edm::Handle<pat::JetCollection>     jets_;
      edm::Handle<reco::GenJetCollection> reGenJets_;
      edm::Handle<reco::GenJetCollection> stdGenJets_;

  };

}

#endif
