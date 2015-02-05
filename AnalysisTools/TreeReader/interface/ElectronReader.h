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
#include "AnalysisTools/ObjectSelection/interface/LeptonId.h"

namespace ucsbsusy {

  class ElectronReader : public BaseReader {

    public :
    enum  Options           {
                              NULLOPT         = 0
                            , LOADRECO        = (1 <<  0)   ///< Load standard electrons
                            , LOADEXTRECO     = (1 <<  1)   ///< Load extra info
                            , FILLOBJ         = (1 <<  2)   ///< Fill objects (as opposed to just pointers)
    };
    static const int defaultOptions;

    ElectronReader();
    ~ElectronReader() {}

    void load(TreeReader *treeReader, int options, std::string branchName);
    void refresh();

    public :
      std::vector<float> *		pt;
      std::vector<float> *		eta;
      std::vector<float> *		phi;
      std::vector<float> *		mass;
      std::vector<int>   *		q;
      std::vector<float> *		scEta;
      std::vector<float> *		r9;
      std::vector<float> *		d0;
      std::vector<float> *		dz;
      std::vector<float> *		pfdbetaiso;
      std::vector<float> *		mvaiso;
      std::vector<float> *		mvaidnontrig;
      std::vector<float> *		mvaidtrig;
      std::vector<bool>  *		isveto;
      std::vector<bool>  *		isloose;
      std::vector<bool>  *		ismedium;
      std::vector<bool>  *		istight;

      //      std::vector<bool>  *              passCutBaseMediumID;

      ElectronFCollection electrons;

    private :
      LeptonId* eleId;

  };

}

#endif
