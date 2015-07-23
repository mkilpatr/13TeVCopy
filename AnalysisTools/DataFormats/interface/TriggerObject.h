//--------------------------------------------------------------------------------------------------
// 
// TriggerObject
// 
// Class for holding trigger object information.
// 
// TriggerObject.h created on Mon Jul 20 15:36:32 CEST 2015
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_DATAFORMATS_TRIGGEROBJECT_H
#define ANALYSISTOOLS_DATAFORMATS_TRIGGEROBJECT_H

#include "AnalysisTools/DataFormats/interface/Momentum.h"

namespace ucsbsusy {

  template < class CoordSystem>
  class TriggerObject : public Momentum<CoordSystem>
  {

    public :
      TriggerObject() : index_(-1), pathflags_(0), filterflags_(0) {}
      template <class InputCoordSystem>
      TriggerObject(const ROOT::Math::LorentzVector<InputCoordSystem> inMomentum,
                    const int inIndex = -1, const unsigned long inPathFlags = 0, const unsigned long inFilterFlags = 0) :
        Momentum<InputCoordSystem>(inMomentum), 
        index_(inIndex), pathflags_(inPathFlags), filterflags_(inFilterFlags) {}

      ~TriggerObject() {}

      int           index()       const { return index_;       }
      unsigned long pathflags()   const { return pathflags_;   }
      unsigned long filterflags() const { return filterflags_; }

      void setIndex      (const int newIndex)       { index_       = newIndex;       }
      void setPathFlags  (const int newPathFlags)   { pathflags_   = newPathFlags;   }
      void setFilterFlags(const int newFilterFlags) { filterflags_ = newFilterFlags; }

    protected :
      int            index_;  //Index in trigger object vector
      unsigned long  pathflags_;
      unsigned long  filterflags_;

  };

  typedef TriggerObject<CylLorentzCoordF> TriggerObjectF;
  typedef std::vector<TriggerObjectF>     TriggerObjectFCollection;

}

#endif
