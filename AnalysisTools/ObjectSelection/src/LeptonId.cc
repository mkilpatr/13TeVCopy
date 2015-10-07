//--------------------------------------------------------------------------------------------------
// 
// LeptonId
// 
// Class for implementing lepton selections.
// 
// LeptonId.cc created on Mon Oct 27 14:08:36 CET 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisTools/ObjectSelection/interface/LeptonId.h"

using namespace ucsbsusy;

LeptonId::LeptonId(bool initElectronIds, int bunchSpacing)
{

  bunchSpacing_ = bunchSpacing;
  eleIdInit_ = false;

  if(initElectronIds) initElectronCuts(bunchSpacing_);

}

LeptonId::~LeptonId()
{

  csa14Cuts_barrel_.clear();
  csa14Cuts_endcap_.clear();

}

bool LeptonId::initElectronCuts(int bunchSpacing)
{

  printf("ID scenario: %dns bunch spacing\n", bunchSpacing);

  switch(bunchSpacing) {

    case 25: {

      csa14Cuts_barrel_["dEtaIn"] 		= new float[4]{0.02, 0.0181, 0.0106, 0.0091};
      csa14Cuts_barrel_["dPhiIn"] 		= new float[4]{0.2579, 0.0936, 0.0323, 0.031};
      csa14Cuts_barrel_["full5x5_sigmaIetaIeta"] = new float[4]{0.0125, 0.0123, 0.0107, 0.0106};
      csa14Cuts_barrel_["hOverE"] 		= new float[4]{0.2564, 0.141, 0.067, 0.0532};
      csa14Cuts_barrel_["d0"] 			= new float[4]{0.025, 0.0166, 0.0131, 0.0126};
      csa14Cuts_barrel_["dz"] 			= new float[4]{0.5863, 0.54342, 0.22310, 0.0116};
      csa14Cuts_barrel_["oOverEmOOverP"] 	= new float[4]{0.1508, 0.1353, 0.1043, 0.0609};
      csa14Cuts_barrel_["pfdbiso"] 		= new float[4]{0.3313, 0.24, 0.2179, 0.1649};
      csa14Cuts_barrel_["nMissingHits"] 	= new float[4]{2, 1, 1, 1};

      csa14Cuts_endcap_["dEtaIn"] 		= new float[4]{0.0141, 0.0124, 0.0108, 0.0106};
      csa14Cuts_endcap_["dPhiIn"] 		= new float[4]{0.2591, 0.0642, 0.0455, 0.0359};
      csa14Cuts_endcap_["full5x5_sigmaIetaIeta"] = new float[4]{0.0371, 0.035, 0.0318, 0.0305};
      csa14Cuts_endcap_["hOverE"] 		= new float[4]{0.1335, 0.1115, 0.097, 0.0835};
      csa14Cuts_endcap_["d0"] 			= new float[4]{0.2232, 0.098, 0.0845, 0.0163};
      csa14Cuts_endcap_["dz"] 			= new float[4]{0.9513, 0.9187, 0.7523, 0.5999};
      csa14Cuts_endcap_["oOverEmOOverP"]	= new float[4]{0.1542, 0.1443, 0.1201, 0.1126};
      csa14Cuts_endcap_["pfdbiso"] 		= new float[4]{0.3816, 0.3529, 0.254, 0.2075};
      csa14Cuts_endcap_["nMissingHits"] 	= new float[4]{3, 1, 1, 1};

      eleIdInit_ = true;
      break;

    }

    case 50: {

      csa14Cuts_barrel_["dEtaIn"] 		= new float[4]{0.021, 0.016, 0.015, 0.012};
      csa14Cuts_barrel_["dPhiIn"] 		= new float[4]{0.25, 0.080, 0.051, 0.024};
      csa14Cuts_barrel_["full5x5_sigmaIetaIeta"] = new float[4]{0.012, 0.012, 0.010, 0.010};
      csa14Cuts_barrel_["hOverE"] 		= new float[4]{0.24, 0.15, 0.10, 0.074};
      csa14Cuts_barrel_["d0"] 			= new float[4]{0.031, 0.019, 0.012, 0.0091};
      csa14Cuts_barrel_["dz"] 			= new float[4]{0.50, 0.036, 0.030, 0.017};
      csa14Cuts_barrel_["oOverEmOOverP"] 	= new float[4]{0.32, 0.11, 0.053, 0.026};
      csa14Cuts_barrel_["pfdbiso"] 		= new float[4]{0.24, 0.18, 0.14, 0.10};
      csa14Cuts_barrel_["nMissingHits"] 	= new float[4]{2, 1, 1, 1};

      csa14Cuts_endcap_["dEtaIn"] 		= new float[4]{0.028, 0.025, 0.023, 0.019};
      csa14Cuts_endcap_["dPhiIn"] 		= new float[4]{0.23, 0.097, 0.056, 0.043};
      csa14Cuts_endcap_["full5x5_sigmaIetaIeta"] = new float[4]{0.035, 0.032, 0.030, 0.029};
      csa14Cuts_endcap_["hOverE"] 		= new float[4]{0.19, 0.12, 0.099, 0.080};
      csa14Cuts_endcap_["d0"] 			= new float[4]{0.22, 0.099, 0.068, 0.037};
      csa14Cuts_endcap_["dz"] 			= new float[4]{0.91, 0.88, 0.78, 0.065};
      csa14Cuts_endcap_["oOverEmOOverP"] 	= new float[4]{0.13, 0.11, 0.11, 0.076};
      csa14Cuts_endcap_["pfdbiso"] 		= new float[4]{0.24, 0.21, 0.15, 0.14};
      csa14Cuts_endcap_["nMissingHits"] 	= new float[4]{3, 1, 1, 1};

      eleIdInit_ = true;
      break;

    }

    default: {

      printf("Cuts not defined for this scenario!\n" );

      eleIdInit_ = false;

    }

  }

  return eleIdInit_;

}

