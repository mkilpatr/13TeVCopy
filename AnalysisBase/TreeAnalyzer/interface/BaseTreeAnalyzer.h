//--------------------------------------------------------------------------------------------------
//
// BaseTreeAnalyzer
//
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISBASE_TREEANALYZER_BASETREEANALYZER_H
#define ANALYSISBASE_TREEANALYZER_BASETREEANALYZER_H

#include <memory>
#include <string>
#include <vector>
#include <assert.h>
#include <TString.h>
#include <TTree.h>
#include <TSystem.h>

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
#include "AnalysisTools/TreeReader/interface/CORRALReader.h"
#include "AnalysisTools/TreeReader/interface/TriggerObjectReader.h"
#include "AnalysisTools/TreeReader/interface/FatJetReader.h"
#include "AnalysisTools/TreeReader/interface/SVReader.h"
#include "AnalysisTools/TreeReader/interface/HTTReader.h"

#include "AnalysisTools/ObjectSelection/interface/ResolvedTopMVA.h"
#include "AnalysisTools/ObjectSelection/interface/SoftdropWTopMulticlassMVA.h"

namespace ucsbsusy {
class BaseEventAnalyzer {
public:
  BaseEventAnalyzer() {};
  virtual ~BaseEventAnalyzer() {};
  virtual void analyzeEvent(BaseTreeAnalyzer * ana, int reportFrequency = 10000, int numEvents = -1, int startEvent = -1);
};



class BaseTreeAnalyzer {
public:

  public:
    BaseTreeAnalyzer(TString fileName, TString treeName,size randomSeed, bool isMCTree,cfgSet::ConfigSet *pars);
    virtual ~BaseTreeAnalyzer() {}

    // Load a variable type to be read from the TTree
    // use the defaultOptions if options is less than 1
    // use the default branch name prefix if set to an empty string
    virtual void load(cfgSet::VarType type, int options = -1, std::string branchName = "" );

    // Same as above but for non-default readers
    void         load(BaseReader * inReader, int options, std::string branchName) { reader.load(inReader,options,branchName); }

    // Load a non-reader variable from the tree
    template<typename varType>
    void setBranchAddress(const std::string  branchName,const std::string varName, varType **var, bool require = false, bool verbose = true)
      { reader.setBranchAddress(branchName,varName,var,false,true); }
    template<typename varType>
    void setBranchAddress(const std::string  branchName, const std::string varName, varType *var, bool require = false, bool verbose = true)
      { reader.setBranchAddress(branchName,varName,var,false,true); }

  public:
    //--------------------------------------------------------------------------------------------------
    // Functions used for basic event processing and loading
    //--------------------------------------------------------------------------------------------------

    //Used to set the particular event analyzer used
    virtual BaseEventAnalyzer * setupEventAnalyzer() {return new BaseEventAnalyzer();}

    //load the next event
    bool nextEvent(const int reportFrequency)  {return reader.nextEvent(reportFrequency);}

    void setEventNumber(const int newEventNumber) {reader.eventNumber = newEventNumber;}

    // Base function that runs the standard process
    virtual void analyze(int reportFrequency = 10000, int numEvents = -1, int startEvent = -1);

    // Sub processes that can be overloaded
    virtual void loadVariables();       //load variables
    virtual void clearVariables();      //clear event variables
    virtual void processVariables();    //event processing
    virtual void runEvent() = 0;        //analysis code

    virtual bool processData(); //process data sample

    //--------------------------------------------------------------------------------------------------
    // Standard information
    //--------------------------------------------------------------------------------------------------
    const cfgSet::ConfigSet& getAnaCfg() const { return configSet;}
    TRandom3 * getRndGen()  { return randGen;}
    int  getEventNumber() const { return reader.eventNumber;  }
    int  getEntries()     const { return reader.getEntries(); }
    bool isMC()           const { return isMC_;               }
    void setLoaded(bool is)     { isLoaded_ = is;           }
    bool isLoaded()       const { return isLoaded_;           }
    void setProcessed(bool is)  { isProcessed_ = is;           }
    bool isProcessed()    const { return isProcessed_;           }
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
    TRandom3*           randGen;
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
    FatJetReader        fatJetReader        ;
    //FatJetReader        fatJetPuppiReader   ;
    CORRALReader        corralReader        ;
    TriggerObjectReader trigObjReader       ;
    SVReader            svReader            ;
    HTTReader           httReader           ;

