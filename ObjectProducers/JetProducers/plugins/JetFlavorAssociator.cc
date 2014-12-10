#ifndef OBJECTPRODUCERS_JETPRODUCERS_JETFLAVORASSOCIATOR_H
#define OBJECTPRODUCERS_JETPRODUCERS_JETFLAVORASSOCIATOR_H

#include <functional>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"


#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/Utilities/interface/JetFlavorMatching.h"

class JetFlavorAssociator : public edm::EDProducer {
public:
  JetFlavorAssociator(const edm::ParameterSet& iConfig);
  virtual ~JetFlavorAssociator(){};
  virtual void produce(edm::Event& iEvent, const edm::EventSetup& iSetup);


protected:
  const edm::InputTag genJetSrc       ;
  const edm::InputTag genMotherSrc    ;
  const edm::InputTag genSrc          ;
  const double        coneSize        ;
 };

//--------------------------------------------------------------------------------------------------
JetFlavorAssociator::JetFlavorAssociator(const edm::ParameterSet& iConfig)
        : genJetSrc       (iConfig.getParameter<edm::InputTag>("genJetsSrc"))
        , genMotherSrc    (iConfig.getParameter<edm::InputTag>("genMotherSrc"))
        , genSrc          (iConfig.getParameter<edm::InputTag>("genSrc"))
        , coneSize        (iConfig.getParameter<double       >("coneSize"))
    {

      produces< std::vector<size8> >("flavors");
      produces< std::vector<float> >("bHadronPTs");
      produces< std::vector<float> >("cHadronPTs");
    }

//--------------------------------------------------------------------------------------------------
void JetFlavorAssociator::produce(edm::Event& iEvent, const edm::EventSetup& iSetup){
  edm::Handle<reco::GenJetCollection>           genJets           ;
  edm::Handle<reco::GenParticleCollection>      genMotherParticles;
  edm::Handle<pat::PackedGenParticleCollection> packedGenParticles;


  iEvent.getByLabel(genJetSrc   ,genJets           );
  iEvent.getByLabel(genMotherSrc,genMotherParticles);
  iEvent.getByLabel(genSrc      ,packedGenParticles);

  std::vector<JetFlavorMatching::ParticleDecay> bHadrons;
  std::vector<JetFlavorMatching::ParticleDecay> cHadrons;
  std::vector<JetFlavorMatching::ParticleDecay> partons;
  std::vector<int> partonParticleAssoc(packedGenParticles->size(),-1);


  JetFlavorMatching::getHadronDecays(genMotherParticles,bHadrons,cHadrons);
  JetFlavorMatching::getPartons(genMotherParticles,partons, true);

  JetFlavorMatching::associateDecayProducts(genMotherParticles,packedGenParticles,
      &bHadrons,&cHadrons,&partons,&partonParticleAssoc);

  std::vector<JetFlavorMatching::ParticleContainments> mainBHadrons;
  std::vector<JetFlavorMatching::ParticleContainments> satelliteBHadrons;

  std::vector<JetFlavorMatching::ParticleContainments> mainCHadrons;
  std::vector<JetFlavorMatching::ParticleContainments> satelliteCHadrons;

  JetFlavorMatching::associateHadronsToJets(packedGenParticles,*genJets,bHadrons,mainBHadrons,satelliteBHadrons);
  JetFlavorMatching::associateHadronsToJets(packedGenParticles,*genJets,cHadrons,mainCHadrons,satelliteCHadrons);

  std::vector<JetFlavorInfo::JetFlavor>                          flavors          (genJets->size(),JetFlavorInfo::unmatched_jet);
  std::vector<std::vector<JetFlavorMatching::ParticleDecayRef> > matchedParticles (genJets->size());
  std::auto_ptr<std::vector<float> >                             bHadronPTs       (new std::vector<float>(genJets->size(),0));
  std::auto_ptr<std::vector<float> >                             cHadronPTs       (new std::vector<float>(genJets->size(),0));

  JetFlavorMatching::assignJetHadronFlavors(*genJets,JetFlavorInfo::b_jet, mainBHadrons,satelliteBHadrons,flavors,*bHadronPTs,matchedParticles);
  JetFlavorMatching::assignJetHadronFlavors(*genJets,JetFlavorInfo::c_jet, mainCHadrons,satelliteCHadrons,flavors,*cHadronPTs,matchedParticles);

  //if cone size is  negative calculate the radius of each jet based on the area of the jet
  if(coneSize < 0){
    std::vector<double> cusRad2(genJets->size(),0);
    for(unsigned int iJ  = 0; iJ < genJets->size(); ++iJ)
      cusRad2[iJ] = genJets->at(iJ).jetArea()/TMath::Pi();

    JetFlavorMatching::assignJetPartonFlavors(*genJets,partons,flavors,matchedParticles,0,&cusRad2);
  } else {
    JetFlavorMatching::assignJetPartonFlavors(*genJets,partons,flavors,matchedParticles,coneSize*coneSize);
  }


  std::auto_ptr<std::vector<size8> >   outFlavors(new std::vector<size8>(flavors.size()) );
  for(unsigned int iF = 0; iF < flavors.size(); ++iF)
    outFlavors->at(iF) = flavors[iF];

  iEvent.put(outFlavors, "flavors");
  iEvent.put(bHadronPTs, "bHadronPTs");
  iEvent.put(cHadronPTs, "cHadronPTs");

}





DEFINE_FWK_MODULE(JetFlavorAssociator);
#endif
