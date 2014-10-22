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
      BaseReader(TTree *tree) : fTree(tree) {}

      virtual ~BaseReader()	{ delete fTree;	}

      virtual bool	initTree() = 0;

      virtual int	loadEvent(Long64_t entry);

      virtual TTree*	getTree()	{ return fTree;		}

    protected :
      TTree		*fTree;

  }; //BaseReader

}

#endif
