import FWCore.ParameterSet.Config as cms

baseAnalyzer_configuration = cms.PSet(
                                      outputPath       = cms.untracked.string ('plots.root'), #give an output path if you want plotter to produce one
                                      outputInterval   = cms.untracked.int32 (-9),            #how rewuesntly to update the outputfile
                                      randomSeed       = cms.uint32 (1234),                   #random seed for the event
                                      )

physicsAnalyzer_configuration = cms.PSet(
                                      outputPath       = cms.untracked.string ('plots.root'), #give an output path if you want plotter to produce one
                                      outputInterval   = cms.untracked.int32 (-9),            #how rewuesntly to update the outputfile
                                      randomSeed       = cms.uint32 (1234),                   #random seed for the event
                                                                                              
                                      isData           = cms.int32  (0),                      #True if running on data
                                      globalTag        = cms.string ("?"),                    
                                      process          = cms.string ("?"),                    #Physics process (QCD, TTBAR..)
                                      dataset          = cms.string ("?"),                    #Full dataset name
                                      crossSection     = cms.double (-99),                    #Input process cross section
                                      totalEvents      = cms.int32  (-99),                    #Total number of ecents in the sample
                                      crossSectionScaling     = cms.double (1),               #Luminosity to scale to (given the cross section and total events)
                                      genEventInfoSource      = cms.InputTag (""),                                                                                                                                  
                                      )

nominal_configuration = cms.PSet(
                                      outputPath       = cms.untracked.string ('plots.root'), #give an output path if you want plotter to produce one
                                      outputInterval   = cms.untracked.int32 (-9),            #how rewuesntly to update the outputfile
                                      randomSeed       = cms.uint32 (1234),                   #random seed for the event
                                                                                              
                                      isData           = cms.int32  (0),                      #True if running on data
                                      globalTag        = cms.string ("?"),                    
                                      process          = cms.string ("?"),                    #Physics process (QCD, TTBAR..)
                                      dataset          = cms.string ("?"),                    #Full dataset name
                                      crossSection     = cms.double (-99),                    #Input process cross section
                                      totalEvents      = cms.int32  (-99),                    #Total number of ecents in the sample
                                      crossSectionScaling     = cms.double (1),               #Luminosity to scale to (given the cross section and total events)
                                      genEventInfoSource      = cms.InputTag (""),                                                                                
                                      
                                      bunchSpacing      = cms.untracked.int32(25),

                                      vertices          = cms.InputTag("offlineSlimmedPrimaryVertices"),
                                      mets              = cms.InputTag("slimmedMETs"),  
                                      
                                      jets              = cms.InputTag('slimmedJets'),
                                      reGenJets         = cms.InputTag('slimmedGenJets'),
                                      stdGenJets        = cms.InputTag('slimmedGenJets'),
                                      genParticles      = cms.InputTag('prunedGenParticles'),
                                      minJetPt          = cms.untracked.double(15.0),
                                      fillJetGenInfo    = cms.untracked.bool(True),
                                      jetsType          = cms.untracked.string('ak4pfchs'),

                                      muons             = cms.InputTag('slimmedMuons'),
                                      minMuonPt         = cms.untracked.double(5.0),
                                      requireLooseMuon  = cms.untracked.bool(True),
                                      fillMuonIDVars    = cms.untracked.bool(False),
                                      fillMuonIsoVars   = cms.untracked.bool(True),

                                      electrons                 = cms.InputTag('slimmedElectrons'),
                                      minElectronPt             = cms.untracked.double(5.0),
                                      printElectronIDs          = cms.untracked.bool(False),
                                      fillElectronIDVars        = cms.untracked.bool(False),
                                      fillElectronIsoVars       = cms.untracked.bool(False),
                                      evaluateElectronPOGIDMVA  = cms.untracked.bool(True),

                                      taus                      = cms.InputTag('slimmedTaus'),
                                      minTauPt                  = cms.untracked.double(18.0),
                                      printTauIDs               = cms.untracked.bool(False),
                                      fillRawTauDiscriminators  = cms.untracked.bool(False),
                                 )
