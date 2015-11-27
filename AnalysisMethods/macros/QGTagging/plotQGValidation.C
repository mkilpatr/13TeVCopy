#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#endif

//root -b -q "../CMSSW_7_4_11/src/AnalysisMethods/macros/QGTagging/plotQGValidation.C+"
void plotQGValidation( const TString inputdir="trees/151118_gluonRwt_central301"
                     , const TString outputdir="plots"
                     , const TString format = "png"
                     )
{
  gSystem->mkdir(outputdir, true);
  TString lumi    = "0.578";
  TString lumiStr = "578 pb^{-1}";
  TString wtVar   = "*weight*puWeight";
  //TString wtVar   = "*weight*puWeight*gMultWt";
  bool scaleToData = false;
  float headerx = 0.5;
  float headery = 0.92;

  PlotStuff* zjetPlots = new PlotStuff("plotQGValidationZjet.conf", inputdir, outputdir, 0, true);
  zjetPlots->setPlotSource(PlotStuff::TREES);
  zjetPlots->setPlotType(PlotStuff::DATAMC);
  zjetPlots->setTree("Events");
  zjetPlots->setFormat(format);
  zjetPlots->setWgtVar(lumi+wtVar);
  zjetPlots->setDataName("data_doublemu");
  //zjetPlots->setDataName("dyjetstoll"); zjetPlots->setDataIsMC();
  zjetPlots->setHeaderText("#sqrt{s} = 13 TeV", lumiStr, "");
  zjetPlots->setHeaderPosition(headerx,headery);
  zjetPlots->setScaleToData(scaleToData);
  zjetPlots->setPlotOverflow(true);
  zjetPlots->setRatioPlot(true);

  PlotStuff* dijetPlots = new PlotStuff("plotQGValidationDijet.conf", inputdir, outputdir);
  dijetPlots->setPlotSource(PlotStuff::TREES);
  dijetPlots->setPlotType(PlotStuff::DATAMC);
  dijetPlots->setTree("Events");
  dijetPlots->setFormat(format);
  dijetPlots->setWgtVar(lumi+wtVar);
  dijetPlots->setDataName("data_jetht");
  dijetPlots->setHeaderText("#sqrt{s} = 13 TeV", lumiStr, "");
  dijetPlots->setHeaderPosition(headerx,headery);
  dijetPlots->setScaleToData(scaleToData);
  dijetPlots->setPlotOverflow(true);
  dijetPlots->setRatioPlot(true);
  dijetPlots->setUnprescale(true);
  dijetPlots->setUnprescaleVar("prescale");
  //dijetPlots->setLogy();
  //dijetPlots->setMinLogScale(100);

  PlotStuff* gjetPlots = new PlotStuff("plotQGValidationGjet.conf", inputdir, outputdir);
  gjetPlots->setPlotSource(PlotStuff::TREES);
  gjetPlots->setPlotType(PlotStuff::DATAMC);
  gjetPlots->setTree("Events");
  gjetPlots->setFormat(format);
  gjetPlots->setWgtVar(lumi+wtVar);
  gjetPlots->setDataName("data_singlegm");
  //gjetPlots->setDataName("gjets"); gjetPlots->setDataIsMC();
  gjetPlots->setHeaderText("#sqrt{s} = 13 TeV", lumiStr, "");
  gjetPlots->setHeaderPosition(headerx,headery);
  gjetPlots->setScaleToData(scaleToData);
  gjetPlots->setPlotOverflow(true);
  gjetPlots->setRatioPlot(true);

  zjetPlots->setColor("dyjetstoll_pu"    ,12);
  zjetPlots->setColor("dyjetstoll_undef" ,11);
  zjetPlots->setColor("dyjetstoll_charm" , 3);
  zjetPlots->setColor("dyjetstoll_bot"   , 7);
  zjetPlots->setColor("dyjetstoll_gluons", 2);
  zjetPlots->setColor("dyjetstoll_quarks", 4);
  dijetPlots->setColor("qcd_pu"    ,12);
  dijetPlots->setColor("qcd_undef" ,11);
  dijetPlots->setColor("qcd_charm" , 3);
  dijetPlots->setColor("qcd_bot"   , 7);
  dijetPlots->setColor("qcd_gluons", 2);
  dijetPlots->setColor("qcd_quarks", 4);
  gjetPlots->setColor("gjets_pu"    ,12);
  gjetPlots->setColor("gjets_undef" ,11);
  gjetPlots->setColor("gjets_charm" , 3);
  gjetPlots->setColor("gjets_bot"   , 7);
  gjetPlots->setColor("gjets_gluons", 2);
  gjetPlots->setColor("gjets_quarks", 4);

  TString dijetselection = "passDijet && passDijet3 && ht>450";
  TString central = " && abs(j0eta)<2.4";
  TString forward = " && abs(j0eta)>2.4 && abs(j0eta)<5.0"; // 3.0

  TString ptlow = " && j0pt>040";
  TString pt020 = " && j0pt>020 && j0pt<030";
  TString pt030 = " && j0pt>030 && j0pt<040";
  TString pt040 = " && j0pt>040 && j0pt<060";
  TString pt060 = " && j0pt>060 && j0pt<080";
  TString pt080 = " && j0pt>080 && j0pt<120";
  TString pt120 = " && j0pt>120 && j0pt<200";
  TString pt200 = " && j0pt>200 && j0pt<600";

  TString pt101 = " && j0pt>100 && j0pt<200";
  TString pt201 = " && j0pt>200 && j0pt<300";
  TString pt301 = " && j0pt>300 && j0pt<400";
  TString pt401 = " && j0pt>400 && j0pt<500";
  TString pt501 = " && j0pt>500 && j0pt<600";
  TString pt500 = " && j0pt>500 && j0pt<1000";
  TString pt600 = " && j0pt>600 && j0pt<1000";

  TString eta0p5 = " && npv<= 8";          // " && abs(j0eta)<0.5";
  TString eta1p0 = " && npv<=14 && npv>8"; // " && abs(j0eta)<1.0 && abs(j0eta)>0.5";
  TString eta1p5 = " && npv>14";           // " && abs(j0eta)<1.5 && abs(j0eta)>1.0";
  TString eta2p0 = " && abs(j0eta)<2.0 && abs(j0eta)>1.5";
  TString eta2p4 = " && abs(j0eta)<2.4 && abs(j0eta)>2.0";

  //zjetPlots->addTreeVar("zjets_dilepmass"      , "dilepmass" , "passZjet && passZmass", "dilepmass"         , 30,  60, 120 );
  //zjetPlots->addTreeVar("zjets_dilepmassNoCut" , "dilepmass" , "passZjet"             , "dilepmass (no cut)", 30,  60, 120 );
  //zjetPlots->addTreeVar("zjets_mu0pt"          , "mu0pt"     , "passZjet && passZmass", "mu0pt"             , 30,   0, 150 );
  //zjetPlots->addTreeVar("zjets_mu1pt"          , "mu1pt"     , "passZjet && passZmass", "mu1pt"             , 30,   0, 150 );
  //zjetPlots->addTreeVar("zjets_mu0eta"         , "mu0eta"    , "passZjet && passZmass", "mu0eta"            , 30,  -5,   5 );
  //zjetPlots->addTreeVar("zjets_mu1eta"         , "mu1eta"    , "passZjet && passZmass", "mu1eta"            , 30,  -5,   5 );
  //zjetPlots->addTreeVar("zjets_npv"            , "npv"       , "passZjet && passZmass", "npv"               , 34,  1,  35 );

/* // ===== eta,npv,etc binned plots

  zjetPlots->addTreeVar("zjets_eta0p5_j0mult" , "j0mult" , "passZjet && passZmass"+ptlow+eta0p5, "multiplicity"  , 25, 0, 50 );
  zjetPlots->addTreeVar("zjets_eta0p5_j0ptd"  , "j0ptd"  , "passZjet && passZmass"+ptlow+eta0p5, "ptD"           , 30, 0,  1 );
  zjetPlots->addTreeVar("zjets_eta0p5_j0axis1", "j0axis1", "passZjet && passZmass"+ptlow+eta0p5, "-Log(axis_{1})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_eta0p5_j0axis2", "j0axis2", "passZjet && passZmass"+ptlow+eta0p5, "-Log(axis_{2})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_eta0p5_j0qgl"  , "j0qgl"  , "passZjet && passZmass"+ptlow+eta0p5, "QGL"           , 30, 0,  1 );
  zjetPlots->addTreeVar("zjets_eta1p0_j0mult" , "j0mult" , "passZjet && passZmass"+ptlow+eta1p0, "multiplicity"  , 25, 0, 50 );
  zjetPlots->addTreeVar("zjets_eta1p0_j0ptd"  , "j0ptd"  , "passZjet && passZmass"+ptlow+eta1p0, "ptD"           , 30, 0,  1 );
  zjetPlots->addTreeVar("zjets_eta1p0_j0axis1", "j0axis1", "passZjet && passZmass"+ptlow+eta1p0, "-Log(axis_{1})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_eta1p0_j0axis2", "j0axis2", "passZjet && passZmass"+ptlow+eta1p0, "-Log(axis_{2})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_eta1p0_j0qgl"  , "j0qgl"  , "passZjet && passZmass"+ptlow+eta1p0, "QGL"           , 30, 0,  1 );
  zjetPlots->addTreeVar("zjets_eta1p5_j0mult" , "j0mult" , "passZjet && passZmass"+ptlow+eta1p5, "multiplicity"  , 25, 0, 50 );
  zjetPlots->addTreeVar("zjets_eta1p5_j0ptd"  , "j0ptd"  , "passZjet && passZmass"+ptlow+eta1p5, "ptD"           , 30, 0,  1 );
  zjetPlots->addTreeVar("zjets_eta1p5_j0axis1", "j0axis1", "passZjet && passZmass"+ptlow+eta1p5, "-Log(axis_{1})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_eta1p5_j0axis2", "j0axis2", "passZjet && passZmass"+ptlow+eta1p5, "-Log(axis_{2})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_eta1p5_j0qgl"  , "j0qgl"  , "passZjet && passZmass"+ptlow+eta1p5, "QGL"           , 30, 0,  1 );
  zjetPlots->addTreeVar("zjets_eta2p0_j0mult" , "j0mult" , "passZjet && passZmass"+ptlow+eta2p0, "multiplicity"  , 25, 0, 50 );
  zjetPlots->addTreeVar("zjets_eta2p0_j0ptd"  , "j0ptd"  , "passZjet && passZmass"+ptlow+eta2p0, "ptD"           , 30, 0,  1 );
  zjetPlots->addTreeVar("zjets_eta2p0_j0axis1", "j0axis1", "passZjet && passZmass"+ptlow+eta2p0, "-Log(axis_{1})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_eta2p0_j0axis2", "j0axis2", "passZjet && passZmass"+ptlow+eta2p0, "-Log(axis_{2})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_eta2p0_j0qgl"  , "j0qgl"  , "passZjet && passZmass"+ptlow+eta2p0, "QGL"           , 30, 0,  1 );
  zjetPlots->addTreeVar("zjets_eta2p4_j0mult" , "j0mult" , "passZjet && passZmass"+ptlow+eta2p4, "multiplicity"  , 25, 0, 50 );
  zjetPlots->addTreeVar("zjets_eta2p4_j0ptd"  , "j0ptd"  , "passZjet && passZmass"+ptlow+eta2p4, "ptD"           , 30, 0,  1 );
  zjetPlots->addTreeVar("zjets_eta2p4_j0axis1", "j0axis1", "passZjet && passZmass"+ptlow+eta2p4, "-Log(axis_{1})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_eta2p4_j0axis2", "j0axis2", "passZjet && passZmass"+ptlow+eta2p4, "-Log(axis_{2})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_eta2p4_j0qgl"  , "j0qgl"  , "passZjet && passZmass"+ptlow+eta2p4, "QGL"           , 30, 0,  1 );
// */
//* // ===== standard Z plots

  zjetPlots->addTreeVar("zjets_dilepmass"      , "dilepmass" , "passZjet && passZmass"+ptlow, "dilepmass"         , 30,  60, 120 );
  zjetPlots->addTreeVar("zjets_dilepmassNoCut" , "dilepmass" , "passZjet"             +ptlow, "dilepmass (no cut)", 30,  60, 120 );
  zjetPlots->addTreeVar("zjets_mu0pt"          , "mu0pt"     , "passZjet && passZmass"+ptlow, "mu0pt"             , 30,   0, 150 );
  zjetPlots->addTreeVar("zjets_mu1pt"          , "mu1pt"     , "passZjet && passZmass"+ptlow, "mu1pt"             , 30,   0, 150 );
  zjetPlots->addTreeVar("zjets_mu0eta"         , "mu0eta"    , "passZjet && passZmass"+ptlow, "mu0eta"            , 30,  -5,   5 );
  zjetPlots->addTreeVar("zjets_mu1eta"         , "mu1eta"    , "passZjet && passZmass"+ptlow, "mu1eta"            , 30,  -5,   5 );
  zjetPlots->addTreeVar("zjets_j0pt"      , "j0pt" , "passZjet && passZmass"+ptlow        , "pt_{T} [GeV]", 30,  0, 120 );
  zjetPlots->addTreeVar("zjets_j0pt_frd"  , "j0pt" , "passZjet && passZmass"+forward+ptlow, "pt_{T} [GeV]", 30,  0, 120 );
  zjetPlots->addTreeVar("zjets_j0eta"     , "j0eta", "passZjet && passZmass"+ptlow        , "#eta"        , 30, -5,   5 );
  zjetPlots->addTreeVar("zjets_j0eta_pt20", "j0eta", "passZjet && passZmass"+pt020        , "#eta (pt20-30)" , 30, -5,   5 );
  zjetPlots->addTreeVar("zjets_rho"       , "rho"  , "passZjet && passZmass"+ptlow        , "#rho"        , 30,  0,  30 );
  zjetPlots->addTreeVar("zjets_npv"       , "npv"  , "passZjet && passZmass"+ptlow        , "npv"         , 34,  1,  35 );
  zjetPlots->addTreeVar("zjets_j0pdgid" , "j0pdgid", "passZjet && passZmass"+ptlow        , "j0pdgid"     , 30,  -5,   5 );

  zjetPlots->addTreeVar("zjets_central_j0mult"   , "j0mult", "passZjet && passZmass"+central+ptlow, "multiplicity", 25, 0, 50 );
  zjetPlots->addTreeVar("zjets_central020_j0mult", "j0mult", "passZjet && passZmass"+central+pt020, "multiplicity", 25, 0, 50 );
  zjetPlots->addTreeVar("zjets_central030_j0mult", "j0mult", "passZjet && passZmass"+central+pt030, "multiplicity", 25, 0, 50 );
  zjetPlots->addTreeVar("zjets_central040_j0mult", "j0mult", "passZjet && passZmass"+central+pt040, "multiplicity", 25, 0, 50 );
  zjetPlots->addTreeVar("zjets_central060_j0mult", "j0mult", "passZjet && passZmass"+central+pt060, "multiplicity", 25, 0, 50 );
  zjetPlots->addTreeVar("zjets_central080_j0mult", "j0mult", "passZjet && passZmass"+central+pt080, "multiplicity", 25, 0, 50 );
  zjetPlots->addTreeVar("zjets_central120_j0mult", "j0mult", "passZjet && passZmass"+central+pt120, "multiplicity", 25, 0, 50 );
  zjetPlots->addTreeVar("zjets_central200_j0mult", "j0mult", "passZjet && passZmass"+central+pt200, "multiplicity", 25, 0, 50 );
  zjetPlots->addTreeVar("zjets_forward_j0mult"   , "j0mult", "passZjet && passZmass"+forward+ptlow, "multiplicity", 25, 0, 50 );
  zjetPlots->addTreeVar("zjets_forward020_j0mult", "j0mult", "passZjet && passZmass"+forward+pt020, "multiplicity", 25, 0, 50 );
  zjetPlots->addTreeVar("zjets_forward030_j0mult", "j0mult", "passZjet && passZmass"+forward+pt030, "multiplicity", 25, 0, 50 );
  zjetPlots->addTreeVar("zjets_forward040_j0mult", "j0mult", "passZjet && passZmass"+forward+pt040, "multiplicity", 25, 0, 50 );
  zjetPlots->addTreeVar("zjets_forward060_j0mult", "j0mult", "passZjet && passZmass"+forward+pt060, "multiplicity", 25, 0, 50 );
  zjetPlots->addTreeVar("zjets_forward080_j0mult", "j0mult", "passZjet && passZmass"+forward+pt080, "multiplicity", 25, 0, 50 );
  zjetPlots->addTreeVar("zjets_forward120_j0mult", "j0mult", "passZjet && passZmass"+forward+pt120, "multiplicity", 25, 0, 50 );
  zjetPlots->addTreeVar("zjets_forward200_j0mult", "j0mult", "passZjet && passZmass"+forward+pt200, "multiplicity", 25, 0, 50 );
  zjetPlots->addTreeVar("zjets_central_j0ptd"   , "j0ptd", "passZjet && passZmass"+central+ptlow, "ptD", 30, 0, 1 );
  zjetPlots->addTreeVar("zjets_central020_j0ptd", "j0ptd", "passZjet && passZmass"+central+pt020, "ptD", 30, 0, 1 );
  zjetPlots->addTreeVar("zjets_central030_j0ptd", "j0ptd", "passZjet && passZmass"+central+pt030, "ptD", 30, 0, 1 );
  zjetPlots->addTreeVar("zjets_central040_j0ptd", "j0ptd", "passZjet && passZmass"+central+pt040, "ptD", 30, 0, 1 );
  zjetPlots->addTreeVar("zjets_central060_j0ptd", "j0ptd", "passZjet && passZmass"+central+pt060, "ptD", 30, 0, 1 );
  zjetPlots->addTreeVar("zjets_central080_j0ptd", "j0ptd", "passZjet && passZmass"+central+pt080, "ptD", 30, 0, 1 );
  zjetPlots->addTreeVar("zjets_central120_j0ptd", "j0ptd", "passZjet && passZmass"+central+pt120, "ptD", 30, 0, 1 );
  zjetPlots->addTreeVar("zjets_central200_j0ptd", "j0ptd", "passZjet && passZmass"+central+pt200, "ptD", 30, 0, 1 );
  zjetPlots->addTreeVar("zjets_forward_j0ptd"   , "j0ptd", "passZjet && passZmass"+forward+ptlow, "ptD", 30, 0, 1 );
  zjetPlots->addTreeVar("zjets_forward020_j0ptd", "j0ptd", "passZjet && passZmass"+forward+pt020, "ptD", 30, 0, 1 );
  zjetPlots->addTreeVar("zjets_forward030_j0ptd", "j0ptd", "passZjet && passZmass"+forward+pt030, "ptD", 30, 0, 1 );
  zjetPlots->addTreeVar("zjets_forward040_j0ptd", "j0ptd", "passZjet && passZmass"+forward+pt040, "ptD", 30, 0, 1 );
  zjetPlots->addTreeVar("zjets_forward060_j0ptd", "j0ptd", "passZjet && passZmass"+forward+pt060, "ptD", 30, 0, 1 );
  zjetPlots->addTreeVar("zjets_forward080_j0ptd", "j0ptd", "passZjet && passZmass"+forward+pt080, "ptD", 30, 0, 1 );
  zjetPlots->addTreeVar("zjets_forward120_j0ptd", "j0ptd", "passZjet && passZmass"+forward+pt120, "ptD", 30, 0, 1 );
  zjetPlots->addTreeVar("zjets_forward200_j0ptd", "j0ptd", "passZjet && passZmass"+forward+pt200, "ptD", 30, 0, 1 );
  zjetPlots->addTreeVar("zjets_central_j0axis1"   , "j0axis1", "passZjet && passZmass"+central+ptlow, "-Log(axis_{1})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_central020_j0axis1", "j0axis1", "passZjet && passZmass"+central+pt020, "-Log(axis_{1})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_central030_j0axis1", "j0axis1", "passZjet && passZmass"+central+pt030, "-Log(axis_{1})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_central040_j0axis1", "j0axis1", "passZjet && passZmass"+central+pt040, "-Log(axis_{1})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_central060_j0axis1", "j0axis1", "passZjet && passZmass"+central+pt060, "-Log(axis_{1})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_central080_j0axis1", "j0axis1", "passZjet && passZmass"+central+pt080, "-Log(axis_{1})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_central120_j0axis1", "j0axis1", "passZjet && passZmass"+central+pt120, "-Log(axis_{1})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_central200_j0axis1", "j0axis1", "passZjet && passZmass"+central+pt200, "-Log(axis_{1})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_forward_j0axis1"   , "j0axis1", "passZjet && passZmass"+forward+ptlow, "-Log(axis_{1})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_forward020_j0axis1", "j0axis1", "passZjet && passZmass"+forward+pt020, "-Log(axis_{1})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_forward030_j0axis1", "j0axis1", "passZjet && passZmass"+forward+pt030, "-Log(axis_{1})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_forward040_j0axis1", "j0axis1", "passZjet && passZmass"+forward+pt040, "-Log(axis_{1})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_forward060_j0axis1", "j0axis1", "passZjet && passZmass"+forward+pt060, "-Log(axis_{1})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_forward080_j0axis1", "j0axis1", "passZjet && passZmass"+forward+pt080, "-Log(axis_{1})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_forward120_j0axis1", "j0axis1", "passZjet && passZmass"+forward+pt120, "-Log(axis_{1})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_forward200_j0axis1", "j0axis1", "passZjet && passZmass"+forward+pt200, "-Log(axis_{1})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_central_j0axis2"   , "j0axis2", "passZjet && passZmass"+central+ptlow, "-Log(axis_{2})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_central020_j0axis2", "j0axis2", "passZjet && passZmass"+central+pt020, "-Log(axis_{2})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_central030_j0axis2", "j0axis2", "passZjet && passZmass"+central+pt030, "-Log(axis_{2})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_central040_j0axis2", "j0axis2", "passZjet && passZmass"+central+pt040, "-Log(axis_{2})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_central060_j0axis2", "j0axis2", "passZjet && passZmass"+central+pt060, "-Log(axis_{2})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_central080_j0axis2", "j0axis2", "passZjet && passZmass"+central+pt080, "-Log(axis_{2})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_central120_j0axis2", "j0axis2", "passZjet && passZmass"+central+pt120, "-Log(axis_{2})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_central200_j0axis2", "j0axis2", "passZjet && passZmass"+central+pt200, "-Log(axis_{2})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_forward_j0axis2"   , "j0axis2", "passZjet && passZmass"+forward+ptlow, "-Log(axis_{2})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_forward020_j0axis2", "j0axis2", "passZjet && passZmass"+forward+pt020, "-Log(axis_{2})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_forward030_j0axis2", "j0axis2", "passZjet && passZmass"+forward+pt030, "-Log(axis_{2})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_forward040_j0axis2", "j0axis2", "passZjet && passZmass"+forward+pt040, "-Log(axis_{2})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_forward060_j0axis2", "j0axis2", "passZjet && passZmass"+forward+pt060, "-Log(axis_{2})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_forward080_j0axis2", "j0axis2", "passZjet && passZmass"+forward+pt080, "-Log(axis_{2})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_forward120_j0axis2", "j0axis2", "passZjet && passZmass"+forward+pt120, "-Log(axis_{2})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_forward200_j0axis2", "j0axis2", "passZjet && passZmass"+forward+pt200, "-Log(axis_{2})", 40, 0, 10 );
  zjetPlots->addTreeVar("zjets_central_j0qgl"   , "j0qgl", "passZjet && passZmass"+central+ptlow, "QGL", 30, 0, 1 );
  zjetPlots->addTreeVar("zjets_central020_j0qgl", "j0qgl", "passZjet && passZmass"+central+pt020, "QGL", 30, 0, 1 );
  zjetPlots->addTreeVar("zjets_central030_j0qgl", "j0qgl", "passZjet && passZmass"+central+pt030, "QGL", 30, 0, 1 );
  zjetPlots->addTreeVar("zjets_central040_j0qgl", "j0qgl", "passZjet && passZmass"+central+pt040, "QGL", 30, 0, 1 );
  zjetPlots->addTreeVar("zjets_central060_j0qgl", "j0qgl", "passZjet && passZmass"+central+pt060, "QGL", 30, 0, 1 );
  zjetPlots->addTreeVar("zjets_central080_j0qgl", "j0qgl", "passZjet && passZmass"+central+pt080, "QGL", 30, 0, 1 );
  zjetPlots->addTreeVar("zjets_central120_j0qgl", "j0qgl", "passZjet && passZmass"+central+pt120, "QGL", 30, 0, 1 );
  zjetPlots->addTreeVar("zjets_central200_j0qgl", "j0qgl", "passZjet && passZmass"+central+pt200, "QGL", 30, 0, 1 );
  zjetPlots->addTreeVar("zjets_forward_j0qgl"   , "j0qgl", "passZjet && passZmass"+forward+ptlow, "QGL", 30, 0, 1 );
  zjetPlots->addTreeVar("zjets_forward020_j0qgl", "j0qgl", "passZjet && passZmass"+forward+pt020, "QGL", 30, 0, 1 );
  zjetPlots->addTreeVar("zjets_forward030_j0qgl", "j0qgl", "passZjet && passZmass"+forward+pt030, "QGL", 30, 0, 1 );
  zjetPlots->addTreeVar("zjets_forward040_j0qgl", "j0qgl", "passZjet && passZmass"+forward+pt040, "QGL", 30, 0, 1 );
  zjetPlots->addTreeVar("zjets_forward060_j0qgl", "j0qgl", "passZjet && passZmass"+forward+pt060, "QGL", 30, 0, 1 );
  zjetPlots->addTreeVar("zjets_forward080_j0qgl", "j0qgl", "passZjet && passZmass"+forward+pt080, "QGL", 30, 0, 1 );
  zjetPlots->addTreeVar("zjets_forward120_j0qgl", "j0qgl", "passZjet && passZmass"+forward+pt120, "QGL", 30, 0, 1 );
  zjetPlots->addTreeVar("zjets_forward200_j0qgl", "j0qgl", "passZjet && passZmass"+forward+pt200, "QGL", 30, 0, 1 );
// */
/* // ===== eta,npv,etc binned plots

  dijetPlots->addTreeVar("dijets_eta0p5_j0mult" , "j0mult" , dijetselection+ptlow+eta0p5, "multiplicity"  , 30, 0, 70 );
  dijetPlots->addTreeVar("dijets_eta0p5_j0ptd"  , "j0ptd"  , dijetselection+ptlow+eta0p5, "ptD"           , 30, 0,  1 );
  dijetPlots->addTreeVar("dijets_eta0p5_j0axis1", "j0axis1", dijetselection+ptlow+eta0p5, "-Log(axis_{1})", 40, 0, 10 );
  dijetPlots->addTreeVar("dijets_eta0p5_j0axis2", "j0axis2", dijetselection+ptlow+eta0p5, "-Log(axis_{2})", 40, 0, 10 );
  dijetPlots->addTreeVar("dijets_eta0p5_j0qgl"  , "j0qgl"  , dijetselection+ptlow+eta0p5, "QGL"           , 30, 0,  1 );
  dijetPlots->addTreeVar("dijets_eta1p0_j0mult" , "j0mult" , dijetselection+ptlow+eta1p0, "multiplicity"  , 30, 0, 70 );
  dijetPlots->addTreeVar("dijets_eta1p0_j0ptd"  , "j0ptd"  , dijetselection+ptlow+eta1p0, "ptD"           , 30, 0,  1 );
  dijetPlots->addTreeVar("dijets_eta1p0_j0axis1", "j0axis1", dijetselection+ptlow+eta1p0, "-Log(axis_{1})", 40, 0, 10 );
  dijetPlots->addTreeVar("dijets_eta1p0_j0axis2", "j0axis2", dijetselection+ptlow+eta1p0, "-Log(axis_{2})", 40, 0, 10 );
  dijetPlots->addTreeVar("dijets_eta1p0_j0qgl"  , "j0qgl"  , dijetselection+ptlow+eta1p0, "QGL"           , 30, 0,  1 );
  dijetPlots->addTreeVar("dijets_eta1p5_j0mult" , "j0mult" , dijetselection+ptlow+eta1p5, "multiplicity"  , 30, 0, 70 );
  dijetPlots->addTreeVar("dijets_eta1p5_j0ptd"  , "j0ptd"  , dijetselection+ptlow+eta1p5, "ptD"           , 30, 0,  1 );
  dijetPlots->addTreeVar("dijets_eta1p5_j0axis1", "j0axis1", dijetselection+ptlow+eta1p5, "-Log(axis_{1})", 40, 0, 10 );
  dijetPlots->addTreeVar("dijets_eta1p5_j0axis2", "j0axis2", dijetselection+ptlow+eta1p5, "-Log(axis_{2})", 40, 0, 10 );
  dijetPlots->addTreeVar("dijets_eta1p5_j0qgl"  , "j0qgl"  , dijetselection+ptlow+eta1p5, "QGL"           , 30, 0,  1 );
  dijetPlots->addTreeVar("dijets_eta2p0_j0mult" , "j0mult" , dijetselection+ptlow+eta2p0, "multiplicity"  , 30, 0, 70 );
  dijetPlots->addTreeVar("dijets_eta2p0_j0ptd"  , "j0ptd"  , dijetselection+ptlow+eta2p0, "ptD"           , 30, 0,  1 );
  dijetPlots->addTreeVar("dijets_eta2p0_j0axis1", "j0axis1", dijetselection+ptlow+eta2p0, "-Log(axis_{1})", 40, 0, 10 );
  dijetPlots->addTreeVar("dijets_eta2p0_j0axis2", "j0axis2", dijetselection+ptlow+eta2p0, "-Log(axis_{2})", 40, 0, 10 );
  dijetPlots->addTreeVar("dijets_eta2p0_j0qgl"  , "j0qgl"  , dijetselection+ptlow+eta2p0, "QGL"           , 30, 0,  1 );
  dijetPlots->addTreeVar("dijets_eta2p4_j0mult" , "j0mult" , dijetselection+ptlow+eta2p4, "multiplicity"  , 30, 0, 70 );
  dijetPlots->addTreeVar("dijets_eta2p4_j0ptd"  , "j0ptd"  , dijetselection+ptlow+eta2p4, "ptD"           , 30, 0,  1 );
  dijetPlots->addTreeVar("dijets_eta2p4_j0axis1", "j0axis1", dijetselection+ptlow+eta2p4, "-Log(axis_{1})", 40, 0, 10 );
  dijetPlots->addTreeVar("dijets_eta2p4_j0axis2", "j0axis2", dijetselection+ptlow+eta2p4, "-Log(axis_{2})", 40, 0, 10 );
  dijetPlots->addTreeVar("dijets_eta2p4_j0qgl"  , "j0qgl"  , dijetselection+ptlow+eta2p4, "QGL"           , 30, 0,  1 );
// */
/* // ===== ht-binned plots

  TString ht950 = " && ht>950";           TString ptht950 = " && ht>950           && j0pt>400 ";
  TString ht750 = " && ht>750 && ht<950"; TString ptht750 = " && ht>750 && ht<950 && j0pt>300 && j0pt<520";
  TString ht625 = " && ht>625 && ht<750"; TString ptht625 = " && ht>625 && ht<750 && j0pt>250 && j0pt<420";
  TString ht550 = " && ht>550 && ht<625"; TString ptht550 = " && ht>550 && ht<625 && j0pt>220 && j0pt<350";
  TString ht500 = " && ht>500 && ht<550"; TString ptht500 = " && ht>500 && ht<550 && j0pt>200 && j0pt<320";
  TString ht450 = " && ht>450 && ht<500"; TString ptht450 = " && ht>450 && ht<500 && j0pt>150 && j0pt<300";

  dijetPlots->addTreeVar("dijets_ht950", "ht" , dijetselection+central+ht950 , "H_{T} [GeV]", 51, 950, 2500 );
  dijetPlots->addTreeVar("dijets_ht750", "ht" , dijetselection+central+ht750 , "H_{T} [GeV]", 20, 750,  950 );
  dijetPlots->addTreeVar("dijets_ht625", "ht" , dijetselection+central+ht625 , "H_{T} [GeV]", 25, 625,  750 );
  dijetPlots->addTreeVar("dijets_ht550", "ht" , dijetselection+central+ht550 , "H_{T} [GeV]", 15, 550,  625 );
  dijetPlots->addTreeVar("dijets_ht500", "ht" , dijetselection+central+ht500 , "H_{T} [GeV]", 10, 500,  550 );
  dijetPlots->addTreeVar("dijets_ht450", "ht" , dijetselection+central+ht450 , "H_{T} [GeV]", 10, 450,  500 );

  dijetPlots->addTreeVar("dijets_j0pt_ht950", "j0pt" , dijetselection+central+ht950 , "pt_{T} [GeV]", 30, 400, 1100 );
  dijetPlots->addTreeVar("dijets_j0pt_ht750", "j0pt" , dijetselection+central+ht750 , "pt_{T} [GeV]", 22, 300,  520 );
  dijetPlots->addTreeVar("dijets_j0pt_ht625", "j0pt" , dijetselection+central+ht625 , "pt_{T} [GeV]", 17, 250,  420 );
  dijetPlots->addTreeVar("dijets_j0pt_ht550", "j0pt" , dijetselection+central+ht550 , "pt_{T} [GeV]", 13, 220,  350 );
  dijetPlots->addTreeVar("dijets_j0pt_ht500", "j0pt" , dijetselection+central+ht500 , "pt_{T} [GeV]", 12, 200,  320 );
  dijetPlots->addTreeVar("dijets_j0pt_ht450", "j0pt" , dijetselection+central+ht450 , "pt_{T} [GeV]", 15, 150,  300 );

  dijetPlots->addTreeVar(  "dijets_j0eta_ht950" , "j0eta"  , dijetselection        +ht950, "#eta"          , 30,  -5,  5 );
  dijetPlots->addTreeVar(    "dijets_npv_ht950" , "npv"    , dijetselection+central+ht950, "npv"           , 34,   1, 34 );
  dijetPlots->addTreeVar( "dijets_j0mult_ht950" , "j0mult" , dijetselection+central+ht950, "multiplicity"  , 35,   0, 70 );
  dijetPlots->addTreeVar(  "dijets_j0ptd_ht950" , "j0ptd"  , dijetselection+central+ht950, "ptD"           , 30,   0,  1 );
  dijetPlots->addTreeVar("dijets_j0axis1_ht950" , "j0axis1", dijetselection+central+ht950, "-Log(axis_{1})", 30,   0, 10 );
  dijetPlots->addTreeVar("dijets_j0axis2_ht950" , "j0axis2", dijetselection+central+ht950, "-Log(axis_{2})", 30,   0, 10 );
  dijetPlots->addTreeVar(  "dijets_j0qgl_ht950" , "j0qgl"  , dijetselection+central+ht950, "QGL"           , 30,   0,  1 );

  dijetPlots->addTreeVar(  "dijets_j0eta_ht750" , "j0eta"  , dijetselection        +ht750, "#eta"          , 30,  -5,  5 );
  dijetPlots->addTreeVar(    "dijets_npv_ht750" , "npv"    , dijetselection+central+ht750, "npv"           , 34,   1, 34 );
  dijetPlots->addTreeVar( "dijets_j0mult_ht750" , "j0mult" , dijetselection+central+ht750, "multiplicity"  , 35,   0, 70 );
  dijetPlots->addTreeVar(  "dijets_j0ptd_ht750" , "j0ptd"  , dijetselection+central+ht750, "ptD"           , 30,   0,  1 );
  dijetPlots->addTreeVar("dijets_j0axis1_ht750" , "j0axis1", dijetselection+central+ht750, "-Log(axis_{1})", 30,   0, 10 );
  dijetPlots->addTreeVar("dijets_j0axis2_ht750" , "j0axis2", dijetselection+central+ht750, "-Log(axis_{2})", 30,   0, 10 );
  dijetPlots->addTreeVar(  "dijets_j0qgl_ht750" , "j0qgl"  , dijetselection+central+ht750, "QGL"           , 30,   0,  1 );

  dijetPlots->addTreeVar(  "dijets_j0eta_ht625" , "j0eta"  , dijetselection        +ht625, "#eta"          , 30,  -5,  5 );
  dijetPlots->addTreeVar(    "dijets_npv_ht625" , "npv"    , dijetselection+central+ht625, "npv"           , 34,   1, 34 );
  dijetPlots->addTreeVar( "dijets_j0mult_ht625" , "j0mult" , dijetselection+central+ht625, "multiplicity"  , 35,   0, 70 );
  dijetPlots->addTreeVar(  "dijets_j0ptd_ht625" , "j0ptd"  , dijetselection+central+ht625, "ptD"           , 30,   0,  1 );
  dijetPlots->addTreeVar("dijets_j0axis1_ht625" , "j0axis1", dijetselection+central+ht625, "-Log(axis_{1})", 30,   0, 10 );
  dijetPlots->addTreeVar("dijets_j0axis2_ht625" , "j0axis2", dijetselection+central+ht625, "-Log(axis_{2})", 30,   0, 10 );
  dijetPlots->addTreeVar(  "dijets_j0qgl_ht625" , "j0qgl"  , dijetselection+central+ht625, "QGL"           , 30,   0,  1 );

  dijetPlots->addTreeVar(  "dijets_j0eta_ht550" , "j0eta"  , dijetselection        +ht550, "#eta"          , 30,  -5,  5 );
  dijetPlots->addTreeVar(    "dijets_npv_ht550" , "npv"    , dijetselection+central+ht550, "npv"           , 34,   1, 34 );
  dijetPlots->addTreeVar( "dijets_j0mult_ht550" , "j0mult" , dijetselection+central+ht550, "multiplicity"  , 35,   0, 70 );
  dijetPlots->addTreeVar(  "dijets_j0ptd_ht550" , "j0ptd"  , dijetselection+central+ht550, "ptD"           , 30,   0,  1 );
  dijetPlots->addTreeVar("dijets_j0axis1_ht550" , "j0axis1", dijetselection+central+ht550, "-Log(axis_{1})", 30,   0, 10 );
  dijetPlots->addTreeVar("dijets_j0axis2_ht550" , "j0axis2", dijetselection+central+ht550, "-Log(axis_{2})", 30,   0, 10 );
  dijetPlots->addTreeVar(  "dijets_j0qgl_ht550" , "j0qgl"  , dijetselection+central+ht550, "QGL"           , 30,   0,  1 );

  dijetPlots->addTreeVar(  "dijets_j0eta_ht500" , "j0eta"  , dijetselection        +ht500, "#eta"          , 30,  -5,  5 );
  dijetPlots->addTreeVar(    "dijets_npv_ht500" , "npv"    , dijetselection+central+ht500, "npv"           , 34,   1, 34 );
  dijetPlots->addTreeVar( "dijets_j0mult_ht500" , "j0mult" , dijetselection+central+ht500, "multiplicity"  , 35,   0, 70 );
  dijetPlots->addTreeVar(  "dijets_j0ptd_ht500" , "j0ptd"  , dijetselection+central+ht500, "ptD"           , 30,   0,  1 );
  dijetPlots->addTreeVar("dijets_j0axis1_ht500" , "j0axis1", dijetselection+central+ht500, "-Log(axis_{1})", 30,   0, 10 );
  dijetPlots->addTreeVar("dijets_j0axis2_ht500" , "j0axis2", dijetselection+central+ht500, "-Log(axis_{2})", 30,   0, 10 );
  dijetPlots->addTreeVar(  "dijets_j0qgl_ht500" , "j0qgl"  , dijetselection+central+ht500, "QGL"           , 30,   0,  1 );

  dijetPlots->addTreeVar(  "dijets_j0eta_ht450" , "j0eta"  , dijetselection        +ht450, "#eta"          , 30,  -5,  5 );
  dijetPlots->addTreeVar(    "dijets_npv_ht450" , "npv"    , dijetselection+central+ht450, "npv"           , 34,   1, 34 );
  dijetPlots->addTreeVar( "dijets_j0mult_ht450" , "j0mult" , dijetselection+central+ht450, "multiplicity"  , 35,   0, 70 );
  dijetPlots->addTreeVar(  "dijets_j0ptd_ht450" , "j0ptd"  , dijetselection+central+ht450, "ptD"           , 30,   0,  1 );
  dijetPlots->addTreeVar("dijets_j0axis1_ht450" , "j0axis1", dijetselection+central+ht450, "-Log(axis_{1})", 30,   0, 10 );
  dijetPlots->addTreeVar("dijets_j0axis2_ht450" , "j0axis2", dijetselection+central+ht450, "-Log(axis_{2})", 30,   0, 10 );
  dijetPlots->addTreeVar(  "dijets_j0qgl_ht450" , "j0qgl"  , dijetselection+central+ht450, "QGL"           , 30,   0,  1 );
// */
//* // ===== standard dijet plots

  dijetPlots->addTreeVar("dijets_j0pt"     , "j0pt" , dijetselection        , "pt_{T} [GeV]", 40, 200, 1000 );
  dijetPlots->addTreeVar("dijets_j0pt_frd" , "j0pt" , dijetselection+forward, "pt_{T} [GeV]", 40, 200, 1000 );
  dijetPlots->addTreeVar("dijets_j0eta"    , "j0eta", dijetselection        , "#eta"        , 30,  -5,    5 );
  dijetPlots->addTreeVar("dijets_rho"      , "rho"  , dijetselection        , "#rho"        , 25,   0,   25 );
  dijetPlots->addTreeVar("dijets_npv"      , "npv"  , dijetselection        , "npv"         , 34,   1,   34 );
  dijetPlots->addTreeVar("dijets_ht"       , "ht"   , dijetselection        , "H_{T} [GeV]" , 50, 450, 1450 );

  dijetPlots->addTreeVar("dijets_central_j0mult"   , "j0mult", dijetselection+central+ptlow, "multiplicity", 35, 0, 70 );
  dijetPlots->addTreeVar("dijets_central201_j0mult", "j0mult", dijetselection+central+pt201, "multiplicity", 35, 0, 70 );
  dijetPlots->addTreeVar("dijets_central301_j0mult", "j0mult", dijetselection+central+pt301, "multiplicity", 35, 0, 70 );
  dijetPlots->addTreeVar("dijets_central401_j0mult", "j0mult", dijetselection+central+pt401, "multiplicity", 35, 0, 70 );
  dijetPlots->addTreeVar("dijets_central501_j0mult", "j0mult", dijetselection+central+pt501, "multiplicity", 35, 0, 70 );
  dijetPlots->addTreeVar("dijets_central600_j0mult", "j0mult", dijetselection+central+pt600, "multiplicity", 35, 0, 70 );
  dijetPlots->addTreeVar("dijets_forward_j0mult"   , "j0mult", dijetselection+forward+ptlow, "multiplicity", 35, 0, 70 );
  dijetPlots->addTreeVar("dijets_forward201_j0mult", "j0mult", dijetselection+forward+pt201, "multiplicity", 35, 0, 70 );
  dijetPlots->addTreeVar("dijets_forward301_j0mult", "j0mult", dijetselection+forward+pt301, "multiplicity", 35, 0, 70 );
  dijetPlots->addTreeVar("dijets_forward401_j0mult", "j0mult", dijetselection+forward+pt401, "multiplicity", 35, 0, 70 );
  dijetPlots->addTreeVar("dijets_forward501_j0mult", "j0mult", dijetselection+forward+pt501, "multiplicity", 35, 0, 70 );
  dijetPlots->addTreeVar("dijets_forward600_j0mult", "j0mult", dijetselection+forward+pt600, "multiplicity", 35, 0, 70 );
  dijetPlots->addTreeVar("dijets_central_j0ptd"   , "j0ptd", dijetselection+central+ptlow, "ptD", 30, 0, 1 );
  dijetPlots->addTreeVar("dijets_central201_j0ptd", "j0ptd", dijetselection+central+pt201, "ptD", 30, 0, 1 );
  dijetPlots->addTreeVar("dijets_central301_j0ptd", "j0ptd", dijetselection+central+pt301, "ptD", 30, 0, 1 );
  dijetPlots->addTreeVar("dijets_central401_j0ptd", "j0ptd", dijetselection+central+pt401, "ptD", 30, 0, 1 );
  dijetPlots->addTreeVar("dijets_central501_j0ptd", "j0ptd", dijetselection+central+pt501, "ptD", 30, 0, 1 );
  dijetPlots->addTreeVar("dijets_central600_j0ptd", "j0ptd", dijetselection+central+pt600, "ptD", 30, 0, 1 );
  dijetPlots->addTreeVar("dijets_forward_j0ptd"   , "j0ptd", dijetselection+forward+ptlow, "ptD", 30, 0, 1 );
  dijetPlots->addTreeVar("dijets_forward201_j0ptd", "j0ptd", dijetselection+forward+pt201, "ptD", 30, 0, 1 );
  dijetPlots->addTreeVar("dijets_forward301_j0ptd", "j0ptd", dijetselection+forward+pt301, "ptD", 30, 0, 1 );
  dijetPlots->addTreeVar("dijets_forward401_j0ptd", "j0ptd", dijetselection+forward+pt401, "ptD", 30, 0, 1 );
  dijetPlots->addTreeVar("dijets_forward501_j0ptd", "j0ptd", dijetselection+forward+pt501, "ptD", 30, 0, 1 );
  dijetPlots->addTreeVar("dijets_forward600_j0ptd", "j0ptd", dijetselection+forward+pt600, "ptD", 30, 0, 1 );
  dijetPlots->addTreeVar("dijets_central_j0axis1"   , "j0axis1", dijetselection+central+ptlow, "-Log(axis_{1})", 30, 0, 10 );
  dijetPlots->addTreeVar("dijets_central201_j0axis1", "j0axis1", dijetselection+central+pt201, "-Log(axis_{1})", 30, 0, 10 );
  dijetPlots->addTreeVar("dijets_central301_j0axis1", "j0axis1", dijetselection+central+pt301, "-Log(axis_{1})", 30, 0, 10 );
  dijetPlots->addTreeVar("dijets_central401_j0axis1", "j0axis1", dijetselection+central+pt401, "-Log(axis_{1})", 30, 0, 10 );
  dijetPlots->addTreeVar("dijets_central501_j0axis1", "j0axis1", dijetselection+central+pt501, "-Log(axis_{1})", 30, 0, 10 );
  dijetPlots->addTreeVar("dijets_central600_j0axis1", "j0axis1", dijetselection+central+pt600, "-Log(axis_{1})", 30, 0, 10 );
  dijetPlots->addTreeVar("dijets_forward_j0axis1"   , "j0axis1", dijetselection+forward+ptlow, "-Log(axis_{1})", 30, 0, 10 );
  dijetPlots->addTreeVar("dijets_forward201_j0axis1", "j0axis1", dijetselection+forward+pt201, "-Log(axis_{1})", 30, 0, 10 );
  dijetPlots->addTreeVar("dijets_forward301_j0axis1", "j0axis1", dijetselection+forward+pt301, "-Log(axis_{1})", 30, 0, 10 );
  dijetPlots->addTreeVar("dijets_forward401_j0axis1", "j0axis1", dijetselection+forward+pt401, "-Log(axis_{1})", 30, 0, 10 );
  dijetPlots->addTreeVar("dijets_forward501_j0axis1", "j0axis1", dijetselection+forward+pt501, "-Log(axis_{1})", 30, 0, 10 );
  dijetPlots->addTreeVar("dijets_forward600_j0axis1", "j0axis1", dijetselection+forward+pt600, "-Log(axis_{1})", 30, 0, 10 );
  dijetPlots->addTreeVar("dijets_central_j0axis2"   , "j0axis2", dijetselection+central+ptlow, "-Log(axis_{2})", 30, 0, 10 );
  dijetPlots->addTreeVar("dijets_central201_j0axis2", "j0axis2", dijetselection+central+pt201, "-Log(axis_{2})", 30, 0, 10 );
  dijetPlots->addTreeVar("dijets_central301_j0axis2", "j0axis2", dijetselection+central+pt301, "-Log(axis_{2})", 30, 0, 10 );
  dijetPlots->addTreeVar("dijets_central401_j0axis2", "j0axis2", dijetselection+central+pt401, "-Log(axis_{2})", 30, 0, 10 );
  dijetPlots->addTreeVar("dijets_central501_j0axis2", "j0axis2", dijetselection+central+pt501, "-Log(axis_{2})", 30, 0, 10 );
  dijetPlots->addTreeVar("dijets_central600_j0axis2", "j0axis2", dijetselection+central+pt600, "-Log(axis_{2})", 30, 0, 10 );
  dijetPlots->addTreeVar("dijets_forward_j0axis2"   , "j0axis2", dijetselection+forward+ptlow, "-Log(axis_{2})", 30, 0, 10 );
  dijetPlots->addTreeVar("dijets_forward201_j0axis2", "j0axis2", dijetselection+forward+pt201, "-Log(axis_{2})", 30, 0, 10 );
  dijetPlots->addTreeVar("dijets_forward301_j0axis2", "j0axis2", dijetselection+forward+pt301, "-Log(axis_{2})", 30, 0, 10 );
  dijetPlots->addTreeVar("dijets_forward401_j0axis2", "j0axis2", dijetselection+forward+pt401, "-Log(axis_{2})", 30, 0, 10 );
  dijetPlots->addTreeVar("dijets_forward501_j0axis2", "j0axis2", dijetselection+forward+pt501, "-Log(axis_{2})", 30, 0, 10 );
  dijetPlots->addTreeVar("dijets_forward600_j0axis2", "j0axis2", dijetselection+forward+pt600, "-Log(axis_{2})", 30, 0, 10 );
  dijetPlots->addTreeVar("dijets_central_j0qgl"   , "j0qgl", dijetselection+central+ptlow, "QGL", 30, 0, 1 );
  dijetPlots->addTreeVar("dijets_central201_j0qgl", "j0qgl", dijetselection+central+pt201, "QGL", 30, 0, 1 );
  dijetPlots->addTreeVar("dijets_central301_j0qgl", "j0qgl", dijetselection+central+pt301, "QGL", 30, 0, 1 );
  dijetPlots->addTreeVar("dijets_central401_j0qgl", "j0qgl", dijetselection+central+pt401, "QGL", 30, 0, 1 );
  dijetPlots->addTreeVar("dijets_central501_j0qgl", "j0qgl", dijetselection+central+pt501, "QGL", 30, 0, 1 );
  dijetPlots->addTreeVar("dijets_central600_j0qgl", "j0qgl", dijetselection+central+pt600, "QGL", 30, 0, 1 );
  dijetPlots->addTreeVar("dijets_forward_j0qgl"   , "j0qgl", dijetselection+forward+ptlow, "QGL", 30, 0, 1 );
  dijetPlots->addTreeVar("dijets_forward201_j0qgl", "j0qgl", dijetselection+forward+pt201, "QGL", 30, 0, 1 );
  dijetPlots->addTreeVar("dijets_forward301_j0qgl", "j0qgl", dijetselection+forward+pt301, "QGL", 30, 0, 1 );
  dijetPlots->addTreeVar("dijets_forward401_j0qgl", "j0qgl", dijetselection+forward+pt401, "QGL", 30, 0, 1 );
  dijetPlots->addTreeVar("dijets_forward501_j0qgl", "j0qgl", dijetselection+forward+pt501, "QGL", 30, 0, 1 );
  dijetPlots->addTreeVar("dijets_forward600_j0qgl", "j0qgl", dijetselection+forward+pt600, "QGL", 30, 0, 1 );
// */
/* // ===== eta,npv,etc binned plots

  gjetPlots->addTreeVar("gjets_eta0p5_j0mult" , "j0mult" , "passGmjet"+ptlow+eta0p5, "multiplicity"  , 25, 0, 50 );
  gjetPlots->addTreeVar("gjets_eta0p5_j0ptd"  , "j0ptd"  , "passGmjet"+ptlow+eta0p5, "ptD"           , 30, 0,  1 );
  gjetPlots->addTreeVar("gjets_eta0p5_j0axis1", "j0axis1", "passGmjet"+ptlow+eta0p5, "-Log(axis_{1})", 40, 0, 10 );
  gjetPlots->addTreeVar("gjets_eta0p5_j0axis2", "j0axis2", "passGmjet"+ptlow+eta0p5, "-Log(axis_{2})", 40, 0, 10 );
  gjetPlots->addTreeVar("gjets_eta0p5_j0qgl"  , "j0qgl"  , "passGmjet"+ptlow+eta0p5, "QGL"           , 30, 0,  1 );
  gjetPlots->addTreeVar("gjets_eta1p0_j0mult" , "j0mult" , "passGmjet"+ptlow+eta1p0, "multiplicity"  , 25, 0, 50 );
  gjetPlots->addTreeVar("gjets_eta1p0_j0ptd"  , "j0ptd"  , "passGmjet"+ptlow+eta1p0, "ptD"           , 30, 0,  1 );
  gjetPlots->addTreeVar("gjets_eta1p0_j0axis1", "j0axis1", "passGmjet"+ptlow+eta1p0, "-Log(axis_{1})", 40, 0, 10 );
  gjetPlots->addTreeVar("gjets_eta1p0_j0axis2", "j0axis2", "passGmjet"+ptlow+eta1p0, "-Log(axis_{2})", 40, 0, 10 );
  gjetPlots->addTreeVar("gjets_eta1p0_j0qgl"  , "j0qgl"  , "passGmjet"+ptlow+eta1p0, "QGL"           , 30, 0,  1 );
  gjetPlots->addTreeVar("gjets_eta1p5_j0mult" , "j0mult" , "passGmjet"+ptlow+eta1p5, "multiplicity"  , 25, 0, 50 );
  gjetPlots->addTreeVar("gjets_eta1p5_j0ptd"  , "j0ptd"  , "passGmjet"+ptlow+eta1p5, "ptD"           , 30, 0,  1 );
  gjetPlots->addTreeVar("gjets_eta1p5_j0axis1", "j0axis1", "passGmjet"+ptlow+eta1p5, "-Log(axis_{1})", 40, 0, 10 );
  gjetPlots->addTreeVar("gjets_eta1p5_j0axis2", "j0axis2", "passGmjet"+ptlow+eta1p5, "-Log(axis_{2})", 40, 0, 10 );
  gjetPlots->addTreeVar("gjets_eta1p5_j0qgl"  , "j0qgl"  , "passGmjet"+ptlow+eta1p5, "QGL"           , 30, 0,  1 );
  gjetPlots->addTreeVar("gjets_eta2p0_j0mult" , "j0mult" , "passGmjet"+ptlow+eta2p0, "multiplicity"  , 25, 0, 50 );
  gjetPlots->addTreeVar("gjets_eta2p0_j0ptd"  , "j0ptd"  , "passGmjet"+ptlow+eta2p0, "ptD"           , 30, 0,  1 );
  gjetPlots->addTreeVar("gjets_eta2p0_j0axis1", "j0axis1", "passGmjet"+ptlow+eta2p0, "-Log(axis_{1})", 40, 0, 10 );
  gjetPlots->addTreeVar("gjets_eta2p0_j0axis2", "j0axis2", "passGmjet"+ptlow+eta2p0, "-Log(axis_{2})", 40, 0, 10 );
  gjetPlots->addTreeVar("gjets_eta2p0_j0qgl"  , "j0qgl"  , "passGmjet"+ptlow+eta2p0, "QGL"           , 30, 0,  1 );
  gjetPlots->addTreeVar("gjets_eta2p4_j0mult" , "j0mult" , "passGmjet"+ptlow+eta2p4, "multiplicity"  , 25, 0, 50 );
  gjetPlots->addTreeVar("gjets_eta2p4_j0ptd"  , "j0ptd"  , "passGmjet"+ptlow+eta2p4, "ptD"           , 30, 0,  1 );
  gjetPlots->addTreeVar("gjets_eta2p4_j0axis1", "j0axis1", "passGmjet"+ptlow+eta2p4, "-Log(axis_{1})", 40, 0, 10 );
  gjetPlots->addTreeVar("gjets_eta2p4_j0axis2", "j0axis2", "passGmjet"+ptlow+eta2p4, "-Log(axis_{2})", 40, 0, 10 );
  gjetPlots->addTreeVar("gjets_eta2p4_j0qgl"  , "j0qgl"  , "passGmjet"+ptlow+eta2p4, "QGL"           , 30, 0,  1 );
// */
//* // ===== standard gjet plots

  gjetPlots->addTreeVar("gjets_j0pt"     , "j0pt" , "passGmjet"        , "pt_{T} [GeV]", 30,  0, 600 );
  gjetPlots->addTreeVar("gjets_j0pt_frd" , "j0pt" , "passGmjet"+forward, "pt_{T} [GeV]", 30,  0, 600 );
  gjetPlots->addTreeVar("gjets_j0eta"    , "j0eta", "passGmjet"        , "#eta"        , 30, -5,   5 );
  gjetPlots->addTreeVar("gjets_rho"      , "rho"  , "passGmjet"        , "#rho"        , 25,  0,  25 );
  gjetPlots->addTreeVar("gjets_npv"      , "npv"  , "passGmjet"        , "npv"         , 34,  1,  35 );
  gjetPlots->addTreeVar("gjets_central_j0mult"   , "j0mult", "passGmjet"+central+ptlow, "multiplicity", 35, 0, 70 );
  gjetPlots->addTreeVar("gjets_central101_j0mult", "j0mult", "passGmjet"+central+pt101, "multiplicity", 35, 0, 70 );
  gjetPlots->addTreeVar("gjets_central201_j0mult", "j0mult", "passGmjet"+central+pt201, "multiplicity", 35, 0, 70 );
  gjetPlots->addTreeVar("gjets_central301_j0mult", "j0mult", "passGmjet"+central+pt301, "multiplicity", 35, 0, 70 );
  gjetPlots->addTreeVar("gjets_central401_j0mult", "j0mult", "passGmjet"+central+pt401, "multiplicity", 35, 0, 70 );
  gjetPlots->addTreeVar("gjets_central500_j0mult", "j0mult", "passGmjet"+central+pt500, "multiplicity", 35, 0, 70 );
  gjetPlots->addTreeVar("gjets_forward_j0mult"   , "j0mult", "passGmjet"+forward+ptlow, "multiplicity", 35, 0, 70 );
  gjetPlots->addTreeVar("gjets_forward101_j0mult", "j0mult", "passGmjet"+forward+pt101, "multiplicity", 35, 0, 70 );
  gjetPlots->addTreeVar("gjets_forward201_j0mult", "j0mult", "passGmjet"+forward+pt201, "multiplicity", 35, 0, 70 );
  gjetPlots->addTreeVar("gjets_forward301_j0mult", "j0mult", "passGmjet"+forward+pt301, "multiplicity", 35, 0, 70 );
  gjetPlots->addTreeVar("gjets_forward401_j0mult", "j0mult", "passGmjet"+forward+pt401, "multiplicity", 35, 0, 70 );
  gjetPlots->addTreeVar("gjets_forward500_j0mult", "j0mult", "passGmjet"+forward+pt500, "multiplicity", 35, 0, 70 );
  gjetPlots->addTreeVar("gjets_central_j0ptd"   , "j0ptd", "passGmjet"+central+ptlow, "ptD", 30, 0, 1 );
  gjetPlots->addTreeVar("gjets_central101_j0ptd", "j0ptd", "passGmjet"+central+pt101, "ptD", 30, 0, 1 );
  gjetPlots->addTreeVar("gjets_central201_j0ptd", "j0ptd", "passGmjet"+central+pt201, "ptD", 30, 0, 1 );
  gjetPlots->addTreeVar("gjets_central301_j0ptd", "j0ptd", "passGmjet"+central+pt301, "ptD", 30, 0, 1 );
  gjetPlots->addTreeVar("gjets_central401_j0ptd", "j0ptd", "passGmjet"+central+pt401, "ptD", 30, 0, 1 );
  gjetPlots->addTreeVar("gjets_central500_j0ptd", "j0ptd", "passGmjet"+central+pt500, "ptD", 30, 0, 1 );
  gjetPlots->addTreeVar("gjets_forward_j0ptd"   , "j0ptd", "passGmjet"+forward+ptlow, "ptD", 30, 0, 1 );
  gjetPlots->addTreeVar("gjets_forward101_j0ptd", "j0ptd", "passGmjet"+forward+pt101, "ptD", 30, 0, 1 );
  gjetPlots->addTreeVar("gjets_forward201_j0ptd", "j0ptd", "passGmjet"+forward+pt201, "ptD", 30, 0, 1 );
  gjetPlots->addTreeVar("gjets_forward301_j0ptd", "j0ptd", "passGmjet"+forward+pt301, "ptD", 30, 0, 1 );
  gjetPlots->addTreeVar("gjets_forward401_j0ptd", "j0ptd", "passGmjet"+forward+pt401, "ptD", 30, 0, 1 );
  gjetPlots->addTreeVar("gjets_forward500_j0ptd", "j0ptd", "passGmjet"+forward+pt500, "ptD", 30, 0, 1 );
  gjetPlots->addTreeVar("gjets_central_j0axis1"   , "j0axis1", "passGmjet"+central+ptlow, "-Log(axis_{1})", 30, 0, 10 );
  gjetPlots->addTreeVar("gjets_central101_j0axis1", "j0axis1", "passGmjet"+central+pt101, "-Log(axis_{1})", 30, 0, 10 );
  gjetPlots->addTreeVar("gjets_central201_j0axis1", "j0axis1", "passGmjet"+central+pt201, "-Log(axis_{1})", 30, 0, 10 );
  gjetPlots->addTreeVar("gjets_central301_j0axis1", "j0axis1", "passGmjet"+central+pt301, "-Log(axis_{1})", 30, 0, 10 );
  gjetPlots->addTreeVar("gjets_central401_j0axis1", "j0axis1", "passGmjet"+central+pt401, "-Log(axis_{1})", 30, 0, 10 );
  gjetPlots->addTreeVar("gjets_central500_j0axis1", "j0axis1", "passGmjet"+central+pt500, "-Log(axis_{1})", 30, 0, 10 );
  gjetPlots->addTreeVar("gjets_forward_j0axis1"   , "j0axis1", "passGmjet"+forward+ptlow, "-Log(axis_{1})", 30, 0, 10 );
  gjetPlots->addTreeVar("gjets_forward101_j0axis1", "j0axis1", "passGmjet"+forward+pt101, "-Log(axis_{1})", 30, 0, 10 );
  gjetPlots->addTreeVar("gjets_forward201_j0axis1", "j0axis1", "passGmjet"+forward+pt201, "-Log(axis_{1})", 30, 0, 10 );
  gjetPlots->addTreeVar("gjets_forward301_j0axis1", "j0axis1", "passGmjet"+forward+pt301, "-Log(axis_{1})", 30, 0, 10 );
  gjetPlots->addTreeVar("gjets_forward401_j0axis1", "j0axis1", "passGmjet"+forward+pt401, "-Log(axis_{1})", 30, 0, 10 );
  gjetPlots->addTreeVar("gjets_forward500_j0axis1", "j0axis1", "passGmjet"+forward+pt500, "-Log(axis_{1})", 30, 0, 10 );
  gjetPlots->addTreeVar("gjets_central_j0axis2"   , "j0axis2", "passGmjet"+central+ptlow, "-Log(axis_{2})", 30, 0, 10 );
  gjetPlots->addTreeVar("gjets_central101_j0axis2", "j0axis2", "passGmjet"+central+pt101, "-Log(axis_{2})", 30, 0, 10 );
  gjetPlots->addTreeVar("gjets_central201_j0axis2", "j0axis2", "passGmjet"+central+pt201, "-Log(axis_{2})", 30, 0, 10 );
  gjetPlots->addTreeVar("gjets_central301_j0axis2", "j0axis2", "passGmjet"+central+pt301, "-Log(axis_{2})", 30, 0, 10 );
  gjetPlots->addTreeVar("gjets_central401_j0axis2", "j0axis2", "passGmjet"+central+pt401, "-Log(axis_{2})", 30, 0, 10 );
  gjetPlots->addTreeVar("gjets_central500_j0axis2", "j0axis2", "passGmjet"+central+pt500, "-Log(axis_{2})", 30, 0, 10 );
  gjetPlots->addTreeVar("gjets_forward_j0axis2"   , "j0axis2", "passGmjet"+forward+ptlow, "-Log(axis_{2})", 30, 0, 10 );
  gjetPlots->addTreeVar("gjets_forward101_j0axis2", "j0axis2", "passGmjet"+forward+pt101, "-Log(axis_{2})", 30, 0, 10 );
  gjetPlots->addTreeVar("gjets_forward201_j0axis2", "j0axis2", "passGmjet"+forward+pt201, "-Log(axis_{2})", 30, 0, 10 );
  gjetPlots->addTreeVar("gjets_forward301_j0axis2", "j0axis2", "passGmjet"+forward+pt301, "-Log(axis_{2})", 30, 0, 10 );
  gjetPlots->addTreeVar("gjets_forward401_j0axis2", "j0axis2", "passGmjet"+forward+pt401, "-Log(axis_{2})", 30, 0, 10 );
  gjetPlots->addTreeVar("gjets_forward500_j0axis2", "j0axis2", "passGmjet"+forward+pt500, "-Log(axis_{2})", 30, 0, 10 );
  gjetPlots->addTreeVar("gjets_central_j0qgl"   , "j0qgl", "passGmjet"+central+ptlow, "QGL", 30, 0, 1 );
  gjetPlots->addTreeVar("gjets_central101_j0qgl", "j0qgl", "passGmjet"+central+pt101, "QGL", 30, 0, 1 );
  gjetPlots->addTreeVar("gjets_central201_j0qgl", "j0qgl", "passGmjet"+central+pt201, "QGL", 30, 0, 1 );
  gjetPlots->addTreeVar("gjets_central301_j0qgl", "j0qgl", "passGmjet"+central+pt301, "QGL", 30, 0, 1 );
  gjetPlots->addTreeVar("gjets_central401_j0qgl", "j0qgl", "passGmjet"+central+pt401, "QGL", 30, 0, 1 );
  gjetPlots->addTreeVar("gjets_central500_j0qgl", "j0qgl", "passGmjet"+central+pt500, "QGL", 30, 0, 1 );
  gjetPlots->addTreeVar("gjets_forward_j0qgl"   , "j0qgl", "passGmjet"+forward+ptlow, "QGL", 30, 0, 1 );
  gjetPlots->addTreeVar("gjets_forward101_j0qgl", "j0qgl", "passGmjet"+forward+pt101, "QGL", 30, 0, 1 );
  gjetPlots->addTreeVar("gjets_forward201_j0qgl", "j0qgl", "passGmjet"+forward+pt201, "QGL", 30, 0, 1 );
  gjetPlots->addTreeVar("gjets_forward301_j0qgl", "j0qgl", "passGmjet"+forward+pt301, "QGL", 30, 0, 1 );
  gjetPlots->addTreeVar("gjets_forward401_j0qgl", "j0qgl", "passGmjet"+forward+pt401, "QGL", 30, 0, 1 );
  gjetPlots->addTreeVar("gjets_forward500_j0qgl", "j0qgl", "passGmjet"+forward+pt500, "QGL", 30, 0, 1 );
// */

  zjetPlots   ->plot();
  dijetPlots  ->plot();
  gjetPlots   ->plot();

  delete zjetPlots;
  delete dijetPlots;
  delete gjetPlots;

}







