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
                            , LOADRECO        = (1 <<  0)   ///< Load standard taus
                            , LOADEXTRECO     = (1 <<  1)   ///< Load extra info
                            , FILLOBJ         = (1 <<  2)   ///< Fill objects (as opposed to just pointers)
    };
    static const int defaultOptions;

    TauReader();
    ~TauReader() {}

    void load(TreeReader *treeReader, int options, std::string branchName);
    void refresh();

    public :
      std::vector<float> *   pt;
      std::vector<float> *   eta;
      std::vector<float> *   phi;
      std::vector<float> *   mass;
      std::vector<float> *		leadcandpt;
      std::vector<float> *		leadcandeta;
      std::vector<float> *		leadcandphi;
      std::vector<float> *		leadcandmass;
      std::vector<float> *		leadchcandpt;
      std::vector<float> *		leadchcandeta;
      std::vector<float> *		leadchcandphi;
      std::vector<float> *		leadchcandmass;
      std::vector<int> *		q;
      std::vector<float> *		dxy;
      std::vector<float> *		dxyerr;
      std::vector<float> *		dxysig;
      std::vector<unsigned long>  *	hpsid;

      TauFCollection    taus;

    private :
      LeptonId* tauId;

  };

}


#endif
