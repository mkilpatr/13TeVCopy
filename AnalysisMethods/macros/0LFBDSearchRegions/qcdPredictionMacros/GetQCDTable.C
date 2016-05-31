#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/HistogramGetter.hh"
#include "QCDPredictionHelper.h"
#include "TDirectory.h"
#include "TFile.h"

using namespace std;

void printTable() {
  QCDSupport::CRegInfo crinfo;
  crinfo.fillOtherBkgSFs(QCDSupport::inputDir + "/met_tree_skimmed_baseline.root",QCDSupport::inputDir + "/nonQCD_tree_skimmed_baseline.root",QCDSupport::inputDir + "/qcd_tree_skimmed_baseline.root", QCDSupport::inputDir + "/qcd_orig_tree_skimmed_baseline.root");
  crinfo.fillCorrectedOtherBkgYields();
  crinfo.fillDataCorr();

  QCDSupport::SRegInfo srinfo;
  srinfo.fillTFVariations(QCDSupport::inputDir + "/qcd_tree_skimmed_baseline.root",QCDSupport::inputDir + "/qcd_orig_tree_skimmed_baseline.root");
  srinfo.loadExternUnc("../extUncs/");

  cout <<"DataCard info!"<<endl;

  for(unsigned int iS=0;iS<srinfo.nSR;++iS) for(unsigned int iM=0;iM<srinfo.nMETBins[iS];++iM) cout <<"bin_"<< srinfo.getBinName(QCDSupport::SRegInfo::SRReg(iS),iM) <<"\t" << "TF"<< "\tqcd\t" << srinfo.getTF(QCDSupport::SRegInfo::SRReg(iS),iM)  <<endl;
  for(unsigned int iS=0;iS<srinfo.nSR;++iS) for(unsigned int iM=0;iM<srinfo.nMETBins[iS];++iM) cout <<"bin_"<< srinfo.getBinName(QCDSupport::SRegInfo::SRReg(iS),iM) <<"\t" << "Other MC yields"<< "\tqcd\t" << crinfo.getScaledOtherYield(srinfo.crRegs[iS],iM)  <<endl;
  for(unsigned int iS=0;iS<srinfo.nSR;++iS) for(unsigned int iM=0;iM<srinfo.nMETBins[iS];++iM) cout <<"bin_"<< srinfo.getBinName(QCDSupport::SRegInfo::SRReg(iS),iM) <<"\t" << "Other MC yields corr"<< "\tqcd\t" << crinfo.getDataYieldCorr(srinfo.crRegs[iS],iM)  <<endl;
  for(unsigned int iS=0;iS<srinfo.nSR;++iS) for(unsigned int iM=0;iM<srinfo.nMETBins[iS];++iM) cout <<"bin_"<< srinfo.getBinName(QCDSupport::SRegInfo::SRReg(iS),iM) <<"\t" << "External Unc"<< "\tqcd\t" << srinfo.getExtUncs(QCDSupport::SRegInfo::SRReg(iS),iM)  <<endl;
  for(unsigned int iS=0;iS<srinfo.nSR;++iS) for(unsigned int iM=0;iM<srinfo.nMETBins[iS];++iM) cout <<"bin_"<< srinfo.getBinName(QCDSupport::SRegInfo::SRReg(iS),iM) <<"\t" << "Total syst"<< "\tqcd\t" << srinfo.getTotalSystUnc(&crinfo,QCDSupport::SRegInfo::SRReg(iS),iM)  <<endl;
  for(unsigned int iS=0;iS<srinfo.nSR;++iS) for(unsigned int iM=0;iM<srinfo.nMETBins[iS];++iM) cout <<"bin_"<< srinfo.getBinName(QCDSupport::SRegInfo::SRReg(iS),iM) <<"\t" << "Data stat"<< "\tqcd\t" <<  crinfo.getDataStatUnc(srinfo.crRegs[iS],iM)  <<endl;
  cout << endl <<endl;

  auto printBigBlock = [&](std::ostream& os, QCDSupport::SRegInfo::SRReg reg) {
    os <<"\\hline"<<endl;
    os <<"\\hline"<<endl;
    for(unsigned int iM = 0; iM < srinfo.nMETBins[reg]; ++iM){
      os << crinfo.getLatexBlock(srinfo.crRegs[reg],iM) << " & "<< srinfo.getLatexBlock(&crinfo,reg,iM) <<"\\\\"<<endl;
    }
    os <<"\\hline"<<endl;
  };

//Now output our new uncertainty files
  std::ofstream uncFile;
  uncFile.open ("values_0l_unc_qcd.conf", ios::out | ios::trunc);
  assert(uncFile.is_open());
  for(unsigned int iS=0;iS<srinfo.nSR;++iS) for(unsigned int iM=0;iM<srinfo.nMETBins[iS];++iM) uncFile <<"bin_"<< srinfo.getBinName(QCDSupport::SRegInfo::SRReg(iS),iM) <<"\t" << "qcd_tfstatunc_"<< srinfo.getBinName(QCDSupport::SRegInfo::SRReg(iS),iM)<<"\tqcd\t" << srinfo.getTFStatUnc(QCDSupport::SRegInfo::SRReg(iS),iM)  <<endl;
  for(unsigned int iS=0;iS<srinfo.nSR;++iS) for(unsigned int iM=0;iM<srinfo.nMETBins[iS];++iM) uncFile <<"bin_"<< srinfo.getBinName(QCDSupport::SRegInfo::SRReg(iS),iM) <<"\t" << "qcd_jetresptailunc"<< "\tqcd\t" << srinfo.getRespUnc(QCDSupport::SRegInfo::SRReg(iS),iM)  <<endl;
  for(unsigned int iS=0;iS<srinfo.nSR;++iS) for(unsigned int iM=0;iM<srinfo.nMETBins[iS];++iM) uncFile <<"bin_"<< srinfo.getBinName(QCDSupport::SRegInfo::SRReg(iS),iM) <<"\t" << "qcd_bkgsubunc_"<< crinfo.getBinName(srinfo.crRegs[iS],iM)<< "\tqcd\t" << crinfo.getDataYieldCorrUnc(srinfo.crRegs[iS],iM)  <<endl;
  uncFile.close();

  //Now output table
  std::ofstream tabFile;
  tabFile.open ("qcd_pred.tex", ios::out | ios::trunc);
  assert(tabFile.is_open());
  tabFile << "\\begin{table}[!!htbp]"<<endl;
  tabFile << "\\begin{center}"<<endl;
  tabFile <<"\\resizebox*{.7\\textwidth}{!}{"<<endl;
//  tabFile <<"\\begin{tabular}{|l|c|c||c|c|}"<<endl;
  tabFile <<"\\begin{tabular}{|l|c|c|c|c||c|c|c|c|}"<<endl;
  tabFile <<"\\hline"<<endl;
//  tabFile <<"& $N_{\\text{data}}$ & $N^{\\text{non-QCD}}_{\\text{MC}}$ & $\\tfqcd$ & $N_\\text{pred}^{\\text{QCD}}$ \\\\"<<endl;
  tabFile <<"& $N_{\\text{data}}$ & $N^{\\text{non-QCD}}_{\\text{MC}}$ & $N^{\\text{CR orig-QCD}}_{\\text{MC}}$ & $N^{\\text{CR smeared-QCD}}_{\\text{MC}}$ & $N^{\\text{SR orig-QCD}}_{\\text{MC}}$ & $N^{\\text{SR smeared-QCD}}_{\\text{MC}}$ & $\\tfqcd$ & $N_\\text{pred}^{\\text{QCD}}$ \\\\"<<endl;
  tabFile <<"\\hline"<<endl;
  for(unsigned int iS=0;iS<srinfo.nSR;++iS){
//    tabFile <<" \\met [GeV] & & & & & \\multicolumn{2}{c|}{$" << srinfo.srSelNamesLatex[iS] << "$} \\\\";
    tabFile <<" \\met [GeV] & & & & & \\multicolumn{4}{c|}{$" << srinfo.srSelNamesLatex[iS] << "$} \\\\";
    printBigBlock(tabFile,QCDSupport::SRegInfo::SRReg(iS));
  }
  tabFile <<"\\end{tabular}"<<endl;
  tabFile <<"}"<<endl;
//  tabFile <<"\\caption{\\label{tab:0l-qcd-pred}The QCD estimation in the various search regions using the 2.3~\\invfb dataset. The number of data events in the control region with $\\dphijonetwothree<0.1$, which is used as the basis of the prediction in each search region, is also listed, along with the estimated contribution from non-QCD processes in the control region based on simulation. A $100\\%$ uncertainty is assigned on the non-QCD contamination when subtracting it from the data observed in the control region in order to obtain the QCD prediction.}"<<endl;
  tabFile <<"\\end{center}"<<endl;
  tabFile <<"\\end{table}"<<endl;
  tabFile.close();
}

#endif

void GetQCDTable(){
  printTable();
}
