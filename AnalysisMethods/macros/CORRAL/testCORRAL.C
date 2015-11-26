#include <vector>
#include <assert.h>
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TH1F.h"
#include "TCanvas.h"
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisTools/Utilities/interface/PartonMatching.h"
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
using PartonMatching::PartonEvent;

static TString MVAprefix;

class Analyze : public BaseTreeAnalyzer{
public:
  PlotterD*                       plotter;
  Polybook                        eventPlots;
  TString                         prefix;
  CORRALReconstructor  corral;
  Analyze(TString fname, string treeName, bool isMCTree, ConfigPars* pars) : BaseTreeAnalyzer(fname,treeName, isMCTree,pars), plotter (new PlotterD(3)), eventPlots(plotter)
  ,corral(MVAprefix)
  {
  };

  virtual void loadVariables(){
    load(EVTINFO);
    load(GENPARTICLES, GenParticleReader::FILLOBJ | GenParticleReader::LOADPARTONDECAY);
    load(PICKYJETS,JetReader::FILLOBJ | JetReader::LOADGEN | JetReader::LOADRECO | JetReader::LOADTOPASSOC | JetReader::LOADJETSHAPE );
    load(ELECTRONS);
    load(MUONS);
    load(PFCANDS);

  }


  void runEvent(){
    if(nJets < 6) return;
    if(met->pt() < 200) return;

    corral.getTopPairs(&genParticleReader, defaultJets,nPV);
    CORRALData& data = corral.data;

    eventPlots.rewind();
    eventPlots("incl__",true);
    eventPlots("noEMuTau__",nVetoedLeptons == 0 && nVetoedTaus == 0);

    ++eventPlots;

    eventPlots("nTopPairs_incl__",true);
    eventPlots("nTopPairs_geq1__",data.reconstructedTop == true);
    eventPlots("nTopPairs_geq1_pure__",(data.top1_disc + 1) * (data.top2_disc +1 )/4 > .2);

    eventPlots.fill(1,1,"nEvents",";Number of events",1,.5,1.5);

    if(data.reconstructedTop == false) return;

    eventPlots.fill((data.top1->mom.p4() + data.top2->mom.p4()).mass() ,1,"topPairInvMass",";Top pair inv. mass",100,0,2000);

    eventPlots.fill(data.top1->mom.mass() ,1,"leadTopMass",";Lead top mass",100,0,500);
    eventPlots.fill(data.top2->mom.mass() ,1,"subleadTopMass",";Sub-lead top mass",100,0,500);

    eventPlots.fill(data.top1->mom.pt() + data.top2->mom.pt() ,1,"topHT",";Sum top p_{T}",100,0,2000);

    eventPlots.fill((data.top1->mom.p4() + data.top2->mom.p4()).pt() ,1,"topVecPT",";(top1 + top 2) p_{T}",100,0,1000);

    eventPlots.fill(PhysicsUtilities::absDeltaPhi(data.top1->mom,data.top2->mom) ,1,"topDPHI",";#Delta#phi(top1,top2)",100,0,3.2);


    eventPlots.fill(data.top1_disc ,1,"leadTopDisc",";Leading top disc.",100,-1,1);
    eventPlots.fill(data.top2_disc ,1,"subleadTopDisc",";Sub-leading top disc.",100,-1,1);
    eventPlots.fill((data.top1_disc + 1) * (data.top2_disc +1 )/4 ,1,"topPairDisc",";top1 disc * top2 dic",100,0,1);



  }

  void out(TString outputPath){
    plotter->write(outputPath);
  }


};


#endif



void testCORRAL(string fname = "evttree.root", string treeName = "TestAnalyzer/Events", TString output = "testPlots.root", TString MVAprefix_ = "",bool isMCTree = true) {

  MVAprefix = MVAprefix_;
  BaseTreeAnalyzer::ConfigPars pars;
  pars.defaultJetCollection = BaseTreeAnalyzer::PICKYJETS;

  Analyze a(fname, treeName,isMCTree,&pars);
  TString name = fname;
  name = ((TObjString*)name.Tokenize(".")->At(0))->GetString();
  a.prefix = name;
  a.prefix += "_";
  a.analyze(100,10000);
//  a.out(TString::Format("%s_plots.root",name.Data()));
  a.out(output);
}
