#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/HistogramGetter.hh"
#include "AnalysisMethods/PlotUtils/interface/SFGetter.hh"
#include "AnalysisTools/TreeReader/interface/Defaults.h"
#include "QCDPredictionHelper.h"
#include "TDirectory.h"
#include "TFile.h"

using namespace std;

TString local_METPresel;
vector <vector <TString> > region_bins;
vector <TString> selRecoBins;
vector <TString> selRecoBinNames;
vector <TString> selGenBins;
vector <TString> selGenBinNames;
vector <TString> recoSels;
vector <TString> genSels;
vector <TString> genSelNames;
vector <TString> genSelMathNames;
vector <TString> ttbarWNormSels;
vector <double>  otherBkgNorms;
TTree* dataTree = QCDSupport::getTree(QCDSupport::inputDir + "/met_tree_skimmed_dphi.root");
TTree* bkgTree  = QCDSupport::getTree(QCDSupport::inputDir + "/nonQCD_tree_skimmed_dphi.root");
TTree* qcdTree  = QCDSupport::getTree(QCDSupport::inputDir + "/qcd_smeared_tree_skimmed_dphi.root");

void generateScaleFactorsAndUncs();
void getTailSFRegion();
void printComparison(TString p_Name, TString p_Desc, vector <TString> p_Names, vector <vector <pair <double, double> > > p_SF_and_Uncs);
void printComparison(TString p_Name, TString p_Desc, TString p_Name_1, TString p_Name_2, vector <pair <double, double> > p_SF_and_Uncs_1, vector <pair <double, double> > p_SF_and_Uncs_2);

#endif

