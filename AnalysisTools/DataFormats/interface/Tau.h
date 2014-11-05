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
      Tau() : index_(-1), leadCand_(), leadChargedCand_(), q_(0), dxy_(0), dxyerr_(0), dxysig_(0), hpsid_(0), passpogid_(false), passpogiso_(false), isgoodpogtau_(false) {}

      template <class InputCoordSystem>
      Tau(ROOT::Math::LorentzVector<InputCoordSystem> inMomentum,
		int inIndex = -1, 
		Momentum<CoordSystem> inLeadcand = Momentum<CoordSystem>(),
		Momentum<CoordSystem> inLeadchcand = Momentum<CoordSystem>(),
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

      int	index() 			    const { return index_;	}
      int	q() 				      const { return q_;		}
      float	dxy() 				  const { return dxy_;		}
      float	dxyerr() 			  const { return dxyerr_;	}
      float	dxysig() 			  const { return dxysig_;	}
      unsigned long	hpsid() const { return hpsid_;	}
      bool	passpogid() 		const { return passpogid_;	}
      bool	passpogiso() 		const { return passpogiso_;	}
      bool	isgoodpogtau() 	const { return isgoodpogtau_;	}
      const Momentum<CoordSystem>&	leadCandidate()	  	  const	{ return leadCand_;		}
      void setLeadCandidate(const Momentum<CoordSystem>& inMom )      { leadCand_ = inMom;    }
      const Momentum<CoordSystem>&	leadChargedCandidate()    const	{ return leadChargedCand_;	}
      void setLeadChargedCandidate(const Momentum<CoordSystem>& inMom)    { leadChargedCand_ = inMom; }

      void	setIndex(int newIndex)		{ index_ = newIndex;	}
      void	setCharge(int newCharge)	{ q_ = newCharge;	}
      void	setDxy(float newDxy)		{ dxy_ = newDxy;	}
      void	setDxyErr(float newDxyErr)	{ dxyerr_ = newDxyErr;	}
      void	setDxySig(float newDxySig)	{ dxysig_ = newDxySig;	}
      void	setHPSId(unsigned long newHPSId){ hpsid_ = newHPSId;	}
      void	setPassPOGId(bool flag)		{ passpogid_ = flag;	}
      void	setPassPOGIso(bool flag)	{ passpogiso_ = flag;	}
      void	setIsGoodPOGTau(bool flag)	{ isgoodpogtau_ = flag;	}

    protected :
      int	index_;  //Index in tau vector
      Momentum<CoordSystem>	leadCand_;
      Momentum<CoordSystem>	leadChargedCand_;
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

      float	isodb3hitsraw() 		{ return isodb3hitsraw_;	}
      float	isomvanoltraw() 		{ return isomvanoltraw_;	}
      float	isomvaltraw() 			{ return isomvaltraw_;		}
      float	antielemvaraw() 		{ return antielemvaraw_;	}
      int	antielemvacat() 		{ return antielemvacat_;	}
      float	antimumvaraw() 			{ return antimumvaraw_;		}
      int	antimumvacat() 			{ return antimumvacat_;		}

      void	setIsoDB3HitsRaw(float newVal)	{ isodb3hitsraw_ = newVal;	}
      void	setIsoMVANoLTRaw(float newVal)	{ isomvanoltraw_ = newVal;	}
      void	setIsoMVALTRaw(float newVal)	{ isomvaltraw_ = newVal;	}
      void	setAntiEleMVARaw(float newVal)	{ antielemvaraw_ = newVal;	}
      void	setAntiEleMVACat(int newVal)	{ antielemvacat_ = newVal;	}
      void	setAntiMuMVARaw(float newVal)	{ antimumvaraw_ = newVal;	}
      void	setAntiMuMVACat(int newVal)	{ antimumvacat_ = newVal;	}

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
