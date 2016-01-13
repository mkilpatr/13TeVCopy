#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisMethods/macros/0LepSearchRegions/ZeroLeptonTreeHelper.hh"
#endif

using namespace ucsbsusy;
TRandom3 rnd;

bool pairCompare_gt(const pair<float, int>& firstElem, const pair<float, int>& secondElem);
bool pairCompare_lt(const pair<float, int>& firstElem, const pair<float, int>& secondElem);

class QCDHTRAnalyzer : public ZeroLeptonAnalyzer {

public :
  QCDHTRAnalyzer(TString fileName, TString treeName, TString outfileName, size randSeed, bool isMCTree, cfgSet::ConfigSet *pars) : FileName(fileName), ZeroLeptonAnalyzer(fileName, treeName, outfileName, randSeed, isMCTree, pars) {}
  TString FileName;
  TRandom3 r;
  vector<GenJetF*> genJets;

  size i_weight                 = 0;
  size i_prescale               = 0;
  size i_unclusteredPt          = 0;
  size i_unclustPtByMET         = 0;
  size i_genht                  = 0;
  size i_allJetPt               = 0;
  size i_genJetIdx              = 0;
  size i_recoJetIdx             = 0;
  size i_jetMMMGenPt            = 0;
  size i_jetMMMRecoPt           = 0;
  size i_jet1GenPt              = 0;
  size i_jet2GenPt              = 0;
  size i_jet3GenPt              = 0;
  size i_jetMMMPseudoResponsePt = 0;
  size i_jetMMMTrueResponsePt   = 0;
  size i_jetMMMResponsePt       = 0;
  size i_jet1ResponsePt         = 0;
  size i_jet2ResponsePt         = 0;
  size i_jet3ResponsePt         = 0;
  size i_jetMMMDeltaR           = 0;
  size i_jet1DeltaR             = 0;
  size i_jet2DeltaR             = 0;
  size i_jet3DeltaR             = 0;

  bool failtright       = false;
  int prescale          = -9;
  int prescale300       = -9;
  int prescale350       = -9;
  int prescale400       = -9;
  int prescale475       = -9;
  int prescale600       = -9;
  int prescale650       = -9;
  int prescale800       = -9;
  int prescale900       = -9;

  const float metcut_ = 75;

  void loadVariables(){
    cout << "isMC() = " << isMC() << endl;
    load(cfgSet::EVTINFO);
    load(cfgSet::AK4JETS);
    load(cfgSet::ELECTRONS);
    load(cfgSet::MUONS);
    load(cfgSet::PHOTONS);
    //load(cfgSet::PFCANDS, PFCandidateReader::LOADRECO | PFCandidateReader::FILLOBJ | PFCandidateReader::LOADTAUVETOMT | PFCandidateReader::LOADPHYS14);
    load(cfgSet::PFCANDS, PFCandidateReader::LOADRECO | PFCandidateReader::FILLOBJ | PFCandidateReader::LOADTAUVETOMT);
    load(cfgSet::CMSTOPS);
    load(cfgSet::AK8FATJETS);
    load(cfgSet::TRIGOBJS);
    if(isMC()) load(cfgSet::GENPARTICLES);
 }

