//--------------------------------------------------------------------------------------------------
//
// JetReader
//
// Class for reading electron object information from TTree.
//
// ElectronReader.cc created on Wed Oct 22 01:39:44 CEST 2014
//
//--------------------------------------------------------------------------------------------------

#include "AnalysisTools/TreeReader/interface/SVReader.h"
#include "AnalysisTools/TreeReader/interface/TreeReader.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"

using namespace std;
using namespace ucsbsusy;

//const int SVReader::defaultOptions = SVReader::LOADRECO | SVReader::FILLOBJ | SVReader::LOADMVA;
const int SVReader::defaultOptions = SVReader::LOADRECO | SVReader::FILLOBJ;

//--------------------------------------------------------------------------------------------------
SVReader::SVReader() : BaseReader(){
  svpt_      = new vector<float>;
  sveta_     = new vector<float>;
  svphi_     = new vector<float>;
  svmass_    = new vector<float>;
  svntracks_ = new vector<int>;
  svchi2_    = new vector<float>;
  svndf_     = new vector<float>;
  svdxy_     = new vector<float>;
  svdxyerr_  = new vector<float>;
  svd3d_     = new vector<float>;
  svd3derr_  = new vector<float>;
  svcossvpv_ = new vector<float>;
}

//--------------------------------------------------------------------------------------------------
void SVReader::load(TreeReader *treeReader, int options, string branchName)
{
    loaded_ = true;
    const_cast<int&>(options_)    = options;
    const_cast<string&>(branchName_) = branchName;

    clog << "Loading (" << branchName << ") SVs with: ";

    if(options_) {
      treeReader->setBranchAddress(branchName_, "sv_pt"          , &svpt_,true);
      treeReader->setBranchAddress(branchName_, "sv_eta"         , &sveta_,true);
      treeReader->setBranchAddress(branchName_, "sv_phi"         , &svphi_,true);
      treeReader->setBranchAddress(branchName_, "sv_mass"        , &svmass_,true);
      treeReader->setBranchAddress(branchName_, "sv_ntracks"     , &svntracks_,true);
      treeReader->setBranchAddress(branchName_, "sv_chi2"        , &svchi2_   ,true);
      treeReader->setBranchAddress(branchName_, "sv_ndf"         , &svndf_    ,true);
      treeReader->setBranchAddress(branchName_, "sv_dxy"         , &svdxy_    ,true);
      treeReader->setBranchAddress(branchName_, "sv_dxyerr"      , &svdxyerr_ ,true);
      treeReader->setBranchAddress(branchName_, "sv_d3d"         , &svd3d_    ,true);
      treeReader->setBranchAddress(branchName_, "sv_d3derr"      , &svd3derr_ ,true);
      treeReader->setBranchAddress(branchName_, "sv_costhetasvpv", &svcossvpv_,true);

    }

    if(options_ & FILLOBJ)
      clog << "+Objects";
    clog << endl;

    if(options_ & LOADMVA){
      TString cmsswpath = getenv("CMSSW_BASE");
      //std::cout << getenv("CMSSW_BASE") << "\n";

      ivfMVA_lpt_b = new IVFMVA(cmsswpath+"/src/data/ivf/pteta_low_b_BDTG.weights.xml", "BDTG");
      ivfMVA_lpt_e = new IVFMVA(cmsswpath+"/src/data/ivf/pteta_low_e_BDTG.weights.xml", "BDTG");
      ivfMVA_hpt_b = new IVFMVA(cmsswpath+"/src/data/ivf/pteta_high_b_BDTG.weights.xml", "BDTG");
      ivfMVA_hpt_e = new IVFMVA(cmsswpath+"/src/data/ivf/pteta_high_e_BDTG.weights.xml", "BDTG");
    }
}

//--------------------------------------------------------------------------------------------------
void SVReader::refresh(){

  if(!(options_ & FILLOBJ)) return;

  SVs.clear();
  SVs.reserve(svpt_->size());
  for(unsigned int iJ = 0; iJ < svpt_->size(); ++iJ){

    SVs.emplace_back(CylLorentzVectorF(svpt_->at(iJ),sveta_->at(iJ),svphi_->at(iJ),svmass_->at(iJ)),iJ);
    SVs.back().setSVNTracks(svntracks_->at(iJ));
    SVs.back().setSVChi2(svchi2_->at(iJ));
    SVs.back().setSVNdf(svndf_->at(iJ));
    SVs.back().setSVdxy(svdxy_->at(iJ));
    SVs.back().setSVdxyerr(svdxyerr_->at(iJ));
    SVs.back().setSVd3D(svd3d_->at(iJ));
    SVs.back().setSVd3Derr(svd3derr_->at(iJ));
    SVs.back().setSVCosSVPV(svcossvpv_->at(iJ));
    SVs.back().setSVMVA(-1.);

  }
  std::sort(SVs.begin(),SVs.end(),PhysicsUtilities::greaterPT<SVF>());

  // add ivfmva value in the SV collection
  if (options_ & LOADMVA){
    for (unsigned int i0=0; i0<SVs.size(); ++i0) {

      if (SVs[i0].pt()<15.  && fabs(SVs[i0].eta())<1.2 ) { SVs[i0].setSVMVA(ivfMVA_lpt_b->getIVFCandScore(SVs[i0])); }
      if (SVs[i0].pt()<15.  && fabs(SVs[i0].eta())>=1.2) { SVs[i0].setSVMVA(ivfMVA_lpt_e->getIVFCandScore(SVs[i0])); }
      if (SVs[i0].pt()>=15. && fabs(SVs[i0].eta())<1.2 ) { SVs[i0].setSVMVA(ivfMVA_hpt_b->getIVFCandScore(SVs[i0])); }
      if (SVs[i0].pt()>=15. && fabs(SVs[i0].eta())>=1.2) { SVs[i0].setSVMVA(ivfMVA_hpt_e->getIVFCandScore(SVs[i0])); }

    }
  }

}
