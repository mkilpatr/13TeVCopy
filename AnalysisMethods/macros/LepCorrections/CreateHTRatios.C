#include <fstream>
#include <iostream>
#include "TF1.h"
#include "TTree.h"
#include "TLine.h"
#include "TLegend.h"
#include "TString.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TRandom.h"
#include "THistPainter.h"
#include "TCanvas.h"
#include <string.h>
#include <math.h>
#include "TLatex.h"
#include "TMultiGraph.h"
#include <vector>
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h"
#include "AnalysisMethods/PlotUtils/interface/EffPlotTools.hh"
#include "AnalysisMethods/macros/LepCorrections/binnedYields.C"

void fillSearchVectors(std::vector<TString>& c_SBins_0l, std::vector<TString>& c_SBins_1l,TString c_presel_0l,TString c_presel_1l);
TH1F* loadHist(TString varDraw, TString cutString, TString fileName,int nBins, double histBegin, double histFinal, int color = 0);
TH1F* loadHist(TString varDraw, TString cutString, TString fileName,int nBins, Double_t* bins, int color = 0);

void tWriterRatioComp(TString  outputDir = "",TString histTitle = "", TString outputPrefix = "",TString  file1 = "",TString  varDraw1 = "",TString varDraw2 = "",int nBins = 30,
double histBegin = 0, double histFinal = 300, TString cutString1 = "", TString cutString2 = "", TString file1LegendName = "Reco Distribution",TString file2LegendName = "Gen Distribution",bool norm = true,bool ratio = true);

void tWriterRatioComp(TString  outputDir = "",TString histTitle = "", TString outputPrefix = "",TString  file1 = "",TString  varDraw1 = "",TString varDraw2 = "",int nBins = 30,
Double_t histValues[] = {}, TString cutString1 = "", TString cutString2 = "", TString file1LegendName = "Reco Distribution",TString file2LegendName = "Gen Distribution",bool norm = true,bool ratio = true);

TFile *f  = new TFile("run/lepEffSlimmed/sm_tree.root");
TString  tTree = "events";
TTree *t = (TTree*)f->Get(tTree);
TString  scaleName1 = "weight";
TString  Lumi = "4";
TLegend *legend;
TCanvas *c1;
TPad *p1;
bool logScale = true;
TString fileIn = "run/lepEffSlimmed/sm_tree.root";
TString filePrefix = "run/lepEffSlimmed/";

////ASSUME THAT TREE HAS BEEN FILLED WITH BASIC MET/JET CUTS, HERE ARE THE NECESSARY REGIONS:
TString baseline 				= "(met >= 200)&&(nbjets >= 1)&&(njets >= 5)&&(njets60 >= 2) && (dphij12met>.4) && (dphij3met>0.4) ";
TString searchString 			= baseline 				 + "&&(mtcsv12MET > 175)&& (dphij12met>1) && (dphij3met>0.5)";

TString zeroLCR 				= baseline				 + "&&(nSelectedElectrons == 0) &&(nSelectedMuons == 0)&&(nvetotau == 0)";
TString zeroLSR 				= zeroLCR 		 + "&&(mtcsv12MET > 175) && (dphij12met>1) && (dphij3met>0.5)";
TString oneLCRstring		 			= baseline 				 + "&&( (nSelectedElectrons >= 1)||(nSelectedMuons >= 1)||(nvetotau >= 1) )";
TString oneLSRstring				= oneLCRstring  		 		 + "&&(mtcsv12MET > 175) && (dphij12met>1) && (dphij3met>0.5)";

TString muonCR     				= baseline		 		 + "&&(nSelectedMuons >= 1)&&(nSelectedElectrons == 0)";
TString eleCR 					= baseline		 		 + "&&(nSelectedElectrons >= 1)";
TString tauCR     				= baseline 				 + "&&(nSelectedElectrons == 0) &&(nSelectedMuons == 0) ";

