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
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "AnalysisBase/Analyzer/interface/BaseFiller.h"
#include "AnalysisTools/Utilities/interface/ParticleUtilities.h"

#include "AnalysisTools/Utilities/interface/TopDecayMatching.h"

namespace ucsbsusy {


  class GenParticleFiller : public BaseFiller {
    public :
      GenParticleFiller(const int options, const string branchName, const edm::InputTag genParticleTag, const edm::InputTag packedGenParticleTag);
      ~GenParticleFiller() {}

      enum  Options           {
                                NULLOPT         = 0
                              , SAVEALL         = (1 <<  0)   // save all gen particles in pruned collection
                              , SAVEPARTONDECAY = (1 <<  1)   // save the parton hadronization energy info
                              , LOADPACKED      = (1 <<  2)   //load the packed gen particles
      };
      static const int defaultOptions;

      void load(const edm::Event& iEvent);
      void fill();
      void reset() override;

    private:
      //typefs to setup how the association is stored, and helpers for that storage
      typedef std::map<size,size> CandMap;
      typedef size16 stor;
      typedef std::vector<stor> storVec;
      void fillMomVec(const reco::GenParticle * c, const CandMap& candMap,const bool requireMoms, storVec& moms) const;
      void fillDauVec(const reco::GenParticle * c, const CandMap& candMap,const bool requireDaus, storVec& daus) const;
      void fillAssoc(const reco::GenParticleRefVector& cands,const CandMap& candMap, const bool requireMoms, const bool requireDaus,
          storVec& assocList,storVec& nMoms,storVec& firstMom,storVec& nDaus,storVec& firstDau ) const;

      //Add the particles from the main interaction
      void addHardInteraction(reco::GenParticleRefVector& outParticles,CandMap& candMap)const;

      //functions related to getting hadron decays
    private:
      //fill all information and do calculations
      void loadPartonDecays(const reco::GenParticleRefVector& genParticles);
    public:
      //Give a jet collection and get partons associated with jets
      // WARNING! if this is called again, the parton collection is changed
      const TopDecayMatching::Partons& getPartons(const reco::GenJetCollection& jets) const;




    private :
      const edm::InputTag genParticleTag_;
      const edm::InputTag packedGenParticleTag_;

      // For reco jets
      size ipt       ;
      size ieta      ;
      size iphi      ;
      size imass     ;
      size istatus   ;
      size ipdgid    ;
      size inmoms    ;
      size ifirstmom ;
      size indaus    ;
      size ifirstdau ;
      size iassoc    ;
      //hadron decay assoc
      size ihade     ;

      //storage quantities
      reco::GenParticleRefVector storedGenParticles;
      CandMap candMap;
      storVec assocList;
      storVec nMoms;
      storVec firstMoms;
      storVec nDaus;
      storVec firstDaus;

      //information related to hadron decay
      TopDecayMatching::Partons partonDecays;
      std::vector<int>   prtPartonAssoc;
      std::vector<float> hadronizationE;

    public :
      edm::Handle<reco::GenParticleCollection> genParticles_;
      edm::Handle<pat::PackedGenParticleCollection> packedGenParticles_;
  };


}

#endif
