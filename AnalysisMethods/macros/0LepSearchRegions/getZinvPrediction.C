#if !defined(__CINT__) || defined(__MAKECINT__)
#include "PredictionHelper.hh"
#endif

using namespace BkgPrediction;

void getZinvPrediction(const TString defaultdir  = "/eos/uscms/store/user/mullin/13TeV/lepCor/trees/160208_met250njets5_pr521",
                       const TString outputdir   = "plots_bkgest/zinv",
                       const TString srconf      = "plotting/run0lepbkgpred.conf",
                       const TString phocrconf   = "plotting/runphotoncrbkgpred.conf",
                       const TString zllcrconf   = "plotting/runzllcrbkgpred.conf",
                       const TString lumistr     = "2.262",
                       const TString region      = "sr",
                       const TString format      = "pdf",
                       const bool    dolownj     = false,
                       vector<TString> vars      = {""}, //lep","btag","mistag","jes","pu"},
                       const bool    plotlog     = false)
{
  gSystem->mkdir(outputdir, true);

  TString basewgt    = lumistr + "*weight*truePUWeight*btagWeight*qcdRespTailWeight*(cttWeight*(ncttstd>0 && mtcsv12met>175) + 1.0*(ncttstd==0 || mtcsv12met<=175))";
  TString lepvetowgt = basewgt + "*leptnpweight*lepvetoweight";
  TString lepselwgt  = basewgt + "*leptnpweight";

  sel["trig"]         = "passjson && passdijetmet && j2pt>75 && met>250 && passcscbeamhaloflt && passeebadscflt && passeebadsc4flt && passhbheisoflt && passhbhefltloose";
  sel["trigpho"]      = "passjson && passtrigphoton165 && origmet<200 && j2pt>75 && met>200 && passcscbeamhaloflt && passeebadscflt && passeebadsc4flt && passhbheisoflt && passhbhefltloose";
  sel["trigzll"]      = "passjson && passTrig && j2pt>75 && met>100 && dilepmass > 80 && dilepmass < 100";
  sel["trigzlloff"]   = "passjson && passTrig && j2pt>75 && met>100 && dilepmass > 20 && (dilepmass < 80 || dilepmass > 100)";
  sel["vetoes"]       = " && nvetolep==0 && (nvetotau==0 || (ismc && npromptgentau>0))";
  sel["njets"]        = dolownj ? " && njets>=2 && njets<5 && nbjets>=1 && nlbjets>=2" : " && njets>=5 && nbjets>=1 && nlbjets>=2";
  sel["dphij123"]     = " && dphij12met>0.5 && dphij3met>0.5 && dphij4met>0.5";
  sel["phocr"]        = sel["trigpho"] + sel["njets"] + sel["dphij123"];
  sel["zllcr"]        = sel["trigzll"] + sel["njets"];
  sel["zlloffcr"]     = sel["trigzlloff"] + sel["njets"];
  sel["sr"]           = sel["trig"] + sel["vetoes"]   + sel["njets"]    + sel["dphij123"];
  sel["lowmt"]        = " && mtcsv12met<=175";
  sel["highmt"]       = " && mtcsv12met>175";
  sel["mednj"]        = dolownj ? "" : " && njets>=5 && njets<7";
  sel["highnj"]       = dolownj ? "" : " && njets>=7";
  sel["nb1"]          = " && nbjets==1";
  sel["nb2"]          = " && nbjets>=2";
  sel["nt0"]          = " && ncttstd==0";
  sel["nt1"]          = " && ncttstd>0";
  sel["lowmt_mednj"]        = sel["lowmt"] + sel["mednj"];
  sel["lowmt_highnj"]       = sel["lowmt"] + sel["highnj"];
  sel["highmt_mednj_nt0"]   = sel["highmt"] + sel["mednj"] + sel["nt0"];
  sel["highmt_highnj_nt0"]  = sel["highmt"] + sel["highnj"] + sel["nt0"];
  sel["highmt_nt1"]         = sel["highmt"] + sel["nt1"];
  sel["lowmt_mednj_nb1"]        = sel["lowmt"] + sel["mednj"] + sel["nb1"];
  sel["lowmt_mednj_nb2"]        = sel["lowmt"] + sel["mednj"] + sel["nb2"];
  sel["lowmt_highnj_nb1"]       = sel["lowmt"] + sel["highnj"] + sel["nb1"];
  sel["lowmt_highnj_nb2"]       = sel["lowmt"] + sel["highnj"] + sel["nb2"];
  sel["highmt_mednj_nt0_nb1"]   = sel["highmt"] + sel["mednj"] + sel["nt0"] + sel["nb1"];
  sel["highmt_mednj_nt0_nb2"]   = sel["highmt"] + sel["mednj"] + sel["nt0"] + sel["nb2"];
  sel["highmt_highnj_nt0_nb1"]  = sel["highmt"] + sel["highnj"] + sel["nt0"] + sel["nb1"];
  sel["highmt_highnj_nt0_nb2"]  = sel["highmt"] + sel["highnj"] + sel["nt0"] + sel["nb2"];
  sel["highmt_nt1_nb1"]         = sel["highmt"] + sel["nt1"] + sel["nb1"];
  sel["highmt_nt1_nb2"]         = sel["highmt"] + sel["nt1"] + sel["nb2"];


  TFile* srpred = 0;
  TString outfilename = outputdir + "/znunupred.root";

  for(unsigned int i = 0; i < vars.size(); ++i) {
    auto var = vars[i];
    cout << "var: " << var.Data() << endl;
    for(unsigned int ivar = 0; ivar != NONE; ++ivar) {
      cout << ivar << endl;
      if(i > 0 && ivar == 0) continue;
      else if(var == "" && ivar > 0) continue;
      SysVar sysvar = static_cast<SysVar>(ivar);

      TString suffix = "";

      switch(sysvar) {
        case NOMINAL : {
          cout << "Computing central values" << endl;
          break;
        }
        case VARUP : {
          cout << "Varying up " << var.Data() << endl;
          suffix = var + "up";
          break;
        }
        case VARDOWN : {
          cout << "Varying down " << var.Data() << endl;
          suffix = var + "down";
          break;
        }
        default : {
          cout << "Not a valid option!" << endl;
          break;
        }
      }

      TString inputdir    = sysvar == NOMINAL ? defaultdir : defaultdir + "/" + suffix;
      TString inputdirphotoncr(inputdir);
      inputdirphotoncr.ReplaceAll(defaultdir, defaultdir + "/photoncr");
      TString inputdirzllcr(inputdir);
      inputdirzllcr.ReplaceAll(defaultdir, defaultdir + "/zllcr");

      if(suffix != "") suffix.Prepend("_");

      PlotStuff* plots0l    = setupPlots(srconf,    inputdir, outputdir, lepvetowgt, plotlog, format, lumistr, "output_0l"+suffix+".root");
      PlotStuff* plotsphocr = setupPlots(phocrconf, inputdirphotoncr, outputdir, basewgt, plotlog, format, lumistr, "output_phocr.root");
      PlotStuff* plotszllcr = setupPlots(zllcrconf, inputdirzllcr, outputdir, lepselwgt, plotlog, format, lumistr, "output_zllcr.root");

      cout << "Plotting 0lepton region" << endl;

      plots0l->addTreeVar("met_sr_lowmt_mednj_nb1",             "met",        sel["sr"] + sel["lowmt_mednj_nb1"],       "#slash{E}_{T} [GeV]", NBINS, metbins);
      plots0l->addTreeVar("met_sr_lowmt_mednj_nb2",             "met",        sel["sr"] + sel["lowmt_mednj_nb2"],       "#slash{E}_{T} [GeV]", NBINS, metbins);
      plots0l->addTreeVar("met_sr_lowmt_highnj_nb1",            "met",        sel["sr"] + sel["lowmt_highnj_nb1"],      "#slash{E}_{T} [GeV]", NBINS, metbins);
      plots0l->addTreeVar("met_sr_lowmt_highnj_nb2",            "met",        sel["sr"] + sel["lowmt_highnj_nb2"],      "#slash{E}_{T} [GeV]", NBINS, metbins);
      plots0l->addTreeVar("met_sr_highmt_mednj_nt0_nb1",        "met",        sel["sr"] + sel["highmt_mednj_nt0_nb1"],  "#slash{E}_{T} [GeV]", NBINS, metbins);
      plots0l->addTreeVar("met_sr_highmt_mednj_nt0_nb2",        "met",        sel["sr"] + sel["highmt_mednj_nt0_nb2"],  "#slash{E}_{T} [GeV]", NBINS, metbins);
      plots0l->addTreeVar("met_sr_highmt_highnj_nt0_nb1",       "met",        sel["sr"] + sel["highmt_highnj_nt0_nb1"], "#slash{E}_{T} [GeV]", NBINS, metbins);
      plots0l->addTreeVar("met_sr_highmt_highnj_nt0_nb2",       "met",        sel["sr"] + sel["highmt_highnj_nt0_nb2"], "#slash{E}_{T} [GeV]", NBINS, metbins);
      plots0l->addTreeVar("met_sr_highmt_nt1_nb1",              "met",        sel["sr"] + sel["highmt_nt1_nb1"],        "#slash{E}_{T} [GeV]", NBINS_NT1, metbins_nt1);
      plots0l->addTreeVar("met_sr_highmt_nt1_nb2",              "met",        sel["sr"] + sel["highmt_nt1_nb2"],        "#slash{E}_{T} [GeV]", NBINS_NT1, metbins_nt1);

      plots0l->plot();

      /*if(!dolownj) {
        TString rmcmd = "rm " + outputdir + "/met_sr_*." + format;
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
      }*/

      cout << "Plotting photon region" << endl;

      plotsphocr->addTreeVar("met_phocr_nbgeq1_nomt_mednj",        "met",        sel["phocr"] + sel["mednj"],                 "#slash{E}_{T} [GeV]", NBINS, metbins);
      plotsphocr->addTreeVar("met_phocr_nbgeq1_nomt_highnj",       "met",        sel["phocr"] + sel["highnj"],                "#slash{E}_{T} [GeV]", NBINS, metbins);
      plotsphocr->addTreeVar("met_phocr_nbgeq1_nomt_mednj_nt0",    "met",        sel["phocr"] + sel["mednj"] + sel["nt0"],    "#slash{E}_{T} [GeV]", NBINS, metbins);
      plotsphocr->addTreeVar("met_phocr_nbgeq1_nomt_highnj_nt0",   "met",        sel["phocr"] + sel["highnj"] + sel["nt0"],   "#slash{E}_{T} [GeV]", NBINS, metbins);
      plotsphocr->addTreeVar("met_phocr_nbgeq1_nomt_nt1",          "met",        sel["phocr"] + sel["nt1"],                   "#slash{E}_{T} [GeV]", NBINS_NT1, metbins_nt1);
    
      plotsphocr->addTreeVar("met_phocr_nbgeq1_lowmt_mednj",       "met",        sel["phocr"] + sel["lowmt_mednj"],        "#slash{E}_{T} [GeV]", NBINS, metbins);
      plotsphocr->addTreeVar("met_phocr_nbgeq1_lowmt_highnj",      "met",        sel["phocr"] + sel["lowmt_highnj"],       "#slash{E}_{T} [GeV]", NBINS, metbins);
      plotsphocr->addTreeVar("met_phocr_nbgeq1_highmt_mednj_nt0",  "met",        sel["phocr"] + sel["highmt_mednj_nt0"],   "#slash{E}_{T} [GeV]", NBINS, metbins);
      plotsphocr->addTreeVar("met_phocr_nbgeq1_highmt_highnj_nt0", "met",        sel["phocr"] + sel["highmt_highnj_nt0"],  "#slash{E}_{T} [GeV]", NBINS, metbins);
      plotsphocr->addTreeVar("met_phocr_nbgeq1_highmt_nt1",        "met",        sel["phocr"] + sel["highmt_nt1"],         "#slash{E}_{T} [GeV]", NBINS_NT1, metbins_nt1);
    //  integrating in met
    //  plotsphocr->addTreeVar("met_phocr_nbgeq1_highmt_nt1_int",    "met",        sel["phocr"] + sel["highmt_nt1"],         "#slash{E}_{T} [GeV]", NBINS_NT1, metbins_nt1);

      plotsphocr->plot();

      cout << "Plotting zll region" << endl;

      plotszllcr->addTreeVar("met_zllcr_nb1",         "met",         sel["zllcr"] + sel["nb1"],      "#slash{E}_{T} [GeV]", 1, 100., 1000.0);
      plotszllcr->addTreeVar("met_zllcr_nb2",         "met",         sel["zllcr"] + sel["nb2"],      "#slash{E}_{T} [GeV]", 1, 100., 1000.0);
      plotszllcr->addTreeVar("met_zllcr_nbgeq1",      "met",         sel["zllcr"],                   "#slash{E}_{T} [GeV]", 1, 100., 1000.0);

      plotszllcr->addTreeVar("met_zllcr_offz_nb1",    "met",         sel["zlloffcr"] + sel["nb1"],   "#slash{E}_{T} [GeV]", 1, 100., 1000.0);
      plotszllcr->addTreeVar("met_zllcr_offz_nb2",    "met",         sel["zlloffcr"] + sel["nb2"],   "#slash{E}_{T} [GeV]", 1, 100., 1000.0);
      plotszllcr->addTreeVar("met_zllcr_offz_nbgeq1", "met",         sel["zlloffcr"],                "#slash{E}_{T} [GeV]", 1, 100., 1000.0);

      plotszllcr->plot();

      cout << "Setting up prediction" << endl;

      TFile* file0l      = new TFile(plots0l->outfileName());
      TFile* filephocr      = new TFile(plotsphocr->outfileName());
      TFile* filezllcr      = new TFile(plotszllcr->outfileName());

      BinMap  phocrtosr, zllcrtosr;

      vector<TString> srbins;
      if (dolownj)
        srbins = {"lowmt_mednj_nb1", "lowmt_mednj_nb2", "highmt_mednj_nt0_nb1", "highmt_mednj_nt0_nb2", "highmt_nt1_nb1", "highmt_nt1_nb2"};
      else
        srbins = {"lowmt_mednj_nb1", "lowmt_mednj_nb2", "lowmt_highnj_nb1", "lowmt_highnj_nb2", "highmt_mednj_nt0_nb1", "highmt_mednj_nt0_nb2", "highmt_highnj_nt0_nb1", "highmt_highnj_nt0_nb2", "highmt_nt1_nb1", "highmt_nt1_nb2"};

      phocrtosr["lowmt_mednj_nb1"]        = "nbgeq1_lowmt_mednj";
      phocrtosr["lowmt_mednj_nb2"]        = "nbgeq1_lowmt_mednj";
      phocrtosr["lowmt_highnj_nb1"]       = "nbgeq1_lowmt_highnj";
      phocrtosr["lowmt_highnj_nb2"]       = "nbgeq1_lowmt_highnj";
      phocrtosr["highmt_mednj_nt0_nb1"]   = "nbgeq1_highmt_mednj_nt0";
      phocrtosr["highmt_mednj_nt0_nb2"]   = "nbgeq1_highmt_mednj_nt0";
      phocrtosr["highmt_highnj_nt0_nb1"]  = "nbgeq1_highmt_highnj_nt0";
      phocrtosr["highmt_highnj_nt0_nb2"]  = "nbgeq1_highmt_highnj_nt0";
      phocrtosr["highmt_nt1_nb1"]         = "nbgeq1_highmt_nt1";
      phocrtosr["highmt_nt1_nb2"]         = "nbgeq1_highmt_nt1";
      for (const auto &bin : srbins)      zllcrtosr[bin] = bin.Contains("nb1") ? "nb1" : "nb2";

      TH1F* znunu          = getZPred  (file0l, filephocr, filezllcr, "sr", "phocr", "zllcr", srbins, phocrtosr, zllcrtosr);

      if(!srpred) srpred = new TFile(outfilename,"RECREATE");
      else srpred = new TFile(outfilename,"UPDATE");
      srpred->cd();

      cout << "\n-----------------------" << endl;
      cout << "Saving Final Prediction" << endl;
      cout << "For " << region << " region" << endl;
      if(sysvar == NOMINAL)
        cout << "NOMINAL" << endl;
      else if(sysvar == VARUP)
        cout << "VARYING UP " << var.Data() << endl;
      else if(sysvar == VARDOWN)
        cout << "VARYING DOWN " << var.Data() << endl;
      cout << "~~~~~~~~~~~~~~~~~~" << endl;

      znunu->GetXaxis()->SetTitle("Search region");

      for(int ibin = 1; ibin < znunu->GetNbinsX()+1; ++ibin) {
        cout << "Bin " << ibin << ": "  << znunu->GetBinContent(ibin) << " +/- " << znunu->GetBinError(ibin) << endl;
      }

      if(sysvar == NOMINAL)
        znunu->Write("znunu_pred_sr");
      else 
        znunu->Write("znunu_pred_sr"+suffix);

      srpred->Close();
    }
  }

  vector<TString> names, labels;

  srpred = new TFile(outfilename,"UPDATE");
  srpred->cd();

  vector<TH1F*> histstodraw;
  TH1F* nominal = (TH1F*)srpred->Get("znunu_pred_sr");
  names.push_back(nominal->GetName());
  labels.push_back("Prediction #pm #sigma_{stat}");
  histstodraw.push_back(nominal);

  if(vars.size() && vars[0] != "") {
    for(auto var : vars) {
      TH1F* varup = (TH1F*)srpred->Get(TString("znunu_pred_sr_"+var+"up").Data());
      TH1F* vardown = (TH1F*)srpred->Get(TString("znunu_pred_sr_"+var+"down").Data());
      TH1F* varhist = getVarHist(histstodraw.back(), varup, vardown, "znunu_pred_sr_var"+var);
      varhist->Write();
      //names.push_back(varhist->GetName());
      //labels.push_back("#pm #sigma_{"+var+"}");
      histstodraw.push_back(varhist);
    }
    names.push_back(histstodraw.back()->GetName());
    labels.push_back("Prediction #pm #sigma_{stat} #pm #sigma_{sys}");
    TH1F* syshist = histstodraw.back();
    for(int ibin = 1; ibin < nominal->GetNbinsX()+1; ++ibin) {
      double staterr = nominal->GetBinError(ibin);
      double toterr  = syshist->GetBinError(ibin);
      cout << "Bin " << ibin << ": " << nominal->GetBinContent(ibin) << " #pm " << staterr << " #pm " << sqrt(toterr*toterr - staterr*staterr) << endl;
    }
  }
  std::reverse(names.begin(), names.end());
  std::reverse(labels.begin(), labels.end());

  PlotStuff* plots = new PlotStuff(srpred, outputdir);

  plots->setPlotSource(PlotStuff::HISTSSINGLEFILE);
  plots->setPlotType(PlotStuff::COMP);
  plots->setFormat(format);
  plots->setYTitle("Events");
  plots->setDrawOption("E");
  plots->setLogy();
  plots->setHeaderText("#sqrt{s} = 13 TeV",TString::Format("%4.2f fb^{-1}",stof(string(lumistr.Data()))),"");
  plots->setHeaderPosition(0.2, 0.93);
  plots->setMaxScale(2.0);  

  plots->addCompSet("znunupred",names,labels);

  plots->plot();

  delete plots;

}
