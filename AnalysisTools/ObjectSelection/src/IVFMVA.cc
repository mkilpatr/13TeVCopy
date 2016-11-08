/*
 * IVFMVA.cc
 *
 *  Created on: Oct 6, 2016
 *      Author: hqu
 */

#include "AnalysisTools/ObjectSelection/interface/IVFMVA.h"
#include "AnalysisTools/TreeReader/interface/Defaults.h"

using namespace ucsbsusy;

IVFMVA::IVFMVA(TString weightfile, TString mvaname) :mvaReader(weightfile, mvaname) {
  initIVFMVA();
}

IVFMVA::~IVFMVA() {
}

float IVFMVA::getIVFCandScore(const SVF& sv) {

  auto varMap = calcIVFCandVars(sv);
  for (const auto &v: varsI){
    mvaReader.setValue(v, varMap.at(v));
  }
  for (const auto &v: varsF){
    mvaReader.setValue(v, varMap.at(v));
  }

  float bdtscore = mvaReader.eval();

  return bdtscore;
}

void IVFMVA::initIVFMVA() {

  varsI = {
    "SV_ntracks",
  };

  varsF = {
    "SV_mass",
    "SV_ip3d",
    "SV_sip3d",
    "min(SV_chi2/max(1,SV_ndof),10)",
    "max(SV_cosTheta,0.98)",
  };

  mvaReader.addVariables(varsI);
  mvaReader.addVariables(varsF);

}


std::map<TString, float> IVFMVA::calcIVFCandVars(const SVF& ivfcand) {
  std::map<TString, float> vars;

  vars["SV_ntracks"]                     = ivfcand.svNTracks();
  vars["SV_mass"]                        = ivfcand.mass();
  vars["SV_ip3d"]                        = ivfcand.svd3D();
  vars["SV_sip3d"]                       = (ivfcand.svd3D())/(ivfcand.svd3Derr());
  vars["min(SV_chi2/max(1,SV_ndof),10)"] = (ivfcand.svChi2())/(ivfcand.svNdf());
  vars["max(SV_cosTheta,0.98)"]          = ivfcand.svCosSVPV();

  return vars;
}
