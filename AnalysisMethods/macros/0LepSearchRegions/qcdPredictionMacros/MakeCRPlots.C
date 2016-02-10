#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/HistogramGetter.hh"
#include "QCDPredictionHelper.h"
#include "TDirectory.h"
#include "TFile.h"
//#include  "/Users/nmccoll/Dropbox/Work/scripts/CMS_lumi_inc.h"

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

  trees.emplace_back(QCDSupport::getTree("pieces/qcd_tree_skimmed_baseline.root")); names.push_back("MC w/o #it{r}_{jet} corr");colors.push_back(color_znunu);
  trees.emplace_back(QCDSupport::getTree("pieces/qcd_origtree_skimmed_baseline.root")); names.push_back("MC w/o smearing or #it{r}_{jet} corr");colors.push_back(color_tW);
  trees.emplace_back(QCDSupport::getTree("pieces/htmht_tree_skimmed_baseline.root")); names.push_back("Data"); colors.push_back(1);
  trees.emplace_back(QCDSupport::getTree("pieces/nonQCD_tree_skimmed_baseline.root")); names.push_back("Non-QCD bkg");colors.push_back(color_ttbar);
  trees.emplace_back(QCDSupport::getTree("pieces/qcd_tree_skimmed_baseline.root")); names.push_back("QCD MC"); colors.push_back(color_qcd);

//  trees.emplace_back(QCDSupport::getTree("pieces/T2tt_500_200_tree.root")); names.push_back("T2tt_500_200"); colors.push_back(color_znunu);
//  trees.emplace_back(QCDSupport::getTree("pieces/T2tt_700_1_tree.root")); names.push_back("T2tt_700_1"); colors.push_back(color_tW);

    for(unsigned int iP = 0; iP < info.nCR; ++iP){
    TString title = TString::Format("%s",info.crSelNames[iP].Data());
    TString name = TString::Format("getRegion_0_%u",iP);
    Plot * plot = new Plot(name,title,QCDSupport::metGetter->plotInfo->xTitle,"Events");
    TString qcdWVSEL= TString::Format("%s && %s",info.crPreQCDWithVeto.Data(),info.crSel[iP].Data());
    TString t2ttSEL= TString::Format("%s && %s",info.crPreData.Data(),info.crSel[iP].Data());
    for(unsigned int iT = 0; iT < trees.size(); ++iT){
      if(names[iT] == "Data"){
        TH1F * hSN = (TH1F*)info.dataYields[iP]->Clone();//    dsinfo.metGetter->getHistogram(trees[iT],sel,"1.0",TString::Format("tree_%u",iT));
        plot->addHist(hSN,names[iT],"",colors[iT],0,colors[iT]);
      }
      else if(names[iT] == "MC w/o #it{r}_{jet} corr"){
        TH1F * hSN = QCDSupport::metGetter->getHistogram(trees[iT],qcdWVSEL,processWeight(QCDSupport::stdMCWeight,iP),TString::Format("tree_%u",iT));
        hSN->Add(info.otherBKGScaledYields[iP]);
        plot->addHist(hSN,names[iT],"hist",colors[iT],0,colors[iT]);
      }
      else if(names[iT] == "MC w/o smearing or #it{r}_{jet} corr"){
        TH1F * hSN = QCDSupport::metGetter->getHistogram(trees[iT],qcdWVSEL,processWeight(QCDSupport::stdMCWeight,iP),TString::Format("tree_%u",iT));
        hSN->Add(info.otherBKGScaledYields[iP]);
        plot->addHist(hSN,names[iT],"",colors[iT],0,colors[iT]);
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
        TH1F * hSN = (TH1F*)info.qcdYieldsWithVeto[iP]->Clone();//   info.metGetter->getHistogram(trees[iT],sel,TString::Format("%s*qcdRespTailWeight",QCDSupport::stdMCWeight.Data()),TString::Format("tree_%u",iT));
        plot->addToStack(hSN,names[iT],colors[iT],1001,1,1,3);
      }
    }




//    TCanvas * c = new TCanvas;
//    plot->setHeader(QCDSupport::header,"");
//    plot->setYRange(0.1,400);
//    plot->setLogy();
//
//
//
//    plot->draw(c,true,"png");
//    QCDSupport::setTitleOffset(c);
//
//
//    c->SaveAs(plot->getName() + TString(".png"));

    //For pas
    TCanvas * c = new TCanvas;
    plot->setDrawCMSLumi();
//    plot->setHeader(QCDSupport::header,"");
    plot->setTitle(" ");
    plot->setLegend(.4,.7,.95,.9);
    plot->getLegend()->SetNColumns(2);
    plot->setYRange(0.1,3000);
    plot->setLogy();

    plot->draw(c,true,"pdf");
    QCDSupport::setTitleOffset(c);

    c->SaveAs(plot->getName() + TString(".pdf"));



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
