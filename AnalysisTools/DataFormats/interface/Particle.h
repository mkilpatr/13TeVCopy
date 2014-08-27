//--------------------------------------------------------------------------------------------------
// 
// Particle
// 
// Basic particle class.
// 
// Particle.h created on Tue Aug 19 16:35:22 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_DATAFORMATS_PARTICLE_H
#define ANALYSISTOOLS_DATAFORMATS_PARTICLE_H




#include <vector>
#include <TObject.h>
#include "Math/GenVector/LorentzVector.h"
#include "Math/GenVector/PtEtaPhiM4D.h"
namespace ucsbsusy {

typedef   ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> >  CylLorentzVectorF;
class Particle;
typedef std::vector<Particle> ParticleCollection;
  class Particle : public TObject
  {

    public :
      Particle() :
	fMom(), fCharge(0) {}
      ~Particle(){}

      enum ParticleType {
	kElectron,
	kMuon,
	kHadronicTau,
	kNeutrino,
	kPhoton,
	kChargedHadron,
	kNeutralHadron
      };

      // Functions to facilitate momentum operations
      Float_t		pt()			const	{ return fMom.Pt();	}
      Float_t		eta()			const	{ return fMom.Eta();		}
      Float_t		phi()			const	{ return fMom.Phi();		}
      Float_t		mass()			const	{ return fMom.M();		}
      Float_t		E()			const	{ return fMom.E();		}
      Float_t		Et()			const	{ return fMom.Et();		}
      Float_t		mt()			const	{ return fMom.Mt();		}
      Float_t		px()			const	{ return fMom.Px();		}
      Float_t		py()			const	{ return fMom.Py();		}
      Float_t		pz()			const	{ return fMom.Pz();		}
      Float_t		p()			const	{ return fMom.P();		}
      Float_t		y()			const	{ return fMom.Rapidity();	}
      Float_t		theta()			const	{ return fMom.Theta();		}

      //Momentum getting and setting functions
      CylLorentzVectorF& p4()              { return fMom; }
      const CylLorentzVectorF& p4()   const{ return fMom; }

      template< class Coords >
      void setP4(const ROOT::Math::LorentzVector<Coords> & v ) {fMom = v;}


      //Charge functions
      void		setCharge(Int_t c)		{ fCharge = c;			}
      Int_t   charge()    const { return fCharge;   }

    protected :
      CylLorentzVectorF	fMom;
      Int_t		fCharge;

    ClassDef(Particle, 1)

  };
}

#endif
