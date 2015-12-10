
#ifndef ANALYSISBASE_TREEANALYZER_QCDRespSmearingAnalyzer_H
#define ANALYSISBASE_TREEANALYZER_QCDRespSmearingAnalyzer_H

#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"

//Actual functions that do the smearing

namespace ucsbsusy {
class QCDRespSmearingBaseEventAnalyzer;
}

namespace JetRespSmear {
enum WindowType {FLAT, LINEAR_GRANULATED, SECORDER_GRANULATED};

struct SmearOptions{
  double minWindow;
  double maxWindow;
  unsigned int nSmears;
  unsigned int nSmearJets;
  unsigned int nBootstraps;
  WindowType winType;
  bool doFlatSampling;
  TString respFileName;
  TString respInputName;

  SmearOptions();
  friend std::ostream& operator<<(std::ostream& os, const SmearOptions& a);
};

struct SmearInfo{
  const ucsbsusy::GenJetF* gJ;
  const int rJI;
  const TH1 * cdf;
  const double minProb;
  const double maxProb;
  const double minRes;
  const double maxRes;

  SmearInfo();
  SmearInfo(const ucsbsusy::GenJetF* gJ, int rJI, const TH1 * cdf, double minProb, double maxProb, double minRes, double maxRes);
};

//--------------- Interpolation functions ---------------------------////
double interpolateResToProb(const TH1 * cdf, const double res);
double interpolateProbToRes(const TH1 * cdf, const double prob);

//--------------- Window getting functions ---------------------------////
//return a window that scales linearly with response
 double getScaledWindow(const double res,const double minW,const double maxW);
//return smallest resp value that includes "res" in its window
double getUpIntegratedScaledWindow(const double res,const double minW,const double maxW);
//return largest resp value that includes "res" in its window
double getLowIntegratedScaledWindow(const double res,const double minW,const double maxW);

//--------------- Window getting functions...this time for POL2 scaling---------------------------////
double getScaledWindowPol2(const double res,const double minW,const double maxW);
double getUpIntegratedScaledWindowPol2(const double res,const double minW,const double maxW);
double getLowIntegratedScaledWindowPol2(const double res,const double minW,const double maxW);

//--------------- Functions to get the probability that the window covers (and the window in some)---------------------------////
//--------------- minProb,maxProb, minRes, maxRes are the outputvalues                            ---------------------------////
//Input a response window...get the probability it covers
//Here minRes,maxRes input values actually matter
void getWindowProb(const TH1 * cdf, double& minProb, double& maxProb, double& minRes,double& maxRes);
//For no window scaling... only use maxWindow
void getConstWindowAndProb      (const TH1 * cdf, const double res, const double minWindow, const double maxWindow, double& minProb, double& maxProb, double& minRes, double& maxRes);
//For window scaling
void getScaledWindowAndProb    (const TH1 * cdf, const double res, const double minWindow, const double maxWindow, double& minProb, double& maxProb, double& minRes, double& maxRes);

//For window scaling (with a pol2)
void getScaledWindowAndProbPol2(const TH1 * cdf, const double res, const double minWindow, const double maxWindow, double& minProb, double& maxProb, double& minRes, double& maxRes);
//get the range and covered probability of responses that can contribute to the input res
void getContributionScaledWindowAndProb(const TH1 * cdf, const double res, const double minWindow, const double maxWindow, double& minProb, double& maxProb,double& minRes, double& maxRes);
//get the range and covered probability of responses that can contribute to the input res...for a pol2
void getContributionScaledWindowAndProbPol2(const TH1 * cdf, const double res, const double minWindow, const double maxWindow, double& minProb, double& maxProb,double& minRes, double& maxRes);
}


namespace ucsbsusy {

//--------------------------------------------------------------------------------------------------
//  helper class to get
//--------------------------------------------------------------------------------------------------

class QCDRespSmearingBaseEventAnalyzer : public BaseEventAnalyzer{
public:

  QCDRespSmearingBaseEventAnalyzer();
  virtual ~QCDRespSmearingBaseEventAnalyzer();

  //--------------- Function that applies the smearing---------------------------////
  //--------------- Warning! this function changes the event!!!------------------////
  //---------------It repeatedly callss the runOneInstance()  ------////
  void applySmearing(BaseTreeAnalyzer * analyzer);

  virtual void setupSmearing(BaseTreeAnalyzer * analyzer);
  virtual void runOneInstance(BaseTreeAnalyzer * analyzer);
  virtual bool doAnything() const {return doAnything_;}
  virtual bool canSmearEvent(BaseTreeAnalyzer * analyzer) const {return doAnything() && analyzer->evtInfoReader.process == defaults::QCD;}
  virtual void analyzeEvent(BaseTreeAnalyzer * ana, int reportFrequency, int numEvents);

  JetRespSmear::SmearOptions smearOptions;
  TFile * respFile;
  const QuickRefold::TH1FContainer* respCont;
  float smearWeight;
  std::vector<ucsbsusy::size8> bootstrapWeights;

private:
  bool doAnything_;
};

class QCDRespSmearingCopierEventAnalyzer : public QCDRespSmearingBaseEventAnalyzer {
public:

  QCDRespSmearingCopierEventAnalyzer();
  virtual ~QCDRespSmearingCopierEventAnalyzer() {}

  virtual void bookSmearingWeights (TreeCopier * analyzer);
  virtual void fillSmearingWeights(TreeCopier * analyzer);

  virtual void runOneInstance(BaseTreeAnalyzer * analyzer);
  virtual void analyzeEvent(BaseTreeAnalyzer * ana, int reportFrequency, int numEvents);

  size i_smearWeight ;
  size i_bootstrapWeight ;
};




}


#endif
