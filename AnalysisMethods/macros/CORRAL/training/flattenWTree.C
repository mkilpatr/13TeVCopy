#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisTools/Utilities/interface/PartonMatching.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "ObjectProducers/TopTagging/interface/CORRAL.h"
using namespace std;
using namespace ucsbsusy;
using namespace CORRAL;

class Copier : public TreeCopierManualBranches {
public:
  WJetLikliMVA  wJetLikliMVA;


  Copier(string fileName, string treeName, string outFileName, bool isMCTree) : TreeCopierManualBranches(fileName,treeName,outFileName,isMCTree)
  , wJetLikliMVA("T2tt_merged_wJetLikli_disc.root","mva_0")
  {};
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
    std::vector<PartonMatching::DecayID> decays;
    if(!setup(&genParticleReader,jetReader, recoJets,decays)) return false;

    vector<jetCandVars> jetVars(recoJets.size());
    for(unsigned int iJ = 0; iJ < recoJets.size(); ++iJ){
      jetVars[iJ] = calculateJetCandVars(nPV,jetReader,recoJets[iJ]);
      wJetLikliMVA.mvaVal(jetVars[iJ]);
    }

    vector<WCand> wCands;
    getWCandidates(recoJets,decays,wCands);
    for(auto& cand : wCands){
      auto vars = calculateWCandVars(jetReader,recoJets,jetVars,cand);
//      if(vars.wMass > 200)      continue;
//      if(vars.maxCSV > .941)     continue;
//      if(vars.wJetLikli2 < -.8) continue;
//      if(vars.deta > 2.5)        continue;
//      if(vars.dr > 3.5)         continue;

      data.fill<float>(wPT       ,float(vars.wPT               ));
      data.fill<float>(wMass     ,float(min(vars.wMass,float(500))));
      data.fill<float>(pt2opt1   ,float(vars.pt2opt1           ));
      data.fill<float>(wJetLikli1,float(vars.wJetLikli1        ));
      data.fill<float>(wJetLikli2,float(vars.wJetLikli2        ));
      data.fill<float>(maxCSV    ,float(max(vars.maxCSV,float(0))));
      data.fill<float>(dr        ,float(vars.dr                ));
      data.fill<float>(deta      ,float(vars.deta              ));
      data.fill<float>(dphi      ,float(vars.dphi              ));
      data.fill<float>(charge    ,float(TMath::Abs(vars.charge)));
      data.fill<float>(pullAng1  ,float(vars.pullAng1          ));
      data.fill<float>(pullAng2  ,float(vars.pullAng2          ));
      data.fill<int  >(nWCon     ,int  (vars.nWCon             ));
      data.fill<bool >(isW       ,cand.isW                      );
      outFile_->cd();
      treeWriter_->fill();
    }

    return true;
  }

  void book() {

    wPT          = data.add<float>("","wPT"       ,"F",0);
    wMass        = data.add<float>("","mass"      ,"F",0);
    pt2opt1      = data.add<float>("","pt2opt1"   ,"F",0);
    wJetLikli1   = data.add<float>("","wJetLikli1","F",0);
    wJetLikli2   = data.add<float>("","wJetLikli2","F",0);
    maxCSV       = data.add<float>("","maxCSV"    ,"F",0);
    dr           = data.add<float>("","dr"        ,"F",0);
    deta         = data.add<float>("","deta"      ,"F",0);
    dphi         = data.add<float>("","dphi"      ,"F",0);
    charge       = data.add<float>("","charge"    ,"F",0);
    pullAng1     = data.add<float>("","pullAng1"  ,"F",0);
    pullAng2     = data.add<float>("","pullAng2"  ,"F",0);
    nWCon        = data.add<int  >("","nWCon"     ,"I",0);
    isW          = data.add<bool >("","isW"       ,"O",0);



  }
  size wPT         ;
  size wMass       ;
  size pt2opt1     ;
  size wJetLikli1  ;
  size wJetLikli2  ;
  size maxCSV      ;
  size dr          ;
  size deta        ;
  size dphi        ;
  size charge      ;
  size pullAng1    ;
  size pullAng2    ;
  size nWCon       ;
  size isW         ;

};


#endif



void flattenWTree(string fileName = "evttree.root", string treeName = "TestAnalyzer/Events", string outFileName ="newTree.root", bool isMCTree = true) {
  Copier a(fileName,treeName,outFileName,isMCTree);
  a.analyze();
}
