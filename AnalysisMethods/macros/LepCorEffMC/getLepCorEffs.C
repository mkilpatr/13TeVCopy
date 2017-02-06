/*
 * getLepCorEffs.C
 * adapted from mullin, june 2016
 * last updated Feb 2017 for AN-16-437 / SUS-16-049
 *
 * a short macro to compute the MC lepton effs
 * creates pdf/svg/root with 2D id and iso eff plots for electrons and muons
 * can also create many additional plots for studies
 *
 * currently, must make four sets of ttbarplusw trees (id/iso times sr/cr lepton configs). See README.
 *   trees must be in file structure <somewhere>/{id,iso}/{sr,cr}, eg asdf/id/sr/ttbarplusw_tree.root
 * and currently, we split between HM / LM baselines. Two roots for the data directory.
 * see "CHANGEME" locations
 * run with root -l -b getLepCorEffs.C+ with rootlogon.C copied into this directory
 *
 * produces lepCorMCEff_HM.root (or LM) to drop into data dir
 *   contains four 2D (pT,eta) histograms: Id/Iso times SR/CR.
 *
 *
 * ID eff formula:
 *  NOTE: For ID trees the iso cuts are turned off. pt/eta cuts are relaxed to 5/2.4.
 *        The genmueta cut is used because the gen mu might be outside 2.4 but matched to reco mu in 2.4.
 *   denominator = baseline + ngoodgenmu>=1 + abs(genmueta)<2.4
 *   numerator = denominator + ngenmumatched>=1
 *
 *   denominator = baseline + ngoodgenele>=1
 *   numerator = denominator + nvetoele>=1
 *
 * ISO eff formula:
 *  NOTE: For ISO trees the id/iso cuts are untouched. pt/eta cuts are relaxed to 5/2.4
 *   denominator = baseline + 
 *   
 *
 *
 * essential functions:
 *   void plotAll2D( TString histExtraName = "", TString extraCuts = "" )
 *     wrapper for calls to createEff2D
 *   void createEff2D( TString histName, TString varX, TString varY, int nBinsX, int nBinsY, float binsX[], float binsY[], TString extraCuts = "" )
 *     creates and saves 2D eff plots as pdf/svg and histos as roots to be dropped into the data directory.
 *
 */

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TH2F.h>
#include <THStack.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TString.h>
#include <TStyle.h>
#include "TSystem.h"
#include "AnalysisMethods/PlotUtils/interface/StyleTools.hh"
#include "AnalysisMethods/PlotUtils/interface/PlotStuff.h" 

  // settings - subdirectory structure (four sets of trees) {id,iso}/{sr/cr} assumed
  const TString treedir = "/eos/uscms/store/user/apatters/trees/20170205-moriond-lepeff-test/"; // apatters, new FW, jan 29th latest ntuples
  //const TString treedir = "/eos/uscms/store/user/apatters/trees/20170203-ichep/"; // apatters, new FW, ichep ntuples (FAT JETS TURNED OFF)
  const TString outdir = "./lepCorEffs/";
  const TString outHistPrefix = "lepCorMCEff_";
  TFile * fout;

  // these are outdated:
  //const bool isId  = lowerTString(treedir).Contains("id");
  //const bool isSR  = lowerTString(treedir).Contains("sr");
  //TFile* f = TFile::Open(treedir + ((isSR) ? "/sr/" : "/cr/") + "ttbarplusw_tree.root","READONLY");
  //TTree* t = (TTree*)f->Get("Events");

  // ichep16
  const TString passvetoes = " && (nvetotau==0 || (ismc && npromptgentau>0)) && nvetolep==0";
  const TString lepcrsel   = " && nvetolep>0 && mtlepmet<100";
  const TString njets75    = " && j2pt>75";
  const TString j1lpt      = " && j1lpt>250";
  const TString njl        = " && njl>=1";
  const TString nlbjets    = " && nlbjets>=2";
  const TString dphij12met = " && dphij12met>0.5";
  const TString dphij3met  = " && dphij3met>0.5";
  const TString dphij4met  = " && dphij4met>0.5";
  const TString dphij34met = " && dphij3met>0.5 && dphij4met>0.5";
  const TString dphij123   = " && dphij12met>1 && dphij3met>0.5 && dphij4met>0.5";

  // moriond17
  const TString trig       = "passmetfilters";
  const TString met        = " && met>250";
  const TString njets      = " && njets>=5";
  const TString njets2     = " && njets>=2";
  const TString nbjets     = " && nbjets>=1";
  const TString dphij1234met = " && dphij1met>0.5 && dphij2met>0.5 && dphij3met>0.5 && dphij4met>0.5";
  const TString dphiLM       = " && dphij1met>0.5 && dphij2met>0.15 && dphij3met>0.15";
  const TString dphij1lmet   = " && dphij1lmet>2";
  const TString zerotopw     = " && nsdtop==0 && nsdw==0 && nrestop==0";
  const TString isr300       = " && ak8isrpt>300 && dphiisrmet>2";
  const TString metovsqrtht= " && metovsqrtht>10";
  const TString pu10to20 = " && npv>10 && npv<20";

  // ichep16 - CHANGEME - uncomment for ichep16 cuts
  //const TString nearHMbasenodphi   = trig +              met + njets + nlbjets + nbjets; // ICHEP16
  //const TString nearLMbasenodphi   = trig +              met + njets2  + njl + j1lpt + dphij1lmet + metovsqrtht; // ICHEP16

  // moriond17 - CHANGEME - uncomment for moriond17 cuts
  const TString nearHMbasenodphi   = trig + njets + met + nbjets;
  const TString nearLMbasenodphi   = trig + njets2 + met + zerotopw + metovsqrtht + isr300;

  // CHANGEME - change 'weight'
  const TString lumistr  = "36.2";
  const TString mcwgt    = lumistr + "*weight";
  const TString weight   = lumistr + "*weight*truePUWeight*topptWeight*btagWeight*sdMVAWeight*resTopWeight"; // Moriond17
  //const TString weight = lumistr + "*weight*truePUWeight*btagWeight"; // ICHEP16
  //const TString weight = mcwgt;

  // CHANGEME - select LM or HM baseline. used elsewhere.
  bool inHM = true;
  const TString baseline = !inHM ? nearLMbasenodphi : nearHMbasenodphi;

  // CHANGEME - set bins of histograms
  std::vector<float> binsPt       { 5, 10, 20, 30, 40, 50, 60, 80, 120};
  std::vector<float> binsPtEl     { 5, 10, 20, 30, 40, 50,         120};
  std::vector<float> binsPtMu     { 5, 10, 20, 25, 30, 40, 50, 60, 120};
  std::vector<float> binsEtaEl    { -2.4, -1.6, -0.8, 0, 0.8, 1.6, 2.4};
  std::vector<float> binsEtaMu    { 0, 0.9, 1.2,  2.1, 2.4};
  std::vector<float> binsAbsEtaEl { 0, 0.8, 1.45, 2.0, 2.4};
  std::vector<float> binsAbsEtaMu { 0, 0.9, 1.2, 2.1, 2.4};
  std::vector<float> binsAn       { 0, 0.02, 0.05, 0.15, 1.0, 1.5};
  std::vector<float> binsHt       { 0, 80, 150, 250, 400, 600, 1200, 2000};
  std::vector<float> binsMet      { 250, 300, 400, 500, 600, 700};
  int nBinsPt        = binsPt      .size()-1;
  int nBinsPtEl      = binsPtEl    .size()-1;
  int nBinsPtMu      = binsPtMu    .size()-1;
  int nBinsEtaEl     = binsEtaEl   .size()-1;
  int nBinsEtaMu     = binsEtaMu   .size()-1;
  int nBinsAbsEtaEl  = binsAbsEtaEl.size()-1;
  int nBinsAbsEtaMu  = binsAbsEtaMu.size()-1;
  int nBinsAn        = binsAn      .size()-1;
  int nBinsMet       = binsMet     .size()-1;

