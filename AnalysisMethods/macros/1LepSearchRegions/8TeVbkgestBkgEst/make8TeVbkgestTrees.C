#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TSystem.h>
#include <TFile.h>
#include <TH1F.h>
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"
#include "AnalysisTools/KinematicVariables/interface/mt2w.h"
#include "AnalysisTools/KinematicVariables/interface/chi2.h"
#include "Math/VectorUtil.h"

using namespace std;
using namespace ucsbsusy;

class Analyze : public BaseTreeAnalyzer{
public:

  Analyze(TString fname, string treeName, bool isMCTree, cfgSet::ConfigSet *pars, TString sname, TString outputdir)
         : BaseTreeAnalyzer(fname, treeName, isMCTree, pars)
  {
    sample_       = sname;
    weight_       = -9 ;
    npv_          = -9 ;
    rho_          = -9 ;
    sampType_     = -9 ;
    passtrige_    = false;
    passtrigmu_   = false;
    met_          = -9 ;
    nvetotau_     = -9 ;
    nvetolep_     = -9 ;
    nselelep_     = -9 ;
    njets_        = -9 ;
    nbjets_       = -9 ;
    ngenlep_      = -9 ;
    ngenels_      = -9 ;
    ngenmus_      = -9 ;
    ngentau_      = -9 ;
    ht_           = -9 ;
    mt_           = -9 ;
    dphilw_       = -9 ;
    mt2w_         = -9 ;
    hadchi2_      = -9 ;
    dphij1met_    = -9 ;
    dphij2met_    = -9 ;
    dphij12met_   = -9 ;
    dilepinvmass_ = -9 ;
    opsignlep_    = false;
    lep1pt_       = -9 ;
    lep2pt_       = -9 ;
    lep1eta_      = -9 ;
    lep2eta_      = -9 ;
    lep1pdgid_    = -9 ;

    // initiliaze tree
    gSystem->mkdir(outputdir,true);
    fout = new TFile (outputdir+"/"+sname+"_tree.root","RECREATE");
    fout->cd();
    outtree = new TTree("Events","analysis tree");
    outtree->Branch( "weight"       , &weight_       ,       "weight/F" );
    outtree->Branch( "npv"          , &npv_          ,          "npv/I" );
    outtree->Branch( "rho"          , &rho_          ,          "rho/F" );
    outtree->Branch( "sampType"     , &sampType_     ,     "sampType/I" );
    outtree->Branch( "passtrige"    , &passtrige_    ,    "passtrige/O" );
    outtree->Branch( "passtrigmu"   , &passtrigmu_   ,   "passtrigmu/O" );
    outtree->Branch( "met"          , &met_          ,          "met/F" );
    outtree->Branch( "nvetotau"     , &nvetotau_     ,     "nvetotau/I" );
    outtree->Branch( "nvetolep"     , &nvetolep_     ,     "nvetolep/I" );
    outtree->Branch( "nselelep"     , &nselelep_     ,     "nselelep/I" );
    outtree->Branch( "njets"        , &njets_        ,        "njets/I" );
    outtree->Branch( "nbjets"       , &nbjets_       ,       "nbjets/I" );
    outtree->Branch( "ngenlep"      , &ngenlep_      ,      "ngenlep/I" );
    outtree->Branch( "ngenels"      , &ngenels_      ,      "ngenels/I" );
    outtree->Branch( "ngenmus"      , &ngenmus_      ,      "ngenmus/I" );
    outtree->Branch( "ngentau"      , &ngentau_      ,      "ngentau/I" );
    outtree->Branch( "ht"           , &ht_           ,           "ht/F" );
    outtree->Branch( "mt"           , &mt_           ,           "mt/F" );
    outtree->Branch( "dphilw"       , &dphilw_       ,       "dphilw/F" );
    outtree->Branch( "mt2w"         , &mt2w_         ,         "mt2w/F" );
    outtree->Branch( "hadchi2"      , &hadchi2_      ,      "hadchi2/F" );
    outtree->Branch( "dphij1met"    , &dphij1met_    ,    "dphij1met/F" );
    outtree->Branch( "dphij2met"    , &dphij2met_    ,    "dphij2met/F" );
    outtree->Branch( "dphij12met"   , &dphij12met_   ,   "dphij12met/F" );
    outtree->Branch( "dilepinvmass" , &dilepinvmass_ , "dilepinvmass/F" );
    outtree->Branch( "opsignlep"    , &opsignlep_    ,    "opsighlep/O" );
    outtree->Branch( "lep1pt"       , &lep1pt_       ,       "lep1pt/F" );
    outtree->Branch( "lep2pt"       , &lep2pt_       ,       "lep2pt/F" );
    outtree->Branch( "lep1eta"      , &lep1eta_      ,      "lep1eta/F" );
    outtree->Branch( "lep2eta"      , &lep2eta_      ,      "lep2eta/F" );
    outtree->Branch( "lep1pdgid"    , &lep1pdgid_    ,    "lep1pdgid/I" );

  }; // Analyze()

