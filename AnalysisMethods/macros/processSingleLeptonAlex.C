#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TSystem.h>
#include <TFile.h>
#include <TH1F.h>
#include "AnalysisMethods/PlotUtils/interface/Sample.hh"
#include "AnalysisMethods/PlotUtils/interface/PlotTools.hh"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisTools/KinematicVariables/interface/Topness.h"
#include "AnalysisTools/TreeReader/interface/ElectronReader.h"

// mt2w, mt2bl
#include <vector>
#include "Math/VectorUtil.h"
#include "AnalysisTools/KinematicVariables/interface/mt2w.h"
#include "AnalysisTools/KinematicVariables/interface/mt2bl.h"
#include "AnalysisTools/TreeReader/interface/Defaults.h" // CSV_MEDIUM
#endif

using namespace ucsbsusy;

class Analyzer : public BaseTreeAnalyzer {

  public :
    void rankedByCSV(vector<RecoJetF*>& inJets,vector<RecoJetF*>& outJets);

  Analyzer(TString fileName, TString treeName, bool isMCTree, ConfigPars * pars, double xSec, TString sname, TString outputdir) :
    BaseTreeAnalyzer(fileName, treeName, isMCTree, pars),xsec_(xSec), sname_(sname), outputdir_(outputdir) {
      // configuration
      tNess     = new Topness();
      tNessInfo = new TopnessInformation();

      // initialize plots
      loadPlots();

      // initiliaze tree
      gSystem->mkdir(outputdir,true);
      fout = new TFile (outputdir+"/"+sname+"_tree.root","RECREATE");
      fout->cd();
      outtree = new TTree("events","analysis tree");
      outtree->Branch("ScaleFactor",&ScaleFactor,"ScaleFactor/F");
      outtree->Branch("NPV",&NPV,"NPV/I");
      outtree->Branch("NJets",&NJets,"NJets/I");
      outtree->Branch("NBJets",&NBJets,"NBJets/I");
      outtree->Branch("NRecoLep",&NRecoLep,"NRecoLep/I");
      outtree->Branch("NVetoTau",&NVetoTau,"NVetoTau/I");
      outtree->Branch("MET",&MET,"MET/F");
      outtree->Branch("HT",&HT,"HT/F");
      outtree->Branch("HTAovA",&HTAovA,"HTAovA/F");
      outtree->Branch("DphiLepW",&DphiLepW,"DphiLepW/F");
      outtree->Branch("DphiLepWRecoSeen",&DphiLepWRecoSeen,"DphiLepWRecoSeen/F");
      outtree->Branch("DphiLepWRecoMiss",&DphiLepWRecoMiss,"DphiLepWRecoMiss/F");
      outtree->Branch("minTopness",&minTopness,"minTopness/F");
      outtree->Branch("minTopnessNoLog",&minTopnessNoLog,"minTopnessNoLog/F");
      outtree->Branch("stdIso",&stdIso,"stdIso/F");
      outtree->Branch("LepPdgId",&LepPdgId,"LepPdgId/I");
      outtree->Branch("LepPt",&LepPt,"LepPt/F");
      outtree->Branch("MT",&mtlepmet,"mtlepmet/F");

      outtree->Branch("mt2w_add1b",&mt2w_add1b,"mt2w_add1b/D");
      outtree->Branch("mt2w_add2b",&mt2w_add2b,"mt2w_add2b/D");
      outtree->Branch("mt2w_add1j",&mt2w_add1j,"mt2w_add1j/D");
      outtree->Branch("mt2w_add2j",&mt2w_add2j,"mt2w_add2j/D");

      outtree->Branch("top_add1b",&top_add1b,"top_add1b/D");
      outtree->Branch("top_add2b",&top_add2b,"top_add2b/D");
      outtree->Branch("top_add1j",&top_add1j,"top_add1j/D");
      outtree->Branch("top_add2j",&top_add2j,"top_add2j/D");
  }

  virtual ~Analyzer() {
    fout->cd();
    outtree->Write();
    fout->Close();
  }

