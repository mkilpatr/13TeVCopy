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

      void	setIndex(int newIndex)		{ index_ = newIndex;	}
      int	index() 			{ return index_;	}
      void	setCharge(int newCharge)	{ q_ = newCharge;	}
      int	q() 				{ return q_;		}
      void	setSCEta(float newSCEta)	{ scEta_ = newSCEta;	}
      float	scEta() 			{ return scEta_;	}
      void	setR9(float newR9)		{ r9_ = newR9;		}
      float	r9() 				{ return r9_;		}
      void	setD0(float newD0)		{ d0_ = newD0;		}
      float	d0() 				{ return d0_;		}
      void	setDz(float newDz)		{ dz_ = newDz;		}
      float	dz() 				{ return dz_;		}
      void	setPFDBetaIso(float newIso)	{ pfdbetaiso_ = newIso;	}
      float	pfdbetaiso() 			{ return pfdbetaiso_;	}
      void	setMVAIDNonTrig(float newID)	{ mvaidnontrig_ = newID;}
      float	mvaidnontrig() 			{ return mvaidnontrig_;	}
      void	setMVAIDTrig(float newID)	{ mvaidtrig_ = newID;	}
      float	mvaidtrig() 			{ return mvaidtrig_;	}
      void	setPassPOGId(bool flag)		{ passpogid_ = flag;	}
      bool	passpogid() 			{ return passpogid_;	}
      void	setPassPOGIso(bool flag)	{ passpogiso_ = flag;	}
      bool	passpogiso() 			{ return passpogiso_;	}
      void	setIsGoodPOGElectron(bool flag)	{ isgoodpogelectron_ = flag;	}
      bool	isgoodpogelectron() 		{ return isgoodpogelectron_;	}

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

      void	setEcalEnergy(float newVal)		{ ecalE_ = newVal;		}
      float	ecalE() 				{ return ecalE_;		}
      void	setEOverPIn(float newVal)		{ eOverPIn_ = newVal;		}
      float	eOverPIn() 				{ return eOverPIn_;		}
      void	setSigIEtaIEta(float newVal)		{ sigietaieta_ = newVal;	}
      float	sigietaieta() 				{ return sigietaieta_;		}
      void	setSigIPhiIPhi(float newVal)		{ sigiphiiphi_ = newVal;	}
      float	sigiphiiphi() 				{ return sigiphiiphi_;		}
      void	setFull5x5SigIEtaIEta(float newVal)	{ full5x5sigietaieta_ = newVal;	}
      float	full5x5sigietaieta() 			{ return full5x5sigietaieta_;	}
      void	setDEtaIn(float newVal)			{ dEtaIn_ = newVal;		}
      float	dEtaIn() 				{ return dEtaIn_;		}
      void	setDPhiIn(float newVal)			{ dPhiIn_ = newVal;		}
      float	dPhiIn() 				{ return dPhiIn_;		}
      void	setHOverE(float newVal)			{ hOverE_ = newVal;		}
      float	hOverE() 				{ return hOverE_;		}
      void	setFBrem(float newVal)			{ fBrem_ = newVal;		}
      float	fBrem() 				{ return fBrem_;		}
      void	setNExpHitsInner(int newVal)		{ nExpHitsInner_ = newVal;	}
      int	nExpHitsInner() 			{ return nExpHitsInner_;	}
      void	setNLostHitsInner(int newVal)		{ nLostHitsInner_ = newVal;	}
      int	nLostHitsInner() 			{ return nLostHitsInner_;	}
      void	setPassConvVeto(bool newVal)		{ passConvVeto_ = newVal;	}
      bool	passConvVeto() 				{ return passConvVeto_;		}
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
