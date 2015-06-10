
//root -l -b -q "yieldPlots2D.C( )"

double getBinNumber(TTree* t_tmp, TString cuts) {

  //TFile* f_tmp = TFile::Open(filename);
  //TTree* t_tmp = (TTree*)f_tmp->Get("events");

  TH1::SetDefaultSumw2(kTRUE);
  TH1D *hTemp = new TH1D("hTemp","hTemp",1,0.,10000.);
  t_tmp->Project("hTemp","ht",cuts);
  double error = 0.;
  double yield = hTemp->IntegralAndError(0,2,error);
  hTemp->Delete();

  return yield;
} // getBinNumber()

void setAxisLables(TH2F* h_tmp) {
  h_tmp->GetXaxis()->SetBinLabel(1,"MT2 200-300");
  h_tmp->GetXaxis()->SetBinLabel(2,"MT2 300-400");
  h_tmp->GetXaxis()->SetBinLabel(3,"MT2 400-500");
  h_tmp->GetXaxis()->SetBinLabel(4,"MT2 500-300");
  h_tmp->GetXaxis()->SetBinLabel(5,"MT2 >=600"  );
  h_tmp->GetYaxis()->SetBinLabel(1,"B=1,T=0");
  h_tmp->GetYaxis()->SetBinLabel(2,"B=1,T>=1");
  h_tmp->GetYaxis()->SetBinLabel(3,"B>=2,T=0");
  h_tmp->GetYaxis()->SetBinLabel(4,"B>=2,T>=1");
  return;
} // setAxisLables()

TFile* f_ttbar      = TFile::Open("ttrees/ttbar_tree.root"          ,"READONLY");
TFile* f_wjets      = TFile::Open("ttrees/wjets_tree.root"          ,"READONLY");
TFile* f_ttw        = TFile::Open("ttrees/ttW_tree.root"            ,"READONLY");
TFile* f_ttz        = TFile::Open("ttrees/ttZ_tree.root"            ,"READONLY");
TFile* f_znunu      = TFile::Open("ttrees/znunu_tree.root"          ,"READONLY");
TFile* f_T2tt850    = TFile::Open("ttrees/T2tt_850_100_tree.root"   ,"READONLY");
TFile* f_T2tt650    = TFile::Open("ttrees/T2tt_650_325_tree.root"   ,"READONLY");
TFile* f_T1tttt1500 = TFile::Open("ttrees/T1tttt_1500_100_tree.root","READONLY");
TFile* f_T1tttt1200 = TFile::Open("ttrees/T1tttt_1200_800_tree.root","READONLY");

TTree* t_ttbar      = (TTree*)f_ttbar     ->Get("events");
TTree* t_wjets      = (TTree*)f_wjets     ->Get("events");
TTree* t_ttw        = (TTree*)f_ttw       ->Get("events");
TTree* t_ttz        = (TTree*)f_ttz       ->Get("events");
TTree* t_znunu      = (TTree*)f_znunu     ->Get("events");
TTree* t_T2tt850    = (TTree*)f_T2tt850   ->Get("events");
TTree* t_T2tt650    = (TTree*)f_T2tt650   ->Get("events");
TTree* t_T1tttt1500 = (TTree*)f_T1tttt1500->Get("events");
TTree* t_T1tttt1200 = (TTree*)f_T1tttt1200->Get("events");

