//--------------------------------------------------------------------------------------------------
// 
// JetFiller
// 
// Class to fill some basic jet information for testing. To be developed as needed.
// 
// JetFiller.cc created on Thu Aug 14 11:10:55 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisBase/Analyzer/interface/JetFiller.h"
#include "AnalysisTools/Utilities/interface/JetFlavorMatching.h"
#include "AnalysisTools/Utilities/interface/Types.h"

#include "AnalysisBase/Analyzer/interface/EventInfoFiller.h"
#include "AnalysisTools/JetShapeVariables/interface/QuarkGluonTagInterface.h"
#include "AnalysisTools/JetShapeVariables/interface/QuarkGluonTaggingVariables.h"

using namespace ucsbsusy;

#define TAGGABLE_TYPE_HACK

const int JetFiller::defaultOptions = JetFiller::NULLOPT;

//--------------------------------------------------------------------------------------------------
JetFiller::JetFiller(const int options, const string branchName, const string genJetsBranchName, const EventInfoFiller * evtInfoFiller
  , const edm::InputTag jetTag
  , const edm::InputTag reGenJetTag
  , const edm::InputTag stdGenJetTag
  , const bool   fillReGenJets
  , const double jptMin
) : BaseFiller(options, branchName), evtInfofiller_   (evtInfoFiller)
  , jetTag_          (jetTag)
  , reGenJetTag_     (reGenJetTag)
  , stdGenJetTag_    (stdGenJetTag)
//  genParticleTag_  (fillGenInfo_      ? cfg.getParameter<edm::InputTag>("genParticles") : edm::InputTag()),
  , jptMin_          (jptMin)
  , fillReGenJets_   (fillReGenJets)
  , qglInterface_    (new QuarkGluonTagInterface)
  , qgTaggingVar_    (new QuarkGluonTaggingVariables)
  , jets_            (0)
  , reGenJets_       (0)
  , stdGenJets_      (0)
{
  ijetpt_     = data.addMulti<float>(branchName_,"jet_pt"    ,0);
  ijeteta_    = data.addMulti<float>(branchName_,"jet_eta"   ,0);
  ijetphi_    = data.addMulti<float>(branchName_,"jet_phi"   ,0);
  ijetmass_   = data.addMulti<float>(branchName_,"jet_mass"  ,0);
  ijetptraw_  = data.addMulti<float>(branchName_,"jet_ptraw" ,0);
  ijetpuId_   = data.addMulti<float>(branchName_,"jet_puId"  ,0);
  ijetcsv_    = data.addMulti<float>(branchName_,"jet_csv"   ,0);

  if(options_ & LOADGEN) {
    ijetflavor_ = data.addMulti<int  >(branchName_,"jet_flavor",0);
    ijetgenindex_ = data.addMulti<int  >(branchName_,"jet_genIndex",-1);
    igenjetpt_     = data.addMulti<float>(branchName_,"genjet_pt"    ,0);
    igenjeteta_    = data.addMulti<float>(branchName_,"genjet_eta"   ,0);
    igenjetphi_    = data.addMulti<float>(branchName_,"genjet_phi"   ,0);
    igenjetmass_   = data.addMulti<float>(branchName_,"genjet_mass"  ,0);
  }

  if(options_ & LOADJETSHAPE) {
    ijetbetaStar_=data.addMulti<float>(branchName_,"jet_betaStar",0);
    ijetqgl_     =data.addMulti<float>(branchName_,"jet_qgl"     ,0);
    ijetptD_     =data.addMulti<float>(branchName_,"jet_ptD"     ,0);
    ijetaxis1_   =data.addMulti<float>(branchName_,"jet_axis1"   ,0);
    ijetaxis2_   =data.addMulti<float>(branchName_,"jet_axis2"   ,0);
    ijetMult_    =data.addMulti<int  >(branchName_,"jet_jetMult" ,0);
    if(options_ & LOADGEN) {
      igenjetptD_  = data.addMulti<float>(branchName_,"genjet_ptD"    ,0);
      igenjetaxis1_= data.addMulti<float>(branchName_,"genjet_axis1"  ,0);
      igenjetaxis2_= data.addMulti<float>(branchName_,"genjet_axis2"  ,0);
      igenjetMult_ = data.addMulti<int  >(branchName_,"genjet_jetMult",0);
    }
  }
}

