#if !defined(__CINT__) || defined(__MAKECINT__)
#include "HMParameters.hh"

using namespace std;
using namespace EstTools;

const string inputDir  = "path_to_trees";
const string outputDir = "MakeQCDRespTailSF";
const TString header = "#sqrt{s} = 13 TeV, L = " + lumistr + " fb^{-1}";
TString local_METPresel, local_ResTailExtraCuts;
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
TTree* tree_data = getTree(inputDir + "/met_tree_skimmed.root");
TTree* tree_qcd  = getTree(inputDir + "/qcd_tree_skimmed.root");
TTree* tree_bkg  = getTree(inputDir + "/nonQCD_tree_skimmed.root");

void generateScaleFactorsAndUncs();
void getTailSFRegion();
TTree* getTree(TString filename);
void setTitleOffset(TCanvas *c, double xOff = .950, double yOff = 1.400);

#endif

void MakeQCDRespTailSF(){
  StyleTools::SetStyle();
  local_ResTailExtraCuts = "(((dphij1met < .1) || (dphij2met < .1)) && (nvetolep == 0) && (pseudoRespPassFilter == 1))";
  local_METPresel = "met >= 250" + datasel;
//  local_METPresel = "met >= 200 && j2pt >= 75" + datasel;
//  local_METPresel = "met >= 200 && met < 225 && j2pt >= 75" + datasel;
//  local_METPresel = "met >= 225 && met < 250 && j2pt >= 75" + datasel;
//  local_METPresel = "met >= 225 && j2pt >= 75" + datasel;
//  local_METPresel = "met >= 200 && met < 250 && j2pt >= 75" + datasel;
//  local_METPresel = "met >= 250 && met < 300 && j2pt >= 75" + datasel;
//  local_METPresel = "met >= 300 && j2pt >= 75" + datasel;
//  local_METPresel = "met >= 250 && j2pt >= 75 && njets <= 5" + datasel;
//  local_METPresel = "met >= 250 && j2pt >= 75 && nbjets <= 1" + datasel;
//  local_METPresel = "met >= 250 && j2pt >= 75 && (pseudoRespPseudoGenPT / pseudoRespPseudoGenHT) > 0.45" + datasel;
//  local_METPresel = "met >= 250 && j2pt >= 75 && (pseudoRespPseudoGenPT / ht) > 0.70" + datasel;

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
}