void createPlots( TString preselection, TString saveDir="plots/2Dyields/" ) {

  int nBinsX = 5;
  vector<TString> binCutsX(nBinsX,"");
  binCutsX[0] = ("&&MT2tp0_000>=200&&MT2tp0_000<300"); // MT2tp0_000
  binCutsX[1] = ("&&MT2tp0_000>=300&&MT2tp0_000<400"); // ptMet
  binCutsX[2] = ("&&MT2tp0_000>=400&&MT2tp0_000<500");
  binCutsX[3] = ("&&MT2tp0_000>=500&&MT2tp0_000<600");
  binCutsX[4] = ("&&MT2tp0_000>=600");

  int nBinsY = 4;
  vector<TString> binCutsY(nBinsY,"");
  binCutsY[0] = ("&&nmBtag==1&&NCTTstd==0");
  binCutsY[1] = ("&&nmBtag==1&&NCTTstd>=1");
  binCutsY[2] = ("&&nmBtag>=2&&NCTTstd==0");
  binCutsY[3] = ("&&nmBtag>=2&&NCTTstd>=1");

  TH2F* h_ttbar      = new TH2F("ttbar"      , "ttbar"       , nBinsX,1,1+nBinsX, nBinsY,1,1+nBinsY); // ttbar, w, ttw
  TH2F* h_znunu      = new TH2F("znunu"      , "znunu"       , nBinsX,1,1+nBinsX, nBinsY,1,1+nBinsY);
  TH2F* h_ttz        = new TH2F("ttz"        , "ttz"         , nBinsX,1,1+nBinsX, nBinsY,1,1+nBinsY);
  TH2F* h_bkg        = new TH2F("bkg"        , "bkg"         , nBinsX,1,1+nBinsX, nBinsY,1,1+nBinsY);
  TH2F* h_T2tt850    = new TH2F("T2tt850"    , "T2tt850"     , nBinsX,1,1+nBinsX, nBinsY,1,1+nBinsY);
  TH2F* h_T2tt650    = new TH2F("T2tt650"    , "T2tt650"     , nBinsX,1,1+nBinsX, nBinsY,1,1+nBinsY);
  TH2F* h_T1tttt1500 = new TH2F("T1tttt1500" , "T1tttt1500"  , nBinsX,1,1+nBinsX, nBinsY,1,1+nBinsY);
  TH2F* h_T1tttt1200 = new TH2F("T1tttt1200" , "T1tttt1200"  , nBinsX,1,1+nBinsX, nBinsY,1,1+nBinsY);

  setAxisLables(h_ttbar     );
  setAxisLables(h_znunu     );
  setAxisLables(h_ttz       );
  setAxisLables(h_bkg       );
  setAxisLables(h_T2tt850   );
  setAxisLables(h_T2tt650   );
  setAxisLables(h_T1tttt1500);
  setAxisLables(h_T1tttt1200);

  for(int iX=1; iX<=nBinsX; ++iX) {
    for(int iY=1; iY<=nBinsY; ++iY) {

      TString scaleCuts = "(5*scaleFactor)*("+preselection+binCutsX[iX-1]+binCutsY[iY-1]+")";
      cout << "scaleCuts = " << scaleCuts << '\n' << endl;
      double n_ttbar = getBinNumber(t_ttbar ,scaleCuts);
      double n_wjets = getBinNumber(t_wjets ,scaleCuts);
      double n_ttw   = getBinNumber(t_ttw   ,scaleCuts);
      double n_ttz   = getBinNumber(t_ttz   ,scaleCuts);
      double n_znunu = getBinNumber(t_znunu ,scaleCuts);
      double n_T2tt850    = getBinNumber(t_T2tt850    ,scaleCuts);
      double n_T2tt650    = getBinNumber(t_T2tt650    ,scaleCuts);
      double n_T1tttt1500 = getBinNumber(t_T1tttt1500 ,scaleCuts);
      double n_T1tttt1200 = getBinNumber(t_T1tttt1200 ,scaleCuts);

      h_ttbar     ->SetBinContent( iX, iY, n_ttbar+n_wjets+n_ttw               );
      h_znunu     ->SetBinContent( iX, iY, n_znunu                             );
      h_ttz       ->SetBinContent( iX, iY, n_ttz                               );
      h_bkg       ->SetBinContent( iX, iY, n_ttbar+n_wjets+n_ttw+n_ttz+n_znunu );
      h_T2tt850   ->SetBinContent( iX, iY, n_T2tt850                           );
      h_T2tt650   ->SetBinContent( iX, iY, n_T2tt650                           );
      h_T1tttt1500->SetBinContent( iX, iY, n_T1tttt1500                        );
      h_T1tttt1200->SetBinContent( iX, iY, n_T1tttt1200                        );



    } // iY<nBinsY
  } // iX<nBinsX

  gStyle->SetOptStat(0);

  TCanvas * c_ttbar      = new TCanvas( "cttbar",      "cttbar",      1000, 1000 ); c_ttbar->cd();
  h_ttbar->Draw("colztext");
  c_ttbar->SaveAs(saveDir+"ttbar.pdf");

  TCanvas * c_znunu      = new TCanvas( "cznunu",      "cznunu",      1000, 1000 ); c_znunu->cd(); //gStyle->SetOptStat(0);
  h_znunu->Draw("colztext");
  c_znunu->SaveAs(saveDir+"znunu.pdf");

  TCanvas * c_ttz        = new TCanvas( "cttz",        "cttz",        1000, 1000 ); c_ttz->cd();   //gStyle->SetOptStat(0);
  h_ttz->Draw("colztext");
  c_ttz->SaveAs(saveDir+"ttz.pdf");

  TCanvas * c_bkg        = new TCanvas( "cbkg",        "cbkg",        1000, 1000 ); c_bkg->cd();// gStyle->SetOptStat(0);
  h_bkg->Draw("colztext");
  c_bkg->SaveAs(saveDir+"bkg.pdf");

  TCanvas * c_T2tt850    = new TCanvas( "cT2tt850",    "cT2tt850",    1000, 1000 ); c_T2tt850->cd(); //gStyle->SetOptStat(0);
  h_T2tt850->Draw("colztext");
  c_T2tt850->SaveAs(saveDir+"T2tt850.pdf");

  TCanvas * c_T2tt650    = new TCanvas( "cT2tt650",    "cT2tt650",    1000, 1000 ); c_T2tt650->cd();// gStyle->SetOptStat(0);
  h_T2tt650->Draw("colztext");
  c_T2tt650->SaveAs(saveDir+"T2tt650.pdf");

  TCanvas * c_T1tttt1500 = new TCanvas( "cT1tttt1500", "cT1tttt1500", 1000, 1000 ); c_T1tttt1500->cd();// gStyle->SetOptStat(0);
  h_T1tttt1500->Draw("colztext");
  c_T1tttt1500->SaveAs(saveDir+"T1tttt1500.pdf");

  TCanvas * c_T1tttt1200 = new TCanvas( "cT1tttt1200", "cT1tttt1200", 1000, 1000 ); c_T1tttt1200->cd();// gStyle->SetOptStat(0);
  h_T1tttt1200->Draw("colztext");
  c_T1tttt1200->SaveAs(saveDir+"T1tttt1200.pdf");


  return;
} // createPlots()

