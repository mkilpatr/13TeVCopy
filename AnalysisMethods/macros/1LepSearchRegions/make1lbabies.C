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
#include "AnalysisTools/KinematicVariables/interface/mt2w.h"
#include "AnalysisTools/KinematicVariables/interface/chi2.h"
//#include "AnalysisTools/KinematicVariables/interface/DileptonAnalyticalSolver.h"
#include "AnalysisTools/TreeReader/interface/ElectronReader.h"
#include "AnalysisTools/DataFormats/interface/GenParticle.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include <sstream>
#include <string>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
//#include <topnessv2.h>
#endif

using namespace ucsbsusy;


bool pairCompare(const std::pair<double, int>& firstElem, const std::pair<double, int>& secondElem) {
  return firstElem.first > secondElem.first;
}

class Analyzer : public BaseTreeAnalyzer {

  public :

  Analyzer(TString fileName, TString treeName,  size randSeed, bool isMCTree, cfgSet::ConfigSet * pars, double xSec, TString sname, TString outputdir) :
    BaseTreeAnalyzer(fileName, treeName, randSeed,isMCTree, pars),xsec_(xSec), sname_(sname), outputdir_(outputdir) {
      // configuration

      tNess     = new Topness();
      tNessInfo = new TopnessInformation();

      // initiliaze tree
      gSystem->mkdir(outputdir,true);
      fout = new TFile (outputdir+"/"+sname+"_tree.root","RECREATE");
      fout->cd();
      outtree = new TTree("t","t");
      outtree->Branch("lep1_pdgid",&lep1_pdgid);
      outtree->Branch("lep1_pt",&lep1_pt);
      outtree->Branch("lep1_eta",&lep1_eta);
      outtree->Branch("lep1_phi",&lep1_phi);
      outtree->Branch("lep1_mass",&lep1_mass);
      outtree->Branch("lep1_passVeto",&lep1_passVeto);
      outtree->Branch("lep1_passMediumID",&lep1_passMediumId);
      outtree->Branch("lep1_d0",&lep1_d0);
      outtree->Branch("lep1_dz",&lep1_dz);
      outtree->Branch("lep1_MiniIso",&lep1_MiniIso);
      outtree->Branch("lep2_pdgid",&lep2_pdgid);
      outtree->Branch("lep2_pt",&lep2_pt);
      outtree->Branch("lep2_eta",&lep2_eta);
      outtree->Branch("lep2_phi",&lep2_phi);
      outtree->Branch("lep2_mass",&lep2_mass);
      outtree->Branch("lep2_passVeto",&lep2_passVeto);
      outtree->Branch("lep2_passMediumID",&lep2_passMediumId);
      outtree->Branch("lep2_d0",&lep2_d0);
      outtree->Branch("lep2_dz",&lep2_dz);
      outtree->Branch("lep2_MiniIso",&lep2_MiniIso);
      outtree->Branch("ak4pfjets_pt", &ak4pfjets_pt);
      outtree->Branch("ak4pfjets_eta", &ak4pfjets_eta);
      outtree->Branch("ak4pfjets_phi", &ak4pfjets_phi);
      outtree->Branch("ak4pfjets_mass", &ak4pfjets_mass);
      outtree->Branch("ak4pfjets_CSV", &ak4pfjets_CSV);
      outtree->Branch("ak4pfjets_loose_pfid", &ak4pfjets_loose_pfid);
      outtree->Branch("ak4pfjets_puid", &ak4pfjets_puid);
      outtree->Branch("dphi_ak4pfjets_met", &dphi_ak4pfjets_met);
      outtree->Branch("dphi_1",&dphi_1);
      outtree->Branch("dphi_2",&dphi_2);
      outtree->Branch("run",&run2);
      outtree->Branch("ls",&ls);
      outtree->Branch("evt",&evt);
      outtree->Branch("PassTrackVeto",&PassTrackVeto);
      outtree->Branch("PassTauVeto",&PassTauVeto);
      outtree->Branch("ak4pfjets_rho", &ak4pfjets_rho);
      outtree->Branch("pfmet",&pfmet);
      outtree->Branch("pfmet_phi",&pfmet_phi);
      outtree->Branch("mt_met_lep",&mt_met_lep);
      outtree->Branch("ngoodjets",&ngoodjets);
      outtree->Branch("ngoodbtags",&ngoodbtags);
      outtree->Branch("ngoodleps",&ngoodleps);
      outtree->Branch("nvetoleps",&nvetoleps);
      outtree->Branch("genlepsfromtop",&genlepsfromtop);
      outtree->Branch("MT2W",&MT2W);
      outtree->Branch("hadronic_top_chi2",&hadronic_top_chi2);
      outtree->Branch("topness",&topness);
      outtree->Branch("dphi_Wlep", &dphi_Wlep);
      outtree->Branch("ak4_HT", &ak4_HT);
      outtree->Branch("ak4_htssm", &ak4_htssm);
      outtree->Branch("ak4_htosm", &ak4_htosm);
      outtree->Branch("Mlb_closestb", &Mlb_closestb);
      outtree->Branch("Mlb_lead_bdiscr", &Mlb_lead_bdiscr);
      outtree->Branch("Mjjj", &Mjjj);
      outtree->Branch("dR_lep_leadb", &dR_lep_leadb);
      outtree->Branch("HLT_SingleMu", &HLT_SingleMu);
      outtree->Branch("HLT_SingleMu_eff", &HLT_SingleMu_eff);
      outtree->Branch("HLT_SingleEl", &HLT_SingleEl);
      outtree->Branch("HLT_SingleEl_eff", &HLT_SingleEl_eff);
      outtree->Branch("pu_weight", &pu_weight);
      outtree->Branch("scale1fb", &scale1fb);
      outtree->Branch("lep_sf", &lep_sf);
      outtree->Branch("btag_sf", &btag_sf);
      outtree->Branch("sparms_names", &sparms_values);
      outtree->Branch("mass_stop", &mass_stop);
      outtree->Branch("mass_lsp", &mass_lsp);
      outtree->Branch("mass_chargino", &mass_chargino);
    }
  
