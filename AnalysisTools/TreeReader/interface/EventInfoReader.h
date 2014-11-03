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

namespace ucsbsusy {

  struct EventInfo {
    unsigned int	run;
    unsigned int	lumi;
    unsigned int	event;
    int			nPV;
    float		pvx;
    float		pvy;
    float		pvz;
    float		met;
    float		metphi;
    float		metsumEt;
  };

  class EventInfoReader : public BaseReader {

    public :
      EventInfoReader();
      ~EventInfoReader() {}

      void	load(TTree *tree, int options=0, string branchName="");
      void	refresh();

      EventInfo	evtInfo;

  };

}



#endif
