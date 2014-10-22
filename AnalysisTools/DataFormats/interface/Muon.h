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
      Muon() : index_(-1), d0_(0), dz_(0), pfdbetaiso_(0), isloose_(false), istight_(false), ispf_(false), isglobal_(false), istracker_(false), isstandalone_(false), passpogid_(false), passpogiso_(false), isgoodpogmuon_(false) {}

      template <class InputCoordSystem>
      Muon(ROOT::Math::LorentzVector<InputCoordSystem> inMomentum,
		int inIndex = -1, float inD0 = 0, float inDz = 0,
		float inPfdbetaiso = 0, bool inIsloose = false,
		bool inIsTight = false, bool inIspf = false,
		bool inIsglobal = false, bool inIstracker = false,
		bool inIsstandalone = false) :
		Momentum<CoordSystem>(inMomentum), 
		index_(inIndex), d0_(inD0), dz_(inDz),
		pfdbetaiso_(inPfdbetaiso), isloose_(inIsloose),
		istight_(inIsTight), ispf_(inIspf),
		isglobal_(inIsglobal), istracker_(inIstracker),
		isstandalone_(inIsstandalone), passpogid_(false), passpogiso_(false),
		isgoodpogmuon_(false) {}

      ~Muon() {}

      void	setIndex(int newIndex)			{ index_ = newIndex;		}
      int	index() 				{ return index_;		}
      void	setD0(float newD0)			{ d0_ = newD0;			}
      float	d0() 					{ return d0_;			}
      void	setDz(float newDz)			{ dz_ = newDz;			}
      float	dz() 					{ return dz_;			}
      void	setPFDBetaIso(float newIso)		{ pfdbetaiso_ = newIso;		}
      float	pfdbetaiso() 				{ return pfdbetaiso_;		}
      void	setIsLoose(bool newType)		{ isloose_ = newType;		}
      bool	isloosemuon() 				{ return isloose_;		}
      void	setIsTight(bool newType)		{ istight_ = newType;		}
      bool	istightmuon() 				{ return istight_;		}
      void	setIsPF(bool newType)			{ ispf_ = newType;		}
      bool	ispfmuon() 				{ return ispf_;			}
      void	setIsGlobal(bool newType)		{ isglobal_ = newType;		}
      bool	isglobal() 				{ return isglobal_;		}
      void	setIsTracker(bool newType)		{ istracker_ = newType;		}
      bool	istracker() 				{ return istracker_;		}
      void	setIsStandalone(bool newType)		{ isstandalone_ = newType;	}
      bool	isstandalone() 				{ return isstandalone_;		}
      void	setPassPOGId(bool flag)			{ passpogid_ = flag;		}
      bool	passpogid() 				{ return passpogid_;		}
      void	setPassPOGIso(bool flag)		{ passpogiso_ = flag;		}
      bool	passpogiso() 				{ return passpogiso_;		}
      void	setIsGoodPOGMuon(bool flag)		{ isgoodpogmuon_ = flag;	}
      bool	isgoodpogmuon() 			{ return isgoodpogmuon_;	}

    protected :
      int	index_;  //Index in muon vector
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

      void	setNChi2(float newVal)			{ nChi2_ = newVal;		}
      float	nChi2() 				{ return nChi2_;		}
      void	setNValidHits(int newVal)		{ nValidHits_ = newVal;		}
      int	nValidHits() 				{ return nValidHits_;		}
      void	setNMatch(int newVal)			{ nMatch_ = newVal;		}
      int	nMatch() 				{ return nMatch_;		}
      void	setNPixHits(int newVal)			{ nPixHits_ = newVal;		}
      int	nPixHits() 				{ return nPixHits_;		}
      void	setNTrackLayers(int newVal)		{ nTrackLayers_ = newVal;	}
      int	nTrackLayers() 				{ return nTrackLayers_;		}
      void	setTrackIso(float newVal)		{ trackiso_ = newVal;		}
      float	trackiso() 				{ return trackiso_;		}
      void	setEcalIso(float newVal)		{ ecaliso_ = newVal;		}
      float	ecaliso() 				{ return ecaliso_;		}
      void	setHcalIso(float newVal)		{ hcaliso_ = newVal;		}
      float	hcaliso() 				{ return hcaliso_;		}
      void	setPFChargedIso(float newVal)		{ pfchargediso_ = newVal;	}
      float	pfchargediso() 				{ return pfchargediso_;		}
      void	setPFNeutralIso(float newVal)		{ pfneutraliso_ = newVal;	}
      float	pfneutraliso() 				{ return pfneutraliso_;		}
      void	setPFPhotonIso(float newVal)		{ pfphotoniso_ = newVal;	}
      float	pfphotoniso() 				{ return pfphotoniso_;		}
      void	setPFPUIso(float newVal)		{ pfpuiso_ = newVal;		}
      float	pfpuiso() 				{ return pfpuiso_;		}

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
