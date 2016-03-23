#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#endif

enum SysVar { NOMINAL, VARUP, VARDOWN, NONE };

vector<TString> vars = {};
vector<TString> bkgs = {"ttZ","qcd","lostlep","znunu"};
vector<TString> rawbkgs = {"ttZ","qcd","ttbarplusw","znunu"};
vector<TString> labels = {"t#bar{t}Z","QCD","t#bar{t}/W+jets","Z(#nu#bar{#nu})+jets"};
vector<TString> sigs = {"T2tt_700_100", "T2tt_250_150", "T2tb_700_100"};
vector<TString> siglabels = {"T2tt(700,100)", "T2tt(250,150)", "T2tb(700,100)"};
const TString unc_name = "bkgtotal_unc_sr";

vector<TString> binlabels = {"bin_250_5_1_0_0", "bin_300_5_1_0_0", "bin_400_5_1_0_0", "bin_500_5_1_0_0", "bin_600_5_1_0_0",
                             "bin_250_5_2_0_0", "bin_300_5_2_0_0", "bin_400_5_2_0_0", "bin_500_5_2_0_0", "bin_600_5_2_0_0",
                             "bin_250_7_1_0_0", "bin_300_7_1_0_0", "bin_400_7_1_0_0", "bin_500_7_1_0_0", "bin_600_7_1_0_0",
                             "bin_250_7_2_0_0", "bin_300_7_2_0_0", "bin_400_7_2_0_0", "bin_500_7_2_0_0", "bin_600_7_2_0_0",
                             "bin_250_5_1_0_175", "bin_300_5_1_0_175", "bin_400_5_1_0_175", "bin_500_5_1_0_175", "bin_600_5_1_0_175",
                             "bin_250_5_2_0_175", "bin_300_5_2_0_175", "bin_400_5_2_0_175", "bin_500_5_2_0_175", "bin_600_5_2_0_175",
                             "bin_250_7_1_0_175", "bin_300_7_1_0_175", "bin_400_7_1_0_175", "bin_500_7_1_0_175", "bin_600_7_1_0_175",
                             "bin_250_7_2_0_175", "bin_300_7_2_0_175", "bin_400_7_2_0_175", "bin_500_7_2_0_175", "bin_600_7_2_0_175",
                             "bin_250_5_1_1_175", "bin_300_5_1_1_175", "bin_400_5_1_1_175", "bin_500_5_1_1_175", "bin_600_5_1_1_175",
                             "bin_250_5_2_1_175", "bin_300_5_2_1_175", "bin_400_5_2_1_175", "bin_500_5_2_1_175", "bin_600_5_2_1_175"};

vector<TString> plotlabels {
    "[250, 300)", "[300, 400)", "[400, 500)", "[500, 600)", "#geq 600",
    "[250, 300)", "[300, 400)", "[400, 500)", "[500, 600)", "#geq 600",
    "[250, 300)", "[300, 400)", "[400, 500)", "[500, 600)", "#geq 600",
    "[250, 300)", "[300, 400)", "[400, 500)", "[500, 600)", "#geq 600",
    "[250, 300)", "[300, 400)", "[400, 500)", "[500, 600)", "#geq 600",
    "[250, 300)", "[300, 400)", "[400, 500)", "[500, 600)", "#geq 600",
    "[250, 300)", "[300, 400)", "[400, 500)", "[500, 600)", "#geq 600",
    "[250, 300)", "[300, 400)", "[400, 500)", "[500, 600)", "#geq 600",
    "[250, 300)", "[300, 400)", "[400, 500)", "[500, 600)", "#geq 600",
    "[250, 300)", "[300, 400)", "[400, 500)", "[500, 600)", "#geq 600"
};

vector<vector<TString>> regionLabels = {
    {"M_{T}(b_{1,2},#slash{E}_{T}) < 175 GeV", "5-6 jets"},
    {"M_{T}(b_{1,2},#slash{E}_{T}) < 175 GeV", "#geq 7 jets"},
    {"M_{T}(b_{1,2},#slash{E}_{T}) > 175 GeV", "5-6 jets, N_{t} = 0"},
    {"M_{T}(b_{1,2},#slash{E}_{T}) > 175 GeV", "#geq 7 jets, N_{t} = 0"},
    {"M_{T}(b_{1,2},#slash{E}_{T}) > 175 GeV", "#geq 5 jets, N_{t} #geq 1"}
};


