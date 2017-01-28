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

const int JetReader::defaultOptions = JetReader::LOADRECO | JetReader::FILLOBJ | JetReader::LOADJETSHAPE | JetReader::LOADDEEPFLAVOR;

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
  jetcmva_          = new vector<float>;
  jetcvsl_          = new vector<float>;
  jetcvsb_          = new vector<float>;
  jetarea_          = new vector<float>;
  jetgenindex_      = new vector<int16  >;
  jetuncertainty_   = new vector<float>;
  jetisrmatch_      = new vector<bool>;
  jetdeepcsv_probudsg_  = new vector<float>;
  jetdeepcsv_probb_     = new vector<float>;
  jetdeepcsv_probc_     = new vector<float>;
  jetdeepcsv_probbb_    = new vector<float>;
  jetdeepcsv_probcc_    = new vector<float>;
  jetdeepcmva_probudsg_ = new vector<float>;
  jetdeepcmva_probb_    = new vector<float>;
  jetdeepcmva_probc_    = new vector<float>;
  jetdeepcmva_probbb_   = new vector<float>;
  jetdeepcmva_probcc_   = new vector<float>;
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
    treeReader->setBranchAddress(branchName_, "jet_looseId" , &jetlooseId_, true);
    treeReader->setBranchAddress(branchName_, "jet_tightId" , &jettightId_, true);
    treeReader->setBranchAddress(branchName_, "jet_csv"     , &jetcsv_   ,true);
    treeReader->setBranchAddress(branchName_, "jet_cmva"    , &jetcmva_  ,true);
    treeReader->setBranchAddress(branchName_, "jet_cvsl"    , &jetcvsl_  ,true);
    treeReader->setBranchAddress(branchName_, "jet_cvsb"    , &jetcvsb_  ,true);
    treeReader->setBranchAddress(branchName_, "jet_area"    , &jetarea_  ,false);
    treeReader->setBranchAddress(branchName_, "jet_uncertainty", &jetuncertainty_, true);
    treeReader->setBranchAddress(branchName_, "jet_chHadEnFrac", &jetchHadEnFrac_, true);
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

    treeReader->setBranchAddress(branchName_,"jet_isrMatch",  &jetisrmatch_ ,false);// FIXME
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
  if(options_ & LOADDEEPFLAVOR){
    treeReader->setBranchAddress(branchName_, "jet_deepcsv_probudsg" , &jetdeepcsv_probudsg_ ,true);
    treeReader->setBranchAddress(branchName_, "jet_deepcsv_probb"    , &jetdeepcsv_probb_    ,true);
    treeReader->setBranchAddress(branchName_, "jet_deepcsv_probc"    , &jetdeepcsv_probc_    ,true);
    treeReader->setBranchAddress(branchName_, "jet_deepcsv_probbb"   , &jetdeepcsv_probbb_   ,true);
    treeReader->setBranchAddress(branchName_, "jet_deepcsv_probcc"   , &jetdeepcsv_probcc_   ,true);

    treeReader->setBranchAddress(branchName_, "jet_deepcmva_probudsg", &jetdeepcmva_probudsg_,true);
    treeReader->setBranchAddress(branchName_, "jet_deepcmva_probb"   , &jetdeepcmva_probb_   ,true);
    treeReader->setBranchAddress(branchName_, "jet_deepcmva_probc"   , &jetdeepcmva_probc_   ,true);
    treeReader->setBranchAddress(branchName_, "jet_deepcmva_probbb"  , &jetdeepcmva_probbb_  ,true);
    treeReader->setBranchAddress(branchName_, "jet_deepcmva_probcc"  , &jetdeepcmva_probcc_  ,true);
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
void JetReader::addRecoJetToObjectList(const unsigned int iJ){
  // --- IMPORTANT ---
  // Any variables used here should be added to JetReader::addRecoJet() too!
  // -----------------

  GenJetF * matchedGen = (options_ & LOADGEN) ? (jetgenindex_->at(iJ) >= 0 ? &genJets[jetgenindex_->at(iJ)] : 0) : 0;
  recoJets.emplace_back(CylLorentzVectorF(jetpt_->at(iJ), jeteta_->at(iJ), jetphi_->at(iJ), jetmass_->at(iJ)), iJ,
                               (*jetcsv_)[iJ], jetptraw_->at(iJ), (jetuncertainty_->size()) ? (jetuncertainty_->at(iJ)) : 0,
                               (*jetlooseId_)[iJ],  matchedGen);
  recoJets.back().setTightId(jettightId_->at(iJ));
  recoJets.back().setCmva(jetcmva_->size() ? jetcmva_->at(iJ) : -9); //FIXME
  recoJets.back().setCvsl(jetcvsl_->at(iJ));
  recoJets.back().setCvsb(jetcvsb_->at(iJ));
  recoJets.back().setChHadFrac((jetchHadEnFrac_->size()) ? (jetchHadEnFrac_->at(iJ)) : 2);
  recoJets.back().setBetaStar(jetbetaStar_->size() ? jetbetaStar_->at(iJ) : -1);
  recoJets.back().setPtD(jetptD_->size() ? jetptD_->at(iJ) : -1);
  recoJets.back().setAxis1(jetaxis1_->size() ? jetaxis1_->at(iJ) : -1);
  recoJets.back().setAxis2(jetaxis2_->size() ? jetaxis2_->at(iJ) : -1);
  recoJets.back().setJetMult(jetMult_->size() ? jetMult_->at(iJ) : -1);
  recoJets.back().setJetcharge(jetcharge_->size() ? jetcharge_->at(iJ) : -1);
  recoJets.back().setQgl(jetqgl_->size() ? jetqgl_->at(iJ) : -1);
  if (options_ & LOADDEEPFLAVOR){
    recoJets.back().setDeepCSV("probudsg",  jetdeepcsv_probudsg_->at(iJ));
    recoJets.back().setDeepCSV("probb",     jetdeepcsv_probb_->at(iJ));
    recoJets.back().setDeepCSV("probc",     jetdeepcsv_probc_->at(iJ));
    recoJets.back().setDeepCSV("probbb",    jetdeepcsv_probbb_->at(iJ));
    recoJets.back().setDeepCSV("probcc",    jetdeepcsv_probcc_->at(iJ));

    recoJets.back().setDeepCMVA("probudsg", jetdeepcmva_probudsg_->at(iJ));
    recoJets.back().setDeepCMVA("probb",    jetdeepcmva_probb_->at(iJ));
    recoJets.back().setDeepCMVA("probc",    jetdeepcmva_probc_->at(iJ));
    recoJets.back().setDeepCMVA("probbb",   jetdeepcmva_probbb_->at(iJ));
    recoJets.back().setDeepCMVA("probcc",   jetdeepcmva_probcc_->at(iJ));
  }
  if (options_ & LOADGEN){
    recoJets.back().setIsrMatch(jetisrmatch_->size() ? jetisrmatch_->at(iJ) : false);
  }
}
//--------------------------------------------------------------------------------------------------
void JetReader::addRecoJet(const RecoJetF * inJet){
  // --- IMPORTANT ---
  // This function should be fully in sync w/ JetReader::addRecoJetToObjectList() to avoid vectors w/ different lengths!
  // Variables not filled in ntuples should NOT be added here, nor read in JetReader::addRecoJetToObjectList().
  // -----------------

  //first get the index...we will add it the end of the list
  unsigned int index = jetpt_->size();
  //THen add all to all of the vectors
  jetpt_         ->push_back(inJet->pt());
  jeteta_        ->push_back(inJet->eta());
  jetphi_        ->push_back(inJet->phi());
  jetmass_       ->push_back(inJet->mass());
  jetptraw_      ->push_back(inJet->pt_raw());
  jetpuId_       ->push_back(1);
  jetlooseId_    ->push_back(inJet->looseid());
  jettightId_    ->push_back(1);
  jetchHadEnFrac_->push_back(inJet->chHadFrac());
  jetcsv_        ->push_back(inJet->csv());
  jetcmva_       ->push_back(inJet->cmva());
  jetcvsl_       ->push_back(inJet->cvsl());
  jetcvsb_       ->push_back(inJet->cvsb());
  jetarea_       ->push_back(0);
  jetgenindex_   ->push_back(inJet->genJet() ? inJet->genJet()->index(): -1 );
  jetuncertainty_->push_back(inJet->uncertainty());
  jetbetaStar_   ->push_back(inJet->betaStar());
  jetqgl_        ->push_back(inJet->qgl());
  jetptD_        ->push_back(inJet->ptD());
  jetaxis1_      ->push_back(inJet->axis1());
  jetaxis2_      ->push_back(inJet->axis2());
  jetMult_       ->push_back(inJet->jetMult());
  jetcharge_     ->push_back(inJet->jetcharge());
  jetpullrap_    ->push_back(0);
  jetpullphi_    ->push_back(0);

  if (options_ & LOADDEEPFLAVOR){
    jetdeepcsv_probudsg_ ->push_back(-1);
    jetdeepcsv_probb_    ->push_back(-1);
    jetdeepcsv_probc_    ->push_back(-1);
    jetdeepcsv_probbb_   ->push_back(-1);
    jetdeepcsv_probcc_   ->push_back(-1);
    jetdeepcmva_probudsg_->push_back(-1);
    jetdeepcmva_probb_   ->push_back(-1);
    jetdeepcmva_probc_   ->push_back(-1);
    jetdeepcmva_probbb_  ->push_back(-1);
    jetdeepcmva_probcc_  ->push_back(-1);
  }

  if (options_ & LOADGEN){
    jetisrmatch_         ->push_back(false);
  }

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
