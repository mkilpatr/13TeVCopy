//--------------------------------------------------------------------------------------------------
// 
// BaseReader
// 
// Class to help with reading branches from TTrees.
// 
// BaseReader.cc created on Tue Oct 21 15:33:32 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisTools/Utilities/interface/BaseReader.h"

using namespace ucsbsusy;

int BaseReader::loadEvent(Long64_t entry)
{

  assert(fTree);
  return fTree->GetEntry(entry);

}
