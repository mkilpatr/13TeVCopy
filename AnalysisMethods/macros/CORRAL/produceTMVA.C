#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/BaseTMVAProducer.h"

using namespace std;
using namespace ucsbsusy;

class Producer : public BaseTMVAProducer {
public:
  Producer(TString outputFileName,TTree* inTree, string inMvaName) : BaseTMVAProducer(outputFileName), inputTree(inTree), mvaName(inMvaName){};
  virtual ~Producer() {};

  void  loadVars() {

    addVariable("tPT"           ,"tPT"         ,'F');
    addVariable("wPT"           ,"wPT"         ,'F');
    addVariable("tMass"         ,"tMass"       ,'F');
    addVariable("wMass"         ,"wMass"       ,'F');
    addVariable("bPTotPT"       ,"bPTotPT"     ,'F');
    addVariable("bCSV"          ,"bCSV"        ,'F');
    addVariable("maxWCSV"       ,"maxWCSV"     ,'F');
    addVariable("bWLikli"       ,"bWLikli"     ,'F');
    addVariable("wDisc"         ,"wDisc"       ,'F');
    addVariable("maxOWDisc"     ,"maxOWDisc"   ,'F');
    addVariable("m23om123"      ,"m23om123"    ,'F');
    addVariable("m13om12"       ,"m13om12"     ,'F');
    addVariable("atan_m13om12"  ,"atan_m13om12",'F');
    addVariable("maxjjdr"       ,"maxjjdr"     ,'F');
    addVariable("wbDR"          ,"wbDR"        ,'F');
    addVariable("wbDEta"        ,"wbDEta"      ,'F');
    addVariable("wbDPhi"        ,"wbDPhi"      ,'F');
    addVariable("nTCon"         ,"nTCon"       ,'I');

    addVariableSet(
        "tPT"          ,
        "wPT"          ,
        "tMass"        ,
        "wMass"        ,
        "bPTotPT"      ,
        "bCSV"         ,
        "maxWCSV"      ,
        "bWLikli"      ,
        "wDisc"        ,
        "maxOWDisc"    ,
        "m23om123"     ,
        "m13om12"      ,
        "atan_m13om12" ,
        "maxjjdr"      ,
        "wbDR"         ,
        "wbDEta"       ,
        "wbDPhi"       ,
        "nTCon"        ,

       "");


  }

  void configure(){
    addInputTree(inputTree,"T","isT == 1");
    addInputTree(inputTree,"nonT","isT == 0");

    addFactory(TMVA::Types::kBDT,"BDTG");
    addAxis(new BinnedSpace("tPT", "tPT", "0,150,300,500,1000"));

    setConfiguration(mvaName,"tDisc","tDiscXML");
  }

  TTree * inputTree;
  string mvaName;
};


#endif



void produceTMVA(string fileName = "evttree.root", string treeName = "Events", string mvaName = "mva", string outFileName ="QGDisc.root") {
  TFile * inFile = new TFile(fileName.c_str(),"read");
  TTree * inTree = 0;
  inFile->GetObject(treeName.c_str(),inTree);
  assert(inTree);

  Producer a(outFileName,inTree,mvaName);
  a.process();
}
