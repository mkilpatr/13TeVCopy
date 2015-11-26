//--------------------------------------------------------------------------------------------------
// 
// JetReader
// 
// Class for reading electron object information from TTree.
// 
// ElectronReader.cc created on Wed Oct 22 01:39:44 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisTools/TreeReader/interface/FatJetReader.h"
#include "AnalysisTools/TreeReader/interface/TreeReader.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"

using namespace std;
using namespace ucsbsusy;

const int FatJetReader::defaultOptions = FatJetReader::LOADRECO | FatJetReader::FILLOBJ;

//--------------------------------------------------------------------------------------------------
FatJetReader::FatJetReader() : BaseReader(){
  fjrawmass_       = new vector<float>;
  fjtrimmedmass_   = new vector<float>;
  fjprunedmass_    = new vector<float>;
  fjsoftdropmass_  = new vector<float>;
  fjfilteredmass_  = new vector<float>;
  fjcmstoptagmass_ = new vector<float>;
  fjtau1_          = new vector<float>;
  fjtau2_          = new vector<float>;
  fjtau3_          = new vector<float>;
  fjpt_            = new vector<float>;
  fjeta_           = new vector<float>;
  fjphi_           = new vector<float>;

}

//--------------------------------------------------------------------------------------------------
void FatJetReader::load(TreeReader *treeReader, int options, string branchName)
{
    loaded_ = true;
    const_cast<int&>(options_)    = options;
    const_cast<string&>(branchName_) = branchName;

    clog << "Loading (" << branchName << ") tops with: ";

    if(options_) {
      treeReader->setBranchAddress(branchName_, "fatjet_rawmass"      , &fjrawmass_      ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_trimmedmass"  , &fjtrimmedmass_  ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_prunedmass"   , &fjprunedmass_   ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_softdropmass" , &fjsoftdropmass_ ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_filteredmass" , &fjfilteredmass_ ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_cmstoptagmass", &fjcmstoptagmass_,true);    
      treeReader->setBranchAddress(branchName_, "fatjet_tau1"         , &fjtau1_         ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_tau2"         , &fjtau2_         ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_tau3"         , &fjtau3_         ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_pt"           , &fjpt_           ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_eta"          , &fjeta_          ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_phi"          , &fjphi_          ,true);
    }


    if(options_ & FILLOBJ)
      clog << "+Objects";
    clog << endl;
}

//--------------------------------------------------------------------------------------------------
void FatJetReader::refresh(){

  if(!(options_ & FILLOBJ)) return;

  fatJets.clear();
  fatJets.reserve(fjpt_->size()); 
  for(unsigned int iJ = 0; iJ < fjpt_->size(); ++iJ){

    fatJets.emplace_back(CylLorentzVectorF(fjpt_->at(iJ),fjeta_->at(iJ),fjphi_->at(iJ),fjrawmass_->at(iJ)),iJ);
    fatJets.back().setFJRawMass(fjrawmass_->at(iJ));
    fatJets.back().setFJTrimmedMass(fjtrimmedmass_->at(iJ));
    fatJets.back().setFJPrunedMass(fjprunedmass_->at(iJ));
    fatJets.back().setFJSoftDropMass(fjsoftdropmass_->at(iJ));
    fatJets.back().setFJFilteredMass(fjfilteredmass_->at(iJ));
    fatJets.back().setFJCmsTopTagMass(fjcmstoptagmass_->at(iJ));
    fatJets.back().setFJTau1(fjtau1_->at(iJ));
    fatJets.back().setFJTau2(fjtau2_->at(iJ));
    fatJets.back().setFJTau3(fjtau3_->at(iJ));

  } 
  std::sort(fatJets.begin(),fatJets.end(),PhysicsUtilities::greaterPT<FatJetF>());

}
