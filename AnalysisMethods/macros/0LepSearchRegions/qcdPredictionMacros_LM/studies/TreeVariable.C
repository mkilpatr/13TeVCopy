#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/PlotUtils/interface/PlotROCCurves.h"
#include "AnalysisTools/Utilities/interface/JetFlavorInfo.h"
#include "AnalysisMethods/PlotUtils/interface/HistogramGetter.hh"
#include "AnalysisTools/TreeReader/interface/Defaults.h"
#include "../QCDPredictionHelper.h"
#include "TDirectory.h"
#include "TFile.h"
#endif

const TString originalTreeDir = "../../../run/plots_16_11_15_Smear";
const TString outputDir = "qcdROCCurves";
QCDSupport::SRegInfo srinfo;
vector<int> colors;

void TreeVariable() {
   //TFile *f = new TFile(QCDSupport::inputDir + "/Total_Bkg_tree.root","update");
   //TFile *f = new TFile(QCDSupport::inputDir + "/QCD_smeared_tree.root","update");
   TFile *f = new TFile(QCDSupport::inputDir + "/T2tt_800_100_tree.root","update");
   TTree *T = (TTree*)f->Get("Events");
   float dphimetj1, dphimetj2, dphimetj3, dphimetj4;
   float dphistarmetj1, dphistarmetj2, dphistarmetj3, dphistarmetj4;
   float omegametj1, omegametj2, omegametj3, omegametj4;
   float chimetj1, chimetj2, chimetj3, chimetj4;
   float dphimetj12, dphimetj123, dphimetj1234;
   float dphistarmetj12, dphistarmetj123, dphistarmetj1234;
   float omegametj12, omegametj123, omegametj1234;
   float chimetj12, chimetj123, chimetj1234;
   TBranch *bdphimetj12 = T->Branch("dphimetj12",&dphimetj12,"dphimetj12/F");
   TBranch *bdphistarmetj12 = T->Branch("dphistarmetj12",&dphistarmetj12,"dphistarmetj12/F");
   TBranch *bomegametj12 = T->Branch("omegametj12",&omegametj12,"omegametj12/F");
   TBranch *bchimetj12 = T->Branch("chimetj12",&chimetj12,"chimetj12/F");
   TBranch *bdphimetj123 = T->Branch("dphimetj123",&dphimetj123,"dphimetj123/F");
   TBranch *bdphistarmetj123 = T->Branch("dphistarmetj123",&dphistarmetj123,"dphistarmetj123/F");
   TBranch *bomegametj123 = T->Branch("omegametj123",&omegametj123,"omegametj123/F");
   TBranch *bchimetj123 = T->Branch("chimetj123",&chimetj123,"chimetj123/F");
   TBranch *bdphimetj1234 = T->Branch("dphimetj1234",&dphimetj1234,"dphimetj1234/F");
   TBranch *bdphistarmetj1234 = T->Branch("dphistarmetj1234",&dphistarmetj1234,"dphistarmetj1234/F");
   TBranch *bomegametj1234 = T->Branch("omegametj1234",&omegametj1234,"omegametj1234/F");
   TBranch *bchimetj1234 = T->Branch("chimetj1234",&chimetj1234,"chimetj1234/F");
   T->SetBranchAddress("dphimetj1",&dphimetj1);
   T->SetBranchAddress("dphimetj2",&dphimetj2);
   T->SetBranchAddress("dphimetj3",&dphimetj3);
   T->SetBranchAddress("dphimetj4",&dphimetj4);
   T->SetBranchAddress("dphistarmetj1",&dphistarmetj1);
   T->SetBranchAddress("dphistarmetj2",&dphistarmetj2);
   T->SetBranchAddress("dphistarmetj3",&dphistarmetj3);
   T->SetBranchAddress("dphistarmetj4",&dphistarmetj4);
   T->SetBranchAddress("omegametj1",&omegametj1);
   T->SetBranchAddress("omegametj2",&omegametj2);
   T->SetBranchAddress("omegametj3",&omegametj3);
   T->SetBranchAddress("omegametj4",&omegametj4);
   T->SetBranchAddress("chimetj1",&chimetj1);
   T->SetBranchAddress("chimetj2",&chimetj2);
   T->SetBranchAddress("chimetj3",&chimetj3);
   T->SetBranchAddress("chimetj4",&chimetj4);
   Long64_t nentries = T->GetEntries();
   for (Long64_t i=0;i<nentries;i++) {
      T->GetEntry(i);
      if(i % 50000 == 0) std::cout << "Analyzing Event: " << i << std::endl;
      dphimetj12 = min(dphimetj1, dphimetj2);
      dphimetj123 = min(dphimetj1, min(dphimetj2, dphimetj3));
      dphimetj1234 = min(min(dphimetj1, dphimetj2), min(dphimetj3, dphimetj4));
      dphistarmetj12 = min(dphistarmetj1, dphistarmetj2);
      dphistarmetj123 = min(dphistarmetj1, min(dphistarmetj2, dphistarmetj3));
      dphistarmetj1234 = min(min(dphistarmetj1, dphistarmetj2), min(dphistarmetj3, dphistarmetj4));
      omegametj12 = min(omegametj1, omegametj2);
      omegametj123 = min(omegametj1, min(omegametj2, omegametj3));
      omegametj1234 = min(min(omegametj1, omegametj2), min(omegametj3, omegametj4));
      chimetj12 = min(chimetj1, chimetj2);
      chimetj123 = min(chimetj1, min(chimetj2, chimetj3));
      chimetj1234 = min(min(chimetj1, chimetj2), min(chimetj3, chimetj4));
      bdphimetj12->Fill();
      bdphimetj123->Fill();
      bdphimetj1234->Fill();
      bdphistarmetj12->Fill();
      bdphistarmetj123->Fill();
      bdphistarmetj1234->Fill();
      bomegametj12->Fill();
      bomegametj123->Fill();
      bomegametj1234->Fill();
      bchimetj12->Fill();
      bchimetj123->Fill();
      bchimetj1234->Fill();
   }
   T->Print();
   T->Write();
   delete f;
}
