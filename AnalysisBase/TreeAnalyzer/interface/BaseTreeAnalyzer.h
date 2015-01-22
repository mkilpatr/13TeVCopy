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
#include "AnalysisTools/TreeReader/interface/GenParticleReader.h"

namespace ucsbsusy {

  class BaseTreeAnalyzer {
  public:
    BaseTreeAnalyzer(TString fileName, TString treeName, bool isMCTree = false, TString readOption = "READ");
    virtual ~BaseTreeAnalyzer() {};

    enum VarType {EVTINFO, AK4JETS,PUPPIJETS,PICKYJETS, ELECTRONS, MUONS, TAUS, GENPARTICLES};

    // Load a variable type to be read from the TTree
    // use the defaultOptions if options is less than 1
    // use the default branch name prefix if set to an empty string
    virtual void load(VarType type, int options = -1, std::string branchName = "" );

    // Same as above but for non-default readers
    void         load(BaseReader * inReader, int options, std::string branchName) { reader.load(inReader,options,branchName); }

    // Load a non-reader variable from the tree
    template<typename varType>
    void setBranchAddress(const TString branchName, varType **var, bool require = false, bool verbose = true)
      { reader.setBranchAddress(branchName,var,false,true); }
    template<typename varType>
    void setBranchAddress(const TString branchName, varType *var, bool require = false, bool verbose = true)
      { reader.setBranchAddress(branchName,var,false,true); }

  public:
    //--------------------------------------------------------------------------------------------------
    // Functions used for basic event processing and loading
    //--------------------------------------------------------------------------------------------------

    // Base function that runs the standard process
    virtual void analyze(int reportFrequency = 10000, int numEvents = -1);

    // Sub processes that can be overloaded
    virtual void loadVariables();       //load variables
    virtual void processVariables();    //event processing
    virtual void runEvent() = 0;        //analysis code

    //--------------------------------------------------------------------------------------------------
    // Standard information
    //--------------------------------------------------------------------------------------------------
    int  getEventNumber() const { return reader.eventNumber;  }
    int  getEntries()     const { return reader.getEntries(); }
    bool isMC()           const { return isMC_;               }
    bool isLoaded()       const { return isLoaded_;           }

    //--------------------------------------------------------------------------------------------------
    // Configuration parameters
    //--------------------------------------------------------------------------------------------------
    void cleanJetsAgainstLeptons(bool clean=true)  { cleanJetsvLeptons_ = clean; }
    void cleanJetsAgainstTaus   (bool clean=true)  { cleanJetsvTaus_ = clean;    }
    void setDefaultJets(VarType type);
    void setDefaultJets(JetReader * injets)        {defaultJets = injets;}
    JetReader * getDefaultJets()                   {return defaultJets;}

    //--------------------------------------------------------------------------------------------------
    // Default processing of physics objects
    //--------------------------------------------------------------------------------------------------
    template <typename Jet>
    bool isGoodJet     (const Jet& jet     ) const {return (jet.pt() > minJetPt && fabs(jet.eta()) < maxJetEta);}
    bool isTightBJet   (const RecoJetF& jet) const;
    bool isMediumBJet  (const RecoJetF& jet) const;
    bool isGoodElectron(const ElectronF& electron) const;
    bool isGoodMuon    (const MuonF& muon        ) const;
    bool isGoodTau     (const TauF& tau          ) const;

    void cleanJets(JetReader * reader,std::vector<RecoJetF*>& jets,std::vector<RecoJetF*>* bJets, std::vector<RecoJetF*>* nonBJets) const;

    //--------------------------------------------------------------------------------------------------
    // TTree readers
    //--------------------------------------------------------------------------------------------------
  protected:
    bool             isLoaded_;
    bool             isProcessed_;
    TreeReader       reader;        // default reader
  public:
    EventInfoReader   evtInfoReader         ;
    JetReader         ak4Reader             ;
    JetReader         puppiJetsReader       ;
    JetReader         pickyJetReader        ;
    ElectronReader    electronReader        ;
    MuonReader        muonReader            ;
    TauReader         tauReader             ;
    GenParticleReader genParticleReader     ;

  public:
    //--------------------------------------------------------------------------------------------------
    // Members to access common information
    //--------------------------------------------------------------------------------------------------
    unsigned int  run;
    unsigned int  lumi;
    unsigned int  event;
    int   nPV;
    float rho;
    int   nLeptons;
    int   nTaus;
    int   nJets;
    int   nBJets;

    //--------------------------------------------------------------------------------------------------
    // Stored collections
    //--------------------------------------------------------------------------------------------------
    MomentumF*                 met     ;
    std::vector<LeptonF*>      leptons ;
    std::vector<TauF*>         taus    ;
    std::vector<RecoJetF*>     jets    ;
    std::vector<RecoJetF*>     pickyJets;
    std::vector<RecoJetF*>     bJets   ;
    std::vector<RecoJetF*>     nonBJets;
    std::vector<GenParticleF*> genParts;

  protected:
    //--------------------------------------------------------------------------------------------------
    // Configuration parameters
    //--------------------------------------------------------------------------------------------------
    const bool   isMC_;
    JetReader  * defaultJets;
    bool         cleanJetsvLeptons_;
    bool         cleanJetsvTaus_;

    //--------------------------------------------------------------------------------------------------
    // Kinematic settings
    //--------------------------------------------------------------------------------------------------
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
