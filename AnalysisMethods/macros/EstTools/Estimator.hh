#ifndef ESTTOOLS_ESTIMATOR_HH_
#define ESTTOOLS_ESTIMATOR_HH_

#if !defined(__CINT__) || defined(__MAKECINT__)

#include "AnalysisMethods/PlotUtils/interface/HistogramGetter.hh"
#include "EstHelper.hh"

using namespace std;
#endif

namespace EstTools{

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class IEstimator {
public:
  IEstimator() : fout_(nullptr) {}
  IEstimator(TString outputdir, TString outputfile):
    outputdir_(outputdir),
    outputfile_(outputfile),
    fout_(nullptr)
  {
    gSystem->mkdir(outputdir, true);
    SetStyle();
  }

  virtual ~IEstimator() {
    TH1::AddDirectory(kFALSE); // Detach the histograms from the file
    if (fout_) fout_->Close();
  }

  void setHeader(const TString& header) {
    header_ = header;
  }

  void setSaveHists(bool saveHists = true) {
    saveHists_ = saveHists;
  }

  void setPostfix(const TString& postfix) {
    postfix_ = postfix;
  }

  void setSelection(const TString& selection, const TString& postfix, const TString& header) {
    selection_ = selection;
    postfix_ = postfix;
    header_ = header;
  }

  void setSelection(const Category& cat){
    selection_ = cat.cut;
    postfix_ = cat.name;
    header_ = cat.label;
  }

  void setConfig(const BaseConfig& config) {
    this->config = config;
  }

public:
  void savePlot(TCanvas *c, TString fn){
    c->SaveAs(outputdir_+"/"+fn+"."+config.plotFormat);
  }

  void saveHist(TH1 *h, TString name = ""){
    if (!fout_)
      fout_ = new TFile(outputdir_ + "/" + outputfile_, "RECREATE");
    fout_->cd();
    h->Write(name, TObject::kOverwrite);
  }

public:
  BaseConfig config;

  TString outputdir_;
  TString outputfile_;
  TFile   *fout_;
  TString header_;
  TString postfix_;
  TString selection_;
  bool    saveHists_ = false;

};
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class BaseEstimator : public IEstimator {
public:
  BaseEstimator() {}
  BaseEstimator(TString outputdir, TString outputfile = "EstOutput.root") :
    IEstimator(outputdir, outputfile) {}
  BaseEstimator(const BaseConfig &config) :
    IEstimator(config.outputdir, "output.root") { setConfig(config); }

  virtual ~BaseEstimator() {}

  template<typename T>
  void printVec(const std::vector<T>& vec, const TString title="", bool printPercents = false) const{
    if (title!="")
      cout << title << endl;
    int ibin = 0;
    for (auto &cat_name : config.categories){
      const auto & cat = config.catMaps.at(cat_name);
      cout << setw(30) << cat.label << "\t ";
      for (auto &b : cat.bin.cuts){
        const auto &quantity = vec.at(ibin++);
        cout << fixed << setprecision(2) << setw(10) << quantity << "\t ";
        if (printPercents && std::is_same<T, Quantity>::value)
          cout << " (" << toString(quantity.error/quantity.value*100, 0, true) << "%) ";
      }
      cout << endl;
    }
    cout << endl;
  }

  void calcYields(vector<TString> excludes = {}){
    // calculate yields for all samples defined in config
    for (auto &p : config.samples){
      if ( std::find(excludes.begin(), excludes.end(), p.first) != excludes.end() ) continue;
      yields[p.first] = vector<Quantity>();
      auto &sample = p.second;
      for (auto &cat_name : config.categories){
        const auto & cat = config.catMaps.at(cat_name);
        auto cut = config.sel + " && " + cat.cut;
        auto v = getYieldVector(sample.tree, sample.wgtvar, cut + sample.sel, cat.bin);
        yields[p.first].insert(yields[p.first].end(), v.begin(), v.end());
      }
    }
  }

  void sumYields(vector<TString> list, TString sum_name){
    // sum yields from samples in the list, and store as "sum_name"
    assert(list.size() <= yields.size());
    yields[sum_name] = vector<Quantity>(config.nbins());
    for (auto &s : list){
      yields[sum_name] = yields[sum_name] + yields.at(s);
    }
  }

  void printYields() const{
    cout << "Yields" << endl;
    for (const auto &p : yields){
      const auto &vec = p.second;
      cout << p.first << ": " << fixed << setprecision(2) << Quantity::sum(vec) << endl;
      printVec(vec);
    }
  }

  void printYieldsTable(vector<TString> columns) const{
    // print yields in tabular format (friendly for pasting into spreadsheets)

    unsigned ibin = 0;

    cout << "Yields" << "\t";
    for (auto &c : columns){
      cout << c << "\t";
    }
    cout << endl;

    for (auto &cat_name : config.categories){
      const auto &cat = config.catMaps.at(cat_name);
      cout << cat.label << endl;
      for (auto &bin : cat.bin.plotnames){
        cout << bin << "\t";
        for (const auto &c : columns){
          cout << fixed << setprecision(2) << yields.at(c).at(ibin) << "\t ";
        }
        cout << endl;
        ++ibin;
      }
    }

  }

  TH1* getHistogram(const BinInfo& var_info, TString sample, const Category& category){
    // get a histogram of the given sample in the given category

    TString plotvar = var_info.var;
    TString title = ";"
        + var_info.label + (var_info.unit=="" ? "" : " ["+var_info.unit + "]") +";"
        + "Events";

    auto cut = config.sel + " && " + category.cut + TString(selection_=="" ? "" : " && "+selection_);

    auto samp = config.samples.at(sample);
    auto hname = filterString(plotvar) + "_" + sample + "_" + category.name + "_" + postfix_;
    auto hist = getHist(samp.tree, plotvar, samp.wgtvar, cut + samp.sel, hname, title, var_info.plotbins);
    prepHists({hist});
    if (saveHists_) saveHist(hist);

    return hist;
  }

  void plotComp(const BinInfo& var_info, const vector<TString> comp_samples, const vector<TString> comp_categories, bool comp_in_samples = true, bool isNormalized = true){
    // plot distribution in *var_info.var* for all given samples and categories
    // and compare them between either *samples* or *categories* (in the ratio plot)

    vector<TH1*> hists;
    vector<TH1*> ratioHists;
    auto leg = initLegend();

    TString plotvar = var_info.var;
    TString title = ";"
        + var_info.label + (var_info.unit=="" ? "" : " ["+var_info.unit + "]") +";"
        + (isNormalized ? "Normalized (to 1.) Events" : "Events");

    for (unsigned isamp=0; isamp<comp_samples.size(); ++isamp){
      TH1* hRef = nullptr;
      auto sname = comp_samples.at(isamp);
      auto sample = config.samples.at(sname);
      for (unsigned icat=0; icat<comp_categories.size(); ++icat){
        auto cat = config.catMaps.at(comp_categories.at(icat));
        auto hname = filterString(plotvar) + "_" + sname + "_" + cat.name + "_" + postfix_;
        auto cut = config.sel + sample.sel + " && " + cat.cut + TString(selection_=="" ? "" : " && "+selection_);
        auto htmp = getHist(sample.tree, plotvar, sample.wgtvar, cut, hname, title, var_info.plotbins);
        htmp->SetLineStyle(icat+1);
        prepHists({htmp}, isNormalized);
        if (saveHists_) saveHist(htmp);
        hists.push_back(htmp);

        // add legend
        addLegendEntry(leg, htmp, sample.label + " " + cat.label);

        // make ratio histograms : compare between categories
        if (!comp_in_samples){
          if (icat==0)
            hRef = htmp;
          else{
            auto rhist = makeRatioHists(htmp, hRef);
            ratioHists.push_back(rhist);
            if (saveHists_) saveHist(rhist);
          }
        }

      }
    }

    // make ratio histograms : compare between samples
    if (comp_in_samples){
      for (unsigned icat=0; icat<comp_categories.size(); ++icat){
        TH1* hRef = nullptr;
        for (unsigned isamp=0; isamp<comp_samples.size(); ++isamp){
          if (isamp==0)
            hRef = hists.at(icat);
          else{
            auto htmp = hists.at(isamp*comp_categories.size()+icat);
            auto rhist = makeRatioHists(htmp, hRef);
            ratioHists.push_back(rhist);
            if (saveHists_) saveHist(rhist);
          }
        }
      }
    }

    TString RYtitle = comp_in_samples ?
        "#frac{dN(" + (comp_samples.size()==2 ? config.samples.at(comp_samples.back()).label : "...") + ")}{dN(" + config.samples.at(comp_samples.front()).label +")}" :
        "#frac{dN(" + (comp_categories.size()==2 ? config.catMaps.at(comp_categories.back()).label : "...") + ")}{dN(" + config.catMaps.at(comp_categories.front()).label +")}";
    auto c = drawCompAndRatio(hists, ratioHists, leg, RYtitle);
    drawHeader(header_);
    savePlot(c, "comp_"+filterString(plotvar)+"_btw_"+(comp_in_samples?"samples":"categories")+"__"+postfix_);

  }

  TH1* plotDataMC(const BinInfo& var_info, const vector<TString> mc_samples, TString data_sample, const Category& category, bool norm_to_data = false, TString norm_cut = ""){
    // make DataMC plots with the given cateogory selection
    // possible to normalize MC to Data with a different selection (set by *norm_cut*)

    vector<TH1*> mchists;
    auto leg = initLegend();

    TString plotvar = var_info.var;
    TString title = ";"
        + var_info.label + (var_info.unit=="" ? "" : " ["+var_info.unit + "]") +";"
        + "Events";
    TString RYTitle = "N_{obs}/N_{exp}";

    auto cut = config.sel + " && " + category.cut + TString(selection_=="" ? "" : " && "+selection_);

    auto d_sample = config.samples.at(data_sample);
    auto hname = filterString(plotvar) + "_" + data_sample + "_" + category.name + "_" + postfix_;
    auto hdata = getHist(d_sample.tree, plotvar, d_sample.wgtvar, cut + d_sample.sel, hname, title, var_info.plotbins);
    prepHists({hdata});
    if (saveHists_) saveHist(hdata);
    addLegendEntry(leg, hdata, d_sample.label);

    for (auto &sname : mc_samples){
      auto sample = config.samples.at(sname);
      auto hname = filterString(plotvar) + "_" + sname + "_" + category.name + "_" + postfix_;
      auto hist = getHist(sample.tree, plotvar, sample.wgtvar, cut + sample.sel, hname, title, var_info.plotbins);
      prepHists({hist});
      if (saveHists_) saveHist(hist);
      mchists.push_back(hist);
      hist->SetFillColor(hist->GetLineColor()); hist->SetFillStyle(1001); hist->SetLineColor(kBlack);

      addLegendEntry(leg, hist, sample.label, "F");
    }

    if (norm_to_data){
      double sf = 1;
      // calc sf
      if (norm_cut==""){
        auto hsum = (TH1*)mchists.front()->Clone("hsum");
        for (unsigned i=1; i<mchists.size(); ++i){
          hsum->Add(mchists.at(i));
        }
        sf = hdata->Integral(1, hsum->GetNbinsX()+1) / hsum->Integral(1, hsum->GetNbinsX()+1);
      }else {
        auto data_inc = getYields(d_sample.tree, d_sample.wgtvar, norm_cut + d_sample.sel);
        vector<Quantity> mc_quantities;
        for (auto &sname : mc_samples){
          auto sample = config.samples.at(sname);
          mc_quantities.push_back(getYields(sample.tree, sample.wgtvar, norm_cut + sample.sel));
        }
        auto mc_inc = Quantity::sum(mc_quantities);
        sf = (data_inc/mc_inc).value;
      }
      // scale the mc hists
      for (auto *hist : mchists) hist->Scale(sf);
    }

    auto c = drawStackAndRatio(mchists, hdata, leg);
    savePlot(c, filterString(plotvar)+"_DataMC_"+category.name+"__"+postfix_);

    return makeRatioHists(hdata, sumHists(mchists, "bkgtotal"));
  }

  void plotDataMC(const vector<TString> mc_samples, TString data_sample, bool norm_to_data = false, TString norm_cut = ""){
    // make Data/MC plots for all categories
    for (auto category : config.categories){
      const auto &cat = config.catMaps.at(category);
      plotDataMC(cat.bin, mc_samples, data_sample, cat, norm_to_data, norm_cut);
    }
  }


public:
  map<TString, vector<Quantity>> yields;  // stores yields of all bins in all categories for each sample

};


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

class ZnunuEstimator : public BaseEstimator {
public:
  ZnunuEstimator() {}
  ZnunuEstimator(TString outputdir, TString outputfile = "znunu_output.root") :
    BaseEstimator(outputdir, outputfile) {}
  ZnunuEstimator(const BaseConfig &config) :
    BaseEstimator(config) {}

