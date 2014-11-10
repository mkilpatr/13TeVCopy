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
#define REDEFINED_GENJET_HACK

const int JetFiller::defaultOptions = JetFiller::NULLOPT;

//--------------------------------------------------------------------------------------------------
JetFiller::JetFiller(const int options, const string branchName, const string genJetsBranchName, const EventInfoFiller * evtInfoFiller
  , const edm::InputTag jetTag
  , const edm::InputTag reGenJetTag
  , const edm::InputTag stdGenJetTag
  , const double jptMin
) : BaseFiller(options, branchName), evtInfofiller_   (evtInfoFiller)
  , jetTag_          (jetTag)
  , reGenJetTag_     (reGenJetTag)
  , stdGenJetTag_    (stdGenJetTag)
//  genParticleTag_  (fillGenInfo_      ? cfg.getParameter<edm::InputTag>("genParticles") : edm::InputTag()),
  , jptMin_          (jptMin)
  , qglInterface_    (new QuarkGluonTagInterface)
  , qgTaggingVar_    (new QuarkGluonTaggingVariables)
  , jets_            (0)
  , reGenJets_       (0)
  , stdGenJets_      (0)
{
  if(options_ & LOADGEN) {
    igenjetpt_            = data.addMulti<float>(genJetsBranchName ,"genjet_pt"    ,0);
    igenjeteta_           = data.addMulti<float>(genJetsBranchName ,"genjet_eta"   ,0);
    igenjetphi_           = data.addMulti<float>(genJetsBranchName ,"genjet_phi"   ,0);
    igenjetmass_          = data.addMulti<float>(genJetsBranchName ,"genjet_mass"  ,0);
  }

  ijetpt_     = data.addMulti<float>(branchName_,"jet_pt"    ,0);
  ijeteta_    = data.addMulti<float>(branchName_,"jet_eta"   ,0);
  ijetphi_    = data.addMulti<float>(branchName_,"jet_phi"   ,0);
  ijetmass_   = data.addMulti<float>(branchName_,"jet_mass"  ,0);
  ijetptraw_  = data.addMulti<float>(branchName_,"jet_ptraw" ,0);
  ijetpuId_   = data.addMulti<float>(branchName_,"jet_puId"  ,0);
  ijetcsv_    = data.addMulti<float>(branchName_,"jet_csv"   ,0);
  ijetflavor_ = data.addMulti<int  >(branchName_,"jet_flavor",0);

  if(options_ & LOADGEN) {
    imatchedgenjetpt_     = data.addMulti<float>(branchName_,"matchedgenjet_pt"    ,0);
    imatchedgenjeteta_    = data.addMulti<float>(branchName_,"matchedgenjet_eta"   ,0);
    imatchedgenjetphi_    = data.addMulti<float>(branchName_,"matchedgenjet_phi"   ,0);
    imatchedgenjetmass_   = data.addMulti<float>(branchName_,"matchedgenjet_mass"  ,0);
    imatchedgenjetflavor_ = data.addMulti<int  >(branchName_,"matchedgenjet_flavor",0);
  }

  if(options_ & LOADJETSHAPE) {
    ijetbetaStar_=data.addMulti<float>(branchName_,"jet_betaStar",0);
    ijetqgl_     =data.addMulti<float>(branchName_,"jet_qgl"     ,0);
    ijetptD_     =data.addMulti<float>(branchName_,"jet_ptD"     ,0);
    ijetaxis1_   =data.addMulti<float>(branchName_,"jet_axis1"   ,0);
    ijetaxis2_   =data.addMulti<float>(branchName_,"jet_axis2"   ,0);
    ijetMult_    =data.addMulti<int  >(branchName_,"jet_jetMult" ,0);
    if(options_ & LOADGEN) {
      imatchedgenjetptD_  = data.addMulti<float>(branchName_,"matchedgenjet_ptD"    ,0);
      imatchedgenjetaxis1_= data.addMulti<float>(branchName_,"matchedgenjet_axis1"  ,0);
      imatchedgenjetaxis2_= data.addMulti<float>(branchName_,"matchedgenjet_axis2"  ,0);
      imatchedgenjetMult_ = data.addMulti<int  >(branchName_,"matchedgenjet_jetMult",0);
    }
  }
}

