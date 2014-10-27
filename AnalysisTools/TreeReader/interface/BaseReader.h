//--------------------------------------------------------------------------------------------------
// 
// BaseReader
// 
// Class to help with reading branches from TTrees.
// 
// BaseReader.h created on Tue Oct 21 15:33:32 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_TREEREADER_BASEREADER_H
#define ANALYSISTOOLS_TREEREADER_BASEREADER_H

#include <string>
#include <vector>
#include <assert.h>
#include <TString.h>
#include <TTree.h>

using namespace std;

namespace ucsbsusy {

  class BaseReader {

    public :
      BaseReader() : branchName_(""), options_(0) {};
      virtual ~BaseReader() {};

      virtual void load(TTree *tree, int options, string branchName) = 0;
      virtual void refresh() = 0;

      const string branchName_;  //branch prefix
      const int options_; //filling options
  }; //BaseReader

}

#endif