void MakeQCDRespTailSF(){
  StyleTools::SetStyle();
  local_METPresel = QCDSupport::METPresel_forRespTailSF;
//  local_METPresel = "passjson && passeebadscflt && passhbhefltloose && passcscbeamhaloflt && passeebadsc4flt && passhbheisoflt && passdijetmet && passaddmetflts && met >= 200 && j2pt >= 75";
//  local_METPresel = "passjson && passeebadscflt && passhbhefltloose && passcscbeamhaloflt && passeebadsc4flt && passhbheisoflt && passdijetmet && passaddmetflts && met >= 200 && met < 225 && j2pt >= 75";
//  local_METPresel = "passjson && passeebadscflt && passhbhefltloose && passcscbeamhaloflt && passeebadsc4flt && passhbheisoflt && passdijetmet && passaddmetflts && met >= 225 && met < 250 && j2pt >= 75";
//  local_METPresel = "passjson && passeebadscflt && passhbhefltloose && passcscbeamhaloflt && passeebadsc4flt && passhbheisoflt && passdijetmet && passaddmetflts && met >= 225 && j2pt >= 75";
//  local_METPresel = "passjson && passeebadscflt && passhbhefltloose && passcscbeamhaloflt && passeebadsc4flt && passhbheisoflt && passdijetmet && passaddmetflts && met >= 200 && met < 250 && j2pt >= 75";
//  local_METPresel = "passjson && passeebadscflt && passhbhefltloose && passcscbeamhaloflt && passeebadsc4flt && passhbheisoflt && passdijetmet && passaddmetflts && met >= 250 && met < 300 && j2pt >= 75";
//  local_METPresel = "passjson && passeebadscflt && passhbhefltloose && passcscbeamhaloflt && passeebadsc4flt && passhbheisoflt && passdijetmet && passaddmetflts && met >= 300 && j2pt >= 75";
//  local_METPresel = "passjson && passeebadscflt && passhbhefltloose && passcscbeamhaloflt && passeebadsc4flt && passhbheisoflt && passdijetmet && passaddmetflts && met >= 250 && j2pt >= 75 && njets <= 5";
//  local_METPresel = "passjson && passeebadscflt && passhbhefltloose && passcscbeamhaloflt && passeebadsc4flt && passhbheisoflt && passdijetmet && passaddmetflts && met >= 250 && j2pt >= 75 && nbjets <= 1";
//  local_METPresel = "passjson && passeebadscflt && passhbhefltloose && passcscbeamhaloflt && passeebadsc4flt && passhbheisoflt && passdijetmet && passaddmetflts && met >= 250 && j2pt >= 75 && (pseudoRespPseudoGenPT / pseudoRespPseudoGenHT) > 0.45";
//  local_METPresel = "passjson && passeebadscflt && passhbhefltloose && passcscbeamhaloflt && passeebadsc4flt && passhbheisoflt && passdijetmet && passaddmetflts && met >= 250 && j2pt >= 75 && (pseudoRespPseudoGenPT / ht) > 0.70";

  ttbarWNormSels.push_back(TString::Format("%s && nvetolep >= 1 && mtlepmet < 100", local_METPresel.Data()));
  ttbarWNormSels.push_back(TString::Format("%s && nvetolep >= 1 && mtlepmet < 100 && nbjets >= 1", local_METPresel.Data()));

//Set binning
  selRecoBins.push_back(TString::Format("pseudoRespCSV <  %.3f", defaults::CSV_LOOSE)); selRecoBinNames.push_back("Jet is not light b-tagged");
  selRecoBins.push_back(TString::Format("pseudoRespCSV >= %.3f", defaults::CSV_MEDIUM)); selRecoBinNames.push_back("Jet is medium b-tagged");
  selGenBins.push_back("trueRespFlv != 4"); selGenBinNames.push_back("!b");
  selGenBins.push_back("trueRespFlv == 4"); selGenBinNames.push_back("b");
//  selRecoBins.push_back("pseudoRespPseudoGenPT < 500"); selRecoBinNames.push_back("p_{T, pseudo} < 500");
//  selRecoBins.push_back("pseudoRespPseudoGenPT > 500"); selRecoBinNames.push_back("p_{T, pseudo} > 500");
//  selGenBins.push_back("trueRespGenPT < 500"); selGenBinNames.push_back("p_{T, true} < 500");
//  selGenBins.push_back("trueRespGenPT > 500"); selGenBinNames.push_back("p_{T, true} > 500");

  vector<TString> temp;
  region_bins.push_back(temp);
  region_bins.push_back(temp);
  region_bins[0].push_back("0.66");
  region_bins[0].push_back("0.33");
//  region_bins[0].push_back("0.5");
//  region_bins[0].push_back("0.2");
  region_bins[1].push_back("0.5");
//  region_bins[1].push_back("0.66");
//  region_bins[1].push_back("0.33");

  recoSels.push_back(selRecoBins[0] + " && pseudoResp >= " + region_bins[0][0]);
  genSels.push_back(selGenBins[0] + " && trueResp >= " + region_bins[0][0]); 
  genSelNames.push_back(selGenBinNames[0] + ", r_{jet} > " + region_bins[0][0]);
  genSelMathNames.push_back(selGenBinNames[0] + ", $\\textit{r}_{\\mathrm{jet}} > " + region_bins[0][0] + "$");
  for(int iB = 0; iB < region_bins[0].size() - 1; ++iB){
    recoSels.push_back(selRecoBins[0] + " && pseudoResp >= " + region_bins[0][iB + 1] + " && pseudoResp < " + region_bins[0][iB]);
    genSels.push_back(selGenBins[0] + " && trueResp >= " + region_bins[0][iB + 1] + " && trueResp < " + region_bins[0][iB]);
    genSelNames.push_back(selGenBinNames[0] + ", " + region_bins[0][iB + 1] + " < r_{jet} < " + region_bins[0][iB]);
    genSelMathNames.push_back(selGenBinNames[0] + ", $" + region_bins[0][iB + 1] + " < \\textit{r}_{\\mathrm{jet}} < " + region_bins[0][iB] + "$");
  }
  recoSels.push_back(selRecoBins[0] + " && pseudoResp < " + region_bins[0][region_bins[0].size() - 1]);
  genSels.push_back(selGenBins[0] + " && trueResp < " + region_bins[0][region_bins[0].size() - 1]);
  genSelNames.push_back(selGenBinNames[0] + ", " + region_bins[0][region_bins[0].size() - 1] + " > r_{jet}");
  genSelMathNames.push_back(selGenBinNames[0] + ", $" + region_bins[0][region_bins[0].size() - 1] + " > \\textit{r}_{\\mathrm{jet}}$");

  recoSels.push_back(selRecoBins[1] + " && pseudoResp >= " + region_bins[1][0]);
  genSels.push_back(selGenBins[1] + " && trueResp >= " + region_bins[1][0]);
  genSelNames.push_back(selGenBinNames[1] + ", r_{jet} > " + region_bins[1][0]);
  genSelMathNames.push_back(selGenBinNames[1] + ", $\\textit{r}_{\\mathrm{jet}} > " + region_bins[1][0] + "$");
  for(int iB = 0; iB < region_bins[1].size() - 1; ++iB){
    recoSels.push_back(selRecoBins[1] + " && pseudoResp >= " + region_bins[1][iB + 1] + " && pseudoResp < " + region_bins[1][iB]);
    genSels.push_back(selGenBins[1] + " && trueResp >= " + region_bins[1][iB + 1] + " && trueResp < " + region_bins[1][iB]);
    genSelNames.push_back(selGenBinNames[1] + ", " + region_bins[1][iB + 1] + " < r_{jet} < " + region_bins[1][iB]);
    genSelMathNames.push_back(selGenBinNames[1] + ", $" + region_bins[1][iB + 1] + " < \\textit{r}_{\\mathrm{jet}} < " + region_bins[1][iB] + "$");
  }
  recoSels.push_back(selRecoBins[1] + " && pseudoResp < " + region_bins[1][region_bins[1].size() - 1]);
  genSels.push_back(selGenBins[1] + " && trueResp < " + region_bins[1][region_bins[1].size() - 1]);
  genSelNames.push_back(selGenBinNames[1] + ", " + region_bins[1][region_bins[1].size() - 1] + " > r_{jet}");
  genSelMathNames.push_back(selGenBinNames[1] + ", $" + region_bins[1][region_bins[1].size() - 1] + " > \\textit{r}_{\\mathrm{jet}}$");

  for(int iB = 0; iB < recoSels.size(); ++iB){
    cout << recoSels[iB] << endl;
  }
  for(int iB = 0; iB < genSels.size(); ++iB){
    cout << genSels[iB] << ", " << genSelNames[iB] << ", " << genSelMathNames[iB] << endl;
  }

  generateScaleFactorsAndUncs();
  getTailSFRegion();

//  vector <TString> names = { "Baseline", "200 #leq #slash{#it{E}}_{T} < 225", "225 #leq #slash{#it{E}}_{T} < 250", "250 #leq #slash{#it{E}}_{T} < 300", "300 #leq #slash{#it{E}}_{T}" };
//  vector <vector <pair <double, double> > > SF_and_Uncs = { { make_pair(0.82, 0.07), make_pair(0.85, 0.05), make_pair(1.14, 0.13), make_pair(0.60, 0.10), make_pair(0.45, 0.11) },
//                                                            { make_pair(0.60, 0.04), make_pair(1.01, 0.04), make_pair(0.95, 0.42), make_pair(0.59, 0.08), make_pair(0.77, 0.08) }, 
//                                                            { make_pair(0.73, 0.06), make_pair(0.92, 0.04), make_pair(1.32, 0.13), make_pair(0.53, 0.09), make_pair(0.71, 0.10) },
//                                                            { make_pair(0.77, 0.07), make_pair(0.86, 0.06), make_pair(1.29, 0.12), make_pair(0.62, 0.10), make_pair(0.58, 0.10) },
//                                                            { make_pair(0.93, 0.11), make_pair(0.82, 0.10), make_pair(1.00, 0.18), make_pair(0.56, 0.12), make_pair(0.29, 0.21) } };
//  printComparison("comparison_met", "Varying #slash{#it{E}}_{T} cuts", names, SF_and_Uncs);  
//  for(int iS = 1; iS < names.size(); ++iS){
//    printComparison(TString::Format("comparison_met_%u", iS), "Varying #slash{#it{E}}_{T} cuts", names[0], names[iS], SF_and_Uncs[0], SF_and_Uncs[iS]);
//  }
//
//  vector <TString> names_no_met = { "Baseline", "#frac{p_{T,pseudo~gen}}{h_{T,pseudo~gen}} > 0.45", "#frac{p_{T,pseudo~gen}}{h_{T}} > 0.70", "N_{jets} #leq 5", "N_{bjets} #leq 1", "Subbing flavor for p_{T,true}" };
//  vector <vector <pair <double, double> > > SF_and_Uncs_no_met = { { make_pair(0.82, 0.07), make_pair(0.85, 0.05), make_pair(1.14, 0.13), make_pair(0.60, 0.10), make_pair(0.45, 0.11) },
//                                                                   { make_pair(0.88, 0.08), make_pair(0.80, 0.06), make_pair(1.12, 0.13), make_pair(0.61, 0.10), make_pair(0.41, 0.13) }, 
//                                                                   { make_pair(1.59, 1.26), make_pair(0.25, 0.63), make_pair(1.31, 0.15), make_pair(0.86, 0.54), make_pair(0.28, 0.40) }, 
//                                                                   { make_pair(0.78, 0.07), make_pair(0.84, 0.06), make_pair(1.13, 0.13), make_pair(0.56, 0.09), make_pair(0.44, 0.12) }, 
//                                                                   { make_pair(0.82, 0.07), make_pair(0.85, 0.05), make_pair(1.13, 0.13), make_pair(0.59, 0.09), make_pair(0.48, 0.13) }, 
//                                                                   { make_pair(12.69, 109.35), make_pair(-0.06, 8.71), make_pair(0.47 , 2.36), make_pair(0.60 , 1.95), make_pair(-1.03, 14.15) } }; 
//  for(int iS = 1; iS < names_no_met.size(); ++iS){
//    printComparison(TString::Format("comparison_no_met_%u", iS), TString::Format("Adding a %s cut", names_no_met[iS].Data()), names_no_met[0], names_no_met[iS], SF_and_Uncs_no_met[0], SF_and_Uncs_no_met[iS]);
//  }
}

