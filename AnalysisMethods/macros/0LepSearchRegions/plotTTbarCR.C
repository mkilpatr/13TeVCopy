#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#endif


void plotTTbarCR(const TString conffile="plotttbarcr_emu.conf",
                 const TString inputdir="trees_ttbarcr",
                 const TString outputdir="plots_ttbarcr/emu",
                 const TString channel="e#mu",
                 const double  sigscale = -1,
                 const bool    plotlog = false,
                 const TString format = "png")
{

  gSystem->mkdir(outputdir, true);

  PlotStuff* plots = new PlotStuff(conffile, inputdir, outputdir);
  plots->setPlotSource(PlotStuff::TREES);
  plots->setPlotType(PlotStuff::DATAMC);
  plots->setTree("Events");
  plots->setRatioPlot();
  plots->setPlotOverflow(1);
  plots->setColor("dyjetstoll-50ns",StyleTools::color_znunu);
  plots->setSigScale(sigscale);
  if(sigscale < 0)
    plots->setAddSigScaleTxt(false);
  plots->setFormat(format);
  if(plotlog)
    plots->setLogy();
  plots->setWgtVar("0.0425*weight*puWeight*normWeight");
  plots->setHeaderText("#sqrt{s} = 13 TeV","42 pb^{-1}",channel);
  plots->setHeaderPosition(0.2, 0.93);

  map<TString,TString> sel;
  sel["trigemu"]       = "passjson && ((passtrigmu17e12 && leptonpdgid==13 && leptonpt>20 && lepton2pdgid==11 && lepton2pt > 15) || (passtrigmu8e17 && leptonpdgid==11 && leptonpt>20 && lepton2pdgid==13 && lepton2pt > 10))";
  sel["trig2mu"]       = "passjson && leptonpdgid==13 && lepton2pdgid==13 && passtrigmu17mu8 && leptonpt>20 && lepton2pt > 10 && (dilepmass<71 || dilepmass>111)";
  sel["trig2e"]        = "passjson && leptonpdgid==11 && lepton2pdgid==11 && passtrige17e12 && leptonpt>20 && lepton2pt > 15 && (dilepmass<71 || dilepmass>111)";
  sel["jets30"]        = " && passcscflt && passeebadscflt && passhbhefixflt && njets30>=1 && nbjets30>=1";

  // emu
  if(channel == "e#mu") {
    plots->addTreeVar("npv_trigemu",                             "npv",                          sel["trigemu"] + sel["jets30"],                                                     "Number of Vertices", 36, -0.5, 35.5);
    plots->addTreeVar("njets_trigemu",                           "njets",                        sel["trigemu"] + sel["jets30"],                                                     "Number of Jets", 11, -0.5, 10.5);
    plots->addTreeVar("njets30_trigemu",                         "njets30",                      sel["trigemu"] + sel["jets30"],                                                     "Number of Jets", 11, -0.5, 10.5);
    plots->addTreeVar("nbjets_trigemu",                          "nbjets",                       sel["trigemu"] + sel["jets30"],                                                     "Number of b-Jets", 6, -0.5, 5.5);
    plots->addTreeVar("nbjets30_trigemu",                        "nbjets30",                     sel["trigemu"] + sel["jets30"],                                                     "Number of b-Jets", 6, -0.5, 5.5);
    plots->addTreeVar("met_trigemu",                             "met",                          sel["trigemu"] + sel["jets30"],                                                     "#slash{E}_{T} [GeV]", 25, 0, 500);
    plots->addTreeVar("metnohf_trigemu",                         "metnohf",                      sel["trigemu"] + sel["jets30"],                                                     "No HF #slash{E}_{T} [GeV]", 25, 0, 500);
    plots->addTreeVar("dilep_pt_trigemu",                        "dileppt",                      sel["trigemu"] + sel["jets30"],                                                     "p_{T}(e#mu) [GeV]", 25, 0, 500);
    plots->addTreeVar("dilep_mass_trigemu",                      "dilepmass",                    sel["trigemu"] + sel["jets30"],                                                     "m(e#mu) [GeV]", 25, 0, 500);
    plots->addTreeVar("ht_trigemu",                              "ht",                           sel["trigemu"] + sel["jets30"],                                                     "H_{T} [GeV]", 25, 0, 300);
    plots->addTreeVar("ht30_trigemu",                            "ht30",                         sel["trigemu"] + sel["jets30"],                                                     "H_{T} [GeV]", 25, 0, 500);
    plots->addTreeVar("j1pt_trigemu",                            "j1pt",                         sel["trigemu"] + sel["jets30"],                                                     "Leading jet p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("j1eta_trigemu",                           "j1eta",                        sel["trigemu"] + sel["jets30"],                                                     "Leading jet #eta", 20, -3.0, 3.0);
    plots->addTreeVar("j2pt_trigemu",                            "j2pt",                         sel["trigemu"] + sel["jets30"] + " && njets30>=2",                                  "Second jet p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("j2eta_trigemu",                           "j2eta",                        sel["trigemu"] + sel["jets30"] + " && njets30>=2",                                  "Second jet #eta", 20, -3.0, 3.0);
    plots->addTreeVar("lepton1pt_trigemu",                       "leptonpt",                     sel["trigemu"] + sel["jets30"],                                                     "Leading lepton p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("lepton1eta_trigemu",                      "leptoneta",                    sel["trigemu"] + sel["jets30"],                                                     "Leading lepton #eta", 20, -3.0, 3.0);
    plots->addTreeVar("lepton2pt_trigemu",                       "lepton2pt",                    sel["trigemu"] + sel["jets30"],                                                     "Second lepton p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("lepton2eta_trigemu",                      "lepton2eta",                   sel["trigemu"] + sel["jets30"],                                                     "Second lepton #eta", 20, -3.0, 3.0);
    plots->addTreeVar("electronpt_trigemu",                      "leptonpt*(leptonpdgid==11)+lepton2pt*(leptonpdgid==13)",   sel["trigemu"] + sel["jets30"],                         "e p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("electroneta_trigemu",                     "leptoneta*(leptonpdgid==11)+lepton2eta*(leptonpdgid==13)", sel["trigemu"] + sel["jets30"],                         "e #eta", 20, -3.0, 3.0);
    plots->addTreeVar("muonpt_trigemu",                          "leptonpt*(leptonpdgid==13)+lepton2pt*(leptonpdgid==11)",   sel["trigemu"] + sel["jets30"],                         "#mu p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("muoneta_trigemu",                         "leptoneta*(leptonpdgid==13)+lepton2eta*(leptonpdgid==11)", sel["trigemu"] + sel["jets30"],                         "#mu #eta", 20, -3.0, 3.0);
  }

  // mumu
  if(channel == "2#mu") {
    plots->addTreeVar("npv_trig2mu",                             "npv",                          sel["trig2mu"] + sel["jets30"],                                                     "Number of Vertices", 36, -0.5, 35.5);
    plots->addTreeVar("njets_trig2mu",                           "njets",                        sel["trig2mu"] + sel["jets30"],                                                     "Number of Jets", 11, -0.5, 10.5);
    plots->addTreeVar("njets30_trig2mu",                         "njets30",                      sel["trig2mu"] + sel["jets30"],                                                     "Number of Jets", 11, -0.5, 10.5);
    plots->addTreeVar("nbjets_trig2mu",                          "nbjets",                       sel["trig2mu"] + sel["jets30"],                                                     "Number of b-Jets", 6, -0.5, 5.5);
    plots->addTreeVar("nbjets30_trig2mu",                        "nbjets30",                     sel["trig2mu"] + sel["jets30"],                                                     "Number of b-Jets", 6, -0.5, 5.5);
    plots->addTreeVar("met_trig2mu",                             "met",                          sel["trig2mu"] + sel["jets30"],                                                     "#slash{E}_{T} [GeV]", 25, 0, 500);
    plots->addTreeVar("metnohf_trig2mu",                         "metnohf",                      sel["trig2mu"] + sel["jets30"],                                                     "No HF #slash{E}_{T} [GeV]", 25, 0, 500);
    plots->addTreeVar("dilep_pt_trig2mu",                        "dileppt",                      sel["trig2mu"] + sel["jets30"],                                                     "p_{T}(#mu#mu) [GeV]", 25, 0, 500);
    plots->addTreeVar("dilep_mass_trig2mu",                      "dilepmass",                    sel["trig2mu"] + sel["jets30"],                                                     "m(#mu#mu) [GeV]", 25, 0, 500);
    plots->addTreeVar("ht_trig2mu",                              "ht",                           sel["trig2mu"] + sel["jets30"],                                                     "H_{T} [GeV]", 25, 0, 300);
    plots->addTreeVar("ht30_trig2mu",                            "ht30",                         sel["trig2mu"] + sel["jets30"],                                                     "H_{T} [GeV]", 25, 0, 500);
    plots->addTreeVar("j1pt_trig2mu",                            "j1pt",                         sel["trig2mu"] + sel["jets30"],                                                     "Leading jet p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("j1eta_trig2mu",                           "j1eta",                        sel["trig2mu"] + sel["jets30"],                                                     "Leading jet #eta", 20, -3.0, 3.0);
    plots->addTreeVar("j2pt_trig2mu",                            "j2pt",                         sel["trig2mu"] + sel["jets30"] + " && njets30>=2",                                  "Second jet p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("j2eta_trig2mu",                           "j2eta",                        sel["trig2mu"] + sel["jets30"] + " && njets30>=2",                                  "Second jet #eta", 20, -3.0, 3.0);
    plots->addTreeVar("muon1pt_trig2mu",                         "leptonpt",                     sel["trig2mu"] + sel["jets30"],                                                     "Leading muon p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("muon1eta_trig2mu",                        "leptoneta",                    sel["trig2mu"] + sel["jets30"],                                                     "Leading muon #eta", 20, -3.0, 3.0);
    plots->addTreeVar("muon2pt_trig2mu",                         "lepton2pt",                    sel["trig2mu"] + sel["jets30"],                                                     "Second muon p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("muon2eta_trig2mu",                        "lepton2eta",                   sel["trig2mu"] + sel["jets30"],                                                     "Second muon #eta", 20, -3.0, 3.0);
  }

  // ee
  if(channel == "2e") {
    plots->addTreeVar("npv_trig2e",                             "npv",                          sel["trig2e"] + sel["jets30"],                                                     "Number of Vertices", 36, -0.5, 35.5);
    plots->addTreeVar("njets_trig2e",                           "njets",                        sel["trig2e"] + sel["jets30"],                                                     "Number of Jets", 11, -0.5, 10.5);
    plots->addTreeVar("njets30_trig2e",                         "njets30",                      sel["trig2e"] + sel["jets30"],                                                     "Number of Jets", 11, -0.5, 10.5);
    plots->addTreeVar("nbjets_trig2e",                          "nbjets",                       sel["trig2e"] + sel["jets30"],                                                     "Number of b-Jets", 6, -0.5, 5.5);
    plots->addTreeVar("nbjets30_trig2e",                        "nbjets30",                     sel["trig2e"] + sel["jets30"],                                                     "Number of b-Jets", 6, -0.5, 5.5);
    plots->addTreeVar("met_trig2e",                             "met",                          sel["trig2e"] + sel["jets30"],                                                     "#slash{E}_{T} [GeV]", 25, 0, 500);
    plots->addTreeVar("metnohf_trig2e",                         "metnohf",                      sel["trig2e"] + sel["jets30"],                                                     "No HF #slash{E}_{T} [GeV]", 25, 0, 500);
    plots->addTreeVar("dilep_pt_trig2e",                        "dileppt",                      sel["trig2e"] + sel["jets30"],                                                     "p_{T}(e#mu) [GeV]", 25, 0, 500);
    plots->addTreeVar("dilep_mass_trig2e",                      "dilepmass",                    sel["trig2e"] + sel["jets30"],                                                     "m(e#mu) [GeV]", 25, 0, 500);
    plots->addTreeVar("ht_trig2e",                              "ht",                           sel["trig2e"] + sel["jets30"],                                                     "H_{T} [GeV]", 25, 0, 300);
    plots->addTreeVar("ht30_trig2e",                            "ht30",                         sel["trig2e"] + sel["jets30"],                                                     "H_{T} [GeV]", 25, 0, 500);
    plots->addTreeVar("j1pt_trig2e",                            "j1pt",                         sel["trig2e"] + sel["jets30"],                                                     "Leading jet p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("j1eta_trig2e",                           "j1eta",                        sel["trig2e"] + sel["jets30"],                                                     "Leading jet #eta", 20, -3.0, 3.0);
    plots->addTreeVar("j2pt_trig2e",                            "j2pt",                         sel["trig2e"] + sel["jets30"] + " && njets30>=2",                                  "Second jet p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("j2eta_trig2e",                           "j2eta",                        sel["trig2e"] + sel["jets30"] + " && njets30>=2",                                  "Second jet #eta", 20, -3.0, 3.0);
    plots->addTreeVar("ele1pt_trig2e",                          "leptonpt",                     sel["trig2e"] + sel["jets30"],                                                     "Leading electron p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("ele1eta_trig2e",                         "leptoneta",                    sel["trig2e"] + sel["jets30"],                                                     "Leading electron #eta", 20, -3.0, 3.0);
    plots->addTreeVar("ele2pt_trig2e",                          "lepton2pt",                    sel["trig2e"] + sel["jets30"],                                                     "Second electron p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("ele2eta_trig2e",                         "lepton2eta",                   sel["trig2e"] + sel["jets30"],                                                     "Second electron #eta", 20, -3.0, 3.0);
  }

  plots->plot();

  delete plots;

}