void setBinLabels(TH1 *h, const vector<TString>& labels){
  for (unsigned i=0; i<labels.size() && i<h->GetNbinsX(); ++i){
    h->GetXaxis()->SetBinLabel(i+1, labels.at(i));
  }
}

void addSysUnc(TH1F* nominal, TH1F* varup, TH1F* vardown, TString name, TString procname, int sysopt = 1)
{

  printf("Effects of %s variation on %s:\n",name.Data(),procname.Data());
  printf("Bin \t +1 sigma \t -1 sigma\n");

  for(int ibin = 1; ibin < nominal->GetNbinsX()+1; ++ibin) {

    double nom   = nominal->GetBinContent(ibin);
    double sysup = varup->GetBinContent(ibin) - nominal->GetBinContent(ibin);
    double sysdown = vardown->GetBinContent(ibin) - nominal->GetBinContent(ibin);

    //if(sysup * sysdown > 0) printf("Up/down variations for %s in bin%d are in the same direction!%4.2f, %4.2f, %4.2f\n",name.Data(),ibin,nom,nom+sysup,nom+sysdown);
    double sysmax = max(fabs(sysup),fabs(sysdown));
    if(sysup < 0) sysmax *= -1;
    double sysavg = 0.5*(sysup - sysdown);

    if(nom == 0) printf("%d: 0 bin\n",ibin);
    else         {
      printf("%4.2f\t%4.2f\n",(1.0 + sysup/nom),(1.0 + sysdown/nom));
      if(sysopt == 0) printf("%s\t%4.2f\n",binlabels[ibin-1].Data(),(1.0 + sysmax/nom));
      else if(sysopt == 1) printf("%s\t%4.2f\n",binlabels[ibin-1].Data(),(1.0 + sysavg/nom));
    }

    double binerr = nominal->GetBinError(ibin);
    nominal->SetBinError(ibin, sqrt(binerr*binerr + (sysopt == 0 ? sysmax*sysmax : sysavg*sysavg)));
  }

}

