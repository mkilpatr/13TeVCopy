
#ifndef ANALYSISBASE_TREEANALYZER_MVAWRAPPER_H
#define ANALYSISBASE_TREEANALYZER_MVAWRAPPER_H

#include <string>
#include <vector>

class Panvariate;
template<typename type>
class Paramatrix;
typedef Paramatrix<Panvariate> ParamatrixMVA;

namespace ucsbsusy {
  class MVAWrapper {
  public:
    MVAWrapper(std::string filename, std::string mvaName);
    int findAxis(std::string axisName) const;
    void setParameters(std::vector<double> parameters);
    int findVariable(std::string varName) const;
    void setVariable(int index, double value) const;
    double evaluate(int method = 0) const;

    const ParamatrixMVA* para;
    const Panvariate * mva;
  };

}

#endif