    ResolvedTopMVA*     resTopMVA = nullptr ;
    SoftdropWTopMulticlassMVA* wtopMVA = nullptr;


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
    float nPU;
    float rho;
    int   nSelLeptons;
    int   nPrimaryLeptons;
    int   nSecondaryLeptons;
    int   nVetoedTracks;
    int   nJets;
    int   nBJets;
    int   nVetoHPSTaus;
    LeptonF* selectedLepton; //"Primary lepton" if there is more than one in the selected leptons collection it is chosen randomly
    int   nSdMVATopTight = 0;
    int   nSdMVAWTight = 0;
    int   nHadronicGenTops = 0;
    int   nHadronicGenWs = 0;
    int   nResMVATopMedium = 0;
    int   nSelSdTops = 0;
    int   nSelSdWs = 0;

    //--------------------------------------------------------------------------------------------------
    // Stored collections
    //--------------------------------------------------------------------------------------------------
    MomentumF* met        ;
    MomentumF* metNoHF    ;
    MomentumF* puppimet   ;
    MomentumF* genmet     ;
    bool       goodvertex ;
    std::unique_ptr<PartonMatching::PartonEvent>  partonEvent; //for hadronicGenTops collection
    std::vector<LeptonF*>        allLeptons        ; //All leptons in the tree w/o selection
    std::vector<LeptonF*>        selectedLeptons   ; //All leptons that pass either the primary or secondary selection
    std::vector<LeptonF*>        primaryLeptons    ; //All leptons that pass the primary (tighter) selection
    std::vector<LeptonF*>        secondaryLeptons  ; //All leptons that pass the secondary (looser) selection but do not pass the primary selection
    std::vector<PFCandidateF*>   vetoedTracks      ;
    std::vector<TauF*>           vetoedTaus        ;
    std::vector<PhotonF*>        selectedPhotons   ;
    std::vector<RecoJetF*>       jets              ;
    std::vector<RecoJetF*>       bJets             ;
    std::vector<RecoJetF*>       isrJets           ;
    std::vector<RecoJetF*>       nonBJets          ;
    std::vector<GenParticleF*>   genParts          ;
    std::vector<FatJetF*>        sdMVATopTight     ;
    std::vector<FatJetF*>        sdMVAWTight       ;
    std::vector<PartonMatching::TopDecay*> hadronicGenTops   ;
    std::vector<PartonMatching::BosonDecay*> hadronicGenWs   ;
    std::vector<TopCand>         resMVATopCands    ; // All resTop candidates after removing overlaps: sorted by MVA score
    std::vector<TopCand>         resMVATopMedium   ; // resTops passing Medium WP: sorted by PT
    std::vector<FatJetF*>        selectedSdTops    ;
    std::vector<FatJetF*>        selectedSdWs      ;
    std::vector<FatJetF*>        fatJets;
    std::vector<FatJetF*>        ak8isrJets        ;
    //    std::vector<FatJetF*>        fatJetsPuppi;
    std::vector<TriggerObjectF*> triggerObjects    ;
    std::vector<TriggerInfo*>    triggerInfo       ;
    std::vector<SVF*>            SVs;
    std::vector<HTTFatJetF*>     httTops           ;


    //--------------------------------------------------------------------------------------------------
    // Correction sets
    //--------------------------------------------------------------------------------------------------
    TtbarCorrectionSet  ttbarCorrections;
    WPolCorrectionSet   wpolCorrections;
    EventCorrectionSet  eventCorrections;
    TriggerCorrectionSet triggerCorrections;
    LeptonCorrectionSet leptonCorrections;
    JetAndMETCorrectionSet  jetAndMETCorrections;
    BTagCorrectionSet    bTagCorrections;
    ISRCorrectionSet    isrCorrections;

    JetReader  * defaultJets;

  protected:
    //--------------------------------------------------------------------------------------------------
    // Configuration parameters
    //--------------------------------------------------------------------------------------------------
    const bool   isMC_;
    cfgSet::ConfigSet   configSet;
    std::vector<CorrectionSet*> corrections;

    bool updateMVA_; // FIXME
};


}


#endif
