===== naming conventions etc =====
Necessary for the limit code (makeDatacards.py) and some of the background prediction macros. 
Currently there are config files to create all the unput files under plotting (run_0LSR.conf, run_Photon.conf, run_Signals.conf, run_ZtoLL.conf) and macros to compile/combine/link them in AnalysisMethods/macros/run/{mergeTrees.py,rename.sh}. The most up-to-date set of files needed for the limits can be compiled from the signal, background, and control region samples listed in the default limit config file (Limits/dc_0l_setup.conf). The following is the current summary and conventions.

[Example Workflow]
(1) Make trees:
./process.py -p ../0LepSearchRegions -m makeZeroLeptonSRTrees.C -c plotting/run_0LSR.conf -t condor -o [your path]/sr/pieces
./submitall.sh
./process.py -p ../0LepSearchRegions -m makeZeroLeptonPhotonCRTrees.C -c plotting/run_Photon.conf -t condor -o [your path]/photoncr/pieces
./submitall.sh
./process.py -p ../0LepSearchRegions -m makeZeroLeptonZtoLLCRTrees.C  -c plotting/run_ZtoLL.conf  -t condor -o [your path]/zllcr/pieces
./submitall.sh
# QCD w/ bootstrapping
./process.py -p ../0LepSearchRegions -m makeZeroLeptonSlimmedQCDTrees.C -c plotting/run_QCD.conf -t condor -o [your path]/qcd/pieces
./submitall.sh
# signals
./process.py -p ../0LepSearchRegions -m makeZeroLeptonSRTrees.C -c plotting/run_SignalsHM.conf -t condor -o [your path]/signals
./submitall.sh
./process.py -p ../0LepSearchRegions -m makeZeroLeptonSRTrees.C -c plotting/run_SignalsLM.conf -t condor -o [your path]/signals
./submitall.sh
###./process.py -p ../0LepSearchRegions -m makeZeroLeptonMETplusLepCRTrees.C -c plotting/run_LepCR.conf -t condor -o [your path]/lepcr/pieces
###./process.py -p ../0LepSearchRegions -m makeZeroLeptonMETplusLepCRTrees.C -c plotting/run_SignalsLM.conf -t condor -o [your path]/onelepcr/signals
# signals -- genMET
./process.py -p ../0LepSearchRegions -m makeZeroLeptonSRTreesGenMET.C -c plotting/run_SignalsHM.conf -t condor -o [your path]/signals_GenMET
./process.py -p ../0LepSearchRegions -m makeZeroLeptonSRTreesGenMET.C -c plotting/run_SignalsLM.conf -t condor -o [your path]/signals_GenMET
###./process.py -p ../0LepSearchRegions -m makeZeroLeptonMETplusLepCRTreesGenMET.C -c plotting/run_SignalsLM.conf -t condor -o [your path]/onelepcr/signals_GenMET
(2) Merge trees:
./mergeTrees.py -c ../0LepSearchRegions/plotting/run_0LSR.conf -i [your path]/sr/pieces -o [your path]/sr
./mergeTrees.py -c ../0LepSearchRegions/plotting/run_Photon.conf -i [your path]/photoncr/pieces -o [your path]/photoncr
./mergeTrees.py -c ../0LepSearchRegions/plotting/run_ZtoLL.conf -i [your path]/zllcr/pieces -o [your path]/zllcr
./mergeTrees.py -c ../0LepSearchRegions/plotting/run_QCD.conf -i [your path]/qcd/pieces -o [your path]/qcd
###./mergeTrees.py -c ../0LepSearchRegions/plotting/run_LepCR.conf -i [your path]/lepcr/pieces -o [your path]/lepcr
For signal trees, just move them to /uscms_data/d3/hqu/trees/20161201.
(3a) Merge trees for bkg estimation:
./rename.sh [your path]
(3b) Rename and link trees (for limits):
./renameAndCopy.sh [your path] [dest path]

***Note that all trees need to be in the same directory to make datacards***

Naming conventions:
  - all trees need to have the same ending (defaults are set to _tree.root)
  - signal points: model_mStop_mLSP (e.g. T2tt_800_450_tree.root)
  - backgrounds: backgroundName (e.g. ttbarplusw_tree.root)
  - control regions: sampleName_controlRegion (e.g. ttbarplusw_onelepcr_tree.root)

