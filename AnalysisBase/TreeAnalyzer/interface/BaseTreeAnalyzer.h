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
    //--------------------------------------------------------------------------------------------------
    // functions used for basic event processing and loading
    //--------------------------------------------------------------------------------------------------
  public:
    BaseTreeAnalyzer(TString fileName, TString treeName, bool isMCTree = false, TString readOption = "READ");
    virtual ~BaseTreeAnalyzer() {};

    enum VarType {EVTINFO, AK4JETS, ELECTRONS, MUONS, TAUS};
    //Load a variable type to be read from the TTree
    //use the defaultOptions if options is less than 1
    //use the default branch name prefix if set to an empty string
    virtual void	load(VarType type, int options = -1, std::string branchName = "" );
    //same as above but for non-default readers
    void	load(BaseReader * inReader, int options, std::string branchName) {reader.load(inReader,options,branchName);}
    //load a non-reader variable from the tree
    template<typename varType>
    void setBranchAddress(const TString branchName, varType **var, bool require = false, bool verbose = true)
      { reader.setBranchAddress(branchName,var,false,true);}
    template<typename varType>
    void setBranchAddress(const TString branchName, varType *var, bool require = false, bool verbose = true)
      { reader.setBranchAddress(branchName,var,false,true);}

  public:
    //Base function that runs the standard process
    virtual void analyze(int reportFrequency = 10000);
    //Sub processes that can be overloaded
    virtual void loadVariables();   //load variables
    virtual void processVariables();//event processing
    virtual void run() = 0;             //analysis code


    int		getEventNumber() const	{ return reader.eventNumber;	}
    int		getEntries()	  const	{ return reader.getEntries();	}
    bool  isMC()          const {return isMC_;}
    bool  isLoaded()      const {return isLoaded_;}

    //--------------------------------------------------------------------------------------------------
    // Configuration parameters
    //--------------------------------------------------------------------------------------------------
  public:
    void    cleanJetsAgainstLeptons(bool clean=true)  { cleanJetsvLeptons = clean;  }
    void    cleanJetsAgainstTaus(bool clean=true)   { cleanJetsvTaus = clean; }

    //--------------------------------------------------------------------------------------------------
    // Default processing of physics objects
    //--------------------------------------------------------------------------------------------------
    bool isGoodJet    (const RecoJetF& jet) const;
    bool isTightBJet  (const RecoJetF& jet) const;
    bool isMediumBJet (const RecoJetF& jet) const;

    bool isGoodElectron   (const ElectronF& electron) const;
    bool isGoodMuon       (const MuonF& muon        ) const;
    bool isGoodTau        (const TauF& tau          ) const;

    //--------------------------------------------------------------------------------------------------
    // Accessors to common information
    //--------------------------------------------------------------------------------------------------
  public:
    unsigned int  run()   const { return evtInfoReader.run;   }
    unsigned int  lumi()  const { return evtInfoReader.lumi;    }
    unsigned int  event() const { return evtInfoReader.event;   }
    int           nPV()   const { return evtInfoReader.nPV;   }

    int   nLeptons() const {return leptons.size();}
    int   nTaus()    const {return taus.size();   }
    int   nJets()    const {return jets.size();   }
    int   nBJets()   const {return bJets.size();  }

    //--------------------------------------------------------------------------------------------------
    // TTree readers
    //--------------------------------------------------------------------------------------------------
  protected:
    bool        isLoaded_;
    TreeReader	reader; //default reader
  public:
    EventInfoReader  evtInfoReader ;
    JetReader        ak4Reader     ;
    ElectronReader   electronReader;
    MuonReader       muonReader    ;
    TauReader        tauReader     ;

    //--------------------------------------------------------------------------------------------------
    // Stored collections
    //--------------------------------------------------------------------------------------------------
  public:
    MomentumF*               met      ;
    std::vector<LeptonF*>    leptons  ;
    std::vector<TauF*>       taus    ;
    std::vector<RecoJetF*>   jets    ;
    std::vector<RecoJetF*>   bJets   ;
    std::vector<RecoJetF*>   nonBJets;

    //--------------------------------------------------------------------------------------------------
    // configuration parameters
    //--------------------------------------------------------------------------------------------------
  protected:
    const bool	isMC_;
    bool    cleanJetsvLeptons;
    bool    cleanJetsvTaus;

    //--------------------------------------------------------------------------------------------------
    // Kinematic settings
    //--------------------------------------------------------------------------------------------------
  protected:
    const float  minElePt;
    const float  minMuPt;
    const float  minTauPt;
    const float  minJetPt;
    const float  minBJetPt;
    const float  maxEleEta;
    const float  maxMuEta;
    const float  maxTauEta;
    const float  maxJetEta;
    const float  maxBJetEta;
    const float  minJetLepDR;
  };
}

#endif
