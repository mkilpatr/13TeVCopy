#include "AnalysisBase/Analyzer/interface/PhysicsAnalyzer.h"
#include "AnalysisBase/Analyzer/interface/RecoJetFiller.h"
#include "AnalysisTools/TreeReader/interface/Defaults.h"
#include "ObjectProducers/JetProducers/interface/Splittiness.h"

using namespace ucsbsusy;
using namespace std;

class PickyJetTreeProducer : public PhysicsAnalyzer {
  public:

  edm::Handle<pat::PackedCandidateCollection> pfCandidates;
  edm::Handle<reco::PFCandidateCollection> pfPuppiCandidates;
  const bool runOnPUPPI;
  int nPrimVerticies;

  enum SplitResult  {PURE_TO_CLEAN, PURE_TO_SPLIT, MIXED_TO_BETTER, MIXED_TO_MOSTLYBETTER, MIXED_TO_CLEANER, MIXED_TO_MOSTLYCLEANER, MIXED_TO_WORSE, EMPTY };

  Splittiness                         splittiness;
    ANALYZER_MODE
    PickyJetTreeProducer(const edm::ParameterSet &cfg) : PhysicsAnalyzer(cfg) , runOnPUPPI(cfg.getUntrackedParameter<bool>("runOnPUPPI")), nPrimVerticies(0),splittiness      (cfg,false)
    {
      initialize(cfg,"EventInfo",EVTINFO);
      initialize(cfg,"Gen",GENPARTICLES);
      initialize(cfg,"PickyJets",PICKYJETS);

      nVert                 = data.add<size8>("","nVert"                ,"b",0);
      isGoodSplit           = data.add<size8>("","splitResult"          ,"b",0);
      isGen                 = data.add<bool >("","isGen"                ,"O",0);
      numSplits             = data.add<size8>("","numSplits"            ,"b",0);
      superJet_pt           = data.add<float>("","superJet_pt"          ,"F",0);
      superJet_eta          = data.add<float>("","superJet_eta"         ,"F",0);
      superJet_phi          = data.add<float>("","superJet_phi"         ,"F",0);
      superJet_mass         = data.add<float>("","superJet_mass"        ,"F",0);
      superJet_purity       = data.add<float>("","superJet_purity"      ,"F",0);
      tau1                  = data.add<float>("","tau1"                 ,"F",0);
      tau2                  = data.add<float>("","tau2"                 ,"F",0);
      subJet_1_pt           = data.add<float>("","subJet_1_pt"          ,"F",0);
      subJet_1_eta          = data.add<float>("","subJet_1_eta"         ,"F",0);
      subJet_1_phi          = data.add<float>("","subJet_1_phi"         ,"F",0);
      subJet_1_mass         = data.add<float>("","subJet_1_mass"        ,"F",0);
      subJet_1_purity       = data.add<float>("","subJet_1_purity"      ,"F",0);
      subJet_2_pt           = data.add<float>("","subJet_2_pt"          ,"F",0);
      subJet_2_eta          = data.add<float>("","subJet_2_eta"         ,"F",0);
      subJet_2_phi          = data.add<float>("","subJet_2_phi"         ,"F",0);
      subJet_2_mass         = data.add<float>("","subJet_2_mass"        ,"F",0);
      subJet_2_purity       = data.add<float>("","subJet_2_purity"      ,"F",0);
      highest_peak          = data.add<float>("","highest_peak"         ,"F",0);
      lowest_peak           = data.add<float>("","lowest_peak"          ,"F",0);
      minimum_value         = data.add<float>("","minimum_value"        ,"F",0);
      lowest_peak_location  = data.add<float>("","lowest_peak_location" ,"F",0);
      highest_peak_location = data.add<float>("","highest_peak_location","F",0);
      minimum_location      = data.add<float>("","minimum_location"     ,"F",0);
      splitDisc             = data.add<float>("","oldDisc"              ,"F",0);
      shouldSplit           = data.add<bool >("","oldDisc_shouldSplit"  ,"O",0);

      data.book(treeWriter());

    }

    bool load(const edm::Event& iEvent, const edm::EventSetup& iSetup) override {
      PhysicsAnalyzer::load(iEvent,iSetup);
      nPrimVerticies = eventInfo->vertices_->size();
      if(runOnPUPPI) FileUtilities::enforceGet(iEvent,edm::InputTag("puppi","Puppi"),pfPuppiCandidates,true);
      else FileUtilities::enforceGet(iEvent,"packedPFCandidates",pfCandidates,true);

      data.reset();
      return true;
    }


