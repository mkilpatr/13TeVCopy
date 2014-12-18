#include "AnalysisBase/Analyzer/interface/PhysicsAnalyzer.h"
#include "AnalysisTools/TreeReader/interface/Defaults.h"

#include "AnalysisTools/Utilities/interface/JetFlavorMatching.h"
#include "AnalysisTools/Utilities/interface/TopDecayMatching.h"
#include "AnalysisTools/Parang/interface/Plotter.h"
#include "AnalysisTools/Parang/interface/Polybook.h"

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



void testDecayProximity() {
  static Polybook eventPlots(plotter);



  std::vector<JetFlavorMatching::ParticleDecay> bHadrons;
  std::vector<JetFlavorMatching::ParticleDecay> cHadrons;
  std::vector<JetFlavorMatching::ParticleDecay> partons;
  std::vector<int> partonParticleAssoc(genparticles->packedGenParticles_->size(),-1);

  JetFlavorMatching::getHadronDecays(genparticles->genParticles_,bHadrons,cHadrons);
  JetFlavorMatching::getPartons(genparticles->genParticles_,partons, true);

  JetFlavorMatching::associateDecayProducts(genparticles->genParticles_,genparticles->packedGenParticles_,
      &bHadrons,&cHadrons,&partons,&partonParticleAssoc);


  vector<const GenParticle*> docPartons;
  vector<int> matchesToBHadrons;

  for(unsigned int iP = 0; iP < genParticles->size(); ++iP){
    const auto& p = genParticles->at(iP);
    if(!ParticleInfo::isDocOutgoing(p.status())) continue;
    if(!ParticleInfo::isQuarkOrGluon(p.pdgId())) continue;

    docPartons.push_back(&p);
    matchesToBHadrons.push_back(-1);

    int nM = 0;

    if(TMath::Abs(p.pdgId()) != ParticleInfo::p_b) continue;
    for(unsigned int iH = 0; iH < bHadrons.size(); ++iH){
      if(!ParticleUtilities::isAncestor(&p,&(*bHadrons[iH].particle))) continue;
      matchesToBHadrons.back() = iH;
      nM++;
    }


    if(matchesToBHadrons.back() == -1){
      ParticleInfo::printGenInfo(*genParticles,-1);
      ParticleInfo::printGenParticleInfo(p,iP);
            throw cms::Exception("Bad decay chain ");
    }

    if(nM > 1){
      matchesToBHadrons.back() = -1;
    }

  }


  for(size iP = 0; iP < docPartons.size(); ++iP){
    if(matchesToBHadrons[iP] < 0) continue;
    const auto& p = *docPartons[iP];

//    const GenParticle * closestMom = 0;
//    double closestDR2 = 99999;
//
//    for(const auto* p2 : docPartons){
//      if(!ParticleUtilities::isAncestor(p2,&(*bHadrons[matchesToBHadrons[iP]].particle))) continue;
//      double dr2 = PhysicsUtilities::deltaR2(*p2,*bHadrons[matchesToBHadrons[iP]].particle);
//      if(dr2 >= closestDR2) continue;
//      closestDR2 = dr2;
//      closestMom = p2;
//
//    }




    eventPlots.rewind();
    eventPlots("",true);
    ++eventPlots;
    eventPlots("",true);
        eventPlots("all_match__",    false );//closestMom == 0);
        eventPlots("right_match__", false );//closestMom == docPartons[iP]);
        eventPlots("wrong_match__", false );//closestMom != docPartons[iP]);
    ++eventPlots;

    double dE = bHadrons[matchesToBHadrons[iP]].particle->energy() /  p.energy();
    double dPT = bHadrons[matchesToBHadrons[iP]].particle->pt() /  p.pt();

    eventPlots("DE_incl__", true);
    eventPlots("DE_lt0p2__", dE < .2);
    eventPlots("DE_eq0p2to0p5__", dE >= .2 && dE < .5);
    eventPlots("DE_eq0p5to1p0__", dE >= .5 && dE < 1);
    eventPlots("DE_geq1p0__", dE >= 1);


    eventPlots("DPT_incl__", true);
    eventPlots("DPT_lt0p2__", dPT < .2);
    eventPlots("DPT_eq0p2to0p5__", dPT >= .2 && dPT < .5);
    eventPlots("DPT_eq0p5to1p0__", dPT >= .5 && dPT < 1);
    eventPlots("DPT_geq1p0__", dPT >= 1);

//    eventPlots("no_match__", closestMom == 0);
//    eventPlots("right_match__", closestMom == docPartons[iP]);
//    eventPlots("wrong_match__", closestMom != docPartons[iP]);

    eventPlots.fill(PhysicsUtilities::deltaR(*bHadrons[matchesToBHadrons[iP]].particle,p),"partonHadronDR",";#DeltaR to parton",100,0,5);



    for(const int iD : bHadrons[matchesToBHadrons[iP]].decayInts ){
      const auto& d = genparticles->packedGenParticles_->at(iD);
      double dr = PhysicsUtilities::deltaR(d,p);


      eventPlots.fill(dr,"partonDR",";#DeltaR to parton",100,0,5);

    }
  }

  for(unsigned int iH = 0; iH < bHadrons.size(); ++iH){

    double partonPT = 0;
    int realMatch = -1;
    for(size iP = 0; iP < matchesToBHadrons.size(); ++iP){
      if(matchesToBHadrons[iP] != int(iH)) continue;
      partonPT = docPartons[iP]->pt();
      realMatch = iP;
      break;
    }
    if(realMatch < 0) continue;

    for(const int iD : bHadrons[iH].decayInts ){
      const auto& d = genparticles->packedGenParticles_->at(iD);

      int closestParton = -1;
      double closestDR2 = 99999;

      for(unsigned int iP = 0; iP < docPartons.size(); ++iP){
        const auto& p = *docPartons[iP];
        double dr2 = PhysicsUtilities::deltaR2(d,p);
        if(dr2 >= closestDR2) continue;
        closestDR2 = dr2;
        closestParton = iP;
      }


      eventPlots.rewind();
      eventPlots("",true);
      ++eventPlots;
      eventPlots("",false);
      eventPlots("all_match__", true);
      eventPlots("right_match__", closestParton == realMatch);
      eventPlots("wrong_match__", closestParton != realMatch);


      eventPlots.fill(d.pt()/partonPT,"partonPTFrac",";Decay product p_{T} fraction",100,0,2);

    }

  }


}







