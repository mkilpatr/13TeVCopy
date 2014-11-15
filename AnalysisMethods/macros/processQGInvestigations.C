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

#include "AnalysisTools/Parang/interface/Plotter.h"
#include "AnalysisTools/Parang/interface/Polybook.h"

using namespace std;
using namespace ucsbsusy;
enum TYPE { Q, G, C, B, PU, U};

struct subJet  {
  subJet() {};
  subJet( JetF * inJet) : jet(inJet),
      betaStar (-1),
      qgl      (-1),
      csv      (-1),
      ptD      (-1),
      axis1    (-1),
      axis2    (-1),
      mult     (-1),
      matchGen (0),
      type(U),
      nearDR(99)
  {};
  JetF * jet;
  float betaStar;
  float qgl     ;
  float csv     ;
  float ptD     ;
  float axis1   ;
  float axis2   ;
  int   mult    ;
  const GenJetF*   matchGen;
  TYPE type;
  float nearDR  ;
};

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
    recoJets.clear();
    genJets.clear();
    if(ak4Reader.isLoaded()){
      for(unsigned int iJ = 0; iJ <  ak4Reader.recoJets.size(); ++iJ){
        RecoJetF& jet = ak4Reader.recoJets.at(iJ);
        if(!isGoodJet(jet)) continue;
        recoJets.emplace_back(&jet);
        recoJets.back().csv = jet.csv();
        recoJets.back().betaStar =  ak4Reader.jetbetaStar_->at(iJ);
        recoJets.back().qgl =  ak4Reader.jetqgl_     ->at(iJ);
        recoJets.back().ptD   =  ak4Reader.jetptD_     ->at(iJ);
        recoJets.back().axis1 =  ak4Reader.jetaxis1_   ->at(iJ);
        recoJets.back().axis2 =  ak4Reader.jetaxis2_   ->at(iJ);
        recoJets.back().mult   =  ak4Reader.jetMult_    ->at(iJ);
        recoJets.back().matchGen = jet.genJet();

      }
      for(unsigned int iJ = 0; iJ <  ak4Reader.genJets.size(); ++iJ){
        GenJetF& jet = ak4Reader.genJets.at(iJ);
        if(!isGoodJet(jet)) continue;
        genJets.emplace_back(&jet);
        genJets.back().ptD    = ak4Reader.genjetptD_  ->at(iJ);
        genJets.back().axis1  = ak4Reader.genjetaxis1_->at(iJ);
        genJets.back().axis2  = ak4Reader.genjetaxis2_->at(iJ);
        genJets.back().mult   = ak4Reader.genjetMult_ ->at(iJ);
      }
    }

    if(genParticleReader.isLoaded()){
      genParts.clear();
      genParts.reserve(4);
      for(auto& p : genParticleReader.genParticles.particles){
        if(p.status() == 23)
          genParts.push_back(&p);
      }
    }

    if(evtInfoReader.isLoaded()) {
      run   = evtInfoReader.run;
      lumi  = evtInfoReader.lumi;
      event = evtInfoReader.event;
      nPV   = evtInfoReader.nPV;
      rho   = evtInfoReader.rho;
      met   = &evtInfoReader.met;
    }
  }

  void categorize(std::vector<subJet>& jets, bool isReco) {
    for(unsigned int iJ = 0; iJ < jets.size(); ++iJ){
      double nearDR = 0;
      int foundPart = PhysicsUtilities::findNearestDRDeref((*jets[iJ].jet),genParts,nearDR,.4,20);;
      jets[iJ].nearDR = nearDR;
      if(foundPart >= 0){
        int pdgId = TMath::Abs(genParts[foundPart]->pdgId());
        if(pdgId == 21) jets[iJ].type = G;
        if(pdgId >= 1 && pdgId < 4) jets[iJ].type = Q;
        if(pdgId == 4) jets[iJ].type = C;
        if(pdgId == 5) jets[iJ].type = B;
      }
      if(isReco && jets[iJ].type == U && jets[iJ].matchGen == 0)
        jets[iJ].type = PU;
    }
  }

  void makePlots(std::vector<subJet>& jets, bool isReco){
    categorize(jets,isReco);
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
