//--------------------------------------------------------------------------------------------------
//
// EventInfoReader
//
// Class for reading event level information from TTree.
//
// EventInfoReader.cc created on Mon Nov 3 12:56:14 CET 2014
//
//--------------------------------------------------------------------------------------------------

#include "AnalysisTools/TreeReader/interface/EventInfoReader.h"
#include "AnalysisTools/TreeReader/interface/TreeReader.h"

using namespace std;
using namespace ucsbsusy;

EventInfoReader::EventInfoReader()
{

  run = 0;
  lumi = 0;
  event = 0;
  nPV = 0;
  nPUTrue = 0;
  rho = 0;
  pvx = 0;
  pvy = 0;
  pvz = 0;
  met_pt = 0;
  met_phi = 0;
  metsumEt = 0;
  metNoHF_pt = 0;
  metNoHF_phi = 0;
  metNoHF_sumEt = 0;
  puppimet_pt = 0;
  puppimet_phi = 0;
  puppimetsumEt = 0;
  genmet_pt = 0;
  genmet_phi = 0;
  metunclusterup = 0;
  metunclusterdn = 0;
  goodvertex = false;
  genweight = 1;
  genqscale = 0;
  xsec = 1;
  xsecweight = 1;
  genevtweight = 1;
  evtweight = 1;
  nStandardGenJets = -1;
  lhecentralweight = 1;
  systweights = new vector<float>;
  proc = 0;
  process = defaults::NUMPROCESSES;
  signalType = defaults::NUM_SIGTYPES;
  datrec = 0;
  datareco = defaults::MC;
  metfilterbitpass_old = new vector<bool>;
  metfilterbitpass = 0;
  massparams = new vector<size16>;
  HBHENoiseFilter                      = false;
  HBHENoiseIsoFilter                   = false;
  globalTightHalo2016Filter            = false;
  EcalDeadCellTriggerPrimitiveFilter   = false;
  goodVertices                         = false;
  eeBadScFilter                        = false;
  badChCand                            = false;
  badPFMuon                            = false;
  massPar1 = 0;
  massPar2 = 0;
  massPar3 = 0;

}

void EventInfoReader::load(TreeReader *treeReader, int options, string branchName)
{
  clog << "Loading (" << branchName << ") event info with: ";

  loaded_ = true;
  treeReader->setBranchAddress(branchName,"run", &run);
  treeReader->setBranchAddress(branchName,"lumi", &lumi);
  treeReader->setBranchAddress(branchName,"event", &event);
  treeReader->setBranchAddress(branchName,"npv", &nPV);
  treeReader->setBranchAddress(branchName,"nputrue", &nPUTrue);
  treeReader->setBranchAddress(branchName,"rho", &rho);
  treeReader->setBranchAddress(branchName,"pv_x", &pvx);
  treeReader->setBranchAddress(branchName,"pv_y", &pvy);
  treeReader->setBranchAddress(branchName,"pv_z", &pvz);
  treeReader->setBranchAddress(branchName,"met_pt", &met_pt);
  treeReader->setBranchAddress(branchName,"met_phi", &met_phi);
  treeReader->setBranchAddress(branchName,"met_sumEt", &metsumEt);
  treeReader->setBranchAddress(branchName,"metnohf_pt", &metNoHF_pt);
  treeReader->setBranchAddress(branchName,"metnohf_phi", &metNoHF_phi);
  treeReader->setBranchAddress(branchName,"metnohf_sumEt", &metNoHF_sumEt);
  treeReader->setBranchAddress(branchName,"puppimet_pt", &puppimet_pt);
  treeReader->setBranchAddress(branchName,"puppimet_phi", &puppimet_phi);
  treeReader->setBranchAddress(branchName,"puppimet_sumEt", &puppimetsumEt);
  treeReader->setBranchAddress(branchName,"genmet_pt", &genmet_pt);
  treeReader->setBranchAddress(branchName,"genmet_phi", &genmet_phi);
  treeReader->setBranchAddress(branchName,"metunclustrun2up_pt", &metunclusterup);
  treeReader->setBranchAddress(branchName,"metunclustrun2dn_pt", &metunclusterdn);
  treeReader->setBranchAddress(branchName,"goodvertex", &goodvertex);
  treeReader->setBranchAddress(branchName,"genweight", &genweight);
  treeReader->setBranchAddress(branchName,"genqscale", &genqscale);
  treeReader->setBranchAddress(branchName,"massparams", &massparams);
  treeReader->setBranchAddress(branchName,"process", &proc);
  treeReader->setBranchAddress(branchName,"datareco", &datrec);
  treeReader->setBranchAddress(branchName,"xsection", &xsec);
  treeReader->setBranchAddress(branchName,"wgtXSec", &xsecweight);
  treeReader->setBranchAddress(branchName,"evtWgtGen", &genevtweight);
  treeReader->setBranchAddress(branchName,"nstdgenjets", &nStandardGenJets);
  treeReader->setBranchAddress(branchName,"lhecentralweight", &lhecentralweight);
  treeReader->setBranchAddress(branchName,"systweights", &systweights);
  treeReader->setBranchAddress(defaults::BRANCH_METFILTERS,"bit_pass", &metfilterbitpass_old);
  treeReader->setBranchAddress(defaults::BRANCH_METFILTERS,"int_bit_pass", &metfilterbitpass);
  treeReader->setBranchAddress(defaults::BRANCH_METFILTERS,"badchcand", &badChCand);
  treeReader->setBranchAddress(defaults::BRANCH_METFILTERS,"badpfmuon", &badPFMuon);

  clog << endl;
}

