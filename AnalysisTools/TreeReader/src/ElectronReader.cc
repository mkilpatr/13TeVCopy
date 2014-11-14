//--------------------------------------------------------------------------------------------------
// 
// ElectronReader
// 
// Class for reading electron object information from TTree.
// 
// ElectronReader.cc created on Wed Oct 22 01:39:44 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisTools/TreeReader/interface/ElectronReader.h"
#include "AnalysisTools/TreeReader/interface/TreeReader.h"

using namespace std;
using namespace ucsbsusy;

const int ElectronReader::defaultOptions = ElectronReader::LOADRECO | ElectronReader::FILLOBJ;

//--------------------------------------------------------------------------------------------------
ElectronReader::ElectronReader() : BaseReader(){
  pt           = new vector<float>;
  eta          = new vector<float>;
  phi          = new vector<float>;
  mass         = new vector<float>;
  q            = new vector<int>   ;
  scEta        = new vector<float> ;
  r9           = new vector<float> ;
  d0           = new vector<float> ;
  dz           = new vector<float> ;
  pfdbetaiso   = new vector<float> ;
  mvaidnontrig = new vector<float> ;
  mvaidtrig    = new vector<float> ;
  isveto       = new vector<bool>  ;
  isloose      = new vector<bool>  ;
  ismedium     = new vector<bool>  ;
  istight      = new vector<bool>  ;
  eleId        = new LeptonId();
}

//--------------------------------------------------------------------------------------------------
void ElectronReader::load(TreeReader *treeReader, int options, string branchName)
{
    loaded_ = true;
    const_cast<int&>(options_)    = options;
    const_cast<string&>(branchName_) = branchName;

    clog << "Loading (" << branchName << ") electrons with: ";

  if(options_ & LOADRECO){
    clog << "reco ";
    treeReader->setBranchAddress(branchName , "pt"          , &pt          , true);
    treeReader->setBranchAddress(branchName , "eta"         , &eta         , true);
    treeReader->setBranchAddress(branchName , "phi"         , &phi         , true);
    treeReader->setBranchAddress(branchName , "mass"        , &mass        , true);
    treeReader->setBranchAddress(branchName , "q"           , &q           , true);
    treeReader->setBranchAddress(branchName , "SCeta"       , &scEta       , true);
    treeReader->setBranchAddress(branchName , "r9"          , &r9          , true);
    treeReader->setBranchAddress(branchName , "d0"          , &d0          , true);
    treeReader->setBranchAddress(branchName , "dz"          , &dz          , true);
    treeReader->setBranchAddress(branchName , "pfdbetaiso"  , &pfdbetaiso  , true);
    treeReader->setBranchAddress(branchName , "mvaidnontrig", &mvaidnontrig, true);
    treeReader->setBranchAddress(branchName , "mvaidtrig"   , &mvaidtrig   , true);
    treeReader->setBranchAddress(branchName , "vetoid"      , &isveto      , true);
    treeReader->setBranchAddress(branchName , "looseid"     , &isloose     , true);
    treeReader->setBranchAddress(branchName , "mediumid"    , &ismedium    , true);
    treeReader->setBranchAddress(branchName , "tightid"     , &istight     , true);
  }
  if(options_ & FILLOBJ)
    clog << "+Objects";
  clog << endl;
}

//--------------------------------------------------------------------------------------------------
void ElectronReader::refresh(){
  if(!(options_ & FILLOBJ)) return;

  if(options_ & LOADRECO){
    electrons.clear();
    electrons.reserve(pt->size());
    for(unsigned int iL = 0; iL < pt->size(); ++iL){
      electrons.emplace_back(CylLorentzVectorF(pt->at(iL),eta->at(iL),phi->at(iL),mass->at(iL)),iL);
      electrons.back().setCharge(q->at(iL));
      electrons.back().setSCEta(scEta->at(iL));
      electrons.back().setR9(r9->at(iL));
      electrons.back().setD0(d0->at(iL));
      electrons.back().setDz(dz->at(iL));
      electrons.back().setPFDBetaIso(pfdbetaiso->at(iL));
      electrons.back().setMVAIDNonTrig(mvaidnontrig->at(iL));
      electrons.back().setMVAIDTrig(mvaidtrig->at(iL));
      electrons.back().setIsVeto(isveto->at(iL));
      electrons.back().setIsLoose(isloose->at(iL));
      electrons.back().setIsMedium(ismedium->at(iL));
      electrons.back().setIsTight(istight->at(iL));
      electrons.back().setIsGoodPOGElectron(eleId->passElectronId((&electrons.back())));
    }
  }
}

