#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#endif

enum SysVars { NOMINAL, VARUP, VARDOWN };

vector<TString> samples = {"data", "ttbarplusw","ttZ","znunu", "qcd"};

map<TString,TString> sel;
typedef map<TString,TString> BinMap;
typedef map<TString,TH1F*> HistMap;

PlotStuff* setupPlots(const TString conf, const TString inputdir, const TString outputdir, const TString wgtvar, const bool plotlog, const TString format, const TString lumistr, const TString outfilename) {

  cout << "initializing plots for " << conf << " with input directory " << inputdir << endl;

  PlotStuff* plots = new PlotStuff(conf, inputdir, outputdir);
  plots->setRatioPlot();
  plots->setPlotOverflow(1);
  plots->setFormat(format);
  if(plotlog)
    plots->setLogy();
  plots->setWgtVar(wgtvar);
  plots->setHeaderText("#sqrt{s} = 13 TeV",lumistr + " pb^{-1}","");
  plots->setHeaderPosition(0.2, 0.93);
  plots->setOutfile(outfilename);
  plots->setWriteHists();

  return plots;

}

void removeZeroes(TH1F* hist) {

  for(int ibin = 1; ibin < hist->GetNbinsX()+1; ++ibin) {
    double bincontent = hist->GetBinContent(ibin);
    double binerror = hist->GetBinError(ibin);
    if(bincontent < 0.0) {
      hist->SetBinContent(ibin, 0);
      hist->SetBinError(ibin, binerror + sqrt(bincontent*bincontent));
      cout << "\nBin " << ibin << " for " << hist->GetName() << " had " << bincontent << " +/- " << binerror << "; set to " << hist->GetBinContent(ibin) << " +/- " << hist->GetBinError(ibin) << endl;
    }
  }

}

TH1F* getSRHist(TFile* file, const TString sample, const TString region, vector<TString> bins, const TString basevar = "met") {

  cout << "\nProducing histogram for " << sample << " in the " << region << " region for bins: ";
  for(auto bin : bins)
    cout << bin << " ";
  cout << endl;

  vector<TH1F*> srhists;

  int nbins = 0;

  for(auto bin : bins) {
    TH1F* hist = (TH1F*)file->Get(basevar + "_" + region + "_" + bin + "_" + sample);
    assert(hist);
    nbins += hist->GetNbinsX();
    srhists.push_back(hist);
    cout << hist->GetName() << " in bin " << bin << ": ";
    for(int ibin = 1; ibin < hist->GetNbinsX()+1; ++ibin) {
      cout << hist->GetBinContent(ibin) << " +/- " << hist->GetBinError(ibin) << "\t";
    }
    cout << endl;
  }

  TH1F* srhist = new TH1F(sample+"_"+region, "", nbins, 0, nbins);
  srhist->Sumw2();
  int srbin = 1;

  for(auto* h : srhists) {
    for(int ibin = 1; ibin < h->GetNbinsX()+1; ++ibin) {
      srhist->SetBinContent(srbin, h->GetBinContent(ibin));
      srhist->SetBinError(srbin, h->GetBinError(ibin));
      srbin++;
    }
  }
  cout << srhist->GetName() << ": ";
  for(int ibin = 1; ibin < srhist->GetNbinsX()+1; ++ibin) {
    cout << srhist->GetBinContent(ibin) << " +/- " << srhist->GetBinError(ibin) << "\t";
  }
  cout << endl;

  return srhist;

}

TH1F* getLLPred(TFile* file0l, TFile* filelepcr, const TString region, const TString crname, vector<TString> bins, BinMap crtosrmap) {

  cout << "\nGetting the Lost Lepton prediction in the " << region << " region. CR label is " << crname << endl;

  TH1F* lostlep_sr = getSRHist(file0l, "ttbarplusw", region, bins);
  vector<TString> crbins;
  for(auto bin : bins) {
    cout << "\nCR bin for " << bin << ": " << crtosrmap[bin] << endl;
    crbins.push_back(crtosrmap[bin]);
  }
  TH1F* lostlep_cr = getSRHist(filelepcr, "ttbarplusw", crname, crbins);
  TH1F* data_cr = getSRHist(filelepcr, "data", crname, crbins);

  TH1F* lostlep_tf = (TH1F*)lostlep_sr->Clone("lostlep_tf_" + region);
  lostlep_tf->Divide(lostlep_cr);

  cout << "\nTransfer factors: ";
  for(int ibin = 1; ibin < lostlep_tf->GetNbinsX()+1; ++ibin) {
    cout << lostlep_tf->GetBinContent(ibin) << " +/- " << lostlep_tf->GetBinError(ibin) << "\t";
  }
  cout << endl;

  TH1F* lostlep_pred = (TH1F*)lostlep_tf->Clone("lostlep_pred_" + region);
  lostlep_pred->Multiply(data_cr);

  cout << "\nLost lepton prediction: ";
  for(int ibin = 1; ibin < lostlep_pred->GetNbinsX()+1; ++ibin) {
    cout << lostlep_pred->GetBinContent(ibin) << " +/- " << lostlep_pred->GetBinError(ibin) << "\t";
  }
  cout << endl;

  return lostlep_pred;

}

