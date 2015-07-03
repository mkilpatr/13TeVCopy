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
void processZeroLepton1LepIgnoredCS(TString sname            = "test",         // sample name
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
  cfgSet::ConfigSet cfg = cfgSet::zl_lepton_set;
  cfg.vetoedLeptons.selectedMuon = (&MuonF::ismultiisovetomuonl);
  cfg.vetoedLeptons.selectedElectron = (&ElectronF::ismultiisovetoelectronl);
  cfg.jets.cleanJetsvSelectedLeptons = true;

 // cfg.jets.cleanJetsvSelectedLeptons = true;
  // Declare analyzer
  Analyzer a(fullname, "Events", isMC, &cfg, xsec, sname, outputdir);//declare analyzer
  //  a.analyze(100000, 100000);
  a.analyze(100000);

}
