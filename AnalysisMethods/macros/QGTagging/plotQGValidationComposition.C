#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#endif

//root -b -q "../CMSSW_7_4_7/src/AnalysisMethods/macros/QGTagging/plotQGValidationComposition.C+"
void plotQGValidationComposition( const TString inputdir="trees/150906_withHTprescales"
                                , const TString outputdir="plots"
                                , const TString format = "png"
                                )
{
  gSystem->mkdir(outputdir, true);
  TString wtVar = "weight*puWeightC"; // weight purwt

  PlotStuff* zjetPlots = new PlotStuff("plotQGValidationMCzjet.conf", inputdir, outputdir);
  zjetPlots->setPlotSource(PlotStuff::TREES);
  zjetPlots->setPlotType(PlotStuff::NORMCOMP);
  zjetPlots->setTree("Events");
  zjetPlots->setFormat(format);
  zjetPlots->setWgtVar(wtVar);
  //zjetPlots->setWgtVar("weight*puWeight");
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

  zjetPlots->setColor("comp2", 4); // quarks
  zjetPlots->setColor("comp3", 2); // gluons
  zjetPlots->setColor("comp4", 7); // bottom
  zjetPlots->setColor("comp5", 3); // charm
  zjetPlots->setColor("comp6",11); // undef
  zjetPlots->setColor("comp7",12); // pile-up
  dijetPlots->setColor("comp2", 4); // quarks
  dijetPlots->setColor("comp3", 2); // gluons
  dijetPlots->setColor("comp4", 7); // bottom
  dijetPlots->setColor("comp5", 3); // charm
  dijetPlots->setColor("comp6",11); // undef
  dijetPlots->setColor("comp7",12); // pile-up
  gjetPlots->setColor("comp2", 4); // quarks
  gjetPlots->setColor("comp3", 2); // gluons
  gjetPlots->setColor("comp4", 7); // bottom
  gjetPlots->setColor("comp5", 3); // charm
  gjetPlots->setColor("comp6",11); // undef
  gjetPlots->setColor("comp7",12); // pile-up


  TString centralSel = " && j0pt>20 && j0eta<2.4";
  TString forwardSel = " && j0pt>20 && j0eta>3.0";
  TString centralSel30 = " && j0pt>30 && j0eta<2.4";
  TString forwardSel30 = " && j0pt>30 && j0eta>3.0";

  zjetPlots   ->addTreeVar(  "zjets_central" , "j0pt", "passZjet && passZmass" +centralSel30, "pt_{T} [GeV]", 60, 0, 600 ); // 17
  zjetPlots   ->addTreeVar(  "zjets_forward" , "j0pt", "passZjet && passZmass" +forwardSel30, "pt_{T} [GeV]", 12, 0, 600 );
  dijetPlots  ->addTreeVar(  "dijets_central", "j0pt", "passDijet && passDijet3 && ht>450"+centralSel, "pt_{T} [GeV]", 25, 0, 600 );
  dijetPlots  ->addTreeVar(  "dijets_forward", "j0pt", "passDijet && passDijet3 && ht>450"+forwardSel, "pt_{T} [GeV]", 25, 0, 600 );
  gjetPlots   ->addTreeVar(  "gjets_central" , "j0pt", "passGmjet"+centralSel, "pt_{T} [GeV]", 25, 0, 600 );
  gjetPlots   ->addTreeVar(  "gjets_forward" , "j0pt", "passGmjet"+forwardSel, "pt_{T} [GeV]", 12, 0, 600 );

  zjetPlots   ->plot();
  dijetPlots  ->plot();
  gjetPlots   ->plot();

  delete zjetPlots;
  delete dijetPlots;
  delete gjetPlots;

}
