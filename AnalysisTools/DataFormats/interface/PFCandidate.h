//--------------------------------------------------------------------------------------------------
// 
// PFCandidate
// 
// Class for storing PFCandidate information.
// 
// PFCandidate.h created on Mon Jan 19 16:21:42 CET 2015 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_DATAFORMATS_PFCANDIDATE_H
#define ANALYSISTOOLS_DATAFORMATS_PFCANDIDATE_H

#include "AnalysisTools/DataFormats/interface/Momentum.h"

namespace ucsbsusy {

  template < class CoordSystem>
  class PFCandidate : public Momentum<CoordSystem>
  {

    public :
      PFCandidate() : index_(-1), pdgid_(0), q_(0), d0_(0), dz_(0), fromPV_(0), mt_(0), taudisc_(0), jetIndex_(-1), tauIndex_(-1), ismvavetotau_(false) {}

      template <class InputCoordSystem>
      PFCandidate(ROOT::Math::LorentzVector<InputCoordSystem> inMomentum,
                int inIndex = -1, int inPdgid = 0, int inCharge = 0,
                float inD0 = 0, float inDz = 0, int inFromPV = 0,
                float inMt = 0, float inTauDisc = 0,
                int inJetIndex = -1, int inTauIndex = -1) :
                Momentum<InputCoordSystem>(inMomentum), 
                index_(inIndex), pdgid_(inPdgid), q_(inCharge),
                d0_(inD0), dz_(inDz), fromPV_(inFromPV),
                mt_(inMt), taudisc_(inTauDisc),
                jetIndex_(inJetIndex), tauIndex_(inTauIndex),
                ismvavetotau_(false) {}

      ~PFCandidate() {}

      enum  PFType {unknown, pfhfhad = 1, pfem = 2, pfelectron = 11, pfmuon = 13, pfphoton = 22, pfh0 = 130, pfhplus = 211};

      int   index()           const { return index_;    }
      int   pdgid()           const { return pdgid_;    }
      int   charge()          const { return q_;        }
      float d0()              const { return d0_;       }
      float dz()              const { return dz_;       }
      int   fromPV()          const { return fromPV_;   }
      float mt()              const { return mt_;       }
      float taudisc()         const { return taudisc_;  }
      int   jetIndex()        const { return jetIndex_; }
      int   tauIndex()        const { return tauIndex_; }
      bool  iselectron()      const { return (fabs(pdgid_) == pfelectron); }
      bool  ismuon()          const { return (fabs(pdgid_) == pfmuon);     }
      bool  isphoton()        const { return (fabs(pdgid_) == pfphoton);   }
      bool  isneutralhadron() const { return (fabs(pdgid_) == pfh0);       }
      bool  ischargedhadron() const { return (fabs(pdgid_) == pfhplus);    }
      bool  ismvavetotau()    const { return ismvavetotau_;                }

      void   setIndex(int newIndex)       { index_ = newIndex;       }
      void   setPdgId(int newPdgId)       { pdgid_ = newPdgId;       }
      void   setCharge(int newCharge)     { q_ = newCharge;          }
      void   setD0(float newD0)           { d0_ = newD0;             }
      void   setDz(float newDz)           { dz_ = newDz;             }
      void   setFromPV(int newFromPV)     { fromPV_ = newFromPV;     }
      void   setMt(float newMt)           { mt_ = newMt;             }
      void   setTauDisc(float newDisc)    { taudisc_ = newDisc;      }
      void   setJetIndex(int newJetIndex) { jetIndex_ = newJetIndex; }
      void   setTauIndex(int newTauIndex) { tauIndex_ = newTauIndex; }
      void   setIsMVAVetoTau(bool flag)   { ismvavetotau_ = flag;    }

    protected :
      int   index_;  //Index in candidate vector
      int   pdgid_;
      int   q_;
      float d0_;
      float dz_;
      int   fromPV_;
      float mt_;
      float taudisc_;
      int   jetIndex_;
      int   tauIndex_;
      bool  ismvavetotau_;

  };

  typedef PFCandidate<CylLorentzCoordF> PFCandidateF;
  typedef std::vector<PFCandidateF>     PFCandidateFCollection;

}

#endif
