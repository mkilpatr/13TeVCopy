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
      PFCandidate() : index_(-1), pdgid_(0), q_(0), d0_(0), dz_(0), fromPV_(0), mt_(0), dphimet_(0), taudisc_(0), jetIndex_(-1), tauIndex_(-1), ismvavetotau_(false) {}

      template <class InputCoordSystem>
      PFCandidate(ROOT::Math::LorentzVector<InputCoordSystem> inMomentum,
                int inIndex = -1, int inPdgid = 0, int inCharge = 0,
                float inD0 = 0, float inDz = 0, int inFromPV = 0,
                float inMt = 0, float inDphiMet = 0, float inTauDisc = 0,
                int inJetIndex = -1, int inTauIndex = -1) :
                Momentum<InputCoordSystem>(inMomentum), 
                index_(inIndex), pdgid_(inPdgid), q_(inCharge),
                d0_(inD0), dz_(inDz), fromPV_(inFromPV),
                mt_(inMt), dphimet_(inDphiMet), taudisc_(inTauDisc),
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
      float dphimet()         const { return dphimet_;  }
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
      void   setDphi(float newDphi)       { dphimet_ = newDphi;      }
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
      float dphimet_;
      float taudisc_;
      int   jetIndex_;
      int   tauIndex_;
      bool  ismvavetotau_;

  };

  typedef PFCandidate<CylLorentzCoordF> PFCandidateF;
  typedef std::vector<PFCandidateF>     PFCandidateFCollection;

  class ExtendedPFCandidate : public PFCandidateF
  {
    public :
      ExtendedPFCandidate() : chiso0p1_(0), chiso0p2_(0), chiso0p3_(0), chiso0p4_(0), totiso0p1_(0), totiso0p2_(0), totiso0p3_(0), totiso0p4_(0), neartrkdr_(0) {}

      ~ExtendedPFCandidate() {}

      float chiso0p1()      const { return chiso0p1_;  }
      float chiso0p2()      const { return chiso0p2_;  }
      float chiso0p3()      const { return chiso0p3_;  }
      float chiso0p4()      const { return chiso0p4_;  }
      float totiso0p1()     const { return totiso0p1_; }
      float totiso0p2()     const { return totiso0p2_; }
      float totiso0p3()     const { return totiso0p3_; }
      float totiso0p4()     const { return totiso0p4_; }
      float neartrkdr()     const { return neartrkdr_; }

      void  setChIso0p1(float newIso)    { chiso0p1_ = newIso;  }
      void  setChIso0p2(float newIso)    { chiso0p2_ = newIso;  }
      void  setChIso0p3(float newIso)    { chiso0p3_ = newIso;  }
      void  setChIso0p4(float newIso)    { chiso0p4_ = newIso;  }
      void  setTotIso0p1(float newIso)   { totiso0p1_ = newIso; }
      void  setTotIso0p2(float newIso)   { totiso0p2_ = newIso; }
      void  setTotIso0p3(float newIso)   { totiso0p3_ = newIso; }
      void  setTotIso0p4(float newIso)   { totiso0p4_ = newIso; }
      void  setNearTrkDR(float newDR)    { neartrkdr_ = newDR;  }

    protected :
      float chiso0p1_;
      float chiso0p2_;
      float chiso0p3_;
      float chiso0p4_;
      float totiso0p1_;
      float totiso0p2_;
      float totiso0p3_;
      float totiso0p4_;
      float neartrkdr_;

  };

  typedef std::vector<ExtendedPFCandidate> ExtendedPFCandidateCollection;

}

#endif
