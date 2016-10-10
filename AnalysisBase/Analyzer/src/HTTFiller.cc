//--------------------------------------------------------------------------------------------------
// 
// FatJetFiller
// 
// Muon filler.
// 
// FatJetFiller.cc created on Fri Oct 17 12:11:09 CEST 2014 
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisBase/Analyzer/interface/HTTFiller.h"
#include "AnalysisBase/Analyzer/interface/JetFiller.h"

using namespace ucsbsusy;

//--------------------------------------------------------------------------------------------------
HTTFiller::HTTFiller(const edm::ParameterSet& cfg, edm::ConsumesCollector && cc, const int options, const string branchName) :
  BaseFiller(options, branchName)
{

  auto jetTag = cfg.getParameter<edm::InputTag>("jetPrefix");
  fatJetToken = cc.consumes<reco::BasicJetCollection>(edm::InputTag(jetTag.label() +"HTT"));
  fatJetInfosToken = cc.consumes<reco::HTTTopJetTagInfoCollection>(edm::InputTag(jetTag.label()+"HTT"));
  bTagsToken = cc.consumes<reco::JetTagCollection>(edm::InputTag(jetTag.label() + "HTTpfCombinedInclusiveSecondaryVertexV2BJetTags"));
  sdJetToken  = cc.consumes<reco::PFJetCollection>(edm::InputTag(jetTag.label() + "SoftdropZ2B1"));
  tau1Token = cc.consumes<edm::ValueMap<float>>(edm::InputTag(jetTag.label()+"SoftdropZ2B1njettiness","tau1"));
  tau2Token = cc.consumes<edm::ValueMap<float>>(edm::InputTag(jetTag.label()+"SoftdropZ2B1njettiness","tau2"));
  tau3Token = cc.consumes<edm::ValueMap<float>>(edm::InputTag(jetTag.label()+"SoftdropZ2B1njettiness","tau3"));


  i_pt          = data.addMulti<float>(branchName_,"fatjet_pt"       ,0);
  i_eta         = data.addMulti<float>(branchName_,"fatjet_eta"      ,0);
  i_phi         = data.addMulti<float>(branchName_,"fatjet_phi"      ,0);
  i_mass        = data.addMulti<float>(branchName_,"fatjet_mass"     ,0);
  i_csv         = data.addMulti<float>(branchName_,"fatjet_csv"      ,0);
  i_ropt        = data.addMulti<float>(branchName_,"fatjet_ropt"     ,0);
  i_frec        = data.addMulti<float>(branchName_,"fatjet_frec"     ,0);
  i_roptcalc    = data.addMulti<float>(branchName_,"fatjet_roptcalc" ,0);
  i_ptforropt   = data.addMulti<float>(branchName_,"fatjet_ptforropt",0);
  i_ropt_pt     = data.addMulti<float>(branchName_,"fatjet_ropt_pt"  ,0);
  i_ropt_eta    = data.addMulti<float>(branchName_,"fatjet_ropt_eta" ,0);
  i_ropt_phi    = data.addMulti<float>(branchName_,"fatjet_ropt_phi" ,0);
  i_ropt_mass   = data.addMulti<float>(branchName_,"fatjet_ropt_mass",0);
  i_ropt_tau1   = data.addMulti<float>(branchName_,"fatjet_ropt_tau1",0);
  i_ropt_tau2   = data.addMulti<float>(branchName_,"fatjet_ropt_tau2",0);
  i_ropt_tau3   = data.addMulti<float>(branchName_,"fatjet_ropt_tau3",0);
  i_sd_mass     = data.addMulti<float>(branchName_,"fatjet_sd_mass"  ,0);
  i_sd_tau1     = data.addMulti<float>(branchName_,"fatjet_sd_tau1"  ,0);
  i_sd_tau2     = data.addMulti<float>(branchName_,"fatjet_sd_tau2"  ,0);
  i_sd_tau3     = data.addMulti<float>(branchName_,"fatjet_sd_tau3"  ,0);
  i_nsubjets    = data.addMulti<int>  (branchName_,"fatjet_nsubjets" ,0);
  i_w1_pt       = data.addMulti<float>(branchName_,"fatjet_w1_pt"    ,0);
  i_w1_eta      = data.addMulti<float>(branchName_,"fatjet_w1_eta"   ,0);
  i_w1_phi      = data.addMulti<float>(branchName_,"fatjet_ws1_phi"  ,0);
  i_w1_mass     = data.addMulti<float>(branchName_,"fatjet_w1_mass"  ,0);
  i_w1_csv      = data.addMulti<float>(branchName_,"fatjet_w1_csv"   ,0);
  i_w2_pt       = data.addMulti<float>(branchName_,"fatjet_w2_pt"    ,0);
  i_w2_eta      = data.addMulti<float>(branchName_,"fatjet_w2_eta"   ,0);
  i_w2_phi      = data.addMulti<float>(branchName_,"fatjet_w2_phi"   ,0);
  i_w2_mass     = data.addMulti<float>(branchName_,"fatjet_w2_mass"  ,0);
  i_w2_csv      = data.addMulti<float>(branchName_,"fatjet_w2_csv"   ,0);
  i_b_pt        = data.addMulti<float>(branchName_,"fatjet_b_pt"     ,0);
  i_b_eta       = data.addMulti<float>(branchName_,"fatjet_b_eta"    ,0);
  i_b_phi       = data.addMulti<float>(branchName_,"fatjet_b_phi"    ,0);
  i_b_mass      = data.addMulti<float>(branchName_,"fatjet_b_mass"   ,0);
  i_b_csv       = data.addMulti<float>(branchName_,"fatjet_b_csv"    ,0);

}

