////#include <vector>
////#include <assert.h>
////#include "TFile.h"
//#include "TTree.h"
//#include "TString.h"
////#include "TH1F.h"
////#include "TCanvas.h"
#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TSystem.h>
#include <TFile.h>
#include <TH1F.h>
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
//#include "AnalysisTools/TreeReader/interface/Defaults.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"

using namespace std;
using namespace ucsbsusy;

class Analyze : public BaseTreeAnalyzer{
public:

  Analyze(TString fname, string treeName, bool isMCTree, cfgSet::ConfigSet *pars, TString sname, TString outputdir)
         : BaseTreeAnalyzer(fname, treeName, isMCTree, pars)
  {
    //loadPlots(); // initialize plots

    weight_   = -9 ;
    npv       = -9 ;
    rho_      = -9 ;
    passDijet = true;
    passZjet  = true;
    passGmjet = true;
    j0eta     = -9 ;
    j0pt      = -9 ;
    j0flavor  = -9 ;
    j0mult    = -9 ;
    j0ptd     = -9 ;
    j0axis1   = -9 ;
    j0axis2   = -9 ;
    j0qgl     = -9 ;

    // initiliaze tree
    gSystem->mkdir(outputdir,true);
    fout = new TFile (outputdir+"/"+sname+"_tree.root","RECREATE");
    fout->cd();
    outtree = new TTree("Events","analysis tree");
    outtree->Branch( "weight"    , &weight_   ,    "weight/F" );
    outtree->Branch( "npv"       , &npv       ,       "npv/I" );
    outtree->Branch( "rho"       , &rho_      ,       "rho/F" );
    outtree->Branch( "passDijet" , &passDijet , "passDijet/O" );
    outtree->Branch( "passZjet"  , &passZjet  ,  "passZjet/O" );
    outtree->Branch( "passGmjet" , &passGmjet , "passGmjet/O" );
    outtree->Branch( "j0eta"     , &j0eta     ,     "j0eta/F" );
    outtree->Branch( "j0pt"      , &j0pt      ,      "j0pt/F" );
    outtree->Branch( "j0flavor"  , &j0flavor  ,  "j0flavor/I" );
    outtree->Branch( "j0mult"    , &j0mult    ,    "j0mult/I" );
    outtree->Branch( "j0ptd"     , &j0ptd     ,     "j0ptd/F" );
    outtree->Branch( "j0axis1"   , &j0axis1   ,   "j0axis1/F" );
    outtree->Branch( "j0axis2"   , &j0axis2   ,   "j0axis2/F" );
    outtree->Branch( "j0qgl"     , &j0qgl     ,     "j0qgl/F" );

  }; // Analyze()

  virtual ~Analyze() {
    fout->cd();
    outtree->Write();
    fout->Close();
  } // ~Analyzer()

  virtual void loadVariables(){
    load(cfgSet::EVTINFO);
    load(cfgSet::AK4JETS,  JetReader::LOADRECO | JetReader::LOADGEN | JetReader::LOADJETSHAPE | JetReader::FILLOBJ );
    load(cfgSet::ELECTRONS);
    load(cfgSet::MUONS);
    load(cfgSet::PHOTONS);
    if(isMC()) load(cfgSet::GENPARTICLES);
  } // loadVariables()

  void runEvent(){

    // event selection (both samples)
    if (nPV<1)         return;
    if (jets.size()<1) return;
    auto& jet0 = jets[0];
    // for jets 1,2 also?
    //if (isLooseBJet(*jet0)) return;
    if (cfgSet::isSelBJet(*jet0,cfgSet::ol_search_jets)) return;
    if (jet0->eta()<2.4 && ak4Reader.jetbetaStar_->at(0) > 0.2*log(nPV-0.67)) return; // only jets in tracker region

    // dijet event selection
    passDijet = true;
    if (jets.size()<2) passDijet = false;
    else if ( jets[0]->pt()<40 ) passDijet = false;
    else if ( jets[1]->pt()<20 ) passDijet = false;
    else if ( PhysicsUtilities::absDeltaPhi(*jets[0],*jets[1])<2.5 )  passDijet = false;
    if (jets.size()>=3){
      if ( jets[2]->pt() > 0.3*(jets[0]->pt() + jets[1]->pt())/2 ) passDijet = false;
    } // jets.size()>=3

    // Z jet event selection
    passZjet = true;
    if (selectedLeptons.size()>=2) {
      int Nmu = 0;
      LeptonF* mu0 = selectedLeptons[0];
      LeptonF* mu1 = selectedLeptons[1];
      for (unsigned int i=0; i<selectedLeptons.size();++i) {
        if (selectedLeptons[i]->ismuon()) {
          if      (Nmu==0) mu0 = selectedLeptons[i];
          else if (Nmu==1) mu1 = selectedLeptons[i];
          ++Nmu;
        } // ismuon()
      } // selectedLeptons.size()
      if (Nmu<2) passZjet = false;
      else {
        if      (mu0->pt()<20) passZjet = false;
        else if (mu1->pt()<10) passZjet = false;
        else if (mu0->q()==mu1->q()) passZjet = false;
        double invMass = (mu0->p4() + mu1->p4()).mass();
        if (invMass<70 || invMass>110) passZjet = false;
        if (jets.size()>=2) {
          if ( jets[1]->pt() > 0.3*(mu0->pt()+mu1->pt()) ) passZjet = false;
        } // jets.size()>=2
      } // Nmu>=2
    } // selectedLeptons.size>=2
    else passZjet = false;

    // Gamma jet event selection
    passGmjet = true;
    if (selectedPhotons.size()<1) passGmjet = false;


    // skip events that don't pass either selection
    if ( !passDijet && !passZjet && !passGmjet ) return;

    // fill tree variables [assmune lumi = 1 fb^(-1)]
    weight_ = weight;
    npv     = nPV;
    rho_    = rho;

    j0eta    = jet0->eta();
    j0pt     = jet0->pt();
    j0mult   = ak4Reader.jetMult_ ->at(0);
    j0ptd    = ak4Reader.jetptD_  ->at(0);
    j0axis1  = -log(ak4Reader.jetaxis1_->at(0));
    j0axis2  = -log(ak4Reader.jetaxis2_->at(0));
    j0qgl    = ak4Reader.jetqgl_  ->at(0);

    // flavor matching
    if (isMC()) {
      int type = 0; // undefined
      double nearDR = 0;
      int foundPart = PhysicsUtilities::findNearestDRDeref((*jet0),genParts,nearDR,.4,20);
      if(foundPart >= 0){
        int pdgId = TMath::Abs(genParts[foundPart]->pdgId());
        if(pdgId >= 1 && pdgId < 4) type = 1; // quark
        if(pdgId == 21) type = 2; // gluon
        if(pdgId ==  4) type = 3; // C
        if(pdgId ==  5) type = 4; // B
      }
      const GenJetF* matchGen = jet0->genJet_;
      if (type==0 && matchGen==0) type = -1; // pile-up
      j0flavor = type;
    } // isMC

    outtree->Fill();

  } // runEvent()

