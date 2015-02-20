#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#endif


void makeYieldTables(const TString conffile="run1leptrees.conf",
                     const TString inputdir="run/trees",
                     const TString outputdir=".")
{

  PlotStuff* tablemaker = new PlotStuff(conffile, inputdir, outputdir);

  tablemaker->setPlotSource(PlotStuff::TREES);
  tablemaker->setWgtVar("ScaleFactor");

  tablemaker->addSelection("baseline","1==1");
  tablemaker->addSelection("$\\ETslash > 550~\\GeV, 4 \\leq N_\\mathrm{j} < 7, N_\\mathrm{b} = 1$","MET>550 && NJets>=4 && NJets < 7 && NBJets==1");
  tablemaker->addSelection("$\\ETslash > 550~\\GeV, N_\\mathrm{j} \\geq 7, N_\\mathrm{b} \\geq 3$","MET>550 && NJets>=7 && NBJets>=3");

  tablemaker->tabulate();

}