void testTopAssociation() {
  TopDecayMatching::Partons partons;
  TopDecayMatching::Partons incomingPartons;
  TopDecayMatching::ColorSinglets colorSinglets;
  TopDecayMatching::getMatchingPartons(genparticles->genParticles_,partons);
  TopDecayMatching::getColorSinglets(genparticles->genParticles_,partons,colorSinglets,incomingPartons);

  ParticleInfo::printGenInfo(*genParticles,-1);
  for(const auto& p : partons){
    ParticleInfo::printGenParticleInfo(*p.parton,p.parton.key());
    cout << p.hasSinglet << endl;
  }

  for(const auto& s : colorSinglets){
    cout << "(" ;
    for(unsigned int iD = 0; iD < s.singletParticles.size(); ++iD){
      cout << s.singletParticles[iD].key() <<",";
    }
    cout <<") part: ";
    for(auto& p : s.inPartons)
      cout << p->parton.key()<<" ";
    cout << " incoming ";
    for(auto& p : s.incomingParticles)
      cout << p->parton.key()<<" ";

    cout << " matching ";
    for(auto& p : s.matchIdxs)
      cout << p<<" ";
    cout << endl;
  }

  std::vector<std::vector<pat::PackedGenParticleRef>> assocToColorSinglets;
  std::vector<pat::PackedGenParticleRef> nonAssoc;

  TopDecayMatching::associateFinalParticles(genparticles->genParticles_,genparticles->packedGenParticles_,
      colorSinglets,assocToColorSinglets,nonAssoc);



  for(size iS = 0; iS < assocToColorSinglets.size(); ++iS){
    cout << iS <<" : ";
    for(size iF = 0; iF < assocToColorSinglets[iS].size(); ++iF){
      cout << assocToColorSinglets[iS][iF].key() <<" ";
    }
    cout << endl;
  }
  cout <<"non : ";
  for(size iF = 0; iF < nonAssoc.size(); ++iF){
    cout << nonAssoc[iF].key() <<" ";
  }
  cout << endl;

//  TopDecayMatching::associateToNearestPartons(partons,colorSinglets,0,genparticles->packedGenParticles_,assocToColorSinglets,nonAssoc);

  ParticleInfo::printPackedGenInfo(*packedGenParticles,-1);
//  cout << "Pass 1" <<endl;
//  for(unsigned int iP = 0; iP < partons.size(); ++iP){
//    cout <<iP<<"("<< partons[iP].hasSinglet<<"," << partons[iP].parton->pt() <<","<< partons[iP].sumFinal.pt() <<"): ";
//    for(auto f : partons[iP].finalPrts) cout << f.key() <<" ";
//    cout << endl;
//  }
//
//  for(size iS = 0; iS < assocToColorSinglets.size(); ++iS){
//    cout << iS <<" : ";
//    for(size iF = 0; iF < assocToColorSinglets[iS].size(); ++iF){
//      cout << assocToColorSinglets[iS][iF].key() <<" ";
//    }
//    cout << endl;
//  }
//  cout <<"non : ";
//  for(size iF = 0; iF < nonAssoc.size(); ++iF){
//    cout << nonAssoc[iF].key() <<" ";
//  }
//  cout << endl;
//
//
  TopDecayMatching::associateSingletsLoosely(partons,incomingPartons,colorSinglets,TopDecayMatching::maxHadronMatchingRadius,TopDecayMatching::maxHadronRelEnergy,genparticles->packedGenParticles_,assocToColorSinglets);

  cout << "Pass 2" <<endl;
  for(unsigned int iP = 0; iP < partons.size(); ++iP){
    cout <<iP<<"("<< partons[iP].hasSinglet<<"," << partons[iP].parton->pt() <<","<< partons[iP].sumFinal.pt() <<"): ";
    for(auto f : partons[iP].finalPrts) cout << f.key() <<" ";
    cout << endl;
  }

  for(size iS = 0; iS < assocToColorSinglets.size(); ++iS){
    cout << iS <<" : ";
    for(size iF = 0; iF < assocToColorSinglets[iS].size(); ++iF){
      cout << assocToColorSinglets[iS][iF].key() <<" ";
    }
    cout << endl;
  }
  cout <<"non : ";
  for(size iF = 0; iF < nonAssoc.size(); ++iF){
    cout << nonAssoc[iF].key() <<" ";
  }
  cout << endl;

  TopDecayMatching::associateRemaining(partons,incomingPartons,colorSinglets,TopDecayMatching::maxHadronMatchingRadius,TopDecayMatching::maxHadronRelEnergy,genparticles->packedGenParticles_,assocToColorSinglets,nonAssoc);

  cout << "Pass 3" <<endl;
  for(unsigned int iP = 0; iP < partons.size(); ++iP){
    cout <<iP<<"("<< partons[iP].hasSinglet<<"," << partons[iP].parton->pt() <<","<< partons[iP].sumFinal.pt() <<"): ";
    for(auto f : partons[iP].finalPrts) cout << f.key() <<" ";
    cout << endl;
  }

  cout <<"non : ";
  for(size iF = 0; iF < nonAssoc.size(); ++iF){
    cout << nonAssoc[iF].key() <<" ";
  }
  cout << endl;


  vector<int> partonPrtAssoc;

  TopDecayMatching::labelPartonOwnership(partons,genparticles->packedGenParticles_,partonPrtAssoc);
  TopDecayMatching::associatePartonsToJets(partons,*genJets,partonPrtAssoc);

  for(unsigned int iP = 0; iP < partons.size(); ++iP){
    cout <<iP<<"("<< partons[iP].parton->pt() <<","<< partons[iP].sumFinal.energy() <<"): ";

    for(const auto& con : partons[iP].jetAssoc) {
        cout <<"["<<con.second.energy()/ partons[iP].sumFinal.energy()<<","<<genJets->at(con.first).pt()<<","<<genJets->at(con.first).eta()<<","<<genJets->at(con.first).phi()<<"] ";
    }
    cout << endl;
  }

}