  const double xsec_;
  const double lumi_         = 1000.0; // in /pb
  const double metcut_       = 80.0;
  const double mtcut_        = 100.;
  //const double dphicut_      = 1.0;
  const int    minNJets_     = 4;
  const int    minNBjets_    = 1;
  const int    maxNTaus_     = 0; // unused
  const int    nLeptons_     = 1;
  const float  minTrigMuPt_  = 30.0; // AN values
  const float  maxTrigMuEta_ = 2.1;
  const float  minTrigEPt_   = 40.0;
  const float  maxTrigEEta_  = 2.1;
  const TString sname_       = "Loukastest.root";
  const TString outputdir_   = "./plots/";

  // start topness/mt2x classes
  Topness *tNess;
  TopnessInformation *tNessInfo;

  mt2w_bisect::mt2w mt2w_event;
  //mt2bl_bisect::mt2bl mt2bl_event;

  TFile *fout;
  TTree *outtree;

  float ScaleFactor;
  int NPV;
  int NJets;
  int NBJets;
  int NRecoLep;
  int NVetoTau;
  float MET;
  float HT;
  float HTAovA;
  float DphiLepW;
  float DphiLepWRecoSeen;
  float DphiLepWRecoMiss;
  float minTopness;
  float minTopnessNoLog;
  float stdIso;
  float mtlepmet;
  int LepPdgId;
  float LepPt;
  float MT;

  double mt2w_add1b, mt2w_add2b, mt2w_add1j, mt2w_add2j;
  double top_add1b, top_add2b, top_add1j, top_add2j;

  void loadVariables();
  void runEvent();
  void loadPlots();

  void out(TString outputName, TString outputPath);

  map<TString,TH1F*> plots;

};

void Analyzer::loadVariables() {
  load(EVTINFO);
  load(AK4JETS);
  load(PICKYJETS);
  load(CASUBJETS);
  load(ELECTRONS);
  load(MUONS);
  load(PHOTONS);
  load(PFCANDS);
  load(CMSTOPS);
  if(isMC()) load(GENPARTICLES);
}

