//--------------------------------------------------------------------------------------------------
// 
// MuonFiller
// 
// Class to fill muon information in a TTree.
// 
// MuonFiller.h created on Fri Oct 17 12:11:09 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISBASE_ANALYZER_CMSTOPFILLER_H
#define ANALYSISBASE_ANALYZER_CMSTOPFILLER_H

#include "AnalysisBase/Analyzer/interface/BaseFiller.h"
#include "AnalysisBase/Analyzer/interface/EventInfoFiller.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisTools/ObjectSelection/interface/LeptonMVA.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/LorentzVector.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "RecoJets/JetAlgorithms/interface/CATopJetHelper.h"
#include "DataFormats/JetReco/interface/CATopJetTagInfo.h"

typedef math::PtEtaPhiMLorentzVectorF LorentzVector;
typedef std::vector<LorentzVector> LorentzVectorCollection;

namespace ucsbsusy {

  class CMSTopFiller : public BaseFiller {

  public :
    CMSTopFiller(const int options,
	       const string branchName,
	       const EventInfoFiller * evtInfoFiller,
	       const edm::InputTag fatJetTag);
    ~CMSTopFiller() {}

    void load(const edm::Event& iEvent, const edm::EventSetup &iSetup);
    void fill();

  private :
    const EventInfoFiller * evtInfoFiller_;
    const edm::InputTag fatJetTag_;

    // Members to hold indices of tree data
    size ictt_fatjet_mass_;
    size ictt_fatjet_trimmedmass_;
    size ictt_fatjet_prunedmass_;
    size ictt_fatjet_softdropmass_;
    size ictt_fatjet_filteredmass_;
    size ictt_fatjet_massdropfilteredmass_;
    size ictt_fatjet_tau1_;
    size ictt_fatjet_tau2_;
    size ictt_fatjet_tau3_;
    size ictt_top_pt_;
    size ictt_top_eta_;
    size ictt_top_phi_;
    size ictt_top_topmass_;
    size ictt_top_wmass_;
    size ictt_top_minmass_;
    size ictt_top_nsubjets_;



  public :
    // Data members
    edm::Handle<pat::JetCollection> fatJets_;
  };

}

#endif
