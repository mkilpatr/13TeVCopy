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
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"


using namespace ucsbsusy;

class Analyzer : public BaseTreeAnalyzer {

  public :
  VariableCalculator0L vars;

  Analyzer(TString fileName, TString treeName, bool isMCTree, cfgSet::ConfigSet *pars, double xSec, TString sname, TString outputdir) :
    BaseTreeAnalyzer(fileName, treeName, isMCTree, pars)
  {
    //loadPlots(); // initialize plots

    // initiliaze tree
	  gSystem->mkdir(outputdir,true);
	  fout = new TFile (outputdir+"/"+sname+"_tree.root","RECREATE");
	  fout->cd();

    outtree = new TTree("events","analysis tree");
    outtree->Branch( "scaleFactor", &scaleFactor, "scaleFactor/F" );
    outtree->Branch( "npv"        , &npv        ,         "npv/I" );
    outtree->Branch( "nAK4pfJets" , &nAK4pfJets ,  "nAK4pfJets/I" );
    outtree->Branch( "nVetoTau"   , &nVetoTau   ,    "nVetoTau/I" );
    outtree->Branch( "nSelLeptons", &nSelLeptons, "nSelLeptons/I" );

    outtree->Branch( "ptMet"            , &vars.ptMet            ,            "ptMet/F" );
    outtree->Branch( "npv"              , &vars.npv              ,              "npv/I" );
    outtree->Branch( "nj60"             , &vars.nj60             ,             "nj60/I" );
    outtree->Branch( "nJ20"             , &vars.nJ20             ,             "nJ20/I" );
    outtree->Branch( "ntBtag"           , &vars.ntBtag           ,           "ntBtag/I" );
    outtree->Branch( "nmBtag"           , &vars.nmBtag           ,           "nmBtag/I" );
    outtree->Branch( "dPhiMET12"        , &vars.dPhiMET12        ,        "dPhiMET12/F" );
    outtree->Branch( "dPhiMET3"         , &vars.dPhiMET3         ,         "dPhiMET3/F" );
    outtree->Branch( "dPhiB0MET"        , &vars.dPhiB0MET        ,        "dPhiB0MET/F" );
    outtree->Branch( "dPhiB1MET"        , &vars.dPhiB1MET        ,        "dPhiB1MET/F" );
    outtree->Branch( "dPhiB2MET"        , &vars.dPhiB2MET        ,        "dPhiB2MET/F" );
    outtree->Branch( "dPhiB01MET"       , &vars.dPhiB01MET       ,       "dPhiB01MET/F" );
    outtree->Branch( "dPhiCVSnearMET"   , &vars.dPhiCVSnearMET   ,   "dPhiCVSnearMET/F" );
    outtree->Branch( "Mb1b2n"           , &vars.Mb1b2n           ,           "Mb1b2n/F" );
    outtree->Branch( "mtB0MET"          , &vars.mtB0MET          ,          "mtB0MET/F" );
    outtree->Branch( "mtB1MET"          , &vars.mtB1MET          ,          "mtB1MET/F" );
    outtree->Branch( "mtB2MET"          , &vars.mtB2MET          ,          "mtB1MET/F" );
    outtree->Branch( "mtB01MET"         , &vars.mtB01MET         ,         "mtB01MET/F" );
    outtree->Branch( "sSumB01oMET"      , &vars.sSumB01oMET      ,      "sSumB01oMET/F" );
    outtree->Branch( "vSumB01oMET"      , &vars.vSumB01oMET      ,      "vSumB01oMET/F" );
    outtree->Branch( "qgl0"             , &vars.qgl0             ,             "qgl0/F" );
    outtree->Branch( "qgl1"             , &vars.qgl1             ,             "qgl1/F" );
    outtree->Branch( "qglprod"          , &vars.qglprod          ,          "qglprod/F" );
    outtree->Branch( "qglprodNorm"      , &vars.qglprodNorm      ,      "qglprodNorm/F" );
    outtree->Branch( "qglprod30"        , &vars.qglprod30        ,        "qglprod30/F" );
    outtree->Branch( "qglprodNorm30"    , &vars.qglprodNorm30    ,    "qglprodNorm30/F" );
    outtree->Branch( "qglsum"           , &vars.qglsum           ,           "qglsum/F" );
    outtree->Branch( "qglsumNorm"       , &vars.qglsumNorm       ,       "qglsumNorm/F" );
    outtree->Branch( "qglsum30"         , &vars.qglsum30         ,         "qglsum30/F" );
    outtree->Branch( "qglsumNorm30"     , &vars.qglsumNorm30     ,     "qglsumNorm30/F" );
    outtree->Branch( "qglJ0"            , &vars.qglJ0            ,            "qglJ0/F" );
    outtree->Branch( "qglJ1"            , &vars.qglJ1            ,            "qglJ1/F" );
    outtree->Branch( "qglCVS0"          , &vars.qglCVS0          ,          "qglCVS0/F" );
    outtree->Branch( "qglCVS1"          , &vars.qglCVS1          ,          "qglCVS1/F" );
    outtree->Branch( "qgl0noB"          , &vars.qgl0noB          ,          "qgl0noB/F" );
    outtree->Branch( "qgl1noB"          , &vars.qgl1noB          ,          "qgl1noB/F" );
    outtree->Branch( "qglprodnoB"       , &vars.qglprodnoB       ,       "qglprodnoB/F" );
    outtree->Branch( "qglprodNormnoB"   , &vars.qglprodNormnoB   ,   "qglprodNormnoB/F" );
    outtree->Branch( "qglprodnoB30"     , &vars.qglprodnoB30     ,     "qglprodnoB30/F" );
    outtree->Branch( "qglprodNormnoB30" , &vars.qglprodNormnoB30 , "qglprodNormnoB30/F" );
    outtree->Branch( "qglsumnoB"        , &vars.qglsumnoB        ,        "qglsumnoB/F" );
    outtree->Branch( "qglsumNormnoB"    , &vars.qglsumNormnoB    ,    "qglsumNormnoB/F" );
    outtree->Branch( "qglsumnoB30"      , &vars.qglsumnoB30      ,      "qglsumnoB30/F" );
    outtree->Branch( "qglsumNormnoB30"  , &vars.qglsumNormnoB30  ,  "qglsumNormnoB30/F" );
    outtree->Branch( "qglJ0noB"         , &vars.qglJ0noB         ,         "qglJ0noB/F" );
    outtree->Branch( "qglJ1noB"         , &vars.qglJ1noB         ,         "qglJ1noB/F" );
    outtree->Branch( "ht"               , &vars.ht               ,               "ht/F" );
    outtree->Branch( "htAlongAway20"    , &vars.htAlongAway20    ,    "htAlongAway20/F" );
    outtree->Branch( "htAlongAway40"    , &vars.htAlongAway40    ,    "htAlongAway40/F" );
    outtree->Branch( "maxMj12"          , &vars.maxMj12          ,          "maxMj12/F" );
    outtree->Branch( "rmsJetPT"         , &vars.rmsJetPT         ,         "rmsJetPT/F" );
    outtree->Branch( "rmsJetDphiMET"    , &vars.rmsJetDphiMET    ,    "rmsJetDphiMET/F" );
    outtree->Branch( "bInvMass"         , &vars.bInvMass         ,         "bInvMass/F" );
    outtree->Branch( "bTransMass"       , &vars.bTransMass       ,       "bTransMass/F" );
    outtree->Branch( "rmsBEta"          , &vars.rmsBEta          ,          "rmsBEta/F" );
    outtree->Branch( "wInvMass"         , &vars.wInvMass         ,         "wInvMass/F" );
    outtree->Branch( "Bpt0"             , &vars.Bpt0             ,             "Bpt0/F" );
    outtree->Branch( "Bpt1"             , &vars.Bpt1             ,             "Bpt1/F" );
    outtree->Branch( "htnoB"            , &vars.htnoB            ,            "htnoB/F" );
    outtree->Branch( "htJ12"            , &vars.htJ12            ,            "htJ12/F" );
    outtree->Branch( "htJ12noB"         , &vars.htJ12noB         ,         "htJ12noB/F" );
    outtree->Branch( "metOsqrtHt"       , &vars.metOsqrtHt       ,       "metOsqrtHt/F" );
    outtree->Branch( "metOsqrtHtnoB"    , &vars.metOsqrtHtnoB    ,    "metOsqrtHtnoB/F" );
    outtree->Branch( "metOsqrtHtJ12"    , &vars.metOsqrtHtJ12    ,    "metOsqrtHtJ12/F" );
    outtree->Branch( "metOsqrtHtJ12noB" , &vars.metOsqrtHtJ12noB , "metOsqrtHtJ12noB/F" );
    outtree->Branch( "dPhivHtMET"       , &vars.dPhivHtMET       ,       "dPhivHtMET/F" );
    outtree->Branch( "dPhivHtMETnoB"    , &vars.dPhivHtMETnoB    ,    "dPhivHtMETnoB/F" );
    outtree->Branch( "dotHtAlongAway"   , &vars.dotHtAlongAway   ,   "dotHtAlongAway/F" );
    outtree->Branch( "dotHtAlongAwayNoB", &vars.dotHtAlongAwayNoB,"dotHtAlongAwayNoB/F" );
    outtree->Branch( "MT2tp0_000"       , &vars.MT2tp0_000       ,       "MT2tp0_000/F" );
    outtree->Branch( "NCTT"             , &vars.NCTT             ,             "NCTT/I" );
    outtree->Branch( "NCTTstd"          , &vars.NCTTstd          ,          "NCTTstd/I" );
    outtree->Branch( "DphiTopMET0"      , &vars.DphiTopMET0      ,      "DphiTopMET0/F" );
    outtree->Branch( "DphiTopMET1"      , &vars.DphiTopMET1      ,      "DphiTopMET1/F" );
    outtree->Branch( "DphiTopMET2"      , &vars.DphiTopMET2      ,      "DphiTopMET2/F" );
    outtree->Branch( "dPhiHtJ12MET"     , &vars.dPhiHtJ12MET     ,     "dPhiHtJ12MET/F" );
    outtree->Branch( "dPhiHtJ123MET"    , &vars.dPhiHtJ123MET    ,    "dPhiHtJ123MET/F" );
    outtree->Branch( "dPhiHtJMET"       , &vars.dPhiHtJMET       ,       "dPhiHtJMET/F" );

  } // Analyzer()

