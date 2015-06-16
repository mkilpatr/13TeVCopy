#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#endif


void plotZeroLep(const TString conffile="run0lep.conf",
                 const TString inputdir="trees",
                 const TString outputdir="plots",
                 const double  sigscale = 10,
                 const bool    plotlog = false,
                 const TString format = "png")
{

  gSystem->mkdir(outputdir, true);

  PlotStuff* plots = new PlotStuff(conffile, inputdir, outputdir);
  plots->setPlotSource(PlotStuff::TREES);
  plots->setPlotType(PlotStuff::DATAMC);
  plots->setTree("Events");
  plots->setSigScale(sigscale);
  if(sigscale < 0)
    plots->setAddSigScaleTxt(false);
  plots->setFormat(format);
  if(plotlog)
    plots->setLogy();
  plots->setWgtVar("4.0*weight");

  map<TString,TString> sel;
  sel["passvetoes"] = "nvetotau==0 && nvetolep_pog==0";
  sel["njets"]      = " && ak4_njets>=5 && ak4_njets60>=2 && ak4_nmbtags>=1";
  sel["njetsmet"]   = " && ak4_njets>=5 && ak4_njets60>=2 && ak4_nmbtags>=1 && met>200";
  sel["dphij12"]    = " && min(ak4_dphij1met,ak4_dphij2met)>1";
  sel["dphij123"]   = " && min(ak4_dphij1met,ak4_dphij2met)>1 && ak4_dphij3met>0.5";
  sel["minmtb1b2"]  = " && min(ak4_mtcsv1met,ak4_mtcsv2met)>175";
  sel["baseline"]   = sel["passvetoes"] + sel["njetsmet"] + sel["dphij123"] + sel["minmtb1b2"];
  sel["nctt0"]      = " && ncttstd==0";
  sel["ncttgeq1"]   = " && ncttstd>=1";

  plots->addTreeVar("njets_passvetoes",                     "ak4_njets",                        sel["passvetoes"],                                 "Number of Jets", 11, -0.5, 10.5);
  plots->addTreeVar("njets60_passvetoes",                   "ak4_njets60",                      sel["passvetoes"],                                 "Number of Jets (p_{T} > 60 GeV)", 11, -0.5, 10.5);
  plots->addTreeVar("nmbtags_passvetoes",                   "ak4_nmbtags",                      sel["passvetoes"],                                 "Number of b-Jets", 6, -0.5, 5.5);
  plots->addTreeVar("met_passvetoes",                       "met",                              sel["passvetoes"],                                 "#slash{E}_{T} [GeV]", 80, 200, 1000);
  plots->addTreeVar("met_passvetoesnjets",                  "met",                              sel["passvetoes"]+sel["njets"],                    "#slash{E}_{T} [GeV]", 80, 200, 1000);
  plots->addTreeVar("dphij12met_passvetoesnjetsmet",        "min(ak4_dphij1met,ak4_dphij2met)", sel["passvetoes"]+sel["njetsmet"],                 "min(#Delta#phi(j1,#slash{E}_{T}),#Delta#phi(j2,#slash{E}_{T}))", 63 ,0, 3.15);
  plots->addTreeVar("dphij3met_passvetoesnjetsmetdphij12",  "ak4_dphij3met",                    sel["passvetoes"]+sel["njetsmet"]+sel["dphij12"],  "#Delta#phi(j3,#slash{E}_{T})", 63 ,0, 3.15);
  plots->addTreeVar("mtb1b2met_passvetoesnjetsmetdphij123", "min(ak4_mtcsv1met,ak4_mtcsv2met)", sel["passvetoes"]+sel["njetsmet"]+sel["dphij123"], "min(m_{T}(b1,#slash{E}_{T}),m_{T}(b2,#slash{E}_{T}))", 20 ,0, 500);
  plots->addTreeVar("met_baseline",                         "met",                              sel["baseline"],                                   "#slash{E}_{T} [GeV]", 80, 200, 1000);
  plots->addTreeVar("ncttstd_baseline",                     "ncttstd",                          sel["baseline"],                                   "Number of CMS Top Tags", 5, -0.5, 4.5);
  plots->addTreeVar("nmbtags_baselinenctt0",                "ak4_nmbtags",                      sel["baseline"]+sel["nctt0"],                      "Number of b-Jets", 5, 0.5, 5.5);
  plots->addTreeVar("nmbtags_baselinencttgeq1",             "ak4_nmbtags",                      sel["baseline"]+sel["ncttgeq1"],                   "Number of b-Jets", 5, 0.5, 5.5);

  plots->plot(); 

  delete plots;

}
