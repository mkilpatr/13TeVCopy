#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"

#include <TRandom3.h>

using namespace std;
using namespace ucsbsusy;

class Copier : public TreeCopierAllBranches {
public:
  Copier(string fileName, string treeName, string outFileName, bool doReco, bool usePuppi, bool isMCTree) : TreeCopierAllBranches(fileName,treeName,outFileName,isMCTree), reco(doReco), puppi(usePuppi), rand(new TRandom3(1234))
  {
  };
  virtual ~Copier() {};

  virtual void setupTree() {
    reader.getTree()->SetBranchStatus("*",1);
    //remove some branches
    //reader.getTree()->SetBranchStatus("nVert",0);

    outFile_ = new TFile(outFileName_,"RECREATE");
    outFile_->cd();
    treeWriter_ = new TreeWriter(reader.getTree()->CloneTree(0));
  } // setupTree()

  void loadVariables() {
	if (reco) {
	  if (puppi){
	    reader.setBranchAddress( "", "ak4pfpuppi_jet_pt", &pt  );
	    reader.setBranchAddress( "", "ak4pfchs_jet_eta",  &eta );
	  } // puppi
	  else {
	    reader.setBranchAddress( "", "ak4_jet_pt",  &pt  );
	    reader.setBranchAddress( "", "ak4_jet_eta", &eta );
	  } // no puppi
	} // reco
	else {
		reader.setBranchAddress( "", "ak4_genjet_pt",  &pt  );
		reader.setBranchAddress( "", "ak4_genjet_eta", &eta );
	} // gen
  } // loadVariables()

  virtual void processVariables(){}

  bool fillEvent() {
    //seperate reco and gen trees <- nope
    //if(reco == isGen) return false;
    //if(splitResult != 1 && splitResult != 2 ) return false;
    //filtering
	if (TMath::Abs(eta)>2.4) return true;
    if      ( pt < 100 && rand->Uniform() < .5 ) return false;
    else if ( pt < 200 && rand->Uniform() < .3 ) return false;
    else if ( pt < 300 && rand->Uniform() < .2 ) return false;
    //data.fill<float>(subjet_dr         ,PhysicsUtilities::deltaR(subJet_1_eta,subJet_1_phi,subJet_2_eta,subJet_2_phi)  );
    return true;
  } // fillEvent()

  void book() {
    //subjet_dr      = data.add<float>("","subjet_dr"      ,"F",0);
  } // Copier


  //size  subjet_dr;
  float pt;
  float eta;

  bool reco;
  bool puppi;
  TRandom3 * rand;
};


#endif



void filterQGMVAtree(string fileName = "evttree.root", string treeName = "Events", string outFileName ="newTree.root", bool doReco = true, bool usePuppi = true, bool isMCTree = true) {
  Copier a(fileName,treeName,outFileName,doReco,usePuppi,isMCTree);
  a.analyze(10000);
}
