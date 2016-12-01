/*
 * SoftdropWTopMulticlassMVA.h
 *
 *  Created on: Nov 18, 2016
 *      Author: hqu
 */

#ifndef ANALYSISTOOLS_OBJECTSELECTION_INTERFACE_SOFTDROPWTOPMULTICLASSMVA_H_
#define ANALYSISTOOLS_OBJECTSELECTION_INTERFACE_SOFTDROPWTOPMULTICLASSMVA_H_

#include "AnalysisTools/DataFormats/interface/FatJet.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisBase/TreeAnalyzer/interface/TMVAReader.h"


namespace ucsbsusy {

class SoftdropWTopMulticlassMVA {
public:
  SoftdropWTopMulticlassMVA(TString weightfile, TString mvaname="BDT");
  virtual ~SoftdropWTopMulticlassMVA();

  enum WTopLabel { FAKE, W, TOP };

  std::vector<float> getMVAScore(const FatJetF* fatjet);
  WTopLabel getMVADecision(const FatJetF* fatjet);

  // No reweighting
//  static constexpr double WP_W_LOOSE  = 0.3;
//  static constexpr double WP_W_TIGHT  = 0.6;
//
//  static constexpr double WP_TOP_LOOSE   = 0.8;
//  static constexpr double WP_TOP_MEDIUM  = 0.92;
//  static constexpr double WP_TOP_TIGHT   = 0.96;

  // flat PT
  static constexpr double WP_W_LOOSE  = 0.3;
  static constexpr double WP_W_MEDIUM = 0.6;
  static constexpr double WP_W_TIGHT  = 0.7;

  static constexpr double WP_TOP_LOOSE   = 0.4;
  static constexpr double WP_TOP_MEDIUM  = 0.6;
  static constexpr double WP_TOP_TIGHT   = 0.7;


  // accessed here and in extra tree fillers
  static bool isPreselected(const FatJetF* fatjet);
  static std::map<TString, float> calcMVAVars(const FatJetF * fatjet);

private:
  void initMVA();

  TMVAReader mvaReader;
  std::vector<TString> varsF;
  std::vector<TString> varsI;

};

} /* namespace ucsbsusy */

#endif /* ANALYSISTOOLS_OBJECTSELECTION_INTERFACE_SOFTDROPWTOPMULTICLASSMVA_H_ */