  void book() {
    ZeroLeptonAnalyzer::book();
    i_weight                 = data.add<float>     ("", "weight",                 "F", 0);
    i_prescale               = data.add<int>       ("", "prescale",               "I", 0);
    i_unclusteredPt          = data.add<float>     ("", "unclusteredPt",          "F", 0);
    i_unclustPtByMET         = data.add<float>     ("", "unclustPtByMET",         "F", 0);
    i_genht                  = data.add<float>     ("", "genht",                  "F", 0);
    i_allJetPt               = data.addMulti<float>("", "allJetPt",                    0);
    i_genJetIdx              = data.add<int>       ("", "genJetIdx",              "I", 0);
    i_recoJetIdx             = data.add<int>       ("", "recoJetIdx",             "I", 0);
    i_jetMMMGenPt            = data.add<float>     ("", "jetMMMGenPt",            "F", 0);
    i_jetMMMRecoPt           = data.add<float>     ("", "jetMMMRecoPt",           "F", 0);
    i_jet1GenPt              = data.add<float>     ("", "jet1GenPt",              "F", 0);
    i_jet2GenPt              = data.add<float>     ("", "jet2GenPt",              "F", 0);
    i_jet3GenPt              = data.add<float>     ("", "jet3GenPt",              "F", 0);
    i_jetMMMPseudoResponsePt = data.add<float>     ("", "jetMMMPseudoResponsePt", "F", 0);
    i_jetMMMTrueResponsePt   = data.add<float>     ("", "jetMMMTrueResponsePt",   "F", 0);
    i_jetMMMResponsePt       = data.add<float>     ("", "jetMMMResponsePt",       "F", 0);
    i_jet1ResponsePt         = data.add<float>     ("", "jet1ResponsePt",         "F", 0);
    i_jet2ResponsePt         = data.add<float>     ("", "jet2ResponsePt",         "F", 0);
    i_jet3ResponsePt         = data.add<float>     ("", "jet3ResponsePt",         "F", 0);
    i_jetMMMDeltaR           = data.add<float>     ("", "jetMMMDeltaR",           "F", 0);
    i_jet1DeltaR             = data.add<float>     ("", "jet1DeltaR",             "F", 0);
    i_jet2DeltaR             = data.add<float>     ("", "jet2DeltaR",             "F", 0);
    i_jet3DeltaR             = data.add<float>     ("", "jet3DeltaR",             "F", 0);
  }

  void processVariables(){
    ZeroLeptonAnalyzer::processVariables();

    float ht = JetKinematics::ht(jets, 20.0, 2.4);
    failtright = false;
    bool passtright300 = false; prescale300 = -9;
    bool passtright350 = false; prescale350 = -9;
    bool passtright400 = false; prescale400 = -9;
    bool passtright475 = false; prescale475 = -9;
    bool passtright600 = false; prescale600 = -9;
    bool passtright650 = false; prescale650 = -9;
    bool passtright800 = false; prescale800 = -9;
    bool passtright900 = false; prescale900 = -9;
    for(auto* tI: triggerInfo){
      if( (tI->trigflag() & kHLT_PFHT300) && tI->passtrig()) passtright300 = true;
      if( (tI->trigflag() & kHLT_PFHT350) && tI->passtrig()) passtright350 = true;
      if( (tI->trigflag() & kHLT_PFHT400) && tI->passtrig()) passtright400 = true;
      if( (tI->trigflag() & kHLT_PFHT475) && tI->passtrig()) passtright475 = true;
      if( (tI->trigflag() & kHLT_PFHT600) && tI->passtrig()) passtright600 = true;
      if( (tI->trigflag() & kHLT_PFHT650) && tI->passtrig()) passtright650 = true;
      if( (tI->trigflag() & kHLT_PFHT800) && tI->passtrig()) passtright800 = true;
      if( (tI->trigflag() & kHLT_PFHT900) && tI->passtrig()) passtright900 = true;
      if(  tI->trigflag() & kHLT_PFHT300) prescale300 = tI->prescale();
      if(  tI->trigflag() & kHLT_PFHT350) prescale350 = tI->prescale();
      if(  tI->trigflag() & kHLT_PFHT400) prescale400 = tI->prescale();
      if(  tI->trigflag() & kHLT_PFHT475) prescale475 = tI->prescale();
      if(  tI->trigflag() & kHLT_PFHT600) prescale600 = tI->prescale();
      if(  tI->trigflag() & kHLT_PFHT650) prescale650 = tI->prescale();
      if(  tI->trigflag() & kHLT_PFHT800) prescale800 = tI->prescale();
      if(  tI->trigflag() & kHLT_PFHT900) prescale900 = tI->prescale();
    }
    prescale = -9999;
    if(!isMC()){
      if(!FileName.Contains("htmht")){
        if      (passtright800 && ht>950           ) prescale = prescale800;
        else if (passtright650 && ht>800 && ht<=950) prescale = prescale650;
        else if (passtright600 && ht>750 && ht<=800) prescale = prescale600;
        else if (passtright475 && ht>625 && ht<=750) prescale = prescale475;
        else if (passtright400 && ht>550 && ht<=625) prescale = prescale400;
        else if (passtright350 && ht>500 && ht<=550) prescale = prescale350;
        else if (passtright300 && ht>450 && ht<=500) prescale = prescale300;
        else failtright = true;
      }
    }
  }

