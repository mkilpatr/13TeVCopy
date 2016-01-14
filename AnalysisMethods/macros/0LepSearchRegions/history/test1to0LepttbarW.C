#include <vector>
#include <assert.h>
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TH1F.h"
#include "TCanvas.h"
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
//#include "selectionVariables.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"

#include "AnalysisTools/Parang/interface/Plotter.h"
#include "AnalysisTools/Parang/interface/Polybook.h"
#include "AnalysisTools/Parang/interface/Panvariate.h"

#include "AnalysisTools/KinematicVariables/interface/Topness.h"
#include "AnalysisMethods/macros/0LepSearchRegions/variableCalculator0L.h"
using namespace std;
using namespace ucsbsusy;

class Analyze : public BaseTreeAnalyzer{
public:
  PlotterD*                       plotter;
  Polybook                        eventPlots;





  Analyze(TString fname, string treeName, bool isMCTree, ConfigPars * pars) : BaseTreeAnalyzer(fname,treeName, isMCTree,pars), plotter (new PlotterD(3)), eventPlots(plotter)
  {
  };

  void loadVariables() override{
    load(EVTINFO);
    load(AK4JETS,JetReader::LOADRECO | JetReader::LOADJETSHAPE | JetReader::FILLOBJ);
    load(PICKYJETS,JetReader::LOADRECO | JetReader::LOADJETSHAPE | JetReader::FILLOBJ);
    load(ELECTRONS);
    load(MUONS);
    load(PFCANDS);
    load(GENPARTICLES);
  load(CMSTOPS);
  }



  MomentumF getInvisibleDaughters(const GenParticleF * mom, int& nInv){
    MomentumF invisbleMom;
    for(unsigned int iD = 0; iD < mom->numberOfDaughters(); ++iD){
        if(ParticleInfo::isInvisible(mom->daughter(iD)->pdgId())){
          invisbleMom.p4() += mom->daughter(iD)->p4();
          nInv++;
        } else{
          invisbleMom.p4() += getInvisibleDaughters(mom->daughter(iD),nInv).p4();
        }
    }
    return invisbleMom;
  }

