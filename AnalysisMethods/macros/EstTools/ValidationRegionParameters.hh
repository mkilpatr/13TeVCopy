#ifndef ESTTOOLS_COMMONPARAMETERS_HH_
#define ESTTOOLS_COMMONPARAMETERS_HH_

#include "Estimator.hh"

#define VALIDATION_REGION

namespace EstTools{

const TString lumistr = "2.317";
const TString wgtvar = lumistr+"*weight*truePUWeight*btagWeight";
//const TString lepvetowgt = wgtvar;
const TString lepvetowgt = wgtvar + "*leptnpweight*lepvetoweight";
const TString lepselwgt  = wgtvar + "*leptnpweight";
//const TString lepselwgt  = wgtvar;
const TString qcdwgt = wgtvar + "*qcdRespTailWeight";
const TString qcdvetowgt = lepvetowgt + "*qcdRespTailWeight";
const TString sigwgt = lepvetowgt + "*btagFastSimWeight";

//const TString trigSR = " && passmetmht90 && met>200 && mht>200";
const TString trigSR = " && passmetmht90 && met>200";
const TString trigLepCR = " && (passtrigel || passtrigmu)";
const TString trigPhoCR = " && passtrigphoton165 && origmet<200";
const TString trigDiLepCR = " && passTrig";

const TString baseline = "met>200 && met<250 && njets>=2 && njl>=1 && dphij1met>0.5 && dphij2met>0.15 && dphij3met>0.15 && metovsqrtht>10 && j1lpt>250 && dphij1lmet>2";
const TString baseNoDPhi = "met>200 && met<250 && njets>=2 && njl>=1 && metovsqrtht>10 && j1lpt>250 && dphij1lmet>2";
const TString dphi = " && dphij1met>0.5 && dphij2met>0.15 && dphij3met>0.15",
    dphi_invert = " && (dphij1met<0.1 || dphij2met<0.1 || dphij3met<0.1)";

//const TString datasel = " && passjson && passcscbeamhaloflt && passeebadscflt && passeebadsc4flt && passhbheisoflt && passhbhefltloose && passaddmetflts";
const TString datasel = " && passjson && passcscbeamhaloflt && passeebadscflt && passeebadsc4flt && passhbheisoflt && passhbhefltloose";
//const TString datasel = " && passjson";

const TString lepcrsel = " && nvetolep==1 && mtlepmet<100 && origmet>100";
const TString revert_vetoes = " && nvetolep>0 && mtlepmet<100";
//const TString vetoes = " && nvetolep==0 && nvetotau==0";
const TString vetoes = " && nvetolep==0 && (nvetotau==0 || (ismc && npromptgentau>0))";

//std::vector<TString> srbins{
//  "nb0_medboost_mednj", "nb0_medboost_highnj",
//  "nb0_highboost_mednj", "nb0_highboost_highnj",
//  "nb1_medboost_lowptb", "nb1_medboost_medptb", "nb1_medboost_highptb",
//  "nb1_highboost_lowptb", "nb1_highboost_medptb", "nb1_highboost_highptb"
//};

std::vector<TString> srbins{
  "nb0_medboost_mednj", "nb0_medboost_highnj",
//  "nb0_highboost_mednj",
  "nb1_medboost_lowptb", "nb1_medboost_medptb", "nb1_medboost_highptb"
//  "nb1_highboost_lowptb", "nb1_highboost_medptb", "nb1_highboost_highptb"
};

const TString mtb = "mtcsv12met<100",
    nb0 = "nbjets==0", nb1 = "nbjets>=1",
    medboost = "j1lpt>250 && j1lpt<500", highboost = "j1lpt>500",
    lowptb = "csvj1pt<40", medptb = "csvj1pt>40 && csvj1pt<70", highptb = "csvj1pt>70",
    mednj = "njets>=2 && njets<=5", highnj = "njets>=6";

std::map<TString, TString> normMap{
  {"nb0", nb0},
  {"nb1", nb1}
};

std::map<TString, TString> srcuts{
  {"nb0_medboost_mednj",    addCuts({nb0, medboost,  mednj})},
  {"nb0_medboost_highnj",   addCuts({nb0, medboost,  highnj})},
  {"nb0_highboost_mednj",   addCuts({nb0, highboost, mednj})},
  {"nb0_highboost_highnj",  addCuts({nb0, highboost, highnj})},

  {"nb1_medboost_lowptb",   addCuts({mtb, nb1, medboost,  lowptb}) },
  {"nb1_medboost_medptb",   addCuts({mtb, nb1, medboost,  medptb}) },
  {"nb1_medboost_highptb",  addCuts({mtb, nb1, medboost,  highptb}) },
  {"nb1_highboost_lowptb",  addCuts({mtb, nb1, highboost, lowptb}) },
  {"nb1_highboost_medptb",  addCuts({mtb, nb1, highboost, medptb}) },
  {"nb1_highboost_highptb", addCuts({mtb, nb1, highboost, highptb}) }
};

std::map<TString, std::vector<int>> srMETbins{
  {"nb0_medboost_mednj",    {200, 250}},
  {"nb0_medboost_highnj",   {200, 250}},
  {"nb0_highboost_mednj",   {200, 250}},
  {"nb0_highboost_highnj",  {200, 250}},

  {"nb1_medboost_lowptb",   {200, 250}},
  {"nb1_medboost_medptb",   {200, 250}},
  {"nb1_medboost_highptb",  {200, 250}},
  {"nb1_highboost_lowptb",  {200, 250}},
  {"nb1_highboost_medptb",  {200, 250}},
  {"nb1_highboost_highptb", {200, 250}}
};

const TString nlb0 = "nlbjets==0", nlb1 = "nlbjets>=1";

std::map<TString, TString> phoNormMap{
  {"nb0", nlb0},
  {"nb1", nlb1}
};

std::map<TString, TString> phocrCuts{
  {"nb0_medboost_mednj",    addCuts({nlb0, medboost,  mednj})},
  {"nb0_medboost_highnj",   addCuts({nlb0, medboost,  highnj})},
  {"nb0_highboost_mednj",   addCuts({nlb0, highboost, mednj})},
  {"nb0_highboost_highnj",  addCuts({nlb0, highboost, highnj})},

  {"nb1_medboost_lowptb",   addCuts({mtb, nlb1, medboost,  lowptb}) },
  {"nb1_medboost_medptb",   addCuts({mtb, nlb1, medboost,  medptb}) },
  {"nb1_medboost_highptb",  addCuts({mtb, nlb1, medboost,  highptb}) },
  {"nb1_highboost_lowptb",  addCuts({mtb, nlb1, highboost, lowptb}) },
  {"nb1_highboost_medptb",  addCuts({mtb, nlb1, highboost, medptb}) },
  {"nb1_highboost_highptb", addCuts({mtb, nlb1, highboost, highptb}) }
};

std::map<TString, std::vector<int>> phocrMETbins = srMETbins;

map<TString, Category> srCatMap(){
  map<TString, Category> cmap;
  for (auto &name : srbins){
    cmap[name] = Category(name, srcuts.at(name), name, BinInfo("met", "#slash{E}_{T}", srMETbins.at(name), "GeV"));
  }
  return cmap;
}

map<TString, Category> phoCatMap(){
  map<TString, Category> cmap;
  for (auto &name : srbins){
    cmap[name] = Category(name, phocrCuts.at(name), name, BinInfo("met", "#slash{E}_{T}", phocrMETbins.at(name), "GeV"));
  }
  return cmap;
}

map<TString, Category> lepCatMap(){
  return srCatMap();
}


map<TString, Category> zllCatMap{
  {"on-z",  Category("on-z",  "dilepmass > 80 && dilepmass < 100",                      "on Z",   BinInfo("met", "#slash{E}_{T}", vector<double>{200, 1000}, "GeV"))},
  {"off-z", Category("off-z", "dilepmass > 20 && (dilepmass < 80 || dilepmass > 100)",  "off Z",  BinInfo("met", "#slash{E}_{T}", vector<double>{200, 1000}, "GeV"))}
};


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
BaseConfig phoConfig(){
  BaseConfig     config;

  config.inputdir = "/tmp/trees";
  config.outputdir = "/tmp/plots/znunu";
  config.header = "#sqrt{s} = 13 TeV, "+lumistr+" fb^{-1}";

  config.addSample("singlepho",   "Data",           "photoncr/singlepho",  "1.0",  datasel + trigPhoCR);

  config.addSample("photon",      "Photon",         "photoncr/photon",     wgtvar, datasel + trigPhoCR);
  config.addSample("znunu",       "Z#rightarrow#nu#nu",   "sr/znunu",      wgtvar, datasel + trigSR + vetoes);

  config.sel = baseline;
  config.categories = srbins;
  config.catMaps = srCatMap();

  return config;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

BaseConfig zllConfig(){
  BaseConfig     config;

  config.inputdir = "/tmp/trees";
  config.outputdir = "/tmp/plots/znunu";
  config.header = "#sqrt{s} = 13 TeV, "+lumistr+" fb^{-1}";

  config.addSample("dyll",      "Z#rightarrowll+jets",    "zllcr/zll",                      lepselwgt, trigDiLepCR);
  config.addSample("ttbar",     "t#bar{t}",               "zllcr/ttbar",                    lepselwgt, trigDiLepCR);
  config.addSample("doublelep", "Data",                   "zllcr/doublelep",                "1.0",     trigDiLepCR);

  config.sel = baseline;
  config.catMaps = zllCatMap;
  for (auto &c : zllCatMap) config.categories.push_back(c.first);

  return config;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

bool ADD_LEP_TO_MET = true;

BaseConfig lepConfig(){
  BaseConfig     config;

  config.inputdir = "/tmp/trees";
  config.outputdir = "/tmp/plots/LLB";
  config.header = "#sqrt{s} = 13 TeV, "+lumistr+" fb^{-1}";

  if (ADD_LEP_TO_MET){
    config.addSample("singlelep",   "Data",          "lepcr/singlelep",       "1.0",     datasel + trigLepCR + lepcrsel);
    config.addSample("ttbar",       "t#bar{t}",      "lepcr/ttbar-mg",        lepselwgt, datasel + trigLepCR + lepcrsel);
    config.addSample("wjets",       "W+jets",        "lepcr/wjets-ht",        lepselwgt, datasel + trigLepCR + lepcrsel);
    config.addSample("tW",          "tW",            "lepcr/tW",              lepselwgt, datasel + trigLepCR + lepcrsel);
    config.addSample("ttW",         "ttW",           "lepcr/ttW",             lepselwgt, datasel + trigLepCR + lepcrsel);
    config.addSample("qcd",         "QCD",           "lepcr/qcd",             lepselwgt, datasel + trigLepCR + lepcrsel);
  }else{
    config.addSample("singlelep",   "Data",          "sr/met",             "1.0",     datasel + trigSR + revert_vetoes);
    config.addSample("ttbar",       "t#bar{t}",      "sr/ttbar-mg",        lepselwgt, datasel + trigSR + revert_vetoes);
    config.addSample("wjets",       "W+jets",        "sr/wjets-ht",        lepselwgt, datasel + trigSR + revert_vetoes);
    config.addSample("tW",          "tW",            "sr/tW",              lepselwgt, datasel + trigSR + revert_vetoes);
    config.addSample("ttW",         "ttW",           "sr/ttW",             lepselwgt, datasel + trigSR + revert_vetoes);
    config.addSample("qcd",         "QCD",           "sr/qcd",             lepselwgt, datasel + trigSR + revert_vetoes);
  }

  config.addSample("ttbar-sr",       "t#bar{t}",      "sr/ttbar-mg",        lepvetowgt, datasel + trigSR + vetoes);
  config.addSample("wjets-sr",       "W+jets",        "sr/wjets-ht",        lepvetowgt, datasel + trigSR + vetoes);
  config.addSample("tW-sr",          "tW",            "sr/tW",              lepvetowgt, datasel + trigSR + vetoes);
  config.addSample("ttW-sr",         "ttW",           "sr/ttW",             lepvetowgt, datasel + trigSR + vetoes);
  config.addSample("qcd-sr",         "QCD",           "sr/qcd",             lepvetowgt, datasel + trigSR + vetoes);

  config.sel = baseline;
  config.categories = srbins;
  config.catMaps = srCatMap();

  return config;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

BaseConfig qcdConfig(){
  BaseConfig     config;

  config.inputdir = "/tmp/trees";
  config.outputdir = "/tmp/plots/QCD";
  config.header = "#sqrt{s} = 13 TeV, "+lumistr+" fb^{-1}";

  // qcdcr
  config.addSample("data-cr",     "Data",          "sr/met",             "1.0",       datasel + trigSR + vetoes + dphi_invert);
  config.addSample("qcd-cr",      "QCD",           "qcdcr/qcd",          qcdwgt,      datasel + trigSR + dphi_invert);

  config.addSample("qcd-cr-withveto",  "QCD",      "qcdcr/qcd",          qcdvetowgt,  datasel + trigSR + vetoes + dphi_invert);
  config.addSample("qcd-sr-withveto",  "QCD",      "qcdcr/qcd",          qcdvetowgt,  datasel + trigSR + vetoes + dphi);

  // qcdcr: other bkg subtraction
  config.addSample("ttbar-cr",       "t#bar{t}",      "sr/ttbar-mg",     lepvetowgt,  datasel + trigSR + vetoes + dphi_invert);
  config.addSample("wjets-cr",       "W+jets",        "sr/wjets-ht",     lepvetowgt,  datasel + trigSR + vetoes + dphi_invert);
  config.addSample("tW-cr",          "tW",            "sr/tW",           lepvetowgt,  datasel + trigSR + vetoes + dphi_invert);
  config.addSample("ttW-cr",         "ttW",           "sr/ttW",          lepvetowgt,  datasel + trigSR + vetoes + dphi_invert);
  config.addSample("znunu-cr",       "Znunu",         "sr/znunu",        lepvetowgt,  datasel + trigSR + vetoes + dphi_invert);

  // onelepcr: norm correction for other bkg subtraction
  config.addSample("data-norm",      "Data",          "sr/met",          "1.0",       datasel + trigSR + dphi + revert_vetoes);
  config.addSample("ttbar-norm",     "t#bar{t}",      "sr/ttbar-mg",     lepselwgt,   datasel + trigSR + dphi + revert_vetoes);
  config.addSample("wjets-norm",     "W+jets",        "sr/wjets-ht",     lepselwgt,   datasel + trigSR + dphi + revert_vetoes);
  config.addSample("tW-norm",        "tW",            "sr/tW",           lepselwgt,   datasel + trigSR + dphi + revert_vetoes);
  config.addSample("ttW-norm",       "ttW",           "sr/ttW",          lepselwgt,   datasel + trigSR + dphi + revert_vetoes);
  config.addSample("znunu-norm",     "Znunu",         "sr/znunu",        lepselwgt,   datasel + trigSR + dphi + revert_vetoes);
  config.addSample("qcd-norm",       "QCD",           "qcdcr/qcd",       lepselwgt,   datasel + trigSR + dphi + revert_vetoes);

  // qcdsr
  config.addSample("qcd-sr",      "QCD",           "qcdcr/qcd",          qcdwgt,      datasel + trigSR + dphi);

  config.sel = baseNoDPhi;
  config.categories = srbins;
  config.catMaps = srCatMap();

  return config;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

BaseConfig sigConfig(){
  BaseConfig     config;

  config.inputdir = "/tmp/trees";
  config.outputdir = "/tmp/plots/sig";
  config.header = "#sqrt{s} = 13 TeV, "+lumistr+" fb^{-1}";

  config.addSample("data-sr",        "Data",             "sr/met",                    "1.0",  datasel + trigSR + vetoes + " && met<250");

  config.addSample("T2fbd_375_355",  "T2-4bd(375,355)",  "sr/signals/T2fbd_375_355",  sigwgt, datasel + trigSR + vetoes);
  config.addSample("T2fbd_375_325",  "T2-4bd(375,325)",  "sr/signals/T2fbd_375_325",  sigwgt, datasel + trigSR + vetoes);
  config.addSample("T2fbd_375_295",  "T2-4bd(375,295)",  "sr/signals/T2fbd_375_295",  sigwgt, datasel + trigSR + vetoes);

  config.sel = baseline;
  config.categories = srbins;
  config.catMaps = srCatMap();

  return config;
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

map<TString, BinInfo> varDict {
  {"norm",      BinInfo("met", "#slash{E}_{T}", vector<int>{0, 1000}, "GeV")},
  {"met",       BinInfo("met", "#slash{E}_{T}", vector<int>{250, 350, 450, 550, 650, 750, 1000}, "GeV")},
  {"met_1",     BinInfo("met/1", "#slash{E}_{T}", 16, 0, 800, "GeV")},
  {"origmet",   BinInfo("origmet", "Original #slash{E}_{T}", 16, 0, 800, "GeV")},
  {"njets",     BinInfo("njets", "N_{j}", 8, -0.5, 7.5)},
  {"njl",       BinInfo("njl", "N_{j}^{ISR}", 4, 0.5, 4.5)},
  {"nlbjets",   BinInfo("nlbjets", "N_{B}^{loose}", 5, -0.5, 4.5)},
  {"nbjets",    BinInfo("nbjets",  "N_{B}^{medium}", 5, -0.5, 4.5)},
  {"dphij1met", BinInfo("dphij1met", "#Delta#phi(j_{1},#slash{E}_{T})", 30, 0, 3)},
  {"dphij2met", BinInfo("dphij2met", "#Delta#phi(j_{2},#slash{E}_{T})", 30, 0, 3)},
  {"metovsqrtht",BinInfo("metovsqrtht", "#slash{E}_{T}/#sqrt{H_{T}}", 10, 0, 20)},
  {"dphij1lmet",BinInfo("dphij1lmet", "#Delta#phi(j_{1}^{ISR},#slash{E}_{T})", vector<double>{0, 2, 3})},
  {"dphij1lmet_fine",BinInfo("dphij1lmet", "#Delta#phi(j_{1}^{ISR},#slash{E}_{T})", 12, 0, 3)},
  {"mtcsv12met",BinInfo("mtcsv12met", "min(m_{T}(b_{1},#slash{E}_{T}),m_{T}(b_{2},#slash{E}_{T}))", 6, 0, 300)},
  {"leptonpt",  BinInfo("leptonpt", "p_{T}^{lep} [GeV]", 16, 0, 800)},
  {"leptonptovermet",  BinInfo("leptonpt/met", "p_{T}^{lep}/#slash{E}_{T}", 20, 0, 1.)},
  {"j1lpt",     BinInfo("j1lpt", "p_{T}(j_{1}^{ISR}) [GeV]", 10, 250, 750)},
  {"csvj1pt",   BinInfo("csvj1pt", "p_{T}(b_{1}) [GeV]", 8, 20, 100)},
  {"csvj1pt_1",   BinInfo("csvj1pt/1", "p_{T}(b_{1}) [GeV]", vector<int>{20, 40, 70, 100})}
};

}
#endif /* ESTTOOLS_COMMONPARAMETERS_HH_ */
