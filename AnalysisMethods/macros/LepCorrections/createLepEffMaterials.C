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
////MAIN FUNCTIONS
void genvsRecoGraphs();
void efficiencyAndCompGraphs();

////ADDITIONAL FUNCTIONS
void fillSearchVectors(std::vector<TString>& c_SBins_0l, std::vector<TString>& c_SBins_1l,TString c_presel_0l,TString c_presel_1l);
TH1F* loadHist(TString varDraw, TString cutString, TString fileName,int nBins, double histBegin, double histFinal, int color = 0);
TH1F* loadHist(TString varDraw, TString cutString, TString fileName,int nBins, Double_t* bins, int color = 0);
void tWriterRatioComp(TString  outputDir = "",TString histTitle = "", TString outputPrefix = "",TString  file1 = "",TString  varDraw1 = "",TString varDraw2 = "",int nBins = 30,
double histBegin = 0, double histFinal = 300, TString cutString1 = "", TString cutString2 = "", TString file1LegendName = "Reco Distribution",TString file2LegendName = "Gen Distribution",bool norm = true,bool ratio = true);
void tWriterRatioComp(TString  outputDir = "",TString histTitle = "", TString outputPrefix = "",TString  file1 = "",TString  varDraw1 = "",TString varDraw2 = "",int nBins = 30,
Double_t histValues[] = {}, TString cutString1 = "", TString cutString2 = "", TString file1LegendName = "Reco Distribution",TString file2LegendName = "Gen Distribution",bool norm = true,bool ratio = true);

////NECESSARY INPUT AND OUTPUT AREAS
TFile *f  = new TFile("run/lepEffSlimmed/sm_tree.root");
TString  tTree = "events";
TTree *t = (TTree*)f->Get(tTree);
TString  scaleName = "weight";
TString  Lumi = "4";
TLegend *legend;
TCanvas *c1;
TPad *p1;
TString fileIn = "run/lepEffSlimmed/sm_tree.root";
TString filePrefix = "run/lepEffSlimmed/";

////ASSUME THAT TREE HAS BEEN FILLED WITH BASIC MET/JET CUTS, HERE ARE THE NECESSARY REGIONS:
TString baseline 				= "(met >= 200)&&(nbjets >= 1)&&(njets60 >= 2) && (dphij12met>.4) && (dphij3met>0.4) ";
TString searchString 			= baseline 				 + "&&(mtcsv12MET > 175)&& (dphij12met>1) && (dphij3met>0.5)";

TString zeroLCR 				= baseline				 + "&&(nSelectedElectrons == 0) &&(nSelectedMuons == 0)&&(nvetotau == 0)";
TString zeroLSR 				= zeroLCR 		 + "&&(mtcsv12MET > 175) && (dphij12met>1) && (dphij3met>0.5)";
TString oneLCR		 			= baseline 				 + "&&( (nSelectedElectrons >= 1)||(nSelectedMuons >= 1)||(nvetotau >= 1) )";
TString oneLSR 					= oneLCR  		 		 + "&&(mtcsv12MET > 175) && (dphij12met>1) && (dphij3met>0.5)";

TString muonCR     				= baseline		 		 + "&&(nSelectedMuons >= 1)&&(nSelectedElectrons == 0)";
TString eleCR 					= baseline		 		 + "&&(nSelectedElectrons >= 1)";
TString tauCR     				= baseline 				 + "&&(nSelectedElectrons == 0) &&(nSelectedMuons == 0) ";

TString muonSR     				= oneLSR 	 			 + "&&(nSelectedMuons >= 1)&&(nSelectedElectrons == 0)";
TString eleSR 					= oneLSR	 			 + "&&(nSelectedElectrons >= 1)";
TString tauSR      				= oneLSR 		 		 + "&&(nSelectedElectrons == 0) &&(nSelectedMuons == 0) ";

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

//Instantiate Binning
Double_t etaBins[] = {-2.4,-1.6,-.8,0,.8,1.6,2.4};
Double_t ptBins[] = {5,15,25,35,45,55,80,105,130};
Double_t htAlongBins[] = {0,75,150,225,300,400,600};

