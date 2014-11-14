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
  jetgenindex_ = new vector<int  >;
  genjetpt_    = new vector<float>;
  genjeteta_   = new vector<float>;
  genjetphi_   = new vector<float>;
  genjetmass_  = new vector<float>;
  jetbetaStar_= new vector<float>;
  jetqgl_     = new vector<float>;
  jetptD_     = new vector<float>;
  jetaxis1_   = new vector<float>;
  jetaxis2_   = new vector<float>;
  jetMult_    = new vector<int  >;
  genjetptD_  = new vector<float>;
  genjetaxis1_= new vector<float>;
  genjetaxis2_= new vector<float>;
  genjetMult_ = new vector<int  >;
}

//--------------------------------------------------------------------------------------------------
void JetReader::load(TreeReader *treeReader, int options, string branchName)
{
    loaded_ = true;
    const_cast<int&>(options_)    = options;
    const_cast<string&>(branchName_) = branchName;

    clog << "Loading (" << branchName << ") jets with: ";

  if(options_ & LOADRECO){
    clog << "reco ";
    treeReader->setBranchAddress(branchName_, "jet_pt"      , &jetpt_    ,true);
    treeReader->setBranchAddress(branchName_, "jet_eta"     , &jeteta_   ,true);
    treeReader->setBranchAddress(branchName_, "jet_phi"     , &jetphi_   ,true);
    treeReader->setBranchAddress(branchName_, "jet_mass"    , &jetmass_  ,true);
    treeReader->setBranchAddress(branchName_, "jet_ptraw"   , &jetptraw_ );
    treeReader->setBranchAddress(branchName_, "jet_puId"    , &jetpuId_  );
    treeReader->setBranchAddress(branchName_, "jet_csv"     , &jetcsv_   ,true);
  }
  if(options_ & LOADGEN){
    clog << "gen ";
    treeReader->setBranchAddress(branchName_, "jet_flavor"  , &jetflavor_   ,true);
    treeReader->setBranchAddress(branchName_, "jet_genIndex", &jetgenindex_ ,true);
    treeReader->setBranchAddress(branchName_,"genjet_pt"    , &genjetpt_    ,true);
    treeReader->setBranchAddress(branchName_,"genjet_eta"   , &genjeteta_   ,true);
    treeReader->setBranchAddress(branchName_,"genjet_phi"   , &genjetphi_   ,true);
    treeReader->setBranchAddress(branchName_,"genjet_mass"  , &genjetmass_  ,true);
  }
  if(options_ & LOADJETSHAPE){
    clog << "jetshape ";
    treeReader->setBranchAddress(branchName_,"jet_betaStar", &jetbetaStar_ );
    treeReader->setBranchAddress(branchName_,"jet_qgl"     , &jetqgl_      );
    treeReader->setBranchAddress(branchName_,"jet_ptD"     , &jetptD_      );
    treeReader->setBranchAddress(branchName_,"jet_axis1"   , &jetaxis1_    );
    treeReader->setBranchAddress(branchName_,"jet_axis2"   , &jetaxis2_    );
    treeReader->setBranchAddress(branchName_,"jet_jetMult" , &jetMult_     );
    if(options_ & LOADGEN){
      treeReader->setBranchAddress(branchName_,"genjet_ptD"     , &genjetptD_   );
      treeReader->setBranchAddress(branchName_,"genjet_axis1"   , &genjetaxis1_ );
      treeReader->setBranchAddress(branchName_,"genjet_axis2"   , &genjetaxis2_ );
      treeReader->setBranchAddress(branchName_,"genjet_jetMult" , &genjetMult_  );
    }
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
      genJets.emplace_back(CylLorentzVectorF(genjetpt_->at(iJ),genjeteta_->at(iJ),genjetphi_->at(iJ),genjetmass_->at(iJ)),iJ);
  }

  if(options_ & LOADRECO){
    recoJets.clear();
    recoJets.reserve(jetpt_->size());
    for(unsigned int iJ = 0; iJ < jetpt_->size(); ++iJ){
     GenJetF * matchedGen = (options_ & LOADGEN) ? (jetgenindex_->at(iJ) >= 0 ? &genJets[jetgenindex_->at(iJ)] : 0) : 0;
      recoJets.emplace_back(CylLorentzVectorF(jetpt_->at(iJ),jeteta_->at(iJ),jetphi_->at(iJ),jetmass_->at(iJ)),iJ,
          &(*jetcsv_)[iJ], (options_ & LOADGEN) ? &jetflavor_->at(iJ) : 0, matchedGen);
    }
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
