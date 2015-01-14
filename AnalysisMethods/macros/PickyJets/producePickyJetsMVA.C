#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/BaseTMVAProducer.h"

using namespace std;
using namespace ucsbsusy;

class Producer : public BaseTMVAProducer {
public:
  Producer(TString outputFileName,TTree* inTree, string inMvaName) : BaseTMVAProducer(outputFileName), inputTree(inTree), mvaName(inMvaName){};
  virtual ~Producer() {};

  void  loadVars() {
     addVariable("superJet_mass"           ,"superJet_mass"         ,'F');
     addVariable("tau1"                    ,"tau1"                  ,'F');
     addVariable("tau2"                    ,"tau2"                  ,'F');
     addVariable("highest_peak"            ,"highest_peak"          ,'F');
     addVariable("lowest_peak"             ,"lowest_peak"           ,'F');
     addVariable("minimum_value"           ,"minimum_value"         ,'F');
     addVariable("lowest_peak_location"    ,"lowest_peak_location"  ,'F');
     addVariable("highest_peak_location"   ,"highest_peak_location" ,'F');
     addVariable("minimum_location"        ,"minimum_location"      ,'F');
     addVariable("subjet_dr"               ,"subjet_dr"             ,'F');

    addVariableSet("superJet_mass","tau1","tau2","highest_peak","lowest_peak","minimum_value","lowest_peak_location","highest_peak_location","minimum_location","subjet_dr","");

  }

  void configure(){
    addInputTree(inputTree,"GoodSplits","splitResult == 0");
    addInputTree(inputTree,"BadSplits","splitResult > 0");

    addFactory(TMVA::Types::kBDT,"BDTG");
    addAxis(new BinnedSpace("superJet_pt", "superJet_pt", "20, 40, 60, 80, 100, 120, 160, 200, 250, 300, 400, 550, 1000"));
    addAxis(new BinnedSpace("superJet_eta", "superJet_eta", "0, 1.9, 2.9, 6"));

    setConfiguration(mvaName,"PickyJetDisc","pickyXML");
  }

  TTree * inputTree;
  string mvaName;
};


#endif



void producePickyJetsMVA(string fileName = "evttree.root", string treeName = "Events", string mvaName = "mva", string outFileName ="QGDisc.root") {
  TFile * inFile = new TFile(fileName.c_str(),"read");
  TTree * inTree = 0;
  inFile->GetObject(treeName.c_str(),inTree);
  assert(inTree);

  Producer a(outFileName,inTree,mvaName);
  a.process();
}
