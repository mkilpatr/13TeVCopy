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
  T_MVA  tMVA;

  Analyze(TString fname, string treeName, bool isMCTree) : BaseTreeAnalyzer(fname,treeName, isMCTree), plotter (new PlotterD(3)), eventPlots(plotter)
  , wJetLikliMVA("T2tt_merged_wJetLikli_disc.root","mva_0")
  , wMVA("T2tt_merged_wCand_disc.root","mva_0")
  , tMVA("T2tt_merged_tCand_disc.root","mva_0")
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

    vector<TCand> tCands;
    getTCandidates(wMVA,recoJets,decays,wCands,wCandVars,tCands);
    vector<TCandVars> tCandVars(tCands.size());
    for(unsigned int iC = 0; iC < tCands.size(); ++iC){
      tCandVars[iC] = calculateTCandVars(jetReader,recoJets,wCands,jetVars,wCandVars,tCands[iC]);
      tMVA.mvaVal(tCandVars[iC]);
    }

    vector<RankedIndex> prunedTops;
    pruneTopCandidates(tCands,tCandVars,prunedTops);

    for(unsigned int iC = 0; iC < tCands.size(); ++iC){
      auto& vars = tCandVars[iC];

      eventPlots.rewind();
      eventPlots("t__"          ,tCands[iC].type == 1);
      eventPlots("t_bad_combo__",tCands[iC].type == 0);
      eventPlots("not_t__"      ,tCands[iC].type == 2);

      auto pt = vars.tPT;
      eventPlots.fill(pt,1,"pt",";p_{T}",100,0,1000,prefix);

      ++eventPlots;
      eventPlots("pt_all__"      ,true);
      eventPlots("pt_lt150__"     ,pt < 150);
      eventPlots("pt_eq150to300__",pt >= 150 && pt < 300);
      eventPlots("pt_eq300to500__",pt >= 300 && pt < 500);
      eventPlots("pt_geq500__"    ,pt >= 500);

      eventPlots.fill(vars.wPT          ,1,"wPT"          ,";W p_{T}",80,0,800,prefix);
      eventPlots.fill(TMath::Abs(tCands[iC].mom.eta())          ,1,"tabseta"          ,";t |#eta|",100,0,5,prefix);
      eventPlots.fill(vars.tMass        ,1,"tMass"        ,";t mass ",100,0,500,prefix);
      eventPlots.fill(vars.wMass        ,1,"wMass"        ,";mass",100,0,500,prefix);
      eventPlots.fill(vars.bPTotPT      ,1,"bPTotPT"      ,";b p_{T} / t p_{T}",100,0,5,prefix);
      eventPlots.fill(vars.bCSV         ,1,"bCSV"         ,";b CSV"            ,100,0,1,prefix);
      eventPlots.fill(vars.maxWCSV      ,1,"maxWCSV"      ,";max W jet CSV"            ,100,0,1,prefix);
      eventPlots.fill(vars.bWLikli      ,1,"bWLikli"      ,";W jet likli (b-jet)"      ,100,-1,1,prefix);
      eventPlots.fill(vars.wDisc        ,1,"wDisc"        ,";W disc."                  ,100,-1,1,prefix);
      eventPlots.fill(vars.maxOWDisc    ,1,"maxOWDisc"    ,";max W disc (other jet combo)",100,-1,1,prefix);
      eventPlots.fill(vars.m23om123     ,1,"m23om123"     ,";m23 / m123",100,0,1,prefix);
      eventPlots.fill(vars.m13om12      ,1,"m13om12"      ,";m13 / m12",100,0,5,prefix);
      eventPlots.fill(vars.atan_m13om12 ,1,"atan_m13om12" ,";tan^{-1}(m13 / m12)",100,0,2,prefix);
      eventPlots.fill(vars.maxjjdr      ,1,"maxjjdr"      ,";max(#DeltaR(jj)"     ,100,0,5,prefix);
      eventPlots.fill(vars.wbDR         ,1,"wbDR"         ,";#DeltaR(wb)",100,0,5,prefix);
      eventPlots.fill(vars.wbDEta       ,1,"wbDEta"       ,";#Delta#eta(wb)",100,0,5,prefix);
      eventPlots.fill(vars.wbDPhi       ,1,"wbDPhi"       ,";#Delta#phi(wb)",100,0,5,prefix);
      eventPlots.fill(vars.nTCon        ,1,"nTCon"        ,";# of other jets in t cone",20, -.5,19.5,prefix);
      eventPlots.fill(vars.mva          ,1,"mva"          ,";MVA",100, -1,1,prefix);


    }

    //Now we do pruning!
    int nFakes = 0;
    int nGoodFakes = 0;
    for(unsigned int iC = 0; iC < prunedTops.size(); ++iC){
      const auto& cand = tCands[prunedTops[iC].second];
      const auto& vars = tCandVars[prunedTops[iC].second];

      eventPlots.rewind();
      eventPlots("t__"          ,cand.type == 1 || cand.type == 0);
      eventPlots("t_bad_combo__",false);
      eventPlots("not_t__"      ,cand.type == 2);

      auto pt = vars.tPT;
      eventPlots.fill(pt,1,"pt",";p_{T}",100,0,1000,prefix);

      ++eventPlots;
      eventPlots("pt_all__"      ,true);
      eventPlots("pt_lt150__"     ,pt < 150);
      eventPlots("pt_eq150to300__",pt >= 150 && pt < 300);
      eventPlots("pt_eq300to500__",pt >= 300 && pt < 500);
      eventPlots("pt_geq500__"    ,pt >= 500);

      eventPlots.fill(vars.mva          ,1,"pruned_mva"          ,";MVA",100, -1,1,prefix);

      bool goodT = tMVA.passMVA(vars.tPT, vars.mva);

      eventPlots.fill(0                   ,1,"allTs"    ,";ts"               ,2,-.5,1.5,prefix);
      if(goodT)eventPlots.fill(1          ,1,"allTs"    ,";ts"               ,2,-.5,1.5,prefix);

      if(cand.type == 2){
        if(goodT) eventPlots.fill(cand.fakeCategory          ,1,"fakeCategory"    ,";w1b2/wf/w(bw)F/w(wf)b/w(ff)b/w(ff)f"               ,6,-.5,5.5,prefix);
        nFakes++;
        if(goodT)nGoodFakes++;
      }
  }
    int n_nonPruned_Fakes = 0;
    int n_nonPruned_GoodFakes = 0;
    //count the non pruned too...
    for(unsigned int iC = 0; iC < tCands.size(); ++iC){
      const auto& cand = tCands[iC];
      const auto& vars = tCandVars[iC];
      if(cand.type != 2) continue;
      bool goodT = tMVA.passMVA(vars.tPT, vars.mva);
      n_nonPruned_Fakes++;
      if(goodT)n_nonPruned_GoodFakes++;
    }

    eventPlots.rewind();
    eventPlots("t__"    ,false);
    eventPlots("t_bad_combo__",false);
    eventPlots("not_t__",true);
    eventPlots.fill(nFakes      ,1,"incl_nFakes"  ,";number of fake ts",200,-.5,199.5,prefix);
    eventPlots.fill(nGoodFakes  ,1,"good_nFakes"  ,";number of fake ts",200,-.5,199.5,prefix);
    eventPlots.fill(n_nonPruned_Fakes      ,1,"incl_noPrune_nFakes"  ,";number of fake ts",200,-.5,199.5,prefix);
    eventPlots.fill(n_nonPruned_GoodFakes  ,1,"good_noPrune_nFakes"  ,";number of fake ts",200,-.5,199.5,prefix);


    //Now for ranking
    vector<int> realRanks;
    int rank = 0;
    for(unsigned int iC = 0; iC < prunedTops.size(); ++iC){
      const auto& cand = tCands[prunedTops[iC].second];
      rank++;
      if(cand.type == 2) continue;
      realRanks.push_back(rank);
    }
    if(realRanks.size() >= 3){
      for(unsigned int iJ = 0; iJ < realRanks.size(); ++iJ){
        const auto& cand = tCands[prunedTops[ realRanks[iJ]  ].second];
        cout << cand.type <<" "<< cand.topIndex << " "<< cand.wInd <<" "<< cand.wCand->isW<<" "<< cand.bInd <<" "<< cand.wCand->ind1 <<" "<<cand.wCand->ind2 << endl;
      }
    }
    assert(realRanks.size() < 3);


    eventPlots.rewind();
    eventPlots("t__"    ,true);
    eventPlots("t_bad_combo__",false);
    eventPlots("not_t__",false);
    if(realRanks.size()) eventPlots.fill(realRanks[0]      ,1,"lead_realRank"  ,";rank of leading real t",100,-.5,99.5,prefix);
    if(realRanks.size() > 1) eventPlots.fill(realRanks[1]      ,1,"sub_realRank"  ,";rank of sub-leading real t",100,-.5,99.5,prefix);

    for(unsigned int iT = 0; iT < realRanks.size(); ++iT)
      eventPlots.fill(realRanks[iT]      ,1,"all_realRank"  ,";rank of real ts",100,-.5,99.5,prefix);

