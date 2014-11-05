//--------------------------------------------------------------------------------------------------
// 
// JetReader
// 
// Class for reading electron object information from TTree.
// 
// ElectronReader.cc created on Wed Oct 22 01:39:44 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisTools/TreeReader/interface/JetReader.h"
#include "AnalysisTools/TreeReader/interface/TreeReader.h"

using namespace std;
using namespace ucsbsusy;

const int JetReader::defaultOptions = JetReader::LOADRECO | JetReader::FILLOBJ;

//--------------------------------------------------------------------------------------------------
JetReader::JetReader() : BaseReader(){
  jetpt_       = new vector<float>;
  jeteta_      = new vector<float>;
  jetphi_      = new vector<float>;
  jetmass_     = new vector<float>;
  jetptraw_    = new vector<float>;
  jetpuId_     = new vector<float>;
  jetcsv_      = new vector<float>;
  jetflavor_   = new vector<int  >;
  genjetpt_    = new vector<float>;
  genjeteta_   = new vector<float>;
  genjetphi_   = new vector<float>;
  genjetmass_  = new vector<float>;
  genjetflavor_= new vector<int  >;
  jetqgl_      = new vector<float>;
}

//--------------------------------------------------------------------------------------------------
void JetReader::load(TreeReader *treeReader, int options, string branchName)
{
    const_cast<int&>(options_)    = options;
    const_cast<string&>(branchName_) = branchName;

    clog << "Loading (" << branchName << ") jets with: ";

  if(options_ & LOADRECO){
    clog << "reco ";
    treeReader->setBranchAddress((branchName_ + "_jet_pt"    ).c_str(), &jetpt_    ,true);
    treeReader->setBranchAddress((branchName_ + "_jet_eta"   ).c_str(), &jeteta_   ,true);
    treeReader->setBranchAddress((branchName_ + "_jet_phi"   ).c_str(), &jetphi_   ,true);
    treeReader->setBranchAddress((branchName_ + "_jet_mass"  ).c_str(), &jetmass_  ,true);
    treeReader->setBranchAddress((branchName_ + "_jet_ptraw" ).c_str(), &jetptraw_ );
    treeReader->setBranchAddress((branchName_ + "_jet_puId"  ).c_str(), &jetpuId_  );
    treeReader->setBranchAddress((branchName_ + "_jet_csv"   ).c_str(), &jetcsv_   ,true);
    treeReader->setBranchAddress((branchName_ + "_jet_flavor").c_str(), &jetflavor_);
  }
  if(options_ & LOADGEN){
    clog << "gen ";
    treeReader->setBranchAddress((branchName_+"_matchedgenjet_pt"    ).c_str(), &genjetpt_    ,true);
    treeReader->setBranchAddress((branchName_+"_matchedgenjet_eta"   ).c_str(), &genjeteta_   ,true);
    treeReader->setBranchAddress((branchName_+"_matchedgenjet_phi"   ).c_str(), &genjetphi_   ,true);
    treeReader->setBranchAddress((branchName_+"_matchedgenjet_mass"  ).c_str(), &genjetmass_  ,true);
    treeReader->setBranchAddress((branchName_+"_matchedgenjet_flavor").c_str(), &genjetflavor_,true);
  }
  if(options_ & LOADJETSHAPE){
    clog << "jetshape ";
    treeReader->setBranchAddress((branchName_+"_jet_qgl"    ).c_str(), &jetqgl_    );
  }
  if(options_ & FILLOBJ)
    clog << "+Objects";
  clog << endl;
}

//--------------------------------------------------------------------------------------------------
void JetReader::refresh(){
  if(!(options_ & FILLOBJ)) return;

  if(options_ & LOADGEN){
    genJets.clear();
    genJets.reserve(genjetpt_->size());
    for(unsigned int iJ = 0; iJ < genjetpt_->size(); ++iJ)
      genJets.emplace_back(CylLorentzVectorF(genjetpt_->at(iJ),genjeteta_->at(iJ),genjetphi_->at(iJ),genjetmass_->at(iJ)),
                            iJ,&(*genjetflavor_)[iJ]);
  }

  if(options_ & LOADRECO){
    recoJets.clear();
    recoJets.reserve(jetpt_->size());
    for(unsigned int iJ = 0; iJ < jetpt_->size(); ++iJ)
      recoJets.emplace_back(CylLorentzVectorF(jetpt_->at(iJ),jeteta_->at(iJ),jetphi_->at(iJ),jetmass_->at(iJ)),
                            iJ,&(*jetcsv_)[iJ], (options_ & LOADGEN) ? &genJets[iJ] : 0);
  }
}

//--------------------------------------------------------------------------------------------------
void JetReader::pushToTree(){
  if(options_ & LOADGEN)
    for(const auto& jet : genJets ){
      (*genjetpt_  )[jet.index()] = jet.pt();
      (*genjeteta_ )[jet.index()] = jet.eta();
      (*genjetphi_ )[jet.index()] = jet.phi();
      (*genjetmass_)[jet.index()] = jet.mass();
    }

  if(options_ & LOADRECO)
    for(const auto& jet : recoJets ){
      (*jetpt_  )[jet.index()] = jet.pt();
      (*jeteta_ )[jet.index()] = jet.eta();
      (*jetphi_ )[jet.index()] = jet.phi();
      (*jetmass_)[jet.index()] = jet.mass();
    }
}
