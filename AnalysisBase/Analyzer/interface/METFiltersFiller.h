//--------------------------------------------------------------------------------------------------
//
// METFiltersFiller
//
// Filler for trigger decisions
//
// METFiltersFiller.h created on Thu Jul 6 19:34:23 CEST 2015
//
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISBASE_ANALYZER_METFILTERSFILLER_H
#define ANALYSISBASE_ANALYZER_METFILTERSFILLER_H

#include "AnalysisBase/Analyzer/interface/BaseFiller.h"

#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"
#include "DataFormats/Common/interface/EDCollection.h"
#include "DataFormats/DetId/interface/DetId.h"

namespace ucsbsusy {

  typedef std::map<std::string, unsigned long>	TrigIdMap;

  class METFiltersFiller : public BaseFiller {

  public :
    METFiltersFiller(const edm::ParameterSet& cfg, edm::ConsumesCollector && cc, const int options, const string branchName);
    ~METFiltersFiller() {}

    enum Options {
      NULLOPT = 0,
      LOADPATFILTERS = (1 << 0), // load extra filters produced in "PAT" step (for data the MET filters are produced in "RECO" step by default; MC is always "PAT")
      LOADECALFIXFLAGS = (1 << 1), // load two flags for ECAL slew rate fix
    };
    static const int defaultOptions = NULLOPT;

    void load(const edm::Event& iEvent, const edm::EventSetup &iSetup);
    void fill();
    void initTriggerNames();

  private :

    edm::EDGetTokenT<edm::TriggerResults>  triggerBitToken_;
    const edm::TriggerNames *              triggerNames_;
    TrigIdMap                              trigIds_;
    edm::EDGetTokenT<bool>                 badChCandToken_;
    edm::EDGetTokenT<bool>                 badPFMuonToken_;
    size itrig_bit_pass;
    size ibadchcand_;
    size ibadpfmuon_;
    size idupECALClusters_;
    size ihaveHitsNotReplaced_;

    // added for the bad muon filters in 03Feb2017 reMiniAOD
    edm::EDGetTokenT<edm::TriggerResults>  patTriggerBitToken_;
    const edm::TriggerNames *              patTriggerNames_;
    // added for the photon slew rate fix in 03Feb2017 reMiniAOD
    edm::EDGetTokenT<bool>                 dupECALClustersToken_;
    edm::EDGetTokenT<edm::EDCollection<DetId>> hitsNotReplacedToken_;

  public :
    edm::Handle<edm::TriggerResults> triggerBits_;
    edm::Handle<edm::TriggerResults> patTriggerBits_;
    edm::Handle<bool>                badChCand_;
    edm::Handle<bool>                badPFMuon_;

    // added for the photon slew rate fix in 03Feb2017 reMiniAOD
    // https://twiki.cern.ch/twiki/bin/view/CMSPublic/ReMiniAOD03Feb2017Notes#EGM
    // events need more scrutiny if dupECALClusters=true or hitsNotReplaced is NOT empty
    edm::Handle<bool>                                dupECALClusters_;
    edm::Handle<edm::EDCollection<DetId>>            hitsNotReplaced_;
  };

}

#endif
