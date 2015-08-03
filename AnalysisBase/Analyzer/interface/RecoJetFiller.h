#ifndef ANALYSISBASE_ANALYZER_RECOJETFILLER_H
#define ANALYSISBASE_ANALYZER_RECOJETFILLER_H

#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "AnalysisBase/Analyzer/interface/JetFiller.h"

namespace ucsbsusy {
  class RecoJetFiller : public JetFiller<reco::PFJet> {
    public:
      RecoJetFiller(const edm::ParameterSet& cfg, edm::ConsumesCollector && cc, const int options, const string branchName, const EventInfoFiller * evtInfoFiller, const GenParticleFiller * genParticleFiller);
      virtual ~RecoJetFiller() {}

      virtual void load(const edm::Event& iEvent,edm::EventSetup const &isetup);

      reco::GenJetRef getReGenJet(const reco::PFJet& jet,const int index = -1, const bool enforce = false)  const;
      reco::GenJetRef getStdGenJet(const reco::PFJet& jet) const;
      reco::CandidatePtr getRecoJet(const size iGen, bool redefined) const;

      //Not currently implemented:
      float getJecUncorrection(const reco::PFJet& jet) const { return 1;}
      float getPUJetId(const reco::PFJet& jet) const {return -10;}
      float getbDisc(const reco::PFJet& jet) const;
      float getQGDisc(const reco::PFJet& jet) const{ return -10;}
      float getBetaStar(const reco::PFJet& jet) const{  return qgTaggingVar_->getBetaStar(&jet,*evtInfofiller_->vertices_,evtInfofiller_->primaryVertexIndex_);}

    protected:
      edm::EDGetTokenT<reco::JetTagCollection>             bTagsToken_;
      edm::EDGetTokenT<edm::ValueMap<reco::CandidatePtr> > reGenJetAssocToken_;

    private:
      edm::Handle<reco::JetTagCollection>             btags_;
      edm::Handle<edm::ValueMap<reco::CandidatePtr> > genJetPtr_;
  };

}
#endif
