//--------------------------------------------------------------------------------------------------
// 
// TauReader
// 
// Class for reading electron object information from TTree.
// 
// TauReader.h created on Wed Oct 22 23:13:49 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_TREEREADER_TAUREADER_H
#define ANALYSISTOOLS_TREEREADER_TAUREADER_H

#include "AnalysisTools/TreeReader/interface/CollectionReader.h"
#include "AnalysisTools/DataFormats/interface/Tau.h"

namespace ucsbsusy {

  class TauReader : public CollectionReader {

    public :
      TauReader(TTree *tree) : CollectionReader(tree, kTau) {}

      ~TauReader() {}

      bool		initTree();

      TauFCollection	getTaus();

    protected :
      TauFCollection		taus;
      vector<float> *		leadcandpt;
      vector<float> *		leadcandeta;
      vector<float> *		leadcandphi;
      vector<float> *		leadcandmass;
      vector<float> *		leadchcandpt;
      vector<float> *		leadchcandeta;
      vector<float> *		leadchcandphi;
      vector<float> *		leadchcandmass;
      vector<int> *		q;
      vector<float> *		dxy;
      vector<float> *		dxyerr;
      vector<float> *		dxysig;
      vector<unsigned long>  *	hpsid;

  };

}


#endif
