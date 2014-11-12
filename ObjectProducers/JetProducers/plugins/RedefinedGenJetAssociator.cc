#ifndef OBJECTPRODUCERS_JETPRODUCERS_REDEFINEDJETASSOCIATOR_H
#define OBJECTPRODUCERS_JETPRODUCERS_REDEFINEDJETASSOCIATOR_H

#include <functional>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"

class RedefinedGenJetAssociator : public edm::EDFilter {
public:
  RedefinedGenJetAssociator(const edm::ParameterSet& iConfig);
  virtual ~RedefinedGenJetAssociator(){};
  virtual bool filter(edm::Event& iEvent, const edm::EventSetup& iSetup);
  virtual void load(edm::Event& iEvent, const edm::EventSetup& iSetup);


protected:
  const edm::InputTag recoJetsSrc     ;
  const edm::InputTag redRecoJetsSrc  ;
  const edm::InputTag redGenJetsSrc   ;
  const edm::InputTag redGenJetsPtrSrc;
  const std::string   redGenJetTag    ;


  edm::Handle<pat::JetCollection>                 recoJets     ;
  edm::Handle<reco::PFJetCollection>              redRecoJets  ;
  edm::Handle<reco::GenJetCollection>             redGenJets   ;
  edm::Handle<edm::ValueMap<reco::CandidatePtr> > redGenJetsPtr;
  };

//--------------------------------------------------------------------------------------------------
RedefinedGenJetAssociator::RedefinedGenJetAssociator(const edm::ParameterSet& iConfig)
        : recoJetsSrc          (iConfig.getParameter<edm::InputTag>("recoJetsSrc"))
        , redRecoJetsSrc       (iConfig.getParameter<edm::InputTag>("redRecoJetsSrc"))
        , redGenJetsSrc        (iConfig.getParameter<edm::InputTag>("redGenJetsSrc"))
        , redGenJetsPtrSrc     (iConfig.getParameter<edm::InputTag>("redGenJetsPtrSrc"))
        , redGenJetTag         (iConfig.getParameter<std::string>("redGenJetTag"))
    {}

//--------------------------------------------------------------------------------------------------
void RedefinedGenJetAssociator::load(edm::Event& iEvent, const edm::EventSetup& iSetup){
  iEvent.getByLabel(recoJetsSrc     ,recoJets     );
  iEvent.getByLabel(redRecoJetsSrc  ,redRecoJets  );
  iEvent.getByLabel(redGenJetsSrc   ,redGenJets   );
  iEvent.getByLabel(redGenJetsPtrSrc,redGenJetsPtr);
}

//--------------------------------------------------------------------------------------------------
bool RedefinedGenJetAssociator::filter(edm::Event& iEvent, const edm::EventSetup& iSetup){
  load(iEvent,iSetup);

  const double maxDeltaR = .1;
  const double maxETAQuality = 4.8;
  std::vector<bool> matches(redRecoJets->size(),false);

  for(const auto& j : (*recoJets.product()) ){
    double nearDR = .5;
    int nearInd = PhysicsUtilities::findNearestDR(j,(*redRecoJets.product()),nearDR,maxDeltaR,.1,&matches );

    if(nearInd < 0){
      if(TMath::Abs(j.eta()) > maxETAQuality) continue;
//      for(const auto& j2 : (*recoJets.product()) ){std::cout << j2.pt() <<" "<<j2.eta() <<" "<<j2.phi()<<std::endl;}
//      std::cout <<"  000000   ";
//      for(const auto& j2 : (*redRecoJets.product()) ){std::cout << j2.pt() <<" "<<j2.eta() <<" "<<j2.phi()<<std::endl;}
//      throw cms::Exception("RedefinedGenJetAssociator.filter()", TString::Format("Could not find a match for a jet with PT: %.2f, Unc: %.2f eta: %.2f phi: %.2f",j.pt(),j.pt()*j.jecFactor("Uncorrected"),j.eta(), j.phi()));
//      std::cout << TString::Format("Could not find a match for a jet with PT: %.2f, Unc: %.2f eta: %.2f phi: %.2f",j.pt(),j.pt()*j.jecFactor("Uncorrected"),j.eta(), j.phi()) << std::endl;
      continue;
    }
//    const double nearPT =redRecoJets->at(nearInd).pt();
//    double diff =  nearPT == 0 ? 10 : TMath::Abs( 1 -  j.pt()*j.jecFactor("Uncorrected") / nearPT );
//    if( diff > .15   ){
//      if(TMath::Abs(j.eta()) > maxETAQuality) continue;
////      throw cms::Exception("RedefinedGenJetAssociator.filter()", TString::Format("Could not find a  good match for a jet with PT: %.2f, Unc: %.2f, eta: %.2f phi: %.2f Match: %.2f",j.pt(),j.pt()*j.jecFactor("Uncorrected"),j.eta(), j.phi(),nearPT));
//      std::cout << TString::Format("Could not find a  good match for a jet with PT: %.2f, Unc: %.2f, eta: %.2f phi: %.2f Match: %.2f",j.pt(),j.pt()*j.jecFactor("Uncorrected"),j.eta(), j.phi(),nearPT) << std::endl;
//    }
    matches[nearInd] = true;
    pat::Jet&                   non_const_jet       = const_cast<pat::Jet&>( j );
    non_const_jet.addUserCand(redGenJetTag,(*redGenJetsPtr)[reco::CandidatePtr(redRecoJets, nearInd)]);
  }

  return true;
}





DEFINE_FWK_MODULE(RedefinedGenJetAssociator);
#endif