    void fillSplitDecision( const bool isGenJet, const fastjet::PseudoJet& superJet,const double superJetGenE, const vector<pair<double,int>>& superJetContainment, const std::vector<fastjet::PseudoJet*>& subJets, const vector<double>& subJetGenEs, const vector<vector<pair<double,int>>>& subJetContainments,
        const SplitResult goodSplit, const int nSplits, const double nSub2){
      assert(subJets.size() == 2);
      data.reset();

      double nSub1 = splittiness.nSubjettiness.getTau(1, superJet.constituents());

      data.fill<size8>(nVert                 ,convertTo<size8>(min(nPrimVerticies,250),"PickyJetTreeProducer::fillSplitDecision -> nPrimVerticies"));
      data.fill<bool >(isGen                 ,isGenJet);
      data.fill<size8>(isGoodSplit           ,goodSplit);
      data.fill<size8>(numSplits             ,convertTo<size8>(nSplits,"PickyJetTreeProducer::fillSplitDecision -> numSplits"));


      data.fill<float>(superJet_pt           ,superJet.pt());
      data.fill<float>(superJet_eta          ,superJet.eta());
      data.fill<float>(superJet_phi          ,superJet.phi());
      data.fill<float>(superJet_mass         ,superJet.m());
      data.fill<float>(superJet_purity       ,(superJetContainment.size() && superJetGenE > 0) ? superJetContainment[0].first/superJetGenE : 0.0);
      data.fill<float>(tau1                  ,nSub1);
      data.fill<float>(tau2                  ,nSub2);

      double splitResult = -10;
      bool   oldSplitDecision = splittiness.shouldSplitNSubjettiness(superJet, subJets, nSub2, &splitResult);
      const Splittiness::JetDeposition *       jetStuff      = splittiness.getJetStuff();
      data.fill<float>(subJet_1_pt           ,subJets[0]->pt() );
      data.fill<float>(subJet_1_pt           ,subJets[0]->pt() );
      data.fill<float>(subJet_1_eta          ,subJets[0]->eta());
      data.fill<float>(subJet_1_phi          ,subJets[0]->phi());
      data.fill<float>(subJet_1_mass         ,subJets[0]->m()  );
      data.fill<float>(subJet_1_purity       ,(subJetContainments[0].size() && subJetGenEs[0] > 0) ? subJetContainments[0][0].first/subJetGenEs[0] : 0.0);
      data.fill<float>(subJet_2_pt           ,subJets[1]->pt() );
      data.fill<float>(subJet_2_eta          ,subJets[1]->eta());
      data.fill<float>(subJet_2_phi          ,subJets[1]->phi());
      data.fill<float>(subJet_2_mass         ,subJets[1]->m()  );
      data.fill<float>(subJet_2_purity       ,(subJetContainments[1].size() && subJetGenEs[1] > 0) ? subJetContainments[1][0].first/subJetGenEs[1] : 0.0);
      data.fill<float>(highest_peak          ,jetStuff->lobes.getHighestPeak()                                       );
      data.fill<float>(lowest_peak           ,jetStuff->lobes.isUnimodal() ? -0.1 : jetStuff->lobes.getLowestPeak  ());
      data.fill<float>(minimum_value         ,jetStuff->lobes.isUnimodal() ? -0.1 : jetStuff->lobes.getMinimumValue());
      data.fill<float>(lowest_peak_location  ,jetStuff->lobes.getLowestLocation () - jetStuff->centerLocation        );
      data.fill<float>(highest_peak_location ,jetStuff->lobes.getHighestLocation() - jetStuff->centerLocation        );
      data.fill<float>(minimum_location      ,jetStuff->lobes.getMinimumLocation() - jetStuff->centerLocation        );
      data.fill<float>(splitDisc             ,splitResult);
      data.fill<bool >(shouldSplit           ,oldSplitDecision);


      treeWriter()->fill();
    }

