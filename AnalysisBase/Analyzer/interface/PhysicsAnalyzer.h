//--------------------------------------------------------------------------------------------------
// 
// PhysicsAnalyzer
// 
// Implementation of BaseAnalyzer with a few additions:
// It provides an interface to event objects, providing functions to load the event content
// It stores the event content classes for easy processing
// It includes a few more advanced header files that are useful in analyzers
// It stores what kind of event it is in addition to the event weight
// 
//--------------------------------------------------------------------------------------------------

#ifndef PHYSICSANALYZER_H
#define PHYSICSANALYZER_H

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenLumiInfoHeader.h"

#include "AnalysisBase/Analyzer/interface/BaseAnalyzer.h"
//#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"

// Data products
#include "AnalysisBase/Analyzer/interface/EventInfoFiller.h"
#include "AnalysisBase/Analyzer/interface/PatJetFiller.h"
#include "AnalysisBase/Analyzer/interface/RecoJetFiller.h"
#include "AnalysisBase/Analyzer/interface/MuonFiller.h"
#include "AnalysisBase/Analyzer/interface/ElectronFiller.h"
#include "AnalysisBase/Analyzer/interface/TauFiller.h"
#include "AnalysisBase/Analyzer/interface/PhotonFiller.h"
#include "AnalysisBase/Analyzer/interface/GenParticleFiller.h"
#include "AnalysisBase/Analyzer/interface/PFCandidateFiller.h"
#include "AnalysisBase/Analyzer/interface/CMSTopFiller.h"
#include "AnalysisBase/Analyzer/interface/FatJetFiller.h"
#include "AnalysisBase/Analyzer/interface/TriggerFiller.h"
#include "AnalysisBase/Analyzer/interface/METFiltersFiller.h"
#include "AnalysisBase/Analyzer/interface/SecondaryVertexFiller.h"
#include "AnalysisBase/Analyzer/interface/HTTFiller.h"

namespace ucsbsusy {

  class PhysicsAnalyzer : public BaseAnalyzer {
  public:


    //--------------------------------------------------------------------------------------------------
    // Functions for base-level running
    //--------------------------------------------------------------------------------------------------
    public :
      PhysicsAnalyzer(const edm::ParameterSet& iConfig);
      virtual ~PhysicsAnalyzer();

      virtual void beginJob() override;
      virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      virtual void book();
      virtual bool load(const edm::Event& iEvent, const edm::EventSetup& iSetup);
      virtual bool filter(edm::Event& iEvent, const edm::EventSetup& iSetup) override;
      virtual void fill();

      //--------------------------------------------------------------------------------------------------
      // Functions for running the default variable types
      //--------------------------------------------------------------------------------------------------
    public:
      enum VarType {EVTINFO, AK4JETS, PUPPIJETS, PICKYJETS, CASUBJETS, CA8JETS, ELECTRONS, MUONS, TAUS, PHOTONS, PFCANDS, GENPARTICLES, CMSTOPS, AK8FATJETS, AK8PUPPIFATJETS, TRIGGERS, METFILTERS, SV, HTTJETS};
      virtual void initialize(const edm::ParameterSet& cfg, const std::string pSetName, const VarType type, const int options = -1, const std::string branchName = "" );
      virtual void initialize(const edm::ParameterSet& cfg, const VarType type, const int options = -1, const std::string branchName = "" );
      virtual void initialize(BaseFiller * filler);

      //--------------------------------------------------------------------------------------------------
      // Functions to manipulate and get stored data members
      //--------------------------------------------------------------------------------------------------
    public:
      bool isData() const;
      bool isMC  () const;

      //--------------------------------------------------------------------------------------------------
      // Data members owned by this class
      //--------------------------------------------------------------------------------------------------
    public :
      const int     isRealData;            // Whether or not processing real data; deduced from input file name and verified once first event is loaded
      const TString globalTag;             // Global tag name
      const bool    printLHERunInfo;       // print LHERunInfo: useful to determine which systematic weights are available
      const bool    printGenLumiInfo;      // print GenLumiInfo: print info contained in lumi header for signal
      const bool    getGenLumiHeader;      // get GenLumiHeader: relevant to get signal model info
      edm::EDGetTokenT<LHERunInfoProduct> lheInfoToken_;
      edm::EDGetTokenT<GenLumiInfoHeader> genLumiHeaderToken_;

      //--------------------------------------------------------------------------------------------------
      // "Filler" classes to store event information
      //--------------------------------------------------------------------------------------------------
    public:
      EventInfoFiller   * eventInfo;
      PatJetFiller      * ak4Jets;
      RecoJetFiller     * puppiJets;
      RecoJetFiller     * pickyJets;
      RecoJetFiller     * caSubJets;
      RecoJetFiller     * ca8Jets;
      MuonFiller        * muons;
      ElectronFiller    * electrons;
      TauFiller         * taus;
      PhotonFiller      * photons;
      PFCandidateFiller * pfcands;
      GenParticleFiller * genparticles;
      CMSTopFiller      * cmstops;
      FatJetFiller      * ak8fatjets;
      HTTFiller         * HTTjets;
      FatJetFiller      * ak8puppifatjets;
      TriggerFiller     * triggers;
      METFiltersFiller  * metfilters;
      SecondaryVertexFiller * sv;
    protected:
      //vector of initialized fillers for automatic processing
      std::vector<BaseFiller*> initializedFillers;
  };

}

#endif
