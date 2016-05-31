#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/HistogramGetter.hh"
#include "QCDPredictionHelper.h"
#include "TDirectory.h"
#include "TFile.h"

using namespace std;

const TString originalTreeDir = "/eos/uscms//store/user/jzabel/13TeV/T2FBD_Trees";

void go() {
  TString trees[] = {
//                     "/qcd_tree",
//                     "/qcd_puWeight_tree",
//                     "/qcd_puWeight_weight_tree",
//                     "/met_tree",
//                     "/nonQCD_tree",
//                     "/qcd_orig_combined_tree",
//                     "/qcd_orig_tree",
                     "/qcd_orig_plus_tree",
//                     "/ttbar-mg_tree",
//                     "/ttbarplusw_tree",
//                     "/wjets-ht_tree",
//                     "/znunu_tree",
//                     "/tZq_tree",
//                     "/ttZ_tree",
//                     "/tW_tree",
//                     "/ttW_tree",
//                     "/ttZ_tZq_tree",
//                     "/T2tt_375_295_tree",
//                     "/T2tt_375_325_tree",
//                     "/T2tt_375_355_tree",
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
    tI->SetBranchStatus("pseudoRespGenInd"       , 1);
    tI->SetBranchStatus("pseudoRespRecoInd"      , 1);
    tI->SetBranchStatus("pseudoRespPT"           , 1);
    tI->SetBranchStatus("pseudoRespCSV"          , 1);
    tI->SetBranchStatus("pseudoRespPseudoGenPT"  , 1);
    tI->SetBranchStatus("pseudoRespPseudoGenHT"  , 1);
    tI->SetBranchStatus("pseudoRespPassFilter"   , 1);
    tI->SetBranchStatus("pseudoResp_4"           , 1);
    tI->SetBranchStatus("pseudoRespGenInd_4"     , 1);
    tI->SetBranchStatus("pseudoRespRecoInd_4"    , 1);
    tI->SetBranchStatus("pseudoRespPT_4"         , 1);
    tI->SetBranchStatus("pseudoRespCSV_4"        , 1);
    tI->SetBranchStatus("pseudoRespPseudoGenPT_4", 1);
    tI->SetBranchStatus("pseudoRespPseudoGenHT_4", 1);
    tI->SetBranchStatus("pseudoRespPassFilter_4" , 1);
    tI->SetBranchStatus("trueResp"               , 1);
    tI->SetBranchStatus("trueRespPT"             , 1);
    tI->SetBranchStatus("trueRespGenPT"          , 1);
    tI->SetBranchStatus("trueRespGenInd"         , 1);
    tI->SetBranchStatus("trueRespRecoInd"        , 1);
    tI->SetBranchStatus("trueRespFlv"            , 1);
    tI->SetBranchStatus("weight"                 , 1);
    tI->SetBranchStatus("truePUWeight"           , 1);
    tI->SetBranchStatus("btagWeight"             , 1);
    tI->SetBranchStatus("upBTagLightWeight"      , 1);
    tI->SetBranchStatus("upBTagHeavyWeight"      , 1);
    tI->SetBranchStatus("passjson"               , 1);
    tI->SetBranchStatus("passeebadscflt"         , 1);
    tI->SetBranchStatus("passhbhefltloose"       , 1);
    tI->SetBranchStatus("passcscbeamhaloflt"     , 1);
    tI->SetBranchStatus("passeebadsc4flt"        , 1);
    tI->SetBranchStatus("passhbheisoflt"         , 1);
    tI->SetBranchStatus("passaddmetflts"         , 1);
    tI->SetBranchStatus("ht"                     , 1);
    tI->SetBranchStatus("met"                    , 1);
    tI->SetBranchStatus("nvetolep"               , 1);
    tI->SetBranchStatus("nvetotau"               , 1);
//    tI->SetBranchStatus("j2pt"                   , 1);
    tI->SetBranchStatus("njl"                    , 1);
    tI->SetBranchStatus("passmetmht90"           , 1);
    tI->SetBranchStatus("passtrigel"             , 1);
    tI->SetBranchStatus("passtrigmu"             , 1);
    tI->SetBranchStatus("ismc"                   , 1);
    tI->SetBranchStatus("dphij1lmet"             , 1);
    tI->SetBranchStatus("j1lpt"                  , 1);
    tI->SetBranchStatus("csvj1pt"                , 1);
//    tI->SetBranchStatus("nmbjets"                , 1);
//    tI->SetBranchStatus("dphij12met"             , 1);
    tI->SetBranchStatus("dphij1met"              , 1);
    tI->SetBranchStatus("dphij2met"              , 1);
    tI->SetBranchStatus("dphij3met"              , 1);
//    tI->SetBranchStatus("dphij4met"              , 1);
    tI->SetBranchStatus("njets"                  , 1);
    tI->SetBranchStatus("nbjets"                 , 1);
    tI->SetBranchStatus("nlbjets"                , 1);
    tI->SetBranchStatus("mtcsv12met"             , 1);
    tI->SetBranchStatus("mtlepmet"               , 1);
    tI->SetBranchStatus("qcdRespTailWeight"      , 1);
    tI->SetBranchStatus("upTailWeight"           , 1);
    tI->SetBranchStatus("downTailWeight"         , 1);
    tI->SetBranchStatus("leptnpweight"           , 1);
    tI->SetBranchStatus("npromptgentau"          , 1);
    tI->SetBranchStatus("lepvetoweight"          , 1);
    tI->SetBranchStatus("lead_MMJ_pt"            , 1);
    tI->SetBranchStatus("lead_MMJ_eta"           , 1);
    tI->SetBranchStatus("lead_MMJ_dPhiGenReco"   , 1);
    tI->SetBranchStatus("lead_MMJ_res"           , 1);
    tI->SetBranchStatus("lead_MMJ_flavor"        , 1);
    tI->SetBranchStatus("lead_MMJ_rank"          , 1);
    tI->SetBranchStatus("lead_MMJ_recorank"      , 1);
    tI->SetBranchStatus("secd_MMJ_pt"            , 1);
    tI->SetBranchStatus("secd_MMJ_eta"           , 1);
    tI->SetBranchStatus("secd_MMJ_dPhiGenReco"   , 1);
    tI->SetBranchStatus("secd_MMJ_res"           , 1);
    tI->SetBranchStatus("secd_MMJ_flavor"        , 1);
    tI->SetBranchStatus("secd_MMJ_rank"          , 1);
    tI->SetBranchStatus("secd_MMJ_recorank"      , 1);
    tI->SetBranchStatus("secd_dMMJ2_dMMJ1"       , 1);
    tI->SetBranchStatus("abso_MMJ_pt"            , 1);
    tI->SetBranchStatus("abso_MMJ_eta"           , 1);
    tI->SetBranchStatus("abso_MMJ_dPhiGenReco"   , 1);
    tI->SetBranchStatus("abso_MMJ_res"           , 1);
    tI->SetBranchStatus("abso_MMJ_flavor"        , 1);
    tI->SetBranchStatus("abso_MMJ_rank"          , 1);
    tI->SetBranchStatus("abso_MMJ_recorank"      , 1);
    tI->SetBranchStatus("abso_dMMJ2_dMMJ1"       , 1);
    tI->SetBranchStatus("jet1_dPhi_genreco"      , 1);
    tI->SetBranchStatus("jet2_dPhi_genreco"      , 1);
    tI->SetBranchStatus("jet3_dPhi_genreco"      , 1);
    tI->SetBranchStatus("jet4_dPhi_genreco"      , 1);
    tI->SetBranchStatus("jet1_csv"               , 1);
    tI->SetBranchStatus("jet2_csv"               , 1);
    tI->SetBranchStatus("jet3_csv"               , 1);
    tI->SetBranchStatus("jet4_csv"               , 1);

    TFile * fO = new TFile(QCDSupport::inputDir + pre +"_skimmed.root","recreate");
    fO->cd();
    TTree * tO = tI->CopyTree(QCDSupport::METPresel);
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
