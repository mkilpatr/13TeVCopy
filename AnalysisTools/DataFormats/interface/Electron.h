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

#include "AnalysisTools/DataFormats/interface/Momentum.h"

namespace ucsbsusy {

  template < class CoordSystem>
  class Electron : public Momentum<CoordSystem>
  {

    public :
      Electron() : index_(-1), q_(0), scEta_(0), r9_(0), d0_(0), dz_(0), pfdbetaiso_(0), mvaidnontrig_(0), mvaidtrig_(0), passpogid_(false), passpogiso_(false), isgoodpogelectron_(false) {}

      template <class InputCoordSystem>
      Electron(ROOT::Math::LorentzVector<InputCoordSystem> inMomentum,
		int inIndex = -1, int inCharge = 0, float inSCEta = 0, float inR9 = 0, 
		float inD0 = 0, float inDz = 0, float inPfdbetaiso = 0, 
		float inMvaidnontrig = 0, float inMvaidtrig = 0) : 
		Momentum<CoordSystem>(inMomentum), 
		index_(inIndex), q_(inCharge), scEta_(inSCEta), r9_(inR9), 
		d0_(inD0), dz_(inDz), pfdbetaiso_(inPfdbetaiso), 
		mvaidnontrig_(inMvaidnontrig), mvaidtrig_(inMvaidtrig),
		passpogid_(false), passpogiso_(false), isgoodpogelectron_(false) {}

      ~Electron() {}

      int	index() 			{ return index_;	}
      int	q() 				{ return q_;		}
      float	scEta() 			{ return scEta_;	}
      float	r9() 				{ return r9_;		}
      float	d0() 				{ return d0_;		}
      float	dz() 				{ return dz_;		}
      float	pfdbetaiso() 			{ return pfdbetaiso_;	}
      float	mvaidnontrig() 			{ return mvaidnontrig_;	}
      float	mvaidtrig() 			{ return mvaidtrig_;	}
      bool	passpogid() 			{ return passpogid_;	}
      bool	passpogiso() 			{ return passpogiso_;	}
      bool	isgoodpogelectron() 		{ return isgoodpogelectron_;	}

      void	setIndex(int newIndex)		{ index_ = newIndex;	}
      void	setCharge(int newCharge)	{ q_ = newCharge;	}
      void	setSCEta(float newSCEta)	{ scEta_ = newSCEta;	}
      void	setR9(float newR9)		{ r9_ = newR9;		}
      void	setD0(float newD0)		{ d0_ = newD0;		}
      void	setDz(float newDz)		{ dz_ = newDz;		}
      void	setPFDBetaIso(float newIso)	{ pfdbetaiso_ = newIso;	}
      void	setMVAIDNonTrig(float newID)	{ mvaidnontrig_ = newID;}
      void	setMVAIDTrig(float newID)	{ mvaidtrig_ = newID;	}
      void	setPassPOGId(bool flag)		{ passpogid_ = flag;	}
      void	setPassPOGIso(bool flag)	{ passpogiso_ = flag;	}
      void	setIsGoodPOGElectron(bool flag)	{ isgoodpogelectron_ = flag;	}

    protected :
      int	index_;  //Index in electron vector
      int	q_;
      float	scEta_;
      float	r9_;
      float	d0_;
      float	dz_;
      float	pfdbetaiso_;
      float	mvaidnontrig_;
      float	mvaidtrig_;
      bool	passpogid_;
      bool	passpogiso_;
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
