#include "AnalysisBase/TreeAnalyzer/interface/TreeCopier.h"


using namespace ucsbsusy;


TreeCopier::TreeCopier(TString fileName, TString treeName, TString outFileName, bool isMCTree,ConfigPars * pars)
: BaseTreeAnalyzer(fileName,treeName,isMCTree,pars,"READ"), outFileName_(outFileName), outFile_(0), treeWriter_(0)
{};
TreeCopier::~TreeCopier(){ if(!outFile_) return; outFile_->cd(); outFile_->Write(0, TObject::kWriteDelete); outFile_->Close(); }

//--------------------------------------------------------------------------------------------------
void TreeCopier::analyze(int reportFrequency, int numEvents)
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
    outFile_->cd();
    treeWriter_->fill();
  }
}


//--------------------------------------------------------------------------------------------------
// TreeFlattenCopier
//--------------------------------------------------------------------------------------------------

TreeFlattenCopier::TreeFlattenCopier(TString fileName, TString treeName, TString outFileName, bool isMCTree,ConfigPars * pars)
: BaseTreeAnalyzer(fileName,treeName,isMCTree,pars,"READ"), outFileName_(outFileName), outFile_(0), treeWriter_(0)
{
  outFile_ = new TFile(outFileName_,"RECREATE");
  outFile_->cd();
  treeWriter_ = new TreeWriter(new TTree(reader.getTree()->GetName(),reader.getTree()->GetTitle()),reader.getTree()->GetName() );
};
TreeFlattenCopier::~TreeFlattenCopier(){outFile_->cd(); outFile_->Write(0, TObject::kWriteDelete); outFile_->Close(); }

//--------------------------------------------------------------------------------------------------
void TreeFlattenCopier::analyze(int reportFrequency, int numEvents)
{
  loadVariables();
  isLoaded_ = true;
  book();
  data.book(treeWriter_);
  while(reader.nextEvent(reportFrequency)){
    isProcessed_ = false;
    if(numEvents >= 0 && getEventNumber() >= numEvents) return;
    processVariables();
    data.reset();
    if(!fillEvent()) continue;
    data.fillLinked();
    size num = data.getVecSize();
    for(size i = 0; i < num; ++i ){
      data.fillLinkedMulti(i);
      outFile_->cd();
      treeWriter_->fill();
    }
  }
}



