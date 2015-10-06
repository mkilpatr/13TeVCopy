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
             fjRawMass_(-9.), fjTrimmedMass_(-9.), fjPrunedMass_(-9.), fjSoftDropMass_(-9.), fjFilteredMass_(-9.), fjCmsTopTagMass_(-9.),
             fjTau1_(-9.), fjTau2_(-9.), fjTau3_(-9.) {}
    


    template <class InputCoordSystem>
      FatJet(const ROOT::Math::LorentzVector<InputCoordSystem>& inMomentum, 
	     const int   inIndex            = -1,
      	     const float inFJRawMass       = -9.,
	     const float inFJTrimmedMass   = -9.,
	     const float inFJPrunedMass    = -9.,
	     const float inFJSoftDropMass  = -9.,
	     const float inFJFilteredMass  = -9.,
	     const float inFJCmsTopTagMass = -9.,
	     const float inFJTau1          = -9.,
	     const float inFJTau2          = -9.,
	     const float inFJTau3          = -9.
	     )
      : Momentum<CoordSystem>(inMomentum), index_(inIndex), fjRawMass_(inFJRawMass), fjTrimmedMass_(inFJTrimmedMass), fjPrunedMass_(inFJPrunedMass), 
      fjSoftDropMass_(inFJSoftDropMass), fjCmsTopTagMass_(inFJCmsTopTagMass), fjTau1_(inFJTau1), fjTau2_(inFJTau2), fjTau3_(inFJTau3) {}

    ~FatJet(){}
    
    int	index()		     const { return index_;           }
    float fjRawMass()       const { return fjRawMass_;      }
    float fjTrimmedMass()   const { return fjTrimmedMass_;  }
    float fjPrunedMass()    const { return fjPrunedMass_;   }
    float fjSoftDropMass()  const { return fjSoftDropMass_; }
    float fjFilteredMass()  const { return fjFilteredMass_; }
    float fjCmsTopTagMass() const { return fjCmsTopTagMass_; }
    float fjTau1()          const { return fjTau1_;         }
    float fjTau2()          const { return fjTau2_;         }
    float fjTau3()          const { return fjTau3_;         }

    void  setIndex(const int& newIndex)                       { index_            = newIndex;           }
    void  setFJRawMass(const float inFJRawMass)             { fjRawMass_       = inFJRawMass;       }
    void  setFJTrimmedMass(const float inFJTrimmedMass)     { fjTrimmedMass_   = inFJTrimmedMass;   }
    void  setFJPrunedMass(const float inFJPrunedMass)       { fjPrunedMass_    = inFJPrunedMass;    }
    void  setFJSoftDropMass(const float inFJSoftDropMass)   { fjSoftDropMass_  = inFJSoftDropMass;  }
    void  setFJFilteredMass(const float inFJFilteredMass)   { fjFilteredMass_  = inFJFilteredMass;  }
    void  setFJCmsTopTagMass(const float inFJCmsTopTagMass) { fjCmsTopTagMass_ = inFJCmsTopTagMass; }
    void  setFJTau1(const float inFJTau1)                   { fjTau1_          = inFJTau1;         }
    void  setFJTau2(const float inFJTau2)                   { fjTau2_          = inFJTau2;         }
    void  setFJTau3(const float inFJTau3)                   { fjTau3_          = inFJTau3;         }
    

    //----Convenience function for throwing an exception when a member does not exist
    void checkStorage (const void * ptr, std::string message) const {
      if(ptr == 0) throw std::invalid_argument(message+std::string("The object was never loaded!"));
    }
    
  protected :
    int	  index_;  //Index in FatJet vector
    float fjRawMass_;
    float fjTrimmedMass_;
    float fjPrunedMass_;
    float fjSoftDropMass_;
    float fjFilteredMass_;
    float fjCmsTopTagMass_;
    float fjTau1_;
    float fjTau2_;
    float fjTau3_;
    
  };
  
  typedef FatJet<CylLorentzCoordF> FatJetF;
  typedef std::vector<FatJetF>     FatJetFCollection;
}
#endif
