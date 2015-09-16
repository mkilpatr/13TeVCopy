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
//namespace  createLepCorrectionHists{

void createSlimLepCorrections(TString varDraw = "met",bool normalize = true);
double addUnc(std::vector<double> errors);
double multiplyUnc(std::vector<double> errors,std::vector<double> vals, double multiply);
void fillSearchVectors(std::vector<TString>& c_SBins_0l, std::vector<TString>& c_SBins_1l,TString c_presel_0l,TString c_presel_1l);

void createSlimLepCorrections(TString varDraw,bool normalize){
    TString  fileTree = "Events";
    TString scaleName = "weight";
    TString dataString = "run/lepEffSlimmed/data_tree.root";
    TString mcString = "run/lepEffSlimmed/sm_tree.root";
    ////STRING FOR FILE SAVE NAME
    TString LumiString = "40pb";
    TString Lumi = ".0402";

    //EVENT SELECTIONS
    TString triggerSelection             = "&&(passdijetmet > 0)";
    TString baseline                     = "(njets60 >= 2) && (dphij12met>.4) && (dphij3met>0.4) && (met>200) &&(njets >=5 ) && (nbjets >= 1)";
    TString searchString                             =  baseline + "&&(mtcsv12met > 175)&& (dphij12met>1) && (dphij3met>0.5)";

    ////GEN LEVEL CUT STRINGS
    TString noPromptEle                  = "&&(ngoodgenele == 0)";
    TString onePromptEle                 = "&&(ngoodgenele >= 1)";
    TString noPromptMu                   = "&&(ngoodgenmu == 0)";
    TString onePromptMu                  = "&&(ngoodgenmu >= 1)";
    TString noPromptTau                  = "&&(npromptgentau == 0)";
    TString onePromptTau                 = "&&(npromptgentau >= 1)";

    ////ORTHOGONALIZE AT RECO LEVEL
    TString recoEleCut                                       = "&&(nvetolele >= 1)&&(ngoodgenmu    == 0)";
    TString recoMuCut                                        = "&&(nvetomu    >= 1)";
    TString recoTauCut                               = "&&(nvetotau    >= 1)&&(nvetolele == 0)&&(nvetomu    == 0)";

	////FILL GEN AND RECO CUT STRINGS
	std::vector<TString> recoLepCuts; 	std::vector<TString> genNoLep; 	std::vector<TString> genOneLep;
	recoLepCuts.push_back(recoEleCut);	recoLepCuts.push_back(recoMuCut);	recoLepCuts.push_back(recoTauCut);
	genNoLep.push_back(noPromptEle);	genNoLep.push_back(noPromptMu);	genNoLep.push_back(noPromptTau);
	genOneLep.push_back(onePromptEle);	genOneLep.push_back(onePromptMu);	genOneLep.push_back(onePromptTau);

	////FILL THE BIN SELECTIONS
    std::vector<TString> c_SBins_1lCR;std::vector<TString> c_SBins_1lSR;
	fillSearchVectors(c_SBins_1lCR,c_SBins_1lSR,baseline,searchString);
	c_SBins_1lCR.push_back(baseline);
	c_SBins_1lSR.push_back(searchString);

	////LOAD ROOT FILES
	TFile *mcFile  = new TFile(mcString);
    TTree *mcTree = (TTree*)mcFile->Get(fileTree);

	TFile *dataFile  = new TFile(dataString);
    TTree *dataTree = (TTree*)dataFile->Get(fileTree);


    //CREATE CORRECTION HISTS
	TH1F* LEP 	 = new TH1F("LEP","LEP",4,0.0,4.0);LEP->Sumw2();

	TH1F* dataInc;TH1F* dataOneLep;TH1F* mcOneLep;TH1F* mcInvEffHist;TH1F *dataEffHist;
	dataInc 	 = new TH1F("dataInc","dataInc",1,0.0,1.0);dataInc->Sumw2();
	dataOneLep = (TH1F*) dataInc->Clone("dataOneLep");TH1F* mcNoLep	 = (TH1F*) dataInc->Clone("mcNoLep");
	mcOneLep   =  (TH1F*) dataInc->Clone("mcOneLep");TH1F* mcInc       = (TH1F*) dataInc->Clone("mcInc");
    mcInvEffHist = (TH1F*) dataInc->Clone("mcInvEffHist");TH1F *mcFakeHist = (TH1F*) dataInc->Clone("mcFakeHist");
    dataEffHist = (TH1F*) dataInc->Clone("dataEffHist");


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
			////FILL THE NECESSARY HISTOGRAMS
			dataTree->Draw(varDraw+">>dataOneLep",scaleName  + "*(" +   baseline + triggerSelection + recoLepCuts.at(i) +")");
			dataTree->Draw(varDraw+">>dataInc",scaleName  + "*("  + baseline + triggerSelection + ")");
			mcTree->Draw(varDraw+">>mcOneLep",scaleName + "*" + Lumi +"*(" + baseline + recoLepCuts.at(i) + genOneLep.at(i) + ")");
			mcTree->Draw(varDraw+">>mcNoLep",scaleName + "*" + Lumi + "*(" + baseline + recoLepCuts.at(i) + genNoLep.at(i) + ")");
			mcTree->Draw(varDraw+">>mcInc",scaleName + "*" + Lumi + "*("  + baseline + ")");

			////COUT THE CORRECTION INPUT AND RESULTS
			double dummyError;
			std::cout << "The index i = (0,1,2) corresponds to (ele,mu,tau)" << std::endl;
			std::cout << "Printing out initial instantiating results" << std::endl;
			std::cout << "The cutstring recoLepCuts reads " << recoLepCuts.at(i) << " At " << i << std::endl;
			std::cout << "The integral of dataOneLep is " << dataOneLep->IntegralAndError(0,10001,dummyError) << " At " << i << std::endl;
			std::cout << "The integral of dataInc is " << dataInc->IntegralAndError(0,10001,dummyError) << " At " << i << std::endl;

			std::cout << "The cutstring genOneLep reads " << genOneLep.at(i) << " At " << i << std::endl;
			std::cout << "The integral of mcOneLep is " << mcOneLep->IntegralAndError(0,10001,dummyError) << " At " << i << std::endl;

			std::cout << "The cutstring genOneLep reads " << genNoLep.at(i) << " At " << i << std::endl;
			std::cout << "The integral of mcNoLep is " << mcNoLep->IntegralAndError(0,10001,dummyError) << " At " << i << std::endl;
			std::cout << "The integral of mcInc is " << mcInc->IntegralAndError(0,10001,dummyError) << " At " << i << std::endl;

			////DIVIDE THE HISTOGRAMS
			mcInvEffHist->Divide(mcInc,mcOneLep,1.0,1.0,"B");
			mcFakeHist->Divide(mcNoLep,mcOneLep);
			dataEffHist->Divide(dataOneLep,dataInc,1.0,1.0,"B");


			////FILL THE THREE LEPTON CORRECTION TERMS
			double mcCorrError;
			double mcCorr = mcInvEffHist->IntegralAndError(0,10001,mcCorrError);
			double dataCorrError;
			double dataCorr = dataEffHist->IntegralAndError(0,10001,dataCorrError);
			double mcFakeError;
			double fakeCorr = mcFakeHist->IntegralAndError(0,10001,mcFakeError);

			////ALTERNATE CALCULATION
			////MULTIPLY AND COMBINE HISTS
			TH1F* LepCorrHist;
			LepCorrHist = mcInvEffHist;
			LepCorrHist->Multiply(dataEffHist);
			mcInvEffHist->Add(mcFakeHist,-1);
			double lepCorrError2;
			double lepCorr2 = LepCorrHist->IntegralAndError(0,10001,lepCorrError2);

			////CALCULATE THE LEPTON CORRECTION
			double lepCorr = (mcCorr * dataCorr - fakeCorr);

			////CALCULATE THE LEPTON CORRECTION ERROR
			std::vector<double>tempErrors;    std::vector<double>tempVals;

			tempErrors.push_back(mcCorrError);    tempErrors.push_back(dataCorrError);
			tempVals.push_back(mcCorr);	   tempVals.push_back(dataCorr);
			double errors2 = multiplyUnc(tempErrors,tempVals,(dataCorr*mcCorr));
			tempErrors.clear();tempVals.clear();

			tempErrors.push_back(mcFakeError);tempErrors.push_back(errors2);
			double lepCorrError = addUnc(tempErrors);
			tempErrors.clear();tempVals.clear();
			////COUT THE AND RESULTS
			std::cout << "Printing out initial correction results" << std::endl;
			std::cout << "The value of the term mcCorr is " << mcCorr << " At " << i << std::endl;
			std::cout << "The value of the term dataCorr is " << dataCorr << " At " << i << std::endl;
			std::cout << "The value of the term fakeCorr is " << fakeCorr << " At " << i << std::endl;
			std::cout << "The value of the total lepCorr is " << lepCorr << " At " << i << std::endl;
			std::cout << "The value of the total error of lepCorr is " << lepCorrError << " At " << i << std::endl;
			std::cout << "The value of the total lepCorr2 is " << lepCorr2 << " At " << i << std::endl;
			std::cout << "The value of lepCorrError2 is " << lepCorrError2 << " At " << i << std::endl;
			/////FILL THE HISTOGRAMS
			LEP->SetBinContent(i+1,lepCorr2);
			LEP->SetBinError(i+1,lepCorrError2);

			std::cout << "The value of the net error is " << lepCorr << std::endl;
			if(i == 0)
			yieldfile_ << "electron &";
			if(i == 1)
			yieldfile_ << "muon &";
			if(i == 2)
			yieldfile_ << "tau &";
			yieldfile_ << std::fixed;
			yieldfile_.precision(3);
			yieldfile_ << dataCorr << " $\\pm$ " << dataCorrError << " & "  << mcCorr << " $\\pm$ " << mcCorrError << " & ";
			yieldfile_ << fakeCorr << " $\\pm$ " << mcFakeError << " & " <<  lepCorr2 << " $\\pm$ " << lepCorrError2 << " \\\\ " << std::endl;
			yieldfile_ << "\\hline"  << std::endl;
    }
	LEP->SetBinContent(4,1);
	LEP->SetBinError(4,0);
    yieldfile_ << "\\end{tabular}"  << std::endl;
    yieldfile_ << "\\end{center}" << std::endl;
	TFile* file = new TFile("lepCorr_" + LumiString + ".root","RECREATE");
	LEP->Write();
	file->Write();
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

