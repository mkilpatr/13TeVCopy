

#include "AnalysisTools/Utilities/interface/TopJetMatching.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"


using namespace std;
using namespace ucsbsusy;
using namespace TopJetMatching;

//We only do > tests so conversion is to the percent floor
conType TopJetMatching::toContainmentType(const float inCon ){ return ucsbsusy::convertTo<conType>( std::floor(inCon*100) ,"TopJetMatching::toContainmentType");}
float TopJetMatching::fromContainmentType(const conType inCon){return float(inCon)/100; }

//--------------------------------------------------------------------------------------------------
PartonDiagnosis Parton::getDiagnosis(const Parton& parton,const std::vector<Jet*>& jets) {
  if( parton.parton->pt() < minPartonPT)             return SOFT_PARTON;
  if( TMath::Abs(parton.parton->eta()) > maxPartonETA)return HIGH_ETA_PARTON;
  if(parton.hadE < minHadronRelE* parton.parton->energy() ) return DISPERSED_PARTON;

  int numSigJets = 0;
  bool numMainJets = 0;
  for(const auto& c : parton.filteredContaiment) {if(c.first > minPartontRelE)numMainJets++;  if(c.first > extraJetsPartonRelE)numSigJets++;   }

  if(!numMainJets)                                   return NO_JET;
  if(numSigJets > 1)                                return SPLIT_JETS;

 if(parton.filteredContaiment[0].first*parton.hadE < minJetRelE*jets[parton.filteredContaiment[0].second ]->energy())
   return DIRTY_JET;



 return RESOLVED_PARTON;
}
//--------------------------------------------------------------------------------------------------
void Parton::finalize(const std::vector<Jet*>& jets) {
  sort(containment.begin(),containment.end(),PhysicsUtilities::greaterFirst<float,int>());
  filteredContaiment.clear();
  for(const auto& c : containment){
	  for(unsigned int iJ =0; iJ < jets.size(); ++iJ)
		  if(jets[iJ]->index() == c.second){
			  filteredContaiment.emplace_back(c.first,iJ);
		  }

  }
  if(filteredContaiment.size()) matchedJet = jets[filteredContaiment[0].second ];

  diag = getDiagnosis(*this,jets);
}
//--------------------------------------------------------------------------------------------------
TopDecay::TopDecay(const Particle * inTop, const std::vector<Parton>& allPartons){
  if(TMath::Abs(inTop->pdgId()) != ParticleInfo::p_t)
    throw std::invalid_argument("TopDecay::TopDecay(): Initialization particle must be a top!");

  top = inTop;
  W = 0;
  for(unsigned int iD = 0; iD< inTop->numberOfDaughters(); ++iD){
    auto dau = inTop->daughterRef(iD);
    const int pdgId = TMath::Abs(dau->pdgId());
    if( pdgId == ParticleInfo::p_Wplus){
      W = inTop->daughter(iD);
    } else if(pdgId == ParticleInfo::p_b || pdgId == ParticleInfo::p_s || pdgId == ParticleInfo::p_d){
      for(const auto&  p: allPartons){
       if(p.genIdx != dau.key() ) continue;
       hadronicPartons.push_back(&p);
       b = &p;
      }
    }
  }

  if(hadronicPartons.size() != 1 || W == 0){
   ParticleInfo::printGenParticleInfo(inTop,-1);
    throw std::invalid_argument("TopDecay::TopDecay(): Top has abnormal decay!");
  }

  //Now do the W decay
  if(W->numberOfDaughters() != 2){
    ParticleInfo::printGenParticleInfo(W,-1);
    throw std::invalid_argument("TopDecay::TopDecay(): W has abnormal decay!");
   }

  CandidateRef<Particle> dau1Ref,dau2Ref;
  if(W->daughter(0)->pt() > W->daughter(1)->pt()){
    dau1Ref = W->daughterRef(0);
    dau2Ref = W->daughterRef(1);
  } else {
    dau1Ref = W->daughterRef(1);
    dau2Ref = W->daughterRef(0);
  }

  //special case for leptons
  if(ParticleInfo::isLeptonOrNeutrino(dau1Ref->pdgId())){
    isLeptonic = true;
    leptonPartons.emplace_back( &*dau1Ref,  dau1Ref.key(), 0);
    leptonPartons.emplace_back( &*dau2Ref,  dau2Ref.key(), 0);
    W_dau1 = &leptonPartons[0];
    W_dau2 = &leptonPartons[1];
  } else {
    isLeptonic = false;

    W_dau1 = 0;
    W_dau2 = 0;

    for(const auto&  p: allPartons){
     if(p.genIdx == dau1Ref.key() ) W_dau1 = &p;
     if(p.genIdx == dau2Ref.key() ) W_dau2 = &p;
    }

    if(W_dau1 == 0 || W_dau2 == 0){
     ParticleInfo::printGenParticleInfo(*dau1Ref,dau1Ref.key());
     ParticleInfo::printGenParticleInfo(*dau2Ref,dau2Ref.key());
      throw std::invalid_argument("TopDecay::TopDecay(): W decay was not found in parton collection!");
    }
    hadronicPartons.push_back(W_dau1);
    hadronicPartons.push_back(W_dau2);
  }

  diag = getDiagnosis(*this);
}
//--------------------------------------------------------------------------------------------------
TopDiagnosis TopDecay::getDiagnosis(const TopDecay& parton){
  int nBadPartons = 0;
  int nlostJets = 0;
  int nsplitJets = 0;
  int ndirtyJets = 0;

  for(unsigned int iP1 = 0 ; iP1 < parton.hadronicPartons.size(); ++iP1){
    const auto* p1 = parton.hadronicPartons[iP1];
    if(p1->diag <= DISPERSED_PARTON ) nBadPartons++;
    else if(p1->diag == NO_JET)       nlostJets++;
    else if(p1->diag == SPLIT_JETS)   nsplitJets++;
    else if(p1->diag == DIRTY_JET)    ndirtyJets++;
  }

  if(nBadPartons) return BAD_PARTON;
  if(nlostJets)   return LOST_JET;
  if(nsplitJets)  return SPLIT_PARTONS;
  if(ndirtyJets)  return CONTAMINATION;
  return RESOLVED_TOP;
}
//--------------------------------------------------------------------------------------------------
void TopDecayEvent::initialize(const std::vector<ucsbsusy::size16 >* genAssocPrtIndex, const std::vector<ucsbsusy::size16 >* genAssocJetIndex, const std::vector<conType>* genAssocCon,
    const std::vector<Particle>* genParticles,const std::vector<float   >* hadronE) {

  //first add all partons
  //map to link particle index to parton index
  vector<int> partonIdxs(genParticles->size(),-1);
  for(unsigned int iP = 0; iP < genParticles->size(); ++iP){
    const int pdgId = TMath::Abs(genParticles->at(iP).pdgId());
    if(!ParticleInfo::isQuarkOrGluon(pdgId) || pdgId == ParticleInfo::p_t ) continue;
    if(!ParticleInfo::isDocOutgoing(genParticles->at(iP).status())) continue;
    partonIdxs[iP] = partons.size();
    partons.emplace_back(&genParticles->at(iP),iP,hadronE->at(iP));
  }

  //fill jet containments!
  int conIndex = -1;
  for(size iJ = 0; iJ < genAssocCon->size(); ++iJ){
    conType con = genAssocCon->at(iJ);
    //if it is a new parton, the assoc is less than 0;
    if(con < 0){
      con *= -1;
      conIndex++;
    }
    assert(conIndex >= 0);
    assert(partonIdxs[genAssocPrtIndex->at(conIndex)] >= 0);
    partons[partonIdxs[genAssocPrtIndex->at(conIndex)]].addContainment(genAssocJetIndex->at(iJ), fromContainmentType(con) );
  }

  for(auto& p : partons ) p.finalize(jets);

  //Now let's get our tops!
  for(const auto& p : *genParticles){
    if(TMath::Abs(p.pdgId()) != ParticleInfo::p_t) continue;
    bool final = true;
    for(unsigned int iD = 0; iD< p.numberOfDaughters(); ++iD){
      if(TMath::Abs(p.daughter(iD)->pdgId()) == ParticleInfo::p_t) final = false;
    }
    if(!final) continue;

    topDecays.emplace_back(&p,partons);
  }

  sort(topDecays.begin(),topDecays.end(),GreaterTopDecayPT<TopDecay>());

}
//--------------------------------------------------------------------------------------------------
void TopDecayEvent::getDecayMatches(const vector<ucsbsusy::RecoJetF*> recoJets, vector<TopDecayEvent::DecayID>& decayIDs)  const {
  decayIDs.clear();
  decayIDs.resize(recoJets.size());

  for(unsigned int iJ = 0; iJ < recoJets.size(); ++iJ){
    const auto * gj = recoJets[iJ]->genJet();
    if(gj == 0) continue;

    for(unsigned int iP = 0; iP < partons.size(); ++iP){
      for(unsigned int iC = 0; iC < partons[iP].containment.size(); ++iC){
        if(gj->index() !=  partons[iP].containment[iC].second) continue;
        decayIDs[iJ].conPartons.emplace_back( partons[iP].containment[iC].first* partons[iP].hadE,&partons[iP]);
      }
    }
    std::sort( decayIDs[iJ].conPartons.begin(),  decayIDs[iJ].conPartons.end(), PhysicsUtilities::greaterFirst<float,const Parton*>());

    if(decayIDs[iJ].conPartons.size()){
      const Parton * leadPtr = decayIDs[iJ].conPartons.front().second;
      for(unsigned int iT = 0; iT < topDecays.size(); ++iT){
        if(leadPtr == topDecays[iT].b){
          decayIDs[iJ].topInd = iT;
          decayIDs[iJ].type = DecayID::TOP_B;
          break;
        }
        if(leadPtr == topDecays[iT].W_dau1){
          decayIDs[iJ].topInd = iT;
          decayIDs[iJ].type = DecayID::TOP_W;
          break;
        }
        if(leadPtr == topDecays[iT].W_dau2){
          decayIDs[iJ].topInd = iT;
          decayIDs[iJ].type = DecayID::TOP_W;
          break;
        }
      }
      if(decayIDs[iJ].type == DecayID::NONE) decayIDs[iJ].type = DecayID::RADIATED;
    }
  }
}