  virtual ~ZnunuEstimator() {}

  Quantity calcRz(const TString &extraCutForNorm = ""){
    cerr << "--->" << __func__ << endl;

    TString extra = extraCutForNorm=="" ? "" : " && " + extraCutForNorm;

    auto doublelep_sample = zllcr_cfg.samples.at("doublelep");
    auto dyll_sample      = zllcr_cfg.samples.at("dyll");
    auto ttbar_sample     = zllcr_cfg.samples.at("ttbar");

    auto cat = zllcr_cfg.catMaps.at("on-z");
    auto cut = zllcr_cfg.sel + extra + " && " + cat.cut;
    auto data_peak = getYields(doublelep_sample.tree, doublelep_sample.wgtvar, cut + doublelep_sample.sel);
    auto dy_peak   = getYields(dyll_sample.tree, dyll_sample.wgtvar, cut + dyll_sample.sel);
    auto tt_peak   = getYields(ttbar_sample.tree, ttbar_sample.wgtvar, cut + ttbar_sample.sel);

    cat = zllcr_cfg.catMaps.at("off-z");
    cut = zllcr_cfg.sel + extra + " && " + cat.cut;
    auto data_off = getYields(doublelep_sample.tree, doublelep_sample.wgtvar, cut + doublelep_sample.sel);
    auto dy_off   = getYields(dyll_sample.tree, dyll_sample.wgtvar, cut + dyll_sample.sel);
    auto tt_off   = getYields(ttbar_sample.tree, ttbar_sample.wgtvar, cut + ttbar_sample.sel);

    auto z_sf = dileptonZtoLLScaleFactorHelper(data_peak, dy_peak, tt_peak, data_off, dy_off, tt_off);
    auto t_sf = dileptonTTbarScaleFactorHelper(data_peak, dy_peak, tt_peak, data_off, dy_off, tt_off);

    cout << fixed << setprecision(2);
    cout << "[ZLL SF]   " << " = " << z_sf << " PEAK: ---> dilep: " << data_peak << ", ttbar: " << tt_peak << ", dy: " << dy_peak << endl;
    cout << "[TTbar SF] " << " = " << t_sf << " OFF:  ---> dilep: " << data_off  << ", ttbar: " << tt_off  << ", dy: " << dy_off  << endl;

    return z_sf;
  }

