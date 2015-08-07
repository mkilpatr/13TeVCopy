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


#include "AnalysisTools/Parang/interface/Plotter.h"
#include "AnalysisTools/Parang/interface/Polybook.h"
#include "AnalysisTools/Parang/interface/Panvariate.h"


using namespace std;
using namespace ucsbsusy;

enum UnfStatus {UNF,UNF_DATA,UNF_NONE};
enum Type {MC, EDATA,MUDATA};

class Analyzer : public BaseTreeAnalyzer {
public:
  Analyzer(TString fileName, TString treeName, TString outPath,  Type type, bool testSF,
      UnfStatus status, QuickRefold::MakeRefold *unf,
      cfgSet::ConfigSet * pars) :
    BaseTreeAnalyzer(fileName,treeName,type == MC,pars), testSF(testSF),status(status),type(type),unf(unf),
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

    if(testSF && process == defaults::TTBAR ){
      const GenParticleF * top1 = 0;
      const GenParticleF * top2 = 0;
      for(const auto * g : genParts){
        if(TMath::Abs(g->pdgId()) != 6) continue;
        if(top1) top2 = g;
        else top1 = g;
      }
      weight *= TMath::Sqrt(TMath::Exp(.156 -0.00137*std::min(400.0,double(top1->pt())))*TMath::Exp(.156 -0.00137*std::min(400.0,double(top2->pt()))));
    }

    if(!isMC() && hasJSONFile() && !passesLumiMask())
      return;

