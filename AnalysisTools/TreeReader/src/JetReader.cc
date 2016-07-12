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
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"

using namespace std;
using namespace ucsbsusy;

//const int JetReader::defaultOptions = JetReader::LOADRECO | JetReader::FILLOBJ;
const int JetReader::defaultOptions = JetReader::LOADRECO | JetReader::FILLOBJ | JetReader::LOADJETEXTRA;

//--------------------------------------------------------------------------------------------------
JetReader::JetReader() : BaseReader(){
  jetpt_            = new vector<float>;
  jeteta_           = new vector<float>;
  jetphi_           = new vector<float>;
  jetmass_          = new vector<float>;
  jetptraw_         = new vector<float>;
  jetpuId_          = new vector<float>;
  jetlooseId_       = new vector<bool >;
  jettightId_       = new vector<bool >;
  jetcsv_           = new vector<float>;
  jetarea_          = new vector<float>;
  jetgenindex_      = new vector<int16  >;
  jetuncertainty_   = new vector<float>;
  genjetpt_         = new vector<float>;
  genjeteta_        = new vector<float>;
  genjetphi_        = new vector<float>;
  genjetmass_       = new vector<float>;
  genjetflavor_     = new vector<size8>;
  jetbetaStar_      = new vector<float>;
  jetqgl_           = new vector<float>;
  jetptD_           = new vector<float>;
  jetaxis1_         = new vector<float>;
  jetaxis2_         = new vector<float>;
  jetMult_          = new vector<size16  >;
  jetcharge_        = new vector<float>;
  jetpullrap_       = new vector<float>;
  jetpullphi_       = new vector<float>;
  genjetptD_        = new vector<float>;
  genjetaxis1_      = new vector<float>;
  genjetaxis2_      = new vector<float>;
  genjetMult_       = new vector<size16  >;
  genjetcharge_     = new vector<float>;
  genjetpullrap_    = new vector<float>;
  genjetpullphi_    = new vector<float>;
  genAssocPrtIndex_ = new std::vector<size16>;
  genAssocJetIndex_ = new std::vector<size16>;
  genAssocCont_     = new std::vector<int8>  ;
  jetchHadEnFrac_   = new std::vector<float>;
  jetchEmEnFrac_    = new std::vector<float>;
  jetmuEnFrac_      = new std::vector<float>;
  jetelEnFrac_      = new std::vector<float>;
  jetneuHadEnFrac_  = new std::vector<float>;
  jetneuEmEnFrac_   = new std::vector<float>;
  jetphoEnFrac_     = new std::vector<float>;
  jetchMult_        = new std::vector<int>  ;
  jetchHadN2_       = new std::vector<int>  ;
  jetchHadN4_       = new std::vector<int>  ;
  jetchHadN6_       = new std::vector<int>  ;
  jetneuMult_       = new std::vector<int>  ;
  jetchHadMult_     = new std::vector<int>  ;
  jetelMult_        = new std::vector<int>  ;
  jetmuMult_        = new std::vector<int>  ;
  jetneuHadMult_    = new std::vector<int>  ;
  jetphoMult_       = new std::vector<int>  ;

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
    treeReader->setBranchAddress(branchName_, "jet_looseId" , &jetlooseId_  );
    treeReader->setBranchAddress(branchName_, "jet_tightId" , &jettightId_  );
    treeReader->setBranchAddress(branchName_, "jet_csv"     , &jetcsv_   ,true);
    treeReader->setBranchAddress(branchName_, "jet_area"    , &jetarea_  ,false);
    treeReader->setBranchAddress(branchName_, "jet_uncertainty", &jetuncertainty_);
    treeReader->setBranchAddress(branchName_, "jet_chHadEnFrac", &jetchHadEnFrac_  );
    treeReader->setBranchAddress(branchName_, "jet_chHadN2", &jetchHadN2_ );
    treeReader->setBranchAddress(branchName_, "jet_chHadN4", &jetchHadN4_ );
    treeReader->setBranchAddress(branchName_, "jet_chHadN6", &jetchHadN6_ );
  }
  if(options_ & LOADGEN){
    clog << "gen ";
    treeReader->setBranchAddress(branchName_,"jet_genIndex",  &jetgenindex_ ,true);
    treeReader->setBranchAddress(branchName_,"genjet_pt"    , &genjetpt_    ,true);
    treeReader->setBranchAddress(branchName_,"genjet_eta"   , &genjeteta_   ,true);
    treeReader->setBranchAddress(branchName_,"genjet_phi"   , &genjetphi_   ,true);
    treeReader->setBranchAddress(branchName_,"genjet_mass"  , &genjetmass_  ,true);
    treeReader->setBranchAddress(branchName_,"genjet_flavor", &genjetflavor_,true);

  }
  if(options_ & LOADJETSHAPE){
    clog << "jetshape ";
    treeReader->setBranchAddress(branchName_,"jet_betaStar", &jetbetaStar_ );
    treeReader->setBranchAddress(branchName_,"jet_qgl"     , &jetqgl_      );
    treeReader->setBranchAddress(branchName_,"jet_ptD"     , &jetptD_      );
    treeReader->setBranchAddress(branchName_,"jet_axis1"   , &jetaxis1_    );
    treeReader->setBranchAddress(branchName_,"jet_axis2"   , &jetaxis2_    );
    treeReader->setBranchAddress(branchName_,"jet_jetMult" , &jetMult_     );
    treeReader->setBranchAddress(branchName_,"jet_charge" ,  &jetcharge_     );
    treeReader->setBranchAddress(branchName_,"jet_pullrap" , &jetpullrap_     );
    treeReader->setBranchAddress(branchName_,"jet_pullphi" , &jetpullphi_    );
    if(options_ & LOADGEN){
      treeReader->setBranchAddress(branchName_,"genjet_ptD"     , &genjetptD_   );
      treeReader->setBranchAddress(branchName_,"genjet_axis1"   , &genjetaxis1_ );
      treeReader->setBranchAddress(branchName_,"genjet_axis2"   , &genjetaxis2_ );
      treeReader->setBranchAddress(branchName_,"genjet_jetMult" , &genjetMult_  );
      treeReader->setBranchAddress(branchName_,"genjet_charge"  , &genjetcharge_     );
      treeReader->setBranchAddress(branchName_,"genjet_pullrap" , &genjetpullrap_     );
      treeReader->setBranchAddress(branchName_,"genjet_pullphi" , &genjetpullphi_    );
    }
  }
  if(options_ & LOADTOPASSOC){
    clog <<"topassoc ";
    treeReader->setBranchAddress(branchName_,"prtassoc_partonIndex" , &genAssocPrtIndex_);
    treeReader->setBranchAddress(branchName_,"prtassoc_jetIndex"    , &genAssocJetIndex_);
    treeReader->setBranchAddress(branchName_,"prtassoc_jetCont"     , &genAssocCont_    );
  }
  if(options_ & LOADJETEXTRA){
    clog <<"jetextra ";
    treeReader->setBranchAddress(branchName_,"jet_chEmEnFrac"   ,&jetchEmEnFrac_   );
    treeReader->setBranchAddress(branchName_,"jet_muEnFrac"     ,&jetmuEnFrac_     );
    treeReader->setBranchAddress(branchName_,"jet_elEnFrac"     ,&jetelEnFrac_     );
    treeReader->setBranchAddress(branchName_,"jet_neuHadEnFrac" ,&jetneuHadEnFrac_ );
    treeReader->setBranchAddress(branchName_,"jet_neuEmEnFrac"  ,&jetneuEmEnFrac_  );
    treeReader->setBranchAddress(branchName_,"jet_phoEnFrac"    ,&jetphoEnFrac_    );
    treeReader->setBranchAddress(branchName_,"jet_chMult"       ,&jetchMult_       );
    treeReader->setBranchAddress(branchName_,"jet_neuMult"      ,&jetneuMult_      );
    treeReader->setBranchAddress(branchName_,"jet_chHadMult"    ,&jetchHadMult_    );
    treeReader->setBranchAddress(branchName_,"jet_elMult"       ,&jetelMult_       );
    treeReader->setBranchAddress(branchName_,"jet_muMult"       ,&jetmuMult_       );
    treeReader->setBranchAddress(branchName_,"jet_neuHadMult"   ,&jetneuHadMult_   );
    treeReader->setBranchAddress(branchName_,"jet_phoMult"      ,&jetphoMult_      );
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
      genJets.emplace_back(CylLorentzVectorF(genjetpt_->at(iJ),genjeteta_->at(iJ),genjetphi_->at(iJ),genjetmass_->at(iJ)),iJ,genjetflavor_->at(iJ));
  }

  if(options_ & LOADRECO){
    recoJets.clear();
    recoJets.reserve(jetpt_->size());
    for(unsigned int iJ = 0; iJ < jetpt_->size(); ++iJ){
      addRecoJetToObjectList(iJ);
    }
    std::sort(recoJets.begin(),recoJets.end(),PhysicsUtilities::greaterPT<RecoJetF>());
  }
  if(options_ & LOADGEN)
    std::sort(genJets.begin(),genJets.end(),PhysicsUtilities::greaterPT<GenJetF>());

}
//--------------------------------------------------------------------------------------------------
void JetReader::addRecoJetToObjectList(const int iJ){

  GenJetF * matchedGen = (options_ & LOADGEN) ? (jetgenindex_->at(iJ) >= 0 ? &genJets[jetgenindex_->at(iJ)] : 0) : 0;
   recoJets.emplace_back(CylLorentzVectorF(jetpt_->at(iJ), jeteta_->at(iJ), jetphi_->at(iJ), jetmass_->at(iJ)), iJ,
                               (*jetcsv_)[iJ], jetptraw_->at(iJ), (jetuncertainty_->size()) ? (jetuncertainty_->at(iJ)) : 0,
                               (*jetlooseId_)[iJ],  matchedGen);
  recoJets.back().setChHadFrac((jetchHadEnFrac_->size()) ? (jetchHadEnFrac_->at(iJ)) : 2);
  recoJets.back().setChHadN2((jetchHadN2_->size()) ? (jetchHadN2_->at(iJ)) : -1);
  recoJets.back().setChHadN4((jetchHadN4_->size()) ? (jetchHadN4_->at(iJ)) : -1);
  recoJets.back().setChHadN6((jetchHadN6_->size()) ? (jetchHadN6_->at(iJ)) : -1);

}
//--------------------------------------------------------------------------------------------------
void JetReader::addRecoJet(const RecoJetF * inJet){
  //first get the index...we will add it the end of the list
  int index = jetpt_->size();
  //THen add all to all of the vectors
  jetpt_         ->push_back(inJet->pt());
  jeteta_        ->push_back(inJet->eta());
  jetphi_        ->push_back(inJet->phi());
  jetmass_       ->push_back(inJet->mass());
  jetptraw_      ->push_back(inJet->pt_raw());
  jetpuId_       ->push_back(1);
  jetlooseId_    ->push_back(inJet->looseid());
  jettightId_    ->push_back(1);
  jetchHadEnFrac_  ->push_back(inJet->chHadFrac());
  jetchHadN2_     ->push_back(inJet->chHadN2());
  jetchHadN4_     ->push_back(inJet->chHadN4());
  jetchHadN6_     ->push_back(inJet->chHadN6());
  jetcsv_        ->push_back(inJet->csv());
  jetarea_       ->push_back(0);
  jetgenindex_   ->push_back(inJet->genJet() ? inJet->genJet()->index(): -1 );
  jetuncertainty_->push_back(inJet->uncertainty());
  jetbetaStar_   ->push_back(0);
  jetqgl_        ->push_back(-1);
  jetptD_        ->push_back(0);
  jetaxis1_      ->push_back(0);
  jetaxis2_      ->push_back(0);
  jetMult_       ->push_back(0);
  jetcharge_     ->push_back(0);
  jetpullrap_    ->push_back(0);
  jetpullphi_    ->push_back(0);

  addRecoJetToObjectList(index);

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
