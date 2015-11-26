To make use of the macros in this folder:

1)To generate the appropriate corrections, make your zero lepton trees with the makeZeroLepTree's macro, then once these have been created you may run

createSlimLepCorrections.C

this will output the corrections into lepCorr_"set lumi"

where "set lumi" is a lumi you must manually set

1)To reproduce my more in depth studies you may use the processLepEff in conjunction with the createLepEffMaterials.

2)To perform in depth tau studies, please use the tauEffAnalysis and the CreateHTRatios

=========================

additional instructions for running createSlimLepCorrections.C:

Use makeZeroLeptonSRTrees.C to make trees for the htmht data and all relevent MC samples. All the MC files need to be added together into one tree. The names/locations of these trees are specified at the top of the macro. The trees I've been using are here:
/uscms/home/mullin/nobackup/stuff2015/lepCor/trees/151008_lepactivity/

The lumi, trigger/filters, cuts, etc are also specified at the top of the file. There's also a string for cuts to get the corrections per bin, but the implementation of this still needs to be verified since I'm still getting some odd results.

A bunch of numbers will be printed out for each lepton type. Most of these are for debugging/cross-checks. The important ones are the MC eff and correction that are printed between the lines of "=".