  vector<Quantity> calcSgamma(const Category& cat, TString extraCutForNorm = ""){

    cerr << "--->" << __func__ << " " << cat.name << endl;

    TString extra = extraCutForNorm=="" ? "" : " && " + extraCutForNorm;

    auto photon_sample = config.samples.at("photon");
    auto singlepho_sample = config.samples.at("singlepho");

    // norm factor
    auto mc_total = getYields(photon_sample.tree, photon_sample.wgtvar, config.sel + extra + photon_sample.sel);
    auto data_total = getYields(singlepho_sample.tree, singlepho_sample.wgtvar, config.sel + extra + singlepho_sample.sel);
    auto norm_factor = (data_total/mc_total).value;

    auto cut = config.sel + " && " + cat.cut;
    auto mc   = getYieldVector(photon_sample.tree,    photon_sample.wgtvar,    cut + photon_sample.sel,    cat.bin);
    auto data = getYieldVector(singlepho_sample.tree, singlepho_sample.wgtvar, cut + singlepho_sample.sel, cat.bin);

    auto s_gamma = data / (mc * norm_factor);
    cout << "    ... norm factor ---> " << norm_factor << endl;
    cout << "    ... sgamma ---> " << s_gamma << endl;
    return s_gamma;

  }

  void pred(){
    // Yields
    calcYields();

    // Sgamma
    yields["_Sgamma"] = vector<Quantity>();
    for (auto &cat_name : config.categories){
      // find norm category
      TString normCut;
      for (const auto &norm : phocr_normMap){
        if (cat_name.Contains(norm.first))
          normCut = norm.second;
      }
      const auto & cat = config.catMaps.at(cat_name);
      auto sgamma = calcSgamma(phocr_categories.at(cat_name), normCut);
      assert(sgamma.size()==1 || sgamma.size()==cat.bin.nbins); // FIXME: only handle size==1 and same size now
      for (unsigned ibin = 0; ibin < cat.bin.nbins; ++ibin){
        yields["_Sgamma"].push_back(sgamma.at( ibin%sgamma.size() ));
      }
    }

    // Rz
    yields["_Rz"] = vector<Quantity>();
    map<TString, Quantity> rzs;
    for (const auto &norm : zll_normMap){
      rzs[norm.first] = calcRz(norm.second);
    }

    for (auto &cat_name : config.categories){
      const auto & cat = config.catMaps.at(cat_name);
      for (unsigned ibin = 0; ibin < cat.bin.nbins; ++ibin){
        // find norm category
        for (const auto &norm : zll_normMap){
          if (cat_name.Contains(norm.first))
            yields["_Rz"].push_back(rzs.at(norm.first));
        }
      }
    }

    yields["_pred"] = yields.at("znunu") * yields.at("_Sgamma") * yields.at("_Rz");
    printVec(yields["_pred"], "Final prediction", true);

  }

