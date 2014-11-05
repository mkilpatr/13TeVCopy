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

      template<typename varType>
      void setBranchAddress(const TString branchName, varType **var, bool require = false, bool verbose = true){
        if(tree->GetBranch(branchName)){
          tree->SetBranchStatus(branchName,1);
          tree->SetBranchAddress(branchName,var);
        }
        else {
          if(require) throw (TString("TreeReader::setBranchAddress could not load variable: ") + branchName );
          if(verbose)std::cout << " -" <<branchName;
        }
      }
      template<typename varType>
      void setBranchAddress(const TString branchName, varType *var, bool require = false, bool verbose = true){
        if(tree->GetBranch(branchName)){
          tree->SetBranchStatus(branchName,1);
          tree->SetBranchAddress(branchName,var);
        }
        else {
          if(require) throw (TString("TreeReader::setBranchAddress could not load variable: ") + branchName );
          if(verbose)std::clog << " -" <<branchName;
        }
      }

      //load a new reader
      void load(BaseReader * reader, int options, std::string branchName);

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
