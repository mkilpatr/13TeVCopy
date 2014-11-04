//--------------------------------------------------------------------------------------------------
// 
// Electron
// 
// Class for storing specific electron object information.
// 
// Electron.h created on Wed Oct 22 00:19:22 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_DATAFORMATS_ELECTRON_H
#define ANALYSISTOOLS_DATAFORMATS_ELECTRON_H

#include <vector>
#include <iostream>

#include "AnalysisTools/DataFormats/interface/Lepton.h"

namespace ucsbsusy {

  template < class CoordSystem>
  class Electron : public Lepton<CoordSystem>
  {

    public :
      Electron() : scEta_(0), r9_(0), mvaidnontrig_(0), mvaidtrig_(0), isveto_(false), isloose_(false), ismedium_(false), istight_(false), isgoodpogelectron_(false) {}

      template <class InputCoordSystem>
      Electron(ROOT::Math::LorentzVector<InputCoordSystem> inMomentum,
		int inIndex = -1, int inPdgid = 11, int inCharge = 0,
		float inD0 = 0, float inDz = 0, float inPfdbetaiso = 0, 
		float inSCEta = 0, float inR9 = 0, 
		float inMvaidnontrig = 0, float inMvaidtrig = 0,
		bool inIsveto = false, bool inIsloose = false,
		bool inIsmedium = false, bool inIstight = false) : 
		Lepton<InputCoordSystem>(inMomentum, inIndex, inPdgid,
		inCharge, inD0, inDz, inPfdbetaiso),
		scEta_(inSCEta), r9_(inR9),
		mvaidnontrig_(inMvaidnontrig), mvaidtrig_(inMvaidtrig),
		isveto_(inIsveto), isloose_(inIsloose),
		ismedium_(inIsmedium), istight_(inIstight),
		isgoodpogelectron_(false) {}

      ~Electron() {}

      float	scEta() 			{ return scEta_;	}
      float	r9() 				{ return r9_;		}
      float	mvaidnontrig() 			{ return mvaidnontrig_;	}
      float	mvaidtrig() 			{ return mvaidtrig_;	}
      bool	isvetoelectron() 		{ return isveto_;	}
      bool	islooseelectron() 		{ return isloose_;	}
      bool	ismediumelectron() 		{ return ismedium_;	}
      bool	istightelectron() 		{ return istight_;	}
      bool	isgoodpogelectron() 		{ return isgoodpogelectron_;	}

      void	setSCEta(float newSCEta)	{ scEta_ = newSCEta;	}
      void	setR9(float newR9)		{ r9_ = newR9;		}
      void	setMVAIDNonTrig(float newID)	{ mvaidnontrig_ = newID;}
      void	setMVAIDTrig(float newID)	{ mvaidtrig_ = newID;	}
      void	setIsVeto(bool newType)		{ isveto_ = newType;	}
      void	setIsLoose(bool newType)	{ isloose_ = newType;	}
      void	setIsMedium(bool newType)	{ ismedium_ = newType;	}
      void	setIsTight(bool newType)	{ istight_ = newType;	}
      void	setIsGoodPOGElectron(bool flag)	{ isgoodpogelectron_ = flag;	}

    protected :
      float	scEta_;
      float	r9_;
      float	mvaidnontrig_;
      float	mvaidtrig_;
      bool	isveto_;
      bool	isloose_;
      bool	ismedium_;
      bool	istight_;
      bool	isgoodpogelectron_;

  };

  typedef Electron<CylLorentzCoordF>	ElectronF;
  typedef std::vector<ElectronF>	ElectronFCollection;

  class ExtendedElectron : public ElectronF
  {
    public :
      ExtendedElectron() : ecalE_(0), eOverPIn_(0), sigietaieta_(0), sigiphiiphi_(0), full5x5sigietaieta_(0), dEtaIn_(0), dPhiIn_(0), hOverE_(0), fBrem_(0), nExpHitsInner_(0), nLostHitsInner_(0), passConvVeto_(0), trackiso_(0), ecaliso_(0), hcaliso_(0), pfchargediso_(0), pfneutraliso_(0), pfphotoniso_(0), pfpuiso_(0) {}

