#include "AnalysisBase/Analyzer/interface/PhysicsAnalyzer.h"
#include "AnalysisBase/Analyzer/interface/RecoJetFiller.h"
#include "AnalysisTools/TreeReader/interface/Defaults.h"
#include "ObjectProducers/JetProducers/interface/Splittiness.h"

using namespace ucsbsusy;
using namespace std;

struct JetVars{
  size superJet_pt          ;
  size superJet_eta         ;
  size superJet_phi         ;
  size superJet_mass        ;
  size tau1                 ;
  size tau2                 ;
  size subJet_1_pt          ;
  size subJet_1_eta         ;
  size subJet_1_phi         ;
  size subJet_1_mass        ;
  size subJet_2_pt          ;
  size subJet_2_eta         ;
  size subJet_2_phi         ;
  size subJet_2_mass        ;
  size subJet_1_pu_E        ;
  size subJet_2_pu_E        ;
  size highest_peak         ;
  size lowest_peak          ;
  size minimum_value        ;
  size center_value         ;
  size lowest_peak_location ;
  size highest_peak_location;
  size minimum_location     ;
  size splitDisc            ;
  size shouldSplit          ;

 JetVars() {};

JetVars(const std::string name, TreeWriterData* data){
  superJet_pt           = data->addMulti<float>(name,"superJet_pt"          ,0);
  superJet_eta          = data->addMulti<float>(name,"superJet_eta"         ,0);
  superJet_phi          = data->addMulti<float>(name,"superJet_phi"         ,0);
  superJet_mass         = data->addMulti<float>(name,"superJet_mass"        ,0);
  tau1                  = data->addMulti<float>(name,"tau1"                 ,0);
  tau2                  = data->addMulti<float>(name,"tau2"                 ,0);
  subJet_1_pt           = data->addMulti<float>(name,"subJet_1_pt"          ,0);
  subJet_1_eta          = data->addMulti<float>(name,"subJet_1_eta"         ,0);
  subJet_1_phi          = data->addMulti<float>(name,"subJet_1_phi"         ,0);
  subJet_1_mass         = data->addMulti<float>(name,"subJet_1_mass"        ,0);
  subJet_2_pt           = data->addMulti<float>(name,"subJet_2_pt"          ,0);
  subJet_2_eta          = data->addMulti<float>(name,"subJet_2_eta"         ,0);
  subJet_2_phi          = data->addMulti<float>(name,"subJet_2_phi"         ,0);
  subJet_2_mass         = data->addMulti<float>(name,"subJet_2_mass"        ,0);
  subJet_1_pu_E         = data->addMulti<float>(name,"subJet_1_pu_E"        ,0);
  subJet_2_pu_E         = data->addMulti<float>(name,"subJet_2_pu_E"        ,0);
  highest_peak          = data->addMulti<float>(name,"highest_peak"         ,0);
  lowest_peak           = data->addMulti<float>(name,"lowest_peak"          ,0);
  minimum_value         = data->addMulti<float>(name,"minimum_value"        ,0);
  center_value          = data->addMulti<float>(name,"center_value"         ,0);
  lowest_peak_location  = data->addMulti<float>(name,"lowest_peak_location" ,0);
  highest_peak_location = data->addMulti<float>(name,"highest_peak_location",0);
  minimum_location      = data->addMulti<float>(name,"minimum_location"     ,0);
  splitDisc             = data->addMulti<float>(name,"splitDisc"            ,0);
  shouldSplit           = data->addMulti<int  >(name,"shouldSplit"          ,0);
}

};


class SubJetFiller : public JetFillerBase, public BaseFiller {
protected:
  const bool          runOnPuppi_;
  const edm::InputTag jetTag_;
  const edm::InputTag genPrtSrc_;
  const edm::InputTag prtSrc_;
  const double        jptMin_;
public:
  edm::Handle<reco::PFJetCollection>       jets_;
  edm::Handle<edm::ValueMap<reco::CandidatePtr> >  genJetsPtr_;
  edm::Handle<reco::GenJetCollection> reGenJets_;
  edm::Handle<reco::PFJetCollection>     puJets_;
  edm::Handle<edm::ValueMap<reco::CandidatePtr> >  puJetsPtr_;
  edm::Handle<pat::PackedGenParticleCollection> genParticles;
  edm::Handle<pat::PackedCandidateCollection> pfCandidates;
  edm::Handle<reco::PFCandidateCollection> pfPuppiCandidates;
  Splittiness * splittiness;
public:
  JetVars* recoJetVars;
  JetVars* genJetVars;

public:
  SubJetFiller(const int options, const string branchName, const bool runOnPuppi, const edm::InputTag jetTag,const edm::InputTag genPrtSrc,const edm::InputTag prtSrc, const double jptMin, Splittiness * splittiness_ ) : BaseFiller(options, branchName)
  , runOnPuppi_      (runOnPuppi)
  , jetTag_          (jetTag)
  , genPrtSrc_       (genPrtSrc)
  , prtSrc_          (prtSrc)
  , jptMin_          (jptMin)
  , jets_            (0)
  , reGenJets_       (0)
  , splittiness      (splittiness_)
  , recoJetVars      (new JetVars(branchName + "_reco",&data))
  , genJetVars       ((options_ & LOADGEN) ? new JetVars(branchName + "_gen",&data) : new JetVars())
  {}
  virtual ~SubJetFiller() {delete recoJetVars; delete genJetVars;}
  virtual void load(const edm::Event& iEvent) {
    reset();
    FileUtilities::enforceGet(iEvent,jetTag_,jets_,true);

    if(runOnPuppi_) FileUtilities::enforceGet(iEvent,prtSrc_,pfPuppiCandidates,true);
    else FileUtilities::enforceGet(iEvent,prtSrc_,pfCandidates,true);

    if(options_ & LOADGEN){
      FileUtilities::enforceGet(iEvent,edm::InputTag(jetTag_.label(),"Gen"),reGenJets_,true);
      FileUtilities::enforceGet(iEvent,edm::InputTag(jetTag_.label(),"GenPtr"),genJetsPtr_,true);
      FileUtilities::enforceGet(iEvent,genPrtSrc_,genParticles,true);
    }
    FileUtilities::enforceGet(iEvent,edm::InputTag(jetTag_.label(),"PU"),puJets_,false);
    FileUtilities::enforceGet(iEvent,edm::InputTag(jetTag_.label(),"PUPtr"),puJetsPtr_,false);
  }

