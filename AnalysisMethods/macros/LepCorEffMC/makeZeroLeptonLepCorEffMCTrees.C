/*
 lepton iso/id MC efficiencies
 feed getLepCorEffs.C with these trees

 two ways to do this:
   1) turn off iso cuts in lep configs for both id & iso effs, then define a set of
      variables with the iso turned back on, just for iso effs.
      then we have eff = (nvetoX>=1)/(ngoodgenX>=1) for X=ele/mu
      with special treatment for muon id (num = ngenmumatched>=1 instead)
      pros: none? cons: have to predict which iso to turn on (?) -- maintenance.
   2) turn off iso cuts for just id eff. OK, eff formula is as above. 
      then for just iso effs, leave the iso cut and define a set of variables
      with the iso cut turned off (easy), using these in the denominator, and the normal
      versions of the vars (with iso on) in the numerator.

 we do option 2 below. the new variables are prefixed "isoincAL".

 new variables filled by this macro (as opposed to making undocumented, inline changes to ZeroHelper...)
   gen{mu,ele}{pt,eta} - of 'good' gen particles, same as ngoodgen{mu,el} calculation
   ngen{mu,el}matched  - # of those 'good' gen particles matched to a selectedLepton with dR 0.4
   isoincALX          - iso inclusive (no iso cuts), drawing from allLeptons
   allX                - drawing from allLeptons. {mu,ele}{pt,eta} usually draw from selectedLeptons.

 selectedLeptons = primaryLeptons + secondaryLeptons. these draw from the lep configs.
 allLeptons = everything from electronReader, not drawing from lep configs.
*/

#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/macros/0LepSearchRegions/ZeroLeptonTreeHelper.hh"
#endif

using namespace ucsbsusy;

class LepCorAnalyzer : public ZeroLeptonAnalyzer {

  public :
    LepCorAnalyzer(TString fileName, TString treeName, TString outfileName, size randomSeed, bool isMCTree, cfgSet::ConfigSet *pars) :
      ZeroLeptonAnalyzer(fileName, treeName, outfileName, randomSeed, isMCTree, pars) {
    }

    const double metcut_   = 200.0 ;
    const int    minnjets_ =   2   ;

    bool isOneLepTTbar = false;
    bool applyCHFFilter    = false ;

    size i_nvetoele;
    size i_nvetomu;
    size i_genmupt;
    size i_genmueta;
    size i_genelept;
    size i_geneleeta;
    size i_ngenmumatched;
    size i_ngenelematched;
    // vars drawing from allLeptons
    size i_allelept; // redundant in its use, here for clarity
    size i_alleleeta;
    size i_allmupt;
    size i_allmueta;
    size i_idisoincALelept; // redundant in its use, here for clarity
    size i_idisoincALeleeta;
    size i_idisoincALmupt;
    size i_idisoincALmueta;
   // vars with iso cuts turned off
    size i_isoincALelept;
    size i_isoincALeleeta;
    size i_isoincALelehtalong;
    size i_isoincALeleannulus;
    size i_isoincALreleleannulus;
    size i_isoincALmupt;
    size i_isoincALmueta;
    size i_isoincALmuhtalong;
    size i_isoincALmuannulus;
    size i_isoincALrelmuannulus;

    //TreeFiller filler; // basic/extra filler made by mother class
    virtual ~LepCorAnalyzer() {}

    void book() {
      filler.book(&data);
      //extraFiller.bookTest(&data);
      extraFiller.bookSyst(&data);
      //extraFiller.bookJetMET(&data);
      //extraFiller.bookLepton(&data);
      //extraFiller.bookGen(&data);
      i_nvetoele = data.add<int>("","nvetoele","I",0);
      i_nvetomu  = data.add<int>("","nvetomu","I",0);
      i_genmupt     = data.addMulti<float>("","genmupt",0);
      i_genmueta    = data.addMulti<float>("","genmueta",0);
      i_genelept    = data.addMulti<float>("","genelept",0);
      i_geneleeta   = data.addMulti<float>("","geneleeta",0);
      i_ngenmumatched = data.add<int>("","ngenmumatched","I",0);
      i_ngenelematched = data.add<int>("","ngenelematched","I",0);
      i_allelept  = data.addMulti<float>("","allelept",0);
      i_alleleeta = data.addMulti<float>("","alleleeta",0);
      i_allmupt = data.addMulti<float>("","allmupt",0);
      i_allmueta = data.addMulti<float>("","allmueta",0);
      i_idisoincALelept  = data.addMulti<float>("","idisoincALelept",0);
      i_idisoincALeleeta = data.addMulti<float>("","idisoincALeleeta",0);
      i_idisoincALmupt = data.addMulti<float>("","idisoincALmupt",0);
      i_idisoincALmueta = data.addMulti<float>("","idisoincALmueta",0);
      // vars with iso cuts turned off
      i_isoincALelept  = data.addMulti<float>("","isoincALelept",0);
      i_isoincALeleeta = data.addMulti<float>("","isoincALeleeta",0);
      i_isoincALelehtalong = data.addMulti<float>("","isoincALelehtalong",0);
      i_isoincALeleannulus = data.addMulti<float>("","isoincALeleannulus",0);
      i_isoincALreleleannulus = data.addMulti<float>("","isoincALreleleannulus",0);
      i_isoincALmupt = data.addMulti<float>("","isoincALmupt",0);
      i_isoincALmueta = data.addMulti<float>("","isoincALmueta",0);
      i_isoincALmuhtalong = data.addMulti<float>("","isoincALmuhtalong",0);
      i_isoincALmuannulus = data.addMulti<float>("","isoincALmuannulus",0);
      i_isoincALrelmuannulus = data.addMulti<float>("","isoincALrelmuannulus",0);
    }

