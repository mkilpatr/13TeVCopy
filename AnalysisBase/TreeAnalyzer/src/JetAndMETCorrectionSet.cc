
#include "AnalysisBase/TreeAnalyzer/interface/JetAndMETCorrectionSet.h"
#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/Utilities/interface/ParticleInfo.h"

namespace ucsbsusy {

float METScaleCorr::scaleCorr(float trueMETPT) const {
  double x = std::max(20.0,double(trueMETPT));
  return ( (-0.0569193 -2.10679/x -52.8809/(x*x)) - (-0.00187469 -2.90383/x -33.1923/(x*x) ) );
}
CylLorentzVectorF METScaleCorr::getCorrectedMET(const CylLorentzVectorF& trueBosons, const CylLorentzVectorF& met) const {
  if(trueBosons.pt() <= 0)
    return met;
  return met + CylLorentzVectorF(trueBosons.pt(),0,trueBosons.phi(),0)*scaleCorr(trueBosons.pt()) ;
}

METResCorr::METResCorr() : Correction("METRes"), metParScale(0.97),metPerpScale(0.94) {}
CylLorentzVectorF METResCorr::getCorrectedMET(const CylLorentzVectorF& trueBosons,const CylLorentzVectorF& trueMET, CylLorentzVectorF met) const {
  if(trueBosons.pt() <= 0)
    return met;
  if(trueMET.pt() >= 0){
    met -= trueMET;
  }

  float metParTrue   = met.pt() * TMath::Cos( PhysicsUtilities::deltaPhi(trueBosons,met) );
  float metPerpTrue  = met.pt() * TMath::Sin( PhysicsUtilities::deltaPhi(trueBosons,met) );

  CylLorentzVectorF truePar ( TMath::Abs(metParTrue) * metParScale, 0, metParTrue > 0 ? trueBosons.phi() : TVector2::Phi_mpi_pi(trueBosons.phi() - TMath::Pi()) , 0);
  CylLorentzVectorF truePerp ( TMath::Abs(metPerpTrue) * metPerpScale, 0,
      metPerpTrue > 0 ? TVector2::Phi_mpi_pi(trueBosons.phi() - TMath::PiOver2() ) : TVector2::Phi_mpi_pi(trueBosons.phi() + TMath::PiOver2() ) , 0 );


  if(trueMET.pt() >= 0){
    return trueMET + truePar + truePerp;
  } else {
    return truePar + truePerp;
  }


}

void JetAndMETCorrectionSet::load(int correctionOptions)
{
  loadSimple("JetAndMET",correctionOptions);
  if(options_ & METSCALE){
    metScale = new METScaleCorr;
    corrections.push_back(metScale);
  }
  if(options_ & METRESOLUTION){
    metResolution = new METResCorr;
    corrections.push_back(metResolution);
  }
}

void JetAndMETCorrectionSet::processMET(const BaseTreeAnalyzer * ana) {
  if(ana->process < defaults::TTBAR || ana->process > defaults::SIGNAL ){
    return;
  }

  originalMET =  *ana->met;
  correctedMET = *ana->met;

  if(options_ & NULLOPT) return;


  trueBosons = CylLorentzVectorF();
  trueMET    = CylLorentzVectorF();


  for(const auto& g: ana->genParticleReader.genParticles ){
    if(!ParticleInfo::isLastInChain(&g)) continue;

    int pdgid = TMath::Abs(g.pdgId());
    if(ParticleInfo::isBSM(pdgid)){
      trueBosons += g.p4();
      trueMET += g.p4();
      continue;
    }
    if(pdgid == ParticleInfo::p_gamma && ParticleInfo::isDoc(g.status())){
      trueBosons += g.p4();
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
          trueMET += dau->p4();
        } else if(pdgid == ParticleInfo::p_tauminus){
          for(unsigned int iD2 = 0; iD2 < dau->numberOfDaughters(); ++iD2){
            GenParticleFRef dau2 = ParticleInfo::getFinal(dau->daughterRef(iD2));
            if(ParticleInfo::isANeutrino(dau2->pdgId()))
              trueMET += dau2->p4();
          }
        }
      }
      if(isLeptonic)
        trueBosons += g.p4();
      continue;
    }
    continue;
  }


  CylLorentzVectorF tempMET = correctedMET.p4();

  if(metScale) {
    tempMET = metScale->getCorrectedMET(trueBosons,tempMET);
  }
  if(metResolution) {
    tempMET = metResolution->getCorrectedMET(trueBosons,trueMET,tempMET);
  }

  correctedMET.setP4(tempMET);
}


} /* namespace ucsbsusy */
