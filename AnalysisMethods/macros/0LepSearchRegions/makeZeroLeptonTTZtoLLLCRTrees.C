#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisMethods/macros/0LepSearchRegions/ZeroLeptonTreeHelper.hh"
#endif

using namespace ucsbsusy;

class ZtoLLCRAnalyzer : public ZeroLeptonAnalyzer {

  public :


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
  bool passCTTSelection(CMSTopF* ctt) {
    return (ctt->topRawMass() > 140.0 && ctt->topRawMass() < 250.0 && ctt->topMinMass() > 50.0 && ctt->topNsubJets() >= 3);
  }

    ZtoLLCRAnalyzer(TString fileName, TString treeName, TString outfileName, bool isMCTree, cfgSet::ConfigSet *pars,TString sname, int proc) :
      ZeroLeptonAnalyzer(fileName, treeName, outfileName, isMCTree, pars), name(sname), extProcess(proc) {}

    // booking
    size i_ptzll = 0;
    size i_mtcsv12metNoLep = 0;
    size i_process = 0;
    size i_onZ  = 0;
    size i_passTrig  = 0;

    size i_passMed = 0;
    size i_passTight = 0;

    size i_ptw = 0;
    size i_minlepPT = 0;

    size i_lumiWeight = 0;

    size i_extProcess = 0;
    size i_maxDiLepPt = 0;

    bool onZ = false;
    double mtcsv12metNoLep = 0;
    double ptzll      = 0;
    double ptw = 0;
    double minlepPT = 0;
    double maxDiLepPt = 0;


    bool  passLLLSel = false;
    bool  passLLLMedSel = false;
    bool  passLLLTightSel = false;


    TString name = "";
    int extProcess = 0;


    void book() {
      ZeroLeptonAnalyzer::book();

      i_ptzll             = data.add<float>("","ptzll","F",0);
      i_mtcsv12metNoLep             = data.add<float>("","mtcsv12metNoLep","F",0);
      i_process             = data.add<int>("","process","I",0);
      i_passMed             = data.add<bool>("","passMed","O",0);
      i_passTight             = data.add<bool>("","passTight","O",0);
      i_ptw             = data.add<float>("","ptw","F",0);
      i_minlepPT             = data.add<float>("","minlepPT","F",0);
      i_onZ             = data.add<bool>("","onZ","O",0);
      i_passTrig             = data.add<bool>("","passTrig","O",0);
      i_lumiWeight             = data.add<float>("","lumiWeight","F",0);
      i_extProcess = data.add<int>("","extProcess","I",0);
      i_maxDiLepPt = data.add<float>("","maxDiLepPt","F",0);
    }

    void processVariables(){
      ZeroLeptonAnalyzer::processVariables();
      mtcsv12metNoLep = 0;
      ptzll = 0;
      ptw = 0;
      minlepPT = 0;
      onZ = false;
      maxDiLepPt = 0;

      // selections: exactly two same flavor leptons, inv mass in (80, 100)
      passLLLSel = false;
      passLLLMedSel = false;
      passLLLTightSel = false;
      if(selectedLeptons.size() != 3)
        return;
      if (selectedLeptons[0]->pt()<20)
        return;

      bool passSecondTrigLep = false;
      for(unsigned int iL = 1; iL < selectedLeptons.size(); ++iL){
        if(selectedLeptons[iL]->pt() >= (selectedLeptons[iL]->iselectron()?15:10 ) ) passSecondTrigLep = true;
      }
      if(!passSecondTrigLep) return;

      passLLLSel = true;

      int nMedLep = 0;
      int nTightLep = 0;
      for(unsigned int iL = 0; iL < selectedLeptons.size(); ++iL){
        auto* lep = selectedLeptons[iL];
        if(lep->iselectron()){
          if( ((ElectronF*)lep)->ismediumid() && ((ElectronF*)lep)->miniiso() < .1) nMedLep++;
          if( ((ElectronF*)lep)->istightelectron() && ((ElectronF*)lep)->miniiso() < .1) nTightLep++;
        } else {
          if( ((MuonF*)lep)->ismediummuon() && ((MuonF*)lep)->miniiso() < .1) nMedLep++;
          if( ((MuonF*)lep)->istightmuon() && ((MuonF*)lep)->miniiso() < .1) nTightLep++;
        }
      }



      int zL1 = -1;
      int zL2 = -1;
      int wL  = -1;

      for(unsigned int iL1 = 0; iL1 < selectedLeptons.size(); ++iL1){
        for(unsigned int iL2 = iL1+1; iL2 < selectedLeptons.size(); ++iL2){
          auto z_p4 = selectedLeptons[iL1]->p4() + selectedLeptons[iL2]->p4();
          if(z_p4.pt() > maxDiLepPt ) maxDiLepPt = z_p4.pt();

          if(selectedLeptons[iL1]->iselectron() != selectedLeptons[iL2]->iselectron()) continue;
          if(selectedLeptons[iL1]->q()*selectedLeptons[iL2]->q() >= 0) continue;
          if (z_p4.mass()<=80 || z_p4.mass()>=100) continue;
          if(z_p4.pt() < ptzll) continue;
          ptzll = z_p4.pt();
          zL1 = iL1;
          zL2 = iL2;
        }
      }
      if(zL1 < 0) {
        onZ = false;
      } else {
        onZ = true;
      }

      minlepPT = selectedLeptons[2]->pt();


      if(nMedLep == 3) passLLLMedSel = true;
      if(nTightLep == 3) passLLLTightSel = true;

    if(zL1 != 0 && zL2 != 0) wL = 0;
    else if(zL1 != 1 && zL2 != 1) wL = 1;
    else if(zL1 != 2 && zL2 != 2) wL = 2;

    auto metPL = met->p4() + selectedLeptons[wL]->p4();
    ptw = metPL.pt();


    vector<RecoJetF*> jetsCSVranked;
    rankedByCSV(jets,jetsCSVranked);
    if(jetsCSVranked.size() > 0) mtcsv12metNoLep = JetKinematics::transverseMass(*jetsCSVranked[0], metPL);
    if(jetsCSVranked.size() > 1) mtcsv12metNoLep = std::min(JetKinematics::transverseMass(*jetsCSVranked[1], metPL),mtcsv12metNoLep);




    }