void generateScaleFactorsAndUncs(){
  TRandom3 * rand = new TRandom3(1234);
  //First get ttbar+W SFs
  TString ttbarWNormSelNames[] = {"Incl", "bTag"};
  HistogramGetter normHisto ("norm", "met", "norm", 1, 150, 1000);
  for(unsigned int iS = 0; iS < ttbarWNormSels.size(); ++iS){
    TH1F* dataH  = normHisto.getHistogram(tree_data, ttbarWNormSels[iS], "1.0",   TString::Format("Data_%i",  iS);
    TH1F* qcdH   = normHisto.getHistogram(tree_qcd,  ttbarWNormSels[iS],  wgtvar, TString::Format("qcd_%i",   iS);
    TH1F* otherH = normHisto.getHistogram(tree_bkg,  ttbarWNormSels[iS],  wgtvar, TString::Format("other_%i", iS);
    otherH->Add(qcdH);
    dataH->Divide(otherH);
    cout << "ttbar+W norm: " << ttbarWNormSelNames[iS] << " " << dataH->GetBinContent(1) << " +/- " << dataH->GetBinError(1) << endl;
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

  TString sel = local_METPresel + " && " local_ResTailExtraCuts;
  vector <double> SF;
  vector <double> SFTotStatUnc;
  vector <double> SFTotDataStatUnc;
  vector <double> SFNoBKGUncDataStatUnc;
  vector <double> SFMCStatUnc;
  vector <double> SFHeavyUp;
  vector <double> SFLightUp;

  //Now do the central value
  cout << "Starting first pass: " << endl;
  cout << "Loading data!" << endl;
  SFGetter::PseudoVectorGausGenerator dataVector(dataTree, bkgTree, &bkgSFs, &bkgUNCs, sel, "1.0", wgtvar, recoSels, rand);
  cout << "Loading MC!" << endl;
  SFGetter::PseudoMatrixBootstrapGenerator mcMatrix(qcdTree, sel, wgtvar, recoSels, genSels, 50, "bootstrapWeight");
  SFGetter::MatrixSolver solver(&dataVector, &mcMatrix, 10000);
  SF = solver.getSF();
  SFTotStatUnc = solver.getSFError();
  SFTotDataStatUnc = solver.getSFDataError();
  SFMCStatUnc = solver.getSFMCError();

  //Now do it again without an uncertainty on the background subtraction
  cout << "Starting second pass: " << endl;
  cout << "Loading data!" << endl;
  SFGetter::PseudoVectorGausGenerator dataVectorNoBKGUnc(dataTree, bkgTree, &bkgSFs, 0, sel, "1.0", wgtvar, recoSels, rand);
  mcMatrix.reset();
  SFGetter::MatrixSolver solverNoBKGUnc(&dataVectorNoBKGUnc, &mcMatrix, 10000);
  SFNoBKGUncDataStatUnc = solverNoBKGUnc.getSFDataError();

  //Get btagup heavy up unc
  const TString upHeavyMCWeight = lumistr + "*weight*truePUWeight*upBTagHeavyWeight";
  cout << "Starting third pass: " << endl;
  cout << "Loading data!" << endl;
  SFGetter::PseudoVectorGausGenerator dataVectorUpHeavy(dataTree, bkgTree, &bkgSFs, &bkgUNCs, sel, "1.0", upHeavyMCWeight, recoSels, rand);
  cout << "Loading MC!" << endl;
  SFGetter::PseudoMatrixGausGenerator mcMatrixUpHeavy(qcdTree, sel, upHeavyMCWeight, recoSels, genSels, rand);
  SFGetter::MatrixSolver solverUpHeavy(&dataVectorUpHeavy, &mcMatrixUpHeavy, 10000);
  SFHeavyUp = solverUpHeavy.getSF();

  //Get btagup light up unc
  const TString upLightMCWeight = lumistr + "*weight*truePUWeight*upBTagLightWeight";
  cout << "Starting fourth pass: " << endl;
  cout << "Loading data!" << endl;
  SFGetter::PseudoVectorGausGenerator dataVectorUpLight(dataTree, bkgTree, &bkgSFs, &bkgUNCs, sel, "1.0", upLightMCWeight, recoSels, rand);
  cout << "Loading MC!" << endl;
  SFGetter::PseudoMatrixGausGenerator mcMatrixUpLight(qcdTree, sel, upLightMCWeight, recoSels, genSels, rand);
  SFGetter::MatrixSolver solverUpLight(&dataVectorUpLight, &mcMatrixUpLight, 10000);
  SFLightUp = solverUpLight.getSF();

  //Print results;
  int nDim = SF.size();
  for(unsigned int i = 0; i < nDim; ++i) cout << genSelNames[i].Data() << "\t"; cout << endl;
  for(unsigned int i = 0; i < nDim; ++i) cout << TString::Format("%.2f",SF[i]                                                                                                               ) <<"\t"; cout << endl;
  for(unsigned int i = 0; i < nDim; ++i) cout << TString::Format("%.2f",SFTotStatUnc[i]                                                                                                     ) <<"\t"; cout << endl;
  for(unsigned int i = 0; i < nDim; ++i) cout << TString::Format("%.2f",SFTotDataStatUnc[i]                                                                                                 ) <<"\t"; cout << endl;
  for(unsigned int i = 0; i < nDim; ++i) cout << TString::Format("%.2f",SFNoBKGUncDataStatUnc[i]                                                                                            ) <<"\t"; cout << endl;
  for(unsigned int i = 0; i < nDim; ++i) cout << TString::Format("%.2f",SFMCStatUnc[i]                                                                                                      ) <<"\t"; cout << endl;
  for(unsigned int i = 0; i < nDim; ++i) cout << TString::Format("%.2f",SFHeavyUp[i]                                                                                                        ) <<"\t"; cout << endl;
  for(unsigned int i = 0; i < nDim; ++i) cout << TString::Format("%.2f",SFLightUp[i]                                                                                                        ) <<"\t"; cout << endl;
  for(unsigned int i = 0; i < nDim; ++i) cout << TString::Format("%.2f",SFHeavyUp[i] - SF[i]                                                                                                ) <<"\t"; cout << endl;
  for(unsigned int i = 0; i < nDim; ++i) cout << TString::Format("%.2f",SFLightUp[i] - SF[i]                                                                                                ) <<"\t"; cout << endl;
  for(unsigned int i = 0; i < nDim; ++i) cout << TString::Format("%.2f",TMath::Sqrt(TMath::Abs(SFTotDataStatUnc[i]*SFTotDataStatUnc[i] - SFNoBKGUncDataStatUnc[i]*SFNoBKGUncDataStatUnc[i]))) <<"\t"; cout << endl;

  vector<double> allUnc;
  for(unsigned int i = 0; i < nDim; ++i)  {
    allUnc.push_back(TMath::Sqrt(SFTotStatUnc[i] *SFTotStatUnc[i]  + (SFLightUp[i] - SF[i])*(SFLightUp[i] - SF[i]) + (SFHeavyUp[i] - SF[i])*(SFHeavyUp[i] - SF[i]) ));
//    allUnc.push_back(TMath::Sqrt(SFTotStatUnc[i] *SFTotStatUnc[i]));
  }

  //Print table
  gSystem->mkdir(TString(outputDir), true);
  ofstream tabFile;
  tabFile.open (outputDir + "/qcd_tail_sf.tex", ios::out | ios::trunc);
  assert(tabFile.is_open());
  tabfile << "\\begin{table}[h!]" << endl;
  tabfile << "  \\caption{\\label{tab:qcd_tailsf}}" << endl;
  tabfile << "  \\begin{center}" << endl;
  tabfile << "    \\begin{tabular}{|c|"; for(unsigned int i = 0; i < nDim; ++i) cout << "c|"; cout << "}" << endl;
  tabfile << "      \\hline" << endl;
  tabfile << "       & ";                    for(unsigned int i = 0; i < nDim; ++i){ TString tempString = genSelMathNames[i]                                                                                                                            + (i == nDim - 1 ? "" : " & "); cout << tempString; } cout << " \\\\ \\hline" << endl;
  tabfile << "      Correction & ";          for(unsigned int i = 0; i < nDim; ++i){ TString tempString = TString::Format("%.2f", SF[i])                                                                                                                + (i == nDim - 1 ? "" : " & "); cout << tempString; } cout << " \\\\ \\hline" << endl;
  tabfile << "      \\hline" << endl;
  tabfile << "      Data statistics & ";     for(unsigned int i = 0; i < nDim; ++i){ TString tempString = TString::Format("%.2f", SFNoBKGUncDataStatUnc[i])                                                                                             + (i == nDim - 1 ? "" : " & "); cout << tempString; } cout << " \\\\ \\hline" << endl;
  tabfile << "      QCD MC statistics & ";   for(unsigned int i = 0; i < nDim; ++i){ TString tempString = TString::Format("%.2f", SFMCStatUnc[i])                                                                                                       + (i == nDim - 1 ? "" : " & "); cout << tempString; } cout << " \\\\ \\hline" << endl;
  tabfile << "      Non-QCD subtraction & "; for(unsigned int i = 0; i < nDim; ++i){ TString tempString = TString::Format("%.2f", TMath::Sqrt(TMath::Abs(SFTotDataStatUnc[i]*SFTotDataStatUnc[i] - SFNoBKGUncDataStatUnc[i]*SFNoBKGUncDataStatUnc[i]))) + (i == nDim - 1 ? "" : " & "); cout << tempString; } cout << " \\\\ \\hline" << endl;
  tabfile << "      Light b-tag SF & ";      for(unsigned int i = 0; i < nDim; ++i){ TString tempString = TString::Format("%.2f", TMath::Abs(SFLightUp[i] - SF[i]) )                                                                                    + (i == nDim - 1 ? "" : " & "); cout << tempString; } cout << " \\\\ \\hline" << endl;
  tabfile << "      Heavy b-tag SF & ";      for(unsigned int i = 0; i < nDim; ++i){ TString tempString = TString::Format("%.2f", TMath::Abs(SFHeavyUp[i] - SF[i]) )                                                                                    + (i == nDim - 1 ? "" : " & "); cout << tempString; } cout << " \\\\ \\hline" << endl;
  tabfile << "      \\hline" << endl;
  tabfile << "      Total unc. & ";          for(unsigned int i = 0; i < nDim; ++i){ TString tempString = TString::Format("%.2f", allUnc[i] )                                                                                                           + (i == nDim - 1 ? "" : " & "); cout << tempString; } cout << " \\\\ \\hline" << endl;
  tabfile << "    \\end{tabular}" << endl;
  tabfile << "  \\end{center}" << endl;
  tabfile << "\\end{table}" << endl;
  tabFile.close();

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

  TFile * corrFile = new TFile(outputDir + "/qcdJetRespTailCorr.root", "recreate");
  corrHist->Write();
  corrFile->Close();
}

void getTailSFRegion(){
  gSystem->mkdir(TString(outputDir), true);

  TString presel = local_METPresel + " && " local_ResTailExtraCuts;
  vector <int> colors = { 821, 811, 812 };
  double bins[] = { 0, .1, .33, .5, .66, .8, 1 };
  HistogramGetter histG("pseudoResp", "pseudoResp", "#it{r}_{pseudo,jet}", 6, bins);

  float yMax = 0.;
  for(unsigned int iB = 0; iB < region_bins.size(); ++iB){
    TString name = TString::Format("tailSFReg_%u", iB);
    Plot* plot = new Plot(name, selRecoBinNames[iB], histG.plotInfo->xTitle, "Events");
    TString sel = TString::Format("%s && %s", presel.Data(), selRecoBins[iB].Data());
cout << sel << endl;
cout << sel + " && " + selGenBins[((iB + 1) % 2)] << endl;
    TH1F* dataH  = histG.getHistogram(tree_data, sel,                                       "1.0",  TString::Format("Data_%i",  iB);
    TH1F* otherH = histG.getHistogram(tree_bkg,  sel,                                       wgtvar, TString::Format("other_%i", iB);
    TH1F* qcdH   = histG.getHistogram(tree_qcd,  sel + " && " + selGenBins[((iB + 1) % 2)], wgtvar, TString::Format("qcd_%i",   iB);
    otherH->Scale(otherBkgNorms[iB]);
    yMax = dataH->GetMaximum();
    plot->addHist(   dataH,  "Data", "", 1, 0, 1);
    plot->addToStack(otherH, "Non-QCD MC", StyleTools::color_ttbar, 1001, 1, 1, 3);
    plot->addToStack(qcdH,   selGenBinNames[((iB + 1) % 2)], StyleTools::color_znunu, 1001, 1, 1, 3);
    for(unsigned int iSB = 0; iSB <= region_bins[iB].size(); ++iSB){
cout << sel + " && " + genSels[iSB + (iB == 0 ? 0 : region_bins[iB - 1].size() + 1)] << endl;
      qcdH = histG.getHistogram(tree_qcd, sel + " && " + genSels[iSB + (iB == 0 ? 0 : region_bins[iB - 1].size() + 1)], wgtvar, TString::Format("qcd_%u_%u", iB, iSB));
      plot->addToStack(qcdH, genSelNames[iSB + (iB == 0 ? 0 : region_bins[iB - 1].size() + 1)], colors[iSB], 1001, 1, 1, 3);
    }
    TCanvas* canvas = new TCanvas;
    plot->setHeader(header, "");
    if(iB == 1){
      plot->setYRange(0., 1.7 * yMax);
    } else {
      plot->setYRange(0., 2.5 * yMax);
    }
    plot->setLegend(.5, .65, .92, .87);
    plot->getLegend()->SetNColumns(2);
    plot->setDrawCMSLumi();
    plot->drawRatioStack(canvas);
    setTitleOffset(canvas, .850);
    canvas->SaveAs(outputDir + "/" + plot->getName() + ".pdf");
  }
}

TTree* getTree(TString filename){
  TFile* sf = new TFile(filename, "read");
  TTree* st = 0;
  sf->GetObject("Events", st);
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
