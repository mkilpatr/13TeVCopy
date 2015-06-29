#ifndef ANALYSISBASE_TREEANALYZER_CONFIGURATIONBASE_H
#define ANALYSISBASE_TREEANALYZER_CONFIGURATIONBASE_H


#include "AnalysisTools/DataFormats/interface/Electron.h"
#include "AnalysisTools/DataFormats/interface/Muon.h"
#include "AnalysisTools/DataFormats/interface/PFCandidate.h"
#include "AnalysisTools/DataFormats/interface/Photon.h"


namespace cfgSet {

  enum VarType {NONE, EVTINFO, AK4JETS,PUPPIJETS,PICKYJETS,CASUBJETS, ELECTRONS, MUONS, TAUS, PHOTONS, PFCANDS, GENPARTICLES, CMSTOPS, CORRAL};

  //base used for all future ConfigSets
  class BaseConfig {
  public:
    BaseConfig(TString inName) : isConfigured(false), name(inName) {};
    void setConfig() {isConfigured = true;}
    bool isConfig() const {return isConfigured;}
    TString getName() const {return name;}
    virtual ~BaseConfig() {};
  private:
    bool isConfigured;
    TString name;
  };
//  BaseConfig::~BaseConfig() { }

  //The actual sets that are used in the analysis....
  class JetConfig : public BaseConfig {
  public:

    VarType jetCollection;

    float   minPt     ;
    float   maxEta    ;
    float   minBJetPt ;
    float   maxBJetEta;
    float   defaultCSV;

    bool  applyJetID               ;
    bool  applyAdHocPUCorr         ;
    bool  cleanJetsvSelectedLeptons;
    bool  cleanJetsvSelectedPhotons;
    float cleanJetsMaxDR           ;


    JetConfig(TString inName = "NULL") :BaseConfig(inName),
      jetCollection(NONE),
      minPt         (-1),
      maxEta        (-1),
      minBJetPt     (-1),
      maxBJetEta    (-1),
      defaultCSV    (-1),
      applyJetID    (false),
      applyAdHocPUCorr         (false),
      cleanJetsvSelectedLeptons(false),
      cleanJetsvSelectedPhotons(false),
      cleanJetsMaxDR           (-1)
    {};
    virtual ~JetConfig() {};
  };

  class LeptonConfig : public BaseConfig {
  public:

    float   minEPt     ;
    float   maxEEta    ;
    bool    (ucsbsusy::ElectronF::*selectedElectron)() const;

    float   minMuPt     ;
    float   maxMuEta    ;
    float   maxMuD0     ;
    float   maxMuDz     ;
    bool    (ucsbsusy::MuonF::*selectedMuon)() const;

    LeptonConfig(TString inName = "NULL") :BaseConfig(inName),
      minEPt  (-1),
      maxEEta (-1),
      selectedElectron(0),
      minMuPt         (-1),
      maxMuEta        (-1),
      maxMuD0         (-1),
      maxMuDz         (-1),
      selectedMuon(0)
    {};
    virtual ~LeptonConfig() {};
  };

  class TrackConfig : public BaseConfig {
  public:

    float   minPt     ;
    float   maxEta    ;
    bool    mtPresel   ; //only used if pre-defined
    float   maxDz     ;
    bool    (ucsbsusy::PFCandidateF::*selected)() const;

    TrackConfig(TString inName = "NULL") :BaseConfig(inName),
      minPt  (-1),
      maxEta (-1),
      mtPresel(false),
      maxDz(-1),
      selected(0)
    {};
    virtual ~TrackConfig() {};
  };

  class PhotonConfig : public BaseConfig {
  public:

    float   minPt     ;
    float   maxEta    ;
    bool    (ucsbsusy::PhotonF::*selected)() const;

    PhotonConfig(TString inName = "NULL") :BaseConfig(inName),
      minPt  (-1),
      maxEta (-1),
      selected(0)
    {};
    virtual ~PhotonConfig() {};
  };


  class METConfig : public BaseConfig {
  public:
    bool addSelectedLeptonsToMET;
    bool addSelectedPhotonsToMET;
    METConfig(TString inName = "NULL") :BaseConfig(inName),
      addSelectedLeptonsToMET(false),
      addSelectedPhotonsToMET(false)
    {};
    virtual ~METConfig() {};
  };


  //The collection of default configs
  struct ConfigSet{
    JetConfig    jets           ;
    LeptonConfig selectedLeptons;
    LeptonConfig vetoedLeptons  ;
    TrackConfig  vetoedTracks   ;
    PhotonConfig selectedPhotons;
    METConfig    met            ;
    ConfigSet() :
      jets            (),
      selectedLeptons (),
      vetoedLeptons   (),
      vetoedTracks    (),
      selectedPhotons (),
      met             ()
    {
    }
  };






}



#endif
