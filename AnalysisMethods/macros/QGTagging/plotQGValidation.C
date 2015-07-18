#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#endif

// root -b -q "../CMSSW_7_3_1/src/AnalysisMethods/macros/QGTagging/plotQGValidation.C+"
void plotQGValidation( const TString inputdir="trees/150717_73X_mcOnly_withBC"
                     , const TString outputdir="plots"
                     , const TString format = "png"
                     )
{
  const double  sigscale = 1;
  gSystem->mkdir(outputdir, true);

  PlotStuff* zjetPlots = new PlotStuff("plotQGValidationZjet.conf", inputdir, outputdir);
  zjetPlots->setPlotSource(PlotStuff::TREES);
  zjetPlots->setPlotType(PlotStuff::DATAMC);
  zjetPlots->setTree("Events");
  zjetPlots->setSigScale(sigscale);
  zjetPlots->setAddSigScaleTxt(false);
  zjetPlots->setFormat(format);
  zjetPlots->setWgtVar("weight");
  zjetPlots->setDataname("dyjetstoll");
  zjetPlots->setHeaderText("#sqrt{s} = 13 TeV", "X fb^{-1}", "");

  PlotStuff* dijetPlots = new PlotStuff("plotQGValidationMCdijet.conf", inputdir, outputdir);
  dijetPlots->setPlotSource(PlotStuff::TREES);
  dijetPlots->setPlotType(PlotStuff::DATAMC);
  dijetPlots->setTree("Events");
  dijetPlots->setSigScale(sigscale);
  dijetPlots->setAddSigScaleTxt(false);
  dijetPlots->setFormat(format);
  dijetPlots->setWgtVar("weight");
  dijetPlots->setDataname("qcd");
  dijetPlots->setHeaderText("#sqrt{s} = 13 TeV", "X fb^{-1}", "");

  PlotStuff* gjetPlots = new PlotStuff("plotQGValidationMCgjet.conf", inputdir, outputdir);
  gjetPlots->setPlotSource(PlotStuff::TREES);
  gjetPlots->setPlotType(PlotStuff::DATAMC);
  gjetPlots->setTree("Events");
  gjetPlots->setSigScale(sigscale);
  gjetPlots->setAddSigScaleTxt(false);
  gjetPlots->setFormat(format);
  gjetPlots->setWgtVar("weight");
  gjetPlots->setDataname("gjets");
  gjetPlots->setHeaderText("#sqrt{s} = 13 TeV", "X fb^{-1}", "");

  zjetPlots->setColor("dyjetstoll_pu"    ,12);
  zjetPlots->setColor("dyjetstoll_undef" ,11);
  zjetPlots->setColor("dyjetstoll_charm" , 3);
  zjetPlots->setColor("dyjetstoll_bot"   , 7);
  zjetPlots->setColor("dyjetstoll_gluons", 2);
  zjetPlots->setColor("dyjetstoll_quarks", 4);
  dijetPlots->setColor("dyjetstoll_pu"    ,12);
  dijetPlots->setColor("dyjetstoll_undef" ,11);
  dijetPlots->setColor("dyjetstoll_charm" , 3);
  dijetPlots->setColor("dyjetstoll_bot"   , 7);
  dijetPlots->setColor("dyjetstoll_gluons", 2);
  dijetPlots->setColor("dyjetstoll_quarks", 4);
  gjetPlots->setColor("dyjetstoll_pu"    ,12);
  gjetPlots->setColor("dyjetstoll_undef" ,11);
  gjetPlots->setColor("dyjetstoll_charm" , 3);
  gjetPlots->setColor("dyjetstoll_bot"   , 7);
  gjetPlots->setColor("dyjetstoll_gluons", 2);
  gjetPlots->setColor("dyjetstoll_quarks", 4);

  TString central = " && abs(j0eta)<2.4";
  TString forward = " && abs(j0eta)>3.0 && abs(j0eta)<5.0";
  TString pt040 = " && j0pt>040 && j0pt<060";
  TString pt060 = " && j0pt>060 && j0pt<080";
  TString pt080 = " && j0pt>080 && j0pt<120";
  TString pt120 = " && j0pt>120 && j0pt<200";
  TString pt200 = " && j0pt>200 && j0pt<600";

  zjetPlots->addTreeVar("zjets_j0pt" , "j0pt" , "passZjet", "pt_{T} [GeV]", 25,  0, 500 );
  zjetPlots->addTreeVar("zjets_j0eta", "j0eta", "passZjet", "eta"         , 25, -5,   5 );
  // add rho
  zjetPlots->addTreeVar("zjets_central040_j0mult", "j0mult", "passZjet"+central+pt040, "multiplicity", 25, 0, 50 );
  zjetPlots->addTreeVar("zjets_central060_j0mult", "j0mult", "passZjet"+central+pt060, "multiplicity", 25, 0, 50 );
  zjetPlots->addTreeVar("zjets_central080_j0mult", "j0mult", "passZjet"+central+pt080, "multiplicity", 25, 0, 50 );
  zjetPlots->addTreeVar("zjets_central120_j0mult", "j0mult", "passZjet"+central+pt120, "multiplicity", 25, 0, 50 );
  zjetPlots->addTreeVar("zjets_central200_j0mult", "j0mult", "passZjet"+central+pt200, "multiplicity", 25, 0, 50 );
  zjetPlots->addTreeVar("zjets_forward040_j0mult", "j0mult", "passZjet"+forward+pt040, "multiplicity", 25, 0, 50 );
  zjetPlots->addTreeVar("zjets_forward060_j0mult", "j0mult", "passZjet"+forward+pt060, "multiplicity", 25, 0, 50 );
  zjetPlots->addTreeVar("zjets_forward080_j0mult", "j0mult", "passZjet"+forward+pt080, "multiplicity", 25, 0, 50 );
  zjetPlots->addTreeVar("zjets_forward120_j0mult", "j0mult", "passZjet"+forward+pt120, "multiplicity", 25, 0, 50 );
  zjetPlots->addTreeVar("zjets_forward200_j0mult", "j0mult", "passZjet"+forward+pt200, "multiplicity", 25, 0, 50 );
  zjetPlots->addTreeVar("zjets_central040_j0ptd", "j0ptd", "passZjet"+central+pt040, "ptD", 25, 0, 1 );
  zjetPlots->addTreeVar("zjets_central060_j0ptd", "j0ptd", "passZjet"+central+pt060, "ptD", 25, 0, 1 );
  zjetPlots->addTreeVar("zjets_central080_j0ptd", "j0ptd", "passZjet"+central+pt080, "ptD", 25, 0, 1 );
  zjetPlots->addTreeVar("zjets_central120_j0ptd", "j0ptd", "passZjet"+central+pt120, "ptD", 25, 0, 1 );
  zjetPlots->addTreeVar("zjets_central200_j0ptd", "j0ptd", "passZjet"+central+pt200, "ptD", 25, 0, 1 );
  zjetPlots->addTreeVar("zjets_forward040_j0ptd", "j0ptd", "passZjet"+forward+pt040, "ptD", 25, 0, 1 );
  zjetPlots->addTreeVar("zjets_forward060_j0ptd", "j0ptd", "passZjet"+forward+pt060, "ptD", 25, 0, 1 );
  zjetPlots->addTreeVar("zjets_forward080_j0ptd", "j0ptd", "passZjet"+forward+pt080, "ptD", 25, 0, 1 );
  zjetPlots->addTreeVar("zjets_forward120_j0ptd", "j0ptd", "passZjet"+forward+pt120, "ptD", 25, 0, 1 );
  zjetPlots->addTreeVar("zjets_forward200_j0ptd", "j0ptd", "passZjet"+forward+pt200, "ptD", 25, 0, 1 );
  zjetPlots->addTreeVar("zjets_central040_j0axis1", "j0axis1", "passZjet"+central+pt040, "axis_{1}", 25, 0, 0.5 );
  zjetPlots->addTreeVar("zjets_central060_j0axis1", "j0axis1", "passZjet"+central+pt060, "axis_{1}", 25, 0, 0.5 );
  zjetPlots->addTreeVar("zjets_central080_j0axis1", "j0axis1", "passZjet"+central+pt080, "axis_{1}", 25, 0, 0.5 );
  zjetPlots->addTreeVar("zjets_central120_j0axis1", "j0axis1", "passZjet"+central+pt120, "axis_{1}", 25, 0, 0.5 );
  zjetPlots->addTreeVar("zjets_central200_j0axis1", "j0axis1", "passZjet"+central+pt200, "axis_{1}", 25, 0, 0.5 );
  zjetPlots->addTreeVar("zjets_forward040_j0axis1", "j0axis1", "passZjet"+forward+pt040, "axis_{1}", 25, 0, 0.5 );
  zjetPlots->addTreeVar("zjets_forward060_j0axis1", "j0axis1", "passZjet"+forward+pt060, "axis_{1}", 25, 0, 0.5 );
  zjetPlots->addTreeVar("zjets_forward080_j0axis1", "j0axis1", "passZjet"+forward+pt080, "axis_{1}", 25, 0, 0.5 );
  zjetPlots->addTreeVar("zjets_forward120_j0axis1", "j0axis1", "passZjet"+forward+pt120, "axis_{1}", 25, 0, 0.5 );
  zjetPlots->addTreeVar("zjets_forward200_j0axis1", "j0axis1", "passZjet"+forward+pt200, "axis_{1}", 25, 0, 0.5 );
  zjetPlots->addTreeVar("zjets_central040_j0axis2", "j0axis2", "passZjet"+central+pt040, "axis_{2}", 25, 0, 0.5 );
  zjetPlots->addTreeVar("zjets_central060_j0axis2", "j0axis2", "passZjet"+central+pt060, "axis_{2}", 25, 0, 0.5 );
  zjetPlots->addTreeVar("zjets_central080_j0axis2", "j0axis2", "passZjet"+central+pt080, "axis_{2}", 25, 0, 0.5 );
  zjetPlots->addTreeVar("zjets_central120_j0axis2", "j0axis2", "passZjet"+central+pt120, "axis_{2}", 25, 0, 0.5 );
  zjetPlots->addTreeVar("zjets_central200_j0axis2", "j0axis2", "passZjet"+central+pt200, "axis_{2}", 25, 0, 0.5 );
  zjetPlots->addTreeVar("zjets_forward040_j0axis2", "j0axis2", "passZjet"+forward+pt040, "axis_{2}", 25, 0, 0.5 );
  zjetPlots->addTreeVar("zjets_forward060_j0axis2", "j0axis2", "passZjet"+forward+pt060, "axis_{2}", 25, 0, 0.5 );
  zjetPlots->addTreeVar("zjets_forward080_j0axis2", "j0axis2", "passZjet"+forward+pt080, "axis_{2}", 25, 0, 0.5 );
  zjetPlots->addTreeVar("zjets_forward120_j0axis2", "j0axis2", "passZjet"+forward+pt120, "axis_{2}", 25, 0, 0.5 );
  zjetPlots->addTreeVar("zjets_forward200_j0axis2", "j0axis2", "passZjet"+forward+pt200, "axis_{2}", 25, 0, 0.5 );
  zjetPlots->addTreeVar("zjets_central040_j0qgl", "j0qgl", "passZjet"+central+pt040, "QGL", 25, 0, 1 );
  zjetPlots->addTreeVar("zjets_central060_j0qgl", "j0qgl", "passZjet"+central+pt060, "QGL", 25, 0, 1 );
  zjetPlots->addTreeVar("zjets_central080_j0qgl", "j0qgl", "passZjet"+central+pt080, "QGL", 25, 0, 1 );
  zjetPlots->addTreeVar("zjets_central120_j0qgl", "j0qgl", "passZjet"+central+pt120, "QGL", 25, 0, 1 );
  zjetPlots->addTreeVar("zjets_central200_j0qgl", "j0qgl", "passZjet"+central+pt200, "QGL", 25, 0, 1 );
  zjetPlots->addTreeVar("zjets_forward040_j0qgl", "j0qgl", "passZjet"+forward+pt040, "QGL", 25, 0, 1 );
  zjetPlots->addTreeVar("zjets_forward060_j0qgl", "j0qgl", "passZjet"+forward+pt060, "QGL", 25, 0, 1 );
  zjetPlots->addTreeVar("zjets_forward080_j0qgl", "j0qgl", "passZjet"+forward+pt080, "QGL", 25, 0, 1 );
  zjetPlots->addTreeVar("zjets_forward120_j0qgl", "j0qgl", "passZjet"+forward+pt120, "QGL", 25, 0, 1 );
  zjetPlots->addTreeVar("zjets_forward200_j0qgl", "j0qgl", "passZjet"+forward+pt200, "QGL", 25, 0, 1 );

  dijetPlots->addTreeVar("dijets_j0pt" , "j0pt" , "passDijet", "pt_{T} [GeV]", 25,  0, 500 );
  dijetPlots->addTreeVar("dijets_j0eta", "j0eta", "passDijet", "eta"         , 25, -5,   5 );
  // add rho
  dijetPlots->addTreeVar("dijets_central040_j0mult", "j0mult", "passDijet"+central+pt040, "multiplicity", 25, 0, 50 );
  dijetPlots->addTreeVar("dijets_central060_j0mult", "j0mult", "passDijet"+central+pt060, "multiplicity", 25, 0, 50 );
  dijetPlots->addTreeVar("dijets_central080_j0mult", "j0mult", "passDijet"+central+pt080, "multiplicity", 25, 0, 50 );
  dijetPlots->addTreeVar("dijets_central120_j0mult", "j0mult", "passDijet"+central+pt120, "multiplicity", 25, 0, 50 );
  dijetPlots->addTreeVar("dijets_central200_j0mult", "j0mult", "passDijet"+central+pt200, "multiplicity", 25, 0, 50 );
  dijetPlots->addTreeVar("dijets_forward040_j0mult", "j0mult", "passDijet"+forward+pt040, "multiplicity", 25, 0, 50 );
  dijetPlots->addTreeVar("dijets_forward060_j0mult", "j0mult", "passDijet"+forward+pt060, "multiplicity", 25, 0, 50 );
  dijetPlots->addTreeVar("dijets_forward080_j0mult", "j0mult", "passDijet"+forward+pt080, "multiplicity", 25, 0, 50 );
  dijetPlots->addTreeVar("dijets_forward120_j0mult", "j0mult", "passDijet"+forward+pt120, "multiplicity", 25, 0, 50 );
  dijetPlots->addTreeVar("dijets_forward200_j0mult", "j0mult", "passDijet"+forward+pt200, "multiplicity", 25, 0, 50 );
  dijetPlots->addTreeVar("dijets_central040_j0ptd", "j0ptd", "passDijet"+central+pt040, "ptD", 25, 0, 1 );
  dijetPlots->addTreeVar("dijets_central060_j0ptd", "j0ptd", "passDijet"+central+pt060, "ptD", 25, 0, 1 );
  dijetPlots->addTreeVar("dijets_central080_j0ptd", "j0ptd", "passDijet"+central+pt080, "ptD", 25, 0, 1 );
  dijetPlots->addTreeVar("dijets_central120_j0ptd", "j0ptd", "passDijet"+central+pt120, "ptD", 25, 0, 1 );
  dijetPlots->addTreeVar("dijets_central200_j0ptd", "j0ptd", "passDijet"+central+pt200, "ptD", 25, 0, 1 );
  dijetPlots->addTreeVar("dijets_forward040_j0ptd", "j0ptd", "passDijet"+forward+pt040, "ptD", 25, 0, 1 );
  dijetPlots->addTreeVar("dijets_forward060_j0ptd", "j0ptd", "passDijet"+forward+pt060, "ptD", 25, 0, 1 );
  dijetPlots->addTreeVar("dijets_forward080_j0ptd", "j0ptd", "passDijet"+forward+pt080, "ptD", 25, 0, 1 );
  dijetPlots->addTreeVar("dijets_forward120_j0ptd", "j0ptd", "passDijet"+forward+pt120, "ptD", 25, 0, 1 );
  dijetPlots->addTreeVar("dijets_forward200_j0ptd", "j0ptd", "passDijet"+forward+pt200, "ptD", 25, 0, 1 );
  dijetPlots->addTreeVar("dijets_central040_j0axis1", "j0axis1", "passDijet"+central+pt040, "axis_{1}", 25, 0, 0.5 );
  dijetPlots->addTreeVar("dijets_central060_j0axis1", "j0axis1", "passDijet"+central+pt060, "axis_{1}", 25, 0, 0.5 );
  dijetPlots->addTreeVar("dijets_central080_j0axis1", "j0axis1", "passDijet"+central+pt080, "axis_{1}", 25, 0, 0.5 );
  dijetPlots->addTreeVar("dijets_central120_j0axis1", "j0axis1", "passDijet"+central+pt120, "axis_{1}", 25, 0, 0.5 );
  dijetPlots->addTreeVar("dijets_central200_j0axis1", "j0axis1", "passDijet"+central+pt200, "axis_{1}", 25, 0, 0.5 );
  dijetPlots->addTreeVar("dijets_forward040_j0axis1", "j0axis1", "passDijet"+forward+pt040, "axis_{1}", 25, 0, 0.5 );
  dijetPlots->addTreeVar("dijets_forward060_j0axis1", "j0axis1", "passDijet"+forward+pt060, "axis_{1}", 25, 0, 0.5 );
  dijetPlots->addTreeVar("dijets_forward080_j0axis1", "j0axis1", "passDijet"+forward+pt080, "axis_{1}", 25, 0, 0.5 );
  dijetPlots->addTreeVar("dijets_forward120_j0axis1", "j0axis1", "passDijet"+forward+pt120, "axis_{1}", 25, 0, 0.5 );
  dijetPlots->addTreeVar("dijets_forward200_j0axis1", "j0axis1", "passDijet"+forward+pt200, "axis_{1}", 25, 0, 0.5 );
  dijetPlots->addTreeVar("dijets_central040_j0axis2", "j0axis2", "passDijet"+central+pt040, "axis_{2}", 25, 0, 0.5 );
  dijetPlots->addTreeVar("dijets_central060_j0axis2", "j0axis2", "passDijet"+central+pt060, "axis_{2}", 25, 0, 0.5 );
  dijetPlots->addTreeVar("dijets_central080_j0axis2", "j0axis2", "passDijet"+central+pt080, "axis_{2}", 25, 0, 0.5 );
  dijetPlots->addTreeVar("dijets_central120_j0axis2", "j0axis2", "passDijet"+central+pt120, "axis_{2}", 25, 0, 0.5 );
  dijetPlots->addTreeVar("dijets_central200_j0axis2", "j0axis2", "passDijet"+central+pt200, "axis_{2}", 25, 0, 0.5 );
  dijetPlots->addTreeVar("dijets_forward040_j0axis2", "j0axis2", "passDijet"+forward+pt040, "axis_{2}", 25, 0, 0.5 );
  dijetPlots->addTreeVar("dijets_forward060_j0axis2", "j0axis2", "passDijet"+forward+pt060, "axis_{2}", 25, 0, 0.5 );
  dijetPlots->addTreeVar("dijets_forward080_j0axis2", "j0axis2", "passDijet"+forward+pt080, "axis_{2}", 25, 0, 0.5 );
  dijetPlots->addTreeVar("dijets_forward120_j0axis2", "j0axis2", "passDijet"+forward+pt120, "axis_{2}", 25, 0, 0.5 );
  dijetPlots->addTreeVar("dijets_forward200_j0axis2", "j0axis2", "passDijet"+forward+pt200, "axis_{2}", 25, 0, 0.5 );
  dijetPlots->addTreeVar("dijets_central040_j0qgl", "j0qgl", "passDijet"+central+pt040, "QGL", 25, 0, 1 );
  dijetPlots->addTreeVar("dijets_central060_j0qgl", "j0qgl", "passDijet"+central+pt060, "QGL", 25, 0, 1 );
  dijetPlots->addTreeVar("dijets_central080_j0qgl", "j0qgl", "passDijet"+central+pt080, "QGL", 25, 0, 1 );
  dijetPlots->addTreeVar("dijets_central120_j0qgl", "j0qgl", "passDijet"+central+pt120, "QGL", 25, 0, 1 );
  dijetPlots->addTreeVar("dijets_central200_j0qgl", "j0qgl", "passDijet"+central+pt200, "QGL", 25, 0, 1 );
  dijetPlots->addTreeVar("dijets_forward040_j0qgl", "j0qgl", "passDijet"+forward+pt040, "QGL", 25, 0, 1 );
  dijetPlots->addTreeVar("dijets_forward060_j0qgl", "j0qgl", "passDijet"+forward+pt060, "QGL", 25, 0, 1 );
  dijetPlots->addTreeVar("dijets_forward080_j0qgl", "j0qgl", "passDijet"+forward+pt080, "QGL", 25, 0, 1 );
  dijetPlots->addTreeVar("dijets_forward120_j0qgl", "j0qgl", "passDijet"+forward+pt120, "QGL", 25, 0, 1 );
  dijetPlots->addTreeVar("dijets_forward200_j0qgl", "j0qgl", "passDijet"+forward+pt200, "QGL", 25, 0, 1 );

  gjetPlots->addTreeVar("gjets_j0pt" , "j0pt" , "passDijet", "pt_{T} [GeV]", 25,  0, 500 );
  gjetPlots->addTreeVar("gjets_j0eta", "j0eta", "passDijet", "eta"         , 25, -5,   5 );
  // add rho
  gjetPlots->addTreeVar("gjets_central040_j0mult", "j0mult", "passGmjet"+central+pt040, "multiplicity", 25, 0, 50 );
  gjetPlots->addTreeVar("gjets_central060_j0mult", "j0mult", "passGmjet"+central+pt060, "multiplicity", 25, 0, 50 );
  gjetPlots->addTreeVar("gjets_central080_j0mult", "j0mult", "passGmjet"+central+pt080, "multiplicity", 25, 0, 50 );
  gjetPlots->addTreeVar("gjets_central120_j0mult", "j0mult", "passGmjet"+central+pt120, "multiplicity", 25, 0, 50 );
  gjetPlots->addTreeVar("gjets_central200_j0mult", "j0mult", "passGmjet"+central+pt200, "multiplicity", 25, 0, 50 );
  gjetPlots->addTreeVar("gjets_forward040_j0mult", "j0mult", "passGmjet"+forward+pt040, "multiplicity", 25, 0, 50 );
  gjetPlots->addTreeVar("gjets_forward060_j0mult", "j0mult", "passGmjet"+forward+pt060, "multiplicity", 25, 0, 50 );
  gjetPlots->addTreeVar("gjets_forward080_j0mult", "j0mult", "passGmjet"+forward+pt080, "multiplicity", 25, 0, 50 );
  gjetPlots->addTreeVar("gjets_forward120_j0mult", "j0mult", "passGmjet"+forward+pt120, "multiplicity", 25, 0, 50 );
  gjetPlots->addTreeVar("gjets_forward200_j0mult", "j0mult", "passGmjet"+forward+pt200, "multiplicity", 25, 0, 50 );
  gjetPlots->addTreeVar("gjets_central040_j0ptd", "j0ptd", "passGmjet"+central+pt040, "ptD", 25, 0, 1 );
  gjetPlots->addTreeVar("gjets_central060_j0ptd", "j0ptd", "passGmjet"+central+pt060, "ptD", 25, 0, 1 );
  gjetPlots->addTreeVar("gjets_central080_j0ptd", "j0ptd", "passGmjet"+central+pt080, "ptD", 25, 0, 1 );
  gjetPlots->addTreeVar("gjets_central120_j0ptd", "j0ptd", "passGmjet"+central+pt120, "ptD", 25, 0, 1 );
  gjetPlots->addTreeVar("gjets_central200_j0ptd", "j0ptd", "passGmjet"+central+pt200, "ptD", 25, 0, 1 );
  gjetPlots->addTreeVar("gjets_forward040_j0ptd", "j0ptd", "passGmjet"+forward+pt040, "ptD", 25, 0, 1 );
  gjetPlots->addTreeVar("gjets_forward060_j0ptd", "j0ptd", "passGmjet"+forward+pt060, "ptD", 25, 0, 1 );
  gjetPlots->addTreeVar("gjets_forward080_j0ptd", "j0ptd", "passGmjet"+forward+pt080, "ptD", 25, 0, 1 );
  gjetPlots->addTreeVar("gjets_forward120_j0ptd", "j0ptd", "passGmjet"+forward+pt120, "ptD", 25, 0, 1 );
  gjetPlots->addTreeVar("gjets_forward200_j0ptd", "j0ptd", "passGmjet"+forward+pt200, "ptD", 25, 0, 1 );
  gjetPlots->addTreeVar("gjets_central040_j0axis1", "j0axis1", "passGmjet"+central+pt040, "axis_{1}", 25, 0, 0.5 );
  gjetPlots->addTreeVar("gjets_central060_j0axis1", "j0axis1", "passGmjet"+central+pt060, "axis_{1}", 25, 0, 0.5 );
  gjetPlots->addTreeVar("gjets_central080_j0axis1", "j0axis1", "passGmjet"+central+pt080, "axis_{1}", 25, 0, 0.5 );
  gjetPlots->addTreeVar("gjets_central120_j0axis1", "j0axis1", "passGmjet"+central+pt120, "axis_{1}", 25, 0, 0.5 );
  gjetPlots->addTreeVar("gjets_central200_j0axis1", "j0axis1", "passGmjet"+central+pt200, "axis_{1}", 25, 0, 0.5 );
  gjetPlots->addTreeVar("gjets_forward040_j0axis1", "j0axis1", "passGmjet"+forward+pt040, "axis_{1}", 25, 0, 0.5 );
  gjetPlots->addTreeVar("gjets_forward060_j0axis1", "j0axis1", "passGmjet"+forward+pt060, "axis_{1}", 25, 0, 0.5 );
  gjetPlots->addTreeVar("gjets_forward080_j0axis1", "j0axis1", "passGmjet"+forward+pt080, "axis_{1}", 25, 0, 0.5 );
  gjetPlots->addTreeVar("gjets_forward120_j0axis1", "j0axis1", "passGmjet"+forward+pt120, "axis_{1}", 25, 0, 0.5 );
  gjetPlots->addTreeVar("gjets_forward200_j0axis1", "j0axis1", "passGmjet"+forward+pt200, "axis_{1}", 25, 0, 0.5 );
  gjetPlots->addTreeVar("gjets_central040_j0axis2", "j0axis2", "passGmjet"+central+pt040, "axis_{2}", 25, 0, 0.5 );
  gjetPlots->addTreeVar("gjets_central060_j0axis2", "j0axis2", "passGmjet"+central+pt060, "axis_{2}", 25, 0, 0.5 );
  gjetPlots->addTreeVar("gjets_central080_j0axis2", "j0axis2", "passGmjet"+central+pt080, "axis_{2}", 25, 0, 0.5 );
  gjetPlots->addTreeVar("gjets_central120_j0axis2", "j0axis2", "passGmjet"+central+pt120, "axis_{2}", 25, 0, 0.5 );
  gjetPlots->addTreeVar("gjets_central200_j0axis2", "j0axis2", "passGmjet"+central+pt200, "axis_{2}", 25, 0, 0.5 );
  gjetPlots->addTreeVar("gjets_forward040_j0axis2", "j0axis2", "passGmjet"+forward+pt040, "axis_{2}", 25, 0, 0.5 );
  gjetPlots->addTreeVar("gjets_forward060_j0axis2", "j0axis2", "passGmjet"+forward+pt060, "axis_{2}", 25, 0, 0.5 );
  gjetPlots->addTreeVar("gjets_forward080_j0axis2", "j0axis2", "passGmjet"+forward+pt080, "axis_{2}", 25, 0, 0.5 );
  gjetPlots->addTreeVar("gjets_forward120_j0axis2", "j0axis2", "passGmjet"+forward+pt120, "axis_{2}", 25, 0, 0.5 );
  gjetPlots->addTreeVar("gjets_forward200_j0axis2", "j0axis2", "passGmjet"+forward+pt200, "axis_{2}", 25, 0, 0.5 );
  gjetPlots->addTreeVar("gjets_central040_j0qgl", "j0qgl", "passGmjet"+central+pt040, "QGL", 25, 0, 1 );
  gjetPlots->addTreeVar("gjets_central060_j0qgl", "j0qgl", "passGmjet"+central+pt060, "QGL", 25, 0, 1 );
  gjetPlots->addTreeVar("gjets_central080_j0qgl", "j0qgl", "passGmjet"+central+pt080, "QGL", 25, 0, 1 );
  gjetPlots->addTreeVar("gjets_central120_j0qgl", "j0qgl", "passGmjet"+central+pt120, "QGL", 25, 0, 1 );
  gjetPlots->addTreeVar("gjets_central200_j0qgl", "j0qgl", "passGmjet"+central+pt200, "QGL", 25, 0, 1 );
  gjetPlots->addTreeVar("gjets_forward040_j0qgl", "j0qgl", "passGmjet"+forward+pt040, "QGL", 25, 0, 1 );
  gjetPlots->addTreeVar("gjets_forward060_j0qgl", "j0qgl", "passGmjet"+forward+pt060, "QGL", 25, 0, 1 );
  gjetPlots->addTreeVar("gjets_forward080_j0qgl", "j0qgl", "passGmjet"+forward+pt080, "QGL", 25, 0, 1 );
  gjetPlots->addTreeVar("gjets_forward120_j0qgl", "j0qgl", "passGmjet"+forward+pt120, "QGL", 25, 0, 1 );
  gjetPlots->addTreeVar("gjets_forward200_j0qgl", "j0qgl", "passGmjet"+forward+pt200, "QGL", 25, 0, 1 );

  zjetPlots ->plot();
  dijetPlots->plot();
  gjetPlots ->plot();

  delete zjetPlots;
  delete dijetPlots;
  delete gjetPlots;

}
