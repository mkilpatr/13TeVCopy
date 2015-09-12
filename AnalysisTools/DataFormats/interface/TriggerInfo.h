//--------------------------------------------------------------------------------------------------
// 
// TriggerInfo
// 
// Class for holding trigger information.
// 
// TriggerInfo.h created on Tue Sep 1 14:56:00 CEST 2015
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_DATAFORMATS_TRIGGERINFO_H
#define ANALYSISTOOLS_DATAFORMATS_TRIGGERINFO_H

#include "AnalysisTools/DataFormats/interface/Momentum.h"

namespace ucsbsusy {

  class TriggerInfo
  {

    public :
      TriggerInfo() : trigflag_(0), passtrig_(false), prescale_(0) {}
      TriggerInfo(const unsigned long inTrigflag = 0, const bool inPasstrig = false, const unsigned int inPrescale = 0 ) :
        trigflag_(inTrigflag), passtrig_(inPasstrig), prescale_(inPrescale) {}

      ~TriggerInfo() {}

      unsigned long trigflag() const { return trigflag_; }
      bool          passtrig() const { return passtrig_; }
      unsigned int  prescale() const { return prescale_; }

      void setTrigflag (const unsigned long newTrigflag) { trigflag_ = newTrigflag; }
      void setPasstrig (const bool          newPasstrig) { passtrig_ = newPasstrig; }
      void setPrescale (const unsigned int  newPrescale) { prescale_ = newPrescale; }

    protected :
      unsigned long trigflag_;
      bool          passtrig_;
      unsigned int  prescale_;

  };

  typedef std::vector<TriggerInfo>     TriggerInfoCollection;

}

#endif
