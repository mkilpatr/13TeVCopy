//--------------------------------------------------------------------------------------------------
// 
// TreeWriter
// 
// Class to help with tree writing.
// 
// TreeWriter.cc created on Tue Oct 14 22:30:18 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisTools/Utilities/interface/TreeWriter.h"

using namespace ucsbsusy;

TreeWriter::TreeWriter(TTree *tree, const char *treename) :
  fBufSize(32000),
  fSplitLevel(99),
  fTreeName(treename),
  fTree(tree)
{
  fTree->SetName(fTreeName.Data());
}

TreeWriter::~TreeWriter()
{
  delete fTree;
}
