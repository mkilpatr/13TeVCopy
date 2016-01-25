#if !defined(__CINT__) || defined(__MAKECINT__)
#include "PredictionHelper.hh"
#endif

using namespace BkgPrediction;

void getLLPrediction(const TString defaultdir  = "/eos/uscms/store/user/vdutta/13TeV/trees/011416",
                     const TString outputdir   = "plots_bkgest/lostlep",
                     const TString srconf      = "plotting/run0lepbkgpred.conf",
                     const TString lumistr     = "2.262",
                     const TString region      = "sr",
                     const TString format      = "pdf",
                     const bool    dolownj     = false,
                     const bool    dotnp       = true,
                     vector<TString> vars      = {""}, //{"lep","btag","mistag","jes","pu"},
                     const bool    plotlog     = false)
{
  gSystem->mkdir(outputdir, true);

  TString basewgt    = lumistr + "*weight*truePUWeight*btagWeight";
  TString lepvetowgt = dotnp ? basewgt + "*leptnpweight*lepvetoweight" : basewgt + "*lepvetoweight";
  TString lepselwgt  = dotnp ? basewgt + "*leptnpweight" : basewgt + "*lepselweight";

  sel["trig"]         = "passjson && passdijetmet && j2pt>75 && met>200 && passcscbeamhaloflt && passeebadscflt && passeebadsc4flt && passhbheisoflt && passhbhefltloose";
  sel["vetoes"]       = dotnp ? " && nvetolep==0 && (nvetotau==0 || (ismc && npromptgentau>0))" : " && ((nvetolep==0 && nvetotau==0) || (ismc && (ngoodgenele>0 || ngoodgenmu>0 || npromptgentau>0)))";
  sel["lepsel"]       = " && nvetolep>0";
  sel["njets"]        = dolownj ? " && njets>=2 && njets<5 && nbjets>=1 && nlbjets>=2" : " && njets>=5 && nbjets>=1 && nlbjets>=2";
  sel["dphij123"]     = " && dphij12met>0.5 && dphij3met>0.5 && dphij4met>0.5";
  sel["dphij3"]       = " && dphij3met>0.5 && dphij4met>0.5";
  sel["dphij123inv"]  = " && dphij12met<0.15";
  sel["lepcrsel"]     = " && nvetolep>0 && mtlepmet<100";
  sel["sr"]           = sel["trig"] + sel["vetoes"]   + sel["njets"]    + sel["dphij123"];
  sel["lepcr"]        = sel["trig"] + sel["lepcrsel"] + sel["njets"]    + sel["dphij123"];
  sel["lowmt"]        = " && mtcsv12met<=175";
  sel["nt0_highmt"]   = " && ncttstd==0 && mtcsv12met>175";
  sel["nt1_highmt"]   = " && ncttstd>0 && mtcsv12met>175";
  sel["nb1_lowmt"]    = " && nbjets==1 && mtcsv12met<=175";
  sel["nb2_lowmt"]    = " && nbjets>=2 && mtcsv12met<=175";
  sel["nb1_nt0_highmt"]   = " && nbjets==1 && ncttstd==0 && mtcsv12met>175";
  sel["nb2_nt0_highmt"]   = " && nbjets>=2 && ncttstd==0 && mtcsv12met>175";
  sel["nb1_nt1_highmt"]   = " && nbjets==1 && ncttstd>0 && mtcsv12met>175";
  sel["nb2_nt1_highmt"]   = " && nbjets>=2 && ncttstd>0 && mtcsv12met>175";


  TFile* srpred = 0;
  TString outfilename = outputdir + "/lostleppred.root";

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
      PlotStuff* plotslepcr = setupPlots(srconf,    inputdir, outputdir, lepselwgt, plotlog, format, lumistr, "output_lepcr"+suffix+".root");

      cout << "Plotting 0lepton region" << endl;

      plots0l->addTreeVar("met_sr_nb1_lowmt",                     "met",         sel["sr"] + sel["nb1_lowmt"],          "#slash{E}_{T} [GeV]", NBINS, metbins);
      plots0l->addTreeVar("met_sr_nb2_lowmt",                     "met",         sel["sr"] + sel["nb2_lowmt"],          "#slash{E}_{T} [GeV]", NBINS, metbins);
      plots0l->addTreeVar("met_sr_nb1_nt0_highmt",                "met",         sel["sr"] + sel["nb1_nt0_highmt"],     "#slash{E}_{T} [GeV]", NBINS, metbins);
      plots0l->addTreeVar("met_sr_nb2_nt0_highmt",                "met",         sel["sr"] + sel["nb2_nt0_highmt"],     "#slash{E}_{T} [GeV]", NBINS, metbins);
      plots0l->addTreeVar("met_sr_nb1_nt1_highmt",                "met",         sel["sr"] + sel["nb1_nt1_highmt"],     "#slash{E}_{T} [GeV]", NBINS, metbins);
      plots0l->addTreeVar("met_sr_nb2_nt1_highmt",                "met",         sel["sr"] + sel["nb2_nt1_highmt"],     "#slash{E}_{T} [GeV]", NBINS, metbins);

      plots0l->plot();

      if(!dolownj) {
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
      }

      cout << "Plotting 1lepton region" << endl;

      plotslepcr->addTreeVar("met_lepcr_nbgeq1_lowmt",        "met",         sel["lepcr"] + sel["lowmt"],                    "#slash{E}_{T} [GeV]", NBINS, metbins);
      plotslepcr->addTreeVar("met_lepcr_nbgeq1_nt0_highmt",   "met",         sel["lepcr"] + sel["nt0_highmt"],               "#slash{E}_{T} [GeV]", NBINS, metbins);
      plotslepcr->addTreeVar("met_lepcr_nbgeq1_nt1_highmt",   "met",         sel["lepcr"] + sel["nt1_highmt"],               "#slash{E}_{T} [GeV]", NBINS, metbins);

      plotslepcr->plot();

      cout << "Setting up prediction" << endl;

      TFile* file0l      = new TFile(plots0l->outfileName());
      TFile* filelepcr   = new TFile(plotslepcr->outfileName());

      BinMap  lepcrtosr;

      vector<TString> srbins = {"nb1_lowmt", "nb2_lowmt", "nb1_nt0_highmt", "nb2_nt0_highmt", "nb1_nt1_highmt", "nb2_nt1_highmt"};
      lepcrtosr["nb1_lowmt"]      = "nbgeq1_lowmt";
      lepcrtosr["nb2_lowmt"]      = "nbgeq1_lowmt";
      lepcrtosr["nb1_nt0_highmt"] = "nbgeq1_nt0_highmt";
      lepcrtosr["nb2_nt0_highmt"] = "nbgeq1_nt0_highmt";
      lepcrtosr["nb1_nt1_highmt"] = "nbgeq1_nt1_highmt";
      lepcrtosr["nb2_nt1_highmt"] = "nbgeq1_nt1_highmt";

      TH1F* lostlep        = getLLPred (file0l, filelepcr, "sr",      "lepcr",      srbins, lepcrtosr);

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

      lostlep->GetXaxis()->SetTitle("Search region");

      for(int ibin = 1; ibin < lostlep->GetNbinsX()+1; ++ibin) {
        cout << "Bin " << ibin << ": "  << lostlep->GetBinContent(ibin) << " +/- " << lostlep->GetBinError(ibin) << endl;
      }

      if(sysvar == NOMINAL)
        lostlep->Write("lostlep_pred_sr");
      else 
        lostlep->Write("lostlep_pred_sr"+suffix);

      srpred->Close();
    }
  }

  vector<TString> names, labels;

  srpred = new TFile(outfilename,"UPDATE");
  srpred->cd();

  vector<TH1F*> histstodraw;
  TH1F* nominal = (TH1F*)srpred->Get("lostlep_pred_sr");
  names.push_back(nominal->GetName());
  labels.push_back("Prediction #pm #sigma_{stat}");
  histstodraw.push_back(nominal);

  if(vars.size() && vars[0] != "") {
    for(auto var : vars) {
      TH1F* varup = (TH1F*)srpred->Get(TString("lostlep_pred_sr_"+var+"up").Data());
      TH1F* vardown = (TH1F*)srpred->Get(TString("lostlep_pred_sr_"+var+"down").Data());
      TH1F* varhist = getVarHist(histstodraw.back(), varup, vardown, "lostlep_pred_sr_var"+var);
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

  plots->addCompSet("lostleppred",names,labels);

  plots->plot();

  delete plots;

}
