//--------------------------------------------------------------------------------------------------
// 
// TriggerInfoReader
// 
// Class for reading trigger object information from TTree
// 
// TriggerInfoReader.cc created on Tue Sep 1 14:56:00 CEST 2015
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisTools/TreeReader/interface/TriggerInfoReader.h"

using namespace std;
using namespace ucsbsusy;

const int TriggerInfoReader::defaultOptions = TriggerInfoReader::LOADOBJ | TriggerInfoReader::FILLOBJ;

//--------------------------------------------------------------------------------------------------
TriggerInfoReader::TriggerInfoReader() : BaseReader() {
  trigflag = new vector<unsigned long> ;
  passtrig = new vector<bool         > ;
  prescale = new vector<unsigned int > ;
}

//--------------------------------------------------------------------------------------------------
void TriggerInfoReader::load(TreeReader *treeReader, int options, string branchName)
{
  loaded_ = true;
  const_cast<int&>(options_)    = options;
  const_cast<string&>(branchName_) = branchName;

  cout << "Loading (" << branchName << ") objects with: ";

  if(options_ & LOADOBJ){
    cout << "obj ";
    treeReader->setBranchAddress(branchName ,"bit_flag"     , &trigflag , true); // data.addMulti<unsigned long>(branchName_,"bit_flag",0);
    treeReader->setBranchAddress(branchName ,"bit_pass"     , &passtrig , true); // data.addMulti<bool         >(branchName_,"bit_pass",0);
    treeReader->setBranchAddress(branchName ,"bit_prescale" , &prescale , true); // data.addMulti<unsigned int >(branchName_,"bit_prescale",0);
  }

  if(options_ & FILLOBJ)
    cout << "+Objects";
  cout << endl;
}

//--------------------------------------------------------------------------------------------------
void TriggerInfoReader::refresh(){
  if(!(options_ & FILLOBJ)) return;
  if(options_ & LOADOBJ){
    triginfo.clear();
    triginfo.reserve(trigflag->size());
    for(unsigned int iF = 0; iF < trigflag->size(); ++iF){
      triginfo.emplace_back(trigflag->at(iF), passtrig->at(iF), prescale->at(iF));
    }
  }
}
