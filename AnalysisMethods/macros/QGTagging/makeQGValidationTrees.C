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
#include <bitset>
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"

using namespace std;
using namespace ucsbsusy;

class Analyze : public BaseTreeAnalyzer{
public:

  Analyze(TString fname, string treeName, bool isMCTree, cfgSet::ConfigSet *pars, TString sname, TString outputdir)
         : BaseTreeAnalyzer(fname, treeName, isMCTree, pars)
  {
    //loadPlots(); // initialize plots
    TObjArray *o = sname.Tokenize("_");

    sample_     = ((TObjString *)(o->At(0)))->String();
    weight_     = -9 ;
    puWeight_   = -9 ;
    puWeightC_  = -9 ;
    npv         = -9 ;
    rho_        = -9 ;
    passDijet   = true;
    passDijet3  = true;
    passHT300   = false;
    passHT350   = false;
    passHT400   = false;
    passHT475   = false;
    passHT600   = false;
    passHT650   = false;
    passHT800   = false;
    passHT900   = false;
    prescale    = -9 ;
    prescale300 = -9 ;
    prescale350 = -9 ;
    prescale400 = -9 ;
    prescale475 = -9 ;
    prescale600 = -9 ;
    prescale650 = -9 ;
    prescale800 = -9 ;
    prescale900 = -9 ;
    passZjet    = true;
    passZmass   = true;
    passGmjet   = true;
    dilepmass   = -1 ;
    ht          = -1 ;
    mu0pt       = -1 ;
    mu1pt       = -1 ;
    mu0eta      = -1 ;
    mu1eta      = -1 ;
    j0eta       = -9 ;
    j0pt        = -9 ;
    j0flavor    = -9 ;
    j0mult      = -9 ;
    j0ptd       = -9 ;
    j0axis1     = -9 ;
    j0axis2     = -9 ;
    j0qgl       = -9 ;
    j0pdgid     = -9 ;

    // initiliaze tree
    gSystem->mkdir(outputdir,true);
    fout = new TFile (outputdir+"/"+sname+"_tree.root","RECREATE");
    fout->cd();
    outtree = new TTree("Events","analysis tree");
    outtree->Branch( "weight"       , &weight_      ,       "weight/F" );
    outtree->Branch( "puWeight"     , &puWeight_    ,     "puWeight/F" );
    outtree->Branch( "puWeightC"    , &puWeightC_   ,    "puWeightC/F" );
    outtree->Branch( "npv"          , &npv          ,          "npv/I" );
    outtree->Branch( "rho"          , &rho_         ,          "rho/F" );
    outtree->Branch( "passDijet"    , &passDijet    ,    "passDijet/O" );
    outtree->Branch( "passDijet3"   , &passDijet3   ,   "passDijet3/O" );
    outtree->Branch( "passHT300"    , &passHT300    ,    "passHT300/O" );
    outtree->Branch( "passHT350"    , &passHT350    ,    "passHT350/O" );
    outtree->Branch( "passHT400"    , &passHT400    ,    "passHT400/O" );
    outtree->Branch( "passHT475"    , &passHT475    ,    "passHT475/O" );
    outtree->Branch( "passHT600"    , &passHT600    ,    "passHT600/O" );
    outtree->Branch( "passHT650"    , &passHT650    ,    "passHT650/O" );
    outtree->Branch( "passHT800"    , &passHT800    ,    "passHT800/O" );
    outtree->Branch( "passHT900"    , &passHT900    ,    "passHT900/O" );
    outtree->Branch( "prescale"     , &prescale     ,     "prescale/I" );
    outtree->Branch( "prescale300"  , &prescale300  ,  "prescale300/I" );
    outtree->Branch( "prescale350"  , &prescale350  ,  "prescale350/I" );
    outtree->Branch( "prescale400"  , &prescale400  ,  "prescale400/I" );
    outtree->Branch( "prescale475"  , &prescale475  ,  "prescale475/I" );
    outtree->Branch( "prescale600"  , &prescale600  ,  "prescale600/I" );
    outtree->Branch( "prescale650"  , &prescale650  ,  "prescale650/I" );
    outtree->Branch( "prescale800"  , &prescale800  ,  "prescale800/I" );
    outtree->Branch( "prescale900"  , &prescale900  ,  "prescale900/I" );
    outtree->Branch( "passZjet"     , &passZjet     ,     "passZjet/O" );
    outtree->Branch( "passZmass"    , &passZmass    ,    "passZmass/O" );
    outtree->Branch( "passGmjet"    , &passGmjet    ,    "passGmjet/O" );
    outtree->Branch( "dilepmass"    , &dilepmass    ,    "dilepmass/F" );
    outtree->Branch( "ht"           , &ht           ,           "ht/F" );
    outtree->Branch( "mu0pt"        , &mu0pt        ,        "mu0pt/F" );
    outtree->Branch( "mu1pt"        , &mu1pt        ,        "mu1pt/F" );
    outtree->Branch( "mu0eta"       , &mu0eta       ,       "mu0eta/F" );
    outtree->Branch( "mu1eta"       , &mu1eta       ,       "mu1eta/F" );
    outtree->Branch( "j0eta"        , &j0eta        ,        "j0eta/F" );
    outtree->Branch( "j0pt"         , &j0pt         ,         "j0pt/F" );
    outtree->Branch( "j0flavor"     , &j0flavor     ,     "j0flavor/I" );
    outtree->Branch( "j0mult"       , &j0mult       ,       "j0mult/I" );
    outtree->Branch( "j0ptd"        , &j0ptd        ,        "j0ptd/F" );
    outtree->Branch( "j0axis1"      , &j0axis1      ,      "j0axis1/F" );
    outtree->Branch( "j0axis2"      , &j0axis2      ,      "j0axis2/F" );
    outtree->Branch( "j0qgl"        , &j0qgl        ,        "j0qgl/F" );
    outtree->Branch( "j0pdgid"      , &j0pdgid      ,      "j0pdgid/I" );

  }; // Analyze()

