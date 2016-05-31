#ifndef QCDPREDICTIONHELPER_H
#define QCDPREDICTIONHELPER_H
#include "TTree.h"
#include<iostream>
#include<regex>
#include<assert.h>
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

  const TString inputDir = "skimmed_trees/";
  const TString format = "pdf";
  const TString lumi = "2.317";
  const TString header = "#sqrt{s} = 13 TeV, L = 2.3 fb^{-1}";
  const TString eventsToSkip = "(event != 45393342 && event != 197488999 && event != 166503616 && event != 56160931)";
//  const TString METPresel = eventsToSkip + " && " + "passjson && passcscbeamhaloflt && passeebadscflt && passeebadsc4flt && passhbheisoflt && passhbhefltloose && (met >= 250)";
  const TString METPresel = "passjson && passcscbeamhaloflt && passeebadscflt && passeebadsc4flt && passhbheisoflt && passhbhefltloose && (met >= 250)";
  const TString ResTailExtraCuts = "(((dphij1met < .1) || (dphij2met < .1)) && (nvetolep == 0) && (pseudoRespPassFilter == 1))";
  const TString BaselineExtraCuts = "(passmetmht90 && (njl>=1) && (njets>=2) && (dphij1lmet>2.) && (j1lpt>=250) && (met/sqrt(ht)>=10))";
  const TString CR_def = "((dphij1met < .1) || (dphij2met < .1) || (dphij3met < .1))";
  const TString SR_def = "((dphij1met >= 0.5) && (dphij2met>=0.15) && (dphij3met>=0.15))";
  const TString stdWeight = "1.0";
  const TString stdMCWeight  = lumi + "*weight*truePUWeight*btagWeight";
  const TString stdQCDWeight = lumi + "*weight*truePUWeight*btagWeight*qcdRespTailWeight";

  const TString mtb       = "(mtcsv12met<100)";
  const TString nb0       = "(nbjets==0)";
  const TString nb1       = "(nbjets>=1)";
  const TString med_boost = "(j1lpt<500)";
  const TString hgh_boost = "(j1lpt>=500)";
  const TString med_nj    = "((njets>=2) && (njets<=5))";
  const TString hgh_nj    = "(njets>=6)";
  const TString low_pTb   = "(csvj1pt<40)";
  const TString med_pTb   = "((csvj1pt>=40) && (csvj1pt<70))";
  const TString hgh_pTb   = "(csvj1pt>=70)";

  TString processWeight(TString weight, int iCR) {return weight;}

  struct CRegInfo {
    static const int nCR = 10;
    enum                     CRReg{  CR_nb0_med_boost_med_nj,   CR_nb0_med_boost_hgh_nj,   CR_nb0_hgh_boost_med_nj,   CR_nb0_hgh_boost_hgh_nj,  CR_nb1_med_boost_low_pTb,   CR_nb1_med_boost_med_pTb,   CR_nb1_med_boost_hgh_pTb,   CR_nb1_hgh_boost_low_pTb,   CR_nb1_hgh_boost_med_pTb,   CR_nb1_hgh_boost_hgh_pTb  };
    TString  crRegBinNames[nCR] = { "nb0_medboost_lownj",       "nb0_medboost_highnj",     "nb0_highboost_lownj",     "nb0_highboost_highnj",   "nb1_medboost_lowptb",      "nb1_medboost_medptb",      "nb1_medboost_highptb",     "nb1_highboost_lowptb",     "nb1_highboost_medptb",     "nb1_highboost_highptb" };
    TString crSel[nCR] = {
                       nb0 + " && " + med_boost + " && " + med_nj,
                       nb0 + " && " + med_boost + " && " + hgh_nj,
                       nb0 + " && " + hgh_boost + " && " + med_nj,
                       nb0 + " && " + hgh_boost + " && " + hgh_nj,
        mtb + " && " + nb1 + " && " + med_boost + " && " + low_pTb,
        mtb + " && " + nb1 + " && " + med_boost + " && " + med_pTb,
        mtb + " && " + nb1 + " && " + med_boost + " && " + hgh_pTb,
        mtb + " && " + nb1 + " && " + hgh_boost + " && " + low_pTb,
        mtb + " && " + nb1 + " && " + hgh_boost + " && " + med_pTb,
        mtb + " && " + nb1 + " && " + hgh_boost + " && " + hgh_pTb
    };
    TString crSelLabels[nCR] = {
        "#splitline{250 #leq p_{T}_{j_{1l}} < 500}{n_{b} = 0, 2 #leq N_{j} #leq 5}",
        "#splitline{250 #leq p_{T}_{j_{1l}} < 500}{n_{b} = 0, N_{j} #geq 6}",
        "#splitline{500 #leq p_{T}_{j_{1l}}}{n_{b} = 0, 2 #leq N_{j} #leq 5}",
        "#splitline{500 #leq p_{T}_{j_{1l}}}{n_{b} = 0, N_{j} #geq 6}",
        "#splitline{n_{b} #geq 1, 250 #leq p_{T}_{j_{1l}} < 500}{m_{T}(b) < 100, p_{T}(b) < 40}",
        "#splitline{n_{b} #geq 1, 250 #leq p_{T}_{j_{1l}} < 500}{m_{T}(b) < 100, 40 #leq p_{T}(b) < 70}",
        "#splitline{n_{b} #geq 1, 250 #leq p_{T}_{j_{1l}} < 500}{m_{T}(b) < 100, 70 #leq p_{T}(b)}",
        "#splitline{n_{b} #geq 1, 500 #leq p_{T}_{j_{1l}}}{m_{T}(b) < 100, p_{T}(b) < 40}",
        "#splitline{n_{b} #geq 1, 500 #leq p_{T}_{j_{1l}}}{m_{T}(b) < 100, 40 #leq p_{T}(b) < 70}",
        "#splitline{n_{b} #geq 1, 500 #leq p_{T}_{j_{1l}}}{m_{T}(b) < 100, 70 #leq p_{T}(b)}"
    };
    TString crSelNamesLatex[nCR] = {
        "n_{b} = 0, 250 \\leq p_{T_{j_{1l}}} < 500, 2 \\leq N_{j} \\leq 5",
        "n_{b} = 0, 250 \\leq p_{T_{j_{1l}}} < 500, N_{j} \\geq 6",
        "n_{b} = 0, 500 \\leq p_{T_{j_{1l}}}, 2 \\leq N_{j} \\leq 5",
        "n_{b} = 0, 500 \\leq p_{T_{j_{1l}}}, N_{j} \\geq 6",
        "n_{b} \\geq 1, m_{T}(b) < 100, 250 \\leq p_{T_{j_{1l}}} < 500, p_{T}(b) < 40",
        "n_{b} \\geq 1, m_{T}(b) < 100, 250 \\leq p_{T_{j_{1l}}} < 500, 40 \\leq p_{T}(b) < 70",
        "n_{b} \\geq 1, m_{T}(b) < 100, 250 \\leq p_{T_{j_{1l}}} < 500, 70 \\leq p_{T}(b)",
        "n_{b} \\geq 1, m_{T}(b) < 100, 500 \\leq p_{T_{j_{1l}}}, p_{T}(b) < 40",
        "n_{b} \\geq 1, m_{T}(b) < 100, 500 \\leq p_{T_{j_{1l}}}, 40 \\leq p_{T}(b) < 70",
        "n_{b} \\geq 1, m_{T}(b) < 100, 500 \\leq p_{T_{j_{1l}}}, 70 \\leq p_{T}(b)"
    };
    TString crSelNames[nCR] = {
        "n_{b} = 0, 250 #leq p_{T}_{j_{1l}} < 500, 2 #leq N_{j} #leq 5",
        "n_{b} = 0, 250 #leq p_{T}_{j_{1l}} < 500, N_{j} #geq 6",
        "n_{b} = 0, 500 #leq p_{T}_{j_{1l}}, 2 #leq N_{j} #leq 5",
        "n_{b} = 0, 500 #leq p_{T}_{j_{1l}}, N_{j} #geq 6",
        "n_{b} #geq 1, m_{T}(b) < 100, 250 #leq p_{T}_{j_{1l}} < 500, p_{T}(b) < 40",
        "n_{b} #geq 1, m_{T}(b) < 100, 250 #leq p_{T}_{j_{1l}} < 500, 40 #leq p_{T}(b) < 70",
        "n_{b} #geq 1, m_{T}(b) < 100, 250 #leq p_{T}_{j_{1l}} < 500, 70 #leq p_{T}(b)",
        "n_{b} #geq 1, m_{T}(b) < 100, 500 #leq p_{T}_{j_{1l}}, p_{T}(b) < 40",
        "n_{b} #geq 1, m_{T}(b) < 100, 500 #leq p_{T}_{j_{1l}}, 40 #leq p_{T}(b) < 70",
        "n_{b} #geq 1, m_{T}(b) < 100, 500 #leq p_{T}_{j_{1l}}, 70 #leq p_{T}(b)"
    };
    int nMETBins[nCR] = {
        4,
        4,
        4,
        4,
        4,
        4,
        3,
        4,
        4,
        3
    };
    double* metBins[nCR] = {
        new double[nMETBins[0] + 1] {      350, 450, 550, 650, 1000 },
        new double[nMETBins[1] + 1] {      350, 450, 550, 650, 1000 },
        new double[nMETBins[2] + 1] {           450, 550, 650, 750, 1000 },
        new double[nMETBins[3] + 1] {           450, 550, 650, 750, 1000 },
        new double[nMETBins[4] + 1] { 250, 350, 450, 550, 1000 },
        new double[nMETBins[5] + 1] { 250, 350, 450, 550, 1000 },
        new double[nMETBins[6] + 1] {      350, 450, 550, 1000 },
        new double[nMETBins[7] + 1] {      350, 450, 550, 650, 1000 },
        new double[nMETBins[8] + 1] {      350, 450, 550, 650, 1000 },
        new double[nMETBins[9] + 1] {           450, 550, 650, 1000 }
    };
    HistogramGetter* CRmetGetters[nCR] = {
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[0], metBins[0]),
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[1], metBins[1]),
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[2], metBins[2]),
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[3], metBins[3]),
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[4], metBins[4]),
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[5], metBins[5]),
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[6], metBins[6]),
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[7], metBins[7]),
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[8], metBins[8]),
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[9], metBins[9]),
    };

    TString crPreData        =  TString::Format("%s && %s && %s && (nvetolep==0) && (nvetotau==0)",                              METPresel.Data(), BaselineExtraCuts.Data(), CR_def.Data());
    TString crPreOther       =  TString::Format("%s && %s && %s && (nvetolep==0) && (nvetotau==0 || (ismc && npromptgentau>0))", METPresel.Data(), BaselineExtraCuts.Data(), CR_def.Data());
    TString crPreQCD         =  TString::Format("%s && %s && %s",                                                                METPresel.Data(), BaselineExtraCuts.Data(), CR_def.Data());
    TString crPreQCDWithVeto =  TString::Format("%s && %s && %s && (nvetolep==0) && (nvetotau==0)",                              METPresel.Data(), BaselineExtraCuts.Data(), CR_def.Data());

    TString crWgtOther       =  TString::Format("%s*leptnpweight*lepvetoweight", stdMCWeight.Data());

    TString normSel          = TString::Format("%s && %s && %s && (nvetolep>0) && (mtlepmet<100)", METPresel.Data(), BaselineExtraCuts.Data(), SR_def.Data());
    TString normMCWT         = TString::Format("%s*leptnpweight", stdMCWeight.Data());

    std::vector<TH1*> otherBKGSFs;
    TTree *otherBKGTree;
    TTree *dataTree;
    TTree *qcdTree;
    TTree *origQcdTree;
    void fillOtherBkgSFs(TString dataTreeIn, TString otherBKGTreeIn, TString qcdTreeIn, TString origTreeIn){
      fillOtherBkgSFs(getTree(dataTreeIn),getTree(otherBKGTreeIn),getTree(qcdTreeIn),getTree(origTreeIn));
    }
    void fillOtherBkgSFs(TTree * dataTreeIn, TTree * otherBKGTreeIn, TTree * qcdTreeIn, TTree * origTreeIn){
      otherBKGTree = otherBKGTreeIn;
      dataTree = dataTreeIn;
      qcdTree = qcdTreeIn;
      origQcdTree = origTreeIn;

      double bins2[] = {250,300,400,700};
      HistogramGetter SFHG(   "SFmet", "met","#slash{#it{E}}_{T} [GeV]", 3, bins2);
      HistogramGetter SFIncHG("SFmet2","met","#slash{#it{E}}_{T} [GeV]", 1, 250, 1000);

      for(unsigned int iS = 0; iS < nCR; ++iS){
        TString sel = TString::Format("%s && %s",normSel.Data(),crSel[iS].Data());
        TH1F * dataH  = SFIncHG/*(iS == 4?SFIncHG:SFHG)*/ .getHistogram(dataTree,sel,"1.0","Data");
        TH1F * otherH = SFIncHG/*(iS == 4?SFIncHG:SFHG)*/ .getHistogram(otherBKGTree,sel,processWeight(normMCWT,iS),"Other");
        TH1F * qcdH   = SFIncHG/*(iS == 4?SFIncHG:SFHG)*/ .getHistogram(qcdTree,sel,processWeight(normMCWT,iS),"MC");
        otherH->Add(qcdH);
        dataH->Divide(otherH);
        TH1 * h = new TH1F(TString::Format("ttbarwSF_%u",iS),";SF",nMETBins[iS],metBins[iS]);
        for(unsigned int iB = 1; iB <= h->GetNbinsX(); ++iB){
          double metV = h->GetBinCenter(iB);
          double sf = dataH->GetBinContent( dataH->FindFixBin(metV) );
          h->SetBinContent(iB,sf);
        }
        otherBKGSFs.push_back(h);
      }
    }

    std::vector<TH1*> otherBKGScaledYields;
    void fillCorrectedOtherBkgYields(){
      for(unsigned int iS = 0; iS < nCR; ++iS){
        TString sel = TString::Format("%s && %s",crPreOther.Data(),crSel[iS].Data());
        TH1 * h = CRmetGetters[iS]->getHistogram(otherBKGTree,sel,processWeight(crWgtOther,iS),TString::Format("tree_%u",iS));
cout << crSelNames[iS] << endl;
        for(unsigned int iB =1; iB <=h->GetNbinsX(); ++iB){
cout << h->GetBinContent(iB) << " -> " << h->GetBinContent(iB) << " * " << otherBKGSFs[iS]->GetBinContent(iB) << " = " << h->GetBinContent(iB)*otherBKGSFs[iS]->GetBinContent(iB) << endl;
          h->SetBinContent(iB,h->GetBinContent(iB) *otherBKGSFs[iS]->GetBinContent(iB) );
          h->SetBinError(iB,h->GetBinError(iB) *otherBKGSFs[iS]->GetBinContent(iB) );
        }
        otherBKGScaledYields.push_back((TH1*)h->Clone());
      }
    }

    std::vector<TH1*> dataYields;
    std::vector<TH1*> qcdYieldsWithVeto;
    std::vector<TH1*> otherBKGCorr;
    std::vector<TH1*> otherBKGCorrUnc;
    std::vector<TH1*> origQCDYields;
    std::vector<TH1*> smearedQCDYields;
    void fillDataCorr(){
      for(unsigned int iS = 0; iS < nCR; ++iS){
        TH1 * hd = CRmetGetters[iS]->getHistogram(dataTree,TString::Format("%s && %s",crPreData.Data(),crSel[iS].Data()),"1.0",TString::Format("tree_%u",iS));
        CRmetGetters[iS]->setNBS(50);
        TH1 * hqwv = CRmetGetters[iS]->getHistogramManual(qcdTree,TString::Format("%s && %s",crPreQCDWithVeto.Data(),crSel[iS].Data()),processWeight(stdQCDWeight,iS),TString::Format("tree_%u",iS));
        CRmetGetters[iS]->setNBS(0);
        TH1 * corr = (TH1*)hd->Clone();
        TH1 * corrU = (TH1*)hd->Clone();
        TH1 * origQCD    = CRmetGetters[iS]->getHistogram(origQcdTree,TString::Format("%s && %s",crPreQCDWithVeto.Data(),crSel[iS].Data()),processWeight(stdQCDWeight,iS),TString::Format("origQCDtree_%u",   iS));
        CRmetGetters[iS]->setNBS(50);
        TH1 * smearedQCD = CRmetGetters[iS]->getHistogram(qcdTree,    TString::Format("%s && %s",crPreQCDWithVeto.Data(),crSel[iS].Data()),processWeight(stdQCDWeight,iS),TString::Format("smearedQCDtree_%u",iS));
        CRmetGetters[iS]->setNBS(0);
        std::cout << crSelNames[iS] << std::endl;
        for(unsigned int iB =1; iB <=hd->GetNbinsX(); ++iB){
          double dV = hd->GetBinContent(iB) < 10 ?  hqwv->GetBinContent(iB) + otherBKGScaledYields[iS]->GetBinContent(iB): hd->GetBinContent(iB);
          double oV = otherBKGScaledYields[iS]->GetBinContent(iB);
          double cV = 1 - oV/dV;
          corr->SetBinContent(iB,cV );
          corrU->SetBinContent(iB,1 + oV/dV );
          std::cout << "("<<cV <<","<<1 + oV/dV  <<") ";
        }
        std::cout << std::endl;
        dataYields.push_back(hd);
        qcdYieldsWithVeto.push_back(hqwv);
        otherBKGCorr.push_back(corr);
        otherBKGCorrUnc.push_back(corrU);
        origQCDYields.push_back(origQCD);
        smearedQCDYields.push_back(smearedQCD);
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
//    TString getLatexBlock(CRReg reg, int metBin){
//      if(metBin+1 == nMETBins[reg])
//        return TString::Format("$>$%.0f & %.0f & %.2f $\\pm$ %.2f",metBins[reg][metBin],getDataYield(reg,metBin),getScaledOtherYield(reg,metBin),getScaledOtherYieldStatUnc(reg,metBin));
//      else
//        return TString::Format("%.0f$-$%.0f & %.0f & %.2f $\\pm$ %.2f",metBins[reg][metBin],metBins[reg][metBin+1],getDataYield(reg,metBin),getScaledOtherYield(reg,metBin),getScaledOtherYieldStatUnc(reg,metBin));
//    }
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
    static const int nSR = 10;
    enum                     SRReg{  SR_nb0_med_boost_med_nj,   SR_nb0_med_boost_hgh_nj,   SR_nb0_hgh_boost_med_nj,   SR_nb0_hgh_boost_hgh_nj,  SR_nb1_med_boost_low_pTb,   SR_nb1_med_boost_med_pTb,   SR_nb1_med_boost_hgh_pTb,   SR_nb1_hgh_boost_low_pTb,   SR_nb1_hgh_boost_med_pTb,   SR_nb1_hgh_boost_hgh_pTb  };
    TString  srRegBinNames[nSR] = { "nb0_medboost_lownj",       "nb0_medboost_highnj",     "nb0_highboost_lownj",     "nb0_highboost_highnj",   "nb1_medboost_lowptb",      "nb1_medboost_medptb",      "nb1_medboost_highptb",     "nb1_highboost_lowptb",     "nb1_highboost_medptb",     "nb1_highboost_highptb" };
    TString srSel[nSR] = {
                       nb0 + " && " + med_boost + " && " + med_nj,
                       nb0 + " && " + med_boost + " && " + hgh_nj,
                       nb0 + " && " + hgh_boost + " && " + med_nj,
                       nb0 + " && " + hgh_boost + " && " + hgh_nj,
        mtb + " && " + nb1 + " && " + med_boost + " && " + low_pTb,
        mtb + " && " + nb1 + " && " + med_boost + " && " + med_pTb,
        mtb + " && " + nb1 + " && " + med_boost + " && " + hgh_pTb,
        mtb + " && " + nb1 + " && " + hgh_boost + " && " + low_pTb,
        mtb + " && " + nb1 + " && " + hgh_boost + " && " + med_pTb,
        mtb + " && " + nb1 + " && " + hgh_boost + " && " + hgh_pTb
    };
    TString srSelLabels[nSR] = {
        "#splitline{250 #leq p_{T}_{j_{1l}} < 500}{n_{b} = 0, 2 #leq N_{j} #leq 5}",
        "#splitline{250 #leq p_{T}_{j_{1l}} < 500}{n_{b} = 0, N_{j} #geq 6}",
        "#splitline{500 #leq p_{T}_{j_{1l}}}{n_{b} = 0, 2 #leq N_{j} #leq 5}",
        "#splitline{500 #leq p_{T}_{j_{1l}}}{n_{b} = 0, N_{j} #geq 6}",
        "#splitline{n_{b} #geq 1, 250 #leq p_{T}_{j_{1l}} < 500}{m_{T}(b) < 100, p_{T}(b) < 40}",
        "#splitline{n_{b} #geq 1, 250 #leq p_{T}_{j_{1l}} < 500}{m_{T}(b) < 100, 40 #leq p_{T}(b) < 70}",
        "#splitline{n_{b} #geq 1, 250 #leq p_{T}_{j_{1l}} < 500}{m_{T}(b) < 100, 70 #leq p_{T}(b)}",
        "#splitline{n_{b} #geq 1, 500 #leq p_{T}_{j_{1l}}}{m_{T}(b) < 100, p_{T}(b) < 40}",
        "#splitline{n_{b} #geq 1, 500 #leq p_{T}_{j_{1l}}}{m_{T}(b) < 100, 40 #leq p_{T}(b) < 70}",
        "#splitline{n_{b} #geq 1, 500 #leq p_{T}_{j_{1l}}}{m_{T}(b) < 100, 70 #leq p_{T}(b)}"
    };
    TString srSelNamesLatex[nSR] = {
        "n_{b} = 0, 250 \\leq p_{T_{j_{1l}}} < 500, 2 \\leq N_{j} \\leq 5",
        "n_{b} = 0, 250 \\leq p_{T_{j_{1l}}} < 500, N_{j} \\geq 6",
        "n_{b} = 0, 500 \\leq p_{T_{j_{1l}}}, 2 \\leq N_{j} \\leq 5",
        "n_{b} = 0, 500 \\leq p_{T_{j_{1l}}}, N_{j} \\geq 6",
        "n_{b} \\geq 1, m_{T}(b) < 100, 250 \\leq p_{T_{j_{1l}}} < 500, p_{T}(b) < 40",
        "n_{b} \\geq 1, m_{T}(b) < 100, 250 \\leq p_{T_{j_{1l}}} < 500, 40 \\leq p_{T}(b) < 70",
        "n_{b} \\geq 1, m_{T}(b) < 100, 250 \\leq p_{T_{j_{1l}}} < 500, 70 \\leq p_{T}(b)",
        "n_{b} \\geq 1, m_{T}(b) < 100, 500 \\leq p_{T_{j_{1l}}}, p_{T}(b) < 40",
        "n_{b} \\geq 1, m_{T}(b) < 100, 500 \\leq p_{T_{j_{1l}}}, 40 \\leq p_{T}(b) < 70",
        "n_{b} \\geq 1, m_{T}(b) < 100, 500 \\leq p_{T_{j_{1l}}}, 70 \\leq p_{T}(b)"
    };
    TString srSelNames[nSR] = {
        "n_{b} = 0, 250 #leq p_{T}_{j_{1l}} < 500, 2 #leq N_{j} #leq 5",
        "n_{b} = 0, 250 #leq p_{T}_{j_{1l}} < 500, N_{j} #geq 6",
        "n_{b} = 0, 500 #leq p_{T}_{j_{1l}}, 2 #leq N_{j} #leq 5",
        "n_{b} = 0, 500 #leq p_{T}_{j_{1l}}, N_{j} #geq 6",
        "n_{b} #geq 1, m_{T}(b) < 100, 250 #leq p_{T}_{j_{1l}} < 500, p_{T}(b) < 40",
        "n_{b} #geq 1, m_{T}(b) < 100, 250 #leq p_{T}_{j_{1l}} < 500, 40 #leq p_{T}(b) < 70",
        "n_{b} #geq 1, m_{T}(b) < 100, 250 #leq p_{T}_{j_{1l}} < 500, 70 #leq p_{T}(b)",
        "n_{b} #geq 1, m_{T}(b) < 100, 500 #leq p_{T}_{j_{1l}}, p_{T}(b) < 40",
        "n_{b} #geq 1, m_{T}(b) < 100, 500 #leq p_{T}_{j_{1l}}, 40 #leq p_{T}(b) < 70",
        "n_{b} #geq 1, m_{T}(b) < 100, 500 #leq p_{T}_{j_{1l}}, 70 #leq p_{T}(b)"
    };
    int nMETBins[nSR] = {
        4,
        4,
        4,
        4,
        4,
        4,
        3,
        4,
        4,
        3
    };
    double* metBins[nSR] = {
        new double[nMETBins[0] + 1] {      350, 450, 550, 650, 1000 },
        new double[nMETBins[1] + 1] {      350, 450, 550, 650, 1000 },
        new double[nMETBins[2] + 1] {           450, 550, 650, 750, 1000 },
        new double[nMETBins[3] + 1] {           450, 550, 650, 750, 1000 },
        new double[nMETBins[4] + 1] { 250, 350, 450, 550, 1000 },
        new double[nMETBins[5] + 1] { 250, 350, 450, 550, 1000 },
        new double[nMETBins[6] + 1] {      350, 450, 550, 1000 },
        new double[nMETBins[7] + 1] {      350, 450, 550, 650, 1000 },
        new double[nMETBins[8] + 1] {      350, 450, 550, 650, 1000 },
        new double[nMETBins[9] + 1] {           450, 550, 650, 1000 }
    };
    HistogramGetter* SRmetGetters[nSR] = {
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[0], metBins[0]),
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[1], metBins[1]),
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[2], metBins[2]),
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[3], metBins[3]),
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[4], metBins[4]),
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[5], metBins[5]),
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[6], metBins[6]),
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[7], metBins[7]),
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[8], metBins[8]),
        new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", nMETBins[9], metBins[9]),
    };
    CRegInfo::CRReg crRegs[nSR] = {
        CRegInfo::CR_nb0_med_boost_med_nj,
        CRegInfo::CR_nb0_med_boost_hgh_nj,
        CRegInfo::CR_nb0_hgh_boost_med_nj,
        CRegInfo::CR_nb0_hgh_boost_hgh_nj,
        CRegInfo::CR_nb1_med_boost_low_pTb,
        CRegInfo::CR_nb1_med_boost_med_pTb,
        CRegInfo::CR_nb1_med_boost_hgh_pTb,
        CRegInfo::CR_nb1_hgh_boost_low_pTb,
        CRegInfo::CR_nb1_hgh_boost_med_pTb,
        CRegInfo::CR_nb1_hgh_boost_hgh_pTb
    };

    TString srPreQCD         = TString::Format("%s && %s && %s",                                   METPresel.Data(), BaselineExtraCuts.Data(), SR_def.Data());
    TString srPreQCDWithVeto = TString::Format("%s && %s && %s && (nvetolep==0) && (nvetotau==0)", METPresel.Data(), BaselineExtraCuts.Data(), SR_def.Data());

    TString processWeight(TString weight, int iSR) {return weight;}

    TTree *qcdTree;
    TTree *origQcdTree;

    std::vector<TH1F*> nominalQCD;
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

        TH1F * origQCD    = SRmetGetters[iS]->getHistogram(origQcdTree,sel,processWeight(stdQCDWeight,iS),TString::Format("origQCDYield_%u",   iS));
        SRmetGetters[iS]->setNBS(50);
        TH1F * smearedQCD = SRmetGetters[iS]->getHistogram(qcdTree,    sel,processWeight(stdQCDWeight,iS),TString::Format("smearedQCDYield_%u",iS));
        SRmetGetters[iS]->setNBS(0);

        SRmetGetters[iS]->setNBS(50);
        TH1F * nom = SRmetGetters[iS]->getTFAndCov(qcdTree,selN,processWeight(stdQCDWeight,iS),selD,processWeight(stdQCDWeight,iS),TString::Format("ratio_%u",iS));
        SRmetGetters[iS]->setNBS(0);

        TH1F * upN    = SRmetGetters[iS]->getHistogram(qcdTree,selN,TString::Format("%s*upTailWeight",processWeight(stdMCWeight,iS).Data()),TString::Format("QCD_%u",iS));
        TH1F * downN  = SRmetGetters[iS]->getHistogram(qcdTree,selN,TString::Format("%s*downTailWeight",processWeight(stdMCWeight,iS).Data()),TString::Format("QCD_%u",iS));
        TH1F * norN   = SRmetGetters[iS]->getHistogram(qcdTree,selN,processWeight(stdMCWeight,iS),TString::Format("QCD_%u",iS));
        TH1F * orN    = origQcdTree ? SRmetGetters[iS]->getHistogram(origQcdTree,selN,processWeight(stdMCWeight,iS),TString::Format("QCD_%u",iS)) : 0;

        TH1F * upD    = SRmetGetters[iS]->getHistogram(qcdTree,selD,TString::Format("%s*upTailWeight",processWeight(stdMCWeight,iS).Data()),TString::Format("QCD_%u",iS));
        TH1F * downD  = SRmetGetters[iS]->getHistogram(qcdTree,selD,TString::Format("%s*downTailWeight",processWeight(stdMCWeight,iS).Data()),TString::Format("QCD_%u",iS));
        TH1F * norD   = SRmetGetters[iS]->getHistogram(qcdTree,selD,processWeight(stdMCWeight,iS),TString::Format("QCD_%u",iS));
        TH1F * orD    = origQcdTree ? SRmetGetters[iS]->getHistogram(origQcdTree,selD,processWeight(stdMCWeight,iS),TString::Format("QCD_%u",iS)) : 0;

        upN        ->Divide(upD);
        downN      ->Divide(downD);
        norN       ->Divide(norD);
        if(orN) orN->Divide(orD);

        origQCDYields.   push_back(origQCD);
        smearedQCDYields.push_back(smearedQCD);
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
          totalExtSystUncs[iS].push_back(sqrt(totalUnc) + 1);
        }
    }

    double getTF(SRReg reg, int metBin){return nominalQCD[reg]->GetBinContent(metBin+1);}
    double getTFStatUnc(SRReg reg, int metBin){return std::min(2.0,nominalQCD[reg]->GetBinError(metBin+1)/nominalQCD[reg]->GetBinContent(metBin+1)+1);}
    double getRespUnc(SRReg reg, int metBin){return (respUp[reg]->GetBinContent(metBin+1) - respDown[reg]->GetBinContent(metBin+1) ) /(2*getTF(reg,metBin)) +1;}
    double getPrediction(CRegInfo* crinfo, SRReg reg, int metBin){
      double dY = crinfo->getDataYield(crRegs[reg],metBin) == 0 ? 1: crinfo->getDataYield(crRegs[reg],metBin);
      return dY*crinfo->getDataYieldCorr(crRegs[reg],metBin)*getTF(reg,metBin);
    }
    double getExtUncs(SRReg reg, int metBin){ return totalExtSystUncs[reg][metBin];}
    TString getBinName (SRReg reg, int metBin) {return  TString::Format("%.0f_%s",metBins[reg][metBin],srRegBinNames[reg].Data());}
    double getTotalSystUnc(CRegInfo * crinfo,SRReg reg, int metBin){
      return TMath::Sqrt(   (getTFStatUnc(reg,metBin) -1)*(getTFStatUnc(reg,metBin) -1)
                          + (getRespUnc(reg,metBin) -1)*(getRespUnc(reg,metBin) -1)
                          + (getExtUncs(reg,metBin) -1)*(getExtUncs(reg,metBin) -1)
                          + (crinfo->getDataYieldCorrUnc(crRegs[reg],metBin)-1)*(crinfo->getDataYieldCorrUnc(crRegs[reg],metBin)-1)
                          ) + 1;
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

      if(tf < .0005 ) return TString::Format(" $<$0.001 & - ");
//      return TString::Format("%.3f $\\pm$ %.3f & %.3f $\\pm$ %.3f (stat.) $\\pm$ %.3f (syst.)",tf,tfUnc,pred,predstat,predsyst );
      return TString::Format("%.3f $\\pm$ %.3f & %.3f $\\pm$ %.3f & %.3f $\\pm$ %.3f & %.3f $\\pm$ %.3f (stat.) $\\pm$ %.3f (syst.)",getOrigQCDYields_OOB(reg,metBin),getOrigQCDYields_OOB_StatUnc(reg,metBin),getSmearedQCDYields_OOB(reg,metBin),getSmearedQCDYields_OOB_StatUnc(reg,metBin),tf,tfUnc,pred,predstat,predsyst );
    }
  };
};

#endif
