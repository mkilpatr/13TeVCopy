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
#include "AnalysisTools/TreeReader/interface/Defaults.h" // CSV_MEDIUM
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
      outtree->Branch("MET",&MET,"MET/F");
      outtree->Branch("HT",&HT,"HT/F");
      outtree->Branch("HTAovA",&HTAovA,"HTAovA/F");
      outtree->Branch("DphiLepW",&DphiLepW,"DphiLepW/F");
      outtree->Branch("DphiLepWRecoSeen",&DphiLepWRecoSeen,"DphiLepWRecoSeen/F");
      outtree->Branch("DphiLepWRecoMiss",&DphiLepWRecoMiss,"DphiLepWRecoMiss/F");
      outtree->Branch("minTopness",&minTopness,"minTopness/F");
      outtree->Branch("stdIso",&stdIso,"stdIso/F");
      outtree->Branch("LepPdgId",&LepPdgId,"LepPdgId/I");
      outtree->Branch("LepPt",&LepPt,"LepPt/F");
      outtree->Branch("MT",&mtlepmet,"mtlepmet/F");
      outtree->Branch("mt2w",&mt2w,"mt2w/D");
      outtree->Branch("hadronic_top_chi2",&hadronic_top_chi2,"hadronic_top_chi2/F");
      outtree->Branch("lep1eta",&lep1eta);
      outtree->Branch("lep1pt",&lep1pt);
      outtree->Branch("jetphi1",&jetphi1);
      outtree->Branch("jetphi2",&jetphi2);
      outtree->Branch("nOldVetoTracks",&nOldVetoTracks);
      outtree->Branch("nOldVetoElTracks",&nOldVetoElTracks);
      outtree->Branch("nOldVetoMuTracks",&nOldVetoMuTracks);
      outtree->Branch("nOldVetoCHTracks",&nOldVetoCHTracks);
      outtree->Branch("nNewVetoTracks",&nNewVetoTracks);
      outtree->Branch("nNewVetoElTracks",&nNewVetoElTracks);
      outtree->Branch("nNewVetoMuTracks",&nNewVetoMuTracks);
      outtree->Branch("nNewVetoCHTracks",&nNewVetoCHTracks);
      outtree->Branch("nVetoMVATaus",&nVetoMVATaus);
      outtree->Branch("nVetoed5GeVLeptons",&nVetoed5GeVLeptons);
      outtree->Branch("nVetoed10GeVLeptons",&nVetoed10GeVLeptons);
      outtree->Branch("nVetoCutTausISO1",&nVetoCutTausISO1);
      outtree->Branch("nVetoCutTausISO2",&nVetoCutTausISO2);
      outtree->Branch("nVetoFNALCutTaus",&nVetoFNALCutTaus);
      outtree->Branch("nElectronsGenFromTops",&nElectronsGenFromTops);
      outtree->Branch("nMuonsGenFromTops",&nMuonsGenFromTops);
      outtree->Branch("nTausGenFromTops",&nTausGenFromTops);       
      outtree->Branch("nLepsGenFromTops",&nLepsGenFromTops);
  }

  virtual ~Analyzer() {
    fout->cd();
    outtree->Write();
    fout->Close();
  }

  double lep1eta, lep1pt;

// baseline selection values
  const double xsec_;
  const double lumi_         = 1000.0; // 1 /fb in units of /pb
  const double metcut_       = 200.0;
  const double mtcut_        = 150.0;
  const double dphicut_      = 1.0;
  const double dphijetcut_   = 0.8;
  const int    nLeptons_     = 1;

// now in config

  const int    minNJets_     = 4;
  const int    minNBjets_    = 1;
  const float  minTrigMuPt_  = 30.0; // AN values
  const float  maxTrigMuEta_ = 2.1;
  const float  minTrigEPt_   = 40.0;
  const float  maxTrigEEta_  = 2.1;


  const TString sname_       = "test.root";
  const TString outputdir_   = "./plots/";

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
  int nVetoFNALCutTaus;
  int nVetoed5GeVLeptons;
  int nVetoed10GeVLeptons;
  int nElectronsGenFromTops, nMuonsGenFromTops, nTausGenFromTops;
  int nLepsGenFromTops;
  void loadVariables();
  void runEvent();
  void loadPlots();
  void out(TString outputName, TString outputPath);

  map<TString,TH1F*> plots;

};