  //void out(TString outputPath){} // out()

  const TString sample_    ;
  const TString outputdir_ ;

  TFile *fout    ;
  TTree *outtree ;

  // variables for trees
  float weight_     ;
  int   npv         ;
  float rho_        ;
  bool  passDijet   ;
  bool  passZjet    ;
  bool  passGmjet   ;

  float j0eta       ;
  float j0pt        ;
  int   j0flavor    ;
  int   j0mult      ;
  float j0ptd       ;
  float j0axis1     ;
  float j0axis2     ;
  float j0qgl       ;

}; // BaseTreeAnalyzer


#endif


//root -b -q "../CMSSW_7_3_1/src/AnalysisMethods/macros/QGTagging/makeQGValidationTrees.C+()"
//     ttbar_1_ntuple_wgtxsec.root
//     dyjetstoll_ntuple_wgtxsec.root
//     gjets_ht600toinf_ntuple_wgtxsec.root
//     qcd_ht1000toinf_ntuple_wgtxsec.root
void makeQGValidationTrees( TString sname      = "qcd1000" // sample name
                  , const int     fileindex  = 4       // index of file (-1 means there is only 1 file for this sample)
                  , const bool    isMC       = true    // data or MC
                  , const TString fname      = "/store/user/vdutta/13TeV/290615/merged/gjets_ht600toinf_ntuple_wgtxsec.root" // path of file to be processed
                  , const double xsec        = 1.0
                  , const string  outputdir  = "trees/"  // directory to which files with histograms will be written
                  , const TString fileprefix = "root://eoscms//eos/cms" //"/eos/uscms/store/user/vdutta/13TeV/290615/merged/"
                  )
{

  printf("Processing file %d of %s sample\n", (fileindex > -1 ? fileindex : 0), sname.Data());

  // Make sure the output has a unique name in case there are multiple files to process
  if(fileindex > -1) sname += TString::Format("_%d",fileindex);

  TString fullname = fileprefix+fname;

  // Adjustments to default configuration
  cfgSet::loadDefaultConfigurations();
  cfgSet::ConfigSet pars = cfgSet::ol_search_set;

  pars.jets.minPt  = 20.0;
  pars.jets.maxEta =  5.0;
  pars.jets.minBJetPt  = 20.0;
  pars.jets.maxBJetEta =  5.0;
  pars.jets.defaultCSV = defaults::CSV_LOOSE;
  pars.jets.applyJetID = true;
  pars.jets.cleanJetsvSelectedPhotons = true;
  pars.jets.cleanJetsvSelectedLeptons = true;

  pars.selectedLeptons.selectedMuon = (&MuonF::isgoodpogmuon);
  pars.selectedLeptons.minMuPt  = 10.0;
  pars.selectedLeptons.maxMuEta =  5.0;
  pars.selectedLeptons.selectedElectron = (&ElectronF::isgoodpogelectron);
  pars.selectedLeptons.minEPt  = 10.0;
  pars.selectedLeptons.maxEEta =  5.0;

  pars.selectedPhotons = cfgSet::zl_sel_photons;
  pars.selectedPhotons.maxEta = 5.0;


  Analyze a(fullname, "Events", isMC, &pars, sname, outputdir);
  a.analyze(10000);

} // makeQGValidationTrees()