bool LeptonId::passEleIdCSA14CutBased(ExtendedElectron *ele, unsigned int WP)
{

  return passEleIdCSA14CutBased(ele->pt(), ele->scEta(),
				ele->dEtaIn(), ele->dPhiIn(),
				ele->full5x5sigietaieta(), ele->hOverE(),
				ele->d0(), ele->dz(),
				ele->ecalE(), ele->eOverPIn(),
				ele->pfdbetaiso(), ele->passConvVeto(),
				ele->nLostHitsInner(), WP);

}

bool LeptonId::passEleIdCSA14CutBased(float elePt, float eleSCeta,
				      float eleDEtaIn, float eleDPhiIn,
				      float eleFull5x5sigietaieta, float eleHOverE,
				      float eleD0, float eleDz,
				      float eleEcalE, float eleEOverPIn,
				      float elePFdbetaiso, bool elePassConvVeto,
				      int eleNMissingHits, unsigned int WP)
{

  if(!eleIdInit_) {
    printf("Cuts have not been initialized!\n");
    return false;
  }

  if(fabs(eleSCeta) > 2.5) return false;

  bool pass = false;

  eleCuts *cuts = fabs(eleSCeta) < 1.479 ? &csa14Cuts_barrel_ : &csa14Cuts_endcap_;

  float eleOoEmooP = 0.0;
  if(eleEcalE == 0.0) {
    eleOoEmooP = 1e30;
  } else if(!std::isfinite(eleEcalE)) {
    eleOoEmooP = 1e30;
  } else {
    eleOoEmooP = fabs((1.0 - eleEOverPIn)/eleEcalE);
  }

  pass =   fabs(eleDEtaIn) < (*cuts)["dEtaIn"][WP]
	&& fabs(eleDPhiIn) < (*cuts)["dPhiIn"][WP]
	&& eleFull5x5sigietaieta < (*cuts)["full5x5_sigmaIetaIeta"][WP]
	&& eleHOverE < (*cuts)["hOverE"][WP]
	&& fabs(eleD0) < (*cuts)["d0"][WP]
	&& fabs(eleDz) < (*cuts)["dz"][WP]
	&& eleOoEmooP < (*cuts)["oOverEmOOverP"][WP]
	&& elePFdbetaiso/elePt < (*cuts)["pfdbiso"][WP]
	&& elePassConvVeto
	&& eleNMissingHits < (*cuts)["nMissingHits"][WP];

  return pass;

}

