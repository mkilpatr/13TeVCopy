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

namespace ucsbsusy {

  class MuonReader : public BaseReader {

  public :
  enum  Options           {
                            NULLOPT         = 0
                          , LOADRECO        = (1 <<  1)   ///< Load standard muons
                          , LOADEXTRECO     = (1 <<  2)   ///< Load extra info
                          , FILLOBJ         = (1 <<  4)   ///< Fill objects (as opposed to just pointers)
  };
  static const int defaultOptions;


  MuonReader();
  ~MuonReader() {}

  void load(TTree *tree, int options, string branchName);
  void refresh();

    public :
      vector<float> *   pt;
      vector<float> *   eta;
      vector<float> *   phi;
      vector<float> *   mass;
      vector<int>   *	q     ;
      vector<float> *	d0    ;
      vector<float> *	dz    ;
      vector<float> *	pfdbetaiso;
      vector<bool>  *	isloose;
      vector<bool>  *	istight;
      vector<bool>  *	ispf;
      vector<bool>  *	isglobal;
      vector<bool>  *	istracker;
      vector<bool>  *	isstandalone;

      MuonFCollection muons;

  };

}

#endif
