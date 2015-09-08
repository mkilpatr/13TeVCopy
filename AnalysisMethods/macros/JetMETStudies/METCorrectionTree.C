#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"


using namespace std;
using namespace ucsbsusy;



class Copier : public TreeCopierManualBranches {
public:
  Copier(string fileName, string treeName, string outFileName, bool isMCTree, cfgSet::ConfigSet * pars, double inResScale)
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
    bool passTrig = (triggerflag & kHLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ) || (triggerflag & kHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ) || (triggerflag & kHLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ);
    if(!passTrig) return false;

    if(nVetoedLeptons != 2) return false;
    if(nSelLeptons != 2) return false;
    if(selectedLeptons[0]->pt() < 20) return false;
    if(selectedLeptons[1]->pt() < (selectedLeptons[1]->iselectron() ? 15 : 10)  ) return false;
    if(selectedLeptons[0]->iselectron() != selectedLeptons[1]->iselectron()) return false;
    if(selectedLeptons[0]->q() == selectedLeptons[1]->q()) return false;

    CylLorentzVectorF zLL = selectedLeptons[0]->p4();
    zLL += selectedLeptons[1]->p4();
    if(zLL.mass()  < 80 || zLL.mass() > 100) return false;


    CylLorentzVectorF metCorr = met->p4();
    double zLLTruePT = 0;
    const GenParticleF * genZ = 0;
    for(const auto * g : genParts ){
      if(TMath::Abs(g->pdgId()) == ParticleInfo::p_Z0){
        genZ = g;
        break;
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



    float metPar  = metCorr.pt() * TMath::Cos( PhysicsUtilities::deltaPhi(metCorr,zLL) );
    float metPerp = metCorr.pt() * TMath::Sin( PhysicsUtilities::deltaPhi(metCorr,zLL) );
//
    float htPar = 0;
    float ht    = 0;
    float htInc = 0;

    CylLorentzVectorF mhtVec;
    CylLorentzVectorF mhtIncVec;
    nJets = 0;
    nBJets = 0;
    for(unsigned int iJ = 0; iJ < jets.size(); ++iJ){
      if(jets[iJ]->pt() < 20) continue;
      htInc += jets[iJ]->pt();
      mhtIncVec -= jets[iJ]->p4();
      if(TMath::Abs(jets[iJ]->eta()) > 2.4) continue;
      nJets++;
      if(cfgSet::isSelBJet(*jets[iJ],configSet.jets))nBJets++;
      htPar += jets[iJ]->pt() * TMath::Cos( PhysicsUtilities::deltaPhi(jets[iJ]->p4(),zLL));
      ht += jets[iJ]->pt();
      mhtVec -= jets[iJ]->p4();
    }

    float mht    = mhtVec.pt();
    float mhtInc = mhtIncVec.pt();
    float soft   = (metCorr - mhtIncVec).pt();


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
    data.fill<float>(i_ht      ,ht     );
    data.fill<float>(i_htInc   ,htInc     );
    data.fill<float>(i_mht      ,mht     );
    data.fill<float>(i_mhtInc   ,mhtInc     );
    data.fill<float>(i_soft    ,soft     );
    data.fill<float>(i_htAlong ,htPar);
    data.fill<float>(i_metPar  ,metPar );
    data.fill<float>(i_metPerp ,metPerp);
    data.fill<float>(i_zllpt   ,zLL.pt()  );
    data.fill<float>(i_zlleta  ,zLL.eta()  );
    data.fill<float>(i_zllTruept   ,zLLTruePT  );

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
    i_ht         = data.add<float>("","ht","F",0);
    i_htInc      = data.add<float>("","htInc","F",0);
    i_mht        = data.add<float>("","mht","F",0);
    i_mhtInc     = data.add<float>("","mhtInc","F",0);
    i_soft       = data.add<float>("","soft","F",0);
    i_htAlong    = data.add<float>("","htAlong","F",0);
    i_metPar     = data.add<float>("","metPar","F",0);
    i_metPerp    = data.add<float>("","metPerp","F",0);
    i_zllpt      = data.add<float>("","zllpt","F",0);
    i_zlleta     = data.add<float>("","zlleta","F",0);
    i_zllTruept  = data.add<float>("","i_zllTruept","F",0);

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
  size i_ht        ;
  size i_htInc     ;
  size i_mht       ;
  size i_mhtInc    ;
  size i_soft      ;
  size i_htAlong   ;
  size i_metPar    ;
  size i_metPerp   ;
  size i_zllpt     ;
  size i_zlleta    ;
  size i_zllTruept ;

  double resScale;

};


#endif

void METCorrectionTree(string fileName,  string treeName = "Events", string outPostfix ="metCorr", bool isMC = true, bool inApplyScale = false, double inResScale = -1, double inMetParScale = -1, double inMetPerpScale = -1) {

  cfgSet::loadDefaultConfigurations();
  cfgSet::setJSONFile("/home/nmccoll/test/CMSSW_7_4_7/src/data/JSON/Cert_246908-251883_13TeV_PromptReco_Collisions15_JSON_v2.txt");
  cfgSet::ConfigSet cfg = cfgSet::ol_search_set;
  cfg.selectedLeptons.minMuPt  = 10;
  cfg.selectedLeptons.maxMuEta = 2.4;
  cfg.selectedLeptons.minEPt  = 10;
  cfg.selectedLeptons.maxEEta = 2.4;
  cfg.jets.minPt = 20;
  cfg.jets.maxEta= 5;
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
  a.jetAndMETCorrections.setResCorr(inMetParScale > 0 ? inMetParScale : 1.0,inMetPerpScale > 0 ? inMetPerpScale : 1.0);

  a.analyze();
}

/* Running
 *
 * root -b -q 'JetMETStudies/METCorrectionTree.C+("/data/nmccoll/13TeV/8_18_15_DiLep/doubleeg-2015b-17jul15_ntuple_postproc_DiLep.root","Events","metCorr",false)' &
root -b -q 'JetMETStudies/METCorrectionTree.C+("/data/nmccoll/13TeV/8_18_15_DiLep/doubleeg-2015b-pr_ntuple_postproc_DiLep.root"     ,"Events","metCorr",false)' &
root -b -q 'JetMETStudies/METCorrectionTree.C+("/data/nmccoll/13TeV/8_18_15_DiLep/doublemu-2015b-17jul15_ntuple_postproc_DiLep.root","Events","metCorr",false)' &
root -b -q 'JetMETStudies/METCorrectionTree.C+("/data/nmccoll/13TeV/8_18_15_DiLep/doublemu-2015b-pr_ntuple_postproc_DiLep.root"     ,"Events","metCorr",false)' &

root -b -q 'JetMETStudies/METCorrectionTree.C+("/data/nmccoll/13TeV/8_18_15_DiLep/SM_DiLep.root","Events","metCorr",true,false,-1)' &
root -b -q 'JetMETStudies/METCorrectionTree.C+("/data/nmccoll/13TeV/8_18_15_DiLep/SM_DiLep.root","Events","corr_metCorr",true,true,-1)' &
root -b -q 'JetMETStudies/METCorrectionTree.C+("/data/nmccoll/13TeV/8_18_15_DiLep/SM_DiLep.root","Events","corr_resp90_metCorr",true,true,.90)' &
root -b -q 'JetMETStudies/METCorrectionTree.C+("/data/nmccoll/13TeV/8_18_15_DiLep/SM_DiLep.root","Events","corr_resp75_metCorr",true,true,.75)' &
root -b -q 'JetMETStudies/METCorrectionTree.C+("/data/nmccoll/13TeV/8_18_15_DiLep/SM_DiLep.root","Events","corr_resp50_metCorr",true,true,.50)' &
root -b -q 'JetMETStudies/METCorrectionTree.C+("/data/nmccoll/13TeV/8_18_15_DiLep/SM_DiLep.root","Events","corr_scale_metCorr",true,true,-1,.97,.94)' &
 *
 */
