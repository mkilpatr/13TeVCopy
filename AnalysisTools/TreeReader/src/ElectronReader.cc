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
    const_cast<int&>(options_)    = options;
    const_cast<string&>(branchName_) = branchName;

    clog << "Loading (" << branchName << ") electrons with: ";

  if(options_ & LOADRECO){
    clog << "reco ";
    treeReader->setBranchAddress((branchName + "_pt"          ).c_str(), &pt          , true);
    treeReader->setBranchAddress((branchName + "_eta"         ).c_str(), &eta         , true);
    treeReader->setBranchAddress((branchName + "_phi"         ).c_str(), &phi         , true);
    treeReader->setBranchAddress((branchName + "_mass"        ).c_str(), &mass        , true);
    treeReader->setBranchAddress((branchName + "_q"           ).c_str(), &q           , true);
    treeReader->setBranchAddress((branchName + "_SCeta"       ).c_str(), &scEta       , true);
    treeReader->setBranchAddress((branchName + "_r9"          ).c_str(), &r9          , true);
    treeReader->setBranchAddress((branchName + "_d0"          ).c_str(), &d0          , true);
    treeReader->setBranchAddress((branchName + "_dz"          ).c_str(), &dz          , true);
    treeReader->setBranchAddress((branchName + "_pfdbetaiso"  ).c_str(), &pfdbetaiso  , true);
    treeReader->setBranchAddress((branchName + "_mvaidnontrig").c_str(), &mvaidnontrig, true);
    treeReader->setBranchAddress((branchName + "_mvaidtrig"   ).c_str(), &mvaidtrig   , true);
    treeReader->setBranchAddress((branchName + "_vetoid"      ).c_str(), &isveto      , true);
    treeReader->setBranchAddress((branchName + "_looseid"     ).c_str(), &isloose     , true);
    treeReader->setBranchAddress((branchName + "_mediumid"    ).c_str(), &ismedium    , true);
    treeReader->setBranchAddress((branchName + "_tightid"     ).c_str(), &istight     , true);
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

