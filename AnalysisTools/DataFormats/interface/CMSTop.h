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
             topRawMass_(-9.), topCmsTopTagMass_(-9.), topTau1_(-9.), topTau2_(-9.), topTau3_(-9.), topMinMass_(-9), topNSubJets_(-9) {}
    


    template <class InputCoordSystem>
      CMSTop(const ROOT::Math::LorentzVector<InputCoordSystem>& inMomentum, 
	     const int   inIndex            = -1,
      	     const float inTopRawMass       = -9.,
	     const float inTopCmsTopTagMass = -9.,
	     const float inTopTau1          = -9.,
	     const float inTopTau2          = -9.,
	     const float inTopTau3          = -9.,
	     const float inTopMinMass       = -9.,
	     const int   inTopNSubJets      = -9.
	     )
      : Momentum<CoordSystem>(inMomentum), index_(inIndex), topRawMass_(inTopRawMass), topCmsTopTagMass_(inTopCmsTopTagMass), topTau1_(inTopTau1), topTau2_(inTopTau2), topTau3_(inTopTau3), topMinMass_(inTopMinMass), topNSubJets_(inTopNSubJets)  {}

    ~CMSTop(){}
    
    int	index()		     const { return index_;           }
    float topRawMass()       const { return topRawMass_;      }
    float topCmsTopTagMass() const { return topCmsTopTagMass_; }
    float topTau1()          const { return topTau1_;         }
    float topTau2()          const { return topTau2_;         }
    float topTau3()          const { return topTau3_;         }
    float topMinMass()       const { return topMinMass_;      }
    int   topNsubJets()      const { return topNSubJets_;     }

    void  setIndex(const int& newIndex)                       { index_            = newIndex;           }
    void  setTopRawMass(const float inTopRawMass)             { topRawMass_       = inTopRawMass;       }
    void  setTopCmsTopTagMass(const float inTopCmsTopTagMass) { topCmsTopTagMass_ = inTopCmsTopTagMass; }
    void  setTopTau1(const float inTopTau1)                   { topTau1_           = inTopTau1;         }
    void  setTopTau2(const float inTopTau2)                   { topTau2_           = inTopTau2;         }
    void  setTopTau3(const float inTopTau3)                   { topTau3_           = inTopTau3;         }
    void  setTopMinMass(const float inTopMinMass)             { topMinMass_        = inTopMinMass;      }
    void  setTopNSubJets(const int inTopNSubJets)             { topNSubJets_       = inTopNSubJets;     }
    

    //----Convenience function for throwing an exception when a member does not exist
    void checkStorage (const void * ptr, std::string message) const {
      if(ptr == 0) throw std::invalid_argument(message+std::string("The object was never loaded!"));
    }
    
  protected :
    int	  index_;  //Index in CMSTop vector
    float topRawMass_;
    float topCmsTopTagMass_;
    float topTau1_;
    float topTau2_;
    float topTau3_;
    float topMinMass_;
    int   topNSubJets_;
    
  };
  
  typedef CMSTop<CylLorentzCoordF> CMSTopF;
  typedef std::vector<CMSTopF>     CMSTopFCollection;
}
#endif
