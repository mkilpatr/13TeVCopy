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

void createLepCorrectionHists(TString varDraw = "met",bool normalize = true);
double addUnc(std::vector<double> errors);
double multiplyUnc(std::vector<double> errors,std::vector<double> vals, double multiply);
void fillSearchVectors(std::vector<TString>& c_SBins_0l, std::vector<TString>& c_SBins_1l,TString c_presel_0l,TString c_presel_1l);

void createLepCorrectionHists(TString varDraw,bool normalize){
	TString  fileTree = "events";
	TString scaleName = "weight";
	TString dataString = "run/lepEffSlimmed/data_tree.root";
	TString mcString = "run/lepEffSlimmed/sm_tree.root";
	////STRING FOR FILE SAVE NAME
	TString LumiString = "40pb";
	TString Lumi = ".0402";

	//EVENT SELECTIONS
	TString triggerSelection             = "&&(passDiJetmet > 0)";
	TString baseline                     = "(nbjets >=1)&&(njets60 >= 2) && (dphij12met>.4) && (dphij3met>0.4) && (met>200)";
	TString searchString 				 =  baseline + "&&(mtcsv12MET > 175)&& (dphij12met>1) && (dphij3met>0.5)";

	////GEN LEVEL CUT STRINGS
	TString noPromptEle                  = "&&(nGoodEle == 0)";
	TString onePromptEle                 = "&&(nGoodEle >= 1)";
	TString noPromptMu                   = "&&(nGoodMu == 0)";
	TString onePromptMu                  = "&&(nGoodMu >= 1)";
	TString noPromptTau                  = "&&(nPromptTau == 0)";
	TString onePromptTau                 = "&&(nPromptTau >= 1)";

	////ORTHOGONALIZE AT RECO LEVEL
	TString recoEleCut 					 = "&&(nSelectedElectrons >= 1)&&(nSelectedMuons    == 0)";
	TString recoMuCut 					 = "&&(nSelectedMuons    >= 1)";
	TString recoTauCut 		     		 = "&&(nvetotau    >= 1)&&(nSelectedElectrons == 0)&&(nSelectedMuons    == 0)";

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
	TH1F* LEP 	 = new TH1F("LEP","LEP",64,0.0,64.0);LEP->Sumw2();
	TH1F* eleCorr 	 = new TH1F("eleCorr","eleCorr",21,0.0,21.0);eleCorr->Sumw2();
	TH1F* muCorr 	 = new TH1F("muCorr","muCorr",21,0.0,21.0);muCorr->Sumw2();
	TH1F* tauCorr 	 = new TH1F("tauCorr","tauCorr",21,0.0,21.0);tauCorr->Sumw2();

	TH1F* dataInc;TH1F* dataOneLep;TH1F* mcOneLep;TH1F* mcInvEffHist;TH1F *dataEffHist;TH1F* binnedCRYields;TH1F* binnedSRYields;
	dataInc 	 = new TH1F("dataInc","dataInc",1,0.0,1.0);dataInc->Sumw2();
	dataOneLep = (TH1F*) dataInc->Clone("dataOneLep");TH1F* mcNoLep	 = (TH1F*) dataInc->Clone("mcNoLep");
	mcOneLep   =  (TH1F*) dataInc->Clone("mcOneLep");TH1F* mcInc       = (TH1F*) dataInc->Clone("mcInc");
    mcInvEffHist = (TH1F*) dataInc->Clone("mcInvEffHist");TH1F *mcFakeHist = (TH1F*) dataInc->Clone("mcFakeHist");
    dataEffHist = (TH1F*) dataInc->Clone("dataEffHist");
    binnedCRYields = (TH1F*) dataInc->Clone("binnedCRYields");
    binnedSRYields = (TH1F*) dataInc->Clone("binnedSRYields");
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

		for(int j = 0; j < (c_SBins_1lCR.size()); j++){

			////FILL THE NECESSARY HISTOGRAMS
			dataTree->Draw(varDraw+">>dataOneLep",scaleName  + "*(" +   baseline + triggerSelection + recoLepCuts.at(i) +")");
			dataTree->Draw(varDraw+">>dataInc",scaleName  + "*("  + baseline + triggerSelection + ")");
			mcTree->Draw(varDraw+">>mcOneLep",scaleName + "*" + Lumi +"*(" + baseline + recoLepCuts.at(i) + genOneLep.at(i) + ")");
			mcTree->Draw(varDraw+">>mcNoLep",scaleName + "*" + Lumi + "*(" + baseline + recoLepCuts.at(i) + genNoLep.at(i) + ")");
			mcTree->Draw(varDraw+">>mcInc",scaleName + "*" + Lumi + "*("  + baseline + ")");
			mcTree->Draw(varDraw+">>binnedCRYields",scaleName  + "*("   + c_SBins_1lCR.at(j) + recoLepCuts.at(i) + ")");
			mcTree->Draw(varDraw+">>binnedSRYields",scaleName  + "*("   + c_SBins_1lSR.at(j) + recoLepCuts.at(i) +")");

			////COUT THE CORRECTION INPUT AND RESULTS
			if(j == 0){
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
			}
			double CRYieldsError;
			double SRYieldsError;
			double CRYields = binnedCRYields->IntegralAndError(0,10001,CRYieldsError);
			double SRYields = binnedSRYields->IntegralAndError(0,10001,SRYieldsError);

			std::cout << "The integral of binnedCRYields is " << CRYields << " At i = " << i << " And j = " << j<< std::endl;
			std::cout << "The error of binnedCRYields is " << CRYieldsError << " At i = " << i << " And j = " << j<< std::endl;

			std::cout << "The integral of binnedSRYields is " <<  SRYields << " At i = " << i << " And j = " << j << std::endl;
			std::cout << "The error of binnedSRYields is " <<  SRYieldsError << " At i = " << i << " And j = " << j << std::endl;


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
				TH1F* lepLEP;
				lepLEP = mcInvEffHist;
				lepLEP->Multiply(dataEffHist);
				mcInvEffHist->Add(mcFakeHist,-1);
				double lepCorrError2;
				double lepCorr2 = lepLEP->IntegralAndError(0,10001,lepCorrError2);
				lepLEP->Multiply(binnedCRYields);
				double corrError;
				double corrYields = lepLEP->IntegralAndError(0,10001,corrError);

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
			if(j == 0){
					std::cout << "Printing out initial correction results" << std::endl;
					std::cout << "The value of the term mcCorr is " << mcCorr << " At " << i << std::endl;
					std::cout << "The value of the term dataCorr is " << dataCorr << " At " << i << std::endl;
					std::cout << "The value of the term fakeCorr is " << fakeCorr << " At " << i << std::endl;
					std::cout << "The value of the total lepCorr is " << lepCorr << " At " << i << std::endl;
					std::cout << "The value of the total error of lepCorr is " << lepCorrError << " At " << i << std::endl;

					std::cout << "The value of the total lepCorr2 is " << lepCorr2 << " At " << i << std::endl;
					std::cout << "The value of lepCorrError2 is " << lepCorrError2 << " At " << i << std::endl;
					std::cout << "The value of the corrected yields is " << corrYields << " At " << i << std::endl;
					std::cout << "The value of the corrected yields error is " << corrError << " At " << i << std::endl;

				}
			/////FILL THE HISTOGRAMS
			if(j == 0){
				LEP->SetBinContent(i+1,lepCorr2);
				LEP->SetBinError(i+1,lepCorrError2);}

			if(i == 0 and j!=20){
				eleCorr->SetBinContent(j+1,CRYields);
				eleCorr->SetBinError(j+1,CRYieldsError);
			}
			else if (i==0){
				eleCorr->SetBinContent(j+1,lepCorr2);
				eleCorr->SetBinError(j+1,lepCorrError2);
			}
			if(i == 1 and j!=20){
				muCorr->SetBinContent(j+1,CRYields);
				muCorr->SetBinError(j+1,CRYieldsError);
			}
			else if (i==1){
				muCorr->SetBinContent(j+1,lepCorr);
				muCorr->SetBinError(j+1,lepCorrError2);
			}
			if(i == 2 and j!=20){
				tauCorr->SetBinContent(j+1,corrYields);
				tauCorr->SetBinError(j+1,CRYieldsError);
			}
			else if (i==2){
				tauCorr->SetBinContent(j+1,lepCorr);
				tauCorr->SetBinError(j+1,lepCorrError2);
			}
			if(j == 0){
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
			    yieldfile_ << fakeCorr << " $\\pm$ " << mcFakeError << " & " <<  lepCorr << " $\\pm$ " << lepCorrError << " \\\\ " << std::endl;
			    yieldfile_ << "\\hline"  << std::endl;
			}
		}
    }
    yieldfile_ << "\\end{tabular}"  << std::endl;
    yieldfile_ << "\\end{center}" << std::endl;
	TFile* file = new TFile("lepCorr_" + LumiString + ".root","RECREATE");
	eleCorr->Write();
	muCorr->Write();
	tauCorr->Write();
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
