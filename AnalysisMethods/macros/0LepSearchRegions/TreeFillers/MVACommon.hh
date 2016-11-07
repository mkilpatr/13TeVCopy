
#ifndef MVACOMMON_HH
#define MVACOMMON_HH

// common gen and matching code between HTTMVA.hh and SoftdropMVA.hh
//   calcGenInfo
//   getmaxdrqfj
//   getTopCand

namespace MVACommon {
  void calcGenInfo(const PartonMatching::TopDecay *thistop, const vector<const PartonMatching::TopDecay*> tops, float &mindrtq, float &mindrqq, float &mindrtt, float &quarkalarmdr, float &quarkalarmpt, float quarkalarm){
    bool dbg = false;
    if(dbg) std::cout << "[   MVA]     calcGenInfo " << std::endl;
    mindrtq = 20., mindrqq = 20., mindrtt = 20., quarkalarmpt = -9., quarkalarmdr = 20.;
    assert(thistop);

    for(const auto top : tops){
      if(top == thistop) continue;
      mindrtt = min(double(mindrtt), PhysicsUtilities::deltaR(*top->top, *thistop->top));
    }

    // other tops' partons for pq and pp
    vector<const GenParticleF*> otherparts;
    for(auto top : tops){
      if(top == thistop) continue;
      otherparts.push_back(top->b->parton); otherparts.push_back(top->W_dau1->parton); otherparts.push_back(top->W_dau2->parton);
    }

    if(dbg) std::cout << "[   MVA]     calcGenInfo, # otherparts:  " << otherparts.size() << std::endl;
    for(const auto otherpart : otherparts){
      mindrtq = min(double(mindrtq), PhysicsUtilities::deltaR(*otherpart, *thistop->top) );
      if(dbg) std::cout << "[   MVA]     calcGenInfo, min dr from this top quark to other parts so far:  " << mindrtq << std::endl;
      if(mindrtq < quarkalarm && otherpart->pt() > quarkalarmpt) {
        quarkalarmdr = PhysicsUtilities::deltaR(*otherpart, *thistop->top);
        quarkalarmpt = otherpart->pt();
        if(dbg) std::cout << "[   MVA]     calcGenInfo QUARK ALARM!!! dr, pt " << quarkalarmdr << " " << quarkalarmpt << " " << quarkalarm << std::endl;
      }

      mindrqq = min(double(mindrqq), PhysicsUtilities::deltaR(*otherpart, *thistop->b->parton) );
      mindrqq = min(double(mindrqq), PhysicsUtilities::deltaR(*otherpart, *thistop->W_dau1->parton) );
      mindrqq = min(double(mindrqq), PhysicsUtilities::deltaR(*otherpart, *thistop->W_dau2->parton) );
    }
    return;
  }

  template<class FatJet>
  float getmaxdrqfj(const PartonMatching::TopDecay *thistop, const FatJet* fj){
    float maxdrqfj = -1.;
    maxdrqfj = max(double(maxdrqfj), PhysicsUtilities::deltaR(*thistop->b->parton, *fj));
    maxdrqfj = max(double(maxdrqfj), PhysicsUtilities::deltaR(*thistop->W_dau1->parton, *fj));
    maxdrqfj = max(double(maxdrqfj), PhysicsUtilities::deltaR(*thistop->W_dau2->parton, *fj));
    return maxdrqfj;
  }

  template<class FatJet>
  const PartonMatching::TopDecay * getTopCand( const FatJet& fj, const std::vector<const PartonMatching::TopDecay*> topdecays, double matchCone, double exclusionCone) {
    bool dbg = false;
    std::vector<const GenParticleF*> tops;
    for(const auto topdecay : topdecays) { tops.push_back(topdecay->top); } // get parton

    if(dbg) std::cout << "[   MVA]     getTopCand using fj pt, topdecays, matchcone, exclusioncone: " << fj->pt() << " " << tops.size() << " " << matchCone << " " << exclusionCone << std::endl; 
    double dr = 20.;
    auto idx = PhysicsUtilities::findNearestDRDeref(*fj, tops, dr, matchCone);
    if(dbg) std::cout << "[   MVA]     return value of findNearestDRDeref: " << idx << " " << (idx >= 0 ? tops.at(idx)->pt() : 0) << std::endl; 
    if( idx < 0 ) return 0;
    const PartonMatching::TopDecay * top = topdecays.at(idx);

    bool quarksContained = PhysicsUtilities::deltaR(*fj, *top->b->parton     ) < matchCone
                        && PhysicsUtilities::deltaR(*fj, *top->W_dau1->parton) < matchCone
                        && PhysicsUtilities::deltaR(*fj, *top->W_dau2->parton) < matchCone;

    float mindrtq = 20., mindrqq = 20., mindrtt = 20., quarkalarmdr = 20., quarkalarmpt = -9.;
    if(top) calcGenInfo(top, topdecays, mindrtq, mindrqq, mindrtt, quarkalarmdr, quarkalarmpt, exclusionCone); // just need mindrtq from this
    bool quarksExcluded = mindrtq > exclusionCone;

    if(dbg) std::cout << "[   MVA]     getTopCand quarksContained, quarksExcluded " << quarksContained << " " << quarksExcluded << std::endl;
    if(dbg) std::cout << "[   MVA]     getTopCand quarksContained raw dR b w1 w2 " << PhysicsUtilities::deltaR(*fj, *top->b->parton     ) << " " << PhysicsUtilities::deltaR(*fj, *top->W_dau1->parton) << " " << PhysicsUtilities::deltaR(*fj, *top->W_dau2->parton) << std::endl;
    return ( (quarksContained && quarksExcluded) ? top : 0);
  }

}//namespace MVACommon

#endif

