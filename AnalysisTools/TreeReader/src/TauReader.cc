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
  hpsid          = new  vector<unsigned long>;
  tauId          = new  LeptonId();
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
    treeReader->setBranchAddress(branchName,"leadcand_pt", &leadcandpt             , true);
    treeReader->setBranchAddress(branchName,"leadcand_eta", &leadcandeta           , true);
    treeReader->setBranchAddress(branchName,"leadcand_phi", &leadcandphi           , true);
    treeReader->setBranchAddress(branchName,"leadcand_mass", &leadcandmass         , true);
    treeReader->setBranchAddress(branchName,"leadchargedcand_pt", &leadchcandpt    , true);
    treeReader->setBranchAddress(branchName,"leadchargedcand_eta", &leadchcandeta  , true);
    treeReader->setBranchAddress(branchName,"leadchargedcand_phi", &leadchcandphi  , true);
    treeReader->setBranchAddress(branchName,"leadchargedcand_mass", &leadchcandmass, true);
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
      taus.back().setLeadCandidate(CylLorentzVectorF(leadcandpt->at(iL), leadcandeta->at(iL), leadcandphi->at(iL), leadcandmass->at(iL)));
      taus.back().setLeadChargedCandidate(CylLorentzVectorF(leadchcandpt->at(iL), leadchcandeta->at(iL), leadchcandphi->at(iL), leadchcandmass->at(iL)));
      taus.back().setIsGoodPOGTau(tauId->passTauId((&taus.back())));
      taus.back().setIsHPSVetoTau((hpsid->at(iL) & kLooseIsoDB3Hits) && ((hpsid->at(iL) & kDecayMode)>0));
    }
  }
}

