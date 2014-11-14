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
      void setBranchAddress(const std::string branchName, const std::string varName, varType **var, bool require = false, bool verbose = true){
        TString tBranchName = (branchName == "" ? varName : branchName + "_" + varName).c_str();
        if(tree->GetBranch(tBranchName)){
          tree->SetBranchStatus(tBranchName,1);
          tree->SetBranchAddress(tBranchName,var);
        }
        else {
          if(require) throw (TString("TreeReader::setBranchAddress could not load variable: ") + tBranchName );
          if(verbose)std::cout << " -" <<tBranchName;
        }
      }

      template<typename varType>
      void setBranchAddress(const std::string branchName, const std::string varName, varType *var, bool require = false, bool verbose = true){
        TString tBranchName = (branchName == "" ? varName : branchName + "_" + varName).c_str();
        if(tree->GetBranch(tBranchName)){
          tree->SetBranchStatus(tBranchName,1);
          tree->SetBranchAddress(tBranchName,var);
        }
        else {
          if(require) throw (TString("TreeReader::setBranchAddress could not load variable: ") + tBranchName );
          if(verbose)std::clog << " -" <<tBranchName;
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
