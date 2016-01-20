#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/QuickRefold/interface/TObjectContainer.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"

#include "AnalysisBase/TreeAnalyzer/interface/QCDRespSmearingAnalyzer.h"

#include <TObjectTable.h>
using namespace std;
using namespace ucsbsusy;

//#define TEST

enum RUNTYPE {ORIG, GLOBAL, CONSTW, GRANW};


class Copier : public TreeCopierManualBranches {
public:
  Copier(string fileName, string treeName, string outFileName, size randSeed, bool isMCTree, cfgSet::ConfigSet * pars, double window, int type) :   TreeCopierManualBranches(fileName,treeName,outFileName,randSeed,isMCTree,pars),
    window(window),
    type(RUNTYPE(type)),
  i_met  (0),
  i_nB   (0),
  i_nBl  (0),
  i_nJ   (0),
  i_nT   (0),
  i_ht   (0),
  i_dPhi (0),
  i_dPhi3(0),
  i_dPhi4(0),
  i_mtB  (0),
  i_nEMu (0),
  i_nTau (0),
  i_leadLoss_pt      (0),
  i_leadLoss_eta     (0),
  i_leadLoss_flavor  (0),
  i_leadLoss_res     (0),
  i_leadLoss_rank    (0),
  i_leadLoss_recorank(0),

  i_weight    (0),
  i_puWeight  (0),
  i_bTagWeight    (0),
  i_nomTailWeight (0),
  i_upTailWeight  (0),
  i_downTailWeight(0),

  i_evtN              (0),
  i_fileN             (0),
  i_passcscbeamhaloflt(0),
  i_passeebadscflt    (0),
  i_passeebadsc4flt   (0),
  i_passhbheisoflt    (0),
  i_passhbhefltloose  (0),
  i_passhbheflttight  (0),

  oldWeights(0),
  i_bs(0),
  randSeed(randSeed)
  {
  };
  virtual ~Copier() { delete oldWeights;};



  virtual void loadVariables(){
    load(cfgSet::EVTINFO);
    load(cfgSet::AK4JETS,JetReader::LOADRECO | JetReader::LOADGEN | JetReader::FILLOBJ);
    load(cfgSet::CMSTOPS);
    load(cfgSet::ELECTRONS);
    load(cfgSet::MUONS);
    load(cfgSet::PFCANDS);

    oldWeights = new std::vector<size8>;
    setBranchAddress("","bootstrapWeight",&oldWeights,false);

  }

//  virtual BaseEventAnalyzer * setupEventAnalyzer() override {
//    QCDRespSmearingCopierEventAnalyzer * ana = new  QCDRespSmearingCopierEventAnalyzer();
//    ana->smearOptions.minWindow = 0.01;
//    ana->smearOptions.maxWindow = window;
//    ana->smearOptions.nSmears = 100;
//    ana->smearOptions.nSmearJets = 2;
//    ana->smearOptions.nBootstraps = 50;
//
//    switch(type){
//    case ORIG:
//      ana->smearOptions.nSmears = 0;
//      ana->smearOptions.nBootstraps = 0;
//      break;
//    case GLOBAL:
//      ana->smearOptions.winType = JetRespSmear::FLAT;
//      ana->smearOptions.doFlatSampling = false;
//      ana->smearOptions.maxWindow = 10;
//      break;
//    case CONSTW:
//      ana->smearOptions.winType = JetRespSmear::FLAT;
//      ana->smearOptions.doFlatSampling = false;
//      break;
//    case GRANW:
//      ana->smearOptions.winType = JetRespSmear::LINEAR_GRANULATED;
//      ana->smearOptions.doFlatSampling = true;
//      break;
//    }
//
//    return ana;
//  }


  bool passCTTSelection(CMSTopF* ctt) {
    return (ctt->topCmsTopTagMass() > 140.0 && ctt->topCmsTopTagMass() < 250.0 && ctt->topMinMass() > 50.0 && ctt->topNsubJets() >= 3 && ctt->p4().pt()>=400. && fabs(ctt->p4().eta())<=2.4);
  }




