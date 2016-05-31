/*
 * Znunu.C
 *
 *  Created on: Sep 23, 2015
 *      Author: hqu
 */

#include <fstream>
#include "ValidationRegionParameters.hh"

using namespace EstTools;

void BkgValidation(){
  auto phocfg = phoConfig();
  ZnunuEstimator z(phocfg);
  z.zllcr_cfg = zllConfig();
  z.zll_normMap = normMap;
  z.phocr_categories = phoCatMap();
  z.phocr_normMap = phoNormMap;
  z.pred();
  z.printYields();

  auto llbcfg = lepConfig();
  LLBEstimator l(llbcfg);
  l.lepcr_categories = lepCatMap();
  l.pred();
  l.printYields();

  auto qcdcfg = qcdConfig();
  QCDEstimator q(qcdcfg);
  q.pred();
  q.naiveTF();
  q.printYields();

  vector<TH1*> pred;
  pred.push_back(convertToHist(q.yields.at("_pred"),"qcd_pred",";Signal Regions;Events"));
  pred.push_back(convertToHist(l.yields.at("_pred"),"ttbarplusw_pred",";Signal Regions;Events"));
  pred.push_back(convertToHist(z.yields.at("_pred"),"znunu_pred",";Signal Regions;Events"));

  vector<TH1*> mc;
  mc.push_back(convertToHist(q.yields.at("qcd-sr-withveto"),"qcd_mc",";Signal Regions;Events"));
  mc.push_back(convertToHist(l.yields.at("ttbarplusw-sr"),"ttbarplusw_mc",";Signal Regions;Events"));
  mc.push_back(convertToHist(z.yields.at("znunu"),"znunu_mc",";Signal Regions;Events"));

  auto sigcfg = sigConfig();
  BaseEstimator s(sigcfg);
  s.calcYields();
  vector<TH1*> sig;
  sig.push_back(convertToHist(s.yields.at("T2fbd_375_295"),"T2fbd_375_295",";Signal Regions;Events"));
  sig.push_back(convertToHist(s.yields.at("T2fbd_375_325"),"T2fbd_375_325",";Signal Regions;Events"));
  sig.push_back(convertToHist(s.yields.at("T2fbd_375_355"),"T2fbd_375_355",";Signal Regions;Events"));

  auto hdata = convertToHist(s.yields.at("data-sr"),"data",";Signal Regions;Events");

  vector<TString> bkglabels = {"QCD", "t#bar{t}/W", "Z#rightarrow#nu#nu"};
  vector<TString> siglabels = {"T2-4bd(375,295)", "T2-4bd(375,325)", "T2-4bd(375,355)"};
  vector<TString> datalabel = {"Data"};
  vector<Color_t> sigcolors = {kViolet-1, kBlue, kRed+3};
//  for (auto *h : sig) h->SetLineStyle(kDashed);

  {
    // plot pred and sig
    prepHists(pred, false, false, true);
    prepHists({hdata}, false, false, false, {kBlack});
//    pred[0]->SetLineColor(kOrange);
//    pred[0]->SetFillColor(kOrange);
//    pred[0]->SetFillStyle(1001);
//    pred[1]->SetLineColor(kRed+2);
//    pred[1]->SetFillColor(kRed+2);
//    pred[1]->SetFillStyle(1001);
//    pred[2]->SetLineColorAlpha(kGray+2,0.8);
//    pred[2]->SetFillColorAlpha(kGray+2,0.8);
//    pred[2]->SetFillStyle(1001);

//    prepHists(sig, false, false);

    auto leg = prepLegends(pred, bkglabels, "F");
    appendLegends(leg, {hdata}, datalabel, "LP");
//    appendLegends(leg, sig, siglabels);
    leg->SetTextSize(0.03);
//    leg->SetNColumns(2);
    leg->SetY1NDC(leg->GetY2NDC() - 0.2);
    auto c = drawStackAndRatio(pred, hdata, leg, "N_{obs}/N_{exp}", 0.001, 2.999);
    c->Print(s.config.outputdir+"/validation.pdf");
    c->Print(s.config.outputdir+"/validation.C");
  }

//  {
//    // plot pred and sig
//    prepHists(mc, false, false, true);
////    mc[0]->SetLineColor(kOrange);
////    mc[0]->SetFillColor(kOrange);
////    mc[0]->SetFillStyle(1001);
////    mc[1]->SetLineColor(kRed+2);
////    mc[1]->SetFillColor(kRed+2);
////    mc[1]->SetFillStyle(1001);
////    mc[2]->SetLineColorAlpha(kGray+2,0.8);
////    mc[2]->SetFillColorAlpha(kGray+2,0.8);
////    mc[2]->SetFillStyle(1001);
//
//
//    prepHists(sig, false, false, false, sigcolors);
//
//    auto leg = prepLegends(mc, bkglabels, "F");
//    appendLegends(leg, sig, siglabels);
//    leg->SetTextSize(0.03);
//    leg->SetNColumns(2);
//    leg->SetY1NDC(leg->GetY2NDC()-0.12);
//    auto c = drawStack(mc, sig, leg);
//    c->Print(s.config.outputdir+"/mc.pdf");
//    c->Print(s.config.outputdir+"/mc.C");
//
//  }
//

  TFile *output = new TFile(sigcfg.outputdir+"/validation.root", "RECREATE");
  for (auto *h : pred) h->Write();
  for (auto *h : mc)   h->Write();
  for (auto *h : sig)  h->Write();
  output->Close();

}
