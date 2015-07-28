#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#endif


void plotOneLep(const TString conffile="run1lep.conf",
                const TString inputdir="trees",
                const TString outputdir="plots")
{

  gSystem->mkdir(outputdir, true);

  PlotStuff* plots = new PlotStuff(conffile, inputdir, outputdir);
  plots->setPlotSource(PlotStuff::TREES);
  plots->setPlotType(PlotStuff::DATAMC);
  plots->setTree("Events");
  plots->setWgtVar("0.0073*weight");
  plots->setRatioPlot();
  plots->setPlotOverflow(1);
  //plots->setScaleToData();
  plots->setColor("t",StyleTools::color_tW);
  plots->setColor("dyjets",StyleTools::color_znunu);
  plots->setColor("wjets",StyleTools::color_ttbar2l);

  map<TString,TString> sel;
  sel["trig"] = "((passtrigmu && abs(leppdgid)==13) || (passtrige && abs(leppdgid)==11))";
  sel["plusmet50"] = " && met>50";
  sel["plusmet100"] = " && met>100";
  sel["plusnjets2"] = " && njets>=2";
  sel["plusnjets3"] = " && njets>=3";
  sel["plusnjets4"] = " && njets>=4";
  sel["met50"] = sel["trig"] + sel["plusmet50"];
  sel["met50njets2"] = sel["trig"] + sel["plusmet50"] + sel["plusnjets2"];
  sel["met50njets3"] = sel["trig"] + sel["plusmet50"] + sel["plusnjets3"];
  sel["met50njets4"] = sel["trig"] + sel["plusmet50"] + sel["plusnjets4"];
  sel["met100"] = sel["trig"] + sel["plusmet100"];
  sel["met100njets2"] = sel["trig"] + sel["plusmet100"] + sel["plusnjets2"];
  sel["met100njets3"] = sel["trig"] + sel["plusmet100"] + sel["plusnjets3"];
  sel["met100njets4"] = sel["trig"] + sel["plusmet100"] + sel["plusnjets4"];

  double mtbins1[6] = {0.0, 50.0, 100.0, 150.0, 200.0, 400.0};
  double mtbins2[5] = {0.0, 50.0, 100.0, 150.0, 400.0};
  double dphibins1[6] = {0.0, 0.5, 1.0, 1.5, 2.0, 3.15};
  double dphibins2[5] = {0.0, 0.5, 1.0, 2.0, 3.15};
  double dphibins3[4] = {0.0, 0.5, 1.0, 3.15};

  plots->addTreeVar("met_met50",                                  "met",                        sel["met50"],                                       "#slash{E}_{T} [Gev]", 16, 0, 400);
  plots->addTreeVar("met_met50njets2",                            "met",                        sel["met50njets2"],                                 "#slash{E}_{T} [Gev]", 16, 0, 400);
  plots->addTreeVar("met_met50njets3",                            "met",                        sel["met50njets3"],                                 "#slash{E}_{T} [Gev]", 16, 0, 400);
  plots->addTreeVar("met_met50njets4",                            "met",                        sel["met50njets4"],                                 "#slash{E}_{T} [Gev]", 16, 0, 400);
  plots->addTreeVar("mt_met50",                                    "mt",                        sel["met50"],                                       "m_{T} [GeV]", 16, 0, 400);
  plots->addTreeVar("mt_met50njets2",                              "mt",                        sel["met50njets2"],                                 "m_{T} [GeV]", 16, 0, 400);
  plots->addTreeVar("mt_met50njets3",                              "mt",                        sel["met50njets3"],                                 "m_{T} [GeV]", 16, 0, 400);
  plots->addTreeVar("mt_met50njets4",                              "mt",                        sel["met50njets4"],                                 "m_{T} [GeV]", 16, 0, 400);
  plots->addTreeVar("mt_met50njets3_varbins1",                     "mt",                        sel["met50njets3"],                                 "m_{T} [GeV]", 5, mtbins1);
  plots->addTreeVar("mt_met50njets3_varbins2",                     "mt",                        sel["met50njets3"],                                 "m_{T} [GeV]", 4, mtbins2);
  plots->addTreeVar("mt_met50njets4_varbins1",                     "mt",                        sel["met50njets4"],                                 "m_{T} [GeV]", 5, mtbins1);
  plots->addTreeVar("mt_met50njets4_varbins2",                     "mt",                        sel["met50njets4"],                                 "m_{T} [GeV]", 4, mtbins2);
  plots->addTreeVar("mt_met100",                                   "mt",                        sel["met100"],                                      "m_{T} [GeV]", 10, 0, 500);
  plots->addTreeVar("mt_met100njets2",                             "mt",                        sel["met100njets2"],                                "m_{T} [GeV]", 10, 0, 500);
  plots->addTreeVar("mt_met100njets3",                             "mt",                        sel["met100njets3"],                                "m_{T} [GeV]", 10, 0, 500);
  plots->addTreeVar("mt_met100njets4",                             "mt",                        sel["met100njets4"],                                "m_{T} [GeV]", 10, 0, 500);
  plots->addTreeVar("mt_met100njets3_varbins1",                    "mt",                        sel["met100njets3"],                                "m_{T} [GeV]", 5, mtbins1);
  plots->addTreeVar("mt_met100njets3_varbins2",                    "mt",                        sel["met100njets3"],                                "m_{T} [GeV]", 4, mtbins2);
  plots->addTreeVar("mt_met100njets4_varbins1",                    "mt",                        sel["met100njets4"],                                "m_{T} [GeV]", 5, mtbins1);
  plots->addTreeVar("mt_met100njets4_varbins2",                    "mt",                        sel["met100njets4"],                                "m_{T} [GeV]", 4, mtbins2);
  plots->addTreeVar("dphilw_met50",                            "dphilw",                        sel["met50"],                                       "|#Delta#phi(l,W)|", 21, 0, 3.15);
  plots->addTreeVar("dphilw_met50njets2",                      "dphilw",                        sel["met50njets2"],                                 "|#Delta#phi(l,W)|", 21, 0, 3.15);
  plots->addTreeVar("dphilw_met50njets3",                      "dphilw",                        sel["met50njets3"],                                 "|#Delta#phi(l,W)|", 9, 0, 3.15);
  plots->addTreeVar("dphilw_met50njets3_varbins1",             "dphilw",                        sel["met50njets3"],                                 "|#Delta#phi(l,W)|", 5, dphibins1);
  plots->addTreeVar("dphilw_met50njets3_varbins2",             "dphilw",                        sel["met50njets3"],                                 "|#Delta#phi(l,W)|", 4, dphibins2);
  plots->addTreeVar("dphilw_met50njets3_varbins3",             "dphilw",                        sel["met50njets3"],                                 "|#Delta#phi(l,W)|", 3, dphibins3);
  plots->addTreeVar("dphilw_met50njets4",                      "dphilw",                        sel["met50njets4"],                                 "|#Delta#phi(l,W)|", 9, 0, 3.15);
  plots->addTreeVar("dphilw_met50njets4_varbins1",             "dphilw",                        sel["met50njets4"],                                 "|#Delta#phi(l,W)|", 5, dphibins1);
  plots->addTreeVar("dphilw_met50njets4_varbins2",             "dphilw",                        sel["met50njets4"],                                 "|#Delta#phi(l,W)|", 4, dphibins2);
  plots->addTreeVar("dphilw_met50njets4_varbins3",             "dphilw",                        sel["met50njets4"],                                 "|#Delta#phi(l,W)|", 3, dphibins3);
  plots->addTreeVar("dphilw_met100",                           "dphilw",                        sel["met100"],                                      "|#Delta#phi(l,W)|", 9, 0, 3.15);
  plots->addTreeVar("dphilw_met100njets2",                     "dphilw",                        sel["met100njets2"],                                "|#Delta#phi(l,W)|", 9, 0, 3.15);
  plots->addTreeVar("dphilw_met100njets3",                     "dphilw",                        sel["met100njets3"],                                "|#Delta#phi(l,W)|", 9, 0, 3.15);
  plots->addTreeVar("dphilw_met100njets3_varbins1",            "dphilw",                        sel["met100njets3"],                                "|#Delta#phi(l,W)|", 5, dphibins1);
  plots->addTreeVar("dphilw_met100njets3_varbins2",            "dphilw",                        sel["met100njets3"],                                "|#Delta#phi(l,W)|", 4, dphibins2);
  plots->addTreeVar("dphilw_met100njets3_varbins3",            "dphilw",                        sel["met100njets3"],                                "|#Delta#phi(l,W)|", 3, dphibins3);
  plots->addTreeVar("dphilw_met100njets4",                     "dphilw",                        sel["met100njets4"],                                "|#Delta#phi(l,W)|", 9, 0, 3.15);
  plots->addTreeVar("dphilw_met100njets4_varbins1",            "dphilw",                        sel["met100njets4"],                                "|#Delta#phi(l,W)|", 5, dphibins1);
  plots->addTreeVar("dphilw_met100njets4_varbins2",            "dphilw",                        sel["met100njets4"],                                "|#Delta#phi(l,W)|", 4, dphibins2);
  plots->addTreeVar("dphilw_met100njets4_varbins3",            "dphilw",                        sel["met100njets4"],                                "|#Delta#phi(l,W)|", 3, dphibins3);

  plots->plot(); 

  delete plots;

}
