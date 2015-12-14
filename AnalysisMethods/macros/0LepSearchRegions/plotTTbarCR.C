#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#endif


void plotTTbarCR(const TString conffile ="plotting/plotttbarcr_emu.conf",
                 const TString inputdir ="trees_ttbarcr_1211",
                 const TString outputdir="plots_ttbarcr/1211/e#mu",
                 const TString channel  ="e#mu",
                 const bool    plotlog  = false,
                 const TString lumistr  = "2.137",
                 const TString format   = "png")
{

  gSystem->mkdir(outputdir, true);

  PlotStuff* plots = new PlotStuff(conffile, inputdir, outputdir);
  plots->setPlotSource(PlotStuff::TREES);
  plots->setPlotType(PlotStuff::DATAMC);
  plots->setTree("Events");
  plots->setRatioPlot();
  plots->setPlotOverflow(1);
  plots->setColor("dytoll",StyleTools::color_znunu);
  plots->setFormat(format);
  if(plotlog)
    plots->setLogy();
  plots->setWgtVar(lumistr+"*weight*truePUWeight");
  plots->setHeaderText("#sqrt{s} = 13 TeV",TString::Format("%4.2f fb^{-1}",stof(string(lumistr.Data()))),channel);
  plots->setHeaderPosition(0.2, 0.93);

  map<TString,TString> sel;
  sel["trigemu"]       = "passjson && ((passtrigmu17e12 && leptonpdgid==13 && leptonpt>30 && lepton2pdgid==11 && lepton2pt > 20) || (passtrigmu8e17 && leptonpdgid==11 && leptonpt>30 && lepton2pdgid==13 && lepton2pt > 15))";
  sel["trig2mu"]       = "passjson && leptonpdgid==13 && lepton2pdgid==13 && passtrigmu17mu8 && leptonpt>30 && lepton2pt > 15 && (dilepmass<71 || dilepmass>111) && !(" + sel["trigemu"] + ")";
  sel["trig2e"]        = "passjson && leptonpdgid==11 && lepton2pdgid==11 && passtrige17e12 && leptonpt>30 && lepton2pt > 15 && (dilepmass<71 || dilepmass>111) && !(" + sel["trigemu"] + ") && !(" + sel["trig2mu"] + ")";
  sel["trigall"]       = "((" + sel["trigemu"]  + ") || (" + sel["trig2mu"] + ") || (" + sel["trig2e"] + "))";
  sel["filt"]          = " && passcscbeamhaloflt && passeebadscflt && passeebadsc4flt && passhbheisoflt && passhbhefltloose";
  sel["jets"]          = " && dilepmass>20 && njets>=1 && nbjets>=1 && met>50"; // + sel["filt"];

  // all
  if(channel == "2l") {
    plots->addTreeVar("npv_trigall",                             "npv",                          sel["trigall"] + sel["jets"],                                                     "Number of Vertices", 36, -0.5, 35.5);
    plots->addTreeVar("njets_trigall",                           "njets",                        sel["trigall"] + sel["jets"],                                                     "Number of Jets", 11, -0.5, 10.5);
    plots->addTreeVar("njets30_trigall",                         "njets30",                      sel["trigall"] + sel["jets"],                                                     "Number of Jets", 11, -0.5, 10.5);
    plots->addTreeVar("nbjets_trigall",                          "nbjets",                       sel["trigall"] + sel["jets"],                                                     "Number of b-Jets", 6, -0.5, 5.5);
    plots->addTreeVar("nbjets30_trigall",                        "nbjets30",                     sel["trigall"] + sel["jets"],                                                     "Number of b-Jets", 6, -0.5, 5.5);
    plots->addTreeVar("met_trigall",                             "met",                          sel["trigall"] + sel["jets"],                                                     "#slash{E}_{T} [GeV]", 25, 0, 500);
    plots->addTreeVar("puppimet_trigall",                        "puppimet",                     sel["trigall"] + sel["jets"],                                                     "PuppE_{T} [GeV]", 25, 0, 500);
    plots->addTreeVar("dilep_pt_trigall",                        "dileppt",                      sel["trigall"] + sel["jets"],                                                     "p_{T}(ll) [GeV]", 20, 0, 400);
    plots->addTreeVar("dilep_mass_trigall",                      "dilepmass",                    sel["trigall"] + sel["jets"],                                                     "m(ll) [GeV]", 20, 0, 400);
    plots->addTreeVar("ht_trigall",                              "ht",                           sel["trigall"] + sel["jets"],                                                     "H_{T} [GeV]", 25, 0, 300);
    plots->addTreeVar("ht30_trigall",                            "ht30",                         sel["trigall"] + sel["jets"],                                                     "H_{T} [GeV]", 25, 0, 500);
    plots->addTreeVar("j1pt_trigall",                            "j1pt",                         sel["trigall"] + sel["jets"],                                                     "Leading jet p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("j1eta_trigall",                           "j1eta",                        sel["trigall"] + sel["jets"],                                                     "Leading jet #eta", 20, -3.0, 3.0);
    plots->addTreeVar("j2pt_trigall",                            "j2pt",                         sel["trigall"] + sel["jets"] + " && njets>=2",                                    "Second jet p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("j2eta_trigall",                           "j2eta",                        sel["trigall"] + sel["jets"] + " && njets>=2",                                    "Second jet #eta", 20, -3.0, 3.0);
    plots->addTreeVar("lepton1pt_trigall",                       "leptonpt",                     sel["trigall"] + sel["jets"],                                                     "Leading lepton p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("lepton1eta_trigall",                      "leptoneta",                    sel["trigall"] + sel["jets"],                                                     "Leading lepton #eta", 20, -3.0, 3.0);
    plots->addTreeVar("lepton2pt_trigall",                       "lepton2pt",                    sel["trigall"] + sel["jets"],                                                     "Second lepton p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("lepton2eta_trigall",                      "lepton2eta",                   sel["trigall"] + sel["jets"],                                                     "Second lepton #eta", 20, -3.0, 3.0);
  }

  // emu
  if(channel == "e#mu") {
    plots->addTreeVar("npv_trigemu",                             "npv",                          sel["trigemu"] + sel["jets"],                                                     "Number of Vertices", 36, -0.5, 35.5);
    plots->addTreeVar("njets_trigemu",                           "njets",                        sel["trigemu"] + sel["jets"],                                                     "Number of Jets", 11, -0.5, 10.5);
    plots->addTreeVar("njets30_trigemu",                         "njets30",                      sel["trigemu"] + sel["jets"],                                                     "Number of Jets", 11, -0.5, 10.5);
    plots->addTreeVar("nbjets_trigemu",                          "nbjets",                       sel["trigemu"] + sel["jets"],                                                     "Number of b-Jets", 6, -0.5, 5.5);
    plots->addTreeVar("nbjets30_trigemu",                        "nbjets30",                     sel["trigemu"] + sel["jets"],                                                     "Number of b-Jets", 6, -0.5, 5.5);
    plots->addTreeVar("met_trigemu",                             "met",                          sel["trigemu"] + sel["jets"],                                                     "#slash{E}_{T} [GeV]", 25, 0, 500);
    plots->addTreeVar("puppimet_trigemu",                        "puppimet",                     sel["trigemu"] + sel["jets"],                                                     "PuppE_{T} [GeV]", 25, 0, 500);
    plots->addTreeVar("dilep_pt_trigemu",                        "dileppt",                      sel["trigemu"] + sel["jets"],                                                     "p_{T}(e#mu) [GeV]", 25, 0, 500);
    plots->addTreeVar("dilep_mass_trigemu",                      "dilepmass",                    sel["trigemu"] + sel["jets"],                                                     "m(e#mu) [GeV]", 25, 0, 500);
    plots->addTreeVar("ht_trigemu",                              "ht",                           sel["trigemu"] + sel["jets"],                                                     "H_{T} [GeV]", 25, 0, 300);
    plots->addTreeVar("ht30_trigemu",                            "ht30",                         sel["trigemu"] + sel["jets"],                                                     "H_{T} [GeV]", 25, 0, 500);
    plots->addTreeVar("j1pt_trigemu",                            "j1pt",                         sel["trigemu"] + sel["jets"],                                                     "Leading jet p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("j1eta_trigemu",                           "j1eta",                        sel["trigemu"] + sel["jets"],                                                     "Leading jet #eta", 20, -3.0, 3.0);
    plots->addTreeVar("j2pt_trigemu",                            "j2pt",                         sel["trigemu"] + sel["jets"] + " && njets>=2",                                  "Second jet p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("j2eta_trigemu",                           "j2eta",                        sel["trigemu"] + sel["jets"] + " && njets>=2",                                  "Second jet #eta", 20, -3.0, 3.0);
    plots->addTreeVar("lepton1pt_trigemu",                       "leptonpt",                     sel["trigemu"] + sel["jets"],                                                     "Leading lepton p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("lepton1eta_trigemu",                      "leptoneta",                    sel["trigemu"] + sel["jets"],                                                     "Leading lepton #eta", 20, -3.0, 3.0);
    plots->addTreeVar("lepton2pt_trigemu",                       "lepton2pt",                    sel["trigemu"] + sel["jets"],                                                     "Second lepton p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("lepton2eta_trigemu",                      "lepton2eta",                   sel["trigemu"] + sel["jets"],                                                     "Second lepton #eta", 20, -3.0, 3.0);
    plots->addTreeVar("electronpt_trigemu",                      "leptonpt*(leptonpdgid==11)+lepton2pt*(leptonpdgid==13)",   sel["trigemu"] + sel["jets"],                         "e p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("electroneta_trigemu",                     "leptoneta*(leptonpdgid==11)+lepton2eta*(leptonpdgid==13)", sel["trigemu"] + sel["jets"],                         "e #eta", 20, -3.0, 3.0);
    plots->addTreeVar("muonpt_trigemu",                          "leptonpt*(leptonpdgid==13)+lepton2pt*(leptonpdgid==11)",   sel["trigemu"] + sel["jets"],                         "#mu p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("muoneta_trigemu",                         "leptoneta*(leptonpdgid==13)+lepton2eta*(leptonpdgid==11)", sel["trigemu"] + sel["jets"],                         "#mu #eta", 20, -3.0, 3.0);
  }

  // mumu
  if(channel == "2#mu") {
    plots->addTreeVar("npv_trig2mu",                             "npv",                          sel["trig2mu"] + sel["jets"],                                                     "Number of Vertices", 36, -0.5, 35.5);
    plots->addTreeVar("njets_trig2mu",                           "njets",                        sel["trig2mu"] + sel["jets"],                                                     "Number of Jets", 11, -0.5, 10.5);
    plots->addTreeVar("njets30_trig2mu",                         "njets30",                      sel["trig2mu"] + sel["jets"],                                                     "Number of Jets", 11, -0.5, 10.5);
    plots->addTreeVar("nbjets_trig2mu",                          "nbjets",                       sel["trig2mu"] + sel["jets"],                                                     "Number of b-Jets", 6, -0.5, 5.5);
    plots->addTreeVar("nbjets30_trig2mu",                        "nbjets30",                     sel["trig2mu"] + sel["jets"],                                                     "Number of b-Jets", 6, -0.5, 5.5);
    plots->addTreeVar("met_trig2mu",                             "met",                          sel["trig2mu"] + sel["jets"],                                                     "#slash{E}_{T} [GeV]", 25, 0, 500);
    plots->addTreeVar("puppimet_trig2mu",                        "puppimet",                     sel["trig2mu"] + sel["jets"],                                                     "PuppE_{T} [GeV]", 25, 0, 500);
    plots->addTreeVar("dilep_pt_trig2mu",                        "dileppt",                      sel["trig2mu"] + sel["jets"],                                                     "p_{T}(#mu#mu) [GeV]", 25, 0, 500);
    plots->addTreeVar("dilep_mass_trig2mu",                      "dilepmass",                    sel["trig2mu"] + sel["jets"],                                                     "m(#mu#mu) [GeV]", 25, 0, 500);
    plots->addTreeVar("ht_trig2mu",                              "ht",                           sel["trig2mu"] + sel["jets"],                                                     "H_{T} [GeV]", 25, 0, 300);
    plots->addTreeVar("ht30_trig2mu",                            "ht30",                         sel["trig2mu"] + sel["jets"],                                                     "H_{T} [GeV]", 25, 0, 500);
    plots->addTreeVar("j1pt_trig2mu",                            "j1pt",                         sel["trig2mu"] + sel["jets"],                                                     "Leading jet p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("j1eta_trig2mu",                           "j1eta",                        sel["trig2mu"] + sel["jets"],                                                     "Leading jet #eta", 20, -3.0, 3.0);
    plots->addTreeVar("j2pt_trig2mu",                            "j2pt",                         sel["trig2mu"] + sel["jets"] + " && njets>=2",                                  "Second jet p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("j2eta_trig2mu",                           "j2eta",                        sel["trig2mu"] + sel["jets"] + " && njets>=2",                                  "Second jet #eta", 20, -3.0, 3.0);
    plots->addTreeVar("muon1pt_trig2mu",                         "leptonpt",                     sel["trig2mu"] + sel["jets"],                                                     "Leading muon p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("muon1eta_trig2mu",                        "leptoneta",                    sel["trig2mu"] + sel["jets"],                                                     "Leading muon #eta", 20, -3.0, 3.0);
    plots->addTreeVar("muon2pt_trig2mu",                         "lepton2pt",                    sel["trig2mu"] + sel["jets"],                                                     "Second muon p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("muon2eta_trig2mu",                        "lepton2eta",                   sel["trig2mu"] + sel["jets"],                                                     "Second muon #eta", 20, -3.0, 3.0);
  }

  // ee
  if(channel == "2e") {
    plots->addTreeVar("npv_trig2e",                             "npv",                          sel["trig2e"] + sel["jets"],                                                     "Number of Vertices", 36, -0.5, 35.5);
    plots->addTreeVar("njets_trig2e",                           "njets",                        sel["trig2e"] + sel["jets"],                                                     "Number of Jets", 11, -0.5, 10.5);
    plots->addTreeVar("njets30_trig2e",                         "njets30",                      sel["trig2e"] + sel["jets"],                                                     "Number of Jets", 11, -0.5, 10.5);
    plots->addTreeVar("nbjets_trig2e",                          "nbjets",                       sel["trig2e"] + sel["jets"],                                                     "Number of b-Jets", 6, -0.5, 5.5);
    plots->addTreeVar("nbjets30_trig2e",                        "nbjets30",                     sel["trig2e"] + sel["jets"],                                                     "Number of b-Jets", 6, -0.5, 5.5);
    plots->addTreeVar("met_trig2e",                             "met",                          sel["trig2e"] + sel["jets"],                                                     "#slash{E}_{T} [GeV]", 25, 0, 500);
    plots->addTreeVar("puppimet_trig2e",                        "puppimet",                     sel["trig2e"] + sel["jets"],                                                     "PuppE_{T} [GeV]", 25, 0, 500);
    plots->addTreeVar("dilep_pt_trig2e",                        "dileppt",                      sel["trig2e"] + sel["jets"],                                                     "p_{T}(e#mu) [GeV]", 25, 0, 500);
    plots->addTreeVar("dilep_mass_trig2e",                      "dilepmass",                    sel["trig2e"] + sel["jets"],                                                     "m(e#mu) [GeV]", 25, 0, 500);
    plots->addTreeVar("ht_trig2e",                              "ht",                           sel["trig2e"] + sel["jets"],                                                     "H_{T} [GeV]", 25, 0, 300);
    plots->addTreeVar("ht30_trig2e",                            "ht30",                         sel["trig2e"] + sel["jets"],                                                     "H_{T} [GeV]", 25, 0, 500);
    plots->addTreeVar("j1pt_trig2e",                            "j1pt",                         sel["trig2e"] + sel["jets"],                                                     "Leading jet p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("j1eta_trig2e",                           "j1eta",                        sel["trig2e"] + sel["jets"],                                                     "Leading jet #eta", 20, -3.0, 3.0);
    plots->addTreeVar("j2pt_trig2e",                            "j2pt",                         sel["trig2e"] + sel["jets"] + " && njets>=2",                                  "Second jet p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("j2eta_trig2e",                           "j2eta",                        sel["trig2e"] + sel["jets"] + " && njets>=2",                                  "Second jet #eta", 20, -3.0, 3.0);
    plots->addTreeVar("ele1pt_trig2e",                          "leptonpt",                     sel["trig2e"] + sel["jets"],                                                     "Leading electron p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("ele1eta_trig2e",                         "leptoneta",                    sel["trig2e"] + sel["jets"],                                                     "Leading electron #eta", 20, -3.0, 3.0);
    plots->addTreeVar("ele2pt_trig2e",                          "lepton2pt",                    sel["trig2e"] + sel["jets"],                                                     "Second electron p_{T} [GeV]", 25, 0, 250);
    plots->addTreeVar("ele2eta_trig2e",                         "lepton2eta",                   sel["trig2e"] + sel["jets"],                                                     "Second electron #eta", 20, -3.0, 3.0);
  }

  plots->plot();

  delete plots;

}
