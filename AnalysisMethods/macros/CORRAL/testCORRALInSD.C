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

  bool setupEvent(){
    // -------------  preselection
    vector<RecoJetF*> ak4Jets;
    vector<RecoJetF*> ak4BJets;
    vector<RecoJetF*> ak4NonBJets;
    cleanJets(&ak4Reader,ak4Jets,&ak4BJets,&ak4NonBJets);
    if(ak4Jets.size() < 6) return false;
    if(met->pt() < 200) return false;
    int nTPICKY = 0;
    for(auto* j : bJets) if(isTightBJet(*j)) nTPICKY++;
    if(nTPICKY == 0) return false;
    if(nVetoedLeptons + nVetoedTaus > 0) return false;

    // -------------  Return corral info from the event
    corralReader.getCORRALData(&genParticleReader,&pickyJetReader,nPV);
    return true;
  }

  void testSRVariables(){
    if(!setupEvent()) return;
    CORRAL::CORRALData& data = *corralReader.corral;

    eventPlots.rewind();
    eventPlots("ttbar__",process == defaults::TTBAR);
    eventPlots("znunu__",process == defaults::SINGLE_Z);
//    eventPlots("ttz__",process == defaults::TTZ);
    eventPlots("",process == defaults::SIGNAL);




    // -------------  Remove leptonic signal
    bool leptonic             = false;
    bool badPartons           = false;
    bool badJets              = false;
    bool badPair              = false;
    bool subLeadingPair       = false;

    if(process == defaults::SIGNAL) {
      std::vector<RecoJetF*> recoJets;
      std::vector<TopJetMatching::TopDecayEvent::DecayID> decays;
      TopJetMatching::TopDecayEvent* topDecayEvent = CORRAL::associateDecays(&genParticleReader,&pickyJetReader,recoJets,decays);
      //filter
        int nB = 0;
        int nBJ = 0;
        vector<MomentumF> tops;
        for(const auto& t : topDecayEvent->topDecays){
          if(t.isLeptonic) leptonic =  true;
          if(t.isLeptonic) continue;
          if(TMath::Abs(t.top->eta()) >= 2.4 ) nB++;
          if(t.diag  == TopJetMatching::BAD_PARTON)nB++;
          if(t.diag  != TopJetMatching::RESOLVED_TOP ) nBJ++;
          tops.push_back(t.top->p4());
      }
      badPartons = nB > 0 || tops.size() != 2;
      badJets =  (nBJ > 0);

      int realRank = -1;
      for(unsigned int iR = 0; iR < data.rankedTPairs.size(); ++iR){
        if(data.tCands[data.rankedTPairs[iR].first].type != 2 &&data.tCands[data.rankedTPairs[iR].second].type != 2 ){
          realRank = iR + 1;
          break;
        }
    }
      badPair =  realRank <= 0;
      subLeadingPair = realRank != 1;
    }
    if(leptonic) return;
    bool classified = false;

    ++eventPlots;
    eventPlots("diag_all__",true);
    eventPlots("diag_bad_partons__",!classified && (classified = badPartons ));
    eventPlots("diag_bad_jets__",!classified && (classified =badJets));
    eventPlots("diag_no_pair__",!classified && (classified =badPair));
    eventPlots("diag_wrong_pair__",!classified && (classified =subLeadingPair));
    eventPlots("diag_good_pair__",!classified);


    // -------------


    ++eventPlots;
    eventPlots("met_geq200__",true);
    eventPlots("met_eq200to300__",met->pt() >= 200 && met->pt() < 300);
    eventPlots("met_eq300to400__",met->pt() >= 300 && met->pt() < 400);
    eventPlots("met_geq400__",met->pt() >= 400);

    ++eventPlots;
    eventPlots("nTopPairs_incl__",true);
    eventPlots("nTopPairs_geq1__",data.reconstructedTop == true);



    // ------------- Plot info on the chosen top pair
    eventPlots.fill(data.top1_disc ,weight,"leadTopDisc",";Leading top disc.",100,-1,1);
    eventPlots.fill(data.top2_disc ,weight,"subleadTopDisc",";Sub-leading top disc.",100,-1,1);
    eventPlots.fill((data.top1_disc + 1) * (data.top2_disc +1 )/4 ,weight,"topTimesPairDisc",";top1 disc * top2 dic",100,0,1);
    eventPlots.fill((data.top1_disc + data.top2_disc) / 2 ,weight,"topPairDisc",";top1 disc + top2 disc",100,-1,1);
    // -------------
    eventPlots.fill(data.bestTopMatches.size(),weight,"numTops","; Number of exclusive tops",10,-0.5,9.5);
    eventPlots.fill(jets.size(),weight,"numPickyJets","; Number of picky jets",20,-0.5,19.5);
    if(data.bestTopMatches.size() > 1)
      eventPlots.fill(data.tCandVars[data.bestTopMatches[0][0]].mva, data.tCandVars[data.bestTopMatches[1][0]].mva  +data.tCandVars[data.bestTopMatches[1][1]].mva, weight,"bestmvacomp", ";best top MVA;best pair MVA", 100,-2,2,100,-2,2    );
    if(data.reconstructedTop){
      eventPlots.fill(data.rankedTPairs[0].first,data.rankedTPairs[0].second,weight,"topranks",";topranks",10,-0.5,9.5,10,-0.5,9.5);
    }

    double nearestMET = 0;
    if(data.reconstructedTop){
      nearestMET =  std::min(PhysicsUtilities::absDeltaPhi(*data.top2->bJet,*met), PhysicsUtilities::absDeltaPhi(*data.top2->wCand->jet1,*met));;
      nearestMET = std::min(nearestMET, PhysicsUtilities::absDeltaPhi(*data.top2->wCand->jet2,*met));
    }
    double top2b = data.reconstructedTop ? PhysicsUtilities::absDeltaPhi(*data.top2->bJet,*met) : 0;
    double mintopb = data.reconstructedTop ? std::min(PhysicsUtilities::absDeltaPhi(*data.top1->bJet,*met),PhysicsUtilities::absDeltaPhi(*data.top2->bJet,*met)) : 0;
    eventPlots.fill( top2b,weight,"top2bjet_nearestMET",";#Delta#phi(#slash{E}_{T},b-jet #in top_2)",100,0,3.3);
    eventPlots.fill( mintopb,weight,"minbjet_nearestMET",";Min[#Delta#phi(#slash{E}_{T},b-jets #in top pair)]",100,0,3.3);
    eventPlots.fill(nearestMET ,weight,"top2Jet_nearestMET",";Min[#Delta#phi(#slash{E}_{T},jets #in top_2)]",100,0,3.3);


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
    vector<RecoJetF*> ak4Jets;
    vector<RecoJetF*> ak4BJets;
    vector<RecoJetF*> ak4NonBJets;
    cleanJets(&ak4Reader,ak4Jets,&ak4BJets,&ak4NonBJets);
    psuedoCalc->makePseudoJets(ak4Jets,ak4pseudoJet1,ak4pseudoJet2,0);
    double ak4_MT2   = Mt2Calc->CalcMT2(&ak4pseudoJet1,&ak4pseudoJet2, met, 0);

    eventPlots.fill(picky_MT2 ,weight,"picky_MT2",";picky MT2.",100,0,900);
    eventPlots.fill(corral_MT2 ,weight,"corral_MT2",";corral MT2.",100,0,900);
    eventPlots.fill(ak4_MT2 ,weight,"ak4_MT2",";ak4 MT2.",100,0,900);

    eventPlots.fill(data.top1_disc,corral_MT2 ,weight,"leadTopDiscvcorral_MT2",";Leading top disc.;corral_MT2",20,-1,1,20,0,900);
    eventPlots.fill(data.top2_disc,corral_MT2 ,weight,"subleadTopDiscvcorral_MT2",";Sub-leading top disc.;corral_MT2",20,-1,1,20,0,900);
    eventPlots.fill(data.top1_disc+data.top2_disc,corral_MT2 ,weight,"topPairDiscvcorral_MT2",";top1 disc + top2 dic;corral_MT2",20,-2,2,20,0,900);
    eventPlots.fill(data.top1_disc+data.top2_disc,picky_MT2 ,weight,"topPairDiscvpicky_MT2",";top1 disc + top2 dic;picky_MT2",20,-2,2,20,0,900);
    eventPlots.fill(data.top1_disc+data.top2_disc,ak4_MT2 ,weight,"topPairDiscvak4_MT2",";top1 disc + top2 dic;ak4_MT2",20,-2,2,20,0,900);


     delete Mt2Calc;
     delete psuedoCalc;

  }

  void testTopResolution(){

    if(!setupEvent()) return;
    CORRAL::CORRALData& data = *corralReader.corral;

    if(!data.reconstructedTop) return;

    eventPlots.rewind();
    eventPlots("",process == defaults::SIGNAL);

    std::vector<RecoJetF*> recoJets;
    std::vector<TopJetMatching::TopDecayEvent::DecayID> decays;
    TopJetMatching::TopDecayEvent* topDecayEvent = CORRAL::associateDecays(&genParticleReader,&pickyJetReader,recoJets,decays);

    int nBadTops = 0;
    int nBadPartons = 0;
    int nBadJets = 0;

    vector<MomentumF> tops;
    for(const auto& t : topDecayEvent->topDecays){
      if(t.isLeptonic) return;
      if(TMath::Abs(t.top->eta()) >= 2.4 ) nBadTops++;
      if(t.diag  == TopJetMatching::BAD_PARTON) nBadPartons++;
      if(t.diag  != TopJetMatching::RESOLVED_TOP ) nBadJets++;
      tops.push_back(t.top->p4());
    }
    sort(tops.begin(), tops.end(), PhysicsUtilities::greaterPT<MomentumF>());
    if(tops.size() != 2) return;

    int realRank = -1;
    for(unsigned int iR = 0; iR < data.rankedTPairs.size(); ++iR){
      if(data.tCands[data.rankedTPairs[iR].first].type != 2 &&data.tCands[data.rankedTPairs[iR].second].type != 2 ){
        realRank = iR + 1;
        break;
      }
    }

    ++eventPlots;
    eventPlots("all__",true);
    eventPlots("good_tops__", nBadTops == 0);
    eventPlots("good_partons__", nBadTops == 0 && nBadPartons == 0 );
    eventPlots("good_jets__", nBadTops == 0 && nBadPartons == 0 && nBadJets == 0);
    eventPlots("good_pair__", nBadTops == 0 && nBadPartons == 0 && nBadJets == 0 && realRank == 1);

    eventPlots.checkPoint(0);

    const CORRAL::TCand * top1 = PhysicsUtilities::deltaR2(data.top1->mom,tops[0]) < PhysicsUtilities::deltaR2(data.top2->mom,tops[0]) ? data.top1 : data.top2;
    const CORRAL::TCand * top2 = top1 == data.top1 ? data.top2 : data.top1;

    eventPlots.revert(0);
    ++eventPlots;
    eventPlots("pt_lt150__", tops[0].pt() < 150);
    eventPlots("pt_eq150to300__", tops[0].pt() >= 150 && tops[0].pt() < 300);
    eventPlots("pt_eq300to500__", tops[0].pt() >= 300 && tops[0].pt() < 500);
    eventPlots("pt_geq500__", tops[0].pt() >= 500);

    eventPlots.fill( tops[0].pt() == 0 ? 10.0 : top1->mom.pt() / tops[0].pt(), weight, "leadTopPTRes",";Leading top p_{T,reco} / p_{T,gen}", 50,0,2);

    eventPlots.revert(0);
    ++eventPlots;
    eventPlots("pt_lt150__", tops[1].pt() < 150);
    eventPlots("pt_eq150to300__", tops[1].pt() >= 150 && tops[1].pt() < 300);
    eventPlots("pt_eq300to500__", tops[1].pt() >= 300 && tops[1].pt() < 500);
    eventPlots("pt_geq500__", tops[1].pt() >= 500);

    eventPlots.fill( tops[1].pt() == 0 ? 10.0 : top2->mom.pt() / tops[1].pt(), weight, "subleadTopPTRes",";Sub leading top p_{T,reco} / p_{T,gen}", 50,0,2);

    double ptAvg = (tops[1].pt() + tops[0].pt()) / 2;
    eventPlots.revert(0);
    ++eventPlots;
    eventPlots("pt_lt150__", ptAvg < 150);
    eventPlots("pt_eq150to300__", ptAvg >= 150 && ptAvg < 300);
    eventPlots("pt_eq300to500__", ptAvg >= 300 && ptAvg < 500);
    eventPlots("pt_geq500__", ptAvg >= 500);

    eventPlots.fill( ptAvg == 0 ? 10.0 : ( (top1->mom.pt() + top2->mom.pt() ) / 2  ) / ptAvg, weight, "avgTopPTRes",";Avg. top p_{T,reco} / p_{T,gen}", 50,0,2);

    eventPlots.fill( PhysicsUtilities::absDeltaPhi(top1->mom,top2->mom) - PhysicsUtilities::absDeltaPhi(tops[0],tops[1]), weight, "deltaPhiRes",";Top pair |#Delta#phi|_{reco} - |#Delta#phi|_{gen}", 50,-3.2,3.2);
    eventPlots.fill( PhysicsUtilities::absDeltaEta(top1->mom,top2->mom) - PhysicsUtilities::absDeltaEta(tops[0],tops[1]), weight, "deltaEtaRes",";Top pair |#Delta#eta|_{reco} - |#Delta#eta|_{gen}", 50,-3.2,3.2);
    eventPlots.fill( (tops[0].p4() + tops[1].p4()).mass() == 0 ? 10.0 : (top1->mom.p4() + top2->mom.p4()).mass() / (tops[0].p4() + tops[1].p4()).mass(), weight, "invMassRes",";top pair  mass_{reco} / mass_{gen}", 50,0,2);

    Mt2Helper * Mt2Calc    = new Mt2Helper();


    double corral_MT2   = data.reconstructedTop ? Mt2Calc->CalcMT2(&data.top1->mom,&data.top2->mom, met, 0) : 0;
    double gen_MT2   =  Mt2Calc->CalcMT2(&tops[0],&tops[1], met, 0);


    MomentumF ak4pseudoJet1; MomentumF ak4pseudoJet2;
    vector<RecoJetF*> ak4Jets;
    vector<RecoJetF*> ak4BJets;
    vector<RecoJetF*> ak4NonBJets;
    cleanJets(&ak4Reader,ak4Jets,&ak4BJets,&ak4NonBJets);
    CreatePseudoJets * psuedoCalc = new CreatePseudoJets();
    psuedoCalc->makePseudoJets(ak4Jets,ak4pseudoJet1,ak4pseudoJet2,0);
    double ak4_MT2   = Mt2Calc->CalcMT2(&ak4pseudoJet1,&ak4pseudoJet2, met, 0);

    eventPlots.fill(gen_MT2 ,weight,"gen_MT2",";gen MT2.",100,0,900);
    eventPlots.fill(corral_MT2 ,weight,"corral_MT2",";corral MT2.",100,0,900);
    eventPlots.fill(ak4_MT2 ,weight,"ak4_MT2",";ak4 MT2.",100,0,900);

    if(gen_MT2 > 0 ){
      eventPlots.fill( gen_MT2 == 0 ? 10.0 : corral_MT2 / gen_MT2, weight, "corralMT2Res",";MT2_{reco top pair}/MT2_{gen top pair}", 75,0,4);
      eventPlots.fill( gen_MT2 == 0 ? 10.0 : ak4_MT2 / gen_MT2, weight, "ak4MT2Res",";MT2_{ak4 jets}/MT2_{gen top pair}", 75,0,4);
    }




  }


  void runEvent(){
    testTopResolution();
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
