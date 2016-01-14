#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TSystem.h>
#include <TFile.h>
#include <TH1F.h>
//#include "AnalysisMethods/PlotUtils/interface/Sample.hh"
//#include "AnalysisMethods/PlotUtils/interface/PlotTools.hh"
//#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
//#include "AnalysisTools/KinematicVariables/interface/Topness.h"
//#include "AnalysisTools/TreeReader/interface/ElectronReader.h"
#include "AnalysisMethods/macros/0LepSearchRegions/variableCalculator0L.h"
#include "AnalysisTools/Utilities/interface/JetFlavorInfo.h"


using namespace ucsbsusy;

class Analyzer : public BaseTreeAnalyzer {

  public :
  VariableCalculator0L vars;
  VariableCalculator0L caVars;
  int caGenNJ20;
  float caGenJ0pt;
  int caGenNB;
  bool isSingleE;
  bool isSingleMu;
  bool isSingleTau;

  Analyzer(TString fileName, TString treeName, bool isMCTree, ConfigPars * pars, double xSec, TString sname, TString outputdir)
          : BaseTreeAnalyzer(fileName, treeName, isMCTree, pars)
          , xsec_(xSec)
          , sample_(sname)
          , outputdir_(outputdir)
  {
    //loadPlots(); // initialize plots

    caGenNJ20 = 0;
    caGenJ0pt = 0;
    caGenNB   = 0;
    isSingleE   = false;
    isSingleMu  = false;
    isSingleTau = false;

    // initiliaze tree
    gSystem->mkdir(outputdir,true);
    fout = new TFile (outputdir+"/"+sname+"_tree.root","RECREATE");
    fout->cd();
    outtree = new TTree("events","analysis tree");
    outtree->Branch( "scaleFactor", &scaleFactor, "scaleFactor/F" );
    outtree->Branch( "npv"        , &npv        ,         "npv/I" );
    outtree->Branch( "nAK4pfJets" , &nAK4pfJets ,  "nAK4pfJets/I" );
    outtree->Branch( "nVetoTau"   , &nVetoTau   ,    "nVetoTau/I" );

    outtree->Branch( "ptMet"            , &vars.ptMet            ,            "ptMet/F" );
    outtree->Branch( "npv"              , &vars.npv              ,              "npv/I" );
    outtree->Branch( "nj60"             , &vars.nj60             ,             "nj60/I" );
    outtree->Branch( "nJ20"             , &vars.nJ20             ,             "nJ20/I" );
    outtree->Branch( "ntBtag"           , &vars.ntBtag           ,           "ntBtag/I" );
    outtree->Branch( "nmBtag"           , &vars.nmBtag           ,           "nmBtag/I" );
    outtree->Branch( "J0pt"             , &vars.J0pt             ,             "J0pt/F" );
    outtree->Branch( "ht"               , &vars.ht               ,               "ht/F" );
    outtree->Branch( "Bpt0"             , &vars.Bpt0             ,             "Bpt0/F" );
    outtree->Branch( "Bpt1"             , &vars.Bpt1             ,             "Bpt1/F" );
    outtree->Branch( "MT2tp0_000"       , &vars.MT2tp0_000       ,       "MT2tp0_000/F" );

    outtree->Branch( "ca_ptMet"            , &caVars.ptMet            ,            "ca_ptMet/F" );
    outtree->Branch( "ca_npv"              , &caVars.npv              ,              "ca_npv/I" );
    outtree->Branch( "ca_nj60"             , &caVars.nj60             ,             "ca_nj60/I" );
    outtree->Branch( "ca_nJ20"             , &caVars.nJ20             ,             "ca_nJ20/I" );
    outtree->Branch( "ca_ntBtag"           , &caVars.ntBtag           ,           "ca_ntBtag/I" );
    outtree->Branch( "ca_nmBtag"           , &caVars.nmBtag           ,           "ca_nmBtag/I" );
    outtree->Branch( "ca_J0pt"             , &caVars.J0pt             ,             "ca_J0pt/F" );
    outtree->Branch( "ca_ht"               , &caVars.ht               ,               "ca_ht/F" );
    outtree->Branch( "ca_Bpt0"             , &caVars.Bpt0             ,             "ca_Bpt0/F" );
    outtree->Branch( "ca_Bpt1"             , &caVars.Bpt1             ,             "ca_Bpt1/F" );
    outtree->Branch( "ca_MT2tp0_000"       , &caVars.MT2tp0_000       ,       "ca_MT2tp0_000/F" );

    outtree->Branch( "caGen_NJ20"        , &caGenNJ20       ,     "caGen_NJ20/I" );
    outtree->Branch( "caGen_J0pt"        , &caGenJ0pt       ,     "caGen_J0pt/F" );
    outtree->Branch( "caGen_NB"          , &caGenNB         ,       "caGen_NB/I" );

    outtree->Branch( "nVetoedLeptons"       , &nSecondaryLeptons       , "nVetoedLeptons/I" );
    outtree->Branch( "nVetoedTaus"          , &nVetoedTaus          ,    "nVetoedTaus/I" );
    outtree->Branch( "nSelLeptons"          , &nSelLeptons       ,       "nSelLeptons/I" );
    outtree->Branch( "isSingleE"        , &isSingleE      ,   "isSingleE/O" );
    outtree->Branch( "isSingleMu"       , &isSingleMu     ,  "isSingleMu/O" );
    outtree->Branch( "isSingleTau"      , &isSingleTau    , "isSingleTau/O" );

  } // Analyzer()

