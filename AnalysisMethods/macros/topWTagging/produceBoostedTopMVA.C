#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/BaseTMVAProducer.h"
#endif


using namespace std;
using namespace ucsbsusy;

class Producer : public BaseTMVAProducer {
public:
  Producer(TString outputFileName,TTree* inSigTree, TTree* inBkgTree, string inMvaName) : BaseTMVAProducer(outputFileName), sigTree(inSigTree), bkgTree(inBkgTree), mvaName(inMvaName){};
  virtual ~Producer() {};

  void  loadVars() {
     addVariable("fj_subjet_pt"               ,"fj_subjet_pt"           ,'F');
     addVariable("fj_tau1"                 ,"fj_tau1"       ,'F');
     addVariable("fj_tau2"                 ,"fj_tau2"     ,'F');
     addVariable("fj_tau3"                 ,"fj_tau3"     ,'F');
     addVariable("fj_ropt"                 ,"fj_ropt"     ,'F');
     addVariable("fj_subjet_mass"          ,"fj_subjet_mass"    ,'F');
     addVariable("fj_subjet_w_mass"        ,"fj_subjet_w_mass"    ,'F');
     addVariable("fj_subjet_w_pt"          ,"fj_subjet_w_pt"   ,'F');
     addVariable("fj_b_pt"                 ,"fj_b_pt"    ,'F');
     addVariable("fj_b_csv"                ,"fj_b_csv"   ,'F');
     addVariable("fj_b_mass"               ,"fj_b_mass"  ,'F');
     addVariable("fj_subjet_w_bycsv_mass"  ,"fj_subjet_w_bycsv_mass"    ,'F');
     addVariable("fj_subjet_w_bycsv_pt"    ,"fj_subjet_w_bycsv_pt"   ,'F');
     addVariable("fj_subjet_b_bycsv_pt"    ,"fj_subjet_b_bycsv_pt"    ,'F');
     addVariable("fj_subjet_b_bycsv_csv"   ,"fj_subjet_b_bycsv_csv"   ,'F');
     addVariable("fj_subjet_b_bycsv_mass"  ,"fj_subjet_b_bycsv_mass"   ,'F');

     addVariableSet("fj_subjet_pt","fj_tau1","fj_tau2","fj_tau3","fj_ropt","fj_subjet_mass","fj_subjet_w_mass","fj_subjet_w_pt","fj_b_pt","fj_b_csv","fj_b_mass","fj_subjet_w_bycsv_mass","fj_subjet_w_bycsv_pt"  ,"fj_subjet_b_bycsv_pt"  ,"fj_subjet_b_bycsv_csv","fj_subjet_b_bycsv_mass","");

  }

  void configure(){
    addInputTree(sigTree,"Signal","(top_maxDR/2 < 1.5 && top_fj_maxDR < 1.5 && top_pt > 0) && (fj_pt >= 200 &&  fj_eta < 2.4 && fj_subjet_min_pt > 20 && fj_subjet_mass > 50 && fj_subjet_mass < 600 && fj_subjet_pt > 200)");
    addInputTree(bkgTree,"Background","(process != 2 || (top_pt <= 0 || top_fj_maxDR > 1.8)) && (fj_pt >= 200 &&  fj_eta < 2.4 && fj_subjet_min_pt > 20 && fj_subjet_mass > 50 && fj_subjet_mass < 600 && fj_subjet_pt > 200)");

    addFactory(TMVA::Types::kBDT,"BDTG","!H:!V:NTrees=1000:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:nCuts=200");
//        addFactory(TMVA::Types::kBDT,"BDTG","");

//    addFactory(TMVA::Types::kMLP,"MLP_ANN","!H:!V:HiddenLayers=16,15");
    addAxis(new BinnedSpace("pt", "pt", "10, 1000"));
setWeightExpr("weight");
    setConfiguration(mvaName,"TopDisc","topXML");

  }

  void process() {
    BaseTMVAProducer::process();
  }

  TTree * sigTree;
  TTree * bkgTree;
  string mvaName;
};




void produceBoostedTopMVA(string sigFileName = "trees/T2tt_850_100_HTT1p5_0_tree.root", string bkgFileName = "trees/SM_HTT1p5.root", string treeName = "Events", string mvaName = "mva", string outFileName ="topDisc_new.root") {
  TFile * sigFile = new TFile(sigFileName.c_str(),"read");
  TFile * bkgFile = new TFile(bkgFileName.c_str(),"read");
  TTree * sigTree = 0;
  TTree * bkgTree = 0;
  sigFile->GetObject(treeName.c_str(),sigTree);
  bkgFile->GetObject(treeName.c_str(),bkgTree);
  assert(sigTree);
  assert(bkgTree);

  Producer a(outFileName,sigTree,bkgTree,mvaName);
  a.process();
}
