#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
//#include "AnalysisTools/QuickRefold/interface/TObjectContainer.h"

using namespace std;
using namespace ucsbsusy;

//#define TEST

class Copier : public TreeCopierManualBranches {
public:
  Copier(string fileName, string treeName, string outFileName, size randSeed, bool isMCTree, cfgSet::ConfigSet * pars) : TreeCopierManualBranches(fileName,treeName,outFileName,randSeed,isMCTree,pars),
//  i_npv       (0),
//  i_rho       (0),
  i_weight    (0),
  i_puWeight  (0),
  i_genjetpt  (0),
  i_genjeteta (0),
  i_genjetrank(0),
  i_flavor    (0),
  i_recojetpt (0)
  {
  };
  virtual ~Copier() {};

  virtual void loadVariables() override {
    load(cfgSet::EVTINFO);
    load(cfgSet::AK4JETS,JetReader::LOADRECO | JetReader::LOADGEN | JetReader::FILLOBJ);
  }

  //--------------------------------------------------------------------------------------------------
  virtual BaseEventAnalyzer * setupEventAnalyzer() override {return new CopierFillYourselfEventAnalyzer();}

  virtual bool fillEvent() override {
#ifdef  TEST
    cout << "------"<<endl;
    cout << *met;
    cout << "------"<<endl;
    for(const auto& j : defaultJets->recoJets){
      cout << j << endl;
    }
    cout << endl;
#endif
//    bool passHBHENoiseFilter                    = this->evtInfoReader.HBHENoiseFilter                   ;
//    bool passHBHENoiseIsoFilter                 = this->evtInfoReader.HBHENoiseIsoFilter                ;
//    bool passCSCTightHalo2015Filter             = this->evtInfoReader.CSCTightHalo2015Filter            ;
//    bool passEcalDeadCellTriggerPrimitiveFilter = this->evtInfoReader.EcalDeadCellTriggerPrimitiveFilter;
//    bool passeeBadScFilter                      = this->evtInfoReader.eeBadScFilter                     ;
//    if(passHBHENoiseFilter && passHBHENoiseIsoFilter && passCSCTightHalo2015Filter && passEcalDeadCellTriggerPrimitiveFilter && passeeBadScFilter && (ljCHEF > 0.1) ){
//    if(passHBHENoiseFilter && passHBHENoiseIsoFilter && passCSCTightHalo2015Filter && passEcalDeadCellTriggerPrimitiveFilter && passeeBadScFilter){
    bool passmetfilters = this->evtInfoReader.HBHENoiseFilter && this->evtInfoReader.HBHENoiseIsoFilter && this->evtInfoReader.globalTightHalo2016Filter && this->evtInfoReader.EcalDeadCellTriggerPrimitiveFilter && this->evtInfoReader.goodVertices && this->evtInfoReader.eeBadScFilter && this->evtInfoReader.badChCand && this->evtInfoReader.badPFMuon;
    float ljCHEF = defaultJets->jetchHadEnFrac_->size() ? defaultJets->jetchHadEnFrac_->at(defaultJets->recoJets[0].index()) : 10;
//    if(passmetfilters && (ljCHEF > 0.1) && (ljCHEF < 0.95)){
    if(passmetfilters && (ljCHEF > 0.1) && (ljCHEF < 0.99)){
      for(unsigned int iJ = 0; iJ < defaultJets->genJets.size(); ++iJ){
        const auto& gJ = defaultJets->genJets[iJ];
        if (gJ.pt()  < 20) continue;
  //      if(iJ > 1) continue;
        //getrecojet
        const RecoJetF* rJ = 0;
        for(unsigned int iR = 0; iR < defaultJets->recoJets.size(); ++iR){
          if(defaultJets->recoJets[iR].genJet() != &gJ) continue;
          rJ = &defaultJets->recoJets[iR];
          break;
        }

  //      data.fill<unsigned int>(i_npv,nPV);
  //      data.fill<float>(i_rho       ,rho);
        data.fill<float>(i_weight    ,weight);
        data.fill<float>(i_puWeight  ,eventCorrections.getPUWeight());
        data.fill<float>(i_genjetpt  ,gJ.pt() );
        data.fill<float>(i_genjeteta ,gJ.eta() );
        data.fill<size8>(i_genjetrank,  std::min(iJ,250u));
        data.fill<size8>(i_flavor    , convertTo<size8>(gJ.flavor(),"Copier::i_flavor"));
        data.fill<float>(i_recojetpt,rJ == 0 ? 9.5 : rJ->pt());
        fillFillingTree();
      }
    }
    return true;
  }

  void book() override {
//    i_npv         = data.add<unsigned int>("","npv"               ,"i",0);
//    i_rho         = data.add<float>("","rho"                      ,"F",0);
    i_weight      = data.add<float>("","weight"                   ,"F",0);
    i_puWeight    = data.add<float>("","puWeight"                 ,"F",0);
    i_genjetpt    = data.add<float>("","genjetpt"                 ,"F",0);
    i_genjeteta   = data.add<float>("","genjeteta"                ,"F",0);
    i_genjetrank  = data.add<size8>("","genjetrank"               ,"s",0);
    i_flavor      = data.add<size8>("","flavor"                   ,"s",0);
    i_recojetpt   = data.add<float>("","recojetpt"                ,"F",0);
  }

  //event level info
//  size i_npv       ;
//  size i_rho       ;
  size i_weight    ;
  size i_puWeight  ;
  size i_genjetpt  ;
  size i_genjeteta ;
  size i_genjetrank;
  size i_flavor    ;
  size i_recojetpt ;
};

#endif

void JetResSkim(string fileName,  int fileIndex = -1, string treeName = "Events", string outPostfix ="jetRes",bool isMC = true) {
  cfgSet::loadDefaultConfigurations();
  cfgSet::ConfigSet cfg = cfgSet::zl_search_set;

#ifdef  TEST
  cfg.corrections.jetAndMETCorrections |= JetAndMETCorrectionSet::JETRESOLUTION;
  cfg.corrections.jetResCorr = 1.1;
#endif

  //get the output name
  TString prefix(fileName);
  prefix.Remove(0,prefix.Last('/') + 1);
  if(prefix.First('.') >= 0) prefix.Resize(prefix.First('.'));
  TString outName = fileIndex < 0 ? TString::Format("%s_%s.root",prefix.Data(),outPostfix.c_str()) : TString::Format("%s_%i_%s.root",prefix.Data(),fileIndex,outPostfix.c_str());

  Copier a(fileName,treeName,outName.Data(),fileIndex+2,isMC, &cfg);

  a.analyze(10000,-1);
}

/*
 * Running
 * nohup root -b -q '/uscms_data/d3/nmccoll/2011-04-15-susyra2/CMSSW_7_4_7/src/AnalysisMethods/macros/JetMETStudies/JetResSkim.C+("/eos/uscms/store/user/ocolegro/13TeV/130815/merged/qcd_ht700to1000-50ns_1_ntuple_postproc.root"  , "Events","jetRes",true)' &
 * For every QCD file
 */
