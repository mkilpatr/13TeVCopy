#include "AnalysisBase/TreeAnalyzer/interface/MVAWrapper.h"
#include "AnalysisTools/Parang/interface/Panvariate.h"

namespace ucsbsusy {
MVAWrapper::MVAWrapper(std::string filename, std::string mvaName){
  TFile*              file1    = TFile::Open(filename.c_str(), "READ");
  if(!file1) throw std::invalid_argument("MVAWrapper::MVAWrapper: file could not be found!");
  para  = dynamic_cast<ParamatrixMVA*>(file1->Get(mvaName.c_str()));
  if(!para) throw std::invalid_argument("MVAWrapper::MVAWrapper: Paramatrix could not be found!");
  delete file1;
  mva=0;
}
int MVAWrapper::findAxis(std::string axisName) const{
  return para->findAxis(axisName.c_str());
}
void MVAWrapper::setParameters(std::vector<double> parameters) {
  mva = para->get(parameters);
  if(!mva) throw std::invalid_argument("MVAWrapper::setParameters: MVA could not be found!");
}
int MVAWrapper::findVariable(std::string varName) const{
  return mva->findVariable(varName.c_str()       );
}
void MVAWrapper::setVariable(int index, double value) const{
  mva->setVariable(index, value);
}
double MVAWrapper::evaluate(int method) const {
  return mva->evaluateMethod(method);
}

} /* namespace ucsbsusy */
