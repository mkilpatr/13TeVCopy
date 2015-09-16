//--------------------------------------------------------------------------------------------------
// 
// TriggerObjectReader
// 
// Class for reading trigger object information from TTree
// 
// TriggerObjectReader.h created on Mon Jul 20 15:34:06 CEST 2015 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_TREEREADER_TRIGGEROBJECTREADER_H
#define ANALYSISTOOLS_TREEREADER_TRIGGEROBJECTREADER_H

#include "AnalysisTools/TreeReader/interface/BaseReader.h"
#include "AnalysisTools/TreeReader/interface/TreeReader.h"
#include "AnalysisTools/DataFormats/interface/TriggerObject.h"
#include "AnalysisTools/DataFormats/interface/TriggerInfo.h"

namespace ucsbsusy {

  class TriggerObjectReader : public BaseReader {

  public :
  enum  Options           {
                            NULLOPT         = 0
                          , LOADOBJ         = (1 <<  0)   ///< Load objects
                          , FILLOBJ         = (1 <<  1)   ///< Fill objects (as opposed to just pointers)
  };
  static const int defaultOptions;

  TriggerObjectReader();
  ~TriggerObjectReader() {}

  void load(TreeReader *treeReader, int options, std::string branchName);
  void refresh();

    public :
      std::vector<float>         * pt;
      std::vector<float>         * eta;
      std::vector<float>         * phi;
      std::vector<float>         * mass;
      std::vector<unsigned long> * pathflags;
      std::vector<unsigned long> * filterflags;

      std::vector<unsigned long> * trigbitflags;
      std::vector<bool         > * trigbitpass;
      std::vector<unsigned int > * trigbitprescale;

      unsigned long            triggerflag;
      TriggerObjectFCollection trigobjs;
      TriggerInfoCollection    triginfo;


  };

}

#endif
