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
    treeReader->setBranchAddress(branchName , "passConvVeto", &passElectronVeto, true);
    treeReader->setBranchAddress(branchName , "hasPixelSeed", &hasPixelSeed    , true);

  }
  if(options_ & FILLOBJ)
    clog << "+Objects";
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
    }
    std::sort(photons.begin(), photons.end(), PhysicsUtilities::greaterPT<PhotonF>());
  }

}
