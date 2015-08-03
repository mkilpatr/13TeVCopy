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
#include <vector>
#include "Math/VectorUtil.h"
#include "AnalysisTools/KinematicVariables/interface/mt2w.h"
#include "AnalysisTools/TreeReader/interface/Defaults.h" // CSV_MEDIUM WP
#include "AnalysisTools/KinematicVariables/interface/chi2.h"
#endif

using namespace ucsbsusy;

class Analyzer : public BaseTreeAnalyzer {

  public :
    void rankedByCSV(vector<RecoJetF*>& inJets,vector<RecoJetF*>& outJets);

  Analyzer(TString fileName, TString treeName, bool isMCTree, cfgSet::ConfigSet * pars, double xSec, TString sname, TString outputdir) :
    BaseTreeAnalyzer(fileName, treeName, isMCTree, pars),xsec_(xSec), sname_(sname), outputdir_(outputdir) {
      // configuration
      tNess     = new Topness();
      tNessInfo = new TopnessInformation();

      // initiliaze tree
      gSystem->mkdir(outputdir,true);
      fout = new TFile (outputdir+"/"+sname+"_tree.root","RECREATE");
      fout->cd();
      outtree = new TTree("events","analysis tree");
      outtree->Branch("ScaleFactor",&ScaleFactor,"ScaleFactor/F");
      outtree->Branch("NPV",&NPV,"NPV/I");
      outtree->Branch("NJets",&NJets,"NJets/I");
      outtree->Branch("NBJets",&NBJets,"NBJets/I");
      outtree->Branch("leadJetPt",&leadJetPt);
      outtree->Branch("leadJetEta",&leadJetEta);
      outtree->Branch("subleadJetPt",&subleadJetPt);
      outtree->Branch("subleadJetEta",&subleadJetEta);
      outtree->Branch("nSelLep",&NRecoLep,"NRecoLep/I");
      outtree->Branch("LepStdIso",&stdIso,"stdIso/F");
      outtree->Branch("LepPdgId",&LepPdgId,"LepPdgId/I");
      outtree->Branch("LepPt",&LepPt,"LepPt/F");
      outtree->Branch("LepEta",&LepEta);
      outtree->Branch("MET",&MET,"MET/F");
      outtree->Branch("HT",&HT,"HT/F");
      outtree->Branch("HTAovA",&HTAovA,"HTAovA/F");
      outtree->Branch("DphiLepW",&DphiLepW,"DphiLepW/F");
      outtree->Branch("DphiLepWRecoSeen",&DphiLepWRecoSeen,"DphiLepWRecoSeen/F");
      outtree->Branch("DphiLepWRecoMiss",&DphiLepWRecoMiss,"DphiLepWRecoMiss/F");
      outtree->Branch("minTopness",&minTopness,"minTopness/F");
      outtree->Branch("MT",&mtlepmet,"mtlepmet/F");
      outtree->Branch("mt2w",&mt2w,"mt2w/D");
      outtree->Branch("hadronic_top_chi2",&hadronic_top_chi2,"hadronic_top_chi2/F");
      outtree->Branch("jetphi1",&jetphi1);
      outtree->Branch("jetphi2",&jetphi2);
      outtree->Branch("nVetoHPSTaus",&nVetoHPSTaus);
      outtree->Branch("nVetoed5GeVLeptons",&nVetoed5GeVLeptons);
      outtree->Branch("nVetoed10GeVLeptons",&nVetoed10GeVLeptons);
      outtree->Branch("nElectronsGenFromTops",&nElectronsGenFromTops);
      outtree->Branch("nMuonsGenFromTops",&nMuonsGenFromTops);
      outtree->Branch("nTausGenFromTops",&nTausGenFromTops);       
      outtree->Branch("nLepsGenFromTops",&nLepsGenFromTops);

// track veto variables. Requires loading PFCANDS.
/*
      outtree->Branch("nOldVetoTracks",&nOldVetoTracks);
      outtree->Branch("nOldVetoElTracks",&nOldVetoElTracks);
      outtree->Branch("nOldVetoMuTracks",&nOldVetoMuTracks);
      outtree->Branch("nOldVetoCHTracks",&nOldVetoCHTracks);
      outtree->Branch("nNewVetoTracks",&nNewVetoTracks);
      outtree->Branch("nNewVetoElTracks",&nNewVetoElTracks);
      outtree->Branch("nNewVetoMuTracks",&nNewVetoMuTracks);
      outtree->Branch("nNewVetoCHTracks",&nNewVetoCHTracks);
      outtree->Branch("nVetoMVATaus",&nVetoMVATaus);
      outtree->Branch("nVetoCutTausISO1",&nVetoCutTausISO1);
      outtree->Branch("nVetoCutTausISO2",&nVetoCutTausISO2);
*/
  }