    bool fillEvent() {

      if(applyCHFFilter && !cfgSet::passCHFFilter(jets)) return false;

      if(!goodvertex) return false;
      if(nJets     < minnjets_) return false;
      if(met->pt() < metcut_  ) return false;


      // alternative: turn off iso in both, then for iso eff, make vars with it on.
      //   pros: none? cons: have to predict which iso to turn on (maintenance)
      //
      // for iso eff, use special vars with iso turned off (prefix: isoincAL)
      //   change to lep configs: NONE. keep both iso + id.
      //   2d var: isoincALXpt, isoincALXeta/isoincALrelXannulus (X=ele/mu)
      //   den = ngoodgenX>=1
      //   num = den + nvetoX>=1 (id + iso; NB: nvetoX from selectedLeptons = primary+secondary)
      //             FOR MUONS den + ngenmumatched>=1 (NB: uses selectedLeptons = prim + sec)
      //
      // for id eff, use normal variables, but change the lep configs to be isoincAL
      //   change to lep configs: make iso all-inclusive; keep id
      //   2d var: Xpt, Xeta/relXannulus
      //   den = ngoodgenX>=1
      //   num = den + nvetoX>=1 (var has id + isoincAL; NB above)
      //   
      if(this->allLeptons.size() > 0) {
        const cfgSet::ConfigSet& cfgs = this->getAnaCfg();
        LeptonSelection::Electron elConfisoincAL = cfgs.electrons;
        LeptonSelection::Muon     muConfisoincAL = cfgs.muons;
        LeptonSelection::Electron elConfidisoincAL = cfgs.electrons;
        LeptonSelection::Muon     muConfidisoincAL = cfgs.muons;
        elConfisoincAL.passISO = &ElectronISO::inclusive;
        muConfisoincAL.passISO = &MuonISO::inclusive;
        elConfidisoincAL.passISO = &ElectronISO::inclusive;
        muConfidisoincAL.passISO = &MuonISO::inclusive;
        elConfidisoincAL.passID = &ElectronID::inclusive;
        muConfidisoincAL.passID = &MuonID::inclusive;
        for(auto* l : this->allLeptons) {
          bool cleanHt = false;
          float htalong = 0;
          for(const auto* jet : this->jets) {
            if(PhysicsUtilities::absDeltaPhi(*jet,*l) < TMath::PiOver2()) {
              htalong += jet->pt();
              if(PhysicsUtilities::deltaR(*jet,*l) < 0.4) cleanHt = true;
            }
          }
          if(cleanHt) htalong -= l->pt();
          // new iso inclusive vars
          if(l->ismuon() && cfgSet::isSelMuon(*(MuonF*)l, muConfisoincAL)) {
            data.fillMulti<float>(i_isoincALmupt, l->pt());
            data.fillMulti<float>(i_isoincALmueta, l->eta());
            data.fillMulti<float>(i_isoincALmuhtalong, htalong);
            data.fillMulti<float>(i_isoincALmuannulus, l->pt()*l->annulusactivity());
            data.fillMulti<float>(i_isoincALrelmuannulus, l->annulusactivity());
          } else if(cfgSet::isSelElectron(*(ElectronF*)l, elConfisoincAL)) { // iselectron() is implied since allLeptons req's ele or mu
            data.fillMulti<float>(i_isoincALelept, l->pt());
            data.fillMulti<float>(i_isoincALeleeta, l->eta());
            data.fillMulti<float>(i_isoincALelehtalong, htalong);
            data.fillMulti<float>(i_isoincALeleannulus, l->pt()*l->annulusactivity());
            data.fillMulti<float>(i_isoincALreleleannulus, l->annulusactivity());
          }
          // new vars, no changes to lep config, using allLeptons instead of selectedLeptons
          if(l->ismuon() && cfgSet::isSelMuon(*(MuonF*)l, cfgs.muons)) {
            data.fillMulti<float>(i_allmupt, l->pt());
            data.fillMulti<float>(i_allmueta, l->eta());          
          } else if(cfgSet::isSelElectron(*(ElectronF*)l, cfgs.electrons)) { // iselectron() is implied since allLeptons req's ele or mu
            data.fillMulti<float>(i_allelept, l->pt());
            data.fillMulti<float>(i_alleleeta, l->eta());
          }
          // new vars, id and iso inclusive,  using allLeptons instead of selectedLeptons
          if(l->ismuon() && cfgSet::isSelMuon(*(MuonF*)l, muConfidisoincAL)) {
            data.fillMulti<float>(i_idisoincALmupt, l->pt());
            data.fillMulti<float>(i_idisoincALmueta, l->eta());
          } else if(cfgSet::isSelElectron(*(ElectronF*)l, elConfidisoincAL)) { // iselectron() is implied since allLeptons req's ele or mu
            data.fillMulti<float>(i_idisoincALelept, l->pt());
            data.fillMulti<float>(i_idisoincALeleeta, l->eta());
          }
        }
      }

      ///////// fillGenInfo ///////////
      // gen el/mu counting and matching. most copied from zerohelper
      std::vector<float> gentoppt_; gentoppt_.clear();
      //int nGoodGenMu = 0; int nGoodGenEle = 0; int nPromptTaus = 0;
      int nGenMuMatched = 0;
      int ngenelematched = 0;
      GenParticleF * stop1 = 0;
      GenParticleF * stop2 = 0;
      if(this->isMC()) {
        std::vector<const LeptonF*> recMu_;
        std::vector<const LeptonF*> recEl_;
        for(const auto* lep : this->selectedLeptons) {
          if(lep->pdgid()==13){
            recMu_.push_back(lep);
          }else if(lep->pdgid()==11){
            recEl_.push_back(lep);
          }
        }
        data.fill<int  >(i_nvetomu, recMu_.size());
        data.fill<int  >(i_nvetoele, recEl_.size());
        for(auto* p : this->genParts) {
          if(ParticleInfo::isA(ParticleInfo::p_stop1, p) and !stop1)
            stop1 = p;
          else if(ParticleInfo::isA(ParticleInfo::p_stop1, p) and !stop2)
            stop2 = p;
          const GenParticleF * genPartMom = 0;

          if (abs(p->pdgId())==6) { float tmppt_ = p->p4().pt(); gentoppt_.push_back(tmppt_); }

          if (p->numberOfMothers()>0) 
            genPartMom = p->mother(0);
          else
            continue;
          if (ParticleInfo::isA(ParticleInfo::p_eminus, p) && (ParticleInfo::isA(ParticleInfo::p_Z0, genPartMom) || ParticleInfo::isA(ParticleInfo::p_Wplus, genPartMom) || ParticleInfo::isA(ParticleInfo::p_tauminus, genPartMom))) {
            //nGoodGenEle++;
            data.fillMulti<float>(i_genelept, p->pt());
            data.fillMulti<float>(i_geneleeta, p->eta());
            double nearDR = 0;
            int near = PhysicsUtilities::findNearestDRDeref(*p, recEl_, nearDR, 0.4);
            if(near>=0) ++ngenelematched;
          }
          else if (ParticleInfo::isA(ParticleInfo::p_muminus, p) && (ParticleInfo::isA(ParticleInfo::p_Z0, genPartMom) || ParticleInfo::isA(ParticleInfo::p_Wplus, genPartMom) || ParticleInfo::isA(ParticleInfo::p_tauminus, genPartMom))) {
            //nGoodGenMu++;
            data.fillMulti<float>(i_genmupt, p->pt());
            data.fillMulti<float>(i_genmueta, p->eta());
            double nearDR = 0;
            int near = PhysicsUtilities::findNearestDRDeref(*p, recMu_, nearDR, 0.4);
            if(near>=0) ++nGenMuMatched;
          }
          else if (ParticleInfo::isA(ParticleInfo::p_tauminus, p) && (ParticleInfo::isA(ParticleInfo::p_Z0, genPartMom) || ParticleInfo::isA(ParticleInfo::p_Wplus, genPartMom))) {
            bool lepDecay = false;
            for(unsigned int itd = 0; itd < p->numberOfDaughters(); itd++) {
              const GenParticleF* dau = p->daughter(itd);
              if(ParticleInfo::isA(ParticleInfo::p_eminus, dau) || ParticleInfo::isA(ParticleInfo::p_muminus, dau)) lepDecay = true;
            }
            //if(!lepDecay)
              //nPromptTaus++;
          }
        }
      }
      data.fill<int  >(i_ngenmumatched, nGenMuMatched);
      data.fill<int  >(i_ngenelematched, ngenelematched);

      filler.fillEventInfo(&data, this);
      //filler.fillJetInfo  (&data, jets, bJets, met);

      //extraFiller.fillTestVars(&data, this);
      extraFiller.fillSystInfo(&data, this);
      //extraFiller.fillLeptonInfo(&data, this);
      //extraFiller.fillGenInfo(&data, this);

      return true;
    }


};//lepcoranalyzer

