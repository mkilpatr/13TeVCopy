//--------------------------------------------------------------------------------------------------
// 
// prodIsoTrks
// 
// Class for storing prodIsoTrks information.
// 
// prodIsoTrks.h created on Mon Jan 19 16:21:42 CET 2015 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_DATAFORMATS_PRODISOTRKS_H
#define ANALYSISTOOLS_DATAFORMATS_PRODISOTRKS_H

#include "AnalysisTools/DataFormats/interface/Momentum.h"

namespace ucsbsusy {

  template < class CoordSystem>
  class prodIsoTrks : public Momentum<CoordSystem>
  {

    public :
  prodIsoTrks() : index_(-1), looseIsoTrks_charge_(0), looseIsoTrks_dz_(0), looseIsoTrks_pdgId_(0), looseIsoTrks_idx_(0), looseIsoTrks_iso_(0), looseIsoTrks_mtw_(0), looseIsoTrks_pfActivity_(0), forVetoIsoTrks_idx_(0), loosenIsoTrks_(0), nIsoTrksForVeto_(0) {} 

      template <class InputCoordSystem>
      prodIsoTrks(ROOT::Math::LorentzVector<InputCoordSystem> inMomentum,
		const int   inIndex   = -1,
                double inCharge = 0, 
		double inDz = 0, 
		int inPdgid = 0, 
		int inIdx = 0,
		double inIso = 0, 
		double inMtw = 0, 
		double inPfActivity = 0,
		int inVetoIsoTrks = 0, 
		unsigned int inNLooseIsoTrks = 0, 
		unsigned int inTrksForVeto = 0) :
                Momentum<InputCoordSystem>(inMomentum), index_(inIndex),
                looseIsoTrks_charge_(inCharge), looseIsoTrks_pdgId_(inPdgid), looseIsoTrks_idx_(inIdx),
		looseIsoTrks_iso_(inIso), looseIsoTrks_mtw_(inMtw), looseIsoTrks_pfActivity_(inPfActivity),
		forVetoIsoTrks_idx_(inVetoIsoTrks), loosenIsoTrks_(inNLooseIsoTrks), nIsoTrksForVeto_(inTrksForVeto) {}

      ~prodIsoTrks() {}

      enum  PFType {unknown, pfhfhad = 1, pfem = 2, pfelectron = 11, pfmuon = 13, pfphoton = 22, pfh0 = 130, pfhplus = 211};

      int      index()     		   const { return index_;     }
      double   looseIsoTrks_charge()       const { return looseIsoTrks_charge_;   }
      double   looseIsoTrks_dz()           const { return looseIsoTrks_dz_;       }
      int      looseIsoTrks_pdgid()        const { return looseIsoTrks_pdgId_;    }
      int      looseIsoTrks_idx()          const { return looseIsoTrks_idx_;      }
      double   looseIsoTrks_iso()          const { return looseIsoTrks_iso_;      }
      double   looseIsoTrks_mtw()          const { return looseIsoTrks_mtw_;      }
      double   looseIsoTrks_pfActivity()   const { return looseIsoTrks_pfActivity_;}
      int      forVetoIsoTrks_idx()	   const { return forVetoIsoTrks_idx_;    }
      unsigned int loosenIsoTrks()         const { return loosenIsoTrks_;         }
      unsigned int nIsoTrksForVeto()       const { return nIsoTrksForVeto_;       }
      bool     iselectron()      	   const { return (fabs(looseIsoTrks_pdgId_) == pfelectron); }
      bool     ismuon()          	   const { return (fabs(looseIsoTrks_pdgId_) == pfmuon);     }
      bool     isphoton()        	   const { return (fabs(looseIsoTrks_pdgId_) == pfphoton);   }
      bool     isneutralhadron() 	   const { return (fabs(looseIsoTrks_pdgId_) == pfh0);       }
      bool     ischargedhadron() 	   const { return (fabs(looseIsoTrks_pdgId_) == pfhplus);    }

      void     setIndex(const int& newIndex)        	       { index_     = newIndex;    		  }
      void     setCharge(double newCharge)  	 	       { looseIsoTrks_charge_ = newCharge;        }
      void     setDz(double newDz)          	 	       { looseIsoTrks_dz_ = newDz;                }
      void     setPdgId(int newPdgId)       	 	       { looseIsoTrks_pdgId_ = newPdgId;          }
      void     setIdx(int newIdx)		  	       { looseIsoTrks_idx_ = newIdx;		  }
      void     setIso(double newIso)        	 	       { looseIsoTrks_iso_ = newIso;              }
      void     setMtw(double newMtw)        	 	       { looseIsoTrks_mtw_ = newMtw;              }
      void     setPfActivity(double newPfActivity) 	       { looseIsoTrks_pfActivity_ = newPfActivity;}
      void     setVetoIsoTrks(int newVetoIsoTrks)  	       { forVetoIsoTrks_idx_ = newVetoIsoTrks;    }
      void     setNLooseIsoTrks(unsigned int newNLooseIsoTrks) { loosenIsoTrks_ = newNLooseIsoTrks; 	  }
      void     setTrksForVeto(unsigned int newTrksForVeto)     { nIsoTrksForVeto_ = newTrksForVeto; 	  }


    protected :
      int     index_;  //Index in prodIsoTrks vector
      double  looseIsoTrks_charge_;
      double  looseIsoTrks_dz_;
      int     looseIsoTrks_pdgId_;
      int     looseIsoTrks_idx_;
      double  looseIsoTrks_iso_;
      double  looseIsoTrks_mtw_;
      double  looseIsoTrks_pfActivity_;
      int     forVetoIsoTrks_idx_;
      unsigned int loosenIsoTrks_;
      unsigned int nIsoTrksForVeto_;

  };

  typedef prodIsoTrks<CylLorentzCoordF> prodIsoTrksF;
  typedef std::vector<prodIsoTrksF>     prodIsoTrksFCollection;

}

#endif
