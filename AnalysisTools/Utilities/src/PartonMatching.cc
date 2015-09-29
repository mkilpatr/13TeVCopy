

#include "AnalysisTools/Utilities/interface/PartonMatching.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"


using namespace std;
using namespace ucsbsusy;


namespace PartonMatching{
//We only do > tests so conversion is to the percent floor
conType toContainmentType(const float inCon ){ return ucsbsusy::convertTo<conType>( std::floor(inCon*100) ,"TopJetMatching::toContainmentType");}
float   fromContainmentType(const conType inCon){return float(inCon)/100; }

float minPartonPT         (20 );
float maxPartonETA        (2.4);
float minHadronRelE       (.50);
float minPartontRelE      (.50);
float extraJetsPartonRelE (.15);
float minJetRelE          (.50);


//--------------------------------------------------------------------------------------------------
void setPartonPurity(bool pure){
  minPartonPT         = pure ? 20   : 20 ;
  maxPartonETA        = pure ? 2.4  : 2.4;
  minHadronRelE       = pure ? .50  : .50;
  minPartontRelE      = pure ? .50  : .15;
  extraJetsPartonRelE = pure ? .15  : .15;
  minJetRelE          = pure ? .50  : .0 ;
}
//--------------------------------------------------------------------------------------------------
PartonDiagnosis getPartonDiagnosis(const Parton& parton, const std::vector<Jet*>& jets,const std::vector<const Parton *>* impPartons) {
  if( parton.parton->pt() < minPartonPT)             return SOFT_PARTON;
  if( TMath::Abs(parton.parton->eta()) > maxPartonETA)return HIGH_ETA_PARTON;
  if(parton.hadE < minHadronRelE* parton.parton->energy() ) return DISPERSED_PARTON;

  int numSigJets = 0;
  bool numMainJets = 0;
  for(const auto& c : parton.filteredContaiment) {if(c.first > minPartontRelE)numMainJets++;  if(c.first > extraJetsPartonRelE)numSigJets++;   }

  if(!numMainJets)                                   return NO_JET;
  if(numSigJets > 1)                                return SPLIT_JETS;


  if(impPartons)
    for(const auto* iP : (*impPartons)){
      if(iP->genIdx == parton.genIdx) continue;
      if(iP->filteredContaiment.size() && iP->filteredContaiment[0].second == parton.filteredContaiment[0].second )
        return MERGED_JET;
    }

 if(parton.filteredContaiment[0].first*parton.hadE < minJetRelE*jets[parton.filteredContaiment[0].second ]->energy())
   return DIRTY_JET;

 return RESOLVED_PARTON;
}


//--------------------------------------------------------------------------------------------------
DecayDiagnosis getDecayDiagnosis(const std::vector<const Parton *>& decayPartons){
  int nBadPartons = 0;
  int nlostJets = 0;
  int nsplitJets = 0;
  int nmergedJets = 0;
  int ndirtyJets = 0;

  for(unsigned int iP1 = 0 ; iP1 < decayPartons.size(); ++iP1){
    const auto* p1 = decayPartons[iP1];
    if(p1->diag <= DISPERSED_PARTON ) nBadPartons++;
    else if(p1->diag == NO_JET)       nlostJets++;
    else if(p1->diag == SPLIT_JETS)   nsplitJets++;
    else if(p1->diag == MERGED_JET)   nmergedJets++;
    else if(p1->diag == DIRTY_JET)    ndirtyJets++;
  }

  if(nBadPartons) return BAD_PARTON;
  if(nlostJets)   return LOST_JET;
  if(nsplitJets)  return SPLIT_PARTONS;
  if(nmergedJets)  return MERGED_PARTONS;
  if(ndirtyJets)  return CONTAMINATION;
  return RESOLVED;
}

//--------------------------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const Parton& m){
  os << m.genIdx <<" "<<ParticleInfo::titleFor(m.parton->pdgId())<<"("<<m.parton->pdgId()<<") "<< m.parton->status() <<" "<< *m.parton
      << " -> "<< m.hadE <<" ";

