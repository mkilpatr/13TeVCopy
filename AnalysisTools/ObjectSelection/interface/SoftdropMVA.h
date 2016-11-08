/*
 * SoftdropMVA.h
 *
 *  Created on: Nov 5, 2016
 *      Author: patterson
 */

#ifndef ANALYSISTOOLS_OBJECTSELECTION_INTERFACE_SOFTDROPMVA_H_
#define ANALYSISTOOLS_OBJECTSELECTION_INTERFACE_SOFTDROPMVA_H_

#include "AnalysisTools/DataFormats/interface/Jet.h"
#include "AnalysisTools/DataFormats/interface/FatJet.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisBase/TreeAnalyzer/interface/TMVAReader.h"


namespace ucsbsusy {

class SoftdropMVA {
public:
  SoftdropMVA(TString weightfile, TString mvaname);
  virtual ~SoftdropMVA();

  float getSoftdropMVAScore(const FatJetF* fatjet);
  std::vector<const FatJetF*> getSoftdropMVATops(const std::vector<const FatJetF*> &fatjets, double WP);

  static constexpr double WP_LOOSE  = 0.08;
  static constexpr double WP_MEDIUM = 0.35;
  static constexpr double WP_TIGHT  = 0.61;

  // accessed here and in extra tree fillers
  static bool isPreselected(const FatJetF* fatjet);
  static std::map<TString, float> calcSoftdropMVAVars(const FatJetF * fatjet, bool fullMonty);

private:
  void initSoftdropMVA();

  TMVAReader mvaReader;
  std::vector<TString> varsF;
  std::vector<TString> varsI;

};

} /* namespace ucsbsusy */

#endif /* ANALYSISTOOLS_OBJECTSELECTION_INTERFACE_SOFTDROPMVA_H_ */