bool LeptonId::passEleIdCSA14MVA(ElectronF *ele, unsigned int WP)
{

  printf("Working points not yet defined!\n");

  return false;

}

bool LeptonId::passEleIsoCSA14(ElectronF *ele, unsigned int WP)
{

  return passEleIsoCSA14(ele->pt(), ele->scEta(), ele->pfdbetaiso(), WP);

}

bool LeptonId::passEleIsoCSA14(float elePt, float eleSCeta, float elePFdbetaiso, unsigned int WP)
{

  if(!eleIdInit_) {
    printf("Cuts have not been initialized!\n");
    return false;
  }

  if(fabs(eleSCeta) > 2.5) return false;

  eleCuts *cuts = fabs(eleSCeta) < 1.479 ? &csa14Cuts_barrel_ : &csa14Cuts_endcap_;

  return elePFdbetaiso/elePt < (*cuts)["pfdbiso"][WP];

}

bool LeptonId::passElectronId(ElectronF *ele, unsigned int WP)
{

  if(WP == VETO) { 
    return ele->isvetoelectron();
  }
  else if(WP == LOOSE) {
    //   return ele->islooseelectron();
    return (ele->islooseid() && ele->miniiso()<0.2);
  }
  else if(WP == MEDIUM) {
    //    return ele->ismediumelectron();
    return (ele->ismediumid() && ele->miniiso()<0.1);
  }
  else if(WP == TIGHT) { 
    return ele->istightelectron();
  }
  else if(WP == MVA) { 
    if(ele->pt() > 10) {
      if(fabs(ele->eta())<0.8) return (ele->mvaidnontrig()>0.967083 && ele->miniiso()<0.1);
      else if(fabs(ele->eta())<1.479) return (ele->mvaidnontrig()>0.929117 && ele->miniiso()<0.1);
      else if (fabs(ele->eta())<2.5) return (ele->mvaidnontrig()>0.726311 && ele->miniiso()<0.1);
      else return false;
    } else {
      if(fabs(ele->eta())<0.8) return (ele->mvaidnontrig()>0.287435 && ele->miniiso()<0.1);
      else if(fabs(ele->eta())<1.479) return (ele->mvaidnontrig()>0.221846 && ele->miniiso()<0.1);
      else if (fabs(ele->eta())<2.5) return (ele->mvaidnontrig()>-0.303263 && ele->miniiso()<0.1);
      else return false;
    }
  }
  else if(WP == MVAVeto){
    if(ele->pt() > 10) {
      if(fabs(ele->eta())<0.8) return (ele->mvaidnontrig()>0.913286 && ele->miniiso()<0.2);
      else if(fabs(ele->eta())<1.479) return (ele->mvaidnontrig()>0.805013 && ele->miniiso()<0.2);
      else if (fabs(ele->eta())<2.5) return (ele->mvaidnontrig()>0.358969 && ele->miniiso()<0.2);
      else return false;
    } else {
      if(fabs(ele->eta())<0.8) return (ele->mvaidnontrig()>-0.083313 && ele->miniiso()<0.2);
      else if(fabs(ele->eta())<1.479) return (ele->mvaidnontrig()>-0.235222 && ele->miniiso()<0.2);
      else if (fabs(ele->eta())<2.5) return (ele->mvaidnontrig()>-0.67099 && ele->miniiso()<0.2);
      else return false;
    }
  }
  else if(WP == MultiIsoVetoL){ 
    if(ele->pt() > 10) {
      if(fabs(ele->eta())<0.8) return (ele->mvaidnontrig()>0.913286 && (ele->miniiso()<0.22 && (ele->ptratio()>0.63 || ele->ptrel()>6.0)));
      else if(fabs(ele->eta())<1.479) return (ele->mvaidnontrig()>0.805013 && (ele->miniiso()<0.22 && (ele->ptratio()>0.63 || ele->ptrel()>6.0)));
      else if (fabs(ele->eta())<2.5) return (ele->mvaidnontrig()>0.358969 && (ele->miniiso()<0.22 && (ele->ptratio()>0.63 || ele->ptrel()>6.0)));
      else return false;
    } else {
      if(fabs(ele->eta())<0.8) return (ele->mvaidnontrig()>-0.083313 && (ele->miniiso()<0.22 && (ele->ptratio()>0.63 || ele->ptrel()>6.0)));
      else if(fabs(ele->eta())<1.479) return (ele->mvaidnontrig()>-0.235222 && (ele->miniiso()<0.22 && (ele->ptratio()>0.63 || ele->ptrel()>6.0)));
      else if (fabs(ele->eta())<2.5) return (ele->mvaidnontrig()>-0.67099 && (ele->miniiso()<0.22 && (ele->ptratio()>0.63 || ele->ptrel()>6.0)));
      else return false;
    }
  }
  else if(WP == MultiIsoVetoVL){ 
    if(ele->pt() > 10) {
      if(fabs(ele->eta())<0.8) return (ele->mvaidnontrig()>0.913286 && (ele->miniiso()<0.23 && (ele->ptratio()>0.60 || ele->ptrel()>5.8)));
      else if(fabs(ele->eta())<1.479) return (ele->mvaidnontrig()>0.805013 && (ele->miniiso()<0.23 && (ele->ptratio()>0.60 || ele->ptrel()>5.8)));
      else if (fabs(ele->eta())<2.5) return (ele->mvaidnontrig()>0.358969 && (ele->miniiso()<0.23 && (ele->ptratio()>0.60 || ele->ptrel()>5.8)));
      else return false;
    } else {
      if(fabs(ele->eta())<0.8) return (ele->mvaidnontrig()>-0.083313 && (ele->miniiso()<0.23 && (ele->ptratio()>0.60 || ele->ptrel()>5.8)));
      else if(fabs(ele->eta())<1.479) return (ele->mvaidnontrig()>-0.235222 && (ele->miniiso()<0.23 && (ele->ptratio()>0.60 || ele->ptrel()>5.8)));
      else if (fabs(ele->eta())<2.5) return (ele->mvaidnontrig()>-0.67099 && (ele->miniiso()<0.23 && (ele->ptratio()>0.60 || ele->ptrel()>5.8)));
      else return false;
    }
  }
  else if(WP == MiniIsoVeto){
    if(ele->pt() > 10) {
      if     (fabs(ele->eta())<0.8  ) return (ele->mvaidnontrig()>0.913286 && ele->miniiso()<0.2);
      else if(fabs(ele->eta())<1.479) return (ele->mvaidnontrig()>0.805013 && ele->miniiso()<0.2);
      else if(fabs(ele->eta())<2.5  ) return (ele->mvaidnontrig()>0.358969 && ele->miniiso()<0.2);
      else return false;
    } else {
      if     (fabs(ele->eta())<0.8  ) return (ele->mvaidnontrig()>-0.083313 && ele->miniiso()<0.2);
      else if(fabs(ele->eta())<1.479) return (ele->mvaidnontrig()>-0.235222 && ele->miniiso()<0.2);
      else if(fabs(ele->eta())<2.5  ) return (ele->mvaidnontrig()>-0.67099  && ele->miniiso()<0.2);
      else return false;
    }
  }
  else if(WP == MT2Veto){
    return (ele->isvetoelectron() && ele->miniiso()<0.1);
  }
  else {
    printf("Electron ID working point not defined!\n");
  }

  return false;

}