    unfold();
    plot();
  }

  void unfold() {
    bool passTrig = isMC() ? triggerflag & kHLT_IsoTkMu24_eta2p1_v1 :  triggerflag & kHLT_IsoTkMu24_eta2p1_v2;
    if(!passTrig) return;

    if(nSelLeptons != 1 || nVetoedLeptons != 1) return; //single lepton selection
    if(selectedLeptons[0]->iselectron()) return; //only muons
//    if(nJets < 3) return; //three jets
    if(nBJets < 2) return; //two b-jets
    if(jets[0]->pt() < 60) return; //QCD killing

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
    if(status == UNF && process == defaults::TTBAR){

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

    } else if(status == UNF && process != defaults::TTBAR) {
      unf->setMeasBin(0,scTopPairPT);
      unf->fillBKG(weight);
    } else if(status == UNF_DATA ){
      unf->setMeasBin(0,scTopPairPT);
      unf->fillMeasurement(weight);
    }
  }

  void plot(){
    TString prefix = "";
    eventPlots.rewind();
    bool                          classified    = false;

    bool passMuTrig = isMC() ? triggerflag & kHLT_IsoTkMu24_eta2p1_v1 :  triggerflag & kHLT_IsoTkMu24_eta2p1_v2;
    bool passETrig =  isMC() ? triggerflag & kHLT_Ele32_eta2p1_WP75_Gsf_v1 :  triggerflag & kHLT_Ele32_eta2p1_WPLoose_Gsf_v1;
    bool passDiLepTrig =  passMuTrig || passETrig;
    if(!isMC()){
      passDiLepTrig = type == MUDATA ? (passMuTrig || passETrig) :  (!passMuTrig && passETrig);
    }


    eventPlots("data__", !classified && (classified = ( !isMC()  )));
    eventPlots("MC__", !classified);
    eventPlots("ttbar__", !classified && (classified = (process == defaults::TTBAR   )));
    eventPlots("Wlnu__", !classified && (classified = (process == defaults::SINGLE_W   )));
    eventPlots("Zll__", !classified && (classified = (process == defaults::SINGLE_Z   )));
    eventPlots("other__", !classified);

    ++eventPlots;
    bool passSingleLep = nSelLeptons == 1 && nVetoedLeptons == 1 && nBJets >= 2 && jets[0]->pt() >= 60;
    bool passDoubleLep = nVetoedLeptons == 2 && nBJets>= 1 && nJets >=2 ;
    if(passDoubleLep && vetoedLeptons[0]->iselectron() == vetoedLeptons[1]->iselectron() ){
      if((vetoedLeptons[0]->p4()+ vetoedLeptons[1]->p4()).mass() < 100 ) passDoubleLep = false;
    }
    eventPlots("e__", passSingleLep && vetoedLeptons[0]->iselectron() && passETrig);
    eventPlots("mu__", passSingleLep && vetoedLeptons[0]->ismuon() && passMuTrig);
    eventPlots("ll__", passDoubleLep && passDiLepTrig);



    //reco jet plots
    eventPlots.fill(nJets,weight,"njets",";N. of jets",8,1.5,9.5,prefix);
    eventPlots.fill(nBJets,weight,"nbjets",";N. of medium b-tags",4,0.5,4.5,prefix);

    double ht = JetKinematics::ht(jets);
    eventPlots.fill(ht,weight,"ht","H_{T}",70,0,700,prefix);


    double mt = JetKinematics::transverseMass(*vetoedLeptons[0],*met);
    eventPlots.fill(mt,weight,"mt","m_{T}",40,0,400,prefix);

    for(unsigned int iJ = 0; iJ < jets.size(); ++iJ){
      TString name = TString::Format("j%u_pt",iJ);
      TString title = TString::Format(";Jet %u p_{T}",iJ);
      if(iJ > 3){
        name = "jgt3_pt";
        title = ";Jet > 3 p_{T}";
      }
      eventPlots.fill(jets[iJ]->pt(),weight,name,title,40,0,400,prefix);
    }
    for(unsigned int iJ = 0; iJ < bJets.size(); ++iJ){
      TString name = TString::Format("bj%u_pt",iJ);
      TString title = TString::Format(";Bjet %u p_{T}",iJ);
      if(iJ > 1){
        name = "bjgt1_pt";
        title = ";BJet > 1 p_{T}";
      }
      eventPlots.fill(bJets[iJ]->pt(),weight,name,title,40,0,400,prefix);
    }

    eventPlots.fill(met->pt(),weight,"met",";#slash{E}_{T}",40,0,400,prefix);
    eventPlots.fill(vetoedLeptons[0]->pt(),weight,"lepton1",";lepton 1 p_{T}",40,0,400,prefix);
    if(nVetoedLeptons > 1){
      eventPlots.fill(vetoedLeptons[1]->pt(),weight,"lepton2",";lepton 2 p_{T}",40,0,400,prefix);
      eventPlots.fill((vetoedLeptons[0]->p4() + vetoedLeptons[1]->p4()).mass(),weight,"diLepMass",";M_{ll}",40,0,400,prefix);
      eventPlots.fill((vetoedLeptons[0]->p4() + vetoedLeptons[1]->p4()).pt(),weight,"diLepPt",";ll p_{T}",40,0,400,prefix);
    }

    CylLorentzVectorF metplepton = met->p4();
    metplepton += vetoedLeptons[0]->p4();
    eventPlots.fill(metplepton.pt(),weight,"metplepton",";W p_{T}",40,0,400,prefix);


    double scTopPairPT  = 0;
    if(nBJets >= 2){
      CylLorentzVectorF metplepton = met->p4();
      CylLorentzVectorF metpleptonb = metplepton;
      if(PhysicsUtilities::absDeltaPhi( metplepton.phi(),bJets[0]->phi()) < PhysicsUtilities::absDeltaPhi( metplepton.phi(),bJets[1]->phi())  ){
        metpleptonb   +=  bJets[0]->p4();
        scTopPairPT = metpleptonb.pt() + bJets[1]->pt();
      } else {
        metpleptonb   +=  bJets[1]->p4();
        scTopPairPT = metpleptonb.pt() + bJets[0]->pt();
      }
      eventPlots.fill(scTopPairPT,weight,"scTopPairPT",";(W + b_jet) p_{T} + bJet  p_{T}",50,0,500,prefix);


    }

    if(isMC()){
      const GenParticleF * top1 = 0;
      const GenParticleF * top2 = 0;
      for(const auto * g : genParts){
        if(TMath::Abs(g->pdgId()) != 6) continue;
        if(top1) top2 = g;
        else top1 = g;
      }
      if(top1 && top2){
        eventPlots.fill(top1->pt() + top2->pt(),weight,"top1ptop2pt",";top 1 p_{T} + top 2 p_{T}",70,0,700,prefix);
        eventPlots.fill(TMath::Sqrt(top1->pt()*top2->pt()),weight,"top1geotop2pt",";Sqrt(top 1 p_{T} * top 2 p_{T})",40,0,400,prefix);
        eventPlots.fill(top1->pt() + top2->pt(),scTopPairPT,weight,"top1ptop2ptvscTopPairPT",";top 1 p_{T} + top 2 p_{T};W p_{T} + b_jet1 p_{T} + bJet2  p_{T}",70,0,700,60,0,600,prefix);
      }
    }







  }


  bool testSF;
  UnfStatus status;
  Type    type;
  QuickRefold::MakeRefold *       unf;
  TString outPath;
  PlotterD*                       plotter;
  Polybook                        eventPlots;

};




