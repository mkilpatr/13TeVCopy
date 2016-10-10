#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/KinematicVariables/interface/JetKinematics.h"
#include "AnalysisTools/Utilities/interface/PartonMatching.h"
#include "AnalysisTools/TreeReader/interface/HTTReader.h"
#include "AnalysisBase/TreeAnalyzer/interface/MVAWrapper.h"


#include <algorithm>

using namespace std;
using namespace ucsbsusy;
float newWeight;

class Copier : public TreeCopierManualBranches {
public:
  Copier(TString fileName, TString treeName, TString outFileName, size rSeed, bool isMCTree, cfgSet::ConfigSet * pars) : TreeCopierManualBranches(fileName,treeName,outFileName,rSeed,isMCTree,pars),
  mvaWrap("topDisc_new.root","mva_0"),
  i_process          (0),
  i_sigType          (0),
  i_mass1            (0),
  i_mass2            (0),
  i_mass3            (0),
  i_weight           (0),
  i_njets               (0),
  i_met                 (0),
  i_top_HTT_pt       (0),
  i_top_HTT_mva       (0),
  i_top_HTT_isMatched(0),
  i_top_CTT_pt          (0),
  i_top_CTT_isMatched   (0)



  {
  };
  virtual ~Copier() {};

  virtual void loadVariables() override{
    load(cfgSet::EVTINFO);
    load(cfgSet::AK4JETS,JetReader::LOADRECO | JetReader::FILLOBJ | JetReader::LOADGEN | JetReader::LOADTOPASSOC);
    load(cfgSet::GENPARTICLES, GenParticleReader::LOADPARTONDECAY | GenParticleReader::FILLOBJ);

    load(cfgSet::ELECTRONS);
    load(cfgSet::MUONS);
    load(cfgSet::PFCANDS);

    load(&httReader, HTTReader::LOADRECO |  HTTReader::FILLOBJ, "CA15HTT");
    load(cfgSet::AK8FATJETS, FatJetReader::LOADRECO | FatJetReader::LOADPUPPI | FatJetReader::FILLOBJ);
  }

  //--------------------------------------------------------------------------------------------------
  virtual BaseEventAnalyzer * setupEventAnalyzer() override {return new CopierFillYourselfEventAnalyzer();}

  bool passEventSel() const {
    if(met->pt() < 250) return false;
    if(nJets < 5) return false;
    size nlbjets = 0;
    for(const auto* j : jets) {
      if(j->csv() > defaults::CSV_LOOSE)  nlbjets++;
    }
    if(nlbjets < 2 || nBJets < 1) return false;
    if(nSelLeptons || nVetoedTracks) return false;

    return true;
  }



  void fillEventProperties() {
    data.fill<unsigned int>(i_process,process);
    data.fill<unsigned int>( i_sigType,size(evtInfoReader.signalType));
    data.fill<float>(  i_mass1 ,evtInfoReader.massparams->size() > 0 ? evtInfoReader.massparams->at(0) : 0 );
    data.fill<float>(  i_mass2 ,evtInfoReader.massparams->size() > 1 ? evtInfoReader.massparams->at(1) : 0);
    data.fill<float>(  i_mass3 ,evtInfoReader.massparams->size() > 2 ? evtInfoReader.massparams->at(2) : 0);
    data.fill<float>(  i_weight ,newWeight);
    data.fill<unsigned int>(  i_njets,size(nJets));
    data.fill<float>(i_met           ,met->pt());
  }

  template<class FatJet>
  int getMatchedJet(const PartonMatching::TopDecay *  top, const std::vector<FatJet>& fatJets, float& minDR) const {
    int idx = -1;
    float minMaxDR2 = -99;
    for(unsigned int iFJ = 0; iFJ < fatJets.size(); ++iFJ){
      float maxDR2 = -99;
      for(unsigned int iP = 0; iP < top->hadronicPartons.size(); ++iP){
        maxDR2 = std::max(double(maxDR2),PhysicsUtilities::deltaR2(*(top->hadronicPartons[iP]->parton),fatJets[iFJ]));
      }
      if(maxDR2 < 0) continue;
      if(minMaxDR2 < 0 || maxDR2 <  minMaxDR2  ) {
        idx = iFJ;
        minMaxDR2 = maxDR2;
      }
    }
    minDR = minMaxDR2 < 0 ? -99 : TMath::Sqrt(minMaxDR2);
    return idx;
  }

