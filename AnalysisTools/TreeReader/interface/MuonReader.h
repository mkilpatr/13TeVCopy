//--------------------------------------------------------------------------------------------------
// 
// MuonReader
// 
// Class for reading muon object information from TTree.
// 
// MuonReader.h created on Wed Oct 22 01:39:44 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_TREEREADER_MUONREADER_H
#define ANALYSISTOOLS_TREEREADER_MUONREADER_H

#include "AnalysisTools/TreeReader/interface/BaseReader.h"
#include "AnalysisTools/DataFormats/interface/Muon.h"
#include "AnalysisTools/ObjectSelection/interface/LeptonId.h"

namespace ucsbsusy {

  class MuonReader : public BaseReader {

  public :
  enum  Options           {
                            NULLOPT         = 0
                          , LOADRECO        = (1 <<  0)   ///< Load standard muons
                          , LOADEXTRECO     = (1 <<  1)   ///< Load extra info
                          , FILLOBJ         = (1 <<  2)   ///< Fill objects (as opposed to just pointers)
  };
  static const int defaultOptions;


  MuonReader();
  ~MuonReader() {}

  void load(TreeReader *treeReader, int options, std::string branchName);
  void refresh();

    public :
      std::vector<float> *   pt;
      std::vector<float> *   eta;
      std::vector<float> *   phi;
      std::vector<float> *   mass;
      std::vector<int>   *	q     ;
      std::vector<float> *	d0    ;
      std::vector<float> *	dz    ;
      std::vector<float> *	sip3d;
      std::vector<float> *	pfdbetaiso;
      std::vector<float> *	mvaiso;
      std::vector<bool>  *	isloose;
      std::vector<bool>  *	ismedium;
      std::vector<bool>  *	istight;
      std::vector<bool>  *	ispf;
      std::vector<bool>  *	isglobal;
      std::vector<bool>  *	istracker;
      std::vector<bool>  *	isstandalone;
      std::vector<float> *	miniiso;
      std::vector<float> *	ptrel;
      std::vector<float> *	ptratio;
      std::vector<float> *	annulus;

      MuonFCollection muons;
  };

}

#endif
