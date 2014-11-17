#include <vector>
#include <assert.h>
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TH1F.h"
#include "TCanvas.h"
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "QGHelper.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"

#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"

#include "AnalysisTools/Parang/interface/Plotter.h"
#include "AnalysisTools/Parang/interface/Polybook.h"

using namespace std;
using namespace ucsbsusy;

class Analyze : public BaseTreeAnalyzer{
public:
  PlotterD*                       plotter;
  Polybook                        eventPlots;
  TString                         prefix;

  Analyze(TString fname, string treeName, bool isMCTree) : BaseTreeAnalyzer(fname,treeName, isMCTree), plotter (new PlotterD(3)), eventPlots(plotter) {};

  virtual void loadVariables(){
    load(EVTINFO);
    load(AK4JETS,JetReader::LOADRECO | JetReader::LOADGEN | JetReader::LOADJETSHAPE | JetReader::FILLOBJ);
    load(GENPARTICLES);
  }

    std::vector<subJet> genJets ;
    std::vector<subJet> recoJets ;

  virtual void processVariables(){
    procesSubjets(recoJets,genJets,this);
  }
  void makePlots(std::vector<subJet>& jets, bool isReco){
    categorize(jets,genParts,isReco);
    eventPlots.rewind();
    eventPlots("",true);

    eventPlots.checkPoint(0);
    if(isReco) eventPlots.fill(nPV    ,1,"nPV"     ,";# of prim. vert."           ,40,-0.5,39.5);
    eventPlots.revert(0);

    ++eventPlots;
    eventPlots("gen__",!isReco);
    eventPlots("reco__",isReco);

    ++eventPlots;
    eventPlots("none__", true);
    eventPlots("npv_nPV_incl__"    , !isReco);
    eventPlots("npv_nPV_lt15__"    , isReco && nPV < 15);
    eventPlots("npv_nPV_eq15to22__", isReco && nPV >= 15 && nPV <22);
    eventPlots("npv_nPV_geq22__"   , isReco && nPV >= 22);

    eventPlots.checkPoint(1);

    for(const auto& j : jets){
      eventPlots.revert(1);
      ++eventPlots;

      eventPlots("nonPU__", j.type != PU);
      eventPlots("def_uds_quark__", j.type == Q);
      eventPlots("def_gluon__", j.type == G);
      eventPlots("def_b_quark__", j.type == B);
      eventPlots("def_pu_jet__", j.type == PU);
      eventPlots("def_undefined__", j.type == U);
      eventPlots("def_c_quark__", j.type == C);

      eventPlots.fill(j.jet->pt()  ,1,"pt"     ,";p_{t}"           ,100,0,1000);
      eventPlots.fill(j.ptD     ,1,"ptD"     ,";p_{t}(D)"           ,50,0,1);
      eventPlots.fill(j.axis1   ,1,"axis1"   ,";axis_{1}"           ,50,0,.4);
      eventPlots.fill(j.axis2   ,1,"axis2"   ,";axis_{2}"           ,50,0,.4);
      eventPlots.fill(j.mult    ,1,"mult"    ,";# of part."         ,100,-0.5,99.5);
      eventPlots.fill(j.nearDR  ,1,"nearDR"  ,";#DeltaR(jet,parton)",50,0,.4);

      if(isReco){
        eventPlots.fill(j.betaStar,1,"betaStar",";betaStar"           ,50, 0,.3);
        eventPlots.fill(j.betaStar / (.2 * TMath::Log(float(nPV) - .67)),1,"betaStar_o_cut",";betaStar/ln(nPV - .67)"           ,50, 0,.3);
        eventPlots.fill(j.qgl     ,1,"qgl"     ,";Q/G likili"         ,50,0,1);
        eventPlots.fill(j.csv     ,1,"csv"     ,";D_{b}(csv)"         ,50,0,1);
      }

    }


  }

  void runEvent(){
    if(nPV < 1) return;

    makePlots(genJets,false);
    makePlots(recoJets,true);

  }

  void out(TString outputPath){
    plotter->write(outputPath);
  }
};


#endif



void processQGInvestigations(string fname = "evttree.root", string treeName = "TestAnalyzer/Events", bool isMCTree = true) {

  Analyze a(fname, treeName, isMCTree);
  TString name = fname;
  name = ((TObjString*)name.Tokenize(".")->At(0))->GetString();
  a.prefix = name;
  a.prefix += "_";
  a.analyze();
  a.out(TString::Format("%s_plots.root",name.Data()));
}
