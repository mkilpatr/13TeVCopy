#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#endif

enum SysVar { NOMINAL, VARUP, VARDOWN, NONE };

vector<TString> vars = {"btag","mistag","jes","lep","pu","lep","tau","wjetsNorm","ttbarNorm"};
vector<TString> bkgs = {"ttz","qcd","lostlep","znunu"};
vector<TString> sigs = {"T2tt_700_1","T2tt_600_200"};
vector<TString> labels = {"t#bar{t}Z","QCD","t#bar{t}/W","Z#rightarrow#nu#nu"};
vector<TString> siglabels = {"T2tt(700,1)","T2tt(600,200)"};

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
    "[250, 300]", "[300, 400]", "[400, 500]", "[500, 600]", ">600",
    "[250, 300]", "[300, 400]", "[400, 500]", "[500, 600]", ">600",
    "[250, 300]", "[300, 400]", "[400, 500]", "[500, 600]", ">600",
    "[250, 300]", "[300, 400]", "[400, 500]", "[500, 600]", ">600",
    "[250, 300]", "[300, 400]", "[400, 500]", "[500, 600]", ">600",
    "[250, 300]", "[300, 400]", "[400, 500]", "[500, 600]", ">600",
    "[250, 300]", "[300, 400]", "[400, 500]", "[500, 600]", ">600",
    "[250, 300]", "[300, 400]", "[400, 500]", "[500, 600]", ">600",
    "[250, 300]", "[300, 400]", "[400, 500]", "[500, 600]", ">600",
    "[250, 300]", "[300, 400]", "[400, 500]", "[500, 600]", ">600"
};

void setBinLabels(TH1 *h, const vector<TString>& labels){
  for (unsigned i=0; i<binlabels.size() && i<h->GetNbinsX(); ++i){
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
    nominal->SetBinError(ibin, sqrt(binerr*binerr + sysopt == 0 ? sysmax*sysmax : sysavg*sysavg));
  }

}

