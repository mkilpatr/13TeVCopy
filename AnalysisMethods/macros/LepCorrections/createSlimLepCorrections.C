#include <fstream>
#include <iostream>
#include <iomanip>
#include "TF1.h"
#include "TTree.h"
#include "TLine.h"
#include "TLegend.h"
#include "TString.h"
#include "TLatex.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TRandom.h"
#include "THistPainter.h"
#include "TCanvas.h"
#include <string.h>
#include <math.h>
#include <vector>

double addUnc(std::vector<double> errors){
  double errorSum = 0;
  for(unsigned int i=0; i<errors.size(); i++) errorSum += pow(errors.at(i),2);
  return sqrt(errorSum);
} // addUnc()

double multiplyUnc(std::vector<double> errors,std::vector<double> vals, double multiplied){
  double errorSum = 0;
  for(unsigned int i = 0; i < errors.size(); i++) errorSum += pow(errors.at(i),2)/(pow(vals.at(i),2));
  return multiplied*sqrt(errorSum);
} // multiplyUnc()

//root -l -b -q "../CMSSW_7_4_11/src/AnalysisMethods/macros/LepCorrections/createSlimLepCorrections.C+"
void createSlimLepCorrections(){
  TString fileTree   = "Events";
  TString scaleName  = "weight*puWeight";
  TString varDraw    = "met";
  TString dataString = "trees/151008_lepactivity/data_tree.root";
  TString mcString   = "trees/151008_lepactivity/sm_tree.root";
  TString LumiString = "test"; // string to track saved hists of corrections
  TString Lumi       = "0.2092";

  // event selection (relaxed SR: no mt cut, relaxed dphij123met cuts)
  //TString triggerSelection = " && passdijetmet";
  //TString baseline         = "met>200 && njets60 >= 2 && njets>=5 && nbjets>=1 && dphij12met>0.4 && dphij3met>0.4";
  TString triggerSelection = " && passdijetmet && passjson && passcscflt && passeebadscflt && passhbheflt"; // passhbhefixflt  passhbheflt
  TString baseline         = "met>200 && njets60 >= 2 && j1pt>75 && j2pt>75 && njets>=5 && nlbjets>=2 && nbjets>=1 && dphij12met>0.4 && dphij3met>0.4";
  TString bin              = ""; //"&& leptonpt>50"; // " && leptonpt>10 "; //  && leptonpt<100    && abs(leptoneta)>1    && htalonglep>200
  std::cout << "using bin: " << bin << std::endl;

  // fill gen/reco cut string vectors (for looping over el,mu,tau later)
  std::vector<TString> genNoLep;
  genNoLep.push_back(" && ngoodgenele==0"  ); // zero gen electrons
  genNoLep.push_back(" && ngoodgenmu==0"   ); // zero gen muons
  genNoLep.push_back(" && npromptgentau==0"); // zero gen taus
  std::vector<TString> genOneLep;
  genOneLep.push_back(" && ngoodgenele>=1"  ); // at least one gen electron
  genOneLep.push_back(" && ngoodgenmu>=1"   ); // at least one gen muon
  genOneLep.push_back(" && npromptgentau>=1"); // at least one gen tau
  // orthogonalize lep selection at reco level
  std::vector<TString> recoLepCuts;
  //recoLepCuts.push_back(" && nvetolele>=1 && ngoodgenmu==0"               ); // reco electrons (no muons)
  recoLepCuts.push_back(" && nvetolele>=1 && nvetomu==0"                  ); // reco electrons (no muons)
  recoLepCuts.push_back(" && nvetomu  >=1"                                ); // reco muons
  //recoLepCuts.push_back(" && nvetotau >=1 && nvetolele==0 && nvetomu==0"  ); // reco taus (no electrons, muons)
  recoLepCuts.push_back(" && nvetohpstaus >=1 && nvetolele==0 && nvetomu==0"  ); // reco taus (no electrons, muons)

	// load input files
	TFile *mcFile   = new TFile(mcString);
  TFile *dataFile = new TFile(dataString);
  TTree *mcTree   = (TTree*)mcFile  ->Get(fileTree);
  TTree *dataTree = (TTree*)dataFile->Get(fileTree);

  // histogram to collect the corrections
  // bins (1,2,3) are for (electrons, muons, taus)
  // bin 4 is for fakes (will be set to 1pm0)
	TH1F* lepCor = new TH1F("lepCor", "lepCor", 4, 0, 4); lepCor->Sumw2();

	// histograms to collect various pieces
  TH1F* dataOneLep  = new TH1F("dataOneLep" , "dataOneLep" , 1, 0, 10000); dataOneLep ->Sumw2();
	TH1F* dataInc     = new TH1F("dataInc"    , "dataInc"    , 1, 0, 10000); dataInc    ->Sumw2();
  TH1F* mcOneLep    = new TH1F("mcOneLep"   , "mcOneLep"   , 1, 0, 10000); mcOneLep   ->Sumw2();
  TH1F* mcNoLep     = new TH1F("mcNoLep"    , "mcNoLep"    , 1, 0, 10000); mcNoLep    ->Sumw2();
  TH1F* mcInc       = new TH1F("mcInc"      , "mcInc"      , 1, 0, 10000); mcInc      ->Sumw2();
  TH1F* mcGenLep    = new TH1F("mcGenLep"   , "mcGenLep"   , 1, 0, 10000); mcGenLep   ->Sumw2();
	TH1F* mcEffHist   = new TH1F("mcEffHist"  , "mcEffHist"  , 1, 0, 10000); mcEffHist  ->Sumw2();
  TH1F* mcFakeHist  = new TH1F("mcFakeHist" , "mcFakeHist" , 1, 0, 10000); mcFakeHist ->Sumw2();
  TH1F* mcGenEff    = new TH1F("mcGenEff"   , "mcGenEff"   , 1, 0, 10000); mcGenEff   ->Sumw2();
  TH1F* dataEffHist = new TH1F("dataEffHist", "dataEffHist", 1, 0, 10000); dataEffHist->Sumw2();

  ofstream yieldfile_; // table on Sep7 s2
  yieldfile_.open("yieldsLep.tex");
  yieldfile_ << "\\begin{center}"               << std::endl
             << "\\begin{tabular}{|c|c|c|c|c|}" << std::endl
             << "\\hline"                       << std::endl
             << "Lepton & "
             << "$\\frac{\\text{Data}_{CR_\\ell}}{\\text{Data}_{inc}}$ & "
             << "$\\frac{\\text{MC}_{inc}}{\\text{MC}_{\\ell - matched }} $ & "
             <<	"$ \\frac{\\text{MC}_{\\ell - fake}}{\\text{MC}_{\\ell - matched}}$	 & "
             <<	"LepCorr$_{\\ell}$ \\\\"        << std::endl
             << "\\hline"                       << std::endl;


  // the corrections and uncertanty   are
	for(int i=0; i<3; i++) {
	  if(i==0) std::cout << std::endl << "starting electrons..." << std::endl;
	  if(i==1) std::cout << std::endl << "starting muons..."     << std::endl;
	  if(i==2) std::cout << std::endl << "starting taus..."      << std::endl;

	  // fill histograms with events in various regions
	  dataTree->Draw(varDraw+">>dataOneLep",                       "(" + baseline + triggerSelection + recoLepCuts.at(i)                   + bin + ")");
	  dataTree->Draw(varDraw+">>dataInc"   ,                       "(" + baseline + triggerSelection                                       +       ")");
	  mcTree  ->Draw(varDraw+">>mcOneLep"  , scaleName+"*"+Lumi + "*(" + baseline + triggerSelection + recoLepCuts.at(i) + genOneLep.at(i) + bin + ")");
	  mcTree  ->Draw(varDraw+">>mcNoLep"   , scaleName+"*"+Lumi + "*(" + baseline + triggerSelection + recoLepCuts.at(i) + genNoLep.at(i)  + bin + ")");
	  mcTree  ->Draw(varDraw+">>mcInc"     , scaleName+"*"+Lumi + "*(" + baseline + triggerSelection                                       +       ")");
	  mcTree  ->Draw(varDraw+">>mcGenLep"  , scaleName+"*"+Lumi + "*(" + baseline + triggerSelection                     + genOneLep.at(i) +       ")");

	  // for "by-hand cross-check"
	  double dummyError;
	  std::cout << "The index i = (0,1,2) corresponds to (ele,mu,tau)"                                << std::endl
	            << "Printing out initial instantiating results"                                       << std::endl
	            << "The cutstring recoLepCuts reads " << recoLepCuts.at(i)                            << std::endl
	            << "The integral of dataOneLep is "   << dataOneLep->IntegralAndError(0,2,dummyError) << std::endl
	            << "The integral of dataInc is    "   << dataInc   ->IntegralAndError(0,2,dummyError) << std::endl
	            << "The cutstring genOneLep reads "   << genOneLep.at(i)                              << std::endl
	            << "The integral of mcOneLep is "     << mcOneLep->IntegralAndError(0,2,dummyError)   << std::endl
	            << "The cutstring genOneLep reads "   << genNoLep.at(i)                               << std::endl
	            << "The integral of mcNoLep is "      << mcNoLep->IntegralAndError(0,2,dummyError)    << std::endl
	            << "The integral of mcInc is   "      << mcInc  ->IntegralAndError(0,2,dummyError)    << std::endl;

	  // pieces of the correction
	  mcEffHist  ->Divide(mcOneLep  ,mcInc,    1, 1, "B");
	  mcFakeHist ->Divide(mcNoLep   ,mcOneLep           );
	  mcGenEff   ->Divide(mcOneLep  ,mcGenLep, 1, 1, "B");
	  dataEffHist->Divide(dataOneLep,dataInc,  1, 1, "B");

	  // for "by-hand cross-check": get pieces and combine
	  double mcCorrError;   double mcCorr   = mcEffHist  ->IntegralAndError(0,2,mcCorrError  );
	  double dataCorrError; double dataCorr = dataEffHist->IntegralAndError(0,2,dataCorrError);
	  double mcFakeError;   double fakeCorr = mcFakeHist ->IntegralAndError(0,2,mcFakeError  );
	  double lepCorr = (dataCorr/mcCorr) - fakeCorr;

    // combine pieces
    TH1F* LepCorrHist = (TH1F*)dataEffHist->Clone("LepCorrHist");
    LepCorrHist->Divide(mcEffHist);
    LepCorrHist->Add(mcFakeHist,-1);
    double lepCorrError2; double lepCorr2 = LepCorrHist->IntegralAndError(0,2,lepCorrError2);

	  // for "by-hand cross-check": get errors
	  std::vector<double>tempErrors; tempErrors.push_back(mcCorrError);  tempErrors.push_back(dataCorrError);
	  std::vector<double>tempVals;   tempVals.push_back(1.0/mcCorr);	       tempVals.push_back(dataCorr);
	  double errors2 = multiplyUnc(tempErrors,tempVals,(dataCorr/mcCorr));

	  tempErrors.clear(); tempVals.clear();
	  tempErrors.push_back(mcFakeError);
	  tempErrors.push_back(errors2);
	  double lepCorrError = addUnc(tempErrors);
	  tempErrors.clear();tempVals.clear();

	  // print out results
	  std::cout << "Printing out initial correction results"                                                   << std::endl
	            << "The value of the term mcCorr is   "           << mcCorr        << " +/- " << mcCorrError   << std::endl
	            << "The value of the term dataCorr is "           << dataCorr      << " +/- " << dataCorrError << std::endl
	            << "The value of the term fakeCorr is "           << fakeCorr      << " +/- " << mcFakeError   << std::endl
	            << "The value of the total lepCorr is "           << lepCorr                                   << std::endl
	            << "The value of the total lepCorr2 is "          << lepCorr2                                  << std::endl
              << "The value of the total error of lepCorr is  " << lepCorrError                              << std::endl
	            << "The value of the total error of lepCorr2 is " << lepCorrError2                             << std::endl;

    // for tables with data and mc eff
    double effMCError = 0; double effMC = mcGenEff   ->IntegralAndError(0,2,effMCError);
    double corrError  = 0; double corr  = LepCorrHist->IntegralAndError(0,2,corrError  );
    TH1F* effDataHist = (TH1F*)LepCorrHist->Clone("effDataHist");
    effDataHist->Multiply(mcEffHist);
    double effDataError = 0; double effData = effDataHist->IntegralAndError(0,2,effDataError);
    std::cout << "===== ===== ===== ===== ===== ===== ===== ===== " << std::endl
              //<< "data eff:   " << effData << "  +/-  " << effDataError << std::endl
              << "MC   eff:   " << effMC   << "  +/-  " << effMCError   << std::endl
              << "correction: " << corr    << "  +/-  " << corrError    << std::endl
              << "===== ===== ===== ===== ===== ===== ===== ===== " << std::endl;

	  // fill the appropriate bin in the hist collecting all corrections
    std::cout << "The value of the net correction is " << lepCorr2 << " +/- " << lepCorrError2 << std::endl;
	  lepCor->SetBinContent(i+1,lepCorr2     );
	  lepCor->SetBinError  (i+1,lepCorrError2);

	  // add a line for the table
	  if(i == 0) yieldfile_ << "electron &";
	  if(i == 1) yieldfile_ << "muon &";
	  if(i == 2) yieldfile_ << "tau &";
	  yieldfile_ << std::fixed;
	  yieldfile_.precision(3);
	  yieldfile_ << dataCorr << " $\\pm$ " << dataCorrError << " & " << mcCorr   << " $\\pm$ " << mcCorrError   << " & ";
	  yieldfile_ << fakeCorr << " $\\pm$ " << mcFakeError   << " & " << lepCorr2 << " $\\pm$ " << lepCorrError2 << " \\\\ " << std::endl;
	  yieldfile_ << "\\hline" << std::endl;

	  std::cout << std::endl;
  } // int i = 0; i < 3; i++

	// fill the final "fake" bin
	lepCor->SetBinContent(4,1); // fake bin
	lepCor->SetBinError(4,0);

	// end the table
  yieldfile_ << "\\end{tabular}" << std::endl;
  yieldfile_ << "\\end{center}"  << std::endl;

  // save the histrogram
	TFile* file = new TFile("lepCorr_" + LumiString + ".root","RECREATE");
	lepCor->Write();
	file->Write();

} // createSlimLepCorrections()
