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

#include "AnalysisBase/Analyzer/interface/BaseFiller.h"
#include "AnalysisTools/Utilities/interface/TreeWriterData.h"

#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/Common/interface/ValueMap.h"

#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"

namespace ucsbsusy {
class EventInfoFiller;
class GenParticleFiller;
class PFCandidateFiller;
class QuarkGluonTagInterface;
class QuarkGluonTaggingVariables;

namespace JetFunctions {
template<typename Jet> bool passLooseJetId(const Jet& j);
template<typename Jet> bool passTightJetId(const Jet& j);
template<typename Jet> bool passMuonEnergyFraction(const Jet& j);
template<typename Jet> unsigned int countNChargedHadrons(const float threshpt, const Jet& j);
}

class JetFillerBase {
public:
  enum  Options           {
                            NULLOPT         = 0
                          , LOADGEN         = (1 <<  0)   ///< load gen jets
                          , LOADJETSHAPE    = (1 <<  1)   ///< load jet shap variables
                          , LOADBTAG        = (1 <<  2)   ///< load btag info for non-standard jets
                          , SAVETOPASSOC    = (1 <<  3)   ///< save the association to top decays
                          , SAVEQGL         = (1 <<  4)   ///< save the quark-gluon likelihood (for AK4 jets)
                          , LOADSUPER       = (1 <<  5)   ///< load links to super jets
                          , LOADUNCFROMFILE = (1 <<  6)   ///< load jet uncertainties from external file
                          , FILLJETEXTRA    = (1 <<  7)   ///< additional info about jet energy fractions and multiplicities
                          , FILLJETCHHADN   = (1 <<  8)   ///< additional info about jet content: charged hadron multiplicity
  };
  static const int defaultOptions = NULLOPT;
  static const std::string REGENJET;  // userClass label for the redefined genJet of the given jet
};


  template<typename Jet>
  class JetFiller : public JetFillerBase, public BaseFiller {
    public:
      JetFiller(const edm::ParameterSet& cfg, edm::ConsumesCollector && cc, const int options, const string branchName, const EventInfoFiller * evtInfoFiller, const GenParticleFiller * genParticleFiller, const PFCandidateFiller * pfCandidateFiller);
      ~JetFiller() {}

      virtual void load(const edm::Event& iEvent, const edm::EventSetup &iSetup);
      virtual void fill();

      virtual reco::GenJetRef getReGenJet(const Jet& jet,const int index = -1, const bool enforce = false) const  = 0;
      virtual reco::GenJetRef getStdGenJet(const Jet& jet) const = 0;
      virtual reco::CandidatePtr getRecoJet(const size iGen, bool redefined) const = 0;

      virtual float getJecUncorrection(const Jet& jet) const = 0;
      virtual float getPUJetId(const Jet& jet) const = 0;
      virtual float getbDisc(const Jet& jet) const = 0;
      virtual float getcvslDisc(const Jet& jet) const = 0;
      virtual float getcvsbDisc(const Jet& jet) const = 0;
      virtual float getQGDisc(const Jet& jet) const = 0;
      virtual float getBetaStar(const Jet& jet) const = 0;

    protected:
      // Input from the config file
      edm::EDGetTokenT<std::vector<Jet> >          jetToken_;
      edm::EDGetTokenT<pat::PackedCandidateCollection> pfCandToken_;
      edm::EDGetTokenT<reco::GenJetCollection>     reGenJetToken_;
      edm::EDGetTokenT<reco::GenJetCollection>     stdGenJetToken_;
      edm::EDGetTokenT<std::vector<size8> >        flvAssocToken_;
      edm::EDGetTokenT<edm::ValueMap<float> >      qglToken_;
      edm::EDGetTokenT<reco::PFJetCollection>      superJetToken_;
      edm::EDGetTokenT<std::vector<int> >          superJetAssocToken_;
      edm::EDGetTokenT<std::vector<unsigned int> > superJetNsubToken_;
      const EventInfoFiller   * evtInfofiller_;
      const GenParticleFiller * genParticleFiller_;
      const PFCandidateFiller * pfCandidateFiller_;

