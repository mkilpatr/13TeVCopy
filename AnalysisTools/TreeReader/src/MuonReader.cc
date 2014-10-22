//--------------------------------------------------------------------------------------------------
// 
// MuonReader
// 
// Class for reading muon object information from TTree.
// 
// MuonReader.cc created on Wed Oct 22 01:39:44 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisTools/TreeReader/interface/MuonReader.h"

using namespace ucsbsusy;

bool MuonReader::initTree()
{

  bool baseinit = CollectionReader::initTree();

  string prefix = "mu_";

  q = new vector<int>(1, 0.0);
  d0 = new vector<float>(1, 0.0);
  dz = new vector<float>(1, 0.0);
  pfdbetaiso = new vector<float>(1, 0.0);
  isloose = new vector<bool>(1, false);
  istight = new vector<bool>(1, false);
  ispf = new vector<bool>(1, false);
  isglobal = new vector<bool>(1, false);
  istracker = new vector<bool>(1, false);
  isstandalone = new vector<bool>(1, false);
  getTree()->SetBranchAddress((prefix + "q").c_str(), &q);
  getTree()->SetBranchAddress((prefix + "d0").c_str(), &d0);
  getTree()->SetBranchAddress((prefix + "dz").c_str(), &dz);
  getTree()->SetBranchAddress((prefix + "pfdbetaiso").c_str(), &pfdbetaiso);
  getTree()->SetBranchAddress((prefix + "isLoose").c_str(), &isloose);
  getTree()->SetBranchAddress((prefix + "isTight").c_str(), &istight);
  getTree()->SetBranchAddress((prefix + "isPF").c_str(), &ispf);
  getTree()->SetBranchAddress((prefix + "isGlobal").c_str(), &isglobal);
  getTree()->SetBranchAddress((prefix + "isTracker").c_str(), &istracker);
  getTree()->SetBranchAddress((prefix + "isStandAlone").c_str(), &isstandalone);

  return baseinit;

}

MuonFCollection MuonReader::getMuons()
{

  int nmuons = pt->size();

  muons.clear();
  muons.reserve(nmuons);

  for(int imu = 0; imu < nmuons; imu++) {
    CylLorentzVectorF mom(pt->at(imu), eta->at(imu), phi->at(imu), mass->at(imu));

    MuonF mu(mom, imu);

    mu.setCharge(q->at(imu));
    mu.setD0(d0->at(imu));
    mu.setDz(dz->at(imu));
    mu.setPFDBetaIso(pfdbetaiso->at(imu));
    mu.setIsLoose(isloose->at(imu));
    mu.setIsTight(istight->at(imu));
    mu.setIsPF(ispf->at(imu));
    mu.setIsGlobal(isglobal->at(imu));
    mu.setIsTracker(istracker->at(imu));
    mu.setIsStandalone(isstandalone->at(imu));

    muons.push_back(mu);
  }

  return muons;

}

