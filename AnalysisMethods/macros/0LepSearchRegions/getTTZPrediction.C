#if !defined(__CINT__) || defined(__MAKECINT__)
#include "PredictionHelper.hh"
#endif

using namespace BkgPrediction;

void getTTZPrediction(const TString defaultdir  = "/eos/uscms/store/user/mullin/13TeV/lepCor/trees/160208_met250njets5_pr521",
                      const TString outputdir   = "plots_bkgest/ttz",
                      const TString srconf      = "plotting/run0lepbkgpred.conf",
                      const TString lumistr     = "2.262",
                      const TString region      = "sr",
                      const TString format      = "pdf",
                      const bool    dolownj     = false,
                      vector<TString> vars      = {""}, //"lep","tau","btag","mistag","jes","pu"},
                      const bool    plotlog     = false)
{
  gSystem->mkdir(outputdir, true);

  TString basewgt    = lumistr + "*weight*truePUWeight*btagWeight*qcdRespTailWeight*(cttWeight*(ncttstd>0 && mtcsv12met>175) + 1.0*(ncttstd==0 || mtcsv12met<=175))";
  TString lepvetowgt =  basewgt + "*leptnpweight*lepvetoweight";

  sel["trig"]         = "passjson && passdijetmet && j2pt>75 && met>250 && passcscbeamhaloflt && passeebadscflt && passeebadsc4flt && passhbheisoflt && passhbhefltloose";
  sel["vetoes"]       = " && nvetolep==0 && (nvetotau==0 || (ismc && npromptgentau>0))";
  sel["njets"]        = dolownj ? " && njets>=2 && njets<5 && nbjets>=1 && nlbjets>=2" : " && njets>=5 && nbjets>=1 && nlbjets>=2";
  sel["dphij123"]     = " && dphij12met>0.5 && dphij3met>0.5 && dphij4met>0.5";
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
  TString outfilename = outputdir + "/ttzpred.root";

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

      if(suffix != "") suffix.Prepend("_");

      PlotStuff* plots0l    = setupPlots(srconf,    inputdir, outputdir, lepvetowgt, plotlog, format, lumistr, "output_0l"+suffix+".root");
      PlotStuff* plots0lnovetoes = setupPlots(srconf,    inputdir, outputdir, basewgt, plotlog, format, lumistr, "output_0lnovetoes.root");

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
      }*/

      cout << "Setting up prediction" << endl;

      TFile* file0l         = new TFile(plots0l->outfileName());

      vector<TString> srbins;
      if (dolownj)
        srbins = {"lowmt_mednj_nb1", "lowmt_mednj_nb2", "highmt_mednj_nt0_nb1", "highmt_mednj_nt0_nb2", "highmt_nt1_nb1", "highmt_nt1_nb2"};
      else
        srbins = {"lowmt_mednj_nb1", "lowmt_mednj_nb2", "lowmt_highnj_nb1", "lowmt_highnj_nb2", "highmt_mednj_nt0_nb1", "highmt_mednj_nt0_nb2", "highmt_highnj_nt0_nb1", "highmt_highnj_nt0_nb2", "highmt_nt1_nb1", "highmt_nt1_nb2"};

      TH1F* ttz        = getTTZPred(file0l, "sr", srbins);

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

      ttz->GetXaxis()->SetTitle("Search region");

      for(int ibin = 1; ibin < ttz->GetNbinsX()+1; ++ibin) {
        cout << "Bin " << ibin << ": "  << ttz->GetBinContent(ibin) << " +/- " << ttz->GetBinError(ibin) << endl;
      }

      if(sysvar == NOMINAL)
        ttz->Write("ttz_pred_sr");
      else 
        ttz->Write("ttz_pred_sr"+suffix);

      srpred->Close();
    }
  }

  vector<TString> names, labels;

  srpred = new TFile(outfilename,"UPDATE");
  srpred->cd();

  vector<TH1F*> histstodraw;
  TH1F* nominal = (TH1F*)srpred->Get("ttz_pred_sr");
  names.push_back(nominal->GetName());
  labels.push_back("Prediction #pm #sigma_{stat}");
  histstodraw.push_back(nominal);

  if(vars.size() && vars[0] != "") {
    for(auto var : vars) {
      TH1F* varup = (TH1F*)srpred->Get(TString("ttz_pred_sr_"+var+"up").Data());
      TH1F* vardown = (TH1F*)srpred->Get(TString("ttz_pred_sr_"+var+"down").Data());
      TH1F* varhist = getVarHist(histstodraw.back(), varup, vardown, "ttz_pred_sr_var"+var);
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

  plots->addCompSet("ttzpred",names,labels);

  plots->plot();

  delete plots;

}
