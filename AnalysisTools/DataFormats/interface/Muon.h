//--------------------------------------------------------------------------------------------------
// 
// Muon
// 
// Class for storing specific muon object information.
// 
// Muon.h created on Wed Oct 22 00:19:22 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_DATAFORMATS_MUON_H
#define ANALYSISTOOLS_DATAFORMATS_MUON_H

#include <vector>
#include <iostream>

#include "AnalysisTools/DataFormats/interface/Momentum.h"

namespace ucsbsusy {

  template < class CoordSystem>
  class Muon : public Momentum<CoordSystem>
  {

    public :
      Muon() : index_(-1), q_(0), d0_(0), dz_(0), pfdbetaiso_(0), isloose_(false), istight_(false), ispf_(false), isglobal_(false), istracker_(false), isstandalone_(false), passpogid_(false), passpogiso_(false), isgoodpogmuon_(false) {}

      template <class InputCoordSystem>
      Muon(ROOT::Math::LorentzVector<InputCoordSystem> inMomentum,
		int inIndex = -1, int inCharge = 0, float inD0 = 0, float inDz = 0,
		float inPfdbetaiso = 0, bool inIsloose = false,
		bool inIsTight = false, bool inIspf = false,
		bool inIsglobal = false, bool inIstracker = false,
		bool inIsstandalone = false) :
		Momentum<CoordSystem>(inMomentum), 
		index_(inIndex), q_(inCharge), d0_(inD0), dz_(inDz),
		pfdbetaiso_(inPfdbetaiso), isloose_(inIsloose),
		istight_(inIsTight), ispf_(inIspf),
		isglobal_(inIsglobal), istracker_(inIstracker),
		isstandalone_(inIsstandalone), passpogid_(false), passpogiso_(false),
		isgoodpogmuon_(false) {}

      ~Muon() {}

      int	index() 				{ return index_;		}
      int	q() 					{ return q_;			}
      float	d0() 					{ return d0_;			}
      float	dz() 					{ return dz_;			}
      float	pfdbetaiso() 				{ return pfdbetaiso_;		}
      bool	isloosemuon() 				{ return isloose_;		}
      bool	istightmuon() 				{ return istight_;		}
      bool	ispfmuon() 				{ return ispf_;			}
      bool	isglobal() 				{ return isglobal_;		}
      bool	istracker() 				{ return istracker_;		}
      bool	isstandalone() 				{ return isstandalone_;		}
      bool	passpogid() 				{ return passpogid_;		}
      bool	passpogiso() 				{ return passpogiso_;		}
      bool	isgoodpogmuon() 			{ return isgoodpogmuon_;	}

      void	setIndex(int newIndex)			{ index_ = newIndex;		}
      void	setCharge(int newCharge)		{ q_ = newCharge;		}
      void	setD0(float newD0)			{ d0_ = newD0;			}
      void	setDz(float newDz)			{ dz_ = newDz;			}
      void	setPFDBetaIso(float newIso)		{ pfdbetaiso_ = newIso;		}
      void	setIsLoose(bool newType)		{ isloose_ = newType;		}
      void	setIsTight(bool newType)		{ istight_ = newType;		}
      void	setIsPF(bool newType)			{ ispf_ = newType;		}
      void	setIsGlobal(bool newType)		{ isglobal_ = newType;		}
      void	setIsTracker(bool newType)		{ istracker_ = newType;		}
      void	setIsStandalone(bool newType)		{ isstandalone_ = newType;	}
      void	setPassPOGId(bool flag)			{ passpogid_ = flag;		}
      void	setPassPOGIso(bool flag)		{ passpogiso_ = flag;		}
      void	setIsGoodPOGMuon(bool flag)		{ isgoodpogmuon_ = flag;	}

    protected :
      int	index_;  //Index in muon vector
      int	q_;
      float	d0_;
      float	dz_;
      float	pfdbetaiso_;
      bool	isloose_;
      bool	istight_;
      bool	ispf_;
      bool	isglobal_;
      bool	istracker_;
      bool	isstandalone_;
      bool	passpogid_;
      bool	passpogiso_;
      bool	isgoodpogmuon_;

  };

  typedef Muon<CylLorentzCoordF>	MuonF;
  typedef std::vector<MuonF>		MuonFCollection;

  class ExtendedMuon : public MuonF
  {
    public :
      ExtendedMuon() : nChi2_(0), nValidHits_(0), nMatch_(0), nPixHits_(0), nTrackLayers_(0), trackiso_(0), ecaliso_(0), hcaliso_(0), pfchargediso_(0), pfneutraliso_(0), pfphotoniso_(0), pfpuiso_(0) {}

      ~ExtendedMuon() {}

      float	nChi2() 				{ return nChi2_;		}
      int	nValidHits() 				{ return nValidHits_;		}
      int	nMatch() 				{ return nMatch_;		}
      int	nPixHits() 				{ return nPixHits_;		}
      int	nTrackLayers() 				{ return nTrackLayers_;		}
      float	trackiso() 				{ return trackiso_;		}
      float	ecaliso() 				{ return ecaliso_;		}
      float	hcaliso() 				{ return hcaliso_;		}
      float	pfchargediso() 				{ return pfchargediso_;		}
      float	pfneutraliso() 				{ return pfneutraliso_;		}
      float	pfphotoniso() 				{ return pfphotoniso_;		}
      float	pfpuiso() 				{ return pfpuiso_;		}

      void	setNChi2(float newVal)			{ nChi2_ = newVal;		}
      void	setNValidHits(int newVal)		{ nValidHits_ = newVal;		}
      void	setNMatch(int newVal)			{ nMatch_ = newVal;		}
      void	setNPixHits(int newVal)			{ nPixHits_ = newVal;		}
      void	setNTrackLayers(int newVal)		{ nTrackLayers_ = newVal;	}
      void	setTrackIso(float newVal)		{ trackiso_ = newVal;		}
      void	setEcalIso(float newVal)		{ ecaliso_ = newVal;		}
      void	setHcalIso(float newVal)		{ hcaliso_ = newVal;		}
      void	setPFChargedIso(float newVal)		{ pfchargediso_ = newVal;	}
      void	setPFNeutralIso(float newVal)		{ pfneutraliso_ = newVal;	}
      void	setPFPhotonIso(float newVal)		{ pfphotoniso_ = newVal;	}
      void	setPFPUIso(float newVal)		{ pfpuiso_ = newVal;		}

    protected :
      float	nChi2_;
      int	nValidHits_;
      int	nMatch_;
      int	nPixHits_;
      int	nTrackLayers_;
      float	trackiso_;
      float	ecaliso_;
      float	hcaliso_;
      float	pfchargediso_;
      float	pfneutraliso_;
      float	pfphotoniso_;
      float	pfpuiso_;

  };

  typedef std::vector<ExtendedMuon>	ExtendedMuonCollection;

}

#endif
