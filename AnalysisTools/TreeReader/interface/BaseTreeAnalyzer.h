//--------------------------------------------------------------------------------------------------
// 
// BaseTreeAnalyzer
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_TREEREADER_BASETREEREADER_H
#define ANALYSISTOOLS_TREEREADER_BASETREEREADER_H

#include <string>
#include <vector>
#include <assert.h>
#include <TString.h>
#include <TTree.h>

#include "AnalysisTools/TreeReader/interface/TreeReader.h"

using namespace std;

namespace ucsbsusy {

  class BaseTreeAnalyzer : public TreeReader {
  public:
    BaseTreeAnalyzer(TString fileName, TString treeName, bool isMCTree = false, TString readOption = "READ") : TreeReader(fileName,treeName,isMCTree,readOption) {}
    ~BaseTreeAnalyzer() {};
  };
}

#endif
