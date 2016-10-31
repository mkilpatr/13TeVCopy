#ifndef ANALYSISBASE_ANALYZER_PATJETFILLER_H
#define ANALYSISBASE_ANALYZER_PATJETFILLER_H

#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "AnalysisBase/Analyzer/interface/JetFiller.h"

namespace ucsbsusy {
  class PatJetFiller : public JetFiller<pat::Jet> {
    public:
    PatJetFiller(const edm::ParameterSet& cfg, edm::ConsumesCollector && cc, const int options, const string branchName, const EventInfoFiller * evtInfoFiller, const GenParticleFiller * genParticleFiller, const PFCandidateFiller * pfCandidateFiller);
      ~PatJetFiller() {}

      reco::GenJetRef getReGenJet(const pat::Jet& jet,const int index = -1, const bool enforce = false)  const;
      reco::GenJetRef getStdGenJet(const pat::Jet& jet) const;
      reco::CandidatePtr getRecoJet(const size iGen, bool redefined) const;

      float getJecUncorrection(const pat::Jet& jet) const { return jet.pt()*jet.jecFactor("Uncorrected");}
      float getPUJetId(const pat::Jet& jet) const {return jet.userFloat("pileupJetId:fullDiscriminant");}
      float getbDisc(const pat::Jet& jet) const {return jet.bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags");}
      float getcmvaDisc(const pat::Jet& jet) const {return jet.bDiscriminator("pfCombinedMVAV2BJetTags");}
      float getcvslDisc(const pat::Jet& jet) const {return jet.bDiscriminator("pfCombinedCvsLJetTags");}
      float getcvsbDisc(const pat::Jet& jet) const {return jet.bDiscriminator("pfCombinedCvsBJetTags");}
      float getQGDisc(const pat::Jet& jet) const{ return -10;}
//      float getQGDisc(const pat::Jet& jet) const{ return qglInterface_->getDiscriminator(jet,*evtInfofiller_->rho_);}
      float getBetaStar(const pat::Jet& jet) const{ return qgTaggingVar_->getBetaStar(&jet,*evtInfofiller_->vertices_,evtInfofiller_->primaryVertexIndex_);}
  };

}
#endif
