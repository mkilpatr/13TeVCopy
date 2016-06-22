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
  topallsubjetmass_ = new vector<float>;
  topcmstoptagmass_ = new vector<float>;
  toppt_            = new vector<float>;
  topeta_           = new vector<float>;
  topphi_           = new vector<float>;
  topminmass_       = new vector<float>;
  topnsubjets_      = new vector<float>  ;

}

//--------------------------------------------------------------------------------------------------
void CMSTopReader::load(TreeReader *treeReader, int options, string branchName)
{
    loaded_ = true;
    const_cast<int&>(options_)    = options;
    const_cast<string&>(branchName_) = branchName;

    clog << "Loading (" << branchName << ") tops with: ";

    treeReader->setBranchAddress(branchName_, "top_allsubjetmass", &topallsubjetmass_,true);
    treeReader->setBranchAddress(branchName_, "top_cmstoptagmass", &topcmstoptagmass_,true);
    treeReader->setBranchAddress(branchName_, "top_pt"           , &toppt_           ,true);
    treeReader->setBranchAddress(branchName_, "top_eta"          , &topeta_          ,true);
    treeReader->setBranchAddress(branchName_, "top_phi"          , &topphi_          ,true);
    treeReader->setBranchAddress(branchName_, "top_minmass"      , &topminmass_      ,true);
    treeReader->setBranchAddress(branchName_, "top_nsubjets"     , &topnsubjets_     ,true);

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
    cmsTops.back().setTopAllSubjetMass(topallsubjetmass_->at(iJ));
    cmsTops.back().setTopCmsTopTagMass(topcmstoptagmass_->at(iJ));
    cmsTops.back().setTopMinMass(topminmass_->at(iJ));
    cmsTops.back().setTopNSubJets(topnsubjets_->at(iJ));

  }
  std::sort(cmsTops.begin(),cmsTops.end(),PhysicsUtilities::greaterPT<CMSTopF>());

}