void plotTopDecayMatching(const double innerDR, const double outerDR, const double maxRelE){
  TString prefix = TString::Format("iDR_%.2f_oDR_%.2f_mRE_%.2f_",innerDR,outerDR,maxRelE);
  prefix.ReplaceAll(".","p");

  TopDecayMatching::Partons partons;
  TopDecayMatching::Partons incomingPartons;
  TopDecayMatching::ColorSinglets colorSinglets;
  TopDecayMatching::getMatchingPartons(genparticles->genParticles_,partons);
  TopDecayMatching::getColorSinglets(genparticles->genParticles_,partons,colorSinglets,incomingPartons);

  std::vector<std::vector<pat::PackedGenParticleRef>> assocToColorSinglets;
  std::vector<pat::PackedGenParticleRef> nonAssoc;

  TopDecayMatching::associateFinalParticles(genparticles->genParticles_,genparticles->packedGenParticles_,
      colorSinglets,assocToColorSinglets,nonAssoc);
  if(innerDR > 0)TopDecayMatching::associateToNearestPartons(partons,colorSinglets,innerDR,genparticles->packedGenParticles_,assocToColorSinglets,nonAssoc);
  TopDecayMatching::associateSingletsLoosely(partons,incomingPartons,colorSinglets,outerDR,maxRelE,genparticles->packedGenParticles_,assocToColorSinglets);
  TopDecayMatching::associateRemaining(partons,incomingPartons,colorSinglets,outerDR,maxRelE,genparticles->packedGenParticles_,assocToColorSinglets,nonAssoc);

  TopDecayMatching::TopDecays topDecays;

  TopDecayMatching::fillTopDecays(genparticles->genParticles_,partons,topDecays);

  static Polybook quarkPlots(plotter);
  quarkPlots.rewind();

  vector<const TopDecayMatching::Parton*> quarks;

  for(const auto& t : topDecays){
    quarks.push_back(&*t.b_decay);
    if(t.isLeptonic) continue;
    quarks.push_back(&*t.w1_decay);
    quarks.push_back(&*t.w2_decay);
  }

  for(const auto * p : quarks){
    quarkPlots.rewind();
    double pt = p->parton->pt();
    quarkPlots("pt_incl__"      , true);
    quarkPlots("pt_lt20__"      , pt < 20);
    quarkPlots("pt_eq20to50__"  , pt >= 20 && pt < 50);
    quarkPlots("pt_eq50to100__" , pt >= 50 && pt < 100);
    quarkPlots("pt_eq100to200__", pt >= 100 && pt < 200);
    quarkPlots("pt_geq200__"    , pt >= 200);

    if(pt > 0) quarkPlots.fill(p->sumFinal.pt()/pt, "quark_ptRes","hadronized p_{T}/quark p_{T}", 100, 0,2,prefix);
  }

  static Polybook wPlots(plotter);

  for(const auto& t : topDecays){
    if(t.isLeptonic) continue;
    if(t.W_dau1->pt() < 20) continue;
    if(t.W_dau2->pt() < 20) continue;
    wPlots.rewind();

    CartLorentzVector w = t.w1_decay->sumFinal;
    w += t.w2_decay->sumFinal;
    double pt = t.W->pt();
    wPlots("pt_incl__"      , true);
    wPlots("pt_lt20__"      , pt < 20);
    wPlots("pt_eq20to50__"  , pt >= 20 && pt < 50);
    wPlots("pt_eq50to100__" , pt >= 50 && pt < 100);
    wPlots("pt_eq100to200__", pt >= 100 && pt < 200);
    wPlots("pt_geq200__"    , pt >= 200);

    if(pt > 0) wPlots.fill(w.pt()/pt, "w_ptRes","hadronized p_{T}/W p_{T}", 100, 0,2,prefix);
    if(t.W->mass() > 0) wPlots.fill(w.mass()/t.W->mass(), "w_massRes","hadronized mass/W mass", 100, 0,2,prefix);
  }

  static Polybook tPlots(plotter);
  tPlots.rewind();

  for(const auto& t : topDecays){
    if(t.isLeptonic) continue;
    if(t.W_dau1->pt() < 20) continue;
    if(t.W_dau2->pt() < 20) continue;
    if(t.b->pt() < 20) continue;
    tPlots.rewind();

    CartLorentzVector top = t.w1_decay->sumFinal;
    top += t.w2_decay->sumFinal;
    top += t.b_decay->sumFinal;
    double pt = t.top->pt();
    tPlots("pt_incl__"      , true);
    tPlots("pt_lt20__"      , pt < 20);
    tPlots("pt_eq20to50__"  , pt >= 20 && pt < 50);
    tPlots("pt_eq50to100__" , pt >= 50 && pt < 100);
    tPlots("pt_eq100to200__", pt >= 100 && pt < 200);
    tPlots("pt_geq200__"    , pt >= 200);

    if(pt > 0) tPlots.fill(top.pt()/pt, "top_ptRes","hadronized p_{T}/top p_{T}", 100, 0,2,prefix);
    if(t.top->mass() > 0) tPlots.fill(top.mass()/t.top->mass(), "top_massRes","hadronized mass/top mass", 100, 0,2,prefix);
  }

}

