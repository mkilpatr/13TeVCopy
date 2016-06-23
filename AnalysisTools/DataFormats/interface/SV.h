//--------------------------------------------------------------------------------------------------
// 
// Jet
// 
// Class to hold basic jet information. To be enhanced as needed.
// 
// Jet.h created on Tue Aug 19 16:26:39 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_DATAFORMATS_SV_H
#define ANALYSISTOOLS_DATAFORMATS_SV_H

#include <vector>
#include <iostream>

#include "AnalysisTools/DataFormats/interface/Momentum.h"

namespace ucsbsusy {

//_____________________________________________________________________________
// Basic jet type....used for both reco and gen jets
//_____________________________________________________________________________

template <class CoordSystem>
class SV : public Momentum<CoordSystem>
  {
  public :
  SV() : 
    index_(-1), svNTracks_(-9.), svChi2_(-9.), svNdf_(-9.), svdxy_(-9.), svd3D_(-9.), svd3Derr_(-9.), svCosSVPV_(-9.) {}
    

    template <class InputCoordSystem>
      SV(const ROOT::Math::LorentzVector<InputCoordSystem>& inMomentum, 
	 const int   inIndex   = -1,
	 //	 const float inSVMass  = -9.,
	 const int   inNTracks = -9.,
         const float inChi2    = -9.,
	 const float inNdf     = -9.,
	 const float indxy     = -9.,
	 const float ind3D     = -9.,
	 const float ind3Derr  = -9.,
	 const float inCosSVPV = -9.
	     )
      : Momentum<CoordSystem>(inMomentum), index_(inIndex), svNTracks_(inNTracks), svChi2_(inChi2), svNdf_(inNdf), 
      svdxy_(indxy), svd3D_(ind3D), svCosSVPV_(inCosSVPV) {}

    ~SV(){}
    
    int   index()     const { return index_;     }
    int   svNTracks() const { return svNTracks_; }
    float svChi2()    const { return svChi2_;    }
    float svNdf()     const { return svNdf_;     }
    float svdxy()     const { return svdxy_;     }
    float svd3D()     const { return svd3D_;     }
    float svd3Derr()  const { return svd3Derr_;  }
    float svCosSVPV() const { return svCosSVPV_; }


    void setIndex(const int& newIndex)        { index_     = newIndex;    }
    void setSVNTracks(const int& inNTracks)   { svNTracks_ = inNTracks; }
    void setSVChi2(const float& inChi2)       { svChi2_    = inChi2;      }
    void setSVNdf(const float& inNdf)         { svNdf_     = inNdf;       }
    void setSVdxy(const float& indxy)         { svdxy_     = indxy;       }
    void setSVd3D(const float& ind3D)         { svd3D_     = ind3D;       }
    void setSVd3Derr(const float& ind3Derr)   { svd3Derr_  = ind3Derr;    }
    void setSVCosSVPV(const float& inCosSVPV) { svCosSVPV_ = inCosSVPV;   }
    

    //----Convenience function for throwing an exception when a member does not exist
    void checkStorage (const void * ptr, std::string message) const {
      if(ptr == 0) throw std::invalid_argument(message+std::string("The object was never loaded!"));
    }
    
  protected :
    int	  index_;  //Index in SV vector
    int   svNTracks_;
    float svChi2_;
    float svNdf_;
    float svdxy_;
    float svd3D_;
    float svd3Derr_;
    float svCosSVPV_;
    
  };
  
  typedef SV<CylLorentzCoordF> SVF;
  typedef std::vector<SVF>     SVFCollection;
}
#endif
