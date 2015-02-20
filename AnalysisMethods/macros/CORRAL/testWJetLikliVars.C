#include <vector>
#include <assert.h>
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TH1F.h"
#include "TCanvas.h"
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisTools/Utilities/interface/TopJetMatching.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"

#include "AnalysisTools/Parang/interface/Plotter.h"
#include "AnalysisTools/Parang/interface/Polybook.h"
#include "AnalysisTools/Parang/interface/Panvariate.h"

#include "CORRALHelper.h"

using namespace std;
using namespace ucsbsusy;

class Analyze : public BaseTreeAnalyzer{
public:
  PlotterD*                       plotter;
  Polybook                        eventPlots;
  TString                         prefix;

  WJetLikliMVA  wJetLikliMVA;

  Analyze(TString fname, string treeName, bool isMCTree) : BaseTreeAnalyzer(fname,treeName, isMCTree), plotter (new PlotterD(3)), eventPlots(plotter)
  , wJetLikliMVA("T2tt_merged_wJetLikli_disc.root","mva_0")
  {
  };

  virtual void loadVariables(){
    load(EVTINFO);
    load(GENPARTICLES, GenParticleReader::FILLOBJ | GenParticleReader::LOADPARTONDECAY);
    load(PICKYJETS,JetReader::FILLOBJ | JetReader::LOADGEN | JetReader::LOADRECO | JetReader::LOADTOPASSOC | JetReader::LOADJETSHAPE );
  }


  void plotJets(JetReader * jetReader, TString prefix){
    std::vector<RecoJetF*> recoJets;
    std::vector<TopDecayEvent::DecayID> decays;

    if(!setup(&genParticleReader,jetReader, recoJets,decays)) return;
    vector<int> types;
    getJetCandidates(recoJets,decays,types);

    for(unsigned int iJ = 0; iJ < recoJets.size(); ++iJ){
      eventPlots.rewind();

      const auto * j = recoJets[iJ];

      eventPlots("W_jet__",types[iJ] == 1);
      eventPlots("B_jet__",types[iJ] == 0);
      eventPlots("O_jet__",types[iJ] == 2);

      auto v = calculateJetCandVars(nPV,jetReader,j);

      eventPlots.fill(v.jetPT,1,"pt",";p_{T}",80,0,800,prefix);

      ++eventPlots;
      eventPlots("pt_all__",true);
      eventPlots("pt_eq20to50__"  ,v.jetPT < 50);
      eventPlots("pt_eq50to100__" ,v.jetPT >= 50 && v.jetPT < 100);
      eventPlots("pt_eq100to200__",v.jetPT >= 100 && v.jetPT < 200);
      eventPlots("pt_geq200__",v.jetPT >= 200);


      eventPlots.fill(v.ptD    ,1,"ptD"     ,";p_{t}(D)"           ,50,0,1,prefix);
      eventPlots.fill(v.axis1   ,1,"axis1"   ,";axis_{1}"           ,50,0,.4,prefix);
      eventPlots.fill(v.axis2   ,1,"axis2"   ,";axis_{2}"           ,50,0,.4,prefix);
      eventPlots.fill(v.jetMult   ,1,"mult"    ,";# of part."         ,100,-0.5,99.5,prefix);
      eventPlots.fill(v.area   ,1,"area"    ,";area"               ,100,0,4,prefix);
      eventPlots.fill(v.mass    ,1,"mass"    ,";mass"               ,100,0,500,prefix);
      eventPlots.fill(v.betaStar    ,1,"betastar"    ,";betastar"               ,100,0,.5,prefix);

      eventPlots.fill(wJetLikliMVA.mvaVal(v),1,"mva"    ,";mva"               ,100,-1,1,prefix);

    }

  }





  void runEvent(){

	  plotJets(&pickyJetReader,"picky_");



  }

  void out(TString outputPath){
    plotter->write(outputPath);
  }


};


#endif



void testWJetLikliVars(string fname = "evttree.root", string treeName = "TestAnalyzer/Events", TString output = "testPlots.root", bool isMCTree = true) {

  Analyze a(fname, treeName, isMCTree);
  TString name = fname;
  name = ((TObjString*)name.Tokenize(".")->At(0))->GetString();
  a.prefix = name;
  a.prefix += "_";
  a.analyze(100000,500000);
//  a.out(TString::Format("%s_plots.root",name.Data()));
  a.out(output);
}
