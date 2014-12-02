#ifndef ANALYSISBASE_ANALYZER_RECOJETFILLER_H
#define ANALYSISBASE_ANALYZER_RECOJETFILLER_H

#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "AnalysisBase/Analyzer/interface/JetFiller.h"

namespace ucsbsusy {
  class RecoJetFiller : public JetFiller<reco::PFJet> {
    public:
    RecoJetFiller(const int options, const string branchName, const EventInfoFiller * evtInfoFiller
          , const edm::InputTag jetTag
          , const edm::InputTag reGenJetTag
          , const edm::InputTag stdGenJetTag
          , const bool fillReGenJets
          , const double jptMin
          );
      virtual ~RecoJetFiller() {}

      virtual void load(const edm::Event& iEvent);

      reco::GenJetRef getReGenJet(const reco::PFJet& jet,const int index = -1, const bool enforce = false)  const;
      reco::GenJetRef getStdGenJet(const reco::PFJet& jet) const;
      reco::CandidatePtr getRecoJet(const size iGen, bool redefined) const;

      //Not currently implemented:
      int   getPartonFlavor(const reco::PFJet& jet) const {return -1;}
      float getJecUncorrection(const reco::PFJet& jet) const { return 1;}
      float getPUJetId(const reco::PFJet& jet) const {return -10;}
      float getbDisc(const reco::PFJet& jet) const {return -10;}
      float getQGDisc(const reco::PFJet& jet) const{ return -10;}
      float getBetaStar(const reco::PFJet& jet) const{ return -10;}

    private:
      edm::Handle<edm::ValueMap<reco::CandidatePtr> > genJetPtr;
  };

}
#endif