  virtual ~Analyzer() {
    fout->cd();
    outtree->Write();
    fout->Close();
  } // ~Analyzer()

  const double  metcut_    = 200.0 ;
  const int     minNJets_  =   3   ;
  const int     minNBjets_ =   0   ;
  const int     maxNTaus_  =   0   ;

  const double  lumi_      = 1000.0 ; // in /pb
  const double  xsec_      ;
  const TString sample_    ;
  const TString outputdir_ ;

  TFile *fout    ;
  TTree *outtree ;

  float scaleFactor ;
  int   npv         ;
  int   nAK4pfJets  ;
  int   nVetoTau    ;


  void loadVariables();
  void runEvent();
  void loadPlots();
  void out(TString outputName, TString outputPath);
  
  map<TString,TH1F*> plots;

}; // Analyzer : BaseTreeAnalyzer

// Define plots, names, and binning
void Analyzer::loadPlots() {
  string passJet = "N_{jets} >= 4, N_{bjets} >=1";
  string passMet = "MET > 200";
  string passPre = "MET > 200, N_{jets} >= 3, N_{bjets} >=1";
  string yTitle  = "Events";

  // partial presel plots
  plots["met_passJet" ] = new TH1F( "met_passJet" , (passJet+"; #slash{E}_{T} [GeV]; "+yTitle).c_str(), 100,  0  , 1000   );
  plots["nj60_passMet"] = new TH1F( "nj60_passMet", (passMet+"; Number of Jets; "     +yTitle).c_str(),  10, -0.5,   19.5 );
  plots["nJ20_passMet"] = new TH1F( "nJ20_passMet", (passMet+"; Number of B Jets; "   +yTitle).c_str(),  10, -0.5,   19.5 );
  // preselection plots
  plots["j1pt_passPre"]              = new TH1F("jet1pt_passPre"           , (passPre+"; Lead jet p_{T} [GeV]; "                  +yTitle).c_str(), 200,   0  , 1000    );

  // set Sumw2 for all histograms
  for(map<TString,TH1F*>::iterator plotsIt = plots.begin(); plotsIt != plots.end(); ++plotsIt) plotsIt->second->Sumw2();
} // loadPlots()

void Analyzer::loadVariables() {
  load(EVTINFO);
  load(AK4JETS, JetReader::LOADRECO | JetReader::LOADGEN | JetReader::LOADJETSHAPE | JetReader::FILLOBJ);
  load(PICKYJETS);
  load(CASUBJETS);
  load(ELECTRONS);
  load(MUONS);
  load(PFCANDS);
  load(CMSTOPS);
  if(isMC()) load(GENPARTICLES);
} // Analyzer::loadVariables()

