#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/Plot.hh"
#include "AnalysisMethods/PlotUtils/interface/HistogramGetter.hh"
#include "QCDPredictionHelper.h"
#include "TDirectory.h"
#include "TFile.h"

using namespace std;

const float CR_cutoff = .1;

void MakeLepShapePlots(QCDSupport::SRegInfo& srinfo, vector<TString>& names, vector<TTree*>& trees, vector<int>& colors) {
  cout << "MakeLepShapePlots..." << endl;
  const TString outputDir = "MakeLepShapePlots";
  gSystem->mkdir(outputDir, true);

  HistogramGetter* met_HG = new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", 20, 200., 1000.);
  for(unsigned int iSR = 0; iSR < 1/*srinfo.nSR*/; ++iSR){
    TString title = TString::Format("%s", srinfo.srSelNames[iSR].Data());
    TString intTitle = TString::Format("%s_integral", srinfo.srSelNames[iSR].Data());
    TString name  = TString::Format("met_lep_shape");
    //TString name  = TString::Format("met_%s", srinfo.srRegBinNames[iSR].Data());
    TString intName = TString::Format("met_lep_shape_integral");
    TString sel          = TString::Format("%s && %s && %s",       	       QCDSupport::METPresel.Data(), QCDSupport::BaselineExtraCuts.Data(), QCDSupport::oneLep.Data());
    TString elecSel      = TString::Format("%s && %s && %s && %s", 	       QCDSupport::METPresel.Data(), QCDSupport::BaselineExtraCuts.Data(), QCDSupport::oneLep.Data(), QCDSupport::elecSel.Data());
    TString muSel        = TString::Format("%s && %s && %s && %s", 	       QCDSupport::METPresel.Data(), QCDSupport::BaselineExtraCuts.Data(), QCDSupport::oneLep.Data(), QCDSupport::muSel.Data());
    TString tauSelElec   = TString::Format("%s && %s && %s && %s && %s",       QCDSupport::METPresel.Data(), QCDSupport::BaselineExtraCuts.Data(), QCDSupport::oneLep.Data(), QCDSupport::tauSel.Data(), QCDSupport::tauElecSel.Data());
    TString tauSelMuon   = TString::Format("%s && %s && %s && %s && %s",       QCDSupport::METPresel.Data(), QCDSupport::BaselineExtraCuts.Data(), QCDSupport::oneLep.Data(), QCDSupport::tauSel.Data(), QCDSupport::tauMuSel.Data());
    TString tauSelHadr   = TString::Format("%s && %s && %s && %s && %s && %s", QCDSupport::METPresel.Data(), QCDSupport::BaselineExtraCuts.Data(), QCDSupport::oneLep.Data(), QCDSupport::tauSel.Data(), QCDSupport::negelecSel.Data(), QCDSupport::negmuSel.Data());
//cout << title << endl;
cout << name  << endl;
cout << elecSel   << endl;
cout << muSel   << endl;
cout << tauSelElec   << endl;
cout << tauSelMuon   << endl;
cout << tauSelHadr   << endl;
    Plot* plot_allMet = new Plot(name, title, met_HG->plotInfo->xTitle, "Events");
    Plot* plot_int = new Plot(intName, intTitle, met_HG->plotInfo->xTitle, "Events");
    float yMin = 99999., yMax = 0., yMinInt = 99999., yMaxInt = 0., yMaxTotal = 0.;
    float yMaxIntArray[5] = {};
    float totMCYield = 0;
    TH1F* hist_nonQCD_bkg = 0;
    TH1F* hist = 0;
    TH1F* hist_elec = 0; 
    TH1F* hist_muon = 0;
    TH1F* intHist_nonQCD = 0;
    for(unsigned int iT = 0; iT < trees.size(); ++iT){
      if(names[iT] == "Data"){
        TH1F* hist = met_HG->getHistogram(trees[iT], elecSel, "1.0", TString::Format("data_%s", name.Data()));
        for(int iBin = 1; iBin <= hist->GetNbinsX(); ++iBin) if(hist->GetBinLowEdge(iBin) + hist->GetBinWidth(iBin) > CR_cutoff) hist->SetBinContent(iBin, -999);
        plot_allMet->addHist(hist, names[iT], "E0", colors[iT], 0, colors[iT]);
        TH1F* intHist = (TH1F*) hist->Clone();
        for(unsigned int iBin = 1; iBin <= hist->GetNbinsX(); ++iBin) intHist->SetBinContent(iBin, hist->Integral(0, iBin));
        plot_int->addHist(intHist, names[iT], "E0", colors[iT], 0, colors[iT]);
      } else if(names[iT] == "Non-QCD bkg"){
        hist_nonQCD_bkg = met_HG->getHistogram(trees[iT], elecSel, QCDSupport::stdMCWeightNonQCD, TString::Format("nonqcd_%s", name.Data()));
        totMCYield += hist_nonQCD_bkg->Integral(0, hist_nonQCD_bkg->GetNbinsX() + 1);
        plot_allMet->addToStack(hist_nonQCD_bkg, names[iT], colors[iT], 1001, 1, 1, 3);
        intHist_nonQCD = (TH1F*) hist_nonQCD_bkg->Clone();
        for(unsigned int iBin = 1; iBin <= hist_nonQCD_bkg->GetNbinsX(); ++iBin) intHist_nonQCD->SetBinContent(iBin, hist_nonQCD_bkg->Integral(0, iBin));
        plot_int->addToStack(intHist_nonQCD, names[iT], colors[iT], 1001, 1, 1, 3);
      } else if(names[iT].Contains("Smeared")){
        TH1F* hist = met_HG->getHistogram(trees[iT], elecSel, QCDSupport::stdQCDWeight, TString::Format("qcd_%s", name.Data()));
        totMCYield += hist->Integral(0, hist->GetNbinsX() + 1);
        if(yMin > hist->GetMinimum()) yMin = hist->GetMinimum();
        if(yMax < hist->GetMaximum()) yMax = hist->GetMaximum();
        plot_allMet->addToStack(hist, names[iT], colors[iT], 1001, 1, 1, 3);
        TH1F* intHist = (TH1F*) hist->Clone();
        for(unsigned int iBin = 1; iBin <= hist->GetNbinsX(); ++iBin) intHist->SetBinContent(iBin, hist->Integral(0, iBin));
        plot_int->addToStack(intHist, names[iT], colors[iT], 1001, 1, 1, 3);
        if(yMinInt > intHist->GetMinimum()) yMinInt = intHist->GetMinimum();
        if(yMaxInt < intHist->GetMaximum()) yMaxInt = intHist->GetMaximum();
      } else if(names[iT] == "With orig. QCD MC"){
        TH1F* hist = met_HG->getHistogram(trees[iT], elecSel, QCDSupport::stdQCDWeight, TString::Format("qcd_orig_%s", name.Data()));
        hist->Add(hist_nonQCD_bkg);
        if(yMin > hist->GetMinimum()) yMin = hist->GetMinimum();
        if(yMax < hist->GetMaximum()) yMax = hist->GetMaximum();
        plot_allMet->addHist(hist, names[iT], "hist", colors[iT], 0, colors[iT]);
        TH1F* intHist = (TH1F*) hist->Clone();
        intHist->Add(intHist_nonQCD);
        for(unsigned int iBin = 1; iBin <= hist->GetNbinsX(); ++iBin) intHist->SetBinContent(iBin, hist->Integral(0, iBin));
        plot_int->addHist(intHist, names[iT], "intHist", colors[iT], 0, colors[iT]);
      } else if(names[iT] == "Total"){
        TH1F* hist = met_HG->getHistogram(trees[iT], sel, QCDSupport::stdMCWeight, TString::Format("signal_%s_%u", name.Data(), iT));
	//plot_allMet->addHist(hist, names[iT], "hist", colors[iT], 0, colors[iT]);
	TH1F* intHist = (TH1F*) hist->Clone();
	for(unsigned int iBin = 1; iBin <= hist->GetNbinsX(); ++iBin) intHist->SetBinContent(iBin, hist->Integral(0, iBin));
        if(yMinInt > intHist->GetMinimum()) yMinInt = intHist->GetMinimum();
        if(yMaxInt < intHist->GetMaximum()) yMaxInt = intHist->GetMaximum();
	yMaxTotal = yMaxInt;
	intHist->Scale(1 / hist->Integral(0, hist->GetNbinsX() + 1));
	plot_int->addHist(intHist, names[iT], "intHist", colors[iT], 0, colors[iT]);
      } else if(names[iT] == "Lep Electron"){
        TH1F* hist = met_HG->getHistogram(trees[iT], elecSel, QCDSupport::stdMCWeight, TString::Format("signal_%s_%u", name.Data(), iT));
	plot_allMet->addHist(hist, names[iT], "hist", colors[iT], 0, colors[iT]);
	TH1F* intHist = (TH1F*) hist->Clone();
	TH1F* Hist_elec = (TH1F*) hist->Clone();
	for(unsigned int iBin = 1; iBin <= hist->GetNbinsX(); ++iBin) intHist->SetBinContent(iBin, hist->Integral(0, iBin));
        yMaxInt = intHist->GetMaximum();
	intHist->Scale((yMaxInt / yMaxTotal) / hist->Integral(0, hist->GetNbinsX() + 1));
	yMaxIntArray[0] = intHist->GetMaximum();
	plot_int->addHist(intHist, names[iT], "intHist", colors[iT], 0, colors[iT]);
      } else if(names[iT] == "Lep Muon"){
        TH1F* hist = met_HG->getHistogram(trees[iT], muSel, QCDSupport::stdMCWeight, TString::Format("signal_%s_%u", name.Data(), iT));
	if(yMin > hist->GetMinimum()) yMin = hist->GetMinimum();
        if(yMax < hist->GetMaximum()) yMax = hist->GetMaximum();
	plot_allMet->addHist(hist, names[iT], "hist", colors[iT], 0, colors[iT]);
	TH1F* intHist = (TH1F*) hist->Clone();
	TH1F* Hist_muon = (TH1F*) hist->Clone();
	for(unsigned int iBin = 1; iBin <= hist->GetNbinsX(); ++iBin) intHist->SetBinContent(iBin, hist->Integral(0, iBin));
        yMaxInt = intHist->GetMaximum();
	intHist->Scale((yMaxInt / yMaxTotal) / hist->Integral(0, hist->GetNbinsX() + 1));
	yMaxIntArray[1] = intHist->GetMaximum();
	plot_int->addHist(intHist, names[iT], "intHist", colors[iT], 0, colors[iT]);
      } else if(names[iT].Contains("Lep Tau")){
	int i = 0;
	     if(names[iT].Contains("Elec")) {hist = met_HG->getHistogram(trees[iT], tauSelElec, QCDSupport::stdMCWeight, TString::Format("signal_%s_%u", name.Data(), iT)); i = 0;} 
        else if(names[iT].Contains("Muon")) {hist = met_HG->getHistogram(trees[iT], tauSelMuon, QCDSupport::stdMCWeight, TString::Format("signal_%s_%u", name.Data(), iT)); i = 1;}
        else if(names[iT].Contains("Hadr")) {hist = met_HG->getHistogram(trees[iT], tauSelHadr, QCDSupport::stdMCWeight, TString::Format("signal_%s_%u", name.Data(), iT)); i = 2;}
	plot_allMet->addToStack(hist, names[iT], colors[iT], 1001, 1, 1, 3);
	TH1F* intHist = (TH1F*) hist->Clone();
	for(unsigned int iBin = 1; iBin <= hist->GetNbinsX(); ++iBin) intHist->SetBinContent(iBin, hist->Integral(0, iBin));
        yMaxInt = intHist->GetMaximum();
	intHist->Scale((yMaxInt / yMaxTotal) / hist->Integral(0, hist->GetNbinsX() + 1));
	yMaxIntArray[2 + i] = intHist->GetMaximum();
	plot_int->addToStack(intHist, names[iT], colors[iT], 1001, 1, 1, 3);
      }
    }

    //Double_t ks = hist_elec->KolmogorovTest(hist_muon, "D");
    //cout << "Kolmogorov Test Muon/Electron: " << std::dec << ks << endl;
	
    TCanvas* c = new TCanvas;
    plot_allMet->setLogy();
    plot_allMet->setDrawCMSLumi();
    //plot_allMet->setUsePoisson();
    if(yMin <= 0) yMin = 0.1;
    plot_allMet->setYRange(0.1 * yMin, 10 * yMax);
    plot_allMet->setLogy();
    //plot_allMet->setYRange(0, 1.4 * yMax);
    plot_allMet->setTitle(" ");
    //plot_allMet->setLegend(.6,.58,.90,.88);
    c->cd();
    plot_allMet->drawRatios(c);
    plot_allMet->draw(c,false,QCDSupport::format);
    c->cd();
    c->Modified();
    TLatex *tl = new TLatex();
    tl->SetTextSize(0.03);
    //tl->DrawLatexNDC(0.2, 0.7, srinfo.srSelLabels[iSR]);
    QCDSupport::setTitleOffset(c);
    c->Update();
    c->SaveAs(outputDir + "/" + plot_allMet->getName() + TString(".") + QCDSupport::format);
    plot_int->setDrawCMSLumi();
    plot_int->setYRange(0, 1);
    plot_int->setTitle(" ");
    //plot_int->setLegend(.6,.58,.90,.88);
    c->cd();
    plot_int->draw(c,false,QCDSupport::format);
    c->cd();
    c->Modified();
    tl->SetTextSize(0.03);
    //tl->DrawLatexNDC(0.2, 0.7, srinfo.srSelLabels[iSR]);
    QCDSupport::setTitleOffset(c);
    c->Update();
    c->SaveAs(outputDir + "/" + plot_int->getName() + TString(".") + QCDSupport::format);
    cout << "Lep Electron Frac: " << std::dec << yMaxIntArray[0] << endl;
    cout << "Lep Muon Frac: "     << std::dec << yMaxIntArray[1] << endl;
    cout << "Lep Tau Elec Frac: " << std::dec << yMaxIntArray[2] << endl;
    cout << "Lep Tau Muon Frac: " << std::dec << yMaxIntArray[3] << endl;
    cout << "Lep Tau Hadr Frac: " << std::dec << yMaxIntArray[4] << endl;
  }
}

