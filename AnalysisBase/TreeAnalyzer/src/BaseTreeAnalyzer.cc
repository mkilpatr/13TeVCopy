//--------------------------------------------------------------------------------------------------
// 
// BaseTreeAnalyzer
// 
//
// 
//--------------------------------------------------------------------------------------------------

#include "AnalysisBase/TreeAnalyzer/interface/BaseTreeAnalyzer.h"
#include "AnalysisTools/Utilities/interface/PhysicsUtilities.h"
#include "AnalysisTools/TreeReader/interface/Defaults.h"
#include "AnalysisBase/TreeAnalyzer/interface/DefaultProcessing.h"

using namespace std;
using namespace ucsbsusy;

//--------------------------------------------------------------------------------------------------
void BaseEventAnalyzer::analyzeEvent(BaseTreeAnalyzer * ana, int reportFrequency, int numEvents, int startEvent){
  clog << "Running over " << (numEvents < 0 ? "all" : TString::Format("at most %i",numEvents).Data()) << " events";
  if(startEvent >= 0 ) clog << ", starting with event: "<< startEvent;
  clog <<endl;
  ana->loadVariables();
  ana->setLoaded(true);
  if(startEvent >= 0 ){
    ana->setEventNumber(startEvent);
    if(numEvents >= 0 ) numEvents += startEvent;
  }
  while(ana->nextEvent(reportFrequency)){
    ana->setProcessed(false);
    if(numEvents >= 0 && ana->getEventNumber() >= numEvents+1) return;
    if(!ana->processData()) continue;
    ana->processVariables();
    ana->runEvent();
  }
}



//--------------------------------------------------------------------------------------------------
BaseTreeAnalyzer::BaseTreeAnalyzer(TString fileName, TString treeName, size randomSeed, bool isMCTree,cfgSet::ConfigSet * pars) : isLoaded_(false),isProcessed_(false), reader(fileName,treeName,"READ"),
    randGen           (new TRandom3(randomSeed)),
    run               (0),
    lumi              (0),
    event             (0),
    weight            (1),
    process           (defaults::NUMPROCESSES),
    datareco          (defaults::MC),
    nPV               (0),
    nPU               (0),
    rho               (0),
    nSelLeptons       (0),
    nPrimaryLeptons    (0),
    nSecondaryLeptons    (0),
    nVetoedTracks     (0),
    nJets             (0),
    nBJets            (0),
    nVetoHPSTaus      (0),
    met               (0),
    metNoHF           (0),
    puppimet          (0),
    genmet            (0),
    goodvertex        (false),
    zIsInvisible      (false),
    defaultJets       (0),
    isMC_             (isMCTree),
    configSet         (pars ? *pars : cfgSet::ConfigSet())

