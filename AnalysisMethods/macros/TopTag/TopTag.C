/*
 * TopTagEff.C
 * Adapted from hqu, June 2016
 * Synced with independent eff/mistag calculation (toptageff.C used by Alex and Loukas)
 */

#include <fstream>
//#include "CommonParameters.hh"
#include "TopTagParameters.hh"

using namespace EstTools;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void plotSdEff(){
  auto config = sdEffConfig();

  BaseEstimator z(config.outputdir);
  z.setConfig(config);

  //BinInfo num("sdtoppasspt","p_{T}", "GeV");
  BinInfo num("ak8toppasspt","p_{T}", "GeV");  
  num.setBinning(vector<int>{400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900, 950, 1000});
  BinInfo denom = num;
  //denom.var = "sdtopcandpt";
  denom.var = "ak8candpt";

  vector<TString> mc = {"qcd-unsmeared","ttbar", "wjets", "other", "znunu"};

  TString data = "singlelep";

  z.plotDataMC(mc,data,false);

  z.plotEfficiencyComp(num, denom, {"mc-unsmeared", data});
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void plotSdMistag(){
  auto config = sdMistagConfig();

  BaseEstimator z(config.outputdir);
  z.setConfig(config);

  //BinInfo num("sdtoppassptnolep","p_{T}", "GeV");
  BinInfo num("ak8toppasspt","p_{T}", "GeV");
  num.setBinning(vector<int>{400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900, 950, 1000});
  BinInfo denom = num;
  //denom.var = "sdtopcandptnolep";
  denom.var = "ak8candpt";

  vector<TString> mc = {"qcd-unsmeared", "ttbar", "wjets", "other", "znunu"};

  TString data = "jetht";

  z.plotDataMC(mc,data,false);

  z.plotEfficiencyComp(num, denom, {"mc-unsmeared", data}); 
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void plotCttEff(){
  auto config = cttEffConfig();

  BaseEstimator z(config.outputdir);
  z.setConfig(config);

  BinInfo num("sfcttpasspt","p_{T}", "GeV");
  num.setBinning(vector<int>{400, 500, 600, 700, 800, 1000});
  BinInfo denom = num;
  denom.var = "sfcttcandpt";

  // to plot wrt met, plotDataMC(met,mc,false)
  //BinInfo met("met", "#slash{E}_{T}", "GeV");
  //met.setBinning<int>(20, 0, 1000);

  vector<TString> mc = {"ttbar", "wjets", "qcd", "other"};
  TString data = "singlemu";

  // mu separated. consider using only muon dataset.
  z.setSelection("fabs(leptonpdgid)==13 && passtrigmu", "muon", "#mu"); // agreed on this one june 2016
  //z.setSelection("fabs(leptonpdgid)==13", "muon", "#mu"); 
  //z.setSelection("fabs(leptonpdgid)==13 && passtrigmu && !passtrige", "muon", "#mu");
  z.plotEfficiencyComp(num, denom, {"mc-all", data});
  z.plotDataMC(mc,data,false);

}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void plotCttMistag(){
  auto config = cttMistagConfig();

  BaseEstimator z(config.outputdir);
  z.setConfig(config);

  BinInfo num("sfcttpasspt","p_{T}", "GeV");
  num.setBinning(vector<int>{400, 500, 600, 700, 800, 1000});
  BinInfo denom = num;
  denom.var = "sfcttcandpt";

  vector<TString> mc = {"ttbar", "wjets", "qcd-smeared"};
  TString data = "jetht";

  z.plotDataMC(mc,data,false);

  z.plotEfficiencyComp(num, denom, {"mc-smeared", data}); 
}
