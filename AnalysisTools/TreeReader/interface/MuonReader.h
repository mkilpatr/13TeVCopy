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

#include "AnalysisTools/TreeReader/interface/CollectionReader.h"
#include "AnalysisTools/DataFormats/interface/Muon.h"

namespace ucsbsusy {

  class MuonReader : public CollectionReader {

    public :
      MuonReader(TTree *tree) : CollectionReader(tree, kMuon) {}

      ~MuonReader() {}

      bool		initTree();

      MuonFCollection	getMuons();

    protected :
      MuonFCollection	muons;
      vector<int> *	q;
      vector<float> *	d0;
      vector<float> *	dz;
      vector<float> *	pfdbetaiso;
      vector<bool>  *	isloose;
      vector<bool>  *	istight;
      vector<bool>  *	ispf;
      vector<bool>  *	isglobal;
      vector<bool>  *	istracker;
      vector<bool>  *	isstandalone;

  };

}

#endif
