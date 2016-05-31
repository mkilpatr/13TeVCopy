#include <iostream>

#include "AnalysisBase/Analyzer/interface/PhysicsAnalyzer.h"
#include "AnalysisBase/Analyzer/interface/FileUtilities.h"
#include "AnalysisTools/TreeReader/interface/Defaults.h"

using namespace ucsbsusy;
using namespace std;

//const bool ucsbsusy::PhysicsAnalyzer::PRINTLHERUNINFO = false; // if you need to print out available LHE info for systematics


//--------------------------------------------------------------------------------------------------
PhysicsAnalyzer::PhysicsAnalyzer(const edm::ParameterSet& iConfig)
: BaseAnalyzer(iConfig)
, isRealData          (iConfig.getParameter<int             >("isData"                 ))
, globalTag           (iConfig.getParameter<string          >("globalTag"       ).data())
, printLHERunInfo     (iConfig.getUntrackedParameter<bool   >("printLHERunInfo",false))
, lheInfoToken_       (consumes<LHERunInfoProduct>(iConfig.getUntrackedParameter<edm::ParameterSet>("EventInfo").getParameter<edm::InputTag>("lheEvtInfo")))
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
, ak8puppifatjets     (0)
, triggers            (0)
, metfilters          (0)
, sv                  (0)
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
void PhysicsAnalyzer::beginRun(edm::Run const &run, edm::EventSetup const &es)
{

  if(!printLHERunInfo) return;

  edm::Handle<LHERunInfoProduct> lheruninfo;
  typedef std::vector<LHERunInfoProduct::Header>::const_iterator headers_const_iterator;

  run.getByToken( lheInfoToken_, lheruninfo );
  LHERunInfoProduct myLHERunInfoProduct = *(lheruninfo.product());

  for (headers_const_iterator iter=myLHERunInfoProduct.headers_begin(); iter!=myLHERunInfoProduct.headers_end(); iter++){
    std::cout << iter->tag() << std::endl;
    std::vector<std::string> lines = iter->lines();
    for (unsigned int iLine = 0; iLine<lines.size(); iLine++) {
     std::cout << lines.at(iLine);
    }
  }

}

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
  for(auto f : initializedFillers) {f->load(iEvent, iSetup); }
  return true;
}
//--------------------------------------------------------------------------------------------------
void PhysicsAnalyzer::fill()
{
  for(auto f : initializedFillers) {f->fill();}
  treeWriter()->fill();
}

//--------------------------------------------------------------------------------------------------
/*void PhysicsAnalyzer::initialize(const edm::ParameterSet& cfg, const std::string pSetName, edm::EventSetup const &es, const VarType type, const int options, const std::string branchName){
  if(cfg.existsAs<edm::ParameterSet>(pSetName, false)) {
    edm::ParameterSet fcfg(cfg.getUntrackedParameter<edm::ParameterSet>(pSetName));   // parameter set for this filler
    bool fill = fcfg.getUntrackedParameter<bool>("isFilled");                         // to fill or not to fill
    if(fill) initialize(fcfg, es, type, options, branchName);
  }
  }*/
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
      int defaultOptions = EventInfoFiller::defaultOptions;
      if(isMC()) defaultOptions |= EventInfoFiller::LOADPUINFO;
      if(isMC()) defaultOptions |= EventInfoFiller::LOADGEN;
      if(isMC() && cfg.getUntrackedParameter<bool>("saveSystematicWeights")) defaultOptions |= EventInfoFiller::LOADLHE;
      if(isMC() && cfg.getUntrackedParameter<bool>("isMassScan")) defaultOptions |= EventInfoFiller::SAVEMASSES;
      eventInfo = new EventInfoFiller(cfg, consumesCollector(),
                                      options < 0 ? defaultOptions : options
                                      );
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
      if(cfg.getUntrackedParameter<bool>("loadJecUncFile"))             defaultOptions |= PatJetFiller::LOADUNCFROMFILE;
      if(cfg.getUntrackedParameter<bool>("fillExtraJetInfo"))           defaultOptions |= PatJetFiller::FILLJETEXTRA;

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
      if(cfg.getUntrackedParameter<bool>("loadJecUncFile"))             defaultOptions |= RecoJetFiller::LOADUNCFROMFILE;
      if(cfg.getUntrackedParameter<bool>("fillExtraJetInfo"))           defaultOptions |= RecoJetFiller::FILLJETEXTRA;

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
      if(cfg.getUntrackedParameter<bool>("loadJecUncFile"))             defaultOptions |= RecoJetFiller::LOADUNCFROMFILE;

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
      if(cfg.getUntrackedParameter<bool>("loadJecUncFile"))             defaultOptions |= RecoJetFiller::LOADUNCFROMFILE;

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
      if(cfg.getUntrackedParameter<bool>("fillCandInfo"))             defaultOptions |= TauFiller::FILLCANDINFO;
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

  case AK8PUPPIFATJETS : { 
    ak8puppifatjets = new FatJetFiller(cfg, consumesCollector(),
				       0,
				       branchName == "" ? defaults::BRANCH_AK8PUPPIFATJETS : branchName
				       );
    initializedFillers.push_back(ak8puppifatjets);
    break;
  }
    
    case TRIGGERS : {
      int defaultOptions = TriggerFiller::defaultOptions;
      if(cfg.getUntrackedParameter<bool>("printTriggerNames")) defaultOptions |= TriggerFiller::PRINTTRIGGERNAMES;
      triggers = new TriggerFiller(cfg, consumesCollector(),
                                   options < 0 ? defaultOptions : options,
                                   branchName == "" ? defaults::BRANCH_TRIGGERS : branchName
                                   );
      initializedFillers.push_back(triggers);
      break;
    }
 
    case METFILTERS : {
      int defaultOptions = METFiltersFiller::defaultOptions;
      metfilters = new METFiltersFiller(cfg, consumesCollector(),
                                        options < 0 ? defaultOptions : options,
				        branchName == "" ? defaults::BRANCH_METFILTERS : branchName
				        );
      initializedFillers.push_back(metfilters);
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

  case SV : { 
    sv = new SecondaryVertexFiller(cfg, consumesCollector(),
				  0,
				  branchName == "" ? defaults::BRANCH_SV : branchName
				  );
    initializedFillers.push_back(sv);
    break;
  }

    default : {
      cout << endl << "No settings for type: " << type << " found, maybe you should use the initialization enabling the event setup!" << endl;
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
