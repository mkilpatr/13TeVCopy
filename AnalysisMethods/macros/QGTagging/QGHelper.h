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
	  blf0     (-1),
	  blf1     (-1),
	  blf2     (-1),
	  matchGen  (0),
	  type      (U),
	  nearDR   (99),
	  origInd  (-1)
  {};

  JetF * jet;
  float betaStar;
  float qgl     ;
  float csv     ;
  float ptD     ;
  float axis1   ;
  float axis2   ;
  int   mult    ;
  float blf0    ;
  float blf1    ;
  float blf2    ;
  float jmQGL   ;
  int   jmMult  ;
  float jmPtD   ;
  float jmAxis2 ;
  const GenJetF*   matchGen;
  TYPE type     ;
  float nearDR  ;
  int origInd   ;
}; // subJet


void procesSubjets(vector<subJet>& recoJets, vector<subJet>& genJets, BaseTreeAnalyzer* analyzer){
  recoJets.clear();
  genJets.clear();
  if(analyzer->ak4Reader.isLoaded()){
    for(unsigned int iJ = 0; iJ <  analyzer->ak4Reader.recoJets.size(); ++iJ){
      RecoJetF& jet = analyzer->ak4Reader.recoJets.at(iJ);
      //if(!analyzer->isGoodJet(jet)) continue;
      if(jet.pt()<20) continue; // to get high eta jets
      recoJets.emplace_back(&jet);
      recoJets.back().origInd  = jet.index();
      recoJets.back().csv      = jet.csv();
      recoJets.back().betaStar = analyzer->ak4Reader.jetbetaStar_->at(jet.index());
      recoJets.back().qgl      = analyzer->ak4Reader.jetqgl_     ->at(jet.index());
      recoJets.back().ptD      = analyzer->ak4Reader.jetptD_     ->at(jet.index());
      recoJets.back().axis1    = analyzer->ak4Reader.jetaxis1_   ->at(jet.index());
      recoJets.back().axis2    = analyzer->ak4Reader.jetaxis2_   ->at(jet.index());
      recoJets.back().mult     = analyzer->ak4Reader.jetMult_    ->at(jet.index());
      recoJets.back().blf0     = analyzer->ak4Reader.jetblf0_    ->at(jet.index());
      recoJets.back().blf1     = analyzer->ak4Reader.jetblf1_    ->at(jet.index());
      recoJets.back().blf2     = analyzer->ak4Reader.jetblf2_    ->at(jet.index());
      recoJets.back().jmQGL    = analyzer->ak4Reader.jetjmQGL_   ->at(jet.index());
      recoJets.back().jmMult   = analyzer->ak4Reader.jetjmMult_  ->at(jet.index());
      recoJets.back().jmPtD    = analyzer->ak4Reader.jetjmPtD_   ->at(jet.index());
      recoJets.back().jmAxis2  = analyzer->ak4Reader.jetjmAxis2_ ->at(jet.index());
      recoJets.back().matchGen = jet.genJet();
    } // iJ < recoJets.size()
    for(unsigned int iJ = 0; iJ <  analyzer->ak4Reader.genJets.size(); ++iJ){
      GenJetF& jet = analyzer->ak4Reader.genJets.at(iJ);
      //if(!analyzer->isGoodJet(jet)) continue;
      if(jet.pt()<20) continue; // to get high eta jets
      genJets.emplace_back(&jet);
      genJets.back().origInd = jet.index();
      genJets.back().ptD    = analyzer->ak4Reader.genjetptD_  ->at(jet.index());
      genJets.back().axis1  = analyzer->ak4Reader.genjetaxis1_->at(jet.index());
      genJets.back().axis2  = analyzer->ak4Reader.genjetaxis2_->at(jet.index());
      genJets.back().mult   = analyzer->ak4Reader.genjetMult_ ->at(jet.index());
      genJets.back().blf0   = analyzer->ak4Reader.genjetblf0_ ->at(jet.index());
      genJets.back().blf1   = analyzer->ak4Reader.genjetblf1_ ->at(jet.index());
      genJets.back().blf2   = analyzer->ak4Reader.genjetblf2_ ->at(jet.index());
    } // iJ < genJets.size()
  } // ak4Reader.isLoaded()

  if(analyzer->genParticleReader.isLoaded()){
    analyzer->genParts.clear();
    analyzer->genParts.reserve(4);
    //for(auto& p : analyzer->genParticleReader.genParticles.particles){
    for(auto& p : analyzer->genParticleReader.genParticles){
      if(p.status() == 23)
        analyzer->genParts.push_back(&p);
    } // particles
  } // genParticleReader.isLoaded()

  if(analyzer->evtInfoReader.isLoaded()) {
    analyzer->run   = analyzer->evtInfoReader.run;
    analyzer->lumi  = analyzer->evtInfoReader.lumi;
    analyzer->event = analyzer->evtInfoReader.event;
    analyzer->nPV   = analyzer->evtInfoReader.nPV;
    analyzer->rho   = analyzer->evtInfoReader.rho;
    analyzer->met   = &analyzer->evtInfoReader.met;
  } // evtInfoReader.isLoaded()

} // procesSubjets()

void categorize(std::vector<subJet>& jets, std::vector<GenParticleF*>& genParts, bool isReco){
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
  } // iJ < jets.size()
} // categorize()

#endif
