//--------------------------------------------------------------------------------------------------
// 
// Lepton
// 
// Parent class for electrons and muons.
// 
// Lepton.h created on Mon Nov 3 21:58:11 CET 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_DATAFORMATS_LEPTON_H
#define ANALYSISTOOLS_DATAFORMATS_LEPTON_H

#include "AnalysisTools/DataFormats/interface/Momentum.h"

namespace ucsbsusy {

  template < class CoordSystem>
  class Lepton : public Momentum<CoordSystem>
  {

    public :
  Lepton() : index_(-1), pdgid_(0), q_(0), d0_(0), dz_(0), sip3d_(0), pfdbetaiso_(0),  miniiso_(0), ptrel_(0), ptratio_(0), annulus_(0), iselectron_(false), ismuon_(false) {}
      template <class InputCoordSystem>
      Lepton(ROOT::Math::LorentzVector<InputCoordSystem> inMomentum, int inIndex = -1) :
		Momentum<InputCoordSystem>(inMomentum), 
		index_(inIndex), pdgid_(0), q_(0), d0_(0), dz_(0), sip3d_(0), pfdbetaiso_(0),  miniiso_(0), ptrel_(0), ptratio_(0), annulus_(0), iselectron_(false), ismuon_(false) {}

      ~Lepton() {}

      int	index() 			    	const { return index_;		}
      int	pdgid() 			    	const { return pdgid_;		}
      int	q() 				      	const { return q_;			}
      float	d0() 				      const	{ return d0_;			}
      float	dz() 				      const	{ return dz_;			}
      float	sip3d() 			    const	{ return sip3d_;		}
      float	pfdbetaiso() 			const	{ return pfdbetaiso_;		}
      float	miniiso() 				const { return miniiso_;		}
      float	ptrel() 				  const { return ptrel_;		}
      float	ptratio() 				const { return ptratio_;		}
      float	annulusactivity() const { return annulus_;		}
      bool	iselectron() 		  const { return iselectron_;		}
      bool	ismuon() 				  const { return ismuon_;		}

      void	setIndex(int newIndex)			{ index_ = newIndex;		}
      void	setPdgId(int newPdgId)			{ pdgid_ = newPdgId;		}
      void	setCharge(int newCharge)		{ q_ = newCharge;		}
      void	setD0(float newD0)			{ d0_ = newD0;			}
      void	setDz(float newDz)			{ dz_ = newDz;			}
      void	setSip3d(float newSip3d)		{ sip3d_ = newSip3d;		}
      void	setPFDBetaIso(float newIso0)		{ pfdbetaiso_ = newIso0;	}
      void  setPtRel(float ptRel)     { ptrel_ = ptRel;         }
      void  setPtRatio(float ptRatio)         { ptratio_ = ptRatio;   }
      void  setAnnulusActivity(float newAnnulus)    { annulus_ = newAnnulus;  }
      void	setIsElectron(bool flag)		{ iselectron_ = flag;		}
      void	setIsMuon(bool flag)			{ ismuon_ = flag;		}
      void	setMiniIso(float newIso2)        	{ miniiso_ = newIso2;		}


    protected :
      int	index_;  //Index in lepton vector
      int	pdgid_;
      int	q_;
      float	d0_;
      float	dz_;
      float	sip3d_;
      float	pfdbetaiso_;
      float     miniiso_;
      float     ptrel_; 
      float     ptratio_ ;
      float     annulus_;
      bool	iselectron_;
      bool	ismuon_;

  };

  typedef Lepton<CylLorentzCoordF>	LeptonF;
  typedef std::vector<LeptonF>		LeptonFCollection;

}

#endif
