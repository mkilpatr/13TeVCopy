#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#endif

// Parses a configuration file with samples and settings for plotting (colors, labels), and processes the corresponding files with histograms to make pretty plots
void plotAll(const TString conf = "run1lep.conf", const TString inputdir = "run/plots", const TString outputdir = "run/plots")
{

  PlotStuff* plots = new PlotStuff(conf, inputdir, outputdir);
  plots->setPlotSource(PlotStuff::HISTS);
  plots->setPlotType(PlotStuff::DATAMC);

  plots->plot();

  delete plots;

}
