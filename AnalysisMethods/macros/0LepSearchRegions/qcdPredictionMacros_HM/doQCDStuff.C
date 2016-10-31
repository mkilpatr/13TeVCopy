#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/HistogramGetter.hh"
#include "QCDPredictionHelper.h"
#include "TDirectory.h"
#include "TFile.h"

using namespace std;

const float CR_cutoff = .1;

void GetQCDTable(QCDSupport::CRegInfo& crinfo, QCDSupport::SRegInfo& srinfo) {
  cout << "GetQCDTable..." << endl;
  const string outputDir = "GetQCDTable";
  gSystem->mkdir(TString(outputDir), true);

  for(unsigned int iS=0;iS<srinfo.nSR;++iS) for(unsigned int iM=0;iM<srinfo.nMETBins[iS];++iM) cout <<"bin_"<< srinfo.getBinName(QCDSupport::SRegInfo::SRReg(iS),iM) <<"\t" << "TF"<< "\tqcd\t" << srinfo.getTF(QCDSupport::SRegInfo::SRReg(iS),iM)  <<endl;
  for(unsigned int iS=0;iS<srinfo.nSR;++iS) for(unsigned int iM=0;iM<srinfo.nMETBins[iS];++iM) cout <<"bin_"<< srinfo.getBinName(QCDSupport::SRegInfo::SRReg(iS),iM) <<"\t" << "Other MC yields"<< "\tqcd\t" << crinfo.getScaledOtherYield(srinfo.crRegs[iS],iM)  <<endl;
  for(unsigned int iS=0;iS<srinfo.nSR;++iS) for(unsigned int iM=0;iM<srinfo.nMETBins[iS];++iM) cout <<"bin_"<< srinfo.getBinName(QCDSupport::SRegInfo::SRReg(iS),iM) <<"\t" << "Other MC yields corr"<< "\tqcd\t" << crinfo.getDataYieldCorr(srinfo.crRegs[iS],iM)  <<endl;
  for(unsigned int iS=0;iS<srinfo.nSR;++iS) for(unsigned int iM=0;iM<srinfo.nMETBins[iS];++iM) cout <<"bin_"<< srinfo.getBinName(QCDSupport::SRegInfo::SRReg(iS),iM) <<"\t" << "External Unc"<< "\tqcd\t" << srinfo.getExtUncs(QCDSupport::SRegInfo::SRReg(iS),iM)  <<endl;
  for(unsigned int iS=0;iS<srinfo.nSR;++iS) for(unsigned int iM=0;iM<srinfo.nMETBins[iS];++iM) cout <<"bin_"<< srinfo.getBinName(QCDSupport::SRegInfo::SRReg(iS),iM) <<"\t" << "Total syst"<< "\tqcd\t" << srinfo.getTotalSystUnc(&crinfo,QCDSupport::SRegInfo::SRReg(iS),iM)  <<endl;
  for(unsigned int iS=0;iS<srinfo.nSR;++iS) for(unsigned int iM=0;iM<srinfo.nMETBins[iS];++iM) cout <<"bin_"<< srinfo.getBinName(QCDSupport::SRegInfo::SRReg(iS),iM) <<"\t" << "Data stat"<< "\tqcd\t" <<  crinfo.getDataStatUnc(srinfo.crRegs[iS],iM)  <<endl;
  for(unsigned int iS=0;iS<srinfo.nSR;++iS){
    if(QCDSupport::integrating_over_met){
      int minBin = 0;
      if( (srinfo.crRegs[iS] == QCDSupport::CRegInfo::CR_nbINC_mtb175_nj5t_nt0_nw1t) || (srinfo.crRegs[iS] == QCDSupport::CRegInfo::CR_nbINC_mtb175_nj5t_nt1t_nw0) ){
        minBin = srinfo.nMETBins[iS] - 1;
      }
      if( (srinfo.crRegs[iS] == QCDSupport::CRegInfo::CR_nbINC_mtb175_nj5t_nt0_nw1t)  || (srinfo.crRegs[iS] == QCDSupport::CRegInfo::CR_nbINC_mtb175_nj5t_nt1t_nw0) || (srinfo.crRegs[iS] == QCDSupport::CRegInfo::CR_nbINC_mtb175_nj5t_nt1t_nw1t) ){
        for(unsigned int iM=minBin;iM<srinfo.nMETBins[iS];++iM) cout <<"bin_"<< srinfo.getBinName(QCDSupport::SRegInfo::SRReg(iS),iM) <<"\t" << "MET Integration Systematic HM"<< "\tqcd\t" << QCDSupport::integrating_met_systematic + 1 <<endl;
      }
    }
  }
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
  uncFile.open (outputDir + "/values_0l_unc_qcd.conf", ios::out | ios::trunc);
  assert(uncFile.is_open());
  for(unsigned int iS=0;iS<srinfo.nSR;++iS) for(unsigned int iM=0;iM<srinfo.nMETBins[iS];++iM) uncFile <<"bin_"<< srinfo.getBinName(QCDSupport::SRegInfo::SRReg(iS),iM) <<"\t" << "qcd_num_tfstatunc_"<<srinfo.getBinName(QCDSupport::SRegInfo::SRReg(iS),iM)<<"\tqcd\t" << srinfo.getTFStatUnc_N(QCDSupport::SRegInfo::SRReg(iS),iM)  <<endl;
  for(unsigned int iS=0;iS<srinfo.nSR;++iS){
    if(QCDSupport::integrating_over_met){
      int minBin = 0;
      if( (srinfo.crRegs[iS] == QCDSupport::CRegInfo::CR_nbINC_mtb175_nj5t_nt0_nw1t) || (srinfo.crRegs[iS] == QCDSupport::CRegInfo::CR_nbINC_mtb175_nj5t_nt1t_nw0) ){
        minBin = srinfo.nMETBins[iS] - 2;
      }
      if( (srinfo.crRegs[iS] == QCDSupport::CRegInfo::CR_nbINC_mtb175_nj5t_nt0_nw1t)  || (srinfo.crRegs[iS] == QCDSupport::CRegInfo::CR_nbINC_mtb175_nj5t_nt1t_nw0) || (srinfo.crRegs[iS] == QCDSupport::CRegInfo::CR_nbINC_mtb175_nj5t_nt1t_nw1t) ){
        for(unsigned int iM=0;iM<srinfo.nMETBins[iS];++iM) uncFile <<"bin_"<< srinfo.getBinName(QCDSupport::SRegInfo::SRReg(iS),iM) <<"\t" << "qcd_den_tfstatunc_"<<(((minBin == 0) || (iM >= minBin)) ? TString::Format("%.0f",srinfo.metBins[iS][minBin]) : TString::Format("%.0f",srinfo.metBins[iS][iM]))<<"_"<<crinfo.crRegBinNames[srinfo.crRegs[iS]]<<"\tqcd\t" << srinfo.getTFStatUnc_D(QCDSupport::SRegInfo::SRReg(iS),iM)  <<endl;
      } else {
        for(unsigned int iM=0;iM<srinfo.nMETBins[iS];++iM) uncFile <<"bin_"<< srinfo.getBinName(QCDSupport::SRegInfo::SRReg(iS),iM) <<"\t" << "qcd_den_tfstatunc_"<< crinfo.getBinName(srinfo.crRegs[iS],iM)<<"\tqcd\t" << srinfo.getTFStatUnc_D(QCDSupport::SRegInfo::SRReg(iS),iM)  <<endl;
      }
    } else {
      for(unsigned int iM=0;iM<srinfo.nMETBins[iS];++iM) uncFile <<"bin_"<< srinfo.getBinName(QCDSupport::SRegInfo::SRReg(iS),iM) <<"\t" << "qcd_den_tfstatunc_"<< crinfo.getBinName(srinfo.crRegs[iS],iM)<<"\tqcd\t" << srinfo.getTFStatUnc_D(QCDSupport::SRegInfo::SRReg(iS),iM)  <<endl;
    }
  }
  for(unsigned int iS=0;iS<srinfo.nSR;++iS) for(unsigned int iM=0;iM<srinfo.nMETBins[iS];++iM) uncFile <<"bin_"<< srinfo.getBinName(QCDSupport::SRegInfo::SRReg(iS),iM) <<"\t" << "qcd_jetresptailunc"<< "\tqcd\t" << srinfo.getRespUnc(QCDSupport::SRegInfo::SRReg(iS),iM)  <<endl;
  for(unsigned int iS=0;iS<srinfo.nSR;++iS){
    if(QCDSupport::integrating_over_met){
      int minBin = 0;
      if( (srinfo.crRegs[iS] == QCDSupport::CRegInfo::CR_nbINC_mtb175_nj5t_nt0_nw1t) || (srinfo.crRegs[iS] == QCDSupport::CRegInfo::CR_nbINC_mtb175_nj5t_nt1t_nw0) ){
        minBin = srinfo.nMETBins[iS] - 2;
      }
      if( (srinfo.crRegs[iS] == QCDSupport::CRegInfo::CR_nbINC_mtb175_nj5t_nt0_nw1t)  || (srinfo.crRegs[iS] == QCDSupport::CRegInfo::CR_nbINC_mtb175_nj5t_nt1t_nw0) || (srinfo.crRegs[iS] == QCDSupport::CRegInfo::CR_nbINC_mtb175_nj5t_nt1t_nw1t) ){
        for(unsigned int iM=0;iM<srinfo.nMETBins[iS];++iM) uncFile <<"bin_"<< srinfo.getBinName(QCDSupport::SRegInfo::SRReg(iS),iM) <<"\t" << "qcd_bkgsubunc_"<<(((minBin == 0) || (iM >= minBin)) ? TString::Format("%.0f",srinfo.metBins[iS][minBin]) : TString::Format("%.0f",srinfo.metBins[iS][iM]))<<"_"<<crinfo.crRegBinNames[srinfo.crRegs[iS]]<<"\tqcd\t" << crinfo.getDataYieldCorrUnc(srinfo.crRegs[iS],iM)  <<endl;
      } else {
        for(unsigned int iM=0;iM<srinfo.nMETBins[iS];++iM) uncFile <<"bin_"<< srinfo.getBinName(QCDSupport::SRegInfo::SRReg(iS),iM) <<"\t" << "qcd_bkgsubunc_"<< crinfo.getBinName(srinfo.crRegs[iS],iM)<<"\tqcd\t" << crinfo.getDataYieldCorrUnc(srinfo.crRegs[iS],iM)  <<endl;
      }
    } else {
      for(unsigned int iM=0;iM<srinfo.nMETBins[iS];++iM) uncFile <<"bin_"<< srinfo.getBinName(QCDSupport::SRegInfo::SRReg(iS),iM) <<"\t" << "qcd_bkgsubunc_"<< crinfo.getBinName(srinfo.crRegs[iS],iM)<< "\tqcd\t" << crinfo.getDataYieldCorrUnc(srinfo.crRegs[iS],iM)  <<endl;
    }
  }
  for(unsigned int iS=0;iS<srinfo.nSR;++iS){
    if(QCDSupport::integrating_over_met){
      int minBin = 0;
      if( (srinfo.crRegs[iS] == QCDSupport::CRegInfo::CR_nbINC_mtb175_nj5t_nt0_nw1t) || (srinfo.crRegs[iS] == QCDSupport::CRegInfo::CR_nbINC_mtb175_nj5t_nt1t_nw0) ){
        minBin = srinfo.nMETBins[iS] - 2;
      }
      if( (srinfo.crRegs[iS] == QCDSupport::CRegInfo::CR_nbINC_mtb175_nj5t_nt0_nw1t)  || (srinfo.crRegs[iS] == QCDSupport::CRegInfo::CR_nbINC_mtb175_nj5t_nt1t_nw0) || (srinfo.crRegs[iS] == QCDSupport::CRegInfo::CR_nbINC_mtb175_nj5t_nt1t_nw1t) ){
        for(unsigned int iM=minBin;iM<srinfo.nMETBins[iS];++iM) uncFile <<"bin_"<< srinfo.getBinName(QCDSupport::SRegInfo::SRReg(iS),iM) <<"\t" << "qcd_met_integration_systematic_" << srinfo.getBinName(QCDSupport::SRegInfo::SRReg(iS),iM) << "\tqcd\t" << QCDSupport::integrating_met_systematic + 1 <<endl;
      }
    }
  }
  uncFile.close();

  //Now output table
  std::ofstream tabFile;
  tabFile.open (outputDir + "/qcd_pred.tex", ios::out | ios::trunc);
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

TH1F* copyHistIntoMergedHist(TH1F* p_Hist, int p_nBins, double* p_Bins, int p_MinBin){
  TH1F* tempHist = new TH1F(TString(p_Hist->GetName()) + "_merged", TString(p_Hist->GetTitle()) + "_merged", p_nBins, p_Bins);
  for(int iB = 1; iB <= p_MinBin; iB++){
    tempHist->SetBinContent(iB, p_Hist->GetBinContent(iB));
    tempHist->SetBinError  (iB, p_Hist->GetBinError(iB));
  }
  return tempHist;
}

void MakeCRPlots(QCDSupport::CRegInfo& crinfo, vector<TString>& names, vector<TTree*>& trees, vector<int>& colors) {
  cout << "MakeCRPlots..." << endl;
  const TString outputDir = "MakeCRPlots";
  gSystem->mkdir(outputDir, true);

  for(unsigned int iS = 0; iS < crinfo.nCR; ++iS){
    TString title = TString::Format("%s",crinfo.crSelNames[iS].Data());
    TString name = TString::Format("getRegion_0_%u",iS);
    Plot * plot = new Plot(name,title,crinfo.CRmetGetters[iS]->plotInfo->xTitle,"Events");
    TString qcdWVSEL = TString::Format("%s && %s",crinfo.crPreQCDWithVeto.Data(),crinfo.crSel[iS].Data());
    TString t2ttSEL  = TString::Format("%s && %s",crinfo.crPreData.Data(),crinfo.crSel[iS].Data());
    float yMax = 0.;
    float totMCYield = 0;
    int minBin = 0;
    int nBins = 2;
    double* tempMetBins = new double[nBins] { crinfo.metBins[iS][0], crinfo.metBins[iS][crinfo.nMETBins[iS]] };
    if(QCDSupport::integrating_over_met){
      if( (iS == QCDSupport::CRegInfo::CR_nbINC_mtb175_nj5t_nt0_nw1t) || (iS == QCDSupport::CRegInfo::CR_nbINC_mtb175_nj5t_nt1t_nw0) ){
        minBin = crinfo.nMETBins[iS] - 1;
        nBins  = minBin + 1;
        tempMetBins = new double[nBins];
        for(unsigned int iM = 0; iM < minBin; iM++){
          tempMetBins[iM] = crinfo.metBins[iS][iM];
        }
        tempMetBins[minBin] = crinfo.metBins[iS][crinfo.nMETBins[iS]];
      }
    }
    for(unsigned int iT = 0; iT < trees.size(); ++iT){
      if(names[iT] == "Data"){
        TH1F * hSN = (TH1F*)crinfo.dataYields[iS]->Clone();
        if(QCDSupport::integrating_over_met && ((iS == QCDSupport::CRegInfo::CR_nbINC_mtb175_nj5t_nt0_nw1t) || (iS == QCDSupport::CRegInfo::CR_nbINC_mtb175_nj5t_nt1t_nw0) || (iS == QCDSupport::CRegInfo::CR_nbINC_mtb175_nj5t_nt1t_nw1t)) ){
          TH1F* tempHist = copyHistIntoMergedHist(hSN, nBins - 1, tempMetBins, minBin + 1);
          plot->addHist(tempHist,names[iT],"E0",colors[iT],0,colors[iT]);
        } else{
          plot->addHist(hSN,names[iT],"E0",colors[iT],0,colors[iT]);
        }
        if(yMax < hSN->GetMaximum()) yMax = hSN->GetMaximum();
      }
      else if(names[iT] == "With orig. QCD MC"){
        TH1F * hSN = crinfo.CRmetGetters[iS]->getHistogram(trees[iT],qcdWVSEL,QCDSupport::stdMCWeight,TString::Format("tree_%s_%u",crinfo.crRegBinNames[iS].Data(), iT));
        hSN->Add(crinfo.otherBKGScaledYields[iS]);
        if(QCDSupport::integrating_over_met && ((iS == QCDSupport::CRegInfo::CR_nbINC_mtb175_nj5t_nt0_nw1t) || (iS == QCDSupport::CRegInfo::CR_nbINC_mtb175_nj5t_nt1t_nw0) || (iS == QCDSupport::CRegInfo::CR_nbINC_mtb175_nj5t_nt1t_nw1t)) ){
          TH1F* tempHist = copyHistIntoMergedHist(hSN, nBins - 1, tempMetBins, minBin + 1);
          plot->addHistForRatio(tempHist,names[iT],"hist",colors[iT],0,colors[iT]);
        } else {
          plot->addHistForRatio(hSN,names[iT],"hist",colors[iT],0,colors[iT]);
        }
      }
      else if (names[iT] == "Non-QCD bkg"){
        TH1F * hSN = (TH1F*)crinfo.otherBKGScaledYields[iS]->Clone();
        if(QCDSupport::integrating_over_met && ((iS == QCDSupport::CRegInfo::CR_nbINC_mtb175_nj5t_nt0_nw1t) || (iS == QCDSupport::CRegInfo::CR_nbINC_mtb175_nj5t_nt1t_nw0) || (iS == QCDSupport::CRegInfo::CR_nbINC_mtb175_nj5t_nt1t_nw1t)) ){
          TH1F* tempHist = copyHistIntoMergedHist(hSN, nBins - 1, tempMetBins, minBin + 1);
          plot->addToStack(tempHist,names[iT],colors[iT],1001,1,1,3);
        } else {
          plot->addToStack(hSN,names[iT],colors[iT],1001,1,1,3);
        }
      }
      else if (names[iT].Contains("T2tt")){
        TH1F* hSN = crinfo.CRmetGetters[iS]->getHistogram(trees[iT], t2ttSEL, QCDSupport::stdMCWeight, TString::Format("tree_%s_%u", crinfo.crRegBinNames[iS].Data(), iT));
        hSN->Scale(totMCYield / hSN->Integral(0, hSN->GetNbinsX() + 1));
        if(QCDSupport::integrating_over_met && ((iS == QCDSupport::CRegInfo::CR_nbINC_mtb175_nj5t_nt0_nw1t) || (iS == QCDSupport::CRegInfo::CR_nbINC_mtb175_nj5t_nt1t_nw0) || (iS == QCDSupport::CRegInfo::CR_nbINC_mtb175_nj5t_nt1t_nw1t)) ){
          TH1F* tempHist = copyHistIntoMergedHist(hSN, nBins - 1, tempMetBins, minBin + 1);
          plot->addHist(tempHist, names[iT], "hist", colors[iT], 0, colors[iT]);
        } else {
          plot->addHist(hSN, names[iT], "hist", colors[iT], 0, colors[iT]);
        }
      }
      else {
        TH1F * hSN = (TH1F*)crinfo.qcdYieldsWithVeto[iS]->Clone();
        if(QCDSupport::integrating_over_met && ((iS == QCDSupport::CRegInfo::CR_nbINC_mtb175_nj5t_nt0_nw1t) || (iS == QCDSupport::CRegInfo::CR_nbINC_mtb175_nj5t_nt1t_nw0) || (iS == QCDSupport::CRegInfo::CR_nbINC_mtb175_nj5t_nt1t_nw1t)) ){
          TH1F* tempHist = copyHistIntoMergedHist(hSN, nBins - 1, tempMetBins, minBin + 1);
          plot->addToStack(tempHist,names[iT],colors[iT],1001,1,1,3);
        } else {
          plot->addToStack(hSN,names[iT],colors[iT],1001,1,1,3);
        }
      }
    }
    TH1F * hSN  = (TH1F*)(crinfo.qcdYieldsWithVeto[iS])->Clone(TString::Format("ratioUnc_%u", iS));
    hSN->Add(crinfo.otherBKGScaledYields[iS]);
    for(unsigned int iB = 1; iB <= hSN->GetNbinsX(); ++iB){
      double subSyst = (crinfo.otherBKGCorrUnc[iS]->GetBinContent(iB) - 1)*hSN->GetBinContent(iB);
      double stat = crinfo.qcdYieldsWithVeto[iS]->GetBinError(iB);
      hSN->SetBinError(iB,TMath::Sqrt(subSyst*subSyst +stat*stat ));
    }
    if(QCDSupport::integrating_over_met && ((iS == QCDSupport::CRegInfo::CR_nbINC_mtb175_nj5t_nt0_nw1t) || (iS == QCDSupport::CRegInfo::CR_nbINC_mtb175_nj5t_nt1t_nw0) || (iS == QCDSupport::CRegInfo::CR_nbINC_mtb175_nj5t_nt1t_nw1t)) ){
      TH1F* tempHist = copyHistIntoMergedHist(hSN, nBins - 1, tempMetBins, minBin + 1);
      plot->setUncertaintyHist((TH1F*)(tempHist));
    } else {
      plot->setUncertaintyHist((TH1F*)(hSN));
    }
    plot->setPlotRatioUncertaintyBand();

    TCanvas * c = new TCanvas;
    plot->setDrawCMSLumi();
    plot->setUsePoisson();
    plot->setYRange(0.1, 1000. * yMax);
    plot->setTitle(" ");
    plot->setLegend(.65,.58,.95,.88);
//    plot->getLegend()->SetNColumns(2);

    plot->setLogy();
    c->cd();
    plot->drawRatioStack(c,false,QCDSupport::format);
    c->cd();
    c->Modified();
    TLatex *tl = new TLatex();
    tl->SetTextSize(0.03);
    tl->DrawLatexNDC(0.2, 0.7, crinfo.crSelLabels[iS]);

    QCDSupport::setTitleOffset(c);
    c->Update();

    c->SaveAs(TString::Format(outputDir + "/QCD_CR_%u.",iS) + QCDSupport::format);
  }
}

void MakeSRPlots(QCDSupport::SRegInfo& srinfo, vector<TString>& names, vector<TTree*>& trees, vector<int>& colors) {
  cout << "MakeSRPlots..." << endl;
  const TString outputDir = "MakeSRPlots";
  gSystem->mkdir(outputDir, true);

  for(unsigned int iS = 0; iS < srinfo.nSR; ++iS){
    TString title = TString::Format("%s",srinfo.srSelNames[iS].Data());
    TString name = TString::Format("getRegion_0_%u",iS);
    Plot * plot = new Plot(name,title,srinfo.SRmetGetters[iS]->plotInfo->xTitle,"Events");
    TString sel_SR = TString::Format("%s && %s", srinfo.srPreQCD.Data(), srinfo.srSel[iS].Data());

    float yMax = 0.;
    float totMCYield = 0;
    for(unsigned int iT = 0; iT < trees.size(); ++iT){
      if(names[iT] == "Data"){
        TH1F * hSN = srinfo.SRmetGetters[iS]->getHistogram(trees[iT],sel_SR,"1.0",TString::Format("tree_%s_%u",srinfo.srRegBinNames[iS].Data(), iT));
        for(int iBin = 1; iBin <= hSN->GetNbinsX(); ++iBin) if(hSN->GetBinLowEdge(iBin) + hSN->GetBinWidth(iBin) > CR_cutoff) hSN->SetBinContent(iBin, -999);
        plot->addHist(hSN,names[iT],"E0",colors[iT],0,colors[iT]);
      }
//      else if(names[iT] == "Without #it{r}_{jet} corr"){
//        TH1F * hSN = srinfo.SRmetGetters[iS]->getHistogram(trees[iT],sel_SR,QCDSupport::stdMCWeight,TString::Format("tree_%u",iT));
//        hSN->Add(srinfo.otherBKGScaledYields[iS]);
//        plot->addHistForRatio(hSN,names[iT],"hist",colors[iT],0,colors[iT]);
//      }
      else if(names[iT] == "With orig. QCD MC"){
        TH1F * hSN = srinfo.SRmetGetters[iS]->getHistogram(trees[iT],sel_SR,QCDSupport::stdQCDWeight,TString::Format("tree_%s_%u",srinfo.srRegBinNames[iS].Data(), iT));
//        plot->addHistForRatio(hSN,names[iT],"hist",colors[iT],0,colors[iT]);
        plot->addHist(hSN,names[iT],"hist",colors[iT],0,colors[iT]);
      }
      else if (names[iT] == "Non-QCD bkg"){
        TH1F * hSN = srinfo.SRmetGetters[iS]->getHistogram(trees[iT],sel_SR,QCDSupport::stdMCWeight,TString::Format("tree_%s_%u",srinfo.srRegBinNames[iS].Data(), iT));
        if(yMax < hSN->GetMaximum()) yMax = hSN->GetMaximum();
        totMCYield += hSN->Integral(0, hSN->GetNbinsX() + 1);
        plot->addToStack(hSN,names[iT],colors[iT],1001,1,1,3);
      }
      else if (names[iT].Contains("T2tt")){
        TH1F* hSN = srinfo.SRmetGetters[iS]->getHistogram(trees[iT], sel_SR, QCDSupport::stdMCWeight, TString::Format("tree_%s_%u", srinfo.srRegBinNames[iS].Data(), iT));
        hSN->Scale(totMCYield / hSN->Integral(0, hSN->GetNbinsX() + 1));
        plot->addHist(hSN, names[iT], "hist", colors[iT], 0, colors[iT]);
      }
      else if (names[iT] == "Smeared QCD MC"){
        TH1F * hSN = srinfo.SRmetGetters[iS]->getHistogram(trees[iT],sel_SR,QCDSupport::stdQCDWeight,TString::Format("tree_%s_%u", srinfo.srRegBinNames[iS].Data(), iT));
        totMCYield += hSN->Integral(0, hSN->GetNbinsX() + 1);
        plot->addToStack(hSN,names[iT],colors[iT],1001,1,1,3);
//        TH1F * hSN = (TH1F*)srinfo.qcdYieldsWithVeto[iS]->Clone();
      }
    }

    TCanvas * c = new TCanvas;
    plot->setDrawCMSLumi();
    plot->setUsePoisson();
    plot->setYRange(0.1, 1000. * yMax);
    plot->setTitle(" ");
    plot->setLegend(.65,.58,.95,.88);
//    plot->getLegend()->SetNColumns(2);

    plot->setLogy();
    c->cd();
    plot->draw(c,false,QCDSupport::format);
    c->cd();
    c->Modified();
    TLatex *tl = new TLatex();
    tl->SetTextSize(0.03);
    tl->DrawLatexNDC(0.2, 0.7, srinfo.srSelLabels[iS]);

    QCDSupport::setTitleOffset(c);
    c->Update();

    c->SaveAs(TString::Format(outputDir + "/QCD_SR_%u.",iS) + QCDSupport::format);
  }
}

void MakeDPhiPlots(QCDSupport::CRegInfo& crinfo, vector<TString>& names, vector<TTree*>& trees, vector<int>& colors) {
  cout << "MakeDPhiPlots..." << endl;
  const TString outputDir = "MakeDPhiPlots";
  gSystem->mkdir(outputDir, true);

  HistogramGetter* dPhi_HG = new HistogramGetter("dphi", "min(dphij1met, min(dphij2met, dphij3met))", "min(#Delta#phi(j_{1,2,3},#slash{#it{E}}_{T}))", 32, 0, 3.2);
  for(unsigned int iCR = 0; iCR < crinfo.nCR; ++iCR){
    TString title = TString::Format("%s", crinfo.crSelNames[iCR].Data());
    TString name  = TString::Format("dphi_%s", crinfo.crRegBinNames[iCR].Data());
    TString sel   = TString::Format("%s && %s && %s && (nvetolep==0) && (nvetotau==0)", QCDSupport::METPresel.Data(), QCDSupport::BaselineExtraCuts.Data(), crinfo.crSel[iCR].Data());
cout << title << endl;
cout << name  << endl;
cout << sel   << endl;
    Plot* plot_allMet = new Plot(name, title, dPhi_HG->plotInfo->xTitle, "Events");
    float yMin = 99999., yMax = 0.;
    float totMCYield = 0;
    TH1F* hist_nonQCD_bkg = 0;
    for(unsigned int iT = 0; iT < trees.size(); ++iT){
      if(names[iT] == "Data"){
        TH1F* hist = dPhi_HG->getHistogram(trees[iT], sel, "1.0", TString::Format("data_%s", name.Data()));
        for(int iBin = 1; iBin <= hist->GetNbinsX(); ++iBin) if(hist->GetBinLowEdge(iBin) + hist->GetBinWidth(iBin) > CR_cutoff) hist->SetBinContent(iBin, -999);
        plot_allMet->addHist(hist, names[iT], "E0", colors[iT], 0, colors[iT]);
      } else if(names[iT] == "Non-QCD bkg"){
        hist_nonQCD_bkg = dPhi_HG->getHistogram(trees[iT], sel, QCDSupport::stdMCWeight, TString::Format("nonqcd_%s", name.Data()));
        totMCYield += hist_nonQCD_bkg->Integral(0, hist_nonQCD_bkg->GetNbinsX() + 1);
        plot_allMet->addToStack(hist_nonQCD_bkg, names[iT], colors[iT], 1001, 1, 1, 3);
      } else if(names[iT].Contains("Smeared")){
        TH1F* hist = dPhi_HG->getHistogram(trees[iT], sel, QCDSupport::stdQCDWeight, TString::Format("qcd_%s", name.Data()));
        totMCYield += hist->Integral(0, hist->GetNbinsX() + 1);
        plot_allMet->addToStack(hist, names[iT], colors[iT], 1001, 1, 1, 3);
      } else if(names[iT] == "With orig. QCD MC"){
        TH1F* hist = dPhi_HG->getHistogram(trees[iT], sel, QCDSupport::stdQCDWeight, TString::Format("qcd_orig_%s", name.Data()));
        hist->Add(hist_nonQCD_bkg);
        if(yMin > hist->GetMinimum()) yMin = hist->GetMinimum();
        if(yMax < hist->GetMaximum()) yMax = hist->GetMaximum();
        plot_allMet->addHist(hist, names[iT], "hist", colors[iT], 0, colors[iT]);
      } else if(names[iT].Contains("T2tt")){
        TH1F* hist = dPhi_HG->getHistogram(trees[iT], sel, QCDSupport::stdMCWeight, TString::Format("signal_%s_%u", name.Data(), iT));
        hist->Scale(totMCYield / hist->Integral(0, hist->GetNbinsX() + 1));
        plot_allMet->addHist(hist, names[iT], "hist", colors[iT], 0, colors[iT]);
      }
    }
    TCanvas* c = new TCanvas;
    plot_allMet->setDrawCMSLumi();
    plot_allMet->setUsePoisson();
//    if(yMin <= 0) yMin = 0.1;
//    plot_allMet->setYRange(0.1 * yMin, 1000 * yMax);
//    plot_allMet->setLogy();
    plot_allMet->setYRange(0, 1.4 * yMax);
    plot_allMet->setTitle(" ");
    plot_allMet->setLegend(.6,.58,.90,.88);
    c->cd();
    plot_allMet->draw(c,false,QCDSupport::format);
    c->cd();
    c->Modified();
    TLatex *tl = new TLatex();
    tl->SetTextSize(0.03);
    tl->DrawLatexNDC(0.2, 0.7, crinfo.crSelLabels[iCR]);
    QCDSupport::setTitleOffset(c);
    c->Update();
    c->SaveAs(outputDir + "/" + plot_allMet->getName() + TString(".") + QCDSupport::format);
    for(int iMet = 0; iMet < crinfo.nMETBins[iCR]; ++iMet){
      if(iMet != crinfo.nMETBins[iCR] - 1){
        title = TString::Format("%.f #leq #slash{#it{E}}_{T} < %.f, %s", crinfo.metBins[iCR][iMet], crinfo.metBins[iCR][iMet + 1], crinfo.crSelNames[iCR].Data());
        name  = TString::Format("dphi_%s_met_%.f_to_%.f", crinfo.crRegBinNames[iCR].Data(), crinfo.metBins[iCR][iMet], crinfo.metBins[iCR][iMet + 1]);
        sel   = TString::Format("met >= %.f && met < %.f && %s && %s && %s && (nvetolep==0) && (nvetotau==0)", crinfo.metBins[iCR][iMet], crinfo.metBins[iCR][iMet + 1], QCDSupport::METPresel.Data(), QCDSupport::BaselineExtraCuts.Data(), crinfo.crSel[iCR].Data());
      } else {
        title = TString::Format("%.f #leq #slash{#it{E}}_{T}, %s", crinfo.metBins[iCR][iMet], crinfo.crSelNames[iCR].Data());
        name  = TString::Format("dphi_%s_met_ge_%.f", crinfo.crRegBinNames[iCR].Data(), crinfo.metBins[iCR][iMet]);
        sel   = TString::Format("met >= %.f && %s && %s && %s && (nvetolep==0) && (nvetotau==0)", crinfo.metBins[iCR][iMet], QCDSupport::METPresel.Data(), QCDSupport::BaselineExtraCuts.Data(), crinfo.crSel[iCR].Data());
      }
cout << title << endl;
cout << name  << endl;
cout << sel   << endl;
      Plot* plot = new Plot(name, title, dPhi_HG->plotInfo->xTitle, "Events");
      yMin = 99999.;
      yMax = 0.;
      totMCYield = 0;
      TH1F* hist_nonQCD_bkg = 0;
      for(unsigned int iT = 0; iT < trees.size(); ++iT){
        if(names[iT] == "Data"){
          TH1F* hist = dPhi_HG->getHistogram(trees[iT], sel, "1.0", TString::Format("data_%s", name.Data()));
          for(int iBin = 1; iBin <= hist->GetNbinsX(); ++iBin) if(hist->GetBinLowEdge(iBin) + hist->GetBinWidth(iBin) > CR_cutoff) hist->SetBinContent(iBin, -999);
          plot->addHist(hist, names[iT], "E0", colors[iT], 0, colors[iT]);
        } else if(names[iT] == "Non-QCD bkg"){
          hist_nonQCD_bkg = dPhi_HG->getHistogram(trees[iT], sel, QCDSupport::stdMCWeight, TString::Format("nonqcd_%s", name.Data()));
          totMCYield += hist_nonQCD_bkg->Integral(0, hist_nonQCD_bkg->GetNbinsX() + 1);
          plot->addToStack(hist_nonQCD_bkg, names[iT], colors[iT], 1001, 1, 1, 3);
        } else if(names[iT].Contains("Smeared")){
          TH1F* hist = dPhi_HG->getHistogram(trees[iT], sel, QCDSupport::stdQCDWeight, TString::Format("qcd_%s", name.Data()));
          totMCYield += hist->Integral(0, hist->GetNbinsX() + 1);
          plot->addToStack(hist, names[iT], colors[iT], 1001, 1, 1, 3);
        } else if(names[iT] == "With orig. QCD MC"){
          TH1F* hist = dPhi_HG->getHistogram(trees[iT], sel, QCDSupport::stdQCDWeight, TString::Format("qcd_orig_%s", name.Data()));
          hist->Add(hist_nonQCD_bkg);
          if(yMin > hist->GetMinimum()) yMin = hist->GetMinimum();
          if(yMax < hist->GetMaximum()) yMax = hist->GetMaximum();
          plot->addHistForRatio(hist, names[iT], "hist", colors[iT], 0, colors[iT]);
        } else if(names[iT].Contains("T2tt")){
          TH1F* hist = dPhi_HG->getHistogram(trees[iT], sel, QCDSupport::stdMCWeight, TString::Format("signal_%s_%u", name.Data(), iT));
          hist->Scale(totMCYield / hist->Integral(0, hist->GetNbinsX() + 1));
          plot->addHist(hist, names[iT], "hist", colors[iT], 0, colors[iT]);
        }
      }
      TCanvas* c = new TCanvas;
      plot->setDrawCMSLumi();
      plot->setUsePoisson();
//      if(yMin <= 0) yMin = 0.1;
//      plot->setYRange(0.1 * yMin, 1000 * yMax);
//      plot->setLogy();
      plot->setYRange(0, 1.4 * yMax);
      plot->setTitle(" ");
      plot->setLegend(.6,.58,.90,.88);
      c->cd();
      plot->draw(c,false,QCDSupport::format);
      c->cd();
      c->Modified();
      TLatex *tl = new TLatex();
      tl->SetTextSize(0.03);
      tl->DrawLatexNDC(0.2, 0.7, crinfo.crSelLabels[iCR]);
      QCDSupport::setTitleOffset(c);
      c->Update();
      c->SaveAs(outputDir + "/" + plot->getName() + TString(".") + QCDSupport::format);
    }
  }
}

void MakeTFPlots(QCDSupport::CRegInfo& crinfo, QCDSupport::SRegInfo& srinfo) {
  cout << "MakeTFPlots..." << endl;
  const TString outputDir = "MakeTFPlots";
  gSystem->mkdir(outputDir, true);

  for(unsigned int iS = 0; iS < srinfo.nSR; ++iS){
    Plot * plot = new Plot(TString::Format("TF_%u",iS),srinfo.srSelNames[iS],"","SR/CR");

    plot->addHist(srinfo.origQCD_TF[iS],"w/o smearing or #it{r}_{jet} corr","",color_tW,0,color_tW);
    plot->addHist(srinfo.noResp[iS],"w/o #it{r}_{jet} corr","hist",color_znunu,0,color_znunu);
//    plot->addHist(srinfo.respUp[iS],"#it{r}_{jet} up variation","hist",color_qcd,0,color_qcd);
//    plot->addHist(srinfo.respDown[iS],"#it{r}_{jet} down variation","hist",5,0,5);
    plot->addHist(srinfo.nominalQCD[iS],"Nominal","",1,0,1);
    float yMin = max(0.001, srinfo.nominalQCD[iS]->GetMinimum());
    float yMax = srinfo.nominalQCD[iS]->GetMaximum();

    TCanvas * c = new TCanvas;
    plot->setDrawCMSLumi();
//    plot->setLegend(.35,.70,.92,.9);
    plot->setYRange(0.1 * yMin, 75 * yMax);
    plot->setTitle(" ");
    plot->setLegend(.65,.68,.95,.88);
    plot->setXTitle(srinfo.SRmetGetters[iS]->plotInfo->xTitle);
    plot->setLogy();
//    plot->getLegend()->SetNColumns(2);
//    plot->setHeader("","");
    c->cd();
    plot->draw(c,false,QCDSupport::format);
    c->cd();
    c->Modified();
    TLatex *tl = new TLatex();
    tl->SetTextSize(0.03);
    tl->DrawLatexNDC(0.2, 0.7, srinfo.srSelLabels[iS]);

    QCDSupport::setTitleOffset(c);
    c->Update();
    c->SaveAs(TString::Format("%s/%s.%s", outputDir.Data(), plot->getName().Data(), QCDSupport::format.Data()));
  }
}

#endif

void doQCDStuff(){
  cout << "doQCDStuff..." << endl;
  StyleTools::SetTDRStyle();
  gStyle->SetTitleOffset(1.400,"Y");
  gStyle->SetTitleOffset(0.950,"X");
  gStyle->SetPadTopMargin(0.08);

  QCDSupport::CRegInfo crinfo;
  crinfo.fillOtherBkgSFs(QCDSupport::inputDir + "/met_tree_skimmed_baseline.root",
                         QCDSupport::inputDir + "/ttbarplusw_tree_skimmed_baseline.root",
                         QCDSupport::inputDir + "/remaining_nonQCD_tree_skimmed_baseline.root",
                         QCDSupport::inputDir + "/qcd_smeared_tree_skimmed_baseline.root",
//                         QCDSupport::inputDir + "/qcd_smeared_CHEF_tree_skimmed_baseline.root",
                         QCDSupport::inputDir + "/qcd_orig_tree_skimmed_baseline.root");
  crinfo.fillCorrectedOtherBkgYields();
  crinfo.fillDataCorr();

  QCDSupport::SRegInfo srinfo;
  srinfo.fillTFVariations(QCDSupport::inputDir + "/qcd_smeared_tree_skimmed_baseline.root",
//  srinfo.fillTFVariations(QCDSupport::inputDir + "/qcd_smeared_CHEF_tree_skimmed_baseline.root",
                          QCDSupport::inputDir + "/qcd_orig_tree_skimmed_baseline.root");
  srinfo.loadExternUnc("../extUncs/");

  cout <<"DataCard info!"<<endl;

  vector<TString> names;
  vector<TTree*> trees;
  vector<int> colors;
  trees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/met_tree_skimmed_baseline.root"));               names.push_back("Data");              colors.push_back(1);
  trees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/nonQCD_tree_skimmed_baseline.root"));            names.push_back("Non-QCD bkg");       colors.push_back(color_ttbar);
  trees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/qcd_smeared_tree_skimmed_baseline.root"));       names.push_back("Smeared QCD MC");    colors.push_back(color_qcd);
//  trees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/qcd_smeared_CHEF_tree_skimmed_baseline.root"));  names.push_back("Smeared QCD MC");    colors.push_back(color_qcd);
  trees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/qcd_orig_tree_skimmed_baseline.root"));          names.push_back("With orig. QCD MC"); colors.push_back(color_tW);
//  trees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/T2tt_425_325_tree_skimmed_baseline.root"));      names.push_back("T2tt(425, 325)");    colors.push_back(kGreen + 2);
//  trees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/T2tt_500_325_tree_skimmed_baseline.root"));      names.push_back("T2tt(500, 325)");    colors.push_back(kRed);
//  trees.emplace_back(QCDSupport::getTree(QCDSupport::inputDir + "/T2tt_850_100_tree_skimmed_baseline.root"));      names.push_back("T2tt(850, 100)");    colors.push_back(kBlue);

  GetQCDTable(crinfo, srinfo);
  MakeCRPlots(crinfo, names, trees, colors);
  MakeSRPlots(srinfo, names, trees, colors);
//  MakeDPhiPlots(crinfo, names, trees, colors);
  MakeTFPlots(crinfo, srinfo);
}
