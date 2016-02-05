#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/HistogramGetter.hh"
#include "AnalysisMethods/PlotUtils/interface/SFGetter.hh"
#include "QCDPredictionHelper.h"
#include "TDirectory.h"
#include "TFile.h"

using namespace std;

void go() {

  TRandom3 * rand = new TRandom3(1234);
  //Set binning
  vector<TString> recoSels;
  recoSels.push_back("pseudoResp >= .66 && pseudoRespCSV < 0.605");
  recoSels.push_back("pseudoResp >= .33 && pseudoResp < .66 && pseudoRespCSV < 0.605");
  recoSels.push_back("pseudoResp <  .33 && pseudoRespCSV < 0.605");
  recoSels.push_back("pseudoResp >= .5 && pseudoRespCSV >= 0.890");
  recoSels.push_back("pseudoResp < . 5 && pseudoRespCSV >= 0.890");

  vector<TString> genSels;
  genSels.push_back("trueResp >= .66 && trueRespFlv !=4");
  genSels.push_back("trueResp >= .33 && trueResp < .66 && trueRespFlv !=4");
  genSels.push_back("trueResp < .33 && trueRespFlv !=4");
  genSels.push_back("trueResp >= .5 && trueRespFlv ==4");
  genSels.push_back("trueResp < .5 && trueRespFlv ==4");

  vector<TString> genSelNames;
  genSelNames.push_back("non-b, r(jet) > 0.66");
  genSelNames.push_back("non-b, 0.66 > r(jet) > 0.33");
  genSelNames.push_back("non-b, 0.33 > r(jet)");
  genSelNames.push_back("b, r(jet) > 0.50");
  genSelNames.push_back("b, 0.50 > r(jet)");


  vector<TString> genSelMathNames;
  genSelMathNames.push_back("non-b, $#it{r}_{\\mathrm{jet}} > 0.66$");
  genSelMathNames.push_back("non-b, $0.66 > #it{r}_{\\mathrm{jet}} > 0.33$");
  genSelMathNames.push_back("non-b, $0.33 > #it{r}_{\\mathrm{jet}}$");
  genSelMathNames.push_back("b, $#it{r}_{\\mathrm{jet}} > 0.50$");
  genSelMathNames.push_back("b, $0.50 > #it{r}_{\\mathrm{jet}}$");


  //First get ttbar+W SFs
  vector<double> otherBkgNorms;
  TTree * dataNormTree= QCDSupport::getTree("pieces/htmht_tree_skimmed.root");
  TTree * bkgNormTree = QCDSupport::getTree("pieces/nonQCD_tree_skimmed.root");
  TTree * qcdNormTree = QCDSupport::getTree("pieces/qcd_tree_skimmed.root");
  TString ttbarWNormSels[] = {TString::Format("%s && nvetolep >= 1 && mtlepmet < 100", QCDSupport::METPresel.Data()),TString::Format("%s && nvetolep >= 1 && mtlepmet < 100 && nbjets >= 1", QCDSupport::METPresel.Data()),""};
  TString ttbarWNormSelNames[] = {"Incl","bTag"};
  HistogramGetter normHisto ("norm","met","norm", 1,150,1000);
  for(unsigned int iS = 0; ttbarWNormSels[iS][0]; ++iS){
    TH1F * dataH  = normHisto.getHistogram(dataNormTree,ttbarWNormSels[iS],"1.0","Data");
    TH1F * otherH = normHisto.getHistogram(bkgNormTree,ttbarWNormSels[iS],QCDSupport::stdMCWeight,"Other");
    TH1F * qcdH   = normHisto.getHistogram(qcdNormTree,ttbarWNormSels[iS],QCDSupport::stdMCWeight,"MC");

    otherH->Add(qcdH);
    dataH->Divide(otherH);
    cout <<"ttbar+W norm: " << ttbarWNormSelNames[iS] <<" "<< dataH->GetBinContent(1) <<" +/- "<<dataH->GetBinError(1) <<endl;
    otherBkgNorms.push_back(dataH->GetBinContent(1));
  }


  TTree * dataTree = QCDSupport::getTree("pieces/htmht_tree_skimmed_dphi.root");
  TTree * bkgTree  = QCDSupport::getTree("pieces/nonQCD_tree_skimmed_dphi.root");
  TTree * qcdTree  = QCDSupport::getTree("pieces/qcd_tree_skimmed_dphi.root");
  vector<double> bkgSFs;
  bkgSFs.push_back(otherBkgNorms[0]);
  bkgSFs.push_back(otherBkgNorms[0]);
  bkgSFs.push_back(otherBkgNorms[0]);
  bkgSFs.push_back(otherBkgNorms[1]);
  bkgSFs.push_back(otherBkgNorms[1]);
  vector<double> bkgUNCs;
  bkgUNCs.push_back(.2);
  bkgUNCs.push_back(.2);
  bkgUNCs.push_back(.2);
  bkgUNCs.push_back(.2);
  bkgUNCs.push_back(.2);


  TString sel = TString::Format("%s && %s", QCDSupport::METPresel.Data(),QCDSupport::ResTailExtraCuts.Data());
  vector<double> SF;
  vector<double> SFTotStatUnc;
  vector<double> SFTotDataStatUnc;
  vector<double> SFNoBKGUncDataStatUnc;
  vector<double> SFMCStatUnc;

  vector<double> SFHeavyUp;
  vector<double> SFLightUp;


  //Now do the central value
  cout <<"Starting first pass: " <<endl;
  cout <<"Loading data!" <<endl;
  SFGetter::PseudoVectorGausGenerator dataVector(dataTree,bkgTree,&bkgSFs,&bkgUNCs,sel,"1.0",QCDSupport::stdMCWeight,recoSels,rand);
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
  SFGetter::PseudoVectorGausGenerator dataVectorNoBKGUnc(dataTree,bkgTree,&bkgSFs,0,sel,"1.0",QCDSupport::stdMCWeight,recoSels,rand);
  mcMatrix.reset();
  SFGetter::MatrixSolver solverNoBKGUnc(&dataVectorNoBKGUnc,&mcMatrix,10000);
  SFNoBKGUncDataStatUnc = solverNoBKGUnc.getSFDataError();

  //Get btagup heavy up unc
  const TString upHeavyMCWeight = "weight*truePUWeight*upBTagHeavyWeight*2.262";
  cout <<"Starting third pass: " <<endl;
  cout <<"Loading data!" <<endl;
  SFGetter::PseudoVectorGausGenerator dataVectorUpHeavy(dataTree,bkgTree,&bkgSFs,&bkgUNCs,sel,"1.0",upHeavyMCWeight,recoSels,rand);
  cout <<"Loading MC!" <<endl;
  SFGetter::PseudoMatrixGausGenerator mcMatrixUpHeavy(qcdTree,sel,upHeavyMCWeight,recoSels,genSels,rand);
  SFGetter::MatrixSolver solverUpHeavy(&dataVectorUpHeavy,&mcMatrixUpHeavy,10000);
  SFHeavyUp = solverUpHeavy.getSF();

  //Get btagup light up unc
  const TString upLightMCWeight = "weight*truePUWeight*upBTagLightWeight*2.262";
  cout <<"Starting fourth pass: " <<endl;
  cout <<"Loading data!" <<endl;
  SFGetter::PseudoVectorGausGenerator dataVectorUpLight(dataTree,bkgTree,&bkgSFs,&bkgUNCs,sel,"1.0",upLightMCWeight,recoSels,rand);
  cout <<"Loading MC!" <<endl;
  SFGetter::PseudoMatrixGausGenerator mcMatrixUpLight(qcdTree,sel,upLightMCWeight,recoSels,genSels,rand);
  SFGetter::MatrixSolver solverUpLight(&dataVectorUpLight,&mcMatrixUpLight,10000);
  SFLightUp = solverUpLight.getSF();


  //Print results;
  int nDim = SF.size();
  for(unsigned int i = 0; i < nDim; ++i)  cout << genSelNames[i].Data() <<"\t"; cout << endl;
  for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",SF[i]                                                                                                    )  <<"\t"; cout << endl;
  for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",SFTotStatUnc[i]                                                                                          )  <<"\t"; cout << endl;
  for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",SFTotDataStatUnc[i]                                                                                      )  <<"\t"; cout << endl;
  for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",SFNoBKGUncDataStatUnc[i]                                                                                 )  <<"\t"; cout << endl;
  for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",SFMCStatUnc[i]                                                                                           )  <<"\t"; cout << endl;
  for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",SFHeavyUp[i]                                                                                             )  <<"\t"; cout << endl;
  for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",SFLightUp[i]                                                                                             )  <<"\t"; cout << endl;
  for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",SFHeavyUp[i] - SF[i]                                                                                     )  <<"\t"; cout << endl;
  for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",SFLightUp[i] - SF[i]                                                                                     )  <<"\t"; cout << endl;
  for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",TMath::Sqrt(SFTotDataStatUnc[i]*SFTotDataStatUnc[i] - SFNoBKGUncDataStatUnc[i]*SFNoBKGUncDataStatUnc[i] ))  <<"\t"; cout << endl;

  vector<double> allUnc;
  for(unsigned int i = 0; i < nDim; ++i)  {
    allUnc.push_back(TMath::Sqrt(SFTotStatUnc[i] *SFTotStatUnc[i]  + (SFLightUp[i] - SF[i])*(SFLightUp[i] - SF[i]) + (SFHeavyUp[i] - SF[i])*(SFHeavyUp[i] - SF[i]) ));
  }

  //Print table
  cout << "\\begin{table}[h!]" <<endl;
  cout << "\\caption{\\label{tab:qcd_tailsf}}"<<endl;
  cout << "\\begin{center}";
  cout << "\\begin{tabular}{|c|"; for(unsigned int i = 0; i < nDim; ++i) cout << "c|"; cout << "}"<<endl;
  cout << "\\hline"<<endl;
  cout <<" & "; for(unsigned int i = 0; i < nDim; ++i)  cout << genSelMathNames[i].Data() <<" & "; cout <<"\\hline" << endl;
  cout <<"Correction & ";for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",SF[i]) <<" & "; cout <<"\\hline" << endl;
  cout << "\\hline"<<endl;
  cout <<"Data statistics & ";for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",SFNoBKGUncDataStatUnc[i]) <<" & "; cout <<"\\hline" << endl;
  cout <<"QCD MC statistics & ";for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",SFMCStatUnc[i]) <<" & "; cout <<"\\hline" << endl;
  cout <<"Non-QCD subtraction & ";for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",TMath::Sqrt(SFTotDataStatUnc[i]*SFTotDataStatUnc[i] - SFNoBKGUncDataStatUnc[i]*SFNoBKGUncDataStatUnc[i] )) <<" & "; cout <<"\\hline" << endl;
  cout <<"Light b-tag SF & ";for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",TMath::Abs(SFLightUp[i] - SF[i]) ) <<" & "; cout <<"\\hline" << endl;
  cout <<"Heavy b-tag SF & ";for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",TMath::Abs(SFHeavyUp[i] - SF[i]) ) <<" & "; cout <<"\\hline" << endl;
  cout << "\\hline" <<endl;
  cout <<"Total unc. & ";for(unsigned int i = 0; i < nDim; ++i)  cout << TString::Format("%.2f",allUnc[i] ) <<" & "; cout <<"\\hline" << endl;
  cout <<"\\end{tabular}"<<endl;
  cout <<"\\end{center}"<<endl;
  cout <<"\\end{table}"<<endl;
}


#endif

void MakeQCDRespTailSF(){
  StyleTools::SetStyle();
  go();
}
