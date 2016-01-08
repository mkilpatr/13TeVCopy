#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#include "AnalysisMethods/macros/0LepSearchRegions/generateQCDPredictions/CommonQCDTools.h"
#endif

TString outputdir   = "multiplePlotsPerCanvas";
TString format      = "pdf";
TString dataSetName = "htmht-pr";
bool run_on_htmht = true;

void multiplePlotsPerCanvas() {
  TString which_ht = "";
  if(!run_on_htmht){
    which_ht = "ht";
    dataSetName  = "jetht-pr";
  }
  outputdir += (which_ht.Sizeof() == 1 ? "" : "_") + which_ht;
  TString inputdir = "QCD_TransferFactor_Plots" + TString(which_ht.Sizeof() == 1 ? "" : "_") + which_ht;
  TString regionConfFile = "../confFiles/regionInfo.conf";
  TString fitType = "_X2";
  gSystem->mkdir(outputdir, true);
  map <string, vector <cutInfo> > cutInfoMap;
  map <TString, RegionInfo>  regionInfoMap;
  map <TString, RegionInfo>::iterator map_YI_iter;
  readRegionInfoFile(cutInfoMap, regionConfFile);
  generateRegionInfoMap(regionInfoMap, cutInfoMap);

  for(map_YI_iter = regionInfoMap.begin(); map_YI_iter != regionInfoMap.end(); map_YI_iter++){
    TFile* inFile_Fits  = TFile::Open(inputdir + "/" + map_YI_iter->first + "_fitResults.root");
    TFile* inFile_Hists = TFile::Open(inputdir + "/" + map_YI_iter->first + "_histResults.root");
    vector <pair <TH1F*, TString> > plotNamesDesc_TR_Fit {
//      make_pair((TH1F*)inFile_Fits->Get(map_YI_iter->first + "_" + dataSetName + "_TR_TF" + fitType), "Data"),
      make_pair((TH1F*)inFile_Fits->Get(map_YI_iter->first + "_qcd_TR_TF"                 + fitType), "MC"),
      make_pair((TH1F*)inFile_Fits->Get(map_YI_iter->first + "_qcd_TR_RW_TF"              + fitType), "MC RW"),
      make_pair((TH1F*)inFile_Fits->Get(map_YI_iter->first + "_qcd_TR_RW_P1_TF"           + fitType), "MC RW + 1"),
      make_pair((TH1F*)inFile_Fits->Get(map_YI_iter->first + "_qcd_TR_RW_M1_TF"           + fitType), "MC RW - 1"),
      make_pair((TH1F*)inFile_Fits->Get(map_YI_iter->first + "_qcd_TR_RW_BP1_TF"          + fitType), "MC RW + 1 + 1"),
      make_pair((TH1F*)inFile_Fits->Get(map_YI_iter->first + "_qcd_TR_RW_BM1_TF"          + fitType), "MC RW - 1 - 1"),
      make_pair((TH1F*)inFile_Fits->Get(map_YI_iter->first + "_qcd_TR_TF"                 + fitType + "_Fit_FitErr"), "MC RW Fit + 1 #sigma"),
    };
    vector <pair <TH1F*, TString> > plotNamesDesc_SR_Fit {
//      make_pair((TH1F*)inFile_Fits->Get(map_YI_iter->first + "_" + dataSetName + "_SR_TF" + fitType), "Data"),
      make_pair((TH1F*)inFile_Fits->Get(map_YI_iter->first + "_qcd_SR_TF"                 + fitType), "MC"),
      make_pair((TH1F*)inFile_Fits->Get(map_YI_iter->first + "_qcd_SR_RW_TF"              + fitType), "MC RW"),
      make_pair((TH1F*)inFile_Fits->Get(map_YI_iter->first + "_qcd_SR_RW_P1_TF"           + fitType), "MC RW + 1"),
      make_pair((TH1F*)inFile_Fits->Get(map_YI_iter->first + "_qcd_SR_RW_M1_TF"           + fitType), "MC RW - 1"),
      make_pair((TH1F*)inFile_Fits->Get(map_YI_iter->first + "_qcd_SR_RW_BP1_TF"          + fitType), "MC RW + 1 + 1"),
      make_pair((TH1F*)inFile_Fits->Get(map_YI_iter->first + "_qcd_SR_RW_BM1_TF"          + fitType), "MC RW - 1 - 1"),
      make_pair((TH1F*)inFile_Fits->Get(map_YI_iter->first + "_qcd_SR_TF"                 + fitType + "_Fit_FitErr"), "MC RW Fit + 1 #sigma"),
    };
//    vector <pair <TH1F*, TString> > plotNamesDesc_CR{
////      make_pair((TH1F*)inFile_Hists->Get(map_YI_iter->first + "_" + dataSetName + "_CR"), "Data"),
//      make_pair((TH1F*)inFile_Hists->Get(map_YI_iter->first + "_qcd_CR_Sum"),             "MC"),
//      make_pair((TH1F*)inFile_Hists->Get(map_YI_iter->first + "_qcd_CR_Sum_RW"),          "MC RW"),
//      make_pair((TH1F*)inFile_Hists->Get(map_YI_iter->first + "_qcd_CR_Sum_RW_P1"),       "MC RW + 1"),
//      make_pair((TH1F*)inFile_Hists->Get(map_YI_iter->first + "_qcd_CR_Sum_RW_M1"),       "MC RW - 1"),
//      make_pair((TH1F*)inFile_Hists->Get(map_YI_iter->first + "_qcd_CR_Sum_RW_BP1"),      "MC RW + 1 + 1"),
//      make_pair((TH1F*)inFile_Hists->Get(map_YI_iter->first + "_qcd_CR_Sum_RW_BM1"),      "MC RW - 1 - 1"),
//    };
//    vector <pair <TH1F*, TString> > plotNamesDesc_TR{
////      make_pair((TH1F*)inFile_Hists->Get(map_YI_iter->first + "_" + dataSetName + "_TR"), "Data"),
//      make_pair((TH1F*)inFile_Hists->Get(map_YI_iter->first + "_qcd_TR_Sum"),             "MC"),
//      make_pair((TH1F*)inFile_Hists->Get(map_YI_iter->first + "_qcd_TR_Sum_RW"),          "MC RW"),
//      make_pair((TH1F*)inFile_Hists->Get(map_YI_iter->first + "_qcd_TR_Sum_RW_P1"),       "MC RW + 1"),
//      make_pair((TH1F*)inFile_Hists->Get(map_YI_iter->first + "_qcd_TR_Sum_RW_M1"),       "MC RW - 1"),
//      make_pair((TH1F*)inFile_Hists->Get(map_YI_iter->first + "_qcd_TR_Sum_RW_BP1"),      "MC RW + 1 + 1"),
//      make_pair((TH1F*)inFile_Hists->Get(map_YI_iter->first + "_qcd_TR_Sum_RW_BM1"),      "MC RW - 1 - 1"),
//    };
//    vector <pair <TH1F*, TString> > plotNamesDesc_SR{
////      make_pair((TH1F*)inFile_Hists->Get(map_YI_iter->first + "_" + dataSetName + "_SR"), "Data"),
//      make_pair((TH1F*)inFile_Hists->Get(map_YI_iter->first + "_qcd_SR_Sum"),             "MC"),
//      make_pair((TH1F*)inFile_Hists->Get(map_YI_iter->first + "_qcd_SR_Sum_RW"),          "MC RW"),
//      make_pair((TH1F*)inFile_Hists->Get(map_YI_iter->first + "_qcd_SR_Sum_RW_P1"),       "MC RW + 1"),
//      make_pair((TH1F*)inFile_Hists->Get(map_YI_iter->first + "_qcd_SR_Sum_RW_M1"),       "MC RW - 1"),
//      make_pair((TH1F*)inFile_Hists->Get(map_YI_iter->first + "_qcd_SR_Sum_RW_BP1"),      "MC RW + 1 + 1"),
//      make_pair((TH1F*)inFile_Hists->Get(map_YI_iter->first + "_qcd_SR_Sum_RW_BM1"),      "MC RW - 1 - 1"),
//    };
    PrintMultiFitCanvas("TR_" + map_YI_iter->first, "Transition Region Transfer Factors", plotNamesDesc_TR_Fit, "TF_TR_Fit", true);
    PrintMultiFitCanvas("SR_" + map_YI_iter->first, "Signal Region Transfer Factors",     plotNamesDesc_SR_Fit, "TF_SR_Fit", true);
//    PrintMultiFitCanvas("TR_" + map_YI_iter->first, "Transition Region Transfer Factors", plotNamesDesc_TR_Fit, "TF_TR_Fit");
//    PrintMultiFitCanvas("SR_" + map_YI_iter->first, "Signal Region Transfer Factors",     plotNamesDesc_SR_Fit, "TF_SR_Fit");
 //   PrintMultiCanvas(   "CR_" + map_YI_iter->first, "Control Region Yields",    plotNamesDesc_CR, true);
 //   PrintMultiCanvas(   "TR_" + map_YI_iter->first, "Transition Region Yields", plotNamesDesc_TR, true);
 //   PrintMultiCanvas(   "SR_" + map_YI_iter->first, "Signal Region Yields",     plotNamesDesc_SR, true);
  }
}
