#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/BaseTMVAProducer.h"

using namespace std;
using namespace ucsbsusy;

class Producer : public BaseTMVAProducer {
public:
  Producer(TString outputFileName,TTree* inTree, bool doReco) : BaseTMVAProducer(outputFileName), inputTree(inTree), reco(doReco) {};
  virtual ~Producer() {};

  void  loadVars() {
    addVariable("rho","rho",'F');
    addVariable("npv","npv",'F');
    if(reco){
      addVariable("betaStar" ,"ak4pfchs_jet_betaStar",'F');
      addVariable("ptD"      ,"ak4pfchs_jet_ptD"     ,'F');
      addVariable("axis1"    ,"ak4pfchs_jet_axis1"   ,'F');
      addVariable("axis2"    ,"ak4pfchs_jet_axis2"   ,'F');
      addVariable("jetMult"  ,"ak4pfchs_jet_jetMult" ,'I');
      addVariable("pt"       ,"ak4pfchs_jet_pt"      ,'F');
      addVariable("eta"      ,"ak4pfchs_jet_eta"     ,'F');
    } else {
      addVariable("ptD"      ,"ak4pfchs_genjet_ptD"    ,'F');
      addVariable("axis1"    ,"ak4pfchs_genjet_axis1"  ,'F');
      addVariable("axis2"    ,"ak4pfchs_genjet_axis2"  ,'F');
      addVariable("jetMult"  ,"ak4pfchs_genjet_jetMult",'I');
      addVariable("pt"       ,"ak4pfchs_genjet_pt"     ,'F');
      addVariable("eta"      ,"ak4pfchs_genjet_eta"    ,'F');
    }
    addVariable("flavor","flavor",'I');

    addVariableSet("ptD"     ,"axis1"   ,"axis2"   ,"jetMult" ,"pt"      ,"eta","");
    if(reco)
      addVariableSet("ptD"     ,"axis1"   ,"axis2"   ,"jetMult" ,"pt"      ,"eta","rho","npv","");
  }

  void configure(){
    addInputTree(inputTree,"Quark","flavor == 0");
    addInputTree(inputTree,"Gluon","flavor == 1");

    addFactory(TMVA::Types::kBDT,"BDTG");
    addAxis(new BinnedSpace("rho", "rho", "0,9999"));
    setConfiguration("QG","QurakGluonDisc", reco ? "qgXML_reco" : "qgXML_gen");
  }

  TTree * inputTree;
  bool reco;
};


#endif



void produceQGMVA(string fileName = "evttree.root", string treeName = "Events", string outFileName ="QGDisc.root", bool doReco = true) {
  TFile * inFile = new TFile(fileName.c_str(),"read");
  TTree * inTree = 0;
  inFile->GetObject(treeName.c_str(),inTree);
  assert(inTree);

  Producer a(outFileName,inTree,doReco);
  a.process();
}
