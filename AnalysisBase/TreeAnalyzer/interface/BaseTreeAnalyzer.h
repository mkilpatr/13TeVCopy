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
struct ConfigPars {
public:
  float        minElePt;
  float        minMuPt;
  float        minTauPt;
  float        minJetPt;
  float        minBJetPt;
  float        maxEleEta;
  float        maxMuEta;
  float        maxTauEta;
  float        maxJetEta;
  float        maxBJetEta;
  float        minJetLepDR;
  bool         cleanJetsvLeptons_;
  bool         cleanJetsvTaus_;

  ConfigPars() :
     minElePt          (10.0),
     minMuPt           (10.0),
     minTauPt          (20.0),
     minJetPt          (30.0),
     minBJetPt         (30.0),
     maxEleEta         (2.5 ),
     maxMuEta          (2.4 ),
     maxTauEta         (2.3 ),
     maxJetEta         (2.4 ),
     maxBJetEta        (2.4 ),
     minJetLepDR       (0.4 ),
     cleanJetsvLeptons_(false),
     cleanJetsvTaus_   (false)
  {}
};

  class BaseTreeAnalyzer {
  public:
    BaseTreeAnalyzer(TString fileName, TString treeName, bool isMCTree = false,ConfigPars *pars = 0, TString readOption = "READ");
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
    void setDefaultJets(VarType type);
    void setDefaultJets(JetReader * injets)        {defaultJets = injets;}
    JetReader * getDefaultJets()                   {return defaultJets;}

    //--------------------------------------------------------------------------------------------------
    // Default processing of physics objects
    //--------------------------------------------------------------------------------------------------
    template <typename Jet>
    bool isGoodJet     (const Jet& jet     ) const {return (jet.pt() > config.minJetPt && fabs(jet.eta()) < config.maxJetEta);}
    bool isTightBJet   (const RecoJetF& jet) const;
    bool isMediumBJet  (const RecoJetF& jet) const;
    bool isLooseBJet  (const RecoJetF& jet) const;
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
    float         weight;
    defaults::Process process;

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
    MomentumF*                 genmet  ;
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
    const ConfigPars config;
  };

}

#endif