  for(const auto& c : m.filteredContaiment )
    os <<"("<<c.second <<","<<c.first<<")";
  return os;
}

//--------------------------------------------------------------------------------------------------
BosonDecay::BosonDecay(const Particle * inBoson, int indx, const std::vector<Parton>& allPartons) :
    boson(0),
    boson_dau1(0),
    boson_dau2(0),
    isHadronic(true),
    diag(numDecayDiagnoses)
{
  if(!ParticleInfo::isEWKBoson(inBoson->pdgId()))
    throw std::invalid_argument("BosonDecay::BosonDecay(): Initialization particle must be a EWK boson!");


  boson = inBoson;

  //treat photons as something special
  if(boson->pdgId() == ParticleInfo::p_gamma) {
    isHadronic = false;
    nonHadronicPartons.emplace_back( inBoson,indx , 0);
    boson_dau1 = &nonHadronicPartons.front();
    return;
  }

  if(boson->numberOfDaughters() < 2){
    ParticleInfo::printGenParticleInfo(boson,-1);
    throw std::invalid_argument("BosonDecay::BosonDecay(): Boson has abnormal decay!");
   }

  CandidateRef<Particle> dau1Ref,dau2Ref;
  for(unsigned int iD = 0; iD< boson->numberOfDaughters(); ++iD){
    bool isLep = false;
    bool isQuark = false;
    auto dau = boson->daughterRef(iD);
    if(ParticleInfo::isLeptonOrNeutrino(dau->pdgId())) {isLep = true; isHadronic = false;}
    if(ParticleInfo::isQuark(dau->pdgId())) isQuark = true;
    if(!isLep && !isQuark) continue;
    if(dau1Ref.isNull()){
      dau1Ref = dau;
      continue;
    }
    if(!dau2Ref.isNull()
        ||
        (isLep != ParticleInfo::isLeptonOrNeutrino(dau1Ref->pdgId()))
    ){
      ParticleInfo::printGenParticleInfo(boson,-1);
      throw std::invalid_argument("BosonDecay::BosonDecay(): Boson has abnormal decay!");
    }
    dau2Ref = dau;
  }

  if(dau2Ref.isNull()){
    ParticleInfo::printGenParticleInfo(boson,-1);
    throw std::invalid_argument("BosonDecay::BosonDecay(): Boson has abnormal decay!");
  }

  if(dau2Ref->pt() > dau1Ref->pt()){
    CandidateRef<Particle> tempRef = dau1Ref;
    dau1Ref = dau2Ref;
    dau2Ref = tempRef;
  }

  //special case for leptons
  if(!isHadronic){
    nonHadronicPartons.emplace_back( &*dau1Ref,  dau1Ref.key(), 0);
    nonHadronicPartons.emplace_back( &*dau2Ref,  dau2Ref.key(), 0);
    boson_dau1 = &nonHadronicPartons[0];
    boson_dau2 = &nonHadronicPartons[1];
  } else {
    for(const auto&  p: allPartons){
     if(p.genIdx == dau1Ref.key() ) boson_dau1 = &p;
     if(p.genIdx == dau2Ref.key() ) boson_dau2 = &p;
    }

    if(boson_dau1 == 0 || boson_dau2 == 0){
     ParticleInfo::printGenParticleInfo(*dau1Ref,dau1Ref.key());
     ParticleInfo::printGenParticleInfo(*dau2Ref,dau2Ref.key());
      throw std::invalid_argument("BosonDecay::BosonDecay(): Boson decay was not found in parton collection!");
    }
    hadronicPartons.push_back(boson_dau1);
    hadronicPartons.push_back(boson_dau2);
  }
}
//--------------------------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const BosonDecay& m){
  os << "boson: "<<ParticleInfo::titleFor(m.boson->pdgId())<<"("<<m.boson->pdgId()<<") "<< m.boson->status() <<" "<< *m.boson <<std::endl;
  os << (m.isHadronic ? "Hadronic" : "Leptonic") <<" diag: "<< m.diag;
  if(m.boson_dau1)
  os<<std::endl << "decay1: "<<*m.boson_dau1;
  if(m.boson_dau2)
  os <<std::endl<< "decay2: "<<*m.boson_dau2;

  return os;
}
//--------------------------------------------------------------------------------------------------
TopDecay::TopDecay(const Particle * inTop,const std::vector<BosonDecay>& bosonDecays, const std::vector<Parton>& allPartons):
    top(0),
    W(0),
    b(0),
    W_dau1(0),
    W_dau2(0),
    isLeptonic(false),
    diag(numDecayDiagnoses)
  {
  if(TMath::Abs(inTop->pdgId()) != ParticleInfo::p_t)
    throw std::invalid_argument("TopDecay::TopDecay(): Initialization particle must be a top!");

  top = inTop;

  for(unsigned int iD = 0; iD< inTop->numberOfDaughters(); ++iD){
    auto dau = inTop->daughterRef(iD);
    const int pdgId = TMath::Abs(dau->pdgId());
    if( pdgId == ParticleInfo::p_Wplus){
      if(W){
        ParticleInfo::printGenParticleInfo(inTop,-1);
        throw std::invalid_argument("TopDecay::TopDecay(): Top has abnormal decay!");
      }
      dau = ParticleInfo::getFinal(dau);
      for(const auto&  p: bosonDecays){
       if(p.boson != &(*dau)) continue;
       W = p.boson;
       W_decay = &p;
       W_dau1 = p.boson_dau1;
       W_dau2 = p.boson_dau2;
       if(p.isHadronic){
         hadronicPartons.push_back(W_dau1);
         hadronicPartons.push_back(W_dau2);
       } else {
         isLeptonic = true;
       }
       break;
      }
    } else if(pdgId == ParticleInfo::p_b || pdgId == ParticleInfo::p_s || pdgId == ParticleInfo::p_d){
      if(b){
        ParticleInfo::printGenParticleInfo(inTop,-1);
        throw std::invalid_argument("TopDecay::TopDecay(): Top has abnormal decay!");
      }
      for(const auto&  p: allPartons){
       if(p.genIdx != dau.key() ) continue;
       hadronicPartons.push_back(&p);
       b = &p;
      }
    }
  }

  if(W == 0 || b == 0){
   ParticleInfo::printGenParticleInfo(inTop,-1);
    throw std::invalid_argument("TopDecay::TopDecay(): Top has abnormal decay!");
  }
}
//--------------------------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const TopDecay& m){
  os << "top: "<<ParticleInfo::titleFor(m.top->pdgId())<<"("<<m.top->pdgId()<<") "<< m.top->status() <<" "<< *m.top <<std::endl;
  os << (m.isLeptonic ?"Leptonic" : "Hadronic" ) <<" diag: "<< m.diag;

  if(m.b)
  os <<std::endl<< "b: " << *m.b;
  if(m.W_decay)
    os <<std::endl<< *m.W_decay;
  return os;
}
//--------------------------------------------------------------------------------------------------
void PartonEvent::getPartonsAndContaiment(const std::vector<ucsbsusy::size16 >* genAssocPrtIndex, const std::vector<ucsbsusy::size16 >* genAssocJetIndex, const std::vector<conType>* genAssocCon,
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

  //First let's fill bosons
    for(unsigned int iP = 0; iP < genParticles->size(); ++iP){
      const auto& p = genParticles->at(iP);
    if(!ParticleInfo::isEWKBoson(p.pdgId())) continue;
    if(!ParticleInfo::isDoc(p.status())) continue;
    if(!ParticleInfo::isLastInChain(&p)) continue;
    for(unsigned int iD = 0; iD < p.numberOfDaughters(); ++iD)
      if(TMath::Abs(p.daughter(iD)->pdgId()) == ParticleInfo::p_t) continue;
    bosonDecays.emplace_back(&p,iP,partons);
  }

  //now for the tops!
  for(const auto& p : *genParticles){
    if(TMath::Abs(p.pdgId()) != ParticleInfo::p_t) continue;
    if(!ParticleInfo::isLastInChain(&p)) continue;
    topDecays.emplace_back(&p,bosonDecays,partons);
  }

  //Now add in the important partons
  for(const auto& d: bosonDecays)
    importantPartons.insert(importantPartons.end(), d.hadronicPartons.begin(), d.hadronicPartons.end());
  for(const auto& d: topDecays) importantPartons.push_back(d.b);

}


