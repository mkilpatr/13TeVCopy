#include <vector>
#include <assert.h>
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TH1F.h"
#include "TCanvas.h"
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisTools/Utilities/interface/JetFlavorInfo.h"

#include "AnalysisTools/Parang/interface/Plotter.h"
#include "AnalysisTools/Parang/interface/Polybook.h"
#include "AnalysisTools/Parang/interface/Panvariate.h"

#include "AnalysisTools/KinematicVariables/interface/Topness.h"
using namespace std;
using namespace ucsbsusy;

class Analyze : public BaseTreeAnalyzer{
public:
  PlotterD*                       plotter;
  Polybook                        eventPlots;


  Analyze(TString fname, string treeName, bool isMCTree, ConfigPars * pars) : BaseTreeAnalyzer(fname,treeName, isMCTree,pars), plotter (new PlotterD(3)), eventPlots(plotter)
  {
  };

  void loadVariables() override{
    load(EVTINFO);
    load(AK4JETS,JetReader::LOADRECO | JetReader::LOADJETSHAPE | JetReader::FILLOBJ);
    load(PICKYJETS,JetReader::LOADRECO | JetReader::LOADGEN | JetReader::LOADJETSHAPE | JetReader::FILLOBJ);
    load(ELECTRONS);
    load(MUONS);
    load(PFCANDS);
    load(GENPARTICLES);
  }



  void runEvent(){
//    TString prefix =defaults::PROCESS_NAMES[process];
//    prefix += "_";
    TString prefix = "";



    //Basic preselection
//    if(met->pt() < 200 || vars.nJ90 < 2) return;
//    if(vars.dPhiMET12 < .5 || vars.dPhiMET3 < .3) return;
//    if(vars.nTightBTags == 0) return;
    if(nJets < 6) return;
    if(nVetoedLeptons + nVetoedTaus > 0) return;

    eventPlots.rewind();
    bool                          classified    = false;


    eventPlots(""                 ,process == defaults::SIGNAL);
    eventPlots("t#bar{t}_plus_W__", !classified && (classified = (process == defaults::TTBAR)));
    eventPlots.checkPoint(0);

    for(auto * j : jets){
      const auto* gJ = j->genJet();
      if(gJ == 0) continue;
      if(TMath::Abs(j->eta()) > 2.4) continue;
      eventPlots.revert(0);
      ++eventPlots;
      eventPlots("pt_inc__", true);
      eventPlots("pt_eq20to50__", j->pt() >=20 && j->pt() < 50);
      eventPlots("pt_eq50to100__", j->pt() >=50 && j->pt() < 100);
      eventPlots("pt_geq100__", j->pt() >=100);

      ++eventPlots;
      eventPlots("b__",gJ->flavor() == JetFlavorInfo::b_jet);
      eventPlots("nonb__",gJ->flavor() != JetFlavorInfo::b_jet);

      eventPlots.fill(j->csv()             , weight, "csv"                , ";CSV"                            , 200,  -1,  1, prefix);

    }
  }


  void out(TString outputPath){
    plotter->write(outputPath);
  }
};




#endif



void testJetProperties(string fname = "evttree.root", string treeName = "Events", bool isMCTree = true) {

  BaseTreeAnalyzer::ConfigPars pars;
  pars.cleanJetsvSelectedLeptons_ = false;
  pars.leptonSelection = BaseTreeAnalyzer::SEL_0_LEP;
  pars.defaultJetCollection = BaseTreeAnalyzer::PICKYJETS;


  Analyze a(fname, treeName, isMCTree,&pars);
  TString name = fname;
  name = ((TObjString*)name.Tokenize(".")->At(0))->GetString();
  a.analyze(100000,-1);
  a.out(TString::Format("%s_plots.root",name.Data()));
}