TString muonSR     				= oneLSRstring 	 			 + "&&(nSelectedMuons >= 1)&&(nSelectedElectrons == 0)";
TString eleSR 					= oneLSRstring	 			 + "&&(nSelectedElectrons >= 1)";
TString tauSR      				= oneLSRstring 		 		 + "&&(nSelectedElectrons == 0) &&(nSelectedMuons == 0) ";

TString genRegionMuonFinal  	= baseline 				 + "&&(nGoodMu >= 1)";
TString genRegionElectronFinal  = baseline  			 + "&&(nGoodEle >= 1)&&(nSelectedMuons == 0)";
TString genRegionTauFinal  		= baseline  		     + "&&(nPromptTau >= 1)&&(nSelectedMuons == 0)&&(nSelectedElectrons == 0)";

TString recoRegionMuonFinal 	= genRegionMuonFinal 	 + "&&(nSelectedMuons >= 1)";
TString recoRegionElectronFinal = genRegionElectronFinal + "&&(nSelectedElectrons >= 1)";
TString recoRegionTauFinal 		= genRegionTauFinal 	 + "&&(nvetotau >= 1)";

TString fakeDenomMuonFinal 		= baseline				 + "&&(nSelectedMuons == 1)&&(nGoodMu >= 0)";
TString fakeDenomElectronFinal  = baseline			  	 + "&&(nSelectedElectrons == 1)&&(nSelectedMuons == 0)&&(nGoodEle >= 0)";
TString fakeDenomTauFinal 		= baseline			 	 + "&&(nvetotau == 1)&&(nSelectedElectrons == 0)&&(nSelectedMuons == 0)&&(nPromptTau >= 0)";

TString fakeNumMuonFinal 		= fakeDenomMuonFinal	 + "&&(nGoodMu == 0)";
TString fakeNumElectronFinal  	= fakeDenomElectronFinal + "&&(nGoodEle == 0)";
TString fakeNumTauFinal 		= fakeDenomTauFinal		 + "&&(nPromptTau == 0)";
Double_t ptBins[] = {5,10,15,20,25,30,40,60,90,120};


