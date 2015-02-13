#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/BaseTMVAProducer.h"

using namespace std;
using namespace ucsbsusy;

class Producer : public BaseTMVAProducer {
public:
  Producer(TString outputFileName,TTree* inTree, bool doReco, bool usePuppi) : BaseTMVAProducer(outputFileName), inputTree(inTree), reco(doReco), usePuppi(usePuppi) {};
  virtual ~Producer() {};

  void  loadVars() {
    addVariable("rho","rho",'F');
    addVariable("npv","npv",'F');
    if(reco){
      if(usePuppi) {
		  addVariable("betaStar" ,"ak4pfpuppi_jet_betaStar",'F');
		  addVariable("ptD"      ,"ak4pfpuppi_jet_ptD"     ,'F');
		  addVariable("axis1"    ,"ak4pfpuppi_jet_axis1"   ,'F');
		  addVariable("axis2"    ,"ak4pfpuppi_jet_axis2"   ,'F');
		  addVariable("jetMult"  ,"ak4pfpuppi_jet_jetMult" ,'I');
		  addVariable("blf0"     ,"ak4pfpuppi_jet_blf0"    ,'F');
		  addVariable("blf1"     ,"ak4pfpuppi_jet_blf1"    ,'F');
		  addVariable("blf2"     ,"ak4pfpuppi_jet_blf2"    ,'F');
		  addVariable("pt"       ,"ak4pfpuppi_jet_pt"      ,'F');
		  addVariable("eta"      ,"ak4pfpuppi_jet_eta"     ,'F');
      } // usePuppi
      else {
		  addVariable("betaStar" ,"ak4pfchs_jet_betaStar",'F');
		  addVariable("ptD"      ,"ak4pfchs_jet_ptD"     ,'F');
		  addVariable("axis1"    ,"ak4pfchs_jet_axis1"   ,'F');
		  addVariable("axis2"    ,"ak4pfchs_jet_axis2"   ,'F');
		  addVariable("jetMult"  ,"ak4pfchs_jet_jetMult" ,'I');
		  addVariable("blf0"     ,"ak4pfchs_jet_blf0"    ,'F');
		  addVariable("blf1"     ,"ak4pfchs_jet_blf1"    ,'F');
		  addVariable("blf2"     ,"ak4pfchs_jet_blf2"    ,'F');
		  addVariable("pt"       ,"ak4pfchs_jet_pt"      ,'F');
		  addVariable("eta"      ,"ak4pfchs_jet_eta"     ,'F');
      } // !usePuppi
    } // reco
    else {
      addVariable("ptD"      ,"ak4pfchs_genjet_ptD"    ,'F');
      addVariable("axis1"    ,"ak4pfchs_genjet_axis1"  ,'F');
      addVariable("axis2"    ,"ak4pfchs_genjet_axis2"  ,'F');
      addVariable("jetMult"  ,"ak4pfchs_genjet_jetMult",'I');
      addVariable("blf0"     ,"ak4pfchs_genjet_blf0"   ,'F');
      addVariable("blf1"     ,"ak4pfchs_genjet_blf1"   ,'F');
      addVariable("blf2"     ,"ak4pfchs_genjet_blf2"   ,'F');
      addVariable("pt"       ,"ak4pfchs_genjet_pt"     ,'F');
      addVariable("eta"      ,"ak4pfchs_genjet_eta"    ,'F');
    } // gen
    addVariable("flavor","flavor",'I');

    addVariableSet("ptD", "axis1", "axis2", "jetMult", "");
    //addVariableSet("ptD", "axis1", "axis2", "blf0", "blf1", "blf2", "");
    //addVariableSet( "blf0", "blf1", "blf2", "");
    //addVariableSet("ptD", "axis1", "axis2", "jetMult", "blf0", "blf1", "blf2", "");
  }

  void configure(){
    addInputTree(inputTree,"Quark","flavor == 0");
    addInputTree(inputTree,"Gluon","flavor == 1");

    addFactory(TMVA::Types::kBDT,"BDTG");
    if (reco) {
    	// loosely binned test
		//if (usePuppi) addAxis( new BinnedSpace("ak4pfpuppi_jet_pt",  "ak4pfpuppi_jet_pt",  "0,30,50,100,300") );
		//else          addAxis( new BinnedSpace("ak4pfchs_jet_pt",    "ak4pfchs_jet_pt",    "0,30,50,100,300") );
		//addAxis(               new BinnedSpace("ak4pfchs_jet_eta",   "ak4pfchs_jet_eta",   "0,2.4,3,4.7")     );
    	// JetMET bins
		if (usePuppi) addAxis( new BinnedSpace("ak4pfpuppi_jet_pt",  "ak4pfpuppi_jet_pt",  "0,20,26,32,40,51,64,80,101,127,159,201,252,317,400,503,633,797,1003,1262,1589,2000,4000") );
		else          addAxis( new BinnedSpace("ak4pfchs_jet_pt",    "ak4pfchs_jet_pt",    "0,20,26,32,40,51,64,80,101,127,159,201,252,317,400,503,633,797,1003,1262,1589,2000,4000") );
		addAxis(               new BinnedSpace("ak4pfchs_jet_eta",   "ak4pfchs_jet_eta",   "0,2.4,5")     );
    } // reco
    else {
    	// loosely binned test
		//addAxis( new BinnedSpace("ak4pfchs_genjet_pt",  "ak4pfchs_genjet_pt",  "0,30,50,100,300") );
		//addAxis( new BinnedSpace("ak4pfchs_genjet_eta", "ak4pfchs_genjet_eta", "0,2.4,3,4.7")     );
    	// JetMET bins
    	addAxis( new BinnedSpace("ak4pfchs_genjet_pt",  "ak4pfchs_genjet_pt",  "0,20,26,32,40,51,64,80,101,127,159,201,252,317,400,503,633,797,1003,1262,1589,2000,4000") );
		addAxis( new BinnedSpace("ak4pfchs_genjet_eta", "ak4pfchs_genjet_eta", "0,2.4,5")     );
    } // !reco
    setConfiguration("QG","QurakGluonDisc", reco ? "qgXML_reco" : "qgXML_gen");
  }

  TTree * inputTree;
  bool reco;
  bool usePuppi;
};


#endif



void produceQGMVA(string fileName = "newTree_gen_ak4_test.root", string treeName = "TestAnalyzer/Events", string outFileName ="QGDisc_reco_puppi_test.root", bool doReco = true, bool usePuppi = true ) {
  TFile * inFile = new TFile(fileName.c_str(),"read");
  TTree * inTree = 0;
  inFile->GetObject(treeName.c_str(),inTree);
  assert(inTree);

  Producer a(outFileName,inTree,doReco,usePuppi);
  a.process();
}
