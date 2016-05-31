#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/HistogramGetter.hh"
#include "QCDPredictionHelper.h"
#include "TDirectory.h"
#include "TFile.h"

using namespace std;

void go() {
  QCDSupport::CRegInfo crinfo;
  crinfo.fillOtherBkgSFs(QCDSupport::inputDir + "/met_tree_skimmed_baseline.root",QCDSupport::inputDir + "/nonQCD_tree_skimmed_baseline.root",QCDSupport::inputDir + "/qcd_tree_skimmed_baseline.root", QCDSupport::inputDir + "/qcd_orig_tree_skimmed_baseline.root");
  crinfo.fillCorrectedOtherBkgYields();
  crinfo.fillDataCorr();

  QCDSupport::SRegInfo srinfo;
  srinfo.fillTFVariations(QCDSupport::inputDir + "/qcd_tree_skimmed_baseline.root",QCDSupport::inputDir + "/qcd_orig_tree_skimmed.root");

  for(unsigned int iS = 0; iS < srinfo.nSR; ++iS){
    Plot * plot = new Plot(TString::Format("TF_%u",iS),srinfo.srSelNames[iS],"","SR/CR");

    plot->addHist(srinfo.origQCD_TF[iS],"w/o smearing or #it{r}_{jet} corr","",color_tW,0,color_tW);
    plot->addHist(srinfo.noResp[iS],"w/o #it{r}_{jet} corr","hist",color_znunu,0,color_znunu);
//    plot->addHist(srinfo.respUp[iS],"#it{r}_{jet} up variation","hist",color_qcd,0,color_qcd);
//    plot->addHist(srinfo.respDown[iS],"#it{r}_{jet} down variation","hist",5,0,5);
    plot->addHist(srinfo.nominalQCD[iS],"Nominal","",1,0,1);
    float yMin = srinfo.nominalQCD[iS]->GetMinimum();
    float yMax = srinfo.nominalQCD[iS]->GetMaximum();

    plot->setLegend(.35,.70,.92,.9);
    plot->setXTitle(srinfo.SRmetGetters[iS]->plotInfo->xTitle);

    plot->setYRange(0.1 * yMin, 10 * yMax);
    plot->setLogy();
    plot->getLegend()->SetNColumns(2);
    plot->setHeader("","");
    TCanvas * c = new TCanvas;
    plot->draw(c,QCDSupport::format);
    QCDSupport::setTitleOffset(c);
    c->SaveAs(plot->getName() + TString(".pdf"));
  }
}
#endif

void MakeTFPlots(){
  StyleTools::SetStyle();
  gStyle->SetTitleOffset(1.400,"Y");
  gStyle->SetTitleOffset(0.950,"X");
  go();
}
