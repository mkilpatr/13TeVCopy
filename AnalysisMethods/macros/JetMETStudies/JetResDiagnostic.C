#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/Plot.hh"
#include "AnalysisMethods/PlotUtils/interface/PlotTools.hh"

#include "../TreeReadingHelper.h"
#include "TDirectory.h"
#include "TTree.h"
#include "TFile.h"
#include <iostream>

using namespace std;

TString outputDir = "diagnostics";
int xCanvasSize = 500;
int yCanvasSize = 500;

void getDiagnostic(TFile* p_File, TString p_HistName, TString p_Suffix, TString p_xTitle, vector <float> p_JetPTs, int p_nXDivisions){
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
    int color = iP + 1;
    if(color >= 10) color++;
    TString title = TString::Format("%.0f-%.0f", p_JetPTs[iP], p_JetPTs[iP + 1]);
    TH1F* hist = (TH1F*)p_File->Get(TString::Format("%s_%u", p_HistName.Data(), hist_num));
    TH1F* hist_clone = (TH1F*)hist->Clone(TString(hist->GetName()) + "_clone");
    StyleTools::InitHist(hist_clone, p_xTitle, yTitle, color, 0);
    TCanvas* temp_canvas = StyleTools::MakeCanvas(TString::Format("%s_%u", tempName.Data(), iP + 1), TString::Format("%s for p_{T} range {%.0f, %.0f)", p_xTitle.Data(), p_JetPTs[iP], p_JetPTs[iP + 1]), xCanvasSize, yCanvasSize);
    temp_canvas->cd();
    hist_clone->SetTitle(title);
    hist_clone->Draw();
    temp_canvas->Update();
    temp_canvas->SaveAs(TString::Format("%s_%u.png", tempName.Data(), iP + 1));
    system(TString::Format("composite -geometry +%u+%u %s_%u.png %s %s", xCanvasSize * (iP % p_nXDivisions), yCanvasSize * int(iP / p_nXDivisions), tempName.Data(), iP + 1, canvas_name.Data(), canvas_name.Data()).Data());
    system(TString::Format("rm %s_%u.png", tempName.Data(), iP + 1));
  }
  system(TString::Format("mv %s %s/%s", canvas_name.Data(), outputDir.Data(), canvas_name.Data()).Data());
}

#endif

void JetResDiagnostic(TString p_FileName = "../resTailOut.root", TString p_Suffix = "weight") {
  StyleTools::SetStyle();
  gSystem->mkdir(outputDir, true);
  p_FileName.Insert(p_FileName.Sizeof() - 6, (p_Suffix.Sizeof() == 1 ? "" : "_" + p_Suffix));
  TFile* inFile = new TFile(p_FileName, "read");

  vector <float> jetPTs_1 = { 0, 50, 75, 100, 125, 150, 200, 250, 300, 400, 500, 700, 1000, 1500, 4000 };
  getDiagnostic(inFile, "res_qcd", p_Suffix, "p_{T,reco}/p_{T,gen}", jetPTs_1, 3);
  vector <float> jetPTs_2 = { 0, 50, 75, 100, 125, 150, 200, 250, 300, 400, 500, 700, 1000, 4000 };
  getDiagnostic(inFile, "res_light",      p_Suffix, "p_{T,reco}/p_{T,gen}", jetPTs_2, 3);
  getDiagnostic(inFile, "res_light_comp", p_Suffix, "p_{T,reco}/p_{T,gen}", jetPTs_2, 3);
  getDiagnostic(inFile, "res_b",          p_Suffix, "p_{T,reco}/p_{T,gen}", jetPTs_2, 3);
  getDiagnostic(inFile, "res_b_comp",     p_Suffix, "p_{T,reco}/p_{T,gen}", jetPTs_2, 3);
}