//--------------------------------------------------------------------------------------------------
void PartonEvent::finalizePartons(const std::vector<const Parton *>* impPartons){
  //first assign important jets to each parton
  for(auto& p : partons){
    sort(p.containment.begin(),p.containment.end(),PhysicsUtilities::greaterFirst<float,int>());
    p.filteredContaiment.clear();
    for(const auto& c : p.containment){
      for(unsigned int iJ =0; iJ < jets.size(); ++iJ)
        if(jets[iJ]->index() == c.second){
          p.filteredContaiment.emplace_back(c.first,iJ);
        }
    }
    if(p.filteredContaiment.size()) p.matchedJet = jets[p.filteredContaiment[0].second ];
  }

  //now get the diagnosis per jet
  for(auto& p : partons){p.diag = getPartonDiagnosis(p,jets,impPartons);}

  //And finally diagnosis per decay
  for(auto& d : bosonDecays){d.diag = getDecayDiagnosis(d.hadronicPartons);}
  for(auto& d : topDecays){d.diag = getDecayDiagnosis(d.hadronicPartons);}
}

//--------------------------------------------------------------------------------------------------
void PartonEvent::processSubtractedJetPTs(float maxNonHadDR){

  subtractedJetPTs.clear();
  subtractedJetPTs.reserve(jets.size());
  for(const auto* j: jets)
    subtractedJetPTs.push_back(j->pt());

  //We first subtract the hadronic energy because we have a good association per jet
  //Contained energy
  vector<float> containedHadronicE(jets.size(),0);
  for(const auto * p: importantPartons){
    for(const auto& c : p->filteredContaiment){
      containedHadronicE[c.second] += c.first*p->hadE;
    }
  }

  for(unsigned int iJ = 0; iJ < jets.size(); ++iJ){
    const auto* j = jets[iJ];
    if(j->energy() == 0) continue;
    float eScale = std::min(std::max((j->energy() - containedHadronicE[iJ]) / j->energy(), float(0.0)),float(1.0));

    if(eScale != 1)
      subtractedJetPTs[iJ] = j->pt() * eScale;
  }


  //now do the leptons and photons...we use a DR match
  for(const auto& b : bosonDecays ){
    for(const auto& p : b.nonHadronicPartons){
      const int absPDGID = TMath::Abs(p.parton->pdgId());
      if(ParticleInfo::isInvisible(absPDGID)) continue;

      CylLorentzVectorF visMom;
      if(absPDGID == ParticleInfo::p_tauminus){
        for(unsigned int iD = 0; iD < p.parton->numberOfDaughters(); ++iD){
          if(ParticleInfo::isInvisible(p.parton->daughterRef(iD)->pdgId())) continue;
          visMom += p.parton->daughterRef(iD)->p4();
        }
      } else {
        visMom = p.parton->p4();
      }

      double nearDR = 0;
      int near = PhysicsUtilities::findNearestDRDeref(visMom,jets,nearDR,maxNonHadDR);
      if(near >= 0)
        subtractedJetPTs[near] = std::max(float(0.0), subtractedJetPTs[near] - visMom.pt());
    }
  }
}
}

