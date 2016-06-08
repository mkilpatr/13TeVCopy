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
#include "AnalysisTools/TreeReader/interface/TreeReader.h"

using namespace std;
using namespace ucsbsusy;

const int TauReader::defaultOptions = TauReader::LOADRECO | TauReader::FILLOBJ;

//--------------------------------------------------------------------------------------------------
TauReader::TauReader() : BaseReader(){
  pt           = new vector<float>;
  eta          = new vector<float>;
  phi          = new vector<float>;
  mass         = new vector<float>;
  q              = new  vector<int>           ;
  dxy            = new  vector<float>         ;
  dxyerr         = new  vector<float>         ;
  dxysig         = new  vector<float>         ;
  hpsid          = new  vector<unsigned long>;
}

//--------------------------------------------------------------------------------------------------
void TauReader::load(TreeReader *treeReader, int options, string branchName)
{
    loaded_ = true;
    const_cast<int&>(options_)    = options;
    const_cast<string&>(branchName_) = branchName;

    cout << "Loading (" << branchName << ") taus with: ";

  if(options_ & LOADRECO){
    cout <<"reco ";
    treeReader->setBranchAddress(branchName,"pt"          , &pt                    , true);
    treeReader->setBranchAddress(branchName,"eta"         , &eta                   , true);
    treeReader->setBranchAddress(branchName,"phi"         , &phi                   , true);
    treeReader->setBranchAddress(branchName,"mass"        , &mass                  , true);
    treeReader->setBranchAddress(branchName,"q", &q                                , true);
    treeReader->setBranchAddress(branchName,"dxy", &dxy                            , true);
    treeReader->setBranchAddress(branchName,"dxyerr", &dxyerr                      , true);
    treeReader->setBranchAddress(branchName,"dxysig", &dxysig                      , true);
    treeReader->setBranchAddress(branchName,"idflags", &hpsid                      , true);
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
      taus.back().setIsGoodPOGTau((hpsid->at(iL) & kMediumIsoDB3Hits)>0 && ((hpsid->at(iL) & kDecayMode)>0));
      taus.back().setIsHPSVetoTau((hpsid->at(iL) & kLooseIsoDB3Hits)>0 && ((hpsid->at(iL) & kDecayMode)>0));
      //      taus.back().setisPOGVLIsoMVARun2NewTau((hpsid->at(iL) & kVLooseIsoMVARun2newDMwLT)>0 && ((hpsid->at(iL) & kDecayModeNew)>0)); //need to be put back as soon as we have new ntuples - then delete last line.
      taus.back().setisPOGVLIsoMVARun2NewTau((hpsid->at(iL) & kVLooseIsoMVARun2newDMwLT)>0);
    }
  }
}

