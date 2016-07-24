#ifndef ANALYSISBASE_TREEANALYZER_INTERFACE_ISRCORRECTIONSET_H_
#define ANALYSISBASE_TREEANALYZER_INTERFACE_ISRCORRECTIONSET_H_

#include "AnalysisBase/TreeAnalyzer/interface/CorrectionSet.h"
#include "AnalysisTools/Utilities/interface/JetFlavorInfo.h"



namespace QuickRefold {
class Refold;
}

namespace ucsbsusy {

class ISRCorr : public Correction {
public:
  enum sigAxes {MASS1,MASS2,MASS3,TYPE};

  ISRCorr(TString corrInput,TString sigNormInput, const std::vector<TString>& sigNormNames  );
  ~ISRCorr();

  double getCorrFactor(CORRTYPE type, int nISRJets) const;
  double getSignalNormFactor(CORRTYPE type, const defaults::SignalType sigType, const std::vector<int>& massParams ) const;
  double getSignalNormCorrFactor(CORRTYPE type,  const defaults::SignalType sigType, const std::vector<int>& massParams, int nISRJets) const
    {return getSignalNormFactor(type,sigType,massParams)*getCorrFactor(type,nISRJets);}

  TFile * corrFile;
  const TH1 * corr;

  TFile * sigNormFile;
  std::map<defaults::SignalType,const QuickRefold::Refold *> sigNorms;

};


class ISRCorrectionSet : public CorrectionSet {
public:
  enum  CorrectionOptions {
                            NULLOPT            = 0
                          , ISRCORR        = (1 <<  0)
                          , ISRCORRTIGHT   = (1 <<  1)
  };
  ISRCorrectionSet(): isrCorr(0),isrCorrTight(0), isrWeight(1), nISRJets(0), massParams(3),isrWeightTight(1),nISRJetsTight(0) {}

  virtual ~ISRCorrectionSet() {};
  virtual void load(TString corrInput,TString normInput,TString normTightInput, const std::vector<TString>& normNames, int correctionOptions = NULLOPT);
  virtual void processCorrection(const BaseTreeAnalyzer * ana);
  int getNISRJets(const BaseTreeAnalyzer * ana, bool tight = false) const;

  //individual accessors
  float getISRWeight() const { return isrWeight; }
  const ISRCorr* getISRCorrector() const { return isrCorr; }
  int getNumberOfISRJets() const {return nISRJets;}
  float getISRWeightTight() const { return isrWeightTight; }
  const ISRCorr* getISRCorrectorTight() const { return isrCorrTight; }
  int getNumberOfISRJetsTight() const {return nISRJetsTight;}

private:
  //Correction list
  ISRCorr * isrCorr;
  ISRCorr * isrCorrTight;

  //output values
  float isrWeight;
  int nISRJets;
  std::vector<int> massParams;

  float isrWeightTight;
  int nISRJetsTight;


};


} /* namespace ucsbsusy */

#endif /* ANALYSISBASE_TREEANALYZER_INTERFACE_CORRECTIONSET_H_ */