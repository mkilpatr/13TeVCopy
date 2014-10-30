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

#include "AnalysisTools/TreeReader/interface/BaseReader.h"
#include "AnalysisTools/DataFormats/interface/Tau.h"
#include "AnalysisTools/ObjectSelection/interface/LeptonId.h"

namespace ucsbsusy {

  class TauReader : public BaseReader {

    public :
    enum  Options           {
                              NULLOPT         = 0
                            , LOADRECO        = (1 <<  1)   ///< Load standard taus
                            , LOADEXTRECO     = (1 <<  2)   ///< Load extra info
                            , FILLOBJ         = (1 <<  4)   ///< Fill objects (as opposed to just pointers)
    };
    static const int defaultOptions;

    TauReader();
    ~TauReader() {}

    void load(TTree *tree, int options, string branchName);
    void refresh();

    public :
      vector<float> *   pt;
      vector<float> *   eta;
      vector<float> *   phi;
      vector<float> *   mass;
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

      TauFCollection    taus;

    private :
      LeptonId* tauId;

  };

}


#endif
