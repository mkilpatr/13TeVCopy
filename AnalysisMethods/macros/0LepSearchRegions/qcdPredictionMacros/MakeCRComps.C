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

  auto stdTree = QCDSupport::getTree("pieces/qcd_tree_skimmed_baseline.root");
  auto origTree = QCDSupport::getTree("pieces/qcd_origtree_skimmed_baseline.root");

  vector<TString> CRNames;
  CRNames.push_back("\\multicolumn{8}{|c|}{$\\mtb < 175$~\\GeV, $5-6$ jets} \\\\");
  CRNames.push_back("\\multicolumn{8}{|c|}{$\\mtb < 175$~\\GeV, $\\geq 7$ jets} \\\\");
  CRNames.push_back("\\multicolumn{8}{|c|}{$\\mtb \\geq 175$~\\GeV, $\\nt = 0$, $5-6$ jets} \\\\");
  CRNames.push_back("\\multicolumn{8}{|c|}{$\\mtb \\geq 175$~\\GeV, $\\nt = 0$, $\\geq 7$ jets} \\\\");
  CRNames.push_back("\\multicolumn{8}{|c|}{$\\mtb \\geq 175$~\\GeV, $\\nt \\geq 1$, $\\geq 5$ jets} \\\\");

  cout << "\\begin{table}[!!htbp]"<<endl;
  cout << "\\begin{center}"<<endl;
  cout <<"\\resizebox*{1\\textwidth}{!}{"<<endl;
  cout <<"\\begin{tabular}{|c|c||c|c||c|c||c|c|}"<<endl;
  cout <<"\\hline"<<endl;
  cout <<"$N_{\\text{data}}$ & $N^{\\text{non-QCD}}_{\\text{MC}}$ & \\multicolumn{2}{|c||}{$N^{\\text{QCD}}_{\\text{Smeared}}$} & \\multicolumn{2}{|c||}{$N^{\\text{QCD}}_{\\text{Without}\\ r_{\\text{jet}}\\ \\text{corr}} $} & \\multicolumn{2}{|c|}{$N^{\\text{QCD}}_{\\text{Orig. QCD MC}} $} \\\\"<<endl;
  cout <<"\\hline"<<endl;

  for(unsigned int iP = 0; iP < info.nCR; ++iP){
    TString qcdWVSEL= TString::Format("%s && %s",info.crPreQCDWithVeto.Data(),info.crSel[iP].Data());
    QCDSupport::metGetter->setNBS(50);
    TH1F * hNR = QCDSupport::metGetter->getHistogramManual(stdTree,qcdWVSEL,processWeight(QCDSupport::stdMCWeight,iP),TString::Format("tree_%u",0));
    QCDSupport::metGetter->setNBS(0);
    TH1F * hNO = QCDSupport::metGetter->getHistogram(origTree,qcdWVSEL,processWeight(QCDSupport::stdMCWeight,iP),TString::Format("tree_%u",1));


    auto reg = QCDSupport::CRegInfo::CRReg(iP);
    double data = info.getDataYield(reg,0);
    double other = info.getScaledOtherYield(reg,0);
    double qcdS = info.qcdYieldsWithVeto[reg]->GetBinContent(1);
    double qcdSE = info.qcdYieldsWithVeto[reg]->GetBinError(1);
    double qcdR = hNR->GetBinContent(1);
    double qcdRE = hNR->GetBinError(1);
    double qcdO = hNO->GetBinContent(1);
    double qcdOE = hNO->GetBinError(1);

    auto getDiff = [&](double QCD, double QCDE) -> double {
      double diff = data - QCD - other;
      double error = TMath::Sqrt(QCD +  QCDE*QCDE);
      return diff/error;
    };

    double qcdSTE = getDiff(qcdS,qcdSE);
    double qcdRTE = getDiff(qcdR,qcdRE);
    double qcdOTE = getDiff(qcdO,qcdOE);

  cout <<CRNames[iP] << endl;
  cout <<"\\hline"<<endl;
  cout << TString::Format("%.0f & %.1f & $%.1f \\pm %.1f$ (%.0f\\%%) & %.1f & $%.1f \\pm %.1f$ (%.0f\\%%) & %.1f & $%.1f \\pm %.1f$ (%.0f\\%%) & %.1f \\\\  ",data,other,qcdS,qcdSE,qcdSE*100/qcdS , qcdSTE,qcdR,qcdRE,qcdRE*100/qcdR,qcdRTE,qcdO,qcdOE,qcdOE*100/qcdO,qcdOTE);
  cout <<"\\hline"<<endl;

  }
  cout <<"\\end{tabular}"<<endl;
  cout <<"}"<<endl;
  cout <<"\\end{center}"<<endl;
  cout <<"\\end{table}"<<endl;

}
#endif

void MakeCRComps(){
  go();
}