void Analyzer::loadVariables()
{
load(cfgSet::PFCANDS); // load pfcands for track vetoes
load(cfgSet::TAUS); // for tau vetoes
load(cfgSet::EVTINFO);
load(cfgSet::AK4JETS);
//load(cfgSet::PICKYJETS);
//load(cfgSet::CASUBJETS);
load(cfgSet::ELECTRONS);
load(cfgSet::MUONS);
load(cfgSet::PHOTONS);
load(cfgSet::PFCANDS);
load(cfgSet::CMSTOPS);
if(isMC()) load(cfgSet::GENPARTICLES);
}

// Define plots, names, and binning
void Analyzer::loadPlots()
{

  TString txt_passjets = "N_{jets} >= 4, N_{bjets} >=1";
  TString txt_passmet = "pass met cut";
  TString txt_passpresel = "pass preselection";
  TString txt_ytitle = "Events";
  // TString txt_passsel = "presel && |#Delta#phi(l,#slash{E}_{T})|>1";
  // TString txt_passtopsel = "presel and minTopness>7";

  // plots after individual preselections
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
  plots["mt2w_passpresel"]       = new TH1F("mt2w_passpresel", TString(txt_passpresel+"; mt2w [GeV]; "+txt_ytitle).Data(), 50, 0, 1000);
  plots["minTopness_passpresel"]            = new TH1F("minTopness_passpresel", TString(txt_passpresel+"; minTopness [GeV]; "+txt_ytitle).Data(), 60,-10,20);

  // set Sumw2 for histograms
  for(map<TString,TH1F*>::iterator plotsIt = plots.begin(); plotsIt != plots.end(); ++plotsIt) {
    plotsIt->second->Sumw2();
  }

}