void testTopDecayMatching() {

  double inPTs[]  = {0,-1};
  double outPTs[] = {1.0,1.2,1.4,-1};
  double relE[]   = {1.0,1.01,-1};

  for(unsigned int iI = 0; inPTs[iI] >= 0; ++iI)
    for(unsigned int iO = 0; outPTs[iO] >= 0; ++iO)
      for(unsigned int iE = 0; relE[iE] >= 0; ++iE){
        plotTopDecayMatching(inPTs[iI],outPTs[iO],relE[iE]);
      }


}








  ANALYZER_MODE
    TestParticleAndJetID(const edm::ParameterSet &cfg) : PhysicsAnalyzer(cfg), plotter (new PlotterD(3))
    {
      initialize(cfg,"EventInfo",EVTINFO);
      initialize(cfg,"Gen",GENPARTICLES);
//      void PhysicsAnalyzer::initialize(const edm::ParameterSet& cfg, const std::string pSetName, const VarType type, const int options, const std::string branchName){
      initialize(cfg, "Jets", AK4JETS);



      book();
    }
  bool load(const edm::Event& iEvent, const edm::EventSetup& iSetup)
  {
    PhysicsAnalyzer::load(iEvent,iSetup);
    genParticles = &(*genparticles->genParticles_);
    packedGenParticles = &(*genparticles->packedGenParticles_);
    genJets = &(*ak4Jets->reGenJets_);
    return true;
  }

  ~TestParticleAndJetID() {    plotter->write("plots.root");}
    void analyze() {
//      printGenInfo();
//      printMatchingParticles();
//      testAssociation();
//      testJetHadronAssociation();
//      testDecayProximity();

//      testJetFlavorAssignment();

      testTopAssociation();
//      testTopDecayMatching();
      fill();
      }

    const reco::GenParticleCollection * genParticles;
    const pat::PackedGenParticleCollection * packedGenParticles;
    const reco::GenJetCollection * genJets;
    PlotterD * plotter;


};

DEFINE_FWK_MODULE(TestParticleAndJetID);
