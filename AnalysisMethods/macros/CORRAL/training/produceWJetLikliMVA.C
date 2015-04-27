#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/BaseTMVAProducer.h"

using namespace std;
using namespace ucsbsusy;

class Producer : public BaseTMVAProducer {
public:
  Producer(TString outputFileName,TTree* inTree, string inMvaName) : BaseTMVAProducer(outputFileName), inputTree(inTree), mvaName(inMvaName){};
  virtual ~Producer() {};

  void  loadVars() {

     addVariable("jetPT"    ,"jetPT"   , 'F');
     addVariable("axis1"    ,"axis1"   , 'F');
     addVariable("axis2"    ,"axis2"   , 'F');
     addVariable("ptD"      ,"ptD"     , 'F');
     addVariable("jetMult"  ,"jetMult" , 'I');
     addVariable("betaStar" ,"betaStar", 'F');
     addVariable("area"     ,"area"    , 'F');
     addVariable("mass"     ,"mass"    , 'F');
     addVariable("isWJet"     ,"isWJet"    , 'O');


    addVariableSet("jetPT","axis1","axis2","ptD","jetMult","betaStar","area","mass","");

  }

  void configure(){
    addInputTree(inputTree,"WJet","isWJet == 1");
    addInputTree(inputTree,"nonTopJet","isWJet == 0");

    addFactory(TMVA::Types::kBDT,"BDTG");
    addAxis(new BinnedSpace("jetPT", "jetPT", "20,40,60,100,200,400,800"));

    setConfiguration(mvaName,"WJetLikliDisc","WJetLikliDiscXML");
  }

  TTree * inputTree;
  string mvaName;
};


#endif



void produceWJetLikliMVA(string fileName = "wJetLikliTree.root", string treeName = "Events", string mvaName = "mva", string outFileName ="wJetLikliDisc.root") {
  TFile * inFile = new TFile(fileName.c_str(),"read");
  TTree * inTree = 0;
  inFile->GetObject(treeName.c_str(),inTree);
  assert(inTree);

  Producer a(outFileName,inTree,mvaName);
  a.process();
}