void makeZeroLeptonLepCorEffMCTrees(TString sname = "T2tt_750_100",
                           const int fileindex = -1,
                           const bool isMC = true,
                           const TString fname = "/store/user/apatters/13TeV/271115/merged/T2tt_750_100_ntuple_postproc.root",
                           const TString outputdir = "trees",
                           const TString fileprefix = "root://cmseos:1094/",
                           const TString json=TString::Format("%s/src/data/JSON/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_v2.txt",getenv("CMSSW_BASE")))
{

  printf("Processing file %d of %s sample\n", (fileindex > -1 ? fileindex : 0), sname.Data());

  if(fileindex > -1)
    sname += TString::Format("_%d",fileindex);

  TString fullname = fileprefix+fname;

  gSystem->mkdir(outputdir,true); // only works in interactive. in condor outputdir = '.'
  TString outfilename = outputdir+"/"+sname+"_tree.root";

  // manually, or through the filename, set the options (four possibilities)
  // if running on condor, note outputdir = '.', so we cannot use the name.
  // can __always__ change manually between submissions using first two lines.
  bool isSR = true; //CHANGEME
  bool isId = true;
  //bool isSR = outputdir.Contains("/sr"); // only works interactively
  //bool isId = outputdir.Contains("/id");

  // lepton configurations
  // for id & iso effs, loosen kinematics. for id, remove iso cut. for iso, leave everything.
  std::cout << "*** Options: isId, isSR: " << isId << " " << isSR << std::endl;
  std::cout << "*** Changing lep config to : " << ((isSR) ? "zl_search_set (SR)" : "zl_lepton_set (CR)") << std::endl;
  std::cout << "*** relaxing pt/eta requirements to 5/2.4" << std::endl;
  cfgSet::ConfigSet pars = (isSR) ? pars0lep(json) : pars0lepDiLepCR(json);
  if((!isSR) && (pars.electrons.type != LeptonSelection::ZL_CTR_ELE)) std::cout << "Houston, we have a problem. Ele type for CR is not ZL_CTR_ELE" << std::endl;
  pars.electrons.maxETA  = 2.4;
  pars.electrons.minPT   = 5.0;
  pars.muons.maxETA      = 2.4;
  pars.muons.minPT       = 5.0;
  if(isId) {
    std::cout << "*** changing lep config to remove iso cut" << std::endl;
    pars.electrons.passISO = &ElectronISO::inclusive; // if Id eff, let numerator leps be inclusive in Iso
    pars.muons.passISO     = &MuonISO::inclusive;     // if Iso eff, require numerator leps to pass Iso
    //pars.electrons.passID  = &ElectronID::inclusive; // help, I'm lost!
    //pars.muons.passID      = &MuonID::inclusive;
  }

  std::cout << std::endl << "*****************************************" << std::endl;
  std::cout << "Handling : " << ((isId) ? "Id" : "Iso") << " effs for the " << ((isSR) ? "SR (veto) region lepton WPs" : "CR lepton WPs") << std::endl;

  // disable JetID for signal samples
  if (sname.Contains("T2tt") || sname.Contains("T2tb") || sname.Contains("T2bW") || sname.Contains("T2fbd") || sname.Contains("T2cc")) pars.jets.applyJetID = false;
  pars.corrections.ttbarCorrections |= ucsbsusy::TtbarCorrectionSet::TOPPAIRPT;

  TString treeName = "Events";
  LepCorAnalyzer a(fullname, treeName, outfilename, fileindex+2, isMC, &pars);

  // CHF filter for FastSim
  if (sname.Contains("T2tt") || sname.Contains("T2tb") || sname.Contains("T2bW") || sname.Contains("T2fbd") || sname.Contains("T2cc")) a.applyCHFFilter = true;

  a.analyze(10000);
  //a.analyze(10000,10000);

}//mainfxn
