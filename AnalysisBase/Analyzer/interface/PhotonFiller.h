/*
 * PhotonFiller.h
 *
 *  Class to fill photon information in a TTree.
 *  Created on: May 12, 2015
 *      Author: hqu
 */

#ifndef ANALYSISBASE_ANALYZER_PHOTONFILLER_H
#define ANALYSISBASE_ANALYZER_PHOTONFILLER_H

#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/Common/interface/ValueMap.h"

#include "AnalysisBase/Analyzer/interface/BaseFiller.h"
#include "AnalysisBase/Analyzer/interface/EventInfoFiller.h"

//#include "DataFormats/JetReco/interface/PFJet.h"
//#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/LorentzVector.h"

typedef math::PtEtaPhiMLorentzVectorF LorentzVector;
typedef std::vector<LorentzVector> LorentzVectorCollection;

namespace ucsbsusy {

  class PhotonFiller : public BaseFiller {

  public :
    PhotonFiller(const int options,
	       const string branchName,
	       const EventInfoFiller * evtInfoFiller,
	       const edm::InputTag photonTag,
           const edm::InputTag looseIdTag,
           const edm::InputTag mediumIdTag,
           const edm::InputTag tightIdTag,
	       const double phoptMin);
    ~PhotonFiller() {}

    enum Options {
      NULLOPT     = 0,
      LOADGEN     = (1 << 0),
      FILLIDVARS  = (1 << 1), // fill ID variables
      FILLISOVARS = (1 << 2)  // fill isolation variables
    };
    static const int defaultOptions = NULLOPT;

    void load(const edm::Event& iEvent, const edm::EventSetup &iSetup);
    void fill();
//    void calculateLSFIso(LorentzVector mu, LorentzVectorCollection lsfSubJets_, float *lsfIso_, float *lsfIsoDR_);

  private :
    const EventInfoFiller * evtInfoFiller_;
    // Input from the config file
    const edm::InputTag photonTag_;
    // For cut-based ID decisions
    const edm::InputTag looseIdTag_;
    const edm::InputTag mediumIdTag_;
    const edm::InputTag tightIdTag_;
    const double        phoptMin_;


    // Members to hold indices of tree data
    size ipt_;
    size ieta_;
    size iSCeta_;
    size iphi_;
    size imass_;
    size ir9_;
    // Id flags: cut-based electron ID flags
    size ilooseid_;
    size imediumid_;
    size itightid_;
    // Standard delta-beta isolation
    size ipfdbetaiso_;
    // ID variables
    size isigietaieta_;
    size ipassElectronVeto_;
    // used in POG photon ID:
    size ifull5x5sigietaieta_;
    size ihOverE_;
    size irhoPFchargedHadronIso_;
    size irhoPFneutralHadronIso_;
    size irhoPFphotonIso_;
    // Isolation quantities
    size itrackiso_;
    size iecaliso_;
    size ihcaliso_;
    size ipfchargedHadronIso_;
    size ipfneutralHadronIso_;
    size ipfphotonIso_;
    size ipfpuiso_;

    enum ISO_TYPE { CHARGED, NEUTRAL, PHOTON };
    float calculateRhoIso(double eta, double pfiso, double rho, ISO_TYPE isotype);


  public :
    // Data members
    edm::Handle<pat::PhotonCollection>  photons_;
    edm::Handle<edm::ValueMap<bool> >   loose_id_decisions_;
    edm::Handle<edm::ValueMap<bool> >   medium_id_decisions_;
    edm::Handle<edm::ValueMap<bool> >   tight_id_decisions_;
//    edm::Handle<LorentzVectorCollection> lsfSubJets3;
    edm::Handle<double>                 rho_;
  };

}




#endif /* ANALYSISBASE_ANALYZER_PHOTONFILLER_H */
