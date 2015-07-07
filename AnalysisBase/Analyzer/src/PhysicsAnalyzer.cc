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
, puppiJets           (0)
, pickyJets           (0)
, caSubJets           (0)
, ca8Jets             (0)
, muons               (0)
, electrons           (0)
, taus                (0)
, photons             (0)
, pfcands             (0)
, genparticles        (0)
, cmstops             (0)
, ak8fatjets          (0)
, triggers            (0)
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
      eventInfo = new EventInfoFiller(cfg, consumesCollector());
      initializedFillers.push_back(eventInfo);
      break;
    }

    case GENPARTICLES : {
      int defaultOptions = GenParticleFiller::defaultOptions;
      if(cfg.getUntrackedParameter<bool>("saveAllGenParticles"))     defaultOptions |= GenParticleFiller::SAVEALL;
      if(cfg.getUntrackedParameter<bool>("savePartonHadronization")) defaultOptions |= GenParticleFiller::SAVEPARTONDECAY;

      genparticles = new GenParticleFiller(cfg, consumesCollector(),
                                           options < 0 ? defaultOptions : options,
                                           branchName == "" ? defaults::BRANCH_GENPARTS : branchName
                                           );
      initializedFillers.push_back(genparticles);
      break;
    }

    case AK4JETS : {
      int defaultOptions = PatJetFiller::defaultOptions; 
      if((isMC() && cfg.getUntrackedParameter<bool>("fillJetGenInfo"))) defaultOptions |= PatJetFiller::LOADGEN;
      if(cfg.getUntrackedParameter<bool>("fillJetShapeInfo"))           defaultOptions |= PatJetFiller::LOADJETSHAPE;
      if(isMC() && cfg.getUntrackedParameter<bool>("fillTopJetAssoc"))  defaultOptions |= PatJetFiller::SAVETOPASSOC;
      if(cfg.getUntrackedParameter<bool>("fillqgl"))                    defaultOptions |= PatJetFiller::SAVEQGL;

      ak4Jets = new PatJetFiller(cfg, consumesCollector(),
                                 options < 0 ? defaultOptions : options,
                                 branchName == "" ? defaults::BRANCH_AK4JETS : branchName,
                                 eventInfo,
                                 genparticles
                                 );
      initializedFillers.push_back(ak4Jets);
      break;
    }

    case PUPPIJETS : {
      int defaultOptions = RecoJetFiller::defaultOptions;
      if((isMC() && cfg.getUntrackedParameter<bool>("fillJetGenInfo"))) defaultOptions |= RecoJetFiller::LOADGEN;
      if(cfg.getUntrackedParameter<bool>("fillJetShapeInfo"))           defaultOptions |= RecoJetFiller::LOADJETSHAPE;
      if(cfg.getUntrackedParameter<bool>("fillCustomBtagInfo"))         defaultOptions |= RecoJetFiller::LOADBTAG;
      if(isMC() && cfg.getUntrackedParameter<bool>("fillTopJetAssoc"))  defaultOptions |= RecoJetFiller::SAVETOPASSOC;
      if(cfg.getUntrackedParameter<bool>("fillqgl"))                    defaultOptions |= PatJetFiller::SAVEQGL;

      puppiJets = new RecoJetFiller(cfg, consumesCollector(),
                                    options < 0 ? defaultOptions : options,
                                    branchName == "" ? defaults::BRANCH_PUPPIJETS : branchName,
                                    eventInfo,
                                    genparticles
                                    );
      initializedFillers.push_back(puppiJets);
      break;
    }

    case PICKYJETS : {
      int defaultOptions = RecoJetFiller::defaultOptions;
      if((isMC() && cfg.getUntrackedParameter<bool>("fillJetGenInfo"))) defaultOptions |= RecoJetFiller::LOADGEN;
      if(cfg.getUntrackedParameter<bool>("fillJetShapeInfo"))           defaultOptions |= RecoJetFiller::LOADJETSHAPE;
      if(cfg.getUntrackedParameter<bool>("fillCustomBtagInfo"))         defaultOptions |= RecoJetFiller::LOADBTAG;
      if(isMC() && cfg.getUntrackedParameter<bool>("fillTopJetAssoc"))  defaultOptions |= RecoJetFiller::SAVETOPASSOC;
      if(cfg.getUntrackedParameter<bool>("fillqgl"))                    defaultOptions |= PatJetFiller::SAVEQGL;

      pickyJets = new RecoJetFiller(cfg, consumesCollector(),
                                    options < 0 ? defaultOptions : options,
                                    branchName == "" ? defaults::BRANCH_PICKYJETS : branchName,
                                    eventInfo,
                                    genparticles
                                    );
      initializedFillers.push_back(pickyJets);
      break;
    }

    case CASUBJETS : {
      int defaultOptions = RecoJetFiller::defaultOptions;
      if((isMC() && cfg.getUntrackedParameter<bool>("fillJetGenInfo"))) defaultOptions |= RecoJetFiller::LOADGEN;
      if(cfg.getUntrackedParameter<bool>("fillJetShapeInfo"))           defaultOptions |= RecoJetFiller::LOADJETSHAPE;
      if(cfg.getUntrackedParameter<bool>("fillCustomBtagInfo"))         defaultOptions |= RecoJetFiller::LOADBTAG;
      if(isMC() && cfg.getUntrackedParameter<bool>("fillTopJetAssoc"))  defaultOptions |= RecoJetFiller::SAVETOPASSOC;
      if(cfg.getUntrackedParameter<bool>("fillqgl"))                    defaultOptions |= PatJetFiller::SAVEQGL;
      if(cfg.getUntrackedParameter<bool>("fillSuper"))                  defaultOptions |= RecoJetFiller::LOADSUPER;

      caSubJets = new RecoJetFiller(cfg, consumesCollector(),
                                    options < 0 ? defaultOptions : options,
                                    branchName == "" ? defaults::BRANCH_CASUBJETS : branchName,
                                    eventInfo,
                                    genparticles
                                    );
      initializedFillers.push_back(caSubJets);
      break;
    }

    case ELECTRONS : {
      int defaultOptions = ElectronFiller::defaultOptions;
      if(cfg.getUntrackedParameter<bool>("fillElectronIDVars"))       defaultOptions |= ElectronFiller::FILLIDVARS;
      if(cfg.getUntrackedParameter<bool>("fillElectronIsoVars"))      defaultOptions |= ElectronFiller::FILLISOVARS;
      if(cfg.getUntrackedParameter<bool>("evaluateElectronPOGIDMVA")) defaultOptions |= ElectronFiller::FILLPOGMVA;
      if(cfg.getUntrackedParameter<bool>("fillElectronGenInfo"))      defaultOptions |= ElectronFiller::LOADGEN;

      electrons = new ElectronFiller(cfg, consumesCollector(),
                                     options < 0 ? defaultOptions : options,
                                     branchName == "" ? defaults::BRANCH_ELECTRONS : branchName,
                                     eventInfo
                                     );
      initializedFillers.push_back(electrons);
      break;
    }

    case MUONS : {
      int defaultOptions = MuonFiller::defaultOptions;
      if(cfg.getUntrackedParameter<bool>("fillMuonIDVars"))  defaultOptions |= MuonFiller::FILLIDVARS;
      if(cfg.getUntrackedParameter<bool>("fillMuonIsoVars")) defaultOptions |= MuonFiller::FILLISOVARS;
      if(cfg.getUntrackedParameter<bool>("fillMuonGenInfo")) defaultOptions |= MuonFiller::LOADGEN;

      muons = new MuonFiller(cfg, consumesCollector(),
                             options < 0 ? defaultOptions : options,
                             branchName == "" ? defaults::BRANCH_MUONS : branchName,
                             eventInfo
                             );
      initializedFillers.push_back(muons);
      break;
    }

    case TAUS : {
      int defaultOptions = TauFiller::defaultOptions;
      if(cfg.getUntrackedParameter<bool>("fillRawTauDiscriminators")) defaultOptions |= TauFiller::FILLRAWDISCS;
      if(cfg.getUntrackedParameter<bool>("printTauIDs"))              defaultOptions |= TauFiller::PRINTIDS;

      taus = new TauFiller(cfg, consumesCollector(),
                           options < 0 ? defaultOptions : options,
                           branchName == "" ? defaults::BRANCH_TAUS : branchName,
                           eventInfo
                           );
      initializedFillers.push_back(taus);
      break;
    }

    case PHOTONS : {
      int defaultOptions = PhotonFiller::defaultOptions;
      if(cfg.getUntrackedParameter<bool>("fillPhotonIDVars"))         defaultOptions |= PhotonFiller::FILLIDVARS;
      if(cfg.getUntrackedParameter<bool>("fillPhotonIsoVars"))        defaultOptions |= PhotonFiller::FILLISOVARS;

      photons = new PhotonFiller(cfg, consumesCollector(),
                                 options < 0 ? defaultOptions : options,
                                 branchName == "" ? defaults::BRANCH_PHOTONS : branchName
                                 );
      initializedFillers.push_back(photons);
      break;
    }

    case PFCANDS : {
      int defaultOptions = PFCandidateFiller::defaultOptions;
      if(cfg.getUntrackedParameter<bool>("saveAllCandidates")) defaultOptions |= PFCandidateFiller::SAVEALLCANDS;

      pfcands = new PFCandidateFiller(cfg, consumesCollector(),
                                      options < 0 ? defaultOptions : options,
                                      branchName == "" ? defaults::BRANCH_PFCANDS : branchName,
                                      eventInfo
                                      );
      initializedFillers.push_back(pfcands);
      break;
    }
 
    case CMSTOPS : { 
      cmstops = new CMSTopFiller(cfg, consumesCollector(),
                                 0,
			         branchName == "" ? defaults::BRANCH_CMSTOPS : branchName
				 );
      initializedFillers.push_back(cmstops);
      break;
    }

    case AK8FATJETS : { 
      ak8fatjets = new FatJetFiller(cfg, consumesCollector(),
                                    0,
				    branchName == "" ? defaults::BRANCH_AK8FATJETS : branchName
				    );
      initializedFillers.push_back(ak8fatjets);
      break;
    }
    
    case TRIGGERS : {
      int defaultOptions = TriggerFiller::defaultOptions;
      triggers = new TriggerFiller(cfg, consumesCollector(),
                                   options < 0 ? defaultOptions : options,
                                   branchName == "" ? defaults::BRANCH_TRIGGERS : branchName
                                   );
      initializedFillers.push_back(triggers);
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
