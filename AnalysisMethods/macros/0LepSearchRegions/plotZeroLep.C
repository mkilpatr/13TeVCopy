#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#endif

void plotZeroLep(const TString conffile="plotting/plot0lep.conf",
                 const TString inputdir="root://cmseos:1094//store/user/mullin/13TeV/lepCor/trees/160211_defaults_pr537",
                 const TString outputdir="plots_0lep",
                 const double  sigscale = 10,
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
  plots->setColor("T2tt_700_1"  ,kRed);
  plots->setColor("T2tt_600_200",kViolet-1);
  plots->setColor("T2tt_350_150",kViolet-1);
  plots->setColor("znunu",kRed-9);
  plots->setPlotOverflow(1);
  plots->setSigScale(sigscale);
  if(sigscale < 0)
    plots->setAddSigScaleTxt(false);
  plots->setFormat(format);
  if(plotlog) {
    plots->setLogy();
    plots->setMinLogScale(0.2);
  }
  plots->setWgtVar(lumistr+"*weight*truePUWeight*btagWeight*leptnpweight*lepvetoweight*qcdRespTailWeight");
  //plots->setHeaderText("#sqrt{s} = 13 TeV",TString::Format("%4.2f fb^{-1}",stof(string(lumistr.Data()))),"");
  plots->setHeaderText("#sqrt{s} = 13 TeV","2.3 fb^{-1}","");
  plots->setHeaderPosition(0.16, 0.93);
  plots->setVLine(kGray+2,kDashed);

  map<TString,TString> sel;
  sel["trig"]       = "passjson && passdijetmet && passcscbeamhaloflt && passeebadscflt && passeebadsc4flt && passhbheisoflt && passhbhefltloose";
  sel["passvetoes"] = " && (nvetotau==0 || (ismc && npromptgentau>0)) && nvetolep==0";
  sel["met"]        = " && met>250";
  sel["njets75"]    = " && j2pt>75";
  sel["njets"]      = " && njets >= 5";
  sel["nlbjets"]    = " && nlbjets >= 2";
  sel["nbjets"]     = " && nbjets >= 1";
  sel["dphij12met"] = " && dphij12met>0.5";
  sel["dphij3met"]  = " && dphij3met>0.5";
  sel["dphij4met"]  = " && dphij4met>0.5";
  sel["dphij34met"] = " && dphij3met>0.5 && dphij4met>0.5";
  sel["dphij123"]   = " && dphij12met>1 && dphij3met>0.5 && dphij4met>0.5";
  sel["baseline"]   = sel["trig"] + sel["passvetoes"] + sel["met"] + sel["njets75"] + sel["njets"] + sel["nlbjets"] + sel["nbjets"] + sel["dphij12met"] + sel["dphij34met"];

  if(plotlog) {
    // logscale (scale to same NX as below)
    plots->addTreeVar("dphij12met_reducedbaseline","dphij12met",               sel["trig"] + sel["passvetoes"] + sel["met"] + sel["njets75"] + sel["njets"] + sel["nlbjets"] + sel["nbjets"]                     + sel["dphij34met"], "min(#Delta#phi(j_{1},#slash{E}_{T}),#Delta#phi(j_{2},#slash{E}_{T}))", 63, 0.0, 3.15, 0,0,0, 0.5);
  }
  else {
    // scale to NX (currently 10)
    plots->addTreeVar("njets75_reducedbaseline",   "njets75",                  sel["trig"] + sel["passvetoes"] + sel["met"]                  + sel["njets"] + sel["nlbjets"] + sel["nbjets"] + sel["dphij12met"] + sel["dphij34met"], "N_{J} (p_{T} > 75 GeV)", 11, -0.5, 10.5, 0,0,0, 1.5);
    plots->addTreeVar("njets_reducedbaseline",     "njets",                    sel["trig"] + sel["passvetoes"] + sel["met"] + sel["njets75"]                + sel["nlbjets"] + sel["nbjets"] + sel["dphij12met"] + sel["dphij34met"], "N_{J}", 11, -0.5, 10.5, 0,0,0, 4.5);
    plots->addTreeVar("nlbjets_reducedbaseline",   "nlbjets",                  sel["trig"] + sel["passvetoes"] + sel["met"] + sel["njets75"] + sel["njets"]                  + sel["nbjets"] + sel["dphij12met"] + sel["dphij34met"], "N_{bl}", 6, -0.5, 5.5, 0,0,0, 1.5);
    plots->addTreeVar("nbjets_reducedbaseline",    "nbjets",                   sel["trig"] + sel["passvetoes"] + sel["met"] + sel["njets75"] + sel["njets"] + sel["nlbjets"]                 + sel["dphij12met"] + sel["dphij34met"], "N_{b}", 6, -0.5, 5.5, 0,0,0, 0.5);
    plots->addTreeVar("dphij34met_reducedbaseline","min(dphij3met,dphij4met)", sel["trig"] + sel["passvetoes"] + sel["met"] + sel["njets75"] + sel["njets"] + sel["nlbjets"] + sel["nbjets"] + sel["dphij12met"]                    , "min(#Delta#phi(j_{3},#slash{E}_{T}),#Delta#phi(j_{4},#slash{E}_{T}))", 21, 0.0, 3.15, 0,0,0, 0.5);
    plots->addTreeVar("met_reducedbaseline",       "met",                      sel["trig"] + sel["passvetoes"]              + sel["njets75"] + sel["njets"] + sel["nlbjets"] + sel["nbjets"] + sel["dphij12met"] + sel["dphij34met"], "#slash{E}_{T} [GeV]", 50, 250.0, 1000.0, 0,0,0);
    plots->addTreeVar("mtb_baseline",              "mtcsv12met",               sel["baseline"], "M_{T}(b_{1,2}, #slash{E}_{T}) [GeV]", 20, 0.0, 500.0);

    // // scale to 5X
    // plots->addTreeVar("nctt_baseline_highmtb",  "ncttstd",    sel["baseline"] + " && mtcsv12met > 175" , "N_{t}", 4, -0.5, 3.5);
    //
    // // scale to MC
    // plots->addTreeVar("nbjets_baseline_lowmtb", "nbjets",     sel["baseline"] + " && mtcsv12met < 175" , "N_{b}",  6, -0.5,  5.5);
    // plots->addTreeVar("njets_baseline_lowmtb" , "njets" ,     sel["baseline"] + " && mtcsv12met < 175" , "N_{J}", 11,  3.5, 14.5);
    // plots->addTreeVar("nbjets_baseline_highmtb", "nbjets",     sel["baseline"] + " && mtcsv12met >= 175" , "N_{b}",  6, -0.5,  5.5);
    // plots->addTreeVar("njets_baseline_highmtb" , "njets" ,     sel["baseline"] + " && mtcsv12met >= 175" , "N_{J}", 11,  3.5, 14.5);

    // removed from AN
    //plots->addTreeVar("met_baseline_lowmtb",    "met",        sel["baseline"] + " && mtcsv12met <= 175", "#slash{E}_{T} [GeV]", 50, 250.0, 1000.0);
    //plots->addTreeVar("met_baseline_highmtb",   "met",        sel["baseline"] + " && mtcsv12met > 175" , "#slash{E}_{T} [GeV]", 50, 250.0, 1000.0);
    //plots->addTreeVar("nctt_baseline_lowmtb",   "ncttstd",    sel["baseline"] + " && mtcsv12met <= 175", "N_{t}", 4, -0.5, 3.5);
    //plots->addTreeVar("nbjets_baseline_nctt0",   "nbjets",     sel["baseline"] + " && ncttstd==0",        "Number of b-Tagged Jets", 6, -0.5, 5.5);
    //plots->addTreeVar("nbjets_baseline_nctt1",   "nbjets",     sel["baseline"] + " && ncttstd>0",         "Number of b-Tagged Jets", 6, -0.5, 5.5);
    //plots->addTreeVar("nbjets_baseline_highmtb_nctt0",   "nbjets",     sel["baseline"] + " && mtcsv12met > 175 && ncttstd==0",        "Number of b-Tagged Jets", 6, -0.5, 5.5);
    //plots->addTreeVar("nbjets_baseline_highmtb_nctt1",   "nbjets",     sel["baseline"] + " && mtcsv12met > 175 && ncttstd>0",         "Number of b-Tagged Jets", 6, -0.5, 5.5);
    //plots->addTreeVar("nbjets_baseline_lowmtb_nctt0",    "nbjets",     sel["baseline"] + " && mtcsv12met <= 175 && ncttstd==0",       "Number of b-Tagged Jets", 6, -0.5, 5.5);
    //plots->addTreeVar("nbjets_baseline_lowmtb_nctt1",    "nbjets",     sel["baseline"] + " && mtcsv12met <= 175 && ncttstd>0",        "Number of b-Tagged Jets", 6, -0.5, 5.5);
  }

  plots->plot(); 

  delete plots;

}