      ~ExtendedElectron() {}

      float	ecalE() 				{ return ecalE_;		}
      float	eOverPIn() 				{ return eOverPIn_;		}
      float	sigietaieta() 				{ return sigietaieta_;		}
      float	sigiphiiphi() 				{ return sigiphiiphi_;		}
      float	full5x5sigietaieta() 			{ return full5x5sigietaieta_;	}
      float	dEtaIn() 				{ return dEtaIn_;		}
      float	dPhiIn() 				{ return dPhiIn_;		}
      float	hOverE() 				{ return hOverE_;		}
      float	fBrem() 				{ return fBrem_;		}
      int	nExpHitsInner() 			{ return nExpHitsInner_;	}
      int	nLostHitsInner() 			{ return nLostHitsInner_;	}
      bool	passConvVeto() 				{ return passConvVeto_;		}
      float	trackiso() 				{ return trackiso_;		}
      float	ecaliso() 				{ return ecaliso_;		}
      float	hcaliso() 				{ return hcaliso_;		}
      float	pfchargediso() 				{ return pfchargediso_;		}
      float	pfneutraliso() 				{ return pfneutraliso_;		}
      float	pfphotoniso() 				{ return pfphotoniso_;		}
      float	pfpuiso() 				{ return pfpuiso_;		}

      void	setEcalEnergy(float newVal)		{ ecalE_ = newVal;		}
      void	setEOverPIn(float newVal)		{ eOverPIn_ = newVal;		}
      void	setSigIEtaIEta(float newVal)		{ sigietaieta_ = newVal;	}
      void	setSigIPhiIPhi(float newVal)		{ sigiphiiphi_ = newVal;	}
      void	setFull5x5SigIEtaIEta(float newVal)	{ full5x5sigietaieta_ = newVal;	}
      void	setDEtaIn(float newVal)			{ dEtaIn_ = newVal;		}
      void	setDPhiIn(float newVal)			{ dPhiIn_ = newVal;		}
      void	setHOverE(float newVal)			{ hOverE_ = newVal;		}
      void	setFBrem(float newVal)			{ fBrem_ = newVal;		}
      void	setNExpHitsInner(int newVal)		{ nExpHitsInner_ = newVal;	}
      void	setNLostHitsInner(int newVal)		{ nLostHitsInner_ = newVal;	}
      void	setPassConvVeto(bool newVal)		{ passConvVeto_ = newVal;	}
      void	setTrackIso(float newVal)		{ trackiso_ = newVal;		}
      void	setEcalIso(float newVal)		{ ecaliso_ = newVal;		}
      void	setHcalIso(float newVal)		{ hcaliso_ = newVal;		}
      void	setPFChargedIso(float newVal)		{ pfchargediso_ = newVal;	}
      void	setPFNeutralIso(float newVal)		{ pfneutraliso_ = newVal;	}
      void	setPFPhotonIso(float newVal)		{ pfphotoniso_ = newVal;	}
      void	setPFPUIso(float newVal)		{ pfpuiso_ = newVal;		}

    protected :
      float	ecalE_;
      float	eOverPIn_;
      float	sigietaieta_;
      float	sigiphiiphi_;
      float	full5x5sigietaieta_;
      float	dEtaIn_;
      float	dPhiIn_;
      float	hOverE_;
      float	fBrem_;
      int	nExpHitsInner_;
      int	nLostHitsInner_;
      bool	passConvVeto_;
      float	trackiso_;
      float	ecaliso_;
      float	hcaliso_;
      float	pfchargediso_;
      float	pfneutraliso_;
      float	pfphotoniso_;
      float	pfpuiso_;

  };

  typedef std::vector<ExtendedElectron>	ExtendedElectronCollection;

}

#endif
