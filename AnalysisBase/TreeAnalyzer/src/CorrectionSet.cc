/*
 * CorrectionSet.cc
 *
 *  Created on: Aug 4, 2015
 *      Author: nmccoll
 */
#include <assert.h>
#include "TFile.h"

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
  assert(file);
  corr = (const QuickRefold::Refold*)(file->Get(name) );
  if(!corr) throw std::invalid_argument("RefoldCorrection::RefoldCorrection: Corrector could not be found!");
}

HistogramCorrection::HistogramCorrection(TString corrName, TFile * file) : Correction(corrName),targetBin(1) {
  assert(file);
  corrHist = (TH1F*)(file->Get(name) );
  if(!corrHist) throw std::invalid_argument("HistogramCorrection::HistogramCorrection: Histogram could not be found!");
}

HistogramCorrection3D::HistogramCorrection3D(TString corrName, TFile * file) : Correction(corrName),binX(1),binY(1),binZ(1) {
  assert(file);
  corrHist = (TH3F*)(file->Get(name) );
  if(!corrHist) throw std::invalid_argument("HistogramCorrection3D::HistogramCorrection3D: Histogram could not be found!");
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
  file = TFile::Open(fileName,"read");
  if(!file) throw std::invalid_argument("CorrectionSet::loadFile: File could not be found!");
}

CorrectionSet::~CorrectionSet() {
  if(file)
    file->Close();
  delete file;

  for(auto correction : corrections)
    delete correction;
}
} /* namespace ucsbsusy */
