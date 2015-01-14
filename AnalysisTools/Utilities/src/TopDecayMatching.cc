#include "AnalysisTools/Utilities/interface/TopDecayMatching.h"
#include "AnalysisTools/Utilities/interface/ParticleUtilities.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"

using namespace std;
using namespace ucsbsusy;
using namespace TopDecayMatching;

//_____________________________________________________________________________
void ColorSinglet::getSingletMotherCompositon( const edm::Handle<reco::GenParticleCollection>& particles, const int idx, vector<int>& partonIDXs, vector<int>& incommingIDxs){
  for(unsigned int iM = 0; iM < particles->at(idx).numberOfMothers(); ++iM){
    const reco::GenParticleRef& m = particles->at(idx).motherRef(iM);
    if(ParticleInfo::isIncoming(m->status()) || ParticleInfo::isDocIntermediate(m->status()) || ParticleInfo::isDocAltered(m->status()) ){
      incommingIDxs.push_back(m.key());
    } else if (ParticleInfo::isDocOutgoing(m->status()) && (!ParticleInfo::isQuarkOrGluon(m->pdgId())) ){ //for photons that have radiation handled by pythia
      incommingIDxs.push_back(m.key());
    } else if(ParticleInfo::isDocOutgoing(m->status())){
      partonIDXs.push_back(m.key());
    } else if(ParticleInfo::isHadronization(m->status())){
      getSingletMotherCompositon(particles,m.key(),partonIDXs,incommingIDxs);
    } else {
      ParticleInfo::printGenInfo(*particles,-1);
      ParticleInfo::printGenParticleInfo(*m,m.key());
      throw cms::Exception("ColorSinglet.getSingletMotherCompositon()","Bad singlet mother!");
    }
  }
}
//_____________________________________________________________________________
void ColorSinglet::addSingletPrt( Partons& partons,Partons& incomingPartons, const edm::Handle<reco::GenParticleCollection>& particles, int index){

  singletParticles.push_back( reco::GenParticleRef(particles,index)  );
  const reco::GenParticle& p = *singletParticles[singletParticles.size() -1  ];

  vector<int> partonIDXs;
  vector<int> incommingIDxs;
  getSingletMotherCompositon(particles,index,partonIDXs,incommingIDxs);

  //add the parton ref if it is a good one...otherwise add a null ref
  if(partonIDXs.size()){
    for(const auto i : partonIDXs){
      int foundIdx = -1;
      for(unsigned int iP = 0; iP < partons.size(); ++iP){
        if(i != int(partons[iP].parton.key())) continue;
        foundIdx = iP;
        break;
      }

      if(foundIdx == -1){
        ParticleInfo::printGenInfo(*particles,-1);
        ParticleInfo::printGenParticleInfo(p,index);
        throw cms::Exception("ColorSinglet.addSingletPrt()","Parton mother was not found in the parton collection!");
      }
      inPartons.push_back(PartonRef(&partons,foundIdx));
      partons[foundIdx].hasSinglet = true;
    }
  } else{
    incomingPartons.emplace_back(particles,index);
    incomingParticles.push_back(PartonRef(&incomingPartons,incomingPartons.size() - 1));
  }
}
//_____________________________________________________________________________
void ColorSinglet::cleanupInPartons(){
  size i1 = 0;
  size i2 = 0;
  while(i1 < inPartons.size()){
    i2 =i1 + 1;
    while(i2 < inPartons.size()){
      if(inPartons[i1] == inPartons[i2]){
        inPartons.erase(inPartons.begin() + i2);
        continue;
      }
      i2++;
    }
    i1++;
  }
}
//_____________________________________________________________________________
void ColorSinglet::addDaughterKeys(reco::GenParticleRef p){
  matchIdxs.push_back(p.key());
  for(unsigned int iD = 0; iD < p->numberOfDaughters(); ++iD){
    addDaughterKeys(p->daughterRef(iD));
  }
}
//_____________________________________________________________________________
void ColorSinglet::fillMatchIdxs(){
  matchIdxs.clear();
  //first, fill all singlet particles
  for(auto p : singletParticles) matchIdxs.push_back(p.key());
  for(auto p : decayProducts) addDaughterKeys(p);

  //now remove duplicates
  std::sort(matchIdxs.begin(), matchIdxs.end());
  auto last = std::unique(matchIdxs.begin(), matchIdxs.end());
  matchIdxs.erase(last, matchIdxs.end());

}
//_____________________________________________________________________________
void TopDecayMatching::getMatchingPartons(const edm::Handle<reco::GenParticleCollection>& particles, TopDecayMatching::Partons& partons){
  partons.clear();

  for (size iPtcl = 0; iPtcl < particles->size(); ++iPtcl) {
    const reco::GenParticle&              particle      = (*particles)[iPtcl];
    const int                             pdgId         = TMath::Abs(particle.pdgId());

    if(!ParticleInfo::isQuarkOrGluon(pdgId) || pdgId == ParticleInfo::p_t ) continue;
    if(!ParticleInfo::isDocOutgoing(particle.status())) continue;
    partons.emplace_back(particles,iPtcl);
  }

  std::sort(partons.begin(), partons.end(), GreaterPartonPT<Parton>());
}
//_____________________________________________________________________________
bool TopDecayMatching::intermediateMother(const reco::GenParticle * p){
  if(ParticleInfo::isIntermediate(p->status())) return true;
  for(auto m : p->motherRefVector()){
    if(intermediateMother(&(*m))) return true;
  }
  return false;
}
//_____________________________________________________________________________
void TopDecayMatching::getColorSinglets(const edm::Handle<reco::GenParticleCollection>& particles, Partons& partons,  ColorSinglets& colorSinglets,Partons& incomingPartons ){
  colorSinglets.clear();

  //run once down the list to see if there are any
  for(unsigned int iP = 0; iP < particles->size(); ++iP){
    const reco::GenParticle& p = particles->at(iP);
    const int status = p.status();

    //look only at color singlet particles
    if(status != 71 && status != 72) continue;

    //if any of the daughters are 71 or 72 skip
    bool singletDau = false;
    for(const auto d : p.daughterRefVector())
      if(d->status() == 71 || d->status() == 72) singletDau = true;
    if(singletDau) continue;

    //if any of the mothers are intermediate skip
    if(intermediateMother(&p)) continue;

    //if it comes from a lepton skip it
    if(outgoingLeptonDaughter(&p)) continue;

    //Make a temporary singlet
    ColorSinglet tempSinglet(partons,incomingPartons,particles,iP);



    //if it is already in the list add it to that one, otherwise keep it as a new one
    bool found = false;
    for(unsigned int iS = 0; iS < colorSinglets.size(); ++iS){
      if(tempSinglet.equivDecays(colorSinglets[iS])){
        colorSinglets[iS].addSingletPrt(partons,incomingPartons,particles,iP);
        found = true;
        break;
      }
    }
    if(!found) colorSinglets.push_back(tempSinglet);
  }

  //remove duplicate inputs
  for(auto& s : colorSinglets) s.cleanupInPartons();
  //add matching ids
  for(auto& s : colorSinglets) s.fillMatchIdxs();

}
//_____________________________________________________________________________
bool TopDecayMatching::outgoingLeptonDaughter(const reco::GenParticle * p){
  if(ParticleInfo::isDocOutgoing(p->status()) && ParticleInfo::isLeptonOrNeutrino(p->pdgId())  ) return true;
  for(size iM = 0; iM < p->numberOfMothers(); ++iM){
    if(outgoingLeptonDaughter(&(*p->motherRef(iM)))) return true;
  }
  return false;
}
//_____________________________________________________________________________
void TopDecayMatching::associateFinalParticles(const edm::Handle<reco::GenParticleCollection>& particles, const edm::Handle<pat::PackedGenParticleCollection>& finalParticles,
    const ColorSinglets& colorSinglets, std::vector<std::vector<pat::PackedGenParticleRef>>& assocToColorSinglets, std::vector<pat::PackedGenParticleRef>& nonAssoc){

  assocToColorSinglets.clear();
  assocToColorSinglets.resize(colorSinglets.size());
  nonAssoc.clear();

  //first map every orig gen particle to a singlet
  vector<int> assocList(particles->size(), -1);
  for(size iC = 0; iC < colorSinglets.size(); ++iC){
    for(const auto iI : colorSinglets[iC].matchIdxs ){
      if(assocList[iI] >= 0){
        ParticleInfo::printGenInfo(*particles,-1);
        ParticleInfo::printGenParticleInfo(particles->at(iI),iI);
        throw cms::Exception("TopDecayMatching.associateFinalParticles()","Particle was already associated!");
      }
      assocList[iI] = iC;
    }
  }
  //Now fill the vector of refs
  for(unsigned int iP = 0; iP < finalParticles->size(); ++iP){
    const auto& p = finalParticles->at(iP);
    int motherID = p.numberOfMothers() > 0 ? p.motherRef().key() : -1;
    if(motherID >= 0 && assocList[motherID] >= 0){
      assocToColorSinglets[assocList[motherID]].emplace_back(finalParticles,iP);
    }else if(motherID >= 0 && outgoingLeptonDaughter(&particles->at(motherID))) {
      continue;
    } else{
      nonAssoc.emplace_back(finalParticles,iP);
    }
  }
}
//_____________________________________________________________________________
void TopDecayMatching::matchDecayProducts(std::vector<Parton*>& matchingPartons, std::vector<pat::PackedGenParticleRef>& finalParticles, const double maxDR){
  size iF = 0;
  while(iF < finalParticles.size()){
    double nearest = 9999;
    int match = PhysicsUtilities::findNearestDRDeref(*finalParticles[iF],matchingPartons,nearest,maxDR);
    if(match >= 0){
      matchingPartons[match]->tempFinal.emplace_back(nearest,finalParticles[iF].key());
      std::swap(finalParticles[iF], finalParticles[ finalParticles.size() -1   ]   );
      finalParticles.pop_back();
      continue;
    }
    iF++;
  }
}
//_____________________________________________________________________________
void TopDecayMatching::addParticlesToParton(Parton& parton, double maxRelE,
    std::vector<pat::PackedGenParticleRef>& particleDump, const edm::Handle<pat::PackedGenParticleCollection>& finalParticles  ){
  auto*  tempFinal = &parton.tempFinal;
  //sort by lesser dr if we are going to throw some away
  if(maxRelE > 0) std::sort(tempFinal->begin(), tempFinal->end(),PhysicsUtilities::lesserAbsFirst<double,int>());

  for(auto& iF : *tempFinal){
    pat::PackedGenParticleRef p(finalParticles,iF.second);
    if(maxRelE > 0 && (parton.sumFinal + p->p4()).energy() > parton.parton->energy()*maxRelE ){
      particleDump.push_back(p);
    } else{
      parton.addFinalParticle(p);
    }
  }
  tempFinal->clear();
}
//_____________________________________________________________________________
void TopDecayMatching::associateToNearestPartons(Partons& partons, const ColorSinglets& colorSinglets,const double maxDR,const edm::Handle<pat::PackedGenParticleCollection>& finalParticles,
    std::vector<std::vector<pat::PackedGenParticleRef>>& assocToColorSinglets, std::vector<pat::PackedGenParticleRef>& nonAssoc){

  //first fo over the color singlets
  for(size iS = 0; iS < colorSinglets.size(); ++iS){
    vector<Parton*> singletPartons;
    singletPartons.reserve(colorSinglets[iS].inPartons.size());
    for(const auto& p : colorSinglets[iS].inPartons) singletPartons.push_back(&partons[p.key()]);
    matchDecayProducts(singletPartons,assocToColorSinglets[iS],maxDR);

    //for the first pass add all particles
    for(auto* p : singletPartons) addParticlesToParton(*p,-1,assocToColorSinglets[iS],finalParticles);
  }

  //now for all particles without singlets
  for(auto& p : partons){
    vector<Parton*> nonSingletPartons;
    if(!p.hasSinglet) nonSingletPartons.push_back(&p);
    matchDecayProducts(nonSingletPartons,nonAssoc,maxDR);
    //for the first pass add all particles
    for(auto* p : nonSingletPartons) addParticlesToParton(*p,-1,nonAssoc,finalParticles);
  }

}
//_____________________________________________________________________________
void TopDecayMatching::associateSingletsLoosely(Partons& partons,Partons& incomingPartons,  const ColorSinglets& colorSinglets,const double maxDR,const double maxRelE,
    const edm::Handle<pat::PackedGenParticleCollection>& finalParticles,std::vector<std::vector<pat::PackedGenParticleRef>>& assocToColorSinglets){
  for(size iS = 0; iS < colorSinglets.size(); ++iS){
    vector<Parton*> allSingletParticles;
    allSingletParticles.reserve(colorSinglets[iS].inPartons.size() + colorSinglets[iS].incomingParticles.size() );
    for(const auto& p : colorSinglets[iS].inPartons) allSingletParticles.push_back(&partons[p.key()]);
    for(const auto& p : colorSinglets[iS].incomingParticles) allSingletParticles.push_back(&incomingPartons[p.key()]);
    matchDecayProducts(allSingletParticles,assocToColorSinglets[iS],maxDR);
    //add particles only if they are within the pT requirement
    for(auto* p : allSingletParticles) addParticlesToParton(*p,maxRelE,assocToColorSinglets[iS],finalParticles);
  }
}
//_____________________________________________________________________________
void TopDecayMatching::associateRemaining(Partons& partons,Partons& incomingPartons, const ColorSinglets& colorSinglets, const double maxDR,const double maxRelE,
    const edm::Handle<pat::PackedGenParticleCollection>& finalParticles,std::vector<std::vector<pat::PackedGenParticleRef>>& assocToColorSinglets,
    std::vector<pat::PackedGenParticleRef>& nonAssoc
    )
{
  //Start by taking the rest of the assoc particles and adding them to the non assoc ones
  for(size iS = 0; iS < assocToColorSinglets.size(); ++iS){
    for(size iF = 0; iF < assocToColorSinglets[iS].size(); ++iF){
      nonAssoc.push_back(assocToColorSinglets[iS][iF]);
    }
    assocToColorSinglets[iS].clear();
  }

  //Now match everything!
  vector<Parton*> allParticles;
  allParticles.reserve(partons.size() + incomingPartons.size() );
  for(auto& p : partons) allParticles.push_back(&p);
  for(auto& p : incomingPartons) allParticles.push_back(&p);
  matchDecayProducts(allParticles,nonAssoc,maxDR);
  //add particles only if they are within the pT requirement
  for(auto* p : allParticles) addParticlesToParton(*p,maxRelE,nonAssoc,finalParticles);

}
//_____________________________________________________________________________
void TopDecayMatching::fillTopDecays(const edm::Handle<reco::GenParticleCollection>& particles,const Partons& partons, TopDecays& topDecays){
  topDecays.clear();

  for(size iP = 0; iP < particles->size(); ++iP){
    const auto& p = particles->at(iP);
    if(TMath::Abs(p.pdgId()) != ParticleInfo::p_t )continue;
    if(!ParticleUtilities::isLastInChain(&p)) continue;
    topDecays.emplace_back(particles,iP);
  }

  for(auto& t : topDecays){

    //get direct decay products
    if(t.top->numberOfDaughters() != 2){
      ParticleInfo::printGenInfo(*particles,-1);
      ParticleInfo::printGenParticleInfo(*t.top,t.top.key());
      throw cms::Exception("TopDecayMatching.fillTopDecays()","Wrong top daughter number!");
    }
    for(size iD = 0; iD < t.top->numberOfDaughters(); ++iD){
      auto d = t.top->daughterRef(iD);
      int pdgID = TMath::Abs(d->pdgId());
      if(pdgID == ParticleInfo::p_b){
        t.b = reco::GenParticleRef(particles,d.key());
      } else       if(pdgID == ParticleInfo::p_Wplus){
        t.W = ParticleUtilities::getFinal(d,particles);
      } else {
        ParticleInfo::printGenInfo(*particles,-1);
        ParticleInfo::printGenParticleInfo(*d,d.key());
        throw cms::Exception("TopDecayMatching.fillTopDecays()","Bad top daughter!");
      }
    }

    //fill w decays
    if(t.W->numberOfDaughters() != 2){
      ParticleInfo::printGenInfo(*particles,-1);
      ParticleInfo::printGenParticleInfo(*t.W,t.W.key());
      throw cms::Exception("TopDecayMatching.fillTopDecays()","Wrong W daughter number!");
    }
    t.W_dau1 = t.W->daughterRef(0);
    t.W_dau2 = t.W->daughterRef(1);

    //fill PartonRefs
    for(size i = 0; i < partons.size(); ++i)
      if(partons[i].parton.key() == t.b.key()){
        t.b_decay = PartonRef(&partons,i);
      }
    if(t.b_decay.isNull()){
      ParticleInfo::printGenInfo(*particles,-1);
      ParticleInfo::printGenParticleInfo(*t.b,t.b.key());
      throw cms::Exception("TopDecayMatching.fillTopDecays()","B has no decay!");
    }

    if(ParticleInfo::isLeptonOrNeutrino(t.W_dau1->pdgId())){
      t.isLeptonic = true;
    } else {
      for(size i = 0; i < partons.size(); ++i){
        if(partons[i].parton.key() == t.W_dau1.key())
          t.w1_decay = PartonRef(&partons,i);
        if(partons[i].parton.key() == t.W_dau2.key())
          t.w2_decay = PartonRef(&partons,i);
      }
      if(t.w1_decay.isNull() || t.w2_decay.isNull()){
        ParticleInfo::printGenInfo(*particles,-1);
        if( t.w1_decay.isNull())ParticleInfo::printGenParticleInfo(*t.W_dau1,t.W_dau1.key());
        if( t.w2_decay.isNull())ParticleInfo::printGenParticleInfo(*t.W_dau2,t.W_dau2.key());
        throw cms::Exception("TopDecayMatching.fillTopDecays()","W  has no decay!");
      }
    }

  }
}
//_____________________________________________________________________________
void TopDecayMatching::labelPartonOwnership(const Partons& partons,const edm::Handle<pat::PackedGenParticleCollection>& finalParticles, std::vector<int>& assocMap){
  assocMap.resize(finalParticles->size());
  for(auto& i : assocMap) i = -1;
  for(size iP = 0; iP < partons.size(); ++iP){
    for(auto d : partons[iP].finalPrts){
      assocMap[d.key()] = iP;
    }
  }
}
//_____________________________________________________________________________
void TopDecayMatching::associatePartonsToJets(const Partons& partons,const reco::GenJetCollection& jets,const std::vector<int>& prtPartonAssoc){

  //first clear out the old stuff
  for(auto& p : partons){
    p.jetAssoc.clear();
  }
  for(size iJ = 0; iJ < jets.size(); ++iJ){
    const auto& j = jets[iJ];
    for (size iDau = 0; iDau < j.numberOfDaughters(); ++iDau) {
      if(j.daughterPtr(iDau).isNull()) continue;
      int pIdx = prtPartonAssoc[j.daughterPtr(iDau).key()];
      if(pIdx < 0) continue;
      partons[pIdx].jetAssoc[iJ]     += j.daughterPtr(iDau)->p4();
    }
  }
}
