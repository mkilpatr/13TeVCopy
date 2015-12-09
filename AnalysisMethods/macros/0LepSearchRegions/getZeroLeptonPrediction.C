#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#endif

enum SysVars { NOMINAL, VARUP, VARDOWN };

vector<TString> samples = {"data", "ttbarplusw","ttZ","znunu", "qcd"};

const int NBINS          = 5;
double metbins[NBINS+1]  = {200.0, 225.0, 250.0, 300.0, 400.0, 600.0};

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

TH1F* getQCDPred(TFile* file, const TString region, const TString crname, vector<TString> bins, BinMap crtosrmap, TString tffilename) {

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
  TFile* tffile = new TFile(tffilename);
  TH1F* tf_nb1 = (TH1F*)tffile->Get("tf_sr_nb1");
  TH1F* tf_nb2 = (TH1F*)tffile->Get("tf_sr_nb2");
  TF1* tf_nb1_fit = tf_nb1->GetFunction("TFfit_SR_sr_nb1");
  TF1* tf_nb2_fit = tf_nb2->GetFunction("TFfit_SR_sr_nb2");
  int nbins = qcd_tf->GetNbinsX();
  for(int ibin = 1; ibin < qcd_tf->GetNbinsX()+1; ++ibin) {
    if (ibin <= nbins/2) {
      int nmetbin = ibin;
      qcd_tf->SetBinContent(ibin, tf_nb1_fit->Eval(0.5*(metbins[nmetbin-1] + metbins[nmetbin])));
    }
    else if(ibin > nbins/2) {
      int nmetbin = (ibin - nbins/2);
      qcd_tf->SetBinContent(ibin, tf_nb2_fit->Eval(0.5*(metbins[nmetbin-1] + metbins[nmetbin])));
    }
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
  vector<TString> phocrbins, zllcrbins;
  for(auto bin : bins) {
    cout << "\nPhoton CR bin for " << bin << ": " << phocrtosrmap[bin] << endl;
    cout << "\nZLL CR bin for " << bin << ": " << zllcrtosrmap[bin] << endl;
    phocrbins.push_back(phocrtosrmap[bin]);
    zllcrbins.push_back(zllcrtosrmap[bin]);
  }
  TH1F* photon_phocr = getSRHist(filephocr, "photon", phocrname, phocrbins);
  TH1F* data_phocr = getSRHist(filephocr, "data", phocrname, phocrbins);

  TH1F* photon_phocr_nomtb = getSRHist(filephocr, "photon", phocrname+"_nomtb", phocrbins);
  TH1F* data_phocr_nomtb = getSRHist(filephocr, "data", phocrname+"_nomtb", phocrbins);

  cout << "\nData yield in Photon CR before mtb cut: " << data_phocr_nomtb->Integral(0, data_phocr_nomtb->GetNbinsX()+1) << endl;
  cout << "\nMC yield in Photon CR before mtb cut: " << photon_phocr_nomtb->Integral(0, photon_phocr_nomtb->GetNbinsX()+1) << endl;

  photon_phocr->Scale(data_phocr_nomtb->Integral(0, data_phocr_nomtb->GetNbinsX()+1) / photon_phocr_nomtb->Integral(0, photon_phocr_nomtb->GetNbinsX()+1));

  cout << "\nData yield in Photon CR after mtb cut: " << data_phocr->Integral(0, data_phocr->GetNbinsX()+1) << endl;
  cout << "\nMC yield in Photon CR after normalizing to data: " << photon_phocr->Integral(0, photon_phocr->GetNbinsX()+1) << endl;

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

  cout << "\nGetting the ttZ prediction in the " << region << " region. " << endl;

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
  TH1F* ttz_sf = (TH1F*)ttz_sr->Clone("ttZ_sf_" + region);
  printSFAndUnc(ttz_sf, groupYields, groupUnc, vecData);

  cout << "\nttZ scale factor: " << ttz_sf->GetBinContent(1) << " +/- " << ttz_sf->GetBinError(1) << endl;

  TH1F* ttz_pred = (TH1F*)ttz_sr->Clone("ttZ_pred_" + region);
  if(applySF){
    cout << "\nttZ scale factor applied. " << endl;
    ttz_pred->Multiply(ttz_sf);
  }else{
    cout << "\nttZ scale factor NOT applied. " << endl;
  }

  cout << "\nttZ prediction: ";
  for(int ibin = 1; ibin < ttz_pred->GetNbinsX()+1; ++ibin) {
    cout << ttz_pred->GetBinContent(ibin) << " +/- " << ttz_pred->GetBinError(ibin) << "\t";
  }
  cout << endl;

  return ttz_pred;

}


void getZeroLeptonPrediction(const TString defaultdir  = "/uscms_data/d3/hqu/Workspace/74X/CMSSW_7_4_11/src/AnalysisMethods/macros/run/trees/wtags/SR",
                             const TString varupdir    = "trees/varup",
                             const TString vardowndir  = "trees/vardown",
                             const TString outputdir   = "plots_bkgest_101415",
                             const TString srconf      = "run0lepmine.conf",
                             const TString phocrconf   = "runphotoncrmine.conf",
                             const TString zllcrconf   = "runzllcrmine.conf",
                             const TString ttzcrconf   = "runttzcrmine.conf",
                             const TString lumistr     = "1.263",
                             const TString crlumistr   = "1.264",
                             //const TString region      = "sr",
                             const TString region      = "srlownj",
                             const TString format      = "pdf",
                             const TString qcdfitfile  = TString::Format("%s/src/data/QCD/tffits.root",getenv("CMSSW_BASE")),
                             const bool    dolownj     = true,
                             const bool    usettzsf    = false,
                             const unsigned int sysvar = NOMINAL,
                             const bool    plotlog     = false)
{
  gSystem->mkdir(outputdir, true);

  TString basewgt    = lumistr + "*weight*truePUWeight";
  TString basewgtcr  = crlumistr + "*weight*truePUWeight";
  TString lepvetowgt = basewgt + "*lepvetoweight";
  TString lepselwgt  = basewgt + "*lepselweight";

  sel["trig"]         = "passjson && passdijetmet && j2pt>75 && met>200 && passcscflt && passeebadscflt && passhbheflttight";
  sel["trigpho"]      = "passjson && passtrigphoton165 && origmet<200 && j2pt>75 && met>200 && passcscflt && passeebadscflt && passhbheflttight";
  sel["trigzll"]      = "passjson && ((iselectron && passtrige17e12) || (!iselectron && (passtrigmu17mu8 || passtrigmu17tkmu8))) && j2pt>75 && met>100 && dilepmass > 80 && dilepmass < 100";
  sel["trigzlloff"]   = "passjson && ((iselectron && passtrige17e12) || (!iselectron && (passtrigmu17mu8 || passtrigmu17tkmu8))) && j2pt>75 && met>100 && dilepmass > 20 && (dilepmass < 80 || dilepmass > 100)";
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
  sel["phocr"]        = sel["trigpho"] + sel["njets"] + sel["dphij123"];
  sel["zllcr"]        = sel["trigzll"] + sel["njets"];
  sel["zlloffcr"]     = sel["trigzlloff"] + sel["njets"];
//  sel["trigttz"]        = "passjson && passTrig && passMed && njets>=4 && minlepPT>15";
  sel["trigttz"]        = "passjson && passTrig && passMed && njets>=4 && minlepPT>15 && dileppt>100";
  sel["ttzSel"]       = sel["trigttz"] + " && nbjets>=1 && nlbjets>=2 && onZ";
  sel["ttzNoZ"]       = sel["trigttz"] + " && nbjets>=1 && nlbjets>=2 && (!onZ)";
  sel["ttzNoB"]       = sel["trigttz"] + " && nbjets==0 && onZ";
  sel["nb1"]          = " && nbjets==1";
  sel["nb2"]          = " && nbjets>=2";
  sel["nb1_nt0"]      = " && nbjets==1 && ncttstd==0";
  sel["nb2_nt0"]      = " && nbjets>=2 && ncttstd==0";
  sel["nb1_nt1"]      = " && nbjets==1 && ncttstd>0";
  sel["nb2_nt1"]      = " && nbjets>=2 && ncttstd>0";

  TString inputdir    = sysvar == NOMINAL ? defaultdir : (sysvar == VARUP ? varupdir : vardowndir);

  PlotStuff* plots0l    = setupPlots(srconf,    inputdir, outputdir, lepvetowgt, plotlog, format, lumistr, "output_0l.root");
  PlotStuff* plotslepcr = setupPlots(srconf,    inputdir, outputdir, lepselwgt, plotlog, format, lumistr, "output_lepcr.root");
  PlotStuff* plotsphocr = setupPlots(phocrconf, inputdir+"/photoncr", outputdir, basewgtcr, plotlog, format, crlumistr, "output_phocr.root");
  PlotStuff* plotszllcr = setupPlots(zllcrconf, inputdir+"/zllcr", outputdir, basewgtcr, plotlog, format, crlumistr, "output_zllcr.root");
  PlotStuff* plotsttzcr = setupPlots(ttzcrconf, inputdir+"/ttzcr", outputdir, basewgtcr, plotlog, format, crlumistr, "output_ttzcr.root");

  cout << "Plotting 0lepton region" << endl;

  plots0l->addTreeVar("dphij12met_qcdincl",         "dphij12met",  sel["qcdincl"],                  "min(#Delta#phi(j1,#slash{E}_{T}),#Delta#phi(j2,#slash{E}_{T}))", 21, 0, 3.15);
  plots0l->addTreeVar("dphij12met_qcdincl_nb1",     "dphij12met",  sel["qcdincl"] + sel["nb1"],     "min(#Delta#phi(j1,#slash{E}_{T}),#Delta#phi(j2,#slash{E}_{T}))", 63, 0, 3.15);
  plots0l->addTreeVar("dphij12met_qcdincl_nb2",     "dphij12met",  sel["qcdincl"] + sel["nb2"],     "min(#Delta#phi(j1,#slash{E}_{T}),#Delta#phi(j2,#slash{E}_{T}))", 63, 0, 3.15);
  plots0l->addTreeVar("dphij12met_qcdincl_nb1_nt0", "dphij12met",  sel["qcdincl"] + sel["nb1_nt0"], "min(#Delta#phi(j1,#slash{E}_{T}),#Delta#phi(j2,#slash{E}_{T}))", 63, 0, 3.15);
  plots0l->addTreeVar("dphij12met_qcdincl_nb2_nt0", "dphij12met",  sel["qcdincl"] + sel["nb2_nt0"], "min(#Delta#phi(j1,#slash{E}_{T}),#Delta#phi(j2,#slash{E}_{T}))", 63, 0, 3.15);
  plots0l->addTreeVar("dphij12met_qcdincl_nb1_nt1", "dphij12met",  sel["qcdincl"] + sel["nb1_nt1"], "min(#Delta#phi(j1,#slash{E}_{T}),#Delta#phi(j2,#slash{E}_{T}))", 63, 0, 3.15);
  plots0l->addTreeVar("dphij12met_qcdincl_nb2_nt1", "dphij12met",  sel["qcdincl"] + sel["nb2_nt1"], "min(#Delta#phi(j1,#slash{E}_{T}),#Delta#phi(j2,#slash{E}_{T}))", 63, 0, 3.15);

  plots0l->addTreeVar("met_sr_nb1",                 "met",         sel["sr"] + sel["nb1"],      "#slash{E}_{T} [GeV]", NBINS, metbins);
  plots0l->addTreeVar("met_sr_nb2",                 "met",         sel["sr"] + sel["nb2"],      "#slash{E}_{T} [GeV]", NBINS, metbins);

  plots0l->addTreeVar("met_qcdincl_nbgeq1",         "met",         sel["qcdincl"],              "#slash{E}_{T} [GeV]", NBINS, metbins);
  plots0l->addTreeVar("met_qcdcr_nbgeq1",           "met",         sel["qcdcr"],                "#slash{E}_{T} [GeV]", NBINS, metbins);

  plots0l->plot();

  cout << "Plotting 1lepton region" << endl;

  plotslepcr->addTreeVar("nvetomu_lepcr",           "nvetomu",     sel["lepcr"],                "Number of Muons", 3, -0.5, 2.5);
  plotslepcr->addTreeVar("nvetoele_lepcr",          "nvetolele",   sel["lepcr"],                "Number of Electrons", 3, -0.5, 2.5);
  plotslepcr->addTreeVar("nvetohpstau_lepcr",       "nvetohpstaus",sel["lepcr"],                "Number of Taus", 3, -0.5, 2.5);

  plotslepcr->addTreeVar("met_lepcr_nbgeq1",        "met",         sel["lepcr"],                "#slash{E}_{T} [GeV]", NBINS, metbins);

  plotslepcr->plot();

  cout << "Plotting photon region" << endl;

  plotsphocr->addTreeVar("met_phocr_nomtb_nbgeq1",  "met",         sel["phocr"],                "#slash{E}_{T} [GeV]", NBINS, metbins);
  plotsphocr->addTreeVar("met_phocr_nbgeq1",        "met",         sel["phocr"]+sel["mtb"],     "#slash{E}_{T} [GeV]", NBINS, metbins);

  plotsphocr->plot();

  cout << "Plotting zll region" << endl;

  plotszllcr->addTreeVar("met_zllcr_nb1",           "met",         sel["zllcr"] + sel["nb1"],      "#slash{E}_{T} [GeV]", 1, 100., 1000.0);
  plotszllcr->addTreeVar("met_zllcr_nb2",           "met",         sel["zllcr"] + sel["nb2"],      "#slash{E}_{T} [GeV]", 1, 100., 1000.0);

  plotszllcr->addTreeVar("met_zllcr_offz_nb1",      "met",         sel["zlloffcr"] + sel["nb1"],   "#slash{E}_{T} [GeV]", 1, 100., 1000.0);
  plotszllcr->addTreeVar("met_zllcr_offz_nb2",      "met",         sel["zlloffcr"] + sel["nb2"],   "#slash{E}_{T} [GeV]", 1, 100., 1000.0);

  plotszllcr->plot();

  cout << "Plotting ttZ region" << endl;

  plotsttzcr->addTreeVar("extProcess_ttzcr_sel",     "extProcess",  sel["ttzSel"],                  "extProcess", 13,-.5,12.5);
  plotsttzcr->addTreeVar("extProcess_ttzcr_noz",     "extProcess",  sel["ttzNoZ"],                  "extProcess", 13,-.5,12.5);
  plotsttzcr->addTreeVar("extProcess_ttzcr_nob",     "extProcess",  sel["ttzNoB"],                  "extProcess", 13,-.5,12.5);

  plotsttzcr->plot();

  cout << "Setting up prediction" << endl;

  vector<Sample*> samples0l;
  PlotTools::initSamples(srconf, samples0l);

  TFile* file0l      = new TFile(plots0l->outfileName());
  TFile* filelepcr   = new TFile(plotslepcr->outfileName());
  TFile* filephocr   = new TFile(plotsphocr->outfileName());
  TFile* filezllcr   = new TFile(plotszllcr->outfileName());
  TFile* filettzcr   = new TFile(plotsttzcr->outfileName());

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
  TH1F* znunu          = getZPred  (file0l, filephocr, filezllcr, "sr", "phocr", "zllcr", srbins, phocrtosr, zllcrtosr);
  TH1F* qcd            = getQCDPred(file0l, "sr", "qcdcr", srbins, qcdcrtosr, qcdfitfile);
  TH1F* ttz            = getTTZPred(file0l, filettzcr, "sr", "ttzcr", srbins, usettzsf);

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
  plots->setHeaderText("#sqrt{s} = 13 TeV",lumistr+" fb^{-1}","");
  plots->setHeaderPosition(0.2, 0.93);
  plots->setRatioPlot();
  plots->setColor("lostlep_pred_sr",StyleTools::color_ttbar);
  plots->setMaxScale(2.0);  

  vector<TString> labels = {"Data", "t#bar{t}/W", "Z#rightarrow#nu#nu", "t#bar{t}Z", "QCD"};
  vector<TString> names = {"data_sr", "lostlep_pred_sr", "znunu_pred_sr", "ttZ_pred_sr", "qcd_pred_sr"};

  plots->setDataName("data_sr");
  plots->addCompSet("datavspred",names,labels);

  plots->plot();

  delete plots;

}
