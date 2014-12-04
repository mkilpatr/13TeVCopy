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
                                      globalTag        = cms.string ('?'),                    
                                      process          = cms.string ('?'),                      #Physics process (QCD, TTBAR..)
                                      dataset          = cms.string ('?'),                      #Full dataset name
                                      crossSection     = cms.double (-99),                      #Input process cross section
                                      totalEvents      = cms.int32  (-99),                      #Total number of ecents in the sample
                                      crossSectionScaling     = cms.double (1),                 #Luminosity to scale to (given the cross section and total events)
                                      genEventInfoSource      = cms.InputTag (''),                                                                                                                                  
                                      )

nominal_configuration = cms.PSet(
                                      outputPath       = cms.untracked.string ('evttree.root'), #give an output path
                                      outputInterval   = cms.untracked.int32 (-9),              #how frequently to update the outputfile
                                      randomSeed       = cms.uint32 (1234),                     #random seed for the event
                                                                                              
                                      isData           = cms.int32  (0),                        #True if running on data
                                      globalTag        = cms.string ('PHYS14_25_V1'),           #Global tag
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
                                      reGenJets         = cms.InputTag('redAK4','Gen'),
                                      stdGenJets        = cms.InputTag('slimmedGenJets'),
                                      fillReGenJets     = cms.untracked.bool(False),
                                      genParticles      = cms.InputTag('prunedGenParticles'),
                                      minJetPt          = cms.untracked.double(15.0),
                                      fillJetGenInfo    = cms.untracked.bool(True),
                                      fillJetShapeInfo  = cms.untracked.bool(True),
                                  ),

                                  PuppiJets = cms.untracked.PSet(
                                      isFilled          = cms.untracked.bool(True),
                                      jetsPuppi         = cms.InputTag('ak4PFJetsPuppi'),
                                      reGenJetsPuppi    = cms.InputTag('redAK4Puppi','Gen'),
                                      stdGenJetsPuppi   = cms.InputTag('slimmedGenJets'),
                                      reGenJetAssocPuppi= cms.InputTag('redPuppiGenAssoc:GenPtr'),
                                      fillReGenJetsPuppi= cms.untracked.bool(True),
                                      minJetPtPuppi     = cms.untracked.double(20.0),
                                      fillJetGenInfo    = cms.untracked.bool(True),
                                      fillJetShapeInfo  = cms.untracked.bool(True),
                                      applyJEC          = cms.untracked.bool(False)
                                  ),

                                  TrimmedJets = cms.untracked.PSet(
                                      isFilled             = cms.untracked.bool(True),
                                      jetsTrimmed          = cms.VPSet(
                                                                 cms.PSet(src = cms.InputTag('ak8PFJetsCHSTrimmedr0p1ptf0p03'), label = cms.untracked.string('ak8pfchstrimmed_r0p1_ptf0p03'), genptrsrc = cms.InputTag('redAK8TrimmedGenAssoc:GenPtr')),
                                                             ),
                                      reGenJetsTrimmed     = cms.InputTag('redAK8CHS','Gen'),
                                      stdGenJetsTrimmed    = cms.InputTag('slimmedGenJets'),
                                      fillReGenJetsTrimmed = cms.untracked.bool(True),
                                      minJetPtTrimmed      = cms.untracked.double(15.0),
                                      fillJetGenInfo       = cms.untracked.bool(True),
                                      fillJetShapeInfo     = cms.untracked.bool(True),
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
                                      fillElectronIsoVars       = cms.untracked.bool(False),
                                      evaluateElectronPOGIDMVA  = cms.untracked.bool(True),
                                      fillElectronGenInfo       = cms.untracked.bool(True),
                                  ),

                                  Taus = cms.untracked.PSet(
                                      isFilled                  = cms.untracked.bool(True),
                                      taus                      = cms.InputTag('slimmedTaus'),
                                      minTauPt                  = cms.untracked.double(18.0),
                                      printTauIDs               = cms.untracked.bool(False),
                                      fillRawTauDiscriminators  = cms.untracked.bool(False),
                                  )
                                 )
