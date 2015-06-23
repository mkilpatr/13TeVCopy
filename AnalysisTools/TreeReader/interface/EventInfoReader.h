//--------------------------------------------------------------------------------------------------
// 
// EventInfoReader
// 
// Class for reading event level information from TTree.
// 
// EventInfoReader.h created on Mon Nov 3 12:56:14 CET 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_TREEREADER_EVENTINFOREADER_H
#define ANALYSISTOOLS_TREEREADER_EVENTINFOREADER_H

#include "AnalysisTools/TreeReader/interface/BaseReader.h"
#include "AnalysisTools/DataFormats/interface/Momentum.h"
#include "AnalysisTools/TreeReader/interface/Defaults.h"

namespace ucsbsusy {

  class EventInfoReader : public BaseReader {

    public :
      EventInfoReader();
      ~EventInfoReader() {}

      void	load(TreeReader *treeReader, int options=0, std::string branchName="");
      void	refresh();

      unsigned int  run;
      unsigned int  lumi;
      unsigned int  event;
      unsigned int  nPV;
      float         rho;
      float         pvx;
      float         pvy;
      float         pvz;
      float         met_pt;
      float         met_phi;
      float         metsumEt;
      float         genmet_pt;
      float         genmet_phi;
      bool          goodvertex;
      size8             proc;
      defaults::Process process;
      float             weight;

      MomentumF    met;
      MomentumF    genmet;

  };

}



#endif
