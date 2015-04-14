#include <vector>
#include <assert.h>
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TH1F.h"
#include "TCanvas.h"
#if !defined(__CINT__) || defined(__MAKECINT__)
//#include "QGHelper.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/Parang/interface/Plotter.h"
#include "AnalysisTools/Parang/interface/Polybook.h"
#include "AnalysisTools/TreeReader/interface/Defaults.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"

using namespace std;
using namespace ucsbsusy;

class Analyze : public BaseTreeAnalyzer{
public:
  PlotterD*      plotter;
  Polybook       eventPlots;

  Analyze(TString fname, string treeName, bool isMCTree, ConfigPars * pars) :
	  BaseTreeAnalyzer(fname, treeName, isMCTree, pars ),
	  plotter (new PlotterD(3)),
	  eventPlots(plotter)
  {}; // Analyze()

  void makePlots(JetReader &reader, TString tag){
    RecoJetFCollection &reco = reader.recoJets;
    GenJetFCollection  &gen  = reader.genJets;

    std::vector<RecoJetF*> recoVec;
    std::vector<GenJetF*>  genVec;

    vector<int> rJpt(5,0);
    for(auto& j : reco){
      if (TMath::Abs(j.eta())>2.4) continue;
      recoVec.push_back( &j );
      if (j.pt()>20 ) ++rJpt[0];
      if (j.pt()>30 ) ++rJpt[1];
      if (j.pt()>50 ) ++rJpt[2];
      if (j.pt()>70 ) ++rJpt[3];
      if (j.pt()>100) ++rJpt[4];
    } // reco jets

    vector<int> gJpt(5,0);
    for(auto& j : gen){
	  if (TMath::Abs(j.eta())>2.4) continue;
      genVec.push_back( &j );
      if (j.pt()>20 ) ++gJpt[0];
      if (j.pt()>30 ) ++gJpt[1];
      if (j.pt()>50 ) ++gJpt[2];
      if (j.pt()>70 ) ++gJpt[3];
      if (j.pt()>100) ++gJpt[4];
    } // gen jets

    eventPlots.rewind();
    eventPlots("",true);

    // fill reco jet multplicity (various pt cuts)
    eventPlots.fill( rJpt[0], 1, "rec_Jpt20",  ";# of reco jets pt20",  26, -0.5, 25.5, tag );
    eventPlots.fill( rJpt[1], 1, "rec_Jpt30",  ";# of reco jets pt30",  26, -0.5, 25.5, tag );
    eventPlots.fill( rJpt[2], 1, "rec_Jpt50",  ";# of reco jets pt50",  26, -0.5, 25.5, tag );
    eventPlots.fill( rJpt[3], 1, "rec_Jpt70",  ";# of reco jets pt70",  26, -0.5, 25.5, tag );
    eventPlots.fill( rJpt[4], 1, "rec_Jpt100", ";# of reco jets pt100", 26, -0.5, 25.5, tag );

    // fill gen jet multplicity (various pt cuts)
    eventPlots.fill( gJpt[0], 1, "gen_Jpt20",  ";# of gen jets pt20",  26, -0.5, 25.5, tag );
    eventPlots.fill( gJpt[1], 1, "gen_Jpt30",  ";# of gen jets pt30",  26, -0.5, 25.5, tag );
    eventPlots.fill( gJpt[2], 1, "gen_Jpt50",  ";# of gen jets pt50",  26, -0.5, 25.5, tag );
    eventPlots.fill( gJpt[3], 1, "gen_Jpt70",  ";# of gen jets pt70",  26, -0.5, 25.5, tag );
    eventPlots.fill( gJpt[4], 1, "gen_Jpt100", ";# of gen jets pt100", 26, -0.5, 25.5, tag );

    // fill reco pt,eta
    if (reco.size()>=1 && TMath::Abs(reco[0].eta())<2.4) eventPlots.fill( reco[0].pt(),  1, "rec_j0pt",  ";p_{t} reco j0", 25,  0, 1000, tag );
    if (reco.size()>=2 && TMath::Abs(reco[1].eta())<2.4) eventPlots.fill( reco[1].pt(),  1, "rec_j1pt",  ";p_{t} reco j1", 25,  0, 1000, tag );
    if (reco.size()>=3 && TMath::Abs(reco[2].eta())<2.4) eventPlots.fill( reco[2].pt(),  1, "rec_j2pt",  ";p_{t} reco j2", 25,  0, 1000, tag );
    if (reco.size()>=4 && TMath::Abs(reco[3].eta())<2.4) eventPlots.fill( reco[3].pt(),  1, "rec_j3pt",  ";p_{t} reco j3", 25,  0, 1000, tag );
    if (reco.size()>=5 && TMath::Abs(reco[4].eta())<2.4) eventPlots.fill( reco[4].pt(),  1, "rec_j4pt",  ";p_{t} reco j4", 25,  0, 1000, tag );
    if (reco.size()>=6 && TMath::Abs(reco[5].eta())<2.4) eventPlots.fill( reco[5].pt(),  1, "rec_j5pt",  ";p_{t} reco j5", 25,  0, 1000, tag );
    if (reco.size()>=1 && reco[0].pt()>20) eventPlots.fill( reco[0].eta(), 1, "rec_j0eta", ";eta reco j0", 20, -5, 5, tag );
    if (reco.size()>=2 && reco[1].pt()>20) eventPlots.fill( reco[1].eta(), 1, "rec_j1eta", ";eta reco j1", 20, -5, 5, tag );
    if (reco.size()>=3 && reco[2].pt()>20) eventPlots.fill( reco[2].eta(), 1, "rec_j2eta", ";eta reco j2", 20, -5, 5, tag );
    if (reco.size()>=4 && reco[3].pt()>20) eventPlots.fill( reco[3].eta(), 1, "rec_j3eta", ";eta reco j3", 20, -5, 5, tag );
    if (reco.size()>=5 && reco[4].pt()>20) eventPlots.fill( reco[4].eta(), 1, "rec_j4eta", ";eta reco j4", 20, -5, 5, tag );
    if (reco.size()>=6 && reco[5].pt()>20) eventPlots.fill( reco[5].eta(), 1, "rec_j5eta", ";eta reco j5", 20, -5, 5, tag );

    // fill gen pt,eta
    if (gen.size()>=1 && TMath::Abs(gen[0].eta())<2.4) eventPlots.fill( gen[0].pt(),  1, "gen_j0pt",  ";p_{t} gen j0", 25,  0, 1000, tag );
    if (gen.size()>=2 && TMath::Abs(gen[1].eta())<2.4) eventPlots.fill( gen[1].pt(),  1, "gen_j1pt",  ";p_{t} gen j1", 25,  0, 1000, tag );
    if (gen.size()>=3 && TMath::Abs(gen[2].eta())<2.4) eventPlots.fill( gen[2].pt(),  1, "gen_j2pt",  ";p_{t} gen j2", 25,  0, 1000, tag );
    if (gen.size()>=4 && TMath::Abs(gen[3].eta())<2.4) eventPlots.fill( gen[3].pt(),  1, "gen_j3pt",  ";p_{t} gen j3", 25,  0, 1000, tag );
    if (gen.size()>=5 && TMath::Abs(gen[4].eta())<2.4) eventPlots.fill( gen[4].pt(),  1, "gen_j4pt",  ";p_{t} gen j4", 25,  0, 1000, tag );
    if (gen.size()>=6 && TMath::Abs(gen[5].eta())<2.4) eventPlots.fill( gen[5].pt(),  1, "gen_j5pt",  ";p_{t} gen j5", 25,  0, 1000, tag );
    if (gen.size()>=1 && gen[0].pt()>20) eventPlots.fill( gen[0].eta(), 1, "gen_j0eta", ";eta gen j0", 20, -5, 5, tag );
    if (gen.size()>=2 && gen[1].pt()>20) eventPlots.fill( gen[1].eta(), 1, "gen_j1eta", ";eta gen j1", 20, -5, 5, tag );
    if (gen.size()>=3 && gen[2].pt()>20) eventPlots.fill( gen[2].eta(), 1, "gen_j2eta", ";eta gen j2", 20, -5, 5, tag );
    if (gen.size()>=4 && gen[3].pt()>20) eventPlots.fill( gen[3].eta(), 1, "gen_j3eta", ";eta gen j3", 20, -5, 5, tag );
    if (gen.size()>=5 && gen[4].pt()>20) eventPlots.fill( gen[4].eta(), 1, "gen_j4eta", ";eta gen j4", 20, -5, 5, tag );
    if (gen.size()>=6 && gen[5].pt()>20) eventPlots.fill( gen[5].eta(), 1, "gen_j5eta", ";eta gen j5", 20, -5, 5, tag );

    // HT
    eventPlots.fill( JetKinematics::ht(recoVec,20,2.4), 1, "rec_ht", ";reco H_{t}", 25, 0, 4000, tag );
    eventPlots.fill( JetKinematics::ht(genVec ,20,2.4), 1, "gen_ht", ";gen H_{t}",  25, 0, 4000, tag );

    // MHT
    eventPlots.fill( JetKinematics::mht(recoVec,20,2.4).pt(), 1, "rec_mht", ";reco MHT", 25, 0, 1000, tag );
    eventPlots.fill( JetKinematics::mht(genVec ,20,2.4).pt(), 1, "gen_mht", ";gen MHT",  25, 0, 1000, tag );

    // pt resolultion (as a function of jet pt)
    for(const auto& j : reco){
      if (TMath::Abs(j.eta())>2.4) continue;
      if (!j.genJet()) continue;
      if (!(j.genJet()->pt()>0)) continue;
      if (j.pt()>020 && j.pt()<050) eventPlots.fill( j.pt()/j.genJet()->pt(), 1, "jetRes_jpt020t050", ";jet resolution (20-50)",   20, 0, 2, tag );
      if (j.pt()>050 && j.pt()<100) eventPlots.fill( j.pt()/j.genJet()->pt(), 1, "jetRes_jpt050t100", ";jet resolution (50-100)",  20, 0, 2, tag );
      if (j.pt()>100 && j.pt()<300) eventPlots.fill( j.pt()/j.genJet()->pt(), 1, "jetRes_jpt100t300", ";jet resolution (100-300)", 20, 0, 2, tag );
      if (j.pt()>300              ) eventPlots.fill( j.pt()/j.genJet()->pt(), 1, "jetRes_jpt300tinf", ";jet resolution (300+)",    20, 0, 2, tag );
    } // reco jets

  } // makePlots()


