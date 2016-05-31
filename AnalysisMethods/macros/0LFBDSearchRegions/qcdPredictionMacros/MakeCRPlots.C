#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/HistogramGetter.hh"
#include "QCDPredictionHelper.h"
#include "TDirectory.h"
#include "TFile.h"

using namespace std;
TString processWeight(TString weight, int iCR) {return weight;}

void go(){
  QCDSupport::CRegInfo info;
  info.fillOtherBkgSFs(QCDSupport::inputDir + "/met_tree_skimmed_baseline.root",QCDSupport::inputDir + "/nonQCD_tree_skimmed_baseline.root",QCDSupport::inputDir + "/qcd_tree_skimmed_baseline.root", QCDSupport::inputDir + "/qcd_orig_tree_skimmed_baseline.root");
  info.fillCorrectedOtherBkgYields();
  info.fillDataCorr();
  cout << "DONE!"<<endl;

  vector<TString> names;
  vector<TTree*> trees;
  vector<int> colors;
  trees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/met_tree_skimmed_baseline.root")); names.push_back("Data"); colors.push_back(1);
  trees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/nonQCD_tree_skimmed_baseline.root")); names.push_back("Non-QCD bkg");colors.push_back(color_ttbar);
  trees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/qcd_tree_skimmed_baseline.root")); names.push_back("Smeared QCD MC"); colors.push_back(color_qcd);
//  trees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/qcd_tree_skimmed_baseline.root")); names.push_back("Without #it{r}_{jet} corr");colors.push_back(color_znunu);
  trees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/qcd_orig_tree_skimmed_baseline.root")); names.push_back("With orig. QCD MC");colors.push_back(color_tW);
//  trees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/qcd_orig_combined_tree_skimmed.root")); names.push_back("With orig. QCD MC");colors.push_back(color_tW);
  trees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/T2tt_375_295_tree_skimmed_baseline.root")); names.push_back("#Delta M = 20 GeV");  colors.push_back(kBlack);
  trees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/T2tt_375_325_tree_skimmed_baseline.root")); names.push_back("#Delta M = 50 GeV");  colors.push_back(kRed);
  trees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/T2tt_375_355_tree_skimmed_baseline.root")); names.push_back("#Delta M = 80 GeV");  colors.push_back(kBlue);

  for(unsigned int iP = 0; iP < info.nCR; ++iP){
    TString title = TString::Format("%s",info.crSelNames[iP].Data());
    TString name = TString::Format("getRegion_0_%u",iP);
    Plot * plot = new Plot(name,title,info.CRmetGetters[iP]->plotInfo->xTitle,"Events");
    TString qcdWVSEL = TString::Format("%s && %s",info.crPreQCDWithVeto.Data(),info.crSel[iP].Data());
    TString t2ttSEL  = TString::Format("%s && %s",info.crPreData.Data(),info.crSel[iP].Data());
    float yMax = 0.;
    float totMCYield = 0;
    for(unsigned int iT = 0; iT < trees.size(); ++iT){
      if(names[iT] == "Data"){
        TH1F * hSN = (TH1F*)info.dataYields[iP]->Clone();
        plot->addHist(hSN,names[iT],"E0",colors[iT],0,colors[iT]);
        if(yMax < hSN->GetMaximum()) yMax = hSN->GetMaximum();
      }
//      else if(names[iT] == "Without #it{r}_{jet} corr"){
//        TH1F * hSN = info.CRmetGetters[iP]->getHistogram(trees[iT],qcdWVSEL,processWeight(QCDSupport::stdMCWeight,iP),TString::Format("tree_%u",iT));
//        hSN->Add(info.otherBKGScaledYields[iP]);
//        plot->addHistForRatio(hSN,names[iT],"hist",colors[iT],0,colors[iT]);
//      }
      else if(names[iT] == "With orig. QCD MC"){
        TH1F * hSN = info.CRmetGetters[iP]->getHistogram(trees[iT],qcdWVSEL,processWeight(QCDSupport::stdMCWeight,iP),TString::Format("tree_%s_%u",info.crRegBinNames[iP].Data(), iT));
        hSN->Add(info.otherBKGScaledYields[iP]);
        plot->addHistForRatio(hSN,names[iT],"hist",colors[iT],0,colors[iT]);
      }
      else if (names[iT] == "Non-QCD bkg"){
        TH1F * hSN = (TH1F*)info.otherBKGScaledYields[iP]->Clone();
        plot->addToStack(hSN,names[iT],colors[iT],1001,1,1,3);
      }
      else if (names[iT].Contains("Delta")){
        TH1F* hSN = info.CRmetGetters[iP]->getHistogram(trees[iT], t2ttSEL, QCDSupport::stdMCWeight, TString::Format("tree_%s_%u", info.crRegBinNames[iP].Data(), iT));
        hSN->Scale(totMCYield / hSN->Integral(0, hSN->GetNbinsX() + 1));
        plot->addHist(hSN, names[iT], "hist", colors[iT], 0, colors[iT]);
      }
      else {
        TH1F * hSN = (TH1F*)info.qcdYieldsWithVeto[iP]->Clone();
        plot->addToStack(hSN,names[iT],colors[iT],1001,1,1,3);
      }
    }
    TH1F * hSN  = (TH1F*)(info.qcdYieldsWithVeto[iP])->Clone();
    hSN->Add(info.otherBKGScaledYields[iP]);
    for(unsigned int iB = 1; iB <= hSN->GetNbinsX(); ++iB){
      double subSyst = (info.otherBKGCorrUnc[iP]->GetBinContent(iB) - 1)*hSN->GetBinContent(iB);
      double stat = info.qcdYieldsWithVeto[iP]->GetBinError(iB);
      hSN->SetBinError(iB,TMath::Sqrt(subSyst*subSyst +stat*stat ));
    }

    plot->setUncertaintyHist((TH1F*)(hSN));
    plot->setPlotRatioUncertaintyBand();

    TCanvas * c = new TCanvas;
    plot->setDrawCMSLumi();
    plot->setUsePoisson();
    plot->setYRange(0.1, 1000. * yMax);
    plot->setTitle(" ");
    plot->setLegend(.6,.58,.90,.88);
//    plot->getLegend()->SetNColumns(2);

    plot->setLogy();
    c->cd();
    plot->drawRatioStack(c,false,QCDSupport::format);
    c->cd();
    c->Modified();
    TLatex *tl = new TLatex();
    tl->SetTextSize(0.03);
    tl->DrawLatexNDC(0.2, 0.7, info.crSelLabels[iP]);

    QCDSupport::setTitleOffset(c);
    c->Update();

    c->SaveAs(TString::Format("QCD_CR_%u.",iP) + QCDSupport::format);
  }
}
#endif

void MakeCRPlots(){
  StyleTools::SetTDRStyle();
  gStyle->SetTitleOffset(1.400,"Y");
  gStyle->SetTitleOffset(0.950,"X");
  gStyle->SetPadTopMargin(0.08);
  go();
}