void generateScaleFactorsAndUncs(){
  TRandom3 * rand = new TRandom3(1234);

  //First get ttbar+W SFs
  TTree* dataNormTree = QCDSupport::getTree(QCDSupport::inputDir + "/met_tree_skimmed.root");
  TTree* bkgNormTree  = QCDSupport::getTree(QCDSupport::inputDir + "/nonQCD_tree_skimmed.root");
  TTree* qcdNormTree  = QCDSupport::getTree(QCDSupport::inputDir + "/qcd_smeared_tree_skimmed.root");
  TString ttbarWNormSelNames[] = {"Incl", "bTag"};
  HistogramGetter normHisto ("norm", "met", "norm", 1, 150, 1000);
  for(unsigned int iS = 0; iS < ttbarWNormSels.size(); ++iS){
    TH1F* dataH  = normHisto.getHistogram(dataNormTree, ttbarWNormSels[iS], QCDSupport::stdWeight, "Data");
    TH1F* otherH = normHisto.getHistogram(bkgNormTree,  ttbarWNormSels[iS], QCDSupport::stdMCWeight, "Other");
    TH1F* qcdH   = normHisto.getHistogram(qcdNormTree,  ttbarWNormSels[iS], QCDSupport::stdMCWeight, "MC");
    otherH->Add(qcdH);
    dataH->Divide(otherH);
    cout <<"ttbar+W norm: " << ttbarWNormSelNames[iS] <<" "<< dataH->GetBinContent(1) <<" +/- "<<dataH->GetBinError(1) <<endl;
    otherBkgNorms.push_back(dataH->GetBinContent(1));
  }
  vector<double> bkgSFs;
  vector<double> bkgUNCs;
  for(int iB = 0; iB < region_bins.size(); ++iB){
    for(int iSB = 0; iSB <= region_bins[iB].size(); ++iSB){
      bkgSFs.push_back(otherBkgNorms[iB]);
      bkgUNCs.push_back(.2);
    }
  }

  TString sel = TString::Format("%s && %s", local_METPresel.Data(), QCDSupport::ResTailExtraCuts.Data());
  vector <double> SF;
  vector <double> SFTotStatUnc;
  vector <double> SFTotDataStatUnc;
  vector <double> SFNoBKGUncDataStatUnc;
  vector <double> SFMCStatUnc;
  vector <double> SFHeavyUp;
  vector <double> SFLightUp;

  //Now do the central value
  cout <<"Starting first pass: " <<endl;
  cout <<"Loading data!" <<endl;
  SFGetter::PseudoVectorGausGenerator dataVector(dataTree,bkgTree,&bkgSFs,&bkgUNCs,sel,QCDSupport::stdWeight,QCDSupport::stdMCWeight,recoSels,rand);
  cout <<"Loading MC!" <<endl;
  SFGetter::PseudoMatrixBootstrapGenerator mcMatrix(qcdTree,sel,QCDSupport::stdMCWeight,recoSels,genSels,50,"bootstrapWeight");
  SFGetter::MatrixSolver solver(&dataVector,&mcMatrix,10000);
  SF = solver.getSF();
  SFTotStatUnc = solver.getSFError();
  SFTotDataStatUnc = solver.getSFDataError();
  SFMCStatUnc = solver.getSFMCError();

  //Now do it again without an uncertainty on the background subtraction
  cout <<"Starting second pass: " <<endl;
  cout <<"Loading data!" <<endl;
  SFGetter::PseudoVectorGausGenerator dataVectorNoBKGUnc(dataTree,bkgTree,&bkgSFs,0,sel,QCDSupport::stdWeight,QCDSupport::stdMCWeight,recoSels,rand);
  mcMatrix.reset();
  SFGetter::MatrixSolver solverNoBKGUnc(&dataVectorNoBKGUnc,&mcMatrix,10000);
  SFNoBKGUncDataStatUnc = solverNoBKGUnc.getSFDataError();

  //Get btagup heavy up unc
  const TString upHeavyMCWeight = QCDSupport::lumi + "*weight*truePUWeight*upBTagHeavyWeight";
  cout <<"Starting third pass: " <<endl;
  cout <<"Loading data!" <<endl;
  SFGetter::PseudoVectorGausGenerator dataVectorUpHeavy(dataTree,bkgTree,&bkgSFs,&bkgUNCs,sel,QCDSupport::stdWeight,upHeavyMCWeight,recoSels,rand);
  cout <<"Loading MC!" <<endl;
  SFGetter::PseudoMatrixGausGenerator mcMatrixUpHeavy(qcdTree,sel,upHeavyMCWeight,recoSels,genSels,rand);
  SFGetter::MatrixSolver solverUpHeavy(&dataVectorUpHeavy,&mcMatrixUpHeavy,10000);
  SFHeavyUp = solverUpHeavy.getSF();

  //Get btagup light up unc
  const TString upLightMCWeight = QCDSupport::lumi + "*weight*truePUWeight*upBTagLightWeight";
  cout <<"Starting fourth pass: " <<endl;
  cout <<"Loading data!" <<endl;
  SFGetter::PseudoVectorGausGenerator dataVectorUpLight(dataTree,bkgTree,&bkgSFs,&bkgUNCs,sel,QCDSupport::stdWeight,upLightMCWeight,recoSels,rand);
  cout <<"Loading MC!" <<endl;
  SFGetter::PseudoMatrixGausGenerator mcMatrixUpLight(qcdTree,sel,upLightMCWeight,recoSels,genSels,rand);
  SFGetter::MatrixSolver solverUpLight(&dataVectorUpLight,&mcMatrixUpLight,10000);
  SFLightUp = solverUpLight.getSF();

  //Print results;
  int nDim = SF.size();
  for(unsigned int i = 0; i < nDim; ++i)  cout << genSelNames[i].Data() <<"\t"; cout << endl;
  for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",SF[i]                                                                                                               ) <<"\t"; cout << endl;
  for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",SFTotStatUnc[i]                                                                                                     ) <<"\t"; cout << endl;
  for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",SFTotDataStatUnc[i]                                                                                                 ) <<"\t"; cout << endl;
  for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",SFNoBKGUncDataStatUnc[i]                                                                                            ) <<"\t"; cout << endl;
  for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",SFMCStatUnc[i]                                                                                                      ) <<"\t"; cout << endl;
  for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",SFHeavyUp[i]                                                                                                        ) <<"\t"; cout << endl;
  for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",SFLightUp[i]                                                                                                        ) <<"\t"; cout << endl;
  for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",SFHeavyUp[i] - SF[i]                                                                                                ) <<"\t"; cout << endl;
  for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",SFLightUp[i] - SF[i]                                                                                                ) <<"\t"; cout << endl;
  for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",TMath::Sqrt(TMath::Abs(SFTotDataStatUnc[i]*SFTotDataStatUnc[i] - SFNoBKGUncDataStatUnc[i]*SFNoBKGUncDataStatUnc[i]))) <<"\t"; cout << endl;

  vector<double> allUnc;
  for(unsigned int i = 0; i < nDim; ++i)  {
    allUnc.push_back(TMath::Sqrt(SFTotStatUnc[i] *SFTotStatUnc[i]  + (SFLightUp[i] - SF[i])*(SFLightUp[i] - SF[i]) + (SFHeavyUp[i] - SF[i])*(SFHeavyUp[i] - SF[i]) ));
//    allUnc.push_back(TMath::Sqrt(SFTotStatUnc[i] *SFTotStatUnc[i]));
  }

  //Print table
  cout << "\\begin{table}[h!]" << endl;
  cout << "  \\caption{\\label{tab:qcd_tailsf}}" << endl;
  cout << "  \\begin{center}" << endl;
  cout << "    \\begin{tabular}{|c|"; for(unsigned int i = 0; i < nDim; ++i) cout << "c|"; cout << "}" << endl;
  cout << "      \\hline" << endl;
  cout << "       & ";                    for(unsigned int i = 0; i < nDim; ++i){ TString tempString = genSelMathNames[i]                                                                                                                            + (i == nDim - 1 ? "" : " & "); cout << tempString; } cout << " \\\\ \\hline" << endl;
  cout << "      Correction & ";          for(unsigned int i = 0; i < nDim; ++i){ TString tempString = TString::Format("%.2f", SF[i])                                                                                                                + (i == nDim - 1 ? "" : " & "); cout << tempString; } cout << " \\\\ \\hline" << endl;
  cout << "      \\hline" << endl;
  cout << "      Data statistics & ";     for(unsigned int i = 0; i < nDim; ++i){ TString tempString = TString::Format("%.2f", SFNoBKGUncDataStatUnc[i])                                                                                             + (i == nDim - 1 ? "" : " & "); cout << tempString; } cout << " \\\\ \\hline" << endl;
  cout << "      QCD MC statistics & ";   for(unsigned int i = 0; i < nDim; ++i){ TString tempString = TString::Format("%.2f", SFMCStatUnc[i])                                                                                                       + (i == nDim - 1 ? "" : " & "); cout << tempString; } cout << " \\\\ \\hline" << endl;
  cout << "      Non-QCD subtraction & "; for(unsigned int i = 0; i < nDim; ++i){ TString tempString = TString::Format("%.2f", TMath::Sqrt(TMath::Abs(SFTotDataStatUnc[i]*SFTotDataStatUnc[i] - SFNoBKGUncDataStatUnc[i]*SFNoBKGUncDataStatUnc[i]))) + (i == nDim - 1 ? "" : " & "); cout << tempString; } cout << " \\\\ \\hline" << endl;
  cout << "      Light b-tag SF & ";      for(unsigned int i = 0; i < nDim; ++i){ TString tempString = TString::Format("%.2f", TMath::Abs(SFLightUp[i] - SF[i]) )                                                                                    + (i == nDim - 1 ? "" : " & "); cout << tempString; } cout << " \\\\ \\hline" << endl;
  cout << "      Heavy b-tag SF & ";      for(unsigned int i = 0; i < nDim; ++i){ TString tempString = TString::Format("%.2f", TMath::Abs(SFHeavyUp[i] - SF[i]) )                                                                                    + (i == nDim - 1 ? "" : " & "); cout << tempString; } cout << " \\\\ \\hline" << endl;
  cout << "      \\hline" << endl;
  cout << "      Total unc. & ";          for(unsigned int i = 0; i < nDim; ++i){ TString tempString = TString::Format("%.2f", allUnc[i] )                                                                                                           + (i == nDim - 1 ? "" : " & "); cout << tempString; } cout << " \\\\ \\hline" << endl;
  cout << "    \\end{tabular}" << endl;
  cout << "  \\end{center}" << endl;
  cout << "\\end{table}" << endl;
cout << local_METPresel << endl;

  double xbins[] = { 0, .33, .5, .66, 1 };
  TH2F * corrHist = new TH2F("RespTailCorr", ";Response;non-b jets/b jets", 4, xbins, 2, 0.5, 2.5);

  corrHist->SetBinContent(1, 1, SF[2]); corrHist->SetBinError(1, 1, allUnc[2]);
  corrHist->SetBinContent(2, 1, SF[1]); corrHist->SetBinError(2, 1, allUnc[1]);
  corrHist->SetBinContent(3, 1, SF[1]); corrHist->SetBinError(3, 1, allUnc[1]);
  corrHist->SetBinContent(4, 1, SF[0]); corrHist->SetBinError(4, 1, allUnc[0]);
  corrHist->SetBinContent(1, 2, SF[4]); corrHist->SetBinError(1, 2, allUnc[4]);
  corrHist->SetBinContent(2, 2, SF[4]); corrHist->SetBinError(2, 2, allUnc[4]);
  corrHist->SetBinContent(3, 2, SF[3]); corrHist->SetBinError(3, 2, allUnc[3]);
  corrHist->SetBinContent(4, 2, SF[3]); corrHist->SetBinError(4, 2, allUnc[3]);

  TFile * corrFile = new TFile("qcdJetRespTailCorr.root", "recreate");
  corrHist->Write();
  corrFile->Close();
}