void run() {

  cfgSet::loadDefaultConfigurations();
  cfgSet::setJSONFile("/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_4_7/src/data/JSON/Cert_246908-251883_13TeV_PromptReco_Collisions15_JSON_v2.txt");
  cfgSet::ConfigSet cfg = cfgSet::ol_search_set;
  cfg.jets.minPt = 20;
  cfg.jets.minBJetPt = 20;
  cfg.jets.cleanJetsvVetoedLeptons = true;

//  TString MCFileName = "MC_extraskim.root";
  TString MCFileName = "MC_extraSkim.root";
  TString MCTreeName = "Events";


  TString DataMUFileName = "pieces/singlemu-2015b-pr_ntuple_lepSkim.root";
  TString DataMUTreeName = "Events";

  TString DataEFileName = "pieces/singleel-2015b-pr_ntuple_lepSkim.root";
  TString DataETreeName = "Events";




    //Run unfolding test
//    QuickRefold::MakeRefold * ttbarUnfold = new QuickRefold::MakeRefold (1,1,QuickRefold::SUBTRACT,1);
//    float genBins[] = {0,100,180,250,300,400,500};
//    float recoBins[] = {0,100,140,200,260,350,500};
//    ttbarUnfold->addMeasAxis(0,"scalarTopDecayPT",6,recoBins);
//    ttbarUnfold->addTruthAxis(0,"TopPairPT",6,genBins);
//    ttbarUnfold->stopSetup();
//
//    Analyzer  * a = new Analyzer(MCFileName,MCTreeName, "origMC_ttbarVal.root", MC,false,UNF,ttbarUnfold,&cfg);
//    a->analyze();
//    delete a;
//    a = new Analyzer(MCFileName,MCTreeName, "sfUpMC_ttbarVal.root",MC,true,UNF_DATA, ttbarUnfold, &cfg);
//    a->analyze();
//    delete a;
//    a = new Analyzer(DataEFileName,DataETreeName, "dataE_ttbarVal.root",EDATA,false,UNF_NONE, 0, &cfg);
//    a->analyze();
//    delete a;
//
//    ttbarUnfold->unfold(RooUnfold::kBayes,4,0,0);
//  //  ttbarUnfold->drawDiag();
//    ttbarUnfold->writeCorr("ttbarCorr_test.root","recreate","TOPPAIRPT",true);
//
//    cfg.corrections.ttbarCorrectionFile = "ttbarCorr_test.root";
//    cfg.corrections.ttbarCorrections = TtbarCorrectionSet::TOPPAIRPT;
//    a = new Analyzer(MCFileName,MCTreeName, "sfUpUnfMC_ttbarVal.root",MC,false, UNF_NONE,0, &cfg);
//    a->analyze();
//    delete a;


//  //Run unfolding test on data
  QuickRefold::MakeRefold * ttbarUnfold = new QuickRefold::MakeRefold (1,1,QuickRefold::SUBTRACT,1);
  float genBins[] = {0,100,180,250,300,400,500};
  float recoBins[] = {0,100,140,200,260,350,500};
  ttbarUnfold->addMeasAxis(0,"scalarTopDecayPT",6,recoBins);
  ttbarUnfold->addTruthAxis(0,"TopPairPT",6,genBins);
  ttbarUnfold->stopSetup();

  Analyzer  * a = new Analyzer(MCFileName,MCTreeName, "origMC_ttbarVal.root", MC,false,UNF,ttbarUnfold,&cfg);
  a->analyze();
  delete a;
  a = new Analyzer(DataMUFileName,DataMUTreeName, "dataMU_ttbarVal.root",MUDATA,false,UNF_DATA, ttbarUnfold, &cfg);
  a->analyze();
  delete a;
  a = new Analyzer(DataEFileName,DataETreeName, "dataE_ttbarVal.root",EDATA,false,UNF_NONE, 0, &cfg);
  a->analyze();
  delete a;

  ttbarUnfold->unfold(RooUnfold::kBayes,4,0,0);
//  ttbarUnfold->drawDiag();
  ttbarUnfold->writeCorr("ttbarCorr.root","recreate","TOPPAIRPT",true);

  cfg.corrections.ttbarCorrectionFile = "ttbarCorr.root";
  cfg.corrections.ttbarCorrections = TtbarCorrectionSet::TOPPAIRPT;
  a = new Analyzer(MCFileName,MCTreeName, "dataUnfMC_ttbarVal.root",MC,false, UNF_NONE,0, &cfg);
  a->analyze();
  delete a;


}


