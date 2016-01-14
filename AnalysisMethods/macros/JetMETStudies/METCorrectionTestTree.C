#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"

using namespace std;
using namespace ucsbsusy;



class Copier : public TreeCopierManualBranches {
public:
  Copier(string fileName, string treeName, string outFileName, bool isMCTree, cfgSet::ConfigSet * pars,double inResScale )
: TreeCopierManualBranches(fileName,treeName,outFileName,isMCTree,pars), resScale(inResScale)
  {};
  virtual ~Copier() {};

  virtual void loadVariables(){
    load(cfgSet::EVTINFO);
    if(isMC())load(cfgSet::AK4JETS,JetReader::LOADRECO | JetReader::LOADGEN | JetReader::FILLOBJ);
    else load(cfgSet::AK4JETS,JetReader::LOADRECO | JetReader::FILLOBJ);
    load(cfgSet::ELECTRONS);
    load(cfgSet::MUONS);
    load(cfgSet::TRIGOBJS);
    if(isMC())load(cfgSet::GENPARTICLES);
  }

  virtual bool fillEvent() {

    //basic cleaning
    if(!goodvertex) return false;
    if(!evtInfoReader.cscFlt) return false;
    if(!evtInfoReader.eeBadSCFlt) return false;
    if(!evtInfoReader.hbheFixFlt) return false;

    if(!isMC() && hasJSONFile() && !passesLumiMask())
      return false;

    //Selection
    bool passTrig = isMC()  ? true : triggerflag & kHLT_DiCentralPFJet55_PFMET110_NoiseCleaned ;
    if(!passTrig) return false;

    if(nSelLeptons == 0) return false;
    if(nJets < 3) return false;
    if(nBJets < 1) return false;
    if(jets[1]->pt() < 60) return false;



    CylLorentzVectorF metCorr = met->p4();
    double zLLTruePT = 0;
    const GenParticleF * genZ = 0;
    for(const auto * g : genParts ){
      if(TMath::Abs(g->pdgId()) == ParticleInfo::p_Z0){
        genZ = g;
        break;
      }
      if(TMath::Abs(g->pdgId()) == ParticleInfo::p_Wplus){
        bool leptonic = false;
        for(unsigned int iP = 0; iP < g->numberOfDaughters(); ++iP){
          if(ParticleInfo::isLeptonOrNeutrino(g->daughter(iP)->pdgId() ))
            leptonic = true;
        }
        if(leptonic){
          if(genZ == 0)
            genZ = g;
          else if(g->pt() > genZ->pt())
            genZ = g;
        }

      }
    }
    if(genZ) zLLTruePT = genZ->pt();
    if(isMC() && resScale > 0)
    for(unsigned int iJ = 0; iJ < jets.size(); ++iJ){
      if(jets[iJ]->genJet() == 0) continue;
      if(jets[iJ]->genJet()->pt() < 20 ) continue;
      double newPT = max( jets[iJ]->genJet()->pt() + resScale*(jets[iJ]->pt() - jets[iJ]->genJet()->pt()), 0. );
      metCorr += jets[iJ]->p4();
      jets[iJ]->setP4( CylLorentzVectorF(newPT,jets[iJ]->eta(), jets[iJ]->phi(),jets[iJ]->mass() ) );
      metCorr -= jets[iJ]->p4();
    }



    CylLorentzVectorF recoW = metCorr;
    recoW += selectedLeptons[0]->p4();


    if(metCorr.pt() < 200) return false;

    //variable processing
    if(isMC())
      weight *= .0404;
    else weight = 1;


    data.fill<float>(i_weight  ,weight );
    data.fill<float>(i_puWeight  ,eventCorrections.getPUWeight() );
    data.fill<unsigned int>(i_process ,process);
    data.fill<bool>(i_muon ,selectedLeptons[0]->ismuon());
    data.fill<float>(i_met     ,metCorr.pt());
    data.fill<int>(i_npv     ,nPV      );
    data.fill<int>(i_njets   ,nJets    );
    data.fill<int>(i_nbjets  ,nBJets   );
    data.fill<float>(i_mtw      , JetKinematics::transverseMass(selectedLeptons[0]->p4(), metCorr)    );
    data.fill<float>(i_ptw      ,recoW.pt()     );


    return true;
  }