TH1F* getQCDPred(TFile* file, const TString region, const TString crname, vector<TString> bins, BinMap crtosrmap) {

  cout << "\nGetting the QCD prediction in the " << region << " region. CR label is " << crname << endl;

  TH1F* qcd_sr = getSRHist(file, "qcd", region, bins);
  vector<TString> crbins;
  for(auto bin : bins) {
    cout << "\nCR bin for " << bin << ": " << crtosrmap[bin] << endl;
    crbins.push_back(crtosrmap[bin]);
  }
  TH1F* qcd_cr = getSRHist(file, "qcd", crname, crbins);
  TH1F* data_cr = getSRHist(file, "data", crname, crbins);
  TH1F* data_less_nonqcd_cr = (TH1F*)data_cr->Clone("data_less_nonqcd_" + crname);
  for(auto sname : samples) {
    if(sname == "qcd" || sname == "data") continue;
    TH1F* mc_cr = getSRHist(file, sname, crname, crbins);
    data_less_nonqcd_cr->Add(mc_cr, -1);
  }

  TH1F* qcd_tf = (TH1F*)qcd_sr->Clone("qcd_tf_" + region);
  qcd_tf->Divide(qcd_cr);

  cout << "\nTransfer factors: ";
  for(int ibin = 1; ibin < qcd_tf->GetNbinsX()+1; ++ibin) {
    cout << qcd_tf->GetBinContent(ibin) << " +/- " << qcd_tf->GetBinError(ibin) << "\t";
  }
  cout << endl;

  TH1F* qcd_pred = (TH1F*)qcd_tf->Clone("qcd_pred_" + region);
  qcd_pred->Multiply(data_less_nonqcd_cr);

  cout << "\nQCD prediction: ";
  for(int ibin = 1; ibin < qcd_pred->GetNbinsX()+1; ++ibin) {
    cout << qcd_pred->GetBinContent(ibin) << " +/- " << qcd_pred->GetBinError(ibin) << "\t";
  }
  cout << endl;

  return qcd_pred;

}

