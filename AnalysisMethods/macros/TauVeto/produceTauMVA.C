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
     addVariable("pt"           ,"pt"           ,'F');
     addVariable("abseta"       ,"abseta"       ,'F');
     addVariable("chiso0p1"     ,"chiso0p1"     ,'F');
     addVariable("chiso0p2"     ,"chiso0p2"     ,'F');
     addVariable("chiso0p3"     ,"chiso0p3"     ,'F');
     addVariable("chiso0p4"     ,"chiso0p4"     ,'F');
     addVariable("totiso0p1"    ,"totiso0p1"    ,'F');
     addVariable("totiso0p2"    ,"totiso0p2"    ,'F');
     addVariable("totiso0p3"    ,"totiso0p3"    ,'F');
     addVariable("totiso0p4"    ,"totiso0p4"    ,'F');
     addVariable("neartrkdr"    ,"neartrkdr"    ,'F');
     addVariable("contjetdr"    ,"contjetdr"    ,'F');
     addVariable("contjetcsv"   ,"contjetcsv"   ,'F');

     addVariableSet("pt","abseta","chiso0p1","chiso0p2","chiso0p3","chiso0p4","totiso0p1","totiso0p2","totiso0p3","totiso0p4","neartrkdr","contjetdr","contjetcsv","");

  }

  void configure(){
    addInputTree(sigTree,"GoodTaus","gentaumatch==1 && nGenLeptons==0 && nGenTaus==nGenHadTaus && nGenHadTaus==1 && njets>3 && misset>150 && mt<100 && pt>10 && ptmatch > 6.");
    addInputTree(bkgTree,"FakeTaus","gentaumatch==0 && nGenLeptons==0 && nGenTaus==0 && njets>3 && misset>150 && mt<100 && pt>10");

    addFactory(TMVA::Types::kBDT,"BDTG");
    addAxis(new BinnedSpace("pt", "pt", "10, 1000"));

    setConfiguration(mvaName,"TauDisc","tauXML");
  }

  TTree * sigTree;
  TTree * bkgTree;
  string mvaName;
};




void produceTauMVA(string sigFileName = "run/plots/ttbar.root", string bkgFileName = "run/plots/T2tt_650_325.root", string treeName = "Candidates", string mvaName = "mva", string outFileName ="tauDisc_mt100presel.root") {
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
