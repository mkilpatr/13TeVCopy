/*
 * ResolvedTopMVA.h
 *
 *  Created on: Oct 6, 2016
 *      Author: hqu
 */

#ifndef ANALYSISTOOLS_OBJECTSELECTION_INTERFACE_IVFMVA_H_
#define ANALYSISTOOLS_OBJECTSELECTION_INTERFACE_IVFMVA_H_

#include "AnalysisTools/DataFormats/interface/SV.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisBase/TreeAnalyzer/interface/TMVAReader.h"


namespace ucsbsusy {


class IVFMVA {
public:
  IVFMVA(TString weightfile, TString mvaname="BDTG");
  virtual ~IVFMVA();

  float getIVFCandScore(const SVF& sv);

private:
  void initIVFMVA();
  std::map<TString, float> calcIVFCandVars(const SVF& ivfcand);

  //public:
  //  static constexpr double WP_TIGHT = 0.80;

private:
  TMVAReader mvaReader;
  std::vector<TString> varsI;
  std::vector<TString> varsF;


};

} /* namespace ucsbsusy */

#endif /* ANALYSISTOOLS_OBJECTSELECTION_INTERFACE_IVFMVA_H_ */