  void rankedByCSV(vector<RecoJetF*> inJets, vector<RecoJetF*>& outJets) {
    outJets.clear();
    outJets.resize(inJets.size());
    vector<pair<double,int> > rankedJets(inJets.size());

    for(unsigned int iJ =0; iJ < inJets.size(); ++iJ){
      rankedJets[iJ].first = inJets[iJ]->csv();
      rankedJets[iJ].second = iJ;
    }

    std::sort(rankedJets.begin(),rankedJets.end(),PhysicsUtilities::greaterFirst<double,int>());
    for(unsigned int iJ =0; iJ < inJets.size(); ++iJ) {
      outJets[iJ] = inJets[rankedJets[iJ].second];
    }
  }


  bool fillEvent() {
    if(!goodvertex) return false;
    if(met->pt() < 200) return false;
    if(jets.size() < 2 || jets[1]->pt() < 75) return false;
    if(jets.size() < 5) return false;


    float ht = 0;
    int nlbs = 0;
    for(const auto* j : jets){
      ht += j->pt();
      if(j->csv() > defaults::CSV_LOOSE)  nlbs++;
    }

    float dPhi  = 10;
    float dPhi3 = 10;
    float dPhi4 = 10;
    if(jets.size()) dPhi = PhysicsUtilities::absDeltaPhi(*jets[0],*met);
    if(jets.size() > 1) dPhi = min(float(PhysicsUtilities::absDeltaPhi(*jets[1],*met)),dPhi);
    if(jets.size() > 2) dPhi3 = PhysicsUtilities::absDeltaPhi(*jets[2],*met);
    if(jets.size() > 3) dPhi4 = PhysicsUtilities::absDeltaPhi(*jets[3],*met);

    vector<RecoJetF*> jetsCSVranked;
    rankedByCSV(jets,jetsCSVranked);

    float mtcsv1met = 0.0, mtcsv2met = 0.0;
    if(jetsCSVranked.size())             mtcsv1met = JetKinematics::transverseMass(*jetsCSVranked[0], *met);
    if(jetsCSVranked.size() > 1)             mtcsv2met = JetKinematics::transverseMass(*jetsCSVranked[1], *met);

    unsigned int ncttstd = 0;
    for(auto* ctt : cttTops) {
      if(passCTTSelection(ctt))ncttstd++;
    }


    data.fill<float>(i_met    ,met->pt());
    data.fill<unsigned int>(i_nB     ,nBJets);
    data.fill<unsigned int>(i_nBl    ,nlbs);
    data.fill<unsigned int>(i_nJ     ,nJets);
    data.fill<unsigned int>(i_nT     ,ncttstd);
    data.fill<float>(i_ht     , ht);
    data.fill<float>(i_dPhi   ,dPhi);
    data.fill<float>(i_dPhi3  ,dPhi3);
    data.fill<float>(i_dPhi4  ,dPhi4);
    data.fill<float>(i_mtB   ,min(mtcsv1met,mtcsv2met));
    data.fill<unsigned int>(i_nEMu   ,nSelLeptons);
    data.fill<unsigned int>(i_nTau   ,nVetoedTracks);


    if(window < 0){

      int leadGenInd = -1;
      int leadGenMatchInd = -1;
      double minMET = met->pt();
      for(unsigned int iG = 0; iG < defaultJets->genJets.size(); ++iG){

        int rJI = -1;

        for(unsigned int iR = 0; iR < defaultJets->recoJets.size(); ++iR){
          if(defaultJets->recoJets[iR].genJet() !=  &defaultJets->genJets[iG]) continue;
          rJI = iR;
          break;
        }
        auto newMET = met->p4();
        if(rJI >= 0)
          newMET += defaultJets->recoJets[rJI].p4();
        newMET -= defaultJets->genJets[iG].p4();

        if(newMET.pt() < minMET){
          minMET = newMET.pt();
          leadGenInd = iG;
          leadGenMatchInd = rJI;
        }
      }

      if(leadGenInd >= 0 && defaultJets->genJets[leadGenInd].pt() == 0 )
        leadGenInd = -1;

      data.fill<float>(i_leadLoss_pt             ,leadGenInd >= 0 ? defaultJets->genJets[leadGenInd].pt() :  0);
      data.fill<float>(i_leadLoss_eta            ,leadGenInd >= 0 ? defaultJets->genJets[leadGenInd].eta() : 0);
      data.fill<unsigned int>(i_leadLoss_flavor  ,leadGenInd >= 0 ? defaultJets->genJets[leadGenInd].flavor() : 99);
      data.fill<unsigned int>(i_leadLoss_rank    ,leadGenInd >= 0 ? leadGenInd : 99);

      data.fill<float>(i_leadLoss_res            ,leadGenMatchInd >= 0 ? defaultJets->recoJets[leadGenMatchInd].pt()/defaultJets->genJets[leadGenInd].pt() : 0);
      data.fill<unsigned int>(i_leadLoss_recorank,leadGenMatchInd >= 0 ? leadGenMatchInd : 99);

//      cout << (leadGenInd >= 0 ? leadGenInd : 99) <<" "<< (leadGenMatchInd >= 0 ? defaultJets->recoJets[leadGenMatchInd].pt()/defaultJets->genJets[leadGenInd].pt() : 0)
//          <<" "<< jetAndMETCorrections.getQCDRespTailCorrector()->mmInd <<" "<< jetAndMETCorrections.getQCDRespTailCorrector()->mmResp
//          <<" "<< jetAndMETCorrections.getQCDRespTailWeight()  <<" "<< jetAndMETCorrections.getQCDRespTailCorrector()->getWeight(UP) <<" "<< jetAndMETCorrections.getQCDRespTailCorrector()->getWeight(DOWN)<<endl;

    }

    data.fill<float>(i_weight          ,weight);
    data.fill<float>(i_puWeight        ,eventCorrections.getPUWeight());
    data.fill<float>(i_bTagWeight      ,bTagCorrections.getBTagByEvtWeight());
    data.fill<float>(i_nomTailWeight   ,jetAndMETCorrections.getQCDRespTailWeight());
    data.fill<float>(i_upTailWeight    ,jetAndMETCorrections.getQCDRespTailCorrector()->getWeight(UP));
    data.fill<float>(i_downTailWeight  ,jetAndMETCorrections.getQCDRespTailCorrector()->getWeight(DOWN));


    for(auto i : *oldWeights)
      data.fillMulti<ucsbsusy::size8>(i_bs,i);

    data.fill<unsigned int>(i_evtN, evtInfoReader.event);
    data.fill<ucsbsusy::size8>(i_fileN, randSeed);
    data.fill<bool>(i_passcscbeamhaloflt, evtInfoReader.cscBeamHaloFlt);
    data.fill<bool>(i_passeebadscflt,evtInfoReader.eeBadSCFlt);
    data.fill<bool>(i_passeebadsc4flt,evtInfoReader.eeBadSC4Flt);
    data.fill<bool>(i_passhbheisoflt,evtInfoReader.hbheIsoFlt);
    data.fill<bool>(i_passhbhefltloose,evtInfoReader.hbheFltR2Loose);
    data.fill<bool>(i_passhbheflttight,evtInfoReader.hbheFltR2Tight);



    return true;
  }