  BaseConfig zllcr_cfg;
  std::map<TString, TString> zll_normMap;

  map<TString, Category> phocr_categories;
  std::map<TString, TString> phocr_normMap;

};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


class LLBEstimator : public BaseEstimator {
public:
  LLBEstimator() {}
  LLBEstimator(TString outputdir, TString outputfile = "LLB_output.root") :
    BaseEstimator(outputdir, outputfile) {}
  LLBEstimator(const BaseConfig &config) :
    BaseEstimator(config) {}

  virtual ~LLBEstimator() {}

  vector<Quantity> calcSLep(){
    // calculate correction factors from single lepton control regions

    cerr << "--->" << __func__ << endl;

    auto data = yields.at("singlelep") - yields.at("qcd");
    cout << "  ... data - qcd : " << data << endl;
    Quantity::removeNegatives(data);
    cout << "  ... data - qcd (zeroes removed): " << data << endl;
    auto mc   = yields.at("ttbarplusw");

    auto s_lep = data / mc;
    cout << "    ---> " << s_lep << endl;
    return s_lep;

  }

  void pred(){
    // Yields
    calcYields();
    sumYields({"ttbar", "wjets", "tW", "ttW"}, "ttbarplusw");
    sumYields({"ttbar-sr", "wjets-sr", "tW-sr", "ttW-sr"}, "ttbarplusw-sr");

    // SLep
//    yields["_SLep"] = vector<Quantity>();
//    for (auto &cat_name : config.categories){
//      const auto & cat = config.catMaps.at(cat_name);
//      auto slep = calcSLep(lepcr_categories.at(cat_name));
//      assert(slep.size()==1 || slep.size()==cat.bin.nbins); // FIXME: only handle size==1 and same size now
//      for (unsigned ibin = 0; ibin < cat.bin.nbins; ++ibin){
//        yields["_Sgamma"].push_back(slep.at( ibin%slep.size() ));
//      }
//    }

    yields["_SLep"] = calcSLep();

    yields["_pred"] = yields.at("ttbarplusw-sr") * yields.at("_SLep");
    printVec(yields["_pred"], "Final prediction", true);

  }

