#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/BaseTMVAProducer.h"

using namespace std;
using namespace ucsbsusy;

class Producer : public BaseTMVAProducer {
public:
  Producer(TString outputFileName,TTree* inTree, string inMvaName) : BaseTMVAProducer(outputFileName), inputTree(inTree), mvaName(inMvaName){};
  virtual ~Producer() {};

  void  loadVars() {
     addVariable("met"               ,"met"               ,'F');
     addVariable("num_j30"           ,"num_j30"           ,'I');
     addVariable("dphi_j3_met"       ,"dphi_j3_met"       ,'F');
     addVariable("num_medium_btags"  ,"num_medium_btags"  ,'I');
     addVariable("q2_likeli"         ,"q2_likeli"         ,'F');
     addVariable("q1_likeli"         ,"q1_likeli"         ,'F');
     addVariable("quark_likeli"      ,"quark_likeli"      ,'F');
     addVariable("ht_along_over_away","ht_along_over_away",'F');
     addVariable("rms_pt"            ,"rms_pt"            ,'F');
     addVariable("rms_dphi"          ,"rms_dphi"          ,'F');
     addVariable("bb_mass"           ,"bb_mass"           ,'F');
     addVariable("mTb"               ,"mTb"               ,'F');
     addVariable("deta_b_rms"        ,"deta_b_rms"        ,'F');
     addVariable("leading_jj_mass"   ,"leading_jj_mass"   ,'F');


    addVariableSet("met","num_j30","dphi_j3_met","num_medium_btags","q2_likeli","q1_likeli","quark_likeli","ht_along_over_away","rms_pt","rms_dphi","bb_mass","mTb","deta_b_rms","leading_jj_mass","");

  }

  void configure(){
    addInputTree(inputTree,"Signal","process == 10");
    addInputTree(inputTree,"Background","process != 10");

    addFactory(TMVA::Types::kBDT,"BDTG");
    addAxis(new BinnedSpace("met", "superJet_pt", "0, 999999"));

    setConfiguration(mvaName,"SR","srXML");
    setWeightExpr("weight");
  }

  TTree * inputTree;
  string mvaName;
};


#endif



void produceSRMVA(string fileName = "evttree.root", string treeName = "Events", string mvaName = "mva", string outFileName ="QGDisc.root") {
  TFile * inFile = new TFile(fileName.c_str(),"read");
  TTree * inTree = 0;
  inFile->GetObject(treeName.c_str(),inTree);
  assert(inTree);

  Producer a(outFileName,inTree,mvaName);
  a.process();
}
