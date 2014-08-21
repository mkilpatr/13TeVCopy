//--------------------------------------------------------------------------------------------------
// 
// EventInfo
// 
// Class to hold some event specific information.
// 
// EventInfo.h created on Mon Aug 11 11:22:00 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_DATAFORMATS_EVENTINFO_H
#define ANALYSISTOOLS_DATAFORMATS_EVENTINFO_H

#include <TObject.h>

namespace ucsbsusy {

  class EventInfo : public TObject
  {

    public :
      EventInfo();
      EventInfo(UInt_t run, UInt_t lumi, UInt_t evt);
      ~EventInfo(){}

      UInt_t	runNum()	const	{ return fRunNum;		} 
      UInt_t	lumiSec()	const	{ return fLumiSec;		} 
      UInt_t	evtNum()	const	{ return fEvtNum;		}
      Bool_t	isMC()		const	{ return fIsMC;			}
      UInt_t	nPV()		const	{ return fNPV;			}
      Double_t	xPV()		const	{ return fPVx;			}
      Double_t	yPV()		const	{ return fPVy;			}
      Double_t	zPV()		const	{ return fPVz;			}
      Double_t	met()		const	{ return fMET;			}
      Double_t	metPhi()	const	{ return fMETPhi;		}
      Double_t	genMet()	const	{ return fGenMET;		}
      Double_t	metUp()		const	{ return fMETUp;		}
      Double_t	metDown()	const	{ return fMETDown;		}

      void	setRunNum(UInt_t r)	{ fRunNum = r;			} 
      void	setLumiSec(UInt_t l)	{ fLumiSec = l;			} 
      void	setEvtNum(UInt_t e)	{ fEvtNum = e;			} 
      void	setIsMC(Bool_t b)	{ fIsMC = b;			} 
      void	setNPV(UInt_t n)	{ fNPV = n;			} 
      void	setPVx(Double_t x)	{ fPVx = x;			} 
      void	setPVy(Double_t y)	{ fPVy = y;			} 
      void	setPVz(Double_t z)	{ fPVz = z;			} 
      void	setMET(Double_t m)	{ fMET = m;			} 
      void	setMETPhi(Double_t m)	{ fMETPhi = m;			} 
      void	setMETSumET(Double_t m)	{ fMETSumET = m;		} 
      void	setGenMET(Double_t m)	{ fGenMET = m;			} 
      void	setMETUp(Double_t m)	{ fMETUp = m;			} 
      void	setMETDown(Double_t m)	{ fMETDown = m;			} 

    protected :
      UInt_t	fRunNum;
      UInt_t	fLumiSec;
      UInt_t	fEvtNum;
      Bool_t	fIsMC;
      UInt_t	fNPV;
      Double_t	fPVx;
      Double_t	fPVy;
      Double_t	fPVz;
      Double_t	fMET;
      Double_t	fMETPhi;
      Double_t	fMETSumET;
      Double_t	fGenMET;
      Double_t	fMETUp;
      Double_t	fMETDown;

    ClassDef(EventInfo, 1)

  };
}

#endif
