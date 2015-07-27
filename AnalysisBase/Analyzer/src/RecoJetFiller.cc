#include "AnalysisBase/Analyzer/interface/RecoJetFiller.h"
//--------------------------------------------------------------------------------------------------
RecoJetFiller::RecoJetFiller(const int options, const string branchName, const EventInfoFiller * evtInfoFiller, const GenParticleFiller * genParticleFiller
  , const edm::InputTag jetTag
  , const edm::InputTag bTagsTag
  , const edm::InputTag reGenJetTag
  , const edm::InputTag stdGenJetTag
  , const edm::InputTag flvAssocTag
  , const edm::InputTag reGenJetAssocTag
  , const bool   fillReGenJets
  , const double jptMin
  , const edm::InputTag superJetTag
  , const edm::InputTag superJetAssocTag
  , const edm::InputTag superJetNsubTag
) : JetFiller<reco::PFJet>(options, branchName, evtInfoFiller, genParticleFiller
    , jetTag
    , reGenJetTag
    , stdGenJetTag
    , flvAssocTag
    , fillReGenJets
    , jptMin
    , superJetTag
    , superJetAssocTag
    , superJetNsubTag
    )
  , bTagsTag_(bTagsTag)
  , reGenJetAssocTag_(reGenJetAssocTag)
{}


//--------------------------------------------------------------------------------------------------
void RecoJetFiller::load(const edm::Event& iEvent, const edm::EventSetup &iSetup){
  JetFiller<reco::PFJet>::load(iEvent, iSetup);
  if(fillReGenJets_) FileUtilities::enforceGet(iEvent,reGenJetAssocTag_,genJetPtr_,true);
  if(options_ & LOADBTAG) FileUtilities::enforceGet(iEvent,bTagsTag_,btags_,true);
}

//--------------------------------------------------------------------------------------------------
float RecoJetFiller::getbDisc(const reco::PFJet& jet) const
{

  if(!btags_.isValid())
    return -10;

  return reco::JetFloatAssociation::getValue(*btags_, jet);

}

//--------------------------------------------------------------------------------------------------
reco::GenJetRef RecoJetFiller::getReGenJet(const reco::PFJet& jet,const int index, const bool enforce) const
{
  if (!reGenJets_.isValid())
    throw cms::Exception("JetFiller.getReGenJet()", "genJets have not been loaded.");

  const reco::CandidatePtr&   genPtr  = (*genJetPtr_)[reco::CandidatePtr(jets_, index)];

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
