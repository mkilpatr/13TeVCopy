//--------------------------------------------------------------------------------------------------
// 
// Momemntum.h
// 
// Momentum class, used as base class for custom objects.
// 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_DATAFORMATS_MOMENTUM_H
#define ANALYSISTOOLS_DATAFORMATS_MOMENTUM_H

#include "AnalysisTools/Utilities/interface/Types.h"

namespace ucsbsusy {

template <class CoordSystem>
class Momentum
{

public :
  Momentum() {}

  template <class InputCoordSystem>
  Momentum(ROOT::Math::LorentzVector<InputCoordSystem> inMomentum) : fMom(inMomentum) {}

  ~Momentum(){}

  // Functions to facilitate momentum operations
  Float_t   pt()      const { return fMom.Pt();       }
  Float_t   eta()     const { return fMom.Eta();      }
  Float_t   phi()     const { return fMom.Phi();      }
  Float_t   mass()    const { return fMom.M();        }
  Float_t   E()       const { return fMom.E();        }
  Float_t   energy()  const { return fMom.E();        }
  Float_t   Et()      const { return fMom.Et();       }
  Float_t   mt()      const { return fMom.Mt();       }
  Float_t   px()      const { return fMom.Px();       }
  Float_t   py()      const { return fMom.Py();       }
  Float_t   pz()      const { return fMom.Pz();       }
  Float_t   p()       const { return fMom.P();        }
  Float_t   y()       const { return fMom.Rapidity(); }
  Float_t   theta()   const { return fMom.Theta();    }

  //Momentum getting and setting functions
  ROOT::Math::LorentzVector<CoordSystem>&        p4()         { return fMom; }
  const  ROOT::Math::LorentzVector<CoordSystem>& p4()   const { return fMom; }

  template< class Coords >
  void setP4(const ROOT::Math::LorentzVector<Coords> & v )    { fMom = v;    }

private:
  ROOT::Math::LorentzVector<CoordSystem>	 fMom;

};

  typedef Momentum<CylLorentzCoord>  MomentumD;
  typedef Momentum<CylLorentzCoordF> MomentumF;

  typedef std::vector<MomentumD> MomentumDCollection;
  typedef std::vector<MomentumF> MomentumFCollection;

}

#endif
