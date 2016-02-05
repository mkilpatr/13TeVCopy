#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/Plot.hh"
#include "AnalysisMethods/PlotUtils/interface/PlotTools.hh"
#include "AnalysisMethods/PlotUtils/interface/SFGetter.hh"
#include "AnalysisMethods/PlotUtils/interface/HistogramGetter.hh"
#include "AnalysisTools/QuickRefold/interface/TObjectContainer.h"

#include "QCDPredictionHelper.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TMultiGraph.h"


using namespace std;




void setTitleOffset(TCanvas *c, double xOff = .950, double yOff = 1.400){
  TList * list = c->GetListOfPrimitives();
  for(unsigned int iP = 0; iP < list->GetSize(); ++iP){
    TH1 * h = dynamic_cast<TH1*>(list->At(iP));
    if(h == 0) continue;
    h->GetXaxis()->SetTitleOffset(xOff);
    h->GetYaxis()->SetTitleOffset(yOff);
  }
}

enum TreeType {Data,QCD, NoSmearQCD, TTbarW, Znuu, ttZ};
const TString TreeNames[] = {"Data", "QCD","NoSmearQCD","t#bar{t}W","Z#rightarrow#nu#nu","t#bar{t}Z"};
const StyleTools::Colors TreeColors[] = {color_data, color_qcd,color_qcd,color_ttbar,color_znunu,color_ttZ};
const TString weights[] = {QCDSupport::stdWeight,QCDSupport::stdMCWeight,QCDSupport::stdMCWeight,QCDSupport::stdMCWeight,QCDSupport::stdMCWeight,QCDSupport::stdMCWeight};

void getSTDTreesAndNames(vector<pair<TTree*,TreeType> >& trees){
  trees.emplace_back(getTree("pieces/htmht_tree_skimmed.root"),Data);
  trees.emplace_back(getTree("pieces/ttZ_tZq_skimmed.root"),ttZ);
  trees.emplace_back(getTree("pieces/znunu_tree_skimmed.root"),Znuu);
  trees.emplace_back(getTree("pieces/ttbarplusw_tree_skimmed.root"),TTbarW);
  trees.emplace_back(getTree("pieces/qcd_Tree_skimmed.root")     ,QCD);
//  trees.emplace_back(getTree("qcd_origTree.root") ,NoSmearQCD);
}

const TString header = "#sqrt{s} = 13 TeV, L = 2.3 fb^{-1}";



void checkCRContamination(){
//  TString presel = TString::Format("%s && nvetolep == 0", QCDSupport::METPresel.Data());
  TString presel = TString::Format("%s && nvetotau == 0 && nvetolep == 0", QCDSupport::METPresel.Data());

//  TString sels[] = {"(dphij12met < .1)","(dphij12met < .15)","(dphij12met < .1 || dphij3met < .1)","(dphij12met < .15 || dphij3met < .15)",""};
  TString sels[] = {
      "(dphij12met < .15)","(dphij12met < .1 || dphij3met < .1)",""};
  TString sels2[] = {"mtcsv12met < 175 && (njets == 5 || njets == 6)","mtcsv12met < 175 && njets >= 7",
                     "mtcsv12met >= 175 && (njets == 5 || njets == 6) && ncttstd == 0", "mtcsv12met >= 175 && njets >= 7 && ncttstd == 0", "mtcsv12met >= 175 && ncttstd >= 1",""};


  vector<HistogramGetter*> histGs;
  double bins[] = {250,300,400,500,600,700};
  histGs.push_back(new HistogramGetter("met","met","#slash{#it{E}}_{T}", 5,bins));

  vector<TString> names;
  vector<TTree*> trees;
//  trees.emplace_back(getTree("pieces/htmht_tree_skimmed.root")); names.push_back("Data");
  trees.emplace_back(getTree("pieces/nonQCD_tree_skimmed.root")); names.push_back("Non-QCD MC");
  trees.emplace_back(getTree("pieces/qcd_tree_skimmed.root")); names.push_back("QCD MC");



  for(unsigned int iH = 0; iH < histGs.size(); ++iH)
    for(unsigned int iP = 0; sels[iP][0]; ++iP)
    for(unsigned int iP2 = 0; sels2[iP2][0]; ++iP2){
    TString title = TString::Format("%s, %s",sels[iP].Data(),sels2[iP2].Data());
    TString name = TString::Format("checkCRContamination_%u_%u",iP,iP2);
    Plot * plot = new Plot(name,title,histGs[iH]->plotInfo->xTitle,"Events");
    TString sel = TString::Format("%s && %s && %s",presel.Data(),sels[iP].Data(),sels2[iP2].Data());
    for(unsigned int iT = 0; iT < trees.size(); ++iT){
      TH1F * hSN = histGs[iH]->getHistogram(trees[iT],sel,QCDSupport::stdMCWeight,TString::Format("tree_%u",iT));
      plot->addHist(hSN,names[iT],"",StyleTools::colorGetter(iT),0,StyleTools::colorGetter(iT));
    }
    TCanvas * c = new TCanvas;
    plot->drawRatios(c,1,true,"png");

    cout << title << endl;
  }
}

#endif


void GetQCDPrediction()
{

  StyleTools::SetStyle();
  gStyle->SetTitleOffset(1.400,"Y");
  gStyle->SetTitleOffset(0.950,"X");

//  gStyle->SetTitleOffset(1.00,"Y");
//  gStyle->SetTitleOffset(0.950,"X");
//  gStyle->SetPadBottomMargin(0.5);
//  gStyle->SetPadTopMargin   (0.08);
//  gStyle->SetPadLeftMargin  (0.15);
//  gStyle->SetPadRightMargin (0.05);
//  getMotivationPlots();
//  getSmearComparision();
//  getTailSFRegion();
//  checkCRRegion();
//  checkCRContamination();
//  checkCRRegion();
//  checkTF();
//  prepareTrees();
//  doFullExtraction();
//  getRegion();
//  checkContamination();
//  getSFs();
//  test();
}
