#ifndef PREDICTIONHELPER_HH
#define PREDICTIONHELPER_HH

#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"

namespace BkgPrediction {

  enum SysVar { NOMINAL, VARUP, VARDOWN, NONE };

  const int NBINS          = 5;
  const int NBINS_NT1      = 5;
  double metbins[NBINS+1]         = {250.0, 300.0, 400.0, 500.0, 600.0, 1000.0};
  double metbins_nt1[NBINS_NT1+1] = {250.0, 300.0, 400.0, 500.0, 600.0, 1000.0};

//  const int NBINS          = 6;
//  const int NBINS_NT1      = 2;
//  double metbins[NBINS+1]         = {200, 225, 250.0, 300.0, 400.0, 500.0, 600.0};
//  double metbins_nt1[NBINS_NT1+1] = {200.0, 400.0, 1000.0};

//  const int NBINS          = 1;
//  const int NBINS_NT1      = 1;
//  double metbins[NBINS+1]         = {200, 250};
//  double metbins_nt1[NBINS_NT1+1] = {200, 250};

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
    plots->setHeaderText("#sqrt{s} = 13 TeV",TString::Format("%4.1f fb^{-1}",stof(string(lumistr.Data()))),"");
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

  void removeZeroes(TH1F* hist, double setVal = 1, double setUnc = 0.8) {

    for(int ibin = 1; ibin < hist->GetNbinsX()+1; ++ibin) {
      double bincontent = hist->GetBinContent(ibin);
      double binerror = hist->GetBinError(ibin);
      if(bincontent == 0.0) {
        hist->SetBinContent(ibin, setVal);
        hist->SetBinError(ibin, setUnc);
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

    for(unsigned ibin = 0; ibin < bins.size(); ++ibin) {
      auto *h = srhists.at(ibin);
      bool isIntegrated = bins.at(ibin).Contains("_int");
      double val, err;
      if(isIntegrated) val = h->IntegralAndError(1, h->GetNbinsX(), err);
      for(int ibin = 1; ibin < h->GetNbinsX()+1; ++ibin) {
        if (!isIntegrated) {
          val = h->GetBinContent(ibin);
          err = h->GetBinError(ibin);
        }
        srhist->SetBinContent(srbin, val);
        srhist->SetBinError(srbin, err);
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

//    For bins with zero event, set to 0.001 +/- 1.8.
    removeZeroes(data_cr, 0.001, 1.8);

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

  TH1F* getQCDPred(TFile* file, TFile* filenovetoes, const TString region, const TString crname, vector<TString> bins, BinMap crtosrmap, vector<TString> bkgsamples) {

    cout << "\nGetting the QCD prediction in the " << region << " region. CR label is " << crname << endl;

    TH1F* qcd_sr = getSRHist(filenovetoes, "qcd", region+"novetoes", bins);
    vector<TString> crbins;
    for(auto bin : bins) {
      cout << "\nCR bin for " << bin << ": " << crtosrmap[bin] << endl;
      crbins.push_back(crtosrmap[bin]);
    }
    TH1F* qcd_cr_withvetoes = getSRHist(file, "qcd", crname, crbins);
    TH1F* qcd_cr = getSRHist(filenovetoes, "qcd", crname+"novetoes", crbins);
    TH1F* data_cr = getSRHist(file, "data", crname, crbins);
    TH1F* data_less_nonqcd_cr = (TH1F*)data_cr->Clone("data_less_nonqcd_" + crname);

    TH1F* nonqcd_cr = (TH1F*)data_cr->Clone("nonqcd_" + crname);

    for(int ibin = 1; ibin < nonqcd_cr->GetNbinsX()+1; ++ibin) {
      nonqcd_cr->SetBinContent(ibin,0);
    }

    for(auto sname : bkgsamples) {
      if(sname == "qcd" || sname == "data") continue;
      TH1F* mc_cr = getSRHist(file, sname, crname, crbins);
      nonqcd_cr->Add(mc_cr);
    }
    TH1F* nonqcd_sf_cr = (TH1F*)data_cr->Clone("nonqcd_sf_" + crname);

    for(int ibin = 1; ibin < nonqcd_sf_cr->GetNbinsX()+1; ++ibin) {
      double dY = data_cr->GetBinContent(ibin) < 10 ? qcd_cr_withvetoes->GetBinContent(ibin) + nonqcd_cr->GetBinContent(ibin) : data_cr->GetBinContent(ibin);
      if(dY <= 0) dY = 1;
      double r = nonqcd_cr->GetBinContent(ibin)/dY;
      nonqcd_sf_cr->SetBinContent(ibin, 1 - r);
      nonqcd_sf_cr->SetBinError(ibin,r);
    }

    cout << "\nData SF for contamination in QCD CR: ";
    for(int ibin = 1; ibin < nonqcd_sf_cr->GetNbinsX()+1; ++ibin) {
      cout << nonqcd_sf_cr->GetBinContent(ibin) << " +/- " << nonqcd_sf_cr->GetBinError(ibin) << "\t";
    }
    cout << endl;
    removeNegatives(data_less_nonqcd_cr);
    removeZeroes(data_less_nonqcd_cr);
    data_less_nonqcd_cr->Multiply(nonqcd_sf_cr);



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

  TH1F* calcRZ(TH1F* hdata, TH1F* hzll, TH1F* httbar, TH1F* hdata_off, TH1F* hzll_off, TH1F* httbar_off){
    TH1F* RZ = (TH1F*)hdata->Clone("RZ");
    for (int ibin = 1; ibin < RZ->GetNbinsX()+1; ++ibin){
      double data = hdata->GetBinContent(ibin), data_err = hdata->GetBinError(ibin);
      double zll = hzll->GetBinContent(ibin), zll_err = hzll->GetBinError(ibin);
      double ttbar = httbar->GetBinContent(ibin), ttbar_err = httbar->GetBinError(ibin);
      double dataoff = hdata_off->GetBinContent(ibin), dataoff_err = hdata_off->GetBinError(ibin);
      double zlloff = hzll_off->GetBinContent(ibin), zlloff_err = hzll_off->GetBinError(ibin);
      double ttbaroff = httbar_off->GetBinContent(ibin), ttbaroff_err = httbar_off->GetBinError(ibin);

      double num = dataoff*ttbar - data*ttbaroff;
      double den = zlloff *ttbar - zll *ttbaroff;
      double val = num/den;
      RZ->SetBinContent(ibin, val);
      vector<double> errs{ttbar*dataoff_err, ttbaroff*data_err, val*ttbar*zlloff_err, val*ttbaroff*zll_err, (dataoff-val*zlloff)*ttbar_err, (-data+val*zll)*ttbaroff_err};
      double sum=0;
      for (double e : errs){
        sum += pow(e, 2);
      }
      RZ->SetBinError(ibin, sqrt(sum)/abs(den));
    }
    return RZ;
  }

  TH1F* getZPred(TFile* file0l, TFile* filephocr, TFile* filezllcr, const TString region, const TString phocrname, const TString zllcrname, vector<TString> bins, BinMap phocrtosrmap, BinMap zllcrtosrmap) {

    cout << "\nGetting the Z prediction in the " << region << " region. CR labels are " << phocrname << ", " << zllcrname << endl;

    TH1F* znunu_sr = getSRHist(file0l, "znunu", region, bins);
    vector<TString> phocrbins, phocrnomtbins, zllcrbins;
    for(auto bin : bins) {
      cout << "\nPhoton CR bin for " << bin << ": " << phocrtosrmap[bin] << endl;
      cout << "\nZLL CR bin for " << bin << ": " << zllcrtosrmap[bin] << endl;
      phocrbins.push_back(phocrtosrmap[bin]);
      TString nomtbin = phocrtosrmap[bin];
      if(nomtbin.Contains("_lowmt"))
        nomtbin.ReplaceAll("_lowmt","_nomt");
      else if (nomtbin.Contains("_highmt"))
        nomtbin.ReplaceAll("_highmt","_nomt");
      if(nomtbin.Contains("_int"))
        nomtbin.ReplaceAll("_int", "");
      phocrnomtbins.push_back(nomtbin);
      zllcrbins.push_back(zllcrtosrmap[bin]);
    }
    TH1F* photon_phocr = getSRHist(filephocr, "photon", phocrname, phocrbins);
    photon_phocr->SetName("photon_phocr_withmt");
    TH1F* data_phocr = getSRHist(filephocr, "data", phocrname, phocrbins);
    data_phocr->SetName("data_phocr_withmt");

    TH1F* photon_phocr_nomtb = getSRHist(filephocr, "photon", phocrname, phocrnomtbins);
    TH1F* data_phocr_nomtb = getSRHist(filephocr, "data", phocrname, phocrnomtbins);

    cout << "\nData yield in Photon CR before mtb cut: " << data_phocr_nomtb->Integral(0, data_phocr_nomtb->GetNbinsX()+1) << endl;
    cout << "\nMC yield in Photon CR before mtb cut: " << photon_phocr_nomtb->Integral(0, photon_phocr_nomtb->GetNbinsX()+1) << endl;

    cout << "\nData yield in Photon CR after mtb cut: " << data_phocr->Integral(0, data_phocr->GetNbinsX()+1) << endl;
    cout << "\nMC yield in Photon CR after mtb cut: " << photon_phocr->Integral(0, photon_phocr->GetNbinsX()+1) << endl;

    photon_phocr->Scale(data_phocr_nomtb->Integral(0, data_phocr_nomtb->GetNbinsX()+1) / photon_phocr_nomtb->Integral(0, photon_phocr_nomtb->GetNbinsX()+1));
    cout << "\nMC yield in Photon CR after normalizing to data: " << photon_phocr->Integral(0, photon_phocr->GetNbinsX()+1) << endl;

    removeZeroes(data_phocr);

    TH1F* znunu_pred = (TH1F*)znunu_sr->Clone("znunu_pred_" + region);
    znunu_pred->Multiply(data_phocr);
    znunu_pred->Divide(photon_phocr);

    cout << "\nZnunu MC x photon shape weights: ";
    for(int ibin = 1; ibin < znunu_pred->GetNbinsX()+1; ++ibin) {
      cout << znunu_pred->GetBinContent(ibin) << " +/- " << znunu_pred->GetBinError(ibin) << "\t";
    }
    cout << endl;

    TH1F* zll_zllcr = getSRHist(filezllcr, "zll", zllcrname, zllcrbins);
    TH1F* ttbar_zllcr = getSRHist(filezllcr, "ttbar", zllcrname, zllcrbins);
    TH1F* data_zllcr = getSRHist(filezllcr, "data", zllcrname, zllcrbins);
    TH1F* zll_zllcr_offz = getSRHist(filezllcr, "zll", zllcrname+"_offz", zllcrbins);
    TH1F* ttbar_zllcr_offz = getSRHist(filezllcr, "ttbar", zllcrname+"_offz", zllcrbins);
    TH1F* data_zllcr_offz = getSRHist(filezllcr, "data", zllcrname+"_offz", zllcrbins);

    TH1F* zll_sf = calcRZ(data_zllcr, zll_zllcr, ttbar_zllcr, data_zllcr_offz, zll_zllcr_offz, ttbar_zllcr_offz);
    zll_sf->SetName("zll_sf_" + region);

    cout << "\nZ scale factors from ZLL CR: ";
    for(int ibin = 1; ibin < zll_sf->GetNbinsX()+1; ++ibin) {
      cout << zll_sf->GetBinContent(ibin) << " +/- " << zll_sf->GetBinError(ibin) << "\t";
    }
    cout << endl;

    double sf_1b = zll_sf->GetBinContent(1);
    double sf_2b = zll_sf->GetBinContent(2);
    double sf_1b_unc = zll_sf->GetBinError(1);
    double sf_2b_unc = zll_sf->GetBinError(2);

    for(int ibin = 1; ibin < znunu_pred->GetNbinsX()+1; ++ibin) {
      double bincontent = znunu_pred->GetBinContent(ibin);
      double binerror = znunu_pred->GetBinError(ibin);
      int whichbin = int((ibin-1)/NBINS);
      if(bins[whichbin].Contains("nb1")) {
        cout << "Bin " << ibin << "(" << bins[whichbin] << "), using SF_1b = " << sf_1b << endl;
        znunu_pred->SetBinContent(ibin, bincontent * sf_1b);
        znunu_pred->SetBinError(ibin, znunu_pred->GetBinContent(ibin) * sqrt((sf_1b_unc*sf_1b_unc/(sf_1b*sf_1b)) + (binerror*binerror/(bincontent*bincontent))));
      } else {
        cout << "Bin " << ibin << "(" << bins[whichbin] << "), using SF_2b = " << sf_2b << endl;
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

  void printSFAndUnc(TH1F* hist, const vector<vector<double> > &M, const vector<vector<double> > &EM, const vector<double>  &D ){

      double M11 = M[0][0]; double M21 = M[1][0]; double M31 = M[2][0];
      double M12 = M[0][1]; double M22 = M[1][1]; double M32 = M[2][1];
      double M13 = M[0][2]; double M23 = M[1][2]; double M33 = M[2][2];

      double EM11 = EM[0][0]; double EM21 = EM[1][0]; double EM31 = EM[2][0];
      double EM12 = EM[0][1]; double EM22 = EM[1][1]; double EM32 = EM[2][1];
      double EM13 = EM[0][2]; double EM23 = EM[1][2]; double EM33 = EM[2][2];

      double D1 = D[0];
      double D2 = D[1];
      double D3 = D[2];

      double A1 = -((-(D3*M13*M22) + D3*M12*M23 + D2*M13*M32 - D1*M23*M32 - D2*M12*M33 + D1*M22*M33)/(M13*M22*M31 - M12*M23*M31 - M13*M21*M32 + M11*M23*M32 + M12*M21*M33 - M11*M22*M33));
      double A2 = -((D3*M13*M21 - D3*M11*M23 - D2*M13*M31 + D1*M23*M31 + D2*M11*M33 - D1*M21*M33)/(M13*M22*M31 - M12*M23*M31 - M13*M21*M32 + M11*M23*M32 + M12*M21*M33 - M11*M22*M33));
      double A3 = -((D3*M12*M21 - D3*M11*M22 - D2*M12*M31 + D1*M22*M31 + D2*M11*M32 - D1*M21*M32)/(-(M13*M22*M31) + M12*M23*M31 + M13*M21*M32 - M11*M23*M32 - M12*M21*M33 + M11*M22*M33));

      double A1E2 =(EM33*pow(M13*M22 - M12*M23,2)*pow(D3*M12*M21 - D3*M11*M22 - D2*M12*M31 + D1*M22*M31 + D2*M11*M32 - D1*M21*M32,2) + EM23*pow(D3*M12*M21 - D3*M11*M22 - D2*M12*M31 + D1*M22*M31 + D2*M11*M32 - D1*M21*M32,2)*pow(M13*M32 - M12*M33,2) +
       EM32*pow(M13*M22 - M12*M23,2)*pow(D3*M13*M21 - D3*M11*M23 - D2*M13*M31 + D1*M23*M31 + D2*M11*M33 - D1*M21*M33,2) + EM22*pow(M13*M32 - M12*M33,2)*pow(D3*M13*M21 - D3*M11*M23 - D2*M13*M31 + D1*M23*M31 + D2*M11*M33 - D1*M21*M33,2) +
       EM13*pow(D3*M12*M21 - D3*M11*M22 - D2*M12*M31 + D1*M22*M31 + D2*M11*M32 - D1*M21*M32,2)*pow(M23*M32 - M22*M33,2) + EM12*pow(D3*M13*M21 - D3*M11*M23 - D2*M13*M31 + D1*M23*M31 + D2*M11*M33 - D1*M21*M33,2)*pow(M23*M32 - M22*M33,2) +
       EM31*pow(M13*M22 - M12*M23,2)*pow(D3*M13*M22 - D3*M12*M23 - D2*M13*M32 + D1*M23*M32 + D2*M12*M33 - D1*M22*M33,2) + EM21*pow(M13*M32 - M12*M33,2)*pow(D3*M13*M22 - D3*M12*M23 - D2*M13*M32 + D1*M23*M32 + D2*M12*M33 - D1*M22*M33,2) +
       EM11*pow(M23*M32 - M22*M33,2)*pow(D3*M13*M22 - D3*M12*M23 - D2*M13*M32 + D1*M23*M32 + D2*M12*M33 - D1*M22*M33,2) + D3*pow(M13*M22 - M12*M23,2)*pow(M13*M22*M31 - M12*M23*M31 - M13*M21*M32 + M11*M23*M32 + M12*M21*M33 - M11*M22*M33,2) +
       D2*pow(M13*M32 - M12*M33,2)*pow(M13*M22*M31 - M12*M23*M31 - M13*M21*M32 + M11*M23*M32 + M12*M21*M33 - M11*M22*M33,2) + D1*pow(M23*M32 - M22*M33,2)*pow(M13*M22*M31 - M12*M23*M31 - M13*M21*M32 + M11*M23*M32 + M12*M21*M33 - M11*M22*M33,2))/
     pow(M13*M22*M31 - M12*M23*M31 - M13*M21*M32 + M11*M23*M32 + M12*M21*M33 - M11*M22*M33,4);
      double A2E2 = (EM33*pow(M13*M21 - M11*M23,2)*pow(D3*M12*M21 - D3*M11*M22 - D2*M12*M31 + D1*M22*M31 + D2*M11*M32 - D1*M21*M32,2) + EM23*pow(D3*M12*M21 - D3*M11*M22 - D2*M12*M31 + D1*M22*M31 + D2*M11*M32 - D1*M21*M32,2)*pow(M13*M31 - M11*M33,2) +
       EM13*pow(D3*M12*M21 - D3*M11*M22 - D2*M12*M31 + D1*M22*M31 + D2*M11*M32 - D1*M21*M32,2)*pow(M23*M31 - M21*M33,2) + EM32*pow(M13*M21 - M11*M23,2)*pow(D3*M13*M21 - D3*M11*M23 - D2*M13*M31 + D1*M23*M31 + D2*M11*M33 - D1*M21*M33,2) +
       EM22*pow(M13*M31 - M11*M33,2)*pow(D3*M13*M21 - D3*M11*M23 - D2*M13*M31 + D1*M23*M31 + D2*M11*M33 - D1*M21*M33,2) + EM12*pow(M23*M31 - M21*M33,2)*pow(D3*M13*M21 - D3*M11*M23 - D2*M13*M31 + D1*M23*M31 + D2*M11*M33 - D1*M21*M33,2) +
       EM31*pow(M13*M21 - M11*M23,2)*pow(D3*M13*M22 - D3*M12*M23 - D2*M13*M32 + D1*M23*M32 + D2*M12*M33 - D1*M22*M33,2) + EM21*pow(M13*M31 - M11*M33,2)*pow(D3*M13*M22 - D3*M12*M23 - D2*M13*M32 + D1*M23*M32 + D2*M12*M33 - D1*M22*M33,2) +
       EM11*pow(M23*M31 - M21*M33,2)*pow(D3*M13*M22 - D3*M12*M23 - D2*M13*M32 + D1*M23*M32 + D2*M12*M33 - D1*M22*M33,2) + D3*pow(M13*M21 - M11*M23,2)*pow(M13*M22*M31 - M12*M23*M31 - M13*M21*M32 + M11*M23*M32 + M12*M21*M33 - M11*M22*M33,2) +
       D2*pow(M13*M31 - M11*M33,2)*pow(M13*M22*M31 - M12*M23*M31 - M13*M21*M32 + M11*M23*M32 + M12*M21*M33 - M11*M22*M33,2) + D1*pow(M23*M31 - M21*M33,2)*pow(M13*M22*M31 - M12*M23*M31 - M13*M21*M32 + M11*M23*M32 + M12*M21*M33 - M11*M22*M33,2))/
     pow(M13*M22*M31 - M12*M23*M31 - M13*M21*M32 + M11*M23*M32 + M12*M21*M33 - M11*M22*M33,4);
      double A3E2 = (EM33*pow(M12*M21 - M11*M22,2)*pow(D3*M12*M21 - D3*M11*M22 - D2*M12*M31 + D1*M22*M31 + D2*M11*M32 - D1*M21*M32,2) + EM23*pow(M12*M31 - M11*M32,2)*pow(D3*M12*M21 - D3*M11*M22 - D2*M12*M31 + D1*M22*M31 + D2*M11*M32 - D1*M21*M32,2) +
       EM13*pow(M22*M31 - M21*M32,2)*pow(D3*M12*M21 - D3*M11*M22 - D2*M12*M31 + D1*M22*M31 + D2*M11*M32 - D1*M21*M32,2) + EM32*pow(M12*M21 - M11*M22,2)*pow(D3*M13*M21 - D3*M11*M23 - D2*M13*M31 + D1*M23*M31 + D2*M11*M33 - D1*M21*M33,2) +
       EM22*pow(M12*M31 - M11*M32,2)*pow(D3*M13*M21 - D3*M11*M23 - D2*M13*M31 + D1*M23*M31 + D2*M11*M33 - D1*M21*M33,2) + EM12*pow(M22*M31 - M21*M32,2)*pow(D3*M13*M21 - D3*M11*M23 - D2*M13*M31 + D1*M23*M31 + D2*M11*M33 - D1*M21*M33,2) +
       EM31*pow(M12*M21 - M11*M22,2)*pow(D3*M13*M22 - D3*M12*M23 - D2*M13*M32 + D1*M23*M32 + D2*M12*M33 - D1*M22*M33,2) + EM21*pow(M12*M31 - M11*M32,2)*pow(D3*M13*M22 - D3*M12*M23 - D2*M13*M32 + D1*M23*M32 + D2*M12*M33 - D1*M22*M33,2) +
       EM11*pow(M22*M31 - M21*M32,2)*pow(D3*M13*M22 - D3*M12*M23 - D2*M13*M32 + D1*M23*M32 + D2*M12*M33 - D1*M22*M33,2) + D3*pow(M12*M21 - M11*M22,2)*pow(M13*M22*M31 - M12*M23*M31 - M13*M21*M32 + M11*M23*M32 + M12*M21*M33 - M11*M22*M33,2) +
       D2*pow(M12*M31 - M11*M32,2)*pow(M13*M22*M31 - M12*M23*M31 - M13*M21*M32 + M11*M23*M32 + M12*M21*M33 - M11*M22*M33,2) + D1*pow(M22*M31 - M21*M32,2)*pow(M13*M22*M31 - M12*M23*M31 - M13*M21*M32 + M11*M23*M32 + M12*M21*M33 - M11*M22*M33,2))/
     pow(M13*M22*M31 - M12*M23*M31 - M13*M21*M32 + M11*M23*M32 + M12*M21*M33 - M11*M22*M33,4);

      cout << A1 <<" " << TMath::Sqrt(A1E2) <<endl
           << A2 <<" " << TMath::Sqrt(A2E2) <<endl
           << A3 <<" " << TMath::Sqrt(A3E2) <<endl ;

      double err = std::sqrt(A1E2);
      for (int ibin=0; ibin<=hist->GetNbinsX()+1; ++ibin){
        hist->SetBinContent(ibin, A1);
        hist->SetBinError(ibin, err);
      }

  }


  TH1F* getTTZPred(TFile* file0l, TFile* filettzcr, const TString region, const TString ttzcrname, vector<TString> bins, bool applySF) {
    vector<TString> crbins{"sel", "noz", "nob"};
    vector<vector<double> > groupYields(3,vector<double>(3)); //ttz ttbar Z
    vector<vector<double> > groupUnc   (3,vector<double>(3));
    vector<double> vecData(3);

    for (unsigned ib=0; ib<crbins.size(); ++ib){
      auto bin = crbins.at(ib);
      cout << "\nCR bin for " << bin << endl;

      //                  0      1  2    3    4   5   6   7   8   9  10 11   12
      //enum EXTPROCESS {DATA, TTZ, TT, TTW, TTG, T, TW,  W, WW, TZ, Z, WZ, ZZ  };

      TH1F* hMC = (TH1F*)filettzcr->Get("extProcess_" + ttzcrname + "_" + bin + "_" + "ttz-all");
      assert(hMC);
      groupYields[ib][0] += hMC->GetBinContent(2);
      groupYields[ib][0] += hMC->GetBinContent(10);
      groupYields[ib][1] += hMC->GetBinContent(3);
      groupYields[ib][1] += hMC->GetBinContent(4);
      groupYields[ib][1] += hMC->GetBinContent(5);
      groupYields[ib][1] += hMC->GetBinContent(6);
      groupYields[ib][1] += hMC->GetBinContent(7);
      groupYields[ib][1] += hMC->GetBinContent(8);
      groupYields[ib][1] += hMC->GetBinContent(9);
      groupYields[ib][2] += hMC->GetBinContent(11);
      groupYields[ib][2] += hMC->GetBinContent(12);
      groupYields[ib][2] += hMC->GetBinContent(13);

      groupUnc[ib][0] += hMC->GetBinError(2) * hMC->GetBinError(2) ;
      groupUnc[ib][0] += hMC->GetBinError(10)* hMC->GetBinError(10);
      groupUnc[ib][1] += hMC->GetBinError(3) * hMC->GetBinError(3) ;
      groupUnc[ib][1] += hMC->GetBinError(4) * hMC->GetBinError(4) ;
      groupUnc[ib][1] += hMC->GetBinError(5) * hMC->GetBinError(5) ;
      groupUnc[ib][1] += hMC->GetBinError(6) * hMC->GetBinError(6) ;
      groupUnc[ib][1] += hMC->GetBinError(7) * hMC->GetBinError(7) ;
      groupUnc[ib][1] += hMC->GetBinError(8) * hMC->GetBinError(8) ;
      groupUnc[ib][1] += hMC->GetBinError(9) * hMC->GetBinError(9) ;
      groupUnc[ib][2] += hMC->GetBinError(11)* hMC->GetBinError(11);
      groupUnc[ib][2] += hMC->GetBinError(12)* hMC->GetBinError(12);
      groupUnc[ib][2] += hMC->GetBinError(13)* hMC->GetBinError(13);

      TH1F* hdata = (TH1F*)filettzcr->Get("extProcess_" + ttzcrname + "_" + bin + "_" + "data");
      assert(hdata);
      vecData[ib] = hdata->GetBinContent(1);
    }

    TH1F* ttz_sr = getSRHist (file0l, "ttZ", region, bins);

    TH1F* ttz_pred = (TH1F*)ttz_sr->Clone("ttZ_pred_" + region);

    TH1F* ttz_sf = (TH1F*)ttz_sr->Clone("ttZ_sf_" + region);
    printSFAndUnc(ttz_sf, groupYields, groupUnc, vecData);

    cout << "\nttZ scale factor: " << ttz_sf->GetBinContent(1) << " +/- " << ttz_sf->GetBinError(1) << endl;

    if(applySF){
      cout << "\nttZ scale factor applied. " << endl;
      ttz_pred->Multiply(ttz_sf);
    }else{
      cout << "\nttZ scale factor NOT applied. " << endl;
    }

    return ttz_pred;

  }

  TH1F* getTTZPred(TFile* file0l, const TString region, vector<TString> bins) {

    cout << "\nGetting the ttZ prediction in the " << region << " region. " << endl;

    TH1F* ttz_sr = getSRHist (file0l, "ttZ", region, bins);

    TH1F* ttz_pred = (TH1F*)ttz_sr->Clone("ttZ_pred_" + region);

    removeNegatives(ttz_pred);

    cout << "\nttZ prediction: ";
    for(int ibin = 1; ibin < ttz_pred->GetNbinsX()+1; ++ibin) {
      cout << ttz_pred->GetBinContent(ibin) << " +/- " << ttz_pred->GetBinError(ibin) << "\t";
    }
    cout << endl;

    return ttz_pred;

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
