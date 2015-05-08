//--------------------------------------------------------------------------------------------------
// 
// Jet
// 
// Class to hold basic jet information. To be enhanced as needed.
// 
// Jet.h created on Tue Aug 19 16:26:39 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_DATAFORMATS_CMSTOP_H
#define ANALYSISTOOLS_DATAFORMATS_CMSTOP_H

#include <vector>
#include <iostream>

#include "AnalysisTools/DataFormats/interface/Momentum.h"

namespace ucsbsusy {

//_____________________________________________________________________________
// Basic jet type....used for both reco and gen jets
//_____________________________________________________________________________

template <class CoordSystem>
class CMSTop : public Momentum<CoordSystem>
  {
  public :
  CMSTop() : index_(-1), 
             fJMass_(-9.), fJTrimmedMass_(-9.), fJPrunedMass_(-9.), fJSoftDropMass_(-9.), fJFilteredMass_(-9.), fJMDFilteredMass_(-9.),
             fJTau1_(-9.), fJTau2_(-9.), fJTau3_(-9.), topMass_(-9), wMass_(-9), minMass_(-9), nSubJets_(-9) {}
    


    template <class InputCoordSystem>
      CMSTop(const ROOT::Math::LorentzVector<InputCoordSystem>& inMomentum, 
	     const int   inIndex            = -1,
      	     const float inFJMass           = -9.,
	     const float inFJTrimmedMass    = -9.,
	     const float inFJPrunedMass     = -9.,
	     const float inFJSoftDropMass   = -9.,
	     const float inFJFilteredMass   = -9.,
	     const float inFJMDFilteredMass = -9.,
	     const float inFJTau1           = -9.,
	     const float inFJTau2           = -9.,
	     const float inFJTau3           = -9.,
	     const float inTopMass          = -9.,
	     const float inWMass            = -9.,
	     const float inMinMass          = -9.,
	     const int   inNSubJets         = -9.
	     )
      : Momentum<CoordSystem>(inMomentum), index_(inIndex), fJMass_(inFJMass), fJTrimmedMass_(inFJTrimmedMass), fJPrunedMass_(inFJPrunedMass), 
      fJSoftDropMass_(inFJSoftDropMass), fJMDFilteredMass_(inFJMDFilteredMass), fJTau1_(inFJTau1), fJTau2_(inFJTau2), fJTau3_(inFJTau3), 
      topMass_(inTopMass), wMass_(inWMass), minMass_(inMinMass), nSubJets_(inNSubJets)  {}

    ~CMSTop(){}
    
    int	index()		     const { return index_;            }
    float fJMass()           const { return fJMass_;           }
    float fJTrimmedMass()    const { return fJTrimmedMass_;    }
    float fJPrunedMass()     const { return fJPrunedMass_;     }
    float fJSoftDropMass()   const { return fJSoftDropMass_;   }
    float fJFilteredMass()   const { return fJFilteredMass_;   }
    float fJMDFilteredMass() const { return fJMDFilteredMass_; }
    float fJTau1()           const { return fJTau1_;           }
    float fJTau2()           const { return fJTau2_;           }
    float fJTau3()           const { return fJTau3_;           }
    float topMass()          const { return topMass_;          }
    float wMass()            const { return wMass_;            }
    float minMass()          const { return minMass_;          }
    int   nSubJets()         const { return nSubJets_;         }

    void  setIndex(const int& newIndex)                       { index_            = newIndex;           }
    void  setFJMass(const float inFJMass)                     { fJMass_           = inFJMass;           }
    void  setFJTrimmedMass(const float inFJTrimmedMass)       { fJTrimmedMass_    = inFJTrimmedMass;    }
    void  setFJPrunedMass(const float inFJPrunedMass)         { fJPrunedMass_     = inFJPrunedMass;     }
    void  setFJSoftDropMass(const float inFJSoftDropMass)     { fJSoftDropMass_   = inFJSoftDropMass;   }
    void  setFJFilteredMass(const float inFJFilteredMass)     { fJFilteredMass_   = inFJFilteredMass;   }
    void  setFJMDFilteredMass(const float inFJMDFilteredMass) { fJMDFilteredMass_ = inFJMDFilteredMass; }
    void  setFJTau1(const float inFJTau1)                     { fJTau1_           = inFJTau1;           }
    void  setFJTau2(const float inFJTau2)                     { fJTau2_           = inFJTau2;           }
    void  setFJTau3(const float inFJTau3)                     { fJTau3_           = inFJTau3;           }
    void  setTopMass(const float inTopMass)                   { topMass_          = inTopMass;          }
    void  setWMass(const float inWMass)                       { wMass_            = inWMass;            }
    void  setMinMass(const float inMinMass)                   { minMass_          = inMinMass;          }
    void  setNSubJets(const int inNSubJets)                   { nSubJets_         = inNSubJets;         }
    

    //----Convenience function for throwing an exception when a member does not exist
    void checkStorage (const void * ptr, std::string message) const {
      if(ptr == 0) throw (message+std::string("The object was never loaded!"));
    }
    
  protected :
    int	  index_;  //Index in CMSTop vector
    float fJMass_;
    float fJTrimmedMass_;
    float fJPrunedMass_;
    float fJSoftDropMass_;
    float fJFilteredMass_;
    float fJMDFilteredMass_;
    float fJTau1_;
    float fJTau2_;
    float fJTau3_;
    float topMass_;
    float wMass_;
    float minMass_;
    int   nSubJets_;
    
  };
  
  typedef CMSTop<CylLorentzCoordF> CMSTopF;
  typedef std::vector<CMSTopF>     CMSTopFCollection;
}
#endif
