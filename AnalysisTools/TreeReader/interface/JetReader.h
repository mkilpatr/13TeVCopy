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
#include <TRandom3.h>

namespace ucsbsusy {

class JetReader : public BaseReader {
public :
  enum  Options           {
                            NULLOPT         = 0
                          , LOADRECO        = (1 <<  0)    ///< Load standard reco jets
                          , LOADGEN         = (1 <<  1)   ///< load gen jets
                          , LOADJETSHAPE    = (1 <<  2)   ///< load jet shap variables
                          , LOADTOPASSOC    = (1 <<  3)   ///< load top - jet assoc
                          , FILLOBJ         = (1 <<  4)   ///< Fill objects (as opposed to just pointers
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
  std::vector<float>* jetarea_;
  std::vector<int16>* jetgenindex_;
  // For genjets matched to reco jets
  std::vector<float>* genjetpt_;
  std::vector<float>* genjeteta_;
  std::vector<float>* genjetphi_;
  std::vector<float>* genjetmass_;
  std::vector<size8>* genjetflavor_;
  // jet shape holders
  std::vector<float>*  jetbetaStar_ ;
  std::vector<float>*  jetqgl_      ;
  std::vector<float>*  jetptD_      ;
  std::vector<float>*  jetaxis1_    ;
  std::vector<float>*  jetaxis2_    ;
  std::vector<size16>* jetMult_     ;
  std::vector<float>*  genjetptD_   ;
  std::vector<float>*  genjetaxis1_ ;
  std::vector<float>*  genjetaxis2_ ;
  std::vector<size16>* genjetMult_  ;
  //for top assoc
  std::vector<size16>* genAssocPrtIndex_;
  std::vector<size16>* genAssocJetIndex_;
  std::vector<int8>*   genAssocCont_;


  //the actual jet collection
  RecoJetFCollection recoJets;
  GenJetFCollection  genJets;
};

}

#endif