// Define plots, names, and binning
void Analyzer::loadPlots()
{

  TString txt_passjets = "N_{J} >= 4, N_{BJ} >=1";
  TString txt_passmet = "pass met";
  TString txt_passpresel = "pass preselection";
  TString txt_ytitle = "Events";
  // TString txt_passsel = "presel && |#Delta#phi(l,#slash{E}_{T})|>1";
  // TString txt_passtopsel = "presel and minTopness>7";
  // TString txt_passall = txt_passsel + " && minTopness>7";

  // N-1 plots
  plots["met_passjets"]          = new TH1F("met_passjets", TString(txt_passjets+"; #slash{E}_{T} [GeV]; "+txt_ytitle).Data(), 50, 0, 500);
  plots["njets_passmet"]         = new TH1F("njets_passmet", TString(txt_passmet+"; Number of Jets; "+txt_ytitle).Data(), 10, -0.5, 9.5);
  plots["nbjets_passmet"]        = new TH1F("nbjets_passmet", TString(txt_passmet+"; Number of B-tagged Jets; "+txt_ytitle).Data(), 8, -0.5, 7.5);

  // plots after preselection
  plots["ht_passpresel"]         = new TH1F("ht_passpresel", TString(txt_passpresel+"; H_{T} [GeV]; "+txt_ytitle).Data(), 60, 0, 1500);
  plots["leppt_passpresel"]      = new TH1F("leppt_passpresel", TString(txt_passpresel+"; Lepton p_{T} [GeV]; "+txt_ytitle).Data(), 100, 0, 500);
  plots["jet1pt_passpresel"]     = new TH1F("jet1pt_passpresel", TString(txt_passpresel+"; Leading jet p_{T} [GeV]; "+txt_ytitle).Data(), 120, 0, 600);
  plots["dphilepmet_passpresel"] = new TH1F("dphilepmet_passpresel", TString(txt_passpresel+"; |#Delta#phi(l, #slash{E}_{T})| [GeV]; "+txt_ytitle).Data(), 50, 0, 3.15);
  plots["dphilepw_passpresel"]   = new TH1F("dphilepw_passpresel", TString(txt_passpresel+"; |#Delta#phi(l, W)| [GeV]; "+txt_ytitle).Data(), 50, 0, 3.15);
  plots["mtlepmet_passpresel"]   = new TH1F("mtlepmet_passpresel", TString(txt_passpresel+"; m_{T}(l, #slash{E}_{T}) [GeV]; "+txt_ytitle).Data(), 40, 0, 200);

  plots["mt2w_add1b"] = new TH1F("mt2w_add1b", TString(txt_passpresel+"; mt2w_add1b [GeV]; "+txt_ytitle).Data(), 40, 0, 1000);
  plots["mt2w_add2b"] = new TH1F("mt2w_add2b", TString(txt_passpresel+"; mt2w_add2b [GeV]; "+txt_ytitle).Data(), 40, 0, 1000);
  plots["mt2w_add1j"] = new TH1F("mt2w_add1j", TString(txt_passpresel+"; mt2w_add1j [GeV]; "+txt_ytitle).Data(), 40, 0, 1000);
  plots["mt2w_add2j"] = new TH1F("mt2w_add2j", TString(txt_passpresel+"; mt2w_add2j [GeV]; "+txt_ytitle).Data(), 40, 0, 1000);

  plots["top_add1b"]  = new TH1F("top_add1b", TString(txt_passpresel+"; top_add1b; "+txt_ytitle).Data(),60,-10,20);
  plots["top_add2b"]  = new TH1F("top_add2b", TString(txt_passpresel+"; top_add2b; "+txt_ytitle).Data(),60,-10,20);
  plots["top_add1j"]  = new TH1F("top_add1j", TString(txt_passpresel+"; top_add1j; "+txt_ytitle).Data(),60,-10,20);
  plots["top_add2j"]  = new TH1F("top_add2j", TString(txt_passpresel+"; top_add2j; "+txt_ytitle).Data(),60,-10,20);

  // set Sumw2 for histograms
  for(map<TString,TH1F*>::iterator plotsIt = plots.begin(); plotsIt != plots.end(); ++plotsIt) {
    plotsIt->second->Sumw2();
  }

}