#endif

void doOneLepStuff(){
  cout << "doOneLepStuff..." << endl;
  StyleTools::SetTDRStyle();
  gStyle->SetTitleOffset(1.400,"Y");
  gStyle->SetTitleOffset(0.950,"X");
  gStyle->SetPadTopMargin(0.08);

  QCDSupport::CRegInfo crinfo;
  //crinfo.fillOtherBkgSFs(QCDSupport::inputDir + "/met_tree_skimmed_baseline.root",
  //                       QCDSupport::inputDir + "/ttbarplusw_tree_skimmed_baseline.root",
  //                       QCDSupport::inputDir + "/remaining_nonQCD_tree_skimmed_baseline.root",
  //                       QCDSupport::inputDir + "/qcd_smeared_tree_skimmed_baseline.root",
////                         QCDSupport::inputDir + "/qcd_smeared_CHEF_tree_skimmed_baseline.root",
  //                       QCDSupport::inputDir + "/qcd_orig_tree_skimmed_baseline.root");
  //crinfo.fillCorrectedOtherBkgYields();
  //crinfo.fillDataCorr();

  QCDSupport::SRegInfo srinfo;
  //srinfo.fillTFVariations(QCDSupport::inputDir + "/qcd_smeared_tree_skimmed_baseline.root",
////  srinfo.fillTFVariations(QCDSupport::inputDir + "/qcd_smeared_CHEF_tree_skimmed_baseline.root",
  //                        QCDSupport::inputDir + "/qcd_orig_tree_skimmed_baseline.root");
  //srinfo.loadExternUnc("../extUncs/");

  cout <<"DataCard info!"<<endl;

  vector<TString> names;
  vector<TTree*> trees;
  vector<int> colors;
  //trees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/met_tree.root"));                                     names.push_back("Data");              colors.push_back(1);
  trees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/ttbarplusw_tree.root"));                             names.push_back("Total");              colors.push_back(kAzure + 10);
  trees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/ttbarplusw_tree.root"));                             names.push_back("Lep Tau Elec");       colors.push_back(color_qcd);
  trees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/ttbarplusw_tree.root"));                             names.push_back("Lep Tau Muon");       colors.push_back(color_tW);
  trees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/ttbarplusw_tree.root"));                             names.push_back("Lep Tau Hadr");       colors.push_back(kBlue);
  trees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/ttbarplusw_tree.root"));                             names.push_back("Lep Electron");       colors.push_back(kMagenta);
  trees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/ttbarplusw_tree.root"));                             names.push_back("Lep Muon");           colors.push_back(kRed);
  //trees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/qcd_tree.root"));                names.push_back("Smeared QCD MC");    colors.push_back(color_qcd);
//  trees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/qcd_smeared_CHEF_tree_skimmed_baseline.root"));  names.push_back("Smeared QCD MC");    colors.push_back(color_qcd);
  //trees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/../plots_16_11_15_NoSmear/qcd_orig_tree.root")); names.push_back("With orig. QCD MC"); colors.push_back(color_tW);
  //trees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/T2tt_400_313_tree.root"));                       names.push_back("T2tt(400, 313)");    colors.push_back(kGreen + 2);
  //trees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/T2tt_600_300_tree.root"));                       names.push_back("T2tt(600, 300)");    colors.push_back(kRed);
  //trees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/T2tt_800_100_tree.root"));                       names.push_back("T2tt(800, 100)");    colors.push_back(kBlue);

  //GetQCDTable(crinfo, srinfo);
  MakeLepShapePlots(    srinfo, names, trees, colors);
}
