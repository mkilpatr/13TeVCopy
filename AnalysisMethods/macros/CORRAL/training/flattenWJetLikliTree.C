#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisTools/Utilities/interface/TopJetMatching.h"
#include "CORRALHelper.h"

using namespace std;
using namespace ucsbsusy;



class Copier : public TreeCopierManualBranches {
public:
  Copier(string fileName, string treeName, string outFileName, bool isMCTree) : TreeCopierManualBranches(fileName,treeName,outFileName,isMCTree){};
  virtual ~Copier() {};

  void loadVariables(){
    load(EVTINFO);
    load(PICKYJETS,JetReader::FILLOBJ | JetReader::LOADGEN | JetReader::LOADRECO | JetReader::LOADTOPASSOC | JetReader::LOADJETSHAPE );
    load(GENPARTICLES, GenParticleReader::FILLOBJ | GenParticleReader::LOADPARTONDECAY);
  }


  void analyze(int reportFrequency = 100000, int numEvents = -1) override
  {
    loadVariables();
    isLoaded_ = true;
    setupTree();
    book();
    data.book(treeWriter_);
    while(reader.nextEvent(reportFrequency)){
      isProcessed_ = false;
      if(numEvents >= 0 && getEventNumber() >= numEvents) return;
      processVariables();
      data.reset();
      if(!fillEvent()) continue;
    }
  }

  bool fillEvent() {
    auto * jetReader = &pickyJetReader;
    std::vector<RecoJetF*> recoJets;
    std::vector<TopDecayEvent::DecayID> decays;
    if(!setup(&genParticleReader,jetReader, recoJets,decays)) return false;
    vector<int> types;
    getJetCandidates(recoJets,decays,types);


    for(unsigned int iJ = 0; iJ < recoJets.size(); ++iJ){
      const auto * j = recoJets[iJ];
      if(types[iJ] == 0) continue; //veto Bs
      auto v = calculateJetCandVars(nPV,jetReader,j);
      data.fill<float>(jetPT   ,float(v.jetPT));
      data.fill<float>(axis1   ,float(v.axis1));
      data.fill<float>(axis2   ,float(v.axis2));
      data.fill<float>(ptD     ,float(v.ptD));
      data.fill<int  >(jetMult ,int(v.jetMult));
      data.fill<float>(betaStar,float(v.betaStar));
      data.fill<float>(area    ,float(v.area));
      data.fill<float>(mass    ,float(v.mass));
      data.fill<bool >(isWJet  ,types[iJ] == 1);

      outFile_->cd();
      treeWriter_->fill();
    }
    return true;
  }

  void book() {
    jetPT       = data.add<float>("","jetPT"    ,"F",0);
    axis1       = data.add<float>("","axis1"    ,"F",0);
    axis2       = data.add<float>("","axis2"    ,"F",0);
    ptD         = data.add<float>("","ptD"      ,"F",0);
    jetMult     = data.add<int  >("","jetMult"  ,"I",0);
    betaStar    = data.add<float>("","betaStar" ,"F",0);
    area        = data.add<float>("","area"     ,"F",0);
    mass        = data.add<float>("","mass"     ,"F",0);
    isWJet      = data.add<bool >("","isWJet"   ,"O",0);

  }


  size jetPT    ;
  size axis1    ;
  size axis2    ;
  size ptD      ;
  size jetMult  ;
  size betaStar ;
  size area     ;
  size mass     ;
  size isWJet   ;
};


#endif



void flattenWJetLikliTree(string fileName = "evttree.root", string treeName = "TestAnalyzer/Events", string outFileName ="wJetLikliTree.root", bool isMCTree = true) {
  Copier a(fileName,treeName,outFileName,isMCTree);
  a.analyze();
}