  template<class FatJet>
  void getMatching(const PartonMatching::TopDecay * top1, const PartonMatching::TopDecay * top2, const std::vector<FatJet>& fatJets,
      float& minDR1, float& minDR2, int& matchedFJ1, int& matchedFJ2 ){
    minDR1 = -99;
    minDR2 = -99;
    matchedFJ1 = -1;
    matchedFJ2 = -1;
    if(top1) matchedFJ1 = getMatchedJet(top1,fatJets, minDR1);
    if(top2) matchedFJ2 = getMatchedJet(top2,fatJets, minDR2);
    if(matchedFJ2 >=0 && matchedFJ2 == matchedFJ1){
      if(minDR2 < minDR1)  matchedFJ1 = -1;
      else matchedFJ2 = -1;
    }
  }

  float evaluateMVA(const HTTFatJetF* fj){

    ROOT::Math::LorentzVector<CylLorentzCoordF> sjMom;
    ROOT::Math::LorentzVector<CylLorentzCoordF> sjWMom;
    ROOT::Math::LorentzVector<CylLorentzCoordF> sjWMom2;
    int iBCSV = -1;
    float minPT = 0;
    for(unsigned int iS = 0; iS < fj->nSubjets(); ++iS){
      sjMom += fj->subJet(iS).p4();
      if(minPT <= 0 || fj->subJet(iS).pt() < minPT) minPT = fj->subJet(iS).pt();
    }
    if(fj && fj->nSubjets() > 2){
      sjWMom = fj->subJet(0).p4() + fj->subJet(1).p4();
      if(fj->subJet(0).csv() > fj->subJet(1).csv() && fj->subJet(0).csv() > fj->subJet(2).csv()){
        sjWMom2 = fj->subJet(1).p4() + fj->subJet(2).p4();
        iBCSV = 0;
      } else if(fj->subJet(1).csv() > fj->subJet(0).csv() && fj->subJet(1).csv() > fj->subJet(2).csv()){
        sjWMom2 = fj->subJet(0).p4() + fj->subJet(2).p4();
        iBCSV = 1;
      } else {
        sjWMom2 = fj->subJet(0).p4() + fj->subJet(1).p4();
        iBCSV = 2;
      }
    }

    if(sjMom.pt() < 200) return -1;
    if(sjMom.mass() < 50) return -1;
    if(sjMom.mass() >600) return -1;
    if(minPT < 20) return -1;

    static const int parIndex_pt = mvaWrap.findAxis("pt");
    static vector<double> mva_parameters(1,0);
    mva_parameters[parIndex_pt] = 10;

    mvaWrap.setParameters(mva_parameters);

    static const int index_fj_subjet_pt           =mvaWrap.findVariable("fj_subjet_pt"          );
    static const int index_fj_tau1                =mvaWrap.findVariable("fj_tau1"               );
    static const int index_fj_tau2                =mvaWrap.findVariable("fj_tau2"               );
    static const int index_fj_tau3                =mvaWrap.findVariable("fj_tau3"               );
    static const int index_fj_ropt                =mvaWrap.findVariable("fj_ropt"               );
    static const int index_fj_subjet_mass         =mvaWrap.findVariable("fj_subjet_mass"        );
    static const int index_fj_subjet_w_mass       =mvaWrap.findVariable("fj_subjet_w_mass"      );
    static const int index_fj_subjet_w_pt         =mvaWrap.findVariable("fj_subjet_w_pt"        );
    static const int index_fj_b_pt                =mvaWrap.findVariable("fj_b_pt"               );
    static const int index_fj_b_csv               =mvaWrap.findVariable("fj_b_csv"              );
    static const int index_fj_b_mass              =mvaWrap.findVariable("fj_b_mass"             );
    static const int index_fj_subjet_w_bycsv_mass =mvaWrap.findVariable("fj_subjet_w_bycsv_mass");
    static const int index_fj_subjet_w_bycsv_pt   =mvaWrap.findVariable("fj_subjet_w_bycsv_pt"  );
    static const int index_fj_subjet_b_bycsv_pt   =mvaWrap.findVariable("fj_subjet_b_bycsv_pt"  );
    static const int index_fj_subjet_b_bycsv_csv  =mvaWrap.findVariable("fj_subjet_b_bycsv_csv");
    static const int index_fj_subjet_b_bycsv_mass =mvaWrap.findVariable("fj_subjet_b_bycsv_mass");



    mvaWrap.setVariable(index_fj_subjet_pt            ,sjMom.pt()          );
    mvaWrap.setVariable(index_fj_tau1                 ,fj->tau1()               );
    mvaWrap.setVariable(index_fj_tau2                 ,fj->tau2()               );
    mvaWrap.setVariable(index_fj_tau3                 ,fj->tau3()               );
    mvaWrap.setVariable(index_fj_ropt                 ,fj->ropt()               );
    mvaWrap.setVariable(index_fj_subjet_mass          ,sjMom.mass()        );
    mvaWrap.setVariable(index_fj_subjet_w_mass        ,sjWMom.mass()      );
    mvaWrap.setVariable(index_fj_subjet_w_pt          ,sjWMom.pt()        );
    mvaWrap.setVariable(index_fj_b_pt                 ,fj->subJet(2).pt()               );
    mvaWrap.setVariable(index_fj_b_csv                ,fj->subJet(2).csv()              );
    mvaWrap.setVariable(index_fj_b_mass               ,fj->subJet(2).mass()             );
    mvaWrap.setVariable(index_fj_subjet_w_bycsv_mass  ,sjWMom2.mass());
    mvaWrap.setVariable(index_fj_subjet_w_bycsv_pt    ,sjWMom2.pt()  );
    mvaWrap.setVariable(index_fj_subjet_b_bycsv_pt    ,iBCSV < 0 ? 0 :fj->subJet(iBCSV).pt()  );
    mvaWrap.setVariable(index_fj_subjet_b_bycsv_csv   ,iBCSV < 0 ? 0 :fj->subJet(iBCSV).csv());
    mvaWrap.setVariable(index_fj_subjet_b_bycsv_mass  ,iBCSV < 0 ? 0 :fj->subJet(iBCSV).mass());
    return mvaWrap.evaluate();
  }

