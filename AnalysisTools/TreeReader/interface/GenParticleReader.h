//--------------------------------------------------------------------------------------------------
// 
// JetReader
// 
// Class for reading electron object information from TTree.
// 
// JetReader.h created on Wed Oct 22 01:39:44 CEST 2014
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_TREEREADER_GENPARTICLEREADER_H
#define ANALYSISTOOLS_TREEREADER_GENPARTICLEREADER_H


#include "AnalysisTools/TreeReader/interface/BaseReader.h"
#include "AnalysisTools/DataFormats/interface/GenParticle.h"


namespace ucsbsusy {
class GenParticleReader : public BaseReader {
  typedef size16 stor;
public:
  enum  Options           {
                            NULLOPT         = 0
                          , LOADPARTONDECAY = (1 <<  0)   ///< Load parton decay info
                          , FILLOBJ         = (1 <<  1)   ///< Fill objects (as opposed to just pointers
  };
  static const int defaultOptions;

public:
  GenParticleReader();
  ~GenParticleReader();

  void load(TreeReader *treeReader, int options, std::string branchName);
  void refresh();
public:
  std::vector<float>*  pt_       ;
  std::vector<float>*  eta_      ;
  std::vector<float>*  phi_      ;
  std::vector<float>*  mass_     ;
  std::vector<size8>*  status_   ;
  std::vector<int  >*  pdgId_    ;
  std::vector<stor >*  nMoms_    ;
  std::vector<stor >*  firstMom_ ;
  std::vector<stor >*  nDaus_    ;
  std::vector<stor >*  firstDau_ ;
  std::vector<stor >*  assocList_;
  std::vector<float>*  hade_     ;

  GenParticleFCollection genParticles;
};

}

#endif