void CreateHTRatios(){


    std::vector<TString> c_SBins_1lCR;
    std::vector<TString> c_SBins_1lSR;
    fillSearchVectors(c_SBins_1lCR,c_SBins_1lSR,oneLCRstring,oneLSRstring);
    TH1F* oneLCR; TH1F* oneLSR;
	TH1F* oneLCRMeanPlotTauPt = new TH1F("oneLCRMeanPlotTauPt","oneLCRMeanPlotTauPt",20,0.0,20.0);
	oneLCRMeanPlotTauPt->Sumw2();
	oneLCRMeanPlotTauPt->SetStats(false);oneLCRMeanPlotTauPt->SetLineWidth(3);oneLCRMeanPlotTauPt->SetLineColor(kRed);
	TH1F* oneLSRMeanPlotTauPt = new TH1F("oneLSRMeanPlotTauPt","oneLSRMeanPlotTauPt",20,0.0,20.0);
	oneLSRMeanPlotTauPt->Sumw2();
	oneLSRMeanPlotTauPt->SetStats(false);oneLSRMeanPlotTauPt->SetLineWidth(3);oneLSRMeanPlotTauPt->SetLineColor(kBlue);

	TH1F* oneLCRMeanPlotTauHt = (TH1F*)oneLCRMeanPlotTauPt->Clone("oneLCRMeanPlotTauHt");
	TH1F* oneLSRMeanPlotTauHt = (TH1F*)oneLSRMeanPlotTauPt->Clone("oneLSRMeanPlotTauHt");


	Double_t searchPtBins[] = {0,10,20,30,40};


for(int i = 0; i < c_SBins_1lCR.size(); i++ ){
    	stringstream ss;
    	ss << i;
    	string str = ss.str();
    	//if (i >  2) continue;
    	//Write inclusive comparisons
    	if(i == 0){
    	  	tWriterRatioComp("run/lepEffSlimmed/output","GEN Tau p_{T}","Tau_CLvsSR_Pt","run/lepEffSlimmed/sm_tree.root","hardestGenTauDPt","hardestGenTauDPt",4,searchPtBins,recoRegionTauFinal+  "&&(hardestGenTauDPt > 0)",recoRegionTauFinal + "&&" +c_SBins_1lSR.at(i) +"&&(hardestGenTauDPt > 0)","#tau-inc.","#tau-CR");
    	  	tWriterRatioComp("run/lepEffSlimmed/output","GEN Tau H_{T}-Along" ,"Tau_CLvsSR_HTAlong","run/lepEffSlimmed/sm_tree.root","htmlAlongHardestTauD","htmlAlongHardestTauD",6,0.01,600  ,recoRegionTauFinal+ "&&(htmlAlongHardestTauD>0)",recoRegionTauFinal+"&&" +c_SBins_1lSR.at(i) +  "&&(htmlAlongHardestTauD>0)","#tau-inc.","#tau-CR");
    	}

  	tWriterRatioComp("run/lepEffSlimmed/output","GEN Tau p_{T} bin_" + str,"Tau_CLvsSR_Pt_bin"+ str,"run/lepEffSlimmed/sm_tree.root","hardestGenTauDPt","hardestGenTauDPt",4,searchPtBins,recoRegionTauFinal + "&&(hardestGenTauDPt > 0)",recoRegionTauFinal+ "&&" +c_SBins_1lSR.at(i)  +"&&(hardestGenTauDPt > 0)","track-inc.","track-CR");
  	tWriterRatioComp("run/lepEffSlimmed/output","GEN Tau HTAlong bin_" + str ,"Tau_CLvsSR_HTAlong_bin"+ str,"run/lepEffSlimmed/sm_tree.root","htmlAlongHardestTauD","htmlAlongHardestTauD",6,0.01,600  ,recoRegionTauFinal + "&&(htmlAlongHardestTauD > 0)",recoRegionTauFinal +"&&" +c_SBins_1lSR.at(i)  +"&&(htmlAlongHardestTauD > 0)","track-inc.","track-CR");

	///Compute all pt
	oneLCR = loadHist("hardestGenTauDPt", recoRegionTauFinal+"&&(hardestGenTauDPt > 0)" , fileIn,20, 5,300);
	oneLSR = loadHist("hardestGenTauDPt",  c_SBins_1lSR.at(i) + "&&" + recoRegionTauFinal +"&&(hardestGenTauDPt > 0)" , fileIn,100,5,300);
	double oneLCRMean = oneLCR->GetMean();
	double oneLSRMean = oneLSR->GetMean();
	oneLCRMeanPlotTauPt->SetBinContent((i+1),oneLCRMean);

	double tempError;
	double tempYields;tempYields = oneLSR->IntegralAndError(0,10001,tempError);
	double oneLSRError = oneLSRMean * tempError/tempYields;
	oneLSRMeanPlotTauPt->SetBinError((i+1),oneLSRError);
	oneLSRMeanPlotTauPt->SetBinContent((i+1),oneLSRMean);



	oneLCR = loadHist("htmlAlongHardestTauD", recoRegionTauFinal+"&&(htmlAlongHardestTauD > 0)" , fileIn,13, 12,0,600);
	oneLSR = loadHist("htmlAlongHardestTauD", c_SBins_1lSR.at(i) + "&&" + recoRegionTauFinal+"&&(htmlAlongHardestTauD > 0)",fileIn,13,12,0,600);
	oneLCRMean = oneLCR->GetMean();
	oneLSRMean = oneLSR->GetMean();


	oneLCRMeanPlotTauHt->SetBinContent((i+1),oneLCRMean);
	tempYields = oneLSR->IntegralAndError(0,10001,tempError);
	oneLSRError = oneLSRMean * tempError/tempYields;
	oneLSRMeanPlotTauHt->SetBinError((i+1),oneLSRError);
	oneLSRMeanPlotTauHt->SetBinContent((i+1),oneLSRMean);

	}

//Build Canvas
c1 = new TCanvas("c1","example",700,600);
c1->cd();
//Build Pad
p1= new TPad("p1","p1",0,.1,1,1);p1->SetLeftMargin  (0.15);p1->SetTopMargin   (0.10);p1->SetRightMargin (0.07);p1->SetBottomMargin(0.03);p1->Draw();p1->cd();


//Draw Ht Avg Vals.
    oneLCRMeanPlotTauHt->SetTitle("Avg Tau H_{T};  Search Bin; H_{T} value;");
	//fakeRecoPttau->GetYaxis()->SetRangeUser(0,.75);
    oneLCRMeanPlotTauHt->Draw("histE");
    oneLSRMeanPlotTauHt->Draw("sameE");oneLSRMeanPlotTauHt->Draw("samehist");
	legend=new TLegend(0.75,0.85,1,1,"");legend->SetTextFont(72);legend->SetTextSize(0.04);
	legend->AddEntry(oneLCRMeanPlotTauHt,"Inc. Avg.","l");legend->AddEntry(oneLSRMeanPlotTauHt,"1L Inc.","l");
	legend->Draw();c1->cd();

	//Save and reset pad
	c1->SaveAs("run/lepEffSlimmed/output/binnedtauht_mean_test.png");	c1->Clear();

	//Build Canvas
	TCanvas *c1 = new TCanvas("c1","example",700,600);
	c1->cd();
	//Build Pad
	p1= new TPad("p1","p1",0,.1,1,1);p1->SetLeftMargin  (0.15);p1->SetTopMargin   (0.10);p1->SetRightMargin (0.07);p1->SetBottomMargin(0.03);p1->Draw();p1->cd();

	//Draw Ht Avg Vals.
		oneLCRMeanPlotTauPt->SetTitle("Avg Tau p_{T} };  Search Bin; p_{T} value;");
		//fakeRecoPttau->GetYaxis()->SetRangeUser(0,.75);
		oneLCRMeanPlotTauPt->Draw("histE");
	    oneLSRMeanPlotTauPt->Draw("sameE");oneLSRMeanPlotTauPt->Draw("samehist");
		legend=new TLegend(0.75,0.85,1,1,"");legend->SetTextFont(72);legend->SetTextSize(0.04);
		legend->AddEntry(oneLCRMeanPlotTauPt,"Inc. Avg.","l");legend->AddEntry(oneLSRMeanPlotTauPt,"1L Inc.","l");
		legend->Draw();c1->cd();

		//Save and reset pad
		c1->SaveAs("run/lepEffSlimmed/output/binnedtaupt_mean_test.png");	c1->Clear();


		//Construct Composite Pt graph and add legend
		TH1F* recoPttau      = loadHist("hardestGenTauDPt",recoRegionMuonFinal,fileIn,9, ptBins,0);
		TH1F* genPttau       = loadHist("hardestGenTauDPt", genRegionMuonFinal, fileIn,9, ptBins);

		recoPttau->SetTitle("Tau Efficiencies vs p_{T};  Tau p_{T}; Tau Efficiency;");
		recoPttau->Divide(genPttau);
		recoPttau->Draw("histE");
		legend=new TLegend(0.75,0.85,1,1,"");legend->SetTextFont(72);legend->SetTextSize(0.04);
		legend->Draw();	c1->cd();

		//Save and reset the padd
		c1->SaveAs("run/lepEffSlimmed/output/tau_pt_test_eff.png");c1->Clear();
		p1= new TPad("p1","p1",0,.1,1,1);p1->SetLeftMargin  (0.15);p1->SetTopMargin   (0.10);p1->SetRightMargin (0.07);p1->SetBottomMargin(0.03);p1->Draw();p1->cd();



}

