//--------------------------------------------------------------------------------------------------
// 
// Jet
// 
// Class to hold basic jet information. To be enhanced as needed.
// 
// Jet.h created on Tue Aug 19 16:26:39 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_DATAFORMATS_FATJET_H
#define ANALYSISTOOLS_DATAFORMATS_FATJET_H

#include <vector>
#include <iostream>

#include "AnalysisTools/DataFormats/interface/Momentum.h"

namespace ucsbsusy {

//_____________________________________________________________________________
// Basic jet type....used for both reco and gen jets
//_____________________________________________________________________________

template <class CoordSystem>
class FatJet : public Momentum<CoordSystem>
  {
  public :
  FatJet() : index_(-1), 
      fjRawMass_(-9.), fjPrunedMass_(-9.), fjSoftDropMass_(-9.), fjCmsTopTagMass_(-9.),
      fjTau1_(-9.), fjTau2_(-9.), fjTau3_(-9.), fjNSDSubjets_(0), 
      sj1mass_(-9.), sj1pt_(-9.), sj1eta_(-9.), sj1phi_(-9.), 
      sj2mass_(-9.), sj2pt_(-9.), sj2eta_(-9.), sj2phi_(-9.) {}
    


    template <class InputCoordSystem>
      FatJet(const ROOT::Math::LorentzVector<InputCoordSystem>& inMomentum, 
	     const int   inIndex            = -1,
      	     const float inFJRawMass       = -9.,
	     const float inFJPrunedMass    = -9.,
	     const float inFJSoftDropMass  = -9.,
	     const float inFJCmsTopTagMass = -9.,
	     const float inFJTau1          = -9.,
	     const float inFJTau2          = -9.,
	     const float inFJTau3          = -9.,
	     const int   inFJNSDSubjets    =  0,
	     const float inSJ1Mass         = -9.,
	     const float inSJ1Pt           = -9.,
	     const float inSJ1Eta          = -9.,
	     const float inSJ1Phi          = -9.,
	     const float inSJ2Mass         = -9.,
	     const float inSJ2Pt           = -9.,
	     const float inSJ2Eta          = -9.,
	     const float inSJ2Phi          = -9.
	     )
      : Momentum<CoordSystem>(inMomentum), index_(inIndex), fjRawMass_(inFJRawMass),  fjPrunedMass_(inFJPrunedMass), 
      fjSoftDropMass_(inFJSoftDropMass), fjCmsTopTagMass_(inFJCmsTopTagMass), fjTau1_(inFJTau1), fjTau2_(inFJTau2), fjTau3_(inFJTau3),
      fjNSDSubjets_(inFJNSDSubjets), sj1mass_(inSJ1Mass), sj1pt_(inSJ1Pt), sj1eta_(inSJ1Eta), sj1phi_(inSJ1Phi), 
      sj2mass_(inSJ2Mass), sj2pt_(inSJ2Pt), sj2eta_(inSJ2Eta), sj2phi_(inSJ2Phi) {}

    ~FatJet(){}
    
    int	index()		     const { return index_;           }
    float fjRawMass()       const { return fjRawMass_;      }
    float fjPrunedMass()    const { return fjPrunedMass_;   }
    float fjSoftDropMass()  const { return fjSoftDropMass_; }
    float fjCmsTopTagMass() const { return fjCmsTopTagMass_; }
    float fjTau1()          const { return fjTau1_;         }
    float fjTau2()          const { return fjTau2_;         }
    float fjTau3()          const { return fjTau3_;         }
    int   fjNSDSubjets()    const { return fjNSDSubjets_; }
    float fjSJ1Mass()       const { return sj1mass_;         }
    float fjSJ1Pt()         const { return sj1pt_;          }
    float fjSJ1Eta()        const { return sj1eta_;         }
    float fjSJ1Phi()        const { return sj1phi_;         }
    float fjSJ2Mass()       const { return sj2mass_;        }
    float fjSJ2Pt()         const { return sj2pt_;          }
    float fjSJ2Eta()        const { return sj2eta_;         }
    float fjSJ2Phi()        const { return sj2phi_;         }

    void  setIndex(const int& newIndex)                     { index_            = newIndex;           }
    void  setFJRawMass(const float inFJRawMass)             { fjRawMass_       = inFJRawMass;       }
    void  setFJPrunedMass(const float inFJPrunedMass)       { fjPrunedMass_    = inFJPrunedMass;    }
    void  setFJSoftDropMass(const float inFJSoftDropMass)   { fjSoftDropMass_  = inFJSoftDropMass;  }
    void  setFJCmsTopTagMass(const float inFJCmsTopTagMass) { fjCmsTopTagMass_ = inFJCmsTopTagMass; }
    void  setFJTau1(const float inFJTau1)                   { fjTau1_          = inFJTau1;         }
    void  setFJTau2(const float inFJTau2)                   { fjTau2_          = inFJTau2;         }
    void  setFJTau3(const float inFJTau3)                   { fjTau3_          = inFJTau3;         }
    void  setFJNSDSubjets(const int inFJNSDSubjets)         { fjNSDSubjets_    = inFJNSDSubjets;   }
    void  setFJSJ1Mass(const float inSJ1Mass)               { sj1mass_         = inSJ1Mass;        }
    void  setFJSJ1Pt(const float inSJ1Pt)                   { sj1pt_           = inSJ1Pt;         }
    void  setFJSJ1Eta(const float inSJ1Eta)                 { sj1eta_          = inSJ1Eta;        }
    void  setFJSJ1Phi(const float inSJ1Phi)                 { sj1phi_          = inSJ1Phi;        }
    void  setFJSJ2Mass(const float inSJ2Mass)               { sj2mass_         = inSJ2Mass;        }
    void  setFJSJ2Pt(const float inSJ2Pt)                   { sj2pt_           = inSJ2Pt;         }
    void  setFJSJ2Eta(const float inSJ2Eta)                 { sj2eta_          = inSJ2Eta;        }
    void  setFJSJ2Phi(const float inSJ2Phi)                 { sj2phi_          = inSJ2Phi;        }
    

    //----Convenience function for throwing an exception when a member does not exist
    void checkStorage (const void * ptr, std::string message) const {
      if(ptr == 0) throw std::invalid_argument(message+std::string("The object was never loaded!"));
    }
    
  protected :
    int	  index_;  //Index in FatJet vector
    float fjRawMass_;
    float fjPrunedMass_;
    float fjSoftDropMass_;
    float fjCmsTopTagMass_;
    float fjTau1_;
    float fjTau2_;
    float fjTau3_;
    int   fjNSDSubjets_;
    float sj1mass_;
    float sj1pt_;
    float sj1eta_;
    float sj1phi_;
    float sj2mass_;
    float sj2pt_;
    float sj2eta_;
    float sj2phi_;
    
  };
  
  typedef FatJet<CylLorentzCoordF> FatJetF;
  typedef std::vector<FatJetF>     FatJetFCollection;
}
#endif