{
  clog << "Running over: " << (isMC_ ? "MC" : "data") <<endl;

  clog <<"Loaded configurations: " << endl;
  if(configSet.jsonProcessing)             clog << "Applying JSON file: " << configSet.jsonFile << endl;
  if(configSet.jets           .isConfig()) clog << configSet.jets  <<" ";
  if(configSet.electrons.isConfig)          clog << "Selected electron config:"  << endl << configSet.electrons <<" ";
  if(configSet.muons.isConfig)              clog << "Selected muon config:"      << endl << configSet.muons     <<" ";
  if(configSet.secondaryElectrons.isConfig) clog << "Secondary electron config:" << endl << configSet.secondaryElectrons <<" ";
  if(configSet.secondaryMuons.isConfig)     clog << "Secondary muon config:"     << endl << configSet.secondaryMuons <<" ";
  if(configSet.tracks   .isConfig()) clog << configSet.tracks    <<" ";
  if(configSet.taus     .isConfig()) clog << configSet.taus      <<" ";
  if(configSet.photons.isConfig()) clog << configSet.photons <<" ";
  if(configSet.corrections.isConfig())     clog << configSet.corrections     <<" ";
 
  clog << endl;

  if(configSet.jets.isConfig()){
    clog << "With default jet type: "  ;
    switch (configSet.jets.jetCollection) {
       case cfgSet::AK4JETS : {
         defaultJets  = &ak4Reader;
         clog << "ak4"  <<endl;
         break;
       }
       case cfgSet::PUPPIJETS : {
         defaultJets  = &puppiJetsReader;
         clog << "puppi"  <<endl;
         break;
       }
       case cfgSet::PICKYJETS : {
         defaultJets  = &pickyJetReader;
         clog << "picky"  <<endl;
         break;
       }
       case cfgSet::CASUBJETS : {
         defaultJets  = &caSubJetReader;
         clog << "subjetsca"  <<endl;
         break;
       }
       default : {
         cout << endl << "No valid jet for type: " << configSet.jets.jetCollection << " found!" << endl;
         break;
       }
     }
  } else {
    clog << "With no default jet type" << endl  ;
  }


  //load corrections
  if(configSet.corrections.isConfig()){
    if(configSet.corrections.ttbarCorrections != TtbarCorrectionSet::NULLOPT){
      ttbarCorrections.load(configSet.corrections.ttbarCorrectionFile,configSet.corrections.ttbarCorrections);
      corrections.push_back(&ttbarCorrections);
    }
    if(configSet.corrections.eventCorrections != EventCorrectionSet::NULLOPT){
      eventCorrections.load(configSet.corrections.eventCorrectionFile,configSet.corrections.eventCorrections);
      if(configSet.corrections.puCorrections != EventCorrectionSet::NULLOPT)
        eventCorrections.load(configSet.corrections.puCorrectionFile,configSet.corrections.puCorrections);
      corrections.push_back(&eventCorrections);
    }
    if(configSet.corrections.leptonCorrections != LeptonCorrectionSet::NULLOPT){
      leptonCorrections.load(configSet.corrections.leptonCorrectionFile,configSet.electrons, configSet.secondaryElectrons,configSet.muons, configSet.secondaryMuons,configSet.corrections.leptonCorrections);
      corrections.push_back(&leptonCorrections);
    }
    if(configSet.corrections.bTagCorrections != BTagCorrectionSet::NULLOPT){
      bTagCorrections.load(configSet.corrections.bTagEffFile,configSet.corrections.bTagSFFile,configSet.corrections.bTagCorrections);
      corrections.push_back(&bTagCorrections);
    }
    if(configSet.corrections.jetAndMETCorrections != JetAndMETCorrectionSet::NULLOPT){
      jetAndMETCorrections.load(configSet.corrections.jetAndMETCorrections,configSet.corrections.jetResFile,configSet.corrections.jetResTailFile,randGen);
      corrections.push_back(&jetAndMETCorrections);
    }
  }
}
//--------------------------------------------------------------------------------------------------
void BaseTreeAnalyzer::load(cfgSet::VarType type, int options, string branchName)
{
  switch (type) {
    case cfgSet::EVTINFO : {
      reader.load(&evtInfoReader, 0, "");
      break;
    }
    case cfgSet::AK4JETS : {
      int defaultOptions = JetReader::defaultOptions | (isMC() ? JetReader::LOADGEN : JetReader::NULLOPT);
      reader.load(&ak4Reader, options < 0 ? defaultOptions : options, branchName == "" ? defaults::BRANCH_AK4JETS : branchName);
      break;
    }
    case cfgSet::PUPPIJETS : {
      int defaultOptions = JetReader::defaultOptions | (isMC() ? JetReader::LOADGEN : JetReader::NULLOPT);
      reader.load(&puppiJetsReader, options < 0 ? defaultOptions : options, branchName == "" ? defaults::BRANCH_PUPPIJETS : branchName);
      break;
    }
    case cfgSet::PICKYJETS : {
      int defaultOptions = JetReader::defaultOptions | (isMC() ? JetReader::LOADGEN : JetReader::NULLOPT);
      reader.load(&pickyJetReader, options < 0 ? defaultOptions : options, branchName == "" ? defaults::BRANCH_PICKYJETS : branchName);
      break;
    }
    case cfgSet::CASUBJETS : {
      int defaultOptions = JetReader::defaultOptions | (isMC() ? JetReader::LOADGEN : JetReader::NULLOPT);
      reader.load(&caSubJetReader, options < 0 ? defaultOptions : options, branchName == "" ? defaults::BRANCH_CASUBJETS : branchName);
      break;
    }
    case cfgSet::ELECTRONS : {
      int defaultOptions = ElectronReader::defaultOptions;
      reader.load(&electronReader, options < 0 ? defaultOptions : options, branchName == "" ? defaults::BRANCH_ELECTRONS : branchName );
      break;
    }
    case cfgSet::MUONS : {
      int defaultOptions = MuonReader::defaultOptions;
      reader.load(&muonReader, options < 0 ? defaultOptions : options, branchName == "" ? defaults::BRANCH_MUONS : branchName );
      break;
    }
    case cfgSet::TAUS : {
      int defaultOptions = TauReader::defaultOptions;
      reader.load(&tauReader, options < 0 ? defaultOptions : options, branchName == "" ? defaults::BRANCH_TAUS : branchName );
      break;
    }
    case cfgSet::PHOTONS : {
      int defaultOptions = PhotonReader::defaultOptions;
      reader.load(&photonReader, options < 0 ? defaultOptions : options, branchName == "" ? defaults::BRANCH_PHOTONS : branchName );
      break;
    }
    case cfgSet::PFCANDS : {
      int defaultOptions = PFCandidateReader::defaultOptions;
      if(configSet.tracks.isConfig() && !configSet.tracks.mtPresel )
        defaultOptions = PFCandidateReader::LOADRECO | PFCandidateReader::FILLOBJ | PFCandidateReader::LOADTAUVETODPHI;
      reader.load(&pfcandReader, options < 0 ? defaultOptions : options, branchName == "" ? defaults::BRANCH_PFCANDS : branchName );
      break;
    }
    case cfgSet::GENPARTICLES : {
      int defaultOptions = GenParticleReader::defaultOptions;
      reader.load(&genParticleReader, options < 0 ? defaultOptions : options, branchName == "" ? defaults::BRANCH_GENPARTS : branchName );
      break;
    }
      
    case cfgSet::CMSTOPS : {
      int defaultOptions = CMSTopReader::defaultOptions;
      reader.load(&cmsTopReader, options < 0 ? defaultOptions : options, branchName == "" ? defaults::BRANCH_CMSTOPS : branchName);
      break;
    }

    case cfgSet::AK8FATJETS : {
      int defaultOptions = FatJetReader::defaultOptions;
      reader.load(&fatJetReader, options < 0 ? defaultOptions : options, branchName == "" ? defaults::BRANCH_AK8FATJETS : branchName);
      break;
    }

    case cfgSet::AK8PUPPIFATJETS : {
      int defaultOptions = FatJetReader::defaultOptions;
      reader.load(&fatJetPuppiReader, options < 0 ? defaultOptions : options, branchName == "" ? defaults::BRANCH_AK8PUPPIFATJETS : branchName);
      break;
    }

    case cfgSet::CORRAL : {
      int defaultOptions = CORRALReader::defaultOptions;
      reader.load(&corralReader, options < 0 ? defaultOptions : options, branchName == "" ? defaults::BRANCH_CORRAL : branchName );
      break;
    }

    case cfgSet::TRIGOBJS : {
      int defaultOptions = TriggerObjectReader::defaultOptions;
      reader.load(&trigObjReader, options < 0 ? defaultOptions : options, branchName == "" ? defaults::BRANCH_TRIGGERS : branchName);
      break;
    }

  default : {
    cout << endl << "No settings for type: " << type << " found!" << endl;
    break;
  }
  }
}
//--------------------------------------------------------------------------------------------------
void BaseTreeAnalyzer::analyze(int reportFrequency, int numEvents, int startEvent) {
  auto * evtAna = setupEventAnalyzer();
  evtAna->analyzeEvent(this,reportFrequency,numEvents,startEvent);
  delete evtAna;
}
//--------------------------------------------------------------------------------------------------
void BaseTreeAnalyzer::loadVariables()
{
  load(cfgSet::EVTINFO);
  load(cfgSet::AK4JETS);
  load(cfgSet::ELECTRONS);
  load(cfgSet::MUONS);
  load(cfgSet::PHOTONS);
  load(cfgSet::TAUS);
  load(cfgSet::PFCANDS);
  load(cfgSet::CMSTOPS);
  load(cfgSet::AK8FATJETS);
  load(cfgSet::AK8PUPPIFATJETS);
  load(cfgSet::TRIGOBJS);
  if(isMC()) load(cfgSet::GENPARTICLES);
}
//--------------------------------------------------------------------------------------------------
void BaseTreeAnalyzer::processVariables()
{
  isProcessed_ = true;


  if(evtInfoReader.isLoaded()) {
    run   = evtInfoReader.run;
    lumi  = evtInfoReader.lumi;
    event = evtInfoReader.event;
    nPV   = evtInfoReader.nPV;
    nPU   = evtInfoReader.nPUTrue;
    rho   = evtInfoReader.rho;
    goodvertex=evtInfoReader.goodvertex;
    met   = &evtInfoReader.met;
    metNoHF = &evtInfoReader.metNoHF;
    puppimet= &evtInfoReader.puppimet;
    genmet= &evtInfoReader.genmet;
    weight=  evtInfoReader.evtweight;
    process =  evtInfoReader.process;
    datareco =  evtInfoReader.datareco;

    if(configSet.corrections.jetAndMETCorrections != JetAndMETCorrectionSet::NULLOPT){
      jetAndMETCorrections.processMET(this);
      (*met) = jetAndMETCorrections.getCorrectedMET();
      (*metNoHF) = jetAndMETCorrections.getCorrectedMETNoHF();
      if(defaultJets && defaultJets->isLoaded()) {
        jetAndMETCorrections.correctJetScale(this,defaultJets->recoJets,*met);
        jetAndMETCorrections.correctJetResolution(this,defaultJets->recoJets,*met);
        jetAndMETCorrections.processRespTail(this,*defaultJets, *met);
      }
    }
  }



  if(genParticleReader.isLoaded()){
    genParts.clear();
    genParts.reserve(genParticleReader.genParticles.size());
    for(auto& p : genParticleReader.genParticles) genParts.push_back(&p);
  }


  if(cmsTopReader.isLoaded()){
    cttTops.clear();
    cttTops.reserve(cmsTopReader.cmsTops.size());
    for(auto& p : cmsTopReader.cmsTops) cttTops.push_back(&p);
  }

  if(fatJetReader.isLoaded()){
    fatJets.clear();
    fatJets.reserve(fatJetReader.fatJets.size());
    for(auto& p : fatJetReader.fatJets) fatJets.push_back(&p);
  }

  if(fatJetPuppiReader.isLoaded()){
    fatJetsPuppi.clear();
    fatJetsPuppi.reserve(fatJetPuppiReader.fatJets.size());
    for(auto& p : fatJetPuppiReader.fatJets) fatJets.push_back(&p);
  }

  if(trigObjReader.isLoaded()){
    triggerflag =  trigObjReader.triggerflag;
    triggerObjects.clear();
    triggerObjects.reserve(trigObjReader.trigobjs.size());
    for(auto& to : trigObjReader.trigobjs)
      triggerObjects.push_back(&to);
    triggerInfo.clear();
    triggerInfo.reserve(trigObjReader.triginfo.size());
    for(auto& tI : trigObjReader.triginfo)
      triggerInfo.push_back(&tI);
  }

  allLeptons.clear();
  selectedLeptons.clear();
  primaryLeptons.clear();
  secondaryLeptons.clear();
  if(muonReader.isLoaded() || electronReader.isLoaded()){
    allLeptons.reserve(electronReader.electrons.size() + muonReader.muons.size());
    if(electronReader.isLoaded())
      for(auto& electron : electronReader.electrons)
        allLeptons.push_back(&electron);
    if(muonReader.isLoaded())
      for(auto& muon : muonReader.muons)
       allLeptons.push_back(&muon);
    std::sort(allLeptons.begin(), allLeptons.end(), PhysicsUtilities::greaterPTDeref<LeptonF>());

    std::vector<LeptonF*> nonPrimaryLeptons(0);
    if(configSet.electrons.isConfig || configSet.muons.isConfig)
      cfgSet::selectLeptons(primaryLeptons, allLeptons, configSet.electrons,configSet.muons,&nonPrimaryLeptons);

    if(configSet.secondaryElectrons.isConfig || configSet.secondaryMuons.isConfig)
      cfgSet::selectLeptons(secondaryLeptons, (configSet.electrons.isConfig || configSet.muons.isConfig) ? nonPrimaryLeptons : allLeptons,
          configSet.secondaryElectrons,configSet.secondaryMuons);
  }

  for(auto* lep : primaryLeptons)
    selectedLeptons.push_back(lep);
  for(auto* lep : secondaryLeptons)
    selectedLeptons.push_back(lep);
  std::sort(selectedLeptons.begin(), selectedLeptons.end(), PhysicsUtilities::greaterPTDeref<LeptonF>());


  nSelLeptons = selectedLeptons.size();
  nPrimaryLeptons = primaryLeptons.size();
  nSecondaryLeptons = secondaryLeptons.size();

  vetoedTracks.clear();
  if(pfcandReader.isLoaded() && configSet.tracks.isConfig())
    cfgSet::selectTracks(vetoedTracks, pfcandReader.pfcands, met, configSet.tracks);
  nVetoedTracks = vetoedTracks.size();

  if(photonReader.isLoaded() && configSet.photons.isConfig())
    cfgSet::selectPhotons(selectedPhotons,photonReader.photons, configSet.photons);

  // must not preceed selectLeptons
  vetoedTaus.clear();
  if(tauReader.isLoaded() && configSet.taus.isConfig())
    cfgSet::selectTaus(vetoedTaus, selectedLeptons, tauReader.taus, configSet.taus);
  nVetoHPSTaus = vetoedTaus.size();

  jets.clear(); bJets.clear(); nonBJets.clear();
  if(defaultJets && defaultJets->isLoaded() && configSet.jets.isConfig()){
    if(configSet.jets.applyAdHocPUCorr) cfgSet::applyAdHocPUCorr(defaultJets->recoJets, *defaultJets->jetarea_, rho);

    cfgSet::selectJets(jets, &bJets, &nonBJets, defaultJets->recoJets,&selectedLeptons,&primaryLeptons,&selectedPhotons,&vetoedTracks,configSet.jets);
  }
  nJets    = jets.size();
  nBJets   = bJets.size();

  //load corrections corrections
  for(auto * iC : corrections){
    iC->processCorrection(this);
  }

}
//--------------------------------------------------------------------------------------------------
bool BaseTreeAnalyzer::processData(){
  //only look at data
  if(evtInfoReader.process == defaults::NUMPROCESSES || evtInfoReader.process <= defaults::SIGNAL ) return true;

  // skip events in PR with run number < 251584 - they are in the re-miniAOD
  if(evtInfoReader.datareco==defaults::PROMPT_50NS && evtInfoReader.run < 251584) return false;

  return true;
}