  virtual ~Analyzer() {
    fout->cd();
    outtree->Write();
    fout->Close();
  }

// baseline selection values
  const double xsec_;
  const double lumi_         = 1000.0; // samples at 1/fb
  const double metcut_       = 200.0;
  const double mtcut_        = 150.0;
  const double dphicut_      = 1.0;
  const double dphijetcut_   = 0.8;
  const int    nLeptons_     = 1;
  const int    minNJets_     = 4;
  const int    minNBjets_    = 1;

  const TString sname_       = "test.root";
  const TString outputdir_   = "./trees/";

// engage topness, mt2x
  Topness *tNess;
  TopnessInformation *tNessInfo;
  mt2w_bisect::mt2w mt2w_event;

  TFile *fout;
  TTree *outtree;

  float ScaleFactor;
  int NPV;
  int NJets;
  int NBJets;
  int NRecoLep;
  float MET;
  float HT;
  float HTAovA;
  float leadJetPt;
  float leadJetEta;
  float subleadJetPt;
  float subleadJetEta;
  float DphiLepW;
  float DphiLepWRecoSeen;
  float DphiLepWRecoMiss;
  float jetphi1;
  float jetphi2;
  float minTopness;
  float stdIso;
  float mtlepmet;
  int LepPdgId;
  float LepPt;
  float LepEta;
  float MT;
  double mt2w;
  float hadronic_top_chi2;
  int nOldVetoTracks;
  int nOldVetoElTracks;
  int nOldVetoMuTracks;
  int nOldVeto10GeVElMuTracks;
  int nOldVetoCHTracks;
  int nNewVetoTracks;
  int nNewVetoElTracks;
  int nNewVetoMuTracks;
  int nNewVeto10GeVElMuTracks;
  int nNewVetoCHTracks;
  int nVetoMVATaus;
  int nVetoCutTausISO1;
  int nVetoCutTausISO2;
  int nVetoHPSTaus;
  int nVetoed5GeVLeptons;
  int nVetoed10GeVLeptons;
  int nElectronsGenFromTops, nMuonsGenFromTops, nTausGenFromTops;
  int nLepsGenFromTops;

  void loadVariables();
  void runEvent();
};

void Analyzer::loadVariables()
{
  //load(cfgSet::PFCANDS); // load pfcands for track vetoes
  load(cfgSet::TAUS); // for tau vetoes
  load(cfgSet::EVTINFO);
  load(cfgSet::AK4JETS);
  //load(cfgSet::PICKYJETS);
  //load(cfgSet::CASUBJETS);
  load(cfgSet::ELECTRONS);
  load(cfgSet::MUONS);
  load(cfgSet::PHOTONS);
  //load(cfgSet::CMSTOPS);
  if(isMC()) load(cfgSet::GENPARTICLES);
}

