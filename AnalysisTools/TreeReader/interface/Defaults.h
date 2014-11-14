//--------------------------------------------------------------------------------------------------
// 
// BaseReader
// 
// Class to help with reading branches from TTrees.
// 
// BaseReader.h created on Tue Oct 21 15:33:32 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISTOOLS_TREEREADER_DEFAULTS_H
#define ANALYSISTOOLS_TREEREADER_DEFAULTS_H

#include <string>

namespace ucsbsusy {
namespace defaults {
  const std::string BRANCH_AK4JETS   = "ak4pfchs";
  const std::string BRANCH_STDGENJETS= "ak4";
  const std::string BRANCH_ELECTRONS = "ele";
  const std::string BRANCH_MUONS     = "mu";
  const std::string BRANCH_TAUS      = "tau";
  const std::string BRANCH_GENPARTS  = "gen";
  const double CSV_LOOSE      = 0.423;
  const double CSV_MEDIUM     = 0.814;
  const double CSV_TIGHT      = 0.941;
  const double CSV_OLD_LOOSE  = 0.244;
  const double CSV_OLD_MEDIUM = 0.679;
  const double CSV_OLD_TIGHT  = 0.898;
}
}

#endif
