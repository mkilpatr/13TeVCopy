//--------------------------------------------------------------------------------------------------
// 
// BaseTreeAnalyzer
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISBASE_TREEANALYZER_BASETREEANALYZER_H
#define ANALYSISBASE_TREEANALYZER_BASETREEANALYZER_H

#include <string>
#include <vector>
#include <assert.h>
#include <TString.h>
#include <TTree.h>

#include "AnalysisTools/TreeReader/interface/TreeReader.h"
#include "AnalysisTools/TreeReader/interface/EventInfoReader.h"
#include "AnalysisTools/TreeReader/interface/JetReader.h"
#include "AnalysisTools/TreeReader/interface/ElectronReader.h"
#include "AnalysisTools/TreeReader/interface/MuonReader.h"
#include "AnalysisTools/TreeReader/interface/TauReader.h"

namespace ucsbsusy {


  class BaseTreeAnalyzer{

  public:
    enum VarType {EVTINFO, AK4JETS, ELECTRONS, MUONS, TAUS};

    BaseTreeAnalyzer(TString fileName, TString treeName, bool isMCTree = false, TString readOption = "READ");
    virtual ~BaseTreeAnalyzer() {};

    //Load a variable type to be read from the TTree
    //use the defaultOptions if options is less than 1
    //use the default branch name prefix if set to an empty string
    void	load(VarType type, int options = -1, std::string branchName = "" );
    //same as above but for non-default readers
    void	load(BaseReader * inReader, int options, std::string branchName) {reader.load(inReader,options,branchName);}
    //load a non-reader variable from the tree
    template<typename varType> varType*	loadObject(std::string branchName){ return reader.loadObject<varType>(branchName);}

    //load the next event
    virtual bool	nextEvent(int reportFrequency = 100000);

    //get event number
    int		getEventNumber()  const	{ return reader.eventNumber;	}

    //get number of entries
    int		getEntries()	  const	{ return reader.getEntries();	}

    // Filters a collection into a vector of pointers
    template <typename Obj>
    std::vector<Obj*> filterObjects(std::vector<Obj>& objs, const double minPT = 0, const double maxEta = 999, bool (*test)(const Obj&) = 0) const {
      const size          numObjects    = objs.size();
      std::vector<Obj*>   outObjs;
      outObjs.reserve(numObjects);
      for(auto& obj : objs){
        if (obj.pt()    < minPT )            continue;
        if (TMath::Abs(obj.eta()) > maxEta)  continue;
        if (test && !(*test)(obj.eta()))     continue;
        outObjs.push_back(&obj);
      }
      return outObjs;
    }

  protected:
    TreeReader	reader; //default reader

  public:
    const bool	isMC;

    //All default readers
    EventInfoReader 	evtInfoReader;
    JetReader		      ak4Reader;
    ElectronReader	  electronReader;
    MuonReader		    muonReader;
    TauReader	      	tauReader;
  };
}

#endif
