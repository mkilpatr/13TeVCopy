/*
 * SoftdropWTagMVA.h
 *
 *  Created on: Nov 9, 2016
 *      Author: hqu
 */

#ifndef ANALYSISTOOLS_OBJECTSELECTION_INTERFACE_SOFTDROPWTAGMVA_H_
#define ANALYSISTOOLS_OBJECTSELECTION_INTERFACE_SOFTDROPWTAGMVA_H_

#include "AnalysisTools/DataFormats/interface/FatJet.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisBase/TreeAnalyzer/interface/TMVAReader.h"


namespace ucsbsusy {

class SoftdropWTagMVA {
public:
  SoftdropWTagMVA(TString weightfile, TString mvaname="BDTG");
  virtual ~SoftdropWTagMVA();

  float getSoftdropWTagMVAScore(const FatJetF* fatjet);

  static constexpr double WP_LOOSE  = -0.28;
  static constexpr double WP_MEDIUM = 0.05;
  static constexpr double WP_TIGHT  = 0.17;

  // accessed here and in extra tree fillers
  static bool isPreselected(const FatJetF* fatjet);
  static std::map<TString, float> calcSoftdropWTagMVAVars(const FatJetF * fatjet, bool fullMonty=false);

private:
  void initSoftdropWTagMVA();

  TMVAReader mvaReader;
  std::vector<TString> varsF;
  std::vector<TString> varsI;

};

} /* namespace ucsbsusy */

#endif /* ANALYSISTOOLS_OBJECTSELECTION_INTERFACE_SOFTDROPWTAGMVA_H_ */
