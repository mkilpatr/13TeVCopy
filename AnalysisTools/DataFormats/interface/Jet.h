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
	fJptRaw(-99), fCSVBTag(-99), fMatchedMCFlavor(-99), fJetArea(-99), fPUJetId(-99), fNChargedHadron(0), fNNeutralHadron(0), fNElectron(0), fNMuon(0), fNPhoton(0), fNeutralHadFraction(-1), fChargedHadFraction(-1), fNeutralEMFraction(-1), fChargedEMFraction(-1), fRawMom(0,0,0,0) {}
      ~Jet(){}

      Double_t		ptRaw()			const	{ return fJptRaw;		}
      Double_t		csv()			const	{ return fCSVBTag;		}
      Int_t		matchedMCFlavor()	const	{ return fMatchedMCFlavor;	}
      Double_t		jetArea()		const	{ return fJetArea;		}
      Double_t		puId()			const	{ return fPUJetId;		}
      UInt_t		nChargedHadron()	const	{ return fNChargedHadron;	}
      UInt_t		nNeutralHadron()	const	{ return fNNeutralHadron;	}
      UInt_t		nElectron()		const	{ return fNElectron;		}
      UInt_t		nMuon()			const	{ return fNMuon;		}
      UInt_t		nPhoton()		const	{ return fNPhoton;		}
      Double_t		neutralHadFrac()	const	{ return fNeutralHadFraction;	}
      Double_t		chargedHadFrac()	const	{ return fChargedHadFraction;	}
      Double_t		neutralEMFrac()		const	{ return fNeutralEMFraction;	}
      Double_t		chargedEMFrac()		const	{ return fChargedEMFraction;	}
      TLorentzVector	rawMom()		const	{ return fRawMom;		}

      void		setPtRaw(Double_t r)		{ fJptRaw = r;			}
      void		setCSV(Double_t c)		{ fCSVBTag = c;			}
      void		setMatchedMCFlavor(Int_t f)	{ fMatchedMCFlavor = f;		}
      void		setJetArea(Double_t a)		{ fJetArea = a;			}
      void		setPUJetId(Double_t i)		{ fPUJetId = i;			}
      void		setNChargedHadron(UInt_t n)	{ fNChargedHadron = n;		}
      void		setNNeutralHadron(UInt_t n)	{ fNNeutralHadron = n;		}
      void		setNElectron(UInt_t n)		{ fNElectron = n;		}
      void		setNMuon(UInt_t n)		{ fNMuon = n;			}
      void		setNPhoton(UInt_t n)		{ fNPhoton = n;			}
      void		setNeutralHadFrac(Double_t n)	{ fNeutralHadFraction = n;	}
      void		setChargedHadFrac(Double_t c)	{ fChargedHadFraction = c;	}
      void		setNeutralEMFrac(Double_t n)	{ fNeutralEMFraction = n;	}
      void		setChargedEMFrac(Double_t c)	{ fChargedEMFraction = c;	}
      void		setRawMom(TLorentzVector &v)	{ fRawMom = v;			}

    protected :
      Double_t		fJptRaw;
      Double_t		fCSVBTag;
      Int_t		fMatchedMCFlavor;
      Double_t		fJetArea;
      Double_t		fPUJetId;
      UInt_t		fNChargedHadron;
      UInt_t		fNNeutralHadron;
      UInt_t		fNElectron;
      UInt_t		fNMuon;
      UInt_t		fNPhoton;
      Double_t		fNeutralHadFraction;
      Double_t		fChargedHadFraction;
      Double_t		fNeutralEMFraction;
      Double_t		fChargedEMFraction;
      TLorentzVector	fRawMom;

    ClassDef(Jet, 1)

  };
}

#endif
