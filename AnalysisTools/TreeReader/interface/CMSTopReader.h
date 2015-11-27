//--------------------------------------------------------------------------------------------------
// 
// JetReader
// 
// Class for reading electron object information from TTree.
// 
// JetReader.h created on Wed Oct 22 01:39:44 CEST 2014
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_TREEREADER_CMSTOPREADER_H
#define ANALYSISTOOLS_TREEREADER_CMSTOPREADER_H


#include "AnalysisTools/TreeReader/interface/BaseReader.h"
#include "AnalysisTools/DataFormats/interface/CMSTop.h"
#include <TRandom3.h>

namespace ucsbsusy {

  class CMSTopReader : public BaseReader {
  public :
    enum  Options {
                      NULLOPT     = 0
                    , LOADRECO    = (1 << 0)
                    , LOADEXTRECO = (1 << 1)   ///< Load extra info
                    , FILLOBJ     = (1 << 2)   ///< Fill objects (as opposed to just pointers
                    , LOADPHYS14  = (1 << 3)
    };
    static const int defaultOptions;
    
    CMSTopReader();
    ~CMSTopReader() {}
    
    void load(TreeReader *treeReader, int options, std::string branchName);
    void refresh();
    
    void pushToTree(); //push changes made to the momentum back to the tree
  public:
    // Members to hold info to be filled in the tree (for now; this implementation is to be updated)
    std::vector<float>* toprawmass_;
    std::vector<float>* toptrimmedmass_;
    std::vector<float>* topprunedmass_;
    std::vector<float>* topsoftdropmass_;
    std::vector<float>* topfilteredmass_;
    std::vector<float>* topcmstoptagmass_;
    std::vector<float>* toptau1_;
    std::vector<float>* toptau2_;
    std::vector<float>* toptau3_;
    std::vector<float>* toppt_;
    std::vector<float>* topeta_;
    std::vector<float>* topphi_;
    std::vector<float>* topwmass_;
    std::vector<float>* topminmass_;
    std::vector<float>*   topnsubjets_;

    //the actual jet collection
    CMSTopFCollection cmsTops;

};

}

#endif
