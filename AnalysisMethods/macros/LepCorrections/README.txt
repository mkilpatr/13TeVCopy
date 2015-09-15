To make use of the macros in this folder:

1)To generate the appropriate corrections, make your zero lepton trees with the makeZeroLepTree's macro, then once these have been created you may run

createSlimLepCorrections.C

this will output the corrections into lepCorr_"set lumi"

where "set lumi" is a lumi you must manually set

1)To reproduce my more in depth studies you may use the processLepEff in conjunction with the createLepEffMaterials.

2)To perform in depth tau studies, please use the tauEffAnalysis and the CreateHTRatios


