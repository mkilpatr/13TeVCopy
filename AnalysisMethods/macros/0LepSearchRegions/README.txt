===== naming conventions etc =====
Necessary for the limit code (makeDatacards.py) and some of the background prediction macros. 
Currently there are config files to create all the unput files under plotting (run_0LSR.conf, run_Photon.conf, run_Signals.conf, run_ZtoLL.conf) and macros to compile/combine/link them in AnalysisMethods/macros/run/{mergeTrees.py,rename.sh}. The most up-to-date set of files needed for the limits can be compiled from the signal, background, and control region samples listed in the default limit config file (Limits/dc_0l_setup.conf). The following is the current summary and conventions.

[Example Workflow]
(1) Make trees:
./process.py -p ../0LepSearchRegions -m makeZeroLeptonSRTrees.C -c plotting/run_0LSR.conf -t condor -o [your path]/sr/pieces
./process.py -p ../0LepSearchRegions -m makeZeroLeptonMETplusLepCRTrees.C -c plotting/run_LepCR.conf -t condor -o [your path]/lepcr/pieces
./process.py -p ../0LepSearchRegions -m makeZeroLeptonPhotonCRTrees.C -c plotting/run_Photon.conf -t condor -o [your path]/photoncr/pieces
./process.py -p ../0LepSearchRegions -m makeZeroLeptonZtoLLCRTrees.C  -c plotting/run_ZtoLL.conf  -t condor -o [your path]/zllcr/pieces
# QCD w/ bootstrap
./process.py -p ../0LepSearchRegions -m makeZeroLeptonQCDTrees.C -c plotting/run_QCD.conf -t condor -o [your path]/qcd-std/pieces
./process.py -p ../0LepSearchRegions -m makeZeroLeptonQCDTreesFBD.C -c plotting/run_QCD.conf -t condor -o [your path]/qcd-4bd/pieces
# signals
./process.py -p ../0LepSearchRegions -m makeZeroLeptonSRTrees.C -c plotting/run_SignalsHM.conf -t condor -o [your path]/signals
./process.py -p ../0LepSearchRegions -m makeZeroLeptonSRTrees.C -c plotting/run_SignalsLM.conf -t condor -o [your path]/signals
./process.py -p ../0LepSearchRegions -m makeZeroLeptonMETplusLepCRTrees.C -c plotting/run_SignalsLM.conf -t condor -o [your path]/onelepcr/signals
# signals -- genMET
./process.py -p ../0LepSearchRegions -m makeZeroLeptonSRTreesGenMET.C -c plotting/run_SignalsHM.conf -t condor -o [your path]/signals_GenMET
./process.py -p ../0LepSearchRegions -m makeZeroLeptonSRTreesGenMET.C -c plotting/run_SignalsLM.conf -t condor -o [your path]/signals_GenMET
./process.py -p ../0LepSearchRegions -m makeZeroLeptonMETplusLepCRTreesGenMET.C -c plotting/run_SignalsLM.conf -t condor -o [your path]/onelepcr/signals_GenMET
(2) Merge trees:
./mergeTrees.py -c ../0LepSearchRegions/plotting/run_0LSR.conf -i [your path]/sr/pieces -o [your path]/sr
./mergeTrees.py -c ../0LepSearchRegions/plotting/run_LepCR.conf -i [your path]/lepcr/pieces -o [your path]/lepcr
./mergeTrees.py -c ../0LepSearchRegions/plotting/run_Photon.conf -i [your path]/photoncr/pieces -o [your path]/photoncr
./mergeTrees.py -c ../0LepSearchRegions/plotting/run_ZtoLL.conf -i [your path]/zllcr/pieces -o [your path]/zllcr
./mergeTrees.py -c ../0LepSearchRegions/plotting/run_QCD.conf -i [your path]/qcd-std/pieces -o [your path]/qcd-std
./mergeTrees.py -c ../0LepSearchRegions/plotting/run_QCD.conf -i [your path]/qcd-4bd/pieces -o [your path]/qcd-4bd
For signal trees, just move them to [your path].
(3) Rename and link trees:
./renameAndCopy.sh [your path] [dest path]

***Note that all trees need to be in the same directory to make datacards***

Naming conventions:
  - all trees need to have the same ending (defaults are set to _tree.root)
  - signal points: model_mStop_mLSP (e.g. T2tt_800_450_tree.root)
  - backgrounds: backgroundName (e.g. ttbarplusw_tree.root)
  - control regions: sampleName_controlRegion (e.g. ttbarplusw_onelepcr_tree.root)

Background MC need to be combined in the following files:
  ttbarplusw: ttbar-powheg, wjets-ht, tW, ttW
  znunu: znunu
  ttZ: ttZ, tZq
  qcd: qcd

Control region MC need to be combined in the following files:
  gjets_photoncr: gjets, qcd-fake, qcd-frag, ttg
  ttbarplusw_onelepcr: ttbar-powheg, wjets-ht, tW, ttW
  qcd_qcdcr: qcd
  nonqcd_qcdcr: ttbarplusw, znunu, ttZ

Control region data:
  data_photoncr: singlepho-2015d
  data_onelepcr: htmht
  data_qcdcr: htmht