  void runEvent(){
    if(nPV < 1) return;
    makePlots( ak4Reader,      "ak4__" );
    makePlots( pickyJetReader, "pky__" );

    for(auto& j : pickyJetReader.recoJets)
      j.setP4( CylLorentzVectorF(correctedPickyPT(j.pt(),j.eta(),pickyJetReader.jetarea_->at(j.index()),rho),j.eta(),j.phi(),j.mass()) );
    std::sort(pickyJetReader.recoJets.begin(), pickyJetReader.recoJets.end(), PhysicsUtilities::greaterPT<RecoJetF>());
    makePlots( pickyJetReader, "cor__" );
  } // runEvent()

  void out(TString outputPath){
    plotter->write(outputPath);
  } // out()

}; // BaseTreeAnalyzer


#endif



void getJetCheckPlots(string fname = "evttree_lowStat_QCD_test.root", TString fnameOut = "out", string treeName = "TestAnalyzer/Events", bool isMCTree = true) {
  //Analyze a(fname, treeName, isMCTree, usePuppi);
  fnameOut = "plots_" + fnameOut + ".root";
  ConfigPars pars;
  //pars.minJetPt = 20;
  //pars.maxJetEta = 2.4;
  pars.correctPickyPT = false;
  Analyze a(fname, treeName, isMCTree, &pars);
  //a.setDefaultJets(BaseTreeAnalyzer::AK4JETS);
  a.analyze();
  a.out(fnameOut);
} // getJetCheckPlots()
