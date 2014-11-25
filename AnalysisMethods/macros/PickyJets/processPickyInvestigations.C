#include <vector>
#include <assert.h>
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TH1F.h"
#include "TCanvas.h"
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "PickyHelper.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"

#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"

#include "AnalysisTools/Parang/interface/Plotter.h"
#include "AnalysisTools/Parang/interface/Polybook.h"
#include "AnalysisTools/Parang/interface/Panvariate.h"

using namespace std;
using namespace ucsbsusy;

class Analyze : public BaseTreeAnalyzer{
public:
  PlotterD*                       plotter;
  Polybook                        eventPlots;
  TString                         prefix;
  PickyReader                     pickyReader;

  Analyze(TString fname, string treeName, bool isMCTree) : BaseTreeAnalyzer(fname,treeName, isMCTree), plotter (new PlotterD(3)), eventPlots(plotter)
  {
  };

  virtual void loadVariables(){
    load(EVTINFO);
    load(GENPARTICLES);
    reader.load(&pickyReader,1,"CA1");
  }
  virtual void processVariables(){
    goodParts.clear();
    vetoParts.clear();
    lepParts.clear();

    SplitVars::getParts(genParticleReader.genParticles.particles,goodParts,vetoParts,lepParts);

    if(evtInfoReader.isLoaded()) {
      run   = evtInfoReader.run;
      lumi  = evtInfoReader.lumi;
      event = evtInfoReader.event;
      nPV   = evtInfoReader.nPV;
      rho   = evtInfoReader.rho;
      met   = &evtInfoReader.met;
    }
  }


