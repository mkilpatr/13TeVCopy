#include "AnalysisBase/Analyzer/interface/PhysicsAnalyzer.h"
#include "AnalysisBase/Analyzer/interface/RecoJetFiller.h"
#include "AnalysisTools/TreeReader/interface/Defaults.h"
#include "ObjectProducers/JetProducers/interface/FastJetClusterer.h"
#include "ObjectProducers/JetProducers/interface/PickyJetSplitting.h"
#include "ObjectProducers/JetProducers/interface/Splittiness.h"

using namespace ucsbsusy;
using namespace std;

class CASubjetCountingTreeProducer : public PhysicsAnalyzer {
  public:

  edm::Handle<pat::PackedCandidateCollection> pfCandidates;
  int nPrimVertices;
  unsigned int run, lumi, event;
  int ngoodpartons;
  PickyJetSplitting * splitter;
  Splittiness splittiness;

  enum SplitResult  {PURE_TO_CLEAN, PURE_TO_SPLIT, MIXED_TO_BETTER, MIXED_TO_MOSTLYBETTER, MIXED_TO_CLEANER, MIXED_TO_MOSTLYCLEANER, MIXED_TO_WORSE, EMPTY };
  enum SplitDecision {NOSPLIT_NO_PARENTS, NOSPLIT_BELOW_MCUT, NOSPLIT_FAIL_PTCUT, NOSPLIT_BELOW_DRMIN, SPLIT_BOTH_SUBJETS, SPLIT_LEADING_SUBJET, PICKY_SHOULD_SPLIT, NOSPLIT_TOO_MANY_SPLITS, NOSPLIT_FAIL_MVA};

    ANALYZER_MODE
    CASubjetCountingTreeProducer(const edm::ParameterSet &cfg) :
      PhysicsAnalyzer(cfg),
      nPrimVertices(0),
      run(0),
      lumi(0),
      event(0),
      ngoodpartons(0),
      splittiness(cfg, false),
      doSplitting(cfg.getParameter<bool>("doSplitting")),
      splittingAlgo(cfg.getParameter<string>("splittingAlgo")),
      mCut(cfg.getParameter<double>("mCut")),
      rMin(cfg.getParameter<double>("rMin")),
      yCut(cfg.getParameter<double>("yCut")),
      maxNSplits(cfg.getParameter<int>("pickyMaxSplits"))
    {
      splitter = splittingAlgo=="Picky" ? new PickyJetSplitting(cfg.getParameter<string>("pickyMVAFileName"),cfg.getParameter<string>("pickyMVAName"),PickyJetSplitting::NOPUPPI_RECO,cfg) : 0;

      initialize(cfg,"EventInfo",EVTINFO);
      initialize(cfg,"Gen",GENPARTICLES);
      initialize(cfg,"PickyJets",PICKYJETS);

      irun                  = data.add<unsigned int>("","run"           ,"i",0);
      ilumi                 = data.add<unsigned int>("","lumi"          ,"i",0);
      ievent                = data.add<unsigned int>("","event"         ,"i",0);
      itoppt                = data.addMulti<float>("","top_pt"          ,0);
      itopeta               = data.addMulti<float>("","top_eta"         ,0);
      itopphi               = data.addMulti<float>("","top_phi"         ,0);
      itopmass              = data.addMulti<float>("","top_mass"        ,0);
      itoppartindex         = data.addMulti<int>  ("","parton_topindex" ,0);
      itoppartpt            = data.addMulti<float>("","parton_pt"       ,0);
      itopparteta           = data.addMulti<float>("","parton_eta"      ,0);
      itoppartphi           = data.addMulti<float>("","parton_phi"      ,0);
      itoppartenergy        = data.addMulti<float>("","parton_energy"   ,0);
      itopparthade          = data.addMulti<float>("","parton_hade"     ,0);
      nGoodPartons          = data.add<size8>("","nGoodPartons"         ,"b",0);
      nVert                 = data.add<size8>("","nVert"                ,"b",0);
      isGoodSplit           = data.add<size8>("","splitResult"          ,"b",0);
      isGen                 = data.add<bool >("","isGen"                ,"O",0);
      numSplits             = data.add<size8>("","numSplits"            ,"b",0);
      superJet_pt           = data.add<float>("","superJet_pt"          ,"F",0);
      superJet_eta          = data.add<float>("","superJet_eta"         ,"F",0);
      superJet_phi          = data.add<float>("","superJet_phi"         ,"F",0);
      superJet_mass         = data.add<float>("","superJet_mass"        ,"F",0);
      superJet_gene         = data.add<float>("","superJet_gene"        ,"F",0);
      superJet_prtIndex     = data.addMulti<int>("","superJet_prtIndex" ,-1);
      superJet_purity       = data.addMulti<float>("","superJet_purity" , 0);
      tau1                  = data.add<float>("","tau1"                 ,"F",0);
      tau2                  = data.add<float>("","tau2"                 ,"F",0);
      tau3                  = data.add<float>("","tau3"                 ,"F",0);
      subJet_1_pt           = data.add<float>("","subJet_1_pt"          ,"F",0);
      subJet_1_eta          = data.add<float>("","subJet_1_eta"         ,"F",0);
      subJet_1_phi          = data.add<float>("","subJet_1_phi"         ,"F",0);
      subJet_1_mass         = data.add<float>("","subJet_1_mass"        ,"F",0);
      subJet_1_gene         = data.add<float>("","subJet_1_gene"        ,"F",0);
      subJet_1_prtIndex     = data.addMulti<int>("","subJet_1_prtIndex" ,-1);
      subJet_1_purity       = data.addMulti<float>("","subJet_1_purity" , 0);
      subJet_2_pt           = data.add<float>("","subJet_2_pt"          ,"F",0);
      subJet_2_eta          = data.add<float>("","subJet_2_eta"         ,"F",0);
      subJet_2_phi          = data.add<float>("","subJet_2_phi"         ,"F",0);
      subJet_2_mass         = data.add<float>("","subJet_2_mass"        ,"F",0);
      subJet_2_gene         = data.add<float>("","subJet_2_gene"        ,"F",0);
      subJet_2_prtIndex     = data.addMulti<int>("","subJet_2_prtIndex" ,-1);
      subJet_2_purity       = data.addMulti<float>("","subJet_2_purity" , 0);
      didSplit              = data.add<size8>("","splitDecision"        ,"b",0);

      data.book(treeWriter());

    }

