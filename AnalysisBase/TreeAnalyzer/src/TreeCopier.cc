#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"


using namespace ucsbsusy;

//--------------------------------------------------------------------------------------------------

void CopierEventAnalyzer::analyzeEvent(BaseTreeAnalyzer * ana, int reportFrequency, int numEvents){
  TreeCopier * newAna = dynamic_cast<TreeCopier*>(ana);
  if (newAna==0) throw std::invalid_argument("TreeCopierEventAnalyzer::analyzeEvent: Can only be used in an analyzer that inherits from TreeCopier!");

  newAna->loadVariables();
  newAna->setLoaded(true);
  newAna->setupTree();
  newAna->book();
  newAna->bookFillingTree();
  while(newAna->nextEvent(reportFrequency)){
    newAna->setProcessed(false);
    if(numEvents >= 0 && newAna->getEventNumber() >= numEvents) return;
    newAna->processVariables();
    newAna->resetFillingData();
    if(!newAna->fillEvent()) continue;
    newAna->fillFillingTree();
  }
}

void CopierFillYourselfEventAnalyzer::analyzeEvent(BaseTreeAnalyzer * ana, int reportFrequency, int numEvents){
  TreeCopier * newAna = dynamic_cast<TreeCopier*>(ana);
  if (newAna==0) throw std::invalid_argument("TreeCopierEventAnalyzer::analyzeEvent: Can only be used in an analyzer that inherits from TreeCopier!");

  newAna->loadVariables();
  newAna->setLoaded(true);
  newAna->setupTree();
  newAna->book();
  newAna->bookFillingTree();
  while(newAna->nextEvent(reportFrequency)){
    newAna->setProcessed(false);
    if(numEvents >= 0 && newAna->getEventNumber() >= numEvents) return;
    newAna->processVariables();
    newAna->resetFillingData();
    newAna->fillEvent();
  }
}

TreeCopier::TreeCopier(TString fileName, TString treeName, TString outFileName, size randomSeed, bool isMCTree,cfgSet::ConfigSet * pars)
: BaseTreeAnalyzer(fileName,treeName,randomSeed,isMCTree,pars), outFileName_(outFileName), outFile_(0), treeWriter_(0)
{};
TreeCopier::~TreeCopier(){ if(!outFile_) return; outFile_->cd(); outFile_->Write(0, TObject::kWriteDelete); outFile_->Close(); }


//--------------------------------------------------------------------------------------------------
// TreeFlattenCopier
//--------------------------------------------------------------------------------------------------

TreeFlattenCopier::TreeFlattenCopier(TString fileName, TString treeName, TString outFileName, size randomSeed, bool isMCTree,cfgSet::ConfigSet * pars)
: BaseTreeAnalyzer(fileName,treeName,randomSeed,isMCTree,pars), outFileName_(outFileName), outFile_(0), treeWriter_(0)
{
  outFile_ = new TFile(outFileName_,"RECREATE");
  outFile_->cd();
  treeWriter_ = new TreeWriter(new TTree(reader.getTree()->GetName(),reader.getTree()->GetTitle()),reader.getTree()->GetName() );
};
TreeFlattenCopier::~TreeFlattenCopier(){outFile_->cd(); outFile_->Write(0, TObject::kWriteDelete); outFile_->Close(); }

void TreeFlattenCopier::fillFillingTree() {
  data.fillLinked();
  size num = data.getVecSize();
  for(size i = 0; i < num; ++i ){
    data.fillLinkedMulti(i);
    outFile_->cd();
    treeWriter_->fill();
  }
}


//--------------------------------------------------------------------------------------------------

void FlattenCopierEventAnalyzer::analyzeEvent(BaseTreeAnalyzer * ana, int reportFrequency, int numEvents){
  TreeFlattenCopier * newAna = dynamic_cast<TreeFlattenCopier*>(ana);
  if (newAna==0) throw std::invalid_argument("TreeFlattenCopierEventAnalyzer::analyzeEvent: Can only be used in an analyzer that inherits from TreeFlattenCopier!");

  newAna->loadVariables();
  newAna->setLoaded(true);
  newAna->book();
  newAna->bookFillingTree();
  while(newAna->nextEvent(reportFrequency)){
    newAna->setProcessed(false);
    if(numEvents >= 0 && newAna->getEventNumber() >= numEvents) return;
    newAna->processVariables();
    newAna->resetFillingData();
    if(!newAna->fillEvent()) continue;
    newAna->fillFillingTree();
  }
}