  virtual ~Analyze() {
    fout->cd();
    outtree->Write();
    fout->Close();
  } // ~Analyzer()

  virtual void loadVariables(){
    load(cfgSet::EVTINFO);
    load(cfgSet::AK4JETS);
    load(cfgSet::ELECTRONS);
    load(cfgSet::MUONS);
    load(cfgSet::PFCANDS);   // load pfcands for track vetoes
    load(cfgSet::TAUS);      // for tau vetoes
    load(cfgSet::TRIGOBJS);
    if(isMC()) load(cfgSet::GENPARTICLES);
  } // loadVariables()

  void runEvent(){

    if(!isMC() && hasJSONFile() && !passesLumiMask()) return;
    if(nPV<1)                    return;
    if(!goodvertex)              return;
    if(selectedLeptons.size()<1) return;
    if(jets.size()<2)            return;
    // not applied event selections:
    //   - Veto events containing an additional electron or muon with pT > 20 GeV, abs(eta) < 2.4 passing the analysis lepton selection
    //   - Require at least one of the jets is b-tagged based on the CSVv2+IVF algorithm, medium working point
    //   - Require MET >  80 GeV
    //   - MT(lep,MET) > 100 GeV
    //   - jets.size() >= 4

    weight_ = weight;
    npv_    = nPV;
    rho_    = rho;
    sampType_ = 0; // MC
    if(!isMC()) {
      if(sample_.Contains("singlee") ) sampType_ = 1; // e
      if(sample_.Contains("singlemu")) sampType_ = 2; // mu
    } // !MC

    auto lep1 = selectedLeptons[0];
    lep1pt_   = lep1->pt();
    lep1eta_  = lep1->eta();

    passtrige_  = isMC() ? triggerflag & kHLT_Ele32_eta2p1_WP75_Gsf : triggerflag & kHLT_Ele32_eta2p1_WPLoose_Gsf ;
    passtrigmu_ = triggerflag & kHLT_IsoTkMu24_eta2p1;
    //if(!passtrige_ && !passtrigmu_) return;

    // check for triggering objects
    bool foundTrgObj = false;
    for(auto* to : triggerObjects) {
      if(passtrige_) {
        if( (to->filterflags() & kSingleEle32) && ((to->pathflags() & kHLT_Ele32_eta2p1_WP75_Gsf)||(to->pathflags() & kHLT_Ele32_eta2p1_WPLoose_Gsf)) ) {
          auto lep = selectedLeptons[0];
          if ( lep->iselectron() && PhysicsUtilities::deltaR(*to,*lep)<0.4 ) foundTrgObj = true;
        } // foundTrgObj
      } // passtrige
      if(passtrigmu_) {
        if( (to->filterflags() & kSingleIsoTkMu24) && (to->pathflags() & kHLT_IsoTkMu24_eta2p1) ) {
          auto lep = selectedLeptons[0];
          if ( lep->ismuon() && PhysicsUtilities::deltaR(*to,*lep)<0.4 ) foundTrgObj = true;
        } // foundTrgObj
      } // passtrigmu
    } // triggerObjects
    if(passtrige_  && !foundTrgObj) passtrige_  = false;
    if(passtrigmu_ && !foundTrgObj) passtrigmu_ = false;
    if(!passtrige_ && !passtrigmu_) return;

    met_      = met->pt();
    nvetotau_ = nVetoHPSTaus;
    nvetolep_ = nVetoedLeptons;
    nselelep_ = nSelLeptons;
    njets_    = int(jets.size());
    nbjets_   = int(bJets.size());
    ht_       = JetKinematics::ht(jets, 30.0, 2.4);
    mt_       = JetKinematics::transverseMass(*lep1, *met);

    MomentumF* W = new MomentumF(lep1->p4() + met->p4());
    dphilw_ = fabs(PhysicsUtilities::deltaPhi(*lep1, *W));

    // conversions required for mt2w, chi2
    vector<float> csvvec;         // vector of the csv's of jets
    vector<LorentzVector> lzjets; // jets converted to lorentzvectors
    vector<float> sigma;          // vector of the number 0.1
    vector<bool > btag;           // bool vector of btags of jets
    LorentzVector lepvec;         // convert lep to lorentzvector
    LorentzVector templz;
    lepvec=lep1->p4();

    // calculate csv and jets conversions, sigma (jet resolution), btag vector
    for(unsigned int n = 0; n < jets.size(); ++n){
      csvvec.push_back(jets[n]->csv());
      templz = jets[n]->p4();         // convert from RecoJetF to LorentzVector
      lzjets.push_back(templz);
      sigma.push_back(0.1);       // per twiki, use flat jet resolution of %10
      bool bjet=false;
      if(jets[n]->csv() > defaults::CSV_MEDIUM) bjet=true;
      btag.push_back(bjet);
    } // jets
    // form ibjets and inonbjets, which are indices into jets (Pieter's code)
    // ibjets is filled with indices of all the bjets
    // whereas inonbjets is constrained in size: ibjets.size+inonbjets.size()<=3
    vector<pair<double,int> > rankedJets(jets.size());
    for(unsigned int iJ =0; iJ < jets.size(); ++iJ){
      rankedJets[iJ].first = btag[iJ];
      rankedJets[iJ].second = iJ;
    } // jets
    std::sort(rankedJets.begin(),rankedJets.end(),PhysicsUtilities::greaterFirst<double,int>());
    vector<int> ibjets;
    vector<int> inonbjets;
    for(unsigned int iJ =0; iJ < rankedJets.size(); ++iJ){
      if(rankedJets[iJ].first>defaults::CSV_MEDIUM)                ibjets.push_back(rankedJets[iJ].second);
      else if(ibjets.size()<2 && ibjets.size()+inonbjets.size()<3) inonbjets.push_back(rankedJets[iJ].second);
    } // ranked jets
    mt2w_    = calculateMT2w(lzjets, ibjets, inonbjets, lepvec, met->pt(), met->phi());
    hadchi2_ = calculateChi2(lzjets, sigma, btag);
    dphij1met_  = fabs(PhysicsUtilities::deltaPhi(*jets[0], *met));
    dphij2met_  = fabs(PhysicsUtilities::deltaPhi(*jets[1], *met));
    dphij12met_ = min(dphij1met_,dphij2met_);

    if(vetoedLeptons.size()>=2) {
      auto lep2 = vetoedLeptons[1];
      lep2pt_   = lep2->pt();
      lep2eta_  = lep2->eta();
      dilepinvmass_ = (lep1->p4() + lep2->p4()).mass();
      if(lep1->q()!=lep2->q()) opsignlep_ = true;
    } // >=2 leps

    lep1pdgid_ = lep1->pdgid();

    if(isMC()) {
      int ngenleps = 0, ngeneles = 0, ngenmus = 0, ngentaus = 0;
      for(auto* p : genParts) {
        const GenParticleF * mom = 0;
        const GenParticleF * gmom = 0;
        if(p->numberOfMothers()==0) continue;
        mom = p->mother(0);
        if(mom->numberOfMothers()==0) continue;
        gmom = mom->mother(0);
        while(abs(gmom->pdgId()) == 24 && gmom->numberOfMothers() > 0) gmom = gmom->mother(0);
        if((abs(p->pdgId()) == 11 || abs(p->pdgId()) == 13 || abs(p->pdgId()) == 15) && abs(mom->pdgId()) == 24 && abs(gmom->pdgId()) == 6) {
          ngenleps++;
          if(abs(p->pdgId()) == 11) ngeneles++;
          if(abs(p->pdgId()) == 13) ngenmus++;
          if(abs(p->pdgId()) == 15) ngentaus++;
        } // p=e,mu,tau and gmom=W,t
      } // genParts
      ngenlep_ = ngenleps;
      ngenels_ = ngeneles;
      ngenmus_ = ngenmus ;
      ngentau_ = ngentaus;
    } // isMC

    outtree->Fill();

  } // runEvent()

