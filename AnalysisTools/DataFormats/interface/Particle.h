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
#include <TLorentzVector.h>

namespace ucsbsusy {

  class Particle;
  typedef std::vector<Particle> ParticleCollection;

  class Particle : public TObject
  {

    public :
      Particle() :
	fMom(0,0,0,0), fCharge(0) {}
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

      Double_t		pt()			const	{ return fMom.Pt();		}
      Double_t		eta()			const	{ return fMom.Eta();		}
      Double_t		phi()			const	{ return fMom.Phi();		}
      Double_t		mass()			const	{ return fMom.M();		}
      Double_t		E()			const	{ return fMom.E();		}
      Double_t		Et()			const	{ return fMom.Et();		}
      Double_t		mt()			const	{ return fMom.Mt();		}
      Double_t		px()			const	{ return fMom.Px();		}
      Double_t		py()			const	{ return fMom.Py();		}
      Double_t		pz()			const	{ return fMom.Pz();		}
      Double_t		p()			const	{ return fMom.P();		}
      Double_t		y()			const	{ return fMom.Rapidity();	}
      Double_t		theta()			const	{ return fMom.Theta();		}
      TLorentzVector	mom()			const	{ return fMom;			}
      Int_t		charge()		const	{ return fCharge;		}

      void		setMom(TLorentzVector &v)	{ fMom = v;			}
      void		setPtEtaPhiM(Double_t ptg, Double_t etag, Double_t phig, Double_t massg)
							{ TLorentzVector v; v.SetPtEtaPhiM(ptg, etag, phig, massg); setMom(v);	}
      void		setCharge(Int_t c)		{ fCharge = c;			}

    protected :
      TLorentzVector	fMom;
      Int_t		fCharge;

    ClassDef(Particle, 1)

  };
}

#endif
