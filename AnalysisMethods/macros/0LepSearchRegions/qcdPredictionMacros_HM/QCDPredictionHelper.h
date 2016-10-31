#ifndef QCDPREDICTIONHELPER_H
#define QCDPREDICTIONHELPER_H
#include "TTree.h"
#include <iostream>
#include <regex>
#include <assert.h>
#include "AnalysisMethods/PlotUtils/interface/HistogramGetter.hh"

namespace QCDSupport {

TTree * getTree(TString filename){
  TFile * sf = new TFile(filename,"read");
  TTree * st =0;
  sf->GetObject("Events",st);
  return st;
}

void setTitleOffset(TCanvas *c, double xOff = .950, double yOff = 1.400){
  TList * list = c->GetListOfPrimitives();
  for(unsigned int iP = 0; iP < list->GetSize(); ++iP){
    TH1 * h = dynamic_cast<TH1*>(list->At(iP));
    if(h == 0) continue;
    h->GetXaxis()->SetTitleOffset(xOff);
    h->GetYaxis()->SetTitleOffset(yOff);
  }
}

  bool integrating_over_met = true;
  double integrating_met_systematic = 0;
  const TString inputDir = "../../../run/plots_16_09_28_Smear";
  const TString format = "pdf";
  const TString lumi = "12.9";
  const TString header = "#sqrt{s} = 13 TeV, L = 12.9 fb^{-1}";
  const TString eventsToSkip = "!(run == 1 && lumi == 91626 && event == 208129617)";
  const TString METPresel = "passjson && passmetmht100 && (passmetfilters || process==10) && (met >= 250) && (j1chEnFrac > 0.1) && (j1chEnFrac < 0.99)";
  const TString METPresel_forRespTailSF = "passjson && passmetmht100 && passmetfilters && (met >= 250) && (j1chEnFrac > 0.1) && (j1chEnFrac < 0.99)";
  const TString ResTailExtraCuts = "(((dphij1met < .1) || (dphij2met < .1)) && (nvetolep == 0) && (pseudoRespPassFilter == 1))";
  const TString BaselineExtraCuts = "((njets >= 5) && (nlbjets>= 2) && (nbjets>=1))";
  const TString METBaseline = "passjson && passmetmht100 && (passmetfilters || process==10) && (met >= 250) && (j1chEnFrac > 0.1) && (j1chEnFrac < 0.99) && (njets >= 5) && (nlbjets>= 2) && (nbjets>=1)";
  const TString CR_def = "((dphij1met < .1) || (dphij2met < .1) || (dphij3met < .1))";
  const TString SR_def = "((dphij1met >= 0.5) && (dphij2met>=0.5) && (dphij3met>=0.5) && (dphij4met>=0.5))";
  const TString stdWeight = "1.0";
  const TString stdMCWeight  = lumi + "*weight*truePUWeight*btagWeight";
  const TString stdQCDWeight = lumi + "*weight*truePUWeight*btagWeight*qcdRespTailWeight";
  const TString stdMCWeightNonQCD = stdMCWeight + "* 0.5205";
  const TString topMCWeight = "cttWeight"; //FIXME

  const TString mtb_lt_175 = "(mtcsv12met<175)";               const TString mtb_lt_175_R = "M_{T}(b_{1,2},#slash{E}_{T}) < 175";        const TString mtb_lt_175_L = "\\mtb < 175";    
  const TString mtb_ge_175 = "(mtcsv12met>=175)";              const TString mtb_ge_175_R = "M_{T}(b_{1,2},#slash{E}_{T}) #geq 175";     const TString mtb_ge_175_L = "\\mtb \\geq 175";
  const TString nbInc      = "(nbjets>=1)";                    const TString nbInc_R      = "N_{b} #geq 1";                              const TString nbInc_L      = "\\nb \\geq 1";
  const TString nb1        = "(nbjets==1)";                    const TString nb1_R        = "N_{b} = 1";                                 const TString nb1_L        = "\\nb = 1";
  const TString nb2        = "(nbjets>=2)";                    const TString nb2_R        = "N_{b} #geq 2";                              const TString nb2_L        = "\\nb \\geq 2";
  const TString int_nj     = "((njets>=5) && (njets<=6))";     const TString int_nj_R     = "5 #leq N_{j} #leq 6";                       const TString int_nj_L     = "5 \\leq \\njet \\leq 6";
  const TString med_nj     = "(njets>=5)";                     const TString med_nj_R     = "N_{j} #geq 5";                              const TString med_nj_L     = "\\njet \\geq 5";
  const TString hgh_nj     = "(njets>=7)";                     const TString hgh_nj_R     = "N_{j} #geq 7";                              const TString hgh_nj_L     = "\\njet \\geq 7";
  const TString nt0        = "(nsdtoploose==0)";               const TString nt0_R        = "N_{t} = 0";                                 const TString nt0_L        = "\\nt = 0";
  const TString nt1        = "(nsdtoploose>=1)";               const TString nt1_R        = "N_{t} #geq 1";                              const TString nt1_L        = "\\nt \\geq 1";
  const TString nw0        = "(nsdwloose==0)";                 const TString nw0_R        = "N_{W} = 0";                                 const TString nw0_L        = "\\nW = 0";
  const TString nw1        = "(nsdwloose>=1)";                 const TString nw1_R        = "N_{W} #geq 1";                              const TString nw1_L        = "\\nW \\geq 1";

  TString processWeight(TString weight, int iCR) {return weight;}

