//--------------------------------------------------------------------------------------------------
// 
// TreeReader
// 
// Class for reading information from TTree.
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_TREEREADER_TREEREADER_H
#define ANALYSISTOOLS_TREEREADER_TREEREADER_H

#include <TFile.h>

#include "AnalysisTools/TreeReader/interface/JetReader.h"
#include "AnalysisTools/TreeReader/interface/ElectronReader.h"
#include "AnalysisTools/TreeReader/interface/MuonReader.h"
#include "AnalysisTools/TreeReader/interface/TauReader.h"

using namespace std;

namespace ucsbsusy {

  class TreeReader {
  public :
      TreeReader(TString fileName, TString treeName, TString readOption = "READ");
      ~TreeReader();

      //load a new reader
      void load(BaseReader * reader, int options, string branchName);

      //Load an object not included in a reader
      template<typename varType>
      varType* loadObject(string branchName){
        varType * tempVar = new varType;
        tree->SetBranchAddress(branchName.c_str(),&tempVar);
        return tempVar;
      }

      //Load the next event from the tree....return false if there are no more events in the tree
      bool nextEvent(bool verbose = false);

      TTree * getTree() {return tree;}
      int getEntries()  const {return tree->GetEntries();}

      int     eventNumber; //current event number

  private:
      TFile * file;
      TTree * tree;
      vector<BaseReader*> readers; //List of loaded readers

  };

}

#endif
