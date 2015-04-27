#if !defined(__CINT__) || defined(__MAKECINT__)
#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "AnalysisTools/Utilities/interface/TopJetMatching.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "ObjectProducers/TopTagging/interface/CORRAL.h"
using namespace std;
using namespace ucsbsusy;
using namespace CORRAL;

class Copier : public TreeCopierManualBranches {
public:
  WJetLikliMVA  wJetLikliMVA;
  WMVA  wMVA;


  Copier(string fileName, string treeName, string outFileName, bool isMCTree, bool onlyGoodTops_) : TreeCopierManualBranches(fileName,treeName,outFileName,isMCTree)
  , wJetLikliMVA("T2tt_merged_wJetLikli_disc.root","mva_0")
  , wMVA("T2tt_merged_wCand_disc.root","mva_0")
  , onlyGoodTops(onlyGoodTops_)

  {};
  virtual ~Copier() {};

   bool onlyGoodTops;

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
    std::vector<TopJetMatching::TopDecayEvent::DecayID> decays;
    if(!setup(&genParticleReader,jetReader, recoJets,decays)) return false;


    if(onlyGoodTops){
      std::vector<RecoJetF*> newRecoJets;
      std::vector<TopJetMatching::TopDecayEvent::DecayID> newDecays;
      for(unsigned int iJ = 0; iJ < recoJets.size(); ++iJ){
        if(decays[iJ].type < TopJetMatching::TopDecayEvent::DecayID::TOP_B ) continue;
        newRecoJets.push_back(recoJets[iJ]);
        newDecays.push_back(decays[iJ]);
      }
      recoJets = newRecoJets;
      decays = newDecays;
    }

    vector<jetCandVars> jetVars(recoJets.size());
    for(unsigned int iJ = 0; iJ < recoJets.size(); ++iJ){
      jetVars[iJ] = calculateJetCandVars(nPV,jetReader,recoJets[iJ]);
      wJetLikliMVA.mvaVal(jetVars[iJ]);
    }

    vector<WCand> wCands;
    getWCandidates(recoJets,decays,wCands);
//    if(onlyGoodTops){
//      vector<WCand> newWCands;
//      for(unsigned int iC = 0; iC < wCands.size(); ++iC){
//        if(!wCands[iC].isW ) continue;
//        newWCands.push_back(wCands[iC]);
//      }
//      wCands.clear();
//      for(unsigned int iC = 0; iC < newWCands.size(); ++iC){
//        wCands.push_back(newWCands[iC]);
//      }
//    }

    vector<WCandVars> wCandVars(wCands.size());
    for(unsigned int iC = 0; iC < wCands.size(); ++iC){
      wCandVars[iC] = calculateWCandVars(jetReader,recoJets,jetVars,wCands[iC]);
      wMVA.mvaVal(wCandVars[iC]);
    }

    vector<TCand> tCands;
    getTCandidates(wMVA,recoJets,decays,wCands,wCandVars, tCands);
    if(onlyGoodTops){
      vector<TCand> newTCands;
      for(unsigned int iC = 0; iC < tCands.size(); ++iC){
        if(tCands[iC].type != 1 ) continue;
        newTCands.push_back(tCands[iC]);
      }
      tCands.clear();
      for(unsigned int iC = 0; iC < newTCands.size(); ++iC){
        tCands.push_back(newTCands[iC]);
      }
    }
    vector<TCandVars> tCandVars(tCands.size());
    for(unsigned int iC = 0; iC < tCands.size(); ++iC){
      tCandVars[iC] = calculateTCandVars(jetReader,recoJets,wCands,jetVars,wCandVars,tCands[iC]);
    }


    for(unsigned int iC = 0; iC < tCands.size(); ++iC){
      if(tCands[iC].type == 0) continue;
      auto& vars = tCandVars[iC];
      data.fill<float>(tPT          ,float(vars.tPT)            );
      data.fill<float>(wPT          ,float(vars.wPT)            );
      data.fill<float>(tMass        ,min(float(500),vars.tMass) );
      data.fill<float>(wMass        ,min(float(500),vars.wMass) );
      data.fill<float>(bPTotPT      ,min(float(5),vars.bPTotPT) );
      data.fill<float>(bCSV         ,max(float(0),vars.bCSV)    );
      data.fill<float>(maxWCSV      ,max(float(0),vars.maxWCSV) );
      data.fill<float>(bWLikli      ,float(vars.bWLikli      )  );
      data.fill<float>(wDisc        ,float(vars.wDisc        )  );
      data.fill<float>(maxOWDisc    ,float(vars.maxOWDisc    )  );
      data.fill<float>(m23om123     ,float(vars.m23om123     )  );
      data.fill<float>(m13om12      ,float(vars.m13om12      )  );
      data.fill<float>(atan_m13om12 ,float(vars.atan_m13om12 )  );
      data.fill<float>(maxjjdr      ,float(vars.maxjjdr      )  );
      data.fill<float>(wbDR         ,float(vars.wbDR         )  );
      data.fill<float>(wbDEta       ,float(vars.wbDEta       )  );
      data.fill<float>(wbDPhi       ,float(vars.wbDPhi       )  );
      data.fill<int  >(nTCon        ,int(vars.nTCon        )    );
      data.fill<bool >(isT          ,tCands[iC].type == 1                      );
      outFile_->cd();
      treeWriter_->fill();
    }


    return true;
  }

  void book() {

    tPT          = data.add<float>("","tPT"         ,"F",0);
    wPT          = data.add<float>("","wPT"         ,"F",0);
    tMass        = data.add<float>("","tMass"       ,"F",0);
    wMass        = data.add<float>("","wMass"       ,"F",0);
    bPTotPT      = data.add<float>("","bPTotPT"     ,"F",0);
    bCSV         = data.add<float>("","bCSV"        ,"F",0);
    maxWCSV      = data.add<float>("","maxWCSV"     ,"F",0);
    bWLikli      = data.add<float>("","bWLikli"     ,"F",0);
    wDisc        = data.add<float>("","wDisc"       ,"F",0);
    maxOWDisc    = data.add<float>("","maxOWDisc"   ,"F",0);
    m23om123     = data.add<float>("","m23om123"    ,"F",0);
    m13om12      = data.add<float>("","m13om12"     ,"F",0);
    atan_m13om12 = data.add<float>("","atan_m13om12","F",0);
    maxjjdr      = data.add<float>("","maxjjdr"     ,"F",0);
    wbDR         = data.add<float>("","wbDR"        ,"F",0);
    wbDEta       = data.add<float>("","wbDEta"      ,"F",0);
    wbDPhi       = data.add<float>("","wbDPhi"      ,"F",0);
    nTCon        = data.add<int  >("","nTCon"       ,"I",0);
    isT          = data.add<bool >("","isT"         ,"O",0);



  }

  size tPT          ;
  size wPT          ;
  size tMass        ;
  size wMass        ;
  size bPTotPT      ;
  size bCSV         ;
  size maxWCSV      ;
  size bWLikli      ;
  size wDisc        ;
  size maxOWDisc    ;
  size m23om123     ;
  size m13om12      ;
  size atan_m13om12 ;
  size maxjjdr      ;
  size wbDR         ;
  size wbDEta       ;
  size wbDPhi       ;
  size nTCon        ;
  size isT          ;



};


#endif



void flattenTTree(string fileName = "evttree.root", string treeName = "TestAnalyzer/Events", string outFileName ="newTree.root", bool isMCTree = true, bool onlyGoodTops = false) {
  Copier a(fileName,treeName,outFileName,isMCTree,onlyGoodTops);
  a.analyze(10000);
}
