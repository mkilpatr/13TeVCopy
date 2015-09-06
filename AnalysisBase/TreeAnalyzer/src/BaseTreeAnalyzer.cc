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
#include "AnalysisBase/TreeAnalyzer/interface/JetCorrections.h"

using namespace std;
using namespace ucsbsusy;

//--------------------------------------------------------------------------------------------------
BaseTreeAnalyzer::BaseTreeAnalyzer(TString fileName, TString treeName, bool isMCTree,cfgSet::ConfigSet * pars, TString readOption) : isLoaded_(false),isProcessed_(false), reader(fileName,treeName,readOption),
    run               (0),
    lumi              (0),
    event             (0),
    weight            (1),
    process           (defaults::NUMPROCESSES),
    datareco          (defaults::MC),
    nPV               (0),
    rho               (0),
    nSelLeptons       (0),
    nVetoedLeptons    (0),
    nVetoedTracks     (0),
    nJets             (0),
    nBJets            (0),
    nVetoHPSTaus      (0),
    met               (0),
    genmet            (0),
    goodvertex        (false),
    isOneLeptonSample (false),
    isMC_             (isMCTree),
    defaultJets       (0),
    configSet         (pars ? *pars : cfgSet::ConfigSet())

{
  clog << "Running over: " << (isMC_ ? "MC" : "data") <<endl;

  clog <<"Loaded configurations: ";
  if(configSet.jets           .isConfig()) clog << configSet.jets  <<" ";
  if(configSet.selectedLeptons.isConfig()) clog << configSet.selectedLeptons <<" ";
  if(configSet.vetoedLeptons  .isConfig()) clog << configSet.vetoedLeptons   <<" ";
  if(configSet.vetoedTracks   .isConfig()) clog << configSet.vetoedTracks    <<" ";
  if(configSet.selectedPhotons.isConfig()) clog << configSet.selectedPhotons <<" ";
  if(configSet.corrections.isConfig())     clog << configSet.corrections <<" ";

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
      corrections.push_back(&eventCorrections);
    }
  }

    jetCorrector.setJES(configSet.jets.JES);
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
      if(configSet.vetoedTracks.isConfig() && !configSet.vetoedTracks.mtPresel )
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
void BaseTreeAnalyzer::loadVariables()
{
  load(cfgSet::EVTINFO);
  load(cfgSet::AK4JETS);
  load(cfgSet::ELECTRONS);
  load(cfgSet::MUONS);
  load(cfgSet::PHOTONS);
  load(cfgSet::PFCANDS);
  load(cfgSet::CMSTOPS);
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
    rho   = evtInfoReader.rho;
    goodvertex=evtInfoReader.goodvertex;
    met   = &evtInfoReader.met;
    genmet= &evtInfoReader.genmet;
    weight=  evtInfoReader.evtweight;
    process =  evtInfoReader.process;
    datareco =  evtInfoReader.datareco;
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
  vetoedLeptons.clear();
  if(muonReader.isLoaded() || electronReader.isLoaded()){
    allLeptons.reserve(electronReader.electrons.size() + muonReader.muons.size());
    if(electronReader.isLoaded())
      for(auto& electron : electronReader.electrons)
        allLeptons.push_back(&electron);
    if(muonReader.isLoaded())
      for(auto& muon : muonReader.muons)
       allLeptons.push_back(&muon);
    std::sort(allLeptons.begin(), allLeptons.end(), PhysicsUtilities::greaterPTDeref<LeptonF>());

    if(configSet.selectedLeptons.isConfig())
      cfgSet::selectLeptons(selectedLeptons, allLeptons, configSet.selectedLeptons);

    if(configSet.vetoedLeptons.isConfig())
      cfgSet::selectLeptons(vetoedLeptons, allLeptons, configSet.vetoedLeptons);
  }
  nSelLeptons = selectedLeptons.size();
  nVetoedLeptons = vetoedLeptons.size();

  vetoedTracks.clear();
  if(pfcandReader.isLoaded() && configSet.vetoedTracks.isConfig())
    cfgSet::selectTracks(vetoedTracks,pfcandReader.pfcands, configSet.vetoedTracks);
  nVetoedTracks = vetoedTracks.size();

  if(photonReader.isLoaded() && configSet.selectedPhotons.isConfig())
    cfgSet::selectPhotons(selectedPhotons,photonReader.photons, configSet.selectedPhotons);

  if(tauReader.isLoaded()){
    HPSTaus.clear();
    HPSTaus.reserve(tauReader.taus.size());
    for(auto& tau : tauReader.taus){
      if(tau.pt() > 20 && fabs(tau.eta())<2.4 && (tau.hpsid() & kMediumIsoMVALT) > 0)
        HPSTaus.push_back(&tau);
    }

    nVetoHPSTaus=0;
    if(selectedLeptons.size()==1){
      for(uint iT=0; iT<HPSTaus.size(); ++iT){
        if(PhysicsUtilities::deltaR(HPSTaus.at(iT)->p4(),selectedLeptons.at(0)->p4())<0.4) continue;
        if(HPSTaus.at(iT)->q()*selectedLeptons.at(0)->q()<0)
          nVetoHPSTaus++;
      }
    }
  }
  
  jetCorrector.shiftJES(defaultJets->recoJets, met);
  if(tauReader.isLoaded()){
    HPSTaus.clear();
    HPSTaus.reserve(tauReader.taus.size());
    for(auto& tau : tauReader.taus){
      if(tau.pt() > 20 && fabs(tau.eta())<2.4 && (tau.hpsid() & kMediumIsoMVALT) > 0)
        HPSTaus.push_back(&tau);
    }

    nVetoHPSTaus=0;
    if(selectedLeptons.size()==1){
      for(uint iT=0; iT<HPSTaus.size(); ++iT){
        if(PhysicsUtilities::deltaR(HPSTaus.at(iT)->p4(),selectedLeptons.at(0)->p4())<0.4) continue;
        if(HPSTaus.at(iT)->q()*selectedLeptons.at(0)->q()<0)
          nVetoHPSTaus++;
      }
    }
  }

  jets.clear(); bJets.clear(); nonBJets.clear();
  if(defaultJets && defaultJets->isLoaded() && configSet.jets.isConfig()){
    if(configSet.jets.applyAdHocPUCorr) cfgSet::applyAdHocPUCorr(defaultJets->recoJets, *defaultJets->jetarea_, rho);
    cfgSet::selectJets(jets, &bJets, &nonBJets, defaultJets->recoJets,&selectedLeptons,&vetoedLeptons,&selectedPhotons,configSet.jets);
  }
  nJets    = jets.size();
  nBJets   = bJets.size();

  //load corrections corrections
  for(auto * iC : corrections){
    iC->processCorrection(this);
  }

}
//--------------------------------------------------------------------------------------------------
void BaseTreeAnalyzer::analyze(int reportFrequency, int numEvents)
{
  clog << "Running over " << (numEvents < 0 ? "all" : TString::Format("at most %i",numEvents).Data()) << " events"  <<endl;
  loadVariables();
  isLoaded_ = true;

  while(reader.nextEvent(reportFrequency)){
    isProcessed_ = false;
    if(numEvents >= 0 && getEventNumber() >= numEvents) return;
    processVariables();
    runEvent();
  }
}