// Analyze event and fill plots
void Analyzer::runEvent()
{
  // old wgt calculation
  // double wgt = lumi_*xsec_/getEntries(); // old way

  // Loukas' fix for weights
  float wgt = evtInfoReader.weight;
  ScaleFactor = wgt;

  if(nSelLeptons < nLeptons_) return; // check for at least one lepton passing pog definitions

  // check the leptons' pt and fill trigLeptons with leptons passing the trigger
  vector<LeptonF*> trigLeptons;
  for(auto* lepton : selectedLeptons) {
    if(lepton->ismuon() && lepton->pt() > minTrigMuPt_ && fabs(lepton->eta()) < maxTrigMuEta_) trigLeptons.push_back(lepton);
    else if(lepton->iselectron() && lepton->pt() > minTrigEPt_ && fabs(lepton->eta()) < maxTrigEEta_) trigLeptons.push_back(lepton);
  }

  if(trigLeptons.size() != 1) return; // require exactly one good lepton in trigger acceptance

  if(nSelLeptons > nLeptons_) return; // veto additional leptons passing pog definitions (N_POG = 1)

  //if(nVetoedTaus > maxNTaus_)     return;

  // fill tree variables
  ScaleFactor = wgt; // 1/fb new ntuples
  NPV = nPV;
  NJets = nJets;
  NBJets = nBJets;
  NRecoLep = nSelLeptons;
  NVetoTau = nVetoedTaus;
  MET = met->pt();
  HT = JetKinematics::ht(jets,40,2.4);
  HTAovA = JetKinematics::htAlongHtAway(*met,jets,40,2.4);

  // collect the single triggered & selected lepton
  LeptonF* lep = trigLeptons.at(0);

  // reconstruct a W from lep+met
  MomentumF* W = new MomentumF(lep->p4() + met->p4());

  // calculate dphi between lep and W (also goes to tree)
  DphiLepW = PhysicsUtilities::deltaPhi(*lep, *W);

  // beta value of lep (why useful?)
  stdIso = (lep->pfdbetaiso())/(lep->pt());

  // lep met mt
  mtlepmet = JetKinematics::transverseMass(*lep, *met);

  // topness
  minTopness = tNess->findMinTopnessConfiguration(selectedLeptons,jets,met,tNessInfo);
  minTopnessNoLog = std::exp(minTopness);

  // calculate dphi between the single lepton, and the two W's (one reconstructed above, one from topness)
  DphiLepWRecoSeen = PhysicsUtilities::deltaPhi(tNessInfo->top1_l,tNessInfo->top1_w);
  DphiLepWRecoMiss = PhysicsUtilities::deltaPhi(tNessInfo->top1_l,tNessInfo->top2_w);
  // access correct b jet here -- no modification of bjet

  // lepton properties
  LepPdgId = lep->pdgid();
  LepPt    = lep->pt();

  // preselection on jets, met, dphi
  bool passjets = nJets >= minNJets_ && nBJets >= minNBjets_;
  //bool passdphi = fabs(DphiLepW) > dphicut_;
  //bool passpreselection = passmet && passjets && passdphi;
  bool passmt = mtlepmet > mtcut_;
  bool passmet = met->pt() > metcut_;
  bool passpreselection = passjets && passmt && passmet;

  // Fill histograms before (passpreselection==true) preselection

  // N-1 plots
  if(passjets) plots["met_passjets"]  ->Fill(met->pt(), wgt);
  if(passmet)  plots["njets_passmet"] ->Fill(nJets, wgt);
  if(passmet)  plots["nbjets_passmet"]->Fill(nBJets, wgt);

  if(!passpreselection) return;

// PROCEDURE
// jetsCSV = jets sorted by CSV
// fjets = jets converted to MomentumF
// bjets = vector of indices of btagged jets into fjets/jets
// nonbjets = same, but untagged
// btags = vector of bool btags of fjets/jets

  // sort the jets in decreasing csv vector<RecoJetF*> jetsCSV;
  vector<RecoJetF*> jetsCSV;
  rankedByCSV(jets,jetsCSV);

  // hardcode into MomentumF form
  MomentumF* jetcsv1F = new MomentumF(jetsCSV[0]->p4()); // won't crash, as size is size of jets, which is >=4
  MomentumF* jetcsv2F = new MomentumF(jetsCSV[1]->p4());
  MomentumF* jetcsv3F = new MomentumF(jetsCSV[2]->p4());
  //MomentumF* jetcsv4F = new MomentumF(jetsCSV[3]->p4());

  // make vector<bool> of medium btags, and
  // convert jets objects from RecoJetF or GenJetF to MomentumF
  vector<bool> btags;
  vector<MomentumF*> fjets;
  MomentumF* tempjet;
  for(auto* jet : jets) {
    btags.push_back(jet->csv() > defaults::CSV_MEDIUM); // 0.814 currently
    tempjet = new MomentumF(jet->p4());
    fjets.push_back(tempjet);
  }

  // use btags to get indices of bjets into the jets vector
  // access like fjets[bjets[0]] for momentumf of hardest bjet
  std::vector<int> bjets;
  std::vector<int> non_bjets;
  for(unsigned int i = 0; i < jets.size(); i++ ) {
      if( btags.at(i) ) {
          bjets.push_back(i);
      } else {
          non_bjets.push_back(i);
      }
  }

  // give lepton a MomentumF version
  MomentumF* lepF = new MomentumF(lep->p4());

  // getMinTopness already orders b1, b2 to minimise topness
  // double Topness::getMinTopness(const MomentumF *lep_, const MomentumF *bjet1_, const MomentumF *bjet2_, const MomentumF *met_,TopnessInformation * info)

  double top_temp, mt2w_temp;

  // If events have >= 2 b-jets, use only those jets
  if (bjets.size() >= 2) {
    double top_add1b = 20.;
    double mt2w_add1b = 1000.;
    for(unsigned int i = 0; i < bjets.size(); i++) {
      for(unsigned int j = 0; j < bjets.size(); j++) {
        if (i == j) continue; // oops, same jet
        top_temp  = tNess->getMinTopness(lepF, fjets[bjets[i]], fjets[bjets[j]], met, tNessInfo);
        mt2w_temp = mt2w_event.mt2wOrder(fjets[bjets[i]], fjets[bjets[j]], lep, met);
        if (top_temp < top_add1b) top_add1b = top_temp;
        if (mt2w_temp < mt2w_add1b) mt2w_add1b = mt2w_temp;
      }
    }
    top_add2b = top_add1b;
    top_add1j = top_add1b;
    top_add2j = top_add1b;
    mt2w_add2b = mt2w_add1b;
    mt2w_add1j = mt2w_add1b;
    mt2w_add2j = mt2w_add1b;
  }
  // if one bjet, combine it with many options for second jet
  else if (bjets.size() == 1) {
    if (jetcsv1F != fjets[bjets[0]]) {
      cout << "highest csv jet isn't lone btagged jet" << endl;
    }
    // add1b, 1 jet with (next) highest b-tag discriminant
    mt2w_add1b = mt2w_event.mt2wOrder(fjets[bjets[0]], jetcsv2F, lep, met);
    top_add1b = tNess->getMinTopness(lepF, fjets[bjets[0]], jetcsv2F, met, tNessInfo);

    // add2b, 2 jets with (next) highest b-tag discriminants
    mt2w_temp = mt2w_event.mt2wOrder(fjets[bjets[0]], jetcsv2F, lep, met);
    mt2w_add2b = min(mt2w_temp, mt2w_event.mt2wOrder(fjets[bjets[0]], jetcsv3F, lep, met));
    top_temp = tNess->getMinTopness(lepF, fjets[bjets[0]], jetcsv2F, met, tNessInfo);
    top_add2b = min(top_temp, tNess->getMinTopness(lepF, fjets[bjets[0]], jetcsv3F, met, tNessInfo));

    // add1j, 1 jet with next highest pt
    for(unsigned int i=0; i<fjets.size(); i++) { // cycle from high to low pt
      if (fjets[i] != fjets[bjets[0]]) { // highest pt jet which isn't the bjet
        mt2w_add1j = mt2w_event.mt2wOrder(fjets[bjets[0]], fjets[i], lep, met);
        top_add1j = tNess->getMinTopness(lepF, fjets[bjets[0]], fjets[i], met, tNessInfo);
      }
    }

    // add2j, 2 jets with next highest pt
    for(unsigned int i=0; i<fjets.size(); i++) { // cycle from high to low pt
      if (fjets[i] != fjets[bjets[0]]) { // highest pt jet which isn't the bjet (stops at i==0 or 1)
        mt2w_temp = mt2w_event.mt2wOrder(fjets[bjets[0]], fjets[i], lep, met);
        top_temp = tNess->getMinTopness(lepF, fjets[bjets[0]], fjets[i], met, tNessInfo);
        if (fjets[i+1] != fjets[bjets[0]]) { // next highest ...
          mt2w_add2j = min(mt2w_temp, mt2w_event.mt2wOrder(fjets[bjets[0]], fjets[i+1], lep, met));
          top_add2j = min(top_temp, tNess->getMinTopness(lepF, fjets[bjets[0]], fjets[i+1], met, tNessInfo));
        }
        else { // i+1 was the bjet, do i+2
          mt2w_add2j = min(mt2w_temp, mt2w_event.mt2wOrder(fjets[bjets[0]], fjets[i+2], lep, met));
          top_add2j = min(top_temp, tNess->getMinTopness(lepF, fjets[bjets[0]], fjets[i+2], met, tNessInfo));
        }
      }
    }

  }
  else if (bjets.size() == 0){
    cout << "no bjets, never reaches this point" << endl;
  }


  // plots after preselection
  plots["ht_passpresel"]        ->Fill(JetKinematics::ht(jets), wgt);
  plots["leppt_passpresel"]     ->Fill(lep->pt(), wgt);
  plots["jet1pt_passpresel"]    ->Fill(jets.at(0)->pt(), wgt);
  plots["dphilepmet_passpresel"]->Fill(fabs(PhysicsUtilities::deltaPhi(*lep, *met)), wgt);
  plots["dphilepw_passpresel"]  ->Fill(fabs(PhysicsUtilities::deltaPhi(*lep, *W)), wgt);
  plots["mtlepmet_passpresel"]  ->Fill(JetKinematics::transverseMass(*lep, *met), wgt);

  plots["mt2w_add1b"]           ->Fill(mt2w_add1b);
  plots["mt2w_add2b"]           ->Fill(mt2w_add2b);
  plots["mt2w_add1j"]           ->Fill(mt2w_add1j);
  plots["mt2w_add2j"]           ->Fill(mt2w_add2j);

  plots["top_add1b"]           ->Fill(top_add1b);
  plots["top_add2b"]           ->Fill(top_add2b);
  plots["top_add1j"]           ->Fill(top_add1j);
  plots["top_add2j"]           ->Fill(top_add2j);

  // tree after preselection
  outtree->Fill();
}