#endif

/*
 * process -> string from defaults::PROCESS_NAMES
 * crossSection -> processCrossSection
 * lumi -> luminosity that you wish to scale for
 * nEvents -> number of events in sample (if < 0 take it from the tree)
 *
 */

void TtbarCorrectionValidation() {
  run();

}


// Test macros
/*

{
    Plot::cache("*_ttbarVal.root");

    TString vars[] = {"njets","nbjets","ht","j0_pt","j1_pt","j2_pt","j3_pt","jgt3_pt","bj0_pt","bj1_pt","bjgt1_pt",""};
    // TString file = "(origMC_|data_).*__ll__";

    // TString file = "_(data|MC)__";
    // TString file = "(origMC_MC|origMC_Wlnu|origMC2_MC|origMC2_Wlnu|data_data)__";
        // TString file = "(origMC_MC|sfUpMC_MC|sfUpUnfMC_MC)__";

        TString file = "(origMC_MC|data_data|dataUnfMC_MC)__ll__";
        TString den = "data";
        // TString den = "sfUpMC_MC";
    o = new TObjArray;
    oo = new TObjArray;
    for(unsigned int iV = 0; vars[iV][0]; ++iV){
        Plot * p = new Plot(file  + vars[iV]);
        if(iV > 1) p->rebin(3);
        p->toUnderOverflow();
        o->Add(p);
        p = p->makeRatiogram(den,"s");
        p->setMinMax(0.,2.);
        oo->Add(p);
    }
    Pint::drawAll(o,"");
        Pint::drawAll(oo,"");

}


{
    Plot::cache("*_ttbarVal.root");

    TString vars[] = {"mt","met","lepton1","metplepton","scTopPairPT","top1ptop2pt","top1geotop2pt","recoil",""};
    // TString file = "(origMC_|data_).*__";
    // TString file = "_(data|MC)__";
    // TString file = "(origMC_MC|origMC_Wlnu|origMC2_MC|origMC2_Wlnu|data_data)__";
            TString file = "(origMC_MC|sfUpMC_MC|sfUpUnfMC_MC)__";
    // TString file = "(origMC_MC|data_data|dataUnfMC_MC)__mu__";



    // TString den = "data";
    TString den = "sfUpMC_MC";

    o = new TObjArray;
    oo = new TObjArray;

    for(unsigned int iV = 0; vars[iV][0]; ++iV){


        Plot * p = new Plot(file + vars[iV]+"$");

        // if(iV == 4){
        //     double recoBins[] = {0,100,140,200,260,350,500};
        //     RebinToNewBins b(6,recoBins);
        //     p->rebin(b);
        // }

        // p->rebin(4);
        p->toUnderOverflow();
        o->Add(p);
        // p->rebin(2);
        if(!p->at(den))continue;
        p = p->makeRatiogram(den,"s");
        p->setMinMax(0.,2.);
        oo->Add(p);

    }
    Pint::drawAll(o,"");
    Pint::drawAll(oo,"");


}

{
    double genBins[] = {0,100,180,250,300,400,500};
    RebinToNewBins x(Axis::X,6,genBins);
    double recoBins[] = {0,100,140,200,260,350,500};
    RebinToNewBins y(Axis::Y,6,recoBins);
    TH2 * h = ttbar__mu__top1ptop2ptvscTopPairPT;
        // TH2 * h = ttbar__vectortopptvrecoil;
    // TH1 * h = data__scTopPairPT;
    Procedures::rebin(h,x);
    Procedures::rebin(h,y);
        Procedures::toUnderOverflow(h);
    h->Draw("COLZ");
        // h->Draw();

}

{
    // setTDRStyle();

    Plot::cache("*_ttbarVal.root");

    // TString lep[] = {"mu","","e","ll",""};
    TString lep[] = {"ll","","ll",""};

    // TString vars[] = {"njets","nbjets","ht","j0_pt","j1_pt","","j2_pt","j3_pt","jgt3_pt","bj0_pt","bj1_pt","bjgt1_pt","diLepPt","diLepMass",""};
    // TString vars[] = {"bj0_pt","met","lepton1","metplepton","scTopPairPT",""};
        TString vars[] = {"njets","nbjets","ht","j0_pt","j1_pt","j2_pt","j3_pt","jgt3_pt","bj0_pt","bj1_pt","bjgt1_pt","diLepPt","diLepMass",""};

    // TString vars[] = {"diLepPt","diLepMass",""};

    TString MCSel = "origMC_(ttbar|Wlnu|Zll|other)__";


    for(unsigned int iL = 0; lep[iL][0]; ++iL){
        o = new TObjArray;
        for(unsigned int iV = 0; vars[iV][0]; ++iV){
            Plot * p = new Plot(MCSel +lep[iL] +"__" + vars[iV]);
            TH1 * d = Plot::getCache()->findOne("data_data__"+lep[iL] +"__" + vars[iV]);
            TH1 * u = Plot::getCache()->findOne("dataUnfMC_MC__"+lep[iL] +"__" + vars[iV]);
            if(d == 0)continue;
            MakeStack * hs = new MakeStack;
                        if(p->at("other"))hs->addHist(p->at("other"),"other");
                                    if(p->at("Zll")  )hs->addHist(p->at("Zll"),"Zll");
                                                if(p->at("Wlnu") )hs->addHist(p->at("Wlnu"),"Wlnu");
            if(p->at("ttbar") )hs->addHist(p->at("ttbar"),"ttbar");





                    hs->addOverlay(d,"data");
                                hs->addOverlay(u,"corr MC",kRed);
            hs->setTitle(lep[iL]);
            double bins[] =  {0,100,200,300,400};
if(iV > 1)
    hs->rebin(4);
            hs->toUnderOverflow();
            TCanvas * c = hs->getRatio(0,"");
            o->Add(c);
        }
        // Pint::drawAll(o,"","");
    }
}
 */