//--------------------------------------------------------------------------------------------------
reco::GenJetRef JetFiller::getReGenJet(const pat::Jet& jet) const
{
  if (!reGenJets_.isValid())
    throw cms::Exception("JetFiller.getReGenJet()", "genJets have not been loaded.");

  const reco::CandidatePtr&   genPtr  = jet.userCand(REGENJET);
  if (genPtr.id() != reGenJets_.id())
    throw cms::Exception("JetFiller.getReGenJet()", "Inconsistent reGenJets collection with pat::Jet::userCand(REGENJET).");

  return reco::GenJetRef(reGenJets_, genPtr.key());
}

//--------------------------------------------------------------------------------------------------
reco::GenJetRef JetFiller::getStdGenJet(const pat::Jet& jet) const
{
  return jet.genJetFwdRef().backRef();
}

//--------------------------------------------------------------------------------------------------
void JetFiller::load(const edm::Event& iEvent)
{
  reset();
  FileUtilities::enforceGet(iEvent,jetTag_,jets_,true);

  if(options_ & LOADGEN) {
    FileUtilities::enforceGet(iEvent,reGenJetTag_,reGenJets_,true);
    FileUtilities::enforceGet(iEvent,stdGenJetTag_,stdGenJets_,true);
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

  // store all standard genjets
  if(options_ & LOADGEN) {
    for(const reco::GenJet &gj : *stdGenJets_) {
      data.fillMulti<float>(igenjetpt_  ,gj.pt());
      data.fillMulti<float>(igenjeteta_ ,gj.eta());
      data.fillMulti<float>(igenjetphi_ ,gj.phi());
      data.fillMulti<float>(igenjetmass_,gj.mass());
    }
  }

  int index = -1;
  for (const pat::Jet &j : *jets_) {

#ifdef REDEFINED_GENJET_HACK
    const reco::GenJetRef gJ = getStdGenJet(j);
    if(j.pt() < jptMin_ && ((options_ & LOADGEN) ? (gJ.isNonnull() ? gJ->pt() : 0) < jptMin_ : true)) continue;
#else
    const reco::GenJet * gJ = fillGenInfo_ ? &(*getReGenJet(j)) : 0;
    if(j.pt() < jptMin_ && (fillGenInfo_ ? gJ->pt() : 0) < jptMin_ : true) continue;
#endif
    index++;

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
	data.fillMulti<float>(imatchedgenjetpt_    ,gJ->pt());
	data.fillMulti<float>(imatchedgenjeteta_   ,gJ->eta());
	data.fillMulti<float>(imatchedgenjetphi_   ,gJ->phi());
	data.fillMulti<float>(imatchedgenjetmass_  ,gJ->mass());
	data.fillMulti<int  >(imatchedgenjetflavor_,j.partonFlavour());
      } else {
        data.fillMulti<float>(imatchedgenjetpt_    );
        data.fillMulti<float>(imatchedgenjeteta_   );
        data.fillMulti<float>(imatchedgenjetphi_   );
        data.fillMulti<float>(imatchedgenjetmass_  );
        data.fillMulti<int  >(imatchedgenjetflavor_);
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
      if(options_ & LOADGEN){
        if(gJ.isNonnull()) {
          qgTaggingVar_->compute(gJ.get());
          data.fillMulti<float>(imatchedgenjetptD_     ,qgTaggingVar_->getPtD());
          data.fillMulti<float>(imatchedgenjetaxis1_   ,qgTaggingVar_->getAxis1());
          data.fillMulti<float>(imatchedgenjetaxis2_   ,qgTaggingVar_->getAxis2());
          data.fillMulti<float>(imatchedgenjetMult_    ,qgTaggingVar_->getTotalMult());
        } else {
          data.fillMulti<float>(imatchedgenjetptD_  );
          data.fillMulti<float>(imatchedgenjetaxis1_);
          data.fillMulti<float>(imatchedgenjetaxis2_);
          data.fillMulti<int  >(imatchedgenjetMult_ );
        }
      }
    }
  }
  isFilled_ = true;
}

//--------------------------------------------------------------------------------------------------
const std::string   JetFiller::REGENJET           = "GenPtr";