void yieldPlots2D() {

  //createPlots("ptMet>200&&nj60>=2&&nJ20>=5&&nmBtag>=1&&mtB01MET>175&&(DphiTopMET>1||DphiTopMET<-1)&&(DphiTopMET<2.8&&DphiTopMET>-2.8)");
  //createPlots("ptMet>200&&nj60>=2&&nJ20>=5&&nmBtag>=1&&mtB01MET>175");

  //createPlots("ptMet>200&&nj60>=2&&nJ20>=5&&nmBtag>=1&&mtB01MET>175"); // case 0
  //createPlots("ptMet>200&&nj60>=2&&nJ20>=5&&nmBtag>=1&&mtB01MET>175&&(DphiTopMET[0]<2.8&&DphiTopMET[0]>-2.8)"); // case a
  //createPlots("ptMet>200&&nj60>=2&&nJ20>=5&&nmBtag>=1&&mtB01MET>175&&(DphiTopMET[1]<2.8&&DphiTopMET[1]>-2.8)"); // case b
  //createPlots("ptMet>200&&nj60>=2&&nJ20>=5&&nmBtag>=1&&mtB01MET>175&&(DphiTopMET[2]<2.8&&DphiTopMET[2]>-2.8)"); // case c
  //createPlots("ptMet>200&&nj60>=2&&nJ20>=5&&nmBtag>=1&&mtB01MET>175&&(DphiTopMET[0]<2.8&&DphiTopMET[0]>-2.8)&&(DphiTopMET[1]<2.8&&DphiTopMET[1]>-2.8)&&(DphiTopMET[2]<2.8&&DphiTopMET[2]>-2.8)"); // case d
  //createPlots("ptMet>200&&nj60>=2&&nJ20>=5&&nmBtag>=1&&mtB01MET>175&&(DphiTopMET<2.8&&DphiTopMET>-2.8)"); // case e
  //createPlots("ptMet>200&&nj60>=2&&nJ20>=5&&nmBtag>=1&&mtB01MET>175&&((DphiTopMET[0]<2.8&&DphiTopMET[0]>-2.8)||(DphiTopMET[1]<2.8&&DphiTopMET[1]>-2.8)||(DphiTopMET[2]<2.8&&DphiTopMET[2]>-2.8))"); // case f

  //createPlots("ptMet>200&&nj60>=2&&nJ20>=5&&nmBtag>=1&&mtB01MET>175&&(DphiTopMET<2.8&&DphiTopMET>-2.8)","plots/2Dyields/bin-met_dphiTopMet/");
  //createPlots("ptMet>200&&nj60>=2&&nJ20>=5&&nmBtag>=1&&mtB01MET>175&&(DphiTopMET[0]<2.8&&DphiTopMET[0]>-2.8)","plots/2Dyields/bin-met_dphiTopMet0/");
  //createPlots("ptMet>200&&nj60>=2&&nJ20>=5&&nmBtag>=1&&mtB01MET>175","plots/2Dyields/bin-met_No-dphiTopMet/");

  createPlots("ptMet>200&&nj60>=2&&nJ20>=5&&nmBtag>=1&&mtB01MET>175&&(DphiTopMET[0]<2.8&&DphiTopMET[0]>-2.8)&&(DphiTopMET[1]<2.8&&DphiTopMET[1]>-2.8)&&(DphiTopMET[2]<2.8&&DphiTopMET[2]>-2.8)","plots/2Dyields/bin-mt2_dphiTopMetAnd/");
  //createPlots("ptMet>200&&nj60>=2&&nJ20>=5&&nmBtag>=1&&mtB01MET>175&&((DphiTopMET[0]<2.8&&DphiTopMET[0]>-2.8)||(DphiTopMET[1]<2.8&&DphiTopMET[1]>-2.8)||(DphiTopMET[2]<2.8&&DphiTopMET[2]>-2.8))","plots/2Dyields/bin-mt2_dphiTopMetOr/");

} // yieldPlots2D()

