
#include "TRandom3.h"
#include "TFile.h"
#include "AnalysisTools/QuickRefold/interface/TF1Container.h"

#include "AnalysisBase/TreeAnalyzer/interface/JetAndMETCorrectionSet.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"


namespace ucsbsusy {
JetResolutionCorr::JetResolutionCorr(TRandom3 * inRandGen, TString jetResFileName,TString jetResCorrFileName )
: Correction("JetResolution"), randGen(inRandGen) {
  std::clog << "     Loading Jet Resolutions: "<< "AK4ResTrend" <<" from file: "<< jetResFileName << std::endl;
  jetResolutionFile = TFile::Open(jetResFileName,"read");
  if(!jetResolutionFile) throw std::invalid_argument("JetResolutionCorr::JetResolutionCorr: Res file not found!");
  jetResolution = (const QuickRefold::TF1Container*)(jetResolutionFile->Get("AK4ResTrend") );
  if(!jetResolution) throw std::invalid_argument("JetResolutionCorr::JetResolutionCorr: Jet resolution could not be found!");

  std::clog << "     Loading Jet Resolution corrections: "<< "AK4ResCorr" <<" from file: "<< jetResCorrFileName << std::endl;
  jetResolutionCorrFile = TFile::Open(jetResCorrFileName,"read");
  if(!jetResolutionCorrFile) throw std::invalid_argument("JetResolutionCorr::JetResolutionCorr: Res corr file not found!");
  jetResolutionCorr = (const TH1*)(jetResolutionCorrFile->Get("AK4ResCorr") );
  if(!jetResolutionCorr) throw std::invalid_argument("JetResolutionCorr::JetResolutionCorr: Jet resolution corr could not be found!");
}

JetResolutionCorr::~JetResolutionCorr(){
  if(jetResolutionFile) jetResolutionFile->Close();
  delete jetResolutionFile;
  if(jetResolutionCorrFile) jetResolutionCorrFile->Close();
  delete jetResolutionCorrFile;
}

bool JetResolutionCorr::getCorrectedPT(const CORRTYPE corrType, const float rho, const RecoJetF& jet, double& newPT) const {
  newPT = jet.pt();

  //base cuts
  if(jet.pt() < 10) return false;

  double eta, pt;
      eta = jet.eta();
      pt = jet.pt();

//  //if the gen jet is above 20 gev we use its properties
//  if(jet.genJet() && jet.genJet()->pt() > 20 ){
//    eta = jet.genJet()->eta();
//    pt = jet.genJet()->pt();
//  } else {
//    eta = jet.eta();
//    pt = jet.pt();
//  }
  jetResolution->setBin(0,eta);
  jetResolution->setBin(1,rho);
  double res = jetResolution->eval(pt);
  int corrBin = std::min(std::max(jetResolutionCorr->FindFixBin(TMath::Abs(eta)),1),jetResolutionCorr->GetNbinsX());
  double corr = 0;
  switch(corrType){
    case NOMINAL: corr = jetResolutionCorr->GetBinContent(corrBin); break;
    case UP: corr = jetResolutionCorr->GetBinContent(corrBin)+jetResolutionCorr->GetBinError(corrBin); break;
    case DOWN: corr = jetResolutionCorr->GetBinContent(corrBin)-jetResolutionCorr->GetBinError(corrBin); break;
    default: corr = 0;
  }

  double rndNum = 10;
  while(TMath::Abs(rndNum) > 3 ){ //only allow three sigma fluctuations
    rndNum = randGen->Gaus(0,1);
  }

  newPT += pt*(TMath::Sqrt(corr*corr -1)*res);
  if(newPT < 0) newPT = 0;


  return true;
}

void JetResolutionCorr::correctJetsAndMET(const CORRTYPE corrType, const double rho, std::vector<RecoJetF>& jets, MomentumF& met) const {
  switch (corrType) {
    case NONE: return;
    case NOMINAL:
    case UP:
    case DOWN:
    break;
    default: throw std::invalid_argument("JetResolutionCorr::correctJetsAndMET: Not a valid correction type!");
  }
//  std::cout << "New Event: " <<met.pt() << std::endl;

  for(auto& j : jets){
    double newPT;
    if(!getCorrectedPT(corrType,rho,j,newPT)) continue;

	if (newPT > 15)
	{
		met.p4() += j.p4();
	}
     CylLorentzVectorF newMom( newPT < 0 ? 0 : newPT, j.eta(), j.phi(), newPT < 0 ? 0 : newPT*j.mass() /j.pt());
     j.setP4(newMom);

	 if (j.pt() > 15)
	 {
		met.p4() -= j.p4();
	 }
  }

  std::sort(jets.begin(), jets.end(), PhysicsUtilities::greaterPT<RecoJetF>());
}

void JetScaleCorr::correctJetsAndMET(CORRTYPE corrType, std::vector<RecoJetF>& jets, MomentumF& met) const {
  if(corrType == NONE || corrType == NOMINAL) return;

  // Not bothering to correct the SumEt

  CartLorentzVector metvec;
  metvec.SetPxPyPzE(met.px(),met.py(),met.pz(),met.E());
  for(auto& j : jets) {
    if(j.uncertainty() <= 0.0) continue;
    double JESFactor = 1.0;
    if(corrType == UP)
      JESFactor += j.uncertainty();
    else if (corrType == DOWN)
      JESFactor -= j.uncertainty();
	if ((JESFactor * j.pt()) > 15) {
		metvec.SetPx(metvec.px() + j.px());
		metvec.SetPy(metvec.py() + j.py());
	}
    j.setP4(JESFactor*j.p4());
	if (j.pt() > 15) {
		metvec.SetPx(metvec.px() - j.px());
		metvec.SetPy(metvec.py() - j.py());
	}
  }
  std::sort(jets.begin(), jets.end(), PhysicsUtilities::greaterPT<RecoJetF>());
  met.setP4(CylLorentzVectorF(metvec.pt(), metvec.eta(), metvec.phi(), 0.0));
}

void RespTailCorr::process(const std::vector<RecoJetF>& jets, const std::vector<GenJetF>& genJets, const MomentumF& met){

  float MM = -1;
  int ind = -1;
  int flv = -1;
  float resp = -1;
  for(unsigned int iG = 0; iG < genJets.size() && iG < 3; ++iG  ){
    if(genJets[iG].pt() == 0) break;
    float fpt = -1;
    for(const auto& rJ : jets)
      if(rJ.genJet() == &genJets[iG]){
        fpt = rJ.pt();
        break;
      }
    if(fpt < 0 )  fpt = 9.5; //for the cases with no reco jet due to being below thresh
    if(MM < 0 ||  TMath::Abs(fpt - genJets[iG].pt()) > MM){
      ind = genJets[iG].index();
      resp =  fpt/genJets[iG].pt();
      MM = TMath::Abs(fpt - genJets[iG].pt());
      flv = genJets[iG].flavor();
    }
  }

  if(ind >= 0){
    mmResp = resp;
    mmInd = ind;
    mmFlv = flv;
  } else {
    mmResp = -1;
    mmInd = -1;
    mmFlv = -1;
  }
}
float RespTailCorr::getWeight(CORRTYPE corrType) const {
  setXAxisNoUnderOver(   mmInd < 0  ? 1.0 : mmResp); //If none was found, use the correction for 1.0;
  setYAxisNoUnderOver(  mmFlv !=4  ?   1 : 2); //If none was found, use the correction for light jet

  if(targetBinY == 1){
    switch (corrType) {
      case NONE: return 1;
      case NOMINAL: return  get();
      case UP:      return int(targetBinX) == 1  ?  get() + getError() : get() - getError();
      case DOWN:    return int(targetBinX) == 1  ?  get() - getError() : get() + getError();
      default: throw std::invalid_argument("RespTailCorr::getWeight: Not a valid correction type!");
    }
  } else {
    switch (corrType) {
      case NONE: return 1;
      case NOMINAL: return  get();
      case UP:      return int(targetBinX) <= 2 ?  get() + getError() : get() - getError();
      case DOWN:    return int(targetBinX) <= 2 ?  get() - getError() : get() + getError();
      default: throw std::invalid_argument("RespTailCorr::getWeight: Not a valid correction type!");
    }
  }
}

float METScaleCorr::scaleCorr(float trueMETPT) const {
  double x = std::min(std::max(20.0,double(trueMETPT)),300.0);
  return (-0.0402319 + 0.285912/x -64.209/(x*x)) - (0.015672 -0.163545/x -38.5237/(x*x));
}
CylLorentzVectorF METScaleCorr::getCorrectedMET(const CylLorentzVectorF& trueBosons, const CylLorentzVectorF& met) const {
  if(trueBosons.pt() <= 0)
    return met;
  return met + CylLorentzVectorF(trueBosons.pt(),0,trueBosons.phi(),0)*scaleCorr(trueBosons.pt()) ;
}

METResCorr::METResCorr() : Correction("METRes"){}
CylLorentzVectorF METResCorr::getCorrectedMET(const CylLorentzVectorF& trueBosons,const CylLorentzVectorF& trueMET, CylLorentzVectorF met) const {
  if(trueBosons.pt() <= 0)
    return met;
  if(trueMET.pt() >= 0){
    met -= trueMET;
  }

  float metParTrue   = met.pt() * TMath::Cos( PhysicsUtilities::deltaPhi(trueBosons,met) );
  float metPerpTrue  = met.pt() * TMath::Sin( PhysicsUtilities::deltaPhi(trueBosons,met) );

  //variable scaling
  double scalingPT = std::min(trueBosons.pt(),float(300.0));
  double parScale = 0.857944 + 0.000750737*scalingPT;
  double perpScale = 0.842937 + 0.000569862*scalingPT;

  CylLorentzVectorF truePar ( TMath::Abs(metParTrue) * parScale, 0, metParTrue > 0 ? trueBosons.phi() : TVector2::Phi_mpi_pi(trueBosons.phi() - TMath::Pi()) , 0);
  CylLorentzVectorF truePerp ( TMath::Abs(metPerpTrue) * perpScale, 0,
      metPerpTrue > 0 ? TVector2::Phi_mpi_pi(trueBosons.phi() - TMath::PiOver2() ) : TVector2::Phi_mpi_pi(trueBosons.phi() + TMath::PiOver2() ) , 0 );


  if(trueMET.pt() >= 0){
    return trueMET + truePar + truePerp;
  } else {
    return truePar + truePerp;
  }


}

  
  METResSystRunI::METResSystRunI() : Correction("METResSyst"){}
  //  CylLorentzVectorF METResSystRunI::getCorrectedMET(const CORRTYPE corrType, const std::vector<RecoJetF*> jets, CylLorentzVectorF met) const {
  CylLorentzVectorF METResSystRunI::getCorrectedMET(const CORRTYPE corrType, const std::vector<RecoJetF>& jets, CylLorentzVectorF met) const {
    //  CylLorentzVectorF METResSystRunI::getCorrectedMET(const CORRTYPE corrType, const std::vector<RecoJetF>& jets, CylLorentzVectorF met) const {
    
    if(corrType == NONE || corrType == NOMINAL) { return met; }
    
    // calculate clustered met
    CylLorentzVectorF clusteredmet; clusteredmet.SetPxPyPzE(0.,0.,0.,0.);
    for (auto& j : jets) {
      if (j.pt() > 15) { clusteredmet -= j.p4(); }
    }

    // calculate unclustered met
    CylLorentzVectorF unclusteredmet; unclusteredmet.SetPxPyPzE(0.,0.,0.,0.);
    unclusteredmet = met - clusteredmet;

    // scale the unclusteredmet
    float scale = 0.;
    if (corrType == UP)   { scale = 1.1; }
    if (corrType == DOWN) { scale = 0.9; }
    unclusteredmet.SetPxPyPzE(scale*unclusteredmet.px(),scale*unclusteredmet.py(),unclusteredmet.pz(),unclusteredmet.E());

    // recalculate met
    CylLorentzVectorF modifiedmet; modifiedmet.SetPxPyPzE(0.,0.,0.,0.);
    modifiedmet = clusteredmet + unclusteredmet;

    return modifiedmet;
}


