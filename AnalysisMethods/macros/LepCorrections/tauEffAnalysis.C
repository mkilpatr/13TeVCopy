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
#include "THStack.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TRandom.h"
#include "THistPainter.h"
#include "TCanvas.h"
#include <string.h>
#include <math.h>
#include <vector>

void tauEffAnalysis(TString varDraw = "met",bool normalize = true);
double addUnc(std::vector<double> errors);
double multiplyUnc(std::vector<double> errors,std::vector<double> vals, double multiply);
TH1F* mc0GEN1RECO;TH1F* mc1GEN1RECO;TH1F* mc1RECO;TH1F* mc1GEN;TH1F* mcHadronic;TH1F* mcContamHist;TH1F* mcFakeHist;TH1F* mcEffHist;TH1F* mc0GEN1RECOFake;
void drawVetoHists(std::vector<TString> fileStrings, TString searchString, bool norm = true);

void tauEffAnalysis(TString varDraw,bool normalize){
		TString inputPath = "run/lepEffSlimmed/";

		TString fileTree  = "events";
		TString scaleName = "weight";
		TString Lumi   = ".0402";
		TString baseline                        = "(njets60 >= 2) && (dphij12met>.4) && (dphij3met>0.4)";
		TString searchString 	    			= baseline + "&&(mtcsv12MET > 175)&& (dphij12met>1) && (dphij3met>0.5)";
		TString tauRegion 						= "(nSelectedElectrons == 0)&&(nSelectedMuons    == 0)";

		std::vector<TString> genNoLep;
		TString noPromptTau                     = "&&(nPromptTau == 0)";
		TString noGoodTau                       = "&&(nPromptTauAcc == 0)";
		TString noOneProngTau                   = "&&(nPromptTau1Prong == 0)";
		TString noThreeProngTau                	= "&&(nPromptTau3Prong == 0)";
		genNoLep.push_back(noPromptTau);	genNoLep.push_back(noGoodTau);
		genNoLep.push_back(noOneProngTau);	genNoLep.push_back(noThreeProngTau);

		std::vector<TString> genOneLep;
		TString onePromptTau                    = "&&(nPromptTau >= 1)";
		TString oneGoodTau                		= "&&(nPromptTauAcc >= 1)";
		TString oneProngTau                     = "&&(nPromptTau1Prong >= 1)";
		TString threeProngTau                	= "&&(nPromptTau3Prong >= 1)";
		genOneLep.push_back(onePromptTau);	genOneLep.push_back(oneGoodTau);
		genOneLep.push_back(oneProngTau);	genOneLep.push_back(threeProngTau);

		std::vector<TString> recoIncCuts;
		TString recoIncCuts1					= baseline     + "&&" + tauRegion;
		TString recoIncCuts2					= searchString + "&&" + tauRegion;
		recoIncCuts.push_back(recoIncCuts1);	recoIncCuts.push_back(recoIncCuts2);

		std::vector<TString> recoLepCuts;
		TString recoTauCut1      				= recoIncCuts1 +"&&(nvetotau    >= 1)";
		TString recoTauCut2      				= recoIncCuts2 +"&&(nvetotau    >= 1)";
		recoLepCuts.push_back(recoTauCut1);	recoLepCuts.push_back(recoTauCut2);

		std::vector<TString> fileStrings;
		fileStrings.push_back(inputPath + "ttbar_74X_tree.root");
		fileStrings.push_back(inputPath + "ttbar_73X_tree.root");
		//fileStrings.push_back(inputPath + "wjets_74X_tree.root");
		fileStrings.push_back(inputPath + "t2tt_74X_tree.root");
		fileStrings.push_back(inputPath + "t2tt_73X_tree.root");

		drawVetoHists(fileStrings,searchString);

		//Create cut hists

		double mcContamError;
		double mcFakeError;
		double mcEffError;

		ofstream yieldfile_;
		yieldfile_.open("taumcFakes.tex");
		yieldfile_ << "\\begin{center}" << std::endl;
		yieldfile_ << "\\begin{tabular}{|c|c|c|c|c|c|}" << std::endl;
		yieldfile_ << "\\hline" << std::endl;
		//($N_{track}$ = 1, $N_{GEN-\\tau}$ = 0)/($N_{track}$ = 1),($N_{track}$ = 1, $N_{GEN-\\tau}$ = 0)/(Inc. Hadronic)
		yieldfile_ << "Sample & Gen. Req. & Region & Efficiency & Contamination & Fake Rate  \\\\" << std::endl;
		yieldfile_ << "\\hline" << std::endl;

		//Calculate and Tabulate

		for(int k = 0; k < fileStrings.size(); k++){
			TFile *mcFile   = new TFile(fileStrings.at(k));
			TTree *mcTree	= (TTree*)mcFile->Get(fileTree);
			mcContamHist 	= new TH1F("mcContamHist","mcContamHist",1,0.0,1.0);	    mcContamHist->Sumw2();
			mcFakeHist		= new TH1F("mcFakeHist","mcFakeHist",1,0.0,1.0);	mcFakeHist->Sumw2();
			mcEffHist		= new TH1F("mcEffHist","mcEffHist",1,0.0,1.0);	mcEffHist->Sumw2();
			mc0GEN1RECO     = new TH1F("mc0GEN1RECO","mc0GEN1RECO",1,0.0,1.0);	mc0GEN1RECO->Sumw2();
			mc1GEN1RECO     = new TH1F("mc1GEN1RECO","mc1GEN1RECO",1,0.0,1.0);	mc1GEN1RECO->Sumw2();
			mc0GEN1RECOFake = new TH1F("mc0GEN1RECOFake","mc0GEN1RECOFake",1,0.0,1.0);	mc0GEN1RECOFake->Sumw2();
			mc1RECO         = new TH1F("mc1RECO","mc1RECO",1,0.0,1.0);	mc1RECO->Sumw2();
			mc1GEN	    	= new TH1F("mc1GEN","mc1GEN",1,0.0,1.0);	  mc1GEN->Sumw2();
			mcHadronic      = new TH1F("mcHadronic","mcHadronic",1,0.0,1.0);	mcHadronic->Sumw2();

				for(int j = 0; j < recoLepCuts.size(); j++){
					for(int i =0; i < genOneLep.size(); i++){
					mcTree->Draw(varDraw+">>mc0GEN1RECO",scaleName + "*" + Lumi +"*(" + recoLepCuts.at(j) + genNoLep.at(i)   +")");
					mcTree->Draw(varDraw+">>mc0GEN1RECOFake",scaleName + "*" + Lumi +"*(" + recoLepCuts.at(j)  + "&&(nPromptLep == 0))");
					mcTree->Draw(varDraw+">>mc1GEN1RECO",scaleName + "*" + Lumi +"*(" + recoLepCuts.at(j)  + genOneLep.at(i)  +  ")" ) ;
					mcTree->Draw(varDraw+">>mc1RECO"    ,scaleName + "*" + Lumi +"*(" +  recoLepCuts.at(j)  + ")");
					mcTree->Draw(varDraw+">>mc1GEN"     ,scaleName + "*" + Lumi +"*(" +  recoIncCuts.at(j)  + genOneLep.at(i) + ")");
					mcTree->Draw(varDraw+">>mcHadronic" ,scaleName + "*" + Lumi + "*(" + recoIncCuts.at(j) + "&&(nPromptLep == 0))");

					mcContamHist->Divide(mc0GEN1RECO,mc1RECO,1.0,1.0,"B");
					mcFakeHist->Divide(mc0GEN1RECOFake,mcHadronic,1.0,1.0,"B");
					mcEffHist->Divide(mc1GEN1RECO,mc1GEN,1.0,1.0,"B");

					double mcContam = mcContamHist->IntegralAndError(0,10001,mcContamError);
					double mcFake   = mcFakeHist->IntegralAndError(0,10001,mcFakeError);
					double mcEff    = mcEffHist->IntegralAndError(0,10001,mcEffError);

					yieldfile_.precision(3);
					yieldfile_ << std::fixed;

					if(fileStrings.at(k).Contains("ttbar_74X")){
					if (i == 0 and j == 0){
					yieldfile_ << "t $\\bar{t}$ 74X & ";}
					else
					yieldfile_ << "& ";
					}
					if(fileStrings.at(k).Contains("ttbar_73X")){
					if (i == 0 and j == 0){
					yieldfile_ << "t $\\bar{t}$ 73X & ";}
					else
					yieldfile_ << "& ";
					}
					/*if(fileStrings.at(k).Contains("wjets_74X")){
					if (i == 0 and j == 0){
					yieldfile_ << "W+jets 74X& ";}
					else
					yieldfile_ << "& ";
					}*/

					if(fileStrings.at(k).Contains("t2tt_74X")){
					if (i == 0 and j == 0){
					yieldfile_ << "T2tt-850-100-74X & ";}
					else
					yieldfile_ << "& ";
					}
					if(fileStrings.at(k).Contains("t2tt_73X")){
					if (i == 0 and j == 0){
					yieldfile_ << "T2tt-850-100-73X & ";}
					else
					yieldfile_ << "& ";
					}
					if(i == 0)
					yieldfile_ << "Prompt Tau &";
					if(i == 1)
					yieldfile_ << "Prompt Tau and Valid Gen. Daughter. &";
					if(i == 2)
					yieldfile_ << "Prompt 1-Prong Tau &";
					if(i == 3)
					yieldfile_ << "Prompt 3-Prong Tau &";
					if(j == 0){
						if (i == 0)
						yieldfile_ << "Tau 1L-Inc &" ;
						else
							yieldfile_ << "&" ;}
					if(j == 1){
						if (i == 0)
						yieldfile_ << "Tau 1LCR &" ;
						else
						yieldfile_ << "&" ;}

					yieldfile_<< mcEff << " $\\pm$ " << mcEffError << "&" <<  mcContam << " $\\pm$ " << mcContamError;

					if(k < 2)
					yieldfile_ << "&" << " \\\\ " << std::endl;
					else
					yieldfile_ << "&" << mcFake << " $\\pm$ " << mcFakeError << "\\\\ " << std::endl;

					if(j == 0 and i == 0)
					yieldfile_ << "\\hline " << std::endl;
					if(j == (recoLepCuts.size() - 1) and i == (genOneLep.size() - 1))
					yieldfile_ << "\\hline " << std::endl;
				}
		}
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
}
return (multiplied*sqrt(errorSum));
}