void createLepEffMaterials(){
	genvsRecoGraphs();
	efficiencyAndCompGraphs();

}
/////FILL IMPORTANT GEN VS RECO QUANTITIES:
void genvsRecoGraphs(){

	////CREATE GEN KINEMATICS COMPARISONS BETWEEN LCR AND LSR
   tWriterRatioComp("run/latestLep/output","GEN Tau p_{T}"        ,"Tau_CLvsSR_Pt","run/lepEffSlimmed/sm_tree.root","hardestGenTauDPt","hardestGenTauDPt",8,ptBins,tauCR+"&&(hardestGenTauDPt > 0)",tauSR+"&&(hardestGenTauDPt > 0)","#tau-CR","#tau-SR");
   tWriterRatioComp("run/latestLep/output","GEN Ele p_{T}"        ,"Ele_CLvsSR_Pt","run/lepEffSlimmed/sm_tree.root","hardestGenElePt","hardestGenElePt",8,ptBins,eleCR+"&&(hardestGenElePt > 0)",eleSR + "&&(hardestGenElePt > 0)","ele-CR","ele-SR");
   tWriterRatioComp("run/latestLep/output","GEN Muon  p_{T}"      ,"Mu_CLvsSR_Pt","run/lepEffSlimmed/sm_tree.root","hardestGenMuPt","hardestGenMuPt",8,ptBins,muonCR+"&&(hardestGenMuPt > 0)",muonSR+  "&&(hardestGenMuPt > 0)","mu-CR","mu-SR");

   tWriterRatioComp("run/latestLep/output","GEN Tau #eta"         ,"Tau_CLvsSR_Eta","run/lepEffSlimmed/sm_tree.root","hardestGenTauEtaD","hardestGenTauEtaD",8,0.0,2.5  ,tauCR+"&&(hardestGenTauDPt > 0)",tauSR + "&&(hardestGenTauDPt > 0)","#tau-CR","#tau-SR");
   tWriterRatioComp("run/latestLep/output","GEN Ele #eta"         , "Ele_CLvsSR_Eta","run/lepEffSlimmed/sm_tree.root","hardestGenEleEta","hardestGenEleEta",8,0.0,2.5,eleCR+"&&(hardestGenElePt > 0)",eleSR+ "&&(hardestGenElePt > 0)","ele-CR","ele-SR");
   tWriterRatioComp("run/latestLep/output","GEN Mu  #eta"         , "Mu_CLvsSR_Eta","run/lepEffSlimmed/sm_tree.root","hardestGenMuEta","hardestGenMuEta",8,0.0,2.5,muonCR+"&&(hardestGenMuPt > 0)",muonSR+ "&&(hardestGenMuPt > 0)","mu-CR","mu-SR");

   tWriterRatioComp("run/latestLep/output","GEN Tau H_{T}-Along"  ,"Tau_CLvsSR_HTAlong","run/lepEffSlimmed/sm_tree.root","htmlAlongHardestTauD","htmlAlongHardestTauD",12,0.01,600  ,tauCR,tauSR,"#tau-CR.","#tau-SR");
   tWriterRatioComp("run/latestLep/output","GEN Ele. H_{T}-Along" , "Ele_CLvsSR_HTAlong","run/lepEffSlimmed/sm_tree.root","htmlAlongHardestEle","htmlAlongHardestEle",12,0.01,600,eleCR,eleSR,"ele-CR","ele-SR");
   tWriterRatioComp("run/latestLep/output","GEN Muon H_{T}-Along" , "Mu_CLvsSR_HTAlong","run/lepEffSlimmed/sm_tree.root","htmlAlongHardestMu","htmlAlongHardestMu",12,0.01,600,muonCR,muonSR,"mu-CR","mu-SR");

	c1 = new TCanvas("c1","example",700,600);
	c1->cd();
	//Build Pad
	p1= new TPad("p1","p1",0,.1,1,1);p1->SetLeftMargin  (0.15);p1->SetTopMargin   (0.10);p1->SetRightMargin (0.07);p1->SetBottomMargin(0.03);p1->Draw();p1->cd();

	////CREATE LEPTON COUNT COMPARISONS BETWEEN LCR AND LSR
   TH1F* nGenTau1L   = loadHist("nPromptTau"      , tauCR			, fileIn,3,- .5,2.5,2); nGenTau1L->Sumw2();
   TH1F* nGenEle1L   = loadHist("nGoodEle"   	  , eleCR	, fileIn,3, -.5,2.5,1); nGenEle1L->Sumw2();
   TH1F* nGenMu1L    = loadHist("nGoodMu"         , muonCR		, fileIn,3, -.5,2.5,0); nGenMu1L->Sumw2();
   TH1F* nLepScale1L = (TH1F*)nGenTau1L->Clone();
   nLepScale1L->Add(nGenEle1L);	nLepScale1L->Add(nGenMu1L);

	//COUNT LEPTONS IN THE LCR
   	nGenTau1L->SetTitle("Number of Gen Lep. LCR;  NGen of type #ell; Sum Norm. to arb. units;");
   	nGenTau1L->Scale(1/(nLepScale1L->Integral()));nGenEle1L->Scale(1/(nLepScale1L->Integral()));nGenMu1L->Scale(1/(nLepScale1L->Integral()));
   	nGenTau1L->GetYaxis()->SetRangeUser(0,.5);
   	nGenTau1L->Draw("histE");nGenEle1L->Draw("sameE");nGenEle1L->Draw("samehist");nGenMu1L->Draw("sameE");nGenMu1L->Draw("samehist");
	legend=new TLegend(0.75,0.85,1,1,"");legend->SetTextFont(72);legend->SetTextSize(0.04);
	legend->AddEntry(nGenMu1L,"Muon CR","l");legend->AddEntry(nGenEle1L,"Electron CR","l");legend->AddEntry(nGenTau1L,"Tau CR","l");
	legend->Draw();	c1->cd();
	//Save and reset pad
	c1->SaveAs("run/latestLep/output/1LInc_nleptons.png");	c1->Clear();
	p1= new TPad("p1","p1",0,.1,1,1);p1->SetLeftMargin  (0.15);p1->SetTopMargin   (0.10);p1->SetRightMargin (0.07);p1->SetBottomMargin(0.03);p1->Draw();p1->cd();

	nGenTau1L   = loadHist("nPromptTau"      , tauSR			, fileIn,3,- .5,2.5,2);
	nGenEle1L   = loadHist("nGoodEle"   	 , eleSR		, fileIn,3, -.5,2.5,1);
	nGenMu1L    = loadHist("nGoodMu"         , muonSR			, fileIn,3, -.5,2.5,0);
	nLepScale1L = (TH1F*)nGenTau1L->Clone();
	nLepScale1L->Add(nGenEle1L);	nLepScale1L->Add(nGenMu1L);

	//COUNT LEPTONS IN THE LCR
   	nGenTau1L->SetTitle("Number of Gen Lep. LSR;  NGenLeptons; Sum Norm. to arb. units;");
   	nGenTau1L->Scale(1/(nLepScale1L->Integral()));nGenEle1L->Scale(1/(nLepScale1L->Integral()));nGenMu1L->Scale(1/(nLepScale1L->Integral()));
   	nGenTau1L->GetYaxis()->SetRangeUser(0,.5);
   	nGenTau1L->Draw("histE");nGenEle1L->Draw("sameE");nGenEle1L->Draw("samehist");nGenMu1L->Draw("sameE");nGenMu1L->Draw("samehist");
	legend=new TLegend(0.75,0.85,1,1,"");legend->SetTextFont(72);legend->SetTextSize(0.04);
	legend->AddEntry(nGenMu1L,"Muon SR, #ell = #mu","l");legend->AddEntry(nGenEle1L,"Electron SR, #ell = e","l");legend->AddEntry(nGenTau1L,"Tau SR, #ell = #tau","l");
	legend->Draw();	c1->cd();
	//Save and reset pad
	c1->SaveAs("run/latestLep/output/LSR_nleptons.png");	c1->Clear();
}
void efficiencyAndCompGraphs(){

	//Build Canvas
	c1 = new TCanvas("c1","example",700,600);
	c1->cd();
	//Build Pad
	p1= new TPad("p1","p1",0,.1,1,1);p1->SetLeftMargin  (0.15);p1->SetTopMargin   (0.10);p1->SetRightMargin (0.07);p1->SetBottomMargin(0.03);p1->Draw();p1->cd();

	TH1F* recoHtAMuon     = loadHist("htmlAlongHardestMu", recoRegionMuonFinal,fileIn,6, htAlongBins,0);
	TH1F* fakeDenomHtAMuon = loadHist("htmlAlongHardestMu", fakeDenomMuonFinal, fileIn,6, htAlongBins,0);
	TH1F* fakeNumHtAMuon = loadHist("htmlAlongHardestMu", fakeNumMuonFinal, fileIn,6, htAlongBins,0);
	TH1F* genHtAMuon      = loadHist("htmlAlongHardestMu", genRegionMuonFinal, fileIn,6, htAlongBins);

	TH1F* recoHtAele      = loadHist("htmlAlongHardestEle", recoRegionElectronFinal,fileIn,6, htAlongBins,1);
	TH1F* fakeDenomHtAele  = loadHist("htmlAlongHardestEle", fakeDenomElectronFinal, fileIn,6, htAlongBins,1);
	TH1F* fakeNumHtAele  = loadHist("htmlAlongHardestEle", fakeNumElectronFinal, fileIn,6, htAlongBins,1);
	TH1F* genHtAele       = loadHist("htmlAlongHardestEle", genRegionElectronFinal, fileIn,6, htAlongBins);

	TH1F* recoHtAtau      = loadHist("htmlAlongHardestTauD", recoRegionTauFinal,fileIn,6, htAlongBins,2);
	TH1F* fakeDenomHtAtau  = loadHist("htmlAlongHardestTauD", fakeDenomTauFinal, fileIn,6, htAlongBins,2);
	TH1F* fakeNumHtAtau     = loadHist("htmlAlongHardestTauD", fakeNumTauFinal, fileIn,6, htAlongBins,2);
	TH1F* genHtAtau       = loadHist("htmlAlongHardestTauD", genRegionTauFinal, fileIn,6, htAlongBins);

	//Construct Composite HTAlong Eff graph and add legend
	recoHtAMuon->SetTitle("Lepton Eff. vs H_{T} - Along_{#ell}; Lepton Efficiency; H_{T} - Along_{#ell} ");
	recoHtAMuon->Divide(recoHtAMuon,genHtAMuon, 1.0, 1.0, "B");	recoHtAele->Divide(recoHtAele,genHtAele, 1.0, 1.0, "B");recoHtAtau->Divide(recoHtAtau, genHtAtau, 1.0, 1.0, "B");
	recoHtAMuon->GetYaxis()->SetRangeUser(.3,1);
	recoHtAMuon->Draw("histE");recoHtAele->Draw("sameE");recoHtAele->Draw("samehist");recoHtAtau->Draw("sameE");recoHtAtau->Draw("samehist");
	legend=new TLegend(0.75,0.85,1,1,"");legend->SetTextFont(72);legend->SetTextSize(0.04);
	legend->AddEntry(recoHtAMuon,"Muon","l");legend->AddEntry(recoHtAele,"Electron","l");legend->AddEntry(recoHtAtau,"Tau","l");
	legend->Draw();	c1->cd();

	//Save and reset the padd
	c1->SaveAs("run/latestLep/output/comp_HtA_test_eff.png");c1->Clear();
	p1= new TPad("p1","p1",0,.1,1,1);p1->SetLeftMargin  (0.15);p1->SetTopMargin   (0.10);p1->SetRightMargin (0.07);p1->SetBottomMargin(0.03);p1->Draw();p1->cd();

	//Construct Composite HTAlong Fake graph and add legend
	fakeNumHtAtau->SetTitle("Lepton Fake Rate vs  HTAlong; Lepton Efficiency; Lep. Subtracted HTAlong ");
	fakeNumHtAtau->Divide(fakeDenomHtAtau);	fakeNumHtAMuon->Divide(fakeDenomHtAMuon); fakeNumHtAele->Divide(fakeDenomHtAele);
	fakeNumHtAtau->Draw("histE");fakeNumHtAMuon->Draw("sameE");fakeNumHtAMuon->Draw("samehist");fakeNumHtAele->Draw("sameE");fakeNumHtAele->Draw("samehist");
	legend=new TLegend(0.75,0.65,1,.85,"");legend->SetTextFont(72);legend->SetTextSize(0.04);
	legend->AddEntry(fakeNumHtAMuon,"Muon Fakes","l");legend->AddEntry(fakeNumHtAele,"Electron Fakes","l");legend->AddEntry(fakeNumHtAtau,"Tau Fakes","l");
	legend->Draw();	c1->cd();

	//Save and reset the padd
	c1->SaveAs("run/latestLep/output/comp_HtA_test_ineff.png");c1->Clear();
	p1= new TPad("p1","p1",0,.1,1,1);p1->SetLeftMargin  (0.15);p1->SetTopMargin   (0.10);p1->SetRightMargin (0.07);p1->SetBottomMargin(0.03);p1->Draw();p1->cd();


	////CONSTRUCT BINNED EFF. COMPARISONS
	TH1F* binnedEff; TH1F* binnedEff2;

	////COMPUTE AND DRAW MUON BINNED EFF LCR VS LSR
	binnedEff = binnedYields::makeBinnedYields(recoRegionMuonFinal,genRegionMuonFinal,fileIn);
	binnedEff2 = binnedYields::makeBinnedYields(searchString + "&&" +recoRegionMuonFinal,searchString + "&&" + genRegionMuonFinal,fileIn);
	binnedEff2->SetLineColor(kBlue);

	binnedEff->SetTitle("Muon Eff. in SR Bins;Search Bins;Selection (Veto) Efficiency");
	p1= new TPad("p1","p1",0,.1,1,1);p1->SetLeftMargin  (0.15);p1->SetTopMargin   (0.10);p1->SetRightMargin (0.07);p1->SetBottomMargin(0.03);p1->Draw();

	   TH1F* testGen1   = loadHist("met"      , recoRegionMuonFinal			, fileIn,3,- .5,2.5,2); testGen1->Sumw2();
		TH1F* testGen2   = loadHist("met"      , genRegionMuonFinal			, fileIn,3,- .5,2.5,2); testGen2->Sumw2();
		Double_t error1;
		Double_t meanEff = testGen1->IntegralAndError(0,1001,error1)/testGen2->IntegralAndError(0,1001,error1);
		for (int i = 0; i < 20; i++ ){
			binnedEff->SetBinContent((i+1),meanEff);
			binnedEff->SetBinError((i+1),0);}
		binnedEff->Draw("histE");

	binnedEff2->Draw("histsame");
	binnedEff2->Draw("Esame");
	legend=new TLegend(0.75,0.85,1,1,"");legend->SetTextFont(72);legend->SetTextSize(0.04);
	legend->AddEntry(binnedEff,"LCR");legend->AddEntry(binnedEff2,"LSR");
	legend->Draw();	c1->cd();
	c1->SaveAs("run/latestLep/output/muon_binned_eff_comp.png");	c1->Clear();



		std::cout << "The binnedeff mean is " << meanEff;
	////COMPUTE AND DRAW ELECTRON BINNED EFF LCR VS LSR
	p1= new TPad("p1","p1",0,.1,1,1);p1->SetLeftMargin  (0.15);p1->SetTopMargin   (0.10);p1->SetRightMargin (0.07);p1->SetBottomMargin(0.03);p1->Draw();
	binnedEff = binnedYields::makeBinnedYields(recoRegionElectronFinal,genRegionElectronFinal,fileIn);
	   testGen1   = loadHist("met"      , recoRegionMuonFinal			, fileIn,3,- .5,2.5,2); testGen1->Sumw2();
		 testGen2   = loadHist("met"      , genRegionMuonFinal			, fileIn,3,- .5,2.5,2); testGen2->Sumw2();

			 meanEff = testGen1->IntegralAndError(0,1001,error1)/testGen2->IntegralAndError(0,1001,error1);
		for (int i = 0; i < 20; i++ ){
			binnedEff->SetBinContent((i+1),meanEff);
			binnedEff->SetBinError((i+1),0);}
	binnedEff2 = binnedYields::makeBinnedYields(searchString + "&&" +recoRegionElectronFinal,searchString + "&&" + genRegionElectronFinal,fileIn);
	binnedEff2->SetLineColor(kBlue);


	binnedEff->SetTitle("Ele. Eff. in SR Bins;Search Bins;Selection (Veto) Efficiency");
	binnedEff->GetYaxis()->SetRangeUser(.3,1.05);

	binnedEff->Draw("histE");
	binnedEff2->Draw("histsame");
	binnedEff2->Draw("Esame");
	legend=new TLegend(0.75,0.85,1,1,"");legend->SetTextFont(72);legend->SetTextSize(0.04);
	legend->AddEntry(binnedEff,"LCR");legend->AddEntry(binnedEff2,"LSR");
	legend->Draw();	c1->cd();
	c1->SaveAs("run/latestLep/output/ele_binned_comp_eff.png");	c1->Clear();
	p1= new TPad("p1","p1",0,.1,1,1);p1->SetLeftMargin  (0.15);p1->SetTopMargin   (0.10);p1->SetRightMargin (0.07);p1->SetBottomMargin(0.03);p1->Draw();
	p1->SetLogy(false); 	c1->SetLogy(false); 	p1->cd();c1->cd();

	////COMPUTE AND DRAW TAU BINNED EFF LCR VS LSR
	binnedEff = binnedYields::makeBinnedYields(recoRegionTauFinal,genRegionTauFinal,fileIn);
	binnedEff2 = binnedYields::makeBinnedYields(searchString + "&&" + recoRegionTauFinal,searchString + "&&" +genRegionTauFinal,fileIn);
	binnedEff->SetTitle("Tau Eff. in SR Bins;Search Bins;Selection (Veto) Efficiency");
	p1= new TPad("p1","p1",0,.1,1,1);p1->SetLeftMargin  (0.15);p1->SetTopMargin   (0.10);p1->SetRightMargin (0.07);p1->SetBottomMargin(0.03);p1->Draw();
	binnedEff2->SetLineColor(kBlue);
	binnedEff->GetYaxis()->SetRangeUser(0,1.05);
	    testGen1   = loadHist("met"      , recoRegionTauFinal			, fileIn,3,- .5,2.5,2); testGen1->Sumw2();
	    testGen2   = loadHist("met"      , genRegionTauFinal			, fileIn,3,- .5,2.5,2); testGen2->Sumw2();

		 meanEff = testGen1->IntegralAndError(0,1001,error1)/testGen2->IntegralAndError(0,1001,error1);

	std::cout << "The binnedeff mean is " << meanEff;

	for (int i = 0; i < 20; i++ ){
		binnedEff->SetBinContent((i+1),meanEff);
		binnedEff->SetBinError((i+1),0);
	}

	binnedEff->Draw("histE");
	binnedEff2->Draw("histsame");
	binnedEff2->Draw("Esame");
	legend=new TLegend(0.75,0.85,1,1,"");legend->SetTextFont(72);legend->SetTextSize(0.04);
	legend->AddEntry(binnedEff,"LCR");legend->AddEntry(binnedEff2,"LSR");
	legend->Draw();	c1->cd();
	c1->SaveAs("run/latestLep/output/tau_binned_comp_eff.png");	c1->Clear();

	p1= new TPad("p1","p1",0,.1,1,1);p1->SetLeftMargin  (0.15);p1->SetTopMargin   (0.10);p1->SetRightMargin (0.07);p1->SetBottomMargin(0.03);p1->Draw();
	p1->cd();c1->cd();

    TH1F* oneLSRHist;TH1F* oneLCRHist;TLegend* legend2;
	vector<TString> samples;
	samples.push_back("qcd_tree.root");
	samples.push_back("ttbar_tree.root");
	samples.push_back("wjets_tree.root");
	samples.push_back("ttW_tree.root");
	samples.push_back("tW_tree.root");
	samples.push_back("ttZ_tree.root");
	samples.push_back("znunu_tree.root");

	vector<TH1F*> hists;vector<TH1F*> histsSR;

	TH1F* binnedSumInc    = new TH1F("binnedSumInc","binnedSumInc",20,0.0,20.0);
	TH1F* binnedSumSearch = new TH1F("binnedSumSearch","binnedSumSearch",20,0.0,20.0);

	TH1F* backCompCRSum     = new TH1F("backCompCRSum","backCompCRSum",6,-.5,5.5);
	TH1F* backCompCRTemp    = new TH1F("backCompCRTemp","backCompCRTemp",6,-.5,5.5);
	backCompCRSum->Sumw2();	backCompCRTemp->Sumw2();
	TH1F* backCompSRSum     = new TH1F("backCompSRSum","backCompSRSum",6,-.5,5.5);
	TH1F* backCompSRTemp    = new TH1F("backCompSRTemp","backCompSRTemp",6,-.5,5.5);
	backCompSRSum->Sumw2();	backCompSRTemp->Sumw2();
	TH1F* backComp0LSRSum     = new TH1F("backComp0LSRSum","backComp0LSRSum",6,-.5,5.5);
	TH1F* backComp0LSRTemp    = new TH1F("backComp0LSRTemp","backComp0LSRTemp",6,-.5,5.5);
	backComp0LSRSum->Sumw2();	backComp0LSRTemp->Sumw2();
	TH1F* backCompFinal;
	vector<TH1F*> compHists;

	legend=new TLegend(0.73,0.85,1,1,"");legend->SetTextFont(12);legend->SetTextSize(0.03);
	legend2=new TLegend(0.73,0.85,1,1,"");legend->SetTextFont(12);legend->SetTextSize(0.03);
	TLegend* legend3=new TLegend(0.73,0.85,1,1,"");legend->SetTextFont(12);legend->SetTextSize(0.03);

	for(int i = 0; i < samples.size(); i++){
		oneLSRHist = binnedYields::makeBinnedYields(oneLCR,oneLCR,filePrefix + samples.at(i),"met",false);
		binnedSumInc->Add(oneLSRHist);
		oneLSRHist  = binnedYields::makeBinnedYields(oneLSR,oneLSR,filePrefix + samples.at(i),"met",false);
		binnedSumSearch->Add(oneLSRHist);

		backCompCRTemp    = loadHist("met"      , oneLCR			, filePrefix + samples.at(i),6,-.5,5.5);
		backCompSRTemp    = loadHist("met"      , oneLSR			, filePrefix + samples.at(i),6,-.5,5.5);
		backComp0LSRTemp   = loadHist("met"      , zeroLCR			, filePrefix + samples.at(i),6,-.5,5.5);
		backCompCRSum->Add(backCompCRTemp);
		backCompSRSum->Add(backCompSRTemp);
		backComp0LSRSum->Add(backComp0LSRTemp);
}

	for(int i = 0; i < samples.size(); i++){
		double tempCRError;		double tempSRError;
		backCompFinal     = new TH1F("backCompFinal","backCompFinal",6,-.5,5.5);
		double backComp0LSRError;

		//std::cout << "The sample string reads " << samples.at(i) << std::endl;
		oneLSRHist = binnedYields::makeBinnedYields(oneLCR,oneLCR,filePrefix + samples.at(i),"met",false);
		oneLSRHist = binnedYields::makeBinnedYields(oneLSR,oneLSR,filePrefix + samples.at(i),"met",false);

		backCompCRTemp    = loadHist("met"      , oneLCR			, filePrefix + samples.at(i),6,-.5,5.5);
		//std::cout << "the back comp final at i = " << i << " before dividing finds a yield of " << backCompCRTemp->IntegralAndError(0,10001,tempCRError);

		backCompCRTemp->Divide(backCompCRSum);

		backCompSRTemp    = loadHist("met"      , oneLSR			, filePrefix + samples.at(i),6,-.5,5.5);
		backCompSRTemp->Divide(backCompSRSum);

		backComp0LSRTemp  = loadHist("met"      , zeroLCR			, filePrefix + samples.at(i),6,-.5,5.5);
		std::cout << "the back comp final at i = " << i << " before dividing finds a yield of " << backComp0LSRTemp->IntegralAndError(0,10001,backComp0LSRError);

		backComp0LSRTemp->Divide(backComp0LSRSum);

		backCompFinal->SetBinContent(1,backCompCRTemp->IntegralAndError(0,10001,tempCRError));
		backCompFinal->SetBinError(1,tempCRError);
		backCompFinal->SetBinContent(3,backCompSRTemp->IntegralAndError(0,10001,tempSRError));
		backCompFinal->SetBinError(3,tempSRError);
		backCompFinal->SetBinContent(5,backComp0LSRTemp->IntegralAndError(0,10001,backComp0LSRError));
		backCompFinal->SetBinError(5,backComp0LSRError);
		std::cout << "the back comp final at i = " << i << "finds a yield of " << backComp0LSRTemp->IntegralAndError(0,10001,tempCRError);

		backCompFinal->SetTitle("LCR Bkg Composition;Control Region, Selection Region;Norm. to arb. units");
		backCompFinal->SetStats(false);
		backCompFinal->SetLineWidth(3);
		oneLSRHist->SetTitle("LCR Bkg Composition;Search Bins;Selection (Veto) Efficiency");
		oneLSRHist->SetTitle("LSR Bkg Composition;Search Bins;Selection (Veto) Efficiency");


		oneLSRHist->Divide(oneLSRHist,binnedSumInc, 1.0, 1.0, "B");
		oneLSRHist->Divide(oneLSRHist,binnedSumSearch, 1.0, 1.0, "B");

		//oneLSRHist->GetXaxis()->SetRangeUser(0,21.0);
		oneLSRHist->GetYaxis()->SetRangeUser(0,1);
		oneLSRHist->GetYaxis()->SetRangeUser(0,1);
		if(samples.at(i) == "qcd_tree.root")
		{
			oneLSRHist->SetLineColor(kYellow);oneLSRHist->SetLineColor(kYellow);backCompFinal->SetLineColor(kYellow);
			legend->AddEntry(oneLSRHist,"QCD");legend2->AddEntry(oneLSRHist,"QCD");legend3->AddEntry(backCompFinal,"QCD");

		}
		if(samples.at(i) == "wjets_tree.root")
		{
			oneLSRHist->SetLineColor(kBlue);oneLSRHist->SetLineColor(kBlue);backCompFinal->SetLineColor(kBlue);
			legend->AddEntry(oneLSRHist,"W+jets");legend2->AddEntry(oneLSRHist,"W+jets");legend3->AddEntry(backCompFinal,"W+jets");
		}
		if(samples.at(i) == "ttbar_tree.root")
		{
			oneLSRHist->SetLineColor(kRed);oneLSRHist->SetLineColor(kRed);backCompFinal->SetLineColor(kRed);
			legend->AddEntry(oneLSRHist,"t #bar{t}");legend2->AddEntry(oneLSRHist,"t #bar{t}");legend3->AddEntry(backCompFinal,"t #bar{t}");
		}
		if(samples.at(i) == "ttW_tree.root")
		{
			oneLSRHist->SetLineColor(kGreen);oneLSRHist->SetLineColor(kGreen);backCompFinal->SetLineColor(kGreen);
			legend->AddEntry(oneLSRHist,"ttW");legend2->AddEntry(oneLSRHist,"ttW");legend3->AddEntry(backCompFinal,"ttW");
		}
		if(samples.at(i) == "ttZ_tree.root")
		{
			oneLSRHist->SetLineColor(kCyan);oneLSRHist->SetLineColor(kCyan);backCompFinal->SetLineColor(kCyan);
			legend->AddEntry(oneLSRHist,"ttZ");legend2->AddEntry(oneLSRHist,"ttZ");legend2->AddEntry(backCompFinal,"ttZ");
		}
		if(samples.at(i) == "tW_tree.root")
		{
			oneLSRHist->SetLineColor(kOrange);oneLSRHist->SetLineColor(kOrange);backCompFinal->SetLineColor(kOrange);
			legend->AddEntry(oneLSRHist,"tW");legend2->AddEntry(oneLSRHist,"tW");legend3->AddEntry(backCompFinal,"tW");
		}
		if(samples.at(i) == "znunu_tree.root")
		{
			oneLSRHist->SetLineColor(kGreen);oneLSRHist->SetLineColor(kGreen);backCompFinal->SetLineColor(kGreen);
			legend->AddEntry(oneLSRHist,"z->vv");legend2->AddEntry(oneLSRHist,"z->vv");legend3->AddEntry(backCompFinal,"z->vv");
		}
		hists.push_back(oneLSRHist);
		histsSR.push_back(oneLSRHist);
		compHists.push_back(backCompFinal);

		std::cout << "The bin content in the first bin of background comp is " << backCompFinal->GetBinContent(1) << std::endl;
		std::cout << "The bin content in the second bin of background comp is " << backCompFinal->GetBinContent(2) << std::endl;

	}
	for(int i = 0; i < hists.size(); i++){
		if(i == 0)
		hists.at(i)->Draw("ehist");
		if(i > 0){
			hists.at(i)->Draw("histsame");
			hists.at(i)->Draw("esame");
		}
	}
	legend->Draw();	c1->cd();
	c1->SaveAs("run/latestLep/output/binned_composition_1linc.png");	c1->Clear();
	p1= new TPad("p1","p1",0,.1,1,1);p1->SetLeftMargin  (0.15);p1->SetTopMargin   (0.10);p1->SetRightMargin (0.07);p1->SetBottomMargin(0.03);p1->Draw();p1->cd();

	for(int i = 0; i < histsSR.size(); i++){
		if(i == 0)
			histsSR.at(i)->Draw("ehist");
		if(i > 0){
			histsSR.at(i)->Draw("histsame");
			histsSR.at(i)->Draw("esame");
		}
	}
	legend->Draw();	c1->cd();
	c1->SaveAs("run/latestLep/output/binned_composition_LSR.png");	c1->Clear();

	p1= new TPad("p1","p1",0,.1,1,1);p1->SetLeftMargin  (0.15);p1->SetTopMargin   (0.10);p1->SetRightMargin (0.07);p1->SetBottomMargin(0.03);p1->Draw();p1->cd();
	for(int i = 0; i < compHists.size(); i++){
		compHists.at(i)->GetYaxis()->SetRangeUser(0,1);
		if(i == 0)
			compHists.at(i)->Draw("ehist");
		if(i > 0){
			compHists.at(i)->Draw("histsame");
			compHists.at(i)->Draw("esame");
		}

		std::cout << "The bin content in the first bin of background comp is " << compHists.at(i)->GetBinContent(1) << std::endl;
		std::cout << "The bin content in the second bin of background comp is " << compHists.at(i)->GetBinContent(2) << std::endl;

	}
	legend->Draw();	c1->cd();
	c1->SaveAs("run/latestLep/output/bkgCompHists.png");	c1->Clear();

}