TH1F* loadHist(TString varDraw, TString cutString, TString fileName,int nBins, double histBegin, double histFinal, int color)
{
	TString tree = "events";
	//Setup
	TFile *f  = new TFile(fileName);
	TTree *t = (TTree*)f->Get(tree);
	TH1F* h1 = new TH1F("h1","h1",nBins,histBegin,histFinal);
	t->Draw(varDraw+">>h1",scaleName1 + "*" + Lumi+ "*" + "(" + cutString + ")");
	if (color == 0)
	h1->SetLineColor(kRed);
	if (color == 1)
	h1->SetLineColor(kBlue);
	if (color == 2)
	h1->SetLineColor(kGreen);

	h1->SetLineWidth(3);h1->SetMarkerSize(.5);h1->GetYaxis()->SetTitleOffset(1.25);
	h1->SetStats(false);

	return h1;
}


void tWriterRatioComp(TString  outputDir, TString histTitle,TString outputPrefix,TString  file1,TString  varDraw1,TString  varDraw2,int nBins,
double histBegin, double histEnd, TString cutString1, TString cutString2,TString file1LegendName, TString file2LegendName,bool norm, bool ratio){


	TString outfileName;
	if(norm)
		outfileName = outputDir + "/" + outputPrefix + "_ratio_norm.png";
	else
		outfileName = outputDir + "/" + outputPrefix + "_ratio.png";
//Setup
	TFile *f1  = new TFile(file1);
    TFile *f2  = new TFile(file1);
    TTree *t1 = (TTree*)f1->Get(tTree);
    TTree *t2 = (TTree*)f2->Get(tTree);


	TH1F* file1Plot = new TH1F("file1Plot","file1Plot",nBins,histBegin,histEnd);
	file1Plot->SetLineWidth(3);
	file1Plot->SetMarkerSize(.5);
	file1Plot->SetMarkerStyle(10);
	file1Plot->SetStats(false);

	TH1F* file2Plot = (TH1F*)file1Plot->Clone("file2Plot");
	file1Plot->Sumw2();
	file2Plot->Sumw2();

	t1->Draw(varDraw1+">>file1Plot",scaleName1 + "*" + Lumi + "*" + cutString1);
	t2->Draw(varDraw2+">>file2Plot",scaleName1 + "*" + Lumi + "*" + cutString2);
	  double error =0.; double integral = file1Plot->IntegralAndError(nBins,nBins+1,error);
	  file1Plot->SetBinContent(nBins,integral);
	  file1Plot->SetBinError(nBins,error);

	  integral = file2Plot->IntegralAndError(nBins,nBins+1,error);
	 	  file2Plot->SetBinContent(nBins,integral);
	 	  file2Plot->SetBinError(nBins,error);
	if(norm){
		file1Plot->Scale(1/file1Plot->Integral());
		file2Plot->Scale(1/file2Plot->Integral());}


	file1Plot->SetTitle("CMS Simulation at #sqrt{s} = 13 TeV;NPV;Number of Events(Normalized to arb. units.);");
	file1Plot->GetXaxis()->SetTitle(histTitle);
	file1Plot->GetYaxis()->SetTitleOffset(1.25);
	file1Plot->SetLineColor(kRed-7);
	file2Plot->SetLineColor(kCyan+1);


	/////Formatting Canvas
	TCanvas *c1 = new TCanvas("c1","example",700,825);
	c1->cd();
	  TPad *p1 = new TPad("p1","p1",0,0.3,1,1);
	  p1->SetLeftMargin  (0.18);
	  p1->SetTopMargin   (0.10);
	  p1->SetRightMargin (0.07);
	  p1->SetBottomMargin(0.03);
	//if(logScale)
	//p1->SetLogy();

	p1->Draw();
	p1->cd();
	//gPad->SetLogy();
	//c1->SetLogy();
	double ymax = file1Plot->GetMaximum();
	if (ymax < file2Plot->GetMaximum()) ymax = file2Plot->GetMaximum();
	file1Plot->GetYaxis()->SetRangeUser(0,1.25*ymax);

	file1Plot->Draw("Ehist");
	file2Plot->Draw("Esame");
	file2Plot->Draw("histsame");

	gPad->Update();

	////Format Legend
	TLegend *legend=new TLegend(0.6,0.80,0.93,.90,"");
	legend->SetTextFont(72);
	legend->SetTextSize(0.04);
	legend->AddEntry(file1Plot,file1LegendName,"l");
	legend->AddEntry(file2Plot,file2LegendName,"l");

	legend->Draw();
	c1->cd();
	if(ratio){
	////Clone into divisor
	TH1F* divideHist = new TH1F("divideHist","divideHist",nBins,histBegin,histEnd);
	divideHist->Sumw2();
	divideHist = (TH1F*)file2Plot->Clone("divideHist"); divideHist->Divide(file1Plot);

	////Building Second pad
	  TPad *p2 = new TPad("p2","p2",0,0,1,0.3);
	  p2->SetLeftMargin  (0.18);
	  p2->SetTopMargin   (0.00);
	  p2->SetRightMargin (0.07);
	  p2->SetBottomMargin(0.30);
	/*	if(logScale)
		p2->SetLogy();
		if(logScale)
		{
		divideHist->SetMinimum(.01);
		}*/
	p2->SetGridy(1);
	p2->Draw();
	p2->cd();

	////Setup the ratioGram
	divideHist->SetLineColor(kBlack);
	divideHist->SetTitleSize  (0.12,"Y");
	divideHist->SetTitleSize  (0.12,"X");
	divideHist->SetLabelSize  (0.10,"X");
	divideHist->SetLabelSize  (0.08,"Y");
	divideHist->GetYaxis()->SetTitleFont(40);
	divideHist->GetYaxis()->CenterTitle(kTRUE);
	divideHist->GetXaxis()->SetTitleFont(40);
	divideHist->GetYaxis()->SetNdivisions(305);
	divideHist->GetYaxis()->SetTitleOffset(0.4);
	divideHist->SetTitle(";;Ratio");
	divideHist->GetXaxis()->SetTitle(histTitle);
	divideHist->GetYaxis()->SetRangeUser(.4,1.6);
	//// Draw the ratioGram
	divideHist->Draw("EP");}
	c1->SaveAs(outfileName);


}
void tWriterRatioComp(TString  outputDir, TString histTitle,TString outputPrefix,TString  file1,TString  varDraw1,TString  varDraw2,int nBins,
		Double_t histValues[], TString cutString1, TString cutString2,TString file1LegendName, TString file2LegendName,bool norm, bool ratio){


	TString outfileName;
	if(norm)
		outfileName = outputDir + "/" + outputPrefix + "_ratio_norm.png";
	else
		outfileName = outputDir + "/" + outputPrefix + "_ratio.png";
//Setup
	TFile *f1  = new TFile(file1);
    TFile *f2  = new TFile(file1);
    TTree *t1 = (TTree*)f1->Get(tTree);
    TTree *t2 = (TTree*)f2->Get(tTree);


	TH1F* file1Plot = new TH1F("file1Plot","file1Plot",nBins,histValues);
	file1Plot->SetLineWidth(3);
	file1Plot->SetMarkerSize(.5);
	file1Plot->SetMarkerStyle(10);
	file1Plot->SetStats(false);

	TH1F* file2Plot = (TH1F*)file1Plot->Clone("file2Plot");
	file1Plot->Sumw2();
	file2Plot->Sumw2();

	t1->Draw(varDraw1+">>file1Plot",scaleName1 + "*" + Lumi + "*" + cutString1);
	t2->Draw(varDraw2+">>file2Plot",scaleName1 + "*" + Lumi + "*" + cutString2);
	  double error =0.; double integral = file1Plot->IntegralAndError(nBins,nBins+1,error);
	  file1Plot->SetBinContent(nBins,integral);
	  file1Plot->SetBinError(nBins,error);

	  integral = file2Plot->IntegralAndError(nBins,nBins+1,error);
	 	  file2Plot->SetBinContent(nBins,integral);
	 	  file2Plot->SetBinError(nBins,error);
	if(norm){
		file1Plot->Scale(1/file1Plot->Integral());
		file2Plot->Scale(1/file2Plot->Integral());}


	file1Plot->SetTitle("CMS Simulation at #sqrt{s} = 13 TeV;NPV;Number of Events(Normalized to arb. units.);");
	file1Plot->GetXaxis()->SetTitle(histTitle);
	file1Plot->GetYaxis()->SetTitleOffset(1.25);
	file1Plot->SetLineColor(kRed-7);
	file2Plot->SetLineColor(kCyan+1);


	/////Formatting Canvas
	TCanvas *c1 = new TCanvas("c1","example",700,825);
	c1->cd();
	  TPad *p1 = new TPad("p1","p1",0,0.3,1,1);
	  p1->SetLeftMargin  (0.18);
	  p1->SetTopMargin   (0.10);
	  p1->SetRightMargin (0.07);
	  p1->SetBottomMargin(0.03);
	//if(logScale)
	//p1->SetLogy();

	p1->Draw();
	p1->cd();
	//gPad->SetLogy();
	//c1->SetLogy();
	double ymax = file1Plot->GetMaximum();
	if (ymax < file2Plot->GetMaximum()) ymax = file2Plot->GetMaximum();
	file1Plot->GetYaxis()->SetRangeUser(0,1.25*ymax);

	file1Plot->Draw("Ehist");
	file2Plot->Draw("Esame");
	file2Plot->Draw("histsame");

	gPad->Update();

	////Format Legend
	TLegend *legend=new TLegend(0.6,0.80,0.93,.90,"");
	legend->SetTextFont(72);
	legend->SetTextSize(0.04);
	legend->AddEntry(file1Plot,file1LegendName,"l");
	legend->AddEntry(file2Plot,file2LegendName,"l");

	legend->Draw();
	c1->cd();
	if(ratio){
	////Clone into divisor
	TH1F* divideHist = new TH1F("divideHist","divideHist",nBins,histValues);
	divideHist->Sumw2();
	divideHist = (TH1F*)file2Plot->Clone("divideHist"); divideHist->Divide(file1Plot);

	////Building Second pad
	  TPad *p2 = new TPad("p2","p2",0,0,1,0.3);
	  p2->SetLeftMargin  (0.18);
	  p2->SetTopMargin   (0.00);
	  p2->SetRightMargin (0.07);
	  p2->SetBottomMargin(0.30);
	/*	if(logScale)
		p2->SetLogy();
		if(logScale)
		{
		divideHist->SetMinimum(.01);
		}*/
	p2->SetGridy(1);
	p2->Draw();
	p2->cd();

	////Setup the ratioGram
	divideHist->SetLineColor(kBlack);
	divideHist->SetTitleSize  (0.12,"Y");
	divideHist->SetTitleSize  (0.12,"X");
	divideHist->SetLabelSize  (0.10,"X");
	divideHist->SetLabelSize  (0.08,"Y");
	divideHist->GetYaxis()->SetTitleFont(40);
	divideHist->GetYaxis()->CenterTitle(kTRUE);
	divideHist->GetXaxis()->SetTitleFont(40);
	divideHist->GetYaxis()->SetNdivisions(305);
	divideHist->GetYaxis()->SetTitleOffset(0.4);
	divideHist->SetTitle(";;Ratio");
	divideHist->GetXaxis()->SetTitle(histTitle);
	divideHist->GetYaxis()->SetRangeUser(0,1.25);
	//// Draw the ratioGram
	divideHist->Draw("EP");}
	c1->SaveAs(outfileName);


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

TH1F* loadHist(TString varDraw, TString cutString, TString fileName,int nBins, Double_t* bins, int color)
{
	TString tree = "events";
	//Setup
	TFile *f  = new TFile(fileName);
	TTree *t = (TTree*)f->Get(tree);
	TH1F* h1 = new TH1F("h1","h1",nBins,bins);
	t->Draw(varDraw+">>h1",scaleName1 + "*" + Lumi+ "*" + "(" + cutString + ")");
	if (color == 0)
	h1->SetLineColor(kRed);
	if (color == 1)
	h1->SetLineColor(kBlue);
	if (color == 2)
	h1->SetLineColor(kGreen);

	h1->SetLineWidth(3);h1->SetMarkerSize(.5);h1->GetYaxis()->SetTitleOffset(1.25);
	h1->SetStats(false);

	return h1;
}

