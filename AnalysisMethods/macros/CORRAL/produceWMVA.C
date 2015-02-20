#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/BaseTMVAProducer.h"

using namespace std;
using namespace ucsbsusy;

class Producer : public BaseTMVAProducer {
public:
  Producer(TString outputFileName,TTree* inTree, string inMvaName) : BaseTMVAProducer(outputFileName), inputTree(inTree), mvaName(inMvaName){};
  virtual ~Producer() {};

  void  loadVars() {

     addVariable("wPT"        ,"wPT"        ,'F');
     addVariable("mass"       ,"mass"       ,'F');
     addVariable("pt2opt1"    ,"pt2opt1"    ,'F');
     addVariable("wJetLikli1" ,"wJetLikli1" ,'F');
     addVariable("wJetLikli2" ,"wJetLikli2" ,'F');
     addVariable("maxCSV"     ,"maxCSV"     ,'F');
     addVariable("dr"         ,"dr"         ,'F');
     addVariable("deta"       ,"deta"       ,'F');
     addVariable("dphi"       ,"dphi"       ,'F');
     addVariable("charge"     ,"charge"     ,'F');
     addVariable("pullAng1"   ,"pullAng1"   ,'F');
     addVariable("pullAng2"   ,"pullAng2"   ,'F');
     addVariable("nWCon"      ,"nWCon"      ,'I');

     addVariableSet(
         "wPT"        ,
         "mass"       ,
         "pt2opt1"    ,
         "wJetLikli1" ,
         "wJetLikli2" ,
         "maxCSV"     ,
         "dr"         ,
         "deta"       ,
         "dphi"       ,
//         "charge"     ,
//         "pullAng1"   ,
//         "pullAng2"   ,
         "nWCon"      ,
         ""

     );

  }

  void configure(){
    addInputTree(inputTree,"W","isW == 1");
    addInputTree(inputTree,"nonW","isW == 0");

    addFactory(TMVA::Types::kBDT,"BDTG");
    addAxis(new BinnedSpace("wPT", "wPT", "0,100,200,300,800"));

    setConfiguration(mvaName,"WDisc","WDiscXML");
  }

  TTree * inputTree;
  string mvaName;
};


#endif



void produceWMVA(string fileName = "evttree.root", string treeName = "Events", string mvaName = "mva", string outFileName ="QGDisc.root") {
  TFile * inFile = new TFile(fileName.c_str(),"read");
  TTree * inTree = 0;
  inFile->GetObject(treeName.c_str(),inTree);
  assert(inTree);

  Producer a(outFileName,inTree,mvaName);
  a.process();
}