// Analyze event
void Analyzer::runEvent()
{
  // double wgt = lumi_*xsec_/getEntries();  // old way
  //float wgt = evtInfoReader.weight; // new way, same thing
  ScaleFactor = weight;

  // good vertex requirement
  if(!goodvertex) return;

  // mandatory jet cut here (since below variables assume >= 2 jets)
  bool passjets = nJets >= minNJets_ && nBJets >= minNBjets_;
  if (!passjets) return;

  // jet kinematics
  leadJetPt  	= jets.at(0)->pt();
  leadJetEta 	= jets.at(0)->eta();
  subleadJetPt 	= jets.at(1)->pt();
  subleadJetEta	= jets.at(1)->eta();

  // one selected lepton; can uncomment this line and use the tree variable
  if(nSelLeptons != 1) return;

  // the selected lepton
  LeptonF* lep = selectedLeptons.at(0);
  LepPdgId = lep->pdgid();
  LepPt    = lep->pt();
  LepEta   = lep->eta();

  // 5 GeV additional lepton veto (it's set to 5 GeV in framework)
  nVetoed5GeVLeptons = nVetoedLeptons;
    
  // 10 GeV add' lepton veto
  nVetoed10GeVLeptons = 0;
  for(uint iT = 0; iT < vetoedLeptons.size(); ++iT){  
    if(vetoedLeptons.at(iT)->pt() > 10) nVetoed10GeVLeptons++;
  }

  // count gen level electrons, muons, taus coming from tops
  nElectronsGenFromTops = 0;
  nMuonsGenFromTops = 0;
  nTausGenFromTops = 0;
         
  for (UInt_t i=0; i<genParts.size(); ++i) {
    const GenParticleF * genPartMom = 0;
    if (genParts.at(i)->numberOfMothers()>0) { genPartMom = genParts.at(i)->mother(0); }
    // make sure mum is a W
    if ((abs(genParts.at(i)->pdgId()) == 11) && (abs(genPartMom->pdgId()) == 24)) nElectronsGenFromTops++;
    if ((abs(genParts.at(i)->pdgId()) == 13) && (abs(genPartMom->pdgId()) == 24)) nMuonsGenFromTops++;
    if ((abs(genParts.at(i)->pdgId()) == 15) && (abs(genPartMom->pdgId()) == 24)) nTausGenFromTops++;
  }
  nLepsGenFromTops = nElectronsGenFromTops + nMuonsGenFromTops + nTausGenFromTops;

  // fill tree variables
  NPV		= nPV;
  NJets 	= nJets;
  NBJets 	= nBJets;
  NRecoLep 	= nSelLeptons;
  MET 		= met->pt();
  HT 		= JetKinematics::ht(jets,30,2.4);
  HTAovA 	= JetKinematics::htAlongHtAway(*met,jets,30,2.4);

  // reconstruct W = lep + met
  MomentumF* W = new MomentumF(lep->p4() + met->p4());

  // dphi(lep,W)
  DphiLepW = fabs(PhysicsUtilities::deltaPhi(*lep, *W));

  // the lepton's iso/pt value
  stdIso = (lep->pfdbetaiso())/(lep->pt());

  // mt(lep,met)
  mtlepmet = JetKinematics::transverseMass(*lep, *met);

  // topness (jet selection can be improved)
  minTopness = tNess->findMinTopnessConfiguration(selectedLeptons,jets,met,tNessInfo);

  // dphi(lep,W_seen), dphi(lep,W_miss), using the seen and miss id's from topness
  DphiLepWRecoSeen = PhysicsUtilities::deltaPhi(tNessInfo->top1_l,tNessInfo->top1_w);
  DphiLepWRecoMiss = PhysicsUtilities::deltaPhi(tNessInfo->top1_l,tNessInfo->top2_w);

  // mt2w, chi2 require type conversions
  vector<float> 	csvvec; // vector of the csv's of jets
  vector<LorentzVector> lzjets; // jets converted to lorentzvectors
  vector<float> 	sigma; 	// vector of the number 0.1
  vector<bool> 		btag; 	// bool vector of btags of jets
  LorentzVector 	lepvec; // convert lep to lorentzvector
  LorentzVector 	templz;
  lepvec = lep->p4();

  // HPS id tau veto
  vector<TauF*> taus;
  if(tauReader.isLoaded()){
    taus.clear();
    taus.reserve(tauReader.taus.size());
    for(auto& tau : tauReader.taus) {
      if (tau.pt() > 20 && fabs(tau.eta())<2.4 && (tau.hpsid() & kMediumIsoMVALT) > 0) taus.push_back(&tau);
      }
  }
 nVetoHPSTaus=0;
  for(uint iT =0; iT < taus.size(); ++iT){
    if(PhysicsUtilities::deltaR(taus.at(iT)->p4(),lep->p4())<0.4) continue;
    if(taus.at(iT)->q()*lep->q()<0) nVetoHPSTaus++;
  }

  // sort the jets in decreasing csv, useful for testing higher level variables
  vector<RecoJetF*> jetsCSV;
  rankedByCSV(jets,jetsCSV);

  // csv and jets type conversions; make sigma (jet resolution); make btag vector
  for(unsigned int i = 0; i < jets.size(); i++ ) {
    csvvec.push_back(jets.at(i)->csv());
    templz = jets.at(i)->p4(); // convert from RecoJetF to LorentzVector
    lzjets.push_back(templz);

    sigma.push_back(0.1);// per twiki, use flat jet resolution of %10

    bool bjet=false;
    if(jets.at(i)->csv() > defaults::CSV_MEDIUM) bjet=true;
    btag.push_back(bjet);
  }
  hadronic_top_chi2 = calculateChi2(lzjets, sigma, btag);
  mt2w = calculateMT2w(lzjets, csvvec, lepvec, met->pt(), met->phi());

  // delta phis from two hardest jets to met, for QCD killer
  jetphi1 = fabs(PhysicsUtilities::deltaPhi(*jets.at(0),*met));
  jetphi2 = fabs(PhysicsUtilities::deltaPhi(*jets.at(1),*met));

// print event list used to sync with Pieter (NOT the twiki sync)
//  int genlepsfromtop = nElectronsGenFromTops+nMuonsGenFromTops+nTausGenFromTops;
//  if (NBJets>=1 && NJets>=4 &&   genlepsfromtop==2 && MET>200 && nSelLeptons==1)
//    std::cout << lumi << " " << event << endl;

// print lepton list to sync with the twiki
// electron list
//   if(fabs(lep->pdgid())==11 && NBJets>0 && NJets>3 && MET>80) std::cout<<run<<" "<<lumi<<" "<<event<<" "<<fixed<<setprecision(4)<<lep->pt()<<" "<<lep->pdgid()<<" "<<MET<<" "<<MT<<" "<< 0. <<" "<<NJets<<" "<<NBJets<<" "<< nLepsGenFromTops<<" "<<DphiLepW<<" "<<0.<<" "<<0.<<" "<<0.<<" "<<mt2w<<" "<<minTopness<<" "<<hadronic_top_chi2<<std::endl;
// muon list
//   if(fabs(lep->pdgid())==13 && NBJets>0 && NJets>3 && MET>80) std::cout<<run<<" "<<lumi<<" "<<event<<" "<<fixed<<setprecision(4)<<lep->pt()<<" "<<lep->pdgid()<<" "<<MET<<" "<<MT<<" "<< 0. <<" "<<NJets<<" "<<NBJets<<" "<< nLepsGenFromTops<<" "<<DphiLepW<<" "<<0.<<" "<<0.<<" "<<0.<<" "<<mt2w<<" "<<minTopness<<" "<<hadronic_top_chi2<<std::endl;

// Pieter's lepton list (from make1lbabies.C) showing which variables aren't calculated here
// if(fabs(lep1_pdgid)==11 && ngoodbtags>0 && ngoodjets>3 && pfmet>80) std::cout<<run<<" "<<ls<<" "<<event<<" "<<fixed<<setprecision(4)<<lep1_pt<<" "<<lep1_pdgid<<" "<<pfmet<<" "<<mt_met_lep<<" "<<lep1_passVeto<<" "<<ngoodjets<<" "<<ngoodbtags<<" "<<genlepsfromtop<<" "<<dphi_Wlep<<" "<<ak4_htssm<<" "<<Mlb_closestb<<" "<<Mjjj<<" "<<MT2W<<" "<<topness<<" "<<hadronic_top_chi2<<std::endl;
// muon list
// if(fabs(lep1_pdgid)==13 && ngoodbtags>0 && ngoodjets>3 && pfmet>80) std::cout<<run<<" "<<ls<<" "<<event<<" "<<fixed<<setprecision(4)<<lep1_pt<<" "<<lep1_pdgid<<" "<<pfmet<<" "<<mt_met_lep<<" "<<PassTrackVeto<<" "<<ngoodjets<<" "<<ngoodbtags<<" "<<genlepsfromtop<<" "<<dphi_Wlep<<" "<<ak4_htssm<<" "<<Mlb_closestb<<" "<< 0. <<" "<<MT2W<<" "<<topness<<" "<<hadronic_top_chi2<<"\n";

// fill _tree.root
  outtree->Fill();
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
  for(unsigned int iJ =0; iJ < inJets.size(); ++iJ){ 
    outJets[iJ] = inJets[rankedJets[iJ].second]; 
  }

}