//    vector<RankedIndex> exclTops1 = getExclusiveTopCandidates(tMVA,tCands,tCandVars,0,prunedTops);
//    vector<RankedIndex> exclTops2 = getExclusiveTopCandidates(tMVA,tCands,tCandVars,1,prunedTops);
//    vector<RankedIndex> exclTops3 = getExclusiveTopCandidates(tMVA,tCands,tCandVars,2,prunedTops);
//
//
//    eventPlots.fill(exclTops1.size()  ,1,"exclTops1_nexcl"  ,";number of exclusive ts",100,-.5,99.5,prefix);
//    eventPlots.fill(exclTops2.size()  ,1,"exclTops2_nexcl"  ,";number of exclusive ts",100,-.5,99.5,prefix);
//    eventPlots.fill(exclTops3.size()  ,1,"exclTops3_nexcl"  ,";number of exclusive ts",100,-.5,99.5,prefix);
//
//    if(exclTops1.size() < 2 && exclTops3.size() >= 2){
//      eventPlots.fill(exclTops3.size()  ,1,"exclTops12_nexcl"  ,";number of exclusive ts",100,-.5,99.5,prefix);
//    } else {
//      eventPlots.fill(exclTops1.size()  ,1,"exclTops12_nexcl"  ,";number of exclusive ts",100,-.5,99.5,prefix);
//    }
//
//
//    vector<int> rankRealTops1;
//    vector<int> rankRealTops2;
//    vector<int> rankRealTops3;
//
//    getRealStats(tCands,exclTops1,rankRealTops1);
//    getRealStats(tCands,exclTops2,rankRealTops2);
//    getRealStats(tCands,exclTops3,rankRealTops3);
//
//    eventPlots.fill(rankRealTops1.size()  ,1,"exclTops1_nreal"  ,";number of real exclusive ts",100,-.5,99.5,prefix);
//    eventPlots.fill(rankRealTops2.size()  ,1,"exclTops2_nreal"  ,";number of real exclusive ts",100,-.5,99.5,prefix);
//    eventPlots.fill(rankRealTops3.size()  ,1,"exclTops3_nreal"  ,";number of real exclusive ts",100,-.5,99.5,prefix);
//    if(exclTops1.size() < 2 && exclTops3.size() >= 2){
//      eventPlots.fill(rankRealTops3.size()  ,1,"exclTops12_nreal"  ,";number of real exclusive ts",100,-.5,99.5,prefix);
//    } else {
//      eventPlots.fill(rankRealTops1.size()  ,1,"exclTops12_nreal"  ,";number of real exclusive ts",100,-.5,99.5,prefix);
//    }
//
//    eventPlots.fill(rankRealTops1.size() > 1 ? rankRealTops1[1] : 0   ,1,"exclTops1_realsubrank"  ,";subleading rank of real exclusive ts",100,-.5,99.5,prefix);
//    eventPlots.fill(rankRealTops2.size() > 1 ? rankRealTops2[1] : 0   ,1,"exclTops2_realsubrank"  ,";subleading rank of real exclusive ts",100,-.5,99.5,prefix);
//    eventPlots.fill(rankRealTops3.size() > 1 ? rankRealTops3[1] : 0   ,1,"exclTops3_realsubrank"  ,";subleading rank of real exclusive ts",100,-.5,99.5,prefix);
//
//    if(exclTops1.size() < 2 && exclTops3.size() >= 2){
//      eventPlots.fill(rankRealTops3.size() > 1 ? rankRealTops3[1] : 0   ,1,"exclTops12_realsubrank"  ,";subleading rank of real exclusive ts",100,-.5,99.5,prefix);
//    } else {
//      eventPlots.fill(rankRealTops1.size() > 1 ? rankRealTops1[1] : 0   ,1,"exclTops12_realsubrank"  ,";subleading rank of real exclusive ts",100,-.5,99.5,prefix);
//    }

//    vector<pair<int,int>> rankedPairs =  getRankedTopPairs(&tMVA,tCands,tCandVars,prunedTops);
//
//    eventPlots.fill(rankedPairs.size()  ,1,"nExclPairs"  ,";# of exclusive pairs",200,-.5,199.5,prefix);
//    int realRank = 0;
//    for(unsigned int iR = 0; iR < rankedPairs.size(); ++iR){
//      if(tCands[rankedPairs[iR].first].type != 2 &&tCands[rankedPairs[iR].second].type != 2 ){
//        realRank = iR + 1;
//        break;
//      }
//    }
//    eventPlots.fill(realRank  ,1,"realPairRanlk"  ,";rank of real top pair",200,-.5,199.5,prefix);



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



void testTJetVars(string fname = "evttree.root", string treeName = "TestAnalyzer/Events", TString output = "testPlots.root", bool isMCTree = true) {

  Analyze a(fname, treeName, isMCTree);
  TString name = fname;
  name = ((TObjString*)name.Tokenize(".")->At(0))->GetString();
  a.prefix = name;
  a.prefix += "_";
  a.analyze(10000,5000000);
//  a.out(TString::Format("%s_plots.root",name.Data()));
  a.out(output);
}