  virtual ~Analyze() {
    fout->cd();
    outtree->Write();
    fout->Close();
  } // ~Analyzer()

  virtual void loadVariables(){
    load(cfgSet::EVTINFO);
    if (isMC()) load(cfgSet::AK4JETS,  JetReader::LOADRECO | JetReader::LOADGEN | JetReader::LOADJETSHAPE | JetReader::FILLOBJ );
    else        load(cfgSet::AK4JETS,  JetReader::LOADRECO |                      JetReader::LOADJETSHAPE | JetReader::FILLOBJ );
    load(cfgSet::ELECTRONS);
    load(cfgSet::MUONS);
    load(cfgSet::PHOTONS);
    load(cfgSet::TRIGOBJS);
    if(isMC()) load(cfgSet::GENPARTICLES);
  } // loadVariables()

  void runEvent(){

    if(!isMC() && hasJSONFile() && !passesLumiMask()) return;
    //if(!triggerflag) return; // skip events that pass no saved triggers

    // skip events in PR with run number < 251584 - they are in the re-miniAOD
    if(!isMC() && datareco==defaults::PROMPT_50NS && run<251584) return;

    bool passTrigger = false;

    // event selection (both samples)
    if (nPV<1)         return;
    if (jets.size()<1) return;
    auto& jet0 = jets[0];
    if ( !ak4Reader.jettightId_->at(jet0->index()) ) return;
    if (cfgSet::isSelBJet(*jet0,cfgSet::zl_search_jets)) return;
    if (abs(jet0->eta())<2.4 && ak4Reader.jetbetaStar_->at(jet0->index()) > 0.2*log(nPV-0.67)) return; // only jets in tracker region

    // dijet event selection
    ht = JetKinematics::ht(jets, 40.0, 3  );
    passDijet  = true;
    passDijet3 = true;
    if (jets.size()<2) passDijet = false;
    else if ( jets[0]->pt()<40 ) passDijet = false;
    else if ( jets[1]->pt()<20 ) passDijet = false;
    else if ( PhysicsUtilities::absDeltaPhi(*jets[0],*jets[1])<2.5 )  passDijet = false;
    if (jets.size()>=3){
      if ( jets[2]->pt() > 0.3*(jets[0]->pt() + jets[1]->pt())/2 ) passDijet3 = false;
    } // jets.size()>=3
    passHT300 = false; prescale300 = -9;
    passHT350 = false; prescale350 = -9;
    passHT400 = false; prescale400 = -9;
    passHT475 = false; prescale475 = -9;
    passHT600 = false; prescale600 = -9;
    passHT650 = false; prescale650 = -9;
    passHT800 = false; prescale800 = -9;
    passHT900 = false; prescale900 = -9;
    for(auto* tI: triggerInfo){
      if((tI->trigflag()&kHLT_PFHT300) && tI->passtrig()) passHT300 = true;
      if((tI->trigflag()&kHLT_PFHT350) && tI->passtrig()) passHT350 = true;
      if((tI->trigflag()&kHLT_PFHT400) && tI->passtrig()) passHT400 = true;
      if((tI->trigflag()&kHLT_PFHT475) && tI->passtrig()) passHT475 = true;
      if((tI->trigflag()&kHLT_PFHT600) && tI->passtrig()) passHT600 = true;
      if((tI->trigflag()&kHLT_PFHT650) && tI->passtrig()) passHT650 = true;
      if((tI->trigflag()&kHLT_PFHT800) && tI->passtrig()) passHT800 = true;
      if((tI->trigflag()&kHLT_PFHT900) && tI->passtrig()) passHT900 = true;
      if(tI->trigflag()&kHLT_PFHT300) prescale300 = tI->prescale();
      if(tI->trigflag()&kHLT_PFHT350) prescale350 = tI->prescale();
      if(tI->trigflag()&kHLT_PFHT400) prescale400 = tI->prescale();
      if(tI->trigflag()&kHLT_PFHT475) prescale475 = tI->prescale();
      if(tI->trigflag()&kHLT_PFHT600) prescale600 = tI->prescale();
      if(tI->trigflag()&kHLT_PFHT650) prescale650 = tI->prescale();
      if(tI->trigflag()&kHLT_PFHT800) prescale800 = tI->prescale();
      if(tI->trigflag()&kHLT_PFHT900) prescale900 = tI->prescale();
    } // passTrigger
    prescale = 1;
    if(!isMC()){
      if      (passHT800 && ht>950           ) prescale = prescale800;
      else if (passHT600 && ht>750 && ht<=950) prescale = prescale600;
      else if (passHT475 && ht>625 && ht<=750) prescale = prescale475;
      else if (passHT400 && ht>550 && ht<=625) prescale = prescale400;
      else if (passHT350 && ht>500 && ht<=550) prescale = prescale350;
      else if (passHT300 && ht>450 && ht<=500) prescale = prescale300;
      else passDijet = false;
    } else if(ht<450) passDijet = false;
/*
    if(passDijet) {
      cout << bitset<64>(triggerflag) << endl;
      for(auto* tI: triggerInfo){
        if(tI->trigflag()&kHLT_PFHT300) cout << bitset<64>(tI->trigflag()) << "\t" << tI->passtrig() << "  " << tI->prescale() << "\t300" << endl;
        if(tI->trigflag()&kHLT_PFHT350) cout << bitset<64>(tI->trigflag()) << "\t" << tI->passtrig() << "  " << tI->prescale() << "\t350" << endl;
        if(tI->trigflag()&kHLT_PFHT400) cout << bitset<64>(tI->trigflag()) << "\t" << tI->passtrig() << "  " << tI->prescale() << "\t400" << endl;
        if(tI->trigflag()&kHLT_PFHT475) cout << bitset<64>(tI->trigflag()) << "\t" << tI->passtrig() << "  " << tI->prescale() << "\t475" << endl;
        if(tI->trigflag()&kHLT_PFHT600) cout << bitset<64>(tI->trigflag()) << "\t" << tI->passtrig() << "  " << tI->prescale() << "\t600" << endl;
        if(tI->trigflag()&kHLT_PFHT650) cout << bitset<64>(tI->trigflag()) << "\t" << tI->passtrig() << "  " << tI->prescale() << "\t650" << endl;
        if(tI->trigflag()&kHLT_PFHT800) cout << bitset<64>(tI->trigflag()) << "\t" << tI->passtrig() << "  " << tI->prescale() << "\t800" << endl;
        if(tI->trigflag()&kHLT_PFHT900) cout << bitset<64>(tI->trigflag()) << "\t" << tI->passtrig() << "  " << tI->prescale() << "\t900" << endl;
      } // passTrigger
      cout << endl;
    } // */

    // Z jet event selection
    passZjet  = true;
    passZmass = true;
    int selMu0 = 0;
    int selMu1 = 0;
    if (selectedLeptons.size()>=2) {
      int Nmu = 0;
      auto mu0 = selectedLeptons[0];
      auto mu1 = selectedLeptons[1];
      for (unsigned int i=0; i<selectedLeptons.size();++i) {
        if ( selectedLeptons[i]->ismuon() && selectedLeptons[i]->eta()<2.1 ) {
          if      (Nmu==0) { mu0 = selectedLeptons[i]; selMu0 = i; }
          else if (Nmu==1) { mu1 = selectedLeptons[i]; selMu1 = i; }
          ++Nmu;
        } // ismuon()
      } // selectedLeptons.size()
      mu0pt  = mu0->pt();
      mu1pt  = mu1->pt();
      mu0eta = mu0->eta();
      mu1eta = mu1->eta();
      if (Nmu<2) passZjet = false;
      else {
        if      (mu0->pt()<20) passZjet = false; // was 20 in note; 30 for singleMu
        else if (mu1->pt()<10) passZjet = false; // was 10 in note; 20 for singleMu
        else if (mu0->q()==mu1->q()) passZjet = false;
        dilepmass = (mu0->p4() + mu1->p4()).mass();
        if (dilepmass<70 || dilepmass>110) passZmass = false;
        if (jets.size()>=2) {
          if ( jets[1]->pt() > 0.3*(mu0->pt()+mu1->pt()) ) passZjet = false;
        } // jets.size()>=2
      } // Nmu>=2
    } // selectedLeptons.size>=2
    else passZjet = false;
    passTrigger = false;
    //if((triggerflag & kHLT_IsoTkMu24_eta2p1) > 0) passTrigger = true;
    if((triggerflag & kHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ) > 0) passTrigger = true;
    if (!passTrigger) passZjet = false;

    // Gamma jet event selection
    passGmjet = true;
    if (selectedPhotons.size()>=1) {
      if (selectedPhotons[0]->pt() <180) passGmjet = false;
      if (selectedPhotons[0]->eta()>2.5) passGmjet = false;
    } // selectedPhotons.size()<1
    else passGmjet = false;
    passTrigger = false;
    if((triggerflag & kHLT_Photon165_HE10) > 0) passTrigger = true;
    if (!passTrigger) passGmjet = false;

    // check trigger objects
    bool foundTrigMu = false;
    for(auto* to : triggerObjects) {
      if(passZjet) {
        bool matchedTrigObj = false;
        if((to->filterflags() & kSingleIsoTkMu24) && (to->pathflags() & kHLT_IsoTkMu24_eta2p1)) matchedTrigObj = true;
        if(matchedTrigObj) {
          auto mu0 = selectedLeptons[selMu0];
          if (PhysicsUtilities::deltaR(*to,*mu0)<0.4 ) foundTrigMu = true;
        } // matchedTrigObj
      } // passZjet
    } // triggerObjects
    if (!foundTrigMu) passZjet  = false;

    // skip events that don't pass any selection
    if (!passDijet && !passZjet && !passGmjet) return;

    // fill tree variables [assmune lumi = 1 fb^(-1)]
    weight_ = weight;
    npv     = nPV;
    rho_    = rho;

    j0eta    = jet0->eta();
    j0pt     = jet0->pt();
    j0mult   =      ak4Reader.jetMult_ ->at(jet0->index()) ;
    j0ptd    =      ak4Reader.jetptD_  ->at(jet0->index()) ;
    j0axis1  = -log(ak4Reader.jetaxis1_->at(jet0->index()));
    j0axis2  = -log(ak4Reader.jetaxis2_->at(jet0->index()));
    j0qgl    =      ak4Reader.jetqgl_  ->at(jet0->index()) ;

    // flavor matching
    if (isMC()) {
      int type = 0; // undefined
      double nearDR = 0;
      //int foundPart = PhysicsUtilities::findNearestDRDeref((*jet0),genParts,nearDR,.3,20);
      int foundPart = PhysicsUtilities::findNearestDRDeref((*jet0),genParts,nearDR,.4,20);
      if(foundPart >= 0){
        int pdgId = TMath::Abs(genParts[foundPart]->pdgId());
        if(genParts[foundPart]->status()!=23)  return; // only want particles from the matrix element
        //genParts[foundPart]->isDoc (in ParticleInfo.h)
        if(pdgId >= 1 && pdgId < 4) type = 1; // quark
        if(pdgId == 21) type = 2; // gluon
        if(pdgId ==  4) type = 3; // C
        if(pdgId ==  5) type = 4; // B
        j0pdgid = pdgId;
      }
      //const GenJetF* matchGen = jet0->genJet();
      //if (type==0 && matchGen==0) type = -1; // pile-up
      else type = -1; // pile-up
      j0flavor = type;
    } // isMC

    // quick PU reweighting by npv
    double rwtZjet[] = {0, 8.72541 , 4.54199 , 0.853572, 0.539271, 0.590481,    0.602232, 0.70575, 0.689348, 0.799847, 0.84226
                         , 0.843986, 0.916837, 1.02284 , 1.05258 , 1.02993 ,    1.1635  , 1.22879, 1.1195  , 1.18014 , 1.25201
                         , 1.14572 , 1.1972  , 1.18144 , 1.15038 , 1.00678 ,    1.25408 , 1.18112, 0.848419, 1.36201 , 1.97805
                         , 1.47935  };
    double rwtDjet[] = {0, 697.591 , 4.24526 , 0.762199, 0.394076, 0.330613,    0.418513, 0.414527, 0.538188, 0.571216, 0.704519
                         , 0.759045, 0.900076, 1.01713 , 1.00742 , 1.21182 ,    1.22338 , 1.40168 , 1.41092 , 1.34799 , 1.42774
                         , 1.33582 , 1.35183 , 1.38252 , 1.34385 , 1.25685 ,    1.22773 , 1.39678 , 1.02986 , 1.22173 , 0.794478
                         , 0.868953 };
    double rwtGjet[] = {0, 457.067 , 7.09757 , 1.12134 , 0.392318, 0.270617,    0.44275 , 0.391132, 0.408531, 0.584548, 0.639067
                         , 0.812284, 0.923726, 1.00071 , 1.05271 , 1.22904 ,    1.27838 , 1.30048 , 1.36448 , 1.36015 , 1.47939
                         , 1.40911 , 1.4732  , 1.44616 , 0.983238, 1.3588  ,    0.996126, 1.59561 , 1.1468  , 1.41155 , 1.56843
                         , 0.964321 };
    int rwtNPV = min(npv,31);
    if(isMC()){
      //cout << sample_ << endl;
      if      (sample_ == "dyjetstoll") puWeight_ = rwtZjet[rwtNPV];
      else if (sample_ == "qcd")        puWeight_ = rwtDjet[rwtNPV];
      else if (sample_ == "gjets")      puWeight_ = rwtGjet[rwtNPV];
      else puWeight_ = -99;
    } else puWeight_ = 1;
    puWeightC_ = eventCorrections.getPUWeight();

    outtree->Fill();

  } // runEvent()

