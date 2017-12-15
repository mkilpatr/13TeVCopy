//--------------------------------------------------------------------------------------------------
// 
// prodIsoTrksReader
// 
// Class for reading prodIsoTrks object information from TTree.
// 
// prodIsoTrksReader.h created on Wed Oct 22 01:39:44 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_TREEREADER_PRODISOTRKSREADER_H
#define ANALYSISTOOLS_TREEREADER_PRODISOTRKSREADER_H

#include "AnalysisTools/TreeReader/interface/BaseReader.h"
#include "AnalysisTools/DataFormats/interface/prodIsoTrks.h"
#include "AnalysisTools/TreeReader/interface/Defaults.h"

namespace ucsbsusy {

  class prodIsoTrksReader : public BaseReader {

    public :
    enum  Options           {
                              NULLOPT         = 0
                            , LOADRECO        = (1 <<  0)   ///< Load standard candidates
                            , FILLOBJ         = (1 <<  2)   ///< Fill objects (as opposed to just pointers)
    };
    static const int defaultOptions;

    prodIsoTrksReader();
    ~prodIsoTrksReader() {}

    void load(TreeReader *treeReader, int options, std::string branchName);
    void refresh();

    public :
      std::vector<float> * looseIsoTrks_pt;
      std::vector<float> * looseIsoTrks_eta;
      std::vector<float> * looseIsoTrks_phi;
      std::vector<float> * looseIsoTrks_mass;
      std::vector<double>* looseIsoTrks_charge;
      std::vector<double>* looseIsoTrks_dz;
      std::vector<int> *   looseIsoTrks_pdgId;
      std::vector<int> *   looseIsoTrks_idx;
      std::vector<double>* looseIsoTrks_iso;
      std::vector<double>* looseIsoTrks_mtw;
      std::vector<double>* looseIsoTrks_pfActivity;
      std::vector<int> *   forVetoIsoTrks_idx;
      unsigned int loosenIsoTrks;
      unsigned int nIsoTrksForVeto;

      prodIsoTrksFCollection prodisotrks;

  };

}

#endif
