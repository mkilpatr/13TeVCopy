//--------------------------------------------------------------------------------------------------
// 
// PFCandidateReader
// 
// Class for reading PFCandidate object information from TTree.
// 
// PFCandidateReader.cc created on Wed Oct 22 01:39:44 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisTools/TreeReader/interface/PFCandidateReader.h"
#include "AnalysisTools/TreeReader/interface/TreeReader.h"

using namespace std;
using namespace ucsbsusy;

const int PFCandidateReader::defaultOptions = PFCandidateReader::LOADRECO | PFCandidateReader::FILLOBJ;

//--------------------------------------------------------------------------------------------------
PFCandidateReader::PFCandidateReader() : BaseReader(){
  pt           = new vector<float>;
  eta          = new vector<float>;
  phi          = new vector<float>;
  mass         = new vector<float>;
  q            = new vector<int>   ;
  pdgid        = new vector<int>   ;
  d0           = new vector<float> ;
  dz           = new vector<float> ;
  mt           = new vector<float> ;
  taudisc      = new vector<float> ;
  fromPV       = new vector<int>   ;
  jetIndex     = new vector<int>   ;
  tauIndex     = new vector<int>   ;
}

//--------------------------------------------------------------------------------------------------
void PFCandidateReader::load(TreeReader *treeReader, int options, string branchName)
{
    loaded_ = true;
    const_cast<int&>(options_)    = options;
    const_cast<string&>(branchName_) = branchName;

    clog << "Loading (" << branchName << ") pfcands with: ";

  if(options_ & LOADRECO){
    clog << "reco ";
    treeReader->setBranchAddress(branchName , "pt"          , &pt          , true);
    treeReader->setBranchAddress(branchName , "eta"         , &eta         , true);
    treeReader->setBranchAddress(branchName , "phi"         , &phi         , true);
    treeReader->setBranchAddress(branchName , "mass"        , &mass        , true);
    treeReader->setBranchAddress(branchName , "q"           , &q           , true);
    treeReader->setBranchAddress(branchName , "pdgid"       , &pdgid       , true);
    treeReader->setBranchAddress(branchName , "d0"          , &d0          , true);
    treeReader->setBranchAddress(branchName , "dz"          , &dz          , true);
    treeReader->setBranchAddress(branchName , "mttrkplusphoton", &mt          , true);
    treeReader->setBranchAddress(branchName , "taudisc"     , &taudisc     , true);
    treeReader->setBranchAddress(branchName , "fromPV"      , &fromPV      , true);
    treeReader->setBranchAddress(branchName , "contJetIndex", &jetIndex    , true);
    treeReader->setBranchAddress(branchName , "contTauIndex", &tauIndex    , true);
  }
  if(options_ & FILLOBJ)
    clog << "+Objects";
  clog << endl;
}

//--------------------------------------------------------------------------------------------------
void PFCandidateReader::refresh(){
  if(!(options_ & FILLOBJ)) return;

  if(options_ & LOADRECO){
    pfcands.clear();
    pfcands.reserve(pt->size());
    for(unsigned int iL = 0; iL < pt->size(); ++iL){
      pfcands.emplace_back(CylLorentzVectorF(pt->at(iL),eta->at(iL),phi->at(iL),mass->at(iL)),iL);
      pfcands.back().setPdgId(pdgid->at(iL));
      pfcands.back().setCharge(q->at(iL));
      pfcands.back().setD0(d0->at(iL));
      pfcands.back().setDz(dz->at(iL));
      pfcands.back().setMt(mt->at(iL));
      pfcands.back().setTauDisc(taudisc->at(iL));
      pfcands.back().setFromPV(fromPV->at(iL));
      pfcands.back().setJetIndex(jetIndex->at(iL));
      pfcands.back().setTauIndex(tauIndex->at(iL));
      pfcands.back().setIsMVAVetoTau(taudisc->at(iL) > defaults::TAU_MVA_VETO);
    }
  }
}

