#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/HistogramGetter.hh"
#include "QCDPredictionHelper.h"
#include "TDirectory.h"
#include "TFile.h"

using namespace std;
TString processWeight(TString weight, int iCR) {return iCR == 4 ? TString::Format("%s*%s",weight.Data(),QCDSupport::topMCWeight.Data()) : weight;}

void go(){

  QCDSupport::CRegInfo info;
  info.fillOtherBkgSFs("pieces/htmht_tree_skimmed_baseline.root","pieces/nonQCD_tree_skimmed_baseline.root","pieces/qcd_tree_skimmed_baseline.root");
  info.fillCorrectedOtherBkgYields();
  info.fillDataCorr();
  cout << "DONE!"<<endl;


  vector<TString> names;
  vector<TTree*> trees;
  vector<int> colors;
  trees.emplace_back(QCDSupport::getTree("pieces/htmht_tree_skimmed_baseline.root")); names.push_back("Data"); colors.push_back(1);
  trees.emplace_back(QCDSupport::getTree("pieces/nonQCD_tree_skimmed_baseline.root")); names.push_back("Non-QCD bkg");colors.push_back(color_ttbar);
  trees.emplace_back(QCDSupport::getTree("pieces/qcd_tree_skimmed_baseline.root")); names.push_back("Smeared QCD MC"); colors.push_back(color_qcd);
//  trees.emplace_back(QCDSupport::getTree("pieces/qcd_tree_skimmed_baseline.root")); names.push_back("Without #it{r}_{jet} corr");colors.push_back(color_znunu);
  trees.emplace_back(QCDSupport::getTree("pieces/qcd_origtree_skimmed_baseline.root")); names.push_back("With orig. QCD MC");colors.push_back(color_tW);
//  trees.emplace_back(QCDSupport::getTree("pieces/T2tt_500_200_tree.root")); names.push_back("T2tt_500_200"); colors.push_back(color_znunu);
//  trees.emplace_back(QCDSupport::getTree("pieces/T2tt_700_1_tree.root")); names.push_back("T2tt_700_1"); colors.push_back(color_tW);

  std::vector<double> minY(info.nCR,0.1);
  std::vector<double> maxY(info.nCR,3000);

  minY[0] = 0.3;maxY[0] = 100000;
  minY[1] = 0.3;maxY[1] = 50000;
  minY[2] = 0.3;maxY[2] = 4000;
  minY[3] = 0.3;maxY[3] = 2000;
  minY[4] = 0.2;maxY[4] = 500;

    for(unsigned int iP = 0; iP < info.nCR; ++iP){
    TString title = TString::Format("%s",info.crSelNames[iP].Data());
    TString name = TString::Format("getRegion_0_%u",iP);
    Plot * plot = new Plot(name,title,QCDSupport::metGetter->plotInfo->xTitle,"Events");
    TString qcdWVSEL= TString::Format("%s && %s",info.crPreQCDWithVeto.Data(),info.crSel[iP].Data());
    TString t2ttSEL= TString::Format("%s && %s",info.crPreData.Data(),info.crSel[iP].Data());
    for(unsigned int iT = 0; iT < trees.size(); ++iT){
      if(names[iT] == "Data"){
        TH1F * hSN = (TH1F*)info.dataYields[iP]->Clone();
        plot->addHist(hSN,names[iT],"E0",colors[iT],0,colors[iT]);

      }
      else if(names[iT] == "Without #it{r}_{jet} corr"){
        TH1F * hSN = QCDSupport::metGetter->getHistogram(trees[iT],qcdWVSEL,processWeight(QCDSupport::stdMCWeight,iP),TString::Format("tree_%u",iT));
        hSN->Add(info.otherBKGScaledYields[iP]);
        plot->addHistForRatio(hSN,names[iT],"hist",colors[iT],0,colors[iT]);
      }
      else if(names[iT] == "With orig. QCD MC"){
        TH1F * hSN = QCDSupport::metGetter->getHistogram(trees[iT],qcdWVSEL,processWeight(QCDSupport::stdMCWeight,iP),TString::Format("tree_%u",iT));
        hSN->Add(info.otherBKGScaledYields[iP]);
        plot->addHistForRatio(hSN,names[iT],"hist",colors[iT],0,colors[iT]);
      }
      else if (names[iT] == "Non-QCD bkg"){
        TH1F * hSN = (TH1F*)info.otherBKGScaledYields[iP]->Clone();
        plot->addToStack(hSN,names[iT],colors[iT],1001,1,1,3);
      }
      else if (names[iT].BeginsWith("T2tt")){
         TH1F * hSN = QCDSupport::metGetter->getHistogram(trees[iT],processWeight(QCDSupport::stdMCWeight,iP),TString::Format("tree_%u",iT));
         plot->addHist(hSN,names[iT],"",colors[iT],0,colors[iT]);
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
    plot->setYRange(minY[iP],maxY[iP]);
    plot->setTitle(" ");
    plot->setLegend(.6,.58,.90,.88);
//    plot->getLegend()->SetNColumns(2);

    plot->setLogy();
    c->cd();
    plot->drawRatioStack(c,true,"pdf");
    c->cd();
    c->Modified();
    TLatex *tl = new TLatex();
    tl->SetTextSize(0.03);
    tl->DrawLatexNDC(0.2, 0.77, "HPTT QCD CS");
    tl->DrawLatexNDC(0.2, 0.69, info.crSelLabels[iP]);



    QCDSupport::setTitleOffset(c);
    c->Update();

    c->SaveAs(plot->getName() + TString(".pdf"));
    c->SaveAs(TString::Format("QCD_CS_%u.pdf",iP));



  }
}
#endif

void MakeCRPlots(){
  StyleTools::SetTDRStyle();
  gStyle->SetTitleOffset(1.400,"Y");
  gStyle->SetTitleOffset(0.950,"X");
  gStyle->SetPadTopMargin   (0.08);
  go();
}
