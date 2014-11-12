//--------------------------------------------------------------------------------------------------
// 
// GenParticleFiller
// 
// Filler for MC truth information.
// 
// GenParticleFiller.cc created on Mon Nov 10 20:01:46 CET 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisBase/Analyzer/interface/GenParticleFiller.h"

using namespace ucsbsusy;

const int GenParticleFiller::defaultOptions = GenParticleFiller::NULLOPT;

//--------------------------------------------------------------------------------------------------
GenParticleFiller::GenParticleFiller(const int options, const string branchName, const edm::InputTag genParticleTag) :
  BaseFiller(options, branchName),
  genParticleTag_(genParticleTag),
  mcpart(new GenParticleCoords("particle")),
  q3rdgen(new GenParticleCoords("q3rdgen")),
  boson(new GenParticleCoords("boson")),
  boson_daughter(new GenParticleCoords("boson_daughter")),
  tau_daughter(new GenParticleCoords("tau_daughter"))
{
  addDataBlock(*mcpart);
  addDataBlock(*q3rdgen);
  addDataBlock(*boson);
  addDataBlock(*boson_daughter);
  addDataBlock(*tau_daughter);
}

//--------------------------------------------------------------------------------------------------
void GenParticleFiller::addDataBlock(GenParticleCoords &coords)
{
  coords.ptindex     = data.addMulti<float>(branchName_,coords.name+"_pt",0);
  coords.etaindex    = data.addMulti<float>(branchName_,coords.name+"_eta",0);
  coords.phiindex    = data.addMulti<float>(branchName_,coords.name+"_phi",0);
  coords.massindex   = data.addMulti<float>(branchName_,coords.name+"_mass",0);
  coords.statusindex = data.addMulti<int  >(branchName_,coords.name+"_status",0);
  coords.pdgidindex  = data.addMulti<int  >(branchName_,coords.name+"_pdgid",0);
}

//--------------------------------------------------------------------------------------------------
void GenParticleFiller::fillDataBlock(GenParticleCoords &coords, const reco::GenParticle* p)
{
  data.fillMulti<float>(coords.ptindex, p->pt());
  data.fillMulti<float>(coords.etaindex, p->eta());
  data.fillMulti<float>(coords.phiindex, p->phi());
  data.fillMulti<float>(coords.massindex, p->mass());
  data.fillMulti<int  >(coords.statusindex, p->status());
  data.fillMulti<int  >(coords.pdgidindex, p->pdgId());
}

//--------------------------------------------------------------------------------------------------
void GenParticleFiller::load(const edm::Event& iEvent)
{
  reset();

  FileUtilities::enforceGet(iEvent,genParticleTag_,genParticles_,true);

  isLoaded_ = true;
}

//--------------------------------------------------------------------------------------------------
void GenParticleFiller::fill()
{

  vector<const reco::GenParticle*> bosons;

  for(const reco::GenParticle &p : *genParticles_) {
    if(&p == nullptr) continue;

    if(!(options_ & SAVEALL))
      if(p.status()!=23 && p.status()!=22 && p.status()!=2 && p.status()!=1) continue;

    fillDataBlock(*mcpart, &p);

    if(MCTruth::isA(MCTruth::kTop, &p) && p.status() == 22)
      fillDataBlock(*q3rdgen, &p);

    if(MCTruth::isA(MCTruth::kBottom, &p) && p.status() == 23)
      fillDataBlock(*q3rdgen, &p);

    if(MCTruth::isAWOrZ(&p) && p.status() == 22)
      bosons.push_back(&p);

  }

  // get daughters of W/Z bosons or of taus from W/Z decay
  vector<const reco::GenParticle*> daughters(2*bosons.size(), 0);
  vector<const reco::GenParticle*> taudaughters(2*bosons.size(), 0);

  for(size_t i = 0; i < bosons.size(); i++) {
    MCTruth::findBosonDaughters(*genParticles_, bosons[i], daughters[i*2], daughters[(i*2)+1]);
    fillDataBlock(*boson, bosons[i]);

    if(daughters[i*2]) {
      fillDataBlock(*boson_daughter, daughters[i*2]);
      // get tau information if any
      if(MCTruth::isA(MCTruth::kTau, daughters[i*2])) {
        MCTruth::findTauDaughter(*genParticles_, daughters[i*2], taudaughters[i*2]);
        if(taudaughters[i*2])
          fillDataBlock(*tau_daughter, taudaughters[i*2]);
      }
    }

    if(daughters[(i*2)+1]) {
      fillDataBlock(*boson_daughter, daughters[(i*2)+1]);
      // get tau information if any
      if(MCTruth::isA(MCTruth::kTau, daughters[(i*2)+1])) {
        MCTruth::findTauDaughter(*genParticles_, daughters[(i*2)+1], taudaughters[(i*2)+1]);
        if(taudaughters[(i*2)+1])
          fillDataBlock(*tau_daughter, taudaughters[(i*2)+1]);
      }
    }
  }

  isFilled_ = true;

}
