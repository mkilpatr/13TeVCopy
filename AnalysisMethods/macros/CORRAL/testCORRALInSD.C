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
#include "AnalysisTools/KinematicVariables/interface/Mt2Helper.h"
#include "AnalysisTools/KinematicVariables/interface/CreatePseudoJets.h"

using namespace std;
using namespace ucsbsusy;


class Analyze : public BaseTreeAnalyzer{
public:
  PlotterD*                       plotter;
  Polybook                        eventPlots;
  TString                         prefix;
  Analyze(TString fname, string treeName, bool isMCTree, ConfigPars* pars) : BaseTreeAnalyzer(fname,treeName, isMCTree,pars), plotter (new PlotterD(3)), eventPlots(plotter)
  {
  };

  virtual void loadVariables(){
    load(EVTINFO);
    load(GENPARTICLES, GenParticleReader::FILLOBJ | GenParticleReader::LOADPARTONDECAY);
    load(PICKYJETS,JetReader::FILLOBJ | JetReader::LOADGEN | JetReader::LOADRECO | JetReader::LOADTOPASSOC | JetReader::LOADJETSHAPE );
    load(AK4JETS,JetReader::LOADRECO | JetReader::LOADJETSHAPE | JetReader::FILLOBJ);
    load(ELECTRONS);
    load(MUONS);
    load(PFCANDS);
    load(CORRAL);

  }


