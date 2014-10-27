//--------------------------------------------------------------------------------------------------
// 
// ElectronReader
// 
// Class for reading electron object information from TTree.
// 
// ElectronReader.h created on Wed Oct 22 01:39:44 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_TREEREADER_ELECTRONREADER_H
#define ANALYSISTOOLS_TREEREADER_ELECTRONREADER_H

#include "AnalysisTools/TreeReader/interface/BaseReader.h"
#include "AnalysisTools/DataFormats/interface/Electron.h"

namespace ucsbsusy {

  class ElectronReader : public BaseReader {

    public :
    enum  Options           {
                              NULLOPT         = 0
                            , LOADRECO        = (1 <<  1)   ///< Load standard electrons
                            , LOADEXTRECO     = (1 <<  2)   ///< Load extra info
                            , FILLOBJ         = (1 <<  4)   ///< Fill objects (as opposed to just pointers)
    };
    static const int defaultOptions;

    ElectronReader();
    ~ElectronReader() {}

    void load(TTree *tree, int options, string branchName);
    void refresh();

    public :
      vector<float> *		pt;
      vector<float> *		eta;
      vector<float> *		phi;
      vector<float> *		mass;
      vector<int>   *		q;
      vector<float> *		scEta;
      vector<float> *		r9;
      vector<float> *		d0;
      vector<float> *		dz;
      vector<float> *		pfdbetaiso;
      vector<float> *		mvaidnontrig;
      vector<float> *		mvaidtrig;

      ElectronFCollection electrons;

  };

}

#endif
