//--------------------------------------------------------------------------------------------------
// 
// JetReader
// 
// Class for reading electron object information from TTree.
// 
// JetReader.h created on Wed Oct 22 01:39:44 CEST 2014
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_TREEREADER_FATJETREADER_H
#define ANALYSISTOOLS_TREEREADER_FATJETREADER_H


#include "AnalysisTools/TreeReader/interface/BaseReader.h"
#include "AnalysisTools/DataFormats/interface/FatJet.h"
#include <TRandom3.h>

namespace ucsbsusy {

  class FatJetReader : public BaseReader {
  public :
    enum  Options {
                      NULLOPT     = 0
                    , LOADRECO    = (1 << 0)
                    , LOADEXTRECO = (1 << 1)   ///< Load extra info
                    , FILLOBJ     = (1 << 2)   ///< Fill objects (as opposed to just pointers
                    , LOADPHYS14  = (1 << 3)
    };
    static const int defaultOptions;
    
    FatJetReader();
    ~FatJetReader() {}
    
    void load(TreeReader *treeReader, int options, std::string branchName);
    void refresh();
    
    void pushToTree(); //push changes made to the momentum back to the tree
  public:
    // Members to hold info to be filled in the tree (for now; this implementation is to be updated)
    std::vector<float>* fjrawmass_;
    std::vector<float>* fjtrimmedmass_;
    std::vector<float>* fjprunedmass_;
    std::vector<float>* fjsoftdropmass_;
    std::vector<float>* fjfilteredmass_;
    std::vector<float>* fjcmstoptagmass_;
    std::vector<float>* fjtau1_;
    std::vector<float>* fjtau2_;
    std::vector<float>* fjtau3_;
    std::vector<float>* fjpt_;
    std::vector<float>* fjeta_;
    std::vector<float>* fjphi_;
    std::vector<float>* fjwmass_;
    std::vector<float>* fjminmass_;
    std::vector<int>*   fjnsubjets_;

    //the actual jet collection
    FatJetFCollection fatJets;

};

}

#endif