//--------------------------------------------------------------------------------------------------
reco::GenJetRef JetFiller::getReGenJet(const pat::Jet& jet, const bool enforce) const
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
reco::GenJetRef JetFiller::getStdGenJet(const pat::Jet& jet) const
{
  return jet.genJetFwdRef().backRef();
}
//_____________________________________________________________________________
reco::CandidatePtr JetFiller::getRecoJet(size iGen, bool redefined) const
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
//--------------------------------------------------------------------------------------------------
void JetFiller::load(const edm::Event& iEvent)
{
  reset();
  FileUtilities::enforceGet(iEvent,jetTag_,jets_,true);

  if(options_ & LOADGEN) {
    if(fillReGenJets_) FileUtilities::enforceGet(iEvent,reGenJetTag_,reGenJets_,true);
    else FileUtilities::enforceGet(iEvent,stdGenJetTag_,stdGenJets_,true);
//    edm::Handle<reco::GenParticleCollection> genParticles_;
//    FileUtilities::enforceGet(iEvent,genParticleTag_,genParticles_,true);

#ifndef TAGGABLE_TYPE_HACK
    std::vector<HadronDecay> bHadrons = JetFlavorMatching::getBHadronDecays(genParticles_);
    JetFlavorMatching::storeBHadronInfo(*jets_,*reGenJets_,bHadrons);
#endif
  }

  isLoaded_ = true;

}

//--------------------------------------------------------------------------------------------------
void JetFiller::fill()
{
  vector<int> filledIndex;
  int curGenIndex = -1;
  if(options_ & LOADGEN){
    const reco::GenJetCollection& genJets = fillReGenJets_ ? (*reGenJets_.product()) : (*stdGenJets_.product());
    filledIndex.reserve(genJets.size());
    for (const reco::Jet &j : genJets) {
      if(j.pt() < jptMin_){ filledIndex.push_back(-1); continue;}
      filledIndex.push_back(++curGenIndex);
      data.fillMulti<float>(igenjetpt_    ,j.pt());
      data.fillMulti<float>(igenjeteta_   ,j.eta());
      data.fillMulti<float>(igenjetphi_   ,j.phi());
      data.fillMulti<float>(igenjetmass_  ,j.mass());
      if(options_ & LOADJETSHAPE){
        assert(evtInfofiller_->isLoaded());
        qgTaggingVar_->compute(&j);
        data.fillMulti<float>(igenjetptD_     ,qgTaggingVar_->getPtD());
        data.fillMulti<float>(igenjetaxis1_   ,qgTaggingVar_->getAxis1());
        data.fillMulti<float>(igenjetaxis2_   ,qgTaggingVar_->getAxis2());
        data.fillMulti<float>(igenjetMult_    ,qgTaggingVar_->getTotalMult());
      }
    }
  }



  for (const pat::Jet &j : *jets_) {
    const reco::GenJetRef gJ = fillReGenJets_ ? getReGenJet(j) : getStdGenJet(j);
    if(j.pt() < jptMin_) continue;

    data.fillMulti<float>(ijetpt_   ,j.pt());
    data.fillMulti<float>(ijeteta_  ,j.eta());
    data.fillMulti<float>(ijetphi_  ,j.phi());
    data.fillMulti<float>(ijetmass_ ,j.mass());
    data.fillMulti<float>(ijetptraw_,j.pt()*j.jecFactor("Uncorrected"));
    data.fillMulti<float>(ijetpuId_ ,j.userFloat("pileupJetId:fullDiscriminant"));
    data.fillMulti<float>(ijetcsv_  ,j.bDiscriminator("combinedInclusiveSecondaryVertexBJetTags"));

    if(options_ & LOADGEN) {
      data.fillMulti<int>(ijetflavor_,j.partonFlavour());
      if(gJ.isNonnull()) {
        data.fillMulti<int>(ijetgenindex_,filledIndex[gJ.key()]);
      } else {
        data.fillMulti<int>(ijetgenindex_   );
      }
    }

    if(options_ & LOADJETSHAPE){
      assert(evtInfofiller_->isLoaded());
      data.fillMulti<float>(ijetqgl_,qglInterface_->getDiscriminator(j,*evtInfofiller_->rho_));
      qgTaggingVar_->compute(&j);
      data.fillMulti<float>(ijetbetaStar_,qgTaggingVar_->getBetaStar(&j,*evtInfofiller_->vertices_,evtInfofiller_->primaryVertexIndex_));
      data.fillMulti<float>(ijetptD_     ,qgTaggingVar_->getPtD());
      data.fillMulti<float>(ijetaxis1_   ,qgTaggingVar_->getAxis1());
      data.fillMulti<float>(ijetaxis2_   ,qgTaggingVar_->getAxis2());
      data.fillMulti<int  >(ijetMult_    ,qgTaggingVar_->getTotalMult());
    }

  }

  isFilled_ = true;
}


//--------------------------------------------------------------------------------------------------
const std::string   JetFiller::REGENJET           = "GenPtr";
