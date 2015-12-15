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
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"

using namespace std;
using namespace ucsbsusy;

class Analyze : public BaseTreeAnalyzer{
public:
  enum sampType { OTHER = 0
                , GJET  = 1
                , QCD   = 2
                , FRAG  = 2
                , FAKE  = 2
  };
  const double dr_cut = 0.4;

  Analyze(TString fname, string treeName, bool isMCTree, cfgSet::ConfigSet *pars, TString sname, TString outputdir)
         : BaseTreeAnalyzer(fname, treeName, 1, isMCTree, pars)
  {
    //loadPlots(); // initialize plots
    TObjArray *o = sname.Tokenize("_");

    sample_     = ((TObjString *)(o->At(0)))->String();
    mctype_     = OTHER;
    weight_     = -9 ;
    puWeight_   = -9 ;
    puWeightC_  = -9 ;
    puWeightRho_= -9 ;
    gMultWt_    = -9 ;
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
    passGgenmch = true;
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
    outtree->Branch( "puWeightRho"  , &puWeightRho_ ,  "puWeightRho/F" );
    outtree->Branch( "gMultWt"      , &gMultWt_     ,      "gMultWt/F" );
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
    outtree->Branch( "passGgenmch"  , &passGgenmch  ,  "passGgenmch/O" );
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
    //if(!isMC() && datareco==defaults::PROMPT_50NS && run<251584) return;

    bool passTrigger = false;

    // event selection (both samples)
    if(!goodvertex) return;
    if (jets.size()<1) return;
    auto& jet0 = jets[0];
    //if ( !ak4Reader.jettightId_->at(jet0->index()) ) return;
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
        if ( selectedLeptons[i]->ismuon() ) {
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
          if ( jets[1]->pt() > 0.3*(dilepmass) ) passZjet = false;
        } // jets.size()>=2
      } // Nmu>=2
    } // selectedLeptons.size>=2
    else passZjet = false;
    passTrigger = false;
    //if((triggerflag & kHLT_IsoTkMu24_eta2p1) > 0) passTrigger = true;
    if((triggerflag & kHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ) > 0) passTrigger = true;
    if (!passTrigger) passZjet = false;

    // Gamma jet event selection
    passGmjet   = true;
    passGgenmch = false;
    if (selectedPhotons.empty())                  passGmjet = false;
    else if(!(triggerflag & kHLT_Photon165_HE10)) passGmjet = false;
    else if(selectedPhotons[0]->pt()    <180)     passGmjet = false;
    else if(selectedPhotons[0]->absEta()>2.5)     passGmjet = false;
    else if(isMC() && mctype_ != OTHER){ // to orthogonalize gjets and qcd MC
      auto pho = selectedPhotons[0];
      vector<GenParticleF*> partons;    // all DocOutgiong quarks and gluons
      vector<GenParticleF*> genphotons; // all final prompt photons
      for(auto *gp : genParts){
        if(ParticleInfo::isDocOutgoing(gp->status()) && ParticleInfo::isQuarkOrGluon(gp->pdgId())) partons.push_back(gp);
        else if (ParticleInfo::isFinal(gp->status()) && gp->pdgId()==ParticleInfo::p_gamma){
          bool isPrompt = false;
          for (int iM=0; iM<gp->numberOfMothers(); ++iM){
            auto mother = gp->mother(iM);
            if (ParticleInfo::isQuark(mother->pdgId()) || mother->pdgId() == ParticleInfo::p_gamma){
              isPrompt = true;
              break;
            }
          } // gp->Mothers
          if (isPrompt) genphotons.push_back(gp);
        } // final photons
      } // genParts

      if (genphotons.empty()) {
        if(mctype_ == FAKE) passGmjet = true;
        else                passGmjet = false;
      }else{
        std::sort(genphotons.begin(), genphotons.end(), PhysicsUtilities::greaterPTDeref<GenParticleF>());
        GenParticleF* genpho; // was: boson
        for (auto *gp : genphotons){
          if (PhysicsUtilities::deltaR2(*gp,*pho)<0.01 && pho->pt()>0.5*gp->pt() && pho->pt()<2*gp->pt()){
            passGgenmch = true;
            genpho = gp;
            break;
          } // gen photon matches sel photon
        } // genphotons

        if (passGgenmch){
          double minDR = 0;
          PhysicsUtilities::findNearestDRDeref(*genpho, partons, minDR);
          if(minDR<=dr_cut) { // outgoing q/g in the cone of the gen photon
            if(mctype_ == FRAG) passGmjet = true;
            else                passGmjet = false;
          }else{ // NO outgoing q/g in the cone of the gen photon
            if     (mctype_ == GJET) passGmjet = true;
            else                     passGmjet = false;
          }
        } // passGgenmch
        else {
          if(mctype_ == FAKE) passGmjet = true;
          else                passGmjet = false;
        }
      } // non-empty genphotons
    } // pass basic gm+jet cuts

    // check trigger objects
    //bool foundTrigMu = false;
    //for(auto* to : triggerObjects) {
    //  if(passZjet) {
    //    bool matchedTrigObj = false;
    //    if((to->filterflags() & kSingleIsoTkMu24)
             // && (to->pathflags() & kHLT_IsoTkMu24_eta2p1)
    //       ) matchedTrigObj = true;
    //    if(matchedTrigObj) {
    //      auto mu0 = selectedLeptons[selMu0];
    //      if (PhysicsUtilities::deltaR(*to,*mu0)<dr_cut ) foundTrigMu = true;
    //    } // matchedTrigObj
    //  } // passZjet
    //} // triggerObjects
    //if (!foundTrigMu) passZjet  = false;

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
    if(isMC()) {
      vector<bool> usePart;
      for(auto gp : genParts) {
        int pdgId = TMath::Abs(gp->pdgId());
        if     (pdgId >= 1 && pdgId <= 5) usePart.push_back(false); // quark
        else if(pdgId == 21             ) usePart.push_back(false); // gluon
        else                              usePart.push_back(true);
      }
      int type = 0; // undefined
      double nearDR = 0;
      //int foundPart = PhysicsUtilities::findNearestDRDeref((*jet0),genParts,nearDR,.3,20);
      int foundPart = PhysicsUtilities::findNearestDRDeref((*jet0),genParts,nearDR,dr_cut,20,&usePart);
      if(foundPart >= 0){
        int pdgId = TMath::Abs(genParts[foundPart]->pdgId());
        if(ParticleInfo::isDoc(genParts[foundPart]->status())) { // only want particles from the matrix element
          if(pdgId >= 1 && pdgId < 4) type = 1; // quark
          if(pdgId == 21) type = 2; // gluon
          if(pdgId ==  4) type = 3; // C
          if(pdgId ==  5) type = 4; // B
        }
        j0pdgid = pdgId;
      }
      //const GenJetF* matchGen = jet0->genJet();
      //if (type==0 && matchGen==0) type = -1; // pile-up
      else type = -1; // pile-up
      j0flavor = type;
    } // isMC

    // quick extra PU reweighting by rho
    double rwtZjet[] = { 0.471556, 0.631976, 0.836271, 0.991505, 1.07365 ,    1.13746 , 1.18152 , 1.14383 , 1.12851 , 1.0442
                       , 0.991142, 0.91913 , 0.831613, 0.7619  , 0.731759,    0.620591, 0.595255, 0.532813, 0.435604, 0.379652
                       };
    double rwtDjet[] = { 0.493354, 0.679612, 0.864667, 0.94803 , 1.11369 ,    1.13768 , 1.17368 , 1.15123 , 1.13993 , 1.05055
                       , 0.993487, 0.895439, 0.850206, 0.757461, 0.698321,    0.606653, 0.57766 , 0.480198, 0.406852, 0.322373
                       };
    double rwtGjet[] = { 0.513891, 0.63127 , 0.912403, 1.01097 , 1.06378 ,    1.18492 , 1.13323 , 1.15701 , 1.04492 , 1.08977
                       , 1.01602 , 0.924783, 0.841563, 0.731833, 0.645179,    0.550822, 0.516163, 0.455437, 0.407913, 0.2996
                       };
    int rwt = (int)rho;
    if (rwt > 20) rwt = 20;
    if (rwt <  1) rwt = 1;
    if(isMC()){
      //cout << sample_ << endl;
      if      (sample_ == "dyjetstoll") puWeightRho_ = rwtZjet[rwt-1];
      else if (sample_ == "qcd")        puWeightRho_ = rwtDjet[rwt-1];
      else if (sample_ == "gjets")      puWeightRho_ = rwtGjet[rwt-1];
      else puWeightRho_ = -99;
    } else puWeightRho_ =   1;
    puWeight_  = eventCorrections.getPUWeight();
    puWeightC_ = eventCorrections.getPUWeight();

    // reweight gluon multiplicity
    double rwtZjetGmult[] = { 0 // DO NOT USE!!!
                            , 0, 21.0445, 25.5486, 11.7367, 9.20504            , 0.783192, 0.991674, -1.64946, -1.08174, -0.560247
                            , -0.441623, 0.360786, 0.774671, 1.67903, 1.60716  , 2.96429, 2.60832, 3.06251, 1.91737, 1.76025
                            , 3.71875, 1.48734, 2.73922, 3.32801, 1.83328      , 1.58729, 2.01649, 3.51253, 5.53429, 9.19973
                            , 2.76685, 2.07348, 3.9401, 3.89945, -0.0293365    , 6.05268, 24.7232, 3.8651, 7.57871, -8.52751
                            , 5.11116, 40.1852, 4.88769, -112.624, 0.191959    , -3.77715, -0, 1.2812, 0, 0
                       }; // 25*2
    double rwtDjetGmult[] = { 0
                            , 0, 0, 28.6158, 4.93411, 0.536764, 2.64397, 2.27779, 4.77995, 5.47224, 6.60748, 6.4766, 9.10514, 7.22784
                            , 5.99012, 5.40705, 3.41891, 3.52568, 2.94832, 2.20819, 1.76844, 1.70158, 1.46635, 1.37784, 1.26883
                            , 1.17281, 1.08488, 0.980761, 0.933901, 0.844996, 0.977604, 0.77676, 0.798663, 0.776461, 0.694869, 0.695897
                            , 0.634346, 0.571494, 0.529396, 0.563128, 0.559592, 0.445727, 0.433191, 0.44345, 0.454631, 0.432487
                            , 0.34726, 0.316925, 0.379809, 0.265298, 0.31232, 0.340197, 0.33152, 0.258042, 0.20269, 0.305671, 0.124458
                            , 0.252885, 0.218703, 0.197533, 0.189032, 0.201527, 0.186056, 0.256245, 0.22102, 0.171713, 0.177715
                            , -0.00522122, 0.229658, 0.0118424, 0.0867014
                            }; // 35*2
    double rwtGjetGmult[] = { 0
                            , 0, 0, 0, 20.5493, 9.67352, 10.7454, 6.77105, 5.88972, 2.67506, 10.2546, 4.30916, 10.0084, 11.3868, 9.68117
                            , 11.336, 5.26551, 7.32094, 4.06644, 3.01145, 2.37808, 1.90956, 2.04402, 1.32419, 1.05764, 1.31391, 0.906093
                            , 0.735941, 0.794332, 0.89091, 0.740491, 0.907406, 0.509547, 0.619333, 0.716255, 0.425226, 0.456154, 0.67118
                            , 0.517335, 0.305512, 0.364712, 0.518962, 0.358548, 0.223662, 0.35196, 0.262922, 0.117702, 0.319071, 0.48766
                            , 0.33106, 0.314219, 0.1594, 0.511811, 0.280961, 0.241776, -0.0286059, 0.268344, -0.0830736, -0.0403848
                            , -0.0130221, 0.193786, -0.0108588, 0.594792, -0.00599611, 0, 1.03968, 2.90775, -0.549424, 0, 3.23507, 0
                            }; // 25*2
    // only for MC gluons with eta<2.4
    if(isMC() && jet0->absEta()<2.4 && j0flavor==2){
      int nMult = j0mult;
      if(nMult<1) nMult=1;
      if      (sample_ == "dyjetstoll") { if(nMult>50) nMult=50; gMultWt_ = 1; }
      else if (sample_ == "qcd")        { if(nMult>70) nMult=70; gMultWt_ = rwtDjetGmult[nMult]; }
      else if (sample_ == "gjets")      { if(nMult>70) nMult=50; gMultWt_ = rwtGjetGmult[nMult]; }
      else gMultWt_ = -99;
    } else gMultWt_ =   1;

    outtree->Fill();

  } // runEvent()

  //void out(TString outputPath){} // out()

  TString  sample_   ;
  sampType mctype_   ;

  TFile *fout    ;
  TTree *outtree ;

  // variables for trees
  float weight_     ;
  float puWeight_   ;
  float puWeightC_  ;
  float puWeightRho_;
  float gMultWt_    ;
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
  bool  passGgenmch ;

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


