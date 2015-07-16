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

   bool initialized = false;
//Histograms for plotting late

   TH1F*  h1;// = new TH1F("h1","h1",6,200.0,800.0);
   TH1F*  h2;//= new TH1F("h2","h2",6,200.0,800.0);

//Rename for tree names (TestAnalyzer is default for trees created by runTestAnalyzer)
   TString  tTree1 = "events";
   TString  tTree2 = "events";
////Rename to your scale factors
   TString scaleName1 = "ScaleFactor";
   TString scaleName2 = "scaleFactor";
   TString Lumi = "4";


   void fillHistograms(TString bins, TString lowVal, TString highVal,TString  file1,TString  file2,TString var1,TString var2, TString cut1, TString cut2);
   bool RatioTCR(
		   	   TString outFile = "run/0L/plots/ratioPlot.png",
		   	   TString cut1 =  "ptMet>200&&nj60>=2&&nJ20>=5&&nmBtag>=1&&mtB01MET>175&&dPhiMET3 > .5 &&  dPhiMET12 > 1 && nmBtag ==1  && NCTTstd == 0",
		   	   TString cut2 = "ptMet>200&&nj60>=2&&nJ20>=5&&nmBtag>=1&&mtB01MET>175&&dPhiMET3 > .5 &&  dPhiMET12 > 1 && nmBtag > 1  && NCTTstd == 0",
			   TString  file1 = "run/0L/ttbar_tree.root", //merged_data_tree_1.root
			   TString  file2 = "run/0L/ttbar_tree.root",
			   TString title = "MET (t#bar{t}),Preselection & N_{t} = 0",
  			   TString  selection1 = "N_{B} = 1",
  			   TString  selection2 = "N_{B} > 1",
  			   TString bins = "6",
 			   TString  lowVal = "200.0",
  			   TString highVal = "800.0",
  			   TString var1 = "MET",
  			   TString var2 = "ptMet",
			   Float_t  bin = 6
   );

   bool RatioTCR(TString outFile,TString cut1,TString cut2,TString  file1,TString  file2,TString title,TString selection1, TString selection2,
		   TString bins,TString  lowVal,TString highVal,
		   TString var1,TString var2,Float_t  bin)
   {
	  ////Filling histograms h1 and h2 (instantiated with Sumw2() before filling)
	   fillHistograms(bins,lowVal,highVal,file1,file2, var1, var2,cut1,cut2);
	   std::cout << "The histograms were filled" << std::endl;
	////Normalizing histograms


   /////Formatting histograms
	   h1->SetStats(false);
	   h2->SetStats(false);
	   double ymax = h1->GetMaximum();
	   if(h2->GetMaximum()>ymax) ymax=h2->GetMaximum();

 	   h1->SetLineWidth(3);
	   h1->SetTitle(title+";;");
	   h2->SetTitle(title+";;");
	   h2->SetLineWidth(3);
	   h2->GetXaxis()->SetTitle("");
	   h2->GetXaxis()->SetLabelOffset(0.20);
	   h2->GetYaxis()->SetTitle("");
	   h1->GetYaxis()->SetRangeUser(0,1.1*ymax);
			 //h1->SetLineColorAlpha(kRed, 0.99);
			// h2->SetLineColorAlpha(kBlue, 0.99);
	   h1->SetLineColor(kRed);
  	   h1->SetMarkerSize(.5);
	   h1->SetMarkerStyle(10);
	   h1->SetMarkerColor(kRed);

	   h2->SetLineColor(kBlue);
  	   h2->SetMarkerSize(.5);
	   h2->SetMarkerStyle(10);
	   h2->SetMarkerColor(kBlue);




	/////Formatting Canvas
	  TCanvas *c1 = new TCanvas("c1","example",600,700);
	  c1->cd();
	  TPad *p1 = new TPad("p1","p1",0,0.3,1,1);
	  p1->SetLeftMargin  (0.18);
	  p1->SetTopMargin   (0.10);
	  p1->SetRightMargin (0.07);
	  p1->SetBottomMargin(0.03);
	  p1->Draw();
	  p1->cd();

	 // c1->SetLogx(fLogx);
	//  c1->SetLogy(fLogy);

  /////Drawing h1 and h2
	//  h1->Draw("");
	  h1->Draw("Ehist");
//	  h2->DrawCopy("E histsame");
	  h2->Draw("Esame");
	  h2->Draw("histsame");


	  ////	Format Legend
	  TLegend *legend=new TLegend(0.6,0.65,0.93,0.85);
	  legend->SetTextFont(72);
	  legend->SetTextSize(0.04);
	  legend->AddEntry(h1,selection1,"l");
	  legend->AddEntry(h2,selection2,"l");
	  legend->Draw();
	  c1->cd();


   //// Clone h2 into h3
	  TH1F* h3 = (TH1F*)h2->Clone("h3"); h3->Divide(h1);	  ymax = h1->GetMaximum()/h2->GetMaximum(); //h3->Divide(h2,h1,1.,1.);
	  h3->SetTitle(selection2+"/"+selection1+";;");
	  if(ymax < h2->GetMaximum()/h1->GetMaximum()) ymax = h2->GetMaximum()/h1->GetMaximum();

  //// Building Second pad
	 TPad *p2 = new TPad("p2","p2",0,0,1,0.3);
	  p2->SetLeftMargin  (0.18);
	  p2->SetTopMargin   (0.00);
	  p2->SetRightMargin (0.07);
	  p2->SetBottomMargin(0.30);
	  p2->SetGridy(1);
	  p2->Draw();
	  p2->cd();

  //// Setup the ratioGram
	  h3->SetLineColor(kBlack);
	  //h1->GetXaxis()->SetTitle(fXTitle);
	  h3->SetTitleSize  (0.12,"Y");
	  h3->SetTitleOffset(0.60,"Y");
	  h3->SetTitleSize  (0.12,"X");
	  h3->SetLabelSize  (0.10,"X");
	  h3->SetLabelSize  (0.08,"Y");
	  h3->GetYaxis()->SetTitleFont(62);
	  h3->GetYaxis()->CenterTitle(kTRUE);
	  h3->GetXaxis()->SetTitleFont(62);
	  h3->GetYaxis()->SetNdivisions(305);
	  //h1->GetXaxis()->SetTitle(fXTitle);
	  h3->GetYaxis()->SetTitle("Ratio");

	  std::cout << "the ymax is now" << ymax << std::endl;
	  h3->GetYaxis()->SetRangeUser(0.0,2*ymax);
 //// Draw the ratioGram
	  h3->Draw("EP");
	  double xmin = h1->GetXaxis()->GetXmin();
	  double xmax = h1->GetXaxis()->GetXmax();
	  TLine *l = new TLine(xmin,1,xmax,1);
	  l->SetLineWidth(3);
	  l->Draw("same");


	  c1->SaveAs(outFile);
   return true;
   }


   void fillHistograms(TString bins,TString lowVal, TString highVal,TString file1, TString file2,TString var1,TString var2,TString cut1,TString cut2)
   {//std::cout << "test a" << std::endl;
   	  std::cout<< "Initizilizing histograms" << std::endl;
   	  if(initialized == true) return;
   	  initialized = true;



      TFile *f1  = new TFile(file1);
   	  TFile *f2  = new TFile(file2);

	  TTree *t1 = (TTree*)f1->Get(tTree1);
	  TTree *t2 = (TTree*)f2->Get(tTree2);
//(" + bins + "," + lowVal + "," + highVal +")"
//(" + bins + "," + lowVal + "," + highVal +")
	   h1 = new TH1F("h1","h1",6,200.0,800.0);
	   h2 = new TH1F("h2","h2",6,200.0,800.0);
	      h1->Sumw2();
	      h2->Sumw2();
	   t1->Draw(var1+">>h1",scaleName1+"*"+Lumi+"*("+cut1+")");
	  // TH1F* h1Temp = (TH1F*)gPad->GetListOfPrimitives()->FindObject("hist1");
	   t2->Draw(var2+">>h2",scaleName2+"*"+Lumi+"*("+cut2+")");
	  // TH1F* h2Temp = (TH1F*)gPad->GetListOfPrimitives()->FindObject("hist2");
	  // h1->Scale(1/h1->Integral(0,(6+1)));

	  // h2->Scale(1/h2->Integral(0,(6+1)));
	   std::cout << "Getting hist entries" << std::endl;
	   std::cout << "Number of entries in hist 1 is " << h1->GetEntries() << std::endl;
	   std::cout << "Number of entries in hist 2 is " << h2->GetEntries() << std::endl;

	   //h1 = h1Temp;
	  // h2 = h2Temp;

   }
