//--------------------------------------------------------------------------------------------------
// 
// Tau
// 
// Class for storing specific hadronic tau object information.
// 
// Tau.h created on Wed Oct 22 22:14:31 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_DATAFORMATS_TAU_H
#define ANALYSISTOOLS_DATAFORMATS_TAU_H

#include "AnalysisTools/DataFormats/interface/Momentum.h"

namespace ucsbsusy {

  template <class CoordSystem>
  class Tau : public Momentum<CoordSystem>
  {

    public :
      Tau() : index_(-1), leadCand_(0), leadChargedCand_(0), q_(0), dxy_(0), dxyerr_(0), dxysig_(0), hpsid_(0), passpogid_(false), passpogiso_(false), isgoodpogtau_(false) {}

      template <class InputCoordSystem>
      Tau(ROOT::Math::LorentzVector<InputCoordSystem> inMomentum,
		int inIndex = -1, 
		Momentum<CoordSystem>* inLeadcand = 0,
		Momentum<CoordSystem>* inLeadchcand = 0,
		int inCharge = 0, float inDxy = 0,
		float inDxyerr = 0, float inDxysig = 0,
		unsigned long inHpsid = 0) :
		Momentum<CoordSystem>(inMomentum),
		index_(inIndex), 
		leadCand_(inLeadcand), leadChargedCand_(inLeadchcand),
		q_(inCharge), dxy_(inDxy), dxyerr_(inDxyerr),
		dxysig_(inDxysig), hpsid_(inHpsid), passpogid_(false),
		passpogiso_(false), isgoodpogtau_(false) {}

      ~Tau() {}

      void	setIndex(int newIndex)		{ index_ = newIndex;	}
      int	index() 			{ return index_;	}
      void	setCharge(int newCharge)	{ q_ = newCharge;	}
      int	q() 				{ return q_;		}
      void	setDxy(float newDxy)		{ dxy_ = newDxy;	}
      float	dxy() 				{ return dxy_;		}
      void	setDxyErr(float newDxyErr)	{ dxyerr_ = newDxyErr;	}
      float	dxyerr() 			{ return dxyerr_;	}
      void	setDxySig(float newDxySig)	{ dxysig_ = newDxySig;	}
      float	dxysig() 			{ return dxysig_;	}
      void	setHPSId(unsigned long newHPSId){ hpsid_ = newHPSId;	}
      unsigned long	hpsid() 		{ return hpsid_;	}
      void	setPassPOGId(bool flag)		{ passpogid_ = flag;	}
      bool	passpogid() 			{ return passpogid_;	}
      void	setPassPOGIso(bool flag)	{ passpogiso_ = flag;	}
      bool	passpogiso() 			{ return passpogiso_;	}
      void	setIsGoodPOGTau(bool flag)	{ isgoodpogtau_ = flag;	}
      bool	isgoodpogtau() 			{ return isgoodpogtau_;	}
      const Momentum<CoordSystem>&	leadCandidate()	  	  const	{ return *leadCand_;		}
      const Momentum<CoordSystem>&	leadChargedCandidate()    const	{ return *leadChargedCand_;	}

    protected :
      int	index_;  //Index in tau vector
      Momentum<CoordSystem>*	leadCand_;
      Momentum<CoordSystem>*	leadChargedCand_;
      int	q_;
      float	dxy_;
      float	dxyerr_;
      float	dxysig_;
      unsigned long hpsid_;
      bool	passpogid_;
      bool	passpogiso_;
      bool	isgoodpogtau_;

  };

  typedef Tau<CylLorentzCoordF>	TauF;
  typedef std::vector<TauF>	TauFCollection;

  class ExtendedTau : public TauF
  {
    public :
      ExtendedTau() : isodb3hitsraw_(0), isomvanoltraw_(0), isomvaltraw_(0), antielemvaraw_(0), antielemvacat_(0), antimumvaraw_(0), antimumvacat_(0) {}

      ~ExtendedTau() {}

      void	setIsoDB3HitsRaw(float newVal)	{ isodb3hitsraw_ = newVal;	}
      float	isodb3hitsraw() 		{ return isodb3hitsraw_;	}
      void	setIsoMVANoLTRaw(float newVal)	{ isomvanoltraw_ = newVal;	}
      float	isomvanoltraw() 		{ return isomvanoltraw_;	}
      void	setIsoMVALTRaw(float newVal)	{ isomvaltraw_ = newVal;	}
      float	isomvaltraw() 			{ return isomvaltraw_;		}
      void	setAntiEleMVARaw(float newVal)	{ antielemvaraw_ = newVal;	}
      float	antielemvaraw() 		{ return antielemvaraw_;	}
      void	setAntiEleMVACat(int newVal)	{ antielemvacat_ = newVal;	}
      int	antielemvacat() 		{ return antielemvacat_;	}
      void	setAntiMuMVARaw(float newVal)	{ antimumvaraw_ = newVal;	}
      float	antimumvaraw() 			{ return antimumvaraw_;		}
      void	setAntiMuMVACat(int newVal)	{ antimumvacat_ = newVal;	}
      int	antimumvacat() 			{ return antimumvacat_;		}

    protected :
      float	isodb3hitsraw_;
      float	isomvanoltraw_;
      float	isomvaltraw_;
      float	antielemvaraw_;
      int	antielemvacat_;
      float	antimumvaraw_;
      int	antimumvacat_;

  };

  typedef std::vector<ExtendedTau>	ExtendedTauCollection;

}

#endif