//root -b -q "../CMSSW_7_4_11/src/AnalysisMethods/macros/QGTagging/makeQGValidationTrees.C+()"
// 74X
//     singlemu-2015b-17jul15_ntuple_postproc.root    (data)     singlemu
//     jetht-2015b-17jul15_ntuple_postproc.root       (data)     jetht
//     jetht-2015b-pr_ntuple_postproc.root                       jetht2
//     dyjetstoll_mll50_10_ntuple_postproc.root       (MC)       dyjetstoll
//     qcd_ht1000to1500-50ns_1_ntuple_postproc.root   (MC)       qcd
void makeQGValidationTrees( TString sname            = "jetht2" // sample name
                          , const int     fileindex  = -1       // index of file (-1 means there is only 1 file for this sample)
                          , const bool    isMC       = false    // data or MC
                          , const TString fname      = "doublemu-2015d-pr_6_ntuple_postproc.root" // path of file to be processed
                          , const double xsec        = 1.0
                          , const string  outputdir  = "trees/test/"  // directory to which files will be written
                          , const TString fileprefix = "/eos/uscms/store/user/hqu/13TeV/101315/merged/"
                          , const TString json       = "Cert_246908-258159_13TeV_PromptReco_Collisions15_25ns_JSON_v3.txt"
                          )
{

  printf("Processing file %d of %s sample with xsec %f\n", (fileindex > -1 ? fileindex : 0), sname.Data(), xsec);

  // Make sure the output has a unique name in case there are multiple files to process
  if(fileindex > -1) sname += TString::Format("_%d",fileindex);

  TString fullname = fileprefix+fname;

  // Adjustments to default configuration
  cfgSet::loadDefaultConfigurations();
  cfgSet::setJSONFile(TString::Format("%s/src/data/JSON/%s", getenv("CMSSW_BASE"), json.Data()));
  cfgSet::ConfigSet qgv_search_set = cfgSet::zl_search_set;;
  //qgv_search_set.jets            = cfgSet::zl_search_jets;
  qgv_search_set.selectedLeptons = cfgSet::zl_ctr_leptons;
  qgv_search_set.selectedPhotons = cfgSet::zl_sel_photons;
  //qgv_search_set.corrections     = cfgSet::standardCorrections;
  qgv_search_set.corrections.eventCorrectionFile = TString::Format("%s/src/data/corrections/eventCorr_allData_600.root",getenv("CMSSW_BASE"));

  //cfgSet::ConfigSet pars = qgv_search_set;

  qgv_search_set.jets.minPt  = 20.0;
  qgv_search_set.jets.maxEta =  5.0;
  qgv_search_set.jets.minBJetPt  = 20.0;
  qgv_search_set.jets.maxBJetEta =  5.0;
  qgv_search_set.jets.cleanJetsvSelectedPhotons = true;
  qgv_search_set.jets.cleanJetsvSelectedLeptons = true;

  qgv_search_set.selectedLeptons.selectedMuon = (&MuonF::isgoodpogmuon);
  qgv_search_set.selectedLeptons.minMuPt  = 10.0;
  qgv_search_set.selectedLeptons.maxMuEta =  5.0;
  qgv_search_set.selectedLeptons.selectedElectron = (&ElectronF::isgoodpogelectron);
  qgv_search_set.selectedLeptons.minEPt  = 10.0;
  qgv_search_set.selectedLeptons.maxEEta =  5.0;

  //qgv_search_set.selectedPhotons = cfgSet::zl_sel_photons;
  qgv_search_set.selectedPhotons.minPt  = 10.0;
  qgv_search_set.selectedPhotons.maxEta =  5.0;

  string    treeName = "";
  if (isMC) treeName = "Events";
  else      treeName = "TestAnalyzer/Events";
  Analyze a(fullname, "Events", isMC, &qgv_search_set, sname, outputdir);
  a.mctype_ = Analyze::OTHER;
  if      (sname.Contains("gjets")) a.mctype_ = Analyze::GJET;
  else if (sname.Contains("qcd"))   a.mctype_ = Analyze::QCD;
  a.analyze(10000);
  //a.analyze(1000,10000);

} // makeQGValidationTrees()