  //void out(TString outputPath){} // out()

  TString sample_    ;

  TFile *fout    ;
  TTree *outtree ;

  // variables for trees
  float weight_     ;
  float puWeight_   ;
  float puWeightC_  ;
  int   npv         ;
  float rho_        ;

  bool  passDijet   ;
  bool  passDijet3  ;
  bool  passHT300   ;
  bool  passHT350   ;
  bool  passHT400   ;
  bool  passHT475   ;
  bool  passHT600   ;
  bool  passHT650   ;
  bool  passHT800   ;
  bool  passHT900   ;
  int   prescale    ;
  int   prescale300 ;
  int   prescale350 ;
  int   prescale400 ;
  int   prescale475 ;
  int   prescale600 ;
  int   prescale650 ;
  int   prescale800 ;
  int   prescale900 ;
  bool  passZjet    ;
  bool  passZmass   ;
  bool  passGmjet   ;

  float ht          ;
  float dilepmass   ;
  float mu0pt       ;
  float mu1pt       ;
  float mu0eta      ;
  float mu1eta      ;

  float j0eta       ;
  float j0pt        ;
  int   j0flavor    ;
  int   j0mult      ;
  float j0ptd       ;
  float j0axis1     ;
  float j0axis2     ;
  float j0qgl       ;
  int   j0pdgid     ;

}; // BaseTreeAnalyzer


