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
    triggerflag       (0),
    nPV               (0),
    nPU               (0),
    rho               (0),
    nSelLeptons       (0),
    nPrimaryLeptons    (0),
    nSecondaryLeptons  (0),
    nVetoedTracks     (0),
    nJets             (0),
    nBJets            (0),
    nVetoHPSTaus      (0),
    selectedLepton    (0),
    nSelCTTTops       (0),
    nSelSdTops        (0),
    nSelSdWs          (0),
    met               (0),
    metNoHF           (0),
    puppimet          (0),
    genmet            (0),
    goodvertex        (false),
    defaultJets       (0),
    isMC_             (isMCTree),
    configSet         (pars ? *pars : cfgSet::ConfigSet())

{

  //Hack to get signal type from filename (sorry) until we integrat into weight code
  if(fileName.Contains("T2tt") ) evtInfoReader.signalType = defaults::T2tt;
  else if (fileName.Contains("T2bW") ) evtInfoReader.signalType = defaults::T2bW;
  else if (fileName.Contains("T2fb") ) evtInfoReader.signalType = defaults::T2fb;
  else if (fileName.Contains("T2tb") ) evtInfoReader.signalType = defaults::T2tb;
  //

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

    if(configSet.corrections.wpolCorrections != WPolCorrectionSet::NULLOPT){
      wpolCorrections.load(configSet.corrections.wpolCorrections);
      corrections.push_back(&wpolCorrections);
    }

    if(configSet.corrections.puCorrections != EventCorrectionSet::NULLOPT){
      eventCorrections.load(configSet.corrections.puCorrectionFile,configSet.corrections.cttCorrectionFile, configSet.corrections.sdCorrectionFile, configSet.corrections.puCorrections);
      corrections.push_back(&eventCorrections);
    }

    if(configSet.corrections.triggerCorrections != TriggerCorrectionSet::NULLOPT){
      triggerCorrections.load(configSet.corrections.triggerCorrectionFile, configSet.corrections.triggerCorrections);
      corrections.push_back(&triggerCorrections);
    }

    if(configSet.corrections.leptonCorrections != LeptonCorrectionSet::NULLOPT){
      //leptonCorrections.load(configSet.corrections.leptonCorrectionFile,configSet.electrons, configSet.secondaryElectrons,configSet.muons, configSet.secondaryMuons,configSet.corrections.leptonCorrections);
      leptonCorrections.load(configSet.corrections.leptonCorrectionFile, configSet.corrections.leptonCorrectionFile2,configSet.electrons, configSet.secondaryElectrons,configSet.muons, configSet.secondaryMuons,configSet.corrections.leptonCorrections);
      corrections.push_back(&leptonCorrections);
    }
    if(configSet.corrections.bTagCorrections != BTagCorrectionSet::NULLOPT){
      bTagCorrections.load(configSet.corrections.bTagEffFile,configSet.corrections.bTagSFFile,configSet.corrections.bTagFastSimEffFile,configSet.corrections.bTagFastSimSFFile,configSet.corrections.bTagCorrections);
      corrections.push_back(&bTagCorrections);
    }
    if(configSet.corrections.jetAndMETCorrections != JetAndMETCorrectionSet::NULLOPT){
      jetAndMETCorrections.load(configSet.corrections.jetAndMETCorrections,configSet.corrections.jetResFile,configSet.corrections.jetResCorrFile,configSet.corrections.jetResTailFile,randGen);
      corrections.push_back(&jetAndMETCorrections);
    }
    if(configSet.corrections.isrCorrections != ISRCorrectionSet::NULLOPT){
      isrCorrections.load(configSet.corrections.isrCorrFile,configSet.corrections.isrSigNormFile,configSet.corrections.isrSigNormTightFile,configSet.corrections.isrSigNorms,configSet.corrections.isrCorrections);
      corrections.push_back(&isrCorrections);
    }
  }

  // load object MVAs
  if (cfgSet::useResolvedTopMVA) resTopMVA = new ResolvedTopMVA(cfgSet::resolvedTopMVAFile, "BDT");

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
      int defaultOptions = JetReader::defaultOptions | (isMC() ? JetReader::LOADGEN | JetReader::LOADTOPASSOC : JetReader::NULLOPT);
      reader.load(&ak4Reader, options < 0 ? defaultOptions : options, branchName == "" ? defaults::BRANCH_AK4JETS : branchName);
      break;
    }
    case cfgSet::PUPPIJETS : {
      int defaultOptions = JetReader::defaultOptions | (isMC() ? JetReader::LOADGEN | JetReader::LOADTOPASSOC: JetReader::NULLOPT);
      reader.load(&puppiJetsReader, options < 0 ? defaultOptions : options, branchName == "" ? defaults::BRANCH_PUPPIJETS : branchName);
      break;
    }
    case cfgSet::PICKYJETS : {
      int defaultOptions = JetReader::defaultOptions | (isMC() ? JetReader::LOADGEN | JetReader::LOADTOPASSOC: JetReader::NULLOPT);
      reader.load(&pickyJetReader, options < 0 ? defaultOptions : options, branchName == "" ? defaults::BRANCH_PICKYJETS : branchName);
      break;
    }
    case cfgSet::CASUBJETS : {
      int defaultOptions = JetReader::defaultOptions | (isMC() ? JetReader::LOADGEN | JetReader::LOADTOPASSOC: JetReader::NULLOPT);
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
      /*
    case cfgSet::AK8PUPPIFATJETS : {
      int defaultOptions = FatJetReader::defaultOptions;
      reader.load(&fatJetPuppiReader, options < 0 ? defaultOptions : options, branchName == "" ? defaults::BRANCH_AK8PUPPIFATJETS : branchName);
      break;
    }
      */
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

    case cfgSet::SV : {
      int defaultOptions = SVReader::defaultOptions;
      reader.load(&svReader, options < 0 ? defaultOptions : options, branchName == "" ? defaults::BRANCH_SV : branchName);
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
  //  load(cfgSet::AK8PUPPIFATJETS);
  load(cfgSet::TRIGOBJS);
  load(cfgSet::SV);
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
    nSelCTTTops = 0;
    selectedCTTTops.clear();
    cttTops.clear();
    cttTops.reserve(cmsTopReader.cmsTops.size());
    for(auto& p : cmsTopReader.cmsTops) cttTops.push_back(&p);
    std::sort(cttTops.begin(), cttTops.end(), PhysicsUtilities::greaterPTDeref<CMSTopF>());
    for(auto* p : cttTops) if(cfgSet::isSelTaggedTop(*p)) selectedCTTTops.push_back(p);
    nSelCTTTops = selectedCTTTops.size();
  }

  if(fatJetReader.isLoaded()){
    nSelSdTops = 0;
    nSelSdWs = 0;
    fatJets.clear();
    selectedSdTops.clear();
    selectedSdWs.clear();
    fatJets.reserve(fatJetReader.fatJets.size());
    for(auto& p : fatJetReader.fatJets) fatJets.push_back(&p);
    for(auto& fj : fatJets){
      if (cfgSet::isSoftDropTagged(fj, 400, 110, 210, 0.69, 1e9)) selectedSdTops.push_back(fj);
      if (cfgSet::isSoftDropTagged(fj, 200, 60,  110, 1e9,  0.60)) selectedSdWs.push_back(fj);
    }
    nSelSdTops = selectedSdTops.size();
    nSelSdWs = selectedSdWs.size();
  }
  /*
  if(fatJetPuppiReader.isLoaded()){
    fatJetsPuppi.clear();
    fatJetsPuppi.reserve(fatJetPuppiReader.fatJets.size());
    for(auto& p : fatJetPuppiReader.fatJets) fatJetsPuppi.push_back(&p);
  }
  */
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

  if(svReader.isLoaded()){
    SVs.clear();
    SVs.reserve(svReader.SVs.size());
    for(auto& p : svReader.SVs) SVs.push_back(&p);
  }

  allLeptons.clear();
  selectedLeptons.clear();
  primaryLeptons.clear();
  secondaryLeptons.clear();
  selectedLepton = 0;
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
  if(nSelLeptons > 0) selectedLepton = nSelLeptons == 1 ? selectedLeptons.front() : selectedLeptons[randGen->Uniform(0,nSelLeptons)];

  vetoedTracks.clear();
  if(pfcandReader.isLoaded() && configSet.tracks.isConfig())
    cfgSet::selectTracks(vetoedTracks, pfcandReader.pfcands, met, configSet.tracks);
  std::sort(vetoedTracks.begin(), vetoedTracks.end(), PhysicsUtilities::greaterPTDeref<PFCandidateF>());
  nVetoedTracks = vetoedTracks.size();

  if(photonReader.isLoaded() && configSet.photons.isConfig())
    cfgSet::selectPhotons(selectedPhotons,photonReader.photons, configSet.photons);

  // must not preceed selectLeptons
  vetoedTaus.clear();
  if(tauReader.isLoaded() && configSet.taus.isConfig())
    cfgSet::selectTaus(vetoedTaus, selectedLeptons, tauReader.taus, configSet.taus);
  nVetoHPSTaus = vetoedTaus.size();

  jets.clear(); bJets.clear(); nonBJets.clear(); isrJets.clear();
  if(defaultJets && defaultJets->isLoaded() && configSet.jets.isConfig()){
    if(configSet.jets.applyAdHocPUCorr) cfgSet::applyAdHocPUCorr(defaultJets->recoJets, *defaultJets->jetarea_, rho);

    cfgSet::selectJets(jets, &bJets, &nonBJets, defaultJets->recoJets,&selectedLeptons,&primaryLeptons,&selectedPhotons,&vetoedTracks,configSet.jets);
    cfgSet::selectISRJets(jets, &isrJets);
  }
  nJets    = jets.size();
  nBJets   = bJets.size();

  //fill MVA resolved tops
  if (cfgSet::useResolvedTopMVA){
    resolvedTops = resTopMVA->getTopCandidates(jets, ResolvedTopMVA::WP_TIGHT);
    std::sort(resolvedTops.begin(), resolvedTops.end(), [](const TopCand &a, const TopCand &b){ return a.topcand.pt()>b.topcand.pt(); });
  }

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