//////////////////////////////////////////////////////////////////////////////////////////
//
// misc accessory functions
//////////////////////////////////////////////////////////////////////////////////////////
// sorely needed in TString class. Only has 'void ToLower()', nothing acting on consts.
TString lowerTString(const TString inString) {
  TString lowered = inString;
  lowered.ToLower();
  return lowered;
}

/*
 * Adds either the underflow (overflow) bin to the first (last) bin of a 1D
 * histogram, also recomputing the error.
 */
void addBin(TH1F* h, int bin){
  int b1 = bin;
  int b2 = bin==1 ? 0 : bin+1;
  float v1 = h->GetBinContent(b1);
  float v2 = h->GetBinContent(b2);
  float e1 = h->GetBinError  (b1);
  float e2 = h->GetBinError  (b2);
  float v = v1 + v2;
  float e = sqrt(e1*e1+e2*e2);
  h->SetBinContent(bin,v);
  h->SetBinError  (bin,e);
} // addBin()

/*
 * Adds the overflow bin to the last bin of a 2D histogram for the given Y
 * row, also recomputing the error.
 */
void addBinX(TH2F* h, int binX, int binY){
  float v1 = h->GetBinContent(binX,binY);
  float v2 = h->GetBinContent(binX,binY+1);
  float e1 = h->GetBinError  (binX,binY);
  float e2 = h->GetBinError  (binX,binY+1);
  float v = v1 + v2;
  float e = sqrt(e1*e1+e2*e2);
  h->SetBinContent(binX,binY,v);
  h->SetBinError  (binX,binY,e);
} // addBinX()

