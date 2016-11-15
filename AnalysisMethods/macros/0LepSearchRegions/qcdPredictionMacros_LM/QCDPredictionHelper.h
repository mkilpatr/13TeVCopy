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
  const TString eventsToSkip = "!(run == 1 && lumi == 46160 && event == 331634716)";
  const TString METPresel = "passjson && passmetmht100 && (passmetfilters || process==10)&& (met >= 250) && (j1chEnFrac > 0.1) && (j1chEnFrac < 0.99)";
  const TString ResTailExtraCuts = "(((dphij1met < .1) || (dphij2met < .1)) && (nvetolep == 0) && (pseudoRespPassFilter == 1))";
  const TString BaselineExtraCuts = "((njl>=1) && (njets>=2) && (dphij1lmet>2.) && (j1lpt>=250) && (met/sqrt(ht)>=10))";
  const TString METBaseline = "passjson && passmetmht100 && (passmetfilters || process==10) && (met >= 250) && (j1chEnFrac > 0.1) && (j1chEnFrac < 0.99) && ((njl>=1) && (njets>=2) && (dphij1lmet>2.) && (j1lpt>=250) && (met/sqrt(ht)>=10))";
  const TString CR_def = "((dphij1met < .1) || (dphij2met < .1) || (dphij3met < .1))";
  const TString SR_def = "((dphij1met >= 0.5) && (dphij2met>=0.15) && (dphij3met>=0.15))";
  const TString stdWeight = "1.0";
  const TString stdMCWeight  = lumi + "*weight*truePUWeight*btagWeight";
  const TString stdQCDWeight = lumi + "*weight*truePUWeight*btagWeight*qcdRespTailWeight";

  const TString mtb       = "(mtcsv12met<100)";                                              const TString mtb_R       = "M_{T}(b_{1,2},#slash{E}_{T}) < 100";             const TString mtb_L       = "\\mtb < 100";
  const TString nb0       = "(nbjets==0)";                                                   const TString nb0_R       = "N_{b} = 0";                                      const TString nb0_L       = "\\nb = 0";
  const TString nb1       = "(nbjets>=1 && nlbjets==1)";                                     const TString nb1_R       = "N_{b} #geq 1, N_{b_{l}} = 1";                    const TString nb1_L       = "\\nb \\geq 1, \\nlb = 1";
  const TString nb2       = "(nbjets>=1 && nlbjets>=2)";                                     const TString nb2_R       = "N_{b} #geq 1, N_{b_{l}} #geq 2";                 const TString nb2_L       = "\\nb \\geq 1, \\nlb \\geq 2";
  const TString med_boost = "(j1lpt<500)";                                                   const TString med_boost_R = "250 #leq p_{T}(j_{ISR}) < 500";                  const TString med_boost_L = "250 \\leq \\ptjisr < 500";
  const TString hgh_boost = "(j1lpt>=500)";                                                  const TString hgh_boost_R = "500 #leq p_{T}(j_{ISR})";                        const TString hgh_boost_L = "500 \\leq \\ptjisr";
  const TString med_nj    = "((njets>=2) && (njets<=5))";                                    const TString med_nj_R    = "2 #leq N_{j} #leq 5";                            const TString med_nj_L    = "2 \\leq \\njet \\leq 5";
  const TString hgh_nj    = "(njets>=6)";                                                    const TString hgh_nj_R    = "N_{j} #geq 6";                                   const TString hgh_nj_L    = "\\njet \\geq 6";
  const TString low_pTb   = "(csvj1pt<40)";                                                  const TString low_pTb_R   = "p_{T}(b) < 40";                                  const TString low_pTb_L   = "\\ptb < 40";
  const TString med_pTb   = "((csvj1pt>=40) && (csvj1pt<70))";                               const TString med_pTb_R   = "40 #leq p_{T}(b) < 70";                          const TString med_pTb_L   = "40 \\leq \\ptb < 70";
  const TString hgh_pTb   = "(csvj1pt>=70)";                                                 const TString hgh_pTb_R   = "70 #leq p_{T}(b)";                               const TString hgh_pTb_L   = "70 \\leq \\ptb";
  const TString low_pTb12 = "((csvj1pt + csvj2pt)<100)";                                     const TString low_pTb12_R = "p_{T}(b_{1}) + p_{T}(b_{2}) < 100";              const TString low_pTb12_L = "\\ptbonetwo < 100";
  const TString med_pTb12 = "(((csvj1pt + csvj2pt)>=100) && ((csvj1pt + csvj2pt)<160))";     const TString med_pTb12_R = "100 #leq p_{T}(b_{1}) + p_{T}(b_{2}) < 160";     const TString med_pTb12_L = "100 \\leq \\ptbonetwo < 160";
  const TString hgh_pTb12 = "((csvj1pt + csvj2pt)>=160)";                                    const TString hgh_pTb12_R = "160 #leq p_{T}(b_{1}) + p_{T}(b_{2})";           const TString hgh_pTb12_L = "160 \\leq \\ptbonetwo";
  const TString metJet_0to500 = "((met->pt() + min(jets[0]->pt(), min(jets[1]->pt(), jets[2]->pt()))) < 500)";	  const TString metJet_0to500_R = "(#slash{E}_{T} + j_{123}) < 500";       const TString metJet_0to500_L = "met + jets < 500";
  const TString metJet_500toInf = "((met->pt() + min(jets[0]->pt(), min(jets[1]->pt(), jets[2]->pt()))) >= 500)";   const TString metJet_500toInf_R = "(#slash{E}_{T} + j_{123}) >= 500"; const TString metJet_500toInf_L = "met + jets >= 500";
