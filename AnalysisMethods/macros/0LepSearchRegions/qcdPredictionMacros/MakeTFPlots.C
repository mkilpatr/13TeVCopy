#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/HistogramGetter.hh"
#include "QCDPredictionHelper.h"
#include "TDirectory.h"
#include "TFile.h"

using namespace std;

void go() {
  QCDSupport::CRegInfo crinfo;
  crinfo.fillOtherBkgSFs("pieces/htmht_tree_skimmed_baseline.root","pieces/nonQCD_tree_skimmed_baseline.root","pieces/qcd_tree_skimmed_baseline.root");
  crinfo.fillCorrectedOtherBkgYields();
  crinfo.fillDataCorr();

  QCDSupport::SRegInfo srinfo;
  srinfo.fillTFVariations("pieces/qcd_tree_skimmed_baseline.root","pieces/qcd_origtree_skimmed_baseline.root");

  for(unsigned int iS = 0; iS < srinfo.nSR; ++iS){
    Plot * plot = new Plot(TString::Format("TF_%u",iS),srinfo.srSelNames[iS],"","SR/CR");

    plot->addHist(srinfo.origQCD[iS],"w/o smearing or #it{r}_{jet} corr","",color_tW,0,color_tW);
    plot->addHist(srinfo.noResp[iS],"w/o #it{r}_{jet} corr","hist",color_znunu,0,color_znunu);
    plot->addHist(srinfo.respUp[iS],"#it{r}_{jet} up variation","hist",color_qcd,0,color_qcd);
    plot->addHist(srinfo.respDown[iS],"#it{r}_{jet} down variation","hist",5,0,5);
    plot->addHist(srinfo.nominalQCD[iS],"Nominal","",1,0,1);

    plot->setLegend(.35,.70,.92,.9);
    plot->setXTitle(QCDSupport::metGetter->plotInfo->xTitle);

    switch(iS){
    case 0: plot->setYRange(0.0001,3);break;
    case 1: plot->setYRange(0.0001,3);break;
    case 2: plot->setYRange(0.00005,.5);break;
    case 3: plot->setYRange(0.001,.1);break;
    case 4: plot->setYRange(0.0001,10);break;
    case 5: plot->setYRange(0.001,5);break;
    case 6: plot->setYRange(0.001,.5);break;
    case 7: plot->setYRange(0.001,.1);break;
    case 8: plot->setYRange(0.00005,300); break;
    case 9: plot->setYRange(0.0001,0.1);break;
    }
    plot->setLogy();
    plot->getLegend()->SetNColumns(2);
    plot->setHeader("","");
    TCanvas * c = new TCanvas;
    plot->draw(c,true,"png");
    QCDSupport::setTitleOffset(c);
    c->SaveAs(plot->getName() + TString(".png"));

  }
}
#endif

void MakeTFPlots(){
  StyleTools::SetStyle();
  gStyle->SetTitleOffset(1.400,"Y");
  gStyle->SetTitleOffset(0.950,"X");
  go();
}