void getTailSFRegion(){
  const string outputDir = "MakeQCDRespTailSF";
  gSystem->mkdir(TString(outputDir), true);

  TString presel = TString::Format("%s && %s", local_METPresel.Data(), QCDSupport::ResTailExtraCuts.Data());
  vector <int> colors = { 821, 811, 812 };
  double bins[] = { 0, .1, .33, .5, .66, .8, 1 };
  HistogramGetter histG("pseudoResp", "pseudoResp", "#it{r}_{pseudo,jet}", 6, bins);

  float yMax = 0.;
  for(unsigned int iB = 0; iB < region_bins.size(); ++iB){
    TString name = TString::Format("tailSFReg_%u", iB);
    Plot* plot = new Plot(name, selRecoBinNames[iB], histG.plotInfo->xTitle, "Events");
    TString sel = TString::Format("%s && %s", presel.Data(), selRecoBins[iB].Data());
cout << sel << endl;
    TH1F* hSN = histG.getHistogram(dataTree, sel, QCDSupport::stdWeight, "tree_Data");
    yMax = hSN->GetMaximum();
    plot->addHist(hSN, "Data", "", 1, 0, 1);
    hSN = histG.getHistogram(bkgTree, sel, QCDSupport::stdMCWeight, "tree_NonQCD_MC");
    hSN->Scale(otherBkgNorms[iB]);
    plot->addToStack(hSN, "Non-QCD MC", StyleTools::color_ttbar, 1001, 1, 1, 3);
cout << sel + " && " + selGenBins[((iB + 1) % 2)] << endl;
    hSN = histG.getHistogram(qcdTree, sel + " && " + selGenBins[((iB + 1) % 2)], QCDSupport::stdMCWeight, "tree_" + selGenBinNames[((iB + 1) % 2)]);
    plot->addToStack(hSN, selGenBinNames[((iB + 1) % 2)], StyleTools::color_znunu, 1001, 1, 1, 3);
    for(unsigned int iSB = 0; iSB <= region_bins[iB].size(); ++iSB){
cout << sel + " && " + genSels[iSB + (iB == 0 ? 0 : region_bins[iB - 1].size() + 1)] << endl;
      hSN = histG.getHistogram(qcdTree, sel + " && " + genSels[iSB + (iB == 0 ? 0 : region_bins[iB - 1].size() + 1)], QCDSupport::stdMCWeight, TString::Format("tree_qcd_%u", iSB));
      plot->addToStack(hSN, genSelNames[iSB + (iB == 0 ? 0 : region_bins[iB - 1].size() + 1)], colors[iSB], 1001, 1, 1, 3);
    }
    TCanvas* canvas = new TCanvas;
    plot->setHeader(QCDSupport::header, "");
    if(iB == 1){
      plot->setYRange(0., 1.7 * yMax);
    } else {
      plot->setYRange(0., 2.5 * yMax);
    }
    plot->setLegend(.5, .65, .92, .87);
    plot->getLegend()->SetNColumns(2);
    plot->setDrawCMSLumi();
    plot->drawRatioStack(canvas, false, QCDSupport::format);
    QCDSupport::setTitleOffset(canvas, .850);
    canvas->SaveAs(outputDir + "/" + plot->getName() + "." + QCDSupport::format);
  }
}

