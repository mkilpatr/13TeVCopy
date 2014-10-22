//--------------------------------------------------------------------------------------------------
// 
// TauReader
// 
// Class for reading electron object information from TTree.
// 
// TauReader.cc created on Wed Oct 22 23:13:49 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisTools/TreeReader/interface/TauReader.h"

using namespace ucsbsusy;

bool TauReader::initTree()
{

  bool baseinit = CollectionReader::initTree();

  string prefix = "tau_";

  leadcandpt = new vector<float>(1, 0.0);
  leadcandeta = new vector<float>(1, 0.0);
  leadcandphi = new vector<float>(1, 0.0);
  leadcandmass = new vector<float>(1, 0.0);

  leadchcandpt = new vector<float>(1, 0.0);
  leadchcandeta = new vector<float>(1, 0.0);
  leadchcandphi = new vector<float>(1, 0.0);
  leadchcandmass = new vector<float>(1, 0.0);

  q = new vector<int>(1, 0.0);
  dxy = new vector<float>(1, 0.0);
  dxyerr = new vector<float>(1, 0.0);
  dxysig = new vector<float>(1, 0.0);
  hpsid = new vector<unsigned long>(1, 0);

  getTree()->SetBranchAddress((prefix + "leadcand_pt").c_str(), &leadcandpt);
  getTree()->SetBranchAddress((prefix + "leadcand_eta").c_str(), &leadcandeta);
  getTree()->SetBranchAddress((prefix + "leadcand_phi").c_str(), &leadcandphi);
  getTree()->SetBranchAddress((prefix + "leadcand_mass").c_str(), &leadcandmass);

  getTree()->SetBranchAddress((prefix + "leadchargedcand_pt").c_str(), &leadchcandpt);
  getTree()->SetBranchAddress((prefix + "leadchargedcand_eta").c_str(), &leadchcandeta);
  getTree()->SetBranchAddress((prefix + "leadchargedcand_phi").c_str(), &leadchcandphi);
  getTree()->SetBranchAddress((prefix + "leadchargedcand_mass").c_str(), &leadchcandmass);

  getTree()->SetBranchAddress((prefix + "q").c_str(), &q);
  getTree()->SetBranchAddress((prefix + "dxy").c_str(), &dxy);
  getTree()->SetBranchAddress((prefix + "dxyerr").c_str(), &dxyerr);
  getTree()->SetBranchAddress((prefix + "dxysig").c_str(), &dxysig);
  getTree()->SetBranchAddress((prefix + "idflags").c_str(), &hpsid);

  return baseinit;

}

TauFCollection TauReader::getTaus()
{

  int ntaus = pt->size();

  taus.clear();
  taus.reserve(ntaus);

  for(int itau = 0; itau < ntaus; itau++) {
    CylLorentzVectorF mom(pt->at(itau), eta->at(itau), phi->at(itau), mass->at(itau));
    CylLorentzVectorF leadcandmom(leadcandpt->at(itau), leadcandeta->at(itau), leadcandphi->at(itau), leadcandmass->at(itau));
    CylLorentzVectorF leadchcandmom(leadchcandpt->at(itau), leadchcandeta->at(itau), leadchcandphi->at(itau), leadchcandmass->at(itau));

    TauF tau(mom, itau, new MomentumF(leadcandmom), new MomentumF(leadchcandmom));

    tau.setCharge(q->at(itau));
    tau.setDxy(dxy->at(itau));
    tau.setDxyErr(dxyerr->at(itau));
    tau.setDxySig(dxysig->at(itau));
    tau.setHPSId(hpsid->at(itau));

    taus.push_back(tau);
  }

  return taus;

}

