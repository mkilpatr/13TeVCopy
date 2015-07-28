#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#endif

// root -b -q "../CMSSW_7_3_1/src/AnalysisMethods/macros/QGTagging/plotQGValidationComposition.C+"
void plotQGValidationComposition( const TString inputdir="trees/150727_fixedCleaning_dilepmass"
                                , const TString outputdir="plots"
                                , const TString format = "png"
                                )
{
  const double  sigscale = 1;
  gSystem->mkdir(outputdir, true);

  PlotStuff* zjetPlots = new PlotStuff("plotQGValidationMCzjet.conf", inputdir, outputdir);
  zjetPlots->setPlotSource(PlotStuff::TREES);
  zjetPlots->setPlotType(PlotStuff::NORMCOMP);
  zjetPlots->setTree("Events");
  zjetPlots->setSigScale(sigscale);
  zjetPlots->setAddSigScaleTxt(false);
  zjetPlots->setFormat(format);
  zjetPlots->setWgtVar("weight");
  zjetPlots->setDataname("dyjetstoll");
  zjetPlots->setYTitle("Fraction of Jets");
  zjetPlots->setHeaderText("CMS Sim", "#sqrt{s} = 13 TeV", "");


  PlotStuff* dijetPlots = new PlotStuff("plotQGValidationMCdijet.conf", inputdir, outputdir);
  dijetPlots->setPlotSource(PlotStuff::TREES);
  dijetPlots->setPlotType(PlotStuff::NORMCOMP);
  dijetPlots->setTree("Events");
  dijetPlots->setSigScale(sigscale);
  dijetPlots->setAddSigScaleTxt(false);
  dijetPlots->setFormat(format);
  dijetPlots->setWgtVar("weight");
  dijetPlots->setDataname("qcd");
  dijetPlots->setYTitle("Fraction of Jets");
  dijetPlots->setHeaderText("CMS Sim", "#sqrt{s} = 13 TeV", "");


  PlotStuff* dijetHTPlots = new PlotStuff("plotQGValidationMCdijetHT.conf", inputdir, outputdir);
  dijetHTPlots->setPlotSource(PlotStuff::TREES);
  dijetHTPlots->setPlotType(PlotStuff::NORMCOMP);
  dijetHTPlots->setTree("Events");
  dijetHTPlots->setSigScale(sigscale);
  dijetHTPlots->setAddSigScaleTxt(false);
  dijetHTPlots->setFormat(format);
  dijetHTPlots->setWgtVar("weight");
  dijetHTPlots->setDataname("qcdHT");
  dijetHTPlots->setYTitle("Fraction of Jets");
  dijetHTPlots->setHeaderText("CMS Sim", "#sqrt{s} = 13 TeV", "");


  PlotStuff* gjetPlots = new PlotStuff("plotQGValidationMCgjet.conf", inputdir, outputdir);
  gjetPlots->setPlotSource(PlotStuff::TREES);
  gjetPlots->setPlotType(PlotStuff::NORMCOMP);
  gjetPlots->setTree("Events");
  gjetPlots->setSigScale(sigscale);
  gjetPlots->setAddSigScaleTxt(false);
  gjetPlots->setFormat(format);
  gjetPlots->setWgtVar("weight");
  gjetPlots->setDataname("gjets");
  gjetPlots->setYTitle("Fraction of Jets");
  gjetPlots->setHeaderText("CMS Sim", "#sqrt{s} = 13 TeV", "");

  TString centralSel = " && j0pt>20 && j0eta<2.4";
  TString forwardSel = " && j0pt>20 && j0eta>3.0";

  zjetPlots   ->addTreeVar(  "zjets_central" , "j0pt", "passZjet && passZmass" +centralSel, "pt_{T} [GeV]", 25, 0, 600 );
  zjetPlots   ->addTreeVar(  "zjets_forward" , "j0pt", "passZjet && passZmass" +forwardSel, "pt_{T} [GeV]", 25, 0, 600 );
  dijetPlots  ->addTreeVar(  "dijets_central", "j0pt", "passDijet"+centralSel, "pt_{T} [GeV]", 25, 0, 600 );
  dijetPlots  ->addTreeVar(  "dijets_forward", "j0pt", "passDijet"+forwardSel, "pt_{T} [GeV]", 25, 0, 600 );
  dijetHTPlots->addTreeVar("dijetsHT_central", "j0pt", "passDijet"+centralSel, "pt_{T} [GeV]", 25, 0, 600 );
  dijetHTPlots->addTreeVar("dijetsHT_forward", "j0pt", "passDijet"+forwardSel, "pt_{T} [GeV]", 25, 0, 600 );
  gjetPlots   ->addTreeVar(  "gjets_central" , "j0pt", "passGmjet"+centralSel, "pt_{T} [GeV]", 25, 0, 600 );
  gjetPlots   ->addTreeVar(  "gjets_forward" , "j0pt", "passGmjet"+forwardSel, "pt_{T} [GeV]", 25, 0, 600 );

  zjetPlots   ->plot();
  dijetPlots  ->plot();
  dijetHTPlots->plot();
  gjetPlots   ->plot();

  delete zjetPlots;
  delete dijetPlots;
  delete dijetHTPlots;
  delete gjetPlots;

}
