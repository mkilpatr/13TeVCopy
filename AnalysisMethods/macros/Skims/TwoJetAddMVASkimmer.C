/*
 * Uses the tree copier to copy over a tree!
 *
 * We use the loaded branches option so that we make the tree smaller
 *
 * Use this guy as a base class or an example!
 *
 */
#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"
#include "AnalysisTools/ObjectSelection/interface/TauMVA.h"
#include "TSystem.h"
#endif



using namespace std;
using namespace ucsbsusy;

class Copier : public TreeCopierAllBranches {
public:
  Copier(string fileName, string treeName, string outFileName, bool isMCTree, cfgSet::ConfigSet * pars) : TreeCopierAllBranches(fileName,treeName,outFileName,isMCTree,pars) {
    tauMVA_ = new TauMVA(getenv("CMSSW_BASE") + string("/src/data/Taus/tauDisc_mt100presel_S15.root"), "mva_0");
  };

  virtual ~Copier() {};

  size    taumva_new;
  TauMVA* tauMVA_;

  vector<RecoJetF*>  ak4jets30;

  virtual void loadVariables(){
    load(cfgSet::EVTINFO);
    load(cfgSet::AK4JETS,JetReader::LOADRECO | JetReader::FILLOBJ);
    load(cfgSet::PFCANDS,PFCandidateReader::LOADRECO | PFCandidateReader::LOADEXTRECO | PFCandidateReader::FILLOBJ | PFCandidateReader::LOADTAUVETOMT);
  }

  virtual void processVariables() {
    BaseTreeAnalyzer::processVariables();

    ak4jets30.clear();
    ak4jets30.reserve(ak4Reader.recoJets.size());

    if(ak4Reader.isLoaded()) {
      for(auto& jet : ak4Reader.recoJets) {
        ak4jets30.push_back(&jet);
      }
    }

  }

  virtual bool fillEvent() {

    if(nJets < 2) return false;

    for(auto& pfc : pfcandReader.extpfcands) {
      if(!pfc.ischargedhadron()) {
        data.fillMulti<float>(taumva_new);
        continue;
      }
      const RecoJetF* jet = pfc.jetIndex() > -1 && pfc.jetIndex() < int(ak4jets30.size()) ? ak4jets30.at(pfc.jetIndex()) : 0;
      float contjetcsv = (pfc.jetIndex() > -1 && pfc.jetIndex() < int(ak4jets30.size()) &&  jet->pt() > 30.0 && fabs(jet->eta()) < 2.4) ? jet->csv() : 0.0;
      float taumva = tauMVA_->evaluateMVA(pfc.pt(), pfc.eta(), pfc.dz(), pfc.chiso0p1(), pfc.chiso0p2(), pfc.chiso0p3(), pfc.chiso0p4(), pfc.totiso0p1(), pfc.totiso0p2(), pfc.totiso0p3(), pfc.totiso0p4(), pfc.neartrkdr(), pfc.contjetdr(), contjetcsv);
      data.fillMulti<float>(taumva_new, taumva);
    }

    return true;
  }

  void book() {
    taumva_new = data.addMulti<float>("pfcand","taudisc_s15",-10.0);
  }


};


void TwoJetAddMVASkimmer(string fileName, string filePrefix="root://cmseos:1094/", string outputDir = "skims", string outPostfix ="twojetskim") {

  cfgSet::loadDefaultConfigurations();
  cfgSet::ConfigSet cfg = cfgSet::zl_search_set;

  //get the output name
  TString prefix(fileName);
  prefix.Remove(0,prefix.Last('/') + 1);
  if(prefix.First('.') >= 0) prefix.Resize(prefix.First('.'));
  TString outName = TString::Format("%s/%s_%s.root",outputDir.c_str(), prefix.Data(),outPostfix.c_str());
  bool isMC = !outName.Contains("2015");
  string fullName = filePrefix+fileName;
  gSystem->mkdir(outputDir.c_str(), true);
  Copier a(fullName,"Events",outName.Data(),isMC, &cfg);

  a.analyze();
}
