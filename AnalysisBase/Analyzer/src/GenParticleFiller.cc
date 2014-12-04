//--------------------------------------------------------------------------------------------------
// 
// GenParticleFiller
// 
// Filler for MC truth information.
// 
// GenParticleFiller.cc created on Mon Nov 10 20:01:46 CET 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisBase/Analyzer/interface/GenParticleFiller.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"

using namespace ucsbsusy;

const int GenParticleFiller::defaultOptions = GenParticleFiller::NULLOPT;

//--------------------------------------------------------------------------------------------------
GenParticleFiller::GenParticleFiller(const int options, const string branchName, const edm::InputTag genParticleTag, const edm::InputTag packedGenParticleTag) :
  BaseFiller(options, branchName),
  genParticleTag_(genParticleTag),
  packedGenParticleTag_(packedGenParticleTag)
{
  ipt          = data.addMulti<float >(branchName_,"pt"       ,0);
  ieta         = data.addMulti<float >(branchName_,"eta"      ,0);
  iphi         = data.addMulti<float >(branchName_,"phi"      ,0);
  imass        = data.addMulti<float >(branchName_,"mass"     ,0);
  istatus      = data.addMulti<size8 >(branchName_,"status"   ,0);
  ipdgid       = data.addMulti<int   >(branchName_,"pdgId"    ,0);
  inmoms       = data.addMulti<stor  >(branchName_,"nMoms"    ,0);
  ifirstmom    = data.addMulti<stor  >(branchName_,"firstMom" ,0);
  indaus       = data.addMulti<stor  >(branchName_,"nDaus"    ,0);
  ifirstdau    = data.addMulti<stor  >(branchName_,"firstDau" ,0);
  iassoc       = data.addMulti<stor  >(branchName_,"assocList",0);
}

//--------------------------------------------------------------------------------------------------
void GenParticleFiller::load(const edm::Event& iEvent)
{
  reset();

  FileUtilities::enforceGet(iEvent,genParticleTag_,genParticles_,true);
  FileUtilities::enforceGet(iEvent,packedGenParticleTag_,packedGenParticles_,options_ & LOADPACKED);

  isLoaded_ = true;
}

