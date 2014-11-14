//--------------------------------------------------------------------------------------------------
// 
// JetReader
// 
// Class for reading electron object information from TTree.
// 
// ElectronReader.cc created on Wed Oct 22 01:39:44 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisTools/TreeReader/interface/GenParticleReader.h"
#include "AnalysisTools/TreeReader/interface/TreeReader.h"

using namespace std;
using namespace ucsbsusy;

const int GenParticleReader::defaultOptions = GenParticleReader::FILLOBJ;

//--------------------------------------------------------------------------------------------------
GenParticleReader::DataContainer::DataContainer() {
  pt     = new std::vector<float>;
  eta    = new std::vector<float>;
  phi    = new std::vector<float>;
  mass   = new std::vector<float>;
  status = new std::vector<int  >;
  pdgid  = new std::vector<int  >;
}
//--------------------------------------------------------------------------------------------------
GenParticleReader::DataContainer::~DataContainer() {
  delete pt    ;
  delete eta   ;
  delete phi   ;
  delete mass  ;
  delete status;
  delete pdgid ;
}

//--------------------------------------------------------------------------------------------------
void GenParticleReader::DataContainer::load(const std::string branchName,const std::string collectionName, TreeReader * treeReader){
  std::clog << collectionName <<" ";
  treeReader->setBranchAddress(branchName,collectionName + "_pt"      , &pt    ,true);
  treeReader->setBranchAddress(branchName,collectionName + "_eta"     , &eta   ,true);
  treeReader->setBranchAddress(branchName,collectionName + "_phi"     , &phi   ,true);
  treeReader->setBranchAddress(branchName,collectionName + "_mass"    , &mass  ,true);
  treeReader->setBranchAddress(branchName,collectionName + "_stats"   , &status,true);
  treeReader->setBranchAddress(branchName,collectionName + "_pdgid"   , &pdgid ,true);
}
//--------------------------------------------------------------------------------------------------
void GenParticleReader::DataContainer::refresh(){
  particles.clear();
  particles.reserve(pt->size());
  for(unsigned int iJ = 0; iJ < pt->size(); ++iJ)
    particles.emplace_back(CylLorentzVectorF(pt->at(iJ),pt->at(iJ),pt->at(iJ),pt->at(iJ)),iJ,
        status->at(iJ),pdgid->at(iJ)
    );
}
//--------------------------------------------------------------------------------------------------
GenParticleReader::GenParticleReader() : BaseReader(){
}

//--------------------------------------------------------------------------------------------------
void GenParticleReader::load(TreeReader *treeReader, int options, string branchName)
{
    loaded_ = true;
    const_cast<int&>(options_)    = options;
    const_cast<string&>(branchName_) = branchName;

    clog << "Loading (" << branchName << ") particles with: ";

    genParticles   .load(branchName_,"particle",treeReader);
    thirdGenQuarks .load(branchName_,"q3rdgen",treeReader);
    bosons         .load(branchName_,"boson",treeReader);
    bosonDaughters .load(branchName_,"boson_daughter",treeReader);
    tauDaughters   .load(branchName_,"tau_daughter",treeReader);

  if(options_ & FILLOBJ)
    clog << "+Objects";
  clog << endl;
}

//--------------------------------------------------------------------------------------------------
void GenParticleReader::refresh(){
  if(!(options_ & FILLOBJ)) return;

  genParticles   .refresh();
  thirdGenQuarks .refresh();
  bosons         .refresh();
  bosonDaughters .refresh();
  tauDaughters   .refresh();
}