TH1F* getZPred(TFile* file0l, TFile* filephocr, TFile* filezeecr, TFile* filezmmcr, const TString region, const TString phocrname, const TString zeecrname, const TString zmmcrname, vector<TString> bins, BinMap phocrtosrmap, BinMap zllcrtosrmap) {

  cout << "\nGetting the Z prediction in the " << region << " region. CR labels are " << phocrname << ", " << zeecrname << ", " << zmmcrname << endl;

  TH1F* znunu_sr = getSRHist(file0l, "znunu", region, bins);
  vector<TString> phocrbins, zllcrbins;
  for(auto bin : bins) {
    cout << "\nPhoton CR bin for " << bin << ": " << phocrtosrmap[bin] << endl;
    cout << "\nZLL CR bin for " << bin << ": " << zllcrtosrmap[bin] << endl;
    phocrbins.push_back(phocrtosrmap[bin]);
    zllcrbins.push_back(zllcrtosrmap[bin]);
  }
  TH1F* photon_phocr = getSRHist(filephocr, "photon", phocrname, phocrbins);
  TH1F* data_phocr = getSRHist(filephocr, "data", phocrname, phocrbins);

  cout << "\nData yield in Photon CR: " << data_phocr->Integral(0, data_phocr->GetNbinsX()+1) << endl;
  cout << "\nMC yield in Photon CR: " << photon_phocr->Integral(0, photon_phocr->GetNbinsX()+1) << endl;

  photon_phocr->Scale(data_phocr->Integral(0, data_phocr->GetNbinsX()+1) / photon_phocr->Integral(0, photon_phocr->GetNbinsX()+1));

  cout << "\nMC yield in Photon CR after normalizing to data: " << photon_phocr->Integral(0, photon_phocr->GetNbinsX()+1) << endl;

  TH1F* znunu_pred = (TH1F*)znunu_sr->Clone("znunu_pred_" + region);
  znunu_pred->Multiply(data_phocr);
  znunu_pred->Divide(photon_phocr);

  cout << "\nZnunu MC x photon shape weights: ";
  for(int ibin = 1; ibin < znunu_pred->GetNbinsX()+1; ++ibin) {
    cout << znunu_pred->GetBinContent(ibin) << " +/- " << znunu_pred->GetBinError(ibin) << "\t";
  }
  cout << endl;

//  TH1F* zll_zeecr = getSRHist(filezeecr, "zll", zeecrname, zllcrbins);
//  TH1F* zll_zmmcr = getSRHist(filezmmcr, "zll", zmmcrname, zllcrbins);
//  TH1F* ttbar_zeecr = getSRHist(filezeecr, "ttbar", zeecrname, zllcrbins);
//  TH1F* ttbar_zmmcr = getSRHist(filezmmcr, "ttbar", zmmcrname, zllcrbins);
//  TH1F* data_zeecr = getSRHist(filezeecr, "data_ee", zeecrname, zllcrbins);
//  TH1F* data_zmmcr = getSRHist(filezmmcr, "data_mm", zmmcrname, zllcrbins);
//  TH1F* zll_zllcr = (TH1F*)zll_zeecr->Clone("zll_zllcr");
//  zll_zllcr->Add(zll_zmmcr);
//  TH1F* ttbar_zllcr = (TH1F*)ttbar_zeecr->Clone("ttbar_zllcr");
//  ttbar_zllcr->Add(ttbar_zmmcr);
//  TH1F* data_zllcr = (TH1F*)data_zeecr->Clone("data_zllcr");
//  data_zllcr->Add(data_zmmcr);
//  TH1F* data_less_ttbar_zllcr = (TH1F*)data_zllcr->Clone("data_less_ttbar_zllcr_" + region);
//  data_less_ttbar_zllcr->Add(ttbar_zllcr, -1);
//
//  cout << "\nTTbar subtracted data in ZLL CR: ";
//  for(int ibin = 1; ibin < data_less_ttbar_zllcr->GetNbinsX()+1; ++ibin) {
//    cout << data_less_ttbar_zllcr->GetBinContent(ibin) << " +/- " << data_less_ttbar_zllcr->GetBinError(ibin) << "\t";
//  }
//  cout << endl;
//
//  TH1F* zll_sf = (TH1F*)data_less_ttbar_zllcr->Clone("zll_sf_" + region);
//  zll_sf->Divide(zll_zllcr);
//
//  cout << "\nZ scale factors from ZLL CR: ";
//  for(int ibin = 1; ibin < zll_sf->GetNbinsX()+1; ++ibin) {
//    cout << zll_sf->GetBinContent(ibin) << " +/- " << zll_sf->GetBinError(ibin) << "\t";
//  }
//  cout << endl;
//
//  double sf_1b = zll_sf->GetBinContent(1);
//  double sf_2b = zll_sf->GetBinContent(2);
//  double sf_1b_unc = zll_sf->GetBinError(1);
//  double sf_2b_unc = zll_sf->GetBinError(2);

  // FIXME: put zll_sf by hand for this moment. These values are for 2<=nj<=4.
  double sf_1b = 1.01;
  double sf_2b = 1.09;
  double sf_1b_unc = 0.17;
  double sf_2b_unc = 0.25;

  for(int ibin = 1; ibin < znunu_pred->GetNbinsX()+1; ++ibin) {
    double bincontent = znunu_pred->GetBinContent(ibin);
    double binerror = znunu_pred->GetBinError(ibin);
    if(ibin <= znunu_pred->GetNbinsX()/2) {
      znunu_pred->SetBinContent(ibin, bincontent * sf_1b);
      znunu_pred->SetBinError(ibin, znunu_pred->GetBinContent(ibin) * sqrt((sf_1b_unc*sf_1b_unc/(sf_1b*sf_1b)) + (binerror*binerror/(bincontent*bincontent))));
    } else {
      znunu_pred->SetBinContent(ibin, bincontent * sf_2b);
      znunu_pred->SetBinError(ibin, znunu_pred->GetBinContent(ibin) * sqrt((sf_2b_unc*sf_2b_unc/(sf_2b*sf_2b)) + (binerror*binerror/(bincontent*bincontent))));
    }
  }

  cout << "\nZnunu MC x photon shape weights x ZLL scale factor: ";
  for(int ibin = 1; ibin < znunu_pred->GetNbinsX()+1; ++ibin) {
    cout << znunu_pred->GetBinContent(ibin) << " +/- " << znunu_pred->GetBinError(ibin) << "\t";
  }
  cout << endl;

  return znunu_pred;

}