    bool fillEvent() {
      if(!passLLLSel)                       return false;
      if(!goodvertex)                       return false;
      filler.fillEventInfo(&data, this);
      filler.fillJetInfo  (&data, jets, bJets, met);

      //trigger info
      bool passTrigger = false;
      if(isMC()){
        if(triggerflag & kHLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ) passTrigger = true;
        if(triggerflag & kHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ) passTrigger = true;
        if(triggerflag & kHLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ) passTrigger = true;
        if(triggerflag & kHLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL) passTrigger = true;
        if(triggerflag & kHLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL) passTrigger = true;
      } else {

        if(name.BeginsWith("mumuData")){
          if(triggerflag & kHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ) passTrigger = true;
          if(triggerflag & kHLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ) passTrigger = true;
        } else if (name.BeginsWith("eeData")){
          if(triggerflag & kHLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ) passTrigger = true;
          //veto mumu
          if(triggerflag & kHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ) passTrigger = false;
          if(triggerflag & kHLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ) passTrigger = false;
        }else if (name.BeginsWith("mueData")){
          if(triggerflag & kHLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL) passTrigger = true;
          if(triggerflag & kHLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL) passTrigger = true;
          //veto mumu
          if(triggerflag & kHLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ) passTrigger = false;
          if(triggerflag & kHLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ) passTrigger = false;
          if(triggerflag & kHLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ) passTrigger = false;
        }
        if(hasJSONFile() && !passesLumiMask()) passTrigger = false; //remove json for data
      }

      float lumiWeight = 1.0;
      if(isMC()){
        if(selectedLeptons[0]->iselectron() && selectedLeptons[1]->iselectron()) lumiWeight = .562;
        else lumiWeight =.577;
      }


      data.fill<bool>(i_passTrig, passTrigger);
      data.fill<bool>(i_onZ, onZ);
      data.fill<float>(i_ptzll, float(ptzll));
      data.fill<float>(i_mtcsv12metNoLep, float(mtcsv12metNoLep));
      data.fill<int>(i_process, process);
      data.fill<bool>(i_passMed, passLLLMedSel);
      data.fill<bool>(i_passTight, passLLLTightSel);
      data.fill<float>(i_minlepPT, minlepPT);
      data.fill<float>(i_ptw, ptw);
      data.fill<float>(i_lumiWeight,lumiWeight);
      data.fill<float>(i_maxDiLepPt,maxDiLepPt);
      data.fill<int>(i_extProcess,extProcess);
      return true;
    }


};

void makeZeroLeptonTTZtoLLLCRTrees(TString sname = "dyjetstoll_cr",
                                 const int fileindex = 0,
                                 const bool isMC = true,
                                 const TString fname = "/store/user/vdutta/13TeV/150715/74X/merged/dyjetstoll_1_ntuple_wgtxsec.root",
                                 const double xsec = 1.0,
                                 const TString outputdir = "trees",
                                 const TString fileprefix = "root://eoscms//eos/cms",
                                 const TString json="")
{

  printf("Processing file %d of %s sample\n", (fileindex > -1 ? fileindex : 0), sname.Data());

  TString origSname = sname;

  if(fileindex > -1)
    sname += TString::Format("_%d",fileindex);

  if(isMC)
    printf("Cross section: %5.2f pb\n", xsec);

  TString fullname = fileprefix+fname;
  gSystem->mkdir(outputdir,true);
  TString outfilename = outputdir+"/"+sname+"_tree.root";

  cfgSet::ConfigSet pars = pars0lep(json);
  pars = cfgSet::zl_dilepton_set;
  pars.selectedLeptons = cfgSet::zl_veto_leptons;

  pars.corrections.eventCorrectionFile =  TString::Format("%s/src/data/corrections/eventCorr_allData.root",cfgSet::CMSSW_BASE);

//                  0      1  2    3    4   5   6   7   8   9  10 11   12
  enum EXTPROCESS {DATA, TTZ, TT, TTW, TTG, T, TW,  W, WW, TZ, Z, WZ, ZZ  };
  int extProcess = DATA;
  if(origSname.BeginsWith("ttZ_")) extProcess = TTZ;
  else if(origSname.BeginsWith("ttbar_")) extProcess = TT;
  else if(origSname.BeginsWith("ttW_")) extProcess = TTW;
  else if(origSname.BeginsWith("ttg_")) extProcess = TTG;
  else if(origSname.BeginsWith("t_")) extProcess = T;
  else if(origSname.BeginsWith("tW_")) extProcess = TW;
  else if(origSname.BeginsWith("W_")) extProcess = W;
  else if(origSname.BeginsWith("tZ_")) extProcess = TZ;
  else if(origSname.BeginsWith("Zll_")) extProcess = Z;
  else if(origSname.BeginsWith("WZ_")) extProcess = WZ;
  else if(origSname.BeginsWith("ZZ_")) extProcess = ZZ;
  else if(origSname.BeginsWith("WW_")) extProcess = WW;


  ZtoLLCRAnalyzer a(fullname, "Events", outfilename, isMC, &pars,origSname,extProcess);
  a.analyze(100000);



}
