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
#include "AnalysisTools/TreeReader/interface/PhotonReader.h"
#include "AnalysisTools/TreeReader/interface/PFCandidateReader.h"
#include "AnalysisTools/TreeReader/interface/GenParticleReader.h"
#include "AnalysisTools/TreeReader/interface/CMSTopReader.h"
#include "AnalysisTools/TreeReader/interface/CORRALReader.h"


namespace ucsbsusy {
class BaseTreeAnalyzer {
public:
    enum VarType {EVTINFO, AK4JETS,PUPPIJETS,PICKYJETS,CASUBJETS, ELECTRONS, MUONS, TAUS, PHOTONS, PFCANDS, GENPARTICLES, CMSTOPS, CORRAL};
    enum TauVetoPresel {MT, DPHI};
    //  enum LeptonSelection  {SEL_0_LEP, SEL_1_LEP,SEL_1_MU,SEL_1_E, SEL_ALL_LEP};

  struct ConfigPars {
  public:
    float minSelEPt;
    float maxSelEETA;
    bool (ElectronF::*selectedElectron)() const;

    float minVetoEPt;
    float maxVetoEETA;
    bool (ElectronF::*vetoedElectron)() const;

    float minSelMuPt;
    float maxSelMuETA;
    float maxSelMuD0;
    float maxSelMuDz;
    bool (MuonF::*selectedMuon)() const;

    float minVetoMuPt;
    float maxVetoMuETA;
    float maxVetoMuD0;
    float maxVetoMuDz;
    bool (MuonF::*vetoedMuon)() const;

    float minVetoTauPt;
    float maxVetoTauETA;
    float maxVetoTauDz;
    bool (PFCandidateF::*vetoedTau)() const;

    float minSelPhoPt;
    float maxSelPhoETA;
    bool (PhotonF::*selectedPhoton)() const;

    //    LeptonSelection leptonSelection;

    float         minJetPt;
    float         minBJetPt;
    float         maxJetEta;
    float         maxBJetEta;
    bool          cleanJetsvSelectedLeptons_;
    float         cleanJetsMaxDR;
    bool          correctPickyPT;
    bool          applyMuIPCuts;
    TauVetoPresel tauVetoPreselection;         
    VarType       defaultJetCollection;

    ConfigPars() :
      minSelEPt (10), //was 32
      maxSelEETA(2.4), //was2.1
      selectedElectron(&ElectronF::isgoodpogelectron),

      minVetoEPt (5),
      maxVetoEETA(2.4),
      vetoedElectron(&ElectronF::ismvavetoelectron),

      minSelMuPt (10), // was 27
      maxSelMuETA(2.4), // was 2.1
      maxSelMuD0(0.02),
      maxSelMuDz(0.1),
      selectedMuon(&MuonF::isgoodpogmuon),

      minVetoMuPt (5),
      maxVetoMuETA(2.4),
      maxVetoMuD0(0.02),
      maxVetoMuDz(0.1),
      vetoedMuon(&MuonF::ismvavetomuon),

      minVetoTauPt (10),
      maxVetoTauETA(2.4),
      maxVetoTauDz(0.2),
      vetoedTau(&PFCandidateF::ismvavetotau),

      minSelPhoPt (10),
      maxSelPhoETA (2.4),
      selectedPhoton(&PhotonF::isloose),

      //      leptonSelection(SEL_0_LEP),

      minJetPt          (20.0),
      minBJetPt         (20.0),
      maxJetEta         (2.4 ),
      maxBJetEta        (2.4 ),
      cleanJetsvSelectedLeptons_(false),
      cleanJetsMaxDR    (0.4),
      correctPickyPT    (true),
      applyMuIPCuts     (true),
      tauVetoPreselection  (MT),
      defaultJetCollection (AK4JETS)
    {}
  };
    
  public:
    BaseTreeAnalyzer(TString fileName, TString treeName, bool isMCTree = false,ConfigPars *pars = 0, TString readOption = "READ");
    virtual ~BaseTreeAnalyzer() {};


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
    bool isLooseBJet   (const RecoJetF& jet) const;
    bool isSelElectron (const ElectronF& electron) const;
    bool isVetoElectron(const ElectronF& electron) const;
    bool isSelMuon     (const MuonF& muon) const;
    bool isVetoMuon    (const MuonF& muon) const;
    bool isVetoTau     (const PFCandidateF& tau) const;
    bool isGoodPhoton  (const PhotonF& pho) const;

    void cleanJets(JetReader * reader,std::vector<RecoJetF*>& jets,std::vector<RecoJetF*>* bJets, std::vector<RecoJetF*>* nonBJets) const;
    double correctedPickyPT(double pt,double eta,double area, double rho) const;

    void selectLeptons(std::vector<LeptonF*>& allLeptons,
                       std::vector<LeptonF*>& selectedLeptons,
                       std::vector<LeptonF*>& vetoedLeptons,
                       std::vector<PFCandidateF*>& vetoedTaus,
                       int& nSelLeptons,
                       int& nVetoedLeptons,
                       int& nVetoedTaus
                       );

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
    JetReader         caSubJetReader        ;
    ElectronReader    electronReader        ;
    MuonReader        muonReader            ;
    TauReader         tauReader             ;
    PhotonReader      photonReader          ;
    PFCandidateReader pfcandReader          ;
    GenParticleReader genParticleReader     ;
    CMSTopReader      cmsTopReader          ;
    CORRALReader      corralReader     ;


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
    int   nSelLeptons;
    int   nVetoedLeptons;
    int   nVetoedTaus;
    int   nJets;
    int   nBJets;

    //--------------------------------------------------------------------------------------------------
    // Stored collections
    //--------------------------------------------------------------------------------------------------
    MomentumF*                 met     ;
    MomentumF*                 genmet  ;
    std::vector<LeptonF*>      allLeptons        ;
    std::vector<LeptonF*>      selectedLeptons   ;
    std::vector<LeptonF*>      vetoedLeptons     ;
    std::vector<PFCandidateF*> vetoedTaus        ;
    std::vector<PhotonF*>      selectedPhotons   ;
    std::vector<RecoJetF*>     jets            ;
    std::vector<RecoJetF*>     bJets   ;
    std::vector<RecoJetF*>     nonBJets;
    std::vector<GenParticleF*> genParts;
    std::vector<CMSTopF*>      cttTops;

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
