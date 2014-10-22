//--------------------------------------------------------------------------------------------------
// 
// CollectionReader
// 
// Class to help with reading collections of objects from TTrees and getting momentum four vectors.
// 
// CollectionReader.h created on Tue Oct 21 15:33:32 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_TREEREADER_COLLECTIONREADER_H
#define ANALYSISTOOLS_TREEREADER_COLLECTIONREADER_H

#include "AnalysisTools/TreeReader/interface/BaseReader.h"
#include "AnalysisTools/DataFormats/interface/Momentum.h"

using namespace std;

namespace ucsbsusy {

  class CollectionReader : public BaseReader {

    public :
      CollectionReader(TTree *tree, int type) : BaseReader(tree), fObjType(type) {}

      virtual ~CollectionReader() {}

      enum	objType	{ kAK4Jet, kElectron, kMuon, kTau	};

      virtual bool		initTree();

      MomentumFCollection	getMomenta();

    protected :
      MomentumFCollection	momenta;
      vector<float> *		pt;
      vector<float> *		eta;
      vector<float> *		phi;
      vector<float> *		mass;
      int			fObjType;

  }; // CollectionReader

}

#endif
