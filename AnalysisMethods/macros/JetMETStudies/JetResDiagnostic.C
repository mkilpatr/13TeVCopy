//<     temp_canvas->Update();



#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/Plot.hh"
#include "AnalysisMethods/PlotUtils/interface/PlotTools.hh"

#include "TreeReadingHelper.h"
#include "TDirectory.h"
#include "TTree.h"
#include "TFile.h"
#include <iostream>

using namespace std;

TString outputDir = "diagnostics";
int xCanvasSize = 1000;
int yCanvasSize = 1000;

void getDiagnostic(TString p_FileDir, vector <pair <TString, TString> >& p_FileNames_Descs, TString p_HistName, TString p_Suffix, TString p_xTitle, vector <float> p_JetPTs, int p_nXDivisions, bool p_savePlots = false){
  vector<TFile*> files;
  for(unsigned int iF = 0; iF < p_FileNames_Descs.size(); ++iF){
    files.push_back(new TFile(p_FileDir + p_FileNames_Descs[iF].first, "read"));
  }
  int nJetPTs = p_JetPTs.size() - 1;
  int nYDivisions = int(nJetPTs / p_nXDivisions) + ( (nJetPTs % p_nXDivisions == 0) ? 0 : 1);
  TString yTitle = "Arbitrary Units";
  TString tempName = p_HistName;
  if(p_Suffix.Sizeof() > 1) tempName += "_" + p_Suffix;
  TString canvas_name      = TString::Format("%s_diagnostic.png", tempName.Data());
  system(TString::Format("convert -size %ux%u xc:skyblue %s", xCanvasSize * p_nXDivisions, yCanvasSize * nYDivisions, canvas_name.Data()).Data());

  unsigned int hist_num = 1;
  if(tempName.Contains("_b")){
    hist_num  = nJetPTs + 1;
  }
  for(unsigned int iP = 0; iP < nJetPTs; ++iP, ++hist_num){
    float yMin = 9999., yMax = 0.;
    TString title = TString::Format("%.0f #leq p_{T} < %.0f", p_JetPTs[iP], p_JetPTs[iP + 1]);
    TString name  = TString::Format("%s_%u", p_HistName.Data(), hist_num);
    Plot* plot = new Plot(name, title, p_xTitle, yTitle);
    for(unsigned int iF = 0; iF < p_FileNames_Descs.size(); ++iF){
      int color = iF + 2;
      if(color >= 10) color++;
      TH1F* hist = (TH1F*)files[iF]->Get(name);
      TH1F* hist_clone = (TH1F*)hist->Clone(name + "_clone");
      StyleTools::InitHist(hist_clone, p_xTitle, yTitle, color, 0);
      hist_clone->SetTitle(title);
      if(yMin > hist_clone->GetMinimum()) yMin = hist_clone->GetMinimum();
      if(yMax < hist_clone->GetMaximum()) yMax = hist_clone->GetMaximum();
      if(iF == 0){
        plot->addHist(        hist_clone, p_FileNames_Descs[iF].second, "", color, 0, color, 1, 0, 0);
      } else {
        plot->addHistForRatio(hist_clone, p_FileNames_Descs[iF].second, "", color, 0, color, 1, 0, 0);
      }
    }
    TCanvas* canvas = StyleTools::MakeCanvas(TString::Format("%s_%u", tempName.Data(), iP + 1), TString::Format("%s for p_{T} range {%.0f, %.0f)", p_xTitle.Data(), p_JetPTs[iP], p_JetPTs[iP + 1]), xCanvasSize, yCanvasSize);
    canvas->cd();
    plot->setLegend(.45, .65, .92, .9);
    if(!p_HistName.Contains("_comp")){
      plot->setLogy();
      plot->setYRange(0.1 * yMin, 1000. * yMax);
    } else {
      plot->setYRange(0., 1.3 * yMax);
    }
    if(p_FileNames_Descs.size() == 1){
      plot->draw(canvas);
    } else {
      plot->drawRatios(canvas);
    }
    canvas->SaveAs(TString::Format("%s_%u.png", tempName.Data(), iP + 1));
    system(TString::Format("composite -geometry +%u+%u %s_%u.png %s %s", xCanvasSize * (iP % p_nXDivisions), yCanvasSize * int(iP / p_nXDivisions), tempName.Data(), iP + 1, canvas_name.Data(), canvas_name.Data()).Data());
    if(p_savePlots){
      system(TString::Format("mv %s_%u.png %s/.", tempName.Data(), iP + 1, outputDir.Data()));
    } else {
      system(TString::Format("rm %s_%u.png", tempName.Data(), iP + 1));
    }
  }
  system(TString::Format("mv %s %s/%s", canvas_name.Data(), outputDir.Data(), canvas_name.Data()).Data());
}