bool LeptonId::passMuonIdOnly(MuonF *mu)
{

  return (fabs(mu->eta()) < 2.4 && mu->ismediummuon());

}

bool LeptonId::passMuonId(MuonF *mu, unsigned int WP)
{

  if(WP == TIGHT) return (fabs(mu->eta()) < 2.4 && mu->istightmuon() && passMuonIso(mu, WP));
  else if(WP == MEDIUM){
    return (mu->ismediummuon() && mu->miniiso()<0.1);
    // return (fabs(mu->eta()) < 2.4 && mu->ismediummuon() && passMuonIso(mu, WP));
  }
  else if(WP==VETO){
    return (mu->isloosemuon() && fabs(mu->d0())<0.1 && fabs(mu->dz())<0.5  && mu->miniiso()<0.2);
  }
  else if (WP == MVAVeto) {
    if(fabs(mu->eta())<1.2 && mu->pt()>10) return (mu->ismediummuon() && mu->mvaiso()>-0.2);
    else if(fabs(mu->eta())<1.2 && mu->pt()<10) return (mu->ismediummuon() && mu->mvaiso()>-0.8);
    else if(fabs(mu->eta())<2.4 && mu->pt()>10) return (mu->ismediummuon() && mu->mvaiso()>-0.5);
    else if(fabs(mu->eta())<2.4 && mu->pt()<10) return (mu->ismediummuon() && mu->mvaiso()>-0.8);
    return false;
  }
  else if (WP == MultiIsoVetoL) { 
    return (mu->ismediummuon() && (mu->miniiso()<0.22 && (mu->ptratio()>0.63 || mu->ptrel()>6)));
  }
  else if (WP == MultiIsoVetoVL) { 
    return (mu->ismediummuon() && (mu->miniiso()<0.23 && (mu->ptratio()>0.60 || mu->ptrel()>5.8)));
  }
  else if (WP == MiniIsoVeto) {
    return (mu->ismediummuon() && mu->miniiso()<0.2);
  }
  else if(WP == MT2Veto){
    return (mu->isloosemuon() && mu->miniiso()<0.2);
  }
  return false;

}

