#include "AnalysisBase/Analyzer/interface/PhysicsAnalyzer.h"


using namespace ucsbsusy;
using namespace std;

class HadronicTTBarEventFilter : public PhysicsAnalyzer {

  public:
    FILTER_MODE
    HadronicTTBarEventFilter(const edm::ParameterSet &cfg) : PhysicsAnalyzer(cfg)
    {
      initialize(cfg, "Gen",GENPARTICLES);
    }

    ~HadronicTTBarEventFilter() {}

    bool filter() override {
      for(const auto& t : *genparticles->genParticles_){
        if(TMath::Abs(t.pdgId()) != ParticleInfo::p_t ) continue;
        const auto * ft = ParticleUtilities::getFinal(&t);
        assert(ParticleInfo::isDoc(ft->status()));

        for(unsigned int iD = 0; iD < ft->numberOfDaughters(); ++iD){
          const auto& w =  *ft->daughterRef(iD);
          if(!ParticleInfo::isEWKBoson(w.pdgId())) continue;
          const auto * wf = ParticleUtilities::getFinal(&w);
          for(unsigned int iWD = 0; iWD < wf->numberOfDaughters(); ++iWD)
            if(ParticleInfo::isLeptonOrNeutrino(wf->daughter(iWD)->pdgId()))
              return false;
        }
      }
      return true;
    }

};

DEFINE_FWK_MODULE(HadronicTTBarEventFilter);
