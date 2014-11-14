//--------------------------------------------------------------------------------------------------
// 
// MuonReader
// 
// Class for reading muon object information from TTree.
// 
// MuonReader.cc created on Wed Oct 22 01:39:44 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisTools/TreeReader/interface/MuonReader.h"
#include "AnalysisTools/TreeReader/interface/TreeReader.h"

using namespace std;
using namespace ucsbsusy;

const int MuonReader::defaultOptions = MuonReader::LOADRECO | MuonReader::FILLOBJ;

//--------------------------------------------------------------------------------------------------
MuonReader::MuonReader() : BaseReader(){
  pt           = new vector<float>;
  eta          = new vector<float>;
  phi          = new vector<float>;
  mass         = new vector<float>;
  q            = new vector<int>   ;
  d0           = new vector<float>;
  dz           = new vector<float>;
  pfdbetaiso   = new vector<float>;
  isloose      = new vector<bool>;
  istight      = new vector<bool>;
  ispf         = new vector<bool>;
  isglobal     = new vector<bool>;
  istracker    = new vector<bool>;
  isstandalone = new vector<bool>;
  muonId       = new LeptonId();
}

//--------------------------------------------------------------------------------------------------
void MuonReader::load(TreeReader *treeReader, int options, string branchName)
{
  loaded_ = true;
    const_cast<int&>(options_)    = options;
    const_cast<string&>(branchName_) = branchName;

    cout << "Loading (" << branchName << ") muons with: ";

  if(options_ & LOADRECO){
    cout << "reco ";
    treeReader->setBranchAddress(branchName ,"pt"          , &pt          , true);
    treeReader->setBranchAddress(branchName ,"eta"         , &eta         , true);
    treeReader->setBranchAddress(branchName ,"phi"         , &phi         , true);
    treeReader->setBranchAddress(branchName ,"mass"        , &mass        , true);
    treeReader->setBranchAddress(branchName ,"q", &q                      , true);
    treeReader->setBranchAddress(branchName ,"d0", &d0                    , true);
    treeReader->setBranchAddress(branchName ,"dz", &dz                    , true);
    treeReader->setBranchAddress(branchName ,"pfdbetaiso", &pfdbetaiso    , true);
    treeReader->setBranchAddress(branchName ,"isLoose", &isloose          , true);
    treeReader->setBranchAddress(branchName ,"isTight", &istight          , true);
    treeReader->setBranchAddress(branchName ,"isPF", &ispf                , true);
    treeReader->setBranchAddress(branchName ,"isGlobal", &isglobal        , true);
    treeReader->setBranchAddress(branchName ,"isTracker", &istracker      , true);
    treeReader->setBranchAddress(branchName ,"isStandAlone", &isstandalone, true);
  }
  if(options_ & FILLOBJ)
    cout << "+Objects";
  cout << endl;
}

//--------------------------------------------------------------------------------------------------
void MuonReader::refresh(){
  if(!(options_ & FILLOBJ)) return;

  if(options_ & LOADRECO){
    muons.clear();
    muons.reserve(pt->size());
    for(unsigned int iL = 0; iL < pt->size(); ++iL){
      muons.emplace_back(CylLorentzVectorF(pt->at(iL),eta->at(iL),phi->at(iL),mass->at(iL)),iL);
      muons.back().setCharge(q->at(iL));
      muons.back().setD0(d0->at(iL));
      muons.back().setDz(dz->at(iL));
      muons.back().setPFDBetaIso(pfdbetaiso->at(iL));
      muons.back().setIsLoose(isloose->at(iL));
      muons.back().setIsTight(istight->at(iL));
      muons.back().setIsPF(ispf->at(iL));
      muons.back().setIsGlobal(isglobal->at(iL));
      muons.back().setIsTracker(istracker->at(iL));
      muons.back().setIsStandalone(isstandalone->at(iL));
      muons.back().setIsGoodPOGMuon(muonId->passMuonId((&muons.back())));
    }
  }
}
