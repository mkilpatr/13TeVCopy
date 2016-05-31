#ifndef ANALYSISBASE_TREEANALYZER_INTERFACE_WPOLCORRECTIONSET_H_
#define ANALYSISBASE_TREEANALYZER_INTERFACE_WPOLCORRECTIONSET_H_

#include "AnalysisBase/TreeAnalyzer/interface/CorrectionSet.h"
#include <vector>
#include "Math/LorentzVector.h"
#include "Math/VectorUtil.h"
#include "TLorentzVector.h"

namespace ucsbsusy {

class WPolCorrectionSet : public CorrectionSet {
public:
  enum  CorrectionOptions {
    NULLOPT         = 0
    , WPOLWGT     = (1 <<  0)   ///< Correct ZPT
  };
  WPolCorrectionSet(): wpolweightup(0), wpolweightdn(1), costhetastar(2) {}
  virtual ~WPolCorrectionSet() {};
  virtual void load(int correctionOptions = NULLOPT);
  virtual void processCorrection(const BaseTreeAnalyzer * ana);

  //individual accessors
  float getWpolWeightUp() const {return wpolweightup;}
  float getWpolWeightDn() const {return wpolweightdn;}
  float getCosThetaStar() const {return costhetastar;}

private:

  //stored variables
  float wpolweightup;
  float wpolweightdn;
  float costhetastar;
  bool wpolon;
};


} /* namespace ucsbsusy */

#endif /* ANALYSISBASE_TREEANALYZER_INTERFACE_CORRECTIONSET_H_ */
