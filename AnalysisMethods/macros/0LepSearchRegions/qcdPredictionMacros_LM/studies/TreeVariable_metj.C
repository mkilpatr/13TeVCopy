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
const TString outputDir = "MetJetBins";
QCDSupport::SRegInfo srinfo;
vector<int> colors;

void TreeVariable_metj() {
   TFile *f = new TFile(QCDSupport::inputDir + "/TT_WJets.root","update");
   //TFile *f = new TFile(QCDSupport::inputDir + "/MET.root","update");
   //TFile *f = new TFile(QCDSupport::inputDir + "/QCD_smeared_tree.root","update");
   //TFile *f = new TFile(QCDSupport::inputDir + "/T2fbd_400_350_tree.root","update");
   TTree *T = (TTree*)f->Get("Events");
   float metj1, metj2, metj3, metj4, metj12, metj123, metj1234;
   float dphimetj1, dphimetj2, dphimetj3, dphimetj4;
   TBranch *bmetj12 = T->Branch("metj12",&metj12,"metj12/F");
   TBranch *bmetj123 = T->Branch("metj123",&metj123,"metj123/F");
   TBranch *bmetj1234 = T->Branch("metj1234",&metj1234,"metj1234/F");
   T->SetBranchAddress("dphimetj1",&dphimetj1);
   T->SetBranchAddress("dphimetj2",&dphimetj2);
   T->SetBranchAddress("dphimetj3",&dphimetj3);
   T->SetBranchAddress("dphimetj4",&dphimetj4);
   T->SetBranchAddress("metj1",&metj1);
   T->SetBranchAddress("metj2",&metj2);
   T->SetBranchAddress("metj3",&metj3);
   T->SetBranchAddress("metj4",&metj4);
   Long64_t nentries = T->GetEntries();
   for (Long64_t i=0;i<nentries;i++) {
      T->GetEntry(i);
      if(i % 50000 == 0) std::cout << "Analyzing Event: " << i << std::endl;
      if(min(dphimetj1, dphimetj2) == dphimetj1)
	metj12 = metj1;
      else
	metj12 = metj2;

      if(min(dphimetj1, min(dphimetj2, dphimetj3)) == dphimetj1)
      	metj123 = metj1;
      else if(min(dphimetj1, min(dphimetj2, dphimetj3)) == dphimetj2)
	metj123 = metj2;
      else 
	metj123 = metj3;

      if(min(min(dphimetj1, dphimetj2), min(dphimetj3, dphimetj4)) == dphimetj1)
	metj1234 = metj1;
      else if(min(min(dphimetj1, dphimetj2), min(dphimetj3, dphimetj4)) == dphimetj2)
	metj1234 = metj2;
      else if(min(min(dphimetj1, dphimetj2), min(dphimetj3, dphimetj4)) == dphimetj3)
	metj1234 = metj3;
      else
	metj1234 = metj4;
      
      bmetj12->Fill();
      bmetj123->Fill();
      bmetj1234->Fill();
   }
   T->Print();
   T->Write();
   delete f;
}