  map<TString, Category> lepcr_categories;  // category map for the lepton CR

};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


class QCDEstimator : public BaseEstimator {
public:
  QCDEstimator() {}
  QCDEstimator(TString outputdir, TString outputfile = "QCD_output.root") :
    BaseEstimator(outputdir, outputfile) {}
  QCDEstimator(const BaseConfig &config) :
    BaseEstimator(config) {}

  virtual ~QCDEstimator() {}

  void naiveTF(){
    yields["_NaiveTF"] = yields.at("qcd-sr-withveto") / yields.at("qcd-cr-withveto");
  }

  void calcTF(){
    // calculate transfer factors

    cerr << "--->" << __func__ << endl;

    std::vector<Quantity> tansfer_factors;

    auto qcd = config.samples.at("qcd-sr");
    auto qcdcr = config.samples.at("qcd-cr");

    for (auto &cat_name : config.categories){
      cerr << cat_name << endl;

      const auto & cat = config.catMaps.at(cat_name);
      auto SRsel = config.sel + " && " + cat.cut + qcd.sel;
      auto CRsel = config.sel + " && " + cat.cut + qcdcr.sel;

      cerr << "SRsel: " << SRsel << endl;
      cerr << "CRsel: " << CRsel << endl;

      auto metGetter = HistogramGetter(cat.bin.var, cat.bin.var, cat.bin.label, cat.bin.nbins, cat.bin.plotbins.data());
      metGetter.setNBS(50);
      auto nom = metGetter.getTFAndCov(qcd.tree, SRsel, qcd.wgtvar, CRsel, qcdcr.wgtvar, "TF_"+cat.name);

      for (int ibin=1; ibin<=nom->GetNbinsX(); ++ibin){
        auto tf = getHistBin(nom, ibin);
        if (tf.error>tf.value) tf.error = tf.value;
        tansfer_factors.push_back(tf);
      }
    }

    yields["_TF"] = tansfer_factors;

  }

