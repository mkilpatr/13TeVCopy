//--------------------------------------------------------------------------------------------------
// 
// MuonFiller
// 
// Class to fill muon information in a TTree.
// 
// MuonFiller.h created on Fri Oct 17 12:11:09 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISBASE_ANALYZER_MUONFILLER_H
#define ANALYSISBASE_ANALYZER_MUONFILLER_H

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

#include "AnalysisBase/Analyzer/interface/BaseFiller.h"
#include "AnalysisBase/Analyzer/interface/EventInfoFiller.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"

#include "AnalysisTools/IsolationVariables/interface/IsolationUtilities.h"


namespace ucsbsusy {

  class MuonFiller : public BaseFiller {

  public :
    MuonFiller(const edm::ParameterSet& cfg, edm::ConsumesCollector && cc, const int options, const string branchName, EventInfoFiller * evtInfoFiller);
    ~MuonFiller() {}

    enum Options {
      NULLOPT     = 0,
      LOADGEN     = (1 << 0),
      FILLIDVARS  = (1 << 1), // fill ID variables
      FILLISOVARS = (1 << 2)  // fill isolation variables
    };
    static const int defaultOptions = NULLOPT;

    void load(const edm::Event& iEvent, const edm::EventSetup &iSetup);
    void fill();

  private :
    const EventInfoFiller * evtInfoFiller_;
    // Input from the config file
    edm::EDGetTokenT<pat::MuonCollection>            muonToken_;
    edm::EDGetTokenT<reco::PFJetCollection>          ca8jetToken_;
    edm::EDGetTokenT<double>                         rhoToken_;
    edm::EDGetTokenT<pat::JetCollection>             jetToken_;
    edm::EDGetTokenT<pat::PackedCandidateCollection> pfcandToken_;
    const bool              requireLoose_;    // only store muons which pass "Loose" ID
    const double            muptMin_;

    // Members to hold indices of tree data
    size ipt_;
    size ieta_;
    size iphi_;
    size imass_;
    size iq_;
    size id0_;
    size idz_;
    size ipfdbetaiso_;
    size iisloose_;
    size iismedium_;
    size iistight_;
    size iispf_;
    size iisglobal_;
    size iistracker_;
    size iisstandalone_;
    size inChi2_;
    size inValidHits_;
    size inMatch_;
    size inPixHits_;
    size inTrackLayers_;
    size itrackiso_;
    size iecaliso_;
    size ihcaliso_;
    size ipfchargediso_;
    size ipfneutraliso_;
    size ipfphotoniso_;
    size ipfpuiso_;
    size iminiiso_;
    size iannulus_;
    size iminiisoeacorr_;
    size iannuluseacorr_;
    size iminiisocharged_;
    size iminiisoneutral_;
    size iminiisopu_;
    size iptrel_;
    size irhoiso_;
    size iLSFIso_;
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

  public :
    // Data members
    edm::Handle<pat::MuonCollection>            muons_;
    edm::Handle<double>                         rho_;
    edm::Handle<pat::PackedCandidateCollection> pfcands_;
    edm::Handle<std::vector<reco::PFJet>>       ca8jets_;
    edm::Handle<pat::JetCollection>             ak4jets_;
  };

}

#endif
