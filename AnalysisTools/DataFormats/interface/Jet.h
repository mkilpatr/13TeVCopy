//--------------------------------------------------------------------------------------------------
// 
// Jet
// 
// Class to hold basic jet information. To be enhanced as needed.
// 
// Jet.h created on Tue Aug 19 16:26:39 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_DATAFORMATS_JET_H
#define ANALYSISTOOLS_DATAFORMATS_JET_H

#include <vector>
#include "AnalysisTools/DataFormats/interface/Particle.h"

namespace ucsbsusy {                                                                                                   

  class Jet;
  typedef std::vector<Jet> JetCollection;

  class Jet : public Particle
  {

    public :
      Jet() :
        fJECUncorrection(0), fCSVBTag(-99), fMatchedMCFlavor(-99), fJetArea(-99), fPUJetId(-99), fNChargedHadron(0), fNNeutralHadron(0), fNElectron(0), fNMuon(0), fNPhoton(0), fNeutralHadFraction(-1), fChargedHadFraction(-1), fNeutralEMFraction(-1), fChargedEMFraction(-1) {}
      ~Jet(){}

      Float_t		ptRaw()			const	{ return pt()*fJECUncorrection;		}
      Float_t		csv()			const	{ return fCSVBTag;		}
      Int_t		matchedMCFlavor()	const	{ return fMatchedMCFlavor;	}
      Float_t		jetArea()		const	{ return fJetArea;		}
      Float_t		puId()			const	{ return fPUJetId;		}
      UInt_t		nChargedHadron()	const	{ return fNChargedHadron;	}
      UInt_t		nNeutralHadron()	const	{ return fNNeutralHadron;	}
      UInt_t		nElectron()		const	{ return fNElectron;		}
      UInt_t		nMuon()			const	{ return fNMuon;		}
      UInt_t		nPhoton()		const	{ return fNPhoton;		}
      Float_t		neutralHadFrac()	const	{ return fNeutralHadFraction;	}
      Float_t		chargedHadFrac()	const	{ return fChargedHadFraction;	}
      Float_t		neutralEMFrac()		const	{ return fNeutralEMFraction;	}
      Float_t		chargedEMFrac()		const	{ return fChargedEMFraction;	}

      void		setJECUncorrection(Float_t r)		{ fJECUncorrection = r;			}
      void		setCSV(Float_t c)		{ fCSVBTag = c;			}
      void		setMatchedMCFlavor(Float_t f)	{ fMatchedMCFlavor = f;		}
      void		setJetArea(Float_t a)		{ fJetArea = a;			}
      void		setPUJetId(Float_t i)		{ fPUJetId = i;			}
      void		setNChargedHadron(Float_t n)	{ fNChargedHadron = n;		}
      void		setNNeutralHadron(Float_t n)	{ fNNeutralHadron = n;		}
      void		setNElectron(UInt_t n)		{ fNElectron = n;		}
      void		setNMuon(UInt_t n)		{ fNMuon = n;			}
      void		setNPhoton(UInt_t n)		{ fNPhoton = n;			}
      void		setNeutralHadFrac(Float_t n)	{ fNeutralHadFraction = n;	}
      void		setChargedHadFrac(Float_t c)	{ fChargedHadFraction = c;	}
      void		setNeutralEMFrac(Float_t n)	{ fNeutralEMFraction = n;	}
      void		setChargedEMFrac(Float_t c)	{ fChargedEMFraction = c;	}

    protected :
      Float_t		fJECUncorrection;
      Float_t		fCSVBTag;
      Int_t		  fMatchedMCFlavor;
      Float_t		fJetArea;
      Float_t		fPUJetId;
      UInt_t		fNChargedHadron;
      UInt_t		fNNeutralHadron;
      UInt_t		fNElectron;
      UInt_t		fNMuon;
      UInt_t		fNPhoton;
      Float_t		fNeutralHadFraction;
      Float_t		fChargedHadFraction;
      Float_t		fNeutralEMFraction;
      Float_t		fChargedEMFraction;

    ClassDef(Jet, 1)

  };
}

#endif