  virtual bool fillEvent() override {

    const auto& httFatJets  = httReader.fatJets;
    const auto& ak8FatJets  = fatJetReader.fatJets;

    if(!passEventSel()) return false;
    fillEventProperties();

    std::vector<GenJetF*> filteredGenJes;
    for(auto * j : jets){ if(j->genJet()) filteredGenJes.push_back(j->genJet());}
    PartonMatching::PartonEvent * partonEvent = new PartonMatching::PartonEvent(genParticleReader,*defaultJets,filteredGenJes);
    const PartonMatching::TopDecay * top1 = partonEvent->topDecays.size() > 0 ? &partonEvent->topDecays[0] : 0 ;
    const PartonMatching::TopDecay * top2 = partonEvent->topDecays.size() > 1 ? &partonEvent->topDecays[1] : 0 ;
    if(top1 && top1->isLeptonic) top1 = 0;
    if(top2 && top2->isLeptonic) top2 = 0;

    float minDR1 = -99;
    float minDR2 = -99;
    int matchedFJ1 = -1;
    int matchedFJ2 = -1;
    vector<pair<float,int>> passedFatJets;
    bool match = false;

    //start with CTT
    getMatching(top1,top2,ak8FatJets,minDR1,minDR2,matchedFJ1,matchedFJ2);
    for(unsigned int iJ = 0; iJ < ak8FatJets.size(); ++iJ){
      const auto& fj = ak8FatJets[iJ];
      if(!cfgSet::isSoftDropTagged(&fj,400, 110, 210, 0.69, 1e9)) continue;
      CylLorentzVectorF softDropMom;
      for(unsigned int iS = 0; iS < fj.nSubjets(); ++iS){
        softDropMom += fj.subJet(iS).p4();
      }
      passedFatJets.emplace_back(softDropMom.pt(),iJ);
    }
    sort(passedFatJets.begin(),passedFatJets.end(),PhysicsUtilities::greaterFirst<float,int>());
    match = false;
    if(passedFatJets.size()){
      if(passedFatJets[0].second == matchedFJ1 && minDR1 < 0.8) match = true;
      else if(passedFatJets[0].second == matchedFJ2 && minDR2 < 0.8) match = true;
    }
    data.fill<float>(i_top_CTT_pt                  ,passedFatJets.size() ? passedFatJets[0].first : -1);
    data.fill<bool>(i_top_CTT_isMatched            ,match);

    //Now do HTT
    getMatching(top1,top2,httFatJets,minDR1,minDR2,matchedFJ1,matchedFJ2);
    vector<pair<float,int>> HTTMVAs;
    for(unsigned int iJ = 0; iJ < httFatJets.size(); ++iJ){
      const auto& fj = httFatJets[iJ];
      if(fj.pt() < 200) continue;
      if(fj.absEta() > 2.4) continue;
      HTTMVAs.emplace_back(evaluateMVA(&fj),iJ);
    }

    sort(HTTMVAs.begin(),HTTMVAs.end(),PhysicsUtilities::greaterFirst<float,int>());
    match = false;
    if(HTTMVAs.size()){
      if(HTTMVAs[0].second == matchedFJ1 && minDR1 < 1.5) match = true;
      else if(HTTMVAs[0].second == matchedFJ2 && minDR2 < 1.5) match = true;
    }

//    cout << top1 << " "<< minDR1 <<" "<<matchedFJ1<< " " << HTTMVAs.size() <<" " << (HTTMVAs.size() ? HTTMVAs[0].second : -1) << " "<< match<< endl;

    data.fill<float>(i_top_HTT_pt                  ,HTTMVAs.size() ? httFatJets[HTTMVAs[0].second].ropt_mom().pt() : -1);
    data.fill<float>(i_top_HTT_mva                 ,HTTMVAs.size() ? HTTMVAs[0].first : -1);
    data.fill<bool>(i_top_HTT_isMatched            ,match);


    fillFillingTree();
    return true;
  }

