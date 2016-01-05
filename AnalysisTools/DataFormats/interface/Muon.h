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

#include "AnalysisTools/DataFormats/interface/Lepton.h"

namespace ucsbsusy {

  template < class CoordSystem>
  class Muon : public Lepton<CoordSystem>
  {

    public :
	Muon() : isloose_(false), ismedium_(false), istight_(false), ispf_(false), isglobal_(false), istracker_(false), isstandalone_(false) {}

      template <class InputCoordSystem>
	Muon(ROOT::Math::LorentzVector<InputCoordSystem> inMomentum, int inIndex = -1) :
    Lepton<InputCoordSystem>(inMomentum, inIndex),
			     isloose_(false), ismedium_(false), istight_(false), ispf_(false), isglobal_(false), istracker_(false), isstandalone_(false) {setPdgId(13);setIsMuon(true);}
    
    ~Muon() {}

      bool	isloosemuon() 	       const { return isloose_;	            }
      bool	ismediummuon() 	       const { return ismedium_;	    }
      bool	istightmuon() 	       const { return istight_;	            }
      bool	ispfmuon()             const { return ispf_;		    }
      bool	isglobal() 	       const { return isglobal_;	    }
      bool	istracker()            const { return istracker_;	    }
      bool	isstandalone() 	       const { return isstandalone_;	    }

      void	setIsLoose(bool newType)	   { isloose_ = newType;		}
      void	setIsMedium(bool newType)	   { ismedium_ = newType;		}
      void	setIsTight(bool newType)	   { istight_ = newType;		}
      void	setIsPF(bool newType)		   { ispf_ = newType;		}
      void	setIsGlobal(bool newType)	   { isglobal_ = newType;		}
      void	setIsTracker(bool newType)	   { istracker_ = newType;		}
      void	setIsStandalone(bool newType)	   { isstandalone_ = newType;	}

    protected :
      bool	isloose_;
      bool	ismedium_;
      bool	istight_;
      bool	ispf_;
      bool	isglobal_;
      bool	istracker_;
      bool	isstandalone_;

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
