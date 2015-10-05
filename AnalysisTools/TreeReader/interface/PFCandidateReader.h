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
#include "AnalysisTools/TreeReader/interface/Defaults.h"

namespace ucsbsusy {

  class PFCandidateReader : public BaseReader {

    public :
    enum  Options           {
                              NULLOPT         = 0
                            , LOADRECO        = (1 <<  0)   ///< Load standard candidates
                            , LOADEXTRECO     = (1 <<  1)   ///< Load extra info
                            , FILLOBJ         = (1 <<  2)   ///< Fill objects (as opposed to just pointers)
                            , LOADTAUVETOMT   = (1 <<  3)   ///< Tau veto based on mT preselection
                            , LOADTAUVETODPHI = (1 <<  4)   ///< Tau veto based on dphi preselection
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
      std::vector<float> * dphimet;
      std::vector<float> * taudisc;
      std::vector<int>   * fromPV;
      std::vector<int>   * jetIndex;
      std::vector<int>   * tauIndex;
      std::vector<float> * chiso0p1;
      std::vector<float> * chiso0p2;
      std::vector<float> * chiso0p3;
      std::vector<float> * chiso0p4;
      std::vector<float> * totiso0p1;
      std::vector<float> * totiso0p2;
      std::vector<float> * totiso0p3;
      std::vector<float> * totiso0p4;
      std::vector<float> * neartrkdr;
      std::vector<float> * contjetdr;
      std::vector<float> * contjetcsv;
      std::vector<float> * trackiso;

      PFCandidateFCollection pfcands;
      ExtendedPFCandidateCollection extpfcands;

  };

}

#endif