    TreeWriterData data;
    size nVert                ;
    size isGoodSplit          ;
    size isGen                ;
    size numSplits            ;
    size superJet_pt          ;
    size superJet_eta         ;
    size superJet_phi         ;
    size superJet_mass        ;
    size superJet_purity      ;
    size tau1                 ;
    size tau2                 ;
    size subJet_1_pt          ;
    size subJet_1_eta         ;
    size subJet_1_phi         ;
    size subJet_1_mass        ;
    size subJet_1_purity      ;
    size subJet_2_pt          ;
    size subJet_2_eta         ;
    size subJet_2_phi         ;
    size subJet_2_mass        ;
    size subJet_2_purity      ;
    size highest_peak         ;
    size lowest_peak          ;
    size minimum_value        ;
    size lowest_peak_location ;
    size highest_peak_location;
    size minimum_location     ;
    size splitDisc            ;
    size shouldSplit          ;


    //For a single jet, get the list of signficant parton containments
    void getContainment(const vector<bool>& goodPartons, const double minJetAssociation, const double minPartonE, const TopDecayMatching::Partons& partons,  int iJet, vector<pair<double,int>>& containment){
      containment.clear();
      for(unsigned int iP = 0; iP < partons.size(); ++iP){
        //if it is not a good parton ignore
        if(!goodPartons[iP]) continue;
        if(partons[iP].sumFinal.energy()  < minPartonE) continue;
        //check to see if the jet has any association to the parton
        std::unordered_map<int,ucsbsusy::CartLorentzVector>::const_iterator jetIt = partons[iP].jetAssoc.find(iJet);
        if(jetIt == partons[iP].jetAssoc.end()) continue;

        //Get the absolute contaiment and see if it passes the cuts;
        double conE = jetIt->second.energy();
        if(conE < minJetAssociation*partons[iP].sumFinal.energy()) continue;

        //otherwise it is a good parton association and add it to the list
        containment.emplace_back(conE,iP);
      }
      sort(containment.begin(),containment.end(),PhysicsUtilities::greaterFirst<double,int>());
    }

    //see if the split was for the better
    SplitResult checkIfGoodSplit(const double minJetRetainment, const double minJetAssociation, const bool isGen,
        const TopDecayMatching::Partons& partons, const std::vector<int>& partPrtAssoc,
        const fastjet::PseudoJet& superJet, const vector<pair<double,int>>& superJetContaiment,
        const std::vector<fastjet::PseudoJet*>      subJets, vector<vector<pair<double,int>>>& subJetContaiments, vector<double>& subjetGenEs ){

      subjetGenEs.clear();
      subjetGenEs.resize(subJets.size());

      //first fill the containments
      subJetContaiments.clear();
      subJetContaiments.resize( subJets.size(), vector<pair<double,int>>(superJetContaiment.size(),pair<double,int>(0,0) )  );

      for(unsigned int iS = 0; iS < subJets.size(); ++iS){
        for(unsigned int iC = 0; iC < superJetContaiment.size(); ++iC) subJetContaiments[iS][iC].second = superJetContaiment[iC].second;
        std::vector<fastjet::PseudoJet>   constituents  = subJets[iS]->constituents();

        for (unsigned int iCon = 0; iCon < constituents.size(); ++iCon) {
          const fastjet::PseudoJet&       constituent   = constituents[iCon];
          if(!constituent.has_user_info()) continue;
          const FastJetClusterer::UserInfo&               info          = constituent.user_info<FastJetClusterer::UserInfo>();
          //make sure it is one of our ID particles (dont exist int he case of gen)
          if(info.type !=  isGen ? FastJetClusterer::RECO : FastJetClusterer::GEN ) continue;
          subjetGenEs[iS] += info->energy();

          //If not matched to any parton...lets skip
          if(partPrtAssoc[info.key()] < 0) continue;

          //see if this particle is one of the important ones!
          for(unsigned int iP = 0; iP < superJetContaiment.size(); ++iP){
            if(partPrtAssoc[info.key()] != superJetContaiment[iP].second ) continue;
            subJetContaiments[iS][iP].first += info->energy();
          }
        }
      }


      //Now we clear out the partons that don't matter
      //and compute our purity
      size nJetsWithPureSplits = 0;
      size nJetsWithSigSplits = 0;
      size nPureSplits = 0;
      for(unsigned int iS = 0; iS < subJets.size(); ++iS){
        bool hasPureSplit = false;
        bool hasSigSplit = false;
        for(unsigned int iP = 0; iP < subJetContaiments[iS].size(); ++iP){
          if(subJetContaiments[iS][iP].first < minJetAssociation * partons[subJetContaiments[iS][iP].second].sumFinal.energy() ){
            subJetContaiments[iS][iP].first = 0;
            continue;
          }
          hasSigSplit = true;
          if(subJetContaiments[iS][iP].first > minJetRetainment*superJetContaiment[iP].first ){
            nPureSplits++;
            hasPureSplit = true;
          }
        }
        if(hasSigSplit)nJetsWithSigSplits++;
        if(hasPureSplit)nJetsWithPureSplits++;
        //now sort and clean
        sort(subJetContaiments[iS].begin(),subJetContaiments[iS].end(),PhysicsUtilities::greaterFirst<double,int>());
        for(int iP = int(subJetContaiments[iS].size()) -1; iP >= 0; --iP ){
          if(subJetContaiments[iS][iP].first > 0) break;
          subJetContaiments[iS].pop_back();
        }
      }

      //If the super jet was empty the split is empty
      if(superJetContaiment.size() == 0)
        return EMPTY;

      //start with pure super jets
      if(superJetContaiment.size() == 1){
        if(nPureSplits) return PURE_TO_CLEAN;
        else return PURE_TO_SPLIT;
      }

      //now for multi parton super jets
      if(nJetsWithPureSplits == subJets.size()){ // does each subjet have at least one pure split in it?
        if(nPureSplits == superJetContaiment.size()) return MIXED_TO_BETTER; //if all partons had a good split it is universally better
        return MIXED_TO_MOSTLYBETTER; //otherwise we lost some of a third parton
      }
      if(nJetsWithPureSplits){
        if(nPureSplits == superJetContaiment.size()) return MIXED_TO_CLEANER; //all partons were cleaned well
        if(nJetsWithPureSplits == nJetsWithSigSplits) return MIXED_TO_MOSTLYCLEANER; //there was a bad split, but nothing that left a significant subjet
        return MIXED_TO_WORSE; //Left a significant residual
      }
      return MIXED_TO_WORSE;
    }

