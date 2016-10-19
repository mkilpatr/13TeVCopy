#include "AnalysisBase/Analyzer/interface/RecoJetFiller.h"
//--------------------------------------------------------------------------------------------------
RecoJetFiller::RecoJetFiller(const edm::ParameterSet& cfg, edm::ConsumesCollector && cc, const int options, const string branchName, const EventInfoFiller * evtInfoFiller, const GenParticleFiller * genParticleFiller, const PFCandidateFiller * pfCandidateFiller) :
  JetFiller<reco::PFJet>(cfg, static_cast<edm::ConsumesCollector&&>(cc), options, branchName, evtInfoFiller, genParticleFiller, pfCandidateFiller),
  bTagsToken_           (cc.consumes<reco::JetTagCollection>(cfg.getParameter<edm::InputTag>("btags"))),
  reGenJetAssocToken_   (cc.consumes<edm::ValueMap<reco::CandidatePtr> >(cfg.getParameter<edm::InputTag>("reGenJetAssoc")))
{}

//--------------------------------------------------------------------------------------------------
void RecoJetFiller::load(const edm::Event& iEvent, const edm::EventSetup &iSetup){
  JetFiller<reco::PFJet>::load(iEvent, iSetup);
  if(fillReGenJets_) iEvent.getByToken(reGenJetAssocToken_,genJetPtr_);
  if(options_ & LOADBTAG) iEvent.getByToken(bTagsToken_,btags_);
}

//--------------------------------------------------------------------------------------------------
float RecoJetFiller::getbDisc(const reco::PFJet& jet) const
{

  if(!btags_.isValid())
    return -10;

  return reco::JetFloatAssociation::getValue(*btags_, jet);

}

//--------------------------------------------------------------------------------------------------
float RecoJetFiller::getcmvaDisc(const reco::PFJet& jet) const
{

  if(!btags_.isValid())
    return -10;

  return reco::JetFloatAssociation::getValue(*btags_, jet);

}

//--------------------------------------------------------------------------------------------------
float RecoJetFiller::getcvslDisc(const reco::PFJet& jet) const
{

  if(!btags_.isValid())
    return -10;

  return reco::JetFloatAssociation::getValue(*btags_, jet);

}


float RecoJetFiller::getcvsbDisc(const reco::PFJet& jet) const
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
