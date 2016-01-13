#ifndef ZEROLEPTONTREEHELPER_HH
#define ZEROLEPTONTREEHELPER_HH

#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"

namespace BkgPrediction {

  enum SysVar { NOMINAL, VARUP, VARDOWN, NONE };

  const int NBINS          = 5;
  double metbins[NBINS+1]  = {250.0, 300.0, 400.0, 500.0, 600.0, 1000.0};

  map<TString,TString> sel;
  typedef map<TString,TString> BinMap;

  PlotStuff* setupPlots(const TString conf, const TString inputdir, const TString outputdir, const TString wgtvar, const bool plotlog, const TString format, const TString lumistr, const TString outfilename) {

    cout << "initializing plots for " << conf << " with input directory " << inputdir << endl;

    PlotStuff* plots = new PlotStuff(conf, inputdir, outputdir);
    plots->setRatioPlot();
    plots->setPlotOverflow(1);
    plots->setFormat(format);
    if(plotlog)
      plots->setLogy();
    plots->setWgtVar(wgtvar);
    plots->setHeaderText("#sqrt{s} = 13 TeV",TString::Format("%4.2f fb^{-1}",stof(string(lumistr.Data()))),"");
    plots->setHeaderPosition(0.2, 0.93);
    plots->setOutfile(outfilename);
    plots->setWriteHists();

    return plots;

  }

  void removeNegatives(TH1F* hist) {

    for(int ibin = 1; ibin < hist->GetNbinsX()+1; ++ibin) {
      double bincontent = hist->GetBinContent(ibin);
      double binerror = hist->GetBinError(ibin);
      if(bincontent < 0.0) {
        hist->SetBinContent(ibin, 0);
        hist->SetBinError(ibin, binerror + fabs(bincontent));
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

  TH1F* getQCDPred(TFile* file, const TString region, const TString crname, vector<TString> bins, BinMap crtosrmap, vector<TString> bkgsamples) {

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
    for(auto sname : bkgsamples) {
      if(sname == "qcd" || sname == "data") continue;
      TH1F* mc_cr = getSRHist(file, sname, crname, crbins);
      data_less_nonqcd_cr->Add(mc_cr, -1);
    }

    removeNegatives(data_less_nonqcd_cr);

    cout << "\nData - non-QCD MC in QCD CR: ";
    for(int ibin = 1; ibin < data_less_nonqcd_cr->GetNbinsX()+1; ++ibin) {
      cout << data_less_nonqcd_cr->GetBinContent(ibin) << " +/- " << data_less_nonqcd_cr->GetBinError(ibin) << "\t";
    }
    cout << endl;

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

  TH1F* getVarHist(TH1F* nominal, TH1F* varup, TH1F* vardown, TString name)
  {

    TH1F* varhist = (TH1F*)nominal->Clone(name);

    for(int ibin = 1; ibin < nominal->GetNbinsX()+1; ++ibin) {

      double sysup = varup->GetBinContent(ibin) - nominal->GetBinContent(ibin);
      double sysdown = vardown->GetBinContent(ibin) - nominal->GetBinContent(ibin);

      if(sysup * sysdown > 0) cout << "Up/down variations for " << name << " in bin " << ibin << " are in the same direction! (" << nominal->GetBinContent(ibin) << ", " << varup->GetBinContent(ibin) << ", " << vardown->GetBinContent(ibin) << ")" << endl;
      double sysmax = max(fabs(sysup),fabs(sysdown));

      double binerr = nominal->GetBinError(ibin);
      varhist->SetBinError(ibin, sqrt(binerr*binerr + sysmax*sysmax));
    }

    return varhist;

  }

}

#endif
