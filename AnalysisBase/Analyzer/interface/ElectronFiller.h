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
#include "DataFormats/PatCandidates/interface/VIDCutFlowResult.h"

#include "AnalysisBase/Analyzer/interface/BaseFiller.h"
#include "AnalysisBase/Analyzer/interface/EventInfoFiller.h"

#include "AnalysisTools/ObjectSelection/interface/EGammaMvaEleEstimatorCSA14.h"
#include "AnalysisTools/ObjectSelection/interface/LeptonId.h"
#include "AnalysisTools/ObjectSelection/interface/LeptonMVA.h"

#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"

#include "AnalysisTools/IsolationVariables/interface/IsolationUtilities.h"

namespace ucsbsusy {

  class ElectronFiller : public BaseFiller {

    public :
      ElectronFiller(const edm::ParameterSet& cfg, edm::ConsumesCollector && cc, const int options, const string branchName, EventInfoFiller * evtInfoFiller);
      ~ElectronFiller() {}

      enum Options {
                    NULLOPT     = 0,
                    LOADGEN     = (1 << 0),
                    FILLIDVARS  = (1 << 1), // fill ID variables
                    FILLISOVARS = (1 << 2), // fill isolation variables
                    FILLPOGMVA  = (1 << 3)  // fill POG MVAs
                   };
      static const int defaultOptions = NULLOPT;

      void load(const edm::Event& iEvent, const edm::EventSetup &iSetup);
      void fill();

    private :
      const EventInfoFiller *                          evtInfoFiller_;
      // Input from the config file
      edm::EDGetTokenT<pat::ElectronCollection>        electronToken_;
      // For cut-based ID decisions
      edm::EDGetTokenT<edm::ValueMap<bool> >           vetoIdToken_;
      edm::EDGetTokenT<edm::ValueMap<bool> >           looseIdToken_;
      edm::EDGetTokenT<edm::ValueMap<bool> >           mediumIdToken_;
      edm::EDGetTokenT<edm::ValueMap<bool> >           tightIdToken_;
      edm::EDGetTokenT<edm::ValueMap<vid::CutFlowResult> > looseIdFullInfoMapToken_;
      edm::EDGetTokenT<edm::ValueMap<vid::CutFlowResult> > mediumIdFullInfoMapToken_;
      edm::EDGetTokenT<edm::ValueMap<bool> >           mvanontrigMediumIdToken_;
      edm::EDGetTokenT<edm::ValueMap<bool> >           mvanontrigTightIdToken_;
      edm::EDGetTokenT<edm::ValueMap<float> >          mvanontrigMVAValueToken_;
      edm::EDGetTokenT<edm::ValueMap<int> >            mvanontrigMVACatToken_;
      edm::EDGetTokenT<edm::ValueMap<bool> >           mvatrigMediumIdToken_;
      edm::EDGetTokenT<edm::ValueMap<bool> >           mvatrigTightIdToken_;
      edm::EDGetTokenT<edm::ValueMap<float> >          mvatrigMVAValueToken_;
      edm::EDGetTokenT<edm::ValueMap<int> >            mvatrigMVACatToken_;
      edm::EDGetTokenT<reco::PFJetCollection>          ca8jetToken_;
      edm::EDGetTokenT<double>                         rhoToken_;
      edm::EDGetTokenT<pat::JetCollection>             jetToken_;
      edm::EDGetTokenT<pat::PackedCandidateCollection> pfcandToken_;
      const double                                     eleptMin_;

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
      // Id flags: cut-based electron ID flags based on Spring15 selection
      size ivetoid_;
      size ilooseid_;
      size imediumid_;
      size itightid_;
      size imvanontrigmediumid_;
      size imvanontrigtightid_;
      size imvatrigmediumid_;
      size imvatrigtightid_;
      // Standard delta-beta isolation
      size ipfdbetaiso_;
      // Spring15 MVA ID
      size imvaidnontrig_;
      size imvaidcatnontrig_;
      size imvaidtrig_;
      size imvaidcattrig_;
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
      size iLSFIso_;
      size iminiiso_;
      size iannulus_;
      size iptrel_;
      size iptratio_;
      size irhoiso_;
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

      // cut-based id-only decision
      size iPassMediumIDOnly_;
      size iPassLooseIDOnly_;

    public :
      // Data members
      edm::Handle<pat::ElectronCollection>        electrons_;
      edm::Handle<edm::ValueMap<bool> >           veto_id_decisions_;
      edm::Handle<edm::ValueMap<bool> >           loose_id_decisions_;
      edm::Handle<edm::ValueMap<bool> >           medium_id_decisions_;
      edm::Handle<edm::ValueMap<bool> >           tight_id_decisions_;
      edm::Handle<edm::ValueMap<vid::CutFlowResult> > loose_id_cutflow_;
      edm::Handle<edm::ValueMap<vid::CutFlowResult> > medium_id_cutflow_;
      edm::Handle<edm::ValueMap<bool> >           mvanontrig_medium_id_decisions_;
      edm::Handle<edm::ValueMap<bool> >           mvanontrig_tight_id_decisions_;
      edm::Handle<edm::ValueMap<float> >          mvanontrig_value_map_;
      edm::Handle<edm::ValueMap<int> >            mvanontrig_category_map_;
      edm::Handle<edm::ValueMap<bool> >           mvatrig_medium_id_decisions_;
      edm::Handle<edm::ValueMap<bool> >           mvatrig_tight_id_decisions_;
      edm::Handle<edm::ValueMap<float> >          mvatrig_value_map_;
      edm::Handle<edm::ValueMap<int> >            mvatrig_category_map_;
      edm::Handle<reco::PFJetCollection>          ca8jets_;
      edm::Handle<double>                         rho_;
      edm::Handle<pat::PackedCandidateCollection> pfcands_;
      edm::Handle<pat::JetCollection>             ak4jets_;

  };

}

#endif
