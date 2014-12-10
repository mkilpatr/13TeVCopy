#include "AnalysisBase/Analyzer/interface/PhysicsAnalyzer.h"
#include "AnalysisTools/TreeReader/interface/Defaults.h"

#include "AnalysisTools/Utilities/interface/JetFlavorMatching.h"

using namespace ucsbsusy;
using namespace std;


class TestParticleAndJetID : public PhysicsAnalyzer {
  public:

  void printGenInfo(){
    ParticleInfo::printGenInfo(*genParticles,-1);
    ParticleInfo::printPackedGenInfo(*packedGenParticles,-1);
  }


  void printMatchingParticles(){
    ParticleInfo::printGenInfo(*genParticles,-1);
    std::vector<JetFlavorMatching::ParticleDecay> bHadrons;
    std::vector<JetFlavorMatching::ParticleDecay> cHadrons;
    std::vector<JetFlavorMatching::ParticleDecay> partons;

    JetFlavorMatching::getHadronDecays(genparticles->genParticles_,bHadrons,cHadrons);
    JetFlavorMatching::getPartons(genparticles->genParticles_,partons, true);

    cout <<"-----New Event------"<<endl;
    cout <<"b:"<<endl;
    for(const auto& p : bHadrons) ParticleInfo::printGenParticleInfo(*p.particle,p.particle.key());
    cout <<"c:"<<endl;
    for(const auto& p : cHadrons) ParticleInfo::printGenParticleInfo(*p.particle,p.particle.key());
    cout <<"p:"<<endl;
    for(const auto& p : partons) ParticleInfo::printGenParticleInfo(*p.particle,p.particle.key());


  }

void testAssociation() {
  std::vector<JetFlavorMatching::ParticleDecay> bHadrons;
  std::vector<JetFlavorMatching::ParticleDecay> cHadrons;
  std::vector<JetFlavorMatching::ParticleDecay> partons;
  std::vector<int> partonParticleAssoc(genparticles->packedGenParticles_->size(),-1);

  JetFlavorMatching::getHadronDecays(genparticles->genParticles_,bHadrons,cHadrons);
  JetFlavorMatching::getPartons(genparticles->genParticles_,partons, true);

  JetFlavorMatching::associateDecayProducts(genparticles->genParticles_,genparticles->packedGenParticles_,
      &bHadrons,&cHadrons,&partons,&partonParticleAssoc);

  ParticleInfo::printGenInfo(*genParticles,-1);


  cout <<"b:"<<endl;
    for(unsigned int iH = 0; iH < bHadrons.size(); ++iH){
      const JetFlavorMatching::ParticleDecay& hadDecay = bHadrons[iH];
      ParticleInfo::printGenParticleInfo(*hadDecay.particle,hadDecay.particle.key());
      cout << ">>"<<endl;
      for(unsigned int iD = 0; iD < hadDecay.decayInts.size(); ++iD){
        ParticleInfo::printPackedGenParticleInfo(&genparticles->packedGenParticles_->at(hadDecay.decayInts[iD]),hadDecay.decayInts[iD]);
      } cout << endl;
    }

    cout <<"c:"<<endl;
    for(unsigned int iH = 0; iH < cHadrons.size(); ++iH){
      const JetFlavorMatching::ParticleDecay& hadDecay = cHadrons[iH];
      ParticleInfo::printGenParticleInfo(*hadDecay.particle,hadDecay.particle.key());
      cout << ">>"<<endl;
      for(unsigned int iD = 0; iD < hadDecay.decayInts.size(); ++iD){
        ParticleInfo::printPackedGenParticleInfo(&genparticles->packedGenParticles_->at(hadDecay.decayInts[iD]),hadDecay.decayInts[iD]);
      } cout << endl;
    }

    cout <<"p:"<<endl;
    for(unsigned int iH = 0; iH < partons.size(); ++iH){
      const JetFlavorMatching::ParticleDecay& hadDecay = partons[iH];
      ParticleInfo::printGenParticleInfo(*hadDecay.particle,hadDecay.particle.key());
      cout << ">>"<<endl;
      for(unsigned int iD = 0; iD < hadDecay.decayInts.size(); ++iD){
        ParticleInfo::printPackedGenParticleInfo(&genparticles->packedGenParticles_->at(hadDecay.decayInts[iD]),hadDecay.decayInts[iD]);
      }cout << endl;
    }


}


void testJetHadronAssociation() {
  std::vector<JetFlavorMatching::ParticleDecay> bHadrons;
  std::vector<JetFlavorMatching::ParticleDecay> cHadrons;
  std::vector<JetFlavorMatching::ParticleDecay> partons;
  std::vector<int> partonParticleAssoc(genparticles->packedGenParticles_->size(),-1);

  JetFlavorMatching::getHadronDecays(genparticles->genParticles_,bHadrons,cHadrons);
  JetFlavorMatching::getPartons(genparticles->genParticles_,partons, true);

  JetFlavorMatching::associateDecayProducts(genparticles->genParticles_,genparticles->packedGenParticles_,
      &bHadrons,&cHadrons,&partons,&partonParticleAssoc);

  std::vector<JetFlavorMatching::ParticleContainments> mainBHadrons;
  std::vector<JetFlavorMatching::ParticleContainments> satelliteBHadrons;

  std::vector<JetFlavorMatching::ParticleContainments> mainCHadrons;
  std::vector<JetFlavorMatching::ParticleContainments> satelliteCHadrons;

  JetFlavorMatching::associateHadronsToJets(genparticles->packedGenParticles_,*genJets,bHadrons,mainBHadrons,satelliteBHadrons);
  JetFlavorMatching::associateHadronsToJets(genparticles->packedGenParticles_,*genJets,cHadrons,mainCHadrons,satelliteCHadrons);

  cout <<"b:"<<endl;
    for(unsigned int iH = 0; iH < bHadrons.size(); ++iH){
      const JetFlavorMatching::ParticleDecay& hadDecay = bHadrons[iH];
      ParticleInfo::printGenParticleInfo(*hadDecay.particle,hadDecay.particle.key());
      cout << ">>"<<endl;
      for(unsigned int iD = 0; iD < hadDecay.decayInts.size(); ++iD){
        ParticleInfo::printPackedGenParticleInfo(&genparticles->packedGenParticles_->at(hadDecay.decayInts[iD]),hadDecay.decayInts[iD]);
      }
    }

    cout <<"c:"<<endl;
    for(unsigned int iH = 0; iH < cHadrons.size(); ++iH){
      const JetFlavorMatching::ParticleDecay& hadDecay = cHadrons[iH];
      ParticleInfo::printGenParticleInfo(*hadDecay.particle,hadDecay.particle.key());
      cout << ">>"<<endl;
      for(unsigned int iD = 0; iD < hadDecay.decayInts.size(); ++iD){
        ParticleInfo::printPackedGenParticleInfo(&genparticles->packedGenParticles_->at(hadDecay.decayInts[iD]),hadDecay.decayInts[iD]);
      }
    }

  for(unsigned int iJ = 0; iJ < genJets->size(); ++iJ){
    if(mainBHadrons[iJ].size() + satelliteBHadrons[iJ].size() + mainCHadrons[iJ].size() +  satelliteCHadrons[iJ].size() == 0) continue;
    const auto& jet = genJets->at(iJ);
    cout <<"("<< jet.pt() <<","<<jet.eta()<<","<<jet.phi()<<")" <<endl <<"mB ";
    for(unsigned int iM = 0; iM < mainBHadrons[iJ].size(); ++iM){
        cout <<"(" <<mainBHadrons[iJ][iM].first.key() <<","<<mainBHadrons[iJ][iM].second.pt() <<") ";
    }
    cout <<"sB ";
    for(unsigned int iM = 0; iM < satelliteBHadrons[iJ].size(); ++iM){
        cout <<"("<<satelliteBHadrons[iJ][iM].first.key() <<","<<satelliteBHadrons[iJ][iM].second.pt() <<") ";
    }

    cout << endl<<"mC ";
    for(unsigned int iM = 0; iM < mainCHadrons[iJ].size(); ++iM){
        cout <<"(" <<mainCHadrons[iJ][iM].first.key() <<","<<mainCHadrons[iJ][iM].second.pt() <<") ";
    }
    cout <<"sC ";
    for(unsigned int iM = 0; iM < satelliteCHadrons[iJ].size(); ++iM){
        cout <<"("<<satelliteCHadrons[iJ][iM].first.key() <<","<<satelliteCHadrons[iJ][iM].second.pt() <<") ";
    }
    cout << endl;
  }
}

void testJetFlavorAssignment() {
  std::vector<JetFlavorMatching::ParticleDecay> bHadrons;
  std::vector<JetFlavorMatching::ParticleDecay> cHadrons;
  std::vector<JetFlavorMatching::ParticleDecay> partons;
  std::vector<int> partonParticleAssoc(genparticles->packedGenParticles_->size(),-1);

  JetFlavorMatching::getHadronDecays(genparticles->genParticles_,bHadrons,cHadrons);
  JetFlavorMatching::getPartons(genparticles->genParticles_,partons, true);

  JetFlavorMatching::associateDecayProducts(genparticles->genParticles_,genparticles->packedGenParticles_,
      &bHadrons,&cHadrons,&partons,&partonParticleAssoc);

  std::vector<JetFlavorMatching::ParticleContainments> mainBHadrons;
  std::vector<JetFlavorMatching::ParticleContainments> satelliteBHadrons;

  std::vector<JetFlavorMatching::ParticleContainments> mainCHadrons;
  std::vector<JetFlavorMatching::ParticleContainments> satelliteCHadrons;

  JetFlavorMatching::associateHadronsToJets(genparticles->packedGenParticles_,*genJets,bHadrons,mainBHadrons,satelliteBHadrons);
  JetFlavorMatching::associateHadronsToJets(genparticles->packedGenParticles_,*genJets,cHadrons,mainCHadrons,satelliteCHadrons);


  std::vector<JetFlavorInfo::JetFlavor>                          flavors          (genJets->size(),JetFlavorInfo::unmatched_jet);
  std::vector<std::vector<JetFlavorMatching::ParticleDecayRef> > matchedParticles (genJets->size());
  std::vector<float>                                             bHadronPTs       (genJets->size(),0);
  std::vector<float>                                             cHadronPTs       (genJets->size(),0);
  ParticleInfo::printGenInfo(*genParticles,-1);
  ParticleInfo::printPackedGenInfo(*packedGenParticles,-1);




  std::vector<JetFlavorMatching::ParticleContainments> partonContainments;

  JetFlavorMatching::assignJetHadronFlavors(*genJets,JetFlavorInfo::b_jet, mainBHadrons,satelliteBHadrons,flavors,bHadronPTs,matchedParticles);
  JetFlavorMatching::assignJetHadronFlavors(*genJets,JetFlavorInfo::c_jet, mainCHadrons,satelliteCHadrons,flavors,cHadronPTs,matchedParticles);
  JetFlavorMatching::assignJetPartonFlavors(*genJets,partons,flavors,matchedParticles,.16);

  cout <<"-----"<<endl;

  cout <<"b:"<<endl;
    for(unsigned int iH = 0; iH < bHadrons.size(); ++iH){
      const JetFlavorMatching::ParticleDecay& hadDecay = bHadrons[iH];
      ParticleInfo::printGenParticleInfo(*hadDecay.particle,hadDecay.particle.key());
      cout << ">> " << iH <<endl;
      for(unsigned int iD = 0; iD < hadDecay.decayInts.size(); ++iD){
        ParticleInfo::printPackedGenParticleInfo(&genparticles->packedGenParticles_->at(hadDecay.decayInts[iD]),hadDecay.decayInts[iD]);
      } cout << endl;
    }

    cout <<"c:"<<endl;
    for(unsigned int iH = 0; iH < cHadrons.size(); ++iH){
      const JetFlavorMatching::ParticleDecay& hadDecay = cHadrons[iH];
      ParticleInfo::printGenParticleInfo(*hadDecay.particle,hadDecay.particle.key());
      cout << ">> " << iH <<endl;
      for(unsigned int iD = 0; iD < hadDecay.decayInts.size(); ++iD){
        ParticleInfo::printPackedGenParticleInfo(&genparticles->packedGenParticles_->at(hadDecay.decayInts[iD]),hadDecay.decayInts[iD]);
      } cout << endl;
    }

    cout <<"p:"<<endl;
    for(unsigned int iH = 0; iH < partons.size(); ++iH){
      const JetFlavorMatching::ParticleDecay& hadDecay = partons[iH];
      ParticleInfo::printGenParticleInfo(*hadDecay.particle,hadDecay.particle.key());
      cout << ">> " << iH <<endl;
    }


    for(unsigned int iG = 0; iG < genJets->size(); ++iG){
      const auto& jet = genJets->at(iG);

      cout <<"("<< jet.pt() <<","<<jet.eta()<<","<<jet.phi()<<")" <<endl <<"mB ";
      for(unsigned int iM = 0; iM < mainBHadrons[iG].size(); ++iM){
          cout <<"(" <<mainBHadrons[iG][iM].first.key() <<","<<mainBHadrons[iG][iM].second.pt() <<") ";
      }
      cout <<"sB ";
      for(unsigned int iM = 0; iM < satelliteBHadrons[iG].size(); ++iM){
          cout <<"("<<satelliteBHadrons[iG][iM].first.key() <<","<<satelliteBHadrons[iG][iM].second.pt() <<") ";
      }

      cout <<"mC ";
      for(unsigned int iM = 0; iM < mainCHadrons[iG].size(); ++iM){
          cout <<"(" <<mainCHadrons[iG][iM].first.key() <<","<<mainCHadrons[iG][iM].second.pt() <<") ";
      }
      cout <<"sC ";
      for(unsigned int iM = 0; iM < satelliteCHadrons[iG].size(); ++iM){
          cout <<"("<<satelliteCHadrons[iG][iM].first.key() <<","<<satelliteCHadrons[iG][iM].second.pt() <<") ";
      }

      cout << endl;

      JetFlavorInfo::JetFlavor patFlv = JetFlavorInfo::numJetFlavors;

      for(unsigned int iJ = 0; iJ < ak4Jets->jets_->size(); ++iJ){
        const auto& genJetRef = ak4Jets->getStdGenJet(ak4Jets->jets_->at(iJ));
        if(genJetRef.isNull()) continue;
        if(genJetRef.key() != iG) continue;
        patFlv = JetFlavorInfo::jetFlavor(ak4Jets->jets_->at(iJ).partonFlavour());
      }



      cout << "("<< JetFlavorInfo::jetFlavorName(patFlv)
      <<","<<JetFlavorInfo::jetFlavorName(flavors[iG])<< ") [";
      for(unsigned int iP = 0; iP < matchedParticles[iG].size(); ++iP){
        cout <<  matchedParticles[iG][iP].key() <<" ";
      }
      cout <<"]"<<endl;

    }



//  std::vector<JetFlavorMatching::HadronDecay> bHadrons = JetFlavorMatching::getBHadronDecays(genparticles->genParticles_);
//  JetFlavorMatching::associateDecayProducts(bHadrons,genparticles->genParticles_,genparticles->packedGenParticles_);
//  std::vector<JetFlavorMatching::HadronContainments>   mainBHadrons;
//  std::vector<JetFlavorMatching::HadronContainments>   satelliteBHadrons;
//  JetFlavorMatching::associateBHadronsToJets(genparticles->packedGenParticles_,*genJets,bHadrons,mainBHadrons,satelliteBHadrons);
//
//  vector<int> matchedParticles;
//  vector<JetFlavorInfo::JetFlavor> jetFlavors;
//  JetFlavorMatching::assignJetPartonFlavors(genparticles->genParticles_,*genJets,jetFlavors,.4,3,0,JetFlavorMatching::isMatchingParticle<reco::GenParticle>,&matchedParticles);
//
//  cout <<"-----"<<endl;
//  for(unsigned int iJ = 0; iJ < ak4Jets->jets_->size(); ++iJ){
//    const auto& genJet = ak4Jets->getStdGenJet(ak4Jets->jets_->at(iJ));
//    if(genJet.isNull()) continue;
//    cout <<"("<< genJet->pt() <<","<<genJet->eta() <<","<<genJet->phi() <<") ["<<JetFlavorInfo::jetFlavorName(
//        JetFlavorInfo::jetFlavor(ak4Jets->getPartonFlavor(ak4Jets->jets_->at(iJ)))
//        ) <<","<< ak4Jets->getPartonFlavor(ak4Jets->jets_->at(iJ))
//        <<"] [" << JetFlavorInfo::jetFlavorName(jetFlavors[genJet.key()]) <<"]" <<endl;
//  }
//

}
















