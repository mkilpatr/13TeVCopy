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
   float dphistarmhtj12_0_500, dphistarmhtj12_500_1000, dphistarmhtj12_1000_1500, dphistarmhtj12_1500_2000, dphistarmhtj12_2000_2500, dphistarmhtj12_2500_Inf;
   float dphistarmhtj1234_0_500, dphistarmhtj1234_500_1000, dphistarmhtj1234_1000_1500, dphistarmhtj1234_1500_2000, dphistarmhtj1234_2000_2500, dphistarmhtj1234_2500_Inf;
   float dphimhtj12_qcd, dphimhtj1234_qcd;
   float dphistarmhtj12_qcd, dphistarmhtj1234_qcd;
   float dphistarmhtj12_qcd_0_500, dphistarmhtj12_qcd_500_1000, dphistarmhtj12_qcd_1000_1500, dphistarmhtj12_qcd_1500_2000, dphistarmhtj12_qcd_2000_2500, dphistarmhtj12_qcd_2500_Inf;
   float dphistarmhtj1234_qcd_0_500, dphistarmhtj1234_qcd_500_1000, dphistarmhtj1234_qcd_1000_1500, dphistarmhtj1234_qcd_1500_2000, dphistarmhtj1234_qcd_2000_2500, dphistarmhtj1234_qcd_2500_Inf;
   TBranch *bdphistarmhtj12_qcd_0_500 = T->Branch("dphistarmhtj12_qcd_0_500",&dphistarmhtj12_qcd_0_500,"dphistarmhtj12_qcd_0_500/F");
   TBranch *bdphistarmhtj1234_qcd_0_500 = T->Branch("dphistarmhtj1234_qcd_0_500",&dphistarmhtj1234_qcd_0_500,"dphistarmhtj1234_qcd_0_500/F");
   TBranch *bdphistarmhtj12_qcd_500_1000 = T->Branch("dphistarmhtj12_qcd_500_1000",&dphistarmhtj12_qcd_500_1000,"dphistarmhtj12_qcd_500_1000/F");
   TBranch *bdphistarmhtj1234_qcd_500_1000 = T->Branch("dphistarmhtj1234_qcd_500_1000",&dphistarmhtj1234_qcd_500_1000,"dphistarmhtj1234_qcd_500_1000/F");
   TBranch *bdphistarmhtj12_qcd_1000_1500 = T->Branch("dphistarmhtj12_qcd_1000_1500",&dphistarmhtj12_qcd_1000_1500,"dphistarmhtj12_qcd_1000_1500/F");
   TBranch *bdphistarmhtj1234_qcd_1000_1500 = T->Branch("dphistarmhtj1234_qcd_1000_1500",&dphistarmhtj1234_qcd_1000_1500,"dphistarmhtj1234_qcd_1000_1500/F");
   TBranch *bdphistarmhtj12_qcd_1500_2000 = T->Branch("dphistarmhtj12_qcd_1500_2000",&dphistarmhtj12_qcd_1500_2000,"dphistarmhtj12_qcd_1500_2000/F");
   TBranch *bdphistarmhtj1234_qcd_1500_2000 = T->Branch("dphistarmhtj1234_qcd_1500_2000",&dphistarmhtj1234_qcd_1500_2000,"dphistarmhtj1234_qcd_1500_2000/F");
   TBranch *bdphistarmhtj12_qcd_2000_2500 = T->Branch("dphistarmhtj12_qcd_2000_2500",&dphistarmhtj12_qcd_2000_2500,"dphistarmhtj12_qcd_2000_2500/F");
   TBranch *bdphistarmhtj1234_qcd_2000_2500 = T->Branch("dphistarmhtj1234_qcd_2000_2500",&dphistarmhtj1234_qcd_2000_2500,"dphistarmhtj1234_qcd_2000_2500/F");
   TBranch *bdphistarmhtj12_qcd_2500_Inf = T->Branch("dphistarmhtj12_qcd_2500_Inf",&dphistarmhtj12_qcd_2500_Inf,"dphistarmhtj12_qcd_2500_Inf/F");
   TBranch *bdphistarmhtj1234_qcd_2500_Inf = T->Branch("dphistarmhtj1234_qcd_2500_Inf",&dphistarmhtj1234_qcd_2500_Inf,"dphistarmhtj1234_qcd_2500_Inf/F");
   TBranch *bdphistarmhtj12_0_500 = T->Branch("dphistarmhtj12_0_500",&dphistarmhtj12_0_500,"dphistarmhtj12_0_500/F");
   TBranch *bdphistarmhtj1234_0_500 = T->Branch("dphistarmhtj1234_0_500",&dphistarmhtj1234_0_500,"dphistarmhtj1234_0_500/F");
   TBranch *bdphistarmhtj12_500_1000 = T->Branch("dphistarmhtj12_500_1000",&dphistarmhtj12_500_1000,"dphistarmhtj12_500_1000/F");
   TBranch *bdphistarmhtj1234_500_1000 = T->Branch("dphistarmhtj1234_500_1000",&dphistarmhtj1234_500_1000,"dphistarmhtj1234_500_1000/F");
   TBranch *bdphistarmhtj12_1000_1500 = T->Branch("dphistarmhtj12_1000_1500",&dphistarmhtj12_1000_1500,"dphistarmhtj12_1000_1500/F");
   TBranch *bdphistarmhtj1234_1000_1500 = T->Branch("dphistarmhtj1234_1000_1500",&dphistarmhtj1234_1000_1500,"dphistarmhtj1234_1000_1500/F");
   TBranch *bdphistarmhtj12_1500_2000 = T->Branch("dphistarmhtj12_1500_2000",&dphistarmhtj12_1500_2000,"dphistarmhtj12_1500_2000/F");
   TBranch *bdphistarmhtj1234_1500_2000 = T->Branch("dphistarmhtj1234_1500_2000",&dphistarmhtj1234_1500_2000,"dphistarmhtj1234_1500_2000/F");
   TBranch *bdphistarmhtj12_2000_2500 = T->Branch("dphistarmhtj12_2000_2500",&dphistarmhtj12_2000_2500,"dphistarmhtj12_2000_2500/F");
   TBranch *bdphistarmhtj1234_2000_2500 = T->Branch("dphistarmhtj1234_2000_2500",&dphistarmhtj1234_2000_2500,"dphistarmhtj1234_2000_2500/F");
   TBranch *bdphistarmhtj12_2500_Inf = T->Branch("dphistarmhtj12_2500_Inf",&dphistarmhtj12_2500_Inf,"dphistarmhtj12_2500_Inf/F");
   TBranch *bdphistarmhtj1234_2500_Inf = T->Branch("dphistarmhtj1234_2500_Inf",&dphistarmhtj1234_2500_Inf,"dphistarmhtj1234_2500_Inf/F");
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
      dphistarmhtj12_qcd_0_500 = dphistarmhtj12;
      dphistarmhtj1234_qcd_0_500 = dphistarmhtj1234;
      dphistarmhtj12_qcd_500_1000 = dphistarmhtj12;
      dphistarmhtj1234_qcd_500_1000 = dphistarmhtj1234;
      dphistarmhtj12_qcd_1000_1500 = dphistarmhtj12;
      dphistarmhtj1234_qcd_1000_1500 = dphistarmhtj1234;
      dphistarmhtj12_qcd_1500_2000 = dphistarmhtj12;
      dphistarmhtj1234_qcd_1500_2000 = dphistarmhtj1234;
      dphistarmhtj12_qcd_2000_2500 = dphistarmhtj12;
      dphistarmhtj1234_qcd_2000_2500 = dphistarmhtj1234;
      dphistarmhtj12_qcd_2500_Inf = dphistarmhtj12;
      dphistarmhtj1234_qcd_2500_Inf = dphistarmhtj1234;
      dphistarmhtj12_0_500 = dphistarmhtj12;
      dphistarmhtj1234_0_500 = dphistarmhtj1234;
      dphistarmhtj12_500_1000 = dphistarmhtj12;
      dphistarmhtj1234_500_1000 = dphistarmhtj1234;
      dphistarmhtj12_1000_1500 = dphistarmhtj12;
      dphistarmhtj1234_1000_1500 = dphistarmhtj1234;
      dphistarmhtj12_1500_2000 = dphistarmhtj12;
      dphistarmhtj1234_1500_2000 = dphistarmhtj1234;
      dphistarmhtj12_2000_2500 = dphistarmhtj12;
      dphistarmhtj1234_2000_2500 = dphistarmhtj1234;
      dphistarmhtj12_2500_Inf = dphistarmhtj12;
      dphistarmhtj1234_2500_Inf = dphistarmhtj1234;
      
      bdphistarmhtj12_qcd_0_500->Fill();
      bdphistarmhtj1234_qcd_0_500->Fill();
      bdphistarmhtj12_qcd_500_1000->Fill();
      bdphistarmhtj1234_qcd_500_1000->Fill();
      bdphistarmhtj12_qcd_1000_1500->Fill();
      bdphistarmhtj1234_qcd_1000_1500->Fill();
      bdphistarmhtj12_qcd_1500_2000->Fill();
      bdphistarmhtj1234_qcd_1500_2000->Fill();
      bdphistarmhtj12_qcd_2000_2500->Fill();
      bdphistarmhtj1234_qcd_2000_2500->Fill();
      bdphistarmhtj12_qcd_2500_Inf->Fill();
      bdphistarmhtj1234_qcd_2500_Inf->Fill();
      bdphistarmhtj12_0_500->Fill();
      bdphistarmhtj1234_0_500->Fill();
      bdphistarmhtj12_500_1000->Fill();
      bdphistarmhtj1234_500_1000->Fill();
      bdphistarmhtj12_1000_1500->Fill();
      bdphistarmhtj1234_1000_1500->Fill();
      bdphistarmhtj12_1500_2000->Fill();
      bdphistarmhtj1234_1500_2000->Fill();
      bdphistarmhtj12_2000_2500->Fill();
      bdphistarmhtj1234_2000_2500->Fill();
      bdphistarmhtj12_2500_Inf->Fill();
      bdphistarmhtj1234_2500_Inf->Fill();
   }
   T->Print();
   T->Write();
   delete f;
}