  template<typename Prt,typename Prt2>
  fastjet::PseudoJet addParticles(const edm::Handle<vector<Prt>>& prtCol, const reco::Jet* jet,const edm::Handle<vector<Prt2>>& puPrtCol, const reco::Jet* puJet){
    vector<fastjet::PseudoJet> jetlist;
    for(unsigned int iD = 0; iD < jet->numberOfDaughters(); ++iD){
      const reco::CandidatePtr  daughter = jet->daughterPtr(iD);
      jetlist.emplace_back(daughter->px(),daughter->py(),daughter->pz(),daughter->energy());
      jetlist.back().set_user_info(new FastJetClusterer::UserInfo(prtCol,daughter.key(), FastJetClusterer::RECO));
    }

    const double puScale = 1e-50;
    if(puJet)
    for(unsigned int iD = 0; iD < puJet->numberOfDaughters(); ++iD){
      const reco::CandidatePtr  daughter = puJet->daughterPtr(iD);
      jetlist.emplace_back(puScale*daughter->px(),puScale*daughter->py(),puScale*daughter->pz(),puScale*daughter->energy());
      jetlist.back().set_user_info(new FastJetClusterer::UserInfo(puPrtCol,daughter.key(), FastJetClusterer::PU));
    }
    return fastjet::join(jetlist);
  }