  void book() override {
    i_process              = data.add<unsigned int>("","process"   ,"i",0);
    i_sigType              = data.add<unsigned int>("","sigType"   ,"i",0);
    i_mass1                = data.add<float>("","mass1"            ,"F",0);
    i_mass2                = data.add<float>("","mass2"            ,"F",0);
    i_mass3                = data.add<float>("","mass3"            ,"F",0);
    i_weight               = data.add<float>("","weight"           ,"F",0);
    i_njets                = data.add<unsigned int>("","njets"     ,"i",0);
    i_met                  = data.add<float>("","met"              ,"F",0);

    i_top_HTT_pt        = data.add<float>("","top_HTT_pt"                     ,"F",0);
    i_top_HTT_mva       = data.add<float>("","top_HTT_mva"                     ,"F",0);
    i_top_HTT_isMatched = data.add<bool>("","top_HTT_isMatched"              ,"O",0);
    i_top_CTT_pt           = data.add<float>("","top_CTT_pt"                        ,"F",0);
    i_top_CTT_isMatched    = data.add<bool>("","top_CTT_isMatched"                 ,"O",0);
  }


  //Process description
  size i_process  ;
  size i_sigType  ;
  size i_mass1    ;
  size i_mass2    ;
  size i_mass3    ;
  size i_weight   ;
  //Event properties
  size i_njets   ;
  size i_met     ;

  size i_top_HTT_pt             ;
  size i_top_HTT_mva            ;
  size i_top_HTT_isMatched      ;
  size i_top_CTT_pt             ;
  size i_top_CTT_isMatched      ;

  HTTReader httReader;
  MVAWrapper mvaWrap;

};


#endif

  void boostedTopTesterTreeMaker(TString sname = "T2tt_HTT1p5",
                               const int fileindex = -1,
                               const bool isMC = true,
                               const TString fname = "/store/user/nmccoll/HTTTest/ntuples/merged/T2tt_850_100_ntuple_postproc.root",
                               const float weight = 1.0,
                               const TString outputdir = "trees",
                             const TString fileprefix = "root://cmseos:1094/",
                             const TString json=TString::Format("%s/src/data/JSON/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_v2.txt",getenv("CMSSW_BASE")))
  {


  printf("Processing file %d of %s sample\n", (fileindex > -1 ? fileindex : 0), sname.Data());
  newWeight = weight;

   if(fileindex > -1)
     sname += TString::Format("_%d",fileindex);

   TString fullname = fileprefix+fname;

   gSystem->mkdir(outputdir,true);
   TString outfilename = outputdir+"/"+sname+"_tree.root";
   cfgSet::loadDefaultConfigurations();
   cfgSet::ConfigSet cfg = cfgSet::zl_search_set;


   // disable JetID for signal samples
   if (sname.Contains("T2tt") || sname.Contains("T2tb") || sname.Contains("T2bW") || sname.Contains("T2fbd") || sname.Contains("T2cc")) cfg.jets.applyJetID = false;

    TString treeName = "Events";
    Copier a(fullname,treeName,outfilename.Data(),fileindex +2,isMC, &cfg);
    a.analyze();

}