void printComparison(TString p_Name, TString p_Desc, vector <TString> p_Names, vector <vector <pair <double, double> > > p_SF_and_Uncs){
  float yMax = 0.;
  Plot* plot = new Plot(p_Name, p_Desc, "SF Region bins", "Scale Factors");
  for(unsigned int iS = 0; iS < p_SF_and_Uncs.size(); ++iS){
    TH1F* hist = new TH1F(TString::Format("comparison_hist_%u", iS), TString::Format("comparison_hist_%u", iS), p_SF_and_Uncs[iS].size(), 0.5, p_SF_and_Uncs[iS].size() + 0.5);
cout << p_Names[iS] << endl;
    for(int iB = 0; iB < p_SF_and_Uncs[iS].size(); ++iB){
cout << p_SF_and_Uncs[iS][iB].first << " +/- " << p_SF_and_Uncs[iS][iB].second << endl;
      hist->SetBinContent(iB + 1, p_SF_and_Uncs[iS][iB].first);
      hist->SetBinError(iB + 1, p_SF_and_Uncs[iS][iB].second);
      if(yMax < p_SF_and_Uncs[iS][iB].first) yMax = p_SF_and_Uncs[iS][iB].first;
    }
    plot->addHist(hist, p_Names[iS], "E1", StyleTools::colorGetter(iS), 0, StyleTools::colorGetter(iS));
  }
  TCanvas* canvas = new TCanvas;
  plot->setYRange(0, 2. * yMax);
  plot->setHeader(QCDSupport::header, "");
  plot->setLegend(.5, .65, .92, .87);
  plot->setDrawCMSLumi();
  plot->drawRatios(canvas);
  QCDSupport::setTitleOffset(canvas, .850);
  canvas->SaveAs(plot->getName() + TString("." + QCDSupport::format));
}

