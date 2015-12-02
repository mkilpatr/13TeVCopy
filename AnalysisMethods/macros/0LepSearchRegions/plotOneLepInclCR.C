#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#endif


void plotOneLepInclCR(const TString conffile="plotting/plot1lepcr_mu.conf",    // plotting/plot1lepcr_e.conf
                      const TString inputdir="trees_1lepinclcr",
                      const TString outputdir="plots_1linclcr/mu",    // plots_1linclcr/e
                      const TString channel="#mu",   // e
                      const double  sigscale = -1,
                      const bool    plotlog = false,
                      const TString lumistr = "578",
                      const TString format = "png")
{

  gSystem->mkdir(outputdir, true);

  PlotStuff* plots = new PlotStuff(conffile, inputdir, outputdir);
  plots->setPlotSource(PlotStuff::TREES);
  plots->setPlotType(PlotStuff::DATAMC);
  plots->setTree("Events");
  plots->setRatioPlot();
  plots->setPlotOverflow(1);
  plots->setColor("dytoll",StyleTools::color_znunu);
  plots->setSigScale(sigscale);
  if(sigscale < 0)
    plots->setAddSigScaleTxt(false);
  plots->setFormat(format);
  if(plotlog)
    plots->setLogy();
  plots->setWgtVar(lumistr+"*0.001*weight*puWeight");
  plots->setHeaderText("#sqrt{s} = 13 TeV",lumistr + " pb^{-1}",channel);
  plots->setHeaderPosition(0.2, 0.93);

  map<TString,TString> sel;
  sel["trige"]   = "passjson && passtrige && leptonpdgid==11";
  sel["trigmu"]  = "passjson && passtrigmu && leptonpdgid==13";
  sel["filt"]    = " && passcscflt && passeebadscflt && passhbheflttight";
  sel["jets"]    = " && njets>=2 && mtlepmet<150 && met>50";
  sel["0b"]      = " && nbjets==0";
  sel["b"]       = " && nbjets>1";

  // electrons
  if(channel == "e") {
    plots->addTreeVar("npv_trige",                             "npv",                          sel["trige"] + sel["jets"],                                                     "Number of Vertices", 36, -0.5, 35.5);
    plots->addTreeVar("njets_trige",                           "njets",                        sel["trige"] + sel["jets"],                                                     "Number of Jets", 11, -0.5, 10.5);
    plots->addTreeVar("njets30_trige",                         "njets30",                      sel["trige"] + sel["jets"],                                                     "Number of Jets", 11, -0.5, 10.5);
    plots->addTreeVar("nbjets_trige",                          "nbjets",                       sel["trige"] + sel["jets"],                                                     "Number of b-Jets", 6, -0.5, 5.5);
    plots->addTreeVar("nbjets30_trige",                        "nbjets30",                     sel["trige"] + sel["jets"],                                                     "Number of b-Jets", 6, -0.5, 5.5);
    plots->addTreeVar("met_trige",                             "met",                          sel["trige"] + sel["jets"],                                                     "#slash{E}_{T} [GeV]", 25, 0, 500);
    plots->addTreeVar("metnohf_trige",                         "metnohf",                      sel["trige"] + sel["jets"],                                                     "No HF #slash{E}_{T} [GeV]", 25, 0, 500);
    plots->addTreeVar("mtlepmet_trige",                        "mtlepmet",                     sel["trige"] + sel["jets"],                                                     "m_{T}(l, #slash{E}_{T}) [GeV]", 25, 0, 250);
    plots->addTreeVar("dphilepw_trige",                        "absdphilepw",                  sel["trige"] + sel["jets"],                                                     "|#Delta#phi(l,W)|", 18, 0, 3.15);
    plots->addTreeVar("dphij12met_trige",                      "dphij12met",                   sel["trige"] + sel["jets"],                                                     "min(#Delta#phi(j1,#slash{E}_{T}),#Delta#phi(j2,#slash{E}_{T}))", 18, 0, 3.15);
    plots->addTreeVar("dphij3met_trige",                       "dphij3met",                    sel["trige"] + sel["jets"] + " && dphij2met>0.4",                                                     "#Delta#phi(j3,#slash{E}_{T})", 18, 0, 3.15);
    plots->addTreeVar("ht_trige",                              "ht",                           sel["trige"] + sel["jets"],                                                     "H_{T} [GeV]", 25, 0, 300);
    plots->addTreeVar("ht30_trige",                            "ht30",                         sel["trige"] + sel["jets"],                                                     "H_{T} [GeV]", 25, 0, 500);
    plots->addTreeVar("j1pt_trige",                            "j1pt",                         sel["trige"] + sel["jets"],                                                     "Leading jet p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("j1eta_trige",                           "j1eta",                        sel["trige"] + sel["jets"],                                                     "Leading jet #eta", 20, -3.0, 3.0);
    plots->addTreeVar("j2pt_trige",                            "j2pt",                         sel["trige"] + sel["jets"] + " && njets>=2",                                  "Second jet p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("j2eta_trige",                           "j2eta",                        sel["trige"] + sel["jets"] + " && njets>=2",                                  "Second jet #eta", 20, -3.0, 3.0);
    plots->addTreeVar("elept_trige",                           "leptonpt",                     sel["trige"] + sel["jets"],                                                     "Electron p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("eleeta_trige",                          "leptoneta",                    sel["trige"] + sel["jets"],                                                     "Electron #eta", 20, -3.0, 3.0);
    plots->addTreeVar("npv_trige_0b",                          "npv",                          sel["trige"] + sel["jets"] + sel["0b"],                                         "Number of Vertices", 36, -0.5, 35.5);
    plots->addTreeVar("njets_trige_0b",                        "njets",                        sel["trige"] + sel["jets"] + sel["0b"],                                         "Number of Jets", 11, -0.5, 10.5);
    plots->addTreeVar("njets30_trige_0b",                      "njets30",                      sel["trige"] + sel["jets"] + sel["0b"],                                         "Number of Jets", 11, -0.5, 10.5);
    plots->addTreeVar("nbjets_trige_0b",                       "nbjets",                       sel["trige"] + sel["jets"] + sel["0b"],                                         "Number of b-Jets", 6, -0.5, 5.5);
    plots->addTreeVar("nbjets30_trige_0b",                     "nbjets30",                     sel["trige"] + sel["jets"] + sel["0b"],                                         "Number of b-Jets", 6, -0.5, 5.5);
    plots->addTreeVar("met_trige_0b",                          "met",                          sel["trige"] + sel["jets"] + sel["0b"],                                         "#slash{E}_{T} [GeV]", 25, 0, 500);
    plots->addTreeVar("metnohf_trige_0b",                      "metnohf",                      sel["trige"] + sel["jets"] + sel["0b"],                                         "No HF #slash{E}_{T} [GeV]", 25, 0, 500);
    plots->addTreeVar("mtlepmet_trige_0b",                     "mtlepmet",                     sel["trige"] + sel["jets"] + sel["0b"],                                         "m_{T}(l, #slash{E}_{T}) [GeV]", 25, 0, 250);
    plots->addTreeVar("dphilepw_trige_0b",                     "absdphilepw",                  sel["trige"] + sel["jets"] + sel["0b"],                                         "|#Delta#phi(l,W)|", 18, 0, 3.15);
    plots->addTreeVar("ht_trige_0b",                           "ht",                           sel["trige"] + sel["jets"] + sel["0b"],                                         "H_{T} [GeV]", 25, 0, 300);
    plots->addTreeVar("ht30_trige_0b",                         "ht30",                         sel["trige"] + sel["jets"] + sel["0b"],                                         "H_{T} [GeV]", 25, 0, 500);
    plots->addTreeVar("j1pt_trige_0b",                         "j1pt",                         sel["trige"] + sel["jets"] + sel["0b"],                                         "Leading jet p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("j1eta_trige_0b",                        "j1eta",                        sel["trige"] + sel["jets"] + sel["0b"],                                         "Leading jet #eta", 20, -3.0, 3.0);
    plots->addTreeVar("j2pt_trige_0b",                         "j2pt",                         sel["trige"] + sel["jets"] + sel["0b"] + " && njets>=2",                      "Second jet p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("j2eta_trige_0b",                        "j2eta",                        sel["trige"] + sel["jets"] + sel["0b"] + " && njets>=2",                      "Second jet #eta", 20, -3.0, 3.0);
    plots->addTreeVar("elept_trige_0b",                        "leptonpt",                     sel["trige"] + sel["jets"] + sel["0b"],                                         "Electron p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("eleeta_trige_0b",                       "leptoneta",                    sel["trige"] + sel["jets"] + sel["0b"],                                         "Electron #eta", 20, -3.0, 3.0);
    plots->addTreeVar("npv_trige_b",                           "npv",                          sel["trige"] + sel["jets"] + sel["b"],                                          "Number of Vertices", 36, -0.5, 35.5);
    plots->addTreeVar("njets_trige_b",                         "njets",                        sel["trige"] + sel["jets"] + sel["b"],                                          "Number of Jets", 11, -0.5, 10.5);
    plots->addTreeVar("njets30_trige_b",                       "njets30",                      sel["trige"] + sel["jets"] + sel["b"],                                          "Number of Jets", 11, -0.5, 10.5);
    plots->addTreeVar("nbjets_trige_b",                        "nbjets",                       sel["trige"] + sel["jets"] + sel["b"],                                          "Number of b-Jets", 6, -0.5, 5.5);
    plots->addTreeVar("nbjets30_trige_b",                      "nbjets30",                     sel["trige"] + sel["jets"] + sel["b"],                                          "Number of b-Jets", 6, -0.5, 5.5);
    plots->addTreeVar("met_trige_b",                           "met",                          sel["trige"] + sel["jets"] + sel["b"],                                          "#slash{E}_{T} [GeV]", 25, 0, 500);
    plots->addTreeVar("metnohf_trige_b",                       "metnohf",                      sel["trige"] + sel["jets"] + sel["b"],                                          "No HF #slash{E}_{T} [GeV]", 25, 0, 500);
    plots->addTreeVar("mtlepmet_trige_b",                      "mtlepmet",                     sel["trige"] + sel["jets"] + sel["b"],                                          "m_{T}(l, #slash{E}_{T}) [GeV]", 25, 0, 250);
    plots->addTreeVar("dphilepw_trige_b",                      "absdphilepw",                  sel["trige"] + sel["jets"] + sel["b"],                                          "|#Delta#phi(l,W)|", 18, 0, 3.15);
    plots->addTreeVar("ht_trige_b",                            "ht",                           sel["trige"] + sel["jets"] + sel["b"],                                          "H_{T} [GeV]", 25, 0, 300);
    plots->addTreeVar("ht30_trige_b",                          "ht30",                         sel["trige"] + sel["jets"] + sel["b"],                                          "H_{T} [GeV]", 25, 0, 500);
    plots->addTreeVar("j1pt_trige_b",                          "j1pt",                         sel["trige"] + sel["jets"] + sel["b"],                                          "Leading jet p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("j1eta_trige_b",                         "j1eta",                        sel["trige"] + sel["jets"] + sel["b"],                                          "Leading jet #eta", 20, -3.0, 3.0);
    plots->addTreeVar("j2pt_trige_b",                          "j2pt",                         sel["trige"] + sel["jets"] + sel["b"] + " && njets>=2",                       "Second jet p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("j2eta_trige_b",                         "j2eta",                        sel["trige"] + sel["jets"] + sel["b"] + " && njets>=2",                       "Second jet #eta", 20, -3.0, 3.0);
    plots->addTreeVar("elept_trige_b",                         "leptonpt",                     sel["trige"] + sel["jets"] + sel["b"],                                          "Electron p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("eleeta_trige_b",                        "leptoneta",                    sel["trige"] + sel["jets"] + sel["b"],                                          "Electron #eta", 20, -3.0, 3.0);
  }

  // muons
  if(channel == "#mu") {
    plots->addTreeVar("npv_trigmu",                            "npv",                          sel["trigmu"] + sel["jets"],                                                    "Number of Vertices", 36, -0.5, 35.5);
    plots->addTreeVar("njets_trigmu",                          "njets",                        sel["trigmu"] + sel["jets"],                                                    "Number of Jets", 11, -0.5, 10.5);
    plots->addTreeVar("njets30_trigmu",                        "njets30",                      sel["trigmu"] + sel["jets"],                                                    "Number of Jets", 11, -0.5, 10.5);
    plots->addTreeVar("nbjets_trigmu",                         "nbjets",                       sel["trigmu"] + sel["jets"],                                                    "Number of b-Jets", 6, -0.5, 5.5);
    plots->addTreeVar("nbjets30_trigmu",                       "nbjets30",                     sel["trigmu"] + sel["jets"],                                                    "Number of b-Jets", 6, -0.5, 5.5);
    plots->addTreeVar("met_trigmu",                            "met",                          sel["trigmu"] + sel["jets"],                                                    "#slash{E}_{T} [GeV]", 25, 0, 500);
    plots->addTreeVar("metnohf_trigmu",                        "metnohf",                      sel["trigmu"] + sel["jets"],                                                    "No HF #slash{E}_{T} [GeV]", 25, 0, 500);
    plots->addTreeVar("mtlepmet_trigmu",                       "mtlepmet",                     sel["trigmu"] + sel["jets"],                                                    "m_{T}(l, #slash{E}_{T}) [GeV]", 25, 0, 250);
    plots->addTreeVar("dphilepw_trigmu",                       "absdphilepw",                  sel["trigmu"] + sel["jets"],                                                    "|#Delta#phi(l,W)|", 18, 0, 3.15);
    plots->addTreeVar("ht_trigmu",                             "ht",                           sel["trigmu"] + sel["jets"],                                                    "H_{T} [GeV]", 25, 0, 300);
    plots->addTreeVar("ht30_trigmu",                           "ht30",                         sel["trigmu"] + sel["jets"],                                                    "H_{T} [GeV]", 25, 0, 500);
    plots->addTreeVar("j1pt_trigmu",                           "j1pt",                         sel["trigmu"] + sel["jets"],                                                    "Leading jet p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("j1eta_trigmu",                          "j1eta",                        sel["trigmu"] + sel["jets"],                                                    "Leading jet #eta", 20, -3.0, 3.0);
    plots->addTreeVar("j2pt_trigmu",                           "j2pt",                         sel["trigmu"] + sel["jets"] + " && njets>=2",                                 "Second jet p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("j2eta_trigmu",                          "j2eta",                        sel["trigmu"] + sel["jets"] + " && njets>=2",                                 "Second jet #eta", 20, -3.0, 3.0);
    plots->addTreeVar("muonpt_trigmu",                         "leptonpt",                     sel["trigmu"] + sel["jets"],                                                    "Muon p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("muoneta_trigmu",                        "leptoneta",                    sel["trigmu"] + sel["jets"],                                                    "Muon #eta", 20, -3.0, 3.0);
    plots->addTreeVar("npv_trigmu_0b",                         "npv",                          sel["trigmu"] + sel["jets"] + sel["0b"],                                        "Number of Vertices", 36, -0.5, 35.5);
    plots->addTreeVar("njets_trigmu_0b",                       "njets",                        sel["trigmu"] + sel["jets"] + sel["0b"],                                        "Number of Jets", 11, -0.5, 10.5);
    plots->addTreeVar("njets30_trigmu_0b",                     "njets30",                      sel["trigmu"] + sel["jets"] + sel["0b"],                                        "Number of Jets", 11, -0.5, 10.5);
    plots->addTreeVar("nbjets_trigmu_0b",                      "nbjets",                       sel["trigmu"] + sel["jets"] + sel["0b"],                                        "Number of b-Jets", 6, -0.5, 5.5);
    plots->addTreeVar("nbjets30_trigmu_0b",                    "nbjets30",                     sel["trigmu"] + sel["jets"] + sel["0b"],                                        "Number of b-Jets", 6, -0.5, 5.5);
    plots->addTreeVar("met_trigmu_0b",                         "met",                          sel["trigmu"] + sel["jets"] + sel["0b"],                                        "#slash{E}_{T} [GeV]", 25, 0, 500);
    plots->addTreeVar("metnohf_trigmu_0b",                     "metnohf",                      sel["trigmu"] + sel["jets"] + sel["0b"],                                        "No HF #slash{E}_{T} [GeV]", 25, 0, 500);
    plots->addTreeVar("mtlepmet_trigmu_0b",                    "mtlepmet",                     sel["trigmu"] + sel["jets"] + sel["0b"],                                        "m_{T}(l, #slash{E}_{T}) [GeV]", 25, 0, 250);
    plots->addTreeVar("dphilepw_trigmu_0b",                    "absdphilepw",                  sel["trigmu"] + sel["jets"] + sel["0b"],                                        "|#Delta#phi(l,W)|", 18, 0, 3.15);
    plots->addTreeVar("ht_trigmu_0b",                          "ht",                           sel["trigmu"] + sel["jets"] + sel["0b"],                                        "H_{T} [GeV]", 25, 0, 300);
    plots->addTreeVar("ht30_trigmu_0b",                        "ht30",                         sel["trigmu"] + sel["jets"] + sel["0b"],                                        "H_{T} [GeV]", 25, 0, 500);
    plots->addTreeVar("j1pt_trigmu_0b",                        "j1pt",                         sel["trigmu"] + sel["jets"] + sel["0b"],                                        "Leading jet p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("j1eta_trigmu_0b",                       "j1eta",                        sel["trigmu"] + sel["jets"] + sel["0b"],                                        "Leading jet #eta", 20, -3.0, 3.0);
    plots->addTreeVar("j2pt_trigmu_0b",                        "j2pt",                         sel["trigmu"] + sel["jets"] + sel["0b"] + " && njets>=2",                     "Second jet p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("j2eta_trigmu_0b",                       "j2eta",                        sel["trigmu"] + sel["jets"] + sel["0b"] + " && njets>=2",                     "Second jet #eta", 20, -3.0, 3.0);
    plots->addTreeVar("muonpt_trigmu_0b",                      "leptonpt",                     sel["trigmu"] + sel["jets"] + sel["0b"],                                        "Muon p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("muoneta_trigmu_0b",                     "leptoneta",                    sel["trigmu"] + sel["jets"] + sel["0b"],                                        "Muon #eta", 20, -3.0, 3.0);
    plots->addTreeVar("npv_trigmu_b",                          "npv",                          sel["trigmu"] + sel["jets"] + sel["b"],                                         "Number of Vertices", 36, -0.5, 35.5);
    plots->addTreeVar("njets_trigmu_b",                        "njets",                        sel["trigmu"] + sel["jets"] + sel["b"],                                         "Number of Jets", 11, -0.5, 10.5);
    plots->addTreeVar("njets30_trigmu_b",                      "njets30",                      sel["trigmu"] + sel["jets"] + sel["b"],                                         "Number of Jets", 11, -0.5, 10.5);
    plots->addTreeVar("nbjets_trigmu_b",                       "nbjets",                       sel["trigmu"] + sel["jets"] + sel["b"],                                         "Number of b-Jets", 6, -0.5, 5.5);
    plots->addTreeVar("nbjets30_trigmu_b",                     "nbjets30",                     sel["trigmu"] + sel["jets"] + sel["b"],                                         "Number of b-Jets", 6, -0.5, 5.5);
    plots->addTreeVar("met_trigmu_b",                          "met",                          sel["trigmu"] + sel["jets"] + sel["b"],                                         "#slash{E}_{T} [GeV]", 25, 0, 500);
    plots->addTreeVar("metnohf_trigmu_b",                      "metnohf",                      sel["trigmu"] + sel["jets"] + sel["b"],                                         "No HF #slash{E}_{T} [GeV]", 25, 0, 500);
    plots->addTreeVar("mtlepmet_trigmu_b",                     "mtlepmet",                     sel["trigmu"] + sel["jets"] + sel["b"],                                         "m_{T}(l, #slash{E}_{T}) [GeV]", 25, 0, 250);
    plots->addTreeVar("dphilepw_trigmu_b",                     "absdphilepw",                  sel["trigmu"] + sel["jets"] + sel["b"],                                         "|#Delta#phi(l,W)|", 18, 0, 3.15);
    plots->addTreeVar("ht_trigmu_b",                           "ht",                           sel["trigmu"] + sel["jets"] + sel["b"],                                         "H_{T} [GeV]", 25, 0, 300);
    plots->addTreeVar("ht30_trigmu_b",                         "ht30",                         sel["trigmu"] + sel["jets"] + sel["b"],                                         "H_{T} [GeV]", 25, 0, 500);
    plots->addTreeVar("j1pt_trigmu_b",                         "j1pt",                         sel["trigmu"] + sel["jets"] + sel["b"],                                         "Leading jet p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("j1eta_trigmu_b",                        "j1eta",                        sel["trigmu"] + sel["jets"] + sel["b"],                                         "Leading jet #eta", 20, -3.0, 3.0);
    plots->addTreeVar("j2pt_trigmu_b",                         "j2pt",                         sel["trigmu"] + sel["jets"] + sel["b"] + " && njets>=2",                      "Second jet p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("j2eta_trigmu_b",                        "j2eta",                        sel["trigmu"] + sel["jets"] + sel["b"] + " && njets>=2",                      "Second jet #eta", 20, -3.0, 3.0);
    plots->addTreeVar("muonpt_trigmu_b",                       "leptonpt",                     sel["trigmu"] + sel["jets"] + sel["b"],                                         "Muon p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("muoneta_trigmu_b",                      "leptoneta",                    sel["trigmu"] + sel["jets"] + sel["b"],                                         "Muon #eta", 20, -3.0, 3.0);
  }

  plots->plot();

  delete plots;

}