#endif

void JetResDiagnostic() {
  StyleTools::SetStyle();
  TString fileDir = "";
//  vector <pair <TString, TString> > fileNames = { make_pair("resTailOut_ORIGINAL.root",          "Nick's original"),
//                                                  make_pair("resTailOut_orig_puWeight_WoH.root", "My original") };
//  TString suffix = "puWeight_ORIGINAL";
//  vector <pair <TString, TString> > fileNames = { make_pair("resTailOut_ORIGINAL.root",          "Nick's original"),
//                                                  make_pair("resTailOut_prev_puWeight_WoH.root", "My original") };
//  TString suffix = "puWeight_ORIGINALS";
//  vector <pair <TString, TString> > fileNames = { make_pair("resTailOut_orig_puWeight_WoH.root",     "original"),
//                                                  make_pair("resTailOut_plus_puWeight_WoH.root",     "new"),
//                                                  make_pair("resTailOut_combined_puWeight_WoH.root", "combined") };
//  TString suffix = "puWeight";
//  vector <pair <TString, TString> > fileNames = { make_pair("resTailOut_orig_puWeight_weight_WoH.root",     "original"),
//                                                  make_pair("resTailOut_plus_puWeight_weight_WoH.root",     "new"),
//                                                  make_pair("resTailOut_combined_puWeight_weight_WoH.root", "combined") };
//  TString suffix = "puWeight_weight";
//  vector <pair <TString, TString> > fileNames = { make_pair("resTailOut_combined_puWeight_WoH.root",        "puWeight"),
//                                                  make_pair("resTailOut_combined_puWeight_weight_WoH.root", "puWeight*weight") };
//  TString suffix = "puWeight_vs_puWeight_weight";
//  vector <pair <TString, TString> > fileNames = { make_pair("resTailOut_combined_puWeight_WoH.root",          "no filters"),
//                                                  make_pair("resTailOut_combined_filtered_puWeight_WoH.root", "filtered") };
//  TString suffix = "puWeight_noFilter_vs_filtered";
  vector <pair <TString, TString> > fileNames = { /*make_pair("resTailOut_combined_filtered_puWeight_weight_WoH.root",      "no CHEF cut"),*/
                                                  make_pair("resTailOut_combined_filtered_CHEF_puWeight_weight_WoH.root", "CHEF > 0.1") };
  TString suffix = "noCHEF_vs_CHEF";
//  vector <pair <TString, TString> > fileNames = { make_pair("resTailOut_combined_filtered_CHEF_puWeight_weight_WoH.root",    "CHEF > 0.1"),
//                                                  make_pair("resTailOut_combined_filtered_newCHEF_puWeight_weight_WoH.root", "0.1 < CHEF < 0.95") };
//  TString suffix = "CHEF_vs_newCHEF";

  outputDir += "_" + suffix;
  gSystem->mkdir(outputDir, true);
//  vector <float> jetPTs_1 = { 0, 50, 75, 100, 125, 150, 200, 250, 300, 400, 500, 700, 1000, 1500, 4000 };
//  getDiagnostic(fileDir, fileNames, "res_qcd", suffix, "p_{T,reco}/p_{T,gen}", jetPTs_1, 3, true);
  vector <float> jetPTs_2 = { 0, 50, 75, 100, 125, 150, 200, 250, 300, 400, 500, 700, 1000, 4000 };
  getDiagnostic(fileDir, fileNames, "res_light",      suffix, "p_{T,reco}/p_{T,gen}", jetPTs_2, 3, true);
  getDiagnostic(fileDir, fileNames, "res_light_comp", suffix, "p_{T,reco}/p_{T,gen}", jetPTs_2, 3, true);
  getDiagnostic(fileDir, fileNames, "res_b",          suffix, "p_{T,reco}/p_{T,gen}", jetPTs_2, 3, true);
  getDiagnostic(fileDir, fileNames, "res_b_comp",     suffix, "p_{T,reco}/p_{T,gen}", jetPTs_2, 3, true);
}
