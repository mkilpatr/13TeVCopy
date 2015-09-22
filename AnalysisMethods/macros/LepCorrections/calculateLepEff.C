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
//namespace  calculateLepEff{

void calculateLepEff(TString varDraw = "met",bool normalize = true);
double addUnc(std::vector<double> errors);
double multiplyUnc(std::vector<double> errors,std::vector<double> vals, double multiply);

void calculateLepEff(TString varDraw,bool normalize){
	TString  fileTree = "events";
	TString scaleName = "weight";
	TString dataString = "run/lepEffSlimmed/data_tree.root";
	TString mcString = "run/lepEffSlimmed/sm_tree.root";
	////STRING FOR FILE SAVE NAME
	TString Lumi = ".0402";

	//EVENT SELECTIONS
	TString triggerSelection             = "&&(passDiJetmet > 0)";
	TString baseline                     = "(njets60 >= 2) && (dphij12met>.4) && (dphij3met>0.4) && (met>200)";
	TString searchString 				 =  baseline + "&&(mtcsv12MET > 175)&&(dphij12met>1)&&(dphij3met>0.5)";

	////GEN LEVEL CUT STRINGS
	TString noPromptEle                  = "&&(nGoodEle == 0)";
	TString onePromptEle                 = "&&(nGoodEle >= 1)";
	TString noPromptMu                   = "&&(nGoodMu == 0)";
	TString onePromptMu                  = "&&(nGoodMu >= 1)";
	TString noPromptTau                  = "&&(nPromptTau == 0)";
	TString onePromptTau                 = "&&(nPromptTau >= 1)";

	////ORTHOGONALIZE AT RECO LEVEL
	TString recoEleCut 					 = "&&(nSelectedElectrons == 1)";
	TString recoMuCut 					 = "&&(nSelectedMuons    == 1)&&(nSelectedElectrons == 0)";
	TString recoTauCut 		     		 = "&&(nvetotau    >= 1)&&(nSelectedElectrons == 0)&&(nSelectedMuons    == 0)";

	////FILL CUT VECTORS
	std::vector<TString> recoLepCuts; 	std::vector<TString> genNoLep; 	std::vector<TString> genOneLep;
	recoLepCuts.push_back(recoEleCut);	recoLepCuts.push_back(recoMuCut);	recoLepCuts.push_back(recoTauCut);
	genNoLep.push_back(noPromptEle);	genNoLep.push_back(noPromptMu);	genNoLep.push_back(noPromptTau);
	genOneLep.push_back(onePromptEle);	genOneLep.push_back(onePromptMu);	genOneLep.push_back(onePromptTau);

	////SETUP MC FILE
	TFile *mcFile  = new TFile(mcString);
    TTree *mcTree = (TTree*)mcFile->Get(fileTree);

	TFile *dataFile  = new TFile(dataString);
    TTree *dataTree = (TTree*)dataFile->Get(fileTree);

	////CREATE CALCULATION HISTS
	TH1F* dataInc = new TH1F("dataInc","dataInc",1,0.0,1.0);								dataInc->Sumw2();
	TH1F* dataOneLep = new TH1F("dataOneLep","dataOneLep",1,0.0,1.0);						dataOneLep->Sumw2();
	TH1F* mcNoLep = new TH1F("mcNoLep","mcNoLep",1,0.0,1.0);								mcNoLep->Sumw2();
	TH1F* mcOneLep = new TH1F("mcOneLep","mcOneLep",1,0.0,1.0);								mcOneLep->Sumw2();
	TH1F* mcInc = new TH1F("mcInc","mcInc",1,0.0,1.0);										mcInc->Sumw2();

    TH1F *mcCorrErrorHist = new TH1F("mcCorrErrorHist","Test binomial",1,0.0,1.0);			mcCorrErrorHist->Sumw2();
    TH1F *mcCorrErrorHist2 = new TH1F("mcCorrErrorHist2","Test binomial",1,0.0,1.0);		mcCorrErrorHist2->Sumw2();
    TH1F *dataCorrErrorHist = new TH1F("dataCorrErrorHist","Test binomial",1,0.0,1.0);      mcCorrErrorHist->Sumw2();
    ofstream yieldfile_;
    yieldfile_.open("yieldsLep.tex");
    yieldfile_ << "\\begin{center}" << std::endl;
    yieldfile_ << "\\begin{tabular}{|c|c|c|c|c|}" << std::endl;
    yieldfile_ << "\\hline" << std::endl;
    yieldfile_ << "Lepton & $\\frac{\\text{Data}_{CR_\\ell}}{\\text{Data}_{inc}}$ & ";
    yieldfile_ << 	"$\\frac{\\text{MC}_{inc}}{\\text{MC}_{\\ell - matched }} $ & ";
    yieldfile_ <<	"$ \\frac{\\text{MC}_{\\ell - fake}}{\\text{MC}_{\\ell - matched}}$	 & ";
    yieldfile_ <<	"LepCorr$_{\\ell}$ \\\\" << std::endl;
    yieldfile_ << "\\hline" << std::endl;

    for(int i = 0; i < 3; i++){
		dataTree->Draw(varDraw+">>dataOneLep",scaleName  + "*(" +   baseline + triggerSelection + recoLepCuts.at(i) +")");
		dataTree->Draw(varDraw+">>dataInc",scaleName  + "*("  + baseline + triggerSelection + ")");
		mcTree->Draw(varDraw+">>mcOneLep",scaleName + "*" + Lumi +"*(" + baseline + recoLepCuts.at(i) + genOneLep.at(i) + ")");
		mcTree->Draw(varDraw+">>mcNoLep",scaleName + "*" + Lumi + "*(" + baseline + recoLepCuts.at(i) + genNoLep.at(i) + ")");
		mcTree->Draw(varDraw+">>mcInc",scaleName + "*" + Lumi + "*("  + baseline + ")");

		mcCorrErrorHist->Divide(mcInc,mcOneLep,1.0,1.0,"B");
		mcCorrErrorHist2->Divide(mcNoLep,mcOneLep,1.0,1.0,"B");
		dataCorrErrorHist->Divide(dataOneLep,dataInc,1.0,1.0,"B");

		double mcCorrError;
		double mcCorr = mcCorrErrorHist->IntegralAndError(0,10001,mcCorrError);
		double dataCorrError;
		double dataCorr = dataCorrErrorHist->IntegralAndError(0,10001,dataCorrError);
		double mcFakeError;
		double fakeCorr = mcCorrErrorHist2->IntegralAndError(0,10001,mcFakeError);
		double lepCorr = (mcCorr * dataCorr - fakeCorr);
		std::cout << "THe value of mcCorr is " << mcCorr <<std::endl;
		std::cout << "THe value of mcCorrError is " << mcCorrError <<std::endl;

		std::cout << "THe value of dataCorr is " << dataCorr<<std::endl;
		std::cout << "THe value of dataCorrError is " << dataCorrError<<std::endl;

		std::cout << "THe value of fakeCorr is " << fakeCorr<<std::endl;
		std::cout << "THe value of mcFakeError is " << mcFakeError<<std::endl;

		std::cout << "The value of lepcorr is " << lepCorr <<std::endl;

		std::vector<double>tempErrors;    std::vector<double>tempVals;

		tempErrors.push_back(mcCorrError);    tempErrors.push_back(dataCorrError);
		tempVals.push_back(mcCorr);	   tempVals.push_back(dataCorr);
		double errors2 = multiplyUnc(tempErrors,tempVals,(dataCorr*mcCorr));
		std::cout << "The correlated value of errors2 is " << errors2 << std::endl;
		tempErrors.clear();tempVals.clear();

		tempErrors.push_back(mcFakeError);tempErrors.push_back(errors2);
		double lepCorrError = addUnc(tempErrors);
		tempErrors.clear();tempVals.clear();

		std::cout << "The value of the net error is " << lepCorrError << std::endl;
		if(i == 0)
	    yieldfile_ << "electron &";
		if(i == 1)
	    yieldfile_ << "muon &";
		if(i == 2)
	    yieldfile_ << "tau &";
		yieldfile_ << std::fixed;
		yieldfile_.precision(3);
	    yieldfile_ << dataCorr << " $\\pm$ " << dataCorrError << " & "  << mcCorr << " $\\pm$ " << mcCorrError << " & ";
	    yieldfile_ << fakeCorr << " $\\pm$ " << mcFakeError << " & " <<  lepCorr << " $\\pm$ " << lepCorrError << " \\\\ " << std::endl;
	    yieldfile_ << "\\hline"  << std::endl;

    }
    yieldfile_ << "\\end{tabular}"  << std::endl;
    yieldfile_ << "\\end{center}" << std::endl;

}

double addUnc(std::vector<double> errors){
	double errorSum = 0;
	for (int i = 0; i < errors.size(); i++)
	{
		errorSum += pow(errors.at(i),2);
	}
return (sqrt(errorSum));
}
double multiplyUnc(std::vector<double> errors,std::vector<double> vals, double multiplied){
	double errorSum = 0;
	for (int i = 0; i < errors.size(); i++)
	{
		errorSum += pow(errors.at(i),2)/(pow(vals.at(i),2));
		//multiply = multiply * vals.at(i);
	}
	return (multiplied*sqrt(errorSum));
}
