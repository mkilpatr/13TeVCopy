#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/macros/0LepSearchRegions/ZeroLeptonTreeHelper.hh"
#endif

using namespace ucsbsusy;

class Analyzer : public BaseTreeAnalyzer {

  public :

  Analyzer(TString fileName, TString treeName, bool isMCTree, double xSec, TString sname, TString outputdir,cfgSet::ConfigSet *pars) :
    BaseTreeAnalyzer(fileName, treeName, isMCTree, pars),xsec_(xSec), sname_(sname), outputdir_(outputdir) {

    // initialize tree
    gSystem->mkdir(outputdir,true);
    fout = new TFile (outputdir+"/"+sname+"_tree.root","RECREATE");
    fout->cd();
    outtree = new TTree("events","analysis tree");
    outtree->Branch("ScaleFactor",&ScaleFactor,"ScaleFactor/F");
    outtree->Branch("NPV",&NPV,"NPV/I");
    outtree->Branch("NJets",&NJets,"NJets/I");
    outtree->Branch("NBJets",&NBJets,"NBJets/I");
    outtree->Branch("NVetoedTau",&NVetoedTau,"NVetoedTau/I");
    outtree->Branch("MET",&MET,"MET/F");
    outtree->Branch("HT",&HT,"HT/F");
    outtree->Branch("DphiJ3MET",&DphiJ3MET,"DphiJ3MET/F");
    outtree->Branch("DphiJ12MET",&DphiJ12MET,"DphiJ12MET/F");
    outtree->Branch("MinDphiB1B2MET",&MinDphiB1B2MET,"MinDphiB1B2MET/F");
    outtree->Branch("MinMTB1B2MET",&MinMTB1B2MET,"MinMTB1B2MET/F");

    // Gen variables
    outtree->Branch("NGenLep",&NGenLep,"NGenLep/I");
    outtree->Branch("NGenEl",&NGenEl,"NGenEl/I");
    outtree->Branch("NGenMu",&NGenMu,"NGenMu/I");
    outtree->Branch("NGenTa",&NGenTa,"NGenTa/I");

    // Control variables in 0 Lep data
    // various cuts and whatnot
    outtree->Branch("NCTT",&NCTT,"NCTT/I");
    outtree->Branch("NCTTstd",&NCTTstd,"NCTTstd/I");
/*    outtree->Branch("NSelLeptons",&NSelLeptons,"NSelLeptons/I");
    outtree->Branch("NTops",&NTops,"NTops/I");
    outtree->Branch("NSelBJets",&NSelBJets,"NSelBJets/I");

    //MET cuts
    outtree->Branch("MET175_225",&MET175_225,"MET175_225/I");
    outtree->Branch("MET200_300",&MET200_300,"MET200_300/I");
    outtree->Branch("MET300_400",&MET300_400,"MET300_400/I");
    outtree->Branch("MET400_500",&MET400_500,"MET400_500/I");
    outtree->Branch("MET500_600",&MET500_600,"MET500_600/I");
    outtree->Branch("MET600_inf",&MET600_inf,"MET600_inf/I");

    //Number of jets cuts
    outtree->Branch("NJets2",&NJets2,"NJets2/I");
    outtree->Branch("NJets3",&NJets3,"NJest3/I");
    outtree->Branch("NJets4",&NJets4,"NJest4/I");
    outtree->Branch("NJets5_inf",&NJets5_inf,"NJest5_inf/I");

    //Nt, Nb cuts
    outtree->Branch("Nt0",&Nt0,"Nt0/I");
    outtree->Branch("Nt1_inf",&Nt1_inf,"Nt1_inf/I");
    outtree->Branch("Nb1",&Nb1,"Nb1/I");
    outtree->Branch("Nb2_inf",&Nb2_inf,"Nb2_inf/I");
*/

    }

  virtual ~Analyzer() {
    fout->cd();
    outtree->Write();
    fout->Close();
  }

  
  const double xsec_;
  const double lumi_         = 1000.0; // in /pb
  const double metcut_       = 150.0;
  const int    minNSelJets_  = 1;
  const int    minNSelBjets_ = 1;
  const int    maxNSelTaus_  = 0;
  const int    nSelLeptons_  = 1;
  const TString sname_       = "testLoukas.root";
  const TString outputdir_   = "./plots/";



  TFile                 *fout;
  TTree                 *outtree;