// Write histograms to file in designated directory
void Analyzer::out(TString outputName, TString outputPath)
{

  gSystem->mkdir(outputPath,true);
  TString filename = outputPath + "/" + outputName + "_plots.root";
  TFile *outfile = new TFile(filename, "RECREATE");
  outfile->cd();

  for(map<TString,TH1F*>::iterator plotsIt = plots.begin(); plotsIt != plots.end(); ++plotsIt) {
    plotsIt->second->Write();
  }

  outfile->Write();
  outfile->Close();
  delete outfile;
}

// sort jets by csv
void Analyzer::rankedByCSV(vector<RecoJetF*>& inJets,vector<RecoJetF*>& outJets) {

  outJets.clear();
  outJets.resize(inJets.size());

  vector<pair<double,int> > rankedJets(inJets.size());

  for(unsigned int iJ =0; iJ < inJets.size(); ++iJ){
    rankedJets[iJ].first = inJets[iJ]->csv();
    rankedJets[iJ].second = iJ;
  }

std::sort(rankedJets.begin(),rankedJets.end(),PhysicsUtilities::greaterFirst<double,int>());
  for(unsigned int iJ =0; iJ < inJets.size(); ++iJ){ outJets[iJ] = inJets[rankedJets[iJ].second]; }

}

// Process file belonging to specified sample with a given cross section
void processSingleLeptonAlex(TString sname = "test",               // sample name
                         const int fileindex = -1,             // index of file (-1 means there is only 1 file for this sample)
                         const bool isMC = true,               // data or MC
                         const TString fname = "evttree_numEvent1000.root", // path of file to be processed
                         const double xsec = 1.0,              // cross section to be used with this file
                         const TString outputdir = "run/plots",    // directory to which files with histograms will be written
                         const TString fileprefix = "file:$CMSSW_BASE/src/AnalysisBase/Analyzer/test/") // prefix for file name, needed e.g. to access files with xrootd
{

  printf("Processing file %d of %s sample\n", (fileindex > -1 ? fileindex : 0), sname.Data());

  // Make sure the output has a unique name in case there are multiple files to process
  if(fileindex > -1)
    sname += TString::Format("_%d",fileindex);

  if(isMC)
    printf("Cross section: %5.2f pb\n", xsec);

  TString fullname = fileprefix+fname;

  // Adjustments to default configuration
  BaseTreeAnalyzer::ConfigPars pars;
  //pars.defaultJetCollection = BaseTreeAnalyzer::PICKYJETS;
  pars.defaultJetCollection = BaseTreeAnalyzer::AK4JETS;
  pars.cleanJetsvSelectedLeptons_ = true;

  // Declare analyzer
  Analyzer a(fullname, "Events", isMC, &pars, xsec, sname, outputdir);

  // Run! Argument is frequency of printout
  a.analyze(100000);

  // Write outputfile with plots
  a.out(sname, outputdir);

}
