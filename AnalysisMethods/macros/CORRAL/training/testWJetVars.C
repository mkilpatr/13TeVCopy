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
#include "AnalysisTools/Utilities/interface/JetFlavorInfo.h"

#include "AnalysisTools/Parang/interface/Plotter.h"
#include "AnalysisTools/Parang/interface/Polybook.h"
#include "AnalysisTools/Parang/interface/Panvariate.h"
#include "ObjectProducers/TopTagging/interface/CORRAL.h"
using namespace std;
using namespace ucsbsusy;
using namespace CORRAL;
using TopJetMatching::TopDecayEvent;

class Analyze : public BaseTreeAnalyzer{
public:
  PlotterD*                       plotter;
  Polybook                        eventPlots;
  TString                         prefix;
  WJetLikliMVA  wJetLikliMVA;
  WMVA  wMVA;


  Analyze(TString fname, string treeName, bool isMCTree) : BaseTreeAnalyzer(fname,treeName, isMCTree), plotter (new PlotterD(3)), eventPlots(plotter)
  , wJetLikliMVA("T2tt_merged_wJetLikli_disc.root","mva_0")
  , wMVA("T2tt_merged_wCand_disc.root","mva_0")
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

    vector<jetCandVars> jetVars(recoJets.size());
    for(unsigned int iJ = 0; iJ < recoJets.size(); ++iJ){
      jetVars[iJ] = calculateJetCandVars(nPV,jetReader,recoJets[iJ]);
      wJetLikliMVA.mvaVal(jetVars[iJ]);
    }

    vector<WCand> wCands;
    getWCandidates(recoJets,decays,wCands);
    vector<WCandVars> wCandVars(wCands.size());
    for(unsigned int iC = 0; iC < wCands.size(); ++iC){
      wCandVars[iC] = calculateWCandVars(jetReader,recoJets,jetVars,wCands[iC]);
      wMVA.mvaVal(wCandVars[iC]);
    }

    for(unsigned int iC = 0; iC < wCands.size(); ++iC){
      auto& vars = wCandVars[iC];

      eventPlots.rewind();
      eventPlots("W__"    ,wCands[iC].isW);
      eventPlots("non_W__",!wCands[iC].isW);

      const float pt = vars.wPT;

      eventPlots.fill(pt,1,"pt",";p_{T}",80,0,800,prefix);

      ++eventPlots;
      eventPlots("pt_all__"      ,true);
      eventPlots("pt_eq0to100__" ,pt < 100);
      eventPlots("pt_eq100to200__" ,pt >= 100 && pt < 200);
      eventPlots("pt_eq200to300__" ,pt >= 200 && pt < 300);
      eventPlots("pt_geq300__" ,pt >= 300);

      eventPlots.fill(vars.wMass        ,1,"mass"      ,";mass"               ,100,0,500,prefix);
      eventPlots.fill(vars.pt2opt1      ,1,"pt2opt1"   ,";j2(p_{T})/j1(p_{T})",100,0,1,prefix);
      eventPlots.fill(vars.wJetLikli1   ,1,"wJetLikli1",";WJet Likelihood 1"  ,100,-1,1,prefix);
      eventPlots.fill(vars.wJetLikli2   ,1,"wJetLikli2",";WJet Likelihood 2"  ,100,-1,1,prefix);
      eventPlots.fill(vars.maxCSV       ,1,"maxCSV"    ,";max CSV"            ,100,0,1,prefix);
      eventPlots.fill(vars.dr           ,1,"dr"        ,";#DeltaR"            ,100,0,5,prefix);
      eventPlots.fill(vars.deta         ,1,"deta"      ,";#Delta#eta"         ,100,0,5,prefix);
      eventPlots.fill(vars.dphi         ,1,"dphi"      ,";#Delta#phi"         ,100,0,5,prefix);
      eventPlots.fill(TMath::Abs(vars.charge)       ,1,"charge"    ,";charge"             ,100,0,3,prefix);
      eventPlots.fill(vars.pullAng1     ,1,"pullAng1"  ,";#Theta_{t,1}"       ,100,-3.2,3.2,prefix);
      eventPlots.fill(vars.pullAng2     ,1,"pullAng2"  ,";#Theta_{t,2}"       ,100,-3.2,3.2,prefix);
      eventPlots.fill(vars.nWCon        ,1,"nWCon"     ,";# of other jets in W",10,-.5,9.5,prefix);
      eventPlots.fill(vars.mva          ,1,"mva"    ,";mva"               ,100,-1,1,prefix);
    }

    int nFakes = 0;
    int nGoodFakes = 0;

    for(unsigned int iC = 0; iC < wCands.size(); ++iC){
      const float pt = wCandVars[iC].wPT;


      eventPlots.rewind();
      eventPlots("W__"    ,wCands[iC].isW);
      eventPlots("non_W__",!wCands[iC].isW);

      ++eventPlots;
      eventPlots("pt_all__"      ,true);
      eventPlots("pt_eq0to100__" ,pt < 100);
      eventPlots("pt_eq100to200__" ,pt >= 100 && pt < 200);
      eventPlots("pt_eq200to300__" ,pt >= 200 && pt < 300);
      eventPlots("pt_geq300__" ,pt >= 300);

      bool goodW = wMVA.passMVA(wCandVars[iC].wPT, wCandVars[iC].mva);

      eventPlots.fill(0                   ,1,"allWs"    ,";Ws"               ,2,-.5,1.5,prefix);
      if(goodW)eventPlots.fill(1          ,1,"allWs"    ,";Ws"               ,2,-.5,1.5,prefix);

      if(!wCands[iC].isW){
        if(goodW)eventPlots.fill(wCands[iC].fakeCategory          ,1,"fakeCategory"    ,";WW/Wb/WF/BB/BF/FF"               ,6,-.5,5.5,prefix);
        nFakes++;
        if(goodW)nGoodFakes++;
      }
    }

    eventPlots.rewind();
    eventPlots("W__"    ,false);
    eventPlots("non_W__",true);
    eventPlots.fill(nFakes      ,1,"incl_nFakes"  ,";number of fake Ws",200,-.5,199.5,prefix);
    eventPlots.fill(nGoodFakes  ,1,"good_nFakes"  ,";number of fake Ws",200,-.5,199.5,prefix);



  }






  void runEvent(){

	  plotJets(&pickyJetReader,"picky_");
//	  plotJets(&ak4Reader,"ak4_");
//


  }

  void out(TString outputPath){
    plotter->write(outputPath);
  }


};


#endif



void testWJetVars(string fname = "evttree.root", string treeName = "TestAnalyzer/Events", TString output = "testPlots.root", bool isMCTree = true) {

  Analyze a(fname, treeName, isMCTree);
  TString name = fname;
  name = ((TObjString*)name.Tokenize(".")->At(0))->GetString();
  a.prefix = name;
  a.prefix += "_";
  a.analyze(10000,-1);
//  a.out(TString::Format("%s_plots.root",name.Data()));
  a.out(output);
}
