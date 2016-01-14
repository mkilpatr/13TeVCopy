/*
 * Uses the tree copier to copy over a tree!
 *
 * We use the loaded branches option so that we make the tree smaller
 *
 * Use this guy as a base class or an example!
 *
 */
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"

#include "AnalysisTools/QuickRefold/interface/MakeRefold.h"

using namespace std;
using namespace ucsbsusy;


class Analyzer : public BaseTreeAnalyzer {
public:
  Analyzer(TString fileName, TString treeName, TString outPath,  bool isMC, QuickRefold::MakeRefold *unf,
      cfgSet::ConfigSet * pars) :
        BaseTreeAnalyzer(fileName,isMC,pars),unf(unf),
        outPath(outPath), plotter (new PlotterD(3)), eventPlots(plotter)
{};
  virtual ~Analyzer() {plotter->write(outPath);};

  virtual void loadVariables(){
    load(cfgSet::EVTINFO);
    if(isMC())load(cfgSet::AK4JETS,JetReader::LOADRECO | JetReader::LOADTOPASSOC | JetReader::FILLOBJ | JetReader::LOADGEN);
    else load(cfgSet::AK4JETS,JetReader::LOADRECO | JetReader::LOADTOPASSOC | JetReader::FILLOBJ);
    load(cfgSet::ELECTRONS);
    load(cfgSet::MUONS);
    load(cfgSet::TRIGOBJS);
    if(isMC()) load(cfgSet::GENPARTICLES);
  }

  virtual void runEvent() {
    if(isMC())
      weight *= .0404;
    else weight = 1;

    if(!passSel()) return;
    unfold();
  }

  bool passSel(){
    //JSON
    if(!isMC() && hasJSONFile() && !passesLumiMask()) return false;
    //trigger
    bool passTrig = isMC() ? triggerflag & kHLT_IsoTkMu24_eta2p1_v1 :  triggerflag & kHLT_IsoTkMu24_eta2p1_v2;
    if(!passTrig) return false;
    //leptons
    if(nSelLeptons != 1 || nSecondaryLeptons != 1) return false;
    if(selectedLeptons[0]->iselectron()) return false;
    if(nBJets < 2) return false;
    if(jets[0]->pt() < 60) return false;

    return true;
  }

  void unfold() {
    CylLorentzVectorF metplepton = met->p4();
    metplepton += selectedLeptons[0]->p4();
    CylLorentzVectorF metpleptonb = metplepton;
    double scTopPairPT  = 0;
    if(PhysicsUtilities::absDeltaPhi( metplepton.phi(),bJets[0]->phi()) < PhysicsUtilities::absDeltaPhi( metplepton.phi(),bJets[1]->phi())  ){
      metpleptonb   +=  bJets[0]->p4();
      scTopPairPT = metpleptonb.pt() + bJets[1]->pt();
    } else {
      metpleptonb   +=  bJets[1]->p4();
      scTopPairPT = metpleptonb.pt() + bJets[0]->pt();
    }
    if(isMC() && process == defaults::TTBAR){

      const GenParticleF * top1 = 0;
      const GenParticleF * top2 = 0;
      for(const auto * g : genParts){
        if(TMath::Abs(g->pdgId()) != 6) continue;
        if(top1) top2 = g;
        else top1 = g;
      }
      unf->setMeasBin(0,scTopPairPT);
      unf->setTruthBin(0,top1->pt() + top2->pt());
      unf->fillInput(weight);

    } else if(isMC() && process != defaults::TTBAR) {
      unf->setMeasBin(0,scTopPairPT);
      unf->fillBKG(weight);
    } else {
      unf->setMeasBin(0,scTopPairPT);
      unf->fillMeasurement(weight);
    }
  }

  QuickRefold::MakeRefold *       unf;
  TString outPath;
  PlotterD*                       plotter;
  Polybook                        eventPlots;

};




void run() {

  //Configuration
  cfgSet::loadDefaultConfigurations();
  cfgSet::setJSONFile("/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_4_7/src/data/JSON/Cert_246908-251883_13TeV_PromptReco_Collisions15_JSON_v2.txt");
  cfgSet::ConfigSet cfg = cfgSet::ol_search_set;
  cfg.jets.minPt = 20;
  cfg.jets.minBJetPt = 20;
  cfg.jets.cleanJetsvSecondaryLeptons = true;

  TString MCFileName = "MC_extraSkim.root";
  TString MCTreeName = "Events";


  TString DataMUFileName = "pieces/singlemu-2015b-pr_ntuple_lepSkim.root";
  TString DataMUTreeName = "Events";

  TString DataEFileName = "pieces/singleel-2015b-pr_ntuple_lepSkim.root";
  TString DataETreeName = "Events";



  //Setup unfolding
  QuickRefold::MakeRefold * ttbarUnfold = new QuickRefold::MakeRefold (1,1,QuickRefold::SUBTRACT,1);
  float genBins[] = {0,100,180,250,300,400,500};
  float recoBins[] = {0,100,140,200,260,350,500};
  ttbarUnfold->addMeasAxis(0,"scalarTopDecayPT",6,recoBins);
  ttbarUnfold->addTruthAxis(0,"TopPairPT",6,genBins);
  ttbarUnfold->stopSetup();

  //Fill data and MC for unfolding
  Analyzer  * a = new Analyzer(MCFileName,MCTreeName, "origMC_ttbarVal.root",true,ttbarUnfold,&cfg);
  a->analyze();
  delete a;
  a = new Analyzer(DataMUFileName,DataMUTreeName, "dataMU_ttbarVal.root",false,ttbarUnfold, &cfg);
  a->analyze();
  delete a;

  //Actually unfold
  ttbarUnfold->unfold(RooUnfold::kBayes,4,0,0);
  ttbarUnfold->writeDiag("ttbarCorr_diag.root","recreate");
  ttbarUnfold->writeCorr("ttbarCorr.root","recreate","TOPPAIRPT",true);

  //If you wanted to try it out change as follows
  cfg.corrections.ttbarCorrectionFile = "ttbarCorr.root";
  cfg.corrections.ttbarCorrections = TtbarCorrectionSet::TOPPAIRPT;
}


#endif


void unfoldingExample() {
  run();

}

