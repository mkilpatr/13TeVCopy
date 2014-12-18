

#include "AnalysisTools/Utilities/interface/TopJetMatching.h"

using namespace std;
using namespace ucsbsusy;
using namespace TopJetMatching;

//We only do > tests so conversion is to the percent floor
conType TopJetMatching::toContainmentType(const float inCon ){ return ucsbsusy::convertTo<conType>( std::floor(inCon*100) ,"TopJetMatching::toContainmentType");}
float TopJetMatching::fromContainmentType(const conType inCon){return float(inCon)/100; }
