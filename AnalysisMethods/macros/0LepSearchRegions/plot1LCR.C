#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#endif

vector<TString> regions = {"lowmt_mednj", "lowmt_highnj", "highmt_mednj_nt0", "highmt_highnj_nt0", "highmt_nt1" };
vector<TString> bkgs      = {/*"ttZ"      , "qcd", "znunu"             ,*/ "ttW"      , "tW", "wjets" , "ttbar"   };
vector<TString> bkglabels = {/*"t#bar{t}Z", "QCD", "Z#rightarrow#nu#nu",*/ "t#bar{t}W", "tW", "W+jets", "t#bar{t}"};
vector<TString> sigs      = {"nb1"              , "nb2"                 };
vector<TString> siglabels = {"LLB (SR, N_{b}=1)", "LLB (SR, N_{b}#geq2)"};

/*
 * To make the input histograms to this macro, run getZeroLeptonPrediction after uncommenting
 * the ttbar, wjets, tW, and ttW lines in run0lepbkgpred.conf (note that the root files for these
 * trees will need to be linked from <treeDir>/sr/ to <treeDir> similar to how the other trees
 * are).
 *
 */
void plot1LCR(const TString inputDir = "plots_bkgest",
                            const TString srFileName = "output_0l.root",
                            const TString crFileName = "output_lepcr.root",
                            const TString format  = "pdf",
                            const bool    plotlog = true
                            )
{

  TFile* infileSR = new TFile(inputDir + "/" + srFileName);
  TFile* infileCR = new TFile(inputDir + "/" + crFileName);

  StyleTools::ColorMap colormap = StyleTools::DefaultColors();
  colormap["ttbarplusw"] = StyleTools::color_ttbar;
  colormap["ttbar"]      = kCyan-7;
  colormap["wjets"]      = StyleTools::color_wjets;
  colormap["tW"]         = StyleTools::color_tW;
  colormap["ttW"]        = StyleTools::color_ttW;
  colormap["znunu"] = kRed-9;
  colormap["qcd"]   = StyleTools::color_qcd;
  colormap["ttz"]   = StyleTools::color_ttZ;
  colormap["nb1"]   = kRed;
  colormap["nb2"]   = kOrange-3; // kGreen+3; // kViolet-1 doesn't work because in intersects with tW's violet in one bin

  StyleTools::SetTDRStyle();

  for(unsigned int ireg=0; ireg<regions.size(); ++ireg) {
    auto region = regions[ireg];

    TCanvas* mycanv = StyleTools::MakeCanvas("myc","",600,600);
    Plot* plots = new Plot("met_lepcr_nbgeq1_"+region,"","#slash{E}_{T} [GeV]","Events");
    plots->setUsePoisson();
    plots->setDrawCMSLumi();

    TH1F* data = (TH1F*)infileCR->Get("met_lepcr_nbgeq1_"+region+"_data;1");
    double intdata = data->Integral(1,data->GetNbinsX());
    plots->addHist(data, "Data", "E0", 1, 0, 1, 0, 0, 2);

    // get full bkg for scaling and ratio
    TH1F* bkgtotal = 0;
    for(unsigned int ibkg = 0; ibkg<bkgs.size(); ++ibkg) {
      TH1F* hbkg = (TH1F*)infileCR->Get("met_lepcr_nbgeq1_"+region+"_"+bkgs[ibkg]+";1");
      if(ibkg == 0) bkgtotal = (TH1F*)hbkg->Clone("bkgtotal");
      else bkgtotal->Add(hbkg);
    } // bkgs
    double intbkg = bkgtotal->Integral(1,bkgtotal->GetNbinsX());

    // get and scale individual bkgs for plotting
    TH1F* unc = 0;
    for(unsigned int ibkg = 0; ibkg<bkgs.size(); ++ibkg) {
      TH1F* hbkg = (TH1F*)infileCR->Get("met_lepcr_nbgeq1_"+region+"_"+bkgs[ibkg]+";1");
      hbkg->Scale(intdata/intbkg);
      if(ibkg == 0) unc = (TH1F*)hbkg->Clone("unc");
      else unc->Add(hbkg);
      for(unsigned int i=1; i<=hbkg->GetNbinsX(); ++i) hbkg->SetBinError(i,0);
      plots->addToStack(hbkg, bkglabels[ibkg], colormap[bkgs[ibkg]], 1001, 1, 1, 2, 0);
    } // bkgs
    plots->setUncertaintyHist(unc);
    plots->setPlotStackUncertainty();
    plots->setPlotRatioUncertaintyBand();

    for(unsigned int isig = 0; isig<sigs.size(); ++isig) {
      TH1F* hsig = (TH1F*)infileSR->Get("met_sr_"+region+"_"+sigs[isig]+"_ttbarplusw"+";1");
      hsig->Scale(intdata/hsig->Integral(1,hsig->GetNbinsX()));
      //plots->addHist(hsig, siglabels[isig], "hist", 0, 0, colormap[sigs[isig]], 11, 0, 3);
      plots->addHistForRatio(hsig, siglabels[isig], "mc_hist", 0, 0, colormap[sigs[isig]], 11, 0, 3, 0);
    }

    plots->setXRange(0,5);
    if(plotlog) {
      plots->setName(plots->getName()+"_log");
      plots->setLogy();
      plots->setYRange(0.03,5000.0);
    }

    plots->drawRatioStack(mycanv,true,format);

    delete plots;

  } // regions

}