    ~CASubjetCountingTreeProducer() {}

    TreeWriterData data       ;
    size irun                 ;
    size ilumi                ;
    size ievent               ;
    size itoppt               ;
    size itopeta              ;
    size itopphi              ;
    size itopmass             ;
    size itoppartindex        ;
    size itoppartpt           ;
    size itopparteta          ;
    size itoppartphi          ;
    size itoppartenergy       ;
    size itopparthade         ;
    size nGoodPartons         ;
    size nVert                ;
    size isGoodSplit          ;
    size isGen                ;
    size numSplits            ;
    size superJet_pt          ;
    size superJet_eta         ;
    size superJet_phi         ;
    size superJet_mass        ;
    size superJet_gene        ;
    size superJet_prtIndex    ;
    size superJet_purity      ;
    size tau1                 ;
    size tau2                 ;
    size tau3                 ;
    size subJet_1_pt          ;
    size subJet_1_eta         ;
    size subJet_1_phi         ;
    size subJet_1_mass        ;
    size subJet_1_gene        ;
    size subJet_1_prtIndex    ;
    size subJet_1_purity      ;
    size subJet_2_pt          ;
    size subJet_2_eta         ;
    size subJet_2_phi         ;
    size subJet_2_mass        ;
    size subJet_2_gene        ;
    size subJet_2_prtIndex    ;
    size subJet_2_purity      ;
    size didSplit             ;

    const bool   doSplitting;
    const std::string         splittingAlgo;
    const double mCut;
    const double rMin;
    const double yCut;
    const int    maxNSplits;
    std::vector<const reco::GenParticle*> tops;
    std::map<int,int>         topPartonIndex;
    std::vector<unsigned int> topIndex;
    TopDecayMatching::Partons topPartons;
    std::vector<float>        topPartonsHadE;

    bool load(const edm::Event& iEvent, const edm::EventSetup& iSetup) override {
      PhysicsAnalyzer::load(iEvent,iSetup);
      nPrimVertices = eventInfo->vertices_->size();
      run = eventInfo->eventCoords.run;
      lumi = eventInfo->eventCoords.lumi;
      event = eventInfo->eventCoords.event;
      FileUtilities::enforceGet(iEvent,"packedPFCandidates",pfCandidates,true);

      data.reset();
      return true;
    }

