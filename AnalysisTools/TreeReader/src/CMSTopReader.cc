//--------------------------------------------------------------------------------------------------
// 
// JetReader
// 
// Class for reading electron object information from TTree.
// 
// ElectronReader.cc created on Wed Oct 22 01:39:44 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisTools/TreeReader/interface/CMSTopReader.h"
#include "AnalysisTools/TreeReader/interface/TreeReader.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"

using namespace std;
using namespace ucsbsusy;

const int CMSTopReader::defaultOptions = CMSTopReader::LOADRECO | CMSTopReader::FILLOBJ;

//--------------------------------------------------------------------------------------------------
CMSTopReader::CMSTopReader() : BaseReader(){
  fjmass_           = new vector<float>;
  fjtrimmedmass_    = new vector<float>;
  fjprunedmass_     = new vector<float>;
  fjsoftdropmass_   = new vector<float>;
  fjfilteredmass_   = new vector<float>;
  fjmdfilteredmass_ = new vector<float>;
  fjtau1_           = new vector<float>;
  fjtau2_           = new vector<float>;
  fjtau3_           = new vector<float>;
  toppt_            = new vector<float>;
  topeta_           = new vector<float>;
  topphi_           = new vector<float>;
  topmass_          = new vector<float>;
  wmass_            = new vector<float>;
  minmass_          = new vector<float>;
  nsubjets_         = new vector<int>  ;

}

//--------------------------------------------------------------------------------------------------
void CMSTopReader::load(TreeReader *treeReader, int options, string branchName)
{
    loaded_ = true;
    const_cast<int&>(options_)    = options;
    const_cast<string&>(branchName_) = branchName;

    clog << "Loading (" << branchName << ") tops with: ";

    treeReader->setBranchAddress(branchName_, "fatjet_mass"                , &fjmass_          ,true);
    treeReader->setBranchAddress(branchName_, "fatjet_trimmedmass"         , &fjtrimmedmass_   ,true);
    treeReader->setBranchAddress(branchName_, "fatjet_prunedmass"          , &fjprunedmass_    ,true);
    treeReader->setBranchAddress(branchName_, "fatjet_softdropmass"        , &fjsoftdropmass_  ,true);
    treeReader->setBranchAddress(branchName_, "fatjet_filteredmass"        , &fjfilteredmass_  ,true);
    treeReader->setBranchAddress(branchName_, "fatjet_massdropfilteredmass", &fjmdfilteredmass_,true);    
    treeReader->setBranchAddress(branchName_, "fatjet_tau1"                , &fjtau1_          ,true);
    treeReader->setBranchAddress(branchName_, "fatjet_tau2"                , &fjtau2_          ,true);
    treeReader->setBranchAddress(branchName_, "fatjet_tau3"                , &fjtau3_          ,true);
    treeReader->setBranchAddress(branchName_, "top_pt"                     , &toppt_           ,true);
    treeReader->setBranchAddress(branchName_, "top_eta"                    , &topeta_          ,true);
    treeReader->setBranchAddress(branchName_, "top_phi"                    , &topphi_          ,true);
    treeReader->setBranchAddress(branchName_, "top_topmass"                , &topmass_         ,true);
    treeReader->setBranchAddress(branchName_, "top_wmass"                  , &wmass_           ,true);
    treeReader->setBranchAddress(branchName_, "top_minmass"                , &minmass_         ,true);
    treeReader->setBranchAddress(branchName_, "top_nsubjets"               , &nsubjets_        ,true);


    if(options_ & FILLOBJ)
      cout << "+Objects";
    clog << endl;
}

//--------------------------------------------------------------------------------------------------
void CMSTopReader::refresh(){

  if(!(options_ & FILLOBJ)) return;

  cmsTops.clear();
  cmsTops.reserve(toppt_->size()); 
  for(unsigned int iJ = 0; iJ < toppt_->size(); ++iJ){
    cmsTops.emplace_back(CylLorentzVectorF(toppt_->at(iJ),topeta_->at(iJ),topphi_->at(iJ),topmass_->at(iJ)),iJ);
    cmsTops.back().setFJMass(fjmass_->at(iJ));
    cmsTops.back().setFJTrimmedMass(fjtrimmedmass_->at(iJ));
    cmsTops.back().setFJPrunedMass(fjprunedmass_->at(iJ));
    cmsTops.back().setFJSoftDropMass(fjsoftdropmass_->at(iJ));
    cmsTops.back().setFJFilteredMass(fjfilteredmass_->at(iJ));
    cmsTops.back().setFJMDFilteredMass(fjmdfilteredmass_->at(iJ));
    cmsTops.back().setFJTau1(fjtau1_->at(iJ));
    cmsTops.back().setFJTau2(fjtau2_->at(iJ));
    cmsTops.back().setFJTau3(fjtau3_->at(iJ));
    cmsTops.back().setTopMass(topmass_->at(iJ));
    cmsTops.back().setWMass(wmass_->at(iJ));
    cmsTops.back().setMinMass(minmass_->at(iJ));
    cmsTops.back().setNSubJets(nsubjets_->at(iJ));

  } 
  std::sort(cmsTops.begin(),cmsTops.end(),PhysicsUtilities::greaterPT<CMSTopF>());

}
