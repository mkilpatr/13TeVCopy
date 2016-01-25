#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#endif


void plotZeroLep(const TString conffile="plotting/plot0lep.conf",
                 const TString inputdir="/eos/uscms/store/user/vdutta/13TeV/trees/121815",
                 const TString outputdir="plots_0lep",
                 const double  sigscale = 50,
                 const bool    plotlog = false,
                 const TString lumistr  = "2.262",
                 const TString format = "pdf")
{

  gSystem->mkdir(outputdir, true);

  PlotStuff* plots = new PlotStuff(conffile, inputdir, outputdir);
  plots->setPlotSource(PlotStuff::TREES);
  plots->setPlotType(PlotStuff::DATAMC);
  plots->setTree("Events");
  //plots->setRatioPlot();
  plots->setColor("T2tt_850_100",kBlue);
  plots->setColor("T2tt_500_325",kPink+2);
  plots->setColor("ttV",StyleTools::color_ttZ);
  plots->setPlotOverflow(1);
  plots->setSigScale(sigscale);
  if(sigscale < 0)
    plots->setAddSigScaleTxt(false);
  plots->setFormat(format);
  if(plotlog) {
    plots->setLogy();
    plots->setMinLogScale(0.2);
  }
  plots->setWgtVar(lumistr+"*weight*truePUWeight*btagWeight");
  plots->setHeaderText("#sqrt{s} = 13 TeV",TString::Format("%4.2f fb^{-1}",stof(string(lumistr.Data()))),"");
  plots->setHeaderPosition(0.16, 0.93);

  map<TString,TString> sel;
  sel["trig"]       = "passjson && passdijetmet && passcscbeamhaloflt && passeebadscflt && passeebadsc4flt && passhbheisoflt && passhbhefltloose";
  sel["passvetoes"] = " && nvetotau==0 && nvetolep==0";
  sel["met"]        = " && met>250";
  sel["njets75"]    = " && njets75 >= 2";
  sel["njets"]      = " && njets >= 5";
  sel["nlbjets"]    = " && nlbjets >= 2";
  sel["nbjets"]     = " && nbjets >= 1";
  sel["dphij12met"] = " && dphij12met>1";
  sel["dphij3met"]  = " && dphij3met>0.5";
  sel["dphij123"]   = " && dphij12met>1 && dphij3met>0.5";
  sel["baseline"]   = sel["trig"] + sel["passvetoes"] + sel["met"] + sel["njets75"] + sel["njets"] + sel["nlbjets"] + sel["nbjets"] + sel["dphij12met"] + sel["dphij3met"];

  plots->addTreeVar("njets75_reducedbaseline",     "njets75",    sel["trig"] + sel["passvetoes"] + sel["met"] + sel["njets"] + sel["nlbjets"] + sel["nbjets"] + sel["dphij12met"] + sel["dphij3met"],    "Number of Jets (p_{T} > 75 GeV)", 11, -0.5, 10.5);
  plots->addTreeVar("njets_reducedbaseline",       "njets",      sel["trig"] + sel["passvetoes"] + sel["met"] + sel["njets75"] + sel["nlbjets"] + sel["nbjets"] + sel["dphij12met"] + sel["dphij3met"],  "Number of Jets", 11, -0.5, 10.5);
  plots->addTreeVar("nlbjets_reducedbaseline",     "nlbjets",    sel["trig"] + sel["passvetoes"] + sel["met"] + sel["njets75"] + sel["njets"] + sel["nbjets"] + sel["dphij12met"] + sel["dphij3met"],    "Number of Loose b-Tagged Jets", 6, -0.5, 5.5);
  plots->addTreeVar("nbjets_reducedbaseline",      "nbjets",     sel["trig"] + sel["passvetoes"] + sel["met"] + sel["njets75"] + sel["njets"] + sel["nlbjets"] + sel["dphij12met"] + sel["dphij3met"],   "Number of b-Tagged Jets", 6, -0.5, 5.5);
  plots->addTreeVar("dphij12met_reducedbaseline",  "dphij12met", sel["trig"] + sel["passvetoes"] + sel["met"] + sel["njets75"] + sel["njets"] + sel["nlbjets"] + sel["nbjets"] + sel["dphij3met"],       "min(#Delta#phi(j_{1},#slash{E}_{T}),#Delta#phi(j_{2},#slash{E}_{T}))", 63, 0.0, 3.15);
  plots->addTreeVar("dphij3met_reducedbaseline",   "dphij3met",  sel["trig"] + sel["passvetoes"] + sel["met"] + sel["njets75"] + sel["njets"] + sel["nlbjets"] + sel["nbjets"] + sel["dphij12met"],      "#Delta#phi(j_{3},#slash{E}_{T})", 21, 0.0, 3.15);
  plots->addTreeVar("met_reducedbaseline",         "met",        sel["trig"] + sel["passvetoes"] + sel["njets75"] + sel["njets"] + sel["nlbjets"] + sel["nbjets"] + sel["dphij12met"] + sel["dphij3met"],"#slash{E}_{T} [GeV]", 50, 250.0, 1000.0);
  plots->addTreeVar("met_baseline_lowmtb",         "met",        sel["baseline"] + " && mtcsv12met <= 175", "#slash{E}_{T} [GeV]", 50, 250.0, 1000.0);
  plots->addTreeVar("met_baseline_highmtb",        "met",        sel["baseline"] + " && mtcsv12met > 175",  "#slash{E}_{T} [GeV]", 50, 250.0, 1000.0);
  plots->addTreeVar("mtb_baseline",            "mtcsv12met", sel["baseline"],                           "min(m_{T}(b_{1},#slash{E}_{T}),m_{T}(b_{2},#slash{E}_{T})) [GeV]", 20, 0.0, 500.0);
  plots->addTreeVar("nctt_baseline_lowmtb",    "ncttstd",    sel["baseline"] + " && mtcsv12met <= 175", "Number of Top Tags", 4, -0.5, 3.5);
  plots->addTreeVar("nctt_baseline_highmtb",   "ncttstd",    sel["baseline"] + " && mtcsv12met > 175",  "Number of Top Tags", 4, -0.5, 3.5);
  plots->addTreeVar("nbjets_baseline_nctt0",   "nbjets",     sel["baseline"] + " && ncttstd==0",        "Number of b-Tagged Jets", 6, -0.5, 5.5);
  plots->addTreeVar("nbjets_baseline_nctt1",   "nbjets",     sel["baseline"] + " && ncttstd>0",         "Number of b-Tagged Jets", 6, -0.5, 5.5);
  plots->addTreeVar("nbjets_baseline_highmtb_nctt0",   "nbjets",     sel["baseline"] + " && mtcsv12met > 175 && ncttstd==0",        "Number of b-Tagged Jets", 6, -0.5, 5.5);
  plots->addTreeVar("nbjets_baseline_highmtb_nctt1",   "nbjets",     sel["baseline"] + " && mtcsv12met > 175 && ncttstd>0",         "Number of b-Tagged Jets", 6, -0.5, 5.5);
  plots->addTreeVar("nbjets_baseline_lowmtb_nctt0",    "nbjets",     sel["baseline"] + " && mtcsv12met <= 175 && ncttstd==0",       "Number of b-Tagged Jets", 6, -0.5, 5.5);
  plots->addTreeVar("nbjets_baseline_lowmtb_nctt1",    "nbjets",     sel["baseline"] + " && mtcsv12met <= 175 && ncttstd>0",        "Number of b-Tagged Jets", 6, -0.5, 5.5);

  plots->plot(); 

  delete plots;

}
