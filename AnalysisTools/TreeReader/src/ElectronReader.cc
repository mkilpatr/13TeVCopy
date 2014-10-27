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
}

//--------------------------------------------------------------------------------------------------
void ElectronReader::load(TTree *tree, int options, string branchName)
{
    const_cast<int&>(options_)    = options;
    const_cast<string&>(branchName_) = branchName;

  if(options_ & LOADRECO){
    tree->SetBranchAddress((branchName + "_pt"          ).c_str(), &pt  );
    tree->SetBranchAddress((branchName + "_eta"         ).c_str(), &eta );
    tree->SetBranchAddress((branchName + "_phi"         ).c_str(), &phi );
    tree->SetBranchAddress((branchName + "_mass"        ).c_str(), &mass);
    tree->SetBranchAddress((branchName + "_q"           ).c_str(), &q);
    tree->SetBranchAddress((branchName + "_SCeta"       ).c_str(), &scEta);
    tree->SetBranchAddress((branchName + "_r9"          ).c_str(), &r9);
    tree->SetBranchAddress((branchName + "_d0"          ).c_str(), &d0);
    tree->SetBranchAddress((branchName + "_dz"          ).c_str(), &dz);
    tree->SetBranchAddress((branchName + "_pfdbetaiso"  ).c_str(), &pfdbetaiso);
    tree->SetBranchAddress((branchName + "_mvaidnontrig").c_str(), &mvaidnontrig);
    tree->SetBranchAddress((branchName + "_mvaidtrig"   ).c_str(), &mvaidtrig);
  }
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
    }
  }
}

