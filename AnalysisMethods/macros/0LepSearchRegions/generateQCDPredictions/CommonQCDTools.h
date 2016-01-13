#ifndef COMMON_H
#define COMMON_H

#include "AnalysisMethods/macros/0LepSearchRegions/generateQCDPredictions/RegionInfo.h"
#include "TLegend.h"
#include "TFitResult.h"

extern TString outputdir;
extern TString format;
extern TString dataSetName;
extern bool run_on_htmht;
//const int numMET_bins = 8;
////const int numMET_bins_used = 8;
////double MET_bins[numMET_bins] = { 200., 225., 250., 300., 400., 500., 600., 1000.};
const TString intLum = "2.14";
const int numMET_bins = 10;
const int numMET_bins_used = 10;
double MET_bins[numMET_bins] = { 150., 175., 200., 250., 300., 400., 500., 600., 800., 1000. };
//vector<int> multiFitColors = { kBlack, kRed + 1, kBlue + 1, kGreen + 3, kGreen -3, kOrange + 1, kOrange - 1, kViolet };
vector<int> multiFitColors = { kBlack, kRed + 1, kGreen + 3, kGreen -3, kOrange + 1, kOrange - 1, kViolet };

void generateSampleInfo(vector <TString>& p_SampleNames, vector <TString>& p_SampleLabels, vector <int>& p_SampleColors, TString p_ConfFile);
void readRegionInfoFile(map <string, vector <cutInfo> >& p_CutInfoMap, TString p_RegionConfFile);
void generateRegionInfoMap(map <TString, RegionInfo>& p_RegionInfoMap, map <string, vector <cutInfo> >& p_CutInfoMap);
void getQCDScaleFactors(float& p_SF_MM, float& p_SF_MM_E, float& p_SF_nMM, float& p_SF_nMM_E, TString p_SF_FileName);
TH1F* makeFitHist(TF1* p_Function, TH1F* p_Hist, TString p_Name);
TH1F* addConfidenceLevelsToFit(TH1F *p_Hist, TF1 *p_Fit, double *p_Matrix, double p_ConfidenceLevel = 0.95);
void createStackPlot(TString p_Name, TString p_Title, vector<TString> p_SampleNames, vector<TString> p_SampleLabels, vector<int> p_SampleColors, vector<TH1F*> p_Hists);
void createStackPlotWithRatio(TString p_Name, TString p_Title, vector<TString> p_SampleNames, vector<TString> p_SampleLabels, vector<int> p_SampleColors, vector<TH1F*> p_Hists, int p_nBins = 0, double* p_Bins = 0);
void PrintCanvas(TString p_Name, TString p_Title, TH1F* p_Hist, TString p_Path, float p_TF = -1, bool p_LogY = false, bool p_Write = true);
void PrintCanvasWithFitError(TString p_Name, TString p_Title, TH1F* p_Hist, TH1F* p_FitWithErrorHist, TString p_Path, float p_TF = -1, bool p_LogY = false, bool p_Write = true);
void PrintMultiFitCanvas(TString p_Name, TString p_Title, vector <pair <TH1F*, TString> >& p_PlotNamesDesc, TString p_FitName, bool p_LogY = false);
void PrintMultiCanvas(TString p_Name, TString p_Title, vector <pair <TH1F*, TString> >& p_PlotNamesDesc, TString p_FitName = "", bool p_LogY = false);
void MyInitHist(TH1 *hist, const char *xtit, const char *ytit, int color, int style);
void MySetLegendStyle(TLegend* leg);

void generateSampleInfo(vector <TString>& p_SampleNames, vector <TString>& p_SampleLabels, vector <int>& p_SampleColors, TString p_ConfFile) {
  string tempString, delimiter;
  fstream samplesFile(p_ConfFile, ios::in);
  if(!samplesFile.good()){
    cout << "could not open the configuration file" << endl;
    assert(0);
  }
  while(getline(samplesFile, tempString)){
    if(tempString[0] == '#') continue;
    if(tempString[0] == '%') continue;
    if(tempString[0] == '$') {
      istringstream input(tempString);
      TString sampleName;
      TString sampleLabel;
      int sampleColor;
      input >> delimiter >> sampleName >> sampleColor >> sampleLabel;
      p_SampleNames.push_back(sampleName);
      p_SampleLabels.push_back(sampleLabel.Remove(0, 1));
      p_SampleColors.push_back(sampleColor);
      continue;
    }
  }
}

