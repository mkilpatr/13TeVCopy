//--------------------------------------------------------------------------------------------------
// 
// prodIsoTrksReader
// 
// Class for reading prodIsoTrks object information from TTree.
// 
// prodIsoTrksReader.cc created on Wed Oct 22 01:39:44 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisTools/TreeReader/interface/prodIsoTrksReader.h"
#include "AnalysisTools/TreeReader/interface/TreeReader.h"

using namespace std;
using namespace ucsbsusy;

const int prodIsoTrksReader::defaultOptions = prodIsoTrksReader::LOADRECO | prodIsoTrksReader::FILLOBJ;

//--------------------------------------------------------------------------------------------------
prodIsoTrksReader::prodIsoTrksReader() : BaseReader(){
  looseIsoTrks_pt           = new vector<float>;
  looseIsoTrks_eta          = new vector<float>;
  looseIsoTrks_phi          = new vector<float>;
  looseIsoTrks_mass         = new vector<float>;
  looseIsoTrks_charge       = new vector<double>;
  looseIsoTrks_dz           = new vector<double>;
  looseIsoTrks_pdgId        = new vector<int>   ;
  //looseIsoTrks_idx          = new vector<int>   ;
  looseIsoTrks_iso          = new vector<double>;
  looseIsoTrks_mtw          = new vector<double>;
  //looseIsoTrks_pfActivity   = new vector<double>;
  //forVetoIsoTrks_idx        = new vector<int>   ;
  //loosenIsoTrks		    = new vector<unsigned int>   ;
  //nIsoTrksForVeto	    = new vector<unsigned int>   ;
  loosenIsoTrks		    = 0;
  nIsoTrksForVeto	    = 0;

}

//--------------------------------------------------------------------------------------------------
void prodIsoTrksReader::load(TreeReader *treeReader, int options, string branchName)
{
    loaded_ = true;
    const_cast<int&>(options_)    = options;
    const_cast<string&>(branchName_) = branchName;

    clog << "Loading (" << branchName << ") prodisotrks with: ";

  if(options_ & LOADRECO){
    clog << "reco ";
    treeReader->setBranchAddress(branchName , "looseIsoTrks_pt"          , &looseIsoTrks_pt          , true);
    treeReader->setBranchAddress(branchName , "looseIsoTrks_eta"         , &looseIsoTrks_eta         , true);
    treeReader->setBranchAddress(branchName , "looseIsoTrks_phi"         , &looseIsoTrks_phi         , true);
    treeReader->setBranchAddress(branchName , "looseIsoTrks_mass"        , &looseIsoTrks_mass        , true);
    treeReader->setBranchAddress(branchName , "looseIsoTrks_charge"      , &looseIsoTrks_charge      , true);
    treeReader->setBranchAddress(branchName , "looseIsoTrks_dz"          , &looseIsoTrks_dz          , true);
    treeReader->setBranchAddress(branchName , "looseIsoTrks_pdgId"       , &looseIsoTrks_pdgId       , true);
    //treeReader->setBranchAddress(branchName , "looseIsoTrks_idx  "       , &looseIsoTrks_idx         , true);
    treeReader->setBranchAddress(branchName , "looseIsoTrks_iso"         , &looseIsoTrks_iso         , true);
    treeReader->setBranchAddress(branchName , "looseIsoTrks_mtw"         , &looseIsoTrks_mtw         , true);
    //treeReader->setBranchAddress(branchName , "looseIsoTrks_pfActivity"  , &looseIsoTrks_pfActivity  , true);
    //treeReader->setBranchAddress(branchName , "forVetoIsoTrks_idx"       , &forVetoIsoTrks_idx       , true);
    treeReader->setBranchAddress(branchName , "loosenIsoTrks"            , &loosenIsoTrks            , true);
    treeReader->setBranchAddress(branchName , "nIsoTrksForVeto"          , &nIsoTrksForVeto          , true);
  }
  if(options_ & FILLOBJ)
    clog << "+Objects";
  clog << endl;
}

//--------------------------------------------------------------------------------------------------
void prodIsoTrksReader::refresh(){
  if(!(options_ & FILLOBJ)) return;

  //if(options_ & LOADRECO){
    prodisotrks.clear();
    prodisotrks.reserve(looseIsoTrks_pt->size());
    for(unsigned int iL = 0; iL < looseIsoTrks_pt->size(); ++iL){
      prodisotrks.emplace_back(CylLorentzVectorF(looseIsoTrks_pt->at(iL),looseIsoTrks_eta->at(iL),looseIsoTrks_phi->at(iL),looseIsoTrks_mass->at(iL)),iL);
      prodisotrks.back().setCharge(looseIsoTrks_charge->at(iL));
      prodisotrks.back().setDz(looseIsoTrks_dz->at(iL));
      prodisotrks.back().setPdgId(looseIsoTrks_pdgId->at(iL));
      //prodisotrks.back().setIdx(looseIsoTrks_idx->at(iL));
      prodisotrks.back().setIso(looseIsoTrks_iso->at(iL));
      prodisotrks.back().setMtw(looseIsoTrks_mtw->at(iL));
      //prodisotrks.back().setPfActivity(looseIsoTrks_pfActivity->at(iL));
      //prodisotrks.back().setVetoIsoTrks(forVetoIsoTrks_idx->at(iL));
      prodisotrks.back().setNLooseIsoTrks(loosenIsoTrks);
      prodisotrks.back().setTrksForVeto(nIsoTrksForVeto);
    }
  //}
}

