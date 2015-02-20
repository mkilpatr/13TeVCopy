#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/BaseTMVAProducer.h"

using namespace std;
using namespace ucsbsusy;

class Producer : public BaseTMVAProducer {
public:
  Producer(TString outputFileName,TTree* inSigTree, TTree* inBkgTree, string inMvaName) : BaseTMVAProducer(outputFileName), sigTree(inSigTree), bkgTree(inBkgTree), mvaName(inMvaName){};
  virtual ~Producer() {};

  void  loadVars() {
     addVariable("pt"           ,"pt"           ,'F');
     addVariable("abseta"       ,"abseta"       ,'F');
     addVariable("absdz"        ,"absdz"        ,'F');
     addVariable("chreliso0p1"  ,"chreliso0p1"  ,'F');
     addVariable("chreliso0p2"  ,"chreliso0p2"  ,'F');
     addVariable("chreliso0p3"  ,"chreliso0p3"  ,'F');
     addVariable("chreliso0p4"  ,"chreliso0p4"  ,'F');
     addVariable("totreliso0p1" ,"totreliso0p1" ,'F');
     addVariable("totreliso0p2" ,"totreliso0p2" ,'F');
     addVariable("totreliso0p3" ,"totreliso0p3" ,'F');
     addVariable("totreliso0p4" ,"totreliso0p4" ,'F');
     addVariable("neartrkdr"    ,"neartrkdr"    ,'F');
     addVariable("contjetdr"    ,"contjetdr"    ,'F');
     addVariable("contjetcsv"   ,"contjetcsv"   ,'F');

     addVariableSet("pt","abseta","absdz","chreliso0p1","chreliso0p2","chreliso0p3","chreliso0p4","totreliso0p1","totreliso0p2","totreliso0p3","totreliso0p4","neartrkdr","contjetdr","contjetcsv","");

  }

  void configure(){
    addInputTree(sigTree,"GoodTaus","gentaumatch==1 && nGenLeptons==0 && nGenTaus==nGenHadTaus && nGenHadTaus>0 && njets>3 && misset>150 && mt < 100 && pt>10");
    addInputTree(bkgTree,"FakeTaus","gentaumatch==0 && nGenLeptons==0 && nGenTaus==0 && njets>3 && misset>150 && mt < 100 && pt>10");

    addFactory(TMVA::Types::kBDT,"BDTG");
    addAxis(new BinnedSpace("pt", "pt", "10, 1000"));

    setConfiguration(mvaName,"TauDisc","tauXML");
  }

  TTree * sigTree;
  TTree * bkgTree;
  string mvaName;
};


#endif



void produceTauMVA(string sigFileName = "../run/plots/ttbar_plots.root", string bkgFileName = "../run/plots/T2tt_650_325_plots.root", string treeName = "Candidates", string mvaName = "mva", string outFileName ="tauDisc_10GeV.root") {
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