bool LeptonId::passMuonIso(MuonF *mu, unsigned int WP)
{

  // working points need to be defined as appropriate ... these are roughly based on Run1 working points
  if(WP == TIGHT) {
    return (mu->pfdbetaiso()/mu->pt() < 0.15);
  }
  else if(WP == MEDIUM) {
    return (mu->pfdbetaiso()/mu->pt() < 0.20);
  }
  else if(WP == LOOSE) {
    return (mu->pfdbetaiso()/mu->pt() < 0.40);
  }
  else {
    printf("Muon isolation working point not defined!\n");
  }

  return false;

}

bool LeptonId::passTauId(TauF *tau, unsigned int isoWP, unsigned int antimuWP, unsigned int antieleWP)
{

  bool passdm = (tau->hpsid() & kDecayMode) > 0; // this should be true by default in MiniAOD
  bool passiso = false;
  bool passantimu = false;
  bool passantiele = false;

  switch(isoWP) {

    case LOOSE : {
      passiso = (tau->hpsid() & kLooseIsoMVALT) > 0;
      break;
    }
    case MEDIUM : {
      passiso = (tau->hpsid() & kMediumIsoMVALT) > 0;
      break;
    }
    case TIGHT : {
      passiso = (tau->hpsid() & kTightIsoMVALT) > 0;
      break;
    }
    default : {
      printf("Tau anti-muon discriminator working point not defined!\n");
      passiso = false;
    }

  }

  switch(antimuWP) {

    case LOOSE : {
      passantimu = (tau->hpsid() & kLooseMuMVA) > 0;
      break;
    }
    case MEDIUM : {
      passantimu = (tau->hpsid() & kMediumMuMVA) > 0;
      break;
    }
    case TIGHT : {
      passantimu = (tau->hpsid() & kTightMuMVA) > 0;
      break;
    }
    default : {
      printf("Tau anti-muon discriminator working point not defined!\n");
      passantimu = false;
    }

  }

  switch(antieleWP) {

    case LOOSE : {
      passantiele = (tau->hpsid() & kLooseEleMVA) > 0;
      break;
    }
    case MEDIUM : {
      passantiele = (tau->hpsid() & kMediumEleMVA) > 0;
      break;
    }
    case TIGHT : {
      passantiele = (tau->hpsid() & kTightEleMVA) > 0;
      break;
    }
    default : {
      printf("Tau anti-electron discriminator working point not defined!\n");
      passantiele = false;
    }

  }

  return (passdm && passiso && passantimu && passantiele);

}