  void runEvent(){

    vector<RecoJetF*> ak4Jets;
    vector<RecoJetF*> ak4BJets;
    vector<RecoJetF*> ak4NonBJets;
    cleanJets(&ak4Reader,ak4Jets,&ak4BJets,&ak4NonBJets);

    //presel
    if(ak4Jets.size() < 6) return;
    if(met->pt() < 200) return;
    int nTPICKY = 0;
    for(auto* j : bJets) if(isTightBJet(*j)) nTPICKY++;
    if(nTPICKY == 0) return;
    if(nVetoedLeptons + nVetoedTaus > 0) return;
    corralReader.getCORRALData(&genParticleReader,&pickyJetReader,nPV);
    CORRAL::CORRALData& data = *corralReader.corral;

    vector<pair<int,int> > newRankedPairs;
    for(unsigned int iT = 0; iT < data.rankedTPairs.size(); ++iT){
      if(data.tCandVars[data.rankedTPairs[iT].first].mva < 0) continue;
      if(data.tCandVars[data.rankedTPairs[iT].second].mva < 0) continue;
      newRankedPairs.push_back(data.rankedTPairs[iT]);
    }
    data.rankedTPairs = newRankedPairs;

    eventPlots.rewind();

    eventPlots("ttbar__",process == defaults::TTBAR);
    eventPlots("znunu__",process == defaults::SINGLE_Z);
    eventPlots("ttz__",process == defaults::TTZ);
    eventPlots("",process == defaults::SIGNAL);

    if(process == defaults::SIGNAL) {
      std::vector<RecoJetF*> recoJets;
      std::vector<TopJetMatching::TopDecayEvent::DecayID> decays;
      TopJetMatching::TopDecayEvent* topDecayEvent = CORRAL::associateDecays(&genParticleReader,&pickyJetReader,recoJets,decays);
      bool decision = true;

      //filter
      for(const auto& t : topDecayEvent->topDecays){
        if(t.isLeptonic) decision =  false;
//        if(t.diag != TopJetMatching::RESOLVED_TOP) decision = false;
      }

      if(decision == false) return;

    }

    ++eventPlots;

    eventPlots("met_geq200__",true);
    eventPlots("met_eq200to300__",met->pt() >= 200 && met->pt() < 300);
    eventPlots("met_eq300to400__",met->pt() >= 300 && met->pt() < 400);
    eventPlots("met_geq400__",met->pt() >= 400);

    ++eventPlots;
    eventPlots("nTopPairs_incl__",true);
    eventPlots("nTopPairs_geq1__",data.reconstructedTop == true);


    eventPlots.fill(data.top1_disc ,weight,"leadTopDisc",";Leading top disc.",100,-1,1);
    eventPlots.fill(data.top2_disc ,weight,"subleadTopDisc",";Sub-leading top disc.",100,-1,1);
    eventPlots.fill((data.top1_disc + 1) * (data.top2_disc +1 )/4 ,weight,"topPairDisc",";top1 disc * top2 dic",100,0,1);

    double maxLead = -1;
    double maxSubLead = -1;
    for(unsigned int iT = 0; iT < data.rankedTPairs.size(); ++iT){
      if(data.tCandVars[data.rankedTPairs[iT].first].mva > maxLead) maxLead = data.tCandVars[data.rankedTPairs[iT].first].mva;
      if(data.tCandVars[data.rankedTPairs[iT].second].mva > maxSubLead) maxSubLead = data.tCandVars[data.rankedTPairs[iT].second].mva;
    }
    eventPlots.fill(maxLead ,weight,"maxLead",";maxLead top disc.",100,-1,1);
    eventPlots.fill(maxSubLead ,weight,"maxSubLead",";maxSubLead top disc.",100,-1,1);


    eventPlots.fill(data.top1_disc,data.top2_disc ,weight,"leadTopDiscvsubleadTopDisc",";Leading top disc.;Sub-leading top disc.",20,-1,1,20,-1,1);

    eventPlots.fill(data.reconstructedTop == true ? data.top1->mom.pt() : 0 ,weight,"leadTopPT",";Leading top pt.",100,0,500);
    eventPlots.fill(data.reconstructedTop == true ? data.top2->mom.pt() : 0 ,weight,"subleadTopPT",";Sub-leading top pt.",100,0,500);

    Mt2Helper * Mt2Calc    = new Mt2Helper();
    CreatePseudoJets * psuedoCalc = new CreatePseudoJets();

    MomentumF pseudoJet1; MomentumF pseudoJet2;
    psuedoCalc->makePseudoJets(jets,pseudoJet1,pseudoJet2,0);
     double picky_MT2   = Mt2Calc->CalcMT2(&pseudoJet1,&pseudoJet2, met, 0);
     double corral_MT2   = data.reconstructedTop ? Mt2Calc->CalcMT2(&data.top1->mom,&data.top2->mom, met, 0) : 0;



    MomentumF ak4pseudoJet1; MomentumF ak4pseudoJet2;
    psuedoCalc->makePseudoJets(ak4Jets,ak4pseudoJet1,ak4pseudoJet2,0);
    double ak4_MT2   = Mt2Calc->CalcMT2(&ak4pseudoJet1,&ak4pseudoJet2, met, 0);

    eventPlots.fill(picky_MT2 ,weight,"picky_MT2",";picky MT2.",100,0,900);
    eventPlots.fill(corral_MT2 ,weight,"corral_MT2",";corral MT2.",100,0,900);
    eventPlots.fill(ak4_MT2 ,weight,"ak4_MT2",";ak4 MT2.",100,0,900);

    eventPlots.fill(data.top1_disc,corral_MT2 ,weight,"leadTopDiscvcorral_MT2",";Leading top disc.;corral_MT2",20,-1,1,20,0,900);
    eventPlots.fill(data.top2_disc,corral_MT2 ,weight,"subleadTopDiscvcorral_MT2",";Sub-leading top disc.;corral_MT2",20,-1,1,20,0,900);
    eventPlots.fill((data.top1_disc + 1) * (data.top2_disc +1 )/4,corral_MT2 ,weight,"topPairDiscvcorral_MT2",";top1 disc * top2 dic;corral_MT2",20,0,1,20,0,900);
    eventPlots.fill((data.top1_disc + 1) * (data.top2_disc +1 )/4,picky_MT2 ,weight,"topPairDiscvpicky_MT2",";top1 disc * top2 dic;picky_MT2",20,0,1,20,0,900);
    eventPlots.fill((data.top1_disc + 1) * (data.top2_disc +1 )/4,ak4_MT2 ,weight,"topPairDiscvak4_MT2",";top1 disc * top2 dic;ak4_MT2",20,0,1,20,0,900);


     delete Mt2Calc;
     delete psuedoCalc;

  }

  void out(TString outputPath){
    plotter->write(outputPath);
  }


};


#endif



void testCORRALInSD(string fname = "evttree.root", string treeName = "TestAnalyzer/Events", TString output = "testPlots.root",bool isMCTree = true) {


  BaseTreeAnalyzer::ConfigPars pars;
  pars.defaultJetCollection = BaseTreeAnalyzer::PICKYJETS;

  Analyze a(fname, treeName,isMCTree,&pars);
  TString name = fname;
  name = ((TObjString*)name.Tokenize(".")->At(0))->GetString();
  a.prefix = name;
  a.prefix += "_";
  a.analyze();
  a.out(output);
}