void drawVetoHists(std::vector<TString> fileStrings, TString searchString, bool norm){
	TString tauRegion 						= searchString + "&&(nSelectedElectrons == 0)&&(nSelectedMuons    == 0)";


TCanvas *c1;
TLegend *sevenFourLegend;TLegend *sevenThreeLegend;TLegend *totalLegend; TLegend* divttbar; TLegend* divt2tt;
TString inputPath = "run/lepEffSlimmed/";TString fileTree  = "events";TString scaleName = "weight";TString Lumi   = ".0402";
TH1F* t2tt_74X;TH1F* t2tt_73X;TH1F* ttbar_74X;TH1F* ttbar_73X;
std::vector<TString> cutStrings;cutStrings.push_back("mtCandVals");cutStrings.push_back("ptCandVals");cutStrings.push_back("etaCandVals");cutStrings.push_back("discCandVals");
std::vector<float> histstarts; histstarts.push_back(0.0);histstarts.push_back(10);histstarts.push_back(0.0);histstarts.push_back(-1.0);
std::vector<float> histends;histends.push_back(350);histends.push_back(100);histends.push_back(2.4);histends.push_back(1.0);
std::vector<int> nbins;nbins.push_back(35);nbins.push_back(90);nbins.push_back(72);nbins.push_back(100);
std::vector<int> nbinsRatio;nbinsRatio.push_back(7);nbinsRatio.push_back(9);nbinsRatio.push_back(6);nbinsRatio.push_back(10);

float mint2tt = 0;
std::vector<TH1F*> total;std::vector<TH1F*> sevenFourX;std::vector<TH1F*> sevenThreeX;

	for(int j = 0; j < cutStrings.size(); j++){			//std::cout << "The value of j is" << j << std::endl;
		totalLegend =new TLegend(0.75,0.85,1,1,"");totalLegend->SetTextFont(72);totalLegend->SetTextSize(0.04);
		sevenFourLegend =new TLegend(0.75,0.85,1,1,"");sevenFourLegend->SetTextFont(72);sevenFourLegend->SetTextSize(0.04);
		sevenThreeLegend =new TLegend(0.75,0.85,1,1,"");sevenThreeLegend->SetTextFont(72);sevenThreeLegend->SetTextSize(0.04);
		divttbar =new TLegend(0.75,0.85,1,1,"");divttbar->SetTextFont(72);divttbar->SetTextSize(0.04);
		divt2tt =new TLegend(0.75,0.85,1,1,"");divt2tt->SetTextFont(72);divt2tt->SetTextSize(0.04);
		for(int k = 0; k < fileStrings.size(); k++){			//std::cout << "The value of k is" << k << std::endl;
		TFile *mcFile   = new TFile(fileStrings.at(k));
		TTree *mcTree	= (TTree*)mcFile->Get(fileTree);
		t2tt_74X 	= new TH1F("t2tt_74X","t2tt_74X",nbins.at(j),histstarts.at(j),histends.at(j));	t2tt_74X->Sumw2();		t2tt_74X->SetLineWidth(3); t2tt_74X->SetStats(false);
		if(j == 0)t2tt_74X->SetTitle("Cand M_{T}; M_{T}; Norm. to arb. units;");
		if(j == 1)t2tt_74X->SetTitle("Cand p_{T};  p_{T}; Norm. to arb. units;");
		if(j == 2)t2tt_74X->SetTitle("Cand eta;  |#eta|; Norm. to arb. units;");
		if(j == 3)t2tt_74X->SetTitle("Cand Disc. Value;  Disc. Value; Norm. to arb. units;");

		t2tt_73X = (TH1F*) t2tt_74X->Clone("t2tt_73X");ttbar_74X = (TH1F*) t2tt_74X->Clone("ttbar_74X");ttbar_73X = (TH1F*) t2tt_74X->Clone("ttbar_73X");
		ttbar_74X->SetLineColor(kRed-3);	ttbar_73X->SetLineColor(kCyan+1);ttbar_74X->SetLineColor(kYellow-3);	ttbar_73X->SetLineColor(kGreen+1);

				if(fileStrings.at(k).Contains("ttbar_74X")){
					std::cout << "Filling ttbar 74X" << std::endl;
					mcTree->Draw(cutStrings.at(j)+">>ttbar_74X",scaleName + "*" + Lumi +"*(" + tauRegion   +")");
					std::cout << "ttbar 74X integrand is " << ttbar_74X->Integral() << std::endl;
					if(norm)ttbar_74X->Scale(1/ttbar_74X->Integral());
					total.push_back(ttbar_74X);sevenFourX.push_back(ttbar_74X);

					totalLegend->AddEntry(ttbar_74X,"ttbar-74X (matched)","l");
					sevenFourLegend->AddEntry(ttbar_74X,"ttbar-74X (matched)","l");
					divttbar->AddEntry(ttbar_74X,"ttbar-74X (matched)","l");
				}
				if(fileStrings.at(k).Contains("ttbar_73X")){
					std::cout << "Filling ttbar 73X" << std::endl;
					mcTree->Draw(cutStrings.at(j)+">>ttbar_73X",scaleName + "*" + Lumi +"*(" + tauRegion   + ")");
					std::cout << "ttbar 73X integrand is " << ttbar_73X->Integral() << std::endl;
					if(norm)ttbar_73X->Scale(1/ttbar_73X->Integral());

					total.push_back(ttbar_73X);sevenThreeX.push_back(ttbar_73X);
					totalLegend->AddEntry(ttbar_73X,"ttbar-73X (matched)","l");
					sevenThreeLegend->AddEntry(ttbar_73X,"ttbar-73X (matched)","l");
					divttbar->AddEntry(ttbar_73X,"ttbar-73X (matched)","l");
				}
				if(fileStrings.at(k).Contains("t2tt_74X")){
					std::cout << "Filling t2tt 74X" << std::endl;
					mcTree->Draw(cutStrings.at(j)+ ">>t2tt_74X",scaleName + "*" + Lumi +"*(" + tauRegion   + "&&(nPromptLep == 0)" +")");
					std::cout<< "The integral of the t2tt 74X is "<< t2tt_74X->Integral() << std::endl;
					if(t2tt_74X->Integral() > mint2tt) mint2tt = t2tt_74X->Integral();
					if(norm)t2tt_74X->Scale(1/t2tt_74X->Integral());

					total.push_back(t2tt_74X);sevenFourX.push_back(t2tt_74X);
					totalLegend->AddEntry(t2tt_74X,"t2tt-74X(0L)","l");
					sevenFourLegend->AddEntry(t2tt_74X,"t2tt-74X(0L)","l");
					divt2tt->AddEntry(t2tt_74X,"t2tt-74X(0L)","l");
				}
				if(fileStrings.at(k).Contains("t2tt_73X")){
					std::cout << "Filling t2tt73X" << std::endl;
					mcTree->Draw(cutStrings.at(j)+">>t2tt_73X",scaleName + "*" + Lumi +"*(" + tauRegion  + "&&(nPromptLep == 0)"   +")");
					std::cout<< "The integral of the t2tt73X is "<< t2tt_73X->Integral() << std::endl;
					if(t2tt_73X->Integral() > mint2tt)mint2tt = t2tt_73X->Integral();
					if(norm)t2tt_73X->Scale(1/t2tt_73X->Integral());

					total.push_back(t2tt_73X);sevenThreeX.push_back(t2tt_73X);
					totalLegend->AddEntry(t2tt_73X,"t2tt-73X (0L)","l");
					sevenThreeLegend->AddEntry(t2tt_73X,"t2tt-73X (0L)","l");
					divt2tt->AddEntry(t2tt_73X,"t2tt-73X (0L)","l");
				}
		}
		c1 = new TCanvas("c1","example",700,600);
		c1->cd();
		int counter = 0;
		for(auto j : sevenFourX){
			if(counter == 0)
			j->Draw("hist");
			else
			j->Draw("histsame");
			counter++;}
		sevenFourLegend->Draw();c1->cd();
		c1->SaveAs("run/lepEffSlimmed/output/" + cutStrings.at(j) + "_74X.png");
	c1 = new TCanvas("c1","example",700,600);
	c1->cd();
	int counter2 = 0;

	for(auto j : sevenThreeX){
				if(counter2 == 0)
				j->Draw("hist");
				else
				j->Draw("histsame");
				counter2++;}
			sevenThreeLegend->Draw();c1->cd();
			c1->SaveAs("run/lepEffSlimmed/output/" + cutStrings.at(j) + "_73X.png");
	c1 = new TCanvas("c1","example",700,600);
	c1->cd();
	int counter3 = 0;

	for(auto j : total){
				if(counter3 == 0)
				j->Draw("hist");
				else
				j->Draw("histsame");
				counter3++;}
			totalLegend->Draw();c1->cd();
			c1->SaveAs("run/lepEffSlimmed/output/" + cutStrings.at(j) + "_total.png");

	std::cout << "the seventhreeX size is " << sevenThreeX.size() << std::endl;
	for(int i =0; i < sevenThreeX.size(); i++){
		TCanvas *c1 = new TCanvas("c1","example",700,825);
		c1->cd();
		TPad *p1 = new TPad("p1","p1",0,0.3,1,1);
		p1->SetLeftMargin  (0.18);
		p1->SetTopMargin   (0.10);
		p1->SetRightMargin (0.07);
		p1->SetBottomMargin(0.03);
		p1->Draw();
		p1->cd();
		sevenThreeX.at(i)->Draw("hist");sevenFourX.at(i)->Draw("histsame");//sevenFourX.at(i)->Draw("Esame");
		gPad->Update();

		////Format Legend
		if (sevenThreeX.at(i)->Integral() > mint2tt ){		divttbar->Draw();}
		else divt2tt->Draw();
		c1->cd();

		////Clone into divisor
		TH1F* tempHist = new TH1F("divideHist","divideHist",nbinsRatio.at(j),histstarts.at(j),histends.at(j));
		tempHist->Sumw2();
		tempHist = (TH1F*)sevenFourX.at(i)->Clone("tempHist"); tempHist->Divide(sevenThreeX.at(i));
		TH1F* divideHist = (TH1F*)tempHist->Rebin(nbinsRatio.at(j),"divideHist");
		////Building Second pad
		TPad *p2 = new TPad("p2","p2",0,0,1,0.3);
		p2->SetLeftMargin  (0.18);
		p2->SetTopMargin   (0.00);
		p2->SetRightMargin (0.07);
		p2->SetBottomMargin(0.30);
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
		divideHist->GetXaxis()->SetTitle(cutStrings.at(j));
		divideHist->GetYaxis()->SetRangeUser(.4,1.6);
		//// Draw the ratioGram
		divideHist->Draw("EP");

		if (sevenThreeX.at(i)->Integral() > mint2tt ){
			c1->SaveAs("run/lepEffSlimmed/output/" + cutStrings.at(j) + "_ttbar_ratio.png");}
		else
			c1->SaveAs("run/lepEffSlimmed/output/" + cutStrings.at(j) + "_t2tt_ratio.png");

	}
	sevenFourX.clear();		sevenThreeX.clear();		total.clear();

	}

}