void fillSearchVectors(std::vector<TString>& c_SBins_0l, std::vector<TString>& c_SBins_1l,TString c_presel_0l,TString c_presel_1l)
{
			TString c_Nb1NCTT0_0l = c_presel_0l+" && (nbjets==1) && (ncttstd==0)";
	        TString c_Nb1NCTT1_0l = c_presel_0l+" && (nbjets==1) && (ncttstd>=1)";
	        TString c_Nb2NCTT0_0l = c_presel_0l+" && (nbjets>=2) && (ncttstd==0)";
	        TString c_Nb2NCTT1_0l = c_presel_0l+" && (nbjets>=2) && (ncttstd>=1)";

	        TString c_Nb1NCTT0_1l = c_presel_1l+" && (nbjets==1) && (ncttstd==0)";
	        TString c_Nb1NCTT1_1l = c_presel_1l+" && (nbjets==1) && (ncttstd>=1)";
	        TString c_Nb2NCTT0_1l = c_presel_1l+" && (nbjets>=2) && (ncttstd==0)";
	        TString c_Nb2NCTT1_1l = c_presel_1l+" && (nbjets>=2) && (ncttstd>=1)";

	        std::vector<TString> c_met;
	        c_met.push_back("(met>=200. && met<300.)" );
	        c_met.push_back("(met>=300. && met<400.)" );
	        c_met.push_back("(met>=400. && met<500.)" );
	        c_met.push_back("(met>=500. && met<600.)" );
	        c_met.push_back("(met>=600. && met<7000.)");

	        std::vector<TString> c_CTTstd;
	        c_CTTstd.push_back("(ncttstd==0)");
	        c_CTTstd.push_back("(ncttstd>=1)");

	        std::vector<TString> c_Nb;
	        c_Nb.push_back("(nbjets==1)");
	        c_Nb.push_back("(nbjets>=2)");


	        for (unsigned int i0=0; i0<c_Nb.size(); ++i0) {
	          for (unsigned int i1=0; i1<c_CTTstd.size(); ++i1) {
	            for (unsigned int i2=0; i2<c_met.size(); ++i2) {
	      	TString tmpStr_0l = "("+c_presel_0l+" && "+c_Nb[i0]+" && "+c_CTTstd[i1]+" && "+c_met[i2]+")";
	      	TString tmpStr_1l = "("+c_presel_1l+" && "+c_Nb[i0]+" && "+c_CTTstd[i1]+" && "+c_met[i2]+")";
	      	c_SBins_0l.push_back(tmpStr_0l);
	      	c_SBins_1l.push_back(tmpStr_1l);
	            } // end of looping over i2
	          } // end of looping over i1
	        } // end of looping over i0
	        // ====
}
