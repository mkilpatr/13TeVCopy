//--------------------------------------------------------------------------------------------------
// 
// DefaultAnalyzer
// 
// Inherits from BaseTreeAnalyzer and implements a default selection and cleaning on the objects.
// 
// DefaultAnalyzer.h created on Mon Nov 3 22:59:28 CET 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISBASE_TREEANALYZER_DEFAULTANALYZER_H
#define ANALYSISBASE_TREEANALYZER_DEFAULTANALYZER_H

#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"

namespace ucsbsusy {

  class DefaultAnalyzer : public BaseTreeAnalyzer {

    public :
      DefaultAnalyzer(TString fileName, TString treeName, bool isMCTree = false, TString readOption = "READ");
      ~DefaultAnalyzer();

      static const float	minElePt;
      static const float	minMuPt;
      static const float	minTauPt;
      static const float	minJetPt;
      static const float	minBJetPt;
      static const float	maxEleEta;
      static const float	maxMuEta;
      static const float	maxTauEta;
      static const float	maxJetEta;
      static const float	maxBJetEta;
      static const float	minBJetCSV;
      static const float	minDR;

      void		cleanJetsAgainstLeptons(bool clean=true)	{ cleanJetsvLeptons = clean;	}
      void		cleanJetsAgainstTaus(bool clean=true)		{ cleanJetsvTaus = clean;	}

      void		process();
      bool		nextEvent(int reportFrequency = 100000);

      // accessors for standard event information
      unsigned int	run()		{ return evtInfoReader.run;		}
      unsigned int	lumi()		{ return evtInfoReader.lumi;		}
      unsigned int	event()		{ return evtInfoReader.event;		}
      float		met()		{ return evtInfoReader.met.pt();	}
      float		metphi()	{ return evtInfoReader.met.phi();	}
      int		nPV()		{ return evtInfoReader.nPV;		}
  
      int		nLeptons()	{ return goodLeptons->size();		}
      int		nTaus()		{ return goodTaus->size();		}
      int		nJets()		{ return cleanJets->size();		}
      int		nBJets()	{ return cleanBJets->size();		}

      LeptonFCollection*	goodLeptons;
      TauFCollection*		goodTaus;
      RecoJetFCollection*	cleanJets;
      RecoJetFCollection*	cleanBJets;
      RecoJetFCollection*	cleanNonBJets;

    protected :
      bool		cleanJetsvLeptons;
      bool		cleanJetsvTaus;

  };

}

#endif
