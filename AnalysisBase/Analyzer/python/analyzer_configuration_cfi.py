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
                                      globalTag        = cms.string ('MCRUN2_74_V9::All'),                    
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
                                      globalTag        = cms.string ('MCRUN2_74_V9::All'),      #Global tag
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
                                      isFilled          = cms.untracked.bool(True),
                                      vertices          = cms.InputTag('offlineSlimmedPrimaryVertices'),
                                      rho               = cms.InputTag('fixedGridRhoFastjetAll'),
                                      mets              = cms.InputTag('slimmedMETs'),  
                                  ),
                                    
                                  Jets = cms.untracked.PSet(  
                                      isFilled          = cms.untracked.bool(True),
                                      jets              = cms.InputTag('slimmedJets'),
                                      reGenJets         = cms.InputTag('ak4Jets','Gen'),
                                      stdGenJets        = cms.InputTag('slimmedGenJets'),
                                      flvAssoc          = cms.InputTag('ak4FlvAssoc','flavors'),
                                      fillReGenJets     = cms.untracked.bool(True),
                                      genParticles      = cms.InputTag('prunedGenParticles'),
                                      minJetPt          = cms.untracked.double(20.0),
                                      fillJetGenInfo    = cms.untracked.bool(True),
                                      fillJetShapeInfo  = cms.untracked.bool(True),
                                      fillTopJetAssoc   = cms.untracked.bool(True),
                                      fillqgl           = cms.untracked.bool(True)
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
                                      minJetPt           = cms.untracked.double(20.0),
                                      fillCustomBtagInfo = cms.untracked.bool(True),
                                      fillJetGenInfo     = cms.untracked.bool(True),
                                      fillJetShapeInfo   = cms.untracked.bool(True),
                                      fillTopJetAssoc    = cms.untracked.bool(True),
                                      applyJEC           = cms.untracked.bool(False),
                                      fillqgl            = cms.untracked.bool(True)
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
                                      minJetPt             = cms.untracked.double(20.0),
                                      fillCustomBtagInfo   = cms.untracked.bool(True),
                                      fillJetGenInfo       = cms.untracked.bool(True),
                                      fillJetShapeInfo     = cms.untracked.bool(True),
                                      fillTopJetAssoc      = cms.untracked.bool(True),
                                      applyJEC             = cms.untracked.bool(False),
                                      fillqgl              = cms.untracked.bool(False)
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
                                      fillCustomBtagInfo   = cms.untracked.bool(True),
                                      fillJetGenInfo       = cms.untracked.bool(True),
                                      fillJetShapeInfo     = cms.untracked.bool(True),
                                      fillTopJetAssoc      = cms.untracked.bool(True),
                                      applyJEC             = cms.untracked.bool(False),
                                      fillqgl              = cms.untracked.bool(False),
                                      fillSuper            = cms.untracked.bool(True),
                                      superJets            = cms.InputTag('subjetsca','Super'),
                                      superJetAssoc        = cms.InputTag('subjetsca','SuperJetIndex'),
                                      superJetNsub         = cms.InputTag('subjetsca','NCASubjets'),
                                  ),

                                  Muons = cms.untracked.PSet(
                                      isFilled          = cms.untracked.bool(True),
                                      muons             = cms.InputTag('slimmedMuons'),
                                      minMuonPt         = cms.untracked.double(5.0),
                                      requireLooseMuon  = cms.untracked.bool(True),
                                      fillMuonIDVars    = cms.untracked.bool(False),
                                      fillMuonIsoVars   = cms.untracked.bool(True),
                                      fillMuonGenInfo   = cms.untracked.bool(True),
                                  ),

                                  Electrons = cms.untracked.PSet(
                                      isFilled                  = cms.untracked.bool(True),
                                      electrons                 = cms.InputTag('slimmedElectrons'),
                                      vetoId                    = cms.InputTag(''),
                                      looseId                   = cms.InputTag(''),
                                      mediumId                  = cms.InputTag(''),
                                      tightId                   = cms.InputTag(''),
                                      minElectronPt             = cms.untracked.double(5.0),
                                      bunchSpacing              = cms.untracked.int32(25),
                                      printElectronIDs          = cms.untracked.bool(False),
                                      fillElectronIDVars        = cms.untracked.bool(False),
                                      fillElectronIsoVars       = cms.untracked.bool(True),
                                      evaluateElectronPOGIDMVA  = cms.untracked.bool(True),
                                      fillElectronGenInfo       = cms.untracked.bool(True),
                                  ),

                                  Taus = cms.untracked.PSet(
                                      isFilled                  = cms.untracked.bool(True),
                                      taus                      = cms.InputTag('slimmedTaus'),
                                      minTauPt                  = cms.untracked.double(18.0),
                                      printTauIDs               = cms.untracked.bool(False),
                                      fillRawTauDiscriminators  = cms.untracked.bool(False),
                                  ),
                                 
                                 Photons = cms.untracked.PSet(
                                      isFilled                  = cms.untracked.bool(True),
                                      photons                   = cms.InputTag('slimmedPhotons'),
                                      looseId                   = cms.InputTag(''),
                                      mediumId                  = cms.InputTag(''),
                                      tightId                   = cms.InputTag(''),
                                      minPhotonPt               = cms.untracked.double(5.0),
                                      fillPhotonIDVars          = cms.untracked.bool(False),
                                      fillPhotonIsoVars         = cms.untracked.bool(False),
                                  ),

                                  PFCandidates = cms.untracked.PSet(
                                      isFilled                  = cms.untracked.bool(True),
                                      pfcands                   = cms.InputTag('packedPFCandidates'),
                                      jets                      = cms.InputTag('slimmedJets'),
                                      taus                      = cms.InputTag('slimmedTaus'),
                                      minCandPt                 = cms.untracked.double(8.0),
                                      maxCandEta                = cms.untracked.double(3.0),
                                      minTauDisc                = cms.untracked.double(0.0),
                                      tauMVAFileName_MtPresel   = cms.untracked.string('tauDisc_mt100presel.root'),
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
                                      )
                                )
