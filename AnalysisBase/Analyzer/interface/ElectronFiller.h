//--------------------------------------------------------------------------------------------------
// 
// ElectronFiller
// 
// Class to fill electron information in a TTree.
// 
// ElectronFiller.h created on Thu Oct 16 12:21:38 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISBASE_ANALYZER_ELECTRONFILLER_H
#define ANALYSISBASE_ANALYZER_ELECTRONFILLER_H

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/Common/interface/ValueMap.h"

#include "AnalysisBase/Analyzer/interface/BaseFiller.h"
#include "AnalysisBase/Analyzer/interface/EventInfoFiller.h"

#include "AnalysisTools/ObjectSelection/interface/EGammaMvaEleEstimatorCSA14.h"
#include "AnalysisTools/ObjectSelection/interface/LeptonId.h"
#include "AnalysisTools/ObjectSelection/interface/LeptonMVA.h"

#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "DataFormats/Math/interface/LorentzVector.h"
#include "TLorentzVector.h"
#include "Math/VectorUtil.h"
#include <fastjet/JetDefinition.hh>
#include <fastjet/PseudoJet.hh>
#include "fastjet/tools/Filter.hh"
#include <fastjet/ClusterSequence.hh>
#include <fastjet/ClusterSequenceArea.hh>

//typedef math::XYZTLorentzVector LorentzVector;
typedef math::PtEtaPhiMLorentzVectorF LorentzVector;
typedef std::vector<LorentzVector> LorentzVectorCollection;

namespace ucsbsusy {

  class ElectronFiller : public BaseFiller {

    public :
      ElectronFiller(const int options,
                     const string branchName,
                     const EventInfoFiller * evtInfoFiller,
                     const edm::InputTag electronTag,
                     const edm::InputTag vetoIdTag,
                     const edm::InputTag looseIdTag,
                     const edm::InputTag mediumIdTag,
                     const edm::InputTag tightIdTag,
                     const int bunchSpacing,
                     const double eleptMin);
      ~ElectronFiller() {}

      enum Options {
                    NULLOPT     = 0,
                    LOADGEN     = (1 << 0),
                    FILLIDVARS  = (1 << 1), // fill ID variables
                    FILLISOVARS = (1 << 2), // fill isolation variables
                    FILLPOGMVA  = (1 << 3)  // fill POG MVAs
                   };
      static const int defaultOptions = NULLOPT;

      void initMVA();
      void load(const edm::Event& iEvent);
      void fill();
      void calculateLSFIso(LorentzVector el, LorentzVectorCollection lsfSubJets_, float *lsfIso_, float *lsfIsoDR_);
      float calculateRhoIso(double eta, double pfchargediso, double pfneutraliso, double pfphotoniso, float rho);
      double getPFIsolation(edm::Handle<pat::PackedCandidateCollection> pfcands, const pat::Electron ptcl, double r_iso_min, double r_iso_max, double kt_scale, bool use_pfweight, bool charged_only);
      double getLeptonPtRel(edm::Handle<pat::JetCollection> jets, const pat::Electron lepton);
      double getLeptonPtRatio(edm::Handle<pat::JetCollection> jets, const pat::Electron lepton);
      double LSF(LorentzVector lep,edm::Handle<std::vector<reco::PFJet>> ca8jets);

    private :
      const EventInfoFiller * evtInfoFiller_;
      // Input from the config file
      const edm::InputTag electronTag_;
      // For cut-based ID decisions
      const edm::InputTag vetoIdTag_;
      const edm::InputTag looseIdTag_;
      const edm::InputTag mediumIdTag_;
      const edm::InputTag tightIdTag_;
      const int           bunchSpacing_;
      const double        eleptMin_;

      // Evaluate POG MVA ID
      EGammaMvaEleEstimatorCSA14* eleMVANonTrig;
      EGammaMvaEleEstimatorCSA14* eleMVATrig;
      EGammaMvaEleEstimatorCSA14* eleMVANonTrigPhys14;
      LeptonId*                   eleIdCuts;
      LeptonMVA*                  eleMVAiso;

      // Members to hold indices of tree data
      size ipt_;
      size ieta_;
      size iSCeta_;
      size iphi_;
      size imass_;
      size iq_;
      size ir9_;
      size id0_;
      size idz_;
      // Id flags: cut-based electron ID flags based on CSA14 selection
      size ivetoid_;
      size ilooseid_;
      size imediumid_;
      size itightid_;
      // Standard delta-beta isolation
      size ipfdbetaiso_;
      // CSA14 MVA ID (can be evalated for either 25ns or 50ns scenarios, determined by bunchSpacing_ parameter)
      size imvaidnontrig_;
      size imvaidtrig_;
      // ID variables, needed for evaluation of cut-based ID from scratch
      size iecalE_;
      size ieOverPIn_;
      size isigietaieta_;
      size isigiphiiphi_;
      size ifull5x5sigietaieta_;
      size idEtaIn_;
      size idPhiIn_;
      size ihOverE_;
      size ifBrem_;
      size inExpHitsInner_;
      size inLostHitsInner_;
      size ipassConvVeto_;
      // Isolation quantities
      size itrackiso_;
      size iecaliso_;
      size ihcaliso_;
      size ipfchargediso_;
      size ipfneutraliso_;
      size ipfphotoniso_;
      size ipfpuiso_;
      size iMVAiso_;
      // different isolations
      size iLSF2Iso_;
      size iLSF3Iso_;
      size iLSF4Iso_;
      size iLSF2IsoDR_;
      size iLSF3IsoDR_;
      size iLSF4IsoDR_;
      size iminiiso_;
      size iptrel_;
      size iptratio_;
      size isip3d_;
      // pat matched gen lepton info
      size igenpt_;
      size igeneta_;
      size igenphi_;
      size igenmass_;
      size igenstatus_;
      size igenpdgid_;
      size igenmotherstatus_;
      size igenmotherpdgid_;

      // cut-based id - put by hand
      //      size iPassTriggerLID_;
      size iPassCutBaseNonIsoMID_;
      //      size iPassCutBaseTID_;

    public :
      // Data members
      edm::Handle<pat::ElectronCollection>        electrons_;
      edm::Handle<edm::ValueMap<bool> >           veto_id_decisions_;
      edm::Handle<edm::ValueMap<bool> >           loose_id_decisions_;
      edm::Handle<edm::ValueMap<bool> >           medium_id_decisions_;
      edm::Handle<edm::ValueMap<bool> >           tight_id_decisions_;
      edm::Handle<reco::PFJetCollection>          ca8jets;
      edm::Handle<LorentzVectorCollection>        lsfSubJets2;
      edm::Handle<LorentzVectorCollection>        lsfSubJets3;
      edm::Handle<LorentzVectorCollection>        lsfSubJets4;
      edm::Handle<double>                         rho_;
      edm::Handle<pat::PackedCandidateCollection> pfcands;
      edm::Handle<pat::JetCollection>             ak4jets_;

  };

}

#endif
