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
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"


#include "AnalysisBase/Analyzer/interface/BaseFiller.h"
#include "AnalysisBase/Analyzer/interface/SuperClusterFootprintRemoval.h"

#include "TMath.h"
#include "TRandom3.h"
#include "TVector3.h"
#include "TRotation.h"

namespace ucsbsusy {

  class PhotonFiller : public BaseFiller {

  public :
    PhotonFiller(const edm::ParameterSet& cfg, edm::ConsumesCollector && cc, const int options, const string branchName);
    ~PhotonFiller() {}

    enum Options {
      NULLOPT     = 0,
      LOADGEN     = (1 << 0),
      FILLIDVARS  = (1 << 1), // fill ID variables
      FILLISOVARS = (1 << 2)  // fill isolation variables
    };
    static const int defaultOptions = NULLOPT;

    void load(const edm::Event& iEvent, edm::EventSetup const &isetup);
    //void fill(const edm::EventSetup&);
    void fill();
//--------------------------------------------------------------------------------------------------
    void beginJob(edm::Run const &run, edm::EventSetup const &isetup);
    void beginRun(edm::Run const&, edm::EventSetup const&);

  private :
    // Input from the config file
    edm::EDGetTokenT<pat::PhotonCollection> photonToken_;
    // For cut-based ID decisions
    edm::EDGetTokenT<edm::ValueMap<bool> >  vetoIdToken_;
    edm::EDGetTokenT<edm::ValueMap<bool> >  looseIdToken_;
    edm::EDGetTokenT<edm::ValueMap<bool> >  mediumIdToken_;
    edm::EDGetTokenT<edm::ValueMap<bool> >  tightIdToken_;
    edm::EDGetTokenT<double>                rhoToken_;
    edm::EDGetTokenT<reco::VertexCollection> vtxToken_;
    edm::EDGetTokenT<pat::ElectronCollection> electronToken_;
    edm::EDGetTokenT<pat::MuonCollection> muonToken_;
    edm::EDGetTokenT<pat::JetCollection> jetToken_;
    edm::EDGetTokenT<pat::PackedCandidateCollection> pfcandidateToken_;
    const double                            phoptMin_;
    const double                            phoptMinRC_;
    const double                            jetptMinRC_;
    const double                            leptonptMinRC_;

    CaloSubdetectorGeometry *barrelGeometry;
    CaloSubdetectorGeometry *endcapGeometry;
    MagneticField *magField;

    // Members to hold indices of tree data
    size ipt_;
    size ieta_;
    size iSCeta_;
    size iphi_;
    size imass_;
    size ir9_;
    // Id flags: cut-based electron ID flags
    size ivetoid_;
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
    size ietaRC_;
    size iphiRC_;
    size irhoPFchargedHadronIsoRC_;
    size irhoPFneutralHadronIsoRC_;
    size irhoPFphotonIsoRC_;
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
    edm::Handle<edm::ValueMap<bool> >   veto_id_decisions_;
    edm::Handle<edm::ValueMap<bool> >   medium_id_decisions_;
    edm::Handle<edm::ValueMap<bool> >   tight_id_decisions_;
    edm::Handle<edm::ValueMap<bool> >   loose_id_decisions_;
    edm::Handle<double>                 rho_;
    edm::Handle<reco::VertexCollection>  vertices_;
    edm::Handle<pat::MuonCollection> muons_;
    edm::Handle<pat::ElectronCollection> electrons_;
    edm::Handle<pat::JetCollection> jets_;
    edm::Handle<pat::PackedCandidateCollection> pfcandidates_;
    //SuperClusterFootprintRemoval remover;
  };

}




#endif /* ANALYSISBASE_ANALYZER_PHOTONFILLER_H */
