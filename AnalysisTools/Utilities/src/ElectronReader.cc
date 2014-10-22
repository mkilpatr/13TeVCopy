//--------------------------------------------------------------------------------------------------
// 
// ElectronReader
// 
// Class for reading electron object information from TTree.
// 
// ElectronReader.cc created on Wed Oct 22 01:39:44 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisTools/Utilities/interface/ElectronReader.h"

using namespace ucsbsusy;

bool ElectronReader::initTree()
{

  bool baseinit = CollectionReader::initTree();

  string prefix = "ele_";

  scEta = new vector<float>(1, 0.0);
  r9 = new vector<float>(1, 0.0);
  d0 = new vector<float>(1, 0.0);
  dz = new vector<float>(1, 0.0);
  pfdbetaiso = new vector<float>(1, 0.0);
  mvaidnontrig = new vector<float>(1, 0.0);
  mvaidtrig = new vector<float>(1, 0.0);
  getTree()->SetBranchAddress((prefix + "SCeta").c_str(), &scEta);
  getTree()->SetBranchAddress((prefix + "r9").c_str(), &r9);
  getTree()->SetBranchAddress((prefix + "d0").c_str(), &d0);
  getTree()->SetBranchAddress((prefix + "dz").c_str(), &dz);
  getTree()->SetBranchAddress((prefix + "pfdbetaiso").c_str(), &pfdbetaiso);
  getTree()->SetBranchAddress((prefix + "mvaidnontrig").c_str(), &mvaidnontrig);
  getTree()->SetBranchAddress((prefix + "mvaidtrig").c_str(), &mvaidtrig);

  return baseinit;

}

ElectronFCollection ElectronReader::getElectrons()
{

  int nelectrons = pt->size();

  electrons.clear();
  electrons.reserve(nelectrons);

  for(int iel = 0; iel < nelectrons; iel++) {
    ROOT::Math::LorentzVector<CylLorentzCoordF> mom(pt->at(iel), eta->at(iel), phi->at(iel), mass->at(iel));
    ElectronF ele(mom, iel);
    ele.setSCEta(scEta->at(iel));
    ele.setR9(r9->at(iel));
    ele.setD0(d0->at(iel));
    ele.setDz(dz->at(iel));
    ele.setPFDBetaIso(pfdbetaiso->at(iel));
    ele.setMVAIDNonTrig(mvaidnontrig->at(iel));
    ele.setMVAIDTrig(mvaidtrig->at(iel));
    electrons.push_back(ele);
  }

  return electrons;

}

