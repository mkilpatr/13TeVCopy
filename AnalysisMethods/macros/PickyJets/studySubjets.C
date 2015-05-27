#if !defined(__CINT__) || defined(__MAKECINT__)
#include "TSystem.h"
#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"
#include "TopDecayHelper.hh"
#include "SubjetHelper.hh"
#endif

using namespace ucsbsusy;
using namespace topdecays;

enum debugOptions {kSilent = 0,
                   kPrintEventDiagnosis = (1 << 0),
                   kPrintJetSplitInfo   = (1 << 1),
                   kPrintIterations     = (1 << 2)};

int verbose = kSilent; //kPrintEventDiagnosis | kPrintJetSplitInfo;

class Analyzer : public TreeCopierAllBranches {

  public :

    Analyzer(TString fileName, TString treeName, TString outfileName, bool isMCTree, TString sname) :
      TreeCopierAllBranches(fileName, treeName, outfileName, isMCTree),
      sname_(sname),
      outfileName_(outfileName),
      evtTree(0),
      evtTreeWriter(0)
    {
      vector<double> mcuts = {10.0, 20.0, 30.0, 40.0, 50.0, 55.0};
      vector<double> rmins = {0.05, 0.10, 0.15, 0.20};
      vector<double> ycuts = {0.05, 0.10, 0.15, 0.20};

      for(auto mcut : mcuts)
        for(auto rmin : rmins)
          for(auto ycut : ycuts) {
            vector<double> tmpcuts = {mcut, rmin, ycut};
            cutsets.push_back(tmpcuts);
          }

      for(auto cutset : cutsets) {
        cachedInfos.push_back(new CachedEventInfo());
        assert(cutset.size() == 3);
        cachedInfos.back()->setCuts(cutset[0], cutset[1], cutset[2]);
        stopSplitting.push_back(false);
      }
    }

    virtual ~Analyzer() {}

    const TString sname_;
    const TString outputdir_;
    const TString outfileName_;

    vector<vector<double> >  cutsets;

    SubjetTreeLoader         vars;
    SubjetTreeFiller         treefiller;

    TTree*                   evtTree;
    TreeWriter*              evtTreeWriter;
    TreeWriterData           evtTreeData;

    vector<CachedEventInfo*> cachedInfos;
    vector<bool>             stopSplitting;

    SplitDecision            newSplitDecision;

    void loadVariables()
    {
      vars.load(&reader);
    }

    void setupTree()
    {
      reader.getTree()->SetBranchStatus("*",1);
      outFile_ = new TFile(outfileName_,"RECREATE");
      outFile_->cd();
      treeWriter_ = new TreeWriter(reader.getTree()->CloneTree(0));
      treeWriter_->setTreeName("Jets");
      evtTree = new TTree("Events","");
      evtTreeWriter = new TreeWriter(evtTree);
    }

    void book()
    {
      treefiller.bookJets(&data);
      for(auto* cachedInfo : cachedInfos) {
        string label = cachedInfo->getLabel();
        treefiller.bookEvents(&evtTreeData, label);
      }
      evtTreeData.book(evtTreeWriter);
    }

    void processVariables() {}

    void analyze(int reportFrequency = 10000, int numEvents = -1);

    bool fillEvent();

};

void Analyzer::analyze(int reportFrequency, int numEvents)
{
  loadVariables();
  isLoaded_ = true;
  setupTree();
  book();
  data.book(treeWriter_);
  evtTreeData.book(evtTreeWriter);
  while(reader.nextEvent(reportFrequency)){
    isProcessed_ = false;
    if(numEvents >= 0 && getEventNumber() >= numEvents) return;
    processVariables();
    data.reset();
    evtTreeData.reset();
    if(!fillEvent()) continue;
    outFile_->cd();
    treeWriter_->fill();
  }
}

