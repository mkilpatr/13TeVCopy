#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#include "AnalysisMethods/macros/0LepSearchRegions/generateQCDPredictions/CommonQCDTools.h"
#endif

TString outputdir;
TString format;
TString dataSetName;
bool run_on_htmht;
const int numJR_bins = 20;
const int JR_min_bin = 0.;
const int JR_max_bin = 1.;
const int numJR_RB_bins = 3;
double JR_RB_bins[numJR_RB_bins] = { 0., .5, 1. };
float non_QCD_MC_Uncertainty = 0.5;

void generateScaleFactors(ofstream& p_Outfile, TH1F* p_Data_Hist, TH1F* p_QCD_Hist_nMM, TH1F* p_QCD_Hist_MM, float& SnMM, float& SnMM_E, float& SMM, float& SMM_E);

void generateQCDScaleFactors(TString regionConfFile_SF = "confFiles/regionInfoSF.conf",
                             TString inputdir    = "../../run/plots_15_12_21/",
                             TString p_outputdir = "QCD_ScaleFactor_Plots",
                             TString p_format  = "pdf",
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

  vector <TString> sampleNames, sampleLabels;
  vector <int> sampleColors;
  generateSampleInfo(sampleNames, sampleLabels, sampleColors, confFile);

  map <string, vector <cutInfo> > cutInfoMap;
  map <TString, RegionInfo>  regionInfoMap;
  map <TString, RegionInfo>::iterator map_YI_iter;
  map <TString, PlotStuff*> plots;
  map <TString, PlotStuff*> plots_qcd;
  readRegionInfoFile(cutInfoMap, regionConfFile_SF);
  generateRegionInfoMap(regionInfoMap, cutInfoMap);

  for(map_YI_iter = regionInfoMap.begin(); map_YI_iter != regionInfoMap.end(); map_YI_iter++){
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
    plots[map_YI_iter->first]->addTreeVar(    map_YI_iter->second.uniqueName + "_CR_JR",      "jetMMMPseudoResponsePt", map_YI_iter->second.getDefn_CR(),                                    "PseudoResponse_{MMM}", numJR_bins, JR_min_bin, JR_max_bin);
    plots_qcd[map_YI_iter->first]->addTreeVar(map_YI_iter->second.uniqueName + "_CR_JR_gt_5", "jetMMMPseudoResponsePt", map_YI_iter->second.getDefn_CR() + " && jetMMMTrueResponsePt > 0.5", "PseudoResponse_{MMM}", numJR_bins, JR_min_bin, JR_max_bin);
    plots_qcd[map_YI_iter->first]->addTreeVar(map_YI_iter->second.uniqueName + "_CR_JR_lt_5", "jetMMMPseudoResponsePt", map_YI_iter->second.getDefn_CR() + " && jetMMMTrueResponsePt < 0.5", "PseudoResponse_{MMM}", numJR_bins, JR_min_bin, JR_max_bin);
    plots[map_YI_iter->first]->plot(); 
    plots_qcd[map_YI_iter->first]->plot(); 
    int dataPosition = -1, qcdPosition = -1;
    for(int i = 0; i < sampleNames.size(); i++){
      if(sampleNames[i] == dataSetName) dataPosition = i;
      if(sampleNames[i].Contains("qcd")) qcdPosition = i;
    }
    TFile* infile     = TFile::Open(plots[map_YI_iter->first]->outfileName());
    TFile* infile_qcd = TFile::Open(plots_qcd[map_YI_iter->first]->outfileName());
    vector <TH1F*>  CR_Hists, CR_QCD_Hists;
    vector <TH1F*>  forTheStack_Hists,    forTheQCDOnlyStack_Hists;
    vector <TH1F*>  forTheStack_Hists_RB, forTheQCDOnlyStack_Hists_RB;
    vector <TString> forTheStack_Names,  forTheQCDOnlyStack_Names;
    vector <TString> forTheStack_Labels, forTheQCDOnlyStack_Labels;
    vector <int>    forTheStack_Colors, forTheQCDOnlyStack_Colors;
    for(int i = 0, counter = 0; i < sampleNames.size(); i++){
      CR_Hists.push_back((TH1F*)infile->Get(map_YI_iter->second.uniqueName + "_CR_JR_" + sampleNames[i]));
    }
    CR_QCD_Hists.push_back((TH1F*)infile_qcd->Get(map_YI_iter->second.uniqueName + "_CR_JR_gt_5_" + sampleNames[qcdPosition]));
    CR_QCD_Hists.push_back((TH1F*)infile_qcd->Get(map_YI_iter->second.uniqueName + "_CR_JR_lt_5_" + sampleNames[qcdPosition]));
    for(int i = 0, counter = 0; i < sampleNames.size(); i++){
      if(i != qcdPosition){
        forTheStack_Hists.push_back((TH1F*)CR_Hists[i]->Clone());
        if(i != dataPosition){
          for(int i_bin = 1; i_bin <= forTheStack_Hists[counter]->GetNbinsX() + 1; i_bin++){
            forTheStack_Hists[counter]->SetBinError(i_bin, non_QCD_MC_Uncertainty * forTheStack_Hists[counter]->GetBinContent(i_bin));
          }
        }
        forTheStack_Names.push_back(sampleNames[i]);
        forTheStack_Colors.push_back(sampleColors[i]);
        forTheStack_Labels.push_back(sampleLabels[i]);
        counter++;
      }
    }

    TH1F* data_only_Hist_CR   = (TH1F*)CR_Hists[dataPosition]->Clone();
    TH1F* qcd_Hist_JR_gt_5_CR = (TH1F*)CR_QCD_Hists[0]->Clone();
    TH1F* qcd_Hist_JR_lt_5_CR = (TH1F*)CR_QCD_Hists[1]->Clone();
    for(int i = 0; i < forTheStack_Hists.size(); i++){
      if(i != dataPosition){
        data_only_Hist_CR->Add(forTheStack_Hists[i], -1);
      }
    }
//we no longer want to normalize qcd to data
//    float data_qcd_SF = data_only_Hist_CR->Integral(0, data_only_Hist_CR->GetNbinsX() + 1) / CR_Hists[qcdPosition]->Integral(0, CR_Hists[qcdPosition]->GetNbinsX() + 1);
//    qcd_Hist_JR_gt_5_CR->Scale(data_qcd_SF);
//    qcd_Hist_JR_lt_5_CR->Scale(data_qcd_SF);
    forTheStack_Hists.push_back((TH1F*)qcd_Hist_JR_gt_5_CR->Clone());
    forTheStack_Hists.push_back((TH1F*)qcd_Hist_JR_lt_5_CR->Clone());
    forTheStack_Names.push_back(sampleNames[qcdPosition] + "_JR_gt_5");
    forTheStack_Names.push_back(sampleNames[qcdPosition] + "_JR_lt_5");
    forTheStack_Colors.push_back(sampleColors[qcdPosition] + 1);
    forTheStack_Colors.push_back(sampleColors[qcdPosition] - 1);
    forTheStack_Labels.push_back(sampleLabels[qcdPosition] + " JR > 0.5");
    forTheStack_Labels.push_back(sampleLabels[qcdPosition] + " JR < 0.5");
    for(int i = 0; i < forTheStack_Hists.size(); i ++){
      forTheStack_Hists_RB.push_back((TH1F*)forTheStack_Hists[i]->Rebin(numJR_RB_bins - 1, TString(forTheStack_Hists[i]->GetName()) + "_RB", JR_RB_bins));
    }
    forTheQCDOnlyStack_Hists.push_back((TH1F*)data_only_Hist_CR->Clone());
    forTheQCDOnlyStack_Hists.push_back((TH1F*)qcd_Hist_JR_gt_5_CR->Clone());
    forTheQCDOnlyStack_Hists.push_back((TH1F*)qcd_Hist_JR_lt_5_CR->Clone());
    forTheQCDOnlyStack_Hists_RB.push_back((TH1F*)data_only_Hist_CR->Rebin(  numJR_RB_bins - 1, TString(data_only_Hist_CR->GetName())   + "_RB", JR_RB_bins));
    forTheQCDOnlyStack_Hists_RB.push_back((TH1F*)qcd_Hist_JR_gt_5_CR->Rebin(numJR_RB_bins - 1, TString(qcd_Hist_JR_gt_5_CR->GetName()) + "_RB", JR_RB_bins));
    forTheQCDOnlyStack_Hists_RB.push_back((TH1F*)qcd_Hist_JR_lt_5_CR->Rebin(numJR_RB_bins - 1, TString(qcd_Hist_JR_lt_5_CR->GetName()) + "_RB", JR_RB_bins));
    forTheQCDOnlyStack_Names.push_back(sampleNames[dataPosition]);
    forTheQCDOnlyStack_Names.push_back(sampleNames[qcdPosition] + "_JR_gt_5");
    forTheQCDOnlyStack_Names.push_back(sampleNames[qcdPosition] + "_JR_lt_5");
    forTheQCDOnlyStack_Colors.push_back(sampleColors[dataPosition]);
    forTheQCDOnlyStack_Colors.push_back(sampleColors[qcdPosition] + 1);
    forTheQCDOnlyStack_Colors.push_back(sampleColors[qcdPosition] - 1);
    forTheQCDOnlyStack_Labels.push_back(sampleLabels[dataPosition] + " - nonQCD MC");
    forTheQCDOnlyStack_Labels.push_back(sampleLabels[qcdPosition] + " JR > 0.5");
    forTheQCDOnlyStack_Labels.push_back(sampleLabels[qcdPosition] + " JR < 0.5");
    createStackPlotWithRatio(map_YI_iter->second.uniqueName + "_Stack",        "Control Region Yields", forTheStack_Names, forTheStack_Labels, forTheStack_Colors, forTheStack_Hists);
    createStackPlotWithRatio(map_YI_iter->second.uniqueName + "_Stack_RB",     "Control Region Yields", forTheStack_Names, forTheStack_Labels, forTheStack_Colors, forTheStack_Hists_RB, numJR_RB_bins - 1, JR_RB_bins);
    createStackPlotWithRatio(map_YI_iter->second.uniqueName + "_Stack_Sub",    "Control Region Yields", forTheQCDOnlyStack_Names, forTheQCDOnlyStack_Labels, forTheQCDOnlyStack_Colors, forTheQCDOnlyStack_Hists);
    createStackPlotWithRatio(map_YI_iter->second.uniqueName + "_Stack_Sub_RB", "Control Region Yields", forTheQCDOnlyStack_Names, forTheQCDOnlyStack_Labels, forTheQCDOnlyStack_Colors, forTheQCDOnlyStack_Hists_RB, numJR_RB_bins - 1, JR_RB_bins);

    float SF_MM = -1, SF_nMM = -1, SF_MM_E = -1, SF_nMM_E = -1;
    ofstream outfile(outputdir + "/" + map_YI_iter->second.uniqueName + ".txt", std::ofstream::out);
    generateScaleFactors(outfile, data_only_Hist_CR, qcd_Hist_JR_gt_5_CR, qcd_Hist_JR_lt_5_CR, SF_nMM, SF_nMM_E, SF_MM, SF_MM_E);
  }
}