  bool fillEvent() {
    if(failtright)          return false;
    if(met->pt() < metcut_) return false;
    if(!goodvertex)         return false;
    if(nJets < 2)           return false;

    filler.fillEventInfo(&data, this);

    data.fill<int>(i_prescale, prescale);
    auto unclusteredSum = met->p4();
    for(auto &j : defaultJets->recoJets){
      if(j.pt() > 15.) unclusteredSum += j.p4();
      if(j.pt() > 10.) data.fillMulti<float>(i_allJetPt, j.pt());
    }
    float uncl_pt = unclusteredSum.pt();
    data.fill<float>(i_unclusteredPt,  uncl_pt);
    data.fill<float>(i_unclustPtByMET, unclusteredSum.pt() / met->pt());

    float temp_weight = weight;
    if(isMC()) {
      genJets.clear();
      vector<float> jetGenPt     = { 0., 0., 0. };
      vector<float> jetResponses = { 0., 0., 0. };
      vector<float> jetDeltaRs   = { 0., 0., 0. };
      vector <pair <float, unsigned int> > jetMisMeasurementsByIdx;
      double minMET = met->pt();
      for(unsigned int iG = 0; iG < defaultJets->genJets.size(); ++iG){
        if(defaultJets->genJets[iG].pt() < 10.) continue;
        auto* tempGen = &defaultJets->genJets[iG];
        genJets.push_back(tempGen);
        for(unsigned int iR = 0; iR < defaultJets->recoJets.size(); ++iR){
          if(defaultJets->recoJets[iR].genJet() != &defaultJets->genJets[iG]) continue;
          if(iG < 3){
            jetGenPt[iG]     = defaultJets->recoJets[iR].genJet()->pt();
            jetResponses[iG] = defaultJets->recoJets[iR].pt() / defaultJets->recoJets[iR].genJet()->pt();
            jetDeltaRs[iG]   = PhysicsUtilities::deltaR(defaultJets->recoJets[iR], *defaultJets->recoJets[iR].genJet());
            jetMisMeasurementsByIdx.push_back(make_pair(fabs(defaultJets->recoJets[iR].pt() - defaultJets->recoJets[iR].genJet()->pt()), iR));
          }
          break;
        }
      }
      sort(jetMisMeasurementsByIdx.begin(), jetMisMeasurementsByIdx.end(), pairCompare_gt);

      unsigned int MMM_Idx = jetMisMeasurementsByIdx[0].second;
      data.fill<float>(i_genht,            JetKinematics::ht(genJets, 20.0, 2.4));
      data.fill<float>(i_jet1GenPt,        jetGenPt[0]);
      data.fill<float>(i_jet2GenPt,        jetGenPt[1]);
      data.fill<float>(i_jet3GenPt,        jetGenPt[2]);
      data.fill<float>(i_jetMMMRecoPt,     defaultJets->recoJets[MMM_Idx].pt());
      data.fill<float>(i_jetMMMGenPt,      defaultJets->recoJets[MMM_Idx].genJet()->pt());
      data.fill<float>(i_jet1ResponsePt,   jetResponses[0]);
      data.fill<float>(i_jet2ResponsePt,   jetResponses[1]);
      data.fill<float>(i_jet3ResponsePt,   jetResponses[2]);
      data.fill<float>(i_jetMMMResponsePt, defaultJets->recoJets[MMM_Idx].pt() / defaultJets->recoJets[MMM_Idx].genJet()->pt());
      data.fill<float>(i_jet1DeltaR,       jetDeltaRs[0]);
      data.fill<float>(i_jet2DeltaR,       jetDeltaRs[1]);
      data.fill<float>(i_jet3DeltaR,       jetDeltaRs[2]);
      data.fill<float>(i_jetMMMDeltaR,     PhysicsUtilities::deltaR(defaultJets->recoJets[MMM_Idx], *defaultJets->recoJets[MMM_Idx].genJet()));
    } else {
      data.fill<float>(i_genht, JetKinematics::ht(jets, 20.0, 2.4));
      data.fill<float>(i_jet1ResponsePt,   1.);
      data.fill<float>(i_jet2ResponsePt,   1.);
      data.fill<float>(i_jet3ResponsePt,   1.);
      data.fill<float>(i_jetMMMResponsePt, 1.);
    }
    data.fill<float>(i_weight, temp_weight);

    filler.fillJetInfo(&data, jets, bJets, met);

    int jetNearMETInd = -1, MMJetDPhi = 999999;
    for(unsigned int iJ = 0; iJ < jets.size() && iJ < 2; ++iJ){
      double dPhi = PhysicsUtilities::absDeltaPhi(*jets[iJ],*met);
      if(dPhi < MMJetDPhi){
        MMJetDPhi     = dPhi;
        jetNearMETInd = iJ;
      }
    }
    float MMPseudoResp = 1, MMTrueResp = 1;
    if(jetNearMETInd >= 0){
      double pseudoGenPT = (met->p4() + jets[jetNearMETInd]->p4()).pt();
      MMPseudoResp = pseudoGenPT > 0 ? jets[jetNearMETInd]->pt()/ pseudoGenPT : 999;
      MMTrueResp   = jets[jetNearMETInd]->genJet() && jets[jetNearMETInd]->genJet()->pt() > 0 ? jets[jetNearMETInd]->pt() / jets[jetNearMETInd]->genJet()->pt() : 999;
    }
    data.fill<float>(i_jetMMMPseudoResponsePt, MMPseudoResp);
    data.fill<float>(i_jetMMMTrueResponsePt,   MMTrueResp);

    return true;
  }
};

