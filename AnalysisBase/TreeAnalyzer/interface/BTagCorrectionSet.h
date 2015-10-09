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
#include "Math/PtEtaPhiM4D.h"

class BTagCalibration;
class BTagCalibrationReader;

namespace QuickRefold {
template<typename data>
class TObjectContainer;
typedef   TObjectContainer<TH1F>  TH1FContainer;
}

namespace ucsbsusy {
typedef ROOT::Math::PtEtaPhiM4D<float> CylLorentzCoordF;
template <class CoordSystem>
class RecoJet;
typedef RecoJet<CylLorentzCoordF> RecoJetF;


class BTagByEvtWeightCorr : public Correction {
public:
  enum axes {TYPE,FLAVOR,ETA,WP};
  enum meastype {MUJET,COMB};

  BTagByEvtWeightCorr(TString effInput, TString sfInput );
  ~BTagByEvtWeightCorr();

  double getJetEff(double jetPT, double jetETA, JetFlavorInfo::JetFlavor flavor, defaults::CSVWPs wp, bool isTTBARLike) const;
  double getJetEffSF(double jetPT, double jetETA, JetFlavorInfo::JetFlavor flavor,defaults::CSVWPs wp, CORRTYPE sytType) const;

  double getJetWeight(const RecoJetF* j, CORRTYPE lightCorrType, CORRTYPE heavyCorrType, bool isTTBARLike  ) const;
  double getEvtWeight(const std::vector<RecoJetF*>& jets, CORRTYPE lightCorrType, CORRTYPE heavyCorrType, bool isTTBARLike  ) const;



  BTagCalibration * calib;
  std::vector<std::vector<std::vector<BTagCalibrationReader*> > > corrReaders; // [mujet/COMB] [L/M/T] [ Nom/Up/Down]
  TFile * effFile;
  const QuickRefold::TH1FContainer * eff;
};


class BTagCorrectionSet : public CorrectionSet {
public:
  enum  CorrectionOptions {
                            NULLOPT          = 0
                          , BYEVTWEIGHT      = (1 <<  0)

  };
  BTagCorrectionSet(): bTagByEvtWeightCorr(0), bTagByEvtWeight(1), bTagByEvtWeight_HeavyType(NONE), bTagByEvtWeight_LightType(NONE) {}

  virtual ~BTagCorrectionSet() {};
  virtual void load(TString effFileName,TString sfFileName,CORRTYPE lightCorrType, CORRTYPE heavyCorrType,  int correctionOptions = NULLOPT);
  virtual void processCorrection(const BaseTreeAnalyzer * ana);

  //individual accessors
  float getBTagByEvtWeight() const { return bTagByEvtWeight; }
  const BTagByEvtWeightCorr* getBTagByEvtWeightCorrector() const { return bTagByEvtWeightCorr; }


private:
  //Correction list
  BTagByEvtWeightCorr * bTagByEvtWeightCorr;

  //output values
  float bTagByEvtWeight;
  CORRTYPE bTagByEvtWeight_HeavyType;
  CORRTYPE bTagByEvtWeight_LightType;

};


} /* namespace ucsbsusy */

#endif /* ANALYSISBASE_TREEANALYZER_INTERFACE_CORRECTIONSET_H_ */
