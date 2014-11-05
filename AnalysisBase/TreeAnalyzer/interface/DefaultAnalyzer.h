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

#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisTools/TreeReader/interface/Defaults.h"

namespace ucsbsusy {

  class DefaultAnalyzer : public BaseTreeAnalyzer {

    public :
      DefaultAnalyzer(TString fileName, TString treeName, bool isMCTree = false, TString readOption = "READ");
      ~DefaultAnalyzer(){};

      void    defaultLoad();

      void		cleanJetsAgainstLeptons(bool clean=true)	{ cleanJetsvLeptons = clean;	}
      void		cleanJetsAgainstTaus(bool clean=true)		{ cleanJetsvTaus = clean;	}

      void    process();
      bool    nextEvent(int reportFrequency = 100000);


      bool isGoodJet    (const RecoJetF& jet) const;
      bool isTightBJet  (const RecoJetF& jet) const;
      bool isMediumBJet (const RecoJetF& jet) const;

      bool isGoodElectron   (const ElectronF& electron) const;
      bool isGoodMuon       (const MuonF& muon        ) const;
      bool isGoodTau        (const TauF& tau          ) const;

      // accessors for standard event information
      unsigned int	run()		const { return evtInfoReader.run;		}
      unsigned int	lumi()  const { return evtInfoReader.lumi;		}
      unsigned int	event() const { return evtInfoReader.event;		}
      int		        nPV()	  const { return evtInfoReader.nPV;		}
  
      int		nLeptons() const {return leptons.size();}
      int		nTaus()		 const {return taus.size();	  }
      int		nJets()		 const {return jets.size();	  }
      int		nBJets()	 const {return bJets.size();  }

      MomentumF*               met      ;
      std::vector<LeptonF*>    leptons  ;
      std::vector<TauF*>       taus    ;
      std::vector<RecoJetF*>   jets    ;
      std::vector<RecoJetF*>   bJets   ;
      std::vector<RecoJetF*>   nonBJets;

    protected :
      bool		cleanJetsvLeptons;
      bool		cleanJetsvTaus;

    public:
      static const float  minElePt;
      static const float  minMuPt;
      static const float  minTauPt;
      static const float  minJetPt;
      static const float  minBJetPt;
      static const float  maxEleEta;
      static const float  maxMuEta;
      static const float  maxTauEta;
      static const float  maxJetEta;
      static const float  maxBJetEta;
      static const float  minDR;
  };

}

#endif
