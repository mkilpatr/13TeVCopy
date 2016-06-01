//--------------------------------------------------------------------------------------------------
// 
// JetReader
// 
// Class for reading electron object information from TTree.
// 
// ElectronReader.cc created on Wed Oct 22 01:39:44 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisTools/TreeReader/interface/Ak8JetReader.h"
#include "AnalysisTools/TreeReader/interface/TreeReader.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"

using namespace std;
using namespace ucsbsusy;

const int Ak8JetReader::defaultOptions = Ak8JetReader::LOADRECO | Ak8JetReader::FILLOBJ;

//--------------------------------------------------------------------------------------------------
Ak8JetReader::Ak8JetReader() : BaseReader(){
  ak8rawmass_       = new vector<float>;
  ak8prunedmass_    = new vector<float>;
  ak8softdropmass_  = new vector<float>;
  //  ak8cmstoptagmass_ = new vector<float>;
  ak8tau1_          = new vector<float>;
  ak8tau2_          = new vector<float>;
  ak8tau3_          = new vector<float>;
  ak8pt_            = new vector<float>;
  ak8eta_           = new vector<float>;
  ak8phi_           = new vector<float>;

}

//--------------------------------------------------------------------------------------------------
void Ak8JetReader::load(TreeReader *treeReader, int options, string branchName)
{
    loaded_ = true;
    const_cast<int&>(options_)    = options;
    const_cast<string&>(branchName_) = branchName;

    clog << "Loading (" << branchName << ") ak8jets with: ";

    treeReader->setBranchAddress(branchName_, "fatjet_rawmass"      , &ak8rawmass_      ,true);
    treeReader->setBranchAddress(branchName_, "fatjet_softdropmass" , &ak8softdropmass_ ,true);
    //    treeReader->setBranchAddress(branchName_, "fatjet_cmstoptagmass", &ak8cmstoptagmass_,true);    
    treeReader->setBranchAddress(branchName_, "fatjet_tau1"         , &ak8tau1_         ,true);
    treeReader->setBranchAddress(branchName_, "fatjet_tau2"         , &ak8tau2_         ,true);
    treeReader->setBranchAddress(branchName_, "fatjet_tau3"         , &ak8tau3_         ,true);
    treeReader->setBranchAddress(branchName_, "fatjet_pt"           , &ak8pt_           ,true);
    treeReader->setBranchAddress(branchName_, "fatjet_eta"          , &ak8eta_          ,true);
    treeReader->setBranchAddress(branchName_, "fatjet_phi"          , &ak8phi_          ,true);

    if(options_ & FILLOBJ)
      clog << "+Objects";
    clog << endl;
}

//--------------------------------------------------------------------------------------------------
void Ak8JetReader::refresh(){

  if(!(options_ & FILLOBJ)) return;

  ak8Jets.clear();
  ak8Jets.reserve(ak8pt_->size()); 
  for(unsigned int iJ = 0; iJ < ak8pt_->size(); ++iJ){
    ak8Jets.emplace_back(CylLorentzVectorF(ak8pt_->at(iJ),ak8eta_->at(iJ),ak8phi_->at(iJ),ak8rawmass_->at(iJ)),iJ);
    ak8Jets.back().setFJRawMass(ak8rawmass_->at(iJ));
    ak8Jets.back().setFJPrunedMass(ak8prunedmass_->at(iJ));
    ak8Jets.back().setFJSoftDropMass(ak8softdropmass_->at(iJ));
    //    ak8Jets.back().setFJCmsTopTagMass(ak8cmstoptagmass_->at(iJ));
    ak8Jets.back().setFJTau1(ak8tau1_->at(iJ));
    ak8Jets.back().setFJTau2(ak8tau2_->at(iJ));
    ak8Jets.back().setFJTau3(ak8tau3_->at(iJ));

  } 
  std::sort(ak8Jets.begin(),ak8Jets.end(),PhysicsUtilities::greaterPT<FatJetF>());

}