  //void out(TString outputPath){} // out()

  TString sample_    ;

  TFile *fout    ;
  TTree *outtree ;

  // variables for trees
  float weight_      ;
  int   npv_         ;
  float rho_         ;
  int   sampType_    ;
  bool  passtrige_   ;
  bool  passtrigmu_  ;
  float met_         ;
  int   nvetotau_    ;
  int   nvetolep_    ;
  int   nselelep_    ;
  int   njets_       ;
  int   nbjets_      ;
  int   ngenlep_     ;
  int   ngenels_     ;
  int   ngenmus_     ;
  int   ngentau_     ;
  float ht_          ;
  float mt_          ;
  float dphilw_      ;
  float mt2w_        ;
  float hadchi2_     ;
  float dphij1met_   ;
  float dphij2met_   ;
  float dphij12met_  ;
  float dilepinvmass_;
  bool  opsignlep_   ;
  float lep1pt_      ;
  float lep2pt_      ;
  float lep1eta_     ;
  float lep2eta_     ;
  int   lep1pdgid_   ;

}; // BaseTreeAnalyzer


#endif


//root -b -q "../CMSSW_7_4_7/src/AnalysisMethods/macros/1LepSearchRegions/make8TeVbkgestTrees.C+()"
// 74X
//     singlemu-2015b-pr_ntuple.root           (data)     singlemu
//     singleel-2015b-pr_ntuple.root           (data)     singleel
//     ttbar-madgraphmlm_1_ntuple_wgtxsec.root (MC)       ttbar_0
void make8TeVbkgestTrees( TString sname            = "ttbar_0" // sample name
                        , const int     fileindex  = 1       // index of file (-1 means there is only 1 file for this sample)
                        , const bool    isMC       = true    // data or MC
                        , const TString fname      = "ttbar-madgraphmlm_1_ntuple_wgtxsec.root" // path of file to be processed
                        , const double  xsec       = 1.0
                        , const string  outputdir  = "trees/test/"  // directory to which files will be written
                        , const TString fileprefix = "/eos/uscms/store/user/vdutta/13TeV/210715/merged/"
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

  cfgSet::loadDefaultConfigurations();
  cfgSet::ConfigSet pars = cfgSet::ol_search_set;
  pars.jets.cleanJetsvVetoedLeptons = true;

  string treeName = isMC ? "Events" : "TestAnalyzer/Events";
  Analyze a(fullname, treeName, isMC, &pars, sname, outputdir);
  a.analyze(10000);
  //a.analyze(10000,100000);

} // make8TeVbkgestTrees()
