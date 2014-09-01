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

using namespace ucsbsusy;

//--------------------------------------------------------------------------------------------------
JetFiller::JetFiller(const edm::ParameterSet &cfg) :
    jetTag_(cfg.getParameter<edm::InputTag>("jets")),
    reGenJetTag_(cfg.getParameter<edm::InputTag>("reGenJets")),
    stdGenJetTag_(cfg.getParameter<edm::InputTag>("stdGenJets")),
    genParticleTag_(cfg.getParameter<edm::InputTag>("genParticles")),
    jptMin_(cfg.getUntrackedParameter<double>("minJetPt")),
    fillGenInfo(false),
    jets_(0),
    reGenJets_(0),
    stdGenJets_(0)

{}
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
void JetFiller::load(edm::Event& iEvent, bool storeOnlyPtr, bool isMC ){
  enforceGet(iEvent,jetTag_,jets_,true);

  if(isMC){
    fillGenInfo = true;
    enforceGet(iEvent,reGenJetTag_,reGenJets_,true);
    enforceGet(iEvent,stdGenJetTag_,stdGenJets_,true);
    edm::Handle<reco::GenParticleCollection> genParticles_;
    enforceGet(iEvent,genParticleTag_,genParticles_,true);

    std::vector<HadronDecay> bHadrons = JetFlavorMatching::getBHadronDecays(genParticles_);
    JetFlavorMatching::storeBHadronInfo(*jets_,*reGenJets_,bHadrons);
  }

  if(storeOnlyPtr) return;

  jets    .clear();
  genJets .clear();
  v_csv   .clear();
  v_flavor.clear();

  jets    .reserve(jets_->size());
  v_csv   .reserve(jets_->size());
  if(isMC){
  genJets .reserve(jets_->size());
  v_flavor.reserve(jets_->size());
  }

  for(const pat::Jet &j : *jets_) {
    const reco::GenJet * gJ = fillGenInfo ? &(*getReGenJet(j)) : 0;
    if(j.pt() < jptMin_ && (fillGenInfo ? gJ->pt() < jptMin_ : true)) continue;

    int index = jets.size();

    GenJetF * genJet = 0;
    if(fillGenInfo){
      v_flavor.push_back(JetFlavorMatching::getTaggableType(j));
      genJets.emplace_back(gJ->polarP4(), jets.size(),&v_flavor.back());
      genJet = &genJets.back();
    }

    v_csv.push_back(j.bDiscriminator("combinedSecondaryVertexBJetTags"));
    jets.emplace_back(j.polarP4(),index,&v_csv[index],genJet);
  }

  std::sort(jets.begin(),jets.end(), greaterPT<RecoJetF>());
}
//--------------------------------------------------------------------------------------------------
void JetFiller::fill(Planter& plant, int& bookMark, const int& numAnalyzed)
{
  if (!numAnalyzed) {
    plant.checkPoint(bookMark);
    plant.book<float       >("reco_pt"            , "p_{T}^{reco}"                  );
    plant.book<float       >("reco_eta"           , "#eta^{reco}"                   );
    plant.book<float       >("reco_phi"           , "#phi^{reco}"                   );
    plant.book<float       >("reco_mass"          , "m(recoJet)"                    );
    plant.book<float       >("reco_csv"           , "D_{b}(CSV)"                    );

    if(fillGenInfo){
      static const TString   FLAVORNAMES = Procedures::join(JetFlavorMatching::TAGGABLE_NAME, 0, JetFlavorMatching::numTaggableTypes-1, ";") + ";other";
      plant.book<float       >("gen_pt"         , "p_{T}^{gen}");
      plant.book<float       >("gen_eta"        , "#eta^{gen}");
      plant.book<float       >("gen_phi"        , "#phi^{gen}");
      plant.book<float       >("gen_mass"       , "m(genJet)" );
      plant.book<multiplicity>("flavor"         , FLAVORNAMES);
    }

    plant.checkPoint(bookMark + 1);
  }

  for(const RecoJetF& jet : jets){
    //.. Reco quantities ......................................................
    plant.revert(bookMark);
    plant.fills(float( jet.pt     () ));                                               plant.next();
    plant.fills(float( jet.eta    () ));                                               plant.next();
    plant.fills(float( jet.phi    () ));                                               plant.next();
    plant.fills(float( jet.mass   () ));                                               plant.next();
    plant.fills(float( jet.csv    () ));                                               plant.next();

    if(fillGenInfo){
      plant.fills(float( jet.genJet().pt     () ));                                               plant.next();
      plant.fills(float( jet.genJet().eta    () ));                                               plant.next();
      plant.fills(float( jet.genJet().phi    () ));                                               plant.next();
      plant.fills(float( jet.genJet().mass   () ));                                               plant.next();
      plant.fills(convertTo<multiplicity>( jet.genJet().flavor (), "flavor" ));                   plant.next();
    }
  }

  plant.revert(++bookMark);
}
//--------------------------------------------------------------------------------------------------
const std::string   JetFiller::REGENJET           = "GenPtr";
