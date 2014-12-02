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
PhysicsAnalyzer::~PhysicsAnalyzer() {for(auto f : initializedFillers) delete f; }

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
void PhysicsAnalyzer::initialize(const edm::ParameterSet& cfg, const std::string pSetName, const VarType type, const int options, const std::string branchName){
  if(cfg.existsAs<edm::ParameterSet>(pSetName, false)) {
    edm::ParameterSet fcfg(cfg.getUntrackedParameter<edm::ParameterSet>(pSetName));   // parameter set for this filler
    bool fill = fcfg.getUntrackedParameter<bool>("isFilled");                         // to fill or not to fill
    if(fill) initialize(fcfg, type, options, branchName);
  }
}

//--------------------------------------------------------------------------------------------------
void PhysicsAnalyzer::initialize(const edm::ParameterSet& cfg, const VarType type, const int options, const std::string branchName){
  switch (type) {

    case EVTINFO : {
      eventInfo = new EventInfoFiller(cfg.getParameter<edm::InputTag>("vertices"),
                                      cfg.getParameter<edm::InputTag>("rho"),
                                      cfg.getParameter<edm::InputTag>("mets")
                                      );
      initializedFillers.push_back(eventInfo);
      break;
    }

    case GENPARTICLES : {
      int defaultOptions = GenParticleFiller::defaultOptions;
      if(cfg.getUntrackedParameter<bool>("saveAllGenParticles")) defaultOptions |= GenParticleFiller::SAVEALL;

      genparticles = new GenParticleFiller(options < 0 ? defaultOptions : options,
                                           branchName == "" ? defaults::BRANCH_GENPARTS : branchName,
                                           cfg.getParameter<edm::InputTag>("prunedGenParticles"),
                                           cfg.getParameter<edm::InputTag>("packedGenParticles")
                                           );
      initializedFillers.push_back(genparticles);
      break;
    }

    case AK4JETS : {
      int defaultOptions = PatJetFiller::defaultOptions; 
      if((isMC() && cfg.getUntrackedParameter<bool>("fillJetGenInfo"))) defaultOptions |= PatJetFiller::LOADGEN;
      if(cfg.getUntrackedParameter<bool>("fillJetShapeInfo"))           defaultOptions |= PatJetFiller::LOADJETSHAPE;

      ak4Jets = new PatJetFiller( options < 0 ? defaultOptions : options
                             , branchName == "" ? defaults::BRANCH_AK4JETS : branchName
                             , eventInfo
                             , cfg.getParameter<edm::InputTag>("jets")
                             , cfg.getParameter<edm::InputTag>("reGenJets")
                             , cfg.getParameter<edm::InputTag>("stdGenJets")
                             , cfg.getUntrackedParameter<bool>("fillReGenJets")
                             , cfg.getUntrackedParameter<double>("minJetPt")
      );
      initializedFillers.push_back(ak4Jets);
      break;
    }

    case PUPPIJETS : {
      int defaultOptions = RecoJetFiller::defaultOptions;

      puppiJets = new RecoJetFiller(options < 0 ? defaultOptions : options,
                                    branchName == "" ? defaults::BRANCH_PUPPIJETS : branchName,
                                    eventInfo,
                                    cfg.getParameter<edm::InputTag>("jetsPuppi"),
                                    cfg.getParameter<edm::InputTag>("reGenJetsPuppi"),
                                    cfg.getParameter<edm::InputTag>("stdGenJetsPuppi"),
                                    cfg.getUntrackedParameter<bool>("fillReGenJetsPuppi"),
                                    cfg.getUntrackedParameter<double>("minJetPtPuppi")
                                    );
      initializedFillers.push_back(puppiJets);
      break;
    }

    case ELECTRONS : {
      int defaultOptions = ElectronFiller::defaultOptions;
      if(cfg.getUntrackedParameter<bool>("fillElectronIDVars"))       defaultOptions |= ElectronFiller::FILLIDVARS;
      if(cfg.getUntrackedParameter<bool>("fillElectronIsoVars"))      defaultOptions |= ElectronFiller::FILLISOVARS;
      if(cfg.getUntrackedParameter<bool>("evaluateElectronPOGIDMVA")) defaultOptions |= ElectronFiller::FILLPOGMVA;

      electrons = new ElectronFiller(options < 0 ? defaultOptions : options,
                                     branchName == "" ? defaults::BRANCH_ELECTRONS : branchName,
                                     eventInfo,
                                     cfg.getParameter<edm::InputTag>("electrons"),
                                     cfg.getParameter<edm::InputTag>("vetoId"),
                                     cfg.getParameter<edm::InputTag>("looseId"),
                                     cfg.getParameter<edm::InputTag>("mediumId"),
                                     cfg.getParameter<edm::InputTag>("tightId"),
                                     cfg.getUntrackedParameter<int>("bunchSpacing"),
                                     cfg.getUntrackedParameter<double>("minElectronPt")
                                     );
      initializedFillers.push_back(electrons);
      break;
    }

    case MUONS : {
      int defaultOptions = MuonFiller::defaultOptions;
      if(cfg.getUntrackedParameter<bool>("fillMuonIDVars"))  defaultOptions |= MuonFiller::FILLIDVARS;
      if(cfg.getUntrackedParameter<bool>("fillMuonIsoVars")) defaultOptions |= MuonFiller::FILLISOVARS;

      muons = new MuonFiller(options < 0 ? defaultOptions : options,
                            branchName == "" ? defaults::BRANCH_MUONS : branchName,
                            eventInfo,
                            cfg.getParameter<edm::InputTag>("muons"),
                            cfg.getUntrackedParameter<bool>("requireLooseMuon"),
                            cfg.getUntrackedParameter<double>("minMuonPt")
                            );
      initializedFillers.push_back(muons);
      break;
    }

    case TAUS : {
      int defaultOptions = TauFiller::defaultOptions;
      if(cfg.getUntrackedParameter<bool>("fillRawTauDiscriminators")) defaultOptions |= TauFiller::FILLRAWDISCS;
      if(cfg.getUntrackedParameter<bool>("printTauIDs"))              defaultOptions |= TauFiller::PRINTIDS;

      taus = new TauFiller(options < 0 ? defaultOptions : options,
                           branchName == "" ? defaults::BRANCH_TAUS : branchName,
                           eventInfo,
                           cfg.getParameter<edm::InputTag>("taus"),
                           cfg.getUntrackedParameter<double>("minTauPt")
                           );
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
