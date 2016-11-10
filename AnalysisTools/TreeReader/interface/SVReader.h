//--------------------------------------------------------------------------------------------------
//
// JetReader
//
// Class for reading electron object information from TTree.
//
// JetReader.h created on Wed Oct 22 01:39:44 CEST 2014
//
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_TREEREADER_SVREADER_H
#define ANALYSISTOOLS_TREEREADER_SVREADER_H


#include "AnalysisTools/TreeReader/interface/BaseReader.h"
#include "AnalysisTools/DataFormats/interface/SV.h"
#include "AnalysisTools/ObjectSelection/interface/IVFMVA.h"

namespace ucsbsusy {

  class SVReader : public BaseReader {
  public :
    enum  Options {
                      NULLOPT     = 0
                    , LOADRECO    = (1 << 0)
                    , FILLOBJ     = (1 << 1)   ///< Fill objects (as opposed to just pointers
                    , UPDATEMVA   = (1 << 2)
    };
    static const int defaultOptions;

    SVReader();
    ~SVReader() {}

    void load(TreeReader *treeReader, int options, std::string branchName);
    void refresh();

    void pushToTree(); //push changes made to the momentum back to the tree
  public:
    // Members to hold info to be filled in the tree (for now; this implementation is to be updated)
    std::vector<float>* svpt_;
    std::vector<float>* sveta_;
    std::vector<float>* svphi_;
    std::vector<float>* svmass_;
    std::vector<int>*   svntracks_;
    std::vector<float>* svchi2_;
    std::vector<float>* svndf_;
    std::vector<float>* svdxy_;
    std::vector<float>* svdxyerr_;
    std::vector<float>* svd3d_;
    std::vector<float>* svd3derr_;
    std::vector<float>* svcossvpv_;
    std::vector<float>* svmva_;

    //the actual sv collection
    SVFCollection SVs;

  private:
    //low pT (<15) and high pT (>15)
    //barrel (|eta| <1.2) and endcap (|eta|> 1.2)
    IVFMVA* ivfMVA_lpt_b = 0;
    IVFMVA* ivfMVA_hpt_b = 0;
    IVFMVA* ivfMVA_lpt_e = 0;
    IVFMVA* ivfMVA_hpt_e = 0;

};

}

#endif
