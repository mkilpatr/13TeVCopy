#include "AnalysisBase/Analyzer/interface/PatJetFiller.h"
//--------------------------------------------------------------------------------------------------
PatJetFiller::PatJetFiller(const edm::ParameterSet& cfg, edm::ConsumesCollector && cc, const int options, const string branchName, const EventInfoFiller * evtInfoFiller, const GenParticleFiller * genParticleFiller) :
 JetFiller<pat::Jet>(cfg, static_cast<edm::ConsumesCollector&&>(cc), options, branchName, evtInfoFiller, genParticleFiller)
{}

//--------------------------------------------------------------------------------------------------
reco::GenJetRef PatJetFiller::getReGenJet(const pat::Jet& jet,const int index, const bool enforce) const
{
  if (!reGenJets_.isValid())
    throw cms::Exception("JetFiller.getReGenJet()", "genJets have not been loaded.");

  const reco::CandidatePtr&   genPtr  = jet.userCand(REGENJET);

  if(genPtr.isNull()){
    if(enforce)
      throw cms::Exception("JetFiller.getReGenJet()", "Could not find genJet!");
    else
      return reco::GenJetRef();
  }


  if (genPtr.id() != reGenJets_.id())
    throw cms::Exception("JetFiller.getReGenJet()", "Inconsistent reGenJets collection with pat::Jet::userCand(REGENJET).");

  return reco::GenJetRef(reGenJets_, genPtr.key());
}

//--------------------------------------------------------------------------------------------------
reco::GenJetRef PatJetFiller::getStdGenJet(const pat::Jet& jet) const
{
  return jet.genJetFwdRef().backRef();
}
//_____________________________________________________________________________
reco::CandidatePtr PatJetFiller::getRecoJet(size iGen, bool redefined) const
{
  vector<reco::CandidatePtr>&        recoJetRef  = ( redefined ? reGenRecoRef_ : stdGenRecoRef_ );
  if (recoJetRef.size())      return recoJetRef[iGen];

  const edm::Handle<vector<reco::GenJet> >&  genJets = ( redefined ? reGenJets_ : stdGenJets_ );
  recoJetRef.resize(genJets->size());

  for (size iReco = 0; iReco < jets_->size(); ++iReco) {
    const pat::Jet&           recoJet     = (*jets_)[iReco];
    int                       genIndex    = -1;
    if (redefined) {
      const reco::GenJetRef  genJetRef                = getReGenJet(recoJet,false);
      genIndex                = ( genJetRef.isNull() ? -1 : int(genJetRef.key()) );
    } else {
      const reco::GenJetRef&  genJetRef   = getStdGenJet(recoJet);
      genIndex                = ( genJetRef.isNull() ? -1 : int(genJetRef.key()) );
    }
    if (genIndex >= 0)        recoJetRef[genIndex]  = reco::CandidatePtr(jets_, iReco);
  } // end loop over recoJets

  return recoJetRef[iGen];
}
