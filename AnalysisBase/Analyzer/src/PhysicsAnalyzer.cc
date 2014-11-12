#include <iostream>

#include "AnalysisBase/Analyzer/interface/PhysicsAnalyzer.h"
#include "AnalysisBase/Analyzer/interface/FileUtilities.h"
#include "AnalysisTools/TreeReader/interface/Defaults.h"

using namespace ucsbsusy;
using namespace std;


//--------------------------------------------------------------------------------------------------
PhysicsAnalyzer::PhysicsAnalyzer(const edm::ParameterSet& iConfig)
: BaseAnalyzer(iConfig)
, isRealData          (iConfig.getParameter<int             >("isData"                 ))
, globalTag           (iConfig.getParameter<string          >("globalTag"       ).data())
// ---- Configure event information
, eventInfo           (0)
, ak4Jets             (0)
, muons               (0)
, electrons           (0)
, taus                (0)
, genparticles        (0)
{

  //-- Dataset info -----------------------------------------------------------
  if(globalTag.Length())
  clog << " ++  globalTag           = " << globalTag                      << endl;
  clog << " ++  dataset             = ";
  if      (isRealData == 1)       clog << "  ...  \033[31mDATA\033[0m";
  else if (isRealData == 0)       clog << "  ...  \033[1;34mMC\033[0m";
  clog << endl;
}

//--------------------------------------------------------------------------------------------------
PhysicsAnalyzer::~PhysicsAnalyzer() {}

//--------------------------------------------------------------------------------------------------
void PhysicsAnalyzer::beginJob() {}

//--------------------------------------------------------------------------------------------------
bool PhysicsAnalyzer::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  return BaseAnalyzer::filter(iEvent,iSetup);
}
//--------------------------------------------------------------------------------------------------
void PhysicsAnalyzer::book()
{
  for(auto f : initializedFillers) {f->book(*treeWriter());}}
//--------------------------------------------------------------------------------------------------
bool PhysicsAnalyzer::load(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  for(auto f : initializedFillers) {f->load(iEvent); }
  return true;
}
//--------------------------------------------------------------------------------------------------
void PhysicsAnalyzer::fill()
{
  for(auto f : initializedFillers) {f->fill();}
  treeWriter()->fill();
}

//--------------------------------------------------------------------------------------------------
void PhysicsAnalyzer::initialize(const edm::ParameterSet& cfg, VarType type, int options, std::string branchName){
  switch (type) {
    case EVTINFO : {
      eventInfo = new EventInfoFiller(
          cfg.getParameter<edm::InputTag>("vertices")
         ,cfg.getParameter<edm::InputTag>("rho")
         ,cfg.getParameter<edm::InputTag>("mets")
         );
      initializedFillers.push_back(eventInfo);
      break;
    }
    case GENPARTICLES : {
      int defaultOptions = GenParticleFiller::defaultOptions | (cfg.getUntrackedParameter<bool>("saveAllGenParticles") ? GenParticleFiller::SAVEALL : GenParticleFiller::NULLOPT);
      genparticles = new GenParticleFiller(options < 0 ? defaultOptions : options, branchName == "" ? "gen" : branchName,
          cfg.getParameter<edm::InputTag>("prunedGenParticles")
         );
      initializedFillers.push_back(genparticles);
      break;
    }
    case AK4JETS : {
      int defaultOptions = PatJetFiller::defaultOptions | ((isMC() && cfg.getUntrackedParameter<bool>("fillJetGenInfo")) ? PatJetFiller::LOADGEN : PatJetFiller::NULLOPT);
      if(cfg.getUntrackedParameter<bool>("fillJetShapeInfo")) defaultOptions |= PatJetFiller::LOADJETSHAPE;
      ak4Jets = new PatJetFiller( options < 0 ? defaultOptions : options, branchName == "" ? defaults::BRANCH_AK4JETS : branchName, defaults::BRANCH_STDGENJETS, eventInfo
                             , cfg.getParameter<edm::InputTag>("jets")
                             , cfg.getParameter<edm::InputTag>("reGenJets")
                             , cfg.getParameter<edm::InputTag>("stdGenJets")
                             , cfg.getUntrackedParameter<bool>("fillReGenJets")
                             , cfg.getUntrackedParameter<double>("minJetPt")
      );
      initializedFillers.push_back(ak4Jets);
      break;
    }
    case ELECTRONS : {
      electrons = new ElectronFiller(cfg, isMC());
      initializedFillers.push_back(electrons);
      break;
    }
    case MUONS : {
      muons = new MuonFiller(cfg, isMC());
      initializedFillers.push_back(muons);
      break;
    }
    case TAUS : {
      taus = new TauFiller(cfg);
      initializedFillers.push_back(taus);
      break;
    }
    default : {
      cout << endl << "No settings for type: " << type << " found!" << endl;
      break;
    }
  }
}
//--------------------------------------------------------------------------------------------------
void PhysicsAnalyzer::initialize(BaseFiller * filler){initializedFillers.push_back(filler);}
//--------------------------------------------------------------------------------------------------
bool PhysicsAnalyzer::isData() const
{
  if (isRealData < 0)
    throw cms::Exception("PhysicsAnalyzer.isData()", "isRealData not set -- This should only be used if the sample information has been loaded from the database, or when processing the first event.");
  return  isRealData;
}

//--------------------------------------------------------------------------------------------------
bool PhysicsAnalyzer::isMC() const
{
  if (isRealData < 0)
    throw cms::Exception("PhysicsAnalyzer.isData()", "isRealData not set -- This should only be used if the sample information has been loaded from the database, or when processing the first event.");
  return !isRealData;
}