  void book() {
    i_met      = data.add<float>("","met"                      ,"F",0);
    i_nB       = data.add<unsigned int>("","nB"                ,"i",0);
    i_nBl      = data.add<unsigned int>("","nBl"               ,"i",0);
    i_nJ       = data.add<unsigned int>("","nJ"                ,"i",0);
    i_nT       = data.add<unsigned int>("","nT"                ,"i",0);
    i_ht       = data.add<float>("","ht"                       ,"F",0);
    i_dPhi     = data.add<float>("","dPhi"                     ,"F",0);
    i_dPhi3     = data.add<float>("","dPhi3"                   ,"F",0);
    i_dPhi4     = data.add<float>("","dPhi4"                   ,"F",0);
    i_mtB      = data.add<float>("","mtB"                      ,"F",0);
    i_nEMu     = data.add<float>("","nEMu"                     ,"i",0);
    i_nTau     = data.add<float>("","nTau"                     ,"i",0);

    if(window < 0){
    i_leadLoss_pt        = data.add<float>("","leadLoss_pt"                      ,"F",0);
    i_leadLoss_eta       = data.add<float>("","leadLoss_eta"                     ,"F",0);
    i_leadLoss_flavor    = data.add<unsigned int>("","leadLoss_flavor"                  ,"i",0);
    i_leadLoss_res       = data.add<float>("","leadLoss_res"                     ,"F",0);
    i_leadLoss_rank      = data.add<unsigned int>("","leadLoss_rank"                    ,"i",0);
    i_leadLoss_recorank  = data.add<unsigned int>("","leadLoss_recorank"                ,"i",0);
    }

    i_weight      = data.add<float>("","weight"                                  ,"F",0);
    i_puWeight      = data.add<float>("","puWeight"                              ,"F",0);
    i_bTagWeight      = data.add<float>("","bTagWeight"                              ,"F",0);
    i_nomTailWeight   = data.add<float>("","nomTailWeight"                              ,"F",0);
    i_upTailWeight    = data.add<float>("","upTailWeight"                              ,"F",0);
    i_downTailWeight  = data.add<float>("","downTailWeight"                              ,"F",0);
    i_bs            = data.addMulti<ucsbsusy::size8>("","bootstrapWeight"           ,0);
    i_evtN                = data.add<unsigned int>("","evtN"                              ,"i",0);
    i_fileN               = data.add<ucsbsusy::size8>("","fileN"                              ,"b",0);
    i_passcscbeamhaloflt = data.add<bool>("","passcscbeamhaloflt","O",0);
    i_passeebadscflt = data.add<bool>("","passeebadscflt","O",0);
    i_passeebadsc4flt = data.add<bool>("","passeebadsc4flt","O",0);
    i_passhbheisoflt  = data.add<bool>("","passhbheisoflt","O",0);
    i_passhbhefltloose    = data.add<bool>("","passhbhefltloose","O",0);
    i_passhbheflttight    = data.add<bool>("","passhbheflttight","O",0);



  }

