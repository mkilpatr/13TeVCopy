#include "AnalysisBase/Analyzer/interface/RecoJetFiller.h"
//--------------------------------------------------------------------------------------------------
RecoJetFiller::RecoJetFiller(const int options, const string branchName, const string genJetsBranchName, const EventInfoFiller * evtInfoFiller
  , const edm::InputTag jetTag
  , const edm::InputTag reGenJetTag
  , const edm::InputTag stdGenJetTag
  , const bool   fillReGenJets
  , const double jptMin
) : JetFiller<reco::PFJet>(options, branchName, genJetsBranchName, evtInfoFiller
    , jetTag
    , reGenJetTag
    , stdGenJetTag
    , fillReGenJets
    , jptMin
    )
{}


void RecoJetFiller::load(const edm::Event& iEvent){
  JetFiller<reco::PFJet>::load(iEvent);
  edm::InputTag ptrTag(reGenJetTag_.label(),"GenPtr");
  if(fillReGenJets_) FileUtilities::enforceGet(iEvent,ptrTag,genJetPtr,true);
}

//--------------------------------------------------------------------------------------------------
reco::GenJetRef RecoJetFiller::getReGenJet(const reco::PFJet& jet,const int index, const bool enforce) const
{
  if (!reGenJets_.isValid())
    throw cms::Exception("JetFiller.getReGenJet()", "genJets have not been loaded.");

  const reco::CandidatePtr&   genPtr  = (*genJetPtr)[reco::CandidatePtr(jets_, index)];

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
reco::GenJetRef RecoJetFiller::getStdGenJet(const reco::PFJet& jet) const
{
  //currently no standard gen jets for reco jets
  return reco::GenJetRef();
}
//_____________________________________________________________________________
reco::CandidatePtr RecoJetFiller::getRecoJet(size iGen, bool redefined) const
{
  vector<reco::CandidatePtr>&        recoJetRef  = ( redefined ? reGenRecoRef_ : stdGenRecoRef_ );
  if (recoJetRef.size())      return recoJetRef[iGen];

  const edm::Handle<vector<reco::GenJet> >&  genJets = ( redefined ? reGenJets_ : stdGenJets_ );
  recoJetRef.resize(genJets->size());

  for (size iReco = 0; iReco < jets_->size(); ++iReco) {
    const reco::PFJet&           recoJet     = (*jets_)[iReco];
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