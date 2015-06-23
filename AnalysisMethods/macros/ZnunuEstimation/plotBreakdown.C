#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#endif


void plotBreakdown(const TString conffile="runPhotonZ.conf",
    const TString inputdir="../run/plots/",
    const TString outputdir="../run/plots/")
{

  TString all          = "genBosonEta<2.4 && genBosonEta>-2.4 && passLoosePhotons==1";
  TString preselection = "NVetoLeps==0 && NVetoTaus==0 && NJets>=5 && NBJets>=1 && MET>=200 && genBosonEta<2.4 && genBosonEta>-2.4";

  {
    PlotStuff* myPlots = new PlotStuff(conffile, inputdir, outputdir);
    myPlots->setPlotSource(PlotStuff::TREES);
    myPlots->setPlotType(PlotStuff::NORMCOMP);
    myPlots->setFormat("pdf");
    myPlots->setWgtVar("ScaleFactor");
    myPlots->setDrawOption("histE");

    myPlots->addTreeVar("genBosonPT_0", "genBosonPT", "fabs(genBosonEta)<2.4 && 1==1", "boson PT [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("genBosonPT_1", "genBosonPT", "fabs(genBosonEta)<2.4 && NVetoLeps==0", "boson PT [GeV]", 18, 100, 1000);//
    myPlots->addTreeVar("genBosonPT_2", "genBosonPT", "fabs(genBosonEta)<2.4 && NVetoLeps==0 && NVetoTaus==0", "boson PT [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("genBosonPT_3", "genBosonPT", "fabs(genBosonEta)<2.4 && NVetoLeps==0 && NVetoTaus==0 && NJets>=5", "boson PT [GeV]", 18, 100, 1000); //
    myPlots->addTreeVar("genBosonPT_4", "genBosonPT", "fabs(genBosonEta)<2.4 && NVetoLeps==0 && NVetoTaus==0 && NJets>=5 && NBJets>=1", "boson PT [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("genBosonPT_5", "genBosonPT", "fabs(genBosonEta)<2.4 && NVetoLeps==0 && NVetoTaus==0 && NJets>=5 && NBJets>=1 && MET>=200", "boson PT [GeV]", 18, 100, 1000);////

    myPlots->addTreeVar("genBosonPT_LepVeto", "genBosonPT", "fabs(genBosonEta)<2.4 && NVetoLeps==0", "boson PT [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("genBosonPT_TauVeto", "genBosonPT", "fabs(genBosonEta)<2.4 && NVetoTaus==0", "boson PT [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("genBosonPT_NJ5", "genBosonPT", "fabs(genBosonEta)<2.4 && NJets>=5", "boson PT [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("genBosonPT_NB1", "genBosonPT", "fabs(genBosonEta)<2.4 && NBJets>=1", "boson PT [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("genBosonPT_MET200", "genBosonPT", "fabs(genBosonEta)<2.4 && MET>=200", "boson PT [GeV]", 18, 100, 1000);


    myPlots->addTreeVar("MET_0", "MET", "1==1", "MET [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("MET_1", "MET", "NVetoLeps==0", "MET [GeV]", 18, 100, 1000);//
    myPlots->addTreeVar("MET_2", "MET", "NVetoLeps==0 && NVetoTaus==0", "MET [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("MET_3", "MET", "NVetoLeps==0 && NVetoTaus==0 && NJets>=5", "MET [GeV]", 18, 100, 1000); //
    myPlots->addTreeVar("MET_4", "MET", "NVetoLeps==0 && NVetoTaus==0 && NJets>=5 && NBJets>=1", "MET [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("MET_5", "MET", "NVetoLeps==0 && NVetoTaus==0 && NJets>=5 && NBJets>=1 && MET>=200", "MET [GeV]", 18, 100, 1000);////

    myPlots->addTreeVar("MET_LepVeto", "MET", "NVetoLeps==0", "MET [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("MET_TauVeto", "MET", "NVetoTaus==0", "MET [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("MET_NJ5", "MET", "NJets>=5", "MET [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("MET_NB1", "MET", "NBJets>=1", "MET [GeV]", 18, 100, 1000);
    myPlots->addTreeVar("MET_MET200", "MET", "MET>=200", "MET [GeV]", 18, 100, 1000);


    myPlots->plot();
    delete myPlots;
  }




}
