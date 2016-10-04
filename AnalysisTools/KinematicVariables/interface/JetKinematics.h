
#ifndef ANALYSISTOOLS_KINEMATICVARIABLES_JETKINEMATICS_H
#define ANALYSISTOOLS_KINEMATICVARIABLES_JETKINEMATICS_H

/*
 * Basic jet kinematic variables
 * The default arguments assume that your collections are already selected...in that you do not
 * want to place any addition pT or eta requirements
 *
 * Jet pT sorting is assumed!!
 *
 */
#include <vector>
#include <utility>

using namespace std;

namespace ucsbsusy {


namespace JetKinematics {
  const double noDist = 10.7;

  template<typename Jet>
  Jet mht(const std::vector<Jet*>& jets, const double minPT = 0, const double maxEta = 9999, const std::vector<bool>* selected = 0);

  template<typename Jet>
  double ht(const std::vector<Jet*>& jets, const double minPT = 0, const double maxEta = 9999, const std::vector<bool>* selected = 0);

  template<typename MET,typename Jet>
  double absDPhiMETJ(const MET& met, const Jet& jet, const double minPT = 0, const double maxEta = 9999);

  template<typename MET,typename Jet>
  double absDPhiMETJ12(const MET& met, const std::vector<Jet*>& jets, const double minPT = 0, const double maxEta = 9999);

  template<typename MET,typename Jet>
  double absDPhiMETJ3(const MET& met, const std::vector<Jet*>& jets, const double minPT = 0, const double maxEta = 9999);

  template<typename Momentum1, typename Momentum2>
  double transverseMass(const Momentum1& visible, const Momentum2& invisible);

  template<typename MET,typename Jet>
  double htAlongHtAway(const MET& met, const std::vector<Jet*>& jets, const double minPT = 0, const double maxEta = 9999);

  template<typename MET,typename Jet>
  double htAlong(const MET& met, const std::vector<Jet*>& jets, const double minPT = 0, const double maxEta = 9999);

  template<typename MET,typename Jet>
  double htAway(const MET& met, const std::vector<Jet*>& jets, const double minPT = 0, const double maxEta = 9999);


  template<typename Jet>
  double ptRMS(const std::vector<Jet*>& jets, const double minPT = 0, const double maxEta = 9999);

  template<typename MET,typename Jet>
  double deltaPhiMETRMS(const MET& met, const std::vector<Jet*>& jets, const double minPT = 0, const double maxEta = 9999);

  template<typename Jet>
  double bJetInvMass(const std::vector<Jet*>& jets, bool (*isBJet)(const Jet&), const double minPT = 0, const double maxEta = 9999);

  template<typename MET,typename Jet>
  double bJetTranverseMass(const MET& met, const std::vector<Jet*>& jets, bool (*isBJet)(const Jet&), const double minPT = 0, const double maxEta = 9999);

  template<typename Jet>
  double deltaEtaBJetRMS(const std::vector<Jet*>& jets, bool (*isBJet)(const Jet&), const double minPT = 0, const double maxEta = 9999);

  template<typename Jet>
  double highestPTJetPair(const std::vector<Jet*>& jets, const double minPT = 0, const double maxEta = 9999);



  template<typename Jet>
  static bool passCuts(const Jet& jet,const double minPT, const double maxEta);
}

}


#include "AnalysisTools/KinematicVariables/src/JetKinematics.icc"
#endif //ANALYSISTOOLS_KINEMATICVARIABLES_JETKINEMATICS_H

