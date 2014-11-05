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
void MuonReader::load(TTree *tree, int options, string branchName)
{
    const_cast<int&>(options_)    = options;
    const_cast<string&>(branchName_) = branchName;

    cout << "Loading (" << branchName << ") muons with: ";

  if(options_ & LOADRECO){
    cout << "reco ";
    tree->SetBranchAddress((branchName + "_pt"          ).c_str(), &pt  );
    tree->SetBranchAddress((branchName + "_eta"         ).c_str(), &eta );
    tree->SetBranchAddress((branchName + "_phi"         ).c_str(), &phi );
    tree->SetBranchAddress((branchName + "_mass"        ).c_str(), &mass);
    tree->SetBranchAddress((branchName + "_q").c_str(), &q);
    tree->SetBranchAddress((branchName + "_d0").c_str(), &d0);
    tree->SetBranchAddress((branchName + "_dz").c_str(), &dz);
    tree->SetBranchAddress((branchName + "_pfdbetaiso").c_str(), &pfdbetaiso);
    tree->SetBranchAddress((branchName + "_isLoose").c_str(), &isloose);
    tree->SetBranchAddress((branchName + "_isTight").c_str(), &istight);
    tree->SetBranchAddress((branchName + "_isPF").c_str(), &ispf);
    tree->SetBranchAddress((branchName + "_isGlobal").c_str(), &isglobal);
    tree->SetBranchAddress((branchName + "_isTracker").c_str(), &istracker);
    tree->SetBranchAddress((branchName + "_isStandAlone").c_str(), &isstandalone);
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
