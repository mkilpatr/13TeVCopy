#ifndef ANALYSISBASE_TREEANALYZER_CONFIGURATIONBASE_H
#define ANALYSISBASE_TREEANALYZER_CONFIGURATIONBASE_H



#include "AnalysisTools/ObjectSelection/interface/LeptonId.h"
#include "AnalysisTools/DataFormats/interface/Tau.h"
#include "AnalysisTools/DataFormats/interface/PFCandidate.h"
#include "AnalysisTools/DataFormats/interface/Photon.h"
#include "AnalysisBase/TreeAnalyzer/interface/JSONProcessing.h"
#include "AnalysisBase/TreeAnalyzer/interface/TtbarCorrectionSet.h"
#include "AnalysisBase/TreeAnalyzer/interface/WPolCorrectionSet.h"
#include "AnalysisBase/TreeAnalyzer/interface/EventCorrectionSet.h"
#include "AnalysisBase/TreeAnalyzer/interface/TriggerCorrectionSet.h"
#include "AnalysisBase/TreeAnalyzer/interface/LeptonCorrectionSet.h"
#include "AnalysisBase/TreeAnalyzer/interface/BTagCorrectionSet.h"
#include "AnalysisBase/TreeAnalyzer/interface/JetAndMETCorrectionSet.h"
#include "AnalysisBase/TreeAnalyzer/interface/ISRCorrectionSet.h"


#include <iostream>

namespace cfgSet {

  enum VarType {NONE, EVTINFO, AK4JETS,PUPPIJETS,PICKYJETS,CASUBJETS, ELECTRONS, MUONS, TAUS, PHOTONS, PFCANDS, GENPARTICLES, CMSTOPS, AK8FATJETS, AK8PUPPIFATJETS, CORRAL, TRIGOBJS, SV};

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
    bool  cleanJetsvLeptons;
    bool  ignoreSeconaryLeptonsWhenCleaning  ;
    bool  cleanJetsvPhotons;
    bool  cleanJetsvTracks   ;
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

      cleanJetsvLeptons(false),
      ignoreSeconaryLeptonsWhenCleaning  (false),
      cleanJetsvPhotons(false),
      cleanJetsvTracks  (false),
      cleanJetsMaxDR           (-1)

    {};
    virtual ~JetConfig() {};

    friend std::ostream& operator<<(std::ostream& os, const JetConfig& a){
      os << "Printing out jet selection information" << std::endl;//<< a.jetCollection <<std::endl;
      os << "The min jet Pt is"<< a.minPt <<std::endl;
      os << "The max jet eta is "<< a.maxEta <<std::endl;
      os << "The min jet pt is "<< a.minBJetPt <<std::endl;
      os << "The max bJet eta is "<< a.maxBJetEta <<std::endl;
      os << "The default CSV is "<< a.defaultCSV <<std::endl;
      if(a.applyJetID) os << "Apply JetID enabled" <<std::endl; else os << "Apply JetID disabled" << std::endl;
      if(a.applyAdHocPUCorr) os << "Applying AdHoc PU Correction" <<std::endl; else os << "No AdHoc PU Correction Applied" << std::endl;
      if(a.cleanJetsvLeptons) os << "Cleaning Jets vs. leptons is enabled" <<std::endl; else os << "Cleaning Jets vs. leptons is disabled" << std::endl;
      if(a.ignoreSeconaryLeptonsWhenCleaning) os << "Ignoring secondary leptons when cleaning jets" <<std::endl;
      if(a.cleanJetsvPhotons) os << "Cleaning Jets vs. Selected Photons is enabled" <<std::endl; else os << "Cleaning Jets vs. Selected Photons is disabled" << std::endl;
      if(a.cleanJetsvTracks) os << "Cleaning Jets vs. Vetoed Tracks is enabled" <<std::endl; else os << "Cleaning Jets vs. Vetoed Tracks is disabled" << std::endl;
      if(a.cleanJetsMaxDR) os << "Cleaning Jets max DR enabled" <<std::endl; else os << "Cleaning Jets max DR is disabled" << std::endl;
      return os;
    };
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

