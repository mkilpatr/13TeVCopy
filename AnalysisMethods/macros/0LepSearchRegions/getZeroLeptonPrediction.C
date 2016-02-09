#if !defined(__CINT__) || defined(__MAKECINT__)
#include "PredictionHelper.hh"
#endif

using namespace BkgPrediction;

void getZeroLeptonPrediction(const TString defaultdir  = "root://cmseos:1094//store/user/vdutta/13TeV/trees/012216",
                             const TString outputdir   = "plots_bkgest_test/srpred",
                             const bool    dolownj     = false,
                             const bool    dolowmet    = false,
                             const bool    intBbins    = true,
                             const bool    plotlog     = true,
                             const TString var         = "",
                             const TString srconf      = "plotting/run0lepbkgpred.conf",
                             const TString phocrconf   = "plotting/runphotoncrbkgpred.conf",
                             const TString zllcrconf   = "plotting/runzllcrbkgpred.conf",
                             const TString ttzcrconf   = "plotting/runttzcrbkgpred.conf",
                             const TString lumistr     = "2.262",
                             const TString region      = "sr",
                             //const TString region      = "srlownj",
                             const TString format      = "pdf",
                             const bool    usettzsf    = false,
                             const unsigned int sysvar = NOMINAL)
{
  TString ext = "";
  if(var != "" && sysvar != NOMINAL) ext = "/" + var + (sysvar == VARUP ? "up" : "down");

  gSystem->mkdir(outputdir+ext, true);

  TString basewgt    = lumistr + "*weight*truePUWeight*btagWeight*qcdRespTailWeight*cttWeight";
  TString lepvetowgt = basewgt + "*leptnpweight*lepvetoweight";
  TString lepselwgt  = basewgt + "*leptnpweight";

  sel["trig"]         = "passjson && passdijetmet && j2pt>75 && met>200 && passcscbeamhaloflt && passeebadscflt && passeebadsc4flt && passhbheisoflt && passhbhefltloose";
  sel["trigpho"]      = "passjson && passtrigphoton165 && origmet<200 && j2pt>75 && met>200 && passcscbeamhaloflt && passeebadscflt && passeebadsc4flt && passhbheisoflt && passhbhefltloose";
  sel["trigzll"]      = "passjson && passTrig && j2pt>75 && met>100 && dilepmass > 80 && dilepmass < 100";
  sel["trigzlloff"]   = "passjson && passTrig && j2pt>75 && met>100 && dilepmass > 20 && (dilepmass < 80 || dilepmass > 100)";
  sel["vetoes"]       = " && nvetolep==0 && (nvetotau==0 || (ismc && npromptgentau>0))";
  sel["lepsel"]       = " && nvetolep>0";
  sel["njets"]        = dolownj ? " && njets>=2 && njets<5 && nbjets>=1 && nlbjets>=2" : " && njets>=5 && nbjets>=1 && nlbjets>=2";
  if (dolowmet)         { sel["trig"] += " && met<250"; sel["trigpho"] += " && met<250"; }
  sel["dphij123"]     = " && dphij12met>0.5 && dphij3met>0.5 && dphij4met>0.5";
  sel["dphij3"]       = " && dphij3met>0.5 && dphij4met>0.5";
  sel["dphij123inv"]  = " && (dphij12met<0.15 || dphij3met<0.15)";
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
  sel["lowmt"]        = " && mtcsv12met<=175";
  sel["highmt"]       = " && mtcsv12met>175";
  sel["mednj"]        = dolownj ? "" : " && njets>=5 && njets<7";
  sel["highnj"]       = dolownj ? "" : " && njets>=7";
  sel["nb1"]          = " && nbjets==1";
  sel["nb2"]          = " && nbjets>=2";
  sel["nt0"]          = " && ncttstd==0";
  sel["nt1"]          = " && ncttstd>0";
  // CRs
  sel["lowmt_mednj"]        = sel["lowmt"] + sel["mednj"];
  sel["lowmt_highnj"]       = sel["lowmt"] + sel["highnj"];
  sel["highmt_mednj_nt0"]   = sel["highmt"] + sel["mednj"] + sel["nt0"];
  sel["highmt_highnj_nt0"]  = sel["highmt"] + sel["highnj"] + sel["nt0"];
  sel["highmt_nt1"]         = sel["highmt"] + sel["nt1"];
  // SRs
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

  TString inputdir    = defaultdir;

  PlotStuff* plots0l    = setupPlots(srconf,    inputdir+ext, outputdir+ext, lepvetowgt, plotlog, format, lumistr, "output_0l.root");
  PlotStuff* plots0lnovetoes = setupPlots(srconf,    inputdir+ext, outputdir+ext, basewgt, plotlog, format, lumistr, "output_0lnovetoes.root");
  PlotStuff* plotslepcr = setupPlots(srconf,    inputdir+ext, outputdir+ext, lepselwgt, plotlog, format, lumistr, "output_lepcr.root");
  PlotStuff* plotsphocr = setupPlots(phocrconf, inputdir+"/photoncr"+ext, outputdir+ext, basewgt, plotlog, format, lumistr, "output_phocr.root");
  PlotStuff* plotszllcr = setupPlots(zllcrconf, inputdir+"/zllcr"+ext, outputdir+ext, lepselwgt, plotlog, format, lumistr, "output_zllcr.root");
  //PlotStuff* plotsttzcr = setupPlots(ttzcrconf, inputdir+"/ttzcr", outputdir, basewgt, plotlog, format, lumistr, "output_ttzcr.root");

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

  if(intBbins) {
    plots0l->addTreeVar("met_qcdcr_nbgeq1_lowmt_mednj",       "met",        sel["qcdcr"] + sel["lowmt_mednj"],        "#slash{E}_{T} [GeV]", NBINS, metbins);
    plots0l->addTreeVar("met_qcdcr_nbgeq1_lowmt_highnj",      "met",        sel["qcdcr"] + sel["lowmt_highnj"],       "#slash{E}_{T} [GeV]", NBINS, metbins);
    plots0l->addTreeVar("met_qcdcr_nbgeq1_highmt_mednj_nt0",  "met",        sel["qcdcr"] + sel["highmt_mednj_nt0"],   "#slash{E}_{T} [GeV]", NBINS, metbins);
    plots0l->addTreeVar("met_qcdcr_nbgeq1_highmt_highnj_nt0", "met",        sel["qcdcr"] + sel["highmt_highnj_nt0"],  "#slash{E}_{T} [GeV]", NBINS, metbins);
    plots0l->addTreeVar("met_qcdcr_nbgeq1_highmt_nt1",        "met",        sel["qcdcr"] + sel["highmt_nt1"],         "#slash{E}_{T} [GeV]", NBINS_NT1, metbins_nt1);
  }
  else {
    plots0l->addTreeVar("met_qcdcr_lowmt_mednj_nb1",             "met",        sel["qcdcr"] + sel["lowmt_mednj_nb1"],       "#slash{E}_{T} [GeV]", NBINS, metbins);
    plots0l->addTreeVar("met_qcdcr_lowmt_mednj_nb2",             "met",        sel["qcdcr"] + sel["lowmt_mednj_nb2"],       "#slash{E}_{T} [GeV]", NBINS, metbins);
    plots0l->addTreeVar("met_qcdcr_lowmt_highnj_nb1",            "met",        sel["qcdcr"] + sel["lowmt_highnj_nb1"],      "#slash{E}_{T} [GeV]", NBINS, metbins);
    plots0l->addTreeVar("met_qcdcr_lowmt_highnj_nb2",            "met",        sel["qcdcr"] + sel["lowmt_highnj_nb2"],      "#slash{E}_{T} [GeV]", NBINS, metbins);
    plots0l->addTreeVar("met_qcdcr_highmt_mednj_nt0_nb1",        "met",        sel["qcdcr"] + sel["highmt_mednj_nt0_nb1"],  "#slash{E}_{T} [GeV]", NBINS, metbins);
    plots0l->addTreeVar("met_qcdcr_highmt_mednj_nt0_nb2",        "met",        sel["qcdcr"] + sel["highmt_mednj_nt0_nb2"],  "#slash{E}_{T} [GeV]", NBINS, metbins);
    plots0l->addTreeVar("met_qcdcr_highmt_highnj_nt0_nb1",       "met",        sel["qcdcr"] + sel["highmt_highnj_nt0_nb1"], "#slash{E}_{T} [GeV]", NBINS, metbins);
    plots0l->addTreeVar("met_qcdcr_highmt_highnj_nt0_nb2",       "met",        sel["qcdcr"] + sel["highmt_highnj_nt0_nb2"], "#slash{E}_{T} [GeV]", NBINS, metbins);
    plots0l->addTreeVar("met_qcdcr_highmt_nt1_nb1",              "met",        sel["qcdcr"] + sel["highmt_nt1_nb1"],        "#slash{E}_{T} [GeV]", NBINS_NT1, metbins_nt1);
    plots0l->addTreeVar("met_qcdcr_highmt_nt1_nb2",              "met",        sel["qcdcr"] + sel["highmt_nt1_nb2"],        "#slash{E}_{T} [GeV]", NBINS_NT1, metbins_nt1);
  }

  plots0l->plot();

  cout << "Plotting 0lepton region with no vetoes" << endl;

  plots0lnovetoes->addTreeVar("met_srnovetoes_lowmt_mednj_nb1",             "met",        sel["srnovetoes"] + sel["lowmt_mednj_nb1"],       "#slash{E}_{T} [GeV]", NBINS, metbins);
  plots0lnovetoes->addTreeVar("met_srnovetoes_lowmt_mednj_nb2",             "met",        sel["srnovetoes"] + sel["lowmt_mednj_nb2"],       "#slash{E}_{T} [GeV]", NBINS, metbins);
  plots0lnovetoes->addTreeVar("met_srnovetoes_lowmt_highnj_nb1",            "met",        sel["srnovetoes"] + sel["lowmt_highnj_nb1"],      "#slash{E}_{T} [GeV]", NBINS, metbins);
  plots0lnovetoes->addTreeVar("met_srnovetoes_lowmt_highnj_nb2",            "met",        sel["srnovetoes"] + sel["lowmt_highnj_nb2"],      "#slash{E}_{T} [GeV]", NBINS, metbins);
  plots0lnovetoes->addTreeVar("met_srnovetoes_highmt_mednj_nt0_nb1",        "met",        sel["srnovetoes"] + sel["highmt_mednj_nt0_nb1"],  "#slash{E}_{T} [GeV]", NBINS, metbins);
  plots0lnovetoes->addTreeVar("met_srnovetoes_highmt_mednj_nt0_nb2",        "met",        sel["srnovetoes"] + sel["highmt_mednj_nt0_nb2"],  "#slash{E}_{T} [GeV]", NBINS, metbins);
  plots0lnovetoes->addTreeVar("met_srnovetoes_highmt_highnj_nt0_nb1",       "met",        sel["srnovetoes"] + sel["highmt_highnj_nt0_nb1"], "#slash{E}_{T} [GeV]", NBINS, metbins);
  plots0lnovetoes->addTreeVar("met_srnovetoes_highmt_highnj_nt0_nb2",       "met",        sel["srnovetoes"] + sel["highmt_highnj_nt0_nb2"], "#slash{E}_{T} [GeV]", NBINS, metbins);
  plots0lnovetoes->addTreeVar("met_srnovetoes_highmt_nt1_nb1",              "met",        sel["srnovetoes"] + sel["highmt_nt1_nb1"],        "#slash{E}_{T} [GeV]", NBINS_NT1, metbins_nt1);
  plots0lnovetoes->addTreeVar("met_srnovetoes_highmt_nt1_nb2",              "met",        sel["srnovetoes"] + sel["highmt_nt1_nb2"],        "#slash{E}_{T} [GeV]", NBINS_NT1, metbins_nt1);

  if(intBbins) {
    plots0lnovetoes->addTreeVar("met_qcdcrnovetoes_nbgeq1_lowmt_mednj",       "met",        sel["qcdcrnovetoes"] + sel["lowmt_mednj"],        "#slash{E}_{T} [GeV]", NBINS, metbins);
    plots0lnovetoes->addTreeVar("met_qcdcrnovetoes_nbgeq1_lowmt_highnj",      "met",        sel["qcdcrnovetoes"] + sel["lowmt_highnj"],       "#slash{E}_{T} [GeV]", NBINS, metbins);
    plots0lnovetoes->addTreeVar("met_qcdcrnovetoes_nbgeq1_highmt_mednj_nt0",  "met",        sel["qcdcrnovetoes"] + sel["highmt_mednj_nt0"],   "#slash{E}_{T} [GeV]", NBINS, metbins);
    plots0lnovetoes->addTreeVar("met_qcdcrnovetoes_nbgeq1_highmt_highnj_nt0", "met",        sel["qcdcrnovetoes"] + sel["highmt_highnj_nt0"],  "#slash{E}_{T} [GeV]", NBINS, metbins);
    plots0lnovetoes->addTreeVar("met_qcdcrnovetoes_nbgeq1_highmt_nt1",        "met",        sel["qcdcrnovetoes"] + sel["highmt_nt1"],         "#slash{E}_{T} [GeV]", NBINS_NT1, metbins_nt1);
  }
  else {
    plots0lnovetoes->addTreeVar("met_qcdcrnovetoes_lowmt_mednj_nb1",             "met",        sel["qcdcrnovetoes"] + sel["lowmt_mednj_nb1"],       "#slash{E}_{T} [GeV]", NBINS, metbins);
    plots0lnovetoes->addTreeVar("met_qcdcrnovetoes_lowmt_mednj_nb2",             "met",        sel["qcdcrnovetoes"] + sel["lowmt_mednj_nb2"],       "#slash{E}_{T} [GeV]", NBINS, metbins);
    plots0lnovetoes->addTreeVar("met_qcdcrnovetoes_lowmt_highnj_nb1",            "met",        sel["qcdcrnovetoes"] + sel["lowmt_highnj_nb1"],      "#slash{E}_{T} [GeV]", NBINS, metbins);
    plots0lnovetoes->addTreeVar("met_qcdcrnovetoes_lowmt_highnj_nb2",            "met",        sel["qcdcrnovetoes"] + sel["lowmt_highnj_nb2"],      "#slash{E}_{T} [GeV]", NBINS, metbins);
    plots0lnovetoes->addTreeVar("met_qcdcrnovetoes_highmt_mednj_nt0_nb1",        "met",        sel["qcdcrnovetoes"] + sel["highmt_mednj_nt0_nb1"],  "#slash{E}_{T} [GeV]", NBINS, metbins);
    plots0lnovetoes->addTreeVar("met_qcdcrnovetoes_highmt_mednj_nt0_nb2",        "met",        sel["qcdcrnovetoes"] + sel["highmt_mednj_nt0_nb2"],  "#slash{E}_{T} [GeV]", NBINS, metbins);
    plots0lnovetoes->addTreeVar("met_qcdcrnovetoes_highmt_highnj_nt0_nb1",       "met",        sel["qcdcrnovetoes"] + sel["highmt_highnj_nt0_nb1"], "#slash{E}_{T} [GeV]", NBINS, metbins);
    plots0lnovetoes->addTreeVar("met_qcdcrnovetoes_highmt_highnj_nt0_nb2",       "met",        sel["qcdcrnovetoes"] + sel["highmt_highnj_nt0_nb2"], "#slash{E}_{T} [GeV]", NBINS, metbins);
    plots0lnovetoes->addTreeVar("met_qcdcrnovetoes_highmt_nt1_nb1",              "met",        sel["qcdcrnovetoes"] + sel["highmt_nt1_nb1"],        "#slash{E}_{T} [GeV]", NBINS_NT1, metbins_nt1);
    plots0lnovetoes->addTreeVar("met_qcdcrnovetoes_highmt_nt1_nb2",              "met",        sel["qcdcrnovetoes"] + sel["highmt_nt1_nb2"],        "#slash{E}_{T} [GeV]", NBINS_NT1, metbins_nt1);
  }

  plots0lnovetoes->plot();

  if(!(true || dolownj || dolowmet)) {
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

  if(intBbins) {
    plotslepcr->addTreeVar("met_lepcr_nbgeq1_lowmt_mednj",       "met",        sel["lepcr"] + sel["lowmt_mednj"],        "#slash{E}_{T} [GeV]", NBINS, metbins);
    plotslepcr->addTreeVar("met_lepcr_nbgeq1_lowmt_highnj",      "met",        sel["lepcr"] + sel["lowmt_highnj"],       "#slash{E}_{T} [GeV]", NBINS, metbins);
    plotslepcr->addTreeVar("met_lepcr_nbgeq1_highmt_mednj_nt0",  "met",        sel["lepcr"] + sel["highmt_mednj_nt0"],   "#slash{E}_{T} [GeV]", NBINS, metbins);
    plotslepcr->addTreeVar("met_lepcr_nbgeq1_highmt_highnj_nt0", "met",        sel["lepcr"] + sel["highmt_highnj_nt0"],  "#slash{E}_{T} [GeV]", NBINS, metbins);
    plotslepcr->addTreeVar("met_lepcr_nbgeq1_highmt_nt1_int",    "met",        sel["lepcr"] + sel["highmt_nt1"],         "#slash{E}_{T} [GeV]", NBINS_NT1, metbins_nt1);
  }
  else {
    plotslepcr->addTreeVar("met_lepcr_lowmt_mednj_nb1",             "met",        sel["lepcr"] + sel["lowmt_mednj_nb1"],       "#slash{E}_{T} [GeV]", NBINS, metbins);
    plotslepcr->addTreeVar("met_lepcr_lowmt_mednj_nb2",             "met",        sel["lepcr"] + sel["lowmt_mednj_nb2"],       "#slash{E}_{T} [GeV]", NBINS, metbins);
    plotslepcr->addTreeVar("met_lepcr_lowmt_highnj_nb1",            "met",        sel["lepcr"] + sel["lowmt_highnj_nb1"],      "#slash{E}_{T} [GeV]", NBINS, metbins);
    plotslepcr->addTreeVar("met_lepcr_lowmt_highnj_nb2",            "met",        sel["lepcr"] + sel["lowmt_highnj_nb2"],      "#slash{E}_{T} [GeV]", NBINS, metbins);
    plotslepcr->addTreeVar("met_lepcr_highmt_mednj_nt0_nb1",        "met",        sel["lepcr"] + sel["highmt_mednj_nt0_nb1"],  "#slash{E}_{T} [GeV]", NBINS, metbins);
    plotslepcr->addTreeVar("met_lepcr_highmt_mednj_nt0_nb2",        "met",        sel["lepcr"] + sel["highmt_mednj_nt0_nb2"],  "#slash{E}_{T} [GeV]", NBINS, metbins);
    plotslepcr->addTreeVar("met_lepcr_highmt_highnj_nt0_nb1",       "met",        sel["lepcr"] + sel["highmt_highnj_nt0_nb1"], "#slash{E}_{T} [GeV]", NBINS, metbins);
    plotslepcr->addTreeVar("met_lepcr_highmt_highnj_nt0_nb2",       "met",        sel["lepcr"] + sel["highmt_highnj_nt0_nb2"], "#slash{E}_{T} [GeV]", NBINS, metbins);
    plotslepcr->addTreeVar("met_lepcr_highmt_nt1_nb1_int",          "met",        sel["lepcr"] + sel["highmt_nt1_nb1"],        "#slash{E}_{T} [GeV]", NBINS_NT1, metbins_nt1);
    plotslepcr->addTreeVar("met_lepcr_highmt_nt1_nb2_int",          "met",        sel["lepcr"] + sel["highmt_nt1_nb2"],        "#slash{E}_{T} [GeV]", NBINS_NT1, metbins_nt1);
  }

  plotslepcr->plot();

  cout << "Plotting photon region" << endl;

  if(intBbins) {
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
  }
  else {
    plotsphocr->addTreeVar("met_phocr_nomt_mednj_nb1",            "met",        sel["phocr"] + sel["mednj"] + sel["nb1"],               "#slash{E}_{T} [GeV]", NBINS, metbins);
    plotsphocr->addTreeVar("met_phocr_nomt_mednj_nb2",            "met",        sel["phocr"] + sel["mednj"] + sel["nb2"],               "#slash{E}_{T} [GeV]", NBINS, metbins);
    plotsphocr->addTreeVar("met_phocr_nomt_highnj_nb1",           "met",        sel["phocr"] + sel["highnj"] + sel["nb1"],              "#slash{E}_{T} [GeV]", NBINS, metbins);
    plotsphocr->addTreeVar("met_phocr_nomt_highnj_nb2",           "met",        sel["phocr"] + sel["highnj"] + sel["nb2"],              "#slash{E}_{T} [GeV]", NBINS, metbins);
    plotsphocr->addTreeVar("met_phocr_nomt_mednj_nt0_nb1",        "met",        sel["phocr"] + sel["mednj"] + sel["nt0"] + sel["nb1"],  "#slash{E}_{T} [GeV]", NBINS, metbins);
    plotsphocr->addTreeVar("met_phocr_nomt_mednj_nt0_nb2",        "met",        sel["phocr"] + sel["mednj"] + sel["nt0"] + sel["nb2"],  "#slash{E}_{T} [GeV]", NBINS, metbins);
    plotsphocr->addTreeVar("met_phocr_nomt_highnj_nt0_nb1",       "met",        sel["phocr"] + sel["highnj"] + sel["nt0"] + sel["nb1"], "#slash{E}_{T} [GeV]", NBINS, metbins);
    plotsphocr->addTreeVar("met_phocr_nomt_highnj_nt0_nb2",       "met",        sel["phocr"] + sel["highnj"] + sel["nt0"] + sel["nb2"], "#slash{E}_{T} [GeV]", NBINS, metbins);
    plotsphocr->addTreeVar("met_phocr_nomt_nt1_nb1",              "met",        sel["phocr"] + sel["nt1"] + sel["nb1"],                 "#slash{E}_{T} [GeV]", NBINS_NT1, metbins_nt1);
    plotsphocr->addTreeVar("met_phocr_nomt_nt1_nb2",              "met",        sel["phocr"] + sel["nt1"] + sel["nb2"],                 "#slash{E}_{T} [GeV]", NBINS_NT1, metbins_nt1);

    plotsphocr->addTreeVar("met_phocr_lowmt_mednj_nb1",             "met",        sel["phocr"] + sel["lowmt_mednj_nb1"],       "#slash{E}_{T} [GeV]", NBINS, metbins);
    plotsphocr->addTreeVar("met_phocr_lowmt_mednj_nb2",             "met",        sel["phocr"] + sel["lowmt_mednj_nb2"],       "#slash{E}_{T} [GeV]", NBINS, metbins);
    plotsphocr->addTreeVar("met_phocr_lowmt_highnj_nb1",            "met",        sel["phocr"] + sel["lowmt_highnj_nb1"],      "#slash{E}_{T} [GeV]", NBINS, metbins);
    plotsphocr->addTreeVar("met_phocr_lowmt_highnj_nb2",            "met",        sel["phocr"] + sel["lowmt_highnj_nb2"],      "#slash{E}_{T} [GeV]", NBINS, metbins);
    plotsphocr->addTreeVar("met_phocr_highmt_mednj_nt0_nb1",        "met",        sel["phocr"] + sel["highmt_mednj_nt0_nb1"],  "#slash{E}_{T} [GeV]", NBINS, metbins);
    plotsphocr->addTreeVar("met_phocr_highmt_mednj_nt0_nb2",        "met",        sel["phocr"] + sel["highmt_mednj_nt0_nb2"],  "#slash{E}_{T} [GeV]", NBINS, metbins);
    plotsphocr->addTreeVar("met_phocr_highmt_highnj_nt0_nb1",       "met",        sel["phocr"] + sel["highmt_highnj_nt0_nb1"], "#slash{E}_{T} [GeV]", NBINS, metbins);
    plotsphocr->addTreeVar("met_phocr_highmt_highnj_nt0_nb2",       "met",        sel["phocr"] + sel["highmt_highnj_nt0_nb2"], "#slash{E}_{T} [GeV]", NBINS, metbins);
    plotsphocr->addTreeVar("met_phocr_highmt_nt1_nb1",              "met",        sel["phocr"] + sel["highmt_nt1_nb1"],        "#slash{E}_{T} [GeV]", NBINS_NT1, metbins_nt1);
    plotsphocr->addTreeVar("met_phocr_highmt_nt1_nb2",              "met",        sel["phocr"] + sel["highmt_nt1_nb2"],        "#slash{E}_{T} [GeV]", NBINS_NT1, metbins_nt1);
  }

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

  vector<TString> srbins;
  if (dolownj)
    srbins = {"lowmt_mednj_nb1", "lowmt_mednj_nb2", "highmt_mednj_nt0_nb1", "highmt_mednj_nt0_nb2", "highmt_nt1_nb1", "highmt_nt1_nb2"};
  else
    srbins = {"lowmt_mednj_nb1", "lowmt_mednj_nb2", "lowmt_highnj_nb1", "lowmt_highnj_nb2",
        "highmt_mednj_nt0_nb1", "highmt_mednj_nt0_nb2", "highmt_highnj_nt0_nb1", "highmt_highnj_nt0_nb2",
        "highmt_nt1_nb1", "highmt_nt1_nb2"};

  if(intBbins) {
    qcdcrtosr["lowmt_mednj_nb1"]        = "nbgeq1_lowmt_mednj";
    qcdcrtosr["lowmt_mednj_nb2"]        = "nbgeq1_lowmt_mednj";
    qcdcrtosr["lowmt_highnj_nb1"]       = "nbgeq1_lowmt_highnj";
    qcdcrtosr["lowmt_highnj_nb2"]       = "nbgeq1_lowmt_highnj";
    qcdcrtosr["highmt_mednj_nt0_nb1"]   = "nbgeq1_highmt_mednj_nt0";
    qcdcrtosr["highmt_mednj_nt0_nb2"]   = "nbgeq1_highmt_mednj_nt0";
    qcdcrtosr["highmt_highnj_nt0_nb1"]  = "nbgeq1_highmt_highnj_nt0";
    qcdcrtosr["highmt_highnj_nt0_nb2"]  = "nbgeq1_highmt_highnj_nt0";
    qcdcrtosr["highmt_nt1_nb1"]         = "nbgeq1_highmt_nt1";
    qcdcrtosr["highmt_nt1_nb2"]         = "nbgeq1_highmt_nt1";

    lepcrtosr = qcdcrtosr;
    lepcrtosr["highmt_nt1_nb1"]         = "nbgeq1_highmt_nt1_int";
    lepcrtosr["highmt_nt1_nb2"]         = "nbgeq1_highmt_nt1_int";

    phocrtosr = qcdcrtosr;
    // integrating in met
    // phocrtosr["highmt_nt1_nb1"]         = "nbgeq1_highmt_nt1_int";
    // phocrtosr["highmt_nt1_nb2"]         = "nbgeq1_highmt_nt1_int";
  }
  else {
    qcdcrtosr["lowmt_mednj_nb1"]        = "lowmt_mednj_nb1"       ;
    qcdcrtosr["lowmt_mednj_nb2"]        = "lowmt_mednj_nb2"       ;
    qcdcrtosr["lowmt_highnj_nb1"]       = "lowmt_highnj_nb1"      ;
    qcdcrtosr["lowmt_highnj_nb2"]       = "lowmt_highnj_nb2"      ;
    qcdcrtosr["highmt_mednj_nt0_nb1"]   = "highmt_mednj_nt0_nb1"  ;
    qcdcrtosr["highmt_mednj_nt0_nb2"]   = "highmt_mednj_nt0_nb2"  ;
    qcdcrtosr["highmt_highnj_nt0_nb1"]  = "highmt_highnj_nt0_nb1" ;
    qcdcrtosr["highmt_highnj_nt0_nb2"]  = "highmt_highnj_nt0_nb2" ;
    qcdcrtosr["highmt_nt1_nb1"]         = "highmt_nt1_nb1"        ;
    qcdcrtosr["highmt_nt1_nb2"]         = "highmt_nt1_nb2"        ;

    lepcrtosr = qcdcrtosr;
    lepcrtosr["highmt_nt1_nb1"]         = "highmt_nt1_nb1_int";
    lepcrtosr["highmt_nt1_nb2"]         = "highmt_nt1_nb2_int";

    phocrtosr = qcdcrtosr;
    // integrating in met
    // phocrtosr["highmt_nt1_nb1"]         = "highmt_nt1_nb1_int";
    // phocrtosr["highmt_nt1_nb2"]         = "highmt_nt1_nb2_int";
  }

  for (const auto &bin : srbins)      zllcrtosr[bin] = bin.Contains("nb1") ? "nb1" : "nb2";

  vector<TString> qcdbkgsamples = {"ttbarplusw","znunu","ttZ"};

  TH1F* data           = (true || dolownj || dolowmet) ? getSRHist (file0l, "data", "sr",      srbins) : 0;
  TH1F* lostlep        = getLLPred (file0l, filelepcr, "sr",      "lepcr",      srbins, lepcrtosr);
  TH1F* znunu          = getZPred  (file0l, filephocr, filezllcr, "sr", "phocr", "zllcr", srbins, phocrtosr, zllcrtosr);
  TH1F* qcd            = getQCDPred (file0l, file0lnovetoes, "sr", "qcdcr", srbins, qcdcrtosr, qcdbkgsamples);
  //TH1F* ttz          = getTTZPred(file0l, filettzcr, "sr", "ttzcr", srbins, usettzsf);
  TH1F* ttz            = getTTZPred(file0l, "sr", srbins);


  if(ext.BeginsWith("/")) ext.ReplaceAll("/","_");
  TString fname = "0lsearch";
  if(dolownj) fname += "_lownj";
  if(dolowmet) fname += "_lowmet";
  TFile* srpred = new TFile(fname+ext+".root","RECREATE");
  srpred->cd();

  cout << "\n-----------------------" << endl;
  cout << "Saving Final Prediction" << endl;
  cout << "For " << region << " region" << endl;
  if(true || dolownj || dolowmet) cout << "Data \t || LostLep \t | Znunu \t | QCD \t | ttZ \t || Total Bkg." << endl;
  else cout << "LostLep \t | Znunu \t | QCD \t | ttZ \t || Total Bkg." << endl;

  lostlep->GetXaxis()->SetTitle("Search region");

  for(int ibin = 1; ibin < lostlep->GetNbinsX()+1; ++ibin) {
    double nkgtot = lostlep->GetBinContent(ibin) + znunu->GetBinContent(ibin) + qcd->GetBinContent(ibin) + ttz->GetBinContent(ibin);
    double nbkgtotuncsq = pow(lostlep->GetBinError(ibin),2) + pow(znunu->GetBinError(ibin),2) + pow(qcd->GetBinError(ibin),2) + pow(ttz->GetBinError(ibin),2);
    cout << "Bin " << ibin << endl;
    if(true || dolownj || dolowmet) cout << data->GetBinContent(ibin) << " +/- " << data->GetBinError(ibin) << "\t || ";
    cout << lostlep->GetBinContent(ibin) << " +/- " << lostlep->GetBinError(ibin) << "\t | ";
    cout << znunu->GetBinContent(ibin) << " +/- " << znunu->GetBinError(ibin) << "\t | ";
    cout << qcd->GetBinContent(ibin) << " +/- " << qcd->GetBinError(ibin) << "\t | ";
    cout << ttz->GetBinContent(ibin) << " +/- " << ttz->GetBinError(ibin) << "\t || ";
    cout << nkgtot << " +/- " << sqrt(nbkgtotuncsq) << endl;
  }

  if(true || dolownj || dolowmet) data->Write();
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
  if(plotlog) plots->setLogy();
  plots->setHeaderText("#sqrt{s} = 13 TeV",TString::Format("%4.2f fb^{-1}",stof(string(lumistr.Data()))),"");
  plots->setHeaderPosition(0.16, 0.93);
  plots->setColor("lostlep_pred_sr",StyleTools::color_ttbar);
  plots->setMaxScale(2.0);
  plots->setCanvasSize(1000, 600);

  vector<TString> labels, names;
  if(true || dolownj || dolowmet) {
    plots->setRatioPlot();
    labels = {"Data", "t#bar{t}Z", "QCD", "Z#rightarrow#nu#nu", "t#bar{t}/W"};
    names = {"data_sr", "ttZ_pred_sr", "qcd_pred_sr", "znunu_pred_sr", "lostlep_pred_sr"};
    plots->setDataName("data_sr");
    plots->setUsePoisson();
    plots->setUncertaintyBand();
    plots->addCompSet("datavspred",names,labels);
  } else {
    labels = {"t#bar{t}Z", "QCD", "Z#rightarrow#nu#nu", "t#bar{t}/W"};
    names = {"ttZ_pred_sr", "qcd_pred_sr", "znunu_pred_sr", "lostlep_pred_sr"};
    plots->addCompSet("pred",names,labels);
  }

  plots->plot();

  delete plots;

}