// Analyze event and fill plots
void Analyzer::runEvent() {

  // preselection
  //if(nVetoedLeptons > 0)  return;
  if(met->pt() < metcut_) return;
  if(nJets<minNJets_)     return;
  if(nBJets < minNBjets_) return;
  //if(nVetoedTaus>0)       return;

  isSingleE   = false;
  isSingleMu  = false;
  isSingleTau = false;
  int Ne   = 0;
  int Nmu  = 0;
  int Ntau = 0;
  for(unsigned int i=0; i<genParts.size(); ++i) {
    int id = abs(genParts[i]->pdgId());
    if(id==11) ++Ne;
    if(id==13) ++Nmu;
    if(id==15) ++Ntau;
  }
  if (Ne==1 && Nmu==0 && Ntau==0) isSingleE   = true;
  if (Ne==0 && Nmu==1 && Ntau==0) isSingleMu  = true;
  if (Ne==0 && Nmu==0 && Ntau==1) isSingleTau = true;


  // fill tree variables [assmune lumi = 1 fb^(-1)]
  scaleFactor = weight; // lumi_*xsec_/getEntries(); <- old procedure, now stored in files to handle split samples correctly
  npv         = nPV;
  nAK4pfJets  = nJets;
  nVetoTau    = nVetoedTaus;

  vars.processVariables(this,&ak4Reader,jets,jets,met,cttTops); // calculate variables

  std::vector<RecoJetF*> caSubJets, caSubJetsB, caSubJetsNoB;
  cleanJets(&caSubJetReader,caSubJets,&caSubJetsB,&caSubJetsNoB);
  caVars.processVariables(this,&ak4Reader,jets,caSubJets,met,cttTops);

  caGenNJ20  =  0;
  caGenJ0pt  = -1;
  caGenNB    =  0;
  for(unsigned int i=0; i< caSubJetReader.genJets.size(); ++i) {
    auto& j = caSubJetReader.genJets[i];
    if (j.pt()<20)   continue;
    if (j.eta()>2.4) continue;
    if (caGenJ0pt<0) caGenJ0pt = j.pt();
    ++caGenNJ20;
    if (j.flavor() == JetFlavorInfo::b_jet) ++caGenNB;
  }

  outtree->Fill();

  return;

} // Analyzer::runEvent()

// Write histograms to file in designated directory
void Analyzer::out(TString outputName, TString outputPath) {


  gSystem->mkdir(outputPath,true);
  TString filename = outputPath + "/" + outputName + "_plots.root";
  TFile  *outfile  = new TFile(filename, "RECREATE");
  outfile->cd();

  for(map<TString,TH1F*>::iterator plotsIt = plots.begin(); plotsIt != plots.end(); ++plotsIt) plotsIt->second->Write();
  outfile->Write();
  outfile->Close();
  delete outfile;
} // Analyzer::out()

// get the x-section for the given sample
double getXsec(string sample) {
  if (sample=="ttbar") return 831.76   ;
  if (sample=="ttZ")   return    .7598 ;
  if (sample=="znunu_ht100to200") return 372.6   ;
  if (sample=="znunu_ht200to400") return 100.8   ;
  if (sample=="znunu_ht400to600") return  11.99  ;
  if (sample=="znunu_ht600toinf") return   4.113 ;
  if (sample=="T2tt_425_325") return 1.31169   ;
  if (sample=="T2tt_500_325") return 0.51848   ;
  if (sample=="T2tt_650_325") return 0.107045  ;
  if (sample=="T2tt_850_100") return 0.0189612 ;
  cout << "WARNING: no cross section available for " << sample << "!" << endl
	     << "         setting xsec = 1" << endl;
  return 1;
}

#endif

// Process file belonging to specified sample with a given cross section
//root -b -q "../CMSSW_7_3_1/src/AnalysisMethods/macros/0LepSearchRegions/processZeroLeptonSlimmed.C+()"
void processZeroLeptonSlimmed(      TString sname      = "ttbar" // sample name
                     , const int     fileindex  = 1       // index of file (-1 means there is only 1 file for this sample)
                     , const bool    isMC       = true    // data or MC
                     , const TString fname      = "ttbar_1_ntuple_wgtxsec.root" // path of file to be processed
                     , const double  xsec       = 831.76    // cross section to be used with this file
                     , const string  outputdir  = "plots/testing/"  // directory to which files with histograms will be written
                     , const TString fileprefix = "/eos/uscms/store/user/vdutta/13TeV/080615/merged/"
                     )
{
  printf("Processing file %d of %s sample\n", (fileindex > -1 ? fileindex : 0), sname.Data());

  // Make sure the output has a unique name in case there are multiple files to process
  if(fileindex > -1) sname += TString::Format("_%d",fileindex);

  if(isMC) printf("Cross section: %5.2f pb\n", xsec);

  TString fullname = fileprefix+fname;

  // Adjustments to default configuration
  BaseTreeAnalyzer::ConfigPars pars;
  pars.defaultJetCollection = BaseTreeAnalyzer::AK4JETS; // BaseTreeAnalyzer::PICKYJETS;
  pars.minJetPt = 20;
  // vetoed leptons     isgoodpogelectron     ismultiisovetoelectronl     ismultiisovetoelectronvl
  pars.minVetoEPt = 5;
  pars.vetoedElectron = &ElectronF::ismultiisovetoelectronl;
  pars.minVetoMuPt = 5;
  pars.vetoedMuon = &MuonF::ismultiisovetomuonl;

  Analyzer a(fullname, "Events", isMC, &pars, xsec, sname, outputdir); // declare analyzer
  //a.analyze(10000); // run: Argument is frequency of printout
  a.analyze(1000,100000); // for testing
  //a.out(sname, outputdir); // write outputfile with plots

} // processSingleLepton()
