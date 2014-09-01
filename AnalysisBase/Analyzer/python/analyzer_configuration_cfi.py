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
                                      
                                      vertices          = cms.InputTag("offlineSlimmedPrimaryVertices"),
                                      mets              = cms.InputTag("slimmedMETs"),  
                                      
                                      jets              = cms.InputTag('slimmedJets'),
                                      reGenJets         = cms.InputTag('slimmedGenJets'),
                                      stdGenJets        = cms.InputTag('slimmedGenJets'),
                                      genParticles      = cms.InputTag('prunedGenParticles'),
                                      minJetPt          = cms.untracked.double(15.0),
                                                                                                                                                                                        
                                      )