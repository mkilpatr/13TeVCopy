//--------------------------------------------------------------------------------------------------
// 
// PFCandidateReader
// 
// Class for reading PFCandidate object information from TTree.
// 
// PFCandidateReader.h created on Wed Oct 22 01:39:44 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_TREEREADER_PFCANDIDATEREADER_H
#define ANALYSISTOOLS_TREEREADER_PFCANDIDATEREADER_H

#include "AnalysisTools/TreeReader/interface/BaseReader.h"
#include "AnalysisTools/DataFormats/interface/PFCandidate.h"

namespace ucsbsusy {

  class PFCandidateReader : public BaseReader {

    public :
    enum  Options           {
                              NULLOPT         = 0
                            , LOADRECO        = (1 <<  0)   ///< Load standard candidates
                            , LOADEXTRECO     = (1 <<  1)   ///< Load extra info
                            , FILLOBJ         = (1 <<  2)   ///< Fill objects (as opposed to just pointers)
    };
    static const int defaultOptions;

    PFCandidateReader();
    ~PFCandidateReader() {}

    void load(TreeReader *treeReader, int options, std::string branchName);
    void refresh();

    public :
      std::vector<float> * pt;
      std::vector<float> * eta;
      std::vector<float> * phi;
      std::vector<float> * mass;
      std::vector<int>   * q;
      std::vector<int>   * pdgid;
      std::vector<float> * d0;
      std::vector<float> * dz;
      std::vector<float> * mt;
      std::vector<float> * taudisc;
      std::vector<int>   * fromPV;
      std::vector<int>   * jetIndex;
      std::vector<int>   * tauIndex;

      PFCandidateFCollection pfcands;

  };

}

#endif