    template<typename Prt,typename FillJet>
    fastjet::PseudoJet addParticles(const edm::Handle<vector<Prt>>& prtCol, const FillJet* jet, const edm::Handle<pat::PackedGenParticleCollection>& genParticles, const reco::GenJet* genJet  ){
      vector<fastjet::PseudoJet> jetlist;
      for(int iD = 0; iD < jet->numberOfDaughters(); ++iD){
        const reco::CandidatePtr  daughter = jet->daughterPtr(iD);
        jetlist.emplace_back(daughter->px(),daughter->py(),daughter->pz(),daughter->energy());
        jetlist.back().set_user_info(new FastJetClusterer::UserInfo(prtCol,daughter.key(), FastJetClusterer::RECO));
      }

      const double genScale = 1e-50;
      if(genJet)
      for(int iD = 0; iD < genJet->numberOfDaughters(); ++iD){
        const reco::CandidatePtr  daughter = genJet->daughterPtr(iD);
        jetlist.emplace_back(genScale*daughter->px(),genScale*daughter->py(),genScale*daughter->pz(),genScale*daughter->energy());
        jetlist.back().set_user_info(new FastJetClusterer::UserInfo(genParticles,daughter.key(), FastJetClusterer::GEN));
      }
      return fastjet::join(jetlist);
    }

    void splitAndFillJet(const double minJetRetainment, const double minJetAssociation, const double minJetPT,  const bool isGen ,
        const TopDecayMatching::Partons& partons,const std::vector<int>& partPrtAssoc,
        const fastjet::PseudoJet& superJet, const double superJetGenE, const vector<pair<double,int>>& superJetContaiment, const int nSplits){

      std::vector<fastjet::PseudoJet*>      subJets;
      const double                          tau2 = splittiness.getNSubjettinessSubjets(superJet,subJets);

      //it must be able to actually split the jets
      if(subJets.size() < 2){
        PhysicsUtilities::trash(subJets);
        return;
      }

      //They can't be pure junk...the subjets must be at least 1 GeV
      bool passPT = true;
      for(const auto* j : subJets){
        if(j->pt2() >= 1) continue;
        passPT = false;
      }
      if(!passPT){
        PhysicsUtilities::trash(subJets);
        return;
      }


      //Now we see how good of a split it was
      vector<vector<pair<double,int>>> subJetContaiments;
      vector<double> subjetGenEs;
      SplitResult splitResult = checkIfGoodSplit(minJetRetainment,minJetAssociation,isGen,partons,partPrtAssoc,superJet,superJetContaiment,subJets,subJetContaiments,subjetGenEs);

      //plot this splitting
      fillSplitDecision(isGen, superJet,superJetGenE, superJetContaiment, subJets, subjetGenEs, subJetContaiments, splitResult, nSplits, tau2);

      //Now split the subjets
      bool stopSplitting = (splitResult == PURE_TO_SPLIT || splitResult == MIXED_TO_WORSE || splitResult == EMPTY);
      if(!stopSplitting){
        for(unsigned int iS = 0; iS < subJets.size(); ++iS){
          auto& subJet = *subJets[iS];
          if(subJet.pt() < minJetPT) continue;
          if(subJetContaiments[iS].size() == 0) continue;


          splitAndFillJet(minJetRetainment,minJetAssociation,minJetPT,isGen,
              partons,partPrtAssoc,subJet,subjetGenEs[iS],subJetContaiments[iS],nSplits+1);
        }
      }
      PhysicsUtilities::trash(subJets);

    }

