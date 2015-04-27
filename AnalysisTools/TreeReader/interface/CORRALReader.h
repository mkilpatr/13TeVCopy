//--------------------------------------------------------------------------------------------------
// 
// JetReader
// 
// Class for reading electron object information from TTree.
// 
// JetReader.h created on Wed Oct 22 01:39:44 CEST 2014
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_TREEREADER_CORRALREADER_H
#define ANALYSISTOOLS_TREEREADER_CORRALREADER_H


#include "AnalysisTools/TreeReader/interface/BaseReader.h"
#include "AnalysisTools/DataFormats/interface/Jet.h"

namespace ucsbsusy {

class CORRALReader : public BaseReader {
public :
  enum  Options           {
                            NULLOPT         = 0
  };
  static const int defaultOptions;

  CORRALReader();
  ~CORRALReader() {}

  void load(TreeReader *treeReader, int options, std::string branchName);
  void refresh();
public:
  // Members to hold info to be filled in the tree (for now; this implementation is to be updated)
  std::vector<int16>* bjetind ;
  std::vector<int16>* wjet1ind;
  std::vector<int16>* wjet2ind;
  std::vector<float>* wmva    ;
  std::vector<float>* tmva    ;

  int transformIndex(int inIndex, const std::vector<RecoJetF*>& recoJets, bool forceMatch = false) const;
};

}

#endif
