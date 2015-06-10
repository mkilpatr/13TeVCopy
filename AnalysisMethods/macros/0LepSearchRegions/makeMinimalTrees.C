#if !defined(__CINT__) || defined(__MAKECINT__)
#include "TSystem.h"
#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"
#include "AnalysisTools/KinematicVariables/interface/Mt2Helper.h"
#include "AnalysisTools/KinematicVariables/interface/CreatePseudoJets.h"
#endif

using namespace ucsbsusy;

class Analyzer : public TreeCopierManualBranches {

  public :

    Analyzer(TString fileName, TString treeName, TString outfileName, bool isMCTree, ConfigPars * pars) :
      TreeCopierManualBranches(fileName, treeName, outfileName, isMCTree, pars),
      pseudoJets(new CreatePseudoJets()),
      mt2Calc   (new Mt2Helper())
    {
      jettypes = {"ak4","picky","subjetsca"};
    }

    CreatePseudoJets* pseudoJets;
    Mt2Helper*        mt2Calc;

    vector<TString>   jettypes;

    const double  metcut_    = 200.0 ;

    virtual ~Analyzer() {}

    bool fillEvent();

    void fillJets(JetReader* reader, TString jettype);

    void book();

    size i_run       ;
    size i_lumi      ;
    size i_event     ;
    size i_weight    ;
    size i_met       ;
    size i_npv       ;
    size i_ncttstd   ;
    size i_nctt      ;
    size i_dphicttmet;

    map<TString, size> i_njets    ;
    map<TString, size> i_njets60  ;
    map<TString, size> i_nmbtags  ;
    map<TString, size> i_ntbtags  ;
    map<TString, size> i_ht       ;
    map<TString, size> i_mt2      ;
    map<TString, size> i_j1pt     ;
    map<TString, size> i_j1eta    ;
    map<TString, size> i_j2pt     ;
    map<TString, size> i_j2eta    ;
    map<TString, size> i_j3pt     ;
    map<TString, size> i_j3eta    ;
    map<TString, size> i_b1pt     ;
    map<TString, size> i_b1eta    ;
    map<TString, size> i_b2pt     ;
    map<TString, size> i_b2eta    ;
    map<TString, size> i_dphij1met;
    map<TString, size> i_dphij2met;
    map<TString, size> i_dphij3met;
    map<TString, size> i_dphib1met;
    map<TString, size> i_dphib2met;
    map<TString, size> i_mtb1met  ;
    map<TString, size> i_mtb2met  ;

};

void Analyzer::book()
{

  i_run        = data.add<unsigned int>("","run","i",0);
  i_lumi       = data.add<unsigned int>("","lumi","i",0);
  i_event      = data.add<unsigned int>("","event","i",0);
  i_weight     = data.add<float>("","weight","F",0);
  i_met        = data.add<float>("","met","F",0);
  i_npv        = data.add<int>("","npv","I",0);
  i_ncttstd    = data.add<int>("","ncttstd","I",0);
  i_nctt       = data.add<int>("","nctt","I",0);
  i_dphicttmet = data.addMulti<float>("","dphicttmet",0);

  for(auto jettype : jettypes) {
    string prefix = string(jettype.Data());
    i_njets    [jettype] = data.add<int>(prefix,"njets","I",0);
    i_njets60  [jettype] = data.add<int>(prefix,"njets60","I",0);
    i_nmbtags  [jettype] = data.add<int>(prefix,"nmbtags","I",0);
    i_ntbtags  [jettype] = data.add<int>(prefix,"ntbtags","I",0);
    i_ht       [jettype] = data.add<float>(prefix,"ht","F",0);
    i_mt2      [jettype] = data.add<float>(prefix,"mt2","F",0);
    i_j1pt     [jettype] = data.add<float>(prefix,"j1pt","F",0);
    i_j1eta    [jettype] = data.add<float>(prefix,"j1eta","F",0);
    i_j2pt     [jettype] = data.add<float>(prefix,"j2pt","F",0);
    i_j2eta    [jettype] = data.add<float>(prefix,"j2eta","F",0);
    i_j3pt     [jettype] = data.add<float>(prefix,"j3pt","F",0);
    i_j3eta    [jettype] = data.add<float>(prefix,"j3eta","F",0);
    i_b1pt     [jettype] = data.add<float>(prefix,"b1pt","F",0);
    i_b1eta    [jettype] = data.add<float>(prefix,"b1eta","F",0);
    i_b2pt     [jettype] = data.add<float>(prefix,"b2pt","F",0);
    i_b2eta    [jettype] = data.add<float>(prefix,"b2eta","F",0);
    i_dphij1met[jettype] = data.add<float>(prefix,"dphij1met","F",0);
    i_dphij2met[jettype] = data.add<float>(prefix,"dphij2met","F",0);
    i_dphij3met[jettype] = data.add<float>(prefix,"dphij3met","F",0);
    i_dphib1met[jettype] = data.add<float>(prefix,"dphib1met","F",0);
    i_dphib2met[jettype] = data.add<float>(prefix,"dphib2met","F",0);
    i_mtb1met  [jettype] = data.add<float>(prefix,"mtb1met","F",0);
    i_mtb2met  [jettype] = data.add<float>(prefix,"mtb2met","F",0);
  }

}

