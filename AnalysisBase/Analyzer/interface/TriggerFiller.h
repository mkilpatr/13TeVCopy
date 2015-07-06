//--------------------------------------------------------------------------------------------------
// 
// TriggerFiller
// 
// Filler for trigger decisions
// 
// TriggerFiller.h created on Thu Jul 2 14:13:32 CEST 2015 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISBASE_ANALYZER_TRIGGERFILLER_H
#define ANALYSISBASE_ANALYZER_TRIGGERFILLER_H

#include "AnalysisBase/Analyzer/interface/BaseFiller.h"

#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"

namespace ucsbsusy {

  typedef std::map<std::string, unsigned int>	TrigIdMap;

  class TriggerFiller : public BaseFiller {

  public :
    TriggerFiller(const int options,
                  const string branchName,
                  const edm::InputTag triggerBitTag,
                  const edm::InputTag triggerObjTag,
                  const edm::InputTag triggerPrescaleTag);
    ~TriggerFiller() {}

    enum Options {
      NULLOPT = 0
    };
    static const int defaultOptions = NULLOPT;

    void load(const edm::Event& iEvent);
    void fill();
    void initTriggerNames();

  private :
    const edm::InputTag triggerBitTag_;
    const edm::InputTag triggerObjTag_;
    const edm::InputTag triggerPrescaleTag_;
    const edm::TriggerNames *triggerNames_;
    TrigIdMap trigIds_;
    TrigIdMap trigFilterIds_;

    size itrig_bit_flag;
    size itrig_bit_pass;
    size itrig_bit_prescale;
    size itrig_obj_pt;
    size itrig_obj_eta;
    size itrig_obj_phi;
    size itrig_obj_mass;
    size itrig_obj_pathflags;
    size itrig_obj_filterflags;

  public :
    edm::Handle<edm::TriggerResults> triggerBits_;
    edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects_;
    edm::Handle<pat::PackedTriggerPrescales> triggerPrescales_;

  };

}

#endif
