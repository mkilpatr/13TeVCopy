/*
 * HTTMVA.h
 *
 *  Created on: Nov 5, 2016
 *      Author: patterson
 */

#ifndef ANALYSISTOOLS_OBJECTSELECTION_INTERFACE_HTTMVA_H_
#define ANALYSISTOOLS_OBJECTSELECTION_INTERFACE_HTTMVA_H_

#include "AnalysisTools/DataFormats/interface/Jet.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisBase/TreeAnalyzer/interface/TMVAReader.h"
#include "AnalysisTools/DataFormats/interface/HTTFatJet.h"

namespace ucsbsusy {

class HTTMVA {
public:
  HTTMVA(TString weightfile, TString mvaname);
  virtual ~HTTMVA();
  
  float getHTTMVAScore(const HTTFatJetF* fatjet);
  bool isPreselected(const HTTFatJetF* fatjet);
  std::vector<const HTTFatJetF*> getHTTMVATops(const std::vector<const HTTFatJetF*> &fatjets, double WP);

  static constexpr double WP_TIGHT = 0.75;
  static constexpr double WP_LOOSE = 0.50;

private:
  void initHTTMVA();
  std::map<TString, float> calcHTTMVAVars(const HTTFatJetF * fatjet, bool fullMonty);

  TMVAReader mvaReader;
  std::vector<TString> varsF;
  std::vector<TString> varsI;

};

} /* namespace ucsbsusy */

#endif /* ANALYSISTOOLS_OBJECTSELECTION_INTERFACE_HTTMVA_H_ */
