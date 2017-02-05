/*
 * PhotonReader.cc
 *
 *  Created on: May 22, 2015
 *      Author: hqu
 */

#include "AnalysisTools/TreeReader/interface/PhotonReader.h"
#include "AnalysisTools/TreeReader/interface/TreeReader.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"

using namespace std;
using namespace ucsbsusy;

const int PhotonReader::defaultOptions = PhotonReader::LOADRECO | PhotonReader::FILLOBJ;

//--------------------------------------------------------------------------------------------------
PhotonReader::PhotonReader() : BaseReader(){
  pt           = new vector<float>;
  eta          = new vector<float>;
  scEta        = new vector<float>;
  phi          = new vector<float>;
  mass         = new vector<float>;
  r9           = new vector<float> ;
  pfdbetaiso   = new vector<float> ;
  isveto       = new vector<bool>  ;
  isloose      = new vector<bool>  ;
  ismedium     = new vector<bool>  ;
  istight      = new vector<bool>  ;
  hasPixelSeed = new vector<bool>  ;
  passElectronVeto = new vector<bool>;

  full5x5sigietaieta = new vector<float>;
  hOverE             = new vector<float>;
  pfchargedHadronIso = new vector<float>;
  pfphotonIso        = new vector<float>;
  rho                = 0;
}

//--------------------------------------------------------------------------------------------------
void PhotonReader::load(TreeReader *treeReader, int options, string branchName)
{
    loaded_ = true;
    const_cast<int&>(options_)    = options;
    const_cast<string&>(branchName_) = branchName;

    clog << "Loading (" << branchName << ") photons with: ";

  if(options_ & LOADRECO){
    clog << "reco ";
    treeReader->setBranchAddress(branchName , "pt"          , &pt          , true);
    treeReader->setBranchAddress(branchName , "eta"         , &eta         , true);
    treeReader->setBranchAddress(branchName , "SCeta"       , &scEta       , true);
    treeReader->setBranchAddress(branchName , "phi"         , &phi         , true);
    treeReader->setBranchAddress(branchName , "mass"        , &mass        , true);
    treeReader->setBranchAddress(branchName , "r9"          , &r9          , true);
    treeReader->setBranchAddress(branchName , "pfdbetaiso"  , &pfdbetaiso  , true);
    treeReader->setBranchAddress(branchName , "looseid"     , &isloose     , true);
    treeReader->setBranchAddress(branchName , "mediumid"    , &ismedium    , true);
    treeReader->setBranchAddress(branchName , "tightid"     , &istight     , true);
    treeReader->setBranchAddress(branchName , "passElectronVeto", &passElectronVeto, true);
    treeReader->setBranchAddress(branchName , "hasPixelSeed", &hasPixelSeed    , true);

  }
  if(options_ & FILLOBJ)
    clog << "+Objects ";
  if(options_ & LOADHIGHPTID){
    clog << "+HighPtID ";
    treeReader->setBranchAddress(branchName , "full5x5sigietaieta", &full5x5sigietaieta, true);
    treeReader->setBranchAddress(branchName , "hOverE",             &hOverE,             true);
    treeReader->setBranchAddress(branchName , "pfchargediso",       &pfchargedHadronIso, true);
    treeReader->setBranchAddress(branchName , "pfphotoniso",        &pfphotonIso,        true);
    treeReader->setBranchAddress(""         , "rho",                &rho,                true);
  }

  clog << endl;
}

//--------------------------------------------------------------------------------------------------
void PhotonReader::refresh(){
  if(!(options_ & FILLOBJ)) return;

  if(options_ & LOADRECO){
    photons.clear();
    photons.reserve(pt->size());
    for(unsigned int iL = 0; iL < pt->size(); ++iL){
      photons.emplace_back(CylLorentzVectorF(pt->at(iL),eta->at(iL),phi->at(iL),mass->at(iL)),iL);
      photons.back().setSCEta(scEta->at(iL));
      photons.back().setR9(r9->at(iL));
      photons.back().setPFDBetaIso(pfdbetaiso->at(iL));
      photons.back().setIsLoose(isloose->at(iL));
      photons.back().setIsMedium(ismedium->at(iL));
      photons.back().setIsTight(istight->at(iL));
      photons.back().setHasPixelSeed(hasPixelSeed->at(iL));
      photons.back().setPassElectronVeto(passElectronVeto->at(iL));

      if (options_ & LOADHIGHPTID){
        float eta_SC = std::abs(scEta->at(iL));
        float iso_ch = pfchargedHadronIso->at(iL);
        float H_over_E = hOverE->at(iL);
        float sigma_ietaieta = full5x5sigietaieta->at(iL);

        float alpha = 2.5, A = 0, kappa = 0;
        if (eta_SC<0.9){
          A = 0.17; kappa = 4.5e-3;
        }else if (eta_SC<1.4442){
          A = 0.14; kappa = 4.5e-3;
        }else if (eta_SC<1.566){
          // excluded
        }else if (eta_SC<2.0){
          A = 0.11; kappa = 3e-3;
        }else if (eta_SC<2.2){
          A = 0.14; kappa = 3e-3;
        }else if (eta_SC<2.5){
          A = 0.22; kappa = 3e-3;
        }
        float iso_pho = alpha + pfphotonIso->at(iL) - rho*A - kappa*pt->at(iL);

        bool passId = false;
        if (eta_SC<1.4442){
          passId = iso_ch<5 && iso_pho<2.75 && H_over_E<0.05 && sigma_ietaieta<0.0105;
        }else if (eta_SC>1.566){
          passId = iso_ch<5 && iso_pho<2.0  && H_over_E<0.05 && sigma_ietaieta<0.028;
        }
        photons.back().setPassHighPtId(passId);
      }

    }
    std::sort(photons.begin(), photons.end(), PhysicsUtilities::greaterPT<PhotonF>());
  }

}