//TMath::Csc[dphistarmhtj1]Tmath::Sin[dphij1met-dphistar]*met->pt() = Jets[0]->pt()  
  TString processWeight(TString weight, int iCR) {return weight;}

  struct CRegInfo {
    static const int nCR = 20;
    enum                     CRReg{ CR_nb0_hgh_boost_low_nj_mj500, CR_nb0_hgh_boost_hgh_nj_mj500, CR_nb1_med_boost_low_pTb_mj500, CR_nb1_med_boost_med_pTb_mj500, CR_nb1_hgh_boost_low_pTb_mj500, CR_nb1_hgh_boost_med_pTb_mj500, CR_nb2_med_boost_low_pTb_mj500, CR_nb2_med_boost_med_pTb_mj500, CR_nb2_hgh_boost_low_pTb_mj500, CR_nb2_hgh_boost_med_pTb_mj500, CR_nb0_hgh_boost_low_nj_mjInf, CR_nb0_hgh_boost_hgh_nj_mjInf, CR_nb1_med_boost_low_pTb_mjInf, CR_nb1_med_boost_med_pTb_mjInf, CR_nb1_hgh_boost_low_pTb_mjInf, CR_nb1_hgh_boost_med_pTb_mjInf, CR_nb2_med_boost_low_pTb_mjInf, CR_nb2_med_boost_med_pTb_mjInf, CR_nb2_hgh_boost_low_pTb_mjInf, CR_nb2_hgh_boost_med_pTb_mjInf };
    TString  crRegBinNames[nCR] = { "nb0_highboost_lownj_mj500",   "nb0_highboost_highnj_mj500",  "nb1_medboost_lowptb_mj500",    "nb1_medboost_medptb_mj500",    "nb1_highboost_lowptb_mj500",   "nb1_highboost_medptb_mj500",   "nb2_medboost_lowptb_mj500",    "nb2_medboost_medptb_mj500",    "nb2_highboost_lowptb_mj500",   "nb2_highboost_medptb_mj500", "nb0_highboost_lownj_mjInf",   "nb0_highboost_highnj_mjInf",  "nb1_medboost_lowptb_mjInf",    "nb1_medboost_medptb_mjInf",    "nb1_highboost_lowptb_mjInf",   "nb1_highboost_medptb_mjInf",   "nb2_medboost_lowptb_mjInf",    "nb2_medboost_medptb_mjInf",    "nb2_highboost_lowptb_mjInf",   "nb2_highboost_medptb_mjInf"   };
    TString crSel[nCR] = {
                       nb0 + " && " + hgh_boost + " && " + med_nj    + " && " + metJet_0to500,
                       nb0 + " && " + hgh_boost + " && " + hgh_nj    + " && " + metJet_0to500,
        mtb + " && " + nb1 + " && " + med_boost + " && " + low_pTb   + " && " + metJet_0to500,
        mtb + " && " + nb1 + " && " + med_boost + " && " + med_pTb   + " && " + metJet_0to500,
        mtb + " && " + nb1 + " && " + hgh_boost + " && " + low_pTb   + " && " + metJet_0to500,
        mtb + " && " + nb1 + " && " + hgh_boost + " && " + med_pTb   + " && " + metJet_0to500,
        mtb + " && " + nb2 + " && " + med_boost + " && " + low_pTb12 + " && " + metJet_0to500,
        mtb + " && " + nb2 + " && " + med_boost + " && " + med_pTb12 + " && " + metJet_0to500,
        mtb + " && " + nb2 + " && " + hgh_boost + " && " + low_pTb12 + " && " + metJet_0to500,
        mtb + " && " + nb2 + " && " + hgh_boost + " && " + med_pTb12 + " && " + metJet_0to500,
                       nb0 + " && " + hgh_boost + " && " + med_nj    + " && " + metJet_500toInf,
                       nb0 + " && " + hgh_boost + " && " + hgh_nj    + " && " + metJet_500toInf,
        mtb + " && " + nb1 + " && " + med_boost + " && " + low_pTb   + " && " + metJet_500toInf,
        mtb + " && " + nb1 + " && " + med_boost + " && " + med_pTb   + " && " + metJet_500toInf,
        mtb + " && " + nb1 + " && " + hgh_boost + " && " + low_pTb   + " && " + metJet_500toInf,
        mtb + " && " + nb1 + " && " + hgh_boost + " && " + med_pTb   + " && " + metJet_500toInf,
        mtb + " && " + nb2 + " && " + med_boost + " && " + low_pTb12 + " && " + metJet_500toInf,
        mtb + " && " + nb2 + " && " + med_boost + " && " + med_pTb12 + " && " + metJet_500toInf,
        mtb + " && " + nb2 + " && " + hgh_boost + " && " + low_pTb12 + " && " + metJet_500toInf,
        mtb + " && " + nb2 + " && " + hgh_boost + " && " + med_pTb12 + " && " + metJet_500toInf,
    };
    TString crSelLabels[nCR] = {
        "#splitline{" + hgh_boost_R + ", " + nb0_R + "}{"                + med_nj_R    + ", " + metJet_0to500_R + "}",
        "#splitline{" + hgh_boost_R + ", " + nb0_R + "}{"                + hgh_nj_R    + ", " + metJet_0to500_R + "}",
        "#splitline{" + med_boost_R + ", " + nb1_R + "}{" + mtb_R + ", " + low_pTb_R   + ", " + metJet_0to500_R + "}",
        "#splitline{" + med_boost_R + ", " + nb1_R + "}{" + mtb_R + ", " + med_pTb_R   + ", " + metJet_0to500_R + "}",
        "#splitline{" + hgh_boost_R + ", " + nb1_R + "}{" + mtb_R + ", " + low_pTb_R   + ", " + metJet_0to500_R + "}",
        "#splitline{" + hgh_boost_R + ", " + nb1_R + "}{" + mtb_R + ", " + med_pTb_R   + ", " + metJet_0to500_R + "}",
        "#splitline{" + med_boost_R + ", " + nb2_R + "}{" + mtb_R + ", " + low_pTb12_R + ", " + metJet_0to500_R + "}",
        "#splitline{" + med_boost_R + ", " + nb2_R + "}{" + mtb_R + ", " + med_pTb12_R + ", " + metJet_0to500_R + "}",
        "#splitline{" + hgh_boost_R + ", " + nb2_R + "}{" + mtb_R + ", " + low_pTb12_R + ", " + metJet_0to500_R + "}",
        "#splitline{" + hgh_boost_R + ", " + nb2_R + "}{" + mtb_R + ", " + med_pTb12_R + ", " + metJet_0to500_R + "}",
        "#splitline{" + hgh_boost_R + ", " + nb0_R + "}{"                + med_nj_R    + ", " + metJet_500toInf_R + "}",
        "#splitline{" + hgh_boost_R + ", " + nb0_R + "}{"                + hgh_nj_R    + ", " + metJet_500toInf_R + "}",
        "#splitline{" + med_boost_R + ", " + nb1_R + "}{" + mtb_R + ", " + low_pTb_R   + ", " + metJet_500toInf_R + "}",
        "#splitline{" + med_boost_R + ", " + nb1_R + "}{" + mtb_R + ", " + med_pTb_R   + ", " + metJet_500toInf_R + "}",
        "#splitline{" + hgh_boost_R + ", " + nb1_R + "}{" + mtb_R + ", " + low_pTb_R   + ", " + metJet_500toInf_R + "}",
        "#splitline{" + hgh_boost_R + ", " + nb1_R + "}{" + mtb_R + ", " + med_pTb_R   + ", " + metJet_500toInf_R + "}",
        "#splitline{" + med_boost_R + ", " + nb2_R + "}{" + mtb_R + ", " + low_pTb12_R + ", " + metJet_500toInf_R + "}",
        "#splitline{" + med_boost_R + ", " + nb2_R + "}{" + mtb_R + ", " + med_pTb12_R + ", " + metJet_500toInf_R + "}",
        "#splitline{" + hgh_boost_R + ", " + nb2_R + "}{" + mtb_R + ", " + low_pTb12_R + ", " + metJet_500toInf_R + "}",
        "#splitline{" + hgh_boost_R + ", " + nb2_R + "}{" + mtb_R + ", " + med_pTb12_R + ", " + metJet_500toInf_R + "}",
    };
    TString crSelNamesLatex[nCR] = {
                       nb0_L + ", " + hgh_boost_L + ", " + med_nj_L    + ", " + metJet_0to500_L,
                       nb0_L + ", " + hgh_boost_L + ", " + hgh_nj_L    + ", " + metJet_0to500_L,
        mtb_L + ", " + nb1_L + ", " + med_boost_L + ", " + low_pTb_L   + ", " + metJet_0to500_L,
        mtb_L + ", " + nb1_L + ", " + med_boost_L + ", " + med_pTb_L   + ", " + metJet_0to500_L,
        mtb_L + ", " + nb1_L + ", " + hgh_boost_L + ", " + low_pTb_L   + ", " + metJet_0to500_L,
        mtb_L + ", " + nb1_L + ", " + hgh_boost_L + ", " + med_pTb_L   + ", " + metJet_0to500_L,
        mtb_L + ", " + nb2_L + ", " + med_boost_L + ", " + low_pTb12_L + ", " + metJet_0to500_L,
        mtb_L + ", " + nb2_L + ", " + med_boost_L + ", " + med_pTb12_L + ", " + metJet_0to500_L,
        mtb_L + ", " + nb2_L + ", " + hgh_boost_L + ", " + low_pTb12_L + ", " + metJet_0to500_L,
        mtb_L + ", " + nb2_L + ", " + hgh_boost_L + ", " + med_pTb12_L + ", " + metJet_0to500_L,
                       nb0_L + ", " + hgh_boost_L + ", " + med_nj_L    + ", " + metJet_500toInf_L,
                       nb0_L + ", " + hgh_boost_L + ", " + hgh_nj_L    + ", " + metJet_500toInf_L,
        mtb_L + ", " + nb1_L + ", " + med_boost_L + ", " + low_pTb_L   + ", " + metJet_500toInf_L,
        mtb_L + ", " + nb1_L + ", " + med_boost_L + ", " + med_pTb_L   + ", " + metJet_500toInf_L,
        mtb_L + ", " + nb1_L + ", " + hgh_boost_L + ", " + low_pTb_L   + ", " + metJet_500toInf_L,
        mtb_L + ", " + nb1_L + ", " + hgh_boost_L + ", " + med_pTb_L   + ", " + metJet_500toInf_L,
        mtb_L + ", " + nb2_L + ", " + med_boost_L + ", " + low_pTb12_L + ", " + metJet_500toInf_L,
        mtb_L + ", " + nb2_L + ", " + med_boost_L + ", " + med_pTb12_L + ", " + metJet_500toInf_L,
        mtb_L + ", " + nb2_L + ", " + hgh_boost_L + ", " + low_pTb12_L + ", " + metJet_500toInf_L,
        mtb_L + ", " + nb2_L + ", " + hgh_boost_L + ", " + med_pTb12_L + ", " + metJet_500toInf_L,
    };
    TString crSelNames[nCR] = {
                       nb0_R + ", " + hgh_boost_R + ", " + med_nj_R    + ", " + metJet_0to500_R,
                       nb0_R + ", " + hgh_boost_R + ", " + hgh_nj_R    + ", " + metJet_0to500_R,
        mtb_R + ", " + nb1_R + ", " + med_boost_R + ", " + low_pTb_R   + ", " + metJet_0to500_R,
        mtb_R + ", " + nb1_R + ", " + med_boost_R + ", " + med_pTb_R   + ", " + metJet_0to500_R,
        mtb_R + ", " + nb1_R + ", " + hgh_boost_R + ", " + low_pTb_R   + ", " + metJet_0to500_R,
        mtb_R + ", " + nb1_R + ", " + hgh_boost_R + ", " + med_pTb_R   + ", " + metJet_0to500_R,
        mtb_R + ", " + nb2_R + ", " + med_boost_R + ", " + low_pTb12_R + ", " + metJet_0to500_R,
        mtb_R + ", " + nb2_R + ", " + med_boost_R + ", " + med_pTb12_R + ", " + metJet_0to500_R,
        mtb_R + ", " + nb2_R + ", " + hgh_boost_R + ", " + low_pTb12_R + ", " + metJet_0to500_R,
        mtb_R + ", " + nb2_R + ", " + hgh_boost_R + ", " + med_pTb12_R + ", " + metJet_0to500_R,
                       nb0_R + ", " + hgh_boost_R + ", " + med_nj_R    + ", " + metJet_500toInf_R,
                       nb0_R + ", " + hgh_boost_R + ", " + hgh_nj_R    + ", " + metJet_500toInf_R,
        mtb_R + ", " + nb1_R + ", " + med_boost_R + ", " + low_pTb_R   + ", " + metJet_500toInf_R,
        mtb_R + ", " + nb1_R + ", " + med_boost_R + ", " + med_pTb_R   + ", " + metJet_500toInf_R,
        mtb_R + ", " + nb1_R + ", " + hgh_boost_R + ", " + low_pTb_R   + ", " + metJet_500toInf_R,
        mtb_R + ", " + nb1_R + ", " + hgh_boost_R + ", " + med_pTb_R   + ", " + metJet_500toInf_R,
        mtb_R + ", " + nb2_R + ", " + med_boost_R + ", " + low_pTb12_R + ", " + metJet_500toInf_R,
        mtb_R + ", " + nb2_R + ", " + med_boost_R + ", " + med_pTb12_R + ", " + metJet_500toInf_R,
        mtb_R + ", " + nb2_R + ", " + hgh_boost_R + ", " + low_pTb12_R + ", " + metJet_500toInf_R,
        mtb_R + ", " + nb2_R + ", " + hgh_boost_R + ", " + med_pTb12_R + ", " + metJet_500toInf_R,
    };
    int nMETBins[nCR] = { 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 };
    double* metBins[nCR] = {
        new double[nMETBins[2] + 1] {      450, 550, 650, 750, 1000 },
        new double[nMETBins[3] + 1] {      450, 550, 650, 750, 1000 },
        new double[nMETBins[4] + 1] { 300, 400, 500, 600,      1000 },
        new double[nMETBins[5] + 1] { 300, 400, 500, 600,      1000 },
        new double[nMETBins[7] + 1] {      450, 550, 650, 750, 1000 },
        new double[nMETBins[8] + 1] {      450, 550, 650, 750, 1000 },
        new double[nMETBins[4] + 1] { 300, 400, 500, 600,      1000 },
        new double[nMETBins[5] + 1] { 300, 400, 500, 600,      1000 },
        new double[nMETBins[7] + 1] {      450, 550, 650, 750, 1000 },
        new double[nMETBins[8] + 1] {      450, 550, 650, 750, 1000 },
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

    TString crWgtOther       =  TString::Format("%s*leptnpweightLM*lepvetoweightLM", stdMCWeight.Data());

    TString normSel          = TString::Format("%s && %s && %s && (nvetolep>0) && (mtlepmet<100)", METPresel.Data(), BaselineExtraCuts.Data(), SR_def.Data());
    TString normMCWT         = TString::Format("%s*leptnpweightLM", stdMCWeight.Data());

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

          if( (iS == CR_nb0_hgh_boost_low_nj_mj500) || (iS == CR_nb0_hgh_boost_hgh_nj_mj500) ){
            dataYields.push_back(hd);
            qcdYieldsWithVeto.push_back(hqwv);
            origQCDYields.push_back(origQCD);
            smearedQCDYields.push_back(smearedQCD);

            dataYields_noInt.push_back(hd_Int);
            qcdYieldsWithVeto_noInt.push_back(hqwv_Int);
            origQCDYields_noInt.push_back(origQCD_Int);
            smearedQCDYields_noInt.push_back(smearedQCD_Int);
            otherBKGScaledYields_noInt.push_back(oBKGSY_Int);

            double D_MC_ratio = hd_Int->GetBinContent(1) / (hqwv_Int->GetBinContent(1) + oBKGSY_Int->GetBinContent(1));
            for(unsigned int iB = 1; iB <= dataYields[iS]->GetNbinsX(); iB++){
              double temp_ratio = dataYields[iS]->GetBinContent(iB) / (qcdYieldsWithVeto[iS]->GetBinContent(iB) + otherBKGScaledYields[iS]->GetBinContent(iB));
              if(fabs(temp_ratio - D_MC_ratio) > integrating_met_systematic) integrating_met_systematic = fabs(temp_ratio - D_MC_ratio);
            }
          } else {
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
    enum                     SRReg{ SR_nb0_hgh_boost_low_nj, SR_nb0_hgh_boost_hgh_nj, SR_nb1_med_boost_low_pTb, SR_nb1_med_boost_med_pTb, SR_nb1_hgh_boost_low_pTb, SR_nb1_hgh_boost_med_pTb, SR_nb2_med_boost_low_pTb, SR_nb2_med_boost_med_pTb, SR_nb2_hgh_boost_low_pTb, SR_nb2_hgh_boost_med_pTb };
    TString  srRegBinNames[nSR] = { "nb0_highboost_lownj",   "nb0_highboost_highnj",  "nb1_medboost_lowptb",    "nb1_medboost_medptb",    "nb1_highboost_lowptb",   "nb1_highboost_medptb",   "nb2_medboost_lowptb",    "nb2_medboost_medptb",    "nb2_highboost_lowptb",   "nb2_highboost_medptb"   };
    TString srSel[nSR] = {
                       nb0 + " && " + hgh_boost + " && " + med_nj,
                       nb0 + " && " + hgh_boost + " && " + hgh_nj,
        mtb + " && " + nb1 + " && " + med_boost + " && " + low_pTb,
        mtb + " && " + nb1 + " && " + med_boost + " && " + med_pTb,
        mtb + " && " + nb1 + " && " + hgh_boost + " && " + low_pTb,
        mtb + " && " + nb1 + " && " + hgh_boost + " && " + med_pTb,
        mtb + " && " + nb2 + " && " + med_boost + " && " + low_pTb12,
        mtb + " && " + nb2 + " && " + med_boost + " && " + med_pTb12,
        mtb + " && " + nb2 + " && " + hgh_boost + " && " + low_pTb12,
        mtb + " && " + nb2 + " && " + hgh_boost + " && " + med_pTb12,
    };
    TString srSelLabels[nSR] = {
        "#splitline{" + nb0_R + ", " + hgh_boost_R + "}{"                + med_nj_R    + "}",
        "#splitline{" + nb0_R + ", " + hgh_boost_R + "}{"                + hgh_nj_R    + "}",
        "#splitline{" + nb1_R + ", " + med_boost_R + "}{" + mtb_R + ", " + low_pTb_R   + "}",
        "#splitline{" + nb1_R + ", " + med_boost_R + "}{" + mtb_R + ", " + med_pTb_R   + "}",
        "#splitline{" + nb1_R + ", " + hgh_boost_R + "}{" + mtb_R + ", " + low_pTb_R   + "}",
        "#splitline{" + nb1_R + ", " + hgh_boost_R + "}{" + mtb_R + ", " + med_pTb_R   + "}",
        "#splitline{" + nb2_R + ", " + med_boost_R + "}{" + mtb_R + ", " + low_pTb12_R + "}",
        "#splitline{" + nb2_R + ", " + med_boost_R + "}{" + mtb_R + ", " + med_pTb12_R + "}",
        "#splitline{" + nb2_R + ", " + hgh_boost_R + "}{" + mtb_R + ", " + low_pTb12_R + "}",
        "#splitline{" + nb2_R + ", " + hgh_boost_R + "}{" + mtb_R + ", " + med_pTb12_R + "}",
    };
    TString srSelNamesLatex[nSR] = {
                       nb0_L + ", " + hgh_boost_L + ", " + med_nj_L,
                       nb0_L + ", " + hgh_boost_L + ", " + hgh_nj_L,
        mtb_L + ", " + nb1_L + ", " + med_boost_L + ", " + low_pTb_L,
        mtb_L + ", " + nb1_L + ", " + med_boost_L + ", " + med_pTb_L,
        mtb_L + ", " + nb1_L + ", " + hgh_boost_L + ", " + low_pTb_L,
        mtb_L + ", " + nb1_L + ", " + hgh_boost_L + ", " + med_pTb_L,
        mtb_L + ", " + nb2_L + ", " + med_boost_L + ", " + low_pTb12_L,
        mtb_L + ", " + nb2_L + ", " + med_boost_L + ", " + med_pTb12_L,
        mtb_L + ", " + nb2_L + ", " + hgh_boost_L + ", " + low_pTb12_L,
        mtb_L + ", " + nb2_L + ", " + hgh_boost_L + ", " + med_pTb12_L,
    };
    TString srSelNames[nSR] = {
                       nb0_R + ", " + hgh_boost_R + ", " + med_nj_R,
                       nb0_R + ", " + hgh_boost_R + ", " + hgh_nj_R,
        mtb_R + ", " + nb1_R + ", " + med_boost_R + ", " + low_pTb_R,
        mtb_R + ", " + nb1_R + ", " + med_boost_R + ", " + med_pTb_R,
        mtb_R + ", " + nb1_R + ", " + hgh_boost_R + ", " + low_pTb_R,
        mtb_R + ", " + nb1_R + ", " + hgh_boost_R + ", " + med_pTb_R,
        mtb_R + ", " + nb2_R + ", " + med_boost_R + ", " + low_pTb12_R,
        mtb_R + ", " + nb2_R + ", " + med_boost_R + ", " + med_pTb12_R,
        mtb_R + ", " + nb2_R + ", " + hgh_boost_R + ", " + low_pTb12_R,
        mtb_R + ", " + nb2_R + ", " + hgh_boost_R + ", " + med_pTb12_R,
    };
    int nMETBins[nSR] = { 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 };
    double* metBins[nSR] = {
        new double[nMETBins[2] + 1] {      450, 550, 650, 750, 1000 },
        new double[nMETBins[3] + 1] {      450, 550, 650, 750, 1000 },
        new double[nMETBins[4] + 1] { 300, 400, 500, 600,      1000 },
        new double[nMETBins[5] + 1] { 300, 400, 500, 600,      1000 },
        new double[nMETBins[7] + 1] {      450, 550, 650, 750, 1000 },
        new double[nMETBins[8] + 1] {      450, 550, 650, 750, 1000 },
        new double[nMETBins[4] + 1] { 300, 400, 500, 600,      1000 },
        new double[nMETBins[5] + 1] { 300, 400, 500, 600,      1000 },
        new double[nMETBins[7] + 1] {      450, 550, 650, 750, 1000 },
        new double[nMETBins[8] + 1] {      450, 550, 650, 750, 1000 },
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
        CRegInfo::CR_nb0_hgh_boost_low_nj_mj500 ,
        CRegInfo::CR_nb0_hgh_boost_hgh_nj_mj500 ,
        CRegInfo::CR_nb1_med_boost_low_pTb_mj500,
        CRegInfo::CR_nb1_med_boost_med_pTb_mj500,
        CRegInfo::CR_nb1_hgh_boost_low_pTb_mj500,
        CRegInfo::CR_nb1_hgh_boost_med_pTb_mj500,
        CRegInfo::CR_nb2_med_boost_low_pTb_mj500,
        CRegInfo::CR_nb2_med_boost_med_pTb_mj500,
        CRegInfo::CR_nb2_hgh_boost_low_pTb_mj500,
        CRegInfo::CR_nb2_hgh_boost_med_pTb_mj500,
    };

    TString srPreQCD         = TString::Format("%s && %s && %s",                                   METPresel.Data(), BaselineExtraCuts.Data(), SR_def.Data());
    TString srPreQCDWithVeto = TString::Format("%s && %s && %s && (nvetolep==0) && (nvetotau==0)", METPresel.Data(), BaselineExtraCuts.Data(), SR_def.Data());

    TString processWeight(TString weight, int iSR) {return weight;}

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
          if( (iS != SR_nb0_hgh_boost_low_nj) && (iS != SR_nb0_hgh_boost_hgh_nj) ){
            int minBin = 0;
            int nBins = 2;
            double* tempMetBins = new double[nBins] { metBins[iS][0], metBins[iS][nMETBins[iS]] };
for(unsigned int iM = 0; iM < nBins; iM++){
  cout << tempMetBins[iM] << ", ";
} cout << endl;
            HistogramGetter* tempHG = new HistogramGetter("met", "met", "#slash{#it{E}}_{T} [GeV]", minBin + 1, tempMetBins);
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

        upN        ->Divide(upD);
        downN      ->Divide(downD);
        norN       ->Divide(norD);
        if(orN) orN->Divide(orD);

        origQCDYields.   push_back(origQCD);
        smearedQCDYields.push_back(smearedQCD);
        nominalQCD      .push_back(nom);
        nominal_N.       push_back(nomN);
        nominal_D.       push_back(nomD);
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
//      return TString::Format("%.3f $\\pm$ %.3f & %.3f $\\pm$ %.3f (stat.) $\\pm$ %.3f (syst.)",tf,tfUnc,pred,predstat,predsyst );
      return TString::Format("%.3f $\\pm$ %.3f & %.3f $\\pm$ %.3f & %.3f $\\pm$ %.3f & %.3f $\\pm$ %.3f (stat.) $\\pm$ %.3f (syst.)",getOrigQCDYields_OOB(reg,metBin),getOrigQCDYields_OOB_StatUnc(reg,metBin),getSmearedQCDYields_OOB(reg,metBin),getSmearedQCDYields_OOB_StatUnc(reg,metBin),tf,tfUnc,pred,predstat,predsyst );
    }
  };
};

#endif
