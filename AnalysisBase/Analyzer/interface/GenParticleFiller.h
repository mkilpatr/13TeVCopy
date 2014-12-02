//--------------------------------------------------------------------------------------------------
// 
// GenParticleFiller
// 
// Filler for MC truth information.
// 
// GenParticleFiller.h created on Mon Nov 10 20:01:46 CET 2014 
// 
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISBASE_ANALYZER_GENPARTICLEFILLER_H
#define ANALYSISBASE_ANALYZER_GENPARTICLEFILLER_H

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "AnalysisBase/Analyzer/interface/BaseFiller.h"
#include "AnalysisTools/Utilities/interface/ParticleUtilities.h"

namespace ucsbsusy {

  struct GenParticleCoords {
    GenParticleCoords(const string inname) : name(inname), ptindex(0), etaindex(0), phiindex(0), massindex(0), statusindex(0), pdgidindex(0) {}
    const string name;
    size ptindex;
    size etaindex;
    size phiindex;
    size massindex;
    size statusindex;
    size pdgidindex;
  };

  class GenParticleFiller : public BaseFiller {

    public :
      GenParticleFiller(const int options, const string branchName, const edm::InputTag genParticleTag);
      ~GenParticleFiller() {}

      enum  Options           {
                                NULLOPT         = 0
                              , SAVEALL         = (1 <<  0)   // save all gen particles in pruned collection
      };
      static const int defaultOptions;

      void load(const edm::Event& iEvent);
      void fill();

      void addDataBlock(GenParticleCoords& coords);
      void fillDataBlock(GenParticleCoords& coords, const reco::GenParticle* p);

    private :
      const edm::InputTag genParticleTag_;

      GenParticleCoords* mcpart;
      GenParticleCoords* q3rdgen;
      GenParticleCoords* boson;
      GenParticleCoords* boson_daughter;
      GenParticleCoords* tau_daughter;

    public :
      edm::Handle<reco::GenParticleCollection> genParticles_;

  };


}

#endif
