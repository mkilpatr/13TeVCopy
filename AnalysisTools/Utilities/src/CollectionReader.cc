//--------------------------------------------------------------------------------------------------
// 
// CollectionReader
// 
// Class to help with reading collections of objects from TTrees.
// 
// CollectionReader.cc created on Tue Oct 21 15:33:32 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisTools/Utilities/interface/CollectionReader.h"

using namespace ucsbsusy;

bool CollectionReader::initTree()
{

  string prefix;
  if(fObjType == kAK4Jet) prefix = "ak4pfchs_jet_";
  else if(fObjType == kElectron) prefix = "ele_";
  else if(fObjType == kMuon) prefix = "mu_";
  else {
    cout << "object type " << fObjType << " not known!" << endl;
    return false;
  }

  pt = new vector<float>(1, 0.0);
  eta = new vector<float>(1, 0.0);
  phi = new vector<float>(1, 0.0);
  mass = new vector<float>(1, 0.0);

  fTree->SetBranchAddress((prefix + "pt").c_str(), &pt);
  fTree->SetBranchAddress((prefix + "eta").c_str(), &eta);
  fTree->SetBranchAddress((prefix + "phi").c_str(), &phi);
  fTree->SetBranchAddress((prefix + "mass").c_str(), &mass);

  return true;

}

MomentumFCollection CollectionReader::getMomenta()
{

  int nobjects = pt->size();
  if(eta->size() != pt->size()) cout << "unequal number of entries for eta and pt vectors!" << endl;
  if(phi->size() != pt->size()) cout << "unequal number of entries for phi and pt vectors!" << endl;
  if(mass->size() != pt->size()) cout << "unequal number of entries for mass and pt vectors!" << endl;

  momenta.clear();
  momenta.reserve(nobjects);

  for(int iobj = 0; iobj < nobjects; iobj++) {
    ROOT::Math::LorentzVector<CylLorentzCoordF> mom(pt->at(iobj), eta->at(iobj), phi->at(iobj), mass->at(iobj));
    momenta.emplace_back(mom);
  }

  return momenta;

}
