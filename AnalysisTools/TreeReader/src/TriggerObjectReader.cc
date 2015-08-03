//--------------------------------------------------------------------------------------------------
// 
// TriggerObjectReader
// 
// Class for reading trigger object information from TTree
// 
// TriggerObjectReader.cc created on Mon Jul 20 15:34:06 CEST 2015 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisTools/TreeReader/interface/TriggerObjectReader.h"

using namespace std;
using namespace ucsbsusy;

const int TriggerObjectReader::defaultOptions = TriggerObjectReader::LOADOBJ | TriggerObjectReader::FILLOBJ;

//--------------------------------------------------------------------------------------------------
TriggerObjectReader::TriggerObjectReader() : BaseReader() {
  pt           = new vector<float>;
  eta          = new vector<float>;
  phi          = new vector<float>;
  mass         = new vector<float>;
  pathflags    = new vector<unsigned long>;
  filterflags  = new vector<unsigned long>;
}

//--------------------------------------------------------------------------------------------------
void TriggerObjectReader::load(TreeReader *treeReader, int options, string branchName)
{
  loaded_ = true;
  const_cast<int&>(options_)    = options;
  const_cast<string&>(branchName_) = branchName;

  cout << "Loading (" << branchName << ") objects with: ";

  if(options_ & LOADOBJ){
    cout << "obj ";
    treeReader->setBranchAddress(branchName ,"obj_pt"          , &pt          , true);
    treeReader->setBranchAddress(branchName ,"obj_eta"         , &eta         , true);
    treeReader->setBranchAddress(branchName ,"obj_phi"         , &phi         , true);
    treeReader->setBranchAddress(branchName ,"obj_mass"        , &mass        , true);
    treeReader->setBranchAddress(branchName ,"obj_pathflags"   , &pathflags   , true);
    treeReader->setBranchAddress(branchName ,"obj_filterflags" , &filterflags , true);
  }

  if(options_ & FILLOBJ)
    cout << "+Objects";
  cout << endl;
}

//--------------------------------------------------------------------------------------------------
void TriggerObjectReader::refresh(){
  if(!(options_ & FILLOBJ)) return;
  if(options_ & LOADOBJ){
    trigobjs.clear();
    trigobjs.reserve(pt->size());
    for(unsigned int iL = 0; iL < pt->size(); ++iL){
      trigobjs.emplace_back(CylLorentzVectorF(pt->at(iL),eta->at(iL),phi->at(iL),mass->at(iL)),iL);
      trigobjs.back().setPathFlags(pathflags->at(iL));
      trigobjs.back().setFilterFlags(filterflags->at(iL));
    }
  }
}
