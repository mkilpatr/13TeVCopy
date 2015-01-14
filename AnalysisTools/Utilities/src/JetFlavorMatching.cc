/*
 * JetFlavorMatching.cc
 *
 *  Created on: Aug 27, 2014
 *      Author: nmccoll
 */

#include "AnalysisTools/Utilities/interface/JetFlavorMatching.h"
#include "AnalysisTools/Utilities/interface/ParticleUtilities.h"

using namespace std;
using namespace ucsbsusy;
using namespace JetFlavorMatching;

//_____________________________________________________________________________
void JetFlavorMatching::getHadronDecays(const edm::Handle<vector<reco::GenParticle> >& particles, std::vector<ParticleDecay>& bDecays, std::vector<ParticleDecay>& cDecays )
{
  bDecays.clear();
  cDecays.clear();

  for (size iPtcl = 0; iPtcl < particles->size(); ++iPtcl) {
    const reco::GenParticle&              particle      = (*particles)[iPtcl];
    const int                             pdgId         = TMath::Abs(particle.pdgId());

    ParticleInfo::HadronType hadronType = ParticleInfo::typeOfHadron(pdgId);

    //bHadrons first
    if (ParticleInfo::isBHadron(hadronType)){
      //Should have no b-hadron daughters
      int numHDaus = 0;
      for(unsigned int iDau = 0; iDau < particle.numberOfDaughters(); ++iDau){
        if (ParticleInfo::isBHadron(ParticleInfo::typeOfHadron(particle.daughter(iDau)->pdgId()))) numHDaus++;
      }
      if(numHDaus) continue;
      bDecays     .emplace_back(particles, iPtcl);
    }
    //then c hadrons
    else if (ParticleInfo::isCHadron(hadronType)){
      //Should have no c-hadron mothers
      int numHDaus = 0;
      for(unsigned int iDau = 0; iDau < particle.numberOfDaughters(); ++iDau){
        if (ParticleInfo::isCHadron(ParticleInfo::typeOfHadron(particle.daughter(iDau)->pdgId()))) numHDaus++;
      }
      if(numHDaus) continue;
      cDecays     .emplace_back(particles, iPtcl);
    }
  }

  std::sort(bDecays.begin(), bDecays.end(), GreaterParticlePT<ParticleDecay>());
  std::sort(cDecays.begin(), cDecays.end(), GreaterParticlePT<ParticleDecay>());

}
void JetFlavorMatching::getPartons(const edm::Handle<std::vector<reco::GenParticle> >& particles, std::vector<ParticleDecay>& partonDecays, const bool skipHeavyFlavor){
  partonDecays.clear();

  for (size iPtcl = 0; iPtcl < particles->size(); ++iPtcl) {
    const reco::GenParticle&              particle      = (*particles)[iPtcl];
    const int                             pdgId         = TMath::Abs(particle.pdgId());

    if(!isMatchingParton(particle)) continue;
    if(skipHeavyFlavor && (pdgId == ParticleInfo::p_b || pdgId == ParticleInfo::p_c) ) continue;
    partonDecays.emplace_back(particles,iPtcl);
  }

  std::sort(partonDecays.begin(), partonDecays.end(), GreaterParticlePT<ParticleDecay>());
}
//_____________________________________________________________________________
void JetFlavorMatching::tagDecays(const int tag, reco::GenParticleRef  particle, std::vector<int>& bHadronMatches){

  //assume that if it was already desiginated that particle has priority
  if(bHadronMatches[particle.key()] >= 0) return;
//Now we are being lazy..kind of have to due to ambiguities in decay path
//  if(bHadronMatches[particle.key()] >= 0 && bHadronMatches[particle.key()] != tag ){
//    throw cms::Exception("JetFlavorMatching.tagDecays()",
//        TString::Format("This particle (%d) has already been associated to %i but you want to associate it to %i!"
//            , particle.key(),tag, bHadronMatches[particle.key()]).Data());
//  }
  bHadronMatches[particle.key()] = tag;
  for(unsigned int iD = 0; iD < particle->numberOfDaughters(); ++iD){
    tagDecays(tag,particle->daughterRef(iD),bHadronMatches);
  }
}
//_____________________________________________________________________________
void JetFlavorMatching::associateDecayProducts(const edm::Handle<std::vector<reco::GenParticle> > & particles, const edm::Handle<pat::PackedGenParticleCollection>& finalParticles,
    std::vector<ParticleDecay>* bDecays,std::vector<ParticleDecay>* cDecays, std::vector<ParticleDecay>* partonDecays, std::vector<int>* partonParticleAssoc){

  //associate each particle in the particles collection to a decay
  vector<int> bDecayMatches(particles->size(),-1);
  vector<int> cDecayMatches(particles->size(),-1);
  vector<int> partonDecayMatches(particles->size(),-1);

  if(bDecays)
    for(unsigned int iP = 0; iP < bDecays->size(); ++iP){
      tagDecays(iP,bDecays->at(iP).particle,bDecayMatches);
    }

  if(cDecays)
    for(unsigned int iP = 0; iP < cDecays->size(); ++iP){
      tagDecays(iP,cDecays->at(iP).particle,cDecayMatches);
    }

  if(partonDecays)
    for(unsigned int iP = 0; iP < partonDecays->size(); ++iP){
      tagDecays(iP,partonDecays->at(iP).particle,partonDecayMatches);
    }



  for(unsigned int iP = 0; iP < finalParticles->size(); ++iP){
    const auto * p = &finalParticles->at(iP);

    if(bDecayMatches[p->motherRef().key()] >= 0)
      addDecayProduct(bDecays->at(bDecayMatches[p->motherRef().key()]), *p, iP);
    else     if(cDecayMatches[p->motherRef().key()] >= 0)
      addDecayProduct(cDecays->at(cDecayMatches[p->motherRef().key()]), *p, iP);
    else     if(partonDecayMatches[p->motherRef().key()] >= 0){
      addDecayProduct(partonDecays->at(partonDecayMatches[p->motherRef().key()]), *p, iP);
      if(partonParticleAssoc) (*partonParticleAssoc)[iP] = partonDecayMatches[p->motherRef().key()];
    }
  }

//  for(unsigned int iH = 0; iH < bDecays.size(); ++iH){
//    cout << "---------------"<<endl;
//    const JetFlavorMatching::HadronDecay& hadDecay = bDecays[iH];
//    ParticleInfo::printGenParticleInfo(*hadDecay.hadron,hadDecay.hadron.key());
//    cout << ">>"<<endl;
//    for(unsigned int iD = 0; iD < hadDecay.decayInts.size(); ++iD){
//      ParticleInfo::printPackedGenParticleInfo(&finalParticles->at(hadDecay.decayInts[iD]),hadDecay.decayInts[iD]);
//    }
//  }

  //Sanity checks
//  for (size iHadron = 0; iHadron < bDecays.size(); ++iHadron) {
//    HadronDecay&                          decay         = bDecays[iHadron];
//    if (decay.decayInts.empty())
//      throw cms::Exception("JetFlavorMatching::associateDecayProducts()", TString::Format("Encountered an undecayed hadron (pdgId = %d, pT = %.4g, eta = %.4g).", decay.hadron->pdgId(), decay.hadron->pt(), decay.hadron->eta()).Data());
//    if (decay.sumVisible.energy() > 0.5 + decay.hadron->energy()) {
//      cerr << endl;
//      cerr << "!!!!!!  " << TString::Format("pt = %8.4g, eta = %8.3g, phi = %8.3g, mass = %8.4g, E = %8.4g", decay.hadron->pt(), decay.hadron->eta(), decay.hadron->phi(), decay.hadron->mass(), decay.hadron->energy()) << endl;
//      cerr << " -vs-   " << TString::Format("pt = %8.4g, eta = %8.3g, phi = %8.3g, mass = %8.4g, E = %8.4g", decay.sumVisible.pt(), decay.sumVisible.eta(), decay.sumVisible.phi(), decay.sumVisible.mass(), decay.sumVisible.energy()) << endl;
//      cerr << endl;
//      throw cms::Exception("JetFlavorMatching::associateDecayProducts()", TString::Format("Visible energy (%.8g) exceeds total energy (%.8g) for hadron (pdgId = %d).", decay.sumVisible.energy(), decay.hadron->energy(), decay.hadron->pdgId()).Data());
//    }
//  } // end loop over hadrons


  //sort by pt
}