  float ScaleFactor;
  int NPV;
  int NJets;
  int NBJets;
  int NVetoedTau;
  float MET;
  float HT;
  float DphiJ3MET;
  float DphiJ12MET;
  float MinDphiB1B2MET;
  float MinMTB1B2MET;
  int   NGenLep;
  int   NGenEl;
  int   NGenMu;
  int   NGenTa;
  int   NCTT;
  int   NCTTstd;
  int   MET175_225;
  int   MET200_300;
  int   MET300_400;
  int   MET400_500;
  int   MET500_600;
  int   MET600_inf;
  int   NJets2;
  int   NJets3;
  int   NJets4;
  int   NJets5_inf;
  int   Nt0;
  int   Nt1_inf;
  int   Nb1;
  int   Nb2_inf;

  float DphiTopMET[50];



  void rankedByCSV(vector<RecoJetF*>& inJets,vector<RecoJetF*>& outJets);
  //void loadVariables();
  void runEvent();


};


/*Load event variables:
void Analyzer::loadVariables(){
  load(cfgSet::EVTINFO);
  load(cfgSet::AK4JETS,JetReader::LOADRECO | JetReader::LOADJETSHAPE | JetReader::FILLOBJ);
  //load(cfgSet::PICKYJETS,JetReader::LOADRECO | JetReader::LOADJETSHAPE | JetReader::FILLOBJ);
  load(cfgSet::ELECTRONS);
  load(cfgSet::MUONS);
  load(cfgSet::PFCANDS);
  load(cfgSet::CMSTOPS);
}*/


// Analyze event and fill plots
void Analyzer::runEvent()
{

  if ( (nSelLeptons!=0) || (nVetoedLeptons>nSelLeptons) || (nJets<=1) || (nBJets<1) || (met->pt()<175)) return;

  float wgt = evtInfoReader.evtweight;

  // fill tree variables
  ScaleFactor = wgt;
  NPV         = nPV;
  NJets       = nJets;
  NBJets      = nBJets;

 // NVetoedTau  = nVetoedTaus;
  MET         = met->pt();
  HT          = JetKinematics::ht(jets,40,2.4);
  DphiJ3MET   = JetKinematics::absDPhiMETJ3(*met,jets);
  DphiJ12MET  = JetKinematics::absDPhiMETJ12(*met,jets);

  // sort the jets in decreasing csv
  vector<RecoJetF*> jetsCSV;
  rankedByCSV(jets,jetsCSV);


  float DphiB1MET = PhysicsUtilities::deltaPhi(jetsCSV.at(0)->p4(),*met);
  float DphiB2MET = PhysicsUtilities::deltaPhi(jetsCSV.at(1)->p4(),*met);
  if (DphiB1MET<=DphiB2MET) { MinDphiB1B2MET = DphiB1MET; }              
  else                      { MinDphiB1B2MET = DphiB2MET; }              

  float MTb1MET   = JetKinematics::transverseMass(jetsCSV.at(0)->p4(),*met);
  float MTb2MET   = JetKinematics::transverseMass(jetsCSV.at(1)->p4(),*met);
  if (MTb1MET<=MTb2MET) { MinMTB1B2MET = MTb1MET; }
  else                  { MinMTB1B2MET = MTb2MET; }

 int NGenLep_ = 0; int NGenEl_ = 0; int NGenMu_ = 0; int NGenTa_ = 0;
  for (UInt_t i=0; i<genParts.size(); ++i) {

    const GenParticleF * genPartMom = 0;
    if (genParts.at(i)->numberOfMothers()>0) { genPartMom = genParts.at(i)->mother(0); } else { continue; }

    // fill the GenLep multiplicities
    if ( (abs(genParts.at(i)->pdgId()) == 11) && (abs(genPartMom->pdgId())==24) ) { ++NGenLep_; ++NGenEl_; }
    if ( (abs(genParts.at(i)->pdgId()) == 13) && (abs(genPartMom->pdgId())==24) ) { ++NGenLep_; ++NGenMu_; }
    if ( (abs(genParts.at(i)->pdgId()) == 15) && (abs(genPartMom->pdgId())==24) ) { ++NGenLep_; ++NGenTa_; }

  }

  NGenLep = NGenLep_;
  NGenEl  = NGenEl_;
  NGenMu  = NGenMu_;
  NGenTa  = NGenTa_;


  //
  NCTT = cttTops.size();
  NCTTstd = 0;

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
  for(unsigned int iJ =0; iJ < inJets.size(); ++iJ){ outJets[iJ] = inJets[rankedJets[iJ].second]; }

}





void makeZeroLeptonLowMETCRTrees(TString sname = "T2tt_850_100",
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
  cfgSet::ConfigSet pars = pars0lep();

  ZeroLeptonAnalyzer a(fullname, "TestAnalyzer/Events", outfilename, isMC, &pars);

  a.analyze(10000,100000); //test
  
  //a.analyze(100000);  //run

}