void printComparison(TString p_Name, TString p_Desc, TString p_Name_1, TString p_Name_2, vector <pair <double, double> > p_SF_and_Uncs_1, vector <pair <double, double> > p_SF_and_Uncs_2){
  float yMax = 0.;
  Plot* plot = new Plot(p_Name, p_Desc, "SF Region bins", "Scale Factors");
  TH1F* hist_1 = new TH1F("comparison_hist_1", "comparison_hist_1", p_SF_and_Uncs_1.size(), 0.5, p_SF_and_Uncs_1.size() + 0.5);
  TH1F* hist_2 = new TH1F("comparison_hist_2", "comparison_hist_2", p_SF_and_Uncs_2.size(), 0.5, p_SF_and_Uncs_2.size() + 0.5);
  for(int iB = 0; iB < p_SF_and_Uncs_1.size(); ++iB){
    hist_1->SetBinContent(iB + 1, p_SF_and_Uncs_1[iB].first);
    hist_1->SetBinError(iB + 1, p_SF_and_Uncs_1[iB].second);
    hist_2->SetBinContent(iB + 1, p_SF_and_Uncs_2[iB].first);
    hist_2->SetBinError(iB + 1, p_SF_and_Uncs_2[iB].second);
    if(yMax < p_SF_and_Uncs_1[iB].first) yMax = p_SF_and_Uncs_1[iB].first;
    if(yMax < p_SF_and_Uncs_2[iB].first) yMax = p_SF_and_Uncs_2[iB].first;
  }
  plot->addHist(hist_1, p_Name_1, "E1", StyleTools::colorGetter(0), 0, StyleTools::colorGetter(0));
  plot->addHist(hist_2, p_Name_2, "E1", StyleTools::colorGetter(1), 0, StyleTools::colorGetter(1));
  TCanvas* canvas = new TCanvas;
  plot->setYRange(0, 2. * yMax);
  plot->setHeader(QCDSupport::header, "");
  plot->setLegend(.5, .65, .92, .87);
  plot->setDrawCMSLumi();
  plot->drawRatios(canvas);
  QCDSupport::setTitleOffset(canvas, .850);
  canvas->SaveAs(plot->getName() + TString("." + QCDSupport::format));
}
