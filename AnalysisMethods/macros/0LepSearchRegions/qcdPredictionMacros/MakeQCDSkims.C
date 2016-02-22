#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/HistogramGetter.hh"
#include "QCDPredictionHelper.h"
#include "TDirectory.h"
#include "TFile.h"

using namespace std;

void go() {

  TString trees[] = {"pieces/htmht_tree",
      "pieces/ttbarplusw_tree",
      "pieces/znunu_tree",
      "pieces/ttZ_tZq",
      "pieces/qcd_tree",
      "pieces/qcd_origtree",
      "pieces/nonQCD_tree",""};



  auto proc = [](TString pre){
    TFile * fI = new TFile(pre +".root","Read");
    TTree *tI =0;
    fI->GetObject("Events",tI);
    tI->SetBranchStatus("*",0);
    tI->SetBranchStatus("bootstrapWeight",1);
    tI->SetBranchStatus("pseudoResp",1);
    tI->SetBranchStatus("pseudoRespCSV",1);
    tI->SetBranchStatus("pseudoRespPassFilter",1);
    tI->SetBranchStatus("trueResp",1);
    tI->SetBranchStatus("trueRespFlv",1);
    tI->SetBranchStatus("weight",1);
    tI->SetBranchStatus("truePUWeight",1);
    tI->SetBranchStatus("btagWeight",1);
    tI->SetBranchStatus("upBTagLightWeight",1);
    tI->SetBranchStatus("upBTagHeavyWeight",1);
    tI->SetBranchStatus("passdijetmet",1);
    tI->SetBranchStatus("passjson",1);
    tI->SetBranchStatus("passeebadscflt",1);
    tI->SetBranchStatus("passhbhefltloose",1);
    tI->SetBranchStatus("passcscbeamhaloflt",1);
    tI->SetBranchStatus("passeebadsc4flt",1);
    tI->SetBranchStatus("passhbheisoflt",1);
    tI->SetBranchStatus("met",1);
    tI->SetBranchStatus("nvetolep",1);
    tI->SetBranchStatus("nvetotau",1);
    tI->SetBranchStatus("ncttstd",1);
    tI->SetBranchStatus("j2pt",1);
    tI->SetBranchStatus("dphij12met",1);
    tI->SetBranchStatus("dphij3met",1);
    tI->SetBranchStatus("dphij4met",1);
    tI->SetBranchStatus("njets",1);
    tI->SetBranchStatus("nbjets",1);
    tI->SetBranchStatus("nlbjets",1);
    tI->SetBranchStatus("mtcsv12met",1);
    tI->SetBranchStatus("mtlepmet",1);
    tI->SetBranchStatus("qcdRespTailWeight",1);
    tI->SetBranchStatus("upTailWeight",1);
    tI->SetBranchStatus("downTailWeight",1);
    tI->SetBranchStatus("leptnpweight",1);
    tI->SetBranchStatus("npromptgentau",1);
    tI->SetBranchStatus("lepvetoweight",1);
    tI->SetBranchStatus("cttWeight",1);
    tI->SetBranchStatus("passaddmetflts",1);

//    tI->SetBranchStatus("ht",1);
//    tI->SetBranchStatus("j1pt",1);
//    tI->SetBranchStatus("j2pt",1);
//    tI->SetBranchStatus("j3pt",1);

    TFile * fO = new TFile(pre +"_skimmed.root","recreate");
    fO->cd();
    TTree * tO = tI->CopyTree(QCDSupport::METPresel);
    tO->Write();
    fO->Close();
  };

  for(unsigned int iT = 0; trees[iT][0]; ++iT)proc(trees[iT]);

  auto proc2 = [](TString pre){
    TFile * fI = new TFile(pre +"_skimmed.root","Read");
    TTree *tI =0;
    fI->GetObject("Events",tI);
    TFile * fO = new TFile(pre +"_skimmed_dphi.root","recreate");
    fO->cd();
    TTree * tO = tI->CopyTree(QCDSupport::ResTailExtraCuts);
    tO->Write();
    fO->Close();
  };

  for(unsigned int iT = 0; trees[iT][0]; ++iT)proc2(trees[iT]);


  auto proc3 = [](TString pre){
    TFile * fI = new TFile(pre +"_skimmed_dphi.root","Read");
    TTree *tI =0;
    fI->GetObject("Events",tI);
    TFile * fO = new TFile(pre +"_skimmed_dphi_split_light_ltp33.root","recreate");
    fO->cd();
    TTree * tO = tI->CopyTree("trueResp < .33 && trueRespFlv !=4");
    tO->Write();
    fO->Close();
    delete fO;
    fO = new TFile(pre +"_skimmed_dphi_split_light_p33top66.root","recreate");
    fO->cd();
    tO = tI->CopyTree("trueResp >= .33 && trueResp < .66 && trueRespFlv !=4");
    tO->Write();
    fO->Close();
    delete fO;
    fO = new TFile(pre +"_skimmed_dphi_split_light_gtp66.root","recreate");
    fO->cd();
    tO = tI->CopyTree("trueResp >= .66 && trueRespFlv !=4");
    tO->Write();
    fO->Close();
    delete fO;
    fO = new TFile(pre +"_skimmed_dphi_split_b_ltp50.root","recreate");
    fO->cd();
    tO = tI->CopyTree("trueResp < .50 && trueRespFlv ==4");
    tO->Write();
    fO->Close();
    delete fO;
    fO = new TFile(pre +"_skimmed_dphi_split_b_gtp50.root","recreate");
    fO->cd();
    tO = tI->CopyTree("trueResp >= .50 && trueRespFlv ==4");
    tO->Write();
    fO->Close();
    fO = new TFile(pre +"_skimmed_dphi_split_b_inc.root","recreate");
    fO->cd();
    tO = tI->CopyTree("trueRespFlv ==4");
    tO->Write();
    fO->Close();
    fO = new TFile(pre +"_skimmed_dphi_split_light_inc.root","recreate");
    fO->cd();
    tO = tI->CopyTree("trueRespFlv !=4");
    tO->Write();
    fO->Close();
    delete fO;
  };
  proc3("pieces/qcd_tree");


  auto proc4 = [](TString pre){
    TFile * fI = new TFile(pre +"_skimmed.root","Read");
    TTree *tI =0;
    fI->GetObject("Events",tI);
    TFile * fO = new TFile(pre +"_skimmed_baseline.root","recreate");
    fO->cd();
    TTree * tO = tI->CopyTree(QCDSupport::BaselineExtraCuts);
    tO->Write();
    fO->Close();
  };

  for(unsigned int iT = 0; trees[iT][0]; ++iT)proc4(trees[iT]);

}

#endif

void MakeQCDSkims(){
  StyleTools::SetStyle();
  go();
}
