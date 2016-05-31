/*
 * Znunu.C
 *
 *  Created on: Sep 23, 2015
 *      Author: hqu
 */

#include <fstream>
#include "CommonParameters.hh"

using namespace EstTools;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void ZnunuPred(){ //+
  auto config = phoConfig();

  ZnunuEstimator z(config.outputdir);
  z.setConfig(config);

  z.zllcr_cfg = zllConfig();
  z.zll_normMap = normMap;
  z.phocr_categories = phoCatMap();
  z.phocr_normMap = phoNormMap;

  z.pred();
  z.printYields();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void Yields(){ //+
  auto config = phoConfig();
  config.samples.erase("znunu");
  config.catMaps = phoCatMap();

  BaseEstimator z(config.outputdir);
  z.setConfig(config);

  z.calcYields();
  z.printYields();

}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void plotSgamma(){ //+
  auto config = phoConfig();
  config.catMaps = phoCatMap();
  config.addSample("gjets",       "#gamma+jets",    "photoncr/gjets",      wgtvar, datasel + trigPhoCR);
  config.addSample("qcd-fake",    "Fake",           "photoncr/qcd-fake",   wgtvar, datasel + trigPhoCR);
  config.addSample("qcd-frag",    "Fragmentation",  "photoncr/qcd-frag",   wgtvar, datasel + trigPhoCR);

  BaseEstimator z(config.outputdir);
  z.setConfig(config);

  vector<TString> mc_samples = {"gjets", "qcd-fake", "qcd-frag"};
  TString data_sample = "singlepho";

  for (auto category : z.config.categories){
    const auto &cat = z.config.catMaps.at(category);
    TString norm_cut = cat.name.Contains("nb0") ? " && nlbjets==0" : " && nlbjets>=1";
    z.plotDataMC(cat.bin, mc_samples, data_sample, cat, true, z.config.sel + norm_cut);
  }

}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void plotRz(){ //+
  auto config = zllConfig();
//  config.addSample("ww",      "WW",    "zllcr/ww",                      lepselwgt, trigDiLepCR);
//  config.addSample("wz",      "WZ",    "zllcr/wz",                      lepselwgt, trigDiLepCR);
//  config.addSample("zz",      "ZZ",    "zllcr/zz",                      lepselwgt, trigDiLepCR);
//  config.sel = baseline + " && dphij1lmet>2";

  ZnunuEstimator z(config.outputdir);
  z.setConfig(config);
  z.zllcr_cfg = config;

  for (const auto &norm : normMap){
    cout << "\n---> " << norm.first << "\n";
    z.setSelection(norm.second, norm.first, "");
    z.plotDataMC({"dyll", "ttbar"}, "doublelep");
    z.calcRz(norm.second);
//    z.plotDataMC({"dyll", "ttbar", "ww", "wz", "zz"}, "doublelep");
  }

}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void ZvsGamma(){ //+

  auto config = phoConfig();
  config.categories.clear();
  config.catMaps.clear();
  config.categories.push_back("dummy");
  config.catMaps["dummy"] = Category::dummy_category();
  config.samples.clear();

  config.addSample("photon",          "#gamma+jets",      "photoncr/photon",     wgtvar, datasel + trigPhoCR);

  config.addSample("photon-nlb1",     "#gamma+jets N_{B}^{loose}#geq1",      "photoncr/photon",     wgtvar, datasel + trigPhoCR + " && nlbjets>=1");
  config.addSample("photon-nb1",      "#gamma+jets N_{B}#geq1",          "photoncr/photon",     wgtvar, datasel + trigPhoCR + " && nbjets>=1");
  config.addSample("znunu-nb1",       "Z#rightarrow#nu#nu N_{B}#geq1",   "sr/znunu",            wgtvar, datasel + trigSR + vetoes + " && nbjets>=1");

  config.addSample("photon-nlb0",     "#gamma+jets N_{B}^{loose}=0",         "photoncr/photon",     wgtvar, datasel + trigPhoCR + " && nlbjets==0");
  config.addSample("photon-nb0",      "#gamma+jets N_{B}=0",             "photoncr/photon",     wgtvar, datasel + trigPhoCR + " && nbjets==0");
  config.addSample("znunu-nb0",       "Z#rightarrow#nu#nu N_{B}=0",      "sr/znunu",            wgtvar, datasel + trigSR + vetoes + " && nbjets==0");

  BaseEstimator z(config.outputdir);
  z.setConfig(config);

  vector<TString> vars;

  vars = {"mtcsv12met", "j1lpt", "csvj1pt", "njets", "met"};
  for (auto v : vars){
    z.setPostfix("baseline_nb0");
    z.plotComp(varDict.at(v), {"znunu-nb0", "photon-nlb0", "photon-nb0"}, {"dummy"});
    z.setPostfix("baseline_nb1");
    z.plotComp(varDict.at(v), {"znunu-nb1", "photon-nlb1", "photon-nb1"}, {"dummy"});
//    z.setPostfix("baseline");
//    z.plotComp(varDict.at(v), {"photon", "znunu-nb1", "znunu-nb0"}, {"dummy"});
  }

  z.config.sel = addCuts({baseline,mtb});
  vars = {"j1lpt", "csvj1pt", "njets", "met"};
  for (auto v : vars){
//    z.setPostfix("srsel_nb0");
//    z.plotComp(varDict.at(v), {"znunu-nb0", "photon-nlb0", "photon-nb0"}, {"dummy"});
    z.setPostfix("srsel_nb1");
    z.plotComp(varDict.at(v), {"znunu-nb1", "photon-nlb1", "photon-nb1"}, {"dummy"});
//    z.setPostfix("srsel");
//    z.plotComp(varDict.at(v), {"photon", "znunu-nb1", "znunu-nb0"}, {"dummy"});
  }


  //  //met>250 && njets>=2 && njl>=1 && nlbjets>=1 && nbjets>=1 && dphij1met>0.5 && dphij2met>0.3 && metovsqrtht>10
//  z.config.sel = "met>250 && njets>=2 && njl>=1 && nlbjets>=1 && nbjets>=1";
//  z.setPostfix("presel");
//  vars = {"njl", "dphij1met", "dphij2met", "metovsqrtht"};
//  for (auto v : vars){
//    z.plotComp(varDict.at(v), {"photon", "znunu"}, {"dummy"});
//  }

}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void DoubleRatios(bool srsel = false){

  vector<TString> vars = {"dphij1lmet", "mtcsv12met", "j1lpt", "csvj1pt", "njets", "met"};
//  vector<TString> vars = {"j1lpt", "csvj1pt"};
  map<TString, vector<TH1*>> hists;

  {
    // photon
    auto config = phoConfig();
    config.catMaps = phoCatMap();
    config.addSample("gjets",       "#gamma+jets",    "photoncr/gjets",      wgtvar, datasel + trigPhoCR);
    config.addSample("qcd-fake",    "Fake",           "photoncr/qcd-fake",   wgtvar, datasel + trigPhoCR);
    config.addSample("qcd-frag",    "Fragmentation",  "photoncr/qcd-frag",   wgtvar, datasel + trigPhoCR);

    ZnunuEstimator z(config.outputdir);
    z.setConfig(config);

    for (auto &norm : phoNormMap){
      if (srsel) {
        z.config.sel = addCuts({baseline, norm.second, mtb});
        z.setPostfix("photon_"+norm.first+"_srsel");
      } else{
        z.config.sel = addCuts({baseline, norm.second});
        z.setPostfix("photon_"+norm.first+"_baseline");
      }

      for (auto var : vars){
        hists["photon_"+norm.first].push_back(z.plotDataMC(varDict.at(var), {"gjets", "qcd-fake", "qcd-frag"}, "singlepho", Category::dummy_category(), true));
      }
    }


  }

  {
    // z->ll

    auto config = zllConfig();
    ZnunuEstimator z(config.outputdir);
    z.setConfig(config);

    for (auto &norm : normMap){
      if (srsel) {
        z.config.sel = addCuts({baseline, zllCatMap["on-z"].cut, norm.second, mtb});
        z.setPostfix("z_"+norm.first+"_srsel");
      } else{
        z.config.sel = addCuts({baseline, zllCatMap["on-z"].cut, norm.second});
        z.setPostfix("z_"+norm.first+"_baseline");
      }

      for (auto var : vars){
        const auto &v = varDict.at(var);

        z.plotDataMC(v, {"dyll", "ttbar"}, "doublelep", Category::dummy_category(), true);

        auto hdata = z.getHistogram(v, "doublelep", Category::dummy_category());
        auto hdyll = z.getHistogram(v, "dyll", Category::dummy_category());
        auto httbar = z.getHistogram(v, "ttbar", Category::dummy_category());

        Quantity RT = (norm.first=="nb0") ? Quantity(1.81, 0.35) : Quantity(0.89, 0.10);
        for (int ib=0; ib<=httbar->GetNbinsX()+1; ++ib){
          auto q = RT * getHistBin(httbar, ib);
          setHistBin(httbar, ib, q);
        }
        hdata->Add(httbar, -1);

        double normFactor = hdata->Integral(0, hdata->GetNbinsX()+1)/hdyll->Integral(0, hdyll->GetNbinsX()+1);
        hdyll->Scale( normFactor );
        hdata->Divide(hdyll);

        hists["z_"+norm.first].push_back(hdata);
      }
    }
  }

  map<TString, TString> zlabels = {
      {"nb0", " N_{B}=0"}, {"nb1", " N_{B}#geq1"}
  };
  map<TString, TString> glabels = {
      {"nb0", " N_{B}^{loose}=0"}, {"nb1", " N_{B}^{loose}#geq1"}
  };
  for (auto &norm : normMap){

    for (unsigned i=0; i<vars.size(); ++i){
      auto h0 = hists["photon_"+norm.first].at(i);
      auto h1 = hists["z_"+norm.first].at(i);
      h0->SetYTitle("Data/Simulation");
      h1->SetYTitle("Data/Simulation");
      h0->SetLineColor(kViolet+2); h0->SetMarkerColor(kViolet+2);
      h1->SetLineColor(kRed); h1->SetMarkerColor(kRed);

      auto leg = prepLegends({h0, h1}, {"#gamma+jets"+glabels.at(norm.first), "Z#rightarrowll"+zlabels.at(norm.first)});
      auto c = drawCompAndRatio({h0, h1}, {makeRatioHists(h1, h0)}, leg, "#frac{Z#rightarrowll}{#gamma+jets}");

      c->SaveAs(phoConfig().outputdir+"/znunu_DR_cmp_"+vars.at(i)+(srsel?"_srsel":"_baseline")+"_"+norm.first+".pdf");

    }
  }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void shapeComp(){

  auto config = phoConfig();

  map<TString, Category> nbCatMap = {
      {"nb1",   Category("nb1",  "nbjets>=1",  "N_{B}#geq1",            varDict.at("met"))},
      {"nlb1",  Category("nlb1", "nlbjets>=1", "N_{B}^{loose}#geq1",    varDict.at("met"))},
      {"nb0",   Category("nb0",  "nbjets>=0",  "N_{B}#geq0",            varDict.at("met"))},
  };


  config.categories = {"nb1", "nlb1", "nb0"};
  config.catMaps = nbCatMap;

  BaseEstimator z(config.outputdir);
  z.setConfig(config);
  vector<TString> vars;

  z.config.sel = "met>250 && njets>=2 && njl>=1 && dphij1met>0.5 && dphij2met>0.3 && metovsqrtht>10 && j1lpt>250";
  z.setPostfix("baseline");
  vars = {"dphij1lmet", "mtcsv12met", "j1lpt", "csvj1pt", "met"};
  for (auto v : vars){
    z.plotComp(varDict.at(v), {"photon"}, config.categories, false);
  }

}

// ~~~~~~~~~~~~~~~~~~~
void predComp(){
  auto config = phoConfig();

  ZnunuEstimator z(config.outputdir);
  z.setConfig(config);
  z.zllcr_cfg = zllConfig();
  z.phocr_categories = config.catMaps;

  map<TString, vector<Quantity>> preds;

//  z.pred();
//  preds["nb0"] = z.yields.at("_pred");

  z.yields.clear();
  z.config.sel = baseline+" && nlbjets>=1";
  z.pred();
  preds["nlb1"] = z.yields.at("_pred");

  z.yields.clear();
  z.config.sel = baseline+" && nbjets>=1";
  z.phocr_categories = phoCatMap();
  z.pred();
  preds["nb1"] = z.yields.at("_pred");

  preds["mc"] = z.yields.at("znunu");

  map<TString, TString> labelMap = {
      {"nb0", "N_{B}#geq0"},
      {"nb1", "N_{B}#geq1"},
      {"nlb1", "N_{B}^{loose}#geq1"},
      {"mc", "Znunu Simulation"}
  };

  auto leg = initLegend();
  vector<TH1*> hists, hnum;
  TH1* hmc = nullptr;
  for (auto &p : preds){
    cout << p.first << p.second << endl;
    auto h = convertToHist(p.second, p.first, ";SR;");
    appendLegends(leg, {h}, {labelMap.at(p.first)});
    hists.push_back(h);
    if (p.first == "mc") hmc = h;
    else hnum.push_back(h);
  }
  prepHists(hists, false, false);

  vector<Color_t> colors = {kBlack,kBlue,kRed};
  for (unsigned i=0; i<hists.size(); ++i){
    auto h = hists.at(i);
    h->SetLineColor(colors.at(i));
    h->SetMarkerColor(colors.at(i));
  }


  auto c=drawCompAndRatio(hists, makeRatioHists(hnum, hmc), leg, "Prediction/Simulation");
  c->SaveAs("/tmp/plots/pred_cmp.pdf");

}