void EventInfoReader::refresh()
{
  met.setP4(CylLorentzVectorF(met_pt,0,met_phi,0));
  metNoHF.setP4(CylLorentzVectorF(metNoHF_pt,0,metNoHF_phi,0));
  puppimet.setP4(CylLorentzVectorF(puppimet_pt,0,puppimet_phi,0));
  genmet.setP4(CylLorentzVectorF(genmet_pt,0,genmet_phi,0));
  process = static_cast<defaults::Process>(proc);
  datareco = static_cast<defaults::DataReco>(datrec);
  evtweight = xsecweight * genevtweight;

  //Hack to get signal type from filename (sorry) until we integrat into weight code
  if(process != defaults::SIGNAL){
    signalType = defaults::NUM_SIGTYPES;
  }


  //// HACK for old and new version of met filters
  if(metfilterbitpass_old->size()){
    HBHENoiseFilter                    = metfilterbitpass_old->size() ? metfilterbitpass_old->at(fFlag_idx_HBHENoiseFilter                   ) : true;
    HBHENoiseIsoFilter                 = metfilterbitpass_old->size() ? metfilterbitpass_old->at(fFlag_idx_HBHENoiseIsoFilter                ) : true;
    globalTightHalo2016Filter          = metfilterbitpass_old->size() ? metfilterbitpass_old->at(fFlag_idx_globalTightHalo2016Filter         ) : true;
    EcalDeadCellTriggerPrimitiveFilter = metfilterbitpass_old->size() ? metfilterbitpass_old->at(fFlag_idx_EcalDeadCellTriggerPrimitiveFilter) : true;
    goodVertices                       = metfilterbitpass_old->size() ? metfilterbitpass_old->at(fFlag_idx_goodVertices                      ) : true;
    eeBadScFilter                      = metfilterbitpass_old->size() ? metfilterbitpass_old->at(fFlag_idx_eeBadScFilter                     ) : true;
  } else {
    HBHENoiseFilter                    = metfilterbitpass & fFlag_HBHENoiseFilter                   ;
    HBHENoiseIsoFilter                 = metfilterbitpass & fFlag_HBHENoiseIsoFilter                ;
    globalTightHalo2016Filter          = metfilterbitpass & fFlag_globalTightHalo2016Filter         ;
    EcalDeadCellTriggerPrimitiveFilter = metfilterbitpass & fFlag_EcalDeadCellTriggerPrimitiveFilter;
    goodVertices                       = metfilterbitpass & fFlag_goodVertices                      ;
    eeBadScFilter                      = metfilterbitpass & fFlag_eeBadScFilter                     ;
  }



  if(massparams->size() > 0)
    massPar1 = massparams->at(0);
  if(massparams->size() > 1)
    massPar2 = massparams->at(1);
  if(massparams->size() > 2)
    massPar3 = massparams->at(2);

}

void EventInfoReader::pushToTree() {
  met_pt = met.pt();
  met_phi = met.phi();

  if(genevtweight)
    xsecweight = evtweight/genevtweight;
}
