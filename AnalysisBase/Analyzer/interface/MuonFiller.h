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

typedef math::PtEtaPhiMLorentzVectorF LorentzVector;
typedef std::vector<LorentzVector> LorentzVectorCollection;

namespace ucsbsusy {

  class MuonFiller : public BaseFiller {

  public :
    MuonFiller(const int options,
	       const string branchName,
	       const EventInfoFiller * evtInfoFiller,
	       const edm::InputTag muonTag,
	       const bool requireLoose,
	       const double muptMin);
    ~MuonFiller() {}

    enum Options {
      NULLOPT     = 0,
      LOADGEN     = (1 << 0),
      FILLIDVARS  = (1 << 1), // fill ID variables
      FILLISOVARS = (1 << 2)  // fill isolation variables
    };
    static const int defaultOptions = NULLOPT;

    LeptonMVA*                  muMVAiso;
    void load(const edm::Event& iEvent);
    void fill();
    void calculateLSFIso(LorentzVector mu, LorentzVectorCollection lsfSubJets_, float *lsfIso_, float *lsfIsoDR_);
    float calculateRhoIso(double eta, double pfchargediso, double pfneutraliso, double pfphotoniso, float rho);
    bool mediumID(bool isLoose, double pt , double pfdbetaiso, double d0, double dz, bool isGlobal, double nChi2, double trkKink, double chi2Local, double validFrac, double segComp);
    double LSF(LorentzVector lep,edm::Handle<std::vector<reco::PFJet>> ca8jets);
    double getPFIsolation(edm::Handle<pat::PackedCandidateCollection> pfcands, const pat::Muon ptcl, double r_iso_min, double r_iso_max, double kt_scale, bool use_pfweight, bool charged_only);
    double getLeptonPtRel(edm::Handle<pat::JetCollection> jets, const pat::Muon lepton);

  private :
    const EventInfoFiller * evtInfoFiller_;
    // Input from the config file
    const edm::InputTag muonTag_;
    const bool          requireLoose_;    // only store muons which pass "Loose" ID
    const double        muptMin_;

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
    size iistight_;
    size iispf_;
    size iisglobal_;
    size iismedium_;
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
    size iMVAiso_;
    size iminiiso_;
    size iptrel_;
    size iLSF3Iso_;


  public :
    // Data members
    edm::Handle<pat::MuonCollection>  muons_;
    edm::Handle<LorentzVectorCollection> lsfSubJets3;
    edm::Handle<double>                 rho_;
    edm::Handle<pat::PackedCandidateCollection> pfcands;
    edm::Handle<std::vector<reco::PFJet>>       ca8jets;
    edm::Handle<pat::JetCollection>         ak4jets_;
  };

}

#endif
