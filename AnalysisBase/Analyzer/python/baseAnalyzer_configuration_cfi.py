baseAnalyzer_configuration = cms.PSet(
                                      outputPath       = cms.untracked.string (''),         #give an output path if you want plotter to produce one
                                      outputInterval   = cms.untracked/int32 (-9),          #how rewuesntly to update the outputfile
                                      randomSeed       = cms.uint32 (1234),                 #random seed for the event
                                      )