//--------------------------------------------------------------------------------------------------
void GenParticleFiller::fill()
{
//  ParticleInfo::printGenInfo(*genParticles_,-1);
  CandMap candMap;
  vector<const reco::GenParticle*> storedParticles;
  if(options_ & SAVEALL)
    for(unsigned int iP = 0; iP < genParticles_->size(); ++iP){
      candMap[iP] =  storedParticles.size();
      storedParticles.push_back(&genParticles_->at(iP));
    }
  else
    addHardInteraction(storedParticles,candMap);

//  cout <<"----"<<endl;
//  cout << storedParticles.size();
//  cout <<"----"<<endl;
//  for(unsigned int iP = 0; iP < genParticles_->size();++iP){
//    CandMap::const_iterator pIt = candMap.find(iP);
//    if(pIt == candMap.end()) continue;
//    cout <<storedParticles[pIt->second]->status() <<" "<< iP <<" "<<storedParticles[pIt->second]->pdgId() <<" "<<storedParticles[pIt->second]->pt() <<endl;
//  }

  storVec assocList;
  storVec nMoms;
  storVec firstMoms;
  storVec nDaus;
  storVec firstDaus;
  fillAssoc(storedParticles,candMap,false,false, assocList,nMoms,firstMoms,nDaus,firstDaus);
//  cout <<"----"<<endl;

//  for(unsigned int iP = 0; iP < assocList.size(); ++iP)
//    cout << assocList[iP]<<endl;
//  cout <<"----"<<endl;

//  for(unsigned int iP = 0; iP < storedParticles.size(); ++iP){
//    cout <<storedParticles[iP]->status() << " "<<ParticleInfo::titleFor(storedParticles[iP]->pdgId()) <<" "<<storedParticles[iP]->pt()<<" ";
//
//    for(unsigned int iM = 0; iM < nMoms[iP]; ++iM){
//      cout << assocList[firstMoms[iP] + iM ] <<",";
//    }
//    cout <<"...";
//    for(unsigned int iM = 0; iM < nDaus[iP]; ++iM){
//      cout << assocList[firstDaus[iP] + iM ] <<",";
//    }
//    cout << endl;
//
//  }


  for(unsigned int iC = 0; iC < storedParticles.size(); ++iC){
    data.fillMulti<float>(ipt      ,storedParticles[iC]->pt());
    data.fillMulti<float>(ieta     ,storedParticles[iC]->eta());
    data.fillMulti<float>(iphi     ,storedParticles[iC]->phi());
    data.fillMulti<float>(imass    ,storedParticles[iC]->mass());
    data.fillMulti<size8>(istatus  ,convertTo<size8>(storedParticles[iC]->status(),"GenParticleFiller::fill() -> status"));
    data.fillMulti<int  >(ipdgid   ,convertTo<int16>(storedParticles[iC]->pdgId() ,"GenParticleFiller::fill() -> pdgid"));
    data.fillMulti<stor >(inmoms   ,convertTo<stor >(nMoms[iC]                    ,"GenParticleFiller::fill() -> nMoms"));
    data.fillMulti<stor >(ifirstmom,convertTo<stor >(firstMoms[iC]                ,"GenParticleFiller::fill() -> firstMom"));
    data.fillMulti<stor >(indaus   ,convertTo<stor >(nDaus[iC]                    ,"GenParticleFiller::fill() -> nDaus"));
    data.fillMulti<stor >(ifirstdau,convertTo<stor >(firstDaus[iC]                ,"GenParticleFiller::fill() -> firstDau"));
  }

  for(const auto& a : assocList) data.fillMulti<stor >(iassoc,convertTo<stor >(a,"GenParticleFiller::fill() -> assocList"));

  isFilled_ = true;

}
//--------------------------------------------------------------------------------------------------
void GenParticleFiller::addHardInteraction(vector<const reco::GenParticle*>& outParticles,CandMap& candMap) const {
  for(unsigned int iP = 0; iP < genParticles_->size(); ++iP){


    const reco::GenParticle * part = &genParticles_->at(iP);
    const int status = part->status();
    const int pdgId  = part->pdgId();

    //Add all status 23 particles or final version of all status 22 particles
    if(!ParticleInfo::isDocOutgoing(status)  && !ParticleInfo::isDocIntermediate(status) ) continue;
    reco::GenParticleRef genRef = reco::GenParticleRef(genParticles_, iP);

    //If this is an intermediate or lepton...we want to get the final version
    if(ParticleInfo::isDocIntermediate(status)  || ParticleInfo::isLeptonOrNeutrino(pdgId))
      genRef = ParticleUtilities::getFinal(genRef,genParticles_);

    //never re-add a particle!
    CandMap::const_iterator pIt = candMap.find(genRef.key());
    if(pIt != candMap.end()) continue;

    //Add the particle
    candMap[genRef.key()] =  outParticles.size();
    outParticles.push_back(&(*genRef));

    //if the added particle was a tau add all daughters
    if(TMath::Abs(pdgId) == ParticleInfo::p_tauminus){
      for(unsigned int iD = 0; iD <genRef->numberOfDaughters(); ++iD){
        reco::GenParticleRef genRef2 = genRef->daughterRef(iD);

        //Always get final leptons
        if(ParticleInfo::isLeptonOrNeutrino(genRef2->pdgId()))
          genRef2 = ParticleUtilities::getFinal(genRef2,genParticles_);

        //never re-add a particle
        CandMap::const_iterator pIt2 = candMap.find(genRef2.key());
        if(pIt2 != candMap.end()) continue;

        candMap[genRef2.key()] =  outParticles.size();
        outParticles.push_back(&(*genRef2));
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------
void GenParticleFiller::fillMomVec(const reco::GenParticle * c, const CandMap& candMap,const bool requireMoms, storVec& moms) const {
  for(unsigned int iM = 0; iM < c->numberOfMothers(); ++iM){
    auto momRef = c->motherRef(iM);
    CandMap::const_iterator momIt = candMap.find(momRef.key());
    if(momIt == candMap.end()){
      if(momRef->numberOfMothers())
        fillMomVec(&(*momRef),candMap,requireMoms,moms);
      else if (requireMoms){
        ParticleInfo::printGenInfo((*genParticles_),-1);
        throw cms::Exception( "GenParticleFiller::fillMomVec()",
            TString::Format ("Particle %u not stored and has no mothers!",momRef.key()));
      }
    }
    else moms.push_back(convertTo<stor>(momIt->second,"fillMomVec"));
  }
}
//--------------------------------------------------------------------------------------------------
void GenParticleFiller::fillDauVec(const reco::GenParticle * c, const CandMap& candMap,const bool requireDaus, storVec& daus) const {
  for(unsigned int iD = 0; iD < c->numberOfDaughters(); ++iD){
    auto dauRef = c->daughterRef(iD);
    CandMap::const_iterator dauIt = candMap.find(dauRef.key());
    if(dauIt == candMap.end()){
      if(dauRef->numberOfDaughters())
        fillDauVec(&(*dauRef),candMap,requireDaus,daus);
      else if (requireDaus){
        ParticleInfo::printGenInfo((*genParticles_),-1);
        throw cms::Exception( "GenParticleFiller::fillDauVec()",
            TString::Format ("Particle %u not stored and has no daughters!",dauRef.key()));
      }
    }
    else daus.push_back(convertTo<stor>(dauIt->second,"fillDauVec"));
  }
}
//--------------------------------------------------------------------------------------------------
void GenParticleFiller::fillAssoc(const std::vector<const reco::GenParticle*>& cands,const CandMap& candMap, const bool requireMoms, const bool requireDaus,
                                  storVec& assocList, storVec& nMoms,storVec& firstMom,storVec& nDaus,storVec& firstDau ) const {

  //check to see if the storage element can hold the number of gen particles
  convertTo<stor>(cands.size(),"Stored candidates size");

  for(const auto* c : cands ){
    storVec moms;
    fillMomVec(c,candMap,requireMoms,moms);
    nMoms.push_back(convertTo<stor>(moms.size(),"fill nMoms"));
    firstMom.push_back(convertTo<stor>(moms.size() == 0 ? 0 : assocList.size(),"Fill fistMom"));
    assocList.insert(assocList.end(),moms.begin(),moms.end());

    storVec daus;
    fillDauVec(c,candMap,requireDaus,daus);
    nDaus.push_back(convertTo<stor>(daus.size(),"fill nDaus"));
    firstDau.push_back(convertTo<stor>(daus.size() == 0 ? 0 : assocList.size(),"fill fistDau"));
    assocList.insert(assocList.end(),daus.begin(),daus.end());
  }
}

