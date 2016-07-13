#ifndef ESTTOOLS_COMMONPARAMETERS_HH_
#define ESTTOOLS_COMMONPARAMETERS_HH_

#include "AnalysisMethods/EstTools/utils/Estimator.hh"

namespace EstTools{

// top tag strings
const TString effsel = "leptonpdgid==13 && passtrigmu && njets>=2 && nbjets>=1 && met>50 && leptonpt>30 && sfclose2lep && nvetolep==1";
const TString fakesel = "nvetolep==0 && passtright800 && njets>=2 && ht>1000 && met>200";
const TString datasel = " && passjson && passmetfilters";

const TString lumistr = "6.26";
const TString mcwgt = lumistr + "*weight";
//const TString wgtvar = lumistr+"*weight*truePUWeight*btagWeight";
const TString wgtvar = lumistr + "*weight*truePUWeight";

map<TString, Category> cttEffCatMap{ // keep extra cut as 1==1 on cands to plot sfcttcandpt below 400 GeV
  {"num", Category("num", "sfcttpasspt > 400", "Pass", BinInfo("sfcttpasspt", "Pass p_{T}", vector<int>{400, 500, 600, 700, 800, 900, 1000}, "GeV"))},
  {"denom", Category("denom", "1==1",          "Cand", BinInfo("sfcttcandpt", "Cand p_{T}", vector<int>{400, 500, 600, 700, 800, 900, 1000}, "GeV"))}
};

map<TString, Category> cttMistagCatMap{
  {"num", Category("num", "sfcttpasspt > 400", "Pass", BinInfo("sfcttpasspt", "Pass p_{T}", vector<int>{400, 500, 600, 700, 800, 1000}, "GeV"))},
  {"denom", Category("denom", "1==1",          "Cand", BinInfo("sfcttcandpt", "Cand p_{T}", vector<int>{200,300,400, 500, 600, 700, 800, 1000}, "GeV"))}
};

map<TString, Category> sdEffCatMap{ // keep extra cut as 1==1 on cands to plot sfcttcandpt below 400 GeV
  {"num", Category("num", "sdtoppasspt > 400", "Pass", BinInfo("sdtoppasspt", "Pass p_{T}", vector<int>{400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900, 950, 1000}, "GeV"))},
  {"denom", Category("denom", "1==1",          "Cand", BinInfo("sdtopcandpt", "Cand p_{T}", vector<int>{400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900, 950, 1000}, "GeV"))}
};

map<TString, Category> sdMistagCatMap{
  {"num", Category("num", "sdtoppassptnolep > 400", "Pass", BinInfo("sdtoppassptnolep", "Pass p_{T}", vector<int>{400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900, 950, 1000}, "GeV"))},
  {"denom", Category("denom", "1==1",          "Cand", BinInfo("sdtopcandptnolep", "Cand p_{T}", vector<int>{400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900, 950, 1000}, "GeV"))}
};


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
BaseConfig sdEffConfig(){
  BaseConfig     config;

  config.inputdir  = "/eos/uscms/store/user/apatters/trees/12072016-soups/lepcr-6fb";
  config.outputdir = "./plots/sdEffPlots";
  config.plotFormat = "svg";

  config.sel = effsel;

  config.addSample("singlelep",     "Data",                   "singlelep",            "1.0",  datasel);

  config.addSample("gjets",          "#gamma+jets",      "gjets",         wgtvar, datasel);
  config.addSample("ttbar",          "t#bar{t}",         "ttbar-mg",      wgtvar, datasel);
  config.addSample("tW",             "tW",               "tW",            wgtvar, datasel);
  config.addSample("ttW",            "ttW",              "ttW",           wgtvar, datasel);
  config.addSample("ttZ",            "ttZ",              "ttZ",           wgtvar, datasel);
  config.addSample("wjets",          "W+jets",           "wjets-ht",      wgtvar, datasel);
  config.addSample("qcd-unsmeared",  "QCD",              "qcd-unsmeared", wgtvar, datasel);
  config.addSample("mc-unsmeared",   "MC",               "mc-unsmeared",  wgtvar, datasel);
  config.addSample("znunu",          "znunu",            "znunu",         wgtvar, datasel);
  config.addSample("other",          "Gamma/tW/ttW/ttZ", "other",         wgtvar, datasel);

  config.catMaps = sdEffCatMap;
  for (auto &c : sdEffCatMap) config.categories.push_back(c.first);
  return config;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
BaseConfig sdMistagConfig(){
  BaseConfig     config;

  config.inputdir  = "/eos/uscms/store/user/apatters/trees/12072016-soups/htcr-6fb/";
  config.outputdir = "./plots/sdMistagPlots";
  config.plotFormat = "svg";

  config.sel = fakesel + " && nbjets==0";


  config.addSample("jetht",     "Data",                   "jetht",            "1.0",  datasel);

  config.addSample("gjets",          "#gamma+jets",      "gjets",         wgtvar, datasel);
  config.addSample("ttbar",          "t#bar{t}",         "ttbar-mg",      wgtvar, datasel);
  config.addSample("tW",             "tW",               "tW",            wgtvar, datasel);
  config.addSample("ttW",            "ttW",              "ttW",           wgtvar, datasel);
  config.addSample("ttZ",            "ttZ",              "ttZ",           wgtvar, datasel);
  config.addSample("wjets",          "W+jets",           "wjets-ht",      wgtvar, datasel);
  config.addSample("qcd-unsmeared",  "QCD",              "qcd-unsmeared", wgtvar, datasel);
  config.addSample("mc-unsmeared",   "MC",               "mc-unsmeared",  wgtvar, datasel);
  config.addSample("znunu",          "znunu",            "znunu",         wgtvar, datasel);
  config.addSample("other",          "Gamma/tW/ttW/ttZ", "other",         wgtvar, datasel);

  config.catMaps = sdMistagCatMap;
  for (auto &c : sdMistagCatMap) config.categories.push_back(c.first);
  return config;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
BaseConfig cttEffConfig(){
  BaseConfig     config;

  config.inputdir  = "/eos/uscms/store/user/apatters/trees/11072016-cttVars/6fb-lepcr-notaddedback-met50/";
  config.outputdir = "./plots/cttEffPlots";
  config.plotFormat = "pdf";
  config.header = "#sqrt{s} = 13 TeV, "+lumistr+" fb^{-1}";

  config.sel = effsel;

#define EFF_USE_POWHEG // comment out to use madgraph

#ifdef EFF_USE_POWHEG
  config.addSample("ttbar",     "t#bar{t}",               "ttbar-powheg",         wgtvar, datasel);
  config.addSample("wjets",     "W+jets",                 "wjets-ht",         wgtvar, datasel);
  config.addSample("other",     "Other",                  "other",            wgtvar, datasel);
  config.addSample("qcd",       "QCD",                    "qcd",              wgtvar, datasel);
  config.addSample("mc-all",    "MC",                     "mc-all-powheg",           wgtvar, datasel);
  config.addSample("singlemu", "Data",                   "singlemu",        "1.0",  datasel);
  config.addSample("singleel", "Data",                   "singleel",        "1.0",  datasel);
  config.addSample("singlelep", "Data",                   "singlelep",        "1.0",  datasel);
#else
  config.addSample("ttbar",     "t#bar{t}",               "ttbar-mg",         wgtvar, datasel);
  config.addSample("wjets",     "W+jets",                 "wjets-ht",         wgtvar, datasel);
  config.addSample("other",     "Other",                  "other",            wgtvar, datasel);
  config.addSample("qcd",       "QCD",                    "qcd",              wgtvar, datasel);
  config.addSample("mc-all",    "MC",                     "mc-all",           wgtvar, datasel);
  config.addSample("singlemu", "Data",                   "singlemu",        "1.0",  datasel);
  config.addSample("singleel", "Data",                   "singleel",        "1.0",  datasel);
  config.addSample("singlelep", "Data",                   "singlelep",        "1.0",  datasel);
#endif

  /* // mcwgtonly
  config.addSample("ttbar",     "t#bar{t}",               "ttbar-mg",         mcwgt, datasel);
  config.addSample("wjets",     "W+jets",                 "wjets-ht",         mcwgt, datasel);
  config.addSample("other",     "Other",                  "other",            mcwgt, datasel);
  config.addSample("qcd",       "QCD",                    "qcd",              mcwgt, datasel);
  config.addSample("mc-all",    "MC",                     "mc-all",           mcwgt, datasel);
  config.addSample("singlemu", "Data",                   "singlemu",        "1.0",  datasel);
  config.addSample("singleel", "Data",                   "singleel",        "1.0",  datasel);
  config.addSample("singlelep", "Data",                   "singlelep",        "1.0",  datasel);
  */

  config.catMaps = cttEffCatMap;
  for (auto &c : cttEffCatMap) config.categories.push_back(c.first);

  return config;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
BaseConfig cttMistagConfig(){
  BaseConfig     config;

  config.inputdir  = "../run/ntuples/HPTT/22062016-qcdcr/";
  config.outputdir = "./plots/cttMistagPlots";
  config.plotFormat = "svg";
  config.header = "#sqrt{s} = 13 TeV, "+lumistr+" fb^{-1}";

//#define MISTAG_USE_POWHEG // comment out to use madgraph
  config.sel = fakesel;

  config.addSample("jetht",     "Data",                   "jetht",            "1.0",  datasel);
  config.addSample("ttbar",     "t#bar{t}",               "ttbar-mg",         wgtvar, datasel);
  config.addSample("wjets",     "W+jets",                 "wjets-ht",         wgtvar, datasel);
  config.addSample("qcd-raw",       "QCD",                    "qcd-raw",              wgtvar, datasel);
  config.addSample("qcd-smeared",       "QCD",                    "qcd-smeared",              wgtvar, datasel);
//  config.addSample("other",     "Other",                  "other",            wgtvar, datasel);
  config.addSample("mc-raw",    "MC",                     "mc-raw",           wgtvar, datasel);
  config.addSample("mc-smeared",    "MC",                     "mc-smeared",           wgtvar, datasel);


//  config.addSample("mc-all",    "MC",                     "mc-raw",           wgtvar, datasel);
//  config.addSample("mc-smeared",    "MC-smeared",                     "mc-smeared",           wgtvar, datasel);
//  config.addSample("qcd",    "QCD",                     "qcd-raw",           wgtvar, datasel);
//  config.addSample("qcd-smeared",    "QCD-smeared",                     "qcd-smeared",           wgtvar, datasel);
// config.addSample("jetht",     "Data",                   "jetht",            "1.0",  datasel);

/*
#ifdef MISTAG_USE_POWHEG
  config.sel = fakesel;
  config.addSample("ttbar",     "t#bar{t}",               "ttbar-powheg",         wgtvar, datasel);
  config.addSample("wjets",     "W+jets",                 "wjets-ht",         wgtvar, datasel);
  config.addSample("other",     "Other",                  "other",            wgtvar, datasel);
  config.addSample("qcd",       "QCD",                    "qcd",              wgtvar, datasel);
  config.addSample("mc-all",    "MC",                     "mc-all-powheg",           wgtvar, datasel);
  config.addSample("jetht",     "Data",                   "jetht",            "1.0",  datasel);
#else
  config.sel = fakesel;
  config.addSample("ttbar",     "t#bar{t}",               "ttbar-mg",         wgtvar, datasel);
  config.addSample("wjets",     "W+jets",                 "wjets-ht",         wgtvar, datasel);
  config.addSample("other",     "Other",                  "other",            wgtvar, datasel);
  config.addSample("qcd",       "QCD",                    "qcd",              wgtvar, datasel);
  config.addSample("mc-all",    "MC",                     "mc-all-madgraph-smearedqcd",           wgtvar, datasel);
  config.addSample("jetht",     "Data",                   "jetht",            "1.0",  datasel);
#endif
*/

  config.catMaps = cttMistagCatMap;
  for (auto &c : cttMistagCatMap) config.categories.push_back(c.first);
  return config;
}

}//EstTools
#endif /* ESTTOOLS_COMMONPARAMETERS_HH_ */
