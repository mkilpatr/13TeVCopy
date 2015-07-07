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

  typedef std::map<std::string, unsigned int>	TrigIdMap;

  class METFiltersFiller : public BaseFiller {

  public :
    METFiltersFiller(const int options,
                  const string branchName,
		     const edm::InputTag triggerBitTag);
    ~METFiltersFiller() {}

    enum Options {
      NULLOPT = 0
    };
    static const int defaultOptions = NULLOPT;

    void load(const edm::Event& iEvent);
    void fill();
    void initTriggerNames();

  private :
    const edm::InputTag triggerBitTag_;
    const edm::TriggerNames *triggerNames_;
    TrigIdMap trigIds_;

    size itrig_bit_flag;
    size itrig_bit_pass;

  public :
    edm::Handle<edm::TriggerResults> triggerBits_;

  };

}

#endif
