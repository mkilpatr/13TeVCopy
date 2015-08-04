#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#endif

//root -b -q "../CMSSW_7_4_7/src/AnalysisMethods/macros/QGTagging/plotQGValidationComposition.C+"
void plotQGValidationComposition( const TString inputdir="trees/150731_74X_purwt"
                                , const TString outputdir="plots"
                                , const TString format = "png"
                                )
{
  gSystem->mkdir(outputdir, true);
  TString wtVar = "purwt"; // weight purwt

  PlotStuff* zjetPlots = new PlotStuff("plotQGValidationMCzjet74X.conf", inputdir, outputdir);
  zjetPlots->setPlotSource(PlotStuff::TREES);
  zjetPlots->setPlotType(PlotStuff::NORMCOMP);
  zjetPlots->setTree("Events");
  zjetPlots->setFormat(format);
  zjetPlots->setWgtVar(wtVar);
  zjetPlots->setDataIsMC();
  zjetPlots->setDataName("dyjetstoll");
  zjetPlots->setYTitle("Fraction of Jets");
  zjetPlots->setHeaderText("CMS Sim", "#sqrt{s} = 13 TeV", "");

  PlotStuff* dijetPlots = new PlotStuff("plotQGValidationMCdijet.conf", inputdir, outputdir);
  dijetPlots->setPlotSource(PlotStuff::TREES);
  dijetPlots->setPlotType(PlotStuff::NORMCOMP);
  dijetPlots->setTree("Events");
  dijetPlots->setFormat(format);
  dijetPlots->setWgtVar(wtVar);
  dijetPlots->setDataIsMC();
  dijetPlots->setDataName("qcd");
  dijetPlots->setYTitle("Fraction of Jets");
  dijetPlots->setHeaderText("CMS Sim", "#sqrt{s} = 13 TeV", "");

  PlotStuff* gjetPlots = new PlotStuff("plotQGValidationMCgjet.conf", inputdir, outputdir);
  gjetPlots->setPlotSource(PlotStuff::TREES);
  gjetPlots->setPlotType(PlotStuff::NORMCOMP);
  gjetPlots->setTree("Events");
  gjetPlots->setFormat(format);
  gjetPlots->setWgtVar(wtVar);
  gjetPlots->setDataIsMC();
  gjetPlots->setDataName("gjets");
  gjetPlots->setYTitle("Fraction of Jets");
  gjetPlots->setHeaderText("CMS Sim", "#sqrt{s} = 13 TeV", "");


  TString centralSel = " && j0pt>20 && j0eta<2.4";
  TString forwardSel = " && j0pt>20 && j0eta>3.0";
  TString centralSel30 = " && j0pt>30 && j0eta<2.4";
  TString forwardSel30 = " && j0pt>30 && j0eta>3.0";

  zjetPlots   ->addTreeVar(  "zjets_central" , "j0pt", "passZjet && passZmass" +centralSel30, "pt_{T} [GeV]", 20, 0, 600 );
  zjetPlots   ->addTreeVar(  "zjets_forward" , "j0pt", "passZjet && passZmass" +forwardSel30, "pt_{T} [GeV]", 12, 0, 600 );
  dijetPlots  ->addTreeVar(  "dijets_central", "j0pt", "passDijet && passDijet3"+centralSel, "pt_{T} [GeV]", 25, 0, 600 );
  dijetPlots  ->addTreeVar(  "dijets_forward", "j0pt", "passDijet && passDijet3"+forwardSel, "pt_{T} [GeV]", 25, 0, 600 );
  gjetPlots   ->addTreeVar(  "gjets_central" , "j0pt", "passGmjet"+centralSel, "pt_{T} [GeV]", 25, 0, 600 );
  gjetPlots   ->addTreeVar(  "gjets_forward" , "j0pt", "passGmjet"+forwardSel, "pt_{T} [GeV]", 12, 0, 600 );

  zjetPlots   ->plot();
  dijetPlots  ->plot();
  gjetPlots   ->plot();

  delete zjetPlots;
  delete dijetPlots;
  delete gjetPlots;

}
