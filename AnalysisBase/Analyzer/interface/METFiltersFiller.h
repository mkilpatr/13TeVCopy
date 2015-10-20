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

namespace ucsbsusy {

  typedef std::map<std::string, unsigned long>	TrigIdMap;

  class METFiltersFiller : public BaseFiller {

  public :
    METFiltersFiller(const edm::ParameterSet& cfg, edm::ConsumesCollector && cc, const int options, const string branchName);
    ~METFiltersFiller() {}

    enum Options {
      NULLOPT = 0
    };
    static const int defaultOptions = NULLOPT;

    void load(const edm::Event& iEvent, const edm::EventSetup &iSetup);
    void fill();
    void initTriggerNames();

  private :


    edm::EDGetTokenT<edm::TriggerResults>  triggerBitToken_;
    edm::EDGetTokenT<bool>                 hNoiseResultToken_;
    edm::EDGetTokenT<bool>                 hNoiseResultRun2LooseToken_;
    edm::EDGetTokenT<bool>                 hNoiseResultRun2TightToken_;
    const edm::TriggerNames *              triggerNames_;
    TrigIdMap                              trigIds_;

    size itrig_bit_flag;
    size itrig_bit_pass;

    size hbheFilterFix_;
    size hbheFilterRun2Loose_;
    size hbheFilterRun2Tight_;

    bool isFastSim_;

  public :
    edm::Handle<edm::TriggerResults> triggerBits_;
    edm::Handle<bool> hNoiseResult_;
    edm::Handle<bool> hNoiseResultRun2Loose_;
    edm::Handle<bool> hNoiseResultRun2Tight_;

  };

}

#endif