//--------------------------------------------------------------------------------------------------
void HTTFiller::load(const edm::Event& iEvent, const edm::EventSetup &iSetup)
{
  reset();
  iEvent.getByToken(fatJetToken,fatJets);
  iEvent.getByToken(fatJetInfosToken,fatJetInfos);
  iEvent.getByToken(bTagsToken,btags);

  iEvent.getByToken(sdJetToken,sdFatJets);
  iEvent.getByToken(tau1Token,tau1s);
  iEvent.getByToken(tau2Token,tau2s);
  iEvent.getByToken(tau3Token,tau3s);

  isLoaded_ = true;
}



vector<int> HTTFiller::getSDInd(){
  vector<bool> isMatched(sdFatJets->size(),false);
  vector<int> matchInds(fatJets->size(),-1);

  for(unsigned int iFJ = 0; iFJ < fatJets->size(); ++iFJ){
    double nearDR = 0;
    int matchIDX = PhysicsUtilities::findNearestDR(fatJets->at(iFJ), *sdFatJets,nearDR,5.0,0,&isMatched);
    if(matchIDX >= 0){
      isMatched[matchIDX] = true;
      matchInds[iFJ] = matchIDX;
    }
  }

  return matchInds;
}

//--------------------------------------------------------------------------------------------------
void HTTFiller::fill()
{

  auto sdMatchInds = getSDInd();

  for(unsigned int iJ = 0; iJ < fatJets->size(); ++iJ){
    const auto &fatjet = fatJets->at(iJ);
    const auto &fatjetInfo = fatJetInfos->at(iJ);


    data.fillMulti<float>(i_pt           , fatjetInfo.properties().fjPt);
    data.fillMulti<float>(i_eta          , fatjetInfo.properties().fjEta);
    data.fillMulti<float>(i_phi          , fatjetInfo.properties().fjPhi);
    data.fillMulti<float>(i_mass         , fatjetInfo.properties().fjMass);


    data.fillMulti<float>(i_frec              , fatjetInfo.properties().fRec);
    data.fillMulti<float>(i_ropt              , fatjetInfo.properties().Ropt);
    data.fillMulti<float>(i_roptcalc          , fatjetInfo.properties().RoptCalc);
    data.fillMulti<float>(i_ptforropt         , fatjetInfo.properties().ptForRoptCalc);
    data.fillMulti<float>(i_ropt_pt           , fatjet.pt());
    data.fillMulti<float>(i_ropt_eta          , fatjet.eta());
    data.fillMulti<float>(i_ropt_phi          , fatjet.phi());
    data.fillMulti<float>(i_ropt_mass         , fatjet.mass());

    data.fillMulti<float>(i_ropt_tau1         , fatjetInfo.properties().tau1Filtered);
    data.fillMulti<float>(i_ropt_tau2         , fatjetInfo.properties().tau2Filtered);
    data.fillMulti<float>(i_ropt_tau3         , fatjetInfo.properties().tau3Filtered);

    int nSJ = fatjet.getJetConstituents().size();
    data.fillMulti<int>(i_nsubjets            , nSJ);


    data.fillMulti<float>(i_w1_pt          , nSJ > 0 ? fatjet.getJetConstituents()[0]->pt()   : 0);
    data.fillMulti<float>(i_w1_eta         , nSJ > 0 ? fatjet.getJetConstituents()[0]->eta()  : 0);
    data.fillMulti<float>(i_w1_phi         , nSJ > 0 ? fatjet.getJetConstituents()[0]->phi()  : 0);
    data.fillMulti<float>(i_w1_mass        , nSJ > 0 ? fatjet.getJetConstituents()[0]->mass() : 0);
    data.fillMulti<float>(i_w1_csv         , nSJ > 0 ? getCSV(*fatjet.getJetConstituents()[0]): 0 );

    data.fillMulti<float>(i_w2_pt          , nSJ > 1 ? fatjet.getJetConstituents()[1]->pt()   : 0);
    data.fillMulti<float>(i_w2_eta         , nSJ > 1 ? fatjet.getJetConstituents()[1]->eta()  : 0);
    data.fillMulti<float>(i_w2_phi         , nSJ > 1 ? fatjet.getJetConstituents()[1]->phi()  : 0);
    data.fillMulti<float>(i_w2_mass        , nSJ > 1 ? fatjet.getJetConstituents()[1]->mass() : 0);
    data.fillMulti<float>(i_w2_csv         , nSJ > 1 ? getCSV(*fatjet.getJetConstituents()[1]): 0 );

    data.fillMulti<float>(i_b_pt          , nSJ > 2 ? fatjet.getJetConstituents()[2]->pt()   : 0);
    data.fillMulti<float>(i_b_eta         , nSJ > 2 ? fatjet.getJetConstituents()[2]->eta()  : 0);
    data.fillMulti<float>(i_b_phi         , nSJ > 2 ? fatjet.getJetConstituents()[2]->phi()  : 0);
    data.fillMulti<float>(i_b_mass        , nSJ > 2 ? fatjet.getJetConstituents()[2]->mass() : 0);
    data.fillMulti<float>(i_b_csv         , nSJ > 2 ? getCSV(*fatjet.getJetConstituents()[2]): 0 );

    data.fillMulti<float>(i_sd_mass         , sdMatchInds[iJ] >= 0 ? sdFatJets->at(sdMatchInds[iJ]).mass() : 0);
    edm::Ptr<reco::PFJet> sdJetPtr(sdFatJets,sdMatchInds[iJ]);
    data.fillMulti<float>(i_sd_tau1         , sdMatchInds[iJ] >= 0 ? (*tau1s)[sdJetPtr] : 0);
    data.fillMulti<float>(i_sd_tau2         , sdMatchInds[iJ] >= 0 ? (*tau2s)[sdJetPtr] : 0);
    data.fillMulti<float>(i_sd_tau3         , sdMatchInds[iJ] >= 0 ? (*tau3s)[sdJetPtr] : 0);
  }


    isFilled_ = true;

  }

