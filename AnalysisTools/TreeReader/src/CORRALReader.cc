
#include "AnalysisTools/TreeReader/interface/CORRALReader.h"
#include "AnalysisTools/TreeReader/interface/TreeReader.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"


using namespace std;
using namespace ucsbsusy;

const int CORRALReader::defaultOptions = CORRALReader::NULLOPT;

//--------------------------------------------------------------------------------------------------
CORRALReader::CORRALReader() : BaseReader(){
  bjetind  = new vector<int16>;
  wjet1ind = new vector<int16>;
  wjet2ind = new vector<int16>;
  wmva     = new vector<float>;
  tmva     = new vector<float>;
}

//--------------------------------------------------------------------------------------------------
void CORRALReader::load(TreeReader *treeReader, int options, string branchName)
{
    loaded_ = true;
    const_cast<int&>(options_)    = options;
    const_cast<string&>(branchName_) = branchName;

    clog << "Loading (" << branchName << ") jets with: ";

    treeReader->setBranchAddress(branchName_, "bjetind"       , &bjetind     ,true);
    treeReader->setBranchAddress(branchName_, "wjet1ind"      , &wjet1ind    ,true);
    treeReader->setBranchAddress(branchName_, "wjet2ind"      , &wjet2ind    ,true);
    treeReader->setBranchAddress(branchName_, "wmva"          , &wmva        ,true);
    treeReader->setBranchAddress(branchName_, "tmva"          , &tmva        ,true);

  clog << endl;
}

//--------------------------------------------------------------------------------------------------
void CORRALReader::refresh(){
}

//--------------------------------------------------------------------------------------------------
int CORRALReader::transformIndex(int inIndex, const vector<RecoJetF*>& recoJets, bool forceMatch) const{
  for(unsigned int iJ = 0; iJ <  recoJets.size(); ++iJ)
    if(inIndex == recoJets[iJ]->index())
      return iJ;

  if(forceMatch){
    throw std::range_error(( TString("CORRALReader::transformIndex you gave an invalid index!" )).Data() );
  }

  return -1;
}
