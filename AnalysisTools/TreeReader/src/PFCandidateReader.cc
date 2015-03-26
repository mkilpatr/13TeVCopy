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

const int PFCandidateReader::defaultOptions = PFCandidateReader::LOADRECO | PFCandidateReader::FILLOBJ | PFCandidateReader::LOADTAUVETOMT;

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
  dphimet      = new vector<float> ;
  taudisc      = new vector<float> ;
  fromPV       = new vector<int>   ;
  jetIndex     = new vector<int>   ;
  tauIndex     = new vector<int>   ;
  chiso0p1     = new vector<float> ;
  chiso0p2     = new vector<float> ;
  chiso0p3     = new vector<float> ;
  chiso0p4     = new vector<float> ;
  totiso0p1    = new vector<float> ;
  totiso0p2    = new vector<float> ;
  totiso0p3    = new vector<float> ;
  totiso0p4    = new vector<float> ;
  neartrkdr    = new vector<float> ;
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
    treeReader->setBranchAddress(branchName , "mttrkplusphoton", &mt       , true);
    treeReader->setBranchAddress(branchName , "dphimet"     , &dphimet       , true);
    if(options_ & LOADTAUVETOMT)
      treeReader->setBranchAddress(branchName , "taudisc_mtpresel", &taudisc , true);
    else if(options_ & LOADTAUVETODPHI)
      treeReader->setBranchAddress(branchName , "taudisc_dphipresel", &taudisc , true);
    treeReader->setBranchAddress(branchName , "fromPV"      , &fromPV      , true);
    treeReader->setBranchAddress(branchName , "contJetIndex", &jetIndex    , true);
    treeReader->setBranchAddress(branchName , "contTauIndex", &tauIndex    , true);
    if(options_ & LOADEXTRECO){
      clog << " + extra info ";
      treeReader->setBranchAddress(branchName , "chiso0p1"     , &chiso0p1    , true);
      treeReader->setBranchAddress(branchName , "chiso0p2"     , &chiso0p2    , true);
      treeReader->setBranchAddress(branchName , "chiso0p3"     , &chiso0p3    , true);
      treeReader->setBranchAddress(branchName , "chiso0p4"     , &chiso0p4    , true);
      treeReader->setBranchAddress(branchName , "totiso0p1"    , &totiso0p1   , true);
      treeReader->setBranchAddress(branchName , "totiso0p2"    , &totiso0p2   , true);
      treeReader->setBranchAddress(branchName , "totiso0p3"    , &totiso0p3   , true);
      treeReader->setBranchAddress(branchName , "totiso0p4"    , &totiso0p4   , true);
      treeReader->setBranchAddress(branchName , "nearestTrkDR" , &neartrkdr   , true);
    }
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
      pfcands.back().setDphiMet(dphimet->at(iL));
      pfcands.back().setTauDisc(taudisc->at(iL));
      pfcands.back().setFromPV(fromPV->at(iL));
      pfcands.back().setJetIndex(jetIndex->at(iL));
      pfcands.back().setTauIndex(tauIndex->at(iL));
      if(options_ & LOADTAUVETOMT)
        pfcands.back().setIsMVAVetoTau(taudisc->at(iL) > defaults::TAU_MVA_VETO_MTPRESEL_MEDIUM && mt->at(iL) < defaults::TAU_MTCUT_VETO);
      else if(options_ & LOADTAUVETODPHI)
        pfcands.back().setIsMVAVetoTau(taudisc->at(iL) > defaults::TAU_MVA_VETO_DPHIPRESEL_MEDIUM && fabs(dphimet->at(iL)) < defaults::TAU_DPHICUT_VETO);
    }
  }
  if(options_ & LOADEXTRECO){
    extpfcands.clear();
    extpfcands.reserve(pt->size());
    for(unsigned int iL = 0; iL < pt->size(); ++iL){
      ExtendedPFCandidate cand;
      cand.setP4(CylLorentzVectorF(pt->at(iL),eta->at(iL),phi->at(iL),mass->at(iL)));
      cand.setIndex(iL);
      cand.setPdgId(pdgid->at(iL));
      cand.setCharge(q->at(iL));
      cand.setD0(d0->at(iL));
      cand.setDz(dz->at(iL));
      cand.setMt(mt->at(iL));
      cand.setDphiMet(dphimet->at(iL));
      cand.setTauDisc(taudisc->at(iL));
      cand.setFromPV(fromPV->at(iL));
      cand.setJetIndex(jetIndex->at(iL));
      cand.setTauIndex(tauIndex->at(iL));
      if(options_ & LOADTAUVETOMT)
        cand.setIsMVAVetoTau(taudisc->at(iL) > defaults::TAU_MVA_VETO_MTPRESEL_MEDIUM && mt->at(iL) < defaults::TAU_MTCUT_VETO);
      else if(options_ & LOADTAUVETODPHI)
        cand.setIsMVAVetoTau(taudisc->at(iL) > defaults::TAU_MVA_VETO_DPHIPRESEL_MEDIUM && fabs(dphimet->at(iL)) < defaults::TAU_DPHICUT_VETO);
      cand.setChIso0p1(chiso0p1->at(iL));
      cand.setChIso0p2(chiso0p2->at(iL));
      cand.setChIso0p3(chiso0p3->at(iL));
      cand.setChIso0p4(chiso0p4->at(iL));
      cand.setTotIso0p1(totiso0p1->at(iL));
      cand.setTotIso0p2(totiso0p2->at(iL));
      cand.setTotIso0p3(totiso0p3->at(iL));
      cand.setTotIso0p4(totiso0p4->at(iL));
      cand.setNearTrkDR(neartrkdr->at(iL));
      extpfcands.push_back(cand);
    }
  }
}