  void runEvent(){
    TString prefix = "";
    weight *= 4;

    std::vector<RecoJetF*>     cleanjets            ;
    std::vector<RecoJetF*>     cleanbJets   ;
    std::vector<RecoJetF*>     cleannonBJets;
    int   cleannJets;
    int   cleannBJets;

    const_cast <ConfigPars&>(config).cleanJetsvSelectedLeptons_ = true;
    cleanjets.clear(); cleanbJets.clear(); cleannonBJets.clear();
    if(defaultJets && defaultJets->isLoaded()){
      cleanJets(defaultJets,cleanjets,&cleanbJets,&cleannonBJets);
    }
    cleannJets    = cleanjets.size();
    cleannBJets   = cleanbJets.size();



    bool passLepSel = selectedLeptons.size() == 1 && selectedLeptons[0]->pt() >= (selectedLeptons[0]->iselectron() ? 40 : 30);

    MomentumF cutMET = *met;
    if(selectedLeptons.size()){
      cutMET.p4()  += selectedLeptons[0]->p4();


//      cutMET.p4() *= min(1.0,0.900191 +cutMET.pt()*6.91181e-05);
//      cutMET.p4() *= min(1.0, 1.1905 +-0.0011767*cutMET.pt() + 9.97764e-07*cutMET.pt()*cutMET.pt());
    }

    MomentumF wMom;
    MomentumF vMom;

    for(const auto* p : genParts){
      if(!ParticleInfo::isEWKBoson(p->pdgId())) continue;
      int nInv = 0;
      MomentumF tempInv = getInvisibleDaughters(p, nInv);
      if(nInv == 0) continue;
      wMom.p4() += p->p4();
      vMom.p4() += tempInv.p4();
    }


//    MomentumF cutMET2 = *met;
//    if(selectedLeptons.size()){
//      cutMET2.p4()  += selectedLeptons[0]->p4();
//      cutMET.p4() *= .9;
//    }


    bool pass0LepPresel = (selectedLeptons.size() == 0 && nVetoedTaus == 0 && nSecondaryLeptons == 0) && (cutMET.pt() >= 200 && nJets >= 5 && nBJets >= 1);
    bool pass0LepLoosePresel  (cutMET.pt() >= 200 && nJets >= 5 && nBJets >= 1);


    bool pass1LepPresel = selectedLeptons.size() == 1 && PhysicsUtilities::absDeltaPhi(cutMET,*selectedLeptons[0]) < 1  && (cutMET.pt() >= 200 && cleannJets >= 5 && cleannBJets >= 1);

    VariableCalculator0L vars;
    vars.processVariables(this,&ak4Reader,pass0LepPresel ? jets : cleanjets,pass0LepPresel ? jets : cleanjets,&cutMET,cttTops); // calculate variables

    eventPlots.rewind();

    eventPlots("0LepPre__", pass0LepPresel);
    eventPlots("0LepNoVetoPre__", pass0LepLoosePresel);
    eventPlots("1LepPre__", pass1LepPresel);
    eventPlots("1LepTightPre__", pass1LepPresel && passLepSel);
    ++eventPlots;
    eventPlots("",vars.dPhiMET12 > 1 && vars.dPhiMET3 > .5 && vars.mtB01MET > 175);
    eventPlots("noMTB__",vars.dPhiMET12 > 1 && vars.dPhiMET3 > .5);
    eventPlots("noDPHi__",true);

    ++eventPlots;
    eventPlots("nTops_incl__",true);
    eventPlots("nTops_eq0__",vars.NCTTstd == 0);
    eventPlots("nTops_geq1__",vars.NCTTstd >= 1);

    ++eventPlots;
    eventPlots("nBs_geq1__",true);
    eventPlots("nBs_eq1__",(pass0LepPresel ? nBJets :cleannBJets)   == 1);
    eventPlots("nBs_gt1__",(pass0LepPresel ? nBJets :cleannBJets)   > 1);

    ++eventPlots;
    eventPlots("all__", true);
    eventPlots("W__", process == defaults::SINGLE_W);
    eventPlots("ttbar__", process == defaults::TTBAR);

    eventPlots.fill(1,weight,"nEvents",";nEvents", 1, .5,1.5);
    eventPlots.fill(cutMET.pt(),weight,"met",";MET", 20, 100,900);
    eventPlots.fill(cutMET.pt(),weight,"coarse_met",";MET", 5, 200,700);

    if(selectedLeptons.size()) eventPlots.fill(selectedLeptons[0]->pt(),weight,"lepPT",";LeptonPT", 100, 0,500);

    eventPlots.fill((met->p4() + wMom.p4() - vMom.p4()).pt(),weight,"metplep",";#slash{E_{T}} + lepton", 40, 0,1000);
    eventPlots.fill(wMom.pt(),weight,"wPT",";wPT", 40, 0,1000);

    if(wMom.pt() > 0) eventPlots.profile(wMom.pt(),vMom.pt()/wMom.pt(),weight,"neutrino_o_w_profile",";p_{T}(W);p_{T}(#nu)/p_{T}(W)",40, 0,1000,"s");
    if((met->p4() + wMom.p4() - vMom.p4()).pt() > 0) eventPlots.profile((met->p4() + wMom.p4() - vMom.p4()).pt(),met->pt()/(met->p4() + wMom.p4() - vMom.p4()).pt(),weight,"met_o_metplep_profile",";#slash{E_{T}};#slash{E_{T}}/(#slash{E_{T}} + lepton)",40, 0,1000,"s");



  }

  void out(TString outputPath){
    plotter->write(outputPath);
  }
};




#endif



void test1to0LepttbarW(string fname = "evttree.root", string treeName = "Events", bool isMCTree = true) {

  BaseTreeAnalyzer::ConfigPars pars;
  pars.cleanJetsvSelectedLeptons_ = false;
  pars.defaultJetCollection = BaseTreeAnalyzer::AK4JETS;
  pars.minSelMuPt = 5;
  pars.minSelEPt = 5;
//  pars.vetoedMuon = (&MuonF::ismultiisovetomuonl);
//  pars.vetoedElectron = (&ElectronF::ismultiisovetoelectronl);
  pars.vetoedMuon = (&MuonF::isgoodpogmuon);
  pars.vetoedElectron = (&ElectronF::isgoodpogelectron);


  Analyze a(fname, treeName, isMCTree,&pars);
  TString name = fname;
  name = ((TObjString*)name.Tokenize(".")->At(0))->GetString();
  a.analyze(100000,-1);
  a.out(TString::Format("%s_plots.root",name.Data()));
}
