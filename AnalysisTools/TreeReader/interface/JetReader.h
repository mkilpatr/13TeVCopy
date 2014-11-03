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

  void load(TTree *tree, int options, string branchName);
  void refresh();

  void pushToTree(); //push changes made to the momentum back to the tree
public:
  // Members to hold info to be filled in the tree (for now; this implementation is to be updated)
  vector<float>* jetpt_;
  vector<float>* jeteta_;
  vector<float>* jetphi_;
  vector<float>* jetmass_;
  vector<float>* jetptraw_;
  vector<float>* jetpuId_;
  vector<float>* jetcsv_;
  vector<int>  * jetflavor_;
  // For genjets matched to reco jets
  vector<float>* genjetpt_;
  vector<float>* genjeteta_;
  vector<float>* genjetphi_;
  vector<float>* genjetmass_;
  vector<int>  * genjetflavor_;
  // jet shape holders
  vector<float>* jetqgl_;


  //the actual jet collection
  RecoJetFCollection recoJets;
  GenJetFCollection  genJets;
};

}

#endif