#endif


//root -b -q "../CMSSW_7_4_7/src/AnalysisMethods/macros/QGTagging/makeQGValidationTrees.C+()"
// 74X
//     singlemu-2015b-17jul15_ntuple_postproc.root    (data)     singlemu
//     jetht-2015b-17jul15_ntuple_postproc.root       (data)     jetht
//     jetht-2015b-pr_ntuple_postproc.root                       jetht2
//     dyjetstoll_mll50_10_ntuple_postproc.root       (MC)       dyjetstoll
//     qcd_ht1000to1500-50ns_1_ntuple_postproc.root   (MC)       qcd
void makeQGValidationTrees( TString sname            = "jetht2" // sample name
                          , const int     fileindex  = -1       // index of file (-1 means there is only 1 file for this sample)
                          , const bool    isMC       = false    // data or MC
                          , const TString fname      = "jetht-2015b-pr_ntuple_postproc.root" // path of file to be processed
                          , const double xsec        = 1.0
                          , const string  outputdir  = "trees/test/"  // directory to which files will be written
                          , const TString fileprefix = "/eos/uscms/store/user/vdutta/13TeV/130815/merged/"
                          //, const TString fileprefix = "/eos/uscms/store/user/ocolegro/13TeV/130815/merged/"
                          )
{

  printf("Processing file %d of %s sample with xsec %f\n", (fileindex > -1 ? fileindex : 0), sname.Data(), xsec);

  // Make sure the output has a unique name in case there are multiple files to process
  if(fileindex > -1) sname += TString::Format("_%d",fileindex);

  TString fullname = fileprefix+fname;

  // Adjustments to default configuration
  cfgSet::loadDefaultConfigurations();
  cfgSet::setJSONFile("Cert_246908-251883_13TeV_PromptReco_Collisions15_JSON_v2.txt");
  //cfgSet::setJSONFile("/uscms/home/mullin/nobackup/stuff2015/CMSSW_7_4_7/src/data/JSON/Cert_246908-251883_13TeV_PromptReco_Collisions15_JSON_v2.txt");
  cfgSet::ConfigSet qgv_search_set;
  qgv_search_set.jets            = cfgSet::zl_search_jets;
  qgv_search_set.selectedLeptons = cfgSet::ol_sel_leptons;
  qgv_search_set.selectedPhotons = cfgSet::zl_sel_photons;
  qgv_search_set.corrections     = cfgSet::standardCorrections;

  cfgSet::ConfigSet pars = qgv_search_set;

  pars.jets.minPt  = 20.0;
  pars.jets.maxEta =  5.0;
  pars.jets.minBJetPt  = 20.0;
  pars.jets.maxBJetEta =  5.0;
  pars.jets.cleanJetsvSelectedPhotons = true;
  pars.jets.cleanJetsvSelectedLeptons = true;

  pars.selectedLeptons.selectedMuon = (&MuonF::isgoodpogmuon);
  pars.selectedLeptons.minMuPt  = 10.0;
  pars.selectedLeptons.maxMuEta =  5.0;
  pars.selectedLeptons.selectedElectron = (&ElectronF::isgoodpogelectron);
  pars.selectedLeptons.minEPt  = 10.0;
  pars.selectedLeptons.maxEEta =  5.0;

  pars.selectedPhotons = cfgSet::zl_sel_photons;
  pars.selectedPhotons.minPt  = 10.0;
  pars.selectedPhotons.maxEta =  5.0;

  string    treeName = "";
  if (isMC) treeName = "Events";
  else      treeName = "TestAnalyzer/Events";
  Analyze a(fullname, "Events", isMC, &pars, sname, outputdir);
  a.analyze(10000);
  //a.analyze(1000,10000);

} // makeQGValidationTrees()