  template<typename Prt,typename Prt2>
  void fillJet(JetVars * vars, const edm::Handle<vector<Prt>>& prtCol, const reco::Jet* jet,const edm::Handle<vector<Prt2>>& puPrtCol, const reco::Jet* puJet){

    fastjet::PseudoJet superJet = addParticles(prtCol, jet, puPrtCol, puJet);

//    cout<<"--> " << superJet.pt() <<","<< superJet.eta() <<","<< superJet.phi() <<endl;
//    for(const auto& c : superJet.constituents())
//      cout<< c.pt() <<","<< c.eta() <<","<< c.phi() << ": "<<c.user_info<FastJetClusterer::UserInfo>().type<<endl;
    std::vector<fastjet::PseudoJet*>      subJets;

    double nSub2 = splittiness->getNSubjettinessSubjets(superJet, subJets);
    double nSub1 = splittiness->nSubjettiness.getTau(1, superJet.constituents());



    for (int iSub = subJets.size(); --iSub >= 0;)
      if (subJets[iSub]->pt2() < 1e-10) {
        delete subJets[iSub];
        subJets[iSub]                     = subJets.back();
        subJets.pop_back();
      }
    assert(subJets.size());



    data.fillMulti<float>(vars->superJet_pt           ,superJet.pt());
    data.fillMulti<float>(vars->superJet_eta          ,superJet.eta());
    data.fillMulti<float>(vars->superJet_phi          ,superJet.phi());
    data.fillMulti<float>(vars->superJet_mass         ,superJet.m());
    data.fillMulti<float>(vars->tau1                  ,nSub1);
    data.fillMulti<float>(vars->tau2                  ,nSub2);


    if(subJets.size() == 2){
      double splitResult = -10;
      bool   shouldSplit = splittiness->shouldSplitNSubjettiness(superJet, subJets, nSub2, &splitResult);
      const Splittiness::JetDeposition *       jetStuff      = splittiness->getJetStuff();
      assert(jetStuff);
      data.fillMulti<float>(vars->subJet_1_pt           ,subJets[0]->pt() );
      data.fillMulti<float>(vars->subJet_1_eta          ,subJets[0]->eta());
      data.fillMulti<float>(vars->subJet_1_phi          ,subJets[0]->phi());
      data.fillMulti<float>(vars->subJet_1_mass         ,subJets[0]->m()  );
      data.fillMulti<float>(vars->subJet_2_pt           ,subJets[1]->pt() );
      data.fillMulti<float>(vars->subJet_2_eta          ,subJets[1]->eta());
      data.fillMulti<float>(vars->subJet_2_phi          ,subJets[1]->phi());
      data.fillMulti<float>(vars->subJet_2_mass         ,subJets[1]->m()  );
      data.fillMulti<float>(vars->subJet_1_pu_E         ,jetStuff->subJetPUEnergy[0]);
      data.fillMulti<float>(vars->subJet_2_pu_E         ,jetStuff->subJetPUEnergy[1]);
      data.fillMulti<float>(vars->highest_peak          ,jetStuff->lobes.getHighestPeak()                                       );
      data.fillMulti<float>(vars->lowest_peak           ,jetStuff->lobes.isUnimodal() ? -0.1 : jetStuff->lobes.getLowestPeak  ());
      data.fillMulti<float>(vars->minimum_value         ,jetStuff->lobes.isUnimodal() ? -0.1 : jetStuff->lobes.getMinimumValue());
      data.fillMulti<float>(vars->center_value          ,jetStuff->centerValue                                                  );
      data.fillMulti<float>(vars->lowest_peak_location  ,jetStuff->lobes.getLowestLocation () - jetStuff->centerLocation        );
      data.fillMulti<float>(vars->highest_peak_location ,jetStuff->lobes.getHighestLocation() - jetStuff->centerLocation        );
      data.fillMulti<float>(vars->minimum_location      ,jetStuff->lobes.getMinimumLocation() - jetStuff->centerLocation        );
      data.fillMulti<float>(vars->splitDisc             ,splitResult);
      data.fillMulti<int  >(vars->shouldSplit           ,shouldSplit);
    } else{
    data.fillMulti<float>(vars->subJet_1_pt          );
    data.fillMulti<float>(vars->subJet_1_eta         );
    data.fillMulti<float>(vars->subJet_1_phi         );
    data.fillMulti<float>(vars->subJet_1_mass        );
    data.fillMulti<float>(vars->subJet_2_pt          );
    data.fillMulti<float>(vars->subJet_2_eta         );
    data.fillMulti<float>(vars->subJet_2_phi         );
    data.fillMulti<float>(vars->subJet_2_mass        );
    data.fillMulti<float>(vars->subJet_1_pu_E        );
    data.fillMulti<float>(vars->subJet_2_pu_E        );
    data.fillMulti<float>(vars->highest_peak         );
    data.fillMulti<float>(vars->lowest_peak          );
    data.fillMulti<float>(vars->minimum_value        );
    data.fillMulti<float>(vars->center_value         );
    data.fillMulti<float>(vars->lowest_peak_location );
    data.fillMulti<float>(vars->highest_peak_location);
    data.fillMulti<float>(vars->minimum_location     );
    data.fillMulti<float>(vars->splitDisc            );
    data.fillMulti<int  >(vars->shouldSplit          );
    }
    PhysicsUtilities::trash(subJets);


  }
  void fill(){
    if(options_ & LOADGEN){
      const reco::GenJetCollection& genJets = (*reGenJets_.product());
      for (unsigned int iJ = 0; iJ < genJets.size(); ++iJ){
        const reco::GenJet& jet = genJets[iJ];
        if(jet.pt() < jptMin_){continue;}
        if(jet.nConstituents() < 3) continue;
        fillJet(genJetVars, genParticles, &jet, pfCandidates, 0 );
      }
    }

    for (unsigned int iJ = 0; iJ < jets_->size(); ++iJ){
      const reco::PFJet& jet = (*jets_)[iJ];
      if(jet.pt() < jptMin_){continue;}
      if(jet.nConstituents() < 3) continue;
      if(runOnPuppi_) fillJet(recoJetVars, pfPuppiCandidates, &jet, pfPuppiCandidates, puJets_.isValid() ? &puJets_->at(iJ) : 0);
      else fillJet(recoJetVars, pfCandidates, &jet, pfCandidates,puJets_.isValid() ? &puJets_->at(iJ) : 0 );
    }
  }
};

class JetSubstructureAnalyzer : public PhysicsAnalyzer {
  public:
  Splittiness                         splittiness;
    ANALYZER_MODE
    JetSubstructureAnalyzer(const edm::ParameterSet &cfg) : PhysicsAnalyzer(cfg) , splittiness      (cfg,false)
    {
      initialize(cfg,"EventInfo",EVTINFO);
      if(isMC()) initialize(cfg,"Gen",GENPARTICLES);
      SubJetFiller * jetFiller = new SubJetFiller( SubJetFiller::LOADGEN, "CA1",cfg.getParameter<bool>("runOnPuppi"), cfg.getParameter<edm::InputTag>("recoJets"), cfg.getParameter<edm::InputTag>("genParts"), cfg.getParameter<edm::InputTag>("recoParts"),30, &splittiness);
      initialize(jetFiller);
      book();
    }
    ~JetSubstructureAnalyzer() {}
    void analyze() {fill();}
};

DEFINE_FWK_MODULE(JetSubstructureAnalyzer);
