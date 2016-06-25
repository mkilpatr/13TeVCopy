/*
 * CorrectionSet.cc
 *
 *  Created on: Aug 4, 2015
 *      Author: nmccoll
 */
#include <assert.h>
#include "TFile.h"
#include "TMath.h"
#include "TList.h"
#include "TF1.h"

#include "AnalysisBase/TreeAnalyzer/interface/CorrectionSet.h"

namespace ucsbsusy {

TString corrTypeName (CORRTYPE type) {
  switch(type){
    case NOMINAL:
      return "NOMINAL";
    case UP:
      return "UP";
    case DOWN:
      return "DOWN";
    case NONE:
      return "NONE";
    default:
      return "??";
  }
}

Correction::Correction(TString corrName) : name(corrName) {
  std::clog << "Loading correction: "<< name <<std::endl;
}
RefoldCorrection::RefoldCorrection(TString corrName, TFile * file) : Correction(corrName),corr(0) {
  if(!file) throw std::invalid_argument("RefoldCorrection::RefoldCorrection: file could not be found!");
  corr = (const QuickRefold::Refold*)(file->Get(name) );
  if(!corr) throw std::invalid_argument("RefoldCorrection::RefoldCorrection: Corrector could not be found!");
}

HistogramCorrection::HistogramCorrection(TString corrName, TFile * file) : Correction(corrName),targetBin(1),inputFile(0) {
  if(!file) throw std::invalid_argument("HistogramCorrection::HistogramCorrection: file could not be found!");
  corrHist = (TH1F*)(file->Get(name) );
  if(!corrHist) throw std::invalid_argument("HistogramCorrection::HistogramCorrection: Histogram could not be found!");
}
HistogramCorrection::HistogramCorrection(TString corrName, TString fileName) : Correction(corrName),targetBin(1) {
  std::clog << "Loading file: "<< fileName << std::endl;
  inputFile = TFile::Open(fileName,"read");
  if(!inputFile) throw std::invalid_argument("HistogramCorrection::HistogramCorrection: file could not be found!");
  corrHist = (TH1F*)(inputFile->Get(name) );
  if(!corrHist) throw std::invalid_argument("HistogramCorrection::HistogramCorrection: Histogram could not be found!");
}
HistogramCorrection::~HistogramCorrection() {
  if(inputFile) inputFile->Close();
}
Histogram2DCorrection::Histogram2DCorrection(TString corrName, TFile * file) : Correction(corrName),targetBinX(1),targetBinY(1),inputFile(0) {
  if(!file) throw std::invalid_argument("Histogram2DCorrection::Histogram2DCorrection: file could not be found!");
  corrHist = (TH2F*)(file->Get(name) );
  if(!corrHist) throw std::invalid_argument("Histogram2DCorrection::Histogram2DCorrection: Histogram could not be found!");
}
Histogram2DCorrection::Histogram2DCorrection(TString corrName, TString fileName) : Correction(corrName),targetBinX(1),targetBinY(1) {
  std::clog << "Loading file: "<< fileName << std::endl;
  inputFile = TFile::Open(fileName,"read");
  if(!inputFile) throw std::invalid_argument("Histogram2DCorrection::Histogram2DCorrection: file could not be found!");
  corrHist = (TH2F*)(inputFile->Get(name) );
  if(!corrHist) throw std::invalid_argument("Histogram2DCorrection::Histogram2DCorrection: Histogram could not be found!");
}
Histogram2DCorrection::~Histogram2DCorrection() {
  if(inputFile) inputFile->Close();
}

CorrectionSet::CorrectionSet() : file(0), options_(0)  {}

void CorrectionSet::loadSimple(TString correctionSetName, int correctionOptions) {
  options_ =correctionOptions;
  std::clog << "Loading correctionSet: " << correctionSetName <<std::endl;
  return;

}

void CorrectionSet::loadFile(TString correctionSetName, TString fileName, int correctionOptions) {
  options_ |=correctionOptions;

  std::clog << "Loading file: "<< fileName <<" and correctionSet: " << correctionSetName <<std::endl;
  if(!file) file = TFile::Open(fileName,"read");
  if(!file) throw std::invalid_argument("CorrectionSet::loadFile: File could not be found!");
}

CorrectionSet::~CorrectionSet() {
  if(file)
    file->Close();
  delete file;

  for(auto correction : corrections)
    delete correction;
}

GraphAsymmErrorsCorrectionHelper::GraphAsymmErrorsCorrectionHelper(TString corrName, TFile* file) : targetBin(0), corrGraph(0) {
  if(!file) throw std::invalid_argument("GraphAsymmErrorsCorrection::GraphAsymmErrorsCorrection: file could not be found!");
  corrGraph = (TGraphAsymmErrors*)(file->Get(corrName));
  if(!corrGraph) throw std::invalid_argument("GraphAsymmErrorsCorrection::GraphAsymmErrorsCorrection: Graph could not be found!");
}

void GraphAsymmErrorsCorrectionHelper::findBin(double x)  {
  // Adapted from TGraph::Eval()
  auto fX = corrGraph->GetX();
  auto fNpoints = corrGraph->GetN();

  if (fX==0) throw std::invalid_argument("[GraphAsymmErrorsCorrection::findBin] Empty graph!");

  if (x <= fX[0]) targetBin = 0;
  else if (x >= fX[fNpoints-1]) targetBin = fNpoints-1;
  else targetBin = TMath::BinarySearch(fNpoints, fX, x);
//  else if (corrGraph->TestBit(TGraph::kIsSortedX) ) targetBin = TMath::BinarySearch(fNpoints, fX, x);
//  else throw std::invalid_argument("[GraphAsymmErrorsCorrection::findBin] Unable to find the bin: points not sorted!");
}

double GraphAsymmErrorsCorrectionHelper::getFromFunction(double x) const  {
  auto *funcs = corrGraph->GetListOfFunctions();
  if(funcs && funcs->At(0)) return ((TF1*)funcs->At(0))->Eval(x);
  else throw std::invalid_argument("[GraphAsymmErrorsCorrection::getFromFunction] No valid fit function!");
}

} /* namespace ucsbsusy */


