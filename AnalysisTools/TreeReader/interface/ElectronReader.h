//--------------------------------------------------------------------------------------------------
// 
// ElectronReader
// 
// Class for reading electron object information from TTree.
// 
// ElectronReader.h created on Wed Oct 22 01:39:44 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_TREEREADER_ELECTRONREADER_H
#define ANALYSISTOOLS_TREEREADER_ELECTRONREADER_H

#include "AnalysisTools/TreeReader/interface/CollectionReader.h"
#include "AnalysisTools/DataFormats/interface/Electron.h"

namespace ucsbsusy {

  class ElectronReader : public CollectionReader {

    public :
      ElectronReader(TTree *tree) : CollectionReader(tree, kElectron) {}

      ~ElectronReader() {}

      bool	initTree();

      ElectronFCollection	getElectrons();

    protected :
      ElectronFCollection	electrons;
      vector<float> *		scEta;
      vector<float> *		r9;
      vector<float> *		d0;
      vector<float> *		dz;
      vector<float> *		pfdbetaiso;
      vector<float> *		mvaidnontrig;
      vector<float> *		mvaidtrig;

  };

}

#endif