  ANALYZER_MODE
    TestParticleAndJetID(const edm::ParameterSet &cfg) : PhysicsAnalyzer(cfg)
    {
      initialize(cfg,"EventInfo",EVTINFO);
      initialize(cfg,"Gen",GENPARTICLES,GenParticleFiller::LOADPACKED);
//      void PhysicsAnalyzer::initialize(const edm::ParameterSet& cfg, const std::string pSetName, const VarType type, const int options, const std::string branchName){
      initialize(cfg, "Jets", AK4JETS);


//      book();
    }
  bool load(const edm::Event& iEvent, const edm::EventSetup& iSetup)
  {
    PhysicsAnalyzer::load(iEvent,iSetup);
    genParticles = &(*genparticles->genParticles_);
    packedGenParticles = &(*genparticles->packedGenParticles_);
    genJets = &(*ak4Jets->stdGenJets_);
    return true;
  }

  ~TestParticleAndJetID() {}
    void analyze() {
//      printGenInfo();
//      printMatchingParticles();
//      testAssociation();
//      testJetHadronAssociation();
//      testBHadronMatching();
      testJetFlavorAssignment();

//      fill();
      }

    const reco::GenParticleCollection * genParticles;
    const pat::PackedGenParticleCollection * packedGenParticles;
    const reco::GenJetCollection * genJets;


};

DEFINE_FWK_MODULE(TestParticleAndJetID);