    ~PickyJetTreeProducer() {}
    void analyze() {
      //using trimmed jets to be general
      RecoJetFiller * filler = pickyJets;
      const auto& genJets  = *filler->reGenJets_;
      const auto& recoJets = *filler->jets_;

      //Parameters
      const double minPartonE           =  30;//The min parton E (hadronized) to even be considered from the start
      const double minJetRetainment     = .90; //A pure split must retain at least 90% of the parton E in the superjet
      const double minJetAssociation    = .15; //Anything with less than 15% of the total parton E is not counted
      const double minJetPT             =  filler->jptMin_; //Don't split jets less than this value




      //get the list of tops
      vector<const reco::GenParticle*> tops;
      for(const auto& g : *genparticles->genParticles_)
        if(TMath::Abs(g.pdgId()) == ParticleInfo::p_t)
          tops.push_back(&g);
      if(tops.size() == 0) return;

      //Do the parton matching to the first set of jets
      const TopDecayMatching::Partons& partons = genparticles->getPartons(genJets);
      const std::vector<int>& partPrtAssoc = genparticles->getPartonPrtAssoc();

      //make a list of the partons that come from top...we don't care about the radiated jets
      vector<bool> goodPartons(partons.size(),false);
      for(unsigned int iP = 0; iP < partons.size(); ++iP){
        for(const auto* t : tops)
          if(ParticleUtilities::isAncestor(t,&*partons[iP].parton)){
            goodPartons[iP] = true;
            break;
          }
      }

      for(unsigned int iJ = 0; iJ < recoJets.size(); ++iJ){
        const auto&  j = recoJets.at(iJ);
        auto gj = filler->getReGenJet(j,iJ,false);
        if(gj.isNull()) continue;
        if(j.pt() < minJetPT) continue;

        //the initial vector of interesting particles in this jet
        vector<pair<double,int>> containment;
        getContainment(goodPartons,minJetAssociation,minPartonE,partons,gj.key(),containment);
        if(containment.size() == 0) continue;

        //Get the PSuedo jet version
        fastjet::PseudoJet superJet = runOnPUPPI ? addParticles(pfPuppiCandidates , &j, genparticles->packedGenParticles_, &*gj)
            : addParticles(pfCandidates , &j, genparticles->packedGenParticles_, &*gj)
            ;
        splitAndFillJet(minJetRetainment,minJetAssociation,minJetPT,false,
            partons,partPrtAssoc,
            superJet,gj->energy(), containment,0);
      }

      for(unsigned int iJ = 0; iJ < genJets.size(); ++iJ){
        const auto&  j = genJets.at(iJ);
        if(j.pt() < minJetPT) continue;

        //the initial vector of interesting particles in this jet
        vector<pair<double,int>> containment;
        getContainment(goodPartons,minJetAssociation,minPartonE,partons,iJ,containment);
        if(containment.size() == 0) continue;

        //Get the PSuedo jet version
        fastjet::PseudoJet superJet = addParticles(genparticles->packedGenParticles_, &j, genparticles->packedGenParticles_, 0);
        splitAndFillJet(minJetRetainment,minJetAssociation,minJetPT,true,
            partons,partPrtAssoc,
            superJet, j.energy(),containment,0);
      }

      }
};

DEFINE_FWK_MODULE(PickyJetTreeProducer);
