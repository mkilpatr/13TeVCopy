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
TreeReader::TreeReader(TString fileName, TString treeName, bool isMCTree, TString readOption) : isMC(isMCTree) , eventNumber(0),
    ak4Reader(0)
    , electronReader(0)
    , muonReader(0)
    , tauReader(0)
    , ak4Jets (0)
    , ak4GenJets(0)
    , electrons(0)
    , muons(0)
    , taus(0)
{
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
void TreeReader::load(VarType type, int options, string branchName)
{
  switch (type){
  case AK4JETS : {
    ak4Reader = new JetReader();
    int defaultOptions = JetReader::defaultOptions | (isMC ? JetReader::LOADGEN : JetReader::NULLOPT);
    ak4Reader->load(tree, options < 0 ? defaultOptions : options, branchName == "" ? "ak4pfchs" : branchName );
    readers.push_back(ak4Reader);
    ak4Jets = &ak4Reader->recoJets;
    ak4GenJets = &ak4Reader->genJets;
    break;
  }
  case ELECTRONS : {
    electronReader = new ElectronReader();
    int defaultOptions = ElectronReader::defaultOptions;
    electronReader->load(tree, options < 0 ? defaultOptions : options, branchName == "" ? "ele" : branchName );
    readers.push_back(electronReader);
    electrons = &electronReader->electrons;
    break;
  }
  case MUONS : {
    muonReader = new MuonReader();
    int defaultOptions = MuonReader::defaultOptions;
    muonReader->load(tree, options < 0 ? defaultOptions : options, branchName == "" ? "mu" : branchName );
    readers.push_back(muonReader);
    muons = &muonReader->muons;
    break;
  }
  case TAUS : {
    tauReader = new TauReader();
    int defaultOptions = TauReader::defaultOptions;
    tauReader->load(tree, options < 0 ? defaultOptions : options, branchName == "" ? "tau" : branchName );
    readers.push_back(tauReader);
    taus = &tauReader->taus;
    break;
  }
  default :{
    cout << endl << "No settings for type: " << type << " found!"<<endl;
    break;
  }
  }
}
//--------------------------------------------------------------------------------------------------
void TreeReader::load(BaseReader * reader, int options, string branchName)
{
  reader->load(tree,options,branchName);
  readers.push_back(reader);
}
//--------------------------------------------------------------------------------------------------
bool TreeReader::nextEvent()
{
  if(eventNumber >= tree->GetEntries()) return false;
  tree->GetEntry(eventNumber);

  for(auto reader : readers)
    reader->refresh();

  eventNumber++;
  return true;
}
