#include <fstream>
#include "CommonParameters.hh"

using namespace EstTools;


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void QCDPred(){
  auto config = qcdConfig();

  QCDEstimator z(config.outputdir);
  z.setConfig(config);

  z.pred();
  z.naiveTF();
  z.printYields();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void plotQCDCR(){
  auto config = qcdConfig();

  BaseEstimator z(config.outputdir);
  z.setConfig(config);

  z.plotDataMC({"ttbar-cr", "wjets-cr", "tW-cr", "ttW-cr", "znunu-cr", "qcd-cr-withveto"}, "data-cr", false);

}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


void Yields(){

  auto config = qcdConfig();

  config.samples.clear();

  config.addSample("data-cr",     "Data",          "sr/met",             "1.0",       datasel + trigSR + vetoes + dphi_invert);
  config.addSample("data-cr-old",     "Data",          "sr/met-old-trig",             "1.0",       datasel + trigSR + vetoes + dphi_invert);

  BaseEstimator z(config.outputdir);
  z.setConfig(config);

  z.calcYields();
  z.printYields();

}

