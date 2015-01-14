#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"

#include <TRandom3.h>

using namespace std;
using namespace ucsbsusy;

class Copier : public TreeCopierAllBranches {
public:
  Copier(string fileName, string treeName, string outFileName, bool doReco, bool isMCTree) : TreeCopierAllBranches(fileName,treeName,outFileName,isMCTree), reco(doReco), rand(new TRandom3(1234))
  {
  };
  virtual ~Copier() {};

  virtual void setupTree() {
    reader.getTree()->SetBranchStatus("*",1);
    //remove some branches
    reader.getTree()->SetBranchStatus("nVert",0);
    reader.getTree()->SetBranchStatus("jetType",0);
    reader.getTree()->SetBranchStatus("numSplits",0);
    reader.getTree()->SetBranchStatus("superJet_phi",0);
    reader.getTree()->SetBranchStatus("subJet_1_pt",0);
    reader.getTree()->SetBranchStatus("subJet_1_mass",0);
    reader.getTree()->SetBranchStatus("subJet_2_pt",0);
    reader.getTree()->SetBranchStatus("subJet_2_mass",0);
    reader.getTree()->SetBranchStatus("center_value",0);
    reader.getTree()->SetBranchStatus("oldDisc",0);
    reader.getTree()->SetBranchStatus("oldDisc_shouldSplit",0);


    outFile_ = new TFile(outFileName_,"RECREATE");
    outFile_->cd();
    treeWriter_ = new TreeWriter(reader.getTree()->CloneTree(0));
  }

  void loadVariables(){
    reader.setBranchAddress("","superJet_eta",  &superJet_eta);
    reader.setBranchAddress("","superJet_pt",  &superJet_pt);
    reader.setBranchAddress("","subJet_1_eta", &subJet_1_eta);
    reader.setBranchAddress("","subJet_1_phi", &subJet_1_phi);
    reader.setBranchAddress("","subJet_2_eta", &subJet_2_eta);
    reader.setBranchAddress("","subJet_2_phi", &subJet_2_phi);
    reader.setBranchAddress("","splitResult", &splitResult);
    reader.setBranchAddress("","isGen", &isGen);
  }

  virtual void processVariables(){}

  bool fillEvent() {
    //seperate reco and gen trees
    if(reco == isGen) return false;
    //filtering
    if(superJet_pt < 160 && TMath::Abs(superJet_eta) < 1.9 && gRandom->Uniform() < .9){
      return false;
    }

    if(superJet_pt >= 300 && TMath::Abs(superJet_eta) >= 1.9 ){
      superJet_pt = 305;
      superJet_eta = 2.0;
    } else if (superJet_pt >= 100 && TMath::Abs(superJet_eta) >= 2.9 ){
      superJet_eta = 2.0;
    }
    superJet_eta = TMath::Abs(superJet_eta);
    data.fill<float>(subjet_dr         ,PhysicsUtilities::deltaR(subJet_1_eta,subJet_1_phi,subJet_2_eta,subJet_2_phi)  );
    return true;
  }

  void book() {
    subjet_dr      = data.add<float>("","subjet_dr"      ,"F",0);
  }


  size  subjet_dr;
  float subJet_1_eta;
  float subJet_1_phi;
  float subJet_2_eta;
  float subJet_2_phi;
  float superJet_pt;
  float superJet_eta;
  size8 splitResult;
  bool  isGen;

  bool reco;
  TRandom3 * rand;
};


#endif



void filterPickyJetsMVATree(string fileName = "evttree.root", string treeName = "TestAnalyzer/Events", string outFileName ="newTree.root", bool doReco = true, bool isMCTree = true) {
  Copier a(fileName,treeName,outFileName,doReco,isMCTree);
  a.analyze(1000000);
}
