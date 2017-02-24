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

  ISRCorr(TString corrInput,TString sigNormInput, const std::vector<TString>& sigNormNames, const std::vector<TString>& bkgNormNames  );
  ~ISRCorr();

  double getCorrFactor(CORRTYPE type, int nISRJets) const;
  double getSignalNormFactor(CORRTYPE type, const defaults::SignalType sigType, const std::vector<int>& massParams ) const;
  double getBKGNormFactor(CORRTYPE type, const defaults::Process process) const ;
  double getSignalNormCorrFactor(CORRTYPE type,  const defaults::SignalType sigType, const std::vector<int>& massParams, int nISRJets) const
    {
	  const double norm = getSignalNormFactor(type,sigType,massParams);
	  if(norm < 0) return -1;
	  return norm*getCorrFactor(type,nISRJets);
    }
  double getBKGNormCorrFactor(CORRTYPE type,  const defaults::Process process, int nISRJets) const
    {
	  const double norm =  getBKGNormFactor(type,process);
	  if(norm < 0) return -1;
	  return norm*getCorrFactor(type,nISRJets);
    }

  TFile * corrFile;
  const TH1 * corr;

  TFile * sigNormFile;
  std::map<defaults::SignalType,const QuickRefold::Refold *> sigNorms;
  std::map<defaults::Process,const QuickRefold::Refold *> bkgNorms;

};


class ISRCorrectionSet : public CorrectionSet {
public:
  enum  CorrectionOptions {
                            NULLOPT            = 0
                          , ISRCORR        = (1 <<  0)
                          , ISRCORRTIGHT   = (1 <<  1)
  };
  ISRCorrectionSet(): isrCorr(0),isrCorrTight(0), defType(NOMINAL), isrWeight(NONE+1,1), nISRJets(0), massParams(3),isrWeightTight(NONE+1,1),nISRJetsTight(0) {}

  virtual ~ISRCorrectionSet() {};
  virtual void load(TString corrInput,TString normInput,TString normTightInput, const std::vector<TString>& normNames,const std::vector<TString>& bkgNormNames, int correctionOptions = NULLOPT);
  virtual void processCorrection(const BaseTreeAnalyzer * ana);
  int getNISRJets(const BaseTreeAnalyzer * ana, bool tight = false) const;

  //individual accessors
  float getISRWeight() const { return isrWeight[defType]; }
  float getISRWeight(CORRTYPE type) const { return isrWeight[type]; }
  const ISRCorr* getISRCorrector() const { return isrCorr; }
  int getNumberOfISRJets() const {return nISRJets;}
  float getISRWeightTight() const { return isrWeightTight[defType]; }
  float getISRWeightTight(CORRTYPE type) const { return isrWeightTight[type]; }
  const ISRCorr* getISRCorrectorTight() const { return isrCorrTight; }
  int getNumberOfISRJetsTight() const {return nISRJetsTight;}

private:
  //Correction list
  ISRCorr * isrCorr;
  ISRCorr * isrCorrTight;

  //output values
  CORRTYPE defType;

  std::vector<float> isrWeight;
  int nISRJets;
  std::vector<int> massParams;


  std::vector<float> isrWeightTight;
  int nISRJetsTight;


};


} /* namespace ucsbsusy */

#endif /* ANALYSISBASE_TREEANALYZER_INTERFACE_CORRECTIONSET_H_ */