TH1F* loadHist(TString varDraw, TString cutString, TString fileName,int nBins, double histBegin, double histFinal, int color)
{
	TString tree = "events";
	//Setup
	TFile *f  = new TFile(fileName);
	TTree *t = (TTree*)f->Get(tree);
	TH1F* h1 = new TH1F("h1","h1",nBins,histBegin,histFinal);
	t->Draw(varDraw+">>h1",scaleName + "*" + Lumi+ "*" + "(" + cutString + ")");
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

TH1F* loadHist(TString varDraw, TString cutString, TString fileName,int nBins, Double_t* bins, int color)
{
	TString tree = "events";
	//Setup
	TFile *f  = new TFile(fileName);
	TTree *t = (TTree*)f->Get(tree);
	TH1F* h1 = new TH1F("h1","h1",nBins,bins);
	t->Draw(varDraw+">>h1",scaleName + "*" + Lumi+ "*" + "(" + cutString + ")");
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

	t1->Draw(varDraw1+">>file1Plot",scaleName + "*" + Lumi + "*" + cutString1);
	t2->Draw(varDraw2+">>file2Plot",scaleName + "*" + Lumi + "*" + cutString2);
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

	t1->Draw(varDraw1+">>file1Plot",scaleName + "*" + Lumi + "*" + cutString1);
	t2->Draw(varDraw2+">>file2Plot",scaleName + "*" + Lumi + "*" + cutString2);
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