  virtual ~Analyzer() {
    fout->cd();
    outtree->Write();
    fout->Close();
  }
  

  Topness *tNess;
  TopnessInformation *tNessInfo;
  TFile *fout;
  TTree *outtree;

  const double xsec_;
  const double luminosity         = 1.0; // in /fb
  const TString sname_       = "babytuple.root";
  const TString outputdir_   = "./babies/";  
  int lep1_pdgid;
  float lep1_pt;
  float lep1_eta;
  float lep1_phi;
  float lep1_mass;
  bool  lep1_passVeto;
  bool  lep1_passMediumId;
  float  lep1_d0;
  float  lep1_dz;
  float  lep1_MiniIso;
  int lep2_pdgid;
  float lep2_pt;
  float lep2_eta;
  float lep2_phi;
  float lep2_mass;
  bool  lep2_passVeto;
  bool  lep2_passMediumId;
  float  lep2_d0;
  float  lep2_dz;
  float  lep2_MiniIso;
  vector<float> ak4pfjets_pt;
  vector<float> ak4pfjets_eta;
  vector<float> ak4pfjets_phi;
  vector<float> ak4pfjets_mass;
  vector<float> ak4pfjets_CSV;
  vector<bool> ak4pfjets_loose_pfid;
  vector<bool> ak4pfjets_puid;
  vector<float> dphi_ak4pfjets_met;
  float dphi_1;
  float dphi_2;
  unsigned int run2;
  unsigned int ls;
  unsigned int evt;
  bool PassTrackVeto;
  bool PassTauVeto;
  float ak4pfjets_rho;
  float pfmet;
  float pfmet_phi;
  float mt_met_lep;
  int ngoodjets;
  int ngoodbtags;
  int ngoodleps;
  int nvetoleps;
  int genlepsfromtop;
  float MT2W;
  float hadronic_top_chi2;
  float topness;
  float dphi_Wlep;
  float ak4_HT;
  float ak4_htssm;
  float ak4_htosm;
  float Mlb_closestb;
  float Mlb_lead_bdiscr;
  float Mjjj;
  float dR_lep_leadb;
  float HLT_SingleMu;
  float HLT_SingleMu_eff;
  float HLT_SingleEl;
  float HLT_SingleEl_eff;
  float pu_weight;
  float scale1fb;
  float lep_sf;
  float btag_sf;
  vector<string> sparms_names;
  vector<float> sparms_values;
  float mass_stop;
  float mass_lsp;
  float mass_chargino;


  void runEvent();


 
};

// Analyze event and fill plots
void Analyzer::runEvent()
{

  //scale1fb = luminosity*xsec_/getEntries();
  float wgt = evtInfoReader.weight;
  scale1fb = wgt;
    
  if(!goodvertex) return;
  if(nSelLeptons==0 || nJets<4) return;
    lep2_pdgid=0;
    lep2_pt=0;
    lep2_eta=0;
    lep2_phi=0;
    lep2_mass=0;
    lep2_d0=0;
    lep2_dz=0;
    lep2_passVeto=kFALSE;
    lep2_passMediumId=kFALSE;
    lep2_MiniIso=0;
    lep1_pdgid=0;
    lep1_pt=0;
    lep1_eta=0;
    lep1_phi=0;
    lep1_mass=0;
    lep1_d0=0;
    lep1_dz=0;
    lep1_passVeto=kFALSE;
    lep1_passMediumId=kFALSE;
    lep1_MiniIso=0;
    ngoodleps=0;
    const LeptonF *lep;
  for(int iL =0; iL < nSelLeptons; ++iL){ 
    if(selectedLeptons.at(iL)->pt()>lep1_pt && fabs(selectedLeptons.at(iL)->eta())<2.1){
    lep=selectedLeptons.at(iL);
    lep2_pdgid=lep1_pdgid;
    lep2_pt=lep1_pt;
    lep2_eta=lep1_eta;
    lep2_phi=lep1_phi;
    lep2_mass=lep1_mass;
    lep2_d0=lep1_d0;
    lep2_dz=lep1_dz;
    lep2_passVeto=lep1_passVeto;
    lep2_passMediumId=lep1_passMediumId;
    lep2_MiniIso=lep1_MiniIso;
    lep1_pdgid=-selectedLeptons.at(iL)->pdgid()*selectedLeptons.at(iL)->q();
    lep1_pt=selectedLeptons.at(iL)->pt();
    lep1_eta=selectedLeptons.at(iL)->eta();
    lep1_phi=selectedLeptons.at(iL)->phi();
    lep1_mass=selectedLeptons.at(iL)->mass(); 
    lep1_passMediumId=kTRUE;
    lep1_d0=selectedLeptons.at(iL)->d0();
    lep1_dz=selectedLeptons.at(iL)->dz();
    lep1_MiniIso=selectedLeptons.at(iL)->miniiso();
    }
    else if(selectedLeptons.at(iL)->pt()>lep2_pt){
    lep2_pdgid=-selectedLeptons.at(iL)->pdgid()*selectedLeptons.at(iL)->q();
    lep2_pt=selectedLeptons.at(iL)->pt();
    lep2_eta=selectedLeptons.at(iL)->eta();
    lep2_phi=selectedLeptons.at(iL)->phi();
    lep2_mass=selectedLeptons.at(iL)->mass(); 
    lep2_passMediumId=kTRUE;
    lep2_d0=selectedLeptons.at(iL)->d0();
    lep2_dz=selectedLeptons.at(iL)->dz();
    lep2_MiniIso=selectedLeptons.at(iL)->miniiso();
    }
  if(fabs(selectedLeptons.at(iL)->eta())<2.1) ngoodleps++;
  }
  if(ngoodleps<1) return;
  // if(ngoodleps!=1) return;
  //  if(nVetoedLeptons>0) return;
  if(ngoodleps<2 && nVetoedLeptons>0){
    lep2_pdgid=-vetoedLeptons.at(0)->pdgid()*vetoedLeptons.at(0)->q();
    lep2_pt=vetoedLeptons.at(0)->pt();
    lep2_eta=vetoedLeptons.at(0)->eta();
    lep2_phi=vetoedLeptons.at(0)->phi();
    lep2_mass=vetoedLeptons.at(0)->mass(); 
    lep2_passVeto=kTRUE;
    lep2_d0=vetoedLeptons.at(0)->d0();
    lep2_dz=vetoedLeptons.at(0)->dz();
    lep2_MiniIso=vetoedLeptons.at(0)->miniiso();
  }

  ngoodjets = nJets;
  ngoodbtags = nBJets;
  nvetoleps = nVetoedLeptons;
  pfmet = met->pt();
  pfmet_phi = met->phi();
  run2=run;
  ls=lumi;
  evt=event;
  ak4pfjets_rho=rho;

  bool PassTrackVeto=kTRUE;

  int nvetotracks=0;
  int nvetotracks10=0;
  for(uint iT =0; iT < vetoedTracks.size(); ++iT){
    if(fabs(vetoedTracks.at(iT)->dz())>0.1) continue;
    if(PhysicsUtilities::deltaR(vetoedTracks.at(iT)->p4(),lep->p4())<0.4) continue;
    if(vetoedTracks.at(iT)->trackiso()/vetoedTracks.at(iT)->pt()>0.2) continue;
    if(vetoedTracks.at(iT)->pt()>5 && (fabs(vetoedTracks.at(iT)->pdgid())==11 || fabs(vetoedTracks.at(iT)->pdgid())==13)){
      nvetotracks++;
    }
    else if(vetoedTracks.at(iT)->pt()>10 && vetoedTracks.at(iT)->charge()*lep1_pdgid>0 && vetoedTracks.at(iT)->trackiso()/vetoedTracks.at(iT)->pt()<0.1){
      nvetotracks10++;
    }
  }
  if(nvetotracks+nvetotracks10>0)   PassTrackVeto=kFALSE;

  ak4pfjets_pt.clear();
  ak4pfjets_eta.clear();
  ak4pfjets_phi.clear();
  ak4pfjets_mass.clear();
  ak4pfjets_CSV.clear();
  ak4pfjets_loose_pfid.clear();
  ak4pfjets_puid.clear();
  dphi_ak4pfjets_met.clear();
  MomentumF* W = new MomentumF(lep->p4() + met->p4());
  dphi_Wlep = PhysicsUtilities::deltaPhi(*lep, *W);
  ak4_HT = JetKinematics::ht(jets,30,2.4);
//  ak4_htssm = JetKinematics::htAlong(*met,jets,30,2.4);
//  ak4_htosm = JetKinematics::htAway(*met,jets,30,2.4);

  vector<LorentzVector> SelJets;
  vector<bool> btag;
  vector<float> bvalue;
  vector <float> sigma;
  std::vector<std::pair<double,int>> rankedJets;
  LorentzVector jet,closestb,leadb;
  double highestcsv=0;
  double closest=100;
 
  for(unsigned int iJ =0; iJ < jets.size(); ++iJ){
    jet=jets[iJ]->p4();
    double bjet=false;
    if(jets[iJ]->csv()>0.814){
      bjet=true;
      if(PhysicsUtilities::deltaR(jet,lep->p4())<closest){
	closest=PhysicsUtilities::deltaR(jet,lep->p4());
	closestb=jet;
      }
    }
    if(jets[iJ]->csv()>highestcsv){
      highestcsv=jets[iJ]->csv();
      leadb=jet;
    }
    std::pair<double,int> rankedJet(fabs(PhysicsUtilities::deltaPhi(*lep,*jets[iJ])),iJ);
    rankedJets.push_back(rankedJet);
    SelJets.push_back(jet);
    btag.push_back(bjet);
    bvalue.push_back(jets[iJ]->csv());
    sigma.push_back(0.1);
    ak4pfjets_pt.push_back(jets[iJ]->pt());
    ak4pfjets_eta.push_back(jets[iJ]->eta());
    ak4pfjets_phi.push_back(jets[iJ]->phi());
    ak4pfjets_mass.push_back(jets[iJ]->mass());
    ak4pfjets_CSV.push_back(jets[iJ]->csv());
    dphi_ak4pfjets_met.push_back(JetKinematics::absDPhiMETJ(*met,*jets[iJ]));
    ak4pfjets_loose_pfid.push_back(kTRUE);
    ak4pfjets_puid.push_back(kTRUE);
  }  
  dphi_1 = dphi_ak4pfjets_met.at(0);
  dphi_2 = dphi_ak4pfjets_met.at(1);
  std::sort(rankedJets.begin(),rankedJets.end(),pairCompare);
  MomentumF* top=new MomentumF(jets[rankedJets.at(0).second]->p4()+jets[rankedJets.at(1).second]->p4()+jets[rankedJets.at(2).second]->p4());
  Mjjj=top->mass();

  LorentzVector lepvec;
  lepvec=lep->p4();
  MT2W = calculateMT2w(SelJets, bvalue, lepvec, pfmet, pfmet_phi);
  hadronic_top_chi2= calculateChi2(SelJets, sigma, btag);
  MomentumF* lclosestb=new MomentumF(lep->p4()+closestb);
  Mlb_closestb=lclosestb->mass();
  MomentumF* lb_lead=new MomentumF(lep->p4()+leadb);
  Mlb_lead_bdiscr=lb_lead->mass();
  dR_lep_leadb=closest;

  HLT_SingleMu=1;
  HLT_SingleMu_eff=1;
  HLT_SingleEl=1;
  HLT_SingleEl_eff=1;
  pu_weight=1;
  lep_sf=1;
  btag_sf=1;
  mass_stop=0;
  mass_lsp=0;
  mass_chargino=0;
  sparms_names.clear();
  sparms_values.clear();
  genlepsfromtop=0;

  for (UInt_t i=0; i<genParts.size(); ++i) {
  const GenParticleF * genPartMom = 0; 
    if (genParts.at(i)->numberOfMothers()>0) { genPartMom = genParts.at(i)->mother(0); }
    if (( ((abs(genParts.at(i)->pdgId()) == 11) || (abs(genParts.at(i)->pdgId()) == 13) || (abs(genParts.at(i)->pdgId()) == 15)) && (abs(genPartMom->pdgId())==24))) genlepsfromtop++;
  }
  topness= tNess->findMinTopnessConfiguration(lep,jets,met,tNessInfo);                                                                                                                                                                   
  mt_met_lep=sqrt(2*pfmet*lep->Et()*(1-cos(lep->phi()-pfmet_phi)));

// KILL VETO LEPS
  if(nVetoedLeptons > 1) return; // no more than 1 lep passing veto
  if(nSelLeptons != 1) return; // want exactly 1 lep passing selection

// PRINTOUTS
/*
 if(fabs(lep1_pdgid)==11 && ngoodbtags>0 && ngoodjets>3 && pfmet>80) std::cout<<run<<" "<<ls<<" "<<event<<" "<<fixed<<setprecision(4)<<lep1_pt<<" "<<lep1_pdgid<<" "<<pfmet<<" "<<mt_met_lep<<" "<<lep1_passVeto<<" "<<ngoodjets<<" "<<ngoodbtags<<" "<<genlepsfromtop<<" "<<dphi_Wlep<<" "<<ak4_htssm<<" "<<Mlb_closestb<<" "<<Mjjj<<" "<<MT2W<<" "<<topness<<" "<<hadronic_top_chi2<<std::endl;
*/
 if(fabs(lep1_pdgid)==13 && ngoodbtags>0 && ngoodjets>3 && pfmet>80) std::cout<<run<<" "<<ls<<" "<<event<<" "<<fixed<<setprecision(4)<<lep1_pt<<" "<<lep1_pdgid<<" "<<pfmet<<" "<<mt_met_lep<<" "<<PassTrackVeto<<" "<<ngoodjets<<" "<<ngoodbtags<<" "<<genlepsfromtop<<" "<<dphi_Wlep<<" "<<ak4_htssm<<" "<<Mlb_closestb<<" "<< 0. <<" "<<MT2W<<" "<<topness<<" "<<hadronic_top_chi2<<"\n";


  delete W;
  delete top;
  delete lclosestb;
  delete lb_lead;
  outtree->Fill();
}

// Process file belonging to specified sample with a given cross section
void make1lbabies(TString sname = "T2tt_650_325",               // sample name
		       const int fileindex = -1,             // index of file (-1 means there is only 1 file for this sample)
		       const bool isMC = true,               // data or MC
		       const TString fname = "T2tt_650_325.root", // path of file to be processed
		       const double xsec =  1,              // cross section to be used with this file in fb
		       const TString outputdir = "output",    // directory to which files with histograms will be written
		       const TString fileprefix = "file:/afs/cern.ch/work/p/peveraer/") // prefix for file name, needed e.g. to access files with xrootd
{

  printf("Processing file %d of %s sample\n", (fileindex > -1 ? fileindex : 0), sname.Data());

  // Make sure the output has a unique name in case there are multiple files to process
  if(fileindex > -1)
    sname += TString::Format("_%d",fileindex);

  if(isMC)
    printf("Cross section: %5.2f pb\n", xsec);

  TString fullname = fileprefix+fname;
  //  TString fullname = sname;

/*
  BaseTreeAnalyzer::ConfigPars pars;
  pars.cleanJetsvSelectedLeptons_ = true;
  //  pars.leptonSelection = BaseTreeAnalyzer::SEL_ALL_LEP;
  pars.defaultJetCollection = BaseTreeAnalyzer::AK4JETS; //PICKYJETS AK4JETS
*/

// load one lepton default configs
  cfgSet::loadDefaultConfigurations();
  cfgSet::ConfigSet cfg = cfgSet::ol_search_set;

  // Declare analyzer
  Analyzer a(fullname, "Events", fileindex+2, isMC, &cfg, xsec, sname, outputdir);
  //     a.analyze(1000,1000);
       a.analyze(100000);

}
