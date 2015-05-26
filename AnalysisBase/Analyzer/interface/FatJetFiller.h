//--------------------------------------------------------------------------------------------------
// 
// MuonFiller
// 
// Class to fill muon information in a TTree.
// 
// MuonFiller.h created on Fri Oct 17 12:11:09 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISBASE_ANALYZER_FATJETFILLER_H
#define ANALYSISBASE_ANALYZER_FATJETFILLER_H

#include "AnalysisBase/Analyzer/interface/BaseFiller.h"
#include "AnalysisBase/Analyzer/interface/EventInfoFiller.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisTools/ObjectSelection/interface/LeptonMVA.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/LorentzVector.h"

#include "DataFormats/PatCandidates/interface/Jet.h"

typedef math::PtEtaPhiMLorentzVectorF LorentzVector;
typedef std::vector<LorentzVector> LorentzVectorCollection;

namespace ucsbsusy {

  class FatJetFiller : public BaseFiller {

  public :
    FatJetFiller(const int options,
	       const string branchName,
	       const EventInfoFiller * evtInfoFiller,
	       const edm::InputTag fatJetTag);
    ~FatJetFiller() {}

    void load(const edm::Event& iEvent);
    void fill();

  private :
    const EventInfoFiller * evtInfoFiller_;
    const edm::InputTag fatJetTag_;

    // Members to hold indices of tree data
    size ifj_mass_;
    size ifj_trimmedmass_;
    size ifj_prunedmass_;
    size ifj_softdropmass_;
    size ifj_filteredmass_;
    size ifj_massdropfilteredmass_;
    size ifj_tau1_;
    size ifj_tau2_;
    size ifj_tau3_;
    size ifj_pt_;
    size ifj_eta_;
    size ifj_phi_;


  public :
    // Data members
    edm::Handle<pat::JetCollection> fatJets_;
  };

}

#endif