bool Analyzer::fillEvent()
{

  if(nVetoedLeptons > 0)  return false;
  if(nVetoedTaus > 0)     return false;
  if(met->pt() < metcut_) return false;

  data.fill<unsigned int>(i_run, run);
  data.fill<unsigned int>(i_lumi, lumi);
  data.fill<unsigned int>(i_event, event);
  data.fill<float>(i_weight, weight);
  data.fill<float>(i_met, met->pt());
  data.fill<int  >(i_npv, nPV);
  data.fill<int  >(i_nctt, cttTops.size());
  int ncttstd = 0;
  for(auto* ctt : cttTops) {
    if(ctt->fJMass() > 140.0 && ctt->fJMass() < 250.0 && ctt->minMass() > 50.0 && ctt->nSubJets() >= 3) ncttstd++;
    data.fillMulti<float>(i_dphicttmet, fabs(PhysicsUtilities::deltaPhi(*ctt, *met)));
  }
  data.fill<int  >(i_ncttstd, ncttstd);

  JetReader* jetreader = 0;
  for(auto jettype : jettypes) {
    if(jettype == "ak4") jetreader = &ak4Reader;
    else if(jettype == "picky") jetreader = &pickyJetReader;
    else if(jettype == "subjetsca") jetreader = &caSubJetReader;
    else printf("Jet type %s is unknown\n", jettype.Data());
    fillJets(jetreader, jettype);
  }

  return true;

}

void Analyzer::fillJets(JetReader* reader, TString jettype)
{

  std::vector<RecoJetF*> cleanjets, cleanbjets, cleannonbjets;

  cleanJets(reader, cleanjets, &cleanbjets, &cleannonbjets);

  int njets60 = 0;
  for(auto* j : cleanjets) {
    if(j->pt() > 60.0) njets60++;
  }

  int ntbtags = 0;
  for(auto* b : cleanbjets) {
    if(b->csv() > defaults::CSV_TIGHT) ntbtags++;
  }

  data.fill<int>(i_njets[jettype], int(cleanjets.size()));
  data.fill<int>(i_njets60[jettype], njets60);
  data.fill<int>(i_nmbtags[jettype], int(cleanbjets.size()));
  data.fill<int>(i_ntbtags[jettype], ntbtags);
  data.fill<float>(i_ht[jettype], JetKinematics::ht(cleanjets, 20.0, 2.4));

  MomentumF pseudoJet1; MomentumF pseudoJet2;
  pseudoJets->makePseudoJets(cleanjets, pseudoJet1, pseudoJet2, 0);
  double mt2 = mt2Calc->CalcMT2(&pseudoJet1, &pseudoJet2, met, 0);
  data.fill<float>(i_mt2[jettype], mt2);

  if(cleanjets.size() > 0) {
    data.fill<float>(i_j1pt[jettype], cleanjets[0]->pt());
    data.fill<float>(i_j1eta[jettype], cleanjets[0]->eta());
    data.fill<float>(i_dphij1met[jettype], fabs(PhysicsUtilities::deltaPhi(*cleanjets[0], *met)));
  }
  if(cleanjets.size() > 1) {
    data.fill<float>(i_j2pt[jettype], cleanjets[1]->pt());
    data.fill<float>(i_j2eta[jettype], cleanjets[1]->eta());
    data.fill<float>(i_dphij2met[jettype], fabs(PhysicsUtilities::deltaPhi(*cleanjets[1], *met)));
  }
  if(cleanjets.size() > 2) {
    data.fill<float>(i_j3pt[jettype], cleanjets[2]->pt());
    data.fill<float>(i_j3eta[jettype], cleanjets[2]->eta());
    data.fill<float>(i_dphij3met[jettype], fabs(PhysicsUtilities::deltaPhi(*cleanjets[2], *met)));
  }
  if(cleanbjets.size() > 0) {
    data.fill<float>(i_b1pt[jettype], cleanbjets[0]->pt());
    data.fill<float>(i_b1eta[jettype], cleanbjets[0]->eta());
    data.fill<float>(i_dphib1met[jettype], fabs(PhysicsUtilities::deltaPhi(*cleanbjets[0], *met)));
    data.fill<float>(i_mtb1met[jettype], JetKinematics::transverseMass(*cleanbjets[0], *met));
  }
  if(cleanbjets.size() > 1) {
    data.fill<float>(i_b2pt[jettype], cleanbjets[1]->pt());
    data.fill<float>(i_b2eta[jettype], cleanbjets[1]->eta());
    data.fill<float>(i_dphib2met[jettype], fabs(PhysicsUtilities::deltaPhi(*cleanbjets[1], *met)));
    data.fill<float>(i_mtb2met[jettype], JetKinematics::transverseMass(*cleanbjets[1], *met));
  }

}

void makeMinimalTrees(TString sname = "T2tt_850_100",
                      const int fileindex = -1,
                      const bool isMC = true,
                      const TString fname = "/store/user/vdutta/13TeV/080615/merged/T2tt_850_100_ntuple_wgtxsec.root",
                      const double xsec = 1.0,
                      const TString outputdir = "trees",
                      const TString fileprefix = "root://eoscms//eos/cms")
{

  printf("Processing file %d of %s sample\n", (fileindex > -1 ? fileindex : 0), sname.Data());

  if(fileindex > -1)
    sname += TString::Format("_%d",fileindex);

  if(isMC)
    printf("Cross section: %5.2f pb\n", xsec);

  TString fullname = fileprefix+fname;

  gSystem->mkdir(outputdir,true);
  TString outfilename = outputdir+"/"+sname+"_tree.root";

  // Adjustments to default configuration
  BaseTreeAnalyzer::ConfigPars pars;
  pars.defaultJetCollection = BaseTreeAnalyzer::AK4JETS;
  pars.minJetPt = 20;
  pars.vetoedMuon = &MuonF::ismultiisovetomuonl;
  pars.vetoedElectron = &ElectronF::ismultiisovetoelectronl;

  Analyzer a(fullname, "Events", outfilename, isMC, &pars);

  a.analyze(10000);

}