bool Analyzer::fillEvent()
{

  if(vars.isGen) return false;

  bool newevent = !cachedInfos[0]->sameEvent(vars.run, vars.lumi, vars.event);

  // fill event tree for previous event if it's a new event
  if(newevent && cachedInfos[0]->run > 0 && !cachedInfos[0]->isfilled) {
    for(unsigned int iinfo = 0; iinfo < cachedInfos.size(); ++iinfo) {
      auto* cachedInfo = cachedInfos[iinfo];
  
      cachedInfo->finalizeDiagnosis();
      treefiller.fillEvent(&evtTreeData, cachedInfo, iinfo);
      if(verbose & kPrintEventDiagnosis) cachedInfo->print();
      cachedInfo->isfilled = true;
    }
    outFile_->cd();
    evtTreeWriter->fill();
  }

  // reset info
  if(newevent || cachedInfos[0]->run == 0) {
    vars.refresh();
    for(auto* cachedInfo : cachedInfos) {
      cachedInfo->reset();
      cachedInfo->setEvent(vars.run, vars.lumi, vars.event);
      cachedInfo->addPartons(vars.partons);
      cachedInfo->addTops(vars.tops);
    }
  } else vars.refreshJets(); 

  if(verbose & kPrintJetSplitInfo) {
    cout << "before" << endl;
    cout << "------" << endl;
  }

  int nSplits = int(vars.numSplits);

  if(nSplits == 0) { // new fat jet
    for(unsigned int iinfo = 0; iinfo < cachedInfos.size(); ++iinfo) {
      auto* cachedInfo = cachedInfos[iinfo];

      cachedInfo->savedReentryPoints.clear();

      AssocJet* fatJet = vars.superJet;
      cachedInfo->fatjets.push_back(fatJet);

      stopSplitting[iinfo] = false; // we're back to first split
    }
  } else {
    for(unsigned int iinfo = 0; iinfo < cachedInfos.size(); ++iinfo) {
      auto* cachedInfo = cachedInfos[iinfo];
      TString label = TString(cachedInfo->getLabel());

      auto index = find(cachedInfo->savedReentryPoints.begin(), cachedInfo->savedReentryPoints.end(), nSplits);

      if(index != cachedInfo->savedReentryPoints.end()) { // return to an iteration that's not yet been processed
        if(verbose & kPrintIterations) {
          cout << label.Data() << endl;
          cout << "found reentry point: " << nSplits << ". indices were " << cachedInfo->printReentries() << endl;
        }

        stopSplitting[iinfo] = false; // reset splitting decision

        cachedInfo->savedReentryPoints.erase(index); // don't want to go back to this index afterwards

        if(verbose & kPrintIterations)
          cout << "indices are " << cachedInfo->printReentries() << endl;
      }
    }
  }

  // analyze this split
  vars.analyzeSplit();

  // print out info before making split decision
  if(verbose & kPrintJetSplitInfo) {
    cout << "nSplits: " << nSplits << endl;
    for(unsigned int iinfo = 0; iinfo < cachedInfos.size(); ++iinfo) {
      auto* cachedInfo = cachedInfos[iinfo];
      TString label = TString(cachedInfo->getLabel());
      cout << label.Data() << endl;
      if(stopSplitting[iinfo]) cout << "\t-->jet: " << vars.superJet->print() << endl;
      else                     cout << "\tjet: " << vars.superJet->print() << endl;
      cout << "\t\tsubjet 1: " << vars.subJet1->print() << endl;
      cout << "\t\tsubjet 2: " << vars.subJet2->print() << endl;
      cout << "\tsaved split decision: " << printSplitDecision(SplitDecision(cachedInfo->savedSplitDecision));
      if(stopSplitting[iinfo]) cout << ", stopped splitting" << endl;
      else                     cout << endl;
      cout << "\tsplit analysis: " << vars.printSplitAnalysis() << endl;
    }
  }

  // make splitting decision for each set of cuts
  for(unsigned int iinfo = 0; iinfo < cachedInfos.size(); ++iinfo) {
    auto* cachedInfo = cachedInfos[iinfo];

    if(!stopSplitting[iinfo]) {
      // print out info after making split decision
      if(verbose & kPrintJetSplitInfo) {
        cout << "after" << endl;
        cout << "------" << endl;
        TString label = TString(cachedInfo->getLabel());
        cout << label.Data() << endl;
      }
  
      if(int(vars.splitDecision)==0) {
        newSplitDecision = NOSPLIT_NO_PARENTS;
        stopSplitting[iinfo] = true;
      }
      else if(vars.superJet->mass() < cachedInfo->mcut) {
        newSplitDecision = NOSPLIT_BELOW_MCUT;
        stopSplitting[iinfo] = true;
      }
      else if(vars.subJetDR() < cachedInfo->rmin) {
        newSplitDecision = NOSPLIT_BELOW_DRMIN;
        stopSplitting[iinfo] = true;
      }
      else if(vars.subJet2->pt() > cachedInfo->ycut*(vars.subJet1->pt()+vars.subJet2->pt())) {
        newSplitDecision = SPLIT_BOTH_SUBJETS;

        if(vars.subJet1->pt() > ptcut) cachedInfo->savedReentryPoints.push_back(nSplits+1);
        if(vars.subJet2->pt() > ptcut) cachedInfo->savedReentryPoints.push_back(nSplits+1);

        if(verbose & kPrintIterations)
          cout << "adding both subjets to iteration. indices: " << cachedInfo->printReentries() << endl;
      }
      else {
        newSplitDecision = SPLIT_LEADING_SUBJET;

        if(vars.subJet1->pt() > ptcut) cachedInfo->savedReentryPoints.push_back(nSplits+1);

        if(verbose & kPrintIterations)
          cout << "adding one subjet to iteration. indices: " << cachedInfo->printReentries() << endl;
      }
  
      if(verbose & kPrintJetSplitInfo) {
        cout << "nSplits: " << nSplits << endl;
        if(stopSplitting[iinfo]) cout << "\t-->jet: " << vars.superJet->print() << endl;
        else                     cout << "\tjet: " << vars.superJet->print() << endl;
        if(newSplitDecision==SPLIT_BOTH_SUBJETS || newSplitDecision==SPLIT_LEADING_SUBJET)
          cout << "\t\t->subjet 1: " << vars.subJet1->print() << endl;
        else
          cout << "\t\tsubjet 1: " << vars.subJet1->print() << endl;
        if(newSplitDecision==SPLIT_BOTH_SUBJETS)
          cout << "\t\t->subjet 2: " << vars.subJet2->print() << endl;
        else
          cout << "\t\tsubjet 2: " << vars.subJet2->print() << endl;
        cout << "\tsplit decision: " << printSplitDecision(newSplitDecision);
        if(stopSplitting[iinfo]) cout << ", stopped splitting" << endl;
        else                     cout << endl;
      }
  
      cachedInfo->savedSplitDecision = newSplitDecision;

      if(stopSplitting[iinfo]) cachedInfo->savedNSplits = vars.numSplits;

      if(stopSplitting[iinfo] && vars.superJet->pt() > ptcut && fabs(vars.superJet->eta()) < maxeta_) {
        AssocJet* jet = vars.superJet;
        jet->setFatJet(cachedInfo->fatjets.back());
        cachedInfo->finaljets.push_back(jet);
      }
    }
  }

  // fill jet info for this splitting iteration
  treefiller.fillJets(&data, &vars, cachedInfos[0]->fatjets.back());

  return true;

}

void studySubjets(TString sname            = "T2tt_850_100_subjets",
                  const int fileindex      = -1,
                  const bool isMC          = true,
                  const TString fname      = "/store/user/vdutta/13TeV/140515/merged/T2tt_850_100_subjet_ntuple.root",
                  const double xsec        = 1.0,
                  const TString outputdir  = "trees",
                  const TString fileprefix = "root://eoscms//eos/cms")
{

  printf("Processing file %d of %s sample\n", (fileindex > -1 ? fileindex : 0), sname.Data());

  if(fileindex > -1)
    sname += TString::Format("_%d",fileindex);

  TString fullname = fileprefix+fname;

  gSystem->mkdir(outputdir,true);
  TString outfilename = outputdir+"/"+sname+"_tree.root";

  Analyzer a(fullname, "TestAnalyzer/Events", outfilename, isMC, sname);

  a.analyze(100000);

}
