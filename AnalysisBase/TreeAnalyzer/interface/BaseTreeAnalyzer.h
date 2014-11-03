//--------------------------------------------------------------------------------------------------
// 
// BaseTreeAnalyzer
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_TREEREADER_BASETREEREADER_H
#define ANALYSISTOOLS_TREEREADER_BASETREEREADER_H

#include <string>
#include <vector>
#include <assert.h>
#include <TString.h>
#include <TTree.h>

#include "AnalysisTools/TreeReader/interface/TreeReader.h"

using namespace std;

namespace ucsbsusy {

  class BaseTreeAnalyzer{

  public:
    enum VarType {EVTINFO, AK4JETS, ELECTRONS, MUONS, TAUS};

    BaseTreeAnalyzer(TString fileName, TString treeName, bool isMCTree = false, TString readOption = "READ");
    ~BaseTreeAnalyzer() {};

    //Load a variable type to be read from the TTree
    //use the defaultOptions if options is less than 1
    //use the default branch name prefix if set to an empty string
    void	load(VarType type, int options = -1, string branchName = "" );
    //same as above but for non-default readers
    void	load(BaseReader * inReader, int options, string branchName) {reader.load(inReader,options,branchName);}
    //load a non-reader variable from the tree
    template<typename varType> varType*	loadObject(string branchName){ return reader.loadObject<varType>(branchName);}

    //load the next event
    bool	nextEvent(int reportFrequency = 100000);

    //get event number
    int		getEventNumber()  const	{ return reader.eventNumber;	}

    //get number of entries
    int		getEntries()	  const	{ return reader.getEntries();	}

  private:
    TreeReader	reader; //default reader

  public:
    const bool	isMC;

    //All default readers
    EventInfoReader	evtInfoReader;
    JetReader		ak4Reader;
    ElectronReader	electronReader;
    MuonReader		muonReader;
    TauReader		tauReader;

    //Pointers to default objects (for ease of access)
    EventInfo*			eventInfo;
    RecoJetFCollection*		ak4Jets;
    GenJetFCollection*		ak4GenJets;
    ElectronFCollection*	electrons;
    MuonFCollection*		muons;
    TauFCollection*		taus;

  };
}

#endif