    void fillSplitDecision(const bool isGenJet, const fastjet::PseudoJet& superJet,
                           const double superJetGenE, const vector<pair<double,int>>& superJetContainment,
                           const vector<fastjet::PseudoJet>& subJets, const vector<double>& subJetGenEs,
                           const vector<vector<pair<double,int>>>& subJetContainments, const SplitResult splitResult,
                           const int nSplits, const SplitDecision splitDecision) {

      assert(subJets.size() <= 2);
      data.reset();

      double nsub1 = splittiness.nSubjettiness.getTau(1, superJet.constituents());
      double nsub2 = splittiness.nSubjettiness.getTau(2, superJet.constituents());
      double nsub3 = splittiness.nSubjettiness.getTau(3, superJet.constituents());

      data.fill<unsigned int>(irun                  ,run);
      data.fill<unsigned int>(ilumi                 ,lumi);
      data.fill<unsigned int>(ievent                ,event);

      for(const auto* t : tops) {
        data.fillMulti<float>(itoppt, t->pt());
        data.fillMulti<float>(itopeta, t->eta());
        data.fillMulti<float>(itopphi, t->phi());
        data.fillMulti<float>(itopmass, t->mass());
      }
      for(unsigned int ip = 0; ip < topPartons.size(); ++ip) {
        auto p = topPartons.at(ip);
        data.fillMulti<int>  (itoppartindex, topIndex.at(ip));
        data.fillMulti<float>(itoppartpt, p.pt());
        data.fillMulti<float>(itopparteta, p.eta());
        data.fillMulti<float>(itoppartphi, p.phi());
        data.fillMulti<float>(itoppartenergy, p.sumFinal.energy());
        data.fillMulti<float>(itopparthade, topPartonsHadE.at(ip));
      }

      data.fill<size8>(nGoodPartons          ,convertTo<size8>(ngoodpartons,"CASubjetCountingTreeProducer::fillSplitDecision -> nGoodPartons"));
      data.fill<size8>(nVert                 ,convertTo<size8>(min(nPrimVertices,250),"CASubjetCountingTreeProducer::fillSplitDecision -> nPrimVertices"));

      data.fill<bool >(isGen                 ,isGenJet);
      data.fill<size8>(isGoodSplit           ,splitResult);
      data.fill<size8>(numSplits             ,convertTo<size8>(nSplits,"CASubjetCountingTreeProducer::fillSplitDecision -> numSplits"));

      data.fill<float>(superJet_pt           ,superJet.pt());
      data.fill<float>(superJet_eta          ,superJet.eta());
      data.fill<float>(superJet_phi          ,superJet.phi());
      data.fill<float>(superJet_mass         ,superJet.m());
      data.fill<float>(superJet_gene         ,superJetGenE);
      for(unsigned int icon = 0; icon < superJetContainment.size(); ++icon) {
        data.fillMulti<int>(superJet_prtIndex, topPartonIndex[superJetContainment[icon].second]);
        data.fillMulti<float>(superJet_purity, superJetGenE > 0 ? superJetContainment[icon].first/superJetGenE : 0.0);
      }

      data.fill<float>(tau1                  ,nsub1);
      data.fill<float>(tau2                  ,nsub2);
      data.fill<float>(tau3                  ,nsub3);

      data.fill<float>(subJet_1_pt           ,subJets.size() >= 1 ? subJets[0].pt()  : 0);
      data.fill<float>(subJet_1_eta          ,subJets.size() >= 1 ? subJets[0].eta() : 0);
      data.fill<float>(subJet_1_phi          ,subJets.size() >= 1 ? subJets[0].phi() : 0);
      data.fill<float>(subJet_1_mass         ,subJets.size() >= 1 ? subJets[0].m()   : 0);
      data.fill<float>(subJet_1_gene         ,subJets.size() >= 1 ? subJetGenEs[0]   : 0);
      if(subJets.size() >= 1) {
        if(subJetContainments.size() >= 1) {
          for(unsigned int icon = 0; icon < subJetContainments[0].size(); ++icon) {
            data.fillMulti<int>(subJet_1_prtIndex, topPartonIndex[subJetContainments[0][icon].second]);
            data.fillMulti<float>(subJet_1_purity, subJetGenEs[0] > 0 ? subJetContainments[0][icon].first/subJetGenEs[0] : 0.0);
          }
        } else {
            data.fillMulti<int>(subJet_1_prtIndex,-1);
            data.fillMulti<float>(subJet_1_purity);
        }
      }

      data.fill<float>(subJet_2_pt           ,subJets.size() >= 2 ? subJets[1].pt()  : 0);
      data.fill<float>(subJet_2_eta          ,subJets.size() >= 2 ? subJets[1].eta() : 0);
      data.fill<float>(subJet_2_phi          ,subJets.size() >= 2 ? subJets[1].phi() : 0);
      data.fill<float>(subJet_2_mass         ,subJets.size() >= 2 ? subJets[1].m()   : 0);
      data.fill<float>(subJet_2_gene         ,subJets.size() >= 2 ? subJetGenEs[1]   : 0);
      if(subJets.size() >= 2) {
        if(subJetContainments.size() >= 2) {
          for(unsigned int icon = 0; icon < subJetContainments[1].size(); ++icon) {
            data.fillMulti<int>(subJet_2_prtIndex, topPartonIndex[subJetContainments[1][icon].second]);
            data.fillMulti<float>(subJet_2_purity, subJetGenEs[1] > 0 ? subJetContainments[1][icon].first/subJetGenEs[1] : 0.0);
          }
        } else {
            data.fillMulti<int>(subJet_2_prtIndex,-1);
            data.fillMulti<float>(subJet_2_purity);
        }
      }

      data.fill<size8>(didSplit              ,splitDecision);

      treeWriter()->fill();
    }