void plotFinalYieldsandUncs(const TString inputDir = "plots_bkgest",
                            vector<TString> fileNames = {"ttz/ttzpred.root","qcd/qcdpred.root","lostlep/lostleppred.root","zinv/znunupred.root"},
                            const TString sigFileName = "signal/signalpred.root",
                            const TString histSuffix = "_pred_sr",
                            const TString outputFileName = "fullbkgestwithunc.root",
                            const TString lumistr = "2.262",
                            const TString format = "pdf",
                            const bool    plotbkgunc = true,
                            const bool    plotlog = true)
{

  TFile* outfile = new TFile(outputFileName,"RECREATE");

  vector<TH1F*> nomhists, sighists;

  TH1F* bkgtotal = 0;
  vector<TH1F*> bkgtotalsup, bkgtotalsdown;

  for (unsigned int ibkg = 0; ibkg < bkgs.size(); ++ibkg) {
    auto bkg = bkgs[ibkg];
    TString inputFileName = inputDir + "/" + fileNames[ibkg];
    TFile* file = new TFile(inputFileName);
    TH1F* hbkg = (TH1F*)file->Get(bkg + histSuffix);
    assert(hbkg);
    setBinLabels(hbkg, plotlabels);
    nomhists.push_back(hbkg);
    if(ibkg == 0) {
      bkgtotal = (TH1F*)hbkg->Clone("bkgtotal");
    } else {
      bkgtotal->Add(hbkg);
    }
    for(unsigned int ivar = 0; ivar < vars.size(); ++ivar) {
      auto var = vars[ivar];
      TString upname = bkg + histSuffix + "_" + var + "up";
      TString downname = bkg + histSuffix + "_" + var + "down";
      TH1F* hbkgup = (TH1F*)file->Get(upname);
      TH1F* hbkgdown = (TH1F*)file->Get(downname);
      if (!hbkgup) {
        cout << "No var " << var << " up for " << bkg.Data() << endl;
        hbkgup = hbkg;
      }
      if (!hbkgdown) {
        cout << "No var " << var << " down for " << bkg.Data() << endl;
        hbkgdown = hbkg;
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

  for(unsigned int ivar = 0; ivar < vars.size(); ++ivar) {
    auto var = vars[ivar];
    addSysUnc(bkgtotal, bkgtotalsup[ivar], bkgtotalsdown[ivar], var, "total");
  }

  TString sigInputFileName = inputDir + "/" + sigFileName;
  TFile* sigfile = new TFile(sigInputFileName);
  for (unsigned int isig = 0; isig < sigs.size(); ++isig) {
    auto sig = sigs[isig];
    TH1F* hsig = (TH1F*)sigfile->Get(sig + histSuffix);
    assert(hsig);
    setBinLabels(hsig, plotlabels);
    sighists.push_back(hsig);
  }

  outfile->cd();

  for(unsigned int ibkg = 0; ibkg < bkgs.size(); ++ibkg) {
    nomhists[ibkg]->Write(bkgs[ibkg]);
  }

  for(unsigned int isig = 0; isig < sigs.size(); ++isig) {
    sighists[isig]->Write(sigs[isig]);
  }

  StyleTools::ColorMap colormap = StyleTools::DefaultColors();
  colormap["lostlep"] = StyleTools::color_ttbar;
  colormap["ttz"] = StyleTools::color_ttZ;
  colormap["znunu"] = kRed-9;
  colormap["T2tt_700_1"] = kRed;
  colormap["T2tt_600_200"] = kViolet-1;

  StyleTools::SetStyle();

  TCanvas* mycanv = StyleTools::MakeCanvas("myc","",800,600);

  Plot* plot_sr1 = new Plot("sryields_lowmt_mednj","","#slash{E}_{T} [GeV]","Events");
  Plot* plot_sr2 = new Plot("sryields_lowmt_highnj","","#slash{E}_{T} [GeV]","Events");
  Plot* plot_sr3 = new Plot("sryields_highmt_nt0_mednj","","#slash{E}_{T} [GeV]","Events");
  Plot* plot_sr4 = new Plot("sryields_highmt_nt0_highnj","","#slash{E}_{T} [GeV]","Events");
  Plot* plot_sr5 = new Plot("sryields_highmt_nt1","","#slash{E}_{T} [GeV]","Events");
  for(unsigned int ibkg = 0; ibkg < bkgs.size(); ++ibkg) {
    plot_sr1->addToStack(nomhists[ibkg], labels[ibkg], colormap[bkgs[ibkg]], 1001, 1, 1, 3, 1);
    plot_sr2->addToStack(nomhists[ibkg], labels[ibkg], colormap[bkgs[ibkg]], 1001, 1, 1, 3, 1);
    plot_sr3->addToStack(nomhists[ibkg], labels[ibkg], colormap[bkgs[ibkg]], 1001, 1, 1, 3, 1);
    plot_sr4->addToStack(nomhists[ibkg], labels[ibkg], colormap[bkgs[ibkg]], 1001, 1, 1, 3, 1);
    plot_sr5->addToStack(nomhists[ibkg], labels[ibkg], colormap[bkgs[ibkg]], 1001, 1, 1, 3, 1);
  }
  if (plotbkgunc) {
    plot_sr1->addHist(bkgtotal, "Bkg. Uncertainty", "E2", kBlue+1, 3013, 1, 0);
    plot_sr1->setName(plot_sr1->getName()+"_bkgunc");
    plot_sr2->addHist(bkgtotal, "Bkg. Uncertainty", "E2", kBlue+1, 3013, 1, 0);
    plot_sr2->setName(plot_sr2->getName()+"_bkgunc");
    plot_sr3->addHist(bkgtotal, "Bkg. Uncertainty", "E2", kBlue+1, 3013, 1, 0);
    plot_sr3->setName(plot_sr3->getName()+"_bkgunc");
    plot_sr4->addHist(bkgtotal, "Bkg. Uncertainty", "E2", kBlue+1, 3013, 1, 0);
    plot_sr4->setName(plot_sr4->getName()+"_bkgunc");
    plot_sr5->addHist(bkgtotal, "Bkg. Uncertainty", "E2", kBlue+1, 3013, 1, 0);
    plot_sr5->setName(plot_sr5->getName()+"_bkgunc");
  }
  plot_sr1->getLegend()->AddEntry((TObject*)0, "", "");
  plot_sr2->getLegend()->AddEntry((TObject*)0, "", "");
  plot_sr3->getLegend()->AddEntry((TObject*)0, "", "");
  plot_sr4->getLegend()->AddEntry((TObject*)0, "", "");
  plot_sr5->getLegend()->AddEntry((TObject*)0, "", "");

  for(unsigned int isig = 0; isig < sigs.size(); ++isig) {
    plot_sr1->addHist(sighists[isig], siglabels[isig], "hist", 0, 0, colormap[sigs[isig]], 11, 1, 5);
    plot_sr2->addHist(sighists[isig], siglabels[isig], "hist", 0, 0, colormap[sigs[isig]], 11, 1, 5);
    plot_sr3->addHist(sighists[isig], siglabels[isig], "hist", 0, 0, colormap[sigs[isig]], 11, 1, 5);
    plot_sr4->addHist(sighists[isig], siglabels[isig], "hist", 0, 0, colormap[sigs[isig]], 11, 1, 5);
    plot_sr5->addHist(sighists[isig], siglabels[isig], "hist", 0, 0, colormap[sigs[isig]], 11, 1, 5);
  }
  plot_sr1->setXRange(0,10);
  plot_sr1->addLine(5,0.01,5,200,kGray+2,kDashed);
  plot_sr2->setXRange(10,20);
  plot_sr2->addLine(15,0.01,15,200,kGray+2,kDashed);
  plot_sr3->setXRange(20,30);
  plot_sr3->addLine(25,0.01,25,50,kGray+2,kDashed);
  plot_sr4->setXRange(30,40);
  plot_sr4->addLine(35,0.01,35,40,kGray+2,kDashed);
  plot_sr5->setXRange(40,50);
  plot_sr5->addLine(45,0.01,45,10,kGray+2,kDashed);

  plot_sr1->setHeader("#sqrt{s} = 13 TeV, " + TString::Format("%4.1f fb^{-1}",stof(string(lumistr.Data()))),"",0.16,0.93);
  plot_sr1->getLegend()->SetNColumns(2);
  plot_sr1->setLegend(0.2,0.75,0.7,0.9);
  plot_sr1->addTextBox("N_{b} = 1", 0.4, 0.65, 0.5, 0.7, 0, 1, 0);
  plot_sr1->addTextBox("N_{b} #geq 2", 0.8, 0.65, 0.9, 0.7, 0, 1, 0);
  plot_sr1->addTextBox("M_{T}(b_{1,2},#slash{E}_{T}) < 175 GeV", 0.65, 0.85, 0.92, 0.9, 0, 1, 0);
  plot_sr1->addTextBox("5-6 jets", 0.82, 0.8, 0.91, 0.85, 0, 1, 0);
  plot_sr2->setHeader("#sqrt{s} = 13 TeV, " + TString::Format("%4.1f fb^{-1}",stof(string(lumistr.Data()))),"",0.16,0.93);
  plot_sr2->getLegend()->SetNColumns(2);
  plot_sr2->setLegend(0.2,0.75,0.7,0.9);
  plot_sr2->addTextBox("N_{b} = 1", 0.4, 0.65, 0.5, 0.7, 0, 1, 0);
  plot_sr2->addTextBox("N_{b} #geq 2", 0.8, 0.65, 0.9, 0.7, 0, 1, 0);
  plot_sr2->addTextBox("M_{T}(b_{1,2},#slash{E}_{T}) < 175 GeV", 0.65, 0.85, 0.92, 0.9, 0, 1, 0);
  plot_sr2->addTextBox("#geq 7 jets", 0.82, 0.8, 0.91, 0.85, 0, 1, 0);
  plot_sr3->setHeader("#sqrt{s} = 13 TeV, " + TString::Format("%4.1f fb^{-1}",stof(string(lumistr.Data()))),"",0.16,0.93);
  plot_sr3->getLegend()->SetNColumns(2);
  plot_sr3->setLegend(0.2,0.75,0.7,0.9);
  plot_sr3->addTextBox("N_{b} = 1", 0.4, 0.65, 0.5, 0.7, 0, 1, 0);
  plot_sr3->addTextBox("N_{b} #geq 2", 0.8, 0.65, 0.9, 0.7, 0, 1, 0);
  plot_sr3->addTextBox("M_{T}(b_{1,2},#slash{E}_{T}) > 175 GeV", 0.65, 0.85, 0.92, 0.9, 0, 1, 0);
  plot_sr3->addTextBox("5-6 jets", 0.82, 0.8, 0.91, 0.85, 0, 1, 0);
  plot_sr3->addTextBox("N_{t} = 0", 0.82, 0.75, 0.91, 0.8, 0, 1, 0);
  plot_sr4->setHeader("#sqrt{s} = 13 TeV, " + TString::Format("%4.1f fb^{-1}",stof(string(lumistr.Data()))),"",0.16,0.93);
  plot_sr4->getLegend()->SetNColumns(2);
  plot_sr4->setLegend(0.2,0.75,0.7,0.9);
  plot_sr4->addTextBox("N_{b} = 1", 0.4, 0.65, 0.5, 0.7, 0, 1, 0);
  plot_sr4->addTextBox("N_{b} #geq 2", 0.8, 0.65, 0.9, 0.7, 0, 1, 0);
  plot_sr4->addTextBox("M_{T}(b_{1,2},#slash{E}_{T}) > 175 GeV", 0.65, 0.85, 0.92, 0.9, 0, 1, 0);
  plot_sr4->addTextBox("#geq 7 jets", 0.82, 0.8, 0.91, 0.85, 0, 1, 0);
  plot_sr4->addTextBox("N_{t} = 0", 0.82, 0.75, 0.91, 0.8, 0, 1, 0);
  plot_sr5->setHeader("#sqrt{s} = 13 TeV, " + TString::Format("%4.1f fb^{-1}",stof(string(lumistr.Data()))),"",0.16,0.93);
  plot_sr5->getLegend()->SetNColumns(2);
  plot_sr5->setLegend(0.2,0.75,0.7,0.9);
  plot_sr5->addTextBox("N_{b} = 1", 0.4, 0.65, 0.5, 0.7, 0, 1, 0);
  plot_sr5->addTextBox("N_{b} #geq 2", 0.8, 0.65, 0.9, 0.7, 0, 1, 0);
  plot_sr5->addTextBox("M_{T}(b_{1,2},#slash{E}_{T}) > 175 GeV", 0.65, 0.85, 0.92, 0.9, 0, 1, 0);
  plot_sr5->addTextBox("#geq 5 jets", 0.82, 0.8, 0.91, 0.85, 0, 1, 0);
  plot_sr5->addTextBox("N_{t} #geq 1", 0.82, 0.75, 0.91, 0.8, 0, 1, 0);

  if(plotlog) {
    plot_sr1->setName(plot_sr1->getName()+"_log");
    plot_sr1->setLogy();
    plot_sr1->setYRange(0.01,5000.0);
    plot_sr2->setName(plot_sr2->getName()+"_log");
    plot_sr2->setLogy();
    plot_sr2->setYRange(0.01,5000.0);
    plot_sr3->setName(plot_sr3->getName()+"_log");
    plot_sr3->setLogy();
    plot_sr3->setYRange(0.01,1000.0);
    plot_sr4->setName(plot_sr4->getName()+"_log");
    plot_sr4->setLogy();
    plot_sr4->setYRange(0.01,500.0);
    plot_sr5->setName(plot_sr5->getName()+"_log");
    plot_sr5->setLogy();
    plot_sr5->setYRange(0.01,100.0);
  }

  plot_sr1->draw(mycanv,true,format);
  plot_sr2->draw(mycanv,true,format);
  plot_sr3->draw(mycanv,true,format);
  plot_sr4->draw(mycanv,true,format);
  plot_sr5->draw(mycanv,true,format);

}
