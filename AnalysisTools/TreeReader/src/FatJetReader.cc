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

const int FatJetReader::defaultOptions = FatJetReader::LOADRECO | FatJetReader::FILLOBJ | FatJetReader::LOADSHAPE;

//--------------------------------------------------------------------------------------------------
FatJetReader::FatJetReader() : BaseReader(){
  fjrawmass_       = new vector<float>;
  fjprunedmass_    = new vector<float>;
  fjsoftdropmass_  = new vector<float>;
  fjtau1_          = new vector<float>;
  fjtau2_          = new vector<float>;
  fjtau3_          = new vector<float>;
  fjpt_            = new vector<float>;
  fjeta_           = new vector<float>;
  fjphi_           = new vector<float>;
  fjcsv_           = new vector<float>;
  fjnsdsubjets_    = new vector<int  >;
  fjsdsj1mass_     = new vector<float>;
  fjsdsj1pt_       = new vector<float>;
  fjsdsj1eta_      = new vector<float>;
  fjsdsj1phi_      = new vector<float>;
  fjsdsj2mass_     = new vector<float>;
  fjsdsj1csv_      = new vector<float>;
  fjsdsj1cmva_     = new vector<float>;
  fjsdsj1cvsl_     = new vector<float>;
  fjsdsj1cvsb_     = new vector<float>;
  fjsdsj1ptD_      = new vector<float>;
  fjsdsj1axis1_    = new vector<float>;
  fjsdsj1axis2_    = new vector<float>;
  fjsdsj1mult_     = new vector<int  >;
  fjsdsj2pt_       = new vector<float>;
  fjsdsj2eta_      = new vector<float>;
  fjsdsj2phi_      = new vector<float>;
  fjsdsj2csv_      = new vector<float>;
  fjsdsj2cmva_     = new vector<float>;
  fjsdsj2cvsl_     = new vector<float>;
  fjsdsj2cvsb_     = new vector<float>;
  fjsdsj2ptD_      = new vector<float>;
  fjsdsj2axis1_    = new vector<float>;
  fjsdsj2axis2_    = new vector<float>;
  fjsdsj2mult_     = new vector<int  >;
  fj_puppi_pt_            = new vector<float>;
  fj_puppi_eta_           = new vector<float>;
  fj_puppi_phi_           = new vector<float>;
  fj_puppi_mass_          = new vector<float>;
  fj_puppi_tau1_          = new vector<float>;
  fj_puppi_tau2_          = new vector<float>;
  fj_puppi_tau3_          = new vector<float>;
  fj_puppi_softdropmass_  = new vector<float>;
  fj_puppi_nsdsubjets_    = new vector<int  >;
  fj_puppi_sdsj1mass_     = new vector<float>;
  fj_puppi_sdsj1pt_       = new vector<float>;
  fj_puppi_sdsj1eta_      = new vector<float>;
  fj_puppi_sdsj1phi_      = new vector<float>;
  fj_puppi_sdsj2mass_     = new vector<float>;
  fj_puppi_sdsj1csv_      = new vector<float>;
  fj_puppi_sdsj1cmva_     = new vector<float>;
  fj_puppi_sdsj1cvsl_     = new vector<float>;
  fj_puppi_sdsj1cvsb_     = new vector<float>;
  fj_puppi_sdsj2pt_       = new vector<float>;
  fj_puppi_sdsj2eta_      = new vector<float>;
  fj_puppi_sdsj2phi_      = new vector<float>;
  fj_puppi_sdsj2csv_      = new vector<float>;
  fj_puppi_sdsj2cmva_     = new vector<float>;
  fj_puppi_sdsj2cvsl_     = new vector<float>;
  fj_puppi_sdsj2cvsb_     = new vector<float>;
}

