import FWCore.ParameterSet.Config as cms

baseAnalyzer_configuration = cms.PSet(
                                      outputPath       = cms.untracked.string ('evttree.root'), #give an output path
                                      outputInterval   = cms.untracked.int32 (-9),              #how frequently to update the outputfile
                                      randomSeed       = cms.uint32 (1234),                     #random seed for the event
                                      )

physicsAnalyzer_configuration = cms.PSet(
                                      outputPath       = cms.untracked.string ('evttree.root'), #give an output path
                                      outputInterval   = cms.untracked.int32 (-9),              #how frequently to update the outputfile
                                      randomSeed       = cms.uint32 (1234),                     #random seed for the event
                                                                                              
                                      isData           = cms.int32  (0),                        #True if running on data
                                      globalTag        = cms.string ('74X_mcRun2_asymptotic_v2'),                    
                                      process          = cms.string ('?'),                      #Physics process (QCD, TTBAR..)
                                      dataset          = cms.string ('?'),                      #Full dataset name
                                      crossSection     = cms.double (-99),                      #Input process cross section
                                      totalEvents      = cms.int32  (-99),                      #Total number of ecents in the sample
                                      crossSectionScaling     = cms.double (1),                 #Luminosity to scale to (given the cross section and total events)
                                      )

nominal_configuration = cms.PSet(
                                      outputPath       = cms.untracked.string ('evttree.root'), #give an output path
                                      outputInterval   = cms.untracked.int32 (-9),              #how frequently to update the outputfile
                                      randomSeed       = cms.uint32 (1234),                     #random seed for the event
                                                                                              
                                      isData           = cms.int32  (0),                        #True if running on data
                                      globalTag        = cms.string ('74X_mcRun2_asymptotic_v2'),      #Global tag
                                      process          = cms.string ('?'),                      #Physics process (QCD, TTBAR..)
                                      dataset          = cms.string ('?'),                      #Full dataset name
                                      crossSection     = cms.double (-99),                      #Input process cross section
                                      totalEvents      = cms.int32  (-99),                      #Total number of ecents in the sample
                                      crossSectionScaling     = cms.double (1),                 #Luminosity to scale to (given the cross section and total events)

                                  Gen = cms.untracked.PSet(
                                      isFilled            = cms.untracked.bool(True),
                                      genEventInfoSource  = cms.InputTag (''),                                                                                
                                      packedGenParticles  = cms.InputTag('packedGenParticles'),
                                      prunedGenParticles  = cms.InputTag('prunedGenParticles'),
                                      saveAllGenParticles = cms.untracked.bool(False),
                                      savePartonHadronization = cms.untracked.bool(True),
                                  ),

                                  EventInfo = cms.untracked.PSet(
                                      isFilled               = cms.untracked.bool(True),
                                      vertices               = cms.InputTag('offlineSlimmedPrimaryVertices'),
                                      pileupSummaryInfos     = cms.InputTag('slimmedAddPileupInfo'),
                                      rho                    = cms.InputTag('fixedGridRhoFastjetAll'),
                                      mets                   = cms.InputTag('slimmedMETs'),  
                                      metsOOB                = cms.InputTag('slimmedMETs'),  
                                      metsNoHF               = cms.InputTag('slimmedMETsNoHF'),  
                                      puppimets              = cms.InputTag('slimmedMETsPuppi'),
                                      genEvtInfo             = cms.InputTag('generator'),
                                      lheEvtInfo             = cms.InputTag('externalLHEProducer'),
                                      saveSystematicWeights  = cms.untracked.bool(True),
                                      whichSystematicWeights = cms.untracked.vuint32(),
                                  ),
                                    
                                  Jets = cms.untracked.PSet(  
                                      isFilled          = cms.untracked.bool(True),
                                      jets              = cms.InputTag('slimmedJets'),
                                      reGenJets         = cms.InputTag('ak4Jets','Gen'),
                                      stdGenJets        = cms.InputTag('slimmedGenJets'),
                                      flvAssoc          = cms.InputTag('ak4FlvAssoc','flavors'),
                                      qgLikelihood      = cms.untracked.InputTag('QGTagger','qgLikelihood'),
                                      fillReGenJets     = cms.untracked.bool(True),
                                      genParticles      = cms.InputTag('prunedGenParticles'),
                                      minJetPt          = cms.untracked.double(10.0),
                                      completeMinJetPt  = cms.untracked.double(20.0),
                                      fillJetGenInfo    = cms.untracked.bool(True),
                                      fillJetShapeInfo  = cms.untracked.bool(False),
                                      fillTopJetAssoc   = cms.untracked.bool(True),
                                      fillqgl           = cms.untracked.bool(False),
                                      superJets         = cms.untracked.InputTag(''),
                                      superJetAssoc     = cms.untracked.InputTag(''),
                                      superJetNsub      = cms.untracked.InputTag(''),
                                      loadJecUncFile    = cms.untracked.bool(False),
                                      jecLabel          = cms.untracked.string('AK4PFchs'),
                                      jetCorrUncertainty= cms.untracked.string('SubTotalMC'),
                                  ),

                                  PuppiJets = cms.untracked.PSet(
                                      isFilled           = cms.untracked.bool(False),
                                      jets               = cms.InputTag('ak4PuppiJets',''),
                                      btags              = cms.InputTag('ak4PuppiCSVIV2'),
                                      reGenJets          = cms.InputTag('ak4PuppiJets','Gen'),
                                      stdGenJets         = cms.InputTag(''),
                                      flvAssoc           = cms.InputTag('ak4PuppiFlvAssoc','flavors'),
                                      reGenJetAssoc      = cms.InputTag('ak4PuppiJets:GenPtr'),
                                      fillReGenJets      = cms.untracked.bool(True),
                                      minJetPt          = cms.untracked.double(20.0),
                                      completeMinJetPt  = cms.untracked.double(20.0),
                                      fillCustomBtagInfo = cms.untracked.bool(True),
                                      fillJetGenInfo     = cms.untracked.bool(True),
                                      fillJetShapeInfo   = cms.untracked.bool(False),
                                      fillTopJetAssoc    = cms.untracked.bool(True),
                                      applyJEC           = cms.untracked.bool(False),
                                      fillqgl            = cms.untracked.bool(False),
                                      superJets          = cms.untracked.InputTag(''),
                                      superJetAssoc      = cms.untracked.InputTag(''),
                                      superJetNsub       = cms.untracked.InputTag(''),
                                      loadJecUncFile     = cms.untracked.bool(False),
                                  ),

                                  PickyJets = cms.untracked.PSet(
                                      isFilled             = cms.untracked.bool(False),
                                      jets                 = cms.InputTag('pickyJets',''),
                                      btags                = cms.InputTag('pickyCSVIV2'),
                                      reGenJets            = cms.InputTag('pickyJets','Gen'),
                                      stdGenJets           = cms.InputTag(''),
                                      flvAssoc             = cms.InputTag('pickyFlvAssoc','flavors'),
                                      reGenJetAssoc        = cms.InputTag('pickyJets:GenPtr'),
                                      fillReGenJets        = cms.untracked.bool(True),
                                      minJetPt          = cms.untracked.double(20.0),
                                      completeMinJetPt  = cms.untracked.double(20.0),
                                      fillCustomBtagInfo   = cms.untracked.bool(True),
                                      fillJetGenInfo       = cms.untracked.bool(True),
                                      fillJetShapeInfo     = cms.untracked.bool(False),
                                      fillTopJetAssoc      = cms.untracked.bool(True),
                                      applyJEC             = cms.untracked.bool(False),
                                      fillqgl              = cms.untracked.bool(False),
                                      superJets            = cms.untracked.InputTag(''),
                                      superJetAssoc        = cms.untracked.InputTag(''),
                                      superJetNsub         = cms.untracked.InputTag(''),
                                      loadJecUncFile       = cms.untracked.bool(False),
                                  ),

                                  CASubJets = cms.untracked.PSet(
                                      isFilled             = cms.untracked.bool(False),
                                      jets                 = cms.InputTag('subjetsca',''),
                                      btags                = cms.InputTag('subjetscaCSVIV2'),
                                      reGenJets            = cms.InputTag('subjetsca','Gen'),
                                      stdGenJets           = cms.InputTag(''),
                                      flvAssoc             = cms.InputTag('subjetscaFlvAssoc','flavors'),
                                      reGenJetAssoc        = cms.InputTag('subjetsca:GenPtr'),
                                      fillReGenJets        = cms.untracked.bool(True),
                                      minJetPt             = cms.untracked.double(20.0),
                                      completeMinJetPt     = cms.untracked.double(20.0),
                                      fillCustomBtagInfo   = cms.untracked.bool(True),
                                      fillJetGenInfo       = cms.untracked.bool(True),
                                      fillJetShapeInfo     = cms.untracked.bool(True),
                                      fillTopJetAssoc      = cms.untracked.bool(True),
                                      applyJEC             = cms.untracked.bool(False),
                                      fillqgl              = cms.untracked.bool(False),
                                      fillSuper            = cms.untracked.bool(True),
                                      superJets            = cms.untracked.InputTag('subjetsca','Super'),
                                      superJetAssoc        = cms.untracked.InputTag('subjetsca','SuperJetIndex'),
                                      superJetNsub         = cms.untracked.InputTag('subjetsca','NCASubjets'),
                                      loadJecUncFile       = cms.untracked.bool(False),
                                  ),

                                  Muons = cms.untracked.PSet(
                                      isFilled          = cms.untracked.bool(True),
                                      muons             = cms.InputTag('slimmedMuons'),
                                      ca8jets           = cms.InputTag('redCA8'),
                                      rho               = cms.InputTag('fixedGridRhoFastjetCentralNeutral'),
                                      jets              = cms.InputTag('slimmedJets'),
                                      pfcands           = cms.InputTag('packedPFCandidates'),
                                      minMuonPt         = cms.untracked.double(5.0),
                                      requireLooseMuon  = cms.untracked.bool(True),
                                      fillMuonIDVars    = cms.untracked.bool(False),
                                      fillMuonIsoVars   = cms.untracked.bool(False),
                                      fillMuonGenInfo   = cms.untracked.bool(True),
                                  ),

                                  Electrons = cms.untracked.PSet(
                                      isFilled                  = cms.untracked.bool(True),
                                      electrons                 = cms.InputTag('slimmedElectrons'),
                                      vetoId                    = cms.InputTag(''),
                                      looseId                   = cms.InputTag(''),
                                      mediumId                  = cms.InputTag(''),
                                      tightId                   = cms.InputTag(''),
                                      mvanontrigwp90Id          = cms.InputTag(''),
                                      mvanontrigwp80Id          = cms.InputTag(''),
                                      mvanontrigValuesMap       = cms.InputTag(''),
                                      mvanontrigCategoriesMap   = cms.InputTag(''),
                                      mvatrigwp90Id             = cms.InputTag(''),
                                      mvatrigwp80Id             = cms.InputTag(''),
                                      mvatrigValuesMap          = cms.InputTag(''),
                                      mvatrigCategoriesMap      = cms.InputTag(''),
                                      ca8jets                   = cms.InputTag('redCA8'),
                                      rho                       = cms.InputTag('fixedGridRhoFastjetCentralNeutral'),
                                      jets                      = cms.InputTag('slimmedJets'),
                                      pfcands                   = cms.InputTag('packedPFCandidates'),
                                      minElectronPt             = cms.untracked.double(5.0),
                                      bunchSpacing              = cms.untracked.int32(25),
                                      printElectronIDs          = cms.untracked.bool(False),
                                      fillElectronIDVars        = cms.untracked.bool(False),
                                      fillElectronIsoVars       = cms.untracked.bool(False),
                                      evaluateElectronPOGIDMVA  = cms.untracked.bool(False),
                                      fillElectronGenInfo       = cms.untracked.bool(True),
                                  ),

                                  Taus = cms.untracked.PSet(
                                      isFilled                  = cms.untracked.bool(True),
                                      taus                      = cms.InputTag('slimmedTaus'),
                                      minTauPt                  = cms.untracked.double(18.0),
                                      printTauIDs               = cms.untracked.bool(False),
                                      fillCandInfo              = cms.untracked.bool(False),
                                      fillRawTauDiscriminators  = cms.untracked.bool(False),
                                  ),
                                 
                                  Photons = cms.untracked.PSet(
                                      isFilled                  = cms.untracked.bool(True),
                                      photons                   = cms.InputTag('slimmedPhotons'),
                                      electrons                 = cms.InputTag('slimmedElectrons'),
                                      muons                     = cms.InputTag('slimmedMuons'),
                                      jets                      = cms.InputTag('slimmedJets'),
                                      pfcandidates              = cms.InputTag('packedPFCandidates'),
                                      vertices                  = cms.InputTag('offlineSlimmedPrimaryVertices'),
                                      vetoId                    = cms.InputTag(''),
                                      looseId                   = cms.InputTag(''),
                                      mediumId                  = cms.InputTag(''),
                                      tightId                   = cms.InputTag(''),
                                      rho                       = cms.InputTag('fixedGridRhoFastjetAll'),
                                      minPhotonPt               = cms.untracked.double(20.0),
                                      minPhotonPtRC             = cms.untracked.double(15.0),
                                      minJetPtRC                = cms.untracked.double(20.0),
                                      minLeptonPtRC             = cms.untracked.double(10.0),
                                      fillPhotonIDVars          = cms.untracked.bool(True),
                                      fillPhotonIsoVars         = cms.untracked.bool(True),
                                  ),

                                  PFCandidates = cms.untracked.PSet(
                                      isFilled                  = cms.untracked.bool(True),
                                      pfcands                   = cms.InputTag('packedPFCandidates'),
                                      jets                      = cms.InputTag('slimmedJets'),
                                      taus                      = cms.InputTag('slimmedTaus'),
                                      minCandPt                 = cms.untracked.double(9.0),
                                      maxCandEta                = cms.untracked.double(2.6),
                                      minTauDisc                = cms.untracked.double(0.0),
                                      tauMVAFileName_MtPresel   = cms.untracked.string('tauDisc_mt100presel_S15.root'),
                                      tauMVAFileName_DphiPresel = cms.untracked.string('tauDisc_dphi1p34presel.root'),
                                      tauMVAName                = cms.untracked.string('mva_0'),
                                      saveAllCandidates         = cms.untracked.bool(False),
                                  ),

                                  CMSTops = cms.untracked.PSet(
                                      isFilled = cms.untracked.bool(True),
                                      fatJets  = cms.InputTag('slimmedJetsAK8'),
                                      ),
                                      
                                  AK8FatJets = cms.untracked.PSet(
                                      isFilled = cms.untracked.bool(True),
                                      fatJets  = cms.InputTag('slimmedJetsAK8'),
                                      puRemoval = cms.untracked.string("CHS")
                                      ),

                                  AK8PuppiFatJets = cms.untracked.PSet(
                                      isFilled = cms.untracked.bool(True),
                                      fatJets  = cms.InputTag('selectedPatJetsAK8PFPuppi'),
                                      puRemoval = cms.untracked.string("Puppi")
                                      ),

                                  Triggers = cms.untracked.PSet(
                                      isFilled  = cms.untracked.bool(True),
                                      bits      = cms.InputTag('TriggerResults','','HLT'),
                                      objects   = cms.InputTag('selectedPatTrigger'),
                                      prescales = cms.InputTag('patTrigger'),
                                      printTriggerNames = cms.untracked.bool(False)
                                      ),

                                  METFilters = cms.untracked.PSet(
                                      isFastSim = cms.untracked.bool(False),
                                      isFilled = cms.untracked.bool(True),
                                      bits     = cms.InputTag('TriggerResults','','PAT'),
                                      hbhe     = cms.InputTag('HBHENoiseFilterResultProducer', 'HBHENoiseFilterResult'),
                                      hbheiso       = cms.InputTag('HBHENoiseFilterResultProducer', 'HBHEIsoNoiseFilterResult'),
                                      hbherun2loose = cms.InputTag('HBHENoiseFilterResultProducer', 'HBHENoiseFilterResultRun2Loose'),
                                      hbherun2tight = cms.InputTag('HBHENoiseFilterResultProducer', 'HBHENoiseFilterResultRun2Tight'),
                                      ),

                                )