/*
 * Adds the overflow bin to the last bin of a 2D histogram for the given X
 * row, also recomputing the error.
 */
void addBinY(TH2F* h, int binX, int binY){
  float v1 = h->GetBinContent(binX  ,binY);
  float v2 = h->GetBinContent(binX+1,binY);
  float e1 = h->GetBinError  (binX  ,binY);
  float e2 = h->GetBinError  (binX+1,binY);
  float v = v1 + v2;
  float e = sqrt(e1*e1+e2*e2);
  h->SetBinContent(binX,binY,v);
  h->SetBinError  (binX,binY,e);
} // addBinY()

void addOverUnderFlow(TH1F* h, bool addUnderflow=false, bool addOverflow=true){
  if(addOverflow ) addBin(h,h->GetNbinsX());
  if(addUnderflow) addBin(h,1);
} // addOverUnderFlow()

void bringOflowIntoLastBins(TH2F* h){
  int nx = h->GetNbinsX();
  int ny = h->GetNbinsY();
  for(int ix=1; ix<=nx; ++ix){
    addBinX(h,ix,ny);
  } // nx
  for(int iy=0; iy<=ny; ++iy){
    addBinY(h,nx,iy);
  } // ny
} // addOverUnderFlow

void setOflowToLastBin(TH2F* h) {
  int nBinsX = h->GetNbinsX();
  int nBinsY = h->GetNbinsY();
  for(int x=1; x<=nBinsX; ++x) {
    h->SetBinContent(x,0       ,h->GetBinContent(x,1     ));
    h->SetBinContent(x,nBinsY+1,h->GetBinContent(x,nBinsY));
  }
  for(int y=0; y<=nBinsY+1; ++y) {
    h->SetBinContent(0       ,y,h->GetBinContent(1     ,y));
    h->SetBinContent(nBinsX+1,y,h->GetBinContent(nBinsX,y));
  }
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// createEff2D
//   meat of the calculation. writes 2d eff plots as svg, pdf, and root for data directory.
//
//   sam cut differently on the first particle (say, recoel or genmu) in the event than
//   the others. awaiting explanation or if it was a bug. this code breaks up the iterator
//   explicitely to reproduce exactly Sam's code for now.
//
//   makes use of root iterator semantics for Project() (same for Draw etc). reference:
//     "Special functions and variables", see Iteration$ and Alt$ especially.
//     https://root.cern.ch/doc/master/classTTree.html#TTree:Draw
//
// explanation of iterator semantics used:
//   we Project() the tree using a few (scalar for ele, vector for mu) cutstrings onto
//   a 2D variable, where each dimension is a vector quantity (ie multiple entries per event). 
//   root handles this by assuming that wherever in the (2D) variable or cutstring there is
//   a vector quantity, it refers to the same particle, and thus we need only one iterator.
//   for example, using the notation Project(variable, cutstring), and vx, vy vector vars,
//       t->Project("hist", vx:vy, scalar cutstring)
//   is interpreted as (maybe it's Draw or something else, I don't know):
//       hist = t->Draw(vx[0]:vy[0], sc. cut) + t->Draw(vx[1]:vy[1], sc. cut) + ...
//   I've tested this very much and matched exactly the results with Sam's old code, which
//   calculated each of these histograms ([i]:[i] for i from 0 to 10) explicitely and summed.
//   
//   if we're doing id for muons, we want also the gen muon's abs(eta) < 2.4, which is done
//   by adding it to the cutstring. but this is a vector quantity, so we now have also an
//   iterator there:
//       (vx:vy, vector cutstring) = (vx[0]:vy[0], cutstring[0]) + same for [1] + ...
//   the index is plugged into whichever vector variables are in cutstring -- for us this
//   may (if Id and muons) be only genmueta.
//  
//   key idea: the iterator is an index into the vector of particles in the event, say
//     reco ele or gen mu, whose variables are accessed as genmupt[i], genmueta[i], etc.
//     just makes sure you don't pass nonsense like varX = genmupt, varY = recomupt into
//     this function, because those two things don't refer to the same particles.
//
//////////////////////////////////////////////////////////////////////////////////////////
void createEff2D( TTree* t, bool isId, bool isSR, TString outPlotFileName, 
    TString varX, TString varY, int nBinsX, int nBinsY, float binsX[], float binsY[], TString extraCuts="") {

  //bool isMu = lowerTString(outPlotFileName).Contains("mu"); // old method
  bool isMu = (lowerTString(varX).Contains("mu") || lowerTString(varY).Contains("mu")); // easier for user

  // histos for zero (z) and greater-than-zero (gz) particle index
  TH2F* h_den  = new TH2F("h_den", "h_den", nBinsX,binsX,nBinsY,binsY);
  TH2F* h_num  = new TH2F("h_num", "h_num", nBinsX,binsX,nBinsY,binsY);

  // form 2D projection variable (histogram axes). vector brackets [] are optional in root.
  TString var2d = "abs("+varY+"):abs("+varX+")";
  //TString var2d = "abs("+varY[0]+"):abs("+varX[0]+")"; // to use only hardest (index 0) lep in each event. may use [1], [2], etc.
  std::cout << "plotting variable: " << var2d << std::endl;

  // extra requirement for Id && Muons case: abs(genmueta)<2.4. iterator same?
  TString c_muidgeneta = (isId) ? " && abs(genmueta[Iteration$])<2.4" : "";

  // to change whether you use only the first or further leps in each event:
  //   see commented-out var2d options
  // to add additional cuts:
  //   change the c_Xextra cutstrings below if you can, this will reduce errors.
  TString c_denmuextra = "";
  TString c_denelextra = "";
  TString c_nummuextra = "";
  TString c_numelextra = "";
  TString c_base  = weight  + "*(" + baseline + extraCuts;
  TString c_denmu = c_base  + c_denmuextra + " && ngoodgenmu>=1" + c_muidgeneta;
  TString c_nummu = c_denmu + c_nummuextra + ((isId) ? " && ngenmumatched>=1" : " && nvetomu>=1");
  TString c_denel = c_base  + c_denelextra + " && ngoodgenele>=1";
  TString c_numel = c_denel + c_numelextra + " && nvetoele>=1";

  std::cout << "denominator cutstring: " << ((isMu) ? c_denmu : c_denel) + ")" << std::endl;
  std::cout << "numerator   cutstring: " << ((isMu) ? c_nummu : c_numel) + ")" << std::endl;
  std::cout << "denominator entries: " << t->Project("h_den", var2d, ((isMu) ? c_denmu : c_denel) + ")" ) << std::endl;
  std::cout << "numerator   entries: " << t->Project("h_num", var2d, ((isMu) ? c_nummu : c_numel) + ")" ) << std::endl;

  // make last bin represent indefinite range [X,inf] by bringing oflow events into last bin of 
  // numerator and denominator. one more step below after we divide to get the eff.
  bringOflowIntoLastBins(h_den);
  bringOflowIntoLastBins(h_num);

  // calculate eff histo
  TString histName = outHistPrefix;
  TString lepTypeRegSuffix  = (isMu) ? "Mu" : "El";
  lepTypeRegSuffix += (isId) ? "_Id" : "_Iso";
  TH2F* h_eff = (TH2F*)h_num->Clone(histName+lepTypeRegSuffix); h_eff->Divide(h_num,h_den,1.0,1.0,"B");
  lepTypeRegSuffix += (isSR) ? "_SR" : "_CR";

  // now set the oflow bins (overwriting them) equal to the last bins. thus accessing with pt > max pt returns eff(max pt) correctly.
  // eg for some pt bins with effs (5,3,2,1.5, oflow=0.5) -> (5,3,2,1.5,1.5).
  // the oflow events were taken into account earlier with bringOflowIntoLastBins.
  setOflowToLastBin(h_eff);


  // set __plotting__ min/max to produce consistent "colz" plots
  h_eff->SetMaximum(1.0);
  if( isMu ){
    if(isId){
      h_eff->SetMinimum(0.60);
    }else{
      h_eff->SetMinimum(0.70);
    }
  }else{
    if(isId){
      h_eff->SetMinimum(0.30);
    }else{
      h_eff->SetMinimum(0.50);
    }
  }
  // save histo to root file. will be used in data directory
//  TString outRootFileName = outdir + outHistPrefix + lepTypeRegSuffix + ".root";
//  std::cout << "Saving data root to : " << outRootFileName << std::endl;
//  TFile* f = new TFile(outRootFileName,"RECREATE");
  fout->cd();
  h_eff->Write(histName+lepTypeRegSuffix);
  //fout->Write();
//  f->Write();
//  delete f;

  // prettify histo for pdf/svg plots
  StyleTools::SetStyle ();
  TCanvas* c = StyleTools::MakeCanvas( "c", "MC Efficiency", 1000, 1000 );
  c->SetRightMargin (0.15);
  c->SetLeftMargin  (0.15);
  c->SetTopMargin   (0.12);
  c->SetBottomMargin(0.12);
  if(varY.Contains("annulus")) gPad->SetLogy();

  if( isMu ){
    h_eff->SetXTitle( (isId) ? "Gen Muon p_{T} [GeV]" : "Muon p_{T} [GeV]"          );
    h_eff->SetYTitle( (isId) ? "Gen Muon |#eta|"      : "Gen Muon |#eta|" ); // was: Relative Annulus Activity
  }else{
    h_eff->SetXTitle( "Electron p_{T} [GeV]" );
    h_eff->SetYTitle( (isId) ? "Electron |#eta|" : "Electron |#eta|" ); // was: Relative Annulus Activity
  }

  // save plots
  StyleTools::InitHist(h_eff,h_eff->GetXaxis()->GetTitle(),h_eff->GetYaxis()->GetTitle());
  //h_eff->SetTitle( (varX.Contains("ele")) ? "Electron MC Efficiency" : "Muon MC Efficiency" );
  h_eff->SetTitleSize(0.035,"X");
  h_eff->SetTitleSize(0.035,"Y");
  h_eff->Draw("colz"); // colztexte
  c->SaveAs(outdir+outPlotFileName+lepTypeRegSuffix+".svg");
  c->SaveAs(outdir+outPlotFileName+lepTypeRegSuffix+".pdf");

  delete c;
  delete h_eff;
  delete h_den;
  delete h_num;

  return;
} // createEff2D


//////////////////////////////////////////////////////////////////////////////////////////
//
// Use no arguments and only uncomment the last two lines (pt vs annulus) for actual effs. Will create
// other plots (using extraOutHistFileName and extraCuts) under "plotEffs/" for other studies.
//
// WARNING: creating other plots will ALSO overwrite the eff histograms, so either move them first
// or plan to create them last.
// 
//////////////////////////////////////////////////////////////////////////////////////////
void plotAll2D(TTree* t, bool isId, bool isSR, TString extraFileName="", TString extraCuts="") {
  if(isId){
    createEff2D( t, isId, isSR, "2D_pt_eta_"    +extraFileName, "idisoincALelept"  , "idisoincALeleeta"       , nBinsPtEl, nBinsAbsEtaEl, &binsPtEl[0], &binsAbsEtaEl[0], extraCuts );
    createEff2D( t, isId, isSR, "2D_pt_eta_"    +extraFileName, "genmupt", "genmueta"     , nBinsPtMu, nBinsAbsEtaMu, &binsPtMu[0], &binsAbsEtaMu[0], extraCuts );
  }else{ //Iso
    createEff2D( t, isId, isSR, "2D_pt_eta_"    +extraFileName, "isoincALelept"  , "isoincALeleeta"                  , nBinsPtEl, nBinsAbsEtaEl, &binsPtEl[0], &binsAbsEtaEl[0], extraCuts );
    createEff2D( t, isId, isSR, "2D_pt_eta_"    +extraFileName, "isoincALmupt"  , "isoincALmueta"                  , nBinsPtMu, nBinsAbsEtaMu, &binsPtMu[0], &binsAbsEtaMu[0], extraCuts );
    //createEff2D( t, isId, isSR, "2D_pt_annulus_"+extraFileName, "isoincALelept"  , "isoincALeleannulus/isoincALelept", nBinsPtEl, nBinsAn      , &binsPtEl[0], &binsAn[0]      , extraCuts ); 
    //createEff2D( t, isId, isSR, "2D_pt_annulus_"+extraFileName, "isoincALmupt"   , "isoincALmuannulus/isoincALmupt"  , nBinsPtMu, nBinsAn      , &binsPtMu[0], &binsAn[0]      , extraCuts );
  }

  // not currently used
  //createEff2D( t, isId, isSR, "2D_pt_htalong_"+extraFileName, "elept", "elehtalong", nBinsPt, nBinsHt,  binsPt, binsHt   , extraCuts );
  //createEff2D( t, isId, isSR, "2D_pt_htalong_"+extraFileName, "mupt" , "muhtalong" , nBinsPt, nBinsHt,  binsPt, binsHt   , extraCuts );
  //createEff2D( t, isId, isSR, "2D_pt_eta_"    +extraFileName, "mupt" , "mueta"     , nBinsPt, nBinsAbsEtaMu, binsPt, binsAbsEtaMu, extraCuts );
}

#endif
//////////////////////////////////////////////////////////////////////////////////////////
//
//              
//////////////////////////////////////////////////////////////////////////////////////////
void getLepCorEffs() {

  TH1::SetDefaultSumw2(kTRUE);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(1);
  gStyle->SetPalette(1);
  gSystem->mkdir(outdir, true);

  TString outRootFileName = outdir + outHistPrefix + (inHM ? "HM" : "LM") + ".root";
  std::cout << "Saving MC eff root to : " << outRootFileName << std::endl;
  fout = new TFile(outRootFileName,"RECREATE");

  for(unsigned int i = 0; i <= 1; ++i) {
    bool isSR  = (bool)i;
    for(unsigned int j = 0; j <= 1; ++j) {
      bool isId  = (bool)j;
      //isId = 1; isSR = 1; //hack if want to test one region quickly
      TFile* f = TFile::Open(treedir + "/" + ((isId) ? "id" : "iso") + "/" + ((isSR) ? "sr" : "cr") + "/" + "ttbarplusw_tree.root","READONLY");
      if (f->IsZombie()) {
        std::cout << "Error opening file" << std::endl;
        exit(-1);
      }
      TTree* t = (TTree*)f->Get("Events");
      std::cout << std::endl << "Handling : " << ((isId) ? "Id" : "Iso") << " effs for the " << ((isSR) ? "SR (veto) region lepton WPs" : "CR lepton WPs") << std::endl;
      std::cout << "      using tree " << treedir + "/" + ((isId) ? "id" : "iso") + "/" + ((isSR) ? "sr" : "cr") + "/" + "ttbarplusw_tree.root" << std::endl;

      // make the 2D eff histos in pdf/png/root format
      plotAll2D(t, isId, isSR);

      f->Close();
      //break; // see previous hack
    }
  }
  fout->Close();
  return;
}
