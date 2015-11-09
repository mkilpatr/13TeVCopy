/*
 * CorrectionSet.h
 *
 *  Created on: Aug 4, 2015
 *      Author: nmccoll
 */

#ifndef ANALYSISBASE_TREEANALYZER_INTERFACE_CORRECTIONSET_H_
#define ANALYSISBASE_TREEANALYZER_INTERFACE_CORRECTIONSET_H_

#include "AnalysisTools/QuickRefold/interface/Refold.h"
#include "AnalysisTools/TreeReader/interface/Defaults.h"
#include "AnalysisTools/DataFormats/interface/GenParticle.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"

class TFile;

namespace ucsbsusy {
class BaseTreeAnalyzer;

enum CORRTYPE {NOMINAL, UP, DOWN, NONE};
TString corrTypeName (CORRTYPE type);

//General class that holds a single correction
class Correction {
public:
  Correction(TString corrName);
  virtual ~Correction() {}
protected:
  const TString name;
};

class RefoldCorrection : public Correction {
public:
  RefoldCorrection(TString corrName, TFile * file);
  virtual float get() const { return corr->getValue(); }
  void setAxis(unsigned int a, float value) const {corr->setBin(a,value);}
protected:
  const QuickRefold::Refold * corr;
};

class HistogramCorrection : public Correction {
public:
  HistogramCorrection(TString corrName, TFile * file);
  void setTargetBin(unsigned int a) {targetBin = a;}
  void setAxis(float value) { targetBin = corrHist->FindBin(value); }
  virtual float get() const { return corrHist->GetBinContent(targetBin);}
  virtual float getError() const { return corrHist->GetBinError(targetBin);}
  TH1F* getHist()        { return corrHist; }
protected:
TH1F* corrHist;
unsigned int targetBin;
};

class LepHistogramCorrection : public Correction {
public:
  LepHistogramCorrection(TString corrName, TString tnpElFileName, TString tnpMuFileName);
  virtual ~LepHistogramCorrection();
  virtual float getElValue(float pt, float eta) const { return corrHistEl->GetBinContent(corrHistEl->GetXaxis()->FindFixBin(pt)
                                                                                        ,corrHistEl->GetYaxis()->FindFixBin(eta)); }
  virtual float getMuValue(float pt, float eta) const { return corrHistMu->GetBinContent(corrHistMu->GetXaxis()->FindFixBin(pt)
                                                                                        ,corrHistMu->GetYaxis()->FindFixBin(eta)); }
  virtual float getElError(float pt, float eta) const { return corrHistEl->GetBinError  (corrHistEl->GetXaxis()->FindFixBin(pt)
                                                                                        ,corrHistEl->GetYaxis()->FindFixBin(eta)); }
  virtual float getMuError(float pt, float eta) const { return corrHistMu->GetBinError  (corrHistMu->GetXaxis()->FindFixBin(pt)
                                                                                        ,corrHistMu->GetYaxis()->FindFixBin(eta)); }
protected:
  TFile* fileEl;
  TFile* fileMu;
  TH2F*  corrHistEl;
  TH2F*  corrHistMu;
};

class CorrectionSet {
public:
  CorrectionSet();
  virtual ~CorrectionSet();
  virtual void processCorrection(const BaseTreeAnalyzer * ana) = 0;
protected:
  virtual void loadSimple(TString correctionSetName, int correctionOptions);
  virtual void loadFile(TString correctionSetName, TString fileName, int correctionOptions);

  TString name;
  TFile * file;
  int options_;
  std::vector<Correction *> corrections;
};

} /* namespace ucsbsusy */

#endif /* ANALYSISBASE_TREEANALYZER_INTERFACE_CORRECTIONSET_H_ */