/*TH1F* getZPred(TFile* file0l, TFile* filephocr, TFile* filezllcr, const TString region, const TString phocrname, const TString zllcrname, vector<TString> bins, BinMap phocrtosrmap, BinMap zllcrtosrmap) {

  cout << "\nGetting the Z prediction in the " << region << " region. CR labels are " << phocrname << ", " << zllcrname << endl;

  TH1F* znunu_sr = getSRHist(file0l, "znunu", region, bins);
  vector<TString> phocrbins, zllcrbins;
  for(auto bin : bins) {
    cout << "\nPhoton CR bin for " << bin << ": " << phocrtosrmap[bin] << endl;
    cout << "\nZLL CR bin for " << bin << ": " << zllcrtosrmap[bin] << endl;
    phocrbins.push_back(phocrtosrmap[bin]);
    zllcrbins.push_back(zllcrtosrmap[bin]);
  }
  TH1F* photon_phocr = getSRHist(filephocr, "photon", phocrname, phocrbins);
  TH1F* data_phocr = getSRHist(filephocr, "data", phocrname, phocrbins);

  cout << "\nData yield in Photon CR: " << data_phocr->Integral(0, data_phocr->GetNbinsX()+1) << endl;
  cout << "\nMC yield in Photon CR: " << photon_phocr->Integral(0, photon_phocr->GetNbinsX()+1) << endl;

  photon_phocr->Scale(data_phocr->Integral(0, data_phocr->GetNbinsX()+1) / photon_phocr->Integral(0, photon_phocr->GetNbinsX()+1));

  cout << "\nMC yield in Photon CR after normalizing to data: " << photon_phocr->Integral(0, photon_phocr->GetNbinsX()+1) << endl;

  cout << "\nZnunu MC x photon shape weights: ";
  for(int ibin = 1; ibin < znunu_pred->GetNbinsX()+1; ++ibin) {
    cout << znunu_pred->GetBinContent(ibin) << " +/- " << znunu_pred->GetBinError(ibin) << "\t";
  }
  cout << endl;

  TH1F* zll_zllcr = getSRHist(filezllcr, "zll", zllcrname, zllcrbins);
  TH1F* ttbar_zllcr = getSRHist(filezllcr, "ttbar", zllcrname, zllcrbins);
  TH1F* data_zllcr = getSRHist(filezllcr, "data", zllcrname, zllcrbins);
  TH1F* data_less_ttbar_zllcr = (TH1F*)data_zllcr->Clone("data_less_ttbar_zllcr_" + region);
  data_less_ttbar_zllcr->Add(ttbar_zllcr, -1);

  cout << "\nTTbar subtracted data in ZLL CR: ";
  for(int ibin = 1; ibin < data_less_ttbar_zllcr->GetNbinsX()+1; ++ibin) {
    cout << data_less_ttbar_zllcr->GetBinContent(ibin) << " +/- " << data_less_ttbar_zllcr->GetBinError(ibin) << "\t";
  }
  cout << endl;

  TH1F* zll_sf = (TH1F*)data_less_ttbar_zllcr->Clone("zll_sf_" + region);
  zll_sf->Divide(zll_zllcr);

  cout << "\nZ scale factors from ZLL CR: ";
  for(int ibin = 1; ibin < zll_sf->GetNbinsX()+1; ++ibin) {
    cout << zll_sf->GetBinContent(ibin) << " +/- " << zll_sf->GetBinError(ibin) << "\t";
  }
  cout << endl;

  double sf_1b = zll_sf->GetBinContent(1);
  double sf_2b = zll_sf->GetBinContent(2);
  double sf_1b_unc = zll_sf->GetBinError(1);
  double sf_2b_unc = zll_sf->GetBinError(2);

  TH1F* znunu_pred = (TH1F*)znunu_sr->Clone("znunu_pred_" + region);
  znunu_pred->Multiply(data_phocr);
  znunu_pred->Divide(photon_phocr);

  for(int ibin = 1; ibin < znunu_pred->GetNbinsX()+1; ++ibin) {
    double bincontent = znunu_pred->GetBinContent(ibin);
    double binerror = znunu_pred->GetBinError(ibin);
    if(ibin <= znunu_pred->GetNbinsX()/2) {
      znunu_pred->SetBinContent(ibin, bincontent * sf_1b);
      znunu_pred->SetBinError(ibin, znunu_pred->GetBinContent(ibin) * sqrt((sf_1b_unc*sf_1b_unc/(sf_1b*sf_1b)) + (binerror*binerror/(bincontent*bincontent))));
    } else {
      znunu_pred->SetBinContent(ibin, bincontent * sf_2b);
      znunu_pred->SetBinError(ibin, znunu_pred->GetBinContent(ibin) * sqrt((sf_2b_unc*sf_2b_unc/(sf_2b*sf_2b)) + (binerror*binerror/(bincontent*bincontent))));
    }
  }

  cout << "\nZnunu MC x photon shape weights x ZLL scale factor: ";
  for(int ibin = 1; ibin < znunu_pred->GetNbinsX()+1; ++ibin) {
    cout << znunu_pred->GetBinContent(ibin) << " +/- " << znunu_pred->GetBinError(ibin) << "\t";
  }
  cout << endl;

  return znunu_pred;

}*/