  void makePlots(vector<splitSet>& splits, bool isReco){
    SplitVars::categorize(splits,goodParts,vetoParts);
    eventPlots.rewind();

    eventPlots("gen__",!isReco);
    eventPlots("reco__",isReco);

    ++eventPlots;
    eventPlots("none__", true);
    eventPlots("npv_nPV_incl__"    , !isReco);
    eventPlots("npv_nPV_lt15__"    , isReco && nPV < 15);
    eventPlots("npv_nPV_eq15to22__", isReco && nPV >= 15 && nPV <22);
    eventPlots("npv_nPV_geq22__"   , isReco && nPV >= 22);
    eventPlots.checkPoint(1);

//    cout <<" --> "<< (isReco ? "Reco" : "Gen") <<endl;

    for(const splitSet& s : splits){
      if(TMath::Abs(s.eta()) > 2.4) continue;
      eventPlots.revert(1);
      ++eventPlots;
      eventPlots("none__", true);
      eventPlots("type_single__", s.type == SINGLE);
      eventPlots("type_multi__", s.type == MULTI);
//      eventPlots("type_other__" , s.type != DOUBLE && s.type != SINGLE);

      ++eventPlots;
      eventPlots("none__",true);
      eventPlots("pt_pt_lt50__",s.superJet.pt() < 50);
      eventPlots("pt_pt_eq50to100__",s.superJet.pt() >= 50 && s.superJet.pt() < 100);
      eventPlots("pt_pt_eq100to200__",s.superJet.pt() >= 100 && s.superJet.pt() < 200);
      eventPlots("pt_pt_geq200__",s.superJet.pt() >= 200);

      ++eventPlots;
      eventPlots("inclusive__", true);
      eventPlots("split__", false);

      eventPlots.fill(s.superJet.pt(), 1, "superJetPT"           , ";Super jet p_{T}"                , 60, 0, 300);
      eventPlots.fill(s.tau1         , 1, "1_subjettiness"       , ";#tau_{1}"                       , 30, 0, 1);
      eventPlots.fill(s.tau2         , 1, "2_subjettiness"       , ";#tau_{2}"                       , 30, 0, 1);
      eventPlots.fill(s.tau1  == 0 ? 10 : s.tau2/s.tau1                 ,1, "2o1_subjettiness"       , ";#tau_{2}/#tau_{1}"                       , 30, 0, 2   );

      eventPlots.revert(1);
      ++eventPlots;
      eventPlots("none__", true);
      eventPlots("type_single__", s.type == SINGLE);
      eventPlots("type_multi__", s.type == MULTI);
//      eventPlots("type_other__" , s.type != DOUBLE && s.type != SINGLE);

      ++eventPlots;
      eventPlots("none__",true);
      eventPlots("pt_pt_lt50__",s.superJet.pt() < 50);
      eventPlots("pt_pt_eq50to100__",s.superJet.pt() >= 50 && s.superJet.pt() < 100);
      eventPlots("pt_pt_eq100to200__",s.superJet.pt() >= 100 && s.superJet.pt() < 200);
      eventPlots("pt_pt_geq200__",s.superJet.pt() >= 200);


      ++eventPlots;
      eventPlots("inclusive__", false);
      eventPlots("split__",  s.subJet1.pt() > 0 );


       eventPlots.fill(s.superJet.pt()       , 1, "superJetPT"           , ";Super jet p_{T}"                , 60, 0, 300 );
       eventPlots.fill(s.subJet1.pt()         ,1, "subJet1PT"            , ";Subjet p_{T}"                   , 30,  0, 200);
       eventPlots.fill(s.subJet2.pt()         ,1, "subJet1PT"            , ";Subjet p_{T}"                   , 30,  0, 200);

       eventPlots.fill(s.superJet.mass()      ,1, "jet_mass"             , ";M(Super jet)}"                  , 30, 0, 200 );
       eventPlots.fill(s.tau1                 ,1, "1_subjettiness"       , ";#tau_{1}"                       , 30, 0, 1   );
       eventPlots.fill(s.tau2                 ,1, "2_subjettiness"       , ";#tau_{2}"                       , 30, 0, 1   );

       eventPlots.fill(s.tau1  == 0 ? 10 : s.tau2/s.tau1                 ,1, "2o1_subjettiness"       , ";#tau_{2}/#tau_{1}"                       , 30, 0, 2   );

       eventPlots.fill(s.subJetDR             ,1, "sub12_dr"             , ";DR(subjet 1, subjet 2)"         , 30, 0, 1.2 );
       eventPlots.fill(s.highest_peak         ,1, "highest_peak"         , ";Highest peak"                   , 60, 0, 2000);
       eventPlots.fill(s.lowest_peak          ,1, "lowest_peak"          , ";Lowest peak"                    , 30, 0, 500 );
       eventPlots.fill(s.minimum_value        ,1, "minimum_value"        , ";Minimum value"                  , 30, 0, 200 );
       eventPlots.fill(s.lowest_peak_location ,1, "lowest_peak_location" , ";Lowest peak location"           , 30, -1, 1  );
       eventPlots.fill(s.highest_peak_location,1, "highest_peak_location", ";Highest peak location"          , 30, -1, 1  );
       eventPlots.fill(s.minimum_location     ,1, "minimum_location"     , ";Minimum location"               , 30, -1, 1  );

       eventPlots.fill(s.splitDisc            ,1, "shouldSplitDisc"      , ";D_{splitting}"                  , 20, -1, 1  );
       eventPlots.fill(s.shouldSplit          ,1, "shouldSplitDescision" , ";Split(No/Yes)"                  , 2, -.5, 1.5  );
    }


  }

  void runEvent(){
    if(nPV < 1) return;
//    cout << goodParts.size()  <<" "<< lepParts.size() <<" "<< vetoParts.size() <<endl;
    if(lepParts.size() > 0) return;
    int nParts = 0;
    for(unsigned int iP = 0; iP < goodParts.size(); ++iP){
      if(TMath::Abs(goodParts[iP]->eta()) > 2.4 ) continue;
      if(TMath::Abs(goodParts[iP]->pt()) < 20 ) continue;
      nParts++;
    }
//    cout << goodParts.size()  <<" "<< nParts<< " "<<vetoParts.size()<<  " "<<pickyReader.recoSplits.size() << " "<<pickyReader.genSplits.size()<< endl;
//    if(nParts < 6) return;

    makePlots(pickyReader.genSplits,false);
    makePlots(pickyReader.recoSplits,true);

  }

  void out(TString outputPath){
    plotter->write(outputPath);
  }

  vector<GenParticleF*> goodParts;
  vector<GenParticleF*> vetoParts;
  vector<GenParticleF*> lepParts;

};


#endif



void processPickyInvestigations(string fname = "evttree.root", string treeName = "analyzer/Events", bool isMCTree = true) {

  Analyze a(fname, treeName, isMCTree);
  TString name = fname;
  name = ((TObjString*)name.Tokenize(".")->At(0))->GetString();
  a.prefix = name;
  a.prefix += "_";
  a.analyze(1000);
  a.out(TString::Format("%s_plots.root",name.Data()));
}
