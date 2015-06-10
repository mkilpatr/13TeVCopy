//--------------------------------------------------------------------------------------------------
// 
// LeptonId
// 
// Class for implementing lepton selections.
// 
// LeptonId.h created on Mon Oct 27 14:08:36 CET 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_OBJECTSELECTION_LEPTONID_H
#define ANALYSISTOOLS_OBJECTSELECTION_LEPTONID_H

#include <map>
#include <iostream>

#include "AnalysisTools/DataFormats/interface/Electron.h"
#include "AnalysisTools/DataFormats/interface/Muon.h"
#include "AnalysisTools/DataFormats/interface/Tau.h"

namespace ucsbsusy {

  typedef std::map<std::string, float* > eleCuts;

  class LeptonId
  {

    public :
      // arguments needed to set up cut values ... this is not very elegant right now
      LeptonId(bool initElectronIds = false, int bunchSpacing = 25);

      ~LeptonId();

      enum WorkingPoint{VETO, LOOSE, MEDIUM, TIGHT, MVA, MVAVeto, MultiIsoVetoL, MultiIsoVetoVL};

      void setBunchSpacing(int val)		{ bunchSpacing_ = val;	}
      bool initElectronCuts(int bunchSpacing);

      // Standard POG selections
      bool passElectronId(ElectronF *ele, unsigned int WP = TIGHT);
      bool passMuonId(MuonF *mu, unsigned int WP = MEDIUM); // WP is for isolation cut
      bool passTauId(TauF *tau, unsigned int isoWP = LOOSE, unsigned int antimuWP = MEDIUM, unsigned int antieleWP = MEDIUM); // choose some defaults for now, to be studied

      bool passEleIdCSA14CutBased(ExtendedElectron *ele, unsigned int WP);
      bool passEleIdCSA14CutBased(float elePt, float eleSCeta,
				  float eleDEtaIn, float eleDPhiIn,
				  float eleFull5x5sigietaieta, float eleHOverE,
				  float eleD0, float eleDz,
				  float eleEcalE, float eleEOverPIn,
				  float elePFdbetaiso, bool elePassConvVeto,
				  int eleNMissingHits, unsigned int WP);
      bool passEleIdCSA14MVA(ElectronF *ele, unsigned int WP);
      bool passEleIsoCSA14(ElectronF *ele, unsigned int WP);
      bool passEleIsoCSA14(float elePt, float eleSCeta, float elePFdbetaiso, unsigned int WP);
      bool passMuonIdOnly(MuonF *mu);
      bool passMuonIso(MuonF *mu, unsigned int WP = MEDIUM);

    private :
      int	bunchSpacing_;
      bool	eleIdInit_;
      eleCuts	csa14Cuts_barrel_;
      eleCuts	csa14Cuts_endcap_;

  };

}

#endif