    friend std::ostream& operator<<(std::ostream& os, const TrackConfig& a){
      os << "Printing out track selection information" << std::endl;//<< a.jetCollection <<std::endl;
      os << "The min track Pt is "<< a.minPt <<std::endl;
      os << "The max track eta is "<< a.maxEta <<std::endl;
      if(a.mtPresel == 1)
      os << "The mt presel is on " << std::endl;
      else
      os << "The mt presel is off " << std::endl;
      os << "The max track Dz is "<< a.maxDz <<std::endl;
      return os;
    };
  };

  class TauConfig : public BaseConfig {
  public:

    float   minPt;
    float   maxEta;
    float    minDeltaRFromSelLepton;
    bool    requireOppositeQToSelLepton;
    bool    (ucsbsusy::TauF::*selected)() const;

    TauConfig(TString inName = "NULL") :BaseConfig(inName),
      minPt  (-1),
      maxEta (-1),
      selected(0)
    {};
    virtual ~TauConfig() {};

    friend std::ostream& operator<<(std::ostream& os, const TauConfig& a){
      os << "Printing out tau selection information" << std::endl;//<< a.jetCollection <<std::endl;
      os << "The min tau Pt is "<< a.minPt <<std::endl;
      os << "The max tau eta is "<< a.maxEta <<std::endl;
      return os;
    };
  };

  class PhotonConfig : public BaseConfig {
  public:

    float   minPt     ;
    float   maxEta    ;
    bool    (ucsbsusy::PhotonF::*selected)() const;
    bool    usePixelSeedVeto;
    bool    useElectronVeto;

    PhotonConfig(TString inName = "NULL") :BaseConfig(inName),
      minPt  (-1),
      maxEta (-1),
      selected(0),
      usePixelSeedVeto(false),
      useElectronVeto(false)
    {};
    virtual ~PhotonConfig() {};

    friend std::ostream& operator<<(std::ostream& os, const PhotonConfig& a){
      os << "Printing out photon selection information" << std::endl;//<< a.jetCollection <<std::endl;
      os << "The min photon Pt is "<< a.minPt <<std::endl;
      os << "The max photon eta is "<< a.maxEta <<std::endl;
      return os;
    };

  };

  class CorrectionConfig : public BaseConfig {
  public:
    int ttbarCorrections;
    int wpolCorrections;
    int eventCorrections;
    int triggerCorrections;
    int puCorrections;
    int leptonCorrections;
    int jetAndMETCorrections;


    TString ttbarCorrectionFile;
    TString eventCorrectionFile;
    TString cttCorrectionFile;
    TString puCorrectionFile;
    ucsbsusy::CORRTYPE cttEffSFType;
    ucsbsusy::CORRTYPE cttMistagSFType;


    TString triggerCorrectionFile;
    ucsbsusy::CORRTYPE trigPhotonCorrType;
    ucsbsusy::CORRTYPE trigElectronCorrType;
    ucsbsusy::CORRTYPE trigMuonCorrType;
    ucsbsusy::CORRTYPE trigMetOrLepCorrType;


    TString leptonCorrectionFile;
    TString leptonCorrectionFile2;
    ucsbsusy::CORRTYPE tnpElCorrType;
    ucsbsusy::CORRTYPE tnpMuCorrType;
    //ucsbsusy::CORRTYPE tnpElIdCorrType;
    //ucsbsusy::CORRTYPE tnpElIsoCorrType;
    //ucsbsusy::CORRTYPE tnpMuIdCorrType;
    //ucsbsusy::CORRTYPE tnpMuIsoCorrType;


    int bTagCorrections;
    TString bTagEffFile;
    TString bTagSFFile;
    ucsbsusy::CORRTYPE heavyBTagCorrType;
    ucsbsusy::CORRTYPE lightBTagCorrType;
    TString bTagFastSimEffFile;
    TString bTagFastSimSFFile;
    ucsbsusy::CORRTYPE heavyFastSimBTagCorrType;
    ucsbsusy::CORRTYPE lightFastSimBTagCorrType;

    TString jetResFile;
    TString jetResCorrFile;
    ucsbsusy::CORRTYPE jetResCorrType;

    ucsbsusy::CORRTYPE jetScaleCorr;


    TString jetResTailFile;
    ucsbsusy::CORRTYPE jetResTailCorrType;

    ucsbsusy::CORRTYPE metResSystRunIType;
    ucsbsusy::CORRTYPE metResSystRunIIType;


    int isrCorrections;
    TString isrCorrFile;
    TString isrSigNormFile;
    TString isrSigNormTightFile;
    std::vector<TString> isrSigNorms;
    ucsbsusy::CORRTYPE isrType;

    CorrectionConfig(TString inName = "NULL") :BaseConfig(inName),
        ttbarCorrections(ucsbsusy::TtbarCorrectionSet::NULLOPT),
        wpolCorrections(ucsbsusy::WPolCorrectionSet::NULLOPT),
        eventCorrections(ucsbsusy::EventCorrectionSet::NULLOPT),
        triggerCorrections(ucsbsusy::TriggerCorrectionSet::NULLOPT),
        puCorrections(ucsbsusy::EventCorrectionSet::NULLOPT),
        leptonCorrections(ucsbsusy::LeptonCorrectionSet::NULLOPT),
        jetAndMETCorrections(ucsbsusy::EventCorrectionSet::NULLOPT),
        cttEffSFType(ucsbsusy::NONE),
        cttMistagSFType(ucsbsusy::NONE),
        trigPhotonCorrType(ucsbsusy::NONE),
        trigElectronCorrType(ucsbsusy::NONE),
        trigMuonCorrType(ucsbsusy::NONE),

        tnpElCorrType(ucsbsusy::NONE),
        tnpMuCorrType(ucsbsusy::NONE),
        //tnpElIdCorrType(ucsbsusy::NONE),
        //tnpElIsoCorrType(ucsbsusy::NONE),
        //tnpMuIdCorrType(ucsbsusy::NONE),
        //tnpMuIsoCorrType(ucsbsusy::NONE),
        bTagCorrections(ucsbsusy::BTagCorrectionSet::NULLOPT),
        heavyBTagCorrType(ucsbsusy::NONE),
        lightBTagCorrType(ucsbsusy::NONE),
        heavyFastSimBTagCorrType(ucsbsusy::NONE),
        lightFastSimBTagCorrType(ucsbsusy::NONE),
        jetResCorrType(ucsbsusy::NONE),
        jetScaleCorr(ucsbsusy::NONE),
        jetResTailFile(1),
      jetResTailCorrType(ucsbsusy::NONE),
      metResSystRunIType(ucsbsusy::NONE),
      metResSystRunIIType(ucsbsusy::NONE),
      isrCorrections(ucsbsusy::ISRCorrectionSet::NULLOPT),
      isrType(ucsbsusy::NONE)
    {};
    friend std::ostream& operator<<(std::ostream& os, const CorrectionConfig& a){
      if(a.ttbarCorrections != ucsbsusy::TtbarCorrectionSet::NULLOPT){
        os << "Applying ttbar corrections from " << a.ttbarCorrectionFile.Data() <<" -> ";
        if(a.ttbarCorrections & ucsbsusy::TtbarCorrectionSet::TOPPAIRPT)
          os << "TOPPAIRPT ";
        os << std::endl;

      }

      if(a.wpolCorrections != ucsbsusy::WPolCorrectionSet::NULLOPT){
        os << "Applying wpol = ";
	if(a.wpolCorrections & ucsbsusy::WPolCorrectionSet::WPOLWGT)   { os << "WPOL WGT ";   }
        os << std::endl;

      }

      if(a.eventCorrections != ucsbsusy::EventCorrectionSet::NULLOPT){
        os << "Applying event corrections from " << a.eventCorrectionFile.Data() <<" -> ";
        if(a.eventCorrections & ucsbsusy::EventCorrectionSet::PU)
          os << "PU ";
        if(a.eventCorrections & ucsbsusy::EventCorrectionSet::NORM)
          os << "NORM ";
        if(a.eventCorrections & ucsbsusy::EventCorrectionSet::CTT)
          os << " CTT Eff("<< a.cttEffSFType <<") Mistag("<<a.cttMistagSFType<<")";
        os << std::endl;

      }

      if(a.triggerCorrections != ucsbsusy::TriggerCorrectionSet::NULLOPT){
        os << "Applying trigger corrections from " << a.triggerCorrectionFile.Data() <<" -> ";
        if(a.triggerCorrections & ucsbsusy::TriggerCorrectionSet::PHOTON)
          os << "TrigPhoton ";
        if(a.triggerCorrections & ucsbsusy::TriggerCorrectionSet::ELECTRON)
          os << "TrigElectron ";
        if(a.triggerCorrections & ucsbsusy::TriggerCorrectionSet::MUON)
          os << "TrigMuon ";
        if(a.triggerCorrections & ucsbsusy::TriggerCorrectionSet::MET_OR_LEP)
          os << "TrigMetOrLep ";
        os << std::endl;

      }


      if(a.bTagCorrections != ucsbsusy::BTagCorrectionSet::NULLOPT){
        if(a.bTagCorrections & ucsbsusy::BTagCorrectionSet::BYEVTWEIGHT){
          os << "Applying bTag corrections from " << a.bTagEffFile.Data()<<","<<a.bTagSFFile.Data() <<" -> ";
          os << "BYEVTWEIGHT H("<<corrTypeName(a.heavyBTagCorrType)<<") L("<<corrTypeName(a.lightBTagCorrType)<<")" << std::endl;
        }
        if(a.bTagCorrections & ucsbsusy::BTagCorrectionSet::FASTSIMBYEVTWEIGHT){
          os << "Applying FastSim bTag corrections from " << a.bTagFastSimEffFile.Data()<<","<<a.bTagFastSimSFFile.Data() <<" -> ";
          os << "FASTSIMBYEVTWEIGHT H("<<corrTypeName(a.heavyFastSimBTagCorrType)<<") L("<<corrTypeName(a.lightFastSimBTagCorrType)<<")" << std::endl;

        }
        os << std::endl;
      }

      if(a.leptonCorrections != ucsbsusy::LeptonCorrectionSet::NULLOPT){
        os << "Applying lepton corrections from " << a.leptonCorrectionFile.Data() << " and " << a.leptonCorrectionFile2.Data() << " and the TnP files... " <<" -> ";
        if(a.leptonCorrections & ucsbsusy::LeptonCorrectionSet::LEP)
          os << "LEP ";
        if(a.leptonCorrections & ucsbsusy::LeptonCorrectionSet::USE_HPSTAUS)
          os << "USE_HPSTAUS ";
        if(a.leptonCorrections & ucsbsusy::LeptonCorrectionSet::LEP_VARY_UP)
          os << "VARY_UP " << std::endl;
        else if(a.leptonCorrections & ucsbsusy::LeptonCorrectionSet::LEP_VARY_DOWN)
          os << "VARY_DOWN " << std::endl;
        os << std::endl;
        if(a.leptonCorrections & ucsbsusy::LeptonCorrectionSet::TNP) {
          os << "Applying TnP lepton corrections using selected leptons"<< std::endl;
        }
      }
      if(a.puCorrections != ucsbsusy::EventCorrectionSet::NULLOPT){
        os << "Applying PU corrections from " << a.puCorrectionFile.Data() <<" -> ";
        if(a.puCorrections & ucsbsusy::EventCorrectionSet::TRUEPU)
          os << "TRUEPU " << std::endl;
        os << std::endl;
      }
      if(a.jetAndMETCorrections != ucsbsusy::JetAndMETCorrectionSet::NULLOPT){
        os << "Applying jet and MET corrections -> ";
        if(a.jetAndMETCorrections & ucsbsusy::JetAndMETCorrectionSet::METSCALE)
          os << "METScale ";
        if(a.jetAndMETCorrections & ucsbsusy::JetAndMETCorrectionSet::METRESOLUTION)
          os << "METResolution ";
        if(a.jetAndMETCorrections & ucsbsusy::JetAndMETCorrectionSet::JETSCALE)
          os << "JetScale ("<< a.jetScaleCorr<<")";
        if(a.jetAndMETCorrections & ucsbsusy::JetAndMETCorrectionSet::JETRESOLUTION)
          os << "JetResolution ("<< a.jetResCorrType<<")";
        if(a.jetAndMETCorrections & ucsbsusy::JetAndMETCorrectionSet::QCDRESPTAIL)
          os << "QCDJetRespTail ("<< a.jetResTailCorrType<<")";
        if(a.jetAndMETCorrections & ucsbsusy::JetAndMETCorrectionSet::METRESSYSTRUNI)
          os << "METResolutionSystRunI ";
        if(a.jetAndMETCorrections & ucsbsusy::JetAndMETCorrectionSet::METRESSYSTRUNII)
          os << "METResolutionSystRunII  ";
        os << std::endl;
      }
      if(a.isrCorrections != ucsbsusy::ISRCorrectionSet::NULLOPT){
        if(a.isrCorrections & ucsbsusy::ISRCorrectionSet::ISRCORR){
          os << "Applying ISR corrections from " << a.isrCorrFile.Data()<<","<<a.isrSigNormFile.Data() <<" -> (";
          for(unsigned int iN = 0; iN < a.isrSigNorms.size(); ++iN){
            os << a.isrSigNorms[iN] <<" ";
          }
          os <<")"<<std::endl;
        }
        if(a.isrCorrections & ucsbsusy::ISRCorrectionSet::ISRCORRTIGHT){
          os << "Applying ISR corrections from " << a.isrCorrFile.Data()<<","<<a.isrSigNormTightFile.Data() <<" -> (";
          for(unsigned int iN = 0; iN < a.isrSigNorms.size(); ++iN){
            os << a.isrSigNorms[iN] <<" ";
          }
          os <<")"<<std::endl;
        }
      }
      return os;
    }
  };


  //The collection of default configs
  struct ConfigSet{
    JetConfig       jets           ;
    LeptonSelection::Electron electrons;
    LeptonSelection::Muon     muons;
    LeptonSelection::Electron secondaryElectrons;
    LeptonSelection::Muon     secondaryMuons;
    TrackConfig     tracks   ;
    TauConfig       taus     ;
    PhotonConfig    photons;
    CorrectionConfig corrections    ;
    TString         jsonFile       ;
    JSONProcessing* jsonProcessing ;
    ConfigSet() :
      jets            (),
      electrons (),
      muons   (),
      secondaryElectrons (),
      secondaryMuons   (),
      tracks    (),
      taus      (),
      photons (),
      corrections     (),
      jsonFile(""),
      jsonProcessing(0)
    {}
  };






}



#endif
