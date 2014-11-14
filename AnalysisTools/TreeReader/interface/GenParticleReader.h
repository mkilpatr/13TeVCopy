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
#include "AnalysisTools/DataFormats/interface/GenParticle.h"


namespace ucsbsusy {
class GenParticleReader : public BaseReader {
public:
  enum  Options           {
                            NULLOPT         = 0
                          , FILLOBJ         = (1 <<  1)   ///< Fill objects (as opposed to just pointers
  };
  static const int defaultOptions;
  class DataContainer {
  public:
    DataContainer();
    ~DataContainer();
    std::vector<float>* pt    ;
    std::vector<float>* eta   ;
    std::vector<float>* phi   ;
    std::vector<float>* mass  ;
    std::vector<int  >* status;
    std::vector<int  >* pdgid ;

    GenParticleFCollection particles;

    void load(const std::string branchName,const std::string collectionName, TreeReader * treeReader);
    void refresh();
  };

public :
  GenParticleReader();
  ~GenParticleReader() {}

  void load(TreeReader *treeReader, int options, std::string branchName);
  void refresh();
public:
  DataContainer genParticles  ;
  DataContainer thirdGenQuarks;
  DataContainer bosons        ;
  DataContainer bosonDaughters;
  DataContainer tauDaughters  ;
};

}

#endif