  void book() {
    i_weight     = data.add<float>("","weight","F",0);
    i_puWeight   = data.add<float>("","puWeight","F",0);
    i_process    = data.add<unsigned int>("","process","i",0);
    i_muon       = data.add<bool>("","muon","O",0);
    i_met        = data.add<float>("","met","F",0);
    i_npv        = data.add<int  >("","npv","I",0);
    i_njets      = data.add<int  >("","njets","I",0);
    i_nbjets     = data.add<int  >("","nbjets","I",0);
    i_mtw         = data.add<float>("","mtw","F",0);
    i_ptw         = data.add<float>("","ptw","F",0);

  }


  //event level info
  size i_weight    ;
  size i_puWeight    ;
  size i_process   ;
  size i_muon      ;
  size i_met       ;
  size i_npv       ;
  size i_njets     ;
  size i_nbjets    ;
  size i_mtw        ;
  size i_ptw        ;

  double resScale;


};


#endif

void METCorrectionTestTree(string fileName,  string treeName = "Events", string outPostfix ="metCorr", bool isMC = true, bool inApplyScale = false, double inResScale = -1, double inMetParScale = -1, double inMetPerpScale = -1) {

  cfgSet::loadDefaultConfigurations();
  cfgSet::setJSONFile("/home/nmccoll/test/CMSSW_7_4_7/src/data/JSON/Cert_246908-251883_13TeV_PromptReco_Collisions15_JSON_v2.txt");
  cfgSet::ConfigSet cfg = cfgSet::ol_search_set;
  cfg.leptons.minMuPt  = 5;
  cfg.leptons.maxMuEta = 2.4;
  cfg.leptons.minEPt  = 5;
  cfg.leptons.maxEEta = 2.4;
  cfg.jets.minPt = 20;
  cfg.jets.minBJetPt = 20;


  if(inApplyScale)
    cfg.corrections.jetAndMETCorrections |= JetAndMETCorrectionSet::METSCALE;
  if(inMetParScale >0 || inMetPerpScale > 0)
    cfg.corrections.jetAndMETCorrections |= JetAndMETCorrectionSet::METRESOLUTION;



  //get the output name
  TString prefix(fileName);
  prefix.Remove(0,prefix.Last('/') + 1);
  if(prefix.First('.') >= 0) prefix.Resize(prefix.First('.'));
  TString outName = TString::Format("%s_%s.root",prefix.Data(),outPostfix.c_str());

  Copier a(fileName,treeName,outName.Data(),isMC, &cfg,inResScale);

  a.analyze();
}


/*
 *
 * Running
 *
 * root -b -q 'JetMETStudies/METCorrectionTestTree.C+("/data/nmccoll/13TeV/8_20_15_ZeroLep/SM_zeroLep.root","Events","metTestCorr",true,false,-1)' &
root -b -q 'JetMETStudies/METCorrectionTestTree.C+("/data/nmccoll/13TeV/8_20_15_ZeroLep/SM_zeroLep.root","Events","corr_metTestCorr",true,true,-1)' &
root -b -q 'JetMETStudies/METCorrectionTestTree.C+("/data/nmccoll/13TeV/8_20_15_ZeroLep/SM_zeroLep.root","Events","corr_scale_metTestCorr",true,true,-1,.97,.94)' &
root -b -q 'JetMETStudies/METCorrectionTestTree.C+("/data/nmccoll/13TeV/8_20_15_ZeroLep/htmht-2015b-pr_ntuple_postproc_zeroLep.root","Events","metTestCorr",false)' &
root -b -q 'JetMETStudies/METCorrectionTestTree.C+("/data/nmccoll/13TeV/8_20_15_ZeroLep/htmht-2015b-17jul15_ntuple_postproc_zeroLep.root","Events","metTestCorr",false)' &
 *
 */
