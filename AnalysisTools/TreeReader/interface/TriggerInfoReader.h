//--------------------------------------------------------------------------------------------------
// 
// TriggerInfoReader
// 
// Class for reading trigger information from TTree
// 
// TriggerInfoReader.h created on Tue Sep 1 14:56:00 CEST 2015
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_TREEREADER_TRIGGERINFOREADER_H
#define ANALYSISTOOLS_TREEREADER_TRIGGERINFOREADER_H

#include "AnalysisTools/TreeReader/interface/BaseReader.h"
#include "AnalysisTools/TreeReader/interface/TreeReader.h"
#include "AnalysisTools/DataFormats/interface/TriggerInfo.h"

namespace ucsbsusy {

  class TriggerInfoReader : public BaseReader {

  public :
  enum  Options           {
                            NULLOPT         = 0
                          , LOADOBJ         = (1 <<  0)   ///< Load objects
                          , FILLOBJ         = (1 <<  1)   ///< Fill objects (as opposed to just pointers)
  };
  static const int defaultOptions;

  TriggerInfoReader();
  ~TriggerInfoReader() {}

  void load(TreeReader *treeReader, int options, std::string branchName);
  void refresh();

    public :
      std::vector<unsigned long> * trigflag;
      std::vector<bool         > * passtrig;
      std::vector<unsigned int > * prescale;

      TriggerInfoCollection triginfo;

  };

}

#endif