void plotFinalYieldsandUncs(const TString inputDir = "plots_bkgest",
                            const TString inputFileName = "srpred.root",
                            const TString predHistSuffix = "_pred_sr",
                            const TString rawHistSuffix = "_raw_sr",
                            const TString outputFileName = "fullbkgestwithunc.root",
                            const TString format = "pdf",
                            const bool    plotbkgunc = true,
                            const bool    plotlog = true)
{

  TFile* outfile = new TFile(outputFileName,"RECREATE");

  vector<TH1F*> nomhists, rawhists, sighists;

  TH1F *bkgtotal = 0, *rawbkgtotal = 0;
  vector<TH1F*> bkgtotalsup, bkgtotalsdown;

  TFile* infile = new TFile(inputDir + "/" + inputFileName);

  TH1F* data = (TH1F*)infile->Get("data_sr");
  setBinLabels(data, plotlabels);

  for (unsigned int ibkg = 0; ibkg < bkgs.size(); ++ibkg) {
    auto bkg = bkgs[ibkg];
    TH1F* hbkgpred = (TH1F*)infile->Get(bkg + predHistSuffix);
    TH1F* hbkgraw  = (TH1F*)infile->Get(rawbkgs[ibkg] + rawHistSuffix);
    assert(hbkgpred);
    assert(hbkgraw);
    setBinLabels(hbkgpred, plotlabels);
    setBinLabels(hbkgraw, plotlabels);
    nomhists.push_back(hbkgpred);
    rawhists.push_back(hbkgraw);
    if(ibkg == 0) {
      bkgtotal = (TH1F*)hbkgpred->Clone("bkgtotal");
      rawbkgtotal = (TH1F*)hbkgraw->Clone("rawbkgtotal");
    } else {
      bkgtotal->Add(hbkgpred);
      rawbkgtotal->Add(hbkgraw);
    }
    for(unsigned int ivar = 0; ivar < vars.size(); ++ivar) {
      auto var = vars[ivar];
      TString upname = bkg + predHistSuffix + "_" + var + "up";
      TString downname = bkg + predHistSuffix + "_" + var + "down";
      TH1F* hbkgup = (TH1F*)infile->Get(upname);
      TH1F* hbkgdown = (TH1F*)infile->Get(downname);
      if (!hbkgup) {
        cout << "No var " << var << " up for " << bkg.Data() << endl;
        hbkgup = hbkgpred;
      }
      if (!hbkgdown) {
        cout << "No var " << var << " down for " << bkg.Data() << endl;
        hbkgdown = hbkgpred;
      }
      if(ibkg == 0) {
        bkgtotalsup.push_back((TH1F*)hbkgup->Clone("bkgtotal_varup"));
        bkgtotalsdown.push_back((TH1F*)hbkgdown->Clone("bkgtotal_vardown"));
      } else {
        bkgtotalsup[ivar]->Add(hbkgup);
        bkgtotalsdown[ivar]->Add(hbkgdown);
      }
      addSysUnc(nomhists[ibkg], hbkgup, hbkgdown, var, bkg);
    }
  }

  // get uncertainty hist
  TH1F* unc = (TH1F*)infile->Get(unc_name);

  for (auto& sig : sigs) {
    TH1F* hsig = (TH1F*)infile->Get(sig + rawHistSuffix);
    assert(hsig);
    setBinLabels(hsig, plotlabels);
    sighists.push_back(hsig);
  }

  outfile->cd();

  for(unsigned int ibkg = 0; ibkg < bkgs.size(); ++ibkg) {
    nomhists[ibkg]->Write(bkgs[ibkg]);
    rawhists[ibkg]->Write(bkgs[ibkg] + "_raw");
  }

  for(unsigned int isig = 0; isig < sigs.size(); ++isig) {
    sighists[isig]->Write(sigs[isig]);
  }

  StyleTools::ColorMap colormap = StyleTools::DefaultColors();
  colormap["lostlep"] = StyleTools::color_ttbar;
  colormap["ttz"] = StyleTools::color_ttZ;
  colormap["znunu"] = kRed-9;
  colormap["T2tt_700_100"] = kRed;
  colormap["T2tt_250_150"] = kViolet-1;
  colormap["T2tb_700_100"] = kGreen+3;

  StyleTools::SetTDRStyle();

  TCanvas* mycanv1 = StyleTools::MakeCanvas("myc1","",800,600);
  TCanvas* mycanv2 = StyleTools::MakeCanvas("myc2","",800,600);
  TCanvas* mycanv3 = StyleTools::MakeCanvas("myc3","",800,600);
  TCanvas* mycanv4 = StyleTools::MakeCanvas("myc4","",800,600);
  TCanvas* mycanv5 = StyleTools::MakeCanvas("myc5","",800,600);

  Plot* plot_sr1 = new Plot("sryields_lowmt_mednj","","#slash{E}_{T} [GeV]","Events");
  Plot* plot_sr2 = new Plot("sryields_lowmt_highnj","","#slash{E}_{T} [GeV]","Events");
  Plot* plot_sr3 = new Plot("sryields_highmt_nt0_mednj","","#slash{E}_{T} [GeV]","Events");
  Plot* plot_sr4 = new Plot("sryields_highmt_nt0_highnj","","#slash{E}_{T} [GeV]","Events");
  Plot* plot_sr5 = new Plot("sryields_highmt_nt1","","#slash{E}_{T} [GeV]","Events");

  plot_sr1->setUsePoisson();
  plot_sr2->setUsePoisson();
  plot_sr3->setUsePoisson();
  plot_sr4->setUsePoisson();
  plot_sr5->setUsePoisson();

  plot_sr1->setDrawCMSLumi(0);
  plot_sr2->setDrawCMSLumi(0);
  plot_sr3->setDrawCMSLumi(0);
  plot_sr4->setDrawCMSLumi(0);
  plot_sr5->setDrawCMSLumi(0);

  plot_sr1->addHist(data, "Observed", "E0", 1, 0, 1, 0, 0, 2);
  plot_sr2->addHist(data, "Observed", "E0", 1, 0, 1, 0, 0, 2);
  plot_sr3->addHist(data, "Observed", "E0", 1, 0, 1, 0, 0, 2);
  plot_sr4->addHist(data, "Observed", "E0", 1, 0, 1, 0, 0, 2);
  plot_sr5->addHist(data, "Observed", "E0", 1, 0, 1, 0, 0, 2);
  plot_sr1->getLegend()->AddEntry((TObject*)0, "", "");
  plot_sr2->getLegend()->AddEntry((TObject*)0, "", "");
  plot_sr3->getLegend()->AddEntry((TObject*)0, "", "");
  plot_sr4->getLegend()->AddEntry((TObject*)0, "", "");
  plot_sr5->getLegend()->AddEntry((TObject*)0, "", "");

  for(unsigned int ibkg = 0; ibkg < bkgs.size(); ++ibkg) {
    plot_sr1->addToStack(nomhists[ibkg], labels[ibkg], colormap[bkgs[ibkg]], 1001, 1, 1, 2, 0);
    plot_sr2->addToStack(nomhists[ibkg], labels[ibkg], colormap[bkgs[ibkg]], 1001, 1, 1, 2, 0);
    plot_sr3->addToStack(nomhists[ibkg], labels[ibkg], colormap[bkgs[ibkg]], 1001, 1, 1, 2, 0);
    plot_sr4->addToStack(nomhists[ibkg], labels[ibkg], colormap[bkgs[ibkg]], 1001, 1, 1, 2, 0);
    plot_sr5->addToStack(nomhists[ibkg], labels[ibkg], colormap[bkgs[ibkg]], 1001, 1, 1, 2, 0);
  }

  if (plotbkgunc) {
    TH1F *histUnc = unc ? unc : bkgtotal;
    plot_sr1->setUncertaintyHist(histUnc);
    plot_sr1->setPlotStackUncertainty();
    plot_sr1->setPlotRatioUncertaintyBand();
    plot_sr2->setUncertaintyHist(histUnc);
    plot_sr2->setPlotStackUncertainty();
    plot_sr2->setPlotRatioUncertaintyBand();
    plot_sr3->setUncertaintyHist(histUnc);
    plot_sr3->setPlotStackUncertainty();
    plot_sr3->setPlotRatioUncertaintyBand();
    plot_sr4->setUncertaintyHist(histUnc);
    plot_sr4->setPlotStackUncertainty();
    plot_sr4->setPlotRatioUncertaintyBand();
    plot_sr5->setUncertaintyHist(histUnc);
    plot_sr5->setPlotStackUncertainty();
    plot_sr5->setPlotRatioUncertaintyBand();
  }

  for(unsigned int isig = 0; isig < sigs.size(); ++isig) {
    plot_sr1->addHist(sighists[isig], siglabels[isig], "hist", 0, 0, colormap[sigs[isig]], 11, 0, 3);
    plot_sr2->addHist(sighists[isig], siglabels[isig], "hist", 0, 0, colormap[sigs[isig]], 11, 0, 3);
    plot_sr3->addHist(sighists[isig], siglabels[isig], "hist", 0, 0, colormap[sigs[isig]], 11, 0, 3);
    plot_sr4->addHist(sighists[isig], siglabels[isig], "hist", 0, 0, colormap[sigs[isig]], 11, 0, 3);
    plot_sr5->addHist(sighists[isig], siglabels[isig], "hist", 0, 0, colormap[sigs[isig]], 11, 0, 3);
  }

//  plot_sr1->addHistForRatio(rawbkgtotal, "Simulation", "hist", kPink-1, 0, kPink-1, 1, 0, 2, 1);
//  plot_sr2->addHistForRatio(rawbkgtotal, "Simulation", "hist", kPink-1, 0, kPink-1, 1, 0, 2, 1);
//  plot_sr3->addHistForRatio(rawbkgtotal, "Simulation", "hist", kPink-1, 0, kPink-1, 1, 0, 2, 1);
//  plot_sr4->addHistForRatio(rawbkgtotal, "Simulation", "hist", kPink-1, 0, kPink-1, 1, 0, 2, 1);
//  plot_sr5->addHistForRatio(rawbkgtotal, "Simulation", "hist", kPink-1, 0, kPink-1, 1, 0, 2, 1);

  plot_sr1->setXRange(0,10);
  plot_sr1->addLine(5,0.03,5,150,kGray+2,kDashed);
  plot_sr2->setXRange(10,20);
  plot_sr2->addLine(15,0.03,15,90,kGray+2,kDashed);
  plot_sr3->setXRange(20,30);
  plot_sr3->addLine(25,0.03,25,60,kGray+2,kDashed);
  plot_sr4->setXRange(30,40);
  plot_sr4->addLine(35,0.03,35,30,kGray+2,kDashed);
  plot_sr5->setXRange(40,50);
  plot_sr5->addLine(45,0.04,45,8,kGray+2,kDashed);

  plot_sr1->getLegend()->SetNColumns(2);
  plot_sr1->setLegend(0.2,0.63,0.68,0.88);

  plot_sr2->getLegend()->SetNColumns(2);
  plot_sr2->setLegend(0.2,0.63,0.68,0.88);

  plot_sr3->getLegend()->SetNColumns(2);
  plot_sr3->setLegend(0.2,0.63,0.68,0.88);

  plot_sr4->getLegend()->SetNColumns(2);
  plot_sr4->setLegend(0.2,0.63,0.68,0.88);

  plot_sr5->getLegend()->SetNColumns(2);
  plot_sr5->setLegend(0.2,0.63,0.68,0.88);

  if(plotlog) {
    plot_sr1->setName(plot_sr1->getName()+"_log");
    plot_sr1->setLogy();
    plot_sr1->setYRange(0.1,12000.0);
    plot_sr2->setName(plot_sr2->getName()+"_log");
    plot_sr2->setLogy();
    plot_sr2->setYRange(0.1,5000.0);
    plot_sr3->setName(plot_sr3->getName()+"_log");
    plot_sr3->setLogy();
    plot_sr3->setYRange(0.1,8000.0);
    plot_sr4->setName(plot_sr4->getName()+"_log");
    plot_sr4->setLogy();
    plot_sr4->setYRange(0.1,2000.0);
    plot_sr5->setName(plot_sr5->getName()+"_log");
    plot_sr5->setLogy();
    plot_sr5->setYRange(0.1,200.0);
  }

  TLatex tl;
  tl.SetTextSize(0.037);
  tl.SetTextAlign(31);

  plot_sr1->drawRatioStack(mycanv1,true,format);
  mycanv1->cd();
  tl.DrawLatexNDC(0.4, 0.65, "N_{b} = 1");
  tl.DrawLatexNDC(0.8, 0.65, "N_{b} #geq 2");
  tl.DrawLatexNDC(0.92, 0.88, "HPTT");
  tl.DrawLatexNDC(0.92, 0.83, regionLabels[0].front());
  tl.DrawLatexNDC(0.92, 0.78, regionLabels[0].back());
  mycanv1->SaveAs(plot_sr1->getName()+".pdf");

  plot_sr2->drawRatioStack(mycanv2,true,format);
  mycanv2->cd();
  tl.DrawLatexNDC(0.4, 0.65, "N_{b} = 1");
  tl.DrawLatexNDC(0.8, 0.65, "N_{b} #geq 2");
  tl.DrawLatexNDC(0.92, 0.88, "HPTT");
  tl.DrawLatexNDC(0.92, 0.83, regionLabels[1].front());
  tl.DrawLatexNDC(0.92, 0.78, regionLabels[1].back());
  mycanv2->SaveAs(plot_sr2->getName()+".pdf");


  plot_sr3->drawRatioStack(mycanv3,true,format);
  mycanv3->cd();
  tl.DrawLatexNDC(0.4, 0.65, "N_{b} = 1");
  tl.DrawLatexNDC(0.8, 0.65, "N_{b} #geq 2");
  tl.DrawLatexNDC(0.92, 0.88, "HPTT");
  tl.DrawLatexNDC(0.92, 0.83, regionLabels[2].front());
  tl.DrawLatexNDC(0.92, 0.78, regionLabels[2].back());
  mycanv3->SaveAs(plot_sr3->getName()+".pdf");

  plot_sr4->drawRatioStack(mycanv4,true,format);
  mycanv4->cd();
  tl.DrawLatexNDC(0.4, 0.65, "N_{b} = 1");
  tl.DrawLatexNDC(0.8, 0.65, "N_{b} #geq 2");
  tl.DrawLatexNDC(0.92, 0.88, "HPTT");
  tl.DrawLatexNDC(0.92, 0.83, regionLabels[3].front());
  tl.DrawLatexNDC(0.92, 0.78, regionLabels[3].back());
  mycanv4->SaveAs(plot_sr4->getName()+".pdf");

  plot_sr5->drawRatioStack(mycanv5,true,format);
  mycanv5->cd();
  tl.DrawLatexNDC(0.37, 0.65, "N_{b} = 1");
  tl.DrawLatexNDC(0.8, 0.65, "N_{b} #geq 2");
  tl.DrawLatexNDC(0.92, 0.88, "HPTT");
  tl.DrawLatexNDC(0.92, 0.83, regionLabels[4].front());
  tl.DrawLatexNDC(0.92, 0.78, regionLabels[4].back());
  mycanv5->SaveAs(plot_sr5->getName()+".pdf");

}
