#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/HistogramGetter.hh"
#include "QCDPredictionHelper.h"
#include "TDirectory.h"
#include "TFile.h"

using namespace std;

const TString originalTreeDir = "../../run/plots_16_09_28_Smear"; 

void go() {
  gSystem->mkdir(QCDSupport::inputDir, true);
  TString trees[] = {
                     "/qcd_smeared_tree",
                     "/met_tree",
                     "/nonQCD_tree",
                     "/remaining_nonQCD_tree",
                     "/qcd_orig_tree",
                     "/ttbar-mg_tree",
                     "/ttbarplusw_tree",
                     "/wjets-ht_tree",
                     "/znunu_tree",
//                     "/tZq_tree",
                     "/ttZ_tree",
                     "/tW_tree",
                     "/ttW_tree",
//                     "/ttZ_tZq_tree",
//                     "/T2tt_425_325_tree",
//                     "/T2tt_500_325_tree",
//                     "/T2tt_850_100_tree",
                     ""};

  auto proc_1 = [](TString pre){
    TFile * fI = new TFile(originalTreeDir + pre +".root","Read");
    TTree *tI =0;
    fI->GetObject("Events",tI);
    tI->SetBranchStatus("*"                      , 0);
    tI->SetBranchStatus("run"                    , 1);
    tI->SetBranchStatus("lumi"                   , 1);
    tI->SetBranchStatus("event"                  , 1);
    tI->SetBranchStatus("bootstrapWeight"        , 1);
    tI->SetBranchStatus("pseudoResp"             , 1);
//    tI->SetBranchStatus("pseudoRespGenInd"       , 1);
//    tI->SetBranchStatus("pseudoRespRecoInd"      , 1);
//    tI->SetBranchStatus("pseudoRespPT"           , 1);
    tI->SetBranchStatus("pseudoRespCSV"          , 1);
    tI->SetBranchStatus("pseudoRespPseudoGenPT"  , 1);
//    tI->SetBranchStatus("pseudoRespPseudoGenHT"  , 1);
    tI->SetBranchStatus("pseudoRespPassFilter"   , 1);
    tI->SetBranchStatus("trueResp"               , 1);
//    tI->SetBranchStatus("trueRespPT"             , 1);
    tI->SetBranchStatus("trueRespGenPT"          , 1);
//    tI->SetBranchStatus("trueRespGenInd"         , 1);
//    tI->SetBranchStatus("trueRespRecoInd"        , 1);
    tI->SetBranchStatus("trueRespFlv"            , 1);
    tI->SetBranchStatus("weight"                 , 1);
    tI->SetBranchStatus("truePUWeight"           , 1);
    tI->SetBranchStatus("btagWeight"             , 1);
    tI->SetBranchStatus("upBTagLightWeight"      , 1);
    tI->SetBranchStatus("upBTagHeavyWeight"      , 1);
    tI->SetBranchStatus("cttWeight"              , 1);
    tI->SetBranchStatus("passjson"               , 1);
    tI->SetBranchStatus("passmetfilters"         , 1);
    tI->SetBranchStatus("ht"                     , 1);
    tI->SetBranchStatus("met"                    , 1);
//    tI->SetBranchStatus("mht"                    , 1);
    tI->SetBranchStatus("nvetolep"               , 1);
    tI->SetBranchStatus("nvetotau"               , 1);
    tI->SetBranchStatus("j1pt"                   , 1);
    tI->SetBranchStatus("j2pt"                   , 1);
    tI->SetBranchStatus("njl"                    , 1);
    tI->SetBranchStatus("passmetmht100"          , 1);
//    tI->SetBranchStatus("passtrigel"             , 1);
    tI->SetBranchStatus("passtrigmu"             , 1);
    tI->SetBranchStatus("ismc"                   , 1);
    tI->SetBranchStatus("dphij1lmet"             , 1);
    tI->SetBranchStatus("j1lpt"                  , 1);
    tI->SetBranchStatus("csvj1pt"                , 1);
    tI->SetBranchStatus("csvj2pt"                , 1);
//    tI->SetBranchStatus("nmbjets"                , 1);
//    tI->SetBranchStatus("dphij12met"             , 1);
    tI->SetBranchStatus("dphij1met"              , 1);
    tI->SetBranchStatus("dphij2met"              , 1);
    tI->SetBranchStatus("dphij3met"              , 1);
    tI->SetBranchStatus("dphij4met"              , 1);
    tI->SetBranchStatus("njets"                  , 1);
    tI->SetBranchStatus("nbjets"                 , 1);
    tI->SetBranchStatus("nlbjets"                , 1);
    tI->SetBranchStatus("nsdtoploose"            , 1);
    tI->SetBranchStatus("nsdwloose"              , 1);
    tI->SetBranchStatus("mtcsv12met"             , 1);
    tI->SetBranchStatus("mtlepmet"               , 1);
    tI->SetBranchStatus("qcdRespTailWeight"      , 1);
    tI->SetBranchStatus("upTailWeight"           , 1);
    tI->SetBranchStatus("downTailWeight"         , 1);
    tI->SetBranchStatus("ngoodgenele"            , 1);
    tI->SetBranchStatus("ngoodgenmu"             , 1);
    tI->SetBranchStatus("npromptgentau"          , 1);
    tI->SetBranchStatus("lepvetoweightHM"        , 1);
    tI->SetBranchStatus("leptnpweightHM"         , 1);
    tI->SetBranchStatus("lepvetoweightLM"        , 1);
    tI->SetBranchStatus("leptnpweightLM"         , 1);
//    tI->SetBranchStatus("whichQCD_HT_Sample"     , 1);
//    tI->SetBranchStatus("whichQCD_Sample"        , 1);
    tI->SetBranchStatus("j1chEnFrac"             , 1);

    TFile * fO = new TFile(QCDSupport::inputDir + pre +"_skimmed.root","recreate");
    fO->cd();
    cout << pre << " -> ";
    pre.Contains("qcd") ? cout << TString::Format("%s && %s", QCDSupport::eventsToSkip.Data(), QCDSupport::METPresel.Data()) << endl : cout << QCDSupport::METPresel << endl;
    TTree * tO = tI->CopyTree(pre.Contains("qcd") ? TString::Format("%s && %s", QCDSupport::eventsToSkip.Data(), QCDSupport::METPresel.Data()) : QCDSupport::METPresel);
    tO->Write();
    fO->Close();
  };
cout << "proc_1..." << endl;
  for(unsigned int iT = 0; trees[iT][0]; ++iT)proc_1(trees[iT]);

  auto proc_2 = [](TString pre){
    TFile * fI = new TFile(QCDSupport::inputDir + pre +"_skimmed.root","Read");
    TTree *tI =0;
    fI->GetObject("Events",tI);
    TFile * fO = new TFile(QCDSupport::inputDir + pre +"_skimmed_dphi.root","recreate");
    fO->cd();
    TTree * tO = tI->CopyTree(QCDSupport::ResTailExtraCuts);
    tO->Write();
    fO->Close();
  };
cout << "proc_2..." << endl;
  for(unsigned int iT = 0; trees[iT][0]; ++iT)proc_2(trees[iT]);

//  auto proc_3 = [](TString pre){
//    TFile * fI = new TFile(QCDSupport::inputDir + pre +"_skimmed_dphi.root","Read");
//    TTree *tI =0;
//    fI->GetObject("Events",tI);
//    TFile * fO = new TFile(QCDSupport::inputDir + pre +"_skimmed_dphi_split_light_ltp33.root","recreate");
//    fO->cd();
//    TTree * tO = tI->CopyTree("trueResp < .33 && trueRespFlv !=4");
//    tO->Write();
//    fO->Close();
//    delete fO;
//    TFile * fO = new TFile(QCDSupport::inputDir + pre +"_skimmed_dphi_split_light_p33top66..root","recreate");
//    fO->cd();
//    tO = tI->CopyTree("trueResp >= .33 && trueResp < .66 && trueRespFlv !=4");
//    tO->Write();
//    fO->Close();
//    delete fO;
//    TFile * fO = new TFile(QCDSupport::inputDir + pre +"_skimmed_dphi_split_light_gtp66..root","recreate");
//    fO->cd();
//    tO = tI->CopyTree("trueResp >= .66 && trueRespFlv !=4");
//    tO->Write();
//    fO->Close();
//    delete fO;
//    TFile * fO = new TFile(QCDSupport::inputDir + pre +"_skimmed_dphi_split_b_ltp50..root","recreate");
//    fO->cd();
//    tO = tI->CopyTree("trueResp < .50 && trueRespFlv ==4");
//    tO->Write();
//    fO->Close();
//    delete fO;
//    TFile * fO = new TFile(QCDSupport::inputDir + pre +"_skimmed_dphi_split_b_gtp50..root","recreate");
//    fO->cd();
//    tO = tI->CopyTree("trueResp >= .50 && trueRespFlv ==4");
//    tO->Write();
//    fO->Close();
//    delete fO;
//  };
//cout << "proc_3..." << endl;
//  proc_3(QCDSupport::inputDir + "/qcd_tree");

  auto proc_4 = [](TString pre){
    TFile * fI = new TFile(QCDSupport::inputDir + pre +"_skimmed.root","Read");
    TTree *tI =0;
    fI->GetObject("Events",tI);
    TFile * fO = new TFile(QCDSupport::inputDir + pre +"_skimmed_baseline.root","recreate");
    fO->cd();
    TTree * tO = tI->CopyTree(QCDSupport::BaselineExtraCuts);
    tO->Write();
    fO->Close();
  };
cout << "proc_4..." << endl;
  for(unsigned int iT = 0; trees[iT][0]; ++iT)proc_4(trees[iT]);
}

#endif

void MakeQCDSkims(){
  StyleTools::SetStyle();
  go();
}