bool pairCompare_gt(const pair<float, int>& firstElem, const pair<float, int>& secondElem) {
  return firstElem.first > secondElem.first;
}

bool pairCompare_lt(const pair<float, int>& firstElem, const pair<float, int>& secondElem) {
  return firstElem.first < secondElem.first;
}

//void makeZeroLeptonQCDCRTrees(TString sname = "jetht",
//            const int fileindex = 0,
//            const bool isMC = false,
//            const TString fname = "/store/user/bora/Stop13TeV/231015/jetht-2015d-05oct15_5_ntuple_postproc.root",
void makeZeroLeptonQCDCRTrees(TString sname = "QCD",
            const int fileindex = 0,
            const bool isMC = true,
            const TString fname = "/uscms/home/nmccoll/nobackup/2011-04-15-susyra2/Work7/skimmer/smearedQCDSkim/qcd_ht2000toinf_1_ntuple_postproc_6_p4_qcdSmearSkim.root",
//            const double xsec = 1.0,
            const TString outputdir = "trees",
//            const TString fileprefix = "root://cmseos.fnal.gov/",
            const TString fileprefix = "",
            const TString json = "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Cert_246908-258159_13TeV_PromptReco_Collisions15_25ns_JSON_v3.txt")
{
  printf("Processing file %d of %s sample\n", (fileindex > -1 ? fileindex : 0), sname.Data());

  if(fileindex > -1) sname += TString::Format("_%d", fileindex);
//  if(isMC) printf("Cross section: %5.2f pb\n", xsec);

  TString fullname = fileprefix + fname;

  gSystem->mkdir(outputdir,true);
  TString outfilename = outputdir+"/"+sname+"_tree.root";

  cfgSet::ConfigSet pars = pars0lep(json);
  pars.corrections.eventCorrectionFile =  TString::Format("%s/src/data/corrections/eventCorr_allData.root",cfgSet::CMSSW_BASE);
//  pars.corrections.jetAndMETCorrections |= JetAndMETCorrectionSet::JETRESOLUTION;
//  pars.corrections.jetResCorr = 1.25;
//  pars.jets.applyJetMisMeasurement = .1;


  double randSeed = fileindex + 2;
  TString treename = "Events";
  QCDHTRAnalyzer a(fullname, treename, outfilename, randSeed, isMC, &pars);

//  a.analyze(1,5000);
  a.analyze(10000);
}
