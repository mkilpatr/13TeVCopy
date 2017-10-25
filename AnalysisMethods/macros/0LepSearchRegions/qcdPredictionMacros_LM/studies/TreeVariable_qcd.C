#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotROCCurves.h"
#include "AnalysisTools/Utilities/interface/JetFlavorInfo.h"
#include "AnalysisMethods/PlotUtils/interface/HistogramGetter.hh"
#include "AnalysisTools/TreeReader/interface/Defaults.h"
#include "../QCDPredictionHelper.h"
#include "TDirectory.h"
#include "TFile.h"
#endif

const TString originalTreeDir = "../../../run/plots_16_09_28_Smear";
const TString outputDir = "qcdROCCurves";
QCDSupport::SRegInfo srinfo;
vector<int> colors;

void TreeVariable_qcd() {
   //TFile *f = new TFile(QCDSupport::inputDir + "/Total_Bkg_tree.root","update");
   //TFile *f = new TFile(QCDSupport::inputDir + "/QCD_smeared_tree_skimmed.root","update");
   TFile *f = new TFile(QCDSupport::inputDir + "/T2tt_800_100_tree.root","update");
   TTree *T = (TTree*)f->Get("Events");
   float dphimhtj12, dphimhtj1234;
   float dphistarmhtj12, dphistarmhtj1234;
   float dphistarmhtj12_low, dphistarmhtj12_high; 
   float dphistarmhtj1234_low, dphistarmhtj1234_high;
   float dphimhtj12_qcd, dphimhtj1234_qcd;
   float dphistarmhtj12_qcd, dphistarmhtj1234_qcd;
   float dphistarmhtj12_qcd_low, dphistarmhtj12_qcd_high;
   float dphistarmhtj1234_qcd_low, dphistarmhtj1234_qcd_high;
   TBranch *bdphistarmhtj12_qcd_low = T->Branch("dphistarmhtj12_qcd_low",&dphistarmhtj12_qcd_low,"dphistarmhtj12_qcd_low/F");
   TBranch *bdphistarmhtj1234_qcd_low = T->Branch("dphistarmhtj1234_qcd_low",&dphistarmhtj1234_qcd_low,"dphistarmhtj1234_qcd_low/F");
   TBranch *bdphistarmhtj12_qcd_high = T->Branch("dphistarmhtj12_qcd_high",&dphistarmhtj12_qcd_high,"dphistarmhtj12_qcd_high/F");
   TBranch *bdphistarmhtj1234_qcd_high = T->Branch("dphistarmhtj1234_qcd_high",&dphistarmhtj1234_qcd_high,"dphistarmhtj1234_qcd_high/F");
   TBranch *bdphistarmhtj12_low = T->Branch("dphistarmhtj12_low",&dphistarmhtj12_low,"dphistarmhtj12_low/F");
   TBranch *bdphistarmhtj1234_low = T->Branch("dphistarmhtj1234_low",&dphistarmhtj1234_low,"dphistarmhtj1234_low/F");
   TBranch *bdphistarmhtj12_high = T->Branch("dphistarmhtj12_high",&dphistarmhtj12_high,"dphistarmhtj12_high/F");
   TBranch *bdphistarmhtj1234_high = T->Branch("dphistarmhtj1234_high",&dphistarmhtj1234_high,"dphistarmhtj1234_high/F");
   T->SetBranchAddress("dphimhtj12",&dphimhtj12);
   T->SetBranchAddress("dphimhtj1234",&dphimhtj1234);
   T->SetBranchAddress("dphistarmhtj12",&dphistarmhtj12);
   T->SetBranchAddress("dphistarmhtj1234",&dphistarmhtj1234);
   Long64_t nentries = T->GetEntries();
   for (Long64_t i=0;i<nentries;i++) {
      T->GetEntry(i);
      if(i % 50000 == 0) std::cout << "Analyzing Event: " << i << std::endl;
      //dphimhtj12_qcd = dphimhtj12;
      //dphimhtj1234_qcd = dphimhtj1234;
      dphistarmhtj12_qcd_low = dphistarmhtj12;
      dphistarmhtj1234_qcd_low = dphistarmhtj1234;
      dphistarmhtj12_qcd_high = dphistarmhtj12;
      dphistarmhtj1234_qcd_high = dphistarmhtj1234;
      dphistarmhtj12_low = dphistarmhtj12;
      dphistarmhtj1234_low = dphistarmhtj1234;
      dphistarmhtj12_high = dphistarmhtj12;
      dphistarmhtj1234_high = dphistarmhtj1234;
      
      bdphistarmhtj12_qcd_low->Fill();
      bdphistarmhtj1234_qcd_low->Fill();
      bdphistarmhtj12_qcd_high->Fill();
      bdphistarmhtj1234_qcd_high->Fill();
      bdphistarmhtj12_low->Fill();
      bdphistarmhtj1234_low->Fill();
      bdphistarmhtj12_high->Fill();
      bdphistarmhtj1234_high->Fill();
   }
   T->Print();
   T->Write();
   delete f;
}
