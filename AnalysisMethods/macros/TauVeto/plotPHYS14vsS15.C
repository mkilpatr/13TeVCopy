#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#endif


void plotPHYS14vsS15(const TString conffile="runtau.conf",
                     const TString inputdir="trees_091115",
                     const TString outputdir="plots_091115",
                     const bool    plotttbar=false)
{

  gSystem->mkdir(outputdir,true);
  PlotStuff* plots = new PlotStuff(conffile, inputdir, outputdir);
  plots->setPlotSource(PlotStuff::TREES);
  plots->setPlotType(PlotStuff::COMP);
  plots->setUnitScale();
  plots->setTree("Candidates");
  plots->setWgtVar("1.0");
  plots->setPlotOverflow(1);
  plots->setRatioPlot();
  plots->setHeaderText("","","");
  //plots->setLegend(0.4, 0.7, 0.9, 0.9);
  plots->setLegend(0.25, 0.7, 0.9, 0.9);
  plots->setYTitle("Candidates");
  plots->setTreeFileSuffix(".root");
  plots->setAddSigScaleTxt(false);
  //plots->setLogy();
  plots->setMinLogScale(0.001);

  plots->addTreeVar("nGenLeptons",      "nGenLeptons",  "1==1",     "Number of Gen Leptons",         5,-0.5,4.5);
  plots->addTreeVar("nGenTaus",         "nGenTaus",     "1==1",     "Number of Gen Taus",            5,-0.5,4.5);
  plots->addTreeVar("nGenHadTaus",      "nGenHadTaus",  "1==1",     "Number of Gen Hadronic Taus",   5,-0.5,4.5);
  plots->addTreeVar("pt",               "pt",           "pt>10.0",  "p_{T} [GeV]",                   250,0.0,500.0);
  plots->addTreeVar("njets",            "njets",        "pt>10.0",  "Number of Jets",                12,-0.5,11.5);
  plots->addTreeVar("mt",               "mt",           "pt>10.0",  "m_{T} [GeV]",                   250,0.0,500.0);
  plots->addTreeVar("mtnophoton",       "mtnophoton",   "pt>10.0",  "m_{T} [GeV]",                   250,0.0,500.0);
  plots->addTreeVar("dphimet",          "dphimet",      "pt>10.0",  "|#Delta#phi(h, #slash{E}_{T})|",252,0.0,3.15);
  plots->addTreeVar("dphiw",            "dphiw",        "pt>10.0",  "|#Delta#phi(h, W)|",            252,0.0,3.15);
  plots->addTreeVar("misset",           "misset",       "pt>10.0",  "#slash{E}_{T} [GeV]",           250,0.0,1000.0);
  plots->addTreeVar("abseta",           "abseta",       "pt>10.0",  "|#eta|",                        200,0.0,3.0);
  plots->addTreeVar("absdz",            "absdz",        "pt>10.0",  "|dz|",                          200,0.0,2.0);
  plots->addTreeVar("chiso0p1",         "chiso0p1",     "pt>10.0",  "Ch. iso. (#DeltaR=0.1)",        200,0.0,5.0);
  plots->addTreeVar("chiso0p2",         "chiso0p2",     "pt>10.0",  "Ch. iso. (#DeltaR=0.2)",        200,0.0,5.0);
  plots->addTreeVar("chiso0p3",         "chiso0p3",     "pt>10.0",  "Ch. iso. (#DeltaR=0.3)",        200,0.0,5.0);
  plots->addTreeVar("chiso0p4",         "chiso0p4",     "pt>10.0",  "Ch. iso. (#DeltaR=0.4)",        200,0.0,5.0);
  plots->addTreeVar("totiso0p1",        "totiso0p1",    "pt>10.0",  "Tot. iso. (#DeltaR=0.1)",       200,0.0,10.0);
  plots->addTreeVar("totiso0p2",        "totiso0p2",    "pt>10.0",  "Tot. iso. (#DeltaR=0.2)",       200,0.0,10.0);
  plots->addTreeVar("totiso0p3",        "totiso0p3",    "pt>10.0",  "Tot. iso. (#DeltaR=0.3)",       200,0.0,10.0);
  plots->addTreeVar("totiso0p4",        "totiso0p4",    "pt>10.0",  "Tot. iso. (#DeltaR=0.4)",       200,0.0,10.0);
  plots->addTreeVar("neartrkdr",        "neartrkdr",    "pt>10.0",  "#DeltaR(nearest trk)",          200,0.0,2.0);
  plots->addTreeVar("contjetdr",        "contjetdr",    "pt>10.0",  "#DeltaR(containing jet)",       200,0.0,0.5);
  plots->addTreeVar("contjetcsv",       "contjetcsv",   "pt>10.0",  "CSV(containing jet)",           200,0.0,1.0);
  plots->addTreeVar("gentaumatch",      "gentaumatch",  "pt>10.0",  "Gen tau match",                 2,-0.5,1.5);
  plots->addTreeVar("taumva",           "taumva",       "pt>10.0",  "Discriminator",                 200,-1.0,1.0);
  if(plotttbar) {
    plots->addTreeVar("pt_mtpresel",              "pt",           "pt>10.0 && absdz<0.2 && mt<100",  "p_{T} [GeV]",                   50,0.0,500.0);
    plots->addTreeVar("njets_mtpresel",           "njets",        "pt>10.0 && absdz<0.2 && mt<100",  "Number of Jets",                12,-0.5,11.5);
    plots->addTreeVar("mt_mtpresel",              "mt",           "pt>10.0 && absdz<0.2 && mt<100",  "m_{T} [GeV]",                   50,0.0,500.0);
    plots->addTreeVar("mtnophoton_mtpresel",      "mtnophoton",   "pt>10.0 && absdz<0.2 && mt<100",  "m_{T} [GeV]",                   50,0.0,500.0);
    plots->addTreeVar("dphimet_mtpresel",         "dphimet",      "pt>10.0 && absdz<0.2 && mt<100",  "|#Delta#phi(h, #slash{E}_{T})|",42,0.0,3.15);
    plots->addTreeVar("dphiw_mtpresel",           "dphiw",        "pt>10.0 && absdz<0.2 && mt<100",  "|#Delta#phi(h, W)|",            42,0.0,3.15);
    plots->addTreeVar("misset_mtpresel",          "misset",       "pt>10.0 && absdz<0.2 && mt<100",  "#slash{E}_{T} [GeV]",           50,0.0,1000.0);
    plots->addTreeVar("abseta_mtpresel",          "abseta",       "pt>10.0 && absdz<0.2 && mt<100",  "|#eta|",                        40,0.0,3.0);
    plots->addTreeVar("absdz_mtpresel",           "absdz",        "pt>10.0 && absdz<0.2 && mt<100",  "|dz|",                          40,0.0,2.0);
    plots->addTreeVar("chiso0p1_mtpresel",        "chiso0p1",     "pt>10.0 && absdz<0.2 && mt<100",  "Ch. iso. (#DeltaR=0.1)",        40,0.0,5.0);
    plots->addTreeVar("chiso0p2_mtpresel",        "chiso0p2",     "pt>10.0 && absdz<0.2 && mt<100",  "Ch. iso. (#DeltaR=0.2)",        40,0.0,5.0);
    plots->addTreeVar("chiso0p3_mtpresel",        "chiso0p3",     "pt>10.0 && absdz<0.2 && mt<100",  "Ch. iso. (#DeltaR=0.3)",        40,0.0,5.0);
    plots->addTreeVar("chiso0p4_mtpresel",        "chiso0p4",     "pt>10.0 && absdz<0.2 && mt<100",  "Ch. iso. (#DeltaR=0.4)",        40,0.0,5.0);
    plots->addTreeVar("totiso0p1_mtpresel",       "totiso0p1",    "pt>10.0 && absdz<0.2 && mt<100",  "Tot. iso. (#DeltaR=0.1)",       40,0.0,10.0);
    plots->addTreeVar("totiso0p2_mtpresel",       "totiso0p2",    "pt>10.0 && absdz<0.2 && mt<100",  "Tot. iso. (#DeltaR=0.2)",       40,0.0,10.0);
    plots->addTreeVar("totiso0p3_mtpresel",       "totiso0p3",    "pt>10.0 && absdz<0.2 && mt<100",  "Tot. iso. (#DeltaR=0.3)",       40,0.0,10.0);
    plots->addTreeVar("totiso0p4_mtpresel",       "totiso0p4",    "pt>10.0 && absdz<0.2 && mt<100",  "Tot. iso. (#DeltaR=0.4)",       40,0.0,10.0);
    plots->addTreeVar("neartrkdr_mtpresel",       "neartrkdr",    "pt>10.0 && absdz<0.2 && mt<100",  "#DeltaR(nearest trk)",          40,0.0,2.0);
    plots->addTreeVar("contjetdr_mtpresel",       "contjetdr",    "pt>10.0 && absdz<0.2 && mt<100",  "#DeltaR(containing jet)",       40,0.0,0.5);
    plots->addTreeVar("contjetcsv_mtpresel",      "contjetcsv",   "pt>10.0 && absdz<0.2 && mt<100",  "CSV(containing jet)",           40,0.0,1.0);
    plots->addTreeVar("gentaumatch_mtpresel",     "gentaumatch",  "pt>10.0 && absdz<0.2 && mt<100",  "Gen tau match",                 2,-0.5,1.5);
    plots->addTreeVar("taumva_mtpresel",          "taumva",       "pt>10.0 && absdz<0.2 && mt<100",  "Discriminator",                 40,-1.0,1.0);
    plots->addTreeVar("pt_mtpresel_base",              "pt",           "pt>10.0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "p_{T} [GeV]",                   50,0.0,500.0);
    plots->addTreeVar("njets_mtpresel_base",           "njets",        "pt>10.0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "Number of Jets",                12,-0.5,11.5);
    plots->addTreeVar("mt_mtpresel_base",              "mt",           "pt>10.0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "m_{T} [GeV]",                   50,0.0,500.0);
    plots->addTreeVar("mtnophoton_mtpresel_base",      "mtnophoton",   "pt>10.0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "m_{T} [GeV]",                   50,0.0,500.0);
    plots->addTreeVar("dphimet_mtpresel_base",         "dphimet",      "pt>10.0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "|#Delta#phi(h, #slash{E}_{T})|",42,0.0,3.15);
    plots->addTreeVar("dphiw_mtpresel_base",           "dphiw",        "pt>10.0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "|#Delta#phi(h, W)|",            42,0.0,3.15);
    plots->addTreeVar("misset_mtpresel_base",          "misset",       "pt>10.0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "#slash{E}_{T} [GeV]",           50,0.0,1000.0);
    plots->addTreeVar("abseta_mtpresel_base",          "abseta",       "pt>10.0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "|#eta|",                        40,0.0,3.0);
    plots->addTreeVar("absdz_mtpresel_base",           "absdz",        "pt>10.0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "|dz|",                          40,0.0,2.0);
    plots->addTreeVar("chiso0p1_mtpresel_base",        "chiso0p1",     "pt>10.0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "Ch. iso. (#DeltaR=0.1)",        40,0.0,5.0);
    plots->addTreeVar("chiso0p2_mtpresel_base",        "chiso0p2",     "pt>10.0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "Ch. iso. (#DeltaR=0.2)",        40,0.0,5.0);
    plots->addTreeVar("chiso0p3_mtpresel_base",        "chiso0p3",     "pt>10.0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "Ch. iso. (#DeltaR=0.3)",        40,0.0,5.0);
    plots->addTreeVar("chiso0p4_mtpresel_base",        "chiso0p4",     "pt>10.0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "Ch. iso. (#DeltaR=0.4)",        40,0.0,5.0);
    plots->addTreeVar("totiso0p1_mtpresel_base",       "totiso0p1",    "pt>10.0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "Tot. iso. (#DeltaR=0.1)",       40,0.0,10.0);
    plots->addTreeVar("totiso0p2_mtpresel_base",       "totiso0p2",    "pt>10.0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "Tot. iso. (#DeltaR=0.2)",       40,0.0,10.0);
    plots->addTreeVar("totiso0p3_mtpresel_base",       "totiso0p3",    "pt>10.0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "Tot. iso. (#DeltaR=0.3)",       40,0.0,10.0);
    plots->addTreeVar("totiso0p4_mtpresel_base",       "totiso0p4",    "pt>10.0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "Tot. iso. (#DeltaR=0.4)",       40,0.0,10.0);
    plots->addTreeVar("neartrkdr_mtpresel_base",       "neartrkdr",    "pt>10.0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "#DeltaR(nearest trk)",          40,0.0,2.0);
    plots->addTreeVar("contjetdr_mtpresel_base",       "contjetdr",    "pt>10.0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "#DeltaR(containing jet)",       40,0.0,0.5);
    plots->addTreeVar("contjetcsv_mtpresel_base",      "contjetcsv",   "pt>10.0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "CSV(containing jet)",           40,0.0,1.0);
    plots->addTreeVar("gentaumatch_mtpresel_base",     "gentaumatch",  "pt>10.0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "Gen tau match",                 2,-0.5,1.5);
    plots->addTreeVar("taumva_mtpresel_base",          "taumva",       "pt>10.0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "Discriminator",                 40,-1.0,1.0);
  } else {
    plots->addTreeVar("pt_0lep",              "pt",           "pt>10.0 && nGenLeptons==0 && nGenTaus==0",  "p_{T} [GeV]",                   125,0.0,500.0);
    plots->addTreeVar("njets_0lep",           "njets",        "pt>10.0 && nGenLeptons==0 && nGenTaus==0",  "Number of Jets",                12,-0.5,11.5);
    plots->addTreeVar("mt_0lep",              "mt",           "pt>10.0 && nGenLeptons==0 && nGenTaus==0",  "m_{T} [GeV]",                   125,0.0,500.0);
    plots->addTreeVar("mtnophoton_0lep",      "mtnophoton",   "pt>10.0 && nGenLeptons==0 && nGenTaus==0",  "m_{T} [GeV]",                   125,0.0,500.0);
    plots->addTreeVar("dphimet_0lep",         "dphimet",      "pt>10.0 && nGenLeptons==0 && nGenTaus==0",  "|#Delta#phi(h, #slash{E}_{T})|",126,0.0,3.15);
    plots->addTreeVar("dphiw_0lep",           "dphiw",        "pt>10.0 && nGenLeptons==0 && nGenTaus==0",  "|#Delta#phi(h, W)|",            126,0.0,3.15);
    plots->addTreeVar("misset_0lep",          "misset",       "pt>10.0 && nGenLeptons==0 && nGenTaus==0",  "#slash{E}_{T} [GeV]",           125,0.0,1000.0);
    plots->addTreeVar("abseta_0lep",          "abseta",       "pt>10.0 && nGenLeptons==0 && nGenTaus==0",  "|#eta|",                        100,0.0,3.0);
    plots->addTreeVar("absdz_0lep",           "absdz",        "pt>10.0 && nGenLeptons==0 && nGenTaus==0",  "|dz|",                          100,0.0,2.0);
    plots->addTreeVar("chiso0p1_0lep",        "chiso0p1",     "pt>10.0 && nGenLeptons==0 && nGenTaus==0",  "Ch. iso. (#DeltaR=0.1)",        100,0.0,5.0);
    plots->addTreeVar("chiso0p2_0lep",        "chiso0p2",     "pt>10.0 && nGenLeptons==0 && nGenTaus==0",  "Ch. iso. (#DeltaR=0.2)",        100,0.0,5.0);
    plots->addTreeVar("chiso0p3_0lep",        "chiso0p3",     "pt>10.0 && nGenLeptons==0 && nGenTaus==0",  "Ch. iso. (#DeltaR=0.3)",        100,0.0,5.0);
    plots->addTreeVar("chiso0p4_0lep",        "chiso0p4",     "pt>10.0 && nGenLeptons==0 && nGenTaus==0",  "Ch. iso. (#DeltaR=0.4)",        100,0.0,5.0);
    plots->addTreeVar("totiso0p1_0lep",       "totiso0p1",    "pt>10.0 && nGenLeptons==0 && nGenTaus==0",  "Tot. iso. (#DeltaR=0.1)",       100,0.0,10.0);
    plots->addTreeVar("totiso0p2_0lep",       "totiso0p2",    "pt>10.0 && nGenLeptons==0 && nGenTaus==0",  "Tot. iso. (#DeltaR=0.2)",       100,0.0,10.0);
    plots->addTreeVar("totiso0p3_0lep",       "totiso0p3",    "pt>10.0 && nGenLeptons==0 && nGenTaus==0",  "Tot. iso. (#DeltaR=0.3)",       100,0.0,10.0);
    plots->addTreeVar("totiso0p4_0lep",       "totiso0p4",    "pt>10.0 && nGenLeptons==0 && nGenTaus==0",  "Tot. iso. (#DeltaR=0.4)",       100,0.0,10.0);
    plots->addTreeVar("neartrkdr_0lep",       "neartrkdr",    "pt>10.0 && nGenLeptons==0 && nGenTaus==0",  "#DeltaR(nearest trk)",          100,0.0,2.0);
    plots->addTreeVar("contjetdr_0lep",       "contjetdr",    "pt>10.0 && nGenLeptons==0 && nGenTaus==0",  "#DeltaR(containing jet)",       100,0.0,0.5);
    plots->addTreeVar("contjetcsv_0lep",      "contjetcsv",   "pt>10.0 && nGenLeptons==0 && nGenTaus==0",  "CSV(containing jet)",           100,0.0,1.0);
    plots->addTreeVar("gentaumatch_0lep",     "gentaumatch",  "pt>10.0 && nGenLeptons==0 && nGenTaus==0",  "Gen tau match",                 2,-0.5,1.5);
    plots->addTreeVar("taumva_0lep",          "taumva",       "pt>10.0 && nGenLeptons==0 && nGenTaus==0",  "Discriminator",                 100,-1.0,1.0);
    plots->addTreeVar("pt_mtpresel_nomatch",              "pt",           "pt>10.0 && nGenLeptons==0 && nGenTaus==0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "p_{T} [GeV]",                   50,0.0,500.0);
    plots->addTreeVar("njets_mtpresel_nomatch",           "njets",        "pt>10.0 && nGenLeptons==0 && nGenTaus==0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "Number of Jets",                12,-0.5,11.5);
    plots->addTreeVar("mt_mtpresel_nomatch",              "mt",           "pt>10.0 && nGenLeptons==0 && nGenTaus==0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "m_{T} [GeV]",                   50,0.0,500.0);
    plots->addTreeVar("mtnophoton_mtpresel_nomatch",      "mtnophoton",   "pt>10.0 && nGenLeptons==0 && nGenTaus==0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "m_{T} [GeV]",                   50,0.0,500.0);
    plots->addTreeVar("dphimet_mtpresel_nomatch",         "dphimet",      "pt>10.0 && nGenLeptons==0 && nGenTaus==0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "|#Delta#phi(h, #slash{E}_{T})|",42,0.0,3.15);
    plots->addTreeVar("dphiw_mtpresel_nomatch",           "dphiw",        "pt>10.0 && nGenLeptons==0 && nGenTaus==0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "|#Delta#phi(h, W)|",            42,0.0,3.15);
    plots->addTreeVar("misset_mtpresel_nomatch",          "misset",       "pt>10.0 && nGenLeptons==0 && nGenTaus==0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "#slash{E}_{T} [GeV]",           50,0.0,1000.0);
    plots->addTreeVar("abseta_mtpresel_nomatch",          "abseta",       "pt>10.0 && nGenLeptons==0 && nGenTaus==0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "|#eta|",                        40,0.0,3.0);
    plots->addTreeVar("absdz_mtpresel_nomatch",           "absdz",        "pt>10.0 && nGenLeptons==0 && nGenTaus==0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "|dz|",                          40,0.0,2.0);
    plots->addTreeVar("chiso0p1_mtpresel_nomatch",        "chiso0p1",     "pt>10.0 && nGenLeptons==0 && nGenTaus==0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "Ch. iso. (#DeltaR=0.1)",        40,0.0,5.0);
    plots->addTreeVar("chiso0p2_mtpresel_nomatch",        "chiso0p2",     "pt>10.0 && nGenLeptons==0 && nGenTaus==0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "Ch. iso. (#DeltaR=0.2)",        40,0.0,5.0);
    plots->addTreeVar("chiso0p3_mtpresel_nomatch",        "chiso0p3",     "pt>10.0 && nGenLeptons==0 && nGenTaus==0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "Ch. iso. (#DeltaR=0.3)",        40,0.0,5.0);
    plots->addTreeVar("chiso0p4_mtpresel_nomatch",        "chiso0p4",     "pt>10.0 && nGenLeptons==0 && nGenTaus==0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "Ch. iso. (#DeltaR=0.4)",        40,0.0,5.0);
    plots->addTreeVar("totiso0p1_mtpresel_nomatch",       "totiso0p1",    "pt>10.0 && nGenLeptons==0 && nGenTaus==0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "Tot. iso. (#DeltaR=0.1)",       40,0.0,10.0);
    plots->addTreeVar("totiso0p2_mtpresel_nomatch",       "totiso0p2",    "pt>10.0 && nGenLeptons==0 && nGenTaus==0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "Tot. iso. (#DeltaR=0.2)",       40,0.0,10.0);
    plots->addTreeVar("totiso0p3_mtpresel_nomatch",       "totiso0p3",    "pt>10.0 && nGenLeptons==0 && nGenTaus==0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "Tot. iso. (#DeltaR=0.3)",       40,0.0,10.0);
    plots->addTreeVar("totiso0p4_mtpresel_nomatch",       "totiso0p4",    "pt>10.0 && nGenLeptons==0 && nGenTaus==0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "Tot. iso. (#DeltaR=0.4)",       40,0.0,10.0);
    plots->addTreeVar("neartrkdr_mtpresel_nomatch",       "neartrkdr",    "pt>10.0 && nGenLeptons==0 && nGenTaus==0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "#DeltaR(nearest trk)",          40,0.0,2.0);
    plots->addTreeVar("contjetdr_mtpresel_nomatch",       "contjetdr",    "pt>10.0 && nGenLeptons==0 && nGenTaus==0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "#DeltaR(containing jet)",       40,0.0,0.5);
    plots->addTreeVar("contjetcsv_mtpresel_nomatch",      "contjetcsv",   "pt>10.0 && nGenLeptons==0 && nGenTaus==0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "CSV(containing jet)",           40,0.0,1.0);
    plots->addTreeVar("gentaumatch_mtpresel_nomatch",     "gentaumatch",  "pt>10.0 && nGenLeptons==0 && nGenTaus==0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "Gen tau match",                 2,-0.5,1.5);
    plots->addTreeVar("taumva_mtpresel_nomatch",          "taumva",       "pt>10.0 && nGenLeptons==0 && nGenTaus==0 && njets>3 && misset>150 && absdz<0.2 && mt<100",  "Discriminator",                 40,-1.0,1.0);
  }

  plots->plot(); 

  delete plots;

}