    //For a single jet, get the list of signficant parton containments
    void getContainment(const vector<bool>& goodPartons, const double minJetAssociation,
                        const double minPartonPT, const double maxPartonETA,
                        const TopDecayMatching::Partons& partons, int iJet,
                        vector<pair<double,int>>& containment) {

      containment.clear();

      for(unsigned int iP = 0; iP < partons.size(); ++iP){
        //if it is not a good parton ignore
        if(!goodPartons[iP]) continue;
        if(partons[iP].pt() < minPartonPT || fabs(partons[iP].eta()) > maxPartonETA) continue;
        //check to see if the jet has any association to the parton
        std::unordered_map<int,ucsbsusy::CartLorentzVector>::const_iterator jetIt = partons[iP].jetAssoc.find(iJet);
        if(jetIt == partons[iP].jetAssoc.end()) continue;

        //Get the absolute contaiment
        double conE = jetIt->second.energy();

        //Add it to the list
        if(conE > 0)
          containment.emplace_back(conE,iP);
      }
      sort(containment.begin(),containment.end(),PhysicsUtilities::greaterFirst<double,int>());
    }

    //see if the split was for the better
    SplitResult checkIfGoodSplit(const double minJetRetainment, const double minJetAssociation,
                                 const bool isGen, const TopDecayMatching::Partons& partons,
                                 const vector<int>& partPrtAssoc, const fastjet::PseudoJet& superJet,
                                 const vector<pair<double,int>>& superJetContaiment, const vector<fastjet::PseudoJet> subJets,
                                 vector<vector<pair<double,int>>>& subJetContaiments, vector<double>& subjetGenEs) {

      subjetGenEs.clear();
      subjetGenEs.resize(subJets.size());

      //first fill the containments
      subJetContaiments.clear();
      subJetContaiments.resize( subJets.size(), vector<pair<double,int>>(superJetContaiment.size(),pair<double,int>(0,0) )  );

      for(unsigned int iS = 0; iS < subJets.size(); ++iS){
        for(unsigned int iC = 0; iC < superJetContaiment.size(); ++iC) {
          subJetContaiments[iS][iC].second = superJetContaiment[iC].second;
        }
        vector<fastjet::PseudoJet>   constituents  = subJets[iS].constituents();

        for (unsigned int iCon = 0; iCon < constituents.size(); ++iCon) {
          const fastjet::PseudoJet& constituent = constituents[iCon];
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

      //Now we compute our purity
      size nJetsWithPureSplits = 0;
      size nJetsWithSigSplits = 0;
      size nPureSplits = 0;
      for(unsigned int iS = 0; iS < subJets.size(); ++iS){
        bool hasPureSplit = false;
        bool hasSigSplit = false;
        for(unsigned int iP = 0; iP < subJetContaiments[iS].size(); ++iP){
          if(subJetContaiments[iS][iP].first > minJetAssociation * partons[subJetContaiments[iS][iP].second].sumFinal.energy() ){
            hasSigSplit = true;
          }
          if(subJetContaiments[iS][iP].first > minJetRetainment*superJetContaiment[iP].first ){
            nPureSplits++;
            hasPureSplit = true;
          }
        }
        if(hasSigSplit)  nJetsWithSigSplits++;
        if(hasPureSplit) nJetsWithPureSplits++;
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
      for(unsigned int iD = 0; iD < jet->numberOfDaughters(); ++iD){
        const reco::CandidatePtr  daughter = jet->daughterPtr(iD);
        jetlist.emplace_back(daughter->px(),daughter->py(),daughter->pz(),daughter->energy());
        jetlist.back().set_user_info(new FastJetClusterer::UserInfo(prtCol,daughter.key(), FastJetClusterer::RECO));
      }

      const double genScale = 1e-50;
      if(genJet)
      for(unsigned int iD = 0; iD < genJet->numberOfDaughters(); ++iD){
        const reco::CandidatePtr  daughter = genJet->daughterPtr(iD);
        jetlist.emplace_back(genScale*daughter->px(),genScale*daughter->py(),genScale*daughter->pz(),genScale*daughter->energy());
        jetlist.back().set_user_info(new FastJetClusterer::UserInfo(genParticles,daughter.key(), FastJetClusterer::GEN));
      }
      return fastjet::join(jetlist);
    }

    // check splitting decision and fill info
    void splitAndFillJet(const double minJetRetainment, const double minJetAssociation,
                         const double minJetPT, const bool isGen,
                         const TopDecayMatching::Partons& partons,const vector<int>& partPrtAssoc,
                         const fastjet::PseudoJet& superJet,
                         const double superJetGenE, const vector<pair<double,int>>& superJetContaiment,
                         const int nSplits) {

      vector<fastjet::PseudoJet>        subJets;
      vector<fastjet::PseudoJet*>       pickySubJets;
      vector<vector<pair<double,int>>>  subJetContaiments;
      vector<double> subjetGenEs;

      SplitDecision splitDecision = NOSPLIT_NO_PARENTS;
      SplitResult   splitResult = EMPTY;

      if(splittingAlgo == "CAsubjets") {
        fastjet::PseudoJet parent1(0.0,0.0,0.0,0.0), parent2(0.0,0.0,0.0,0.0);
        bool had_parents = doSplitting ? superJet.validated_cs()->has_parents(superJet,parent1,parent2) : false;

        //it must be able to actually split the jets
        if(!had_parents) {
          splitDecision = NOSPLIT_NO_PARENTS;
          splitResult = EMPTY;
        } else {
          if (parent1.perp() < parent2.perp()) std::swap(parent1,parent2);
          subJets.push_back(parent1);
          subJets.push_back(parent2);

          //Now we see how good of a split it was
          splitResult = checkIfGoodSplit(minJetRetainment,minJetAssociation,isGen,partons,partPrtAssoc,superJet,superJetContaiment,subJets,subJetContaiments,subjetGenEs);

          double pt1=parent1.perp();
          double pt2=parent2.perp();
          double totalpt=pt1+pt2;

          if(superJet.m() < mCut){
            splitDecision = NOSPLIT_BELOW_MCUT;
          }
          else if(parent1.plain_distance(parent2) < (rMin*rMin)){
            splitDecision = NOSPLIT_BELOW_DRMIN;
          }
          else if(pt2 > yCut*totalpt && pt2 > minJetPT) {
            splitDecision = SPLIT_BOTH_SUBJETS;
          }
          else if(pt1 > minJetPT) {
            splitDecision = SPLIT_LEADING_SUBJET;
          }
          else {
            splitDecision = NOSPLIT_FAIL_PTCUT;
          }
        }
      }
      else if(splittingAlgo == "Picky") {

        //const double splitMetric = splitter->getSubjets(superJet, pickySubJets);
        const double splitMetric = splittiness.getNSubjettinessSubjets(superJet, pickySubJets);

        //it must be able to actually split the jets
        if(pickySubJets.size() < 2){
          splitDecision = NOSPLIT_NO_PARENTS;
          splitResult = EMPTY;
        }
        else {
          //They can't be pure junk...the subjets must be at least 1 GeV
          bool passPT = true;
          for(const auto* j : pickySubJets){
            subJets.push_back(*j);
            if(j->pt2() >= 1) continue;
            passPT = false;
          }
          if(!passPT){
            splitDecision = NOSPLIT_FAIL_PTCUT;
            splitResult = EMPTY;
            subJets.clear();
            PhysicsUtilities::trash(pickySubJets);
          } else {
            splitResult = checkIfGoodSplit(minJetRetainment,minJetAssociation,isGen,partons,partPrtAssoc,superJet,superJetContaiment,subJets,subJetContaiments,subjetGenEs);
            if(splitter->shouldSplit(superJet, pickySubJets, &splitMetric, 0)) {
              if(nSplits < maxNSplits) splitDecision = PICKY_SHOULD_SPLIT;
              else splitDecision = NOSPLIT_TOO_MANY_SPLITS;
            } else {
              splitDecision = NOSPLIT_FAIL_MVA;
            }
          }
        }

      }

      //plot this splitting
      fillSplitDecision(isGen, superJet,superJetGenE, superJetContaiment, subJets, subjetGenEs, subJetContaiments, splitResult, nSplits, splitDecision);

      bool stopSplitting = false; // apply splitting algorithm decision by hand in analysis code, for now, keep every split! //(splitResult == PURE_TO_SPLIT || splitResult == MIXED_TO_WORSE || splitResult == EMPTY);
      if(doSplitting && !stopSplitting){
        for(unsigned int iS = 0; iS < subJets.size(); ++iS){
          auto& subJet = subJets[iS];
          if(subJet.perp() < minJetPT) continue;
          splitAndFillJet(minJetRetainment,minJetAssociation,minJetPT,isGen,partons,partPrtAssoc,subJet,subjetGenEs[iS],subJetContaiments[iS],nSplits+1);
        }
      }

      PhysicsUtilities::trash(pickySubJets);
      subJets.clear();
      subJetContaiments.clear();
      subjetGenEs.clear();
      return;

    }

    void analyze() {
      //using picky jet filler to be general
      RecoJetFiller * filler = pickyJets;
      const auto& genJets  = *filler->reGenJets_;
      const auto& recoJets = *filler->jets_;

      //Parameters
      const double minPartonPT          = 20;  //Min parton pT for good partons
      const double maxPartonETA         = 2.4; //Max parton eta for good partons
      const double minJetRetainment     = .90; //A pure split must retain at least 90% of the parton E in the superjet
      const double minJetAssociation    = 0.15; //Anything with less than 15% of the total parton E is not counted
      const double minJetPT             = filler->jptMin_; //Don't split jets less than this value

      //get the list of tops
      tops.clear();
      for(const auto& g : *genparticles->genParticles_)
        if(TMath::Abs(g.pdgId()) == ParticleInfo::p_t && ParticleUtilities::isLastInChain(&g))
          tops.push_back(&g);
      if(tops.size() == 0) return;

      //Do the parton matching to the first set of jets
      const TopDecayMatching::Partons& partons = genparticles->getPartons(genJets);
      const vector<int>& partPrtAssoc = genparticles->getPartonPrtAssoc();
      const vector<float>& hadE = genparticles->getHadronizedE();

      //make a list of the partons that come from top...we don't care about the radiated jets
      topPartonIndex.clear();
      topIndex.clear();
      topPartons.clear();
      topPartonsHadE.clear();
      vector<bool> goodPartons(partons.size(),false);
      ngoodpartons = 0;
      for(unsigned int iP = 0; iP < partons.size(); ++iP){
        for(unsigned int it = 0; it < tops.size(); ++it) {
          const auto* t = tops.at(it);
          if(ParticleUtilities::isAncestor(t,&*partons[iP].parton)) {
            goodPartons[iP] = true;
            topPartons.push_back(partons[iP]);
            topPartonsHadE.push_back(hadE[partons[iP].storedIndex]);
            topPartonIndex[iP] = topPartons.size()-1;
            topIndex.push_back(int(it));
            if(partons[iP].pt() > minPartonPT && fabs(partons[iP].eta()) < maxPartonETA) ngoodpartons++;
            break;
          }
        }
      }

      for(unsigned int iJ = 0; iJ < recoJets.size(); ++iJ){
        const auto&  j = recoJets.at(iJ);
        auto gj = filler->getReGenJet(j,iJ,false);
        if(j.pt() < minJetPT) continue;
        if(gj.isNull()) continue;
        //the initial vector of interesting particles in this jet
        vector<pair<double,int>> containment;
        getContainment(goodPartons,minJetAssociation,minPartonPT,maxPartonETA,partons,gj.key(),containment);
        //cout << j.pt() << " " << j.eta() << " " << j.phi() << " " << j.mass() << " " << containment.size() << endl;
        if(containment.size() == 0) continue;

        //Get the PSuedo jet version
        fastjet::PseudoJet superJet = addParticles(pfCandidates , &j, genparticles->packedGenParticles_, &*gj);

        if(doSplitting) {
          if(splittingAlgo == "CAsubjets") {
            //Use CA for declustering
            fastjet::JetDefinition jetDef(fastjet::cambridge_algorithm, fastjet::JetDefinition::max_allowable_R);

            fastjet::ClusterSequence clust_seq(superJet.constituents(), jetDef);
            vector<fastjet::PseudoJet> ca_jets = sorted_by_pt(clust_seq.inclusive_jets());

            splitAndFillJet(minJetRetainment,minJetAssociation,minJetPT,false,
                partons, partPrtAssoc,
                ca_jets[0], gj->energy(), containment, 0);
          }
          else if(splittingAlgo == "Picky") {
            splitAndFillJet(minJetRetainment,minJetAssociation,minJetPT,false,
                partons, partPrtAssoc,
                superJet, gj->energy(), containment, 0);
          }
          else {
            throw cms::Exception("UnknownAlgo") << "splittingAlgo " << splittingAlgo << " unknown! Use either CAsubjets or Picky\n";
          }
        } else {
          splitAndFillJet(minJetRetainment,minJetAssociation,minJetPT,false,
              partons, partPrtAssoc,
              superJet, gj->energy(), containment, 0);
        }
      }

      for(unsigned int iJ = 0; iJ < genJets.size(); ++iJ){
        const auto&  j = genJets.at(iJ);
        if(j.pt() < minJetPT) continue;

        //the initial vector of interesting particles in this jet
        vector<pair<double,int>> containment;
        getContainment(goodPartons,minJetAssociation,minPartonPT,maxPartonETA,partons,iJ,containment);
        if(containment.size() == 0) continue;

        //Get the PSuedo jet version
        fastjet::PseudoJet superJet = addParticles(genparticles->packedGenParticles_, &j, genparticles->packedGenParticles_, 0);

        if(doSplitting) {
          if(splittingAlgo == "CAsubjets") {
            //Use CA for declustering
            fastjet::JetDefinition jetDef(fastjet::cambridge_algorithm, fastjet::JetDefinition::max_allowable_R);

            fastjet::ClusterSequence clust_seq(superJet.constituents(), jetDef);
            vector<fastjet::PseudoJet> ca_jets = sorted_by_pt(clust_seq.inclusive_jets());

            splitAndFillJet(minJetRetainment,minJetAssociation,minJetPT,true,
              partons,partPrtAssoc,
              ca_jets[0], j.energy(), containment, 0);
          }
          else if(splittingAlgo == "Picky") {
            splitAndFillJet(minJetRetainment,minJetAssociation,minJetPT,true,
                partons, partPrtAssoc,
                superJet, j.energy(), containment, 0);
          }
          else {
            throw cms::Exception("UnknownAlgo") << "splittingAlgo " << splittingAlgo << "unknown! Use either CAsubjets or Picky\n";
          }
        } else {
          splitAndFillJet(minJetRetainment,minJetAssociation,minJetPT,true,
              partons,partPrtAssoc,
              superJet, j.energy(),containment,0);
        }
      }

    }
};

DEFINE_FWK_MODULE(CASubjetCountingTreeProducer);
