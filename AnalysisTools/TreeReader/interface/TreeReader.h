//--------------------------------------------------------------------------------------------------
// 
// TreeReader
// 
// Class for reading information from TTree.
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_TREEREADER_TREEREADER_H
#define ANALYSISTOOLS_TREEREADER_TREEREADER_H
#include <TTree.h>
#include <iostream>
#include <string>



namespace ucsbsusy {

  class BaseReader;

  class TreeReader {
  public :
      TreeReader(TString fileName, TString treeName, TString readOption = "READ");
      ~TreeReader();

      //load a new reader
      void load(BaseReader * reader, int options, std::string branchName);

      //Load an object not included in a reader
      template<typename varType>
      varType* loadObject(std::string branchName){
        varType * tempVar = new varType;
        tree->SetBranchAddress(branchName.c_str(),&tempVar);
        return tempVar;
      }

      //Load the next event from the tree....return false if there are no more events in the tree
      bool nextEvent(int reportFrequency = 1000000);

      TTree * getTree() {return tree;}
      int getEntries()  const {return tree->GetEntries();}

      int     eventNumber; //current event number

  private:
      TFile * file;
      TTree * tree;
      std::vector<BaseReader*> readers; //List of loaded readers

  };

}

#endif