    protected:
      // Members to hold index of most recently filled tree data
      // For standard genjets
      // For reco jets
      size ijetpt_       ;
      size ijeteta_      ;
      size ijetphi_      ;
      size ijetmass_     ;
      size ijetptraw_    ;
      size ijetpuId_     ;
      size ijetlooseId_  ;
      size ijettightId_  ;
      size ijetchHadEnFrac_ ;
      size ijetchHadN4_     ;
      size ijetchHadN6_     ;
      size ijetchHadN8_     ;
      size ijetchEmEnFrac_  ;
      size ijetmuEnFrac_    ;
      size ijetelEnFrac_    ;
      size ijetneuHadEnFrac_;
      size ijetneuEmEnFrac_ ;
      size ijetphoEnFrac_   ;
      size ijetchMult_      ;
      size ijetneuMult_     ;
      size ijetchHadMult_   ;
      size ijetelMult_      ;
      size ijetmuMult_      ;
      size ijetneuHadMult_  ;
      size ijetphoMult_     ;
      size ijetcsv_      ;
      size ijetcvsl_     ;
      size ijetcvsb_     ;
      size ijetarea_     ;
      size ijetgenindex_ ;
      size ijetsuperindex_ ;
      size ijetuncertainty_;
      // For genjets matched to reco jets
      size igenjetpt_    ;
      size igenjeteta_   ;
      size igenjetphi_   ;
      size igenjetmass_  ;
      size igenjetflavor_;
      // For superjets corresponding to (reco) subjets
      size isuperjetpt_    ;
      size isuperjeteta_   ;
      size isuperjetphi_   ;
      size isuperjetmass_  ;
      size isuperjetnsubjets_;
      // For jetShape info
      size ijetbetaStar_;
      size ijetqgl_     ;
      size ijetptD_     ;
      size ijetaxis1_   ;
      size ijetaxis2_   ;
      size ijetMult_    ;
      size ijetcharge_  ;
      size ijetpullrap_  ;
      size ijetpullphi_  ;
      size igenjetptD_   ;
      size igenjetaxis1_ ;
      size igenjetaxis2_ ;
      size igenjetMult_  ;
      size igenjetcharge_  ;
      size igenjetpullrap_  ;
      size igenjetpullphi_  ;
      //for top assoc
      size iGenAssocPrtIndex_;
      size iGenAssocJetIndex_;
      size iGenAssocCont_;

    protected:
      QuarkGluonTagInterface    * qglInterface_;
      QuarkGluonTaggingVariables* qgTaggingVar_;

      mutable std::vector<reco::CandidatePtr>  reGenRecoRef_ ;
      mutable std::vector<reco::CandidatePtr>  stdGenRecoRef_ ;


    public:
      // Data members
      const bool                              fillReGenJets_;
      const double                            jptCompleteMin_; //To save all info
      const double                            jptMin_; //To save just basic info
      edm::Handle<std::vector<Jet>>           jets_;
      edm::Handle<pat::PackedCandidateCollection> pfcands_;
      edm::Handle<reco::GenJetCollection>     reGenJets_;
      edm::Handle<reco::GenJetCollection>     stdGenJets_;
      edm::Handle<std::vector<size8   > >     flvAssoc_;
      edm::Handle<edm::ValueMap<float>>       qgHandleQGL_;
      edm::Handle<reco::PFJetCollection>      superJets_;
      edm::Handle<std::vector<int     > >     superJetAssoc_;
      edm::Handle<std::vector<unsigned int> > superJetNsub_;

      // JEC uncertainties
      std::string               jecLabel_;
      JetCorrectionUncertainty *jecUnc_;

  };

}

#include "AnalysisBase/Analyzer/src/JetFiller.icc"
#endif
