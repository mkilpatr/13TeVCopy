#ifndef ANALYSISBASE_ANALYZER_PATJETFILLER_H
#define ANALYSISBASE_ANALYZER_PATJETFILLER_H

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "AnalysisBase/Analyzer/interface/JetFiller.h"

namespace ucsbsusy {
  class PatJetFiller : public JetFiller<pat::Jet> {
    public:
    PatJetFiller(const int options, const string branchName, const string genJetsBranchName, const EventInfoFiller * evtInfoFiller
          , const edm::InputTag jetTag
          , const edm::InputTag reGenJetTag
          , const edm::InputTag stdGenJetTag
          , const bool fillReGenJets
          , const double jptMin
          );
      ~PatJetFiller() {}

      reco::GenJetRef getReGenJet(const pat::Jet& jet, const bool enforce = false)  const;
      reco::GenJetRef getStdGenJet(const pat::Jet& jet) const;
      reco::CandidatePtr getRecoJet(const size iGen, bool redefined) const;

      float getJecUncorrection(const pat::Jet& jet) const { return jet.jecFactor("Uncorrected");}
      float getPUJetId(const pat::Jet& jet) const {return jet.userFloat("pileupJetId:fullDiscriminant");}
      float getbDisc(const pat::Jet& jet) const {return jet.bDiscriminator("combinedInclusiveSecondaryVertexBJetTags");}
      float getQGDisc(const pat::Jet& jet) const{ return qglInterface_->getDiscriminator(jet,*evtInfofiller_->rho_);}
  };

}
#endif