void generateScaleFactors(ofstream& p_Outfile, TH1F* p_Data_Hist, TH1F* p_QCD_Hist_nMM, TH1F* p_QCD_Hist_MM, float& SnMM, float& SnMM_E, float& SMM, float& SMM_E) {
  float Da_Oa = 0.,   Db_Ob = 0.,   QaMM = 0.,   QbMM = 0.,   QanMM = 0.,   QbnMM = 0.;
  float Da_Oa_E = 0., Db_Ob_E = 0., QaMM_E = 0., QbMM_E = 0., QanMM_E = 0., QbnMM_E = 0.;
  for(int i = 1; i <= numJR_bins / 2; i++){
    Da_Oa   += p_Data_Hist->GetBinContent(i);
    Da_Oa_E += p_Data_Hist->GetBinError(i);
    QaMM    += p_QCD_Hist_MM->GetBinContent(i);
    QaMM_E  += p_QCD_Hist_nMM->GetBinError(i);
    QanMM   += p_QCD_Hist_nMM->GetBinContent(i);
    QanMM_E += p_QCD_Hist_MM->GetBinError(i);
  }
  for(int i = numJR_bins / 2 + 1; i <= numJR_bins + 1; i++){
    Db_Ob   += p_Data_Hist->GetBinContent(i);
    Db_Ob_E += p_QCD_Hist_MM->GetBinError(i);
    QbMM    += p_QCD_Hist_MM->GetBinContent(i);
    QbMM_E  += p_Data_Hist->GetBinError(i);
    QbnMM   += p_QCD_Hist_nMM->GetBinContent(i);
    QbnMM_E += p_QCD_Hist_nMM->GetBinError(i);
  }
  SnMM = (QaMM * Db_Ob - QbMM * Da_Oa) / (QaMM * QbnMM - QbMM * QanMM);
  SMM  = (Da_Oa - SnMM * QanMM) / QaMM;
  SnMM_E = sqrt( pow(QaMM * QbnMM - QbMM * QanMM, -2) * (pow(QaMM * Db_Ob_E, 2) + pow(QbMM * Da_Oa_E, 2))
               + pow(Da_Oa * QbnMM - Db_Ob * QanMM, 2) * pow(QaMM * QbnMM - QbMM * QanMM, -4) * (pow(QbMM * QaMM_E, 2) + pow(QaMM * QbMM_E, 2))
               + pow(QaMM * Db_Ob - QbMM * Da_Oa, 2) * pow(QaMM * QbnMM - QbMM * QanMM, -4) * (pow(QaMM * QbnMM_E, 2) + pow(QbMM * QanMM_E, 2)) );
  SMM_E  = sqrt( pow(QaMM * QbnMM - QbMM * QanMM, -2) * (pow(QanMM * Db_Ob_E, 2) + pow(QbnMM * Da_Oa_E, 2))
               + pow(Da_Oa * QbnMM - Db_Ob * QanMM, 2) * pow(QaMM * QbnMM - QbMM * QanMM, -4) * (pow(QanMM * QbMM_E, 2) + pow(QbnMM * QaMM_E, 2))
               + pow(Da_Oa * QbMM - Db_Ob * QaMM, 2) * pow(QaMM * QbnMM - QbMM * QanMM, -4) * (pow(QbnMM * QanMM_E, 2) + pow(QanMM * QbnMM_E, 2)) );
  p_Outfile << "Da_Oa = " << Da_Oa << " +/- " << Da_Oa_E << endl;
  p_Outfile << "Db_Ob = " << Db_Ob << " +/- " << Db_Ob_E << endl;
  p_Outfile << "QaMM  = " << QaMM  << " +/- " << QaMM_E  << endl;
  p_Outfile << "QbMM  = " << QbMM  << " +/- " << QbMM_E  << endl;
  p_Outfile << "QanMM = " << QanMM << " +/- " << QanMM_E << endl;
  p_Outfile << "QbnMM = " << QbnMM << " +/- " << QbnMM_E << endl;
  p_Outfile << "SnMM  = " << SnMM  << " +/- " << SnMM_E  << endl;
  p_Outfile << "SMM   = " << SMM   << " +/- " << SMM_E   << endl;
}