// Analyze event and fill plots
void Analyzer::runEvent()
{
  // good vertex requirement
  if(!goodvertex) return;

  // double wgt = lumi_*xsec_/getEntries();  // old way
  //float wgt = evtInfoReader.weight;
  ScaleFactor = weight;

  // always require exactly one selected lepton
  if(nSelLeptons != 1) return; // want exactly 1 lep passing selection
  //if(nVetoedLeptons > 1) return; // no more than 1 lep passing veto

  // the selected lepton object
  LeptonF* lep = selectedLeptons.at(0);
  LepPdgId = lep->pdgid();
  LepPt    = lep->pt();

  // SECOND LEPTON VETO (5 GeV natural)
  // need framework to have lepton pt veto lowered to 5 GeV, otherwise
  // we would need to remake the vetoedLeptons vector from allLeptons by hand here
  nVetoed5GeVLeptons=nVetoedLeptons;
    
  // SECOND LEPTON VETO (more restrictive 10 GeV)
  nVetoed10GeVLeptons=0;
  for(uint iT =0; iT < vetoedLeptons.size(); ++iT){  
    if(vetoedLeptons.at(iT)->pt() > 10) nVetoed10GeVLeptons++;
  }

  // OLD TRACK VETO
  // use pf candidates instead of tracks to avoid framework config issues (per Valentina)
  nOldVetoTracks=0;
  nOldVetoElTracks=0;
  nOldVetoMuTracks=0;
  nOldVeto10GeVElMuTracks=0;
  nOldVetoCHTracks=0;
  for(uint iT =0; iT < pfcandReader.pfcands.size(); ++iT){
    // all veto tracks are dz < 0.1, eta < 2.4, charge != 0
    if(fabs(pfcandReader.pfcands.at(iT).dz())   > 0.1) continue; 
    if(fabs(pfcandReader.pfcands.at(iT).eta())  > 2.4) continue;
    if(     pfcandReader.pfcands.at(iT).charge() == 0) continue;
    if(PhysicsUtilities::deltaR(pfcandReader.pfcands.at(iT).p4(),lep->p4())<0.1) continue;

    // lepton track veto
    if ((fabs(pfcandReader.pfcands.at(iT).pdgid())==11) || (fabs(pfcandReader.pfcands.at(iT).pdgid())==13)) {
	if ((pfcandReader.pfcands.at(iT).pt() > 5) && 
	    (pfcandReader.pfcands.at(iT).trackiso()/pfcandReader.pfcands.at(iT).pt() < 0.2)) {
		nOldVetoTracks++;
		if (fabs(pfcandReader.pfcands.at(iT).pdgid())==11) nOldVetoElTracks++;
		if (fabs(pfcandReader.pfcands.at(iT).pdgid())==13) nOldVetoMuTracks++;
 	}
    }
    // charged hadron track veto
    if (fabs(pfcandReader.pfcands.at(iT).pdgid())==211) {
    	if ((pfcandReader.pfcands.at(iT).charge()*lep->q() < 0) && 
	    (pfcandReader.pfcands.at(iT).trackiso()/pfcandReader.pfcands.at(iT).pt() < 0.1) && 
	    (pfcandReader.pfcands.at(iT).pt() > 10)) {
		nOldVetoTracks++;
		nOldVetoCHTracks++;
	}
    }
  }

  // NEW TRACK VETO
  nNewVetoTracks=0;
  nNewVetoElTracks=0;
  nNewVetoMuTracks=0;
  nNewVeto10GeVElMuTracks = 0;
  nNewVetoCHTracks=0;
  for(uint iT =0; iT < pfcandReader.pfcands.size(); ++iT){
    if(fabs(pfcandReader.pfcands.at(iT).dz())   > 0.1) continue; 
    if(fabs(pfcandReader.pfcands.at(iT).eta())  > 2.4) continue;
    if(     pfcandReader.pfcands.at(iT).charge() == 0) continue;
    if(PhysicsUtilities::deltaR(pfcandReader.pfcands.at(iT).p4(),lep->p4())<0.1) continue;

    // electron
    if (fabs(pfcandReader.pfcands.at(iT).pdgid())==11) {
	if (pfcandReader.pfcands.at(iT).pt() < 5) continue;
	if ((pfcandReader.pfcands.at(iT).pt() < 60) && 
	    (pfcandReader.pfcands.at(iT).trackiso()/pfcandReader.pfcands.at(iT).pt() < 0.1)) {
		nNewVetoTracks++;
		nNewVetoElTracks++;
	}
 	if ((pfcandReader.pfcands.at(iT).pt() > 60) && 
	    (pfcandReader.pfcands.at(iT).trackiso() < 6.0)) {
		nNewVetoTracks++;
		nNewVetoElTracks++;
	}
    }
    // muon
    if (fabs(pfcandReader.pfcands.at(iT).pdgid())==13) {
	if (pfcandReader.pfcands.at(iT).pt() < 5) continue;
	if ((pfcandReader.pfcands.at(iT).pt() < 30) && 
	    (pfcandReader.pfcands.at(iT).trackiso()/pfcandReader.pfcands.at(iT).pt() < 0.2)) {
		nNewVetoTracks++;
		nNewVetoMuTracks++;

	}
 	if ((pfcandReader.pfcands.at(iT).pt() > 30) && 
	    (pfcandReader.pfcands.at(iT).trackiso() < 6.0)) {
		nNewVetoTracks++;
		nNewVetoMuTracks++;
	}
    }
    // charged hadron track veto
    if (fabs(pfcandReader.pfcands.at(iT).pdgid())==211) {
	if (pfcandReader.pfcands.at(iT).pt() < 10) continue;
    	if (pfcandReader.pfcands.at(iT).charge()*lep->q() >= 0) continue; // want < 0
	if ((pfcandReader.pfcands.at(iT).pt() < 60) && 
	    (pfcandReader.pfcands.at(iT).trackiso()/pfcandReader.pfcands.at(iT).pt() < 0.1)) {
		nNewVetoTracks++;
		nNewVetoCHTracks++;
	}
	if ((pfcandReader.pfcands.at(iT).pt() > 60) && 
	    (pfcandReader.pfcands.at(iT).trackiso() < 6.0)) {
		nNewVetoTracks++;
		nNewVetoCHTracks++;
	}
    }
  }


  // MVA TAU VETO
  nVetoMVATaus = 0;
  for(uint iT =0; iT < pfcandReader.pfcands.size(); ++iT){
//    if(PhysicsUtilities::deltaR(pfcandReader.pfcands.at(iT).p4(),lep.p4())<0.4) continue;
    if ((fabs(pfcandReader.pfcands.at(iT).eta()) < 2.4) &&
             (pfcandReader.pfcands.at(iT).pt() > 10) && 
	     (pfcandReader.pfcands.at(iT).ismvavetotau()) && 
	(fabs(pfcandReader.pfcands.at(iT).dz()) < 0.2) && 
	(fabs(pfcandReader.pfcands.at(iT).pdgid()) == 211))
		nVetoMVATaus++;  
  }

  // TAU CUT VETO
  // not sure what yet, so try a few options for iso
  nVetoCutTausISO1 = 0;
  nVetoCutTausISO2 = 0;
  for(uint iT =0; iT < pfcandReader.pfcands.size(); ++iT){
//    if(PhysicsUtilities::deltaR(pfcandReader.pfcands.at(iT).p4(),lep.p4())<0.4) continue;
    if ((fabs(pfcandReader.pfcands.at(iT).pdgid()) == 211) && 
	(fabs(pfcandReader.pfcands.at(iT).dz()) < 0.2) && 
	     (pfcandReader.pfcands.at(iT).pt() > 10) &&
	     (fabs(pfcandReader.pfcands.at(iT).eta()) < 2.4) &&
	     (pfcandReader.pfcands.at(iT).mt() < defaults::TAU_MTCUT_VETO)) {
		if (pfcandReader.pfcands.at(iT).trackiso()/pfcandReader.pfcands.at(iT).pt() < 0.1) nVetoCutTausISO1++;
		if (pfcandReader.pfcands.at(iT).trackiso()/pfcandReader.pfcands.at(iT).pt() < 0.2) nVetoCutTausISO2++;
    }
  }

// HPS tau veto ("FNAL cut-based veto")
  vector<TauF*> taus;
  if(tauReader.isLoaded()){
    taus.clear();
    taus.reserve(tauReader.taus.size());
    for(auto& tau : tauReader.taus) {
      if (tau.pt() > 20 && fabs(tau.eta())<2.4 && (tau.hpsid() & kMediumIsoMVALT) > 0) taus.push_back(&tau);
      }
  }

 nVetoFNALCutTaus=0;
 for(uint iT =0; iT < taus.size(); ++iT){
    if(PhysicsUtilities::deltaR(taus.at(iT)->p4(),lep->p4())<0.4) continue;
    if(taus.at(iT)->q()*lep->q()<0) nVetoFNALCutTaus++;
  }



  // count gen level electrons, muons, taus coming from tops
  nElectronsGenFromTops=0;
  nMuonsGenFromTops=0;
  nTausGenFromTops=0;
         
  for (UInt_t i=0; i<genParts.size(); ++i) {
    const GenParticleF * genPartMom = 0;
    if (genParts.at(i)->numberOfMothers()>0) { genPartMom = genParts.at(i)->mother(0); }
    // make sure mum is a W
    if ((abs(genParts.at(i)->pdgId()) == 11) && (abs(genPartMom->pdgId()) == 24)) nElectronsGenFromTops++;
    if ((abs(genParts.at(i)->pdgId()) == 13) && (abs(genPartMom->pdgId()) == 24)) nMuonsGenFromTops++;
    if ((abs(genParts.at(i)->pdgId()) == 15) && (abs(genPartMom->pdgId()) == 24)) nTausGenFromTops++;
  }
  nLepsGenFromTops = nElectronsGenFromTops+nMuonsGenFromTops+nTausGenFromTops;


  lep1eta = selectedLeptons[0]->eta();
  lep1pt  = selectedLeptons[0]->pt();

  // fill tree variables
  NPV = nPV;
  NJets = nJets;
  NBJets = nBJets;
  NRecoLep = nSelLeptons;
  MET = met->pt();
  HT = JetKinematics::ht(jets,30,2.4);
  HTAovA = JetKinematics::htAlongHtAway(*met,jets,30,2.4);

  // reconstruct W=lep+met
  MomentumF* W = new MomentumF(lep->p4() + met->p4());

  // dphi(lep,W)
  DphiLepW = fabs(PhysicsUtilities::deltaPhi(*lep, *W));

  // the lepton's iso/pt value
  stdIso = (lep->pfdbetaiso())/(lep->pt());

  // mt(lep,met)
  mtlepmet = JetKinematics::transverseMass(*lep, *met);

  // topness
  minTopness = tNess->findMinTopnessConfiguration(selectedLeptons,jets,met,tNessInfo);

  // dphi(lep,W_seen), dphi(lep,W_miss), seen and miss id's come from topness
  DphiLepWRecoSeen = PhysicsUtilities::deltaPhi(tNessInfo->top1_l,tNessInfo->top1_w);
  DphiLepWRecoMiss = PhysicsUtilities::deltaPhi(tNessInfo->top1_l,tNessInfo->top2_w);

  // conversions required for mt2w, chi2
  vector<float> csvvec; // vector of the csv's of jets
  vector<LorentzVector> lzjets; // jets converted to lorentzvectors
  vector<float> sigma; // vector of the number 0.1
  vector<bool> btag; // bool vector of btags of jets

  LorentzVector lepvec; // convert lep to lorentzvector
  LorentzVector templz;
  lepvec=lep->p4();

  // calculate csv and jets conversions, sigma (jet resolution), btag vector
  for(unsigned int i = 0; i < jets.size(); i++ ) {
    csvvec.push_back(jets.at(i)->csv());
    templz = jets.at(i)->p4(); // convert from RecoJetF to LorentzVector
    lzjets.push_back(templz);

    sigma.push_back(0.1);// per twiki, use flat jet resolution of %10

    bool bjet=false;
    if(jets.at(i)->csv() > defaults::CSV_MEDIUM) bjet=true;
    btag.push_back(bjet);
  }
  
  hadronic_top_chi2= calculateChi2(lzjets, sigma, btag);

  // delta phis from two hardest jets to met, for QCD killer
  jetphi1 = fabs(PhysicsUtilities::deltaPhi(*jets[0],*met));
  jetphi2 = fabs(PhysicsUtilities::deltaPhi(*jets[1],*met));

  // always want to pass jets (array-length errors in rankedByCSV follow if not done)
  bool passjets   = nJets >= minNJets_ && nBJets >= minNBjets_;
  if (!passjets) return;

  // preselection and selection cuts if desired
  bool passmet    = met->pt() > metcut_;
  bool passdphi   = fabs(DphiLepW) > dphicut_;
  bool passmt     = mtlepmet > mtcut_;
  bool passchi2   = hadronic_top_chi2 < 10.;
  bool passjetphi = min(jetphi1,jetphi2) > 0.8;
  bool passpreselection = passmet && passmt && passchi2 && passjetphi;

  // fill histos for partial preselections
  //if(passjets) plots["met_passjets"]  ->Fill(met->pt(), wgt);
  //if(passmet)  plots["njets_passmet"] ->Fill(nJets, wgt);
  //if(passmet)  plots["nbjets_passmet"]->Fill(nBJets, wgt);

  if(passjets) plots["met_passjets"]  ->Fill(met->pt(), weight);
  if(passmet)  plots["njets_passmet"] ->Fill(nJets, weight);
  if(passmet)  plots["nbjets_passmet"]->Fill(nBJets, weight);

  // enforce preselection
//  if(!passpreselection) return;

  // calculate selection variables only after preselection (we might not have enough jets etc)
  mt2w = calculateMT2w(lzjets, csvvec, lepvec, met->pt(), met->phi());

  // sort the jets in decreasing csv, useful for higher level variables
  vector<RecoJetF*> jetsCSV;
  rankedByCSV(jets,jetsCSV);

  // plots after preselection
  //plots["ht_passpresel"]        ->Fill(JetKinematics::ht(jets), wgt);
  //plots["ht_passpresel"]        ->Fill(JetKinematics::ht(jets), wgt);
  //plots["leppt_passpresel"]     ->Fill(lep->pt(), wgt);
  //plots["jet1pt_passpresel"]    ->Fill(jets.at(0)->pt(), wgt);
  //plots["dphilepmet_passpresel"]->Fill(fabs(PhysicsUtilities::deltaPhi(*lep, *met)), wgt);
  //plots["dphilepw_passpresel"]  ->Fill(fabs(PhysicsUtilities::deltaPhi(*lep, *W)), wgt);
  //plots["mtlepmet_passpresel"]  ->Fill(JetKinematics::transverseMass(*lep, *met), wgt);

  //plots["mt2w_passpresel"]      ->Fill(mt2w,wgt);
  //plots["minTopness_passpresel"]       ->Fill(minTopness,wgt);

  plots["ht_passpresel"]        ->Fill(JetKinematics::ht(jets), weight);
  plots["leppt_passpresel"]     ->Fill(lep->pt(), weight);
  plots["jet1pt_passpresel"]    ->Fill(jets.at(0)->pt(), weight);
  plots["dphilepmet_passpresel"]->Fill(fabs(PhysicsUtilities::deltaPhi(*lep, *met)), weight);
  plots["dphilepw_passpresel"]  ->Fill(fabs(PhysicsUtilities::deltaPhi(*lep, *W)), weight);
  plots["mtlepmet_passpresel"]  ->Fill(JetKinematics::transverseMass(*lep, *met), weight);

  plots["mt2w_passpresel"]      ->Fill(mt2w,weight);
  plots["minTopness_passpresel"]       ->Fill(minTopness,weight);

// event list used to sync with Pieter (NOT the twiki sync)
//  int genlepsfromtop = nElectronsGenFromTops+nMuonsGenFromTops+nTausGenFromTops;
//  if (NBJets>=1 && NJets>=4 &&   genlepsfromtop==2 && MET>200 && nSelLeptons==1)
//    std::cout << lumi << " " << event << endl;

// lepton printouts to sync with the twiki
// electron list
//   if(fabs(lep->pdgid())==11 && NBJets>0 && NJets>3 && MET>80) std::cout<<run<<" "<<lumi<<" "<<event<<" "<<fixed<<setprecision(4)<<lep->pt()<<" "<<lep->pdgid()<<" "<<MET<<" "<<MT<<" "<< 0. <<" "<<NJets<<" "<<NBJets<<" "<< nLepsGenFromTops<<" "<<DphiLepW<<" "<<0.<<" "<<0.<<" "<<0.<<" "<<mt2w<<" "<<minTopness<<" "<<hadronic_top_chi2<<std::endl;
// muon list
//   if(fabs(lep->pdgid())==13 && NBJets>0 && NJets>3 && MET>80) std::cout<<run<<" "<<lumi<<" "<<event<<" "<<fixed<<setprecision(4)<<lep->pt()<<" "<<lep->pdgid()<<" "<<MET<<" "<<MT<<" "<< 0. <<" "<<NJets<<" "<<NBJets<<" "<< nLepsGenFromTops<<" "<<DphiLepW<<" "<<0.<<" "<<0.<<" "<<0.<<" "<<mt2w<<" "<<minTopness<<" "<<hadronic_top_chi2<<std::endl;

// Pieter's printouts (from make1lbabies.C) to see which variables are skipped here
// if(fabs(lep1_pdgid)==11 && ngoodbtags>0 && ngoodjets>3 && pfmet>80) std::cout<<run<<" "<<ls<<" "<<event<<" "<<fixed<<setprecision(4)<<lep1_pt<<" "<<lep1_pdgid<<" "<<pfmet<<" "<<mt_met_lep<<" "<<lep1_passVeto<<" "<<ngoodjets<<" "<<ngoodbtags<<" "<<genlepsfromtop<<" "<<dphi_Wlep<<" "<<ak4_htssm<<" "<<Mlb_closestb<<" "<<Mjjj<<" "<<MT2W<<" "<<topness<<" "<<hadronic_top_chi2<<std::endl;
// muon list
// if(fabs(lep1_pdgid)==13 && ngoodbtags>0 && ngoodjets>3 && pfmet>80) std::cout<<run<<" "<<ls<<" "<<event<<" "<<fixed<<setprecision(4)<<lep1_pt<<" "<<lep1_pdgid<<" "<<pfmet<<" "<<mt_met_lep<<" "<<PassTrackVeto<<" "<<ngoodjets<<" "<<ngoodbtags<<" "<<genlepsfromtop<<" "<<dphi_Wlep<<" "<<ak4_htssm<<" "<<Mlb_closestb<<" "<< 0. <<" "<<MT2W<<" "<<topness<<" "<<hadronic_top_chi2<<"\n";

// fill _tree.root
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
  for(unsigned int iJ =0; iJ < inJets.size(); ++iJ){ 
    outJets[iJ] = inJets[rankedJets[iJ].second]; 
  }

}

// Process file belonging to specified sample with a given cross section
void processSingleLepton(TString sname = "test",               // sample name
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

// load one lepton default configs
  cfgSet::loadDefaultConfigurations();
  cfgSet::ConfigSet cfg = cfgSet::ol_search_set;
//  cfg.jets.jetCollection = cfgSet::PICKYJETS; // to override ak4jets with pickyjets

  // use the following for tau/track veto comparison
  //cfg.vetoedLeptons.minEPt = 5;
  //cfg.vetoedLeptons.minMuPt = 5;

  // Declare analyzer
  //Analyzer a(fullname, "Events", isMC, &cfg, xsec, sname, outputdir);
  Analyzer a(fullname, "TestAnalyzer/Events", isMC, &cfg, xsec, sname, outputdir);

  // Run! Argument is frequency of printout
  a.analyze(200000000);

  // Write outputfile with plots
  a.out(sname, outputdir);

}
