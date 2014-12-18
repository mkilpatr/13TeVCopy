#ifndef TOPJETMATCHING_H_
#define TOPJETMATCHING_H_

#include "AnalysisTools/Utilities/interface/Types.h"


namespace TopJetMatching {

//types used for containment
//kept as typedef for easier changing if the need arises
typedef ucsbsusy::int8 conType;  //containment (in percent...so times 100)...if negative means we have a new parton

//because we want to store the contianments as chars, let's have these conversions handy
conType toContainmentType(const float inCon );
float fromContainmentType(const conType inCon);
//
//enum  PartonDiagnosis {
//                        SOFT_PARTON        //low pT...no use matching
//                      , NO_JET             //Not associated to any jet
//                      , CONFUSED_JETS      //Split and merged jet
//                      , SPLIT_JETS         //Parton deposits sign. energy in more than one jet
//                      , MERGED_JET         //Jet with parton energy also has significant energy from another
//                      , DIRTY_JET          //Jet has significant contribution from non-hard sources
//                      , RESOLVED_PARTON    //Fully resolved parton
//                      , numPartonDiagnoses
//                      };
//
//
//template<typename Particle>
//class PartonJetAssoc{
//public:
//  const Particle* particle;
//  PartonDiagnosis diag;
//  const int       mainJetIdx; //index in jetContainments of the main jet (most energy)
//  const std::vector<std::pair<iJet,iCon> > jetContainments;
//
//
//  void diagnoseParton(iCon sigEFraction, iCon minEFraction);
//};
//
//
};

//#include "AnalysisTools/Utilities/src/TopJetMatching.icc"


#endif
