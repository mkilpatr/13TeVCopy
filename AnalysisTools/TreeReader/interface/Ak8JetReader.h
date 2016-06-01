//--------------------------------------------------------------------------------------------------
// 
// JetReader
// 
// Class for reading electron object information from TTree.
// 
// JetReader.h created on Wed Oct 22 01:39:44 CEST 2014
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_TREEREADER_AK8JETREADER_H
#define ANALYSISTOOLS_TREEREADER_AK8JETREADER_H


#include "AnalysisTools/TreeReader/interface/BaseReader.h"
#include "AnalysisTools/DataFormats/interface/FatJet.h"
#include <TRandom3.h>

namespace ucsbsusy {

  class Ak8JetReader : public BaseReader {
  public :
    enum  Options {
                      NULLOPT     = 0
                    , LOADRECO    = (1 << 0)
                    , LOADEXTRECO = (1 <<  1)   ///< Load extra info
                    , FILLOBJ     = (1 << 2)   ///< Fill objects (as opposed to just pointers
    };
    static const int defaultOptions;
    
    Ak8JetReader();
    ~Ak8JetReader() {}
    
    void load(TreeReader *treeReader, int options, std::string branchName);
    void refresh();
    
    void pushToTree(); //push changes made to the momentum back to the tree
  public:
    // Members to hold info to be filled in the tree (for now; this implementation is to be updated)
    std::vector<float>* ak8rawmass_;
    std::vector<float>* ak8prunedmass_;
    std::vector<float>* ak8softdropmass_;
    //    std::vector<float>* ak8cmstoptagmass_;
    std::vector<float>* ak8tau1_;
    std::vector<float>* ak8tau2_;
    std::vector<float>* ak8tau3_;
    std::vector<float>* ak8pt_;
    std::vector<float>* ak8eta_;
    std::vector<float>* ak8phi_;

    //the actual jet collection
    FatJetFCollection ak8Jets;

};

}

#endif