  double window  ;
  RUNTYPE type;

  //event level info
  size i_met       ;
  size i_nB        ;
  size i_nBl       ;
  size i_nJ        ;
  size i_ht        ;
  size i_nT        ;
  size i_dPhi      ;
  size i_dPhi3     ;
  size i_dPhi4     ;
  size i_mtB       ;
  size i_nEMu      ;
  size i_nTau      ;

  size i_leadLoss_pt          ;
  size i_leadLoss_eta         ;
  size i_leadLoss_flavor      ;
  size i_leadLoss_res         ;
  size i_leadLoss_rank        ;
  size i_leadLoss_recorank    ;



  size i_weight            ;
  size i_puWeight          ;
  size i_bTagWeight        ;
  size i_nomTailWeight     ;
  size i_upTailWeight      ;
  size i_downTailWeight    ;
  size i_evtN              ;
  size i_fileN             ;
  size i_passcscbeamhaloflt;
  size i_passeebadscflt    ;
  size i_passeebadsc4flt   ;
  size i_passhbheisoflt    ;
  size i_passhbhefltloose  ;
  size i_passhbheflttight  ;

  std::vector<size8>*  oldWeights;
  size i_bs     ;
  int randSeed;

};


#endif


void JetResTestSkim(string fileName,  int fileIndex = -1, string treeName = "Events", string outPostfix ="jetResTest", int type = 0, double window = -1,  bool isMC = true) {

  cfgSet::loadDefaultConfigurations();
  cfgSet::ConfigSet cfg = cfgSet::zl_search_set;
  cfg.corrections.jetResTailCorrType = NOMINAL;
  cfg.corrections.jetResCorrType = NOMINAL;
  cfg.corrections.jetAndMETCorrections     = ucsbsusy::JetAndMETCorrectionSet::QCDRESPTAIL | ucsbsusy::JetAndMETCorrectionSet::JETRESOLUTION;
//  cfg.corrections.jetResCorrType = DOWN;

  //get the output name
  TString prefix(fileName);
  prefix.Remove(0,prefix.Last('/') + 1);
  if(prefix.First('.') >= 0) prefix.Resize(prefix.First('.'));
  TString outName = fileIndex < 0 ? TString::Format("%s_%s.root",prefix.Data(),outPostfix.c_str()) : TString::Format("%s_%i_%s.root",prefix.Data(),fileIndex,outPostfix.c_str());

  Copier a(fileName,treeName,outName.Data(),fileIndex+2,isMC, &cfg, window,type);

  a.analyze(10000);
}


/*
 * Running
 * nohup root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_4_7/src/AnalysisMethods/macros/JetMETStudies/JetResSkim.C+("/eos/uscms/store/user/ocolegro/13TeV/130815/merged/qcd_ht700to1000-50ns_1_ntuple_postproc.root"  , "Events","jetRes",true)' &
 * For every QCD file
 */
