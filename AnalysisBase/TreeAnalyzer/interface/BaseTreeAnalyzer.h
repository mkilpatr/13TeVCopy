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

#include "AnalysisBase/TreeAnalyzer/interface/DefaultConfigurations.h"

#include "AnalysisTools/TreeReader/interface/TreeReader.h"
#include "AnalysisTools/TreeReader/interface/EventInfoReader.h"
#include "AnalysisTools/TreeReader/interface/JetReader.h"
#include "AnalysisTools/TreeReader/interface/ElectronReader.h"
#include "AnalysisTools/TreeReader/interface/MuonReader.h"
#include "AnalysisTools/TreeReader/interface/TauReader.h"
#include "AnalysisTools/TreeReader/interface/PhotonReader.h"
#include "AnalysisTools/TreeReader/interface/PFCandidateReader.h"
#include "AnalysisTools/TreeReader/interface/GenParticleReader.h"
#include "AnalysisTools/TreeReader/interface/CMSTopReader.h"
#include "AnalysisTools/TreeReader/interface/CORRALReader.h"
#include "AnalysisTools/TreeReader/interface/TriggerObjectReader.h"
#include "AnalysisBase/TreeAnalyzer/interface/JetCorrections.h"


namespace ucsbsusy {
class BaseTreeAnalyzer {
public:
    
  public:
    BaseTreeAnalyzer(TString fileName, TString treeName, bool isMCTree = false,cfgSet::ConfigSet *pars = 0, TString readOption = "READ");
    virtual ~BaseTreeAnalyzer() {};


    // Load a variable type to be read from the TTree
    // use the defaultOptions if options is less than 1
    // use the default branch name prefix if set to an empty string
    virtual void load(cfgSet::VarType type, int options = -1, std::string branchName = "" );

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

    virtual bool processData(); //process data sample

    //--------------------------------------------------------------------------------------------------
    // Standard information
    //--------------------------------------------------------------------------------------------------
    cfgSet::ConfigSet getAnaCfg() const { return configSet;}
    int  getEventNumber() const { return reader.eventNumber;  }
    int  getEntries()     const { return reader.getEntries(); }
    bool isMC()           const { return isMC_;               }
    bool isLoaded()       const { return isLoaded_;           }
    bool hasJSONFile()    const { return configSet.jsonFile != ""; }
    bool passesLumiMask() const {
      return configSet.jsonProcessing->hasRunLumi(run, lumi);
    }


    //--------------------------------------------------------------------------------------------------
    // TTree readers
    //--------------------------------------------------------------------------------------------------
  protected:
    bool             isLoaded_;
    bool             isProcessed_;
    TreeReader       reader;        // default reader
  public:
    EventInfoReader     evtInfoReader       ;
    JetReader           ak4Reader           ;
    JetReader           puppiJetsReader     ;
    JetReader           pickyJetReader      ;
    JetReader           caSubJetReader      ;
    ElectronReader      electronReader      ;
    MuonReader          muonReader          ;
    TauReader           tauReader           ;
    PhotonReader        photonReader        ;
    PFCandidateReader   pfcandReader        ;
    GenParticleReader   genParticleReader   ;
    CMSTopReader        cmsTopReader        ;
    CORRALReader        corralReader        ;
    TriggerObjectReader trigObjReader       ;
    JetCorrector        jetCorrector        ;


  public:
    //--------------------------------------------------------------------------------------------------
    // Members to access common information
    //--------------------------------------------------------------------------------------------------
    unsigned int       run;
    unsigned int       lumi;
    unsigned int       event;
    float              weight;
    defaults::Process  process;
    defaults::DataReco datareco;
    unsigned long      triggerflag;

    int   nPV;
    float rho;
    int   nSelLeptons;
    int   nVetoedLeptons;
    int   nVetoedTracks;
    int   nJets;
    int   nBJets;
    int   nVetoHPSTaus;

    //--------------------------------------------------------------------------------------------------
    // Stored collections
    //--------------------------------------------------------------------------------------------------
    MomentumF* met        ;
    MomentumF* metNoHF    ;
    MomentumF* genmet     ;
    bool       goodvertex ;
    std::vector<LeptonF*>        allLeptons        ;
    std::vector<LeptonF*>        selectedLeptons   ;
    std::vector<LeptonF*>        vetoedLeptons     ;
    std::vector<PFCandidateF*>   vetoedTracks      ;
    std::vector<TauF*>           vetoedTaus        ;
    std::vector<PhotonF*>        selectedPhotons   ;
    std::vector<RecoJetF*>       jets              ;
    std::vector<RecoJetF*>       bJets             ;
    std::vector<RecoJetF*>       nonBJets          ;
    std::vector<GenParticleF*>   genParts          ;
    std::vector<CMSTopF*>        cttTops           ;
    std::vector<TriggerObjectF*> triggerObjects    ;
    std::vector<TriggerInfo*>    triggerInfo       ;
    std::vector<TauF*>           HPSTaus           ;

    //--------------------------------------------------------------------------------------------------
    // Correction sets
    //--------------------------------------------------------------------------------------------------
    TtbarCorrectionSet  ttbarCorrections;
    EventCorrectionSet  eventCorrections;
    JetCorrectionSet    jetCorrections;
    JetAndMETCorrectionSet  jetAndMETCorrections;


    //hack for DY PU
    bool zIsInvisible;

  protected:
    //--------------------------------------------------------------------------------------------------
    // Configuration parameters
    //--------------------------------------------------------------------------------------------------
    const bool   isMC_;
    JetReader  * defaultJets;
    cfgSet::ConfigSet   configSet;
    std::vector<CorrectionSet*> corrections;
  };


}


#endif
