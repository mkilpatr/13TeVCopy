//--------------------------------------------------------------------------------------------------
// 
// TreeReader
// 
// Class for reading information from TTree.
//
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisTools/TreeReader/interface/TreeReader.h"

using namespace ucsbsusy;

//--------------------------------------------------------------------------------------------------
TreeReader::TreeReader(TString fileName, TString treeName, TString readOption) : eventNumber(0)
{
  cout << "Loading file: "<< fileName <<" and tree: " << treeName <<endl;

  file = new TFile(fileName,readOption);
  assert(file);
  tree = (TTree*)(file->Get(treeName) );
  assert(tree);
}
//--------------------------------------------------------------------------------------------------
TreeReader::~TreeReader()
{
  file->Close();
  delete file;
}
//--------------------------------------------------------------------------------------------------
void TreeReader::load(BaseReader * reader, int options, string branchName)
{
  reader->load(tree,options,branchName);
  readers.push_back(reader);
}
//--------------------------------------------------------------------------------------------------
bool TreeReader::nextEvent(bool verbose)
{
  if(eventNumber >= tree->GetEntries()) return false;
  tree->GetEntry(eventNumber);

  if(verbose)
    cout << "Running over event: " << eventNumber << endl;

  for(auto reader : readers)
    reader->refresh();

  eventNumber++;
  return true;
}
