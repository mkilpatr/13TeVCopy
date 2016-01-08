#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#include "AnalysisMethods/macros/0LepSearchRegions/generateQCDPredictions/CommonQCDTools.h"
#endif

TString outputdir;
TString format;
TString dataSetName;
bool run_on_htmht;
const int numMET_bins_Data = 4;
const int numMET_bins_Data_used = 2;
double MET_bins_Data[numMET_bins_Data] = { 150., 175., 200., 250. };

void generateQCDTransferFactors(TString regionConfFile_SF = "confFiles/regionInfoSF.conf",
                                TString regionConfFile    = "confFiles/regionInfo.conf",
                                TString inputdir  = "../../run/plots_15_12_21/",
                                TString p_outputdir = "QCD_TransferFactor_Plots",
                                TString p_format = "pdf",
                                bool p_run_on_htmht = true){
  format = p_format;
  TString which_ht = "";
  dataSetName  = "htmht-pr";
  run_on_htmht = p_run_on_htmht;
  if(!run_on_htmht){
    which_ht = "ht";
    dataSetName  = "jetht-pr";
  }
  TString confFile      = "confFiles/run0lep_" + which_ht + ".conf";
  TString confFile_qcd  = "confFiles/run0lep_qcd_only.conf";
  TString wgtVar = intLum + "*weight";
  outputdir = p_outputdir + (which_ht.Sizeof() == 1 ? "" : "_") + which_ht;
  gSystem->mkdir(outputdir, true);
  gSystem->mkdir(outputdir + "_X2", true);
  ofstream outfile(outputdir + "_X2/faledFitResults.txt", std::ofstream::out);
  float TR_Max = 1.1;
  float SR_Max = 0.5;

  vector <TString> sampleNames;
  vector <TString> sampleLabels;
  vector <int> sampleColors;
  generateSampleInfo(sampleNames, sampleLabels, sampleColors, confFile);

  map <string, vector <cutInfo> > cutInfoMap, cutInfoMap_SF;
  map <TString, RegionInfo>  regionInfoMap, regionInfoMap_SF;
  map <TString, RegionInfo>::iterator map_YI_iter;
  map <TString, PlotStuff*> plots;
  map <TString, PlotStuff*> plots_qcd;
  readRegionInfoFile(cutInfoMap_SF, regionConfFile_SF);
  readRegionInfoFile(cutInfoMap, regionConfFile);
  generateRegionInfoMap(regionInfoMap_SF, cutInfoMap_SF);
  generateRegionInfoMap(regionInfoMap, cutInfoMap);

  float SF_MM, SF_MM_E, SF_nMM, SF_nMM_E;
  getQCDScaleFactors(SF_MM, SF_MM_E, SF_nMM, SF_nMM_E, "QCD_ScaleFactor_Plots/" + regionInfoMap_SF.begin()->first + ".txt");
  cout << "SnMM  = " << SF_nMM << " +/- " << SF_nMM_E << endl;
  cout << "SMM   = " << SF_MM  << " +/- " << SF_MM_E  << endl;

  for(map_YI_iter = regionInfoMap.begin(); map_YI_iter != regionInfoMap.end(); map_YI_iter++){
    cout << map_YI_iter->second.uniqueName << endl;
    plots[map_YI_iter->first] = new PlotStuff(confFile, inputdir, outputdir);
    plots[map_YI_iter->first]->setPlotSource(PlotStuff::TREES);
    plots[map_YI_iter->first]->setPlotType(PlotStuff::DATAMC);
    plots[map_YI_iter->first]->setTree("Events");
    plots[map_YI_iter->first]->setFormat(format);
    plots[map_YI_iter->first]->setOutfile(map_YI_iter->first + "_toDetermine_SFs.root");
    plots[map_YI_iter->first]->setWriteHists();
    plots[map_YI_iter->first]->setHeaderText("","","");
    plots[map_YI_iter->first]->setLegend(0.25, 0.7, 0.9, 0.9);
    plots[map_YI_iter->first]->setTreeFileSuffix("_tree.root");
    plots[map_YI_iter->first]->setAddSigScaleTxt(false);
    plots[map_YI_iter->first]->setWgtVar(wgtVar);
    if(!run_on_htmht){
      plots[map_YI_iter->first]->setUnprescale(true);
      plots[map_YI_iter->first]->setUnprescaleVar("prescale");
    }
    plots_qcd[map_YI_iter->first] = new PlotStuff(confFile_qcd, inputdir, outputdir);
    plots_qcd[map_YI_iter->first]->setPlotSource(PlotStuff::TREES);
    plots_qcd[map_YI_iter->first]->setPlotType(PlotStuff::DATAMC);
    plots_qcd[map_YI_iter->first]->setTree("Events");
    plots_qcd[map_YI_iter->first]->setFormat(format);
    plots_qcd[map_YI_iter->first]->setOutfile(map_YI_iter->first + "_toDetermine_SFs_qcd.root");
    plots_qcd[map_YI_iter->first]->setWriteHists();
    plots_qcd[map_YI_iter->first]->setHeaderText("","","");
    plots_qcd[map_YI_iter->first]->setLegend(0.25, 0.7, 0.9, 0.9);
    plots_qcd[map_YI_iter->first]->setTreeFileSuffix("_tree.root");
    plots_qcd[map_YI_iter->first]->setAddSigScaleTxt(false);
    plots_qcd[map_YI_iter->first]->setWgtVar(wgtVar);
    cout << "adding CR plot with def:  " << map_YI_iter->second.getDefn_CR() << " to plot file:  " << plots[map_YI_iter->first]->outfileName() << endl;
    cout << "adding TR plot with def:  " << map_YI_iter->second.getDefn_TR() << " to plot file:  " << plots[map_YI_iter->first]->outfileName() << endl;
    cout << "adding SR plot with def:  " << map_YI_iter->second.getDefn_SR() << " to plot file:  " << plots[map_YI_iter->first]->outfileName() << endl;
        plots[map_YI_iter->first]->addTreeVar(map_YI_iter->second.uniqueName + "_CR",         "met", map_YI_iter->second.getDefn_CR(),                                    "#slash{E}_{T} [GeV]", numMET_bins - 1, MET_bins);
        plots[map_YI_iter->first]->addTreeVar(map_YI_iter->second.uniqueName + "_TR",         "met", map_YI_iter->second.getDefn_TR(),                                    "#slash{E}_{T} [GeV]", numMET_bins - 1, MET_bins);
        plots[map_YI_iter->first]->addTreeVar(map_YI_iter->second.uniqueName + "_SR",         "met", map_YI_iter->second.getDefn_SR(),                                    "#slash{E}_{T} [GeV]", numMET_bins - 1, MET_bins);
    plots_qcd[map_YI_iter->first]->addTreeVar(map_YI_iter->second.uniqueName + "_CR_JR_gt_5", "met", map_YI_iter->second.getDefn_CR() + " && jetMMMTrueResponsePt > 0.5", "#slash{E}_{T} [GeV]", numMET_bins - 1, MET_bins);
    plots_qcd[map_YI_iter->first]->addTreeVar(map_YI_iter->second.uniqueName + "_TR_JR_gt_5", "met", map_YI_iter->second.getDefn_TR() + " && jetMMMTrueResponsePt > 0.5", "#slash{E}_{T} [GeV]", numMET_bins - 1, MET_bins);
    plots_qcd[map_YI_iter->first]->addTreeVar(map_YI_iter->second.uniqueName + "_SR_JR_gt_5", "met", map_YI_iter->second.getDefn_SR() + " && jetMMMTrueResponsePt > 0.5", "#slash{E}_{T} [GeV]", numMET_bins - 1, MET_bins);
    plots_qcd[map_YI_iter->first]->addTreeVar(map_YI_iter->second.uniqueName + "_CR_JR_lt_5", "met", map_YI_iter->second.getDefn_CR() + " && jetMMMTrueResponsePt < 0.5", "#slash{E}_{T} [GeV]", numMET_bins - 1, MET_bins);
    plots_qcd[map_YI_iter->first]->addTreeVar(map_YI_iter->second.uniqueName + "_TR_JR_lt_5", "met", map_YI_iter->second.getDefn_TR() + " && jetMMMTrueResponsePt < 0.5", "#slash{E}_{T} [GeV]", numMET_bins - 1, MET_bins);
    plots_qcd[map_YI_iter->first]->addTreeVar(map_YI_iter->second.uniqueName + "_SR_JR_lt_5", "met", map_YI_iter->second.getDefn_SR() + " && jetMMMTrueResponsePt < 0.5", "#slash{E}_{T} [GeV]", numMET_bins - 1, MET_bins);
    plots[map_YI_iter->first]->plot(); 
    plots_qcd[map_YI_iter->first]->plot(); 
    int dataPosition = -1, qcdPosition = -1;
    for(int i = 0; i < sampleNames.size(); i++){
      if(sampleNames[i] == dataSetName) dataPosition = i;
      if(sampleNames[i].Contains("qcd")) qcdPosition = i;
    }
    TFile* infile_TF     = TFile::Open(plots[map_YI_iter->first]->outfileName());
    TFile* infile_TF_qcd = TFile::Open(plots_qcd[map_YI_iter->first]->outfileName());
    TFile* histResultsFile = new TFile(outputdir + "/" + map_YI_iter->second.uniqueName + "_histResults.root", "RECREATE");
    TFile* fitResultsFile  = new TFile(outputdir + "/" + map_YI_iter->second.uniqueName + "_fitResults.root",  "RECREATE");
    vector <TH1F*> CR_Hists,     TR_Hists,     SR_Hists;
    vector <TH1F*> CR_QCD_Hists, TR_QCD_Hists, SR_QCD_Hists;
    for(int i = 0; i < sampleNames.size(); i++){
      CR_Hists.push_back((TH1F*)infile_TF->Get(map_YI_iter->second.uniqueName + "_CR_" + sampleNames[i]));
      TR_Hists.push_back((TH1F*)infile_TF->Get(map_YI_iter->second.uniqueName + "_TR_" + sampleNames[i]));
      SR_Hists.push_back((TH1F*)infile_TF->Get(map_YI_iter->second.uniqueName + "_SR_" + sampleNames[i]));
    }
    CR_QCD_Hists.push_back((TH1F*)infile_TF_qcd->Get(map_YI_iter->second.uniqueName + "_CR_JR_gt_5_" + sampleNames[qcdPosition]));
    CR_QCD_Hists.push_back((TH1F*)infile_TF_qcd->Get(map_YI_iter->second.uniqueName + "_CR_JR_lt_5_" + sampleNames[qcdPosition]));
    TR_QCD_Hists.push_back((TH1F*)infile_TF_qcd->Get(map_YI_iter->second.uniqueName + "_TR_JR_gt_5_" + sampleNames[qcdPosition]));
    TR_QCD_Hists.push_back((TH1F*)infile_TF_qcd->Get(map_YI_iter->second.uniqueName + "_TR_JR_lt_5_" + sampleNames[qcdPosition]));
    SR_QCD_Hists.push_back((TH1F*)infile_TF_qcd->Get(map_YI_iter->second.uniqueName + "_SR_JR_gt_5_" + sampleNames[qcdPosition]));
    SR_QCD_Hists.push_back((TH1F*)infile_TF_qcd->Get(map_YI_iter->second.uniqueName + "_SR_JR_lt_5_" + sampleNames[qcdPosition]));
    TH1F* CR_Hist_Data    = (TH1F*)CR_Hists[dataPosition]->Clone(map_YI_iter->second.uniqueName + "_" + sampleNames[dataPosition] + "_CR");
    TH1F* TF_TR_Hist_Data = (TH1F*)TR_Hists[dataPosition]->Clone(map_YI_iter->second.uniqueName + "_" + sampleNames[dataPosition] + "_TR");
    TH1F* TF_SR_Hist_Data = (TH1F*)SR_Hists[dataPosition]->Clone(map_YI_iter->second.uniqueName + "_" + sampleNames[dataPosition] + "_SR");

    for(int i = 0; i < sampleNames.size(); i++){
      if( (i != qcdPosition) && (i != dataPosition) ){
           CR_Hist_Data->Add(CR_Hists[i], -1);
        TF_TR_Hist_Data->Add(TR_Hists[i], -1);
        TF_SR_Hist_Data->Add(SR_Hists[i], -1);
      }
    }
    for(int i = numMET_bins_Data; i <= CR_Hist_Data->GetNbinsX(); i++){
         CR_Hist_Data->SetBinContent(i, -999);
      TF_TR_Hist_Data->SetBinContent(i, -999);
      TF_SR_Hist_Data->SetBinContent(i, -999);
    }
    histResultsFile->cd();
       CR_Hist_Data->Write();
    TF_TR_Hist_Data->Write();
    TF_SR_Hist_Data->Write();
    TF_TR_Hist_Data->SetName(map_YI_iter->second.uniqueName + "_" + sampleNames[dataPosition] + "_TR_TF_X2");
    TF_SR_Hist_Data->SetName(map_YI_iter->second.uniqueName + "_" + sampleNames[dataPosition] + "_SR_TF_X2");
    TF_TR_Hist_Data->SetYTitle("TR/CR");
    TF_SR_Hist_Data->SetYTitle("SR/CR");
    TF_TR_Hist_Data->Divide(CR_Hist_Data);
    TF_SR_Hist_Data->Divide(CR_Hist_Data);
    TF_TR_Hist_Data->SetMinimum(0.);
    TF_SR_Hist_Data->SetMinimum(0.);
    TF_TR_Hist_Data->SetMaximum(TR_Max);
    TF_SR_Hist_Data->SetMaximum(SR_Max);
    TF1* TF_TR_Fit_Data = new TF1("TF_TR_Fit", "[0]/([1]+(x^[2]))", MET_bins_Data[0], MET_bins_Data[numMET_bins_Data - 1]);
    TF1* TF_SR_Fit_Data = new TF1("TF_SR_Fit", "[0]/([1]+(x^[2]))", MET_bins_Data[0], MET_bins_Data[numMET_bins_Data - 1]);
    TF_TR_Fit_Data->SetLineColor(kBlack);
    TF_SR_Fit_Data->SetLineColor(kBlack);
    TFitResultPtr TF_TR_FitResultPtr_Data = TF_TR_Hist_Data->Fit(TF_TR_Fit_Data, "QSE", "", MET_bins_Data[0], MET_bins_Data[numMET_bins_Data - 1]);
    TFitResultPtr TF_SR_FitResultPtr_Data = TF_SR_Hist_Data->Fit(TF_SR_Fit_Data, "QSE", "", MET_bins_Data[0], MET_bins_Data[numMET_bins_Data - 1]);
    fitResultsFile->cd();
    TF_TR_Hist_Data->Write();
    TF_SR_Hist_Data->Write();
    PrintCanvas(TF_TR_Hist_Data->GetName(), TF_TR_Hist_Data->GetTitle(), TF_TR_Hist_Data, outputdir + "_X2");
    PrintCanvas(TF_SR_Hist_Data->GetName(), TF_SR_Hist_Data->GetTitle(), TF_SR_Hist_Data, outputdir + "_X2");

    for(int i = 0; i < 6; i++){
      float temp_SF_MM = 1., temp_SF_MM_E = 0., temp_SF_nMM = 1., temp_SF_nMM_E = 0;
      TString suffix = "";
      switch(i){
        case 0:
          suffix = "";
          temp_SF_MM = 1.; temp_SF_MM_E = 0.; temp_SF_nMM = 1.; temp_SF_nMM_E = 0;
          break;
        case 1:
          suffix = "_RW";
          temp_SF_MM = SF_MM; temp_SF_MM_E = 0.; temp_SF_nMM = SF_nMM; temp_SF_nMM_E = 0;
          break;
        case 2:
          suffix = "_RW_P1";
          temp_SF_MM = SF_MM; temp_SF_MM_E = SF_MM_E; temp_SF_nMM = SF_nMM; temp_SF_nMM_E = 0;
          break;
        case 3:
          suffix = "_RW_M1";
          temp_SF_MM = SF_MM; temp_SF_MM_E = -SF_MM_E; temp_SF_nMM = SF_nMM; temp_SF_nMM_E = 0;
          break;
        case 4:
          suffix = "_RW_BP1";
          temp_SF_MM = SF_MM; temp_SF_MM_E = SF_MM_E; temp_SF_nMM = SF_nMM; temp_SF_nMM_E = SF_nMM_E;
          break;
        case 5:
          suffix = "_RW_BM1";
          temp_SF_MM = SF_MM; temp_SF_MM_E = -SF_MM_E; temp_SF_nMM = SF_nMM; temp_SF_nMM_E = -SF_nMM_E;
          break;
      }
      TH1F* qcd_sum_CR = (TH1F*)CR_QCD_Hists[0]->Clone(map_YI_iter->second.uniqueName + "_" + sampleNames[qcdPosition] + "_CR_Sum" + suffix);
      TH1F* qcd_sum_TR = (TH1F*)TR_QCD_Hists[0]->Clone(map_YI_iter->second.uniqueName + "_" + sampleNames[qcdPosition] + "_TR_Sum" + suffix);
      TH1F* qcd_sum_SR = (TH1F*)SR_QCD_Hists[0]->Clone(map_YI_iter->second.uniqueName + "_" + sampleNames[qcdPosition] + "_SR_Sum" + suffix);
      qcd_sum_CR->Scale(temp_SF_nMM + temp_SF_nMM_E);
      qcd_sum_TR->Scale(temp_SF_nMM + temp_SF_nMM_E);
      qcd_sum_SR->Scale(temp_SF_nMM + temp_SF_nMM_E);
      qcd_sum_CR->Add(CR_QCD_Hists[1], temp_SF_MM + temp_SF_MM_E);
      qcd_sum_TR->Add(TR_QCD_Hists[1], temp_SF_MM + temp_SF_MM_E);
      qcd_sum_SR->Add(SR_QCD_Hists[1], temp_SF_MM + temp_SF_MM_E);
      TH1F* CR_Hist_MC    = (TH1F*)qcd_sum_CR->Clone(map_YI_iter->second.uniqueName + "_" + sampleNames[qcdPosition] + "_CR" + suffix);
      TH1F* TF_TR_Hist_MC = (TH1F*)qcd_sum_TR->Clone(map_YI_iter->second.uniqueName + "_" + sampleNames[qcdPosition] + "_TR" + suffix + "_TF_X2");
      TH1F* TF_SR_Hist_MC = (TH1F*)qcd_sum_SR->Clone(map_YI_iter->second.uniqueName + "_" + sampleNames[qcdPosition] + "_SR" + suffix + "_TF_X2");
      histResultsFile->cd();
      if( (suffix == "_RW_P1") || (suffix == "_RW_M1") || (suffix == "_RW_BP1") || (suffix == "_RW_BM1") ){
        for(int i_bin = 0; i_bin < qcd_sum_CR->GetNbinsX(); i_bin++){
          qcd_sum_CR->SetBinError(i_bin, 0.);
          qcd_sum_TR->SetBinError(i_bin, 0.);
          qcd_sum_SR->SetBinError(i_bin, 0.);
        }
      }
      qcd_sum_CR->Write();
      qcd_sum_TR->Write();
      qcd_sum_SR->Write();
      TF_TR_Hist_MC->SetYTitle("TR/CR");
      TF_SR_Hist_MC->SetYTitle("SR/CR");
      TF_TR_Hist_MC->Divide(CR_Hist_MC);
      TF_SR_Hist_MC->Divide(CR_Hist_MC);
      TF_TR_Hist_MC->SetMinimum(0.);
      TF_SR_Hist_MC->SetMinimum(0.);
      TF_TR_Hist_MC->SetMaximum(TR_Max);
      TF_SR_Hist_MC->SetMaximum(SR_Max);
      TF1* TF_TR_Fit_MC = new TF1("TF_TR_Fit", "[0]/([1]+(x^[2]))", MET_bins[0], MET_bins[numMET_bins - 1]);
      TF1* TF_SR_Fit_MC = new TF1("TF_SR_Fit", "[0]/([1]+(x^[2]))", MET_bins[0], MET_bins[numMET_bins - 1]);
      TF_TR_Fit_MC->SetLineColor(kRed);
      TF_SR_Fit_MC->SetLineColor(kRed);
      TFitResultPtr TF_TR_FitResultPtr_MC = TF_TR_Hist_MC->Fit(TF_TR_Fit_MC, "QSE", "", MET_bins[0], MET_bins[numMET_bins - 1]);
      TFitResultPtr TF_SR_FitResultPtr_MC = TF_SR_Hist_MC->Fit(TF_SR_Fit_MC, "QSE", "", MET_bins[0], MET_bins[numMET_bins - 1]);
      int TR_Fit_Status = 1, SR_Fit_Status = 1, TR_counter = 0, SR_counter = 0;
      while(TR_Fit_Status != 0){
        TF_TR_Fit_MC->SetParameters(TF_TR_FitResultPtr_MC->Parameter(0), TF_TR_FitResultPtr_MC->Parameter(1), TF_TR_FitResultPtr_MC->Parameter(2));
        TF_TR_FitResultPtr_MC = TF_TR_Hist_MC->Fit(TF_TR_Fit_MC, "QSE", "", MET_bins[0], MET_bins[numMET_bins - 1]);
        TR_Fit_Status = TF_TR_FitResultPtr_MC;
        TR_counter++;
        if(TR_counter > 10){
          break;
        }
      }
      while(SR_Fit_Status != 0){
        TF_SR_Fit_MC->SetParameters(TF_SR_FitResultPtr_MC->Parameter(0), TF_SR_FitResultPtr_MC->Parameter(1), TF_SR_FitResultPtr_MC->Parameter(2));
        TF_SR_FitResultPtr_MC = TF_SR_Hist_MC->Fit(TF_SR_Fit_MC, "QSE", "", MET_bins[0], MET_bins[numMET_bins - 1]);
        SR_Fit_Status = TF_SR_FitResultPtr_MC;
        SR_counter++;
        if(SR_counter > 10){
          break;
        }
      }
      TF_TR_FitResultPtr_MC->Print("V");
      TF_SR_FitResultPtr_MC->Print("V");
      if(TR_Fit_Status != 0){
        outfile << "TR fit fail:  suffix: " << suffix << ", uniqueName: " << map_YI_iter->second.uniqueName << ", status: " << TR_Fit_Status << endl;
        outfile << "  parameters = {" << TF_TR_FitResultPtr_MC->Parameter(0) << ", " << TF_TR_FitResultPtr_MC->Parameter(1) << ", " << TF_TR_FitResultPtr_MC->Parameter(2) << "}" << endl;
      }
      if(SR_Fit_Status != 0){
        outfile << "SR fit fail:  suffix: " << suffix << ", uniqueName: " << map_YI_iter->second.uniqueName << ", status: " << SR_Fit_Status << endl;
        outfile << "  parameters = {" << TF_SR_FitResultPtr_MC->Parameter(0) << ", " << TF_SR_FitResultPtr_MC->Parameter(1) << ", " << TF_SR_FitResultPtr_MC->Parameter(2) << "}" << endl;
      }
      TH1F* TF_TR_FitHist_MC = makeFitHist(TF_TR_Fit_MC, TF_TR_Hist_MC, map_YI_iter->second.uniqueName + "_" + sampleNames[qcdPosition ] + "_TR" + suffix + "_TF_X2_Fit");
      TH1F* TF_SR_FitHist_MC = makeFitHist(TF_SR_Fit_MC, TF_SR_Hist_MC, map_YI_iter->second.uniqueName + "_" + sampleNames[qcdPosition ] + "_SR" + suffix + "_TF_X2_Fit");
      TH1F* TF_TR_FitHistWithError_MC = addConfidenceLevelsToFit(TF_TR_FitHist_MC, TF_TR_Fit_MC, TF_TR_FitResultPtr_MC->GetCovarianceMatrix().GetMatrixArray(), 0.68);
      TH1F* TF_SR_FitHistWithError_MC = addConfidenceLevelsToFit(TF_SR_FitHist_MC, TF_SR_Fit_MC, TF_SR_FitResultPtr_MC->GetCovarianceMatrix().GetMatrixArray(), 0.68);
      if( (suffix == "_RW_P1") || (suffix == "_RW_M1") || (suffix == "_RW_BP1") || (suffix == "_RW_BM1") ){
        for(int i_bin = 0; i_bin < qcd_sum_CR->GetNbinsX(); i_bin++){
          TF_TR_Hist_MC->SetBinError(i_bin, 0.);
          TF_SR_Hist_MC->SetBinError(i_bin, 0.);
        }
      }
      fitResultsFile->cd();
      TF_TR_Hist_MC->Write();
      TF_SR_Hist_MC->Write();
      TF_TR_FitResultPtr_MC->Write();
      TF_SR_FitResultPtr_MC->Write();
//      TF_TR_FitHist_MC->Write();
//      TF_SR_FitHist_MC->Write();
      TF_TR_FitHistWithError_MC->Write();
      TF_SR_FitHistWithError_MC->Write();
      PrintCanvas(TF_TR_Hist_MC->GetName() + suffix, TF_TR_Hist_MC->GetTitle(), TF_TR_Hist_MC, outputdir + "_X2");
      PrintCanvas(TF_SR_Hist_MC->GetName() + suffix, TF_SR_Hist_MC->GetTitle(), TF_SR_Hist_MC, outputdir + "_X2");
      PrintCanvasWithFitError(TF_TR_FitHistWithError_MC->GetName() + suffix, TF_TR_FitHistWithError_MC->GetTitle(), TF_TR_Hist_MC, TF_TR_FitHistWithError_MC, outputdir + "_X2");
      PrintCanvasWithFitError(TF_SR_FitHistWithError_MC->GetName() + suffix, TF_SR_FitHistWithError_MC->GetTitle(), TF_SR_Hist_MC, TF_SR_FitHistWithError_MC, outputdir + "_X2");
    }
  }
}
