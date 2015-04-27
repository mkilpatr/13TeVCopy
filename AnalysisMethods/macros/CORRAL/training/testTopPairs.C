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

static TString MVAprefix;

class Analyze : public BaseTreeAnalyzer{
public:
  PlotterD*                       plotter;
  Polybook                        eventPlots;
  TString                         prefix;
  WJetLikliMVA  wJetLikliMVA;
  WMVA  wMVA;
  T_MVA  tMVA;

  Analyze(TString fname, string treeName, bool isMCTree) : BaseTreeAnalyzer(fname,treeName, isMCTree), plotter (new PlotterD(3)), eventPlots(plotter)
  , wJetLikliMVA( MVAprefix + "T2tt_merged_wJetLikli_disc.root","mva_0")
  , wMVA(MVAprefix + "T2tt_merged_wCand_disc.root","mva_0")
  , tMVA(MVAprefix +"T2tt_merged_tCand_disc.root","mva_0")
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
    TopJetMatching::TopDecayEvent * topDecayEvent =  associateDecays(&genParticleReader,jetReader, recoJets,decays);

    bool baseline = true;
    bool resolved = true;

    for(const auto& t : topDecayEvent->topDecays){
      if(t.isLeptonic) baseline =  false;
      if(t.diag == TopJetMatching::BAD_PARTON)   baseline = false;
      if(t.diag != TopJetMatching::RESOLVED_TOP) resolved = false;
    }

    if(!baseline) return;
    assert(topDecayEvent->topDecays.size() == 2);
    int realRank = 0;
    int nFakes = 0;
    if(resolved){
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
      pruneTopCandidates(tCands,tCandVars,prunedTops,&tMVA);

      vector<pair<int,int>> rankedPairs =  getRankedTopPairs(tCands,tCandVars,prunedTops);

      for(unsigned int iR = 0; iR < rankedPairs.size(); ++iR){
        if(tCands[rankedPairs[iR].first].type != 2 &&tCands[rankedPairs[iR].second].type != 2 ){
          realRank = iR + 1;
          break;
        }
      }
      nFakes = prunedTops.size() - (realRank > 0 ? 1 : 0);

      if(realRank > 0){
        //single top info
        int top1 = -1;
        for(unsigned int iT = 0; iT < prunedTops.size(); ++iT){
          const auto& cand = tCands[prunedTops[iT].second];
          const auto& vars = tCandVars[prunedTops[iT].second];

          if(cand.type != 2 && top1 < 0)
            top1 = iT;

          eventPlots.rewind();
          eventPlots("t__"          ,cand.type != 2);
          eventPlots("t1__"         ,cand.type != 2 && int(iT) == top1);
          eventPlots("t2__"         ,cand.type != 2 && int(iT) != top1);
          eventPlots("not_t__"      ,cand.type == 2);
          eventPlots("",false);

          auto pt = vars.tPT;

          ++eventPlots;
          eventPlots("pt_all__"      ,true);
          eventPlots("pt_lt150__"     ,pt < 150);
          eventPlots("pt_eq150to300__",pt >= 150 && pt < 300);
          eventPlots("pt_eq300to500__",pt >= 300 && pt < 500);
          eventPlots("pt_geq500__"    ,pt >= 500);
          eventPlots("",false);

          eventPlots.fill(iT,1,"rank",";top cand. rank",200,-.5,199.5,prefix);
          eventPlots.fill(prunedTops[iT].first,1,"mva",";top cand. MVA",100,-1,1,prefix);
        }

        //pair info
        for(unsigned int iT = 0; iT < rankedPairs.size(); ++iT){
          const auto& var1 = tCandVars[rankedPairs[iT].first];
          const auto& var2 = tCandVars[rankedPairs[iT].second];

          eventPlots.rewind();
          eventPlots("t__"          ,iT == realRank -1);
          eventPlots("t1__"         ,false);
          eventPlots("t2__"         ,false);
          eventPlots("not_t__"      ,iT != realRank -1);
          eventPlots("",false);


          ++eventPlots;
          eventPlots("pt_all__"       ,false);
          eventPlots("pt_lt150__"     ,false);
          eventPlots("pt_eq150to300__",false);
          eventPlots("pt_eq300to500__",false);
          eventPlots("pt_geq500__"    ,false);
          eventPlots("",true);



          eventPlots.fill(iT,1,"rank",";top pair rank",30,-.5,29.5,prefix);

          double transMVA1 = (var1.mva +1)/2;
          double transMVA2 = (var2.mva +1)/2;

          eventPlots.fill((transMVA1 + transMVA2)/2,1,"mva1p2",";MVA(t1+t2)",100,0,1,prefix);
          eventPlots.fill(transMVA1 * transMVA2,1,"mva1t2",";MVA(t1 x t2)",100,0,1,prefix);

          eventPlots.fill(transMVA1 * transMVA2,1,"mva1t2",";MVA(t1 x t2)",100,0,1,prefix);

          eventPlots.fill(((var1.mva +1)*(var2.mva +1) + (1-var1.mva)*(1-var2.mva)) == 0 ? 0 : (var1.mva +1)*(var2.mva +1)/((var1.mva +1)*(var2.mva +1) + (1-var1.mva)*(1-var2.mva)) ,"mval",";mval",100,0,1,prefix);

          eventPlots.fill(var1.mva,var2.mva  ,1,"top1mva_top2mva"  ,";top1 mva;top2 mva",100,-1,1,100,-1,1,prefix);
        }

      }
    }


    eventPlots.rewind();
    eventPlots("t__"          ,false);
    eventPlots("t1__"         ,false);
    eventPlots("t2__"         ,false);
    eventPlots("not_t__"      ,false);
    eventPlots("",true);
    ++eventPlots;
    eventPlots("pt_all__"       ,false);
    eventPlots("pt_lt150__"     ,false);
    eventPlots("pt_eq150to300__",false);
    eventPlots("pt_eq300to500__",false);
    eventPlots("pt_geq500__"    ,false);
    eventPlots("",true);
    ++eventPlots;
    eventPlots("all__"          ,true);
    eventPlots("resolved_jets__",resolved == true);
    eventPlots("reconstructed__",realRank >0);
    eventPlots("best_pair__"    ,realRank == 1);

    eventPlots.fill(topDecayEvent->topDecays[0].top->pt(),1,"leadTopPT",";Leading Top p_{T}",150,0,1500,prefix);
    eventPlots.fill(topDecayEvent->topDecays[1].top->pt(),1,"subLeadTopPT",";Sub-leading Top p_{T}",150,0,1500,prefix);
    eventPlots.fill((topDecayEvent->topDecays[1].top->pt() + topDecayEvent->topDecays[0].top->pt())/2 ,1,"avgTopPT",";Average Top p_{T}",150,0,1500,prefix);

    if(resolved)
      eventPlots.fill(nFakes,1,"nFakes",";# of fakes",30,-.5,29.5,prefix);
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



void testTopPairs(string fname = "evttree.root", string treeName = "TestAnalyzer/Events", TString output = "testPlots.root", TString MVAprefix_ = "", bool pureMatching = true, bool isMCTree = true) {

  MVAprefix = MVAprefix_;
  if(!pureMatching) TopJetMatching::Parton::setPurity(false);

  Analyze a(fname, treeName,isMCTree);
  TString name = fname;
  name = ((TObjString*)name.Tokenize(".")->At(0))->GetString();
  a.prefix = name;
  a.prefix += "_";
  a.analyze(10000,5000000);
//  a.out(TString::Format("%s_plots.root",name.Data()));
  a.out(output);
}
