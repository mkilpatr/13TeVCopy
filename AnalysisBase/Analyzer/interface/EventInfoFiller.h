//--------------------------------------------------------------------------------------------------
//
// EventInfoFiller
//
// Simple class to write some event info into a TTree.
//
// EventInfoFiller.h created on Mon Aug 11 10:23:46 CEST 2014
//
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISBASE_ANALYZER_EVENTINFOFILLER_H
#define ANALYSISBASE_ANALYZER_EVENTINFOFILLER_H

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "DataFormats/JetReco/interface/GenJet.h"

#include "AnalysisBase/Analyzer/interface/BaseFiller.h"

namespace ucsbsusy {

  struct EventCoords {
    EventCoords() : run(0),lumi(0),event(0) {}
    unsigned int run;
    unsigned int lumi;
    unsigned int event;
  };

  class EventInfoFiller : public BaseFiller {

    public:
      EventInfoFiller(const edm::ParameterSet& cfg, edm::ConsumesCollector && cc, const int options);

      ~EventInfoFiller() {};

      enum Options {
        NULLOPT     = 0,
        LOADGEN     = (1 << 0),
        LOADLHE     = (1 << 1),
        LOADPUINFO  = (1 << 2),
        SAVEMASSES  = (1 << 3),
        LOADGENJETS = (1 << 4)
      };
      static const int defaultOptions = NULLOPT;

      void load(const edm::Event& iEvent, const edm::EventSetup &iSetup);
      void fill();

      void setModelString(std::string modstring) { modelString_ = modstring; }

      reco::Vertex::Point primaryVertex() const { return primaryVertexIndex_ >= 0 ? (*vertices_)[primaryVertexIndex_].position() : reco::Vertex::Point(); }
      const pat::MET*     met()           const { return met_; }
      const pat::MET*     metNoHF()       const { return metNoHF_; }

    private:
      // Input from the config file
      edm::EDGetTokenT<reco::VertexCollection>          vtxToken_;
      edm::EDGetTokenT<double>                          rhoToken_;
      edm::EDGetTokenT<std::vector<PileupSummaryInfo> > puSummaryToken_;
      edm::EDGetTokenT<pat::METCollection>              metToken_;
      edm::EDGetTokenT<pat::METCollection>              metOOBToken_;
      edm::EDGetTokenT<pat::METCollection>              metNoHFToken_;
      edm::EDGetTokenT<pat::METCollection>              puppimetToken_;
      edm::EDGetTokenT<GenEventInfoProduct>             genEvtInfoToken_;
      edm::EDGetTokenT<LHEEventProduct>                 lheEvtInfoToken_;
      std::vector<unsigned int>                         systWgtIndices_;
      edm::EDGetTokenT<reco::GenJetCollection>          stdGenJetToken_;

      std::string modelString_;

      // Members to hold index of most recently filled tree data
      size irun_          ;
      size ilumi_         ;
      size ievent_        ;
      size inpv_          ;
      size igoodvertex_   ;
      size irho_          ;
      size ipvx_          ;
      size ipvy_          ;
      size ipvz_          ;
      size ibunchspacing_ ;
      size intruepuints_  ;
      size inpuverts_     ;
      size inpuvertsbx1_  ;
      size inpuvertsbxm1_ ;
      size imetpt_        ;
      size imetphi_       ;
      size imetsumEt_     ;
      size irawmetpt_     ;
      size irawmetphi_    ;
      size irawmetsumEt_  ;
      size imetnohfpt_    ;
      size imetnohfphi_   ;
      size imetnohfsumEt_ ;
      size ipuppimetpt_   ;
      size ipuppimetphi_  ;
      size ipuppimetsumEt_;
      size igenmetpt_     ;
      size igenmetphi_    ;
      size igenwgt_       ;
      size igenqscale_    ;
      size inmeparts_     ;
      size inmefiltparts_ ;
      size ilhecentralwgt_;
      size isystwgts_     ;
      size imasspar_      ;
      size instdgenjets_  ;
      size imetunclustrun2uppt_ ;
      size imetunclustrun2dnpt_ ;

    public:
      // Data members
      EventCoords    eventCoords;
      int            primaryVertexIndex_;
      const pat::MET *met_;
      const pat::MET *metOOB_;
      const pat::MET *metNoHF_;
      const pat::MET *puppimet_;
      edm::Handle<reco::VertexCollection>          vertices_;
      edm::Handle<double>                          rho_;
      edm::Handle<std::vector<PileupSummaryInfo> > puSummaryInfos_;
      edm::Handle<pat::METCollection>              mets_;
      edm::Handle<pat::METCollection>              metsOOB_;
      edm::Handle<pat::METCollection>              metsNoHF_;
      edm::Handle<pat::METCollection>              puppimets_;
      edm::Handle<GenEventInfoProduct>             genEvtInfo_;
      edm::Handle<LHEEventProduct>                 lheEvtInfo_;
      edm::Handle<reco::GenJetCollection>          stdGenJets_;

  };

}

#endif
