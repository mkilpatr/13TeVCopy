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
#include <TTree.h>

namespace ucsbsusy {
  class TreeReader;
  class BaseReader {

    public :
      BaseReader() : branchName_(""), options_(0), loaded_(false) {};
      virtual ~BaseReader() {};

      virtual void load(TreeReader *treeReader, int options, std::string branchName) = 0;
      virtual void refresh() = 0;

      bool isLoaded() const {return loaded_;}

    protected:
      const std::string branchName_;  //branch prefix
      const int options_; //filling options
      bool loaded_; //has been loaded
  }; //BaseReader

}

#endif
