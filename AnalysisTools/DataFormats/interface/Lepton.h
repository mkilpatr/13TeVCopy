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
      Lepton() : index_(-1), pdgid_(0), q_(0), d0_(0), dz_(0), pfdbetaiso_(0), passpogid_(false), passpogiso_(false), isgoodpoglepton_(false) {}

      template <class InputCoordSystem>
      Lepton(ROOT::Math::LorentzVector<InputCoordSystem> inMomentum,
		int inIndex = -1, int inPdgid = 0, int inCharge = 0,
		float inD0 = 0, float inDz = 0, float inPfdbetaiso = 0) :
		Momentum<InputCoordSystem>(inMomentum), 
		index_(inIndex), pdgid_(inPdgid), q_(inCharge),
		d0_(inD0), dz_(inDz), pfdbetaiso_(inPfdbetaiso),
		passpogid_(false), passpogiso_(false),
		isgoodpoglepton_(false) {}

      ~Lepton() {}

      int	index() 				{ return index_;		}
      int	pdgid() 				{ return pdgid_;		}
      int	q() 					{ return q_;			}
      float	d0() 					{ return d0_;			}
      float	dz() 					{ return dz_;			}
      float	pfdbetaiso() 				{ return pfdbetaiso_;		}
      bool	passpogid() 				{ return passpogid_;		}
      bool	passpogiso() 				{ return passpogiso_;		}
      bool	isgoodpoglepton() 			{ return isgoodpoglepton_;	}

      void	setIndex(int newIndex)			{ index_ = newIndex;		}
      void	setPdgId(int newPdgId)			{ pdgid_ = newPdgId;		}
      void	setCharge(int newCharge)		{ q_ = newCharge;		}
      void	setD0(float newD0)			{ d0_ = newD0;			}
      void	setDz(float newDz)			{ dz_ = newDz;			}
      void	setPFDBetaIso(float newIso)		{ pfdbetaiso_ = newIso;		}
      void	setPassPOGId(bool flag)			{ passpogid_ = flag;		}
      void	setPassPOGIso(bool flag)		{ passpogiso_ = flag;		}
      void	setIsGoodPOGLepton(bool flag)		{ isgoodpoglepton_ = flag;	}

    protected :
      int	index_;  //Index in lepton vector
      int	pdgid_;
      int	q_;
      float	d0_;
      float	dz_;
      float	pfdbetaiso_;
      bool	passpogid_;
      bool	passpogiso_;
      bool	isgoodpoglepton_;

  };

  typedef Lepton<CylLorentzCoordF>	LeptonF;
  typedef std::vector<LeptonF>		LeptonFCollection;

}

#endif