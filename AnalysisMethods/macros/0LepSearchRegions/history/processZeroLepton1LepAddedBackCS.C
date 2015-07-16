#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TSystem.h>
#include <TFile.h>
#include <TH1F.h>
#include "AnalysisMethods/PlotUtils/interface/Sample.hh"
#include "AnalysisMethods/PlotUtils/interface/PlotTools.hh"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisTools/DataFormats/interface/GenParticle.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#endif

using namespace ucsbsusy;


class Analyzer : public BaseTreeAnalyzer {

  public :

	 Analyzer(TString fileName, TString treeName, bool isMCTree, cfgSet::ConfigSet *pars, double xSec, TString sname, TString outputdir) :
	    BaseTreeAnalyzer(fileName, treeName, isMCTree, pars){

    // initiliaze tree
    gSystem->mkdir(outputdir,true);
    fout = new TFile (outputdir+"/"+sname+"_tree.root","RECREATE");
    fout->cd();
    outtree = new TTree("events","analysis tree");
    outtree->Branch("ScaleFactor"    ,&ScaleFactor,    "ScaleFactor/F"    );
    outtree->Branch("NPV"            ,&NPV,            "NPV/I"            );
    outtree->Branch("NJets"          ,&NJets,          "NJets/I"          );
    outtree->Branch("NRemovedJets"   ,&NRemovedJets,   "NRemovedJets/I"   );
    outtree->Branch("NBJets"         ,&NBJets,         "NBJets/I"         );
    outtree->Branch("NVetoedTau"     ,&NVetoedTau,     "NVetoedTau/I"     );
    outtree->Branch("MET"            ,&MET,            "MET/F"            );
    outtree->Branch("DphiJ3MET"      ,&DphiJ3MET,      "DphiJ3MET/F"      );
    outtree->Branch("DphiJ12MET"     ,&DphiJ12MET,     "DphiJ12MET/F"     );
    outtree->Branch("MinDphiB1B2MET" ,&MinDphiB1B2MET, "MinDphiB1B2MET/F" );
    outtree->Branch("MinMTB1B2MET"   ,&MinMTB1B2MET,   "MinMTB1B2MET/F"   );
    outtree->Branch("MLepCloseJet"   ,&MLepCloseJet,   "MLepCloseJet/F"   );
    outtree->Branch("MTLepCloseJet"  ,&MTLepCloseJet,  "MTLepCloseJet/F"  );
    outtree->Branch("DRCSVLLep"      ,&DRCSVLLep,      "DRCSVLLep/F"      );
    outtree->Branch("DRCSVMLep"      ,&DRCSVMLep,      "DRCSVMLep/F"      );
    outtree->Branch("DRCSVTLep"      ,&DRCSVTLep,      "DRCSVTLep/F"      );
    //outtree->Branch("METn"           ,&METn,           "METn/F"           );
    outtree->Branch("DphiLepW"       ,&DphiLepW,       "DphiLepW/F"       );
   // outtree->Branch("DphiJ3METn"     ,&DphiJ3METn,     "DphiJ3METn/F"     );
   // outtree->Branch("DphiJ12METn"    ,&DphiJ12METn,    "DphiJ12METn/F"    );
   // outtree->Branch("MinDphiB1B2METn",&MinDphiB1B2METn,"MinDphiB1B2METn/F");
   // outtree->Branch("MinMTB1B2METn"  ,&MinMTB1B2METn,  "MinMTB1B2METn/F"  );

    // top variables
    outtree->Branch("NCTT"      ,&NCTT     ,"NCTT/I"    );
    outtree->Branch("NCTTb"     ,&NCTTb    ,"NCTTb/I"   );
    outtree->Branch("NCTTd"     ,&NCTTd    ,"NCTTd/I"   );
    outtree->Branch("NCTTstd"   ,&NCTTstd  ,"NCTTstd/I" );
    outtree->Branch("NCTTstdb"  ,&NCTTstdb ,"NCTTstdb/I");
    outtree->Branch("NCTTstdc"  ,&NCTTstdc ,"NCTTstdc/I");
    outtree->Branch("NCTTstdd"  ,&NCTTstdd ,"NCTTstdd/I");
    outtree->Branch("NCTTstde"  ,&NCTTstde ,"NCTTstde/I");

    // NCTTstd specific
    outtree->Branch("DRTopLep"     ,DRTopLep     ,"DRTopLep[NCTTstd]/F"     );
    outtree->Branch("DphiTopMET"   ,DphiTopMET   ,"DphiTopMET[NCTTstd]/F"   );
    outtree->Branch("DphiTopLep"   ,DphiTopLep   ,"DphiTopLep[NCTTstd]/F"   );
    outtree->Branch("PtLepOvTop"   ,PtLepOvTop   ,"PtLepOvTop[NCTTstd]/F"   );
    outtree->Branch("METOvPtTop"   ,METOvPtTop   ,"METOvPtTop[NCTTstd]/F"   );
    outtree->Branch("DRRemJetTop"  ,DRRemJetTop  ,"DRRemJetTop[NCTTstd]/F"  );
    outtree->Branch("DphiRemJetTop",DphiRemJetTop,"DphiRemJetTop[NCTTstd]/F");
    outtree->Branch("PtRemJetOvTop",PtRemJetOvTop,"PtRemJetOvTop[NCTTstd]/F");

    outtree->Branch("DRTop1Lep"  ,&DRTop1Lep  ,"DRTop1Lep/F"  );
    outtree->Branch("PtLepOvTop1",&PtLepOvTop1,"PtLepOvTop1/F");
    outtree->Branch("DRTop2Lep"  ,&DRTop2Lep  ,"DRTop2Lep/F"  );
    outtree->Branch("PtLepOvTop2",&PtLepOvTop2,"PtLepOvTop2/F");
    outtree->Branch("DRB1Lep"    ,&DRB1Lep    ,"DRB1Lep/F"    );
    outtree->Branch("PtLepOvB1"  ,&PtLepOvB1  ,"PtLepOvB1/F"  );
    outtree->Branch("DRB2Lep"    ,&DRB2Lep    ,"DRB2Lep/F"    );
    outtree->Branch("PtLepOvB2"  ,&PtLepOvB2  ,"PtLepOvB2/F"  );

    // Gen variables
    outtree->Branch("NGenLep",&NGenLep,"NGenLep/I");
    outtree->Branch("NGenEl" ,&NGenEl ,"NGenEl/I"   );
    outtree->Branch("NGenMu" ,&NGenMu ,"NGenMu/I"   );
    outtree->Branch("NGenTa" ,&NGenTa ,"NGenTa/I"   );

  }

  virtual ~Analyzer() {
    fout->cd();
    outtree->Write();
    fout->Close();
  }

  const double metcut_       = 150.0;
  const int    minNSelJets_  = 4;
  const int    minNSelBjets_ = 1;
  const int    maxNSelTaus_  = 0;
  const int    nSelLeptons_  = 1;
  const TString sname_       = "testLoukas.root";
  const TString outputdir_   = "./plots/";


  TFile              *fout;
  TTree              *outtree;

  float ScaleFactor;
  int   NPV;
  int   NJets;
  int   NBJets;
  int   NRemovedJets;
  int   NVetoedTau;
 // float METn;
 // float DphiJ3METn;
 //  float DphiJ12METn;
 // float MinDphiB1B2METn;
 // float MinMTB1B2METn;
  float MET;
  float DphiLepW;
  float DphiJ3MET;
  float DphiJ12MET;
  float MinDphiB1B2MET;
  float MinMTB1B2MET;
  float MLepCloseJet;
  float MTLepCloseJet;
  float DRCSVLLep;
  float DRCSVMLep;
  float DRCSVTLep;
  int   NGenLep;
  int   NGenEl;
  int   NGenMu;
  int   NGenTa;
  int   NCTT;
  int   NCTTb;
  int   NCTTd;
  int   NCTTstd;
  int   NCTTstdb;
  int   NCTTstdc;
  int   NCTTstdd;
  int   NCTTstde;
  float DRTopLep[50];
  float DphiTopMET[50];
  float DphiTopLep[50];
  float PtLepOvTop[50];
  float METOvPtTop[50];
  float DRRemJetTop[50];
  float DphiRemJetTop[50];
  float PtRemJetOvTop[50];

  float DRTop1Lep;
  float PtLepOvTop1;
  float DRTop2Lep;
  float PtLepOvTop2;
  float DRB1Lep;
  float PtLepOvB1;
  float DRB2Lep;
  float PtLepOvB2;


  void cttAlgo(float fjMass,float minMass,int nSubJets,float tau1,float tau2,float tau3,
	       vector<RecoJetF*> bJets,CMSTopF* fatJet,vector<bool>& cttDef);
  void rankedByCSV(vector<RecoJetF*>& inJets,vector<RecoJetF*>& outJets);
  void findClosestJet2Lep(vector<RecoJetF*>& inJets,MomentumF* lep,RecoJetF* &closestJet);
  void findClosestJet2Top(vector<RecoJetF*>& inJets,CMSTopF* fatJet,RecoJetF* &closestJet);
  void runEvent();
  void loadVariables();


};

//Load event variables:
void Analyzer::loadVariables(){
  load(cfgSet::EVTINFO);
  load(cfgSet::AK4JETS,JetReader::LOADRECO | JetReader::LOADJETSHAPE | JetReader::FILLOBJ);
  load(cfgSet::PICKYJETS,JetReader::LOADRECO | JetReader::LOADJETSHAPE | JetReader::FILLOBJ);
  load(cfgSet::ELECTRONS);
  load(cfgSet::MUONS);
  load(cfgSet::PFCANDS);
  load(cfgSet::CMSTOPS);
}

// Analyze event and fill plots
void Analyzer::runEvent()
{

  //  if ( (nSelLeptons!=1) || (nVetoedLeptons>nSelLeptons) || (nJets<=3)) return;
  if ( (nSelLeptons==0) || (nVetoedLeptons<nSelLeptons) || (nJets<=3)) return;

  float wgt    = evtInfoReader.weight;

  ScaleFactor  = wgt;
  NPV          = nPV;
  NJets        = nJets;
  NBJets       = nBJets;
  //NRemovedJets = nRemovedJets; (This seems to be missing from the current framework)
  NVetoedTau   = nVetoedTracks;

  TRandom3 rnd(0);
  float maxLep = nSelLeptons;
  int whichLep = rnd.Uniform(0.,nSelLeptons);
  MomentumF* lep = new MomentumF(selectedLeptons.at(whichLep)->p4());


  std::vector<RecoJetF*> jetsCSV;
  rankedByCSV(jets,jetsCSV);

  RecoJetF* closestJet2Lep;
  findClosestJet2Lep(jets,lep,closestJet2Lep);
  if (closestJet2Lep->csv()>0.423) { DRCSVLLep = PhysicsUtilities::deltaR(closestJet2Lep->p4(),lep->p4()); } else { DRCSVLLep = 99.; }
  if (closestJet2Lep->csv()>0.814) { DRCSVMLep = PhysicsUtilities::deltaR(closestJet2Lep->p4(),lep->p4()); } else { DRCSVMLep = 99.; }
  if (closestJet2Lep->csv()>0.941) { DRCSVTLep = PhysicsUtilities::deltaR(closestJet2Lep->p4(),lep->p4()); } else { DRCSVTLep = 99.; }

  DRB1Lep   = PhysicsUtilities::deltaR(jetsCSV.at(0)->p4(),lep->p4());
  PtLepOvB1 = lep->p4().pt()/jetsCSV.at(0)->p4().pt();
  DRB2Lep   = PhysicsUtilities::deltaR(jetsCSV.at(1)->p4(),lep->p4());
  PtLepOvB2 = lep->p4().pt()/jetsCSV.at(1)->p4().pt();


  MomentumF* W = new MomentumF(lep->p4() + met->p4());
  DphiLepW = PhysicsUtilities::deltaPhi(*lep, *W);


  // uncoreected variables
  MomentumF* metn = new MomentumF(met->p4() + lep->p4());

  MET           = metn->pt();
  DphiJ3MET     = JetKinematics::absDPhiMETJ3(*metn,jets);
  DphiJ12MET    = JetKinematics::absDPhiMETJ12(*metn,jets);
  MLepCloseJet  = (closestJet2Lep->p4()+lep->p4()).M();
  MTLepCloseJet = JetKinematics::transverseMass(closestJet2Lep->p4(),lep->p4());

  float DphiB1MET = PhysicsUtilities::deltaPhi(jetsCSV.at(0)->p4(),*metn);
  float DphiB2MET = PhysicsUtilities::deltaPhi(jetsCSV.at(1)->p4(),*metn);
  if (DphiB1MET<=DphiB2MET) { MinDphiB1B2MET = DphiB1MET; }
  else                      { MinDphiB1B2MET = DphiB2MET; }

  float MTb1MET   = JetKinematics::transverseMass(jetsCSV.at(0)->p4(),*metn);
  float MTb2MET   = JetKinematics::transverseMass(jetsCSV.at(1)->p4(),*metn);
  if (MTb1MET<=MTb2MET) { MinMTB1B2MET = MTb1MET; }
  else                  { MinMTB1B2MET = MTb2MET; }
  // ===

/*
  // corected variables

  MomentumF* metn = new MomentumF(met->p4() + lep->p4());
  METn         = metn->pt();

  DphiJ3METn   = JetKinematics::absDPhiMETJ3(*metn,jets);
  DphiJ12METn  = JetKinematics::absDPhiMETJ12(*metn,jets);

  float DphiB1METn = PhysicsUtilities::deltaPhi(jetsCSV.at(0)->p4(),*metn);
  float DphiB2METn = PhysicsUtilities::deltaPhi(jetsCSV.at(1)->p4(),*metn);
  if (DphiB1METn<=DphiB2METn) { MinDphiB1B2METn = DphiB1METn; }
  else                        { MinDphiB1B2METn = DphiB2METn; }

  float MTb1METn   = JetKinematics::transverseMass(jetsCSV.at(0)->p4(),*metn);
  float MTb2METn   = JetKinematics::transverseMass(jetsCSV.at(1)->p4(),*metn);
  if (MTb1METn<=MTb2METn) { MinMTB1B2METn = MTb1METn; }
  else                    { MinMTB1B2METn = MTb2METn; }
*/  // ===


  // top tagging
  DRTop1Lep = -9.; PtLepOvTop1 = -9.;
  DRTop2Lep = -9.; PtLepOvTop2 = -9.;

  NCTT     = cttTops.size();
  NCTTb    = 0;
  NCTTd    = 0;
  NCTTstd  = 0;
  NCTTstdb = 0;
  NCTTstdc = 0;
  NCTTstdd = 0;
  NCTTstde = 0;
  for (UInt_t i=0; i<cttTops.size(); ++i) {

    //    RecoJetF* closestJet2Top; cout << removedJets.size() << "\n";
    //    findClosestJet2Top(removedJets,cttTops.at(i),closestJet2Top);

    float DRTopLep_      = PhysicsUtilities::deltaR(cttTops.at(i)->p4(),lep->p4());
    float DphiTopLep_    = PhysicsUtilities::deltaPhi(cttTops.at(i)->p4(),lep->p4());
    //    float DRRemJetTop_   = PhysicsUtilities::deltaR(cttTops.at(i)->p4(),closestJet2Top->p4());
    //    float DphiRemJetTop_ = PhysicsUtilities::deltaPhi(cttTops.at(i)->p4(),closestJet2Top->p4());
    float DphiTopMET_    = PhysicsUtilities::deltaPhi(cttTops.at(i)->p4(),*met);
    float PtLepOvTop_    = (lep->p4().pt())/(cttTops.at(i)->p4().pt());
    float METOvPtTop_    = (met->p4().pt())/(cttTops.at(i)->p4().pt());
    //    float PtRemJetOvTop_ = (closestJet2Top->p4().pt())/(cttTops.at(i)->p4().pt());

    if (DRTopLep_>0.8)    { ++NCTTb; }
    //    if (DRRemJetTop_>0.8) { ++NCTTd; }


    std::vector<bool> cttDef;
    cttAlgo(cttTops.at(i)->fJMass(),cttTops.at(i)->minMass(),cttTops.at(i)->nSubJets(),
	    cttTops.at(i)->fJTau1(),cttTops.at(i)->fJTau2(),cttTops.at(i)->fJTau3(),
	    jetsCSV,cttTops.at(i),cttDef);

    if (cttDef[0]) {

    DRTopLep[NCTTstd]      = DRTopLep_;
    DphiTopLep[NCTTstd]    = DphiTopLep_;
    //    DRRemJetTop[NCTTstd]   = DRRemJetTop_;
    //    DphiRemJetTop[NCTTstd] = DphiRemJetTop_;
    DphiTopMET[NCTTstd]    = DphiTopMET_;
    PtLepOvTop[NCTTstd]    = PtLepOvTop_;
    METOvPtTop[NCTTstd]    = METOvPtTop_;
    //    PtRemJetOvTop[NCTTstd] = PtRemJetOvTop_;
    ++NCTTstd;

     if (DRTopLep_>0.8)                                                              { ++NCTTstdb; }
     //     if (DRRemJetTop_>0.8)                                                           { ++NCTTstdd; }
     //     if (DRRemJetTop_>0.8 || ((DRRemJetTop_<0.8) && (cttTops.at(i)->nSubJets()>3)) ) { ++NCTTstde; }

     if (i==0) {
       DRTop1Lep   = PhysicsUtilities::deltaR(cttTops.at(i)->p4(),lep->p4());
       PtLepOvTop1 = lep->p4().pt()/cttTops.at(i)->p4().pt(); }

     if (i==1) {
       DRTop2Lep   = PhysicsUtilities::deltaR(cttTops.at(i)->p4(),lep->p4());
       PtLepOvTop2 = lep->p4().pt()/cttTops.at(i)->p4().pt(); }

    }

  }
  // ====


  int NGenLep_ = 0; int NGenEl_ = 0; int NGenMu_ = 0; int NGenTa_ = 0;
  for (UInt_t i=0; i<genParts.size(); ++i) {

    const GenParticleF * genPartMom = 0;
    if (genParts.at(i)->numberOfMothers()>0) { genPartMom = genParts.at(i)->mother(0); } else { continue; }

    if ( (abs(genParts.at(i)->pdgId()) == 11) && (abs(genPartMom->pdgId())==24) ) { ++NGenLep_; ++NGenEl_; }
    if ( (abs(genParts.at(i)->pdgId()) == 13) && (abs(genPartMom->pdgId())==24) ) { ++NGenLep_; ++NGenMu_; }
    if ( (abs(genParts.at(i)->pdgId()) == 15) && (abs(genPartMom->pdgId())==24) ) { ++NGenLep_; ++NGenTa_; }

  }

  NGenLep = NGenLep_;
  NGenEl = NGenEl_;
  NGenMu = NGenMu_;
  NGenTa = NGenTa_;


  outtree->Fill();
}
void Analyzer::cttAlgo(float fjMass,float minMass,int nSubJets,float tau1,float tau2,float tau3,
		       vector<RecoJetF*> bJets,CMSTopF* fatJet,vector<bool>& cttDef) {

  bool stddef     = false;
  bool nsubjetdef = false;
  bool btagdef    = false;

  float dR1 = PhysicsUtilities::deltaR(fatJet->p4(),bJets.at(0)->p4());
  float dR2 = PhysicsUtilities::deltaR(fatJet->p4(),bJets.at(1)->p4());

  // std definiton
  if ( ((fjMass)>140.) && ((fjMass)<250.) && ((minMass)>50.) && ((nSubJets)>=3) ) { stddef     = true; }
  if ( ((tau3/tau2)<0.7) && ((tau2/tau1)>0.1) )                                   { nsubjetdef = true; }
  if ( (dR1<0.8) || (dR2<0.8) )                                                   { btagdef    = true; }


  cttDef.push_back(stddef);
  cttDef.push_back(stddef*nsubjetdef);
  cttDef.push_back(stddef*nsubjetdef*btagdef);

} // end of cttAlgo

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


void Analyzer::findClosestJet2Lep(vector<RecoJetF*>& inJets,MomentumF* lep,RecoJetF* &closestJet) {

  float drmin_ = 999.;
  for(unsigned int iJ =0; iJ < inJets.size(); ++iJ){

    float tmpDR_ = PhysicsUtilities::deltaR(lep->p4(),inJets[iJ]->p4());
    if (tmpDR_<drmin_) {
      drmin_=tmpDR_;
      closestJet = inJets[iJ]; }

  }

}


void Analyzer::findClosestJet2Top(vector<RecoJetF*>& inJets,CMSTopF* fatJet,RecoJetF* &closestJet) {

  float drmin_ = 999.;
  for(unsigned int iJ =0; iJ < inJets.size(); ++iJ){

    float tmpDR_ = PhysicsUtilities::deltaR(fatJet->p4(),inJets[iJ]->p4());
    if (tmpDR_<drmin_) {
      drmin_=tmpDR_;
      closestJet = inJets[iJ]; }

  }

}




// Process file belonging to specified sample with a given cross section
// Lepton is ignored in the event, this is the conceptually correct calculation
void processZeroLepton1LepAddedBackCS(TString sname            = "test",         // sample name
			     const int fileindex      = -1,             // index of file (-1 means there is only 1 file for this sample)
			     const bool isMC          = true,           // data or MC
			     const TString fname      = "evttree_numEvent500.root", // path of file to be processed
			     const double xsec        = 1.0,              // cross section to be used with this file
			     const TString outputdir  = "run/plots",    // directory to which files with histograms will be written
			     const TString fileprefix = "file:/afs/cern.ch/work/g/gouskos/private/UCSB_2015/CMSSW_7_2_0/src/AnalysisBase/Analyzer/test/") // prefix for file name, needed e.g. to access files with xrootd
{

  printf("Processing file %d of %s sample\n", (fileindex > -1 ? fileindex : 0), sname.Data());

  // Make sure the output has a unique name in case there are multiple files to process
  if(fileindex > -1)
    sname += TString::Format("_%d",fileindex);

  if(isMC)
    printf("Cross section: %5.2f pb\n", xsec);

  TString fullname = fileprefix+fname;

  cfgSet::loadDefaultConfigurations();
  cfgSet::ConfigSet cfg = cfgSet::ol_search_set;

  Analyzer a(fullname, "Events", isMC, &cfg, xsec, sname, outputdir);//declare analyzer
  //  a.analyze(100000, 100000);
  a.analyze(100000);

}
