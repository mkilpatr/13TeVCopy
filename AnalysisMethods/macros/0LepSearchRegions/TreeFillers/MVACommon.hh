// common gen and matching code between HTTMVA.hh and SoftdropMVA.hh
//   calcGenInfo
//   getmaxdrqfj
//   getTopCand

namespace MVACommon {
  void calcGenInfo(const PartonMatching::TopDecay *thistop, const vector<const PartonMatching::TopDecay*> tops, float &mindrtq, float &mindrqq, float &mindrtt, float &quarkalarmdr, float &quarkalarmpt, float quarkalarm){
    mindrtq = 99., mindrqq = 99., mindrtt = 99.;
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

    for(const auto otherpart : otherparts){
      mindrtq = min(double(mindrtq), PhysicsUtilities::deltaR(*otherpart, *thistop->top) );
      if(mindrtq < quarkalarm) {
        quarkalarmdr = PhysicsUtilities::deltaR(*otherpart, *thistop->top);
        quarkalarmpt = otherpart->pt();
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
  PartonMatching::TopDecay * getTopCand( const FatJet& fj, const std::vector<const PartonMatching::TopDecay*> tops, double matchCone, double exclusionCone) {
    double dr = 99.;
    auto idx = PhysicsUtilities::findNearestDRDeref(*fj, tops, dr, matchCone);
    if( idx < 0 ) return 0;
    const PartonMatching::TopDecay * top = tops.at(idx);

    bool quarksContained = deltaR(*fj, *top->b->parton     ) < matchCone
                        && deltaR(*fj, *top->W_dau1->parton) < matchCone
                        && deltaR(*fj, *top->W_dau2->parton) < matchCone;

    float mindrtq = 99., mindrqq = 99., mindrtt = 99., quarkalarmdr = 99., quarkalarmpt = -9.;
    if(top) calcGenInfo(top, tops, mindrtq, mindrqq, mindrtt, quarkalarmdr, quarkalarmpt, exclusionCone); // just need mindrtq from this
    bool quarksExcluded = mindrtq > exclusionCone;

    return (quarksContained && quarksExcluded ? top : 0);
  }

}//namespace MVACommon
