
#ifndef ANALYSISTOOLS_KINEMATICVARIABLES_JETKINEMATICS_H
#define ANALYSISTOOLS_KINEMATICVARIABLES_JETKINEMATICS_H

/*
 * Basic jet kinematic variables
 * The default arguments assume that your collections are already selected...in that you do not
 * want to place any addition pT or eta requirements
 *
 * Jet pT sorting is assumed!!
 */
#include <vector>
#include <utility>

using namespace std;

namespace ucsbsusy {


namespace JetKinematics {
  const double noDist = 999;

  template<typename Jet>
  Jet mht(const std::vector<Jet*>& jets, const double minPT = 0, const double maxEta = 9999, const std::vector<bool>* selected = 0);

  template<typename Jet>
  double ht(const std::vector<Jet*>& jets, const double minPT = 0, const double maxEta = 9999, const std::vector<bool>* selected = 0);

//  template<typename Jet>
//  int numJets(const std::vector<Jet*>& jets, const double minPT = 0, const double maxEta = 9999){return PhysicsUtilities::countObjectsDeref(jets,minPT,maxEta);}

  template<typename MET,typename Jet>
  double absDPhiMETJ(const MET& met, const Jet& jet, const double minPT = 0, const double maxEta = 9999);

  template<typename MET,typename Jet>
  double absDPhiMETJ12(const MET& met, const std::vector<Jet*>& jets, const double minPT = 0, const double maxEta = 9999);

  template<typename MET,typename Jet>
  double absDPhiMETJ3(const MET& met, const std::vector<Jet*>& jets, const double minPT = 0, const double maxEta = 9999);



  template<typename Jet>
  bool passCuts(const Jet& jet,const double minPT, const double maxEta);


  int dummyFunction();

}

}


#include "AnalysisTools/KinematicVariables/src/JetKinematics.icc"
#endif //ANALYSISTOOLS_KINEMATICVARIABLES_JETKINEMATICS_H

