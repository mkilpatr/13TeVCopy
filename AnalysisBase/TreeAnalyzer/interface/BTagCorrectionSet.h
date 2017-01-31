/*
 * CorrectionSet.h
 *
 *  Created on: Aug 4, 2015
 *      Author: nmccoll
 */

#ifndef ANALYSISBASE_TREEANALYZER_INTERFACE_BTAGCORRECTIONSET_H_
#define ANALYSISBASE_TREEANALYZER_INTERFACE_BTAGCORRECTIONSET_H_

#include "AnalysisBase/TreeAnalyzer/interface/CorrectionSet.h"
#include "AnalysisTools/Utilities/interface/JetFlavorInfo.h"

class BTagCalibration;
class BTagCalibrationReader;

namespace QuickRefold {
template<typename data>
class TObjectContainer;
typedef   TObjectContainer<TH1F>  TH1FContainer;
}

namespace ROOT{
namespace Math{
template <class ScalarType>
class PtEtaPhiM4D;
}
}

namespace ucsbsusy {
typedef ROOT::Math::PtEtaPhiM4D<float> CylLorentzCoordF;
template <class CoordSystem>
class RecoJet;
typedef RecoJet<CylLorentzCoordF> RecoJetF;


class BTagByEvtWeightCorr : public Correction {
public:
  enum axes {TYPE,FLAVOR,ETA,WP};
  enum meastype {COMB,INCL};

  BTagByEvtWeightCorr(TString effInput, TString sfInput, bool isFastSim );
  ~BTagByEvtWeightCorr();

  int flavorToOurHistBin(JetFlavorInfo::JetFlavor flavor) const;

  double getJetEff         (double jetPT, double jetETA, JetFlavorInfo::JetFlavor flavor, defaults::CSVWPs wp, bool isTtbarLike) const;
  double getJetFastSimEff  (double jetPT, double jetETA, JetFlavorInfo::JetFlavor flavor, defaults::CSVWPs wp, bool isTtbarLike) const;
  double getJetEffSF       (double jetPT, double jetETA, JetFlavorInfo::JetFlavor flavor, defaults::CSVWPs wp, CORRTYPE systType) const;
  double getJetFastSimEffSF(double jetPT, double jetETA, JetFlavorInfo::JetFlavor flavor, defaults::CSVWPs wp, CORRTYPE systType) const;

  double getJetWeight(const BaseTreeAnalyzer * ana, const RecoJetF* j, CORRTYPE lightCorrType, CORRTYPE heavyCorrType, bool isTtbarLike  ) const;
  double getEvtWeight(const BaseTreeAnalyzer * ana, const std::vector<RecoJetF*>& jets, CORRTYPE lightCorrType, CORRTYPE heavyCorrType, bool isTtbarLike, double maxETA, double minPT  ) const;

  BTagCalibration * calib;
  double (BTagByEvtWeightCorr::*effGetter)(double jetPT, double jetETA, JetFlavorInfo::JetFlavor flavor, defaults::CSVWPs wp, bool isTtbarLike) const;
  double (BTagByEvtWeightCorr::*sfGetter)(double jetPT, double jetETA, JetFlavorInfo::JetFlavor flavor,defaults::CSVWPs wp, CORRTYPE systType) const;
  std::vector<BTagCalibrationReader*> corrReadersByOp;
  TFile * effFile;
  const QuickRefold::TH1FContainer * eff;
};


class BTagCorrectionSet : public CorrectionSet {
public:
  enum  CorrectionOptions {
                            NULLOPT            = 0
                          , BYEVTWEIGHT        = (1 << 0)
                          , FASTSIMBYEVTWEIGHT = (1 << 1)

  };
  BTagCorrectionSet(): bTagByEvtWeightCorr(0), bTagFastSimByEvtWeightCorr(0), bTagByEvtWeight(1), bTagFastSimByEvtWeight(1) {}

  virtual ~BTagCorrectionSet() {};
  virtual void load(TString effFileName,TString sfFileName,TString fastSimEffFileName,TString fastSimSfFileName, int correctionOptions = NULLOPT);
  virtual void processCorrection(const BaseTreeAnalyzer * ana);

  //individual accessors
  float getBTagByEvtWeight() const { return bTagByEvtWeight; }
  float getBTagFastSimByEvtWeight() const { return bTagFastSimByEvtWeight; }
  const BTagByEvtWeightCorr* getBTagByEvtWeightCorrector() const { return bTagByEvtWeightCorr; }
  const BTagByEvtWeightCorr* getBTagFastSimByEvtWeightCorrector() const { return bTagFastSimByEvtWeightCorr; }


private:
  //Correction list
  BTagByEvtWeightCorr * bTagByEvtWeightCorr;
  BTagByEvtWeightCorr * bTagFastSimByEvtWeightCorr;

  //output values
  float bTagByEvtWeight;
  float bTagFastSimByEvtWeight;
};


} /* namespace ucsbsusy */

#endif /* ANALYSISBASE_TREEANALYZER_INTERFACE_CORRECTIONSET_H_ */