// Process file belonging to specified sample with a given cross section
void makeOneLepTrees(TString sname = "test",               // sample name
                         const int fileindex = -1,             // index of file (-1 means there is only 1 file for this sample)
                         const bool isMC = true,               // data or MC
                         const TString fname = "evttree_numEvent1000.root", // path of file to be processed
                         const double xsec = 1.0,              // cross section to be used with this file
                         const TString outputdir = "run/trees",    // directory to which files with trees will be written
                         const TString fileprefix = "file:$CMSSW_BASE/src/AnalysisBase/Analyzer/test/") // prefix for file name, needed e.g. to access files with xrootd
{

  printf("Processing file %d of %s sample\n", (fileindex > -1 ? fileindex : 0), sname.Data());

  // Make sure the output has a unique name in case there are multiple files to process
  if(fileindex > -1)
    sname += TString::Format("_%d",fileindex);

  if(isMC)
    printf("Cross section: %5.2f pb\n", xsec);

  TString fullname = fileprefix+fname;

// load one lepton default configs
  cfgSet::loadDefaultConfigurations();
  cfgSet::ConfigSet cfg = cfgSet::ol_search_set;

// to use pickyjets instead, load PICKYJETS and uncomment this line
//  cfg.jets.jetCollection = cfgSet::PICKYJETS;

  // vetoed lepton thresholds have been updated in framework to 5 GeV. 
  //cfg.vetoedLeptons.minEPt  = 5;
  //cfg.vetoedLeptons.minMuPt = 5;

  // Declare analyzer
  TString treename = isMC ? "Events":"TestAnalyzer/Events";
  Analyzer a(fullname, treename, isMC, &cfg, xsec, sname, outputdir);

  // Run! Argument is frequency of printout
  a.analyze(100000);
}

