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
  toprawmass_       = new vector<float>;
  toptrimmedmass_   = new vector<float>;
  topprunedmass_    = new vector<float>;
  topsoftdropmass_  = new vector<float>;
  topfilteredmass_  = new vector<float>;
  topcmstoptagmass_ = new vector<float>;
  toptau1_          = new vector<float>;
  toptau2_          = new vector<float>;
  toptau3_          = new vector<float>;
  toppt_            = new vector<float>;
  topeta_           = new vector<float>;
  topphi_           = new vector<float>;
  topwmass_         = new vector<float>;
  topminmass_       = new vector<float>;
  topnsubjets_      = new vector<int>  ;

}

//--------------------------------------------------------------------------------------------------
void CMSTopReader::load(TreeReader *treeReader, int options, string branchName)
{
    loaded_ = true;
    const_cast<int&>(options_)    = options;
    const_cast<string&>(branchName_) = branchName;

    clog << "Loading (" << branchName << ") tops with: ";

    if(options_ & LOADPHYS14) {
      treeReader->setBranchAddress(branchName_, "fatjet_mass"      , &toprawmass_      ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_trimmedmass"  , &toptrimmedmass_  ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_prunedmass"   , &topprunedmass_   ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_softdropmass" , &topsoftdropmass_ ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_massdropfilteredmass" , &topfilteredmass_ ,true);
      treeReader->setBranchAddress(branchName_, "top_topmass"         , &topcmstoptagmass_,true);    
      treeReader->setBranchAddress(branchName_, "fatjet_tau1"         , &toptau1_         ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_tau2"         , &toptau2_         ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_tau3"         , &toptau3_         ,true);
      treeReader->setBranchAddress(branchName_, "top_pt"           , &toppt_           ,true);
      treeReader->setBranchAddress(branchName_, "top_eta"          , &topeta_          ,true);
      treeReader->setBranchAddress(branchName_, "top_phi"          , &topphi_          ,true);
      treeReader->setBranchAddress(branchName_, "top_wmass"        , &topwmass_        ,true);
      treeReader->setBranchAddress(branchName_, "top_minmass"      , &topminmass_      ,true);
      treeReader->setBranchAddress(branchName_, "top_nsubjets"     , &topnsubjets_     ,true);
    }
    else {
      treeReader->setBranchAddress(branchName_, "top_rawmass"      , &toprawmass_      ,true);
      treeReader->setBranchAddress(branchName_, "top_trimmedmass"  , &toptrimmedmass_  ,true);
      treeReader->setBranchAddress(branchName_, "top_prunedmass"   , &topprunedmass_   ,true);
      treeReader->setBranchAddress(branchName_, "top_softdropmass" , &topsoftdropmass_ ,true);
      treeReader->setBranchAddress(branchName_, "top_filteredmass" , &topfilteredmass_ ,true);
      treeReader->setBranchAddress(branchName_, "top_cmstoptagmass", &topcmstoptagmass_,true);    
      treeReader->setBranchAddress(branchName_, "top_tau1"         , &toptau1_         ,true);
      treeReader->setBranchAddress(branchName_, "top_tau2"         , &toptau2_         ,true);
      treeReader->setBranchAddress(branchName_, "top_tau3"         , &toptau3_         ,true);
      treeReader->setBranchAddress(branchName_, "top_pt"           , &toppt_           ,true);
      treeReader->setBranchAddress(branchName_, "top_eta"          , &topeta_          ,true);
      treeReader->setBranchAddress(branchName_, "top_phi"          , &topphi_          ,true);
      treeReader->setBranchAddress(branchName_, "top_wmass"        , &topwmass_        ,true);
      treeReader->setBranchAddress(branchName_, "top_minmass"      , &topminmass_      ,true);
      treeReader->setBranchAddress(branchName_, "top_nsubjets"     , &topnsubjets_     ,true);
    }


    if(options_ & FILLOBJ)
      clog << "+Objects";
    clog << endl;
}

//--------------------------------------------------------------------------------------------------
void CMSTopReader::refresh(){

  if(!(options_ & FILLOBJ)) return;

  cmsTops.clear();
  cmsTops.reserve(toppt_->size()); 
  for(unsigned int iJ = 0; iJ < toppt_->size(); ++iJ){

    cmsTops.emplace_back(CylLorentzVectorF(toppt_->at(iJ),topeta_->at(iJ),topphi_->at(iJ),topcmstoptagmass_->at(iJ)),iJ);
    cmsTops.back().setTopRawMass(toprawmass_->at(iJ));
    cmsTops.back().setTopTrimmedMass(toptrimmedmass_->at(iJ));
    cmsTops.back().setTopPrunedMass(topprunedmass_->at(iJ));
    cmsTops.back().setTopSoftDropMass(topsoftdropmass_->at(iJ));
    cmsTops.back().setTopFilteredMass(topfilteredmass_->at(iJ));
    cmsTops.back().setTopCmsTopTagMass(topcmstoptagmass_->at(iJ));
    cmsTops.back().setTopTau1(toptau1_->at(iJ));
    cmsTops.back().setTopTau2(toptau2_->at(iJ));
    cmsTops.back().setTopTau3(toptau3_->at(iJ));
    cmsTops.back().setTopWMass(topwmass_->at(iJ));
    cmsTops.back().setTopMinMass(topminmass_->at(iJ));
    cmsTops.back().setTopNSubJets(topnsubjets_->at(iJ));

  } 
  std::sort(cmsTops.begin(),cmsTops.end(),PhysicsUtilities::greaterPT<CMSTopF>());

}
