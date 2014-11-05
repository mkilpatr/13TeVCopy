//--------------------------------------------------------------------------------------------------
// 
// JetReader
// 
// Class for reading electron object information from TTree.
// 
// JetReader.h created on Wed Oct 22 01:39:44 CEST 2014
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_TREEREADER_JETREADER_H
#define ANALYSISTOOLS_TREEREADER_JETREADER_H


#include "AnalysisTools/TreeReader/interface/BaseReader.h"
#include "AnalysisTools/DataFormats/interface/Jet.h"


namespace ucsbsusy {

class JetReader : public BaseReader {
public :
  enum  Options           {
                            NULLOPT         = 0
                          , LOADRECO        = (1 <<  0)    ///< Load standard reco jets
                          , LOADGEN         = (1 <<  1)   ///< load gen jets
                          , LOADJETSHAPE    = (1 <<  2)   ///< load jet shap variables
                          , FILLOBJ         = (1 <<  3)   ///< Fill objects (as opposed to just pointers
  };
  static const int defaultOptions;

  JetReader();
  ~JetReader() {}

  void load(TreeReader *treeReader, int options, std::string branchName);
  void refresh();

  void pushToTree(); //push changes made to the momentum back to the tree
public:
  // Members to hold info to be filled in the tree (for now; this implementation is to be updated)
  std::vector<float>* jetpt_;
  std::vector<float>* jeteta_;
  std::vector<float>* jetphi_;
  std::vector<float>* jetmass_;
  std::vector<float>* jetptraw_;
  std::vector<float>* jetpuId_;
  std::vector<float>* jetcsv_;
  std::vector<int>  * jetflavor_;
  // For genjets matched to reco jets
  std::vector<float>* genjetpt_;
  std::vector<float>* genjeteta_;
  std::vector<float>* genjetphi_;
  std::vector<float>* genjetmass_;
  std::vector<int>  * genjetflavor_;
  // jet shape holders
  std::vector<float>* jetqgl_;


  //the actual jet collection
  RecoJetFCollection recoJets;
  GenJetFCollection  genJets;
};

}

#endif
