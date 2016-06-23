/*
 * TriggerCorrectionSet.h
 *
 */

#ifndef ANALYSISBASE_TREEANALYZER_INTERFACE_TRIGGERCORRECTIONSET_H_
#define ANALYSISBASE_TREEANALYZER_INTERFACE_TRIGGERCORRECTIONSET_H_

#include "AnalysisBase/TreeAnalyzer/interface/CorrectionSet.h"

namespace ucsbsusy {

class PhotonTriggerCorrection : public Correction {
public :
  PhotonTriggerCorrection(TString corrName, TFile* file) :
    Correction(corrName), corrEE("HLT_Photon165_HE10_EE", file), corrEB("HLT_Photon165_HE10_EB", file) {} // needs a better way to set the graph names
  ~PhotonTriggerCorrection() {}

  const double ETA_CUT = 1.479;

  double get(CORRTYPE corrType, double pt, double eta);

private:
  GraphAsymmErrorsCorrectionHelper corrEE;
  GraphAsymmErrorsCorrectionHelper corrEB;
};

class TriggerCorrectionSet : public CorrectionSet {
public:
  enum  CorrectionOptions {
                            NULLOPT          = 0
                          , PHOTON           = (1 <<  0)
                          , ELECTRON         = (1 <<  1)
                          , MUON             = (1 <<  1)

  };
  TriggerCorrectionSet(): trigPhoCorr(0), trigPhoWeight(1) {}

  virtual ~TriggerCorrectionSet() {};
  virtual void load(TString filename, int correctionOptions = NULLOPT);
  virtual void processCorrection(const BaseTreeAnalyzer * ana);

  //individual accessors
  float getTrigPhoWeight() const {return trigPhoWeight;}

private:
  //Correction list
  PhotonTriggerCorrection *trigPhoCorr;

  //output values
  float trigPhoWeight;

};


} /* namespace ucsbsusy */

#endif /* ANALYSISBASE_TREEANALYZER_INTERFACE_TRIGGERCORRECTIONSET_H_ */
