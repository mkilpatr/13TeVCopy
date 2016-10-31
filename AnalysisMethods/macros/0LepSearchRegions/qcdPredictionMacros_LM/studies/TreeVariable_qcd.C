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
   TFile *f = new TFile(QCDSupport::inputDir + "/Total_Bkg_tree.root","update");
   //TFile *f = new TFile(QCDSupport::inputDir + "/QCD_smeared_tree_skimmed.root","update");
   //TFile *f = new TFile(QCDSupport::inputDir + "/T2fbd_400_350_tree.root","update");
   TTree *T = (TTree*)f->Get("Events");
   float dphimhtj12, dphimhtj123;
   float dphistarmhtj12, dphistarmhtj123;
   float dphistarmhtj12_0_500, dphistarmhtj12_500_1000, dphistarmhtj12_1000_1500, dphistarmhtj12_1500_2000, dphistarmhtj12_2000_2500, dphistarmhtj12_2500_Inf;
   float dphistarmhtj123_0_500, dphistarmhtj123_500_1000, dphistarmhtj123_1000_1500, dphistarmhtj123_1500_2000, dphistarmhtj123_2000_2500, dphistarmhtj123_2500_Inf;
   float dphimhtj12_qcd, dphimhtj123_qcd;
   float dphistarmhtj12_qcd, dphistarmhtj123_qcd;
   float dphistarmhtj12_qcd_0_500, dphistarmhtj12_qcd_500_1000, dphistarmhtj12_qcd_1000_1500, dphistarmhtj12_qcd_1500_2000, dphistarmhtj12_qcd_2000_2500, dphistarmhtj12_qcd_2500_Inf;
   float dphistarmhtj123_qcd_0_500, dphistarmhtj123_qcd_500_1000, dphistarmhtj123_qcd_1000_1500, dphistarmhtj123_qcd_1500_2000, dphistarmhtj123_qcd_2000_2500, dphistarmhtj123_qcd_2500_Inf;
   TBranch *bdphistarmhtj12_qcd_0_500 = T->Branch("dphistarmhtj12_qcd_0_500",&dphistarmhtj12_qcd_0_500,"dphistarmhtj12_qcd_0_500/F");
   TBranch *bdphistarmhtj123_qcd_0_500 = T->Branch("dphistarmhtj123_qcd_0_500",&dphistarmhtj123_qcd_0_500,"dphistarmhtj123_qcd_0_500/F");
   TBranch *bdphistarmhtj12_qcd_500_1000 = T->Branch("dphistarmhtj12_qcd_500_1000",&dphistarmhtj12_qcd_500_1000,"dphistarmhtj12_qcd_500_1000/F");
   TBranch *bdphistarmhtj123_qcd_500_1000 = T->Branch("dphistarmhtj123_qcd_500_1000",&dphistarmhtj123_qcd_500_1000,"dphistarmhtj123_qcd_500_1000/F");
   TBranch *bdphistarmhtj12_qcd_1000_1500 = T->Branch("dphistarmhtj12_qcd_1000_1500",&dphistarmhtj12_qcd_1000_1500,"dphistarmhtj12_qcd_1000_1500/F");
   TBranch *bdphistarmhtj123_qcd_1000_1500 = T->Branch("dphistarmhtj123_qcd_1000_1500",&dphistarmhtj123_qcd_1000_1500,"dphistarmhtj123_qcd_1000_1500/F");
   TBranch *bdphistarmhtj12_qcd_1500_2000 = T->Branch("dphistarmhtj12_qcd_1500_2000",&dphistarmhtj12_qcd_1500_2000,"dphistarmhtj12_qcd_1500_2000/F");
   TBranch *bdphistarmhtj123_qcd_1500_2000 = T->Branch("dphistarmhtj123_qcd_1500_2000",&dphistarmhtj123_qcd_1500_2000,"dphistarmhtj123_qcd_1500_2000/F");
   TBranch *bdphistarmhtj12_qcd_2000_2500 = T->Branch("dphistarmhtj12_qcd_2000_2500",&dphistarmhtj12_qcd_2000_2500,"dphistarmhtj12_qcd_2000_2500/F");
   TBranch *bdphistarmhtj123_qcd_2000_2500 = T->Branch("dphistarmhtj123_qcd_2000_2500",&dphistarmhtj123_qcd_2000_2500,"dphistarmhtj123_qcd_2000_2500/F");
   TBranch *bdphistarmhtj12_qcd_2500_Inf = T->Branch("dphistarmhtj12_qcd_2500_Inf",&dphistarmhtj12_qcd_2500_Inf,"dphistarmhtj12_qcd_2500_Inf/F");
   TBranch *bdphistarmhtj123_qcd_2500_Inf = T->Branch("dphistarmhtj123_qcd_2500_Inf",&dphistarmhtj123_qcd_2500_Inf,"dphistarmhtj123_qcd_2500_Inf/F");
   TBranch *bdphistarmhtj12_0_500 = T->Branch("dphistarmhtj12_0_500",&dphistarmhtj12_0_500,"dphistarmhtj12_0_500/F");
   TBranch *bdphistarmhtj123_0_500 = T->Branch("dphistarmhtj123_0_500",&dphistarmhtj123_0_500,"dphistarmhtj123_0_500/F");
   TBranch *bdphistarmhtj12_500_1000 = T->Branch("dphistarmhtj12_500_1000",&dphistarmhtj12_500_1000,"dphistarmhtj12_500_1000/F");
   TBranch *bdphistarmhtj123_500_1000 = T->Branch("dphistarmhtj123_500_1000",&dphistarmhtj123_500_1000,"dphistarmhtj123_500_1000/F");
   TBranch *bdphistarmhtj12_1000_1500 = T->Branch("dphistarmhtj12_1000_1500",&dphistarmhtj12_1000_1500,"dphistarmhtj12_1000_1500/F");
   TBranch *bdphistarmhtj123_1000_1500 = T->Branch("dphistarmhtj123_1000_1500",&dphistarmhtj123_1000_1500,"dphistarmhtj123_1000_1500/F");
   TBranch *bdphistarmhtj12_1500_2000 = T->Branch("dphistarmhtj12_1500_2000",&dphistarmhtj12_1500_2000,"dphistarmhtj12_1500_2000/F");
   TBranch *bdphistarmhtj123_1500_2000 = T->Branch("dphistarmhtj123_1500_2000",&dphistarmhtj123_1500_2000,"dphistarmhtj123_1500_2000/F");
   TBranch *bdphistarmhtj12_2000_2500 = T->Branch("dphistarmhtj12_2000_2500",&dphistarmhtj12_2000_2500,"dphistarmhtj12_2000_2500/F");
   TBranch *bdphistarmhtj123_2000_2500 = T->Branch("dphistarmhtj123_2000_2500",&dphistarmhtj123_2000_2500,"dphistarmhtj123_2000_2500/F");
   TBranch *bdphistarmhtj12_2500_Inf = T->Branch("dphistarmhtj12_2500_Inf",&dphistarmhtj12_2500_Inf,"dphistarmhtj12_2500_Inf/F");
   TBranch *bdphistarmhtj123_2500_Inf = T->Branch("dphistarmhtj123_2500_Inf",&dphistarmhtj123_2500_Inf,"dphistarmhtj123_2500_Inf/F");
   T->SetBranchAddress("dphimhtj12",&dphimhtj12);
   T->SetBranchAddress("dphimhtj123",&dphimhtj123);
   T->SetBranchAddress("dphistarmhtj12",&dphistarmhtj12);
   T->SetBranchAddress("dphistarmhtj123",&dphistarmhtj123);
   Long64_t nentries = T->GetEntries();
   for (Long64_t i=0;i<nentries;i++) {
      T->GetEntry(i);
      if(i % 50000 == 0) std::cout << "Analyzing Event: " << i << std::endl;
      //dphimhtj12_qcd = dphimhtj12;
      //dphimhtj123_qcd = dphimhtj123;
      dphistarmhtj12_qcd_0_500 = dphistarmhtj12;
      dphistarmhtj123_qcd_0_500 = dphistarmhtj123;
      dphistarmhtj12_qcd_500_1000 = dphistarmhtj12;
      dphistarmhtj123_qcd_500_1000 = dphistarmhtj123;
      dphistarmhtj12_qcd_1000_1500 = dphistarmhtj12;
      dphistarmhtj123_qcd_1000_1500 = dphistarmhtj123;
      dphistarmhtj12_qcd_1500_2000 = dphistarmhtj12;
      dphistarmhtj123_qcd_1500_2000 = dphistarmhtj123;
      dphistarmhtj12_qcd_2000_2500 = dphistarmhtj12;
      dphistarmhtj123_qcd_2000_2500 = dphistarmhtj123;
      dphistarmhtj12_qcd_2500_Inf = dphistarmhtj12;
      dphistarmhtj123_qcd_2500_Inf = dphistarmhtj123;
      dphistarmhtj12_0_500 = dphistarmhtj12;
      dphistarmhtj123_0_500 = dphistarmhtj123;
      dphistarmhtj12_500_1000 = dphistarmhtj12;
      dphistarmhtj123_500_1000 = dphistarmhtj123;
      dphistarmhtj12_1000_1500 = dphistarmhtj12;
      dphistarmhtj123_1000_1500 = dphistarmhtj123;
      dphistarmhtj12_1500_2000 = dphistarmhtj12;
      dphistarmhtj123_1500_2000 = dphistarmhtj123;
      dphistarmhtj12_2000_2500 = dphistarmhtj12;
      dphistarmhtj123_2000_2500 = dphistarmhtj123;
      dphistarmhtj12_2500_Inf = dphistarmhtj12;
      dphistarmhtj123_2500_Inf = dphistarmhtj123;
      
      bdphistarmhtj12_qcd_0_500->Fill();
      bdphistarmhtj123_qcd_0_500->Fill();
      bdphistarmhtj12_qcd_500_1000->Fill();
      bdphistarmhtj123_qcd_500_1000->Fill();
      bdphistarmhtj12_qcd_1000_1500->Fill();
      bdphistarmhtj123_qcd_1000_1500->Fill();
      bdphistarmhtj12_qcd_1500_2000->Fill();
      bdphistarmhtj123_qcd_1500_2000->Fill();
      bdphistarmhtj12_qcd_2000_2500->Fill();
      bdphistarmhtj123_qcd_2000_2500->Fill();
      bdphistarmhtj12_qcd_2500_Inf->Fill();
      bdphistarmhtj123_qcd_2500_Inf->Fill();
      bdphistarmhtj12_0_500->Fill();
      bdphistarmhtj123_0_500->Fill();
      bdphistarmhtj12_500_1000->Fill();
      bdphistarmhtj123_500_1000->Fill();
      bdphistarmhtj12_1000_1500->Fill();
      bdphistarmhtj123_1000_1500->Fill();
      bdphistarmhtj12_1500_2000->Fill();
      bdphistarmhtj123_1500_2000->Fill();
      bdphistarmhtj12_2000_2500->Fill();
      bdphistarmhtj123_2000_2500->Fill();
      bdphistarmhtj12_2500_Inf->Fill();
      bdphistarmhtj123_2500_Inf->Fill();
   }
   T->Print();
   T->Write();
   delete f;
}
