//--------------------------------------------------------------------------------------------------
// 
// TauReader
// 
// Class for reading electron object information from TTree.
// 
// TauReader.cc created on Wed Oct 22 23:13:49 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisTools/TreeReader/interface/TauReader.h"

using namespace ucsbsusy;

const int TauReader::defaultOptions = TauReader::LOADRECO | TauReader::FILLOBJ;

//--------------------------------------------------------------------------------------------------
TauReader::TauReader() : BaseReader(){
  pt           = new vector<float>;
  eta          = new vector<float>;
  phi          = new vector<float>;
  mass         = new vector<float>;
  leadcandpt     = new  vector<float>         ;
  leadcandeta    = new  vector<float>         ;
  leadcandphi    = new  vector<float>         ;
  leadcandmass   = new  vector<float>         ;
  leadchcandpt   = new  vector<float>         ;
  leadchcandeta  = new  vector<float>         ;
  leadchcandphi  = new  vector<float>         ;
  leadchcandmass = new  vector<float>         ;
  q              = new  vector<int>           ;
  dxy            = new  vector<float>         ;
  dxyerr         = new  vector<float>         ;
  dxysig         = new  vector<float>         ;
  hpsid           = new  vector<unsigned long>;
}

//--------------------------------------------------------------------------------------------------
void TauReader::load(TTree *tree, int options, string branchName)
{
    const_cast<int&>(options_)    = options;
    const_cast<string&>(branchName_) = branchName;

    cout << "Loading (" << branchName << ") taus with: ";

  if(options_ & LOADRECO){
    cout <<"reco ";
    tree->SetBranchAddress((branchName + "_pt"          ).c_str(), &pt  );
    tree->SetBranchAddress((branchName + "_eta"         ).c_str(), &eta );
    tree->SetBranchAddress((branchName + "_phi"         ).c_str(), &phi );
    tree->SetBranchAddress((branchName + "_mass"        ).c_str(), &mass);
    tree->SetBranchAddress((branchName + "_leadcand_pt").c_str(), &leadcandpt);
    tree->SetBranchAddress((branchName + "_leadcand_eta").c_str(), &leadcandeta);
    tree->SetBranchAddress((branchName + "_leadcand_phi").c_str(), &leadcandphi);
    tree->SetBranchAddress((branchName + "_leadcand_mass").c_str(), &leadcandmass);
    tree->SetBranchAddress((branchName + "_leadchargedcand_pt").c_str(), &leadchcandpt);
    tree->SetBranchAddress((branchName + "_leadchargedcand_eta").c_str(), &leadchcandeta);
    tree->SetBranchAddress((branchName + "_leadchargedcand_phi").c_str(), &leadchcandphi);
    tree->SetBranchAddress((branchName + "_leadchargedcand_mass").c_str(), &leadchcandmass);
    tree->SetBranchAddress((branchName + "_q").c_str(), &q);
    tree->SetBranchAddress((branchName + "_dxy").c_str(), &dxy);
    tree->SetBranchAddress((branchName + "_dxyerr").c_str(), &dxyerr);
    tree->SetBranchAddress((branchName + "_dxysig").c_str(), &dxysig);
    tree->SetBranchAddress((branchName + "_idflags").c_str(), &hpsid);
  }
  if(options_ & FILLOBJ)
    cout << "+Objects";
  cout << endl;
}

//--------------------------------------------------------------------------------------------------
void TauReader::refresh(){
  if(!(options_ & FILLOBJ)) return;

  if(options_ & LOADRECO){
    taus.clear();
    taus.reserve(pt->size());
    for(unsigned int iL = 0; iL < pt->size(); ++iL){
      taus.emplace_back(CylLorentzVectorF(pt->at(iL),eta->at(iL),phi->at(iL),mass->at(iL)),iL);
      taus.back().setCharge(q->at(iL));
      taus.back().setDxy(dxy->at(iL));
      taus.back().setDxyErr(dxyerr->at(iL));
      taus.back().setDxySig(dxysig->at(iL));
      taus.back().setHPSId(hpsid->at(iL));
      taus.back().setLeadCandidate(CylLorentzVectorF(leadcandpt->at(iL), leadcandeta->at(iL), leadcandphi->at(iL), leadcandmass->at(iL)));
      taus.back().setLeadChargedCandidate(CylLorentzVectorF(leadchcandpt->at(iL), leadchcandeta->at(iL), leadchcandphi->at(iL), leadchcandmass->at(iL)));
    }
  }
}

