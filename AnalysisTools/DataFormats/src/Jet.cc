//--------------------------------------------------------------------------------------------------
// 
// Jet
// 
// Class to hold basic jet information. To be enhanced as needed.
// 
// Jet.cc created on Tue Aug 19 16:26:39 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisTools/DataFormats/interface/Jet.h"

#include "AnalysisTools/Parang/interface/Planter.h"

template<>  const Char_t*  Planter::MultiVariable<ucsbsusy::Jet >::getTypeName() const { return  "Jet" ; }

ClassImp(ucsbsusy::Jet)
