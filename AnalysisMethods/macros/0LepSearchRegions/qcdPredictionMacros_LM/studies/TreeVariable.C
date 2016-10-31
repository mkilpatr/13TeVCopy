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

void TreeVariable() {
   //TFile *f = new TFile(QCDSupport::inputDir + "/Total_Bkg_tree.root","update");
   //TFile *f = new TFile(QCDSupport::inputDir + "/QCD_smeared_tree_skimmed.root","update");
   TFile *f = new TFile(QCDSupport::inputDir + "/T2tt_600_300_tree.root","update");
   TTree *T = (TTree*)f->Get("Events");
   float dphimhtj1, dphimhtj2, dphimhtj3, dphimhtj4;
   float dphistarmhtj1, dphistarmhtj2, dphistarmhtj3, dphistarmhtj4;
   float omegamhtj1, omegamhtj2, omegamhtj3, omegamhtj4;
   float chimhtj1, chimhtj2, chimhtj3, chimhtj4;
   float dphimhtj12, dphimhtj123, dphimhtj1234;
   float dphistarmhtj12, dphistarmhtj123, dphistarmhtj1234;
   float omegamhtj12, omegamhtj123, omegamhtj1234;
   float chimhtj12, chimhtj123, chimhtj1234;
   TBranch *bdphimhtj12 = T->Branch("dphimhtj12",&dphimhtj12,"dphimhtj12/F");
   TBranch *bdphistarmhtj12 = T->Branch("dphistarmhtj12",&dphistarmhtj12,"dphistarmhtj12/F");
   TBranch *bomegamhtj12 = T->Branch("omegamhtj12",&omegamhtj12,"omegamhtj12/F");
   TBranch *bchimhtj12 = T->Branch("chimhtj12",&chimhtj12,"chimhtj12/F");
   TBranch *bdphimhtj123 = T->Branch("dphimhtj123",&dphimhtj123,"dphimhtj123/F");
   TBranch *bdphistarmhtj123 = T->Branch("dphistarmhtj123",&dphistarmhtj123,"dphistarmhtj123/F");
   TBranch *bomegamhtj123 = T->Branch("omegamhtj123",&omegamhtj123,"omegamhtj123/F");
   TBranch *bchimhtj123 = T->Branch("chimhtj123",&chimhtj123,"chimhtj123/F");
   TBranch *bdphimhtj1234 = T->Branch("dphimhtj1234",&dphimhtj1234,"dphimhtj1234/F");
   TBranch *bdphistarmhtj1234 = T->Branch("dphistarmhtj1234",&dphistarmhtj1234,"dphistarmhtj1234/F");
   TBranch *bomegamhtj1234 = T->Branch("omegamhtj1234",&omegamhtj1234,"omegamhtj1234/F");
   TBranch *bchimhtj1234 = T->Branch("chimhtj1234",&chimhtj1234,"chimhtj1234/F");
   T->SetBranchAddress("dphimhtj1",&dphimhtj1);
   T->SetBranchAddress("dphimhtj2",&dphimhtj2);
   T->SetBranchAddress("dphimhtj3",&dphimhtj3);
   T->SetBranchAddress("dphimhtj4",&dphimhtj4);
   T->SetBranchAddress("dphistarmhtj1",&dphistarmhtj1);
   T->SetBranchAddress("dphistarmhtj2",&dphistarmhtj2);
   T->SetBranchAddress("dphistarmhtj3",&dphistarmhtj3);
   T->SetBranchAddress("dphistarmhtj4",&dphistarmhtj4);
   T->SetBranchAddress("omegamhtj1",&omegamhtj1);
   T->SetBranchAddress("omegamhtj2",&omegamhtj2);
   T->SetBranchAddress("omegamhtj3",&omegamhtj3);
   T->SetBranchAddress("omegamhtj4",&omegamhtj4);
   T->SetBranchAddress("chimhtj1",&chimhtj1);
   T->SetBranchAddress("chimhtj2",&chimhtj2);
   T->SetBranchAddress("chimhtj3",&chimhtj3);
   T->SetBranchAddress("chimhtj4",&chimhtj4);
   Long64_t nentries = T->GetEntries();
   for (Long64_t i=0;i<nentries;i++) {
      T->GetEntry(i);
      if(i % 50000 == 0) std::cout << "Analyzing Event: " << i << std::endl;
      dphimhtj12 = min(dphimhtj1, dphimhtj2);
      dphimhtj123 = min(dphimhtj1, min(dphimhtj2, dphimhtj3));
      dphimhtj1234 = min(min(dphimhtj1, dphimhtj2), min(dphimhtj3, dphimhtj4));
      dphistarmhtj12 = min(dphistarmhtj1, dphistarmhtj2);
      dphistarmhtj123 = min(dphistarmhtj1, min(dphistarmhtj2, dphistarmhtj3));
      dphistarmhtj1234 = min(min(dphistarmhtj1, dphistarmhtj2), min(dphistarmhtj3, dphistarmhtj4));
      omegamhtj12 = min(omegamhtj1, omegamhtj2);
      omegamhtj123 = min(omegamhtj1, min(omegamhtj2, omegamhtj3));
      omegamhtj1234 = min(min(omegamhtj1, omegamhtj2), min(omegamhtj3, omegamhtj4));
      chimhtj12 = min(chimhtj1, chimhtj2);
      chimhtj123 = min(chimhtj1, min(chimhtj2, chimhtj3));
      chimhtj1234 = min(min(chimhtj1, chimhtj2), min(chimhtj3, chimhtj4));
      bdphimhtj12->Fill();
      bdphimhtj123->Fill();
      bdphimhtj1234->Fill();
      bdphistarmhtj12->Fill();
      bdphistarmhtj123->Fill();
      bdphistarmhtj1234->Fill();
      bomegamhtj12->Fill();
      bomegamhtj123->Fill();
      bomegamhtj1234->Fill();
      bchimhtj12->Fill();
      bchimhtj123->Fill();
      bchimhtj1234->Fill();
   }
   T->Print();
   T->Write();
   delete f;
}