void readRegionInfoFile(map <string, vector <cutInfo> >& p_CutInfoMap, TString p_RegionConfFile) {
  string tempString, delimiter, cut_ID, info;
  fstream regionFile(p_RegionConfFile, ios::in);
  if(!regionFile.good()){
    cout << "could not open the configuration file" << endl;
    assert(0);
  }
  while(getline(regionFile, tempString)){
    if(tempString[0] == '#') continue;
    if(tempString[0] == '%') continue;
    if(tempString[0] == '@') {
      istringstream input_ID(tempString);
      input_ID >> delimiter >> cut_ID;
//      cout << cut_ID << endl;
      while(getline(regionFile, tempString)){
        if(tempString[0] == '@') break;
        else if(tempString[0] == '#') continue;
//        cout << tempString << endl;
        istringstream input(tempString);
        vector <TString> stringSegments;
        while(getline(input, info, ';')){
          stringSegments.push_back(info);
        }
//        for(int i = 0; i < stringSegments.size(); i++){
//          cout << stringSegments[i] << ", ";
//        } cout << endl;
        p_CutInfoMap[cut_ID].push_back(cutInfo(stringSegments[0], stringSegments[1], stringSegments[2], stringSegments[3]));
      }
      continue;
    }
  }
}

void generateRegionInfoMap(map <TString, RegionInfo>& p_RegionInfoMap, map <string, vector <cutInfo> >& p_CutInfoMap) {
  RegionInfo tempInfo;
  if(run_on_htmht){
    tempInfo.CR.common_CI.defn += " && passdijetmet";
  }
  for(int i_common = 0; i_common < p_CutInfoMap["common_CR"].size(); i_common++){
    for(int i_MET = 0; i_MET < p_CutInfoMap["MET_Cut_CR"].size(); i_MET++){
      for(int i_Nj = 0; i_Nj < p_CutInfoMap["nJets_Cut_CR"].size(); i_Nj++){
        for(int i_MT = 0; i_MT < p_CutInfoMap["MT_Cut_CR"].size(); i_MT++){
          for(int i_hT = 0; i_hT < p_CutInfoMap["hT_Cut_CR"].size(); i_hT++){
            for(int i_dPhi12 = 0; i_dPhi12 < p_CutInfoMap["dPhi12_Cut_CR"].size(); i_dPhi12++){
              for(int i_dPhi3 = 0; i_dPhi3 < p_CutInfoMap["dPhi3_Cut_CR"].size(); i_dPhi3++){
                for(int i_Nj75 = 0; i_Nj75 < p_CutInfoMap["nJets75_Cut_CR"].size(); i_Nj75++){
                  for(int i_Nbl = 0; i_Nbl < p_CutInfoMap["nlBjets_Cut_CR"].size(); i_Nbl++){
                    for(int i_Nb = 0; i_Nb < p_CutInfoMap["nBjets_Cut_CR"].size(); i_Nb++){
                      for(int i_Nt = 0; i_Nt < p_CutInfoMap["nTtags_Cut_CR"].size(); i_Nt++){
                        tempInfo.CR.common_CI  = p_CutInfoMap["common_CR"][i_common];
                        tempInfo.CR.MET_CI     = p_CutInfoMap["MET_Cut_CR"][i_MET];
                        tempInfo.CR.nJets_CI   = p_CutInfoMap["nJets_Cut_CR"][i_Nj];
                        tempInfo.CR.MT_CI      = p_CutInfoMap["MT_Cut_CR"][i_MT];
                        tempInfo.CR.hT_CI      = p_CutInfoMap["hT_Cut_CR"][i_hT];
                        tempInfo.CR.dPhi12_CI  = p_CutInfoMap["dPhi12_Cut_CR"][i_dPhi12];
                        tempInfo.CR.dPhi3_CI   = p_CutInfoMap["dPhi3_Cut_CR"][i_dPhi3];
                        tempInfo.CR.nJets75_CI = p_CutInfoMap["nJets75_Cut_CR"][i_Nj75];
                        tempInfo.CR.nlBjets_CI = p_CutInfoMap["nlBjets_Cut_CR"][i_Nbl];
                        tempInfo.CR.nBjets_CI  = p_CutInfoMap["nBjets_Cut_CR"][i_Nb];
                        tempInfo.CR.nTtags_CI  = p_CutInfoMap["nTtags_Cut_CR"][i_Nt];
                        tempInfo.uniqueName = "CR_" + tempInfo.CR.getName();
                        if(p_CutInfoMap["nBjets_Cut_TR"].size() == 0){
                            p_RegionInfoMap[tempInfo.uniqueName] = tempInfo;
                            cout << tempInfo.uniqueName << endl;
                            cout << p_RegionInfoMap[tempInfo.uniqueName].getDefn_CR() << endl;
                        }
                        for(int j_nBjets = 0; j_nBjets < p_CutInfoMap["nBjets_Cut_TR"].size(); j_nBjets++){
                          tempInfo.uniqueName = "CR_" + tempInfo.CR.getName() + "_TR" + p_CutInfoMap["nBjets_Cut_TR"][j_nBjets].name;
                          tempInfo.TR.common_CI  = tempInfo.CR.common_CI;
                          tempInfo.TR.MET_CI     = tempInfo.CR.MET_CI;
                          tempInfo.TR.nJets_CI   = tempInfo.CR.nJets_CI;
                          tempInfo.TR.MT_CI      = tempInfo.CR.MT_CI;
                          tempInfo.TR.hT_CI      = tempInfo.CR.hT_CI;
                          tempInfo.TR.dPhi12_CI  = p_CutInfoMap["dPhi12_Cut_TR"][0];
                          tempInfo.TR.dPhi3_CI   = tempInfo.CR.dPhi3_CI;
                          tempInfo.TR.nJets75_CI = tempInfo.CR.nJets75_CI;
                          tempInfo.TR.nlBjets_CI = tempInfo.CR.nlBjets_CI;
                          tempInfo.TR.nBjets_CI  = p_CutInfoMap["nBjets_Cut_TR"][j_nBjets];
                          tempInfo.TR.nTtags_CI  = tempInfo.CR.nTtags_CI;
                          tempInfo.SR.common_CI  = tempInfo.CR.common_CI;
                          tempInfo.SR.MET_CI     = tempInfo.TR.MET_CI;
                          tempInfo.SR.nJets_CI   = tempInfo.TR.nJets_CI;
                          tempInfo.SR.MT_CI      = tempInfo.TR.MT_CI;
                          tempInfo.SR.hT_CI      = tempInfo.TR.hT_CI;
                          tempInfo.SR.dPhi12_CI  = p_CutInfoMap["dPhi12_Cut_SR"][0];
                          tempInfo.SR.dPhi3_CI   = tempInfo.TR.dPhi3_CI;
                          tempInfo.SR.nJets75_CI = tempInfo.TR.nJets75_CI;
                          tempInfo.SR.nlBjets_CI = tempInfo.TR.nlBjets_CI;
                          tempInfo.SR.nBjets_CI  = tempInfo.TR.nBjets_CI;
                          tempInfo.SR.nTtags_CI  = tempInfo.TR.nTtags_CI;
                          p_RegionInfoMap[tempInfo.uniqueName] = tempInfo;
                          cout << tempInfo.uniqueName << endl;
                          cout << p_RegionInfoMap[tempInfo.uniqueName].getDefn_CR() << endl;
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

void getQCDScaleFactors(float& p_SF_MM, float& p_SF_MM_E, float& p_SF_nMM, float& p_SF_nMM_E, TString p_SF_FileName) {
  string tempString, delimiter;
  fstream SF_File(p_SF_FileName, ios::in);
  if(!SF_File.good()){
    cout << "could not open the configuration file: " << p_SF_FileName << endl;
    assert(0);
  }
  while(getline(SF_File, tempString)){
    if(tempString[0] != 'S') continue;
    istringstream input(tempString);
    input >> delimiter;
    if(delimiter == "SnMM"){
      input >> delimiter >> p_SF_nMM >> delimiter >> p_SF_nMM_E;
    } else if(delimiter == "SMM"){
      input >> delimiter >> p_SF_MM >> delimiter >> p_SF_MM_E;
    }
    continue;
  }
}

TH1F* makeFitHist(TF1* p_Function, TH1F* p_Hist, TString p_Name) {
  TH1F* fit_Hist = (TH1F*)p_Hist->Clone(p_Name);
  for(int x_bin = 1; x_bin <= fit_Hist->GetNbinsX() + 1; ++x_bin){
    fit_Hist->SetBinContent(x_bin, p_Function->Eval(fit_Hist->GetXaxis()->GetBinCenter(x_bin)));
  }
  return fit_Hist;
}

TH1F* addConfidenceLevelsToFit(TH1F *p_Hist, TF1 *p_Fit, double *p_Matrix, double p_ConfidenceLevel) {
  TH1F* fitError_Hist = (TH1F*)p_Hist->Clone(TString(p_Hist->GetName()) + "_FitErr");
  int nParameters = p_Fit->GetNpar();
  double *gradient  = new double[nParameters];
  double *sumVector = new double[nParameters];
  double x_info[3] = {0.,0.,0};
  double studentQuantile = TMath::StudentQuantile(0.5 + p_ConfidenceLevel / 2, p_Fit->GetNDF());
  double chi2_div_ndf    = TMath::Sqrt(p_Fit->GetChisquare() / p_Fit->GetNDF());
  double c = 0;
  for(int x_bin = 1; x_bin <= fitError_Hist->GetNbinsX() + 1; x_bin++){
    x_info[0] = fitError_Hist->GetXaxis()->GetBinCenter(x_bin);
    p_Fit->GradientPar(x_info, gradient);
    for(int i_row = 0; i_row < nParameters; i_row++){
      sumVector[i_row] = 0;
      for(int i_col = 0; i_col < nParameters; i_col++){
        sumVector[i_row] += p_Matrix[i_row * nParameters + i_col] * gradient[i_col];
      }
    }
    c = 0;
    for(int i = 0; i < nParameters; i++){
      c += gradient[i] * sumVector[i];
    }
    c = TMath::Sqrt(c);
    fitError_Hist->SetBinContent(x_bin, p_Fit->EvalPar(x_info));
    fitError_Hist->SetBinError(x_bin, c * studentQuantile * chi2_div_ndf);
  }
  delete [] gradient;
  delete [] sumVector;
  return fitError_Hist;
}

void createStackPlot(TString p_Name, TString p_Title, vector<TString> p_SampleNames, vector<TString> p_SampleLabels, vector<int> p_SampleColors, vector<TH1F*> p_Hists) {
  TCanvas* hist_Can = StyleTools::MakeCanvas(p_Name + "_Canvas", p_Title + " canvas", 600, 600);
  StyleTools::SetStyle();
  THStack* stack = new THStack(p_Name + "_stack_name", p_Title + " Stack title");
  TLegend* leg = new TLegend(0.6, 0.9 - .04 * p_SampleLabels.size(), 0.95, 0.9);
  SetLegendStyle(leg);
  leg->SetTextSize(.03);
  int dataPosition;
  float maxValue = 0;
  vector<TH1F*> hist_Clones;
  for(int i = 0; i < p_Hists.size(); i++){
    if(maxValue < p_Hists[i]->GetMaximum()) maxValue = p_Hists[i]->GetMaximum();
    if(p_SampleNames[i] == dataSetName){
      dataPosition = i;
    }
    hist_Clones.push_back((TH1F*)p_Hists[i]->Clone());
  }
  for(int i = 0; i < hist_Clones.size(); i++){
    if(p_SampleNames[i] == dataSetName){
      InitHist(hist_Clones[i], hist_Clones[i]->GetXaxis()->GetTitle(), hist_Clones[i]->GetYaxis()->GetTitle(), p_SampleColors[i], 0);
      leg->AddEntry(hist_Clones[i], p_SampleLabels[i]);
      continue;
    }
    InitHist(hist_Clones[i], hist_Clones[i]->GetXaxis()->GetTitle(), hist_Clones[i]->GetYaxis()->GetTitle(), p_SampleColors[i], 1001);
    leg->AddEntry(hist_Clones[i], p_SampleLabels[i], "F");
    stack->Add(hist_Clones[i]);
  }
  hist_Can->cd();
  stack->Draw("hist");
  stack->SetMaximum(1.5 * maxValue);
  stack->SetTitle(p_Title);
  stack->GetXaxis()->SetTitle(hist_Clones[dataPosition]->GetXaxis()->GetTitle());
  stack->GetYaxis()->SetTitle(hist_Clones[dataPosition]->GetYaxis()->GetTitle());
  stack->Draw("hist");
  hist_Clones[dataPosition]->Draw("same");
  leg->Draw("same");
  hist_Can->RedrawAxis();
  hist_Can->Print(outputdir + "/" + p_Name + "." + format);
}

void createStackPlotWithRatio(TString p_Name, TString p_Title, vector<TString> p_SampleNames, vector<TString> p_SampleLabels, vector<int> p_SampleColors, vector<TH1F*> p_Hists, int p_nBins, double* p_Bins) {
  TCanvas* hist_Can = StyleTools::MakeCanvas(p_Name + "_Canvas", p_Title + " canvas", 600, 600);
  StyleTools::SetStyle();
  hist_Can->cd();
  TPad* hist_Can_pad1 = new TPad("can_pad1", "can_pad1", 0, 0.3, 1, 1);
  hist_Can_pad1->SetLeftMargin  (0.18);
  hist_Can_pad1->SetTopMargin   (0.08);
  hist_Can_pad1->SetRightMargin (0.07);
  hist_Can_pad1->SetBottomMargin(0.01);
  hist_Can_pad1->Draw();
  hist_Can->cd();
  TPad* hist_Can_pad2 = new TPad("can_pad2", "can_pad2", 0, 0, 1, 0.3);
  hist_Can_pad2->SetLeftMargin  (0.18);
  hist_Can_pad2->SetTopMargin   (0.00);
  hist_Can_pad2->SetRightMargin (0.07);
  hist_Can_pad2->SetBottomMargin(0.35);
  hist_Can_pad2->SetGridy(1);
  hist_Can_pad2->Draw();
  hist_Can->cd();
  THStack* stack = new THStack(p_Name + "_stack_name", p_Title + " Stack title");
  TLegend* leg = new TLegend(0.6, 0.9 - .04 * p_SampleLabels.size(), 0.95, 0.9);
  SetLegendStyle(leg);
  leg->SetTextSize(.03);
  int dataPosition;
  float maxValue = 0;
  vector<TH1F*> hist_Clones;
  TH1F *frac, *mc_sum;
  for(int i = 0; i < p_Hists.size(); i++){
    if(maxValue < p_Hists[i]->GetMaximum()) maxValue = p_Hists[i]->GetMaximum();
    if(p_SampleNames[i] == dataSetName){
      frac = (TH1F*)p_Hists[i]->Clone(p_Name + "_Fraction");
      dataPosition = i;
    }
    hist_Clones.push_back((TH1F*)p_Hists[i]->Clone());
  }
  InitHist(frac, frac->GetXaxis()->GetTitle(), frac->GetYaxis()->GetTitle(), p_SampleColors[dataPosition], 0);
  if(p_nBins == 0){
    mc_sum = new TH1F(p_Name + "_MC_Sum", p_Title + " MC Sum", frac->GetNbinsX(), frac->GetBinLowEdge(1), frac->GetBinLowEdge(frac->GetNbinsX()) + frac->GetBinWidth(frac->GetNbinsX()));
  } else {
    mc_sum = new TH1F(p_Name + "_MC_Sum", p_Title + " MC Sum", p_nBins, p_Bins);
  }
  mc_sum->Sumw2();
  for(int i = 0; i < hist_Clones.size(); i++){
    if(p_SampleNames[i] == dataSetName){
      InitHist(hist_Clones[i], hist_Clones[i]->GetXaxis()->GetTitle(), hist_Clones[i]->GetYaxis()->GetTitle(), p_SampleColors[i], 0);
      leg->AddEntry(hist_Clones[i], p_SampleLabels[i]);
      continue;
    }
    mc_sum->Add(hist_Clones[i]);
    InitHist(hist_Clones[i], hist_Clones[i]->GetXaxis()->GetTitle(), hist_Clones[i]->GetYaxis()->GetTitle(), p_SampleColors[i], 1001);
    leg->AddEntry(hist_Clones[i], p_SampleLabels[i], "F");
    stack->Add(hist_Clones[i]);
  }
  frac->Divide(mc_sum);
  frac->SetTitleSize  (0.12,"Y");
  frac->SetTitleOffset(0.60,"Y");
  frac->SetLabelSize  (0.08,"Y");
  frac->SetTitleSize  (0.12,"X");
  frac->SetLabelSize  (0.10,"X");
  frac->GetYaxis()->CenterTitle(kTRUE);
  frac->SetTitle("");
  frac->GetYaxis()->SetTitle("Data/MC");
  frac->GetYaxis()->SetNdivisions(305);
  frac->GetYaxis()->SetRangeUser(0.0,2.0);
  hist_Can_pad1->cd();
  stack->Draw("hist");
  stack->SetMaximum(1.5 * maxValue);
  stack->SetTitle(p_Title);
  stack->GetXaxis()->SetTitle(hist_Clones[dataPosition]->GetXaxis()->GetTitle());
  stack->GetYaxis()->SetTitle(hist_Clones[dataPosition]->GetYaxis()->GetTitle());
  stack->Draw("hist");
  hist_Clones[dataPosition]->Draw("same");
  leg->Draw("same");
  hist_Can->RedrawAxis();
  hist_Can_pad2->cd();
  frac->Draw("EP");
  hist_Can->Print(outputdir + "/" + p_Name + "." + format);
}

void PrintCanvas(TString p_Name, TString p_Title, TH1F* p_Hist, TString p_Path, float p_TF, bool p_LogY, bool p_Write) {
  TCanvas* hist_Can = StyleTools::MakeCanvas(p_Name, p_Title, 600, 600);
  StyleTools::SetStyle();
//  gStyle->SetOptFit(1111);
  InitHist(p_Hist, p_Hist->GetXaxis()->GetTitle(), p_Hist->GetYaxis()->GetTitle(), 1, 0);
  hist_Can->cd();
  if(p_LogY){
    hist_Can->SetLogy();
  }
  p_Hist->UseCurrentStyle();
  p_Hist->Draw("P");
  if(p_TF != -1){
    TLine* TF_Line = new TLine(MET_bins[numMET_bins - numMET_bins_used], p_TF, MET_bins[numMET_bins - 1], p_TF);
    TF_Line->SetLineColor(2);
    TF_Line->SetLineWidth(2);
    TF_Line->SetLineStyle(9);
    TF_Line->Draw("same");
  }
  if(p_Write){
    hist_Can->Print(p_Path + "/" + p_Name + "." + format);
  } else {
    hist_Can->Write(p_Name + "." + format);
  }
}

void PrintCanvasWithFitError(TString p_Name, TString p_Title, TH1F* p_Hist, TH1F* p_FitWithErrorHist, TString p_Path, float p_TF, bool p_LogY, bool p_Write) {
  TCanvas* hist_Can = StyleTools::MakeCanvas(p_Name, p_Title, 600, 600);
  StyleTools::SetStyle();
  InitHist(p_Hist,             p_Hist->GetXaxis()->GetTitle(),             p_Hist->GetYaxis()->GetTitle(),             kBlack,   0);
  InitHist(p_FitWithErrorHist, p_FitWithErrorHist->GetXaxis()->GetTitle(), p_FitWithErrorHist->GetYaxis()->GetTitle(), kRed + 2, 3001);
  p_FitWithErrorHist->GetYaxis()->SetRangeUser(0, 1.1 * p_Hist->GetMaximum());
  p_FitWithErrorHist->SetLineColor(kRed + 2);
  p_FitWithErrorHist->SetMarkerColor(kBlack);
  p_FitWithErrorHist->SetMarkerSize(0);
  hist_Can->cd();
  if(p_LogY){
    hist_Can->SetLogy();
  }
  p_Hist->UseCurrentStyle();
  p_FitWithErrorHist->Draw("E3");
  p_Hist->Draw("same");
  TLegend* leg = new TLegend(0.65, 0.7, 0.9, 0.9);
  SetLegendStyle(leg);
  leg->AddEntry(p_Hist,             "TF",              "LP");
  leg->AddEntry(p_FitWithErrorHist, "Fit #pm 1#sigma", "LF");
  leg->Draw("same");
  if(p_TF != -1){
    TLine* TF_Line = new TLine(MET_bins[numMET_bins - numMET_bins_used], p_TF, MET_bins[numMET_bins - 1], p_TF);
    TF_Line->SetLineColor(2);
    TF_Line->SetLineWidth(2);
    TF_Line->SetLineStyle(9);
    TF_Line->Draw("same");
  }
  if(p_Write){
    hist_Can->Print(p_Path + "/" + p_Name + "." + format);
  } else {
    hist_Can->Write(p_Name + "." + format);
  }
}

void PrintMultiFitCanvas(TString p_Name, TString p_Title, vector <pair <TH1F*, TString> >& p_PlotNamesDesc, TString p_FitName, bool p_LogY){
  TCanvas* hist_Can = StyleTools::MakeCanvas(p_Name + "_Fit", p_Title, 600, 600);
  StyleTools::SetStyle();
  TLegend* leg = new TLegend(0.7, 0.9 - .05 * p_PlotNamesDesc.size(), 0.9, 0.9);
  MySetLegendStyle(leg);
  for(int i = 0; i < p_PlotNamesDesc.size(); i++){
    MyInitHist(p_PlotNamesDesc[i].first, p_PlotNamesDesc[i].first->GetXaxis()->GetTitle(), p_PlotNamesDesc[i].first->GetYaxis()->GetTitle(), multiFitColors[i], 0);
    p_PlotNamesDesc[i].first->SetStats(kFALSE);
    TF1* thisFit = p_PlotNamesDesc[i].first->GetFunction(p_FitName);
    thisFit->SetLineColor(multiFitColors[i]);
    leg->AddEntry(p_PlotNamesDesc[i].first, p_PlotNamesDesc[i].second, "lp");
  }
  hist_Can->cd();
  if(p_LogY){
    p_PlotNamesDesc[p_PlotNamesDesc.size() - 1].first->GetYaxis()->SetRangeUser(0.001, 1.5);
    hist_Can->SetLogy();
  } else {
    p_PlotNamesDesc[p_PlotNamesDesc.size() - 1].first->GetYaxis()->SetRangeUser(0., 1.5);
  }
  p_PlotNamesDesc[p_PlotNamesDesc.size() - 1].first->SetTitle(p_Title);
  p_PlotNamesDesc[p_PlotNamesDesc.size() - 1].first->SetMarkerSize(0);
  p_PlotNamesDesc[p_PlotNamesDesc.size() - 1].first->SetFillStyle(3001);
  p_PlotNamesDesc[p_PlotNamesDesc.size() - 1].first->Draw("E3");
  for(int i = 0; i < p_PlotNamesDesc.size() - 1; i++){
    if(p_LogY){
      p_PlotNamesDesc[i].first->GetYaxis()->SetRangeUser(0.001, 1.5);
    } else {
      p_PlotNamesDesc[i].first->GetYaxis()->SetRangeUser(0., 1.5);
    }
    p_PlotNamesDesc[i].first->Draw("same");
  }
  leg->Draw();
  hist_Can->SaveAs(outputdir + "/multi_" + p_Name + "_Fit." + format);
}

void PrintMultiCanvas(TString p_Name, TString p_Title, vector <pair <TH1F*, TString> >& p_PlotNamesDesc, TString p_FitName, bool p_LogY){
  TCanvas* hist_Can = StyleTools::MakeCanvas(p_Name, p_Title, 600, 600);
  StyleTools::SetStyle();
  TLegend* leg = new TLegend(0.7, 0.9 - .05 * p_PlotNamesDesc.size(), 0.9, 0.9);
  MySetLegendStyle(leg);
  for(int i = 0; i < p_PlotNamesDesc.size(); i++){
    MyInitHist(p_PlotNamesDesc[i].first, p_PlotNamesDesc[i].first->GetXaxis()->GetTitle(), p_PlotNamesDesc[i].first->GetYaxis()->GetTitle(), multiFitColors[i], 0);
    p_PlotNamesDesc[i].first->SetStats(kFALSE);
    if(p_FitName != ""){
      TF1* thisFit = p_PlotNamesDesc[i].first->GetFunction(p_FitName);
      thisFit->SetLineColor(multiFitColors[i]);
    }
    leg->AddEntry(p_PlotNamesDesc[i].first, p_PlotNamesDesc[i].second, "lp");
  }
  hist_Can->cd();
  if(p_LogY){
    p_PlotNamesDesc[0].first->GetYaxis()->SetRangeUser(0.001, 1.0);
    hist_Can->SetLogy();
  } else {
    p_PlotNamesDesc[0].first->GetYaxis()->SetRangeUser(0., 1.0);
  }
  p_PlotNamesDesc[0].first->SetTitle(p_Title);
  p_PlotNamesDesc[0].first->Draw("");
  for(int i = 1; i < p_PlotNamesDesc.size(); i++){
    if(p_LogY){
      p_PlotNamesDesc[i].first->GetYaxis()->SetRangeUser(0.001, 1.0);
    } else {
      p_PlotNamesDesc[i].first->GetYaxis()->SetRangeUser(0., 1.0);
    }
    p_PlotNamesDesc[i].first->Draw("same");
  }
  leg->Draw();
  hist_Can->SaveAs(outputdir + "/multi_" + p_Name + "." + format);
}

void MyInitHist(TH1 *hist, const char *xtit, const char *ytit, int color, int style) {
  hist->SetXTitle(xtit);
  hist->SetYTitle(ytit);
  hist->SetLineColor(color);
//  hist->SetLineColor(kBlack);
  hist->SetLineWidth(    3.);
  hist->SetFillColor(color );
  hist->SetFillStyle(style );
  hist->SetTitleSize  (0.055,"Y");
  hist->SetTitleOffset(1.600,"Y");
  hist->SetLabelOffset(0.014,"Y");
  hist->SetLabelSize  (0.040,"Y");
  hist->SetLabelFont  (62   ,"Y");
  hist->SetTitleSize  (0.055,"X");
  hist->SetTitleOffset(1.300,"X");
  hist->SetLabelOffset(0.014,"X");
  hist->SetLabelSize  (0.040,"X");
  hist->SetLabelFont  (62   ,"X");
//  hist->SetMarkerStyle(20);
  hist->SetMarkerColor(color);
  hist->SetMarkerSize (1.1);
  hist->GetYaxis()->SetTitleFont(62);
  hist->GetXaxis()->SetTitleFont(62);
  hist->SetTitle("");
  return;
}

void MySetLegendStyle(TLegend* leg) {
  leg->SetFillStyle (0);
  leg->SetFillColor (0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.02);
}

#endif