  virtual ~Analyzer() {
    fout->cd();
    outtree->Write();
    fout->Close();
  } // ~Analyzer()

  const double  metcut_    = 200.0 ;
  const int     minNJets_  =   5   ;
  const int     minNBjets_ =   1   ;
  const int     maxNTaus_  =   0   ;

  TFile *fout    ;
  TTree *outtree ;

  float scaleFactor ;
  int   npv         ;
  int   nAK4pfJets  ;
  int   nVetoTau    ;


  void loadVariables();
  void runEvent();
  
}; // Analyzer : BaseTreeAnalyzer


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
void Analyzer::runEvent() {


	  if(selectedLeptons.size() != 0) return;
	  if(nJets < minNJets_) return;
	  if(nBJets < minNBjets_) return;
	  if(met->pt() < metcut_) return;
	  if(nVetoedTracks>0)       return;


	  scaleFactor = weight; // lumi_*xsec_/getEntries(); <- old procedure, now stored in files to handle split samples correctly
	  npv         = nPV;
	  nAK4pfJets  = nJets;
	  nVetoTau    = nVetoedTracks;
	  vars.processVariables(this,&ak4Reader,jets,jets,met,cttTops); // calculate variables
	  outtree->Fill();

	  return;

} // Analyzer::runEvent()


/*
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
} // Analyzer::out()*/

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
//root -b -q "../CMSSW_7_3_1/src/AnalysisMethods/macros/0LepSearchRegions/processZeroLepton.C+()"
void processZeroLepton(      TString sname      = "ttbar" // sample name
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

  //Load up search configuration
  cfgSet::loadDefaultConfigurations();
  cfgSet::ConfigSet cfg = cfgSet::zl_search_set;
  cfg.vetoedLeptons.selectedMuon = (&MuonF::ismultiisovetomuonl);
  cfg.vetoedLeptons.selectedElectron = (&ElectronF::ismultiisovetoelectronl);


  //Create analyzer
  Analyzer a(fullname, "Events", isMC, &cfg, xsec, sname, outputdir);//declare analyzer
  a.analyze(10000); // run: Argument is frequency of printout
  //a.analyze(1000,100000); // for testing
  //a.out(sname, outputdir); // write outputfile with plots

} // processSingleLepton()
