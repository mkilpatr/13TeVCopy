//--------------------------------------------------------------------------------------------------
// 
// TreeReader
// 
// Class for reading information from TTree.
//
// 
//--------------------------------------------------------------------------------------------------
#include <assert.h>
#include <iostream>
#include "AnalysisTools/TreeReader/interface/TreeReader.h"

using namespace ucsbsusy;

//--------------------------------------------------------------------------------------------------
TreeReader::TreeReader(TString fileName, TString treeName, TString readOption) : eventNumber(0)
{
  std::cout << "Loading file: "<< fileName <<" and tree: " << treeName <<std::endl;

  file = TFile::Open(fileName,readOption);
  assert(file);
  tree = (TTree*)(file->Get(treeName) );
  assert(tree);

  std::cout << getEntries() << " entries to process" << std::endl;

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
bool TreeReader::nextEvent(int reportFrequency)
{
  if(eventNumber >= tree->GetEntries()) return false;
  tree->GetEntry(eventNumber);

  if(eventNumber%reportFrequency == 0)
    cout << "Processing event " << eventNumber << endl;

  for(auto reader : readers)
    reader->refresh();

  eventNumber++;
  return true;
}