  struct CRegInfo {
    static const int nCR = 7;
    enum CRReg{ CR_nbINC_mtb0_nj5, CR_nbINC_mtb0_nj7, CR_nbINC_mtb175_nj5_nt0_nw0, CR_nbINC_mtb175_nj7_nt0_nw0, CR_nbINC_mtb175_nj5t_nt0_nw1t, CR_nbINC_mtb175_nj5t_nt1t_nw0, CR_nbINC_mtb175_nj5t_nt1t_nw1t };
    TString crRegBinNames[nCR] = { "nbINC_mtb0_nj5", "nbINC_mtb0_nj7", "nbINC_mtb175_nj5_nt0_nw0", "nbINC_mtb175_nj7_nt0_nw0", "nbINC_mtb175_nj5t_nt0_nw1t", "nbINC_mtb175_nj5t_nt1t_nw0", "nbINC_mtb175_nj5t_nt1t_nw1t" };
    TString crSel[nCR] = {
      mtb_lt_175 + " && " + nbInc + " && " + int_nj,
      mtb_lt_175 + " && " + nbInc + " && " + hgh_nj,
      mtb_ge_175 + " && " + nbInc + " && " + int_nj + " && " + nt0 + " && " + nw0,
      mtb_ge_175 + " && " + nbInc + " && " + hgh_nj + " && " + nt0 + " && " + nw0,
      mtb_ge_175 + " && " + nbInc + " && " + med_nj + " && " + nt0 + " && " + nw1,
      mtb_ge_175 + " && " + nbInc + " && " + med_nj + " && " + nt1 + " && " + nw0,
      mtb_ge_175 + " && " + nbInc + " && " + med_nj + " && " + nt1 + " && " + nw1,
    };
    TString crSelLabels[nCR] = {
      "#splitline{" + mtb_lt_175_R + "}{" + nbInc_R + ", " + int_nj_R + "}",
      "#splitline{" + mtb_lt_175_R + "}{" + nbInc_R + ", " + hgh_nj_R + "}",
      "#splitline{" + mtb_ge_175_R + "}{" + nbInc_R + ", " + int_nj_R + ", " + nt0_R + ", " + nw0_R + "}",
      "#splitline{" + mtb_ge_175_R + "}{" + nbInc_R + ", " + hgh_nj_R + ", " + nt0_R + ", " + nw0_R + "}",
      "#splitline{" + mtb_ge_175_R + "}{" + nbInc_R + ", " + med_nj_R + ", " + nt0_R + ", " + nw1_R + "}",
      "#splitline{" + mtb_ge_175_R + "}{" + nbInc_R + ", " + med_nj_R + ", " + nt1_R + ", " + nw0_R + "}",
      "#splitline{" + mtb_ge_175_R + "}{" + nbInc_R + ", " + med_nj_R + ", " + nt1_R + ", " + nw1_R + "}",
    };
    TString crSelNamesLatex[nCR] = {
      mtb_lt_175_L + ", " + nbInc_L + ", " + int_nj_L,
      mtb_lt_175_L + ", " + nbInc_L + ", " + hgh_nj_L,
      mtb_ge_175_L + ", " + nbInc_L + ", " + int_nj_L + ", " + nt0_L + ", " + nw0_L,
      mtb_ge_175_L + ", " + nbInc_L + ", " + hgh_nj_L + ", " + nt0_L + ", " + nw0_L,
      mtb_ge_175_L + ", " + nbInc_L + ", " + med_nj_L + ", " + nt0_L + ", " + nw1_L,
      mtb_ge_175_L + ", " + nbInc_L + ", " + med_nj_L + ", " + nt1_L + ", " + nw0_L,
      mtb_ge_175_L + ", " + nbInc_L + ", " + med_nj_L + ", " + nt1_L + ", " + nw1_L,
    };
    TString crSelNames[nCR] = {
      mtb_lt_175_R + ", " + nbInc_R + ", " + int_nj_R,
      mtb_lt_175_R + ", " + nbInc_R + ", " + hgh_nj_R,
      mtb_ge_175_R + ", " + nbInc_R + ", " + int_nj_R + ", " + nt0_R + ", " + nw0_R,
      mtb_ge_175_R + ", " + nbInc_R + ", " + hgh_nj_R + ", " + nt0_R + ", " + nw0_R,
      mtb_ge_175_R + ", " + nbInc_R + ", " + med_nj_R + ", " + nt0_R + ", " + nw1_R,
      mtb_ge_175_R + ", " + nbInc_R + ", " + med_nj_R + ", " + nt1_R + ", " + nw0_R,
      mtb_ge_175_R + ", " + nbInc_R + ", " + med_nj_R + ", " + nt1_R + ", " + nw1_R,
    };
    int nMETBins[nCR] = { 4, 4, 4, 4, 5, 5, 4 };
    double* metBins[nCR] = {
        new double[nMETBins[0] + 1] { 250, 300, 400, 500, 1000 },
        new double[nMETBins[1] + 1] { 250, 300, 400, 500, 1000 },
        new double[nMETBins[2] + 1] { 250, 350, 450, 550, 1000 },
        new double[nMETBins[3] + 1] { 250, 350, 450, 550, 1000 },
        new double[nMETBins[4] + 1] { 250, 350, 450, 550, 650, 1000 },
        new double[nMETBins[5] + 1] { 250, 350, 450, 550, 650, 1000 },
        new double[nMETBins[6] + 1] { 250, 300, 400, 500, 1000 },
    };
    HistogramGetter* CRmetGetters[nCR] = {
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[0], metBins[0]),
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[1], metBins[1]),
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[2], metBins[2]),
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[3], metBins[3]),
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[4], metBins[4]),
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[5], metBins[5]),
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[6], metBins[6]),
    };

    TString crPreData        =  TString::Format("%s && %s && nvetotau == 0 && nvetolep == 0 && %s", METPresel.Data(), BaselineExtraCuts.Data(), CR_def.Data());
    TString crPreOther       =  TString::Format("%s && %s && nvetolep==0 && (nvetotau==0 || npromptgentau>0) && %s", METPresel.Data(), BaselineExtraCuts.Data(), CR_def.Data());
    TString crPreQCD         =  TString::Format("%s && %s && %s", METPresel.Data(), BaselineExtraCuts.Data(), CR_def.Data());
    TString crPreQCDWithVeto =  TString::Format("%s && %s && nvetotau == 0 && nvetolep == 0 && %s", METPresel.Data(), BaselineExtraCuts.Data(), CR_def.Data());

    TString crWgtOther       =  TString::Format("%s*leptnpweightHM*lepvetoweightHM", stdMCWeight.Data());

    TString normSel          = TString::Format("%s && %s && %s && nvetolep > 0 && mtlepmet<100", METPresel.Data(), BaselineExtraCuts.Data(), SR_def.Data());
    TString normMCWT         = TString::Format("%s*leptnpweightHM", stdMCWeight.Data());

    std::vector<TH1*> ttBarW_BKGSFs;
    TTree *remaining_BKGTree;
    TTree *ttBarW_BKGTree;
    TTree *dataTree;
    TTree *qcdTree;
    TTree *origQcdTree;
    void fillOtherBkgSFs(TString dataTreeIn, TString ttBarW_BKGTreeIn, TString remaining_BKGTreeIn, TString qcdTreeIn, TString origTreeIn){
      fillOtherBkgSFs(getTree(dataTreeIn),getTree(ttBarW_BKGTreeIn),getTree(remaining_BKGTreeIn),getTree(qcdTreeIn),getTree(origTreeIn));
    }
    void fillOtherBkgSFs(TTree * dataTreeIn, TTree * ttBarW_BKGTreeIn, TTree * remaining_BKGTreeIn, TTree * qcdTreeIn, TTree * origTreeIn){
      remaining_BKGTree = remaining_BKGTreeIn;
      ttBarW_BKGTree = ttBarW_BKGTreeIn;
      dataTree = dataTreeIn;
      qcdTree = qcdTreeIn;
      origQcdTree = origTreeIn;

      double bins2[] = {250,300,400,700};
      HistogramGetter SFHG(   "SFmet", "met","#slash{#it{E}}_{T} [GeV]", 3, bins2);
      HistogramGetter SFIncHG("SFmet2","met","#slash{#it{E}}_{T} [GeV]", 1, 250, 1000);

      cout << "Determining ttBarW Scale Factors" << endl;
      cout << "\\begin{table}[!!htbp]" << endl;
      cout << "  \\begin{center}" << endl;
      cout << "    \\resizebox*{\\textwidth}{!}{" << endl;
      cout << "      \\begin{tabular}{|c|c|}" << endl;
      cout << "        \\hline" << endl;
      cout << "        Region & Scale Factor \\\\" << endl;
      cout << "        \\hline" << endl;
      for(unsigned int iS = 0; iS < nCR; ++iS){
        TString sel = TString::Format("%s && %s && met >= %.1f", normSel.Data(), crSel[iS].Data(), metBins[iS][0]);
        TH1F * dataH   = SFIncHG/*(iS == 4?SFIncHG:SFHG)*/ .getHistogram(dataTree,sel,stdWeight,"Data");
        TH1F * ttBarWH = SFIncHG/*(iS == 4?SFIncHG:SFHG)*/ .getHistogram(ttBarW_BKGTree,sel,processWeight(normMCWT,iS),"Other");
        TH1F * qcdH    = SFIncHG/*(iS == 4?SFIncHG:SFHG)*/ .getHistogram(qcdTree,sel,processWeight(normMCWT,iS),"MC");
        ttBarWH->Add(qcdH);
        dataH->Divide(ttBarWH);
        TH1 * h = new TH1F(TString::Format("ttbarwSF_%u",iS),";SF",nMETBins[iS],metBins[iS]);
        h->Sumw2();
        for(unsigned int iB = 1; iB <= h->GetNbinsX(); ++iB){
          double metV = h->GetBinCenter(iB);
          double sf   = dataH->GetBinContent( dataH->FindFixBin(metV) );
          double sf_E = dataH->GetBinError(   dataH->FindFixBin(metV) );
          h->SetBinContent(iB,sf);
          h->SetBinError(  iB,sf_E);
        }
        cout << "        $" << crSelNamesLatex[iS] << "$ & " << h->GetBinContent(1) << " $\\pm$ " << h->GetBinError(1) << " \\\\ \\hline" << endl;
        ttBarW_BKGSFs.push_back(h);
      }
      cout << "      \\end{tabular}" << endl;
      cout << "    }" << endl;
      cout << "  \\end{center}" << endl;
      cout << "\\end{table}" << endl;
      cout << endl;
    }

    std::vector<TH1*> otherBKGScaledYields;
    void fillCorrectedOtherBkgYields(){
      for(unsigned int iS = 0; iS < nCR; ++iS){
        TString sel = TString::Format("%s && %s",crPreOther.Data(),crSel[iS].Data());
        TH1 * remaining_H = CRmetGetters[iS]->getHistogram(remaining_BKGTree,sel,processWeight(crWgtOther,iS),TString::Format("tree_remaining_%u",iS));
        TH1 * ttBarW_H    = CRmetGetters[iS]->getHistogram(ttBarW_BKGTree,   sel,processWeight(crWgtOther,iS),TString::Format("tree_ttBarW_%u",   iS));
        for(unsigned int iB =1; iB <=ttBarW_H->GetNbinsX(); ++iB){
          ttBarW_H->SetBinContent(iB,ttBarW_H->GetBinContent(iB) *ttBarW_BKGSFs[iS]->GetBinContent(iB) );
          ttBarW_H->SetBinError(iB,ttBarW_H->GetBinError(iB) *ttBarW_BKGSFs[iS]->GetBinContent(iB) );
        }
        TH1 * otherBKG_H = (TH1*)ttBarW_H->Clone();
        otherBKG_H->Add(remaining_H);
        otherBKGScaledYields.push_back(otherBKG_H);
      }
    }

    std::vector<TH1*> dataYields;
    std::vector<TH1*> qcdYieldsWithVeto;
    std::vector<TH1*> otherBKGCorr;
    std::vector<TH1*> otherBKGCorrUnc;
    std::vector<TH1*> origQCDYields;
    std::vector<TH1*> smearedQCDYields;
    std::vector<TH1*> dataYields_noInt;
    std::vector<TH1*> qcdYieldsWithVeto_noInt;
    std::vector<TH1*> otherBKGScaledYields_noInt;
    std::vector<TH1*> origQCDYields_noInt;
    std::vector<TH1*> smearedQCDYields_noInt;
    void fillDataCorr(){
      for(unsigned int iS = 0; iS < nCR; ++iS){
        TH1 * hd      = CRmetGetters[iS]->getHistogram(dataTree,TString::Format("%s && %s",crPreData.Data(),crSel[iS].Data()),stdWeight,TString::Format("datatree_%u",iS));
        TH1 * origQCD = CRmetGetters[iS]->getHistogram(origQcdTree,TString::Format("%s && %s",crPreQCD.Data(),crSel[iS].Data()),processWeight(stdQCDWeight,iS),TString::Format("origQCDtree_%u",   iS));
        CRmetGetters[iS]->setNBS(50);
        TH1 * hqwv       = CRmetGetters[iS]->getHistogramManual(qcdTree,TString::Format("%s && %s",crPreQCDWithVeto.Data(),crSel[iS].Data()),processWeight(stdQCDWeight,iS),TString::Format("qcdWithVetotree_%u",iS));
        TH1 * smearedQCD = CRmetGetters[iS]->getHistogramManual(qcdTree,TString::Format("%s && %s",crPreQCD.Data(),crSel[iS].Data()),processWeight(stdQCDWeight,iS),TString::Format("smearedQCDtree_%u",iS));
        CRmetGetters[iS]->setNBS(0);
        TH1 * hd_Int         = (TH1*)        hd->Clone();
        TH1 * origQCD_Int    = (TH1*)   origQCD->Clone();
        TH1 * hqwv_Int       = (TH1*)      hqwv->Clone();
        TH1 * smearedQCD_Int = (TH1*)smearedQCD->Clone();
        TH1 * oBKGSY_Int     = (TH1*)otherBKGScaledYields[iS]->Clone();
        TH1 * corr  = (TH1*)hd->Clone();
        TH1 * corrU = (TH1*)hd->Clone();
        if(integrating_over_met){
          int minBin = 0;
          int nBins = 2;
          double* tempMetBins = new double[nBins] { metBins[iS][0], metBins[iS][nMETBins[iS]] };
          if( (iS == CR_nbINC_mtb175_nj5t_nt0_nw1t) || (iS == CR_nbINC_mtb175_nj5t_nt1t_nw0) ){
            minBin = nMETBins[iS] - 1;
            nBins  = minBin + 1;
            tempMetBins = new double[nBins];
            for(unsigned int iM = 0; iM < minBin; iM++){
              tempMetBins[iM] = metBins[iS][iM];
            }
            tempMetBins[minBin] = metBins[iS][nMETBins[iS]];
          }
for(unsigned int iM = 0; iM < nBins; iM++){
  cout << tempMetBins[iM] << ", ";
} cout << endl;
          HistogramGetter* tempHG = new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nBins - 1, tempMetBins);
          TH1 * hd_temp      = tempHG->getHistogram(dataTree,TString::Format("%s && %s",crPreData.Data(),crSel[iS].Data()),stdWeight,TString::Format("datatree_temp_%u",iS));
          TH1 * origQCD_temp = tempHG->getHistogram(origQcdTree,TString::Format("%s && %s",crPreQCD.Data(),crSel[iS].Data()),processWeight(stdQCDWeight,iS),TString::Format("origQCDtree_temp_%u",iS));
          tempHG->setNBS(50);
          TH1 * hqwv_temp       = tempHG->getHistogramManual(qcdTree,TString::Format("%s && %s",crPreQCDWithVeto.Data(),crSel[iS].Data()),processWeight(stdQCDWeight,iS),TString::Format("qcdWithVetotree_temp_%u",iS));
          TH1 * smearedQCD_temp = tempHG->getHistogramManual(qcdTree,TString::Format("%s && %s",crPreQCD.Data(),crSel[iS].Data()),processWeight(stdQCDWeight,iS),TString::Format("smearedQCDtree_temp_%u",iS));
          tempHG->setNBS(0);

          double otherBKGScaledYields_Int_E = 0;
          double otherBKGScaledYields_Int = otherBKGScaledYields[iS]->IntegralAndError(otherBKGScaledYields[iS]->FindFixBin(metBins[iS][minBin]), otherBKGScaledYields[iS]->GetNbinsX() + 1, otherBKGScaledYields_Int_E);
cout << otherBKGScaledYields[iS]->FindFixBin(metBins[iS][minBin]) << ": " << otherBKGScaledYields_Int << endl;
          for(unsigned int iB = minBin; iB <= hd_Int->GetNbinsX(); ++iB){
            double metX = hd_Int->GetBinCenter(iB);
                    hd_Int->SetBinContent(iB,         hd_temp->GetBinContent(        hd_temp->FindFixBin(metX)));
                  hqwv_Int->SetBinContent(iB,       hqwv_temp->GetBinContent(      hqwv_temp->FindFixBin(metX)));
               origQCD_Int->SetBinContent(iB,    origQCD_temp->GetBinContent(   origQCD_temp->FindFixBin(metX)));
            smearedQCD_Int->SetBinContent(iB, smearedQCD_temp->GetBinContent(smearedQCD_temp->FindFixBin(metX)));
                    hd_Int->SetBinError  (iB,         hd_temp->GetBinError  (        hd_temp->FindFixBin(metX)));
                  hqwv_Int->SetBinError  (iB,       hqwv_temp->GetBinError  (      hqwv_temp->FindFixBin(metX)));
               origQCD_Int->SetBinError  (iB,    origQCD_temp->GetBinError  (   origQCD_temp->FindFixBin(metX)));
            smearedQCD_Int->SetBinError  (iB, smearedQCD_temp->GetBinError  (smearedQCD_temp->FindFixBin(metX)));
                oBKGSY_Int->SetBinContent(iB, otherBKGScaledYields_Int);
                oBKGSY_Int->SetBinError  (iB, otherBKGScaledYields_Int_E);
          }
          if( (iS == CR_nbINC_mtb175_nj5t_nt0_nw1t)  || (iS == CR_nbINC_mtb175_nj5t_nt1t_nw0) || (iS == CR_nbINC_mtb175_nj5t_nt1t_nw1t) ){
            dataYields_noInt.push_back(hd);
            qcdYieldsWithVeto_noInt.push_back(hqwv);
            origQCDYields_noInt.push_back(origQCD);
            smearedQCDYields_noInt.push_back(smearedQCD);
            otherBKGScaledYields_noInt.push_back(otherBKGScaledYields[iS]);

            dataYields.push_back(hd_Int);
            qcdYieldsWithVeto.push_back(hqwv_Int);
            origQCDYields.push_back(origQCD_Int);
            smearedQCDYields.push_back(smearedQCD_Int);
            otherBKGScaledYields[iS] = oBKGSY_Int;
          } else {
            dataYields.push_back(hd);
            qcdYieldsWithVeto.push_back(hqwv);
            origQCDYields.push_back(origQCD);
            smearedQCDYields.push_back(smearedQCD);

            dataYields_noInt.push_back(hd_Int);
            qcdYieldsWithVeto_noInt.push_back(hqwv_Int);
            origQCDYields_noInt.push_back(origQCD_Int);
            smearedQCDYields_noInt.push_back(smearedQCD_Int);
            otherBKGScaledYields_noInt.push_back(oBKGSY_Int);

            if( (iS == CR_nbINC_mtb175_nj5_nt0_nw0) || (iS == CR_nbINC_mtb175_nj7_nt0_nw0) ){
              double D_MC_ratio = hd_Int->GetBinContent(1) / (hqwv_Int->GetBinContent(1) + oBKGSY_Int->GetBinContent(1));
              for(unsigned int iB = 1; iB <= dataYields[iS]->GetNbinsX(); iB++){
                double temp_ratio = dataYields[iS]->GetBinContent(iB) / (qcdYieldsWithVeto[iS]->GetBinContent(iB) + otherBKGScaledYields[iS]->GetBinContent(iB));
                if(fabs(temp_ratio - D_MC_ratio) > integrating_met_systematic) integrating_met_systematic = fabs(temp_ratio - D_MC_ratio);
              }
            }
          }
        } else {
          dataYields.push_back(hd);
          qcdYieldsWithVeto.push_back(hqwv);
          origQCDYields.push_back(origQCD);
          smearedQCDYields.push_back(smearedQCD);
        }
        std::cout << crSelNames[iS] << std::endl;
        for(unsigned int iB =1; iB <=dataYields[iS]->GetNbinsX(); ++iB){
          double dV = dataYields[iS]->GetBinContent(iB) < 10 ?  qcdYieldsWithVeto[iS]->GetBinContent(iB) + otherBKGScaledYields[iS]->GetBinContent(iB) : dataYields[iS]->GetBinContent(iB);
          double oV = otherBKGScaledYields[iS]->GetBinContent(iB);
          double cV = 1 - oV/dV;
          corr->SetBinContent(iB,cV );
          corrU->SetBinContent(iB,1 + oV/dV );
          std::cout << "("<<cV <<","<<1 + oV/dV  <<") ";
        }
        std::cout << std::endl;
        otherBKGCorr.push_back(corr);
        otherBKGCorrUnc.push_back(corrU);
      }
    }

    float getDataYield(CRReg reg, int metBin){ return dataYields[reg]->GetBinContent(metBin+1);}
    double getDataStatUnc(CRReg reg, int metBin) {
      double dY = getDataYield(reg,metBin);
      if(dY == 0) dY = 1;
      return TMath::Sqrt(dY)/dY + 1;
    };
    float getScaledOtherYield(CRReg reg, int metBin){ return otherBKGScaledYields[reg]->GetBinContent(metBin+1);}
    float getScaledOtherYieldStatUnc(CRReg reg, int metBin){ return otherBKGScaledYields[reg]->GetBinError(metBin+1);}
    float getDataYieldCorr(CRReg reg, int metBin){ return otherBKGCorr[reg]->GetBinContent(metBin+1);}
    float getDataYieldCorrUnc(CRReg reg, int metBin){ return otherBKGCorrUnc[reg]->GetBinContent(metBin+1);}
    float getOrigQCDYields_OOB           (CRReg reg, int metBin){ return    origQCDYields[reg]->GetBinContent(metBin+1);}
    float getOrigQCDYields_OOB_StatUnc   (CRReg reg, int metBin){ return    origQCDYields[reg]->GetBinError  (metBin+1);}
    float getSmearedQCDYields_OOB        (CRReg reg, int metBin){ return smearedQCDYields[reg]->GetBinContent(metBin+1);}
    float getSmearedQCDYields_OOB_StatUnc(CRReg reg, int metBin){ return smearedQCDYields[reg]->GetBinError  (metBin+1);}
    TString getBinName (CRReg reg, int metBin) {return  TString::Format("%.0f_%s",metBins[reg][metBin],crRegBinNames[reg].Data());}
    TString getLatexBlock(CRReg reg, int metBin){
      if(metBin+1 == nMETBins[reg])
        return TString::Format("$>$%.0f     & %.0f & %.2f $\\pm$ %.2f & %.2f $\\pm$ %.2f & %.2f $\\pm$ %.2f",metBins[reg][metBin],getDataYield(reg,metBin),getScaledOtherYield(reg,metBin),getScaledOtherYieldStatUnc(reg,metBin),
            getOrigQCDYields_OOB(reg,metBin),getOrigQCDYields_OOB_StatUnc(reg,metBin),getSmearedQCDYields_OOB(reg,metBin),getSmearedQCDYields_OOB_StatUnc(reg,metBin));
      else
        return TString::Format("%.0f$-$%.0f & %.0f & %.2f $\\pm$ %.2f & %.2f $\\pm$ %.2f & %.2f $\\pm$ %.2f",metBins[reg][metBin],metBins[reg][metBin+1],getDataYield(reg,metBin),getScaledOtherYield(reg,metBin),getScaledOtherYieldStatUnc(reg,metBin),
            getOrigQCDYields_OOB(reg,metBin),getOrigQCDYields_OOB_StatUnc(reg,metBin),getSmearedQCDYields_OOB(reg,metBin),getSmearedQCDYields_OOB_StatUnc(reg,metBin));
    }
  };

  struct SRegInfo {
    static const int nSR = 14;
    enum SRReg{ SR_nb1_mtb0_nj5, SR_nb1_mtb0_nj7, SR_nb2_mtb0_nj5, SR_nb2_mtb0_nj7, SR_nb1_mtb175_nj5_nt0_nw0, SR_nb1_mtb175_nj7_nt0_nw0, SR_nb1_mtb175_nj5t_nt0_nw1t, SR_nb1_mtb175_nj5t_nt1t_nw0, SR_nb1_mtb175_nj5t_nt1t_nw1t, SR_nb2_mtb175_nj5_nt0_nw0, SR_nb2_mtb175_nj7_nt0_nw0, SR_nb2_mtb175_nj5t_nt0_nw1t, SR_nb2_mtb175_nj5t_nt1t_nw0, SR_nb2_mtb175_nj5t_nt1t_nw1t };
    TString srRegBinNames[nSR] = { "nb1_mtb0_nj5", "nb1_mtb0_nj7", "nb2_mtb0_nj5", "nb2_mtb0_nj7", "nb1_mtb175_nj5_nt0_nw0", "nb1_mtb175_nj7_nt0_nw0", "nb1_mtb175_nj5t_nt0_nw1t", "nb1_mtb175_nj5t_nt1t_nw0", "nb1_mtb175_nj5t_nt1t_nw1t", "nb2_mtb175_nj5_nt0_nw0", "nb2_mtb175_nj7_nt0_nw0", "nb2_mtb175_nj5t_nt0_nw1t", "nb2_mtb175_nj5t_nt1t_nw0", "nb2_mtb175_nj5t_nt1t_nw1t" };
    TString srSel[nSR] = {
      mtb_lt_175 + " && " + nb1 + " && " + int_nj,
      mtb_lt_175 + " && " + nb1 + " && " + hgh_nj,
      mtb_lt_175 + " && " + nb2 + " && " + int_nj,
      mtb_lt_175 + " && " + nb2 + " && " + hgh_nj,
      mtb_ge_175 + " && " + nb1 + " && " + int_nj + " && " + nt0 + " && " + nw0,
      mtb_ge_175 + " && " + nb1 + " && " + hgh_nj + " && " + nt0 + " && " + nw0,
      mtb_ge_175 + " && " + nb1 + " && " + med_nj + " && " + nt0 + " && " + nw1,
      mtb_ge_175 + " && " + nb1 + " && " + med_nj + " && " + nt1 + " && " + nw0,
      mtb_ge_175 + " && " + nb1 + " && " + med_nj + " && " + nt1 + " && " + nw1,
      mtb_ge_175 + " && " + nb2 + " && " + int_nj + " && " + nt0 + " && " + nw0,
      mtb_ge_175 + " && " + nb2 + " && " + hgh_nj + " && " + nt0 + " && " + nw0,
      mtb_ge_175 + " && " + nb2 + " && " + med_nj + " && " + nt0 + " && " + nw1,
      mtb_ge_175 + " && " + nb2 + " && " + med_nj + " && " + nt1 + " && " + nw0,
      mtb_ge_175 + " && " + nb2 + " && " + med_nj + " && " + nt1 + " && " + nw1,
    };
    TString srSelLabels[nSR] = {
      "#splitline{" + mtb_lt_175_R + "}{" + nb1_R + ", " + int_nj_R + "}",
      "#splitline{" + mtb_lt_175_R + "}{" + nb1_R + ", " + hgh_nj_R + "}",
      "#splitline{" + mtb_lt_175_R + "}{" + nb2_R + ", " + int_nj_R + "}",
      "#splitline{" + mtb_lt_175_R + "}{" + nb2_R + ", " + hgh_nj_R + "}",
      "#splitline{" + mtb_ge_175_R + "}{" + nb1_R + ", " + int_nj_R + ", " + nt0_R + ", " + nw0_R + "}",
      "#splitline{" + mtb_ge_175_R + "}{" + nb1_R + ", " + hgh_nj_R + ", " + nt0_R + ", " + nw0_R + "}",
      "#splitline{" + mtb_ge_175_R + "}{" + nb1_R + ", " + med_nj_R + ", " + nt0_R + ", " + nw1_R + "}",
      "#splitline{" + mtb_ge_175_R + "}{" + nb1_R + ", " + med_nj_R + ", " + nt1_R + ", " + nw0_R + "}",
      "#splitline{" + mtb_ge_175_R + "}{" + nb1_R + ", " + med_nj_R + ", " + nt1_R + ", " + nw1_R + "}",
      "#splitline{" + mtb_ge_175_R + "}{" + nb2_R + ", " + int_nj_R + ", " + nt0_R + ", " + nw0_R + "}",
      "#splitline{" + mtb_ge_175_R + "}{" + nb2_R + ", " + hgh_nj_R + ", " + nt0_R + ", " + nw0_R + "}",
      "#splitline{" + mtb_ge_175_R + "}{" + nb2_R + ", " + med_nj_R + ", " + nt0_R + ", " + nw1_R + "}",
      "#splitline{" + mtb_ge_175_R + "}{" + nb2_R + ", " + med_nj_R + ", " + nt1_R + ", " + nw0_R + "}",
      "#splitline{" + mtb_ge_175_R + "}{" + nb2_R + ", " + med_nj_R + ", " + nt1_R + ", " + nw1_R + "}",
    };
    TString srSelNamesLatex[nSR] = {
      mtb_lt_175_L + ", " + nb1_L + ", " + int_nj_L,
      mtb_lt_175_L + ", " + nb1_L + ", " + hgh_nj_L,
      mtb_lt_175_L + ", " + nb2_L + ", " + int_nj_L,
      mtb_lt_175_L + ", " + nb2_L + ", " + hgh_nj_L,
      mtb_ge_175_L + ", " + nb1_L + ", " + int_nj_L + ", " + nt0_L + ", " + nw0_L,
      mtb_ge_175_L + ", " + nb1_L + ", " + hgh_nj_L + ", " + nt0_L + ", " + nw0_L,
      mtb_ge_175_L + ", " + nb1_L + ", " + med_nj_L + ", " + nt0_L + ", " + nw1_L,
      mtb_ge_175_L + ", " + nb1_L + ", " + med_nj_L + ", " + nt1_L + ", " + nw0_L,
      mtb_ge_175_L + ", " + nb1_L + ", " + med_nj_L + ", " + nt1_L + ", " + nw1_L,
      mtb_ge_175_L + ", " + nb2_L + ", " + int_nj_L + ", " + nt0_L + ", " + nw0_L,
      mtb_ge_175_L + ", " + nb2_L + ", " + hgh_nj_L + ", " + nt0_L + ", " + nw0_L,
      mtb_ge_175_L + ", " + nb2_L + ", " + med_nj_L + ", " + nt0_L + ", " + nw1_L,
      mtb_ge_175_L + ", " + nb2_L + ", " + med_nj_L + ", " + nt1_L + ", " + nw0_L,
      mtb_ge_175_L + ", " + nb2_L + ", " + med_nj_L + ", " + nt1_L + ", " + nw1_L,
    };
    TString srSelNames[nSR] = {
      mtb_lt_175_R + ", " + nb1_R + ", " + int_nj_R,
      mtb_lt_175_R + ", " + nb1_R + ", " + hgh_nj_R,
      mtb_lt_175_R + ", " + nb2_R + ", " + int_nj_R,
      mtb_lt_175_R + ", " + nb2_R + ", " + hgh_nj_R,
      mtb_ge_175_R + ", " + nb1_R + ", " + int_nj_R + ", " + nt0_R + ", " + nw0_R,
      mtb_ge_175_R + ", " + nb1_R + ", " + hgh_nj_R + ", " + nt0_R + ", " + nw0_R,
      mtb_ge_175_R + ", " + nb1_R + ", " + med_nj_R + ", " + nt0_R + ", " + nw1_R,
      mtb_ge_175_R + ", " + nb1_R + ", " + med_nj_R + ", " + nt1_R + ", " + nw0_R,
      mtb_ge_175_R + ", " + nb1_R + ", " + med_nj_R + ", " + nt1_R + ", " + nw1_R,
      mtb_ge_175_R + ", " + nb2_R + ", " + int_nj_R + ", " + nt0_R + ", " + nw0_R,
      mtb_ge_175_R + ", " + nb2_R + ", " + hgh_nj_R + ", " + nt0_R + ", " + nw0_R,
      mtb_ge_175_R + ", " + nb2_R + ", " + med_nj_R + ", " + nt0_R + ", " + nw1_R,
      mtb_ge_175_R + ", " + nb2_R + ", " + med_nj_R + ", " + nt1_R + ", " + nw0_R,
      mtb_ge_175_R + ", " + nb2_R + ", " + med_nj_R + ", " + nt1_R + ", " + nw1_R,
    };
    int nMETBins[nSR] = { 4, 4, 4, 4, 4, 4, 5, 5, 4, 4, 4, 5, 5, 4 };
    double* metBins[nSR] = {
        new double[nMETBins[ 0] + 1] { 250, 300, 400, 500, 1000 },
        new double[nMETBins[ 1] + 1] { 250, 300, 400, 500, 1000 },
        new double[nMETBins[ 2] + 1] { 250, 300, 400, 500, 1000 },
        new double[nMETBins[ 3] + 1] { 250, 300, 400, 500, 1000 },
        new double[nMETBins[ 4] + 1] { 250, 350, 450, 550, 1000 },
        new double[nMETBins[ 5] + 1] { 250, 350, 450, 550, 1000 },
        new double[nMETBins[ 6] + 1] { 250, 350, 450, 550, 650, 1000 },
        new double[nMETBins[ 7] + 1] { 250, 350, 450, 550, 650, 1000 },
        new double[nMETBins[ 8] + 1] { 250, 300, 400, 500, 1000 },
        new double[nMETBins[ 9] + 1] { 250, 350, 450, 550, 1000 },
        new double[nMETBins[10] + 1] { 250, 350, 450, 550, 1000 },
        new double[nMETBins[11] + 1] { 250, 350, 450, 550, 650, 1000 },
        new double[nMETBins[12] + 1] { 250, 350, 450, 550, 650, 1000 },
        new double[nMETBins[13] + 1] { 250, 300, 400, 500, 1000 },
    };
    HistogramGetter* SRmetGetters[nSR] = {
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[ 0], metBins[ 0]),
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[ 1], metBins[ 1]),
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[ 2], metBins[ 2]),
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[ 3], metBins[ 3]),
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[ 4], metBins[ 4]),
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[ 5], metBins[ 5]),
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[ 6], metBins[ 6]),
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[ 7], metBins[ 7]),
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[ 8], metBins[ 8]),
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[ 9], metBins[ 9]),
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[10], metBins[10]),
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[11], metBins[11]),
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[12], metBins[12]),
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[13], metBins[13]),
    };
    CRegInfo::CRReg crRegs[nSR] = {
        CRegInfo::CR_nbINC_mtb0_nj5,
        CRegInfo::CR_nbINC_mtb0_nj7,
        CRegInfo::CR_nbINC_mtb0_nj5,
        CRegInfo::CR_nbINC_mtb0_nj7,
        CRegInfo::CR_nbINC_mtb175_nj5_nt0_nw0,
        CRegInfo::CR_nbINC_mtb175_nj7_nt0_nw0,
        CRegInfo::CR_nbINC_mtb175_nj5t_nt0_nw1t,
        CRegInfo::CR_nbINC_mtb175_nj5t_nt1t_nw0,
        CRegInfo::CR_nbINC_mtb175_nj5t_nt1t_nw1t,
        CRegInfo::CR_nbINC_mtb175_nj5_nt0_nw0,
        CRegInfo::CR_nbINC_mtb175_nj7_nt0_nw0,
        CRegInfo::CR_nbINC_mtb175_nj5t_nt0_nw1t,
        CRegInfo::CR_nbINC_mtb175_nj5t_nt1t_nw0,
        CRegInfo::CR_nbINC_mtb175_nj5t_nt1t_nw1t,
    };

    TString srPreQCD         =   TString::Format("%s && %s && %s", METPresel.Data(), BaselineExtraCuts.Data(), SR_def.Data());
    TString srPreQCDWithVeto = TString::Format("%s && %s && %s && (nvetolep==0) && (nvetotau==0)", METPresel.Data(), BaselineExtraCuts.Data(), SR_def.Data());

    TString processWeight(TString weight, int iSR) { return weight; }

    TTree *qcdTree;
    TTree *origQcdTree;

    std::vector<TH1F*> nominalQCD;
    std::vector<TH1F*> nominal_N;
    std::vector<TH1F*> nominal_D;
    std::vector<TH1F*> respUp;
    std::vector<TH1F*> respDown;
    std::vector<TH1F*> noResp;
    std::vector<TH1F*> origQCD_TF;
    std::vector<TH1F*> origQCDYields;
    std::vector<TH1F*> smearedQCDYields;
    void fillTFVariations( TString qcdTreeIn,TString origTreeIn){
      fillTFVariations(getTree(qcdTreeIn),getTree(origTreeIn));
    }
    void fillTFVariations(TTree* qcdTreeIn,TTree * origTreeIn){
      qcdTree = qcdTreeIn;
      origQcdTree = origTreeIn;
      CRegInfo crInfo;
      for(unsigned int iS = 0; iS < nSR; ++iS){
        TString sel  = TString::Format("%s && %s",srPreQCDWithVeto.Data(),srSel[iS].Data());
        TString selN = TString::Format("%s && %s",srPreQCD.Data(),srSel[iS].Data());
        TString selD = TString::Format("%s && %s",crInfo.crPreQCD.Data(),crInfo.crSel[crRegs[iS]].Data());

        TH1F * origQCD    = SRmetGetters[iS]->getHistogram(origQcdTree,selN,processWeight(stdQCDWeight,iS),TString::Format("origQCDYield_%u",   iS));
        SRmetGetters[iS]->setNBS(50);
        TH1F * smearedQCD = SRmetGetters[iS]->getHistogramManual(qcdTree,selN,processWeight(stdQCDWeight,iS),TString::Format("smearedQCDYield_%u",iS));
        TH1F * nomN       = SRmetGetters[iS]->getHistogramManual(qcdTree,selN,processWeight(stdQCDWeight,iS),TString::Format("nomN_%u",iS));
        TH1F * nomD       = SRmetGetters[iS]->getHistogramManual(qcdTree,selD,processWeight(stdQCDWeight,iS),TString::Format("nomD_%u",iS));
        TH1F * nom        = SRmetGetters[iS]->getTFAndCov(qcdTree,selN,processWeight(stdQCDWeight,iS),selD,processWeight(stdQCDWeight,iS),TString::Format("ratio_%u",iS));
        SRmetGetters[iS]->setNBS(0);

        TH1F * upN    = SRmetGetters[iS]->getHistogram(qcdTree,selN,TString::Format("%s*upTailWeight",processWeight(stdMCWeight,iS).Data()),TString::Format("QCD_%u",iS));
        TH1F * downN  = SRmetGetters[iS]->getHistogram(qcdTree,selN,TString::Format("%s*downTailWeight",processWeight(stdMCWeight,iS).Data()),TString::Format("QCD_%u",iS));
        TH1F * norN   = SRmetGetters[iS]->getHistogram(qcdTree,selN,processWeight(stdMCWeight,iS),TString::Format("QCD_%u",iS));
        TH1F * orN    = origQcdTree ? SRmetGetters[iS]->getHistogram(origQcdTree,selN,processWeight(stdMCWeight,iS),TString::Format("QCD_%u",iS)) : 0;

        TH1F * upD    = SRmetGetters[iS]->getHistogram(qcdTree,selD,TString::Format("%s*upTailWeight",processWeight(stdMCWeight,iS).Data()),TString::Format("QCD_%u",iS));
        TH1F * downD  = SRmetGetters[iS]->getHistogram(qcdTree,selD,TString::Format("%s*downTailWeight",processWeight(stdMCWeight,iS).Data()),TString::Format("QCD_%u",iS));
        TH1F * norD   = SRmetGetters[iS]->getHistogram(qcdTree,selD,processWeight(stdMCWeight,iS),TString::Format("QCD_%u",iS));
        TH1F * orD    = origQcdTree ? SRmetGetters[iS]->getHistogram(origQcdTree,selD,processWeight(stdMCWeight,iS),TString::Format("QCD_%u",iS)) : 0;

        if(integrating_over_met){
          int minBin = 0;
          int nBins = 2;
          double* tempMetBins = new double[nBins] { metBins[iS][0], metBins[iS][nMETBins[iS]] };
          if( (iS == SR_nb1_mtb175_nj5t_nt0_nw1t) || (iS == SR_nb2_mtb175_nj5t_nt0_nw1t) || (iS == SR_nb1_mtb175_nj5t_nt1t_nw0) || (iS == SR_nb2_mtb175_nj5t_nt1t_nw0) ){
            minBin = nMETBins[iS] - 1;
            nBins  = minBin + 1;
            tempMetBins = new double[nBins];
            for(unsigned int iM = 0; iM < minBin; iM++){
              tempMetBins[iM] = metBins[iS][iM];
            }
            tempMetBins[minBin] = metBins[iS][nMETBins[iS]];
          }
for(unsigned int iM = 0; iM < nBins; iM++){
  cout << tempMetBins[iM] << ", ";
} cout << endl;
          if( (iS == SR_nb1_mtb175_nj5t_nt0_nw1t)  || (iS == SR_nb2_mtb175_nj5t_nt0_nw1t) || (iS == SR_nb1_mtb175_nj5t_nt1t_nw0) || (iS == SR_nb2_mtb175_nj5t_nt1t_nw0) ||
              (iS == SR_nb1_mtb175_nj5t_nt1t_nw1t) || (iS == SR_nb2_mtb175_nj5t_nt1t_nw1t) ){
            HistogramGetter* tempHG = new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nBins - 1, tempMetBins);
            SRmetGetters[iS]->setNBS(50);
            tempHG->setNBS(50);
            TH1F * nom_temp   = SRmetGetters[iS]->getHistogramManual(qcdTree,selN,processWeight(stdQCDWeight,iS),TString::Format("ratio_%u",iS));
            TH1F * nomD_temp  =           tempHG->getHistogramManual(qcdTree,selD,processWeight(stdQCDWeight,iS),TString::Format("nomD_%u",iS));
            SRmetGetters[iS]->setNBS(0);
            tempHG->setNBS(0);
            TH1F * upD_temp   =           tempHG->getHistogramManual(qcdTree,selD,TString::Format("%s*upTailWeight",  processWeight(stdMCWeight,iS).Data()),TString::Format("QCD_%u",iS));
            TH1F * downD_temp =           tempHG->getHistogramManual(qcdTree,selD,TString::Format("%s*downTailWeight",processWeight(stdMCWeight,iS).Data()),TString::Format("QCD_%u",iS));
            TH1F * norD_temp  =           tempHG->getHistogramManual(qcdTree,selD,processWeight(stdMCWeight,iS),TString::Format("QCD_%u",iS));
            TH1F * orD_temp   = origQcdTree ? tempHG->getHistogramManual(origQcdTree,selD,processWeight(stdMCWeight,iS),TString::Format("QCD_%u",iS)) : 0;
            for(unsigned int iB = 1; iB <= nom->GetNbinsX(); ++iB){
              double metX = nom->GetBinCenter(iB);
                nom->SetBinContent(iB,       nomN->GetBinContent(iB));
               nomD->SetBinContent(iB,  nomD_temp->GetBinContent( nomD_temp->FindFixBin(metX)));
                upD->SetBinContent(iB,   upD_temp->GetBinContent(  upD_temp->FindFixBin(metX)));
              downD->SetBinContent(iB, downD_temp->GetBinContent(downD_temp->FindFixBin(metX)));
               norD->SetBinContent(iB,  norD_temp->GetBinContent( norD_temp->FindFixBin(metX)));
                orD->SetBinContent(iB,   orD_temp->GetBinContent(  orD_temp->FindFixBin(metX)));
                nom->SetBinError  (iB,       nomN->GetBinError  (iB));
               nomD->SetBinError  (iB,  nomD_temp->GetBinError  ( nomD_temp->FindFixBin(metX)));
                upD->SetBinError  (iB,   upD_temp->GetBinError  (  upD_temp->FindFixBin(metX)));
              downD->SetBinError  (iB, downD_temp->GetBinError  (downD_temp->FindFixBin(metX)));
               norD->SetBinError  (iB,  norD_temp->GetBinError  ( norD_temp->FindFixBin(metX)));
                orD->SetBinError  (iB,   orD_temp->GetBinError  (  orD_temp->FindFixBin(metX)));
            }
            nom->Divide(nomD);
          }
        }

        if(iS == SR_nb2_mtb175_nj5t_nt1t_nw1t){ //the last bin in this region had a zero transfer factor so am setting to the value of the previous bin
          cout << "resetting tf value!!!!!!!!!!!!!!!" << endl;
          cout << nom->GetBinContent(nom->GetNbinsX()) << " is being set to " << nom->GetBinContent(nom->GetNbinsX() - 1) << endl;
          nom->SetBinContent(nom->GetNbinsX(), nom->GetBinContent(nom->GetNbinsX() - 1));
          nom->SetBinError  (nom->GetNbinsX(), nom->GetBinError  (nom->GetNbinsX() - 1));
        }

        upN        ->Divide(upD);
        downN      ->Divide(downD);
        norN       ->Divide(norD);
        if(orN) orN->Divide(orD);

        origQCDYields.   push_back(origQCD);
        smearedQCDYields.push_back(smearedQCD);
        nominal_N.       push_back(nomN);
        nominal_D.       push_back(nomD);
        nominalQCD      .push_back(nom);
        respUp          .push_back(upN);
        respDown        .push_back(downN);
        noResp          .push_back(norN);
        origQCD_TF      .push_back(orN);
      }
    }

    std::string convertToTemplateName(std::string inBinName, int metV){
      std::regex e ("(\\d+)_(\\d+)_(\\d+)_(\\d+)");
      std::smatch sm;    // same as std::match_results<string::const_iterator> sm;
      bool matched = std::regex_match (inBinName,sm,e);

      assert(matched);
      assert(sm.size() == 5);

      return  "template_met" + std::to_string(metV)+"_njets"+sm[1].str() +"_nbjets"+sm[2].str()+"_ncttstd"+sm[3].str()+"_mtcsv12met"+sm[4].str()+".txt_sr_template";
    }

    void getQCDUncertainties(std::string fIn, std::vector<std::string>& uncNames,std::vector<float>& uncs){
      uncs.clear();
      uncNames.clear();

      std::string line;
      std::ifstream inFile (fIn);
      if(!inFile.is_open())std::cout <<"Could not find: " << fIn << std::endl;
      assert(inFile.is_open());
      std::regex e ("^(\\S+)\\s+lnN\\s+\\S+\\s+\\S+\\s+\\S+\\s+\\S+\\s+(\\S+)\\s*$");
      std::smatch sm;

      while(getline(inFile,line)){
        if (std::regex_match (line,sm,e)){
          assert(sm.size() == 3);
          bool isGood = true;
          float unc = -1;
          try {unc = stof(sm[2]);}catch (const std::invalid_argument& ia) {isGood = false;}
          if(TString(sm[1]).BeginsWith("qcd_")) isGood = false;
          if(isGood){
            uncs.push_back(unc);
            uncNames.push_back(sm[1]);
          }
        }
      }
    }

    double getQCDCardYields(std::string fIn){
      std::string line;
      std::ifstream inFile (fIn);
      if(!inFile.is_open())std::cout <<"Could not find: " << fIn << std::endl;
      assert(inFile.is_open());
      std::regex e ("^(\\S+)\\s+gmN\\s+(\\S+)\\s+\\S+\\s+\\S+\\s+\\S+\\s+\\S+\\s+(\\S+)\\s*$");
      std::smatch sm;

      while(getline(inFile,line)){
        if (std::regex_match (line,sm,e)){
          assert(sm.size() == 3);
          bool isGood = true;
          float data = -1;
          float tf = -1;
          try {data = stof(sm[2]);}catch (const std::invalid_argument& ia) {isGood = false;}
          try {tf = stof(sm[3]);}catch (const std::invalid_argument& ia) {isGood = false;}
          if(!TString(sm[1]).BeginsWith("qcd_")) isGood = false;
          if(isGood){
            return tf*data;
          }
        }
      }
      return -1;
    }

    std::vector<std::vector<double> > totalExtSystUncs = std::vector<std::vector<double> >(nSR,std::vector<double>(0,0));
    void loadExternUnc(std::string dcDir){
      std::cout << "Processing ext. uncs: "<< dcDir <<std::endl;
      int nUncs = -1;
      for(unsigned int iS = 0; iS < nSR; ++iS)
        for(unsigned int iM = 0; iM < nMETBins[iS]; ++iM){
          std::vector<float> uncs;
          std::vector<std::string> uncNames;
          getQCDUncertainties(dcDir+convertToTemplateName(srRegBinNames[iS].Data(),metBins[iS][iM]),uncNames,uncs); //
//          std::cout << getBinName(SRReg(iS),iM) << std::endl;
          for(unsigned int iU = 0; iU < uncs.size(); ++iU) std::cout << uncNames[iU] <<" -> "<< uncs[iU]<<std::endl;
          if(nUncs < 0){
            for(unsigned int iU = 0; iU < uncs.size(); ++iU) std::cout << uncNames[iU] <<" -> "<< uncs[iU]<<std::endl;
            nUncs = uncs.size();
          }
          assert(nUncs == uncs.size());
          double totalUnc =0;
          for(unsigned int iU = 0; iU < nUncs; ++iU){
            std::cout << fabs(uncs[iU]-1)*fabs(uncs[iU]-1) <<" ";
            totalUnc += fabs(uncs[iU]-1)*fabs(uncs[iU]-1);
          }
//          std::cout << std::endl << sqrt(totalUnc) + 1 <<std::endl;
          totalExtSystUncs[iS].push_back(sqrt(totalUnc) + 1);
        }
    }

    double getTF(SRReg reg, int metBin){return nominalQCD[reg]->GetBinContent(metBin+1);}
    double getTFStatUnc_N(SRReg reg, int metBin){ return std::min(2.0,nominal_N[reg]->GetBinError(metBin+1)/nominal_N[reg]->GetBinContent(metBin+1)+1);}
    double getTFStatUnc_D(SRReg reg, int metBin){ return std::min(2.0,nominal_D[reg]->GetBinError(metBin+1)/nominal_D[reg]->GetBinContent(metBin+1)+1);}
    double getTFStatUnc(SRReg reg, int metBin){return std::min(2.0,nominalQCD[reg]->GetBinError(metBin+1)/nominalQCD[reg]->GetBinContent(metBin+1)+1);}
    double getRespUnc(SRReg reg, int metBin){return (respUp[reg]->GetBinContent(metBin+1) - respDown[reg]->GetBinContent(metBin+1) ) /(2*getTF(reg,metBin)) +1;}
    double getPrediction(CRegInfo* crinfo, SRReg reg, int metBin){
      double dY = crinfo->getDataYield(crRegs[reg],metBin) == 0 ? 1: crinfo->getDataYield(crRegs[reg],metBin);
      return dY*crinfo->getDataYieldCorr(crRegs[reg],metBin)*getTF(reg,metBin);
    }
    double getExtUncs(SRReg reg, int metBin){ return totalExtSystUncs[reg][metBin];}
    TString getBinName (SRReg reg, int metBin) {return  TString::Format("%.0f_%s",metBins[reg][metBin],srRegBinNames[reg].Data());}
    double getTotalSystUnc(CRegInfo * crinfo,SRReg reg, int metBin){
      if(integrating_over_met){
        return TMath::Sqrt(   (getTFStatUnc(reg,metBin) -1)*(getTFStatUnc(reg,metBin) -1)
                            + (getRespUnc(reg,metBin) -1)*(getRespUnc(reg,metBin) -1)
                            + (getExtUncs(reg,metBin) -1)*(getExtUncs(reg,metBin) -1)
                            + (crinfo->getDataYieldCorrUnc(crRegs[reg],metBin)-1)*(crinfo->getDataYieldCorrUnc(crRegs[reg],metBin)-1)
                            + (integrating_met_systematic - 1) * (integrating_met_systematic - 1)
                            ) + 1;
      } else {
        return TMath::Sqrt(   (getTFStatUnc(reg,metBin) -1)*(getTFStatUnc(reg,metBin) -1)
                            + (getRespUnc(reg,metBin) -1)*(getRespUnc(reg,metBin) -1)
                            + (getExtUncs(reg,metBin) -1)*(getExtUncs(reg,metBin) -1)
                            + (crinfo->getDataYieldCorrUnc(crRegs[reg],metBin)-1)*(crinfo->getDataYieldCorrUnc(crRegs[reg],metBin)-1)
                            ) + 1;
      }
    }
    float getOrigQCDYields_OOB           (SRReg reg, int metBin){ return    origQCDYields[reg]->GetBinContent(metBin+1);}
    float getOrigQCDYields_OOB_StatUnc   (SRReg reg, int metBin){ return    origQCDYields[reg]->GetBinError  (metBin+1);}
    float getSmearedQCDYields_OOB        (SRReg reg, int metBin){ return smearedQCDYields[reg]->GetBinContent(metBin+1);}
    float getSmearedQCDYields_OOB_StatUnc(SRReg reg, int metBin){ return smearedQCDYields[reg]->GetBinError  (metBin+1);}

    TString getLatexBlock (CRegInfo* crinfo, SRReg reg, int metBin) {
      double tf       = getTF(reg,metBin);
      double tfUnc    = tf*(getTFStatUnc(reg,metBin) - 1);
      double pred     = getPrediction(crinfo,reg,metBin);
      double predstat = pred*(crinfo->getDataStatUnc(crRegs[reg],metBin) - 1);
      double predsyst = pred*(getTotalSystUnc(crinfo,reg,metBin) - 1);

//      if(tf < .0005 ) return TString::Format(" $<$0.001 & - ");
      return TString::Format("%.3f $\\pm$ %.3f & %.3f $\\pm$ %.3f & %.3f $\\pm$ %.3f & %.3f $\\pm$ %.3f (stat.) $\\pm$ %.3f (syst.)",getOrigQCDYields_OOB(reg,metBin),getOrigQCDYields_OOB_StatUnc(reg,metBin),getSmearedQCDYields_OOB(reg,metBin),getSmearedQCDYields_OOB_StatUnc(reg,metBin),tf,tfUnc,pred,predstat,predsyst );

    }
  };
};

#endif
