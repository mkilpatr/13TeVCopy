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
      enum VarType {AK4JETS, ELECTRONS, MUONS, TAUS};

      TreeReader(TString fileName, TString treeName, bool isMCTree = false, TString readOption = "READ");
      ~TreeReader();

      //Load a variable type to be read from the TTree
      //use the defaultOptions if options is less than 1
      //use the default branch name prefix if set to an empty string
      void load(VarType type, int options = -1, string branchName = "" );

      //same as before but for non-default reader
      void load(BaseReader * reader, int options, string branchName);

      //Load the next event from the tree....return false if there are no more events in the tree
      bool nextEvent(bool verbose = false);

      bool    isMC;
      int     eventNumber; //current event number

      //Pointers to all default readers
      JetReader      * ak4Reader;
      ElectronReader * electronReader;
      MuonReader     * muonReader;
      TauReader      * tauReader;

      //Pointers to default objects (for ease of access)
      RecoJetFCollection * ak4Jets;
      GenJetFCollection  * ak4GenJets;
      ElectronFCollection* electrons;
      MuonFCollection    * muons;
      TauFCollection     * taus;

  private:
      TFile * file;
      TTree * tree;
      vector<BaseReader*> readers; //List of loaded readers

  };

}

#endif
