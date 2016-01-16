#if !defined(__CINT__) || defined(__MAKECINT__)
#include "PredictionHelper.hh"
#endif

using namespace BkgPrediction;

void getZeroLeptonPrediction(const TString defaultdir  = "/eos/uscms/store/user/vdutta/13TeV/trees/121815",
                             const TString outputdir   = "plots_bkgest_011416/lownj",
                             const TString var         = "",
                             const TString srconf      = "plotting/run0lepbkgpred.conf",
                             const TString phocrconf   = "plotting/runphotoncrbkgpred.conf",
                             const TString zllcrconf   = "plotting/runzllcrbkgpred.conf",
                             const TString ttzcrconf   = "plotting/runttzcrbkgpred.conf",
                             const TString lumistr     = "2.137",
                             const TString region      = "sr",
                             //const TString region      = "srlownj",
                             const TString format      = "pdf",
                             const bool    dolownj     = true,
                             const bool    usettzsf    = false,
                             const unsigned int sysvar = NOMINAL,
                             const bool    plotlog     = false)
{
  TString ext = "";
  if(var != "" && sysvar != NOMINAL) ext = "/" + var + (sysvar == VARUP ? "up" : "down");

  gSystem->mkdir(outputdir+ext, true);

  TString basewgt    = lumistr + "*weight*truePUWeight*btagWeight";
  TString lepvetowgt = basewgt + "*leptnpweight*lepvetoweight";
  TString lepselwgt  = basewgt + "*leptnpweight";

  sel["trig"]         = "passjson && passdijetmet && j2pt>75 && met>250 && passcscbeamhaloflt && passeebadscflt && passeebadsc4flt && passhbheisoflt && passhbhefltloose";
  sel["trigpho"]      = "passjson && passtrigphoton165 && origmet<200 && j2pt>75 && met>250 && passcscbeamhaloflt && passeebadscflt && passeebadsc4flt && passhbheisoflt && passhbhefltloose";
  sel["trigzll"]      = "passjson && ((iselectron && passtrige17e12) || (!iselectron && (passtrigmu17mu8 || passtrigmu17tkmu8))) && j2pt>75 && met>100 && dilepmass > 80 && dilepmass < 100";
  sel["trigzlloff"]   = "passjson && ((iselectron && passtrige17e12) || (!iselectron && (passtrigmu17mu8 || passtrigmu17tkmu8))) && j2pt>75 && met>100 && dilepmass > 20 && (dilepmass < 80 || dilepmass > 100)";
  sel["vetoes"]       = " && ((nvetolep==0 && nvetotau==0) || (ismc && npromptgentau>0))";
  sel["lepsel"]       = " && nvetolep>0";
  sel["njets"]        = dolownj ? " && njets>=2 && njets<5 && nbjets>=1 && nlbjets>=2" : " && njets>=5 && nbjets>=1 && nlbjets>=2";
  sel["dphij123"]     = " && dphij12met>0.5 && dphij3met>0.5";
  sel["dphij3"]       = " && dphij3met>0.5";
  sel["dphij123inv"]  = " && dphij12met<0.15";
  sel["lepcrsel"]     = " && nvetolep>0 && mtlepmet<100";
  sel["sr"]           = sel["trig"] + sel["vetoes"]   + sel["njets"]    + sel["dphij123"];
  sel["srnovetoes"]   = sel["trig"] + sel["njets"]    + sel["dphij123"];
  sel["lepcr"]        = sel["trig"] + sel["lepcrsel"] + sel["njets"]    + sel["dphij123"];
  sel["qcdcr"]        = sel["trig"] + sel["vetoes"]   + sel["njets"]    + sel["dphij123inv"];
  sel["qcdcrnovetoes"]= sel["trig"] + sel["njets"]    + sel["dphij123inv"];
  sel["qcdincl"]      = sel["trig"] + sel["vetoes"]   + sel["njets"]    + sel["dphij3"];
  sel["phocr"]        = sel["trigpho"] + sel["njets"] + sel["dphij123"];
  sel["zllcr"]        = sel["trigzll"] + sel["njets"];
  sel["zlloffcr"]     = sel["trigzlloff"] + sel["njets"];
  sel["trigttz"]      = "passjson && passTrig && passMed && njets>=4 && minlepPT>15 && dileppt>100";
  sel["ttzSel"]       = sel["trigttz"] + " && nbjets>=1 && nlbjets>=2 && onZ";
  sel["ttzNoZ"]       = sel["trigttz"] + " && nbjets>=1 && nlbjets>=2 && (!onZ)";
  sel["ttzNoB"]       = sel["trigttz"] + " && nbjets==0 && onZ";
  sel["nt0"]          = " && ncttstd==0";
  sel["nt1"]          = " && ncttstd>0";
  sel["lowmt"]        = " && mtcsv12met<=175";
  sel["nt0_highmt"]   = " && ncttstd==0 && mtcsv12met>175";
  sel["nt1_highmt"]   = " && ncttstd>0 && mtcsv12met>175";
  sel["nb1"]          = " && nbjets==1";
  sel["nb2"]          = " && nbjets>=2";
  sel["nb1_lowmt"]    = " && nbjets==1 && mtcsv12met<=175";
  sel["nb2_lowmt"]    = " && nbjets>=2 && mtcsv12met<=175";
  sel["nb1_nt0_highmt"]   = " && nbjets==1 && ncttstd==0 && mtcsv12met>175";
  sel["nb2_nt0_highmt"]   = " && nbjets>=2 && ncttstd==0 && mtcsv12met>175";
  sel["nb1_nt1_highmt"]   = " && nbjets==1 && ncttstd>0 && mtcsv12met>175";
  sel["nb2_nt1_highmt"]   = " && nbjets>=2 && ncttstd>0 && mtcsv12met>175";

  TString inputdir    = defaultdir;

  PlotStuff* plots0l    = setupPlots(srconf,    inputdir+ext, outputdir+ext, lepvetowgt, plotlog, format, lumistr, "output_0l.root");
  PlotStuff* plots0lnovetoes = setupPlots(srconf,    inputdir+ext, outputdir+ext, basewgt, plotlog, format, lumistr, "output_0lnovetoes.root");
  PlotStuff* plotslepcr = setupPlots(srconf,    inputdir+ext, outputdir+ext, lepselwgt, plotlog, format, lumistr, "output_lepcr.root");
  PlotStuff* plotsphocr = setupPlots(phocrconf, inputdir+"/photoncr"+ext, outputdir+ext, basewgt, plotlog, format, lumistr, "output_phocr.root");
  PlotStuff* plotszllcr = setupPlots(zllcrconf, inputdir+"/zllcr"+ext, outputdir+ext, lepselwgt, plotlog, format, lumistr, "output_zllcr.root");
  //PlotStuff* plotsttzcr = setupPlots(ttzcrconf, inputdir+"/ttzcr", outputdir, basewgt, plotlog, format, lumistr, "output_ttzcr.root");

  cout << "Plotting 0lepton region" << endl;

  plots0l->addTreeVar("met_sr_nb1_lowmt",              "met",         sel["sr"] + sel["nb1_lowmt"],          "#slash{E}_{T} [GeV]", NBINS, metbins);
  plots0l->addTreeVar("met_sr_nb2_lowmt",              "met",         sel["sr"] + sel["nb2_lowmt"],          "#slash{E}_{T} [GeV]", NBINS, metbins);
  plots0l->addTreeVar("met_sr_nb1_nt0_highmt",         "met",         sel["sr"] + sel["nb1_nt0_highmt"],     "#slash{E}_{T} [GeV]", NBINS, metbins);
  plots0l->addTreeVar("met_sr_nb2_nt0_highmt",         "met",         sel["sr"] + sel["nb2_nt0_highmt"],     "#slash{E}_{T} [GeV]", NBINS, metbins);
  plots0l->addTreeVar("met_sr_nb1_nt1_highmt",         "met",         sel["sr"] + sel["nb1_nt1_highmt"],     "#slash{E}_{T} [GeV]", NBINS_NT1, metbins_nt1);
  plots0l->addTreeVar("met_sr_nb2_nt1_highmt",         "met",         sel["sr"] + sel["nb2_nt1_highmt"],     "#slash{E}_{T} [GeV]", NBINS_NT1, metbins_nt1);

  plots0l->addTreeVar("met_qcdcr_nbgeq1_lowmt",        "met",         sel["qcdcr"] + sel["lowmt"],           "#slash{E}_{T} [GeV]", NBINS, metbins);
  plots0l->addTreeVar("met_qcdcr_nbgeq1_nt0_highmt",   "met",         sel["qcdcr"] + sel["nt0_highmt"],      "#slash{E}_{T} [GeV]", NBINS, metbins);
  plots0l->addTreeVar("met_qcdcr_nbgeq1_nt1_highmt",   "met",         sel["qcdcr"] + sel["nt1_highmt"],      "#slash{E}_{T} [GeV]", NBINS_NT1, metbins_nt1);

  plots0l->plot();

  cout << "Plotting 0lepton region with no vetoes" << endl;

  plots0lnovetoes->addTreeVar("met_srnovetoes_nb1_lowmt",            "met",         sel["srnovetoes"] + sel["nb1_lowmt"],      "#slash{E}_{T} [GeV]", NBINS, metbins);
  plots0lnovetoes->addTreeVar("met_srnovetoes_nb2_lowmt",            "met",         sel["srnovetoes"] + sel["nb2_lowmt"],      "#slash{E}_{T} [GeV]", NBINS, metbins);
  plots0lnovetoes->addTreeVar("met_srnovetoes_nb1_nt0_highmt",       "met",         sel["srnovetoes"] + sel["nb1_nt0_highmt"], "#slash{E}_{T} [GeV]", NBINS, metbins);
  plots0lnovetoes->addTreeVar("met_srnovetoes_nb2_nt0_highmt",       "met",         sel["srnovetoes"] + sel["nb2_nt0_highmt"], "#slash{E}_{T} [GeV]", NBINS, metbins);
  plots0lnovetoes->addTreeVar("met_srnovetoes_nb1_nt1_highmt",       "met",         sel["srnovetoes"] + sel["nb1_nt1_highmt"], "#slash{E}_{T} [GeV]", NBINS_NT1, metbins_nt1);
  plots0lnovetoes->addTreeVar("met_srnovetoes_nb2_nt1_highmt",       "met",         sel["srnovetoes"] + sel["nb2_nt1_highmt"], "#slash{E}_{T} [GeV]", NBINS_NT1, metbins_nt1);

  plots0lnovetoes->addTreeVar("met_qcdcrnovetoes_nbgeq1_lowmt",      "met",         sel["qcdcrnovetoes"] + sel["lowmt"],       "#slash{E}_{T} [GeV]", NBINS, metbins);
  plots0lnovetoes->addTreeVar("met_qcdcrnovetoes_nbgeq1_nt0_highmt", "met",         sel["qcdcrnovetoes"] + sel["nt0_highmt"],  "#slash{E}_{T} [GeV]", NBINS, metbins);
  plots0lnovetoes->addTreeVar("met_qcdcrnovetoes_nbgeq1_nt1_highmt", "met",         sel["qcdcrnovetoes"] + sel["nt1_highmt"],  "#slash{E}_{T} [GeV]", NBINS_NT1, metbins_nt1);

  plots0lnovetoes->plot();

  if(!dolownj) {
    TString rmcmd = "rm " + outputdir + "/met_sr*." + format;
    gSystem->Exec(rmcmd.Data());
    TFile* file      = TFile::Open(plots0l->outfileName(),"UPDATE");
    TIter nextkey(gDirectory->GetListOfKeys());
    while(TKey *key = (TKey*)nextkey()) {
      TObject *obj = key->ReadObj();
      if(TString(obj->GetName()).Contains("_sr_") && TString(obj->GetName()).Contains("_data")) {
        TString rmstr(obj->GetName());
        rmstr += ";*";
        file->Delete(rmstr.Data());
      }
    }
    file->Write();
    file->Close();
    TFile* filenovetoes      = TFile::Open(plots0lnovetoes->outfileName(),"UPDATE");
    TIter nextkey2(gDirectory->GetListOfKeys());
    while(TKey *key = (TKey*)nextkey2()) {
      TObject *obj = key->ReadObj();
      if(TString(obj->GetName()).Contains("_sr_") && TString(obj->GetName()).Contains("_data")) {
        TString rmstr(obj->GetName());
        rmstr += ";*";
        filenovetoes->Delete(rmstr.Data());
      }
    }
    filenovetoes->Write();
    filenovetoes->Close();
  }

  cout << "Plotting 1lepton region" << endl;

  plotslepcr->addTreeVar("met_lepcr_nbgeq1_lowmt",        "met",         sel["lepcr"] + sel["lowmt"],         "#slash{E}_{T} [GeV]", NBINS, metbins);
  plotslepcr->addTreeVar("met_lepcr_nbgeq1_nt0_highmt",   "met",         sel["lepcr"] + sel["nt0_highmt"],    "#slash{E}_{T} [GeV]", NBINS, metbins);
  plotslepcr->addTreeVar("met_lepcr_nbgeq1_nt1_highmt",   "met",         sel["lepcr"] + sel["nt1_highmt"],    "#slash{E}_{T} [GeV]", NBINS_NT1, metbins_nt1);

  plotslepcr->plot();

  cout << "Plotting photon region" << endl;

  plotsphocr->addTreeVar("met_phocr_nbgeq1_nomt",         "met",         sel["phocr"],                                      "#slash{E}_{T} [GeV]", NBINS, metbins);
  plotsphocr->addTreeVar("met_phocr_nbgeq1_lowmt",        "met",         sel["phocr"] + sel["lowmt"],       "#slash{E}_{T} [GeV]", NBINS, metbins);
  plotsphocr->addTreeVar("met_phocr_nbgeq1_nt0_nomt",     "met",         sel["phocr"] + sel["nt0"],         "#slash{E}_{T} [GeV]", NBINS, metbins);
  plotsphocr->addTreeVar("met_phocr_nbgeq1_nt0_highmt",   "met",         sel["phocr"] + sel["nt0_highmt"],  "#slash{E}_{T} [GeV]", NBINS, metbins);
  plotsphocr->addTreeVar("met_phocr_nbgeq1_nt1_nomt",     "met",         sel["phocr"] + sel["nt1"],                         "#slash{E}_{T} [GeV]", NBINS_NT1, metbins_nt1);
  plotsphocr->addTreeVar("met_phocr_nbgeq1_nt1_highmt",   "met",         sel["phocr"] + sel["nt1_highmt"],  "#slash{E}_{T} [GeV]", NBINS_NT1, metbins_nt1);

  plotsphocr->plot();

  cout << "Plotting zll region" << endl;

  plotszllcr->addTreeVar("met_zllcr_nb1",         "met",         sel["zllcr"] + sel["nb1"],      "#slash{E}_{T} [GeV]", 1, 100., 1000.0);
  plotszllcr->addTreeVar("met_zllcr_nb2",         "met",         sel["zllcr"] + sel["nb2"],      "#slash{E}_{T} [GeV]", 1, 100., 1000.0);
  plotszllcr->addTreeVar("met_zllcr_nbgeq1",      "met",         sel["zllcr"],                   "#slash{E}_{T} [GeV]", 1, 100., 1000.0);

  plotszllcr->addTreeVar("met_zllcr_offz_nb1",    "met",         sel["zlloffcr"] + sel["nb1"],   "#slash{E}_{T} [GeV]", 1, 100., 1000.0);
  plotszllcr->addTreeVar("met_zllcr_offz_nb2",    "met",         sel["zlloffcr"] + sel["nb2"],   "#slash{E}_{T} [GeV]", 1, 100., 1000.0);
  plotszllcr->addTreeVar("met_zllcr_offz_nbgeq1", "met",         sel["zlloffcr"],                "#slash{E}_{T} [GeV]", 1, 100., 1000.0);

  plotszllcr->plot();

  /*cout << "Plotting ttZ region" << endl;

  plotsttzcr->addTreeVar("extProcess_ttzcr_sel",     "extProcess",  sel["ttzSel"],                  "extProcess", 13,-.5,12.5);
  plotsttzcr->addTreeVar("extProcess_ttzcr_noz",     "extProcess",  sel["ttzNoZ"],                  "extProcess", 13,-.5,12.5);
  plotsttzcr->addTreeVar("extProcess_ttzcr_nob",     "extProcess",  sel["ttzNoB"],                  "extProcess", 13,-.5,12.5);

  plotsttzcr->plot();*/

  cout << "Setting up prediction" << endl;

  vector<Sample*> samples0l;
  PlotTools::initSamples(srconf, samples0l);

  TFile* file0l         = new TFile(plots0l->outfileName());
  TFile* file0lnovetoes = new TFile(plots0lnovetoes->outfileName());
  TFile* filelepcr      = new TFile(plotslepcr->outfileName());
  TFile* filephocr      = new TFile(plotsphocr->outfileName());
  TFile* filezllcr      = new TFile(plotszllcr->outfileName());
  //TFile* filettzcr    = new TFile(plotsttzcr->outfileName());

  BinMap  lepcrtosr, qcdcrtosr, phocrtosr, zllcrtosr;

  vector<TString> srbins = {"nb1_lowmt", "nb2_lowmt", "nb1_nt0_highmt", "nb2_nt0_highmt", "nb1_nt1_highmt", "nb2_nt1_highmt"};
  lepcrtosr["nb1_lowmt"]      = "nbgeq1_lowmt";
  lepcrtosr["nb2_lowmt"]      = "nbgeq1_lowmt";
  lepcrtosr["nb1_nt0_highmt"] = "nbgeq1_nt0_highmt";
  lepcrtosr["nb2_nt0_highmt"] = "nbgeq1_nt0_highmt";
  lepcrtosr["nb1_nt1_highmt"] = "nbgeq1_nt1_highmt";
  lepcrtosr["nb2_nt1_highmt"] = "nbgeq1_nt1_highmt";
  qcdcrtosr["nb1_lowmt"]      = "nbgeq1_lowmt";
  qcdcrtosr["nb2_lowmt"]      = "nbgeq1_lowmt";
  qcdcrtosr["nb1_nt0_highmt"] = "nbgeq1_nt0_highmt";
  qcdcrtosr["nb2_nt0_highmt"] = "nbgeq1_nt0_highmt";
  qcdcrtosr["nb1_nt1_highmt"] = "nbgeq1_nt1_highmt";
  qcdcrtosr["nb2_nt1_highmt"] = "nbgeq1_nt1_highmt";
  phocrtosr["nb1_lowmt"]      = "nbgeq1_lowmt";
  phocrtosr["nb2_lowmt"]      = "nbgeq1_lowmt";
  phocrtosr["nb1_nt0_highmt"] = "nbgeq1_nt0_highmt";
  phocrtosr["nb2_nt0_highmt"] = "nbgeq1_nt0_highmt";
  phocrtosr["nb1_nt1_highmt"] = "nbgeq1_nt1_highmt";
  phocrtosr["nb2_nt1_highmt"] = "nbgeq1_nt1_highmt";
  zllcrtosr["nb1_lowmt"]      = "nb1";
  zllcrtosr["nb2_lowmt"]      = "nb2";
  zllcrtosr["nb1_nt0_highmt"] = "nb1";
  zllcrtosr["nb2_nt0_highmt"] = "nb2";
  zllcrtosr["nb1_nt1_highmt"] = "nb1";
  zllcrtosr["nb2_nt1_highmt"] = "nb2";

  vector<TString> qcdbkgsamples = {"ttbarplusw","znunu","ttZ"};

  TH1F* data           = dolownj ? getSRHist (file0l, "data", "sr",      srbins) : 0;
  TH1F* lostlep        = getLLPred (file0l, filelepcr, "sr",      "lepcr",      srbins, lepcrtosr);
  TH1F* znunu          = getZPred  (file0l, filephocr, filezllcr, "sr", "phocr", "zllcr", srbins, phocrtosr, zllcrtosr);
  TH1F* qcd            = getQCDPred (file0l, file0lnovetoes, "sr", "qcdcr", srbins, qcdcrtosr, qcdbkgsamples);
  //TH1F* ttz          = getTTZPred(file0l, filettzcr, "sr", "ttzcr", srbins, usettzsf);
  TH1F* ttz            = getTTZPred(file0l, "sr", srbins);


  if(ext.BeginsWith("/")) ext.ReplaceAll("/","_");
  TString fname = "0lsearch";
  if(dolownj) fname += "_lownj";
  TFile* srpred = new TFile(fname+ext+".root","RECREATE");
  srpred->cd();

  cout << "\n-----------------------" << endl;
  cout << "Saving Final Prediction" << endl;
  cout << "For " << region << " region" << endl;
  if(dolownj) cout << "Data \t || LostLep \t | Znunu \t | QCD \t | ttZ \t || Total Bkg." << endl;
  else cout << "LostLep \t | Znunu \t | QCD \t | ttZ \t || Total Bkg." << endl;

  lostlep->GetXaxis()->SetTitle("Search region");

  for(int ibin = 1; ibin < lostlep->GetNbinsX()+1; ++ibin) {
    double nkgtot = lostlep->GetBinContent(ibin) + znunu->GetBinContent(ibin) + qcd->GetBinContent(ibin) + ttz->GetBinContent(ibin);
    double nbkgtotuncsq = pow(lostlep->GetBinError(ibin),2) + pow(znunu->GetBinError(ibin),2) + pow(qcd->GetBinError(ibin),2) + pow(ttz->GetBinError(ibin),2);
    cout << "Bin " << ibin << endl;
    if(dolownj) cout << data->GetBinContent(ibin) << " +/- " << data->GetBinError(ibin) << "\t || ";
    cout << lostlep->GetBinContent(ibin) << " +/- " << lostlep->GetBinError(ibin) << "\t | ";
    cout << znunu->GetBinContent(ibin) << " +/- " << znunu->GetBinError(ibin) << "\t | ";
    cout << qcd->GetBinContent(ibin) << " +/- " << qcd->GetBinError(ibin) << "\t | ";
    cout << ttz->GetBinContent(ibin) << " +/- " << ttz->GetBinError(ibin) << "\t || ";
    cout << nkgtot << " +/- " << sqrt(nbkgtotuncsq) << endl;
  }

  if(dolownj) data->Write();
  lostlep->Write();
  znunu->Write();
  qcd->Write();
  ttz->Write();

  if(ext.BeginsWith("_")) ext.ReplaceAll("_","/");
  PlotStuff* plots = new PlotStuff(srpred, outputdir+ext);

  plots->setPlotSource(PlotStuff::HISTSSINGLEFILE);
  plots->setPlotType(PlotStuff::DATAMC);
  plots->setFormat(format);
  plots->setYTitle("Events");
  //plots->setLogy();
  plots->setHeaderText("#sqrt{s} = 13 TeV",TString::Format("%4.2f fb^{-1}",stof(string(lumistr.Data()))),"");
  plots->setHeaderPosition(0.16, 0.93);
  plots->setColor("lostlep_pred_sr",StyleTools::color_ttbar);
  plots->setMaxScale(2.0);  

  vector<TString> labels, names;
  if(dolownj) {
    plots->setRatioPlot();
    labels = {"Data", "t#bar{t}/W", "Z#rightarrow#nu#nu", "t#bar{t}Z", "QCD"};
    names = {"data_sr", "lostlep_pred_sr", "znunu_pred_sr", "ttZ_pred_sr", "qcd_pred_sr"};
    plots->setDataName("data_sr");
    plots->setUsePoisson();
    plots->setUncertaintyBand();
    plots->addCompSet("datavspred",names,labels);
  } else {
    labels = {"t#bar{t}/W", "Z#rightarrow#nu#nu", "t#bar{t}Z", "QCD"};
    names = {"lostlep_pred_sr", "znunu_pred_sr", "ttZ_pred_sr", "qcd_pred_sr"};
    plots->addCompSet("pred",names,labels);
  }

  plots->plot();

  delete plots;

}