  METResSystRunII::METResSystRunII() : Correction("METResSystII"){}
  CylLorentzVectorF METResSystRunII::getCorrectedMET(const CORRTYPE corrType,  CylLorentzVectorF met, EventInfoReader evtreader) const {

    CylLorentzVectorF modifiedmet; modifiedmet.SetPt(0.); modifiedmet.SetEta(0.); modifiedmet.SetPhi(0.); modifiedmet.SetM(0.);
    if(corrType == NONE || corrType == NOMINAL) { return met; }
    if (corrType == UP) { 
      modifiedmet.SetPt(evtreader.metunclusterup); 
      modifiedmet.SetEta(met.eta()); 
      modifiedmet.SetPhi(met.phi()); 
      modifiedmet.SetM(met.mass()); 
    }

    if (corrType == DOWN) { 
      modifiedmet.SetPt(evtreader.metunclusterdn); 
      modifiedmet.SetEta(met.eta()); 
      modifiedmet.SetPhi(met.phi()); 
      modifiedmet.SetM(met.mass()); 
    }

    return modifiedmet;
  }




float METNoHFScaleCorr::scaleCorr(float trueMETPT) const {
  double x = std::min(std::max(20.0,double(trueMETPT)),300.0);
  return ( (-0.0569193 -2.10679/x -52.8809/(x*x)) - (-0.00187469 -2.90383/x -33.1923/(x*x) ) );
}
CylLorentzVectorF METNoHFScaleCorr::getCorrectedMET(const CylLorentzVectorF& trueBosons, const CylLorentzVectorF& met) const {
  if(trueBosons.pt() <= 0)
    return met;
  return met + CylLorentzVectorF(trueBosons.pt(),0,trueBosons.phi(),0)*scaleCorr(trueBosons.pt()) ;
}

METNoHFResCorr::METNoHFResCorr() : Correction("METNoHFResCorr"){}
CylLorentzVectorF METNoHFResCorr::getCorrectedMET(const CylLorentzVectorF& trueBosons,const CylLorentzVectorF& trueMET, CylLorentzVectorF met) const {
  if(trueBosons.pt() <= 0)
    return met;
  if(trueMET.pt() >= 0){
    met -= trueMET;
  }

  float metParTrue   = met.pt() * TMath::Cos( PhysicsUtilities::deltaPhi(trueBosons,met) );
  float metPerpTrue  = met.pt() * TMath::Sin( PhysicsUtilities::deltaPhi(trueBosons,met) );

  CylLorentzVectorF truePar ( TMath::Abs(metParTrue) * 0.97, 0, metParTrue > 0 ? trueBosons.phi() : TVector2::Phi_mpi_pi(trueBosons.phi() - TMath::Pi()) , 0);
  CylLorentzVectorF truePerp ( TMath::Abs(metPerpTrue) * 0.94, 0,
      metPerpTrue > 0 ? TVector2::Phi_mpi_pi(trueBosons.phi() - TMath::PiOver2() ) : TVector2::Phi_mpi_pi(trueBosons.phi() + TMath::PiOver2() ) , 0 );


  if(trueMET.pt() >= 0){
    return trueMET + truePar + truePerp;
  } else {
    return truePar + truePerp;
  }


}
JetAndMETCorrectionSet::JetAndMETCorrectionSet(): metScale(0),metResolution(0), jetResolution(0), jetScale(0), metNoHFScale(0), metNoHFResolution(0)
,originalMET(new MomentumF),correctedMET(new MomentumF),originalMETNoHF(new MomentumF),correctedMETNoHF(new MomentumF),
trueBosons(new CylLorentzVectorF),trueMET(new CylLorentzVectorF),respTailWeight(1),metResolutionSystRunI(0),metResolutionSystRunII(0)
{}
JetAndMETCorrectionSet::~JetAndMETCorrectionSet(){
  delete originalMET;
  delete correctedMET;
  delete originalMETNoHF;
  delete correctedMETNoHF;
  delete trueBosons;
  delete trueMET;
}
void JetAndMETCorrectionSet::load(int correctionOptions,TString jetResolutionFile,TString jetResolutionCorrFile,TString jetResponseTailFile,TRandom3 * randomGenerator)
{
  loadSimple("JetAndMET",correctionOptions);
  if(options_ & METSCALE){
    metScale = new METScaleCorr;
    corrections.push_back(metScale);
    metNoHFScale = new METNoHFScaleCorr;
    corrections.push_back(metNoHFScale);
  }
  if(options_ & METRESOLUTION){
    metResolution = new METResCorr;
    corrections.push_back(metResolution);
    metNoHFResolution = new METNoHFResCorr;
    corrections.push_back(metNoHFResolution);
  }
  if(options_ & JETRESOLUTION){
    jetResolution = new JetResolutionCorr(randomGenerator,jetResolutionFile, jetResolutionCorrFile);
    corrections.push_back(jetResolution);
  }
  if(options_ & JETSCALE){
    jetScale = new JetScaleCorr();
    corrections.push_back(jetScale);
  }
  if(options_ & QCDRESPTAIL){
    respTail = new RespTailCorr(jetResponseTailFile);
    corrections.push_back(respTail);
  }
  if(options_ & METRESSYSTRUNI){
    metResolutionSystRunI = new METResSystRunI;
    corrections.push_back(metResolutionSystRunI);
  }

  if(options_ & METRESSYSTRUNII){ 
    metResolutionSystRunII = new METResSystRunII;
    corrections.push_back(metResolutionSystRunII);
  }

}

void JetAndMETCorrectionSet::processMET(const BaseTreeAnalyzer * ana) {
  (*originalMET) =  (*ana->met);
  (*correctedMET) = (*ana->met);

  (*originalMETNoHF) =  (*ana->metNoHF);
  (*correctedMETNoHF) = (*ana->metNoHF);

  if(ana->process < defaults::TTBAR || ana->process > defaults::SIGNAL ){
    return;
  }

  if(options_ & NULLOPT) return;


  (*trueBosons) = CylLorentzVectorF();
  (*trueMET)    = CylLorentzVectorF();


  for(const auto& g: ana->genParticleReader.genParticles ){
    if(!ParticleInfo::isLastInChain(&g)) continue;

    int pdgid = TMath::Abs(g.pdgId());
    if(ParticleInfo::isBSM(pdgid)){
      (*trueBosons) += g.p4();
      (*trueMET) += g.p4();
      continue;
    }
    if(pdgid == ParticleInfo::p_gamma && ParticleInfo::isDoc(g.status())){
      (*trueBosons) += g.p4();
      continue;
    }
    if(pdgid == ParticleInfo::p_Wplus || pdgid == ParticleInfo::p_Z0){
      bool isLeptonic = false;
      for(unsigned int iD = 0; iD < g.numberOfDaughters(); ++iD){
        GenParticleFRef dau = ParticleInfo::getFinal(g.daughterRef(iD));
        int dauPdgid = TMath::Abs(dau->pdgId());
        if(!ParticleInfo::isLeptonOrNeutrino(dauPdgid)) continue;
        isLeptonic = true;

        if(ParticleInfo::isANeutrino(dauPdgid)){
          (*trueMET) += dau->p4();
        } else if(pdgid == ParticleInfo::p_tauminus){
          for(unsigned int iD2 = 0; iD2 < dau->numberOfDaughters(); ++iD2){
            GenParticleFRef dau2 = ParticleInfo::getFinal(dau->daughterRef(iD2));
            if(ParticleInfo::isANeutrino(dau2->pdgId()))
              (*trueMET) += dau2->p4();
          }
        }
      }
      if(isLeptonic)
        (*trueBosons) += g.p4();
      continue;
    }
    continue;
  }


  CylLorentzVectorF tempMET = correctedMET->p4();
  CylLorentzVectorF origMET = originalMET->p4();


  if(metResolution) {
    tempMET = metResolution->getCorrectedMET(*trueBosons,*trueMET,tempMET);
  }
  if(metScale) {
    tempMET = metScale->getCorrectedMET(*trueBosons,tempMET);
  }
  if(metResolutionSystRunI) { 
    //    tempMET = metResolutionSystRunI->getCorrectedMET(ana->getAnaCfg().corrections.metResSystRunIType, ana->defaultJets->recoJets, origMET);
    tempMET = metResolutionSystRunI->getCorrectedMET(ana->getAnaCfg().corrections.metResSystRunIType, ana->defaultJets->recoJets, origMET);
    //    tempMET = metResolutionSystRunI->getCorrectedMET(ana->getAnaCfg().corrections.metResSystRunIType, ana->defaultJets, origMET);
  }

  if(metResolutionSystRunII) {  
    tempMET = metResolutionSystRunII->getCorrectedMET(ana->getAnaCfg().corrections.metResSystRunIIType, origMET, ana->evtInfoReader);
  }

  correctedMET->setP4(tempMET);

  CylLorentzVectorF tempMETNoHF = correctedMETNoHF->p4();

  if(metResolution) {
    tempMETNoHF = metNoHFResolution->getCorrectedMET(*trueBosons,*trueMET,tempMETNoHF);
  }
  if(metScale) {
    tempMETNoHF = metNoHFScale->getCorrectedMET(*trueBosons,tempMETNoHF);
  }
  correctedMETNoHF->setP4(tempMETNoHF);

}

void JetAndMETCorrectionSet::correctJetResolution(const BaseTreeAnalyzer * ana, RecoJetFCollection& jets, MomentumF& met){
  if(!jetResolution) return;
  if(!ana->isMC()) return;
  jetResolution->correctJetsAndMET(ana->getAnaCfg().corrections.jetResCorrType,ana->rho,jets,met);
}

void JetAndMETCorrectionSet::correctJetScale(const BaseTreeAnalyzer * ana, RecoJetFCollection& jets, MomentumF& met){
  if(!jetScale) return;
  if(!ana->isMC()) return;
  jetScale->correctJetsAndMET(ana->getAnaCfg().corrections.jetScaleCorr,jets,met);
}

void JetAndMETCorrectionSet::processRespTail(const BaseTreeAnalyzer * ana, const JetReader& jetReader, const MomentumF& met) {
  if(!respTail) return;
  if(ana->process != defaults::QCD){ respTailWeight = 1; return;}
  respTail->process(jetReader.recoJets,jetReader.genJets,met);
  respTailWeight = respTail->getWeight(ana->getAnaCfg().corrections.jetResTailCorrType);
}


MomentumF JetAndMETCorrectionSet::getCorrectedMET() const {return *correctedMET;}
MomentumF JetAndMETCorrectionSet::getOriginalMET() const {return *originalMET;}
MomentumF JetAndMETCorrectionSet::getCorrectedMETNoHF() const {return *correctedMETNoHF;}
MomentumF JetAndMETCorrectionSet::getOriginalMETNoHF() const {return *originalMETNoHF;}

} /* namespace ucsbsusy */
