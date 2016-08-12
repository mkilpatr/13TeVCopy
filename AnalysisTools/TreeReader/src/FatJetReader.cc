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
  fjprunedmass_    = new vector<float>;
  fjsoftdropmass_  = new vector<float>;
  //  fjcmstoptagmass_ = new vector<float>;
  fjtau1_          = new vector<float>;
  fjtau2_          = new vector<float>;
  fjtau3_          = new vector<float>;
  fjpt_            = new vector<float>;
  fjeta_           = new vector<float>;
  fjphi_           = new vector<float>;
  fjnsdsubjets_    = new vector<int  >;
  fjsdsj1mass_     = new vector<float>;
  fjsdsj1pt_       = new vector<float>;
  fjsdsj1eta_      = new vector<float>;
  fjsdsj1phi_      = new vector<float>;
  fjsdsj2mass_     = new vector<float>;
  fjsdsj2pt_       = new vector<float>;
  fjsdsj2eta_      = new vector<float>;
  fjsdsj2phi_      = new vector<float>;
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
      treeReader->setBranchAddress(branchName_, "fatjet_prunedmass"   , &fjprunedmass_   ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_softdropmass" , &fjsoftdropmass_ ,true);
      //      treeReader->setBranchAddress(branchName_, "fatjet_cmstoptagmass", &fjcmstoptagmass_,true);    
      treeReader->setBranchAddress(branchName_, "fatjet_tau1"         , &fjtau1_         ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_tau2"         , &fjtau2_         ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_tau3"         , &fjtau3_         ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_pt"           , &fjpt_           ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_eta"          , &fjeta_          ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_phi"          , &fjphi_          ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_nsoftdropsubjets" , &fjnsdsubjets_ ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_sdsubjet1_mass"   , &fjsdsj1mass_ ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_sdsubjet1_pt"     , &fjsdsj1pt_ ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_sdsubjet1_eta"    , &fjsdsj1eta_ ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_sdsubjet1_phi"    , &fjsdsj1phi_ ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_sdsubjet2_mass"   , &fjsdsj2mass_ ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_sdsubjet2_pt"     , &fjsdsj2pt_ ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_sdsubjet2_eta"    , &fjsdsj2eta_ ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_sdsubjet2_phi"    , &fjsdsj2phi_ ,true);
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
    fatJets.back().setFJPrunedMass(fjprunedmass_->at(iJ));
    fatJets.back().setFJSoftDropMass(fjsoftdropmass_->at(iJ));
    //    fatJets.back().setFJCmsTopTagMass(fjcmstoptagmass_->at(iJ));
    fatJets.back().setFJTau1(fjtau1_->at(iJ));
    fatJets.back().setFJTau2(fjtau2_->at(iJ));
    fatJets.back().setFJTau3(fjtau3_->at(iJ));
    fatJets.back().setFJNSDSubjets(fjnsdsubjets_->at(iJ));
    fatJets.back().setFJSJ1Mass(fjsdsj1mass_->at(iJ));
    fatJets.back().setFJSJ1Pt(fjsdsj1pt_->at(iJ));
    fatJets.back().setFJSJ1Eta(fjsdsj1eta_->at(iJ));
    fatJets.back().setFJSJ1Phi(fjsdsj1phi_->at(iJ));
    fatJets.back().setFJSJ2Mass(fjsdsj2mass_->at(iJ));
    fatJets.back().setFJSJ2Pt(fjsdsj2pt_->at(iJ));
    fatJets.back().setFJSJ2Eta(fjsdsj2eta_->at(iJ));
    fatJets.back().setFJSJ2Phi(fjsdsj2phi_->at(iJ));

  } 
  std::sort(fatJets.begin(),fatJets.end(),PhysicsUtilities::greaterPT<FatJetF>());

}
