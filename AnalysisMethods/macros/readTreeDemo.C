#if !defined(__CINT__) || defined(__MAKECINT__)
#include <vector>
#include <assert.h>
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "AnalysisTools/TreeReader/interface/BaseTreeAnalyzer.h"

using namespace ucsbsusy;

class Analyze : public BaseTreeAnalyzer{
public:
  Analyze(TString fname, string treeName, bool isMCTree) : BaseTreeAnalyzer(fname,treeName, isMCTree)  {
    load(AK4JETS, JetReader::LOADRECO | JetReader::FILLOBJ);
    load(ELECTRONS);
    load(MUONS);
    load(TAUS);
  }

  void run(){
    for(const auto& jet : (*ak4Jets)){
      cout << "Jet " << ": pt = " << jet.pt() << "; eta = " << jet.eta() << "; phi = " << jet.phi()<<endl;
    }

    for(auto& electron : (*electrons)){
      cout << "Electron " << ": pt = " << electron.pt() << "; eta = " << electron.eta() << "; phi = " << electron.phi() <<  "; mvaID = " << electron.mvaidnontrig() << "; iso = " << electron.pfdbetaiso()/electron.pt() << endl;
    }

    for(auto& muon : (*muons)){
            cout << "Muon "  << ": pt = " << muon.pt() << "; eta = " << muon.eta() << "; phi = " << muon.phi() <<  "; isPFMuon = " << muon.ispfmuon() << "; iso = " << muon.pfdbetaiso()/muon.pt() << endl;
    }

    for(auto& tau : (*taus)){
            bool passantimutight = (tau.hpsid() & kTightMuMVA);
            bool passantielemedium = (tau.hpsid() & kMediumEleMVA);
            bool passmvaloose = (tau.hpsid() & kLooseIsoMVALT);
            cout << "Tau " <<": pt = " << tau.pt() << "; eta = " << tau.eta() << "; phi = " << tau.phi() <<  "; dxysig = " << tau.dxysig() << "; anti-muon tight rejection = " << passantimutight << "; anti-electron medium rejection = " << passantielemedium << "; pass mva iso (with lifetime) loose = " << passmvaloose << endl;
    }
  }
};


#endif



void readTreeDemo(string fname = "$CMSSW_BASE/src/AnalysisBase/Analyzer/test/evttree.root", string treeName = "TestAnalyzer/Events", bool isMCTree = false) {

  Analyze a(fname, treeName, isMCTree);
  while(a.nextEvent(true)) a.run();
}