void getZeroLeptonPrediction(const TString defaultdir  = "trees_0lepbkgest_101415",
                             const TString varupdir    = "trees/varup",
                             const TString vardowndir  = "trees/vardown",
                             const TString outputdir   = "plots_bkgest_101415",
                             const TString srconf      = "run0lepmine.conf",
                             const TString phocrconf   = "runphotoncrmine.conf",
                             //const TString zllcrconf   = "runzllcrmine.conf",
                             const TString zeecrconf   = "runzeecrmine.conf",
                             const TString zmmcrconf   = "runzmmcrmine.conf",
                             const TString lumistr     = "1264",
                             const TString crlumistr   = "1264",
                             const TString zeecrlumistr= "1264",
                             //const TString region      = "sr",
                             const TString region      = "srlownj",
                             const TString format      = "png",
                             const bool    dolownj     = true,
                             const unsigned int sysvar = NOMINAL,
                             const bool    plotlog     = false)
{
  gSystem->mkdir(outputdir, true);

  TString basewgt    = lumistr + "*0.001*weight*puWeight";
  TString basewgtcr  = crlumistr + "*0.001*weight*puWeight";
  TString basewgtcrzee  = zeecrlumistr + "*0.001*weight*puWeight";
  TString lepvetowgt = basewgt + "*lepvetoweight";
  TString lepselwgt  = basewgt + "*lepselweight";

  sel["trig"]         = "passjson && passdijetmet && j2pt>75 && met>200 && passcscflt && passeebadscflt && passhbheflttight";
  sel["trigpho"]      = "passjson && passtrigphoton165 && j2pt>75 && met>200 && passcscflt && passeebadscflt && passhbheflttight";
  sel["trigzll"]      = "passjson && ((iselectron && passtrige17e12) || (!iselectron && (passtrigmu17mu8 || passtrigmu17tkmu8))) && j2pt>75 && met>200 && dilepmass > 80 && dilepmass < 100";
  sel["trigzee"]      = "passjson && iselectron && passtrige17e12 && j2pt>75 && met>200 && dilepmass > 80 && dilepmass < 100";
  sel["trigzmm"]      = "passjson && !iselectron && (passtrigmu17mu8 || passtrigmu17tkmu8) && j2pt>75 && met>200 && dilepmass > 80 && dilepmass < 100";
  sel["vetoes"]       = " && ((nvetolep==0 && nvetohpstaus==0) || (ismc && (ngoodgenele>0 || ngoodgenmu>0 || npromptgentau>0)))";
  sel["lepsel"]       = " && nvetolep>0";
  sel["njets"]        = dolownj ? " && njets>=2 && njets<5 && nbjets>=1 && nlbjets>=2" : " && njets>=5 && nbjets>=1 && nlbjets>=2";
  sel["dphij123"]     = " && dphij12met>1 && dphij3met>0.5";
  sel["dphij3"]       = " && dphij3met>0.5";
  sel["dphij123inv"]  = " && dphij12met<0.15 && dphij3met>0.5";
  sel["mtb"]          = " && mtcsv12met>175";
  sel["lepcrsel"]     = " && nvetolep>0 && mtlepmet<100";
  sel["sr"]           = sel["trig"] + sel["vetoes"]   + sel["njets"]    + sel["dphij123"] + sel["mtb"];
  sel["lepcr"]        = sel["trig"] + sel["lepcrsel"] + sel["njets"]    + sel["dphij123"] + sel["mtb"];
  sel["qcdcr"]        = sel["trig"] + sel["vetoes"]   + sel["njets"]    + sel["dphij123inv"] + sel["mtb"];
  sel["qcdincl"]      = sel["trig"] + sel["vetoes"]   + sel["njets"]    + sel["dphij3"] + sel["mtb"];
  sel["phocr"]        = sel["trigpho"] + sel["njets"] + sel["dphij123"] + sel["mtb"];
  sel["zllcr"]        = sel["trigzll"] + sel["njets"];
  sel["zeecr"]        = sel["trigzee"] + sel["njets"];
  sel["zmmcr"]        = sel["trigzmm"] + sel["njets"];
  sel["nb1"]          = " && nbjets==1";
  sel["nb2"]          = " && nbjets>=2";
  sel["nb1_nt0"]      = " && nbjets==1 && ncttstd==0";
  sel["nb2_nt0"]      = " && nbjets>=2 && ncttstd==0";
  sel["nb1_nt1"]      = " && nbjets==1 && ncttstd>0";
  sel["nb2_nt1"]      = " && nbjets>=2 && ncttstd>0";

  double metbins[]    = {200.0, 225.0, 250.0, 300.0, 400.0, 600.0};

  TString inputdir    = sysvar == NOMINAL ? defaultdir : (sysvar == VARUP ? varupdir : vardowndir);

  PlotStuff* plots0l    = setupPlots(srconf,    inputdir, outputdir, lepvetowgt, plotlog, format, lumistr, "output_0l.root");
  PlotStuff* plotslepcr = setupPlots(srconf,    inputdir, outputdir, lepselwgt, plotlog, format, lumistr, "output_lepcr.root");
  PlotStuff* plotsphocr = setupPlots(phocrconf, inputdir+"/photoncr", outputdir, basewgtcr, plotlog, format, crlumistr, "output_phocr.root");
  //PlotStuff* plotszllcr = setupPlots(zllcrconf, inputdir+"/zllcr", outputdir, basewgtcr, plotlog, format, crlumistr, "output_zllcr.root");
  PlotStuff* plotszeecr = setupPlots(zeecrconf, inputdir+"/zllcr", outputdir, basewgtcr, plotlog, format, zeecrlumistr, "output_zeecr.root");
  PlotStuff* plotszmmcr = setupPlots(zmmcrconf, inputdir+"/zllcr", outputdir, basewgtcr, plotlog, format, crlumistr, "output_zmmcr.root");

  cout << "Plotting 0lepton region" << endl;

  plots0l->addTreeVar("dphij12met_qcdincl",         "dphij12met",  sel["qcdincl"],                  "min(#Delta#phi(j1,#slash{E}_{T}),#Delta#phi(j2,#slash{E}_{T}))", 21, 0, 3.15);
  plots0l->addTreeVar("dphij12met_qcdincl_nb1",     "dphij12met",  sel["qcdincl"] + sel["nb1"],     "min(#Delta#phi(j1,#slash{E}_{T}),#Delta#phi(j2,#slash{E}_{T}))", 63, 0, 3.15);
  plots0l->addTreeVar("dphij12met_qcdincl_nb2",     "dphij12met",  sel["qcdincl"] + sel["nb2"],     "min(#Delta#phi(j1,#slash{E}_{T}),#Delta#phi(j2,#slash{E}_{T}))", 63, 0, 3.15);
  plots0l->addTreeVar("dphij12met_qcdincl_nb1_nt0", "dphij12met",  sel["qcdincl"] + sel["nb1_nt0"], "min(#Delta#phi(j1,#slash{E}_{T}),#Delta#phi(j2,#slash{E}_{T}))", 63, 0, 3.15);
  plots0l->addTreeVar("dphij12met_qcdincl_nb2_nt0", "dphij12met",  sel["qcdincl"] + sel["nb2_nt0"], "min(#Delta#phi(j1,#slash{E}_{T}),#Delta#phi(j2,#slash{E}_{T}))", 63, 0, 3.15);
  plots0l->addTreeVar("dphij12met_qcdincl_nb1_nt1", "dphij12met",  sel["qcdincl"] + sel["nb1_nt1"], "min(#Delta#phi(j1,#slash{E}_{T}),#Delta#phi(j2,#slash{E}_{T}))", 63, 0, 3.15);
  plots0l->addTreeVar("dphij12met_qcdincl_nb2_nt1", "dphij12met",  sel["qcdincl"] + sel["nb2_nt1"], "min(#Delta#phi(j1,#slash{E}_{T}),#Delta#phi(j2,#slash{E}_{T}))", 63, 0, 3.15);

  plots0l->addTreeVar("met_sr_nb1",                 "met",         sel["sr"] + sel["nb1"],      "#slash{E}_{T} [GeV]", 5, metbins);
  plots0l->addTreeVar("met_sr_nb2",                 "met",         sel["sr"] + sel["nb2"],      "#slash{E}_{T} [GeV]", 5, metbins);

  plots0l->addTreeVar("met_qcdincl_nbgeq1",         "met",         sel["qcdincl"],              "#slash{E}_{T} [GeV]", 5, metbins);
  plots0l->addTreeVar("met_qcdcr_nbgeq1",           "met",         sel["qcdcr"],                "#slash{E}_{T} [GeV]", 5, metbins);

  plots0l->plot();

  cout << "Plotting 1lepton region" << endl;

  plotslepcr->addTreeVar("nvetomu_lepcr",           "nvetomu",     sel["lepcr"],                "Number of Muons", 3, -0.5, 2.5);
  plotslepcr->addTreeVar("nvetoele_lepcr",          "nvetolele",   sel["lepcr"],                "Number of Electrons", 3, -0.5, 2.5);
  plotslepcr->addTreeVar("nvetohpstau_lepcr",       "nvetohpstaus",sel["lepcr"],                "Number of Taus", 3, -0.5, 2.5);

  plotslepcr->addTreeVar("met_lepcr_nbgeq1",        "met",         sel["lepcr"],                "#slash{E}_{T} [GeV]", 5, metbins);

  plotslepcr->plot();

  cout << "Plotting photon region" << endl;

  plotsphocr->addTreeVar("met_phocr_nbgeq1",        "met",         sel["phocr"],                "#slash{E}_{T} [GeV]", 5, metbins);

  plotsphocr->plot();

  /*cout << "Plotting zll region" << endl;

  plotszllcr->addTreeVar("met_zllcr_nb1",           "met",         sel["zllcr"] + sel["nb1"],      "#slash{E}_{T} [GeV]", 1, 200., 1000.0);
  plotszllcr->addTreeVar("met_zllcr_nb2",           "met",         sel["zllcr"] + sel["nb2"],      "#slash{E}_{T} [GeV]", 1, 200., 1000.0);

  plotszllcr->plot();*/

  cout << "Plotting zee region" << endl;

  plotszeecr->addTreeVar("met_zeecr_nb1",           "met",         sel["zeecr"] + sel["nb1"],      "#slash{E}_{T} [GeV]", 1, 200., 1000.0);
  plotszeecr->addTreeVar("met_zeecr_nb2",           "met",         sel["zeecr"] + sel["nb2"],      "#slash{E}_{T} [GeV]", 1, 200., 1000.0);

  plotszeecr->plot();

  cout << "Plotting zmm region" << endl;

  plotszmmcr->addTreeVar("met_zmmcr_nb1",           "met",         sel["zmmcr"] + sel["nb1"],      "#slash{E}_{T} [GeV]", 1, 200., 1000.0);
  plotszmmcr->addTreeVar("met_zmmcr_nb2",           "met",         sel["zmmcr"] + sel["nb2"],      "#slash{E}_{T} [GeV]", 1, 200., 1000.0);

  plotszmmcr->plot();

  cout << "Setting up prediction" << endl;

  vector<Sample*> samples0l;
  PlotTools::initSamples(srconf, samples0l);

  TFile* file0l      = new TFile(plots0l->outfileName());
  TFile* filelepcr   = new TFile(plotslepcr->outfileName());
  TFile* filephocr   = new TFile(plotsphocr->outfileName());
  //TFile* filezllcr   = new TFile(plotszllcr->outfileName());
  TFile* filezeecr   = new TFile(plotszeecr->outfileName());
  TFile* filezmmcr   = new TFile(plotszmmcr->outfileName());

  //HistMap data, lostlep, znunu, qcd, ttz;
  BinMap  lepcrtosr, qcdcrtosr, phocrtosr, zllcrtosr;

  vector<TString> srbins = {"nb1", "nb2"};
  lepcrtosr["nb1"] = "nbgeq1";
  lepcrtosr["nb2"] = "nbgeq1";
  qcdcrtosr["nb1"] = "nbgeq1";
  qcdcrtosr["nb2"] = "nbgeq1";
  phocrtosr["nb1"] = "nbgeq1";
  phocrtosr["nb2"] = "nbgeq1";
  zllcrtosr["nb1"] = "nb1";
  zllcrtosr["nb2"] = "nb2";

  TH1F* data           = getSRHist (file0l, "data", "sr",      srbins);
  TH1F* lostlep        = getLLPred (file0l, filelepcr, "sr",      "lepcr",      srbins, lepcrtosr);
  //TH1F* znunu          = getZPred  (file0l, filephocr, filezllcr, "sr", "phocr", "zllcr", srbins, phocrtosr, zllcrtosr); 
  TH1F* znunu          = getZPred  (file0l, filephocr, filezeecr, filezmmcr, "sr", "phocr", "zeecr", "zmmcr", srbins, phocrtosr, zllcrtosr); 
  TH1F* qcd            = getQCDPred(file0l, "sr", "qcdcr", srbins, qcdcrtosr);
  TH1F* ttz            = getSRHist (file0l, "ttZ", "sr",      srbins);

  removeZeroes(qcd);
  removeZeroes(ttz);

  TFile* srpred = new TFile("0lsearch.root","RECREATE");
  srpred->cd();

  cout << "\n-----------------------" << endl;
  cout << "Saving Final Prediction" << endl;
  cout << "For " << region << " region" << endl;
  cout << "Data \t || LostLep \t | Znunu \t | QCD \t | ttZ \t || Total Bkg." << endl;

  data->GetXaxis()->SetTitle("Search region");

  for(int ibin = 1; ibin < data->GetNbinsX()+1; ++ibin) {
    double nkgtot = lostlep->GetBinContent(ibin) + znunu->GetBinContent(ibin) + qcd->GetBinContent(ibin) + ttz->GetBinContent(ibin);
    double nbkgtotuncsq = pow(lostlep->GetBinError(ibin),2) + pow(znunu->GetBinError(ibin),2) + pow(qcd->GetBinError(ibin),2) + pow(ttz->GetBinError(ibin),2);
    cout << "Bin " << ibin << endl;
    cout << data->GetBinContent(ibin) << " +/- " << data->GetBinError(ibin) << "\t || ";
    cout << lostlep->GetBinContent(ibin) << " +/- " << lostlep->GetBinError(ibin) << "\t | ";
    cout << znunu->GetBinContent(ibin) << " +/- " << znunu->GetBinError(ibin) << "\t | ";
    cout << qcd->GetBinContent(ibin) << " +/- " << qcd->GetBinError(ibin) << "\t | ";
    cout << ttz->GetBinContent(ibin) << " +/- " << ttz->GetBinError(ibin) << "\t || ";
    cout << nkgtot << " +/- " << sqrt(nbkgtotuncsq) << endl;
  }

  data->Write();
  lostlep->Write();
  znunu->Write();
  qcd->Write();
  ttz->Write();

  PlotStuff* plots = new PlotStuff(srpred, outputdir);

  plots->setPlotSource(PlotStuff::HISTSSINGLEFILE);
  plots->setPlotType(PlotStuff::DATAMC);
  plots->setFormat(format);
  plots->setYTitle("Events");
  plots->setHeaderText("#sqrt{s} = 13 TeV",lumistr+" pb^{-1}","");
  plots->setHeaderPosition(0.2, 0.93);
  plots->setRatioPlot();
  plots->setColor("lostlep_pred_sr",StyleTools::color_ttbar);
  plots->setMaxScale(2.0);  

  vector<TString> labels = {"Data", "t#bar{t}/W", "Z#rightarrow#nu#nu", "t#bar{t}Z", "QCD"};
  vector<TString> names = {"data_sr", "lostlep_pred_sr", "znunu_pred_sr", "ttZ_sr", "qcd_pred_sr"};

  plots->setDataName("data_sr");
  plots->addCompSet("datavspred",names,labels);

  plots->plot();

  delete plots;

}