//--------------------------------------------------------------------------------------------------
void FatJetReader::load(TreeReader *treeReader, int options, string branchName)
{
    loaded_ = true;
    const_cast<int&>(options_)    = options;
    const_cast<string&>(branchName_) = branchName;

    clog << "Loading (" << branchName << ") tops with: ";

    if(options_ & LOADRECO) {
      clog << " +Reco ";
      treeReader->setBranchAddress(branchName_, "fatjet_rawmass"          , &fjrawmass_      ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_prunedmass"       , &fjprunedmass_   ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_softdropmass"     , &fjsoftdropmass_ ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_tau1"             , &fjtau1_         ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_tau2"             , &fjtau2_         ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_tau3"             , &fjtau3_         ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_pt"               , &fjpt_           ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_eta"              , &fjeta_          ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_phi"              , &fjphi_          ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_csv"              , &fjcsv_          ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_nsoftdropsubjets" , &fjnsdsubjets_ ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_sdsubjet1_mass"   , &fjsdsj1mass_ ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_sdsubjet1_pt"     , &fjsdsj1pt_ ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_sdsubjet1_eta"    , &fjsdsj1eta_ ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_sdsubjet1_phi"    , &fjsdsj1phi_ ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_sdsubjet1_csv"    , &fjsdsj1csv_ ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_sdsubjet2_mass"   , &fjsdsj2mass_ ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_sdsubjet2_pt"     , &fjsdsj2pt_ ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_sdsubjet2_eta"    , &fjsdsj2eta_ ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_sdsubjet2_phi"    , &fjsdsj2phi_ ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_sdsubjet2_csv"    , &fjsdsj2csv_ ,true);

      if(options_ & LOADSHAPE){
        clog << " +JetShape ";
        treeReader->setBranchAddress(branchName_, "fatjet_sdsubjet1_ptD"    , &fjsdsj1ptD_,   false);//FIXME
        treeReader->setBranchAddress(branchName_, "fatjet_sdsubjet1_axis1"  , &fjsdsj1axis1_, false);
        treeReader->setBranchAddress(branchName_, "fatjet_sdsubjet1_axis2"  , &fjsdsj1axis2_, false);
        treeReader->setBranchAddress(branchName_, "fatjet_sdsubjet1_jetMult", &fjsdsj1mult_,  false);

        treeReader->setBranchAddress(branchName_, "fatjet_sdsubjet2_ptD"    , &fjsdsj2ptD_,   false);
        treeReader->setBranchAddress(branchName_, "fatjet_sdsubjet2_axis1"  , &fjsdsj2axis1_, false);
        treeReader->setBranchAddress(branchName_, "fatjet_sdsubjet2_axis2"  , &fjsdsj2axis2_, false);
        treeReader->setBranchAddress(branchName_, "fatjet_sdsubjet2_jetMult", &fjsdsj2mult_,  false);
      }

      if(options_ & LOADCTAG){
        clog << " +CTag ";
        treeReader->setBranchAddress(branchName_, "fatjet_sdsubjet1_cmva"   , &fjsdsj1cmva_ ,false);//FIXME
        treeReader->setBranchAddress(branchName_, "fatjet_sdsubjet1_cvsl"   , &fjsdsj1cvsl_ ,false);
        treeReader->setBranchAddress(branchName_, "fatjet_sdsubjet1_cvsb"   , &fjsdsj1cvsb_ ,false);
        treeReader->setBranchAddress(branchName_, "fatjet_sdsubjet2_cmva"   , &fjsdsj2cmva_ ,false);
        treeReader->setBranchAddress(branchName_, "fatjet_sdsubjet2_cvsl"   , &fjsdsj2cvsl_ ,false);
        treeReader->setBranchAddress(branchName_, "fatjet_sdsubjet2_cvsb"   , &fjsdsj2cvsb_ ,false);

      }

    }
    if(options_ & LOADPUPPI) {
      clog << " +Reco ";
      treeReader->setBranchAddress(branchName_, "fatjet_puppi_mass"             , &fj_puppi_mass_      ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_puppi_sdmass"           , &fj_puppi_softdropmass_ ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_puppi_tau1"             , &fj_puppi_tau1_         ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_puppi_tau2"             , &fj_puppi_tau2_         ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_puppi_tau3"             , &fj_puppi_tau3_         ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_puppi_pt"               , &fj_puppi_pt_           ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_puppi_eta"              , &fj_puppi_eta_          ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_puppi_phi"              , &fj_puppi_phi_          ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_puppi_nsoftdropsubjets" , &fj_puppi_nsdsubjets_ ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_puppi_sdsubjet1_mass"   , &fj_puppi_sdsj1mass_ ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_puppi_sdsubjet1_pt"     , &fj_puppi_sdsj1pt_ ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_puppi_sdsubjet1_eta"    , &fj_puppi_sdsj1eta_ ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_puppi_sdsubjet1_phi"    , &fj_puppi_sdsj1phi_ ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_puppi_sdsubjet1_csv"    , &fj_puppi_sdsj1csv_ ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_puppi_sdsubjet2_mass"   , &fj_puppi_sdsj2mass_ ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_puppi_sdsubjet2_pt"     , &fj_puppi_sdsj2pt_ ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_puppi_sdsubjet2_eta"    , &fj_puppi_sdsj2eta_ ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_puppi_sdsubjet2_phi"    , &fj_puppi_sdsj2phi_ ,true);
      treeReader->setBranchAddress(branchName_, "fatjet_puppi_sdsubjet2_csv"    , &fj_puppi_sdsj2csv_ ,true);

      if(options_ & LOADCTAG){
        clog << " +CTag ";
        treeReader->setBranchAddress(branchName_, "fatjet_puppi_sdsubjet1_cmva"   , &fj_puppi_sdsj1cmva_,false);//FIXME
        treeReader->setBranchAddress(branchName_, "fatjet_puppi_sdsubjet1_cvsl"   , &fj_puppi_sdsj1cvsl_,false);
        treeReader->setBranchAddress(branchName_, "fatjet_puppi_sdsubjet1_cvsb"   , &fj_puppi_sdsj1cvsb_,false);
        treeReader->setBranchAddress(branchName_, "fatjet_puppi_sdsubjet2_cmva"   , &fj_puppi_sdsj2cmva_,false);
        treeReader->setBranchAddress(branchName_, "fatjet_puppi_sdsubjet2_cvsl"   , &fj_puppi_sdsj2cvsl_,false);
        treeReader->setBranchAddress(branchName_, "fatjet_puppi_sdsubjet2_cvsb"   , &fj_puppi_sdsj2cvsb_,false);
      }
    }

    if(options_ & FILLOBJ)
      clog << " +Objects ";
    clog << endl;
}

//--------------------------------------------------------------------------------------------------
void FatJetReader::refresh(){

  if(!(options_ & FILLOBJ)) return;
  if(!(options_ & LOADRECO)) return;

  fatJets.clear();
  fatJets.reserve(fjpt_->size());

  for(unsigned int iJ = 0; iJ < fjpt_->size(); ++iJ){
    fatJets.emplace_back(CylLorentzVectorF(fjpt_->at(iJ),fjeta_->at(iJ),fjphi_->at(iJ),fjrawmass_->at(iJ)),
        iJ, fjcsv_->at(iJ), fjprunedmass_->at(iJ),fjsoftdropmass_->at(iJ),fjtau1_->at(iJ),fjtau2_->at(iJ),fjtau3_->at(iJ));

    if(fjnsdsubjets_->at(iJ) > 0 ){
      fatJets.back().addSubjet(CylLorentzVectorF(fjsdsj1pt_->at(iJ),fjsdsj1eta_->at(iJ),fjsdsj1phi_->at(iJ),fjsdsj1mass_->at(iJ)),
          fjsdsj1csv_->at(iJ));

      if(options_ & LOADSHAPE){
        if(fjsdsj1ptD_->empty()) throw std::range_error("[FatJetReader::refresh] Subjet shape variables not filled in ntuples!");
        fatJets.back().subJet(0).addQuarkGluonVars(fjsdsj1ptD_->at(iJ), fjsdsj1axis1_->at(iJ), fjsdsj1axis2_->at(iJ), fjsdsj1mult_->at(iJ));
      }

      if(options_ & LOADCTAG){
        if(fjsdsj1cmva_->empty()) throw std::range_error("[FatJetReader::refresh] Subjet c-tagger not filled in ntuples!");
        fatJets.back().subJet(0).addCTagging(fjsdsj1cmva_->at(iJ), fjsdsj1cvsl_->at(iJ), fjsdsj1cvsb_->at(iJ));
      }
    }

    if(fjnsdsubjets_->at(iJ) > 1 ){
      fatJets.back().addSubjet(CylLorentzVectorF(fjsdsj2pt_->at(iJ),fjsdsj2eta_->at(iJ),fjsdsj2phi_->at(iJ),fjsdsj2mass_->at(iJ)),
          fjsdsj2csv_->at(iJ));

      if(options_ & LOADSHAPE){
        if(fjsdsj2ptD_->empty()) throw std::range_error("[FatJetReader::refresh] Subjet shape variables not filled in ntuples!");
        fatJets.back().subJet(1).addQuarkGluonVars(fjsdsj2ptD_->at(iJ), fjsdsj2axis1_->at(iJ), fjsdsj2axis2_->at(iJ), fjsdsj2mult_->at(iJ));
      }

      if(options_ & LOADCTAG){
        if(fjsdsj2cmva_->empty()) throw std::range_error("[FatJetReader::refresh] Subjet c-tagger not filled in ntuples!");
        fatJets.back().subJet(1).addCTagging(fjsdsj2cmva_->at(iJ), fjsdsj2cvsl_->at(iJ), fjsdsj2cvsb_->at(iJ));
      }
    }

    if(options_ & LOADPUPPI){
      fatJets.back().addPuppiInfo(CylLorentzVectorF(fj_puppi_pt_->at(iJ),fj_puppi_eta_->at(iJ),fj_puppi_phi_->at(iJ),fj_puppi_mass_->at(iJ)),
          fj_puppi_softdropmass_->at(iJ),fj_puppi_tau1_->at(iJ),fj_puppi_tau2_->at(iJ),fj_puppi_tau3_->at(iJ));

      if(fj_puppi_nsdsubjets_->at(iJ) > 0 ){
        fatJets.back().addPuppiSubjet(CylLorentzVectorF(fj_puppi_sdsj1pt_->at(iJ),fj_puppi_sdsj1eta_->at(iJ),fj_puppi_sdsj1phi_->at(iJ),fj_puppi_sdsj1mass_->at(iJ)),
            fj_puppi_sdsj1csv_->at(iJ));

        if(options_ & LOADCTAG){
          if (fj_puppi_sdsj1cmva_->empty()) throw std::range_error("[FatJetReader::refresh] Subjet c-tagger not filled in ntuples!");
          fatJets.back().subJet(0).addCTagging(fj_puppi_sdsj1cmva_->at(iJ), fj_puppi_sdsj1cvsl_->at(iJ), fj_puppi_sdsj1cvsb_->at(iJ));
        }
      }

      if(fj_puppi_nsdsubjets_->at(iJ) > 1 ){
        fatJets.back().addPuppiSubjet(CylLorentzVectorF(fj_puppi_sdsj2pt_->at(iJ),fj_puppi_sdsj2eta_->at(iJ),fj_puppi_sdsj2phi_->at(iJ),fj_puppi_sdsj2mass_->at(iJ)),
            fj_puppi_sdsj2csv_->at(iJ));

        if(options_ & LOADCTAG){
          if (fj_puppi_sdsj2cmva_->empty()) throw std::range_error("[FatJetReader::refresh] Subjet c-tagger not filled in ntuples!");
          fatJets.back().subJet(1).addCTagging(fj_puppi_sdsj2cmva_->at(iJ), fj_puppi_sdsj2cvsl_->at(iJ), fj_puppi_sdsj2cvsb_->at(iJ));
        }
      }

    }
  }
  std::sort(fatJets.begin(),fatJets.end(),PhysicsUtilities::greaterPT<FatJetF>());

}
