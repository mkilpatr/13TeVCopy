//--------------------------------------------------------------------------------------------------
// 
// LeptonId
// 
// Class for implementing lepton selections.
// 
// LeptonId.h created on Mon Oct 27 14:08:36 CET 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_UTILITIES_LEPTONID_H
#define ANALYSISTOOLS_UTILITIES_LEPTONID_H

#include <map>
#include <array>
#include <iostream>

#include "AnalysisTools/DataFormats/interface/Electron.h"
#include "AnalysisTools/DataFormats/interface/Muon.h"

namespace ucsbsusy {

  typedef std::map<std::string, std::array<float, 4> > eleCuts;

  class LeptonId
  {

    public :
      // arguments needed to set up cut values ... this is not very elegant right now
      LeptonId(int bunchSpacing = 25, bool initElectronIds = true);

      ~LeptonId();

      enum WorkingPoint{VETO, LOOSE, MEDIUM, TIGHT};

      void setBunchSpacing(int val)		{ bunchSpacing_ = val;	}
      bool initElectronCuts(int bunchSpacing);

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

    private :
      int	bunchSpacing_;
      bool	eleIdInit_;
      eleCuts	csa14Cuts_barrel_;
      eleCuts	csa14Cuts_endcap_;

  };

}

#endif
