//--------------------------------------------------------------------------------------------------
// 
// JetReader
// 
// Class for reading electron object information from TTree.
// 
// ElectronReader.cc created on Wed Oct 22 01:39:44 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisTools/TreeReader/interface/GenParticleReader.h"
#include "AnalysisTools/TreeReader/interface/TreeReader.h"

using namespace std;
using namespace ucsbsusy;

const int GenParticleReader::defaultOptions = GenParticleReader::FILLOBJ;

//--------------------------------------------------------------------------------------------------
GenParticleReader::GenParticleReader() : BaseReader(){
  pt_       = new vector<float>(0);
  eta_      = new vector<float>(0);
  phi_      = new vector<float>(0);
  mass_     = new vector<float>(0);
  status_   = new vector<size8>(0);
  pdgId_    = new vector<int  >(0);
  nMoms_    = new vector<stor >(0);
  firstMom_ = new vector<stor >(0);
  nDaus_    = new vector<stor >(0);
  firstDau_ = new vector<stor >(0);
  assocList_= new vector<stor >(0);
}
//--------------------------------------------------------------------------------------------------
GenParticleReader::~GenParticleReader(){
  delete pt_       ;
  delete eta_      ;
  delete phi_      ;
  delete mass_     ;
  delete status_   ;
  delete pdgId_    ;
  delete nMoms_    ;
  delete firstMom_ ;
  delete nDaus_    ;
  delete firstDau_ ;
  delete assocList_;
}

//--------------------------------------------------------------------------------------------------
void GenParticleReader::load(TreeReader *treeReader, int options, string branchName)
{
    loaded_ = true;
    const_cast<int&>(options_)    = options;
    const_cast<string&>(branchName_) = branchName;

    clog << "Loading (" << branchName << ") particles with: ";

    treeReader->setBranchAddress(branchName_,"pt"       ,&pt_       ,true);
    treeReader->setBranchAddress(branchName_,"eta"      ,&eta_      ,true);
    treeReader->setBranchAddress(branchName_,"phi"      ,&phi_      ,true);
    treeReader->setBranchAddress(branchName_,"mass"     ,&mass_     ,true);
    treeReader->setBranchAddress(branchName_,"status"   ,&status_   ,true);
    treeReader->setBranchAddress(branchName_,"pdgId"    ,&pdgId_    ,true);
    treeReader->setBranchAddress(branchName_,"nMoms"    ,&nMoms_    ,true);
    treeReader->setBranchAddress(branchName_,"firstMom" ,&firstMom_ ,true);
    treeReader->setBranchAddress(branchName_,"nDaus"    ,&nDaus_    ,true);
    treeReader->setBranchAddress(branchName_,"firstDau" ,&firstDau_ ,true);
    treeReader->setBranchAddress(branchName_,"assocList",&assocList_,true);

  if(options_ & FILLOBJ)
    clog << "+Objects";
  clog << endl;
}

//--------------------------------------------------------------------------------------------------
void GenParticleReader::refresh(){
  if(!(options_ & FILLOBJ)) return;

  genParticles.clear();
  genParticles.reserve(pt_->size());

  for(unsigned int iP = 0; iP < pt_->size(); ++iP){
    genParticles.emplace_back(CylLorentzVectorF(pt_->at(iP),eta_->at(iP),phi_->at(iP),mass_->at(iP)),&genParticles);
    genParticles.back().setStorage(status_->at(iP),pdgId_->at(iP),nMoms_->at(iP),firstMom_->at(iP),nDaus_->at(iP),firstDau_->at(iP),assocList_);
  }
}
