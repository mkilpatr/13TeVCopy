#include <fstream>
#include <iostream>
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
namespace  binnedYields{

void fillCutVectors(std::vector<TString>& c_SBins_histNum, std::vector<TString>& c_SBins_1l,TString cutString1,TString cutString2);
TH1F* makeBinnedYields(TString cutString1 = "",TString cutString2 = "",TString  inputFile = "run/0LLatestFWK/ttsum_tree.root", TString varDraw = "met",bool normalize = true);


TH1F* makeBinnedYields(TString cutString1,TString cutString2, TString inputFile,TString varDraw,bool normalize){
	TString  fileTree = "events";
	TString scaleName = "weight";
	TString Lumi = "4";

//Setup
	TFile *f1  = new TFile(inputFile);
    TTree *t1 = (TTree*)f1->Get(fileTree);

//Fill cut strings
    std::vector<TString> c_SBins_histNum; std::vector<TString>  c_SDivBins_histDiv;
    fillCutVectors(c_SBins_histNum,c_SDivBins_histDiv,cutString1,cutString2);
    //c_SBins_histNum.push_back( "(" + cutString1 + ")");
    //c_SDivBins_histDiv.push_back("(" + cutString2 + ")");
	TH1F* signalBinPlot = new TH1F("signalBinPlot","signalBinPlot",20,0.0,20.0);
	signalBinPlot->Sumw2();

	TH1F* h1 = new TH1F("h1","h1",5,0.0,10000.0);
	TH1F* h2 = new TH1F("h2","h2",5,0.0,10000.0);
    h1->Sumw2();
    h2->Sumw2();

///Fill the signal bin plot
	for(unsigned int i =0; i<c_SBins_histNum.size(); i++)
	{

		t1->Draw(varDraw+">>h1",scaleName + "*" + Lumi + "*" + c_SBins_histNum.at(i));
		t1->Draw(varDraw+">>h2",scaleName + "*" + Lumi + "*" + c_SDivBins_histDiv.at(i));

		double binError;
		double binFill;

		if(normalize)
		h1->Divide(h1, h2, 1.0, 1.0, "B");

		binFill = h1->IntegralAndError(0,10001,binError);
		signalBinPlot->Fill(i,binFill);
		signalBinPlot->SetBinError(i,binError);

		/*if(i == 0 or i == 20){
		std::cout << "The cutstring at i = " << i << "is " << c_SBins_histNum.at(i) << std::endl;;
		std::cout << "the bin fill is " << binFill << std::endl;
		std::cout << "the binError is " << binError << std::endl;
		}*/


	}

/////////////////////
///Make the plot/////
/////////////////////

	/////Formatting histograms
	signalBinPlot->SetStats(false);
	signalBinPlot->SetLineWidth(3);
	signalBinPlot->SetLineColor(kRed);
	signalBinPlot->SetMarkerSize(.5);
	signalBinPlot->SetMarkerStyle(10);
	signalBinPlot->SetMarkerColor(kRed);

	signalBinPlot->SetTitle("test;testX;testY");

	return signalBinPlot;
}


void fillCutVectors(std::vector<TString>& c_SBins_histNum, std::vector<TString>& c_SBins_1l,TString cutString1,TString cutString2)
{
	  	  	TString c_presel_0l =   cutString1;  //+ "&&(njets>=5) && (nbjets>=1) && (dphij12met>1) && (dphij3met>0.5) && (met>200) && (mtcsv12met>175)";
	        TString c_presel_1l =  cutString2; //+ "&&(njets>=5) && (nbjets>=1) && (dphij12met>1) && (dphij3met>0.5) && (met>200) && (mtcsv12met>175)";

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
	      	c_SBins_histNum.push_back(tmpStr_0l);
	      	c_SBins_1l.push_back(tmpStr_1l);
	            } // end of looping over i2
	          } // end of looping over i1
	        } // end of looping over i0
	        // ====

}

}///End Namespace