  void calcDataCorr(){

    vector<TString> otherbkg_samples = {"ttbar-cr", "wjets-cr", "tW-cr", "ttW-cr", "znunu-cr"};
    vector<TString> norm_samples = {"ttbar-norm", "wjets-norm", "tW-norm", "ttW-norm", "znunu-norm", "qcd-norm"};
    calcYields(norm_samples);
    sumYields(otherbkg_samples, "non-qcd");
    yields["otherbkgs"] = yields.at("non-qcd");

    yields["_SubNormCorr"] = std::vector<Quantity>();
    unsigned ibin = 0;
    for (auto &cat_name : config.categories){
      const auto & cat = config.catMaps.at(cat_name);

      auto samp = config.samples.at("data-norm");
      auto norm_datayield = getYields(samp.tree, samp.wgtvar, config.sel + " && " + cat.cut + samp.sel);
      Quantity norm_bkgtotal(0, 0);
      for (auto &s : norm_samples){
        samp = config.samples.at(s);
        norm_bkgtotal = norm_bkgtotal + getYields(samp.tree, samp.wgtvar, config.sel + " && " + cat.cut + samp.sel);
      }
      Quantity norm_factor = norm_datayield / norm_bkgtotal;
      cerr << endl << "~~~" << cat_name << ": data(norm) = " << norm_datayield << ", total bkg (norm) = " << norm_bkgtotal << endl << endl;

      for (auto &c : cat.bin.cuts){
        yields.at("_SubNormCorr").push_back(norm_factor);
        yields.at("otherbkgs").at(ibin) = yields.at("otherbkgs").at(ibin) * norm_factor;
        ++ibin;
      }
    }

    auto vdata = yields.at("data-cr");
    Quantity::removeZeroes(vdata);

    yields["_DataCorr"] = std::vector<Quantity>();
    yields["_DATA"] = std::vector<Quantity>();
    for (unsigned i=0; i<vdata.size(); ++i){
      double otherVal = yields.at("otherbkgs").at(i).value;
      double dataVal = vdata.at(i).value;
      if (dataVal<10) dataVal = yields.at("qcd-cr-withveto").at(i).value + otherVal;
      double sub = otherVal/dataVal;
      Quantity corr(1-sub, sub*(1-sub)); // 100% unc on the subtraction: FIXME?
      yields.at("_DataCorr").push_back(corr);
      yields.at("_DATA").push_back(vdata.at(i) * corr);
    }

  }

  void pred(){

    calcTF();
    calcDataCorr();

    yields["_pred"] = yields.at("_DATA") * yields.at("_TF");
    printVec(yields["_pred"], "QCD prediction", true);

  }


};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


}

#endif /*ESTTOOLS_ESTIMATOR_HH_*/
