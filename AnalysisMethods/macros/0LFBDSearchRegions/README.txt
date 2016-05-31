===== naming conventions etc =====
Necessary for the limit code (makeDatacards.py) and some of the background prediction macros. 
Currently there are config files to create all the unput files under plotting (run_0LSR.conf, run_Photon.conf, run_Signals.conf, run_ZtoLL.conf) and macros to compile/combine/link them in AnalysisMethods/macros/run/{mergeTrees.py,rename.sh}. The most up-to-date set of files needed for the limits can be compiled from the signal, background, and control region samples listed in the default limit config file (Limits/dc_0l_setup.conf). The following is the current summary and conventions.

[Example Workflow]
(1) Make trees:
./process.py -p ../0LFBDSearchRegions -m makeZeroLeptonPhotonCRTreesFBD.C -c run_PhotonFBD.conf -t condor -o [your path]/photoncr/pieces
./process.py -p ../0LFBDSearchRegions -m makeZeroLeptonZtoLLCRTreesFBD.C  -c run_ZtoLLFBD.conf  -t condor -o [your path]/zllcr/pieces
./process.py -p ../0LFBDSearchRegions -m makeZeroLeptonSRTreesFBD.C -c run_0LFBDSR.conf -t condor -o [your path]/sr/pieces
./process.py -p ../0LFBDSearchRegions -m makeZeroLeptonMETplusLepCRFBD.C -c run_0LFBDLepCR.conf -t condor -o [your path]/lepcr/pieces
./process.py -p ../0LFBDSearchRegions -m makeZeroLeptonSRTreesFBD.C -c run_SignalsFBD.conf -t condor -o [your path]/sr/signals
./process.py -p ../0LFBDSearchRegions -m makeZeroLeptonMETplusLepCRFBD.C -c run_SignalsFBD.conf -t condor -o [your path]/lepcr/signals
(2) Merge trees:
./mergeTrees.py -c ../0LFBDSearchRegions/run_PhotonFBD.conf -i [your path]/photoncr/pieces -o [your path]/photoncr
./mergeTrees.py -c ../0LFBDSearchRegions/run_ZtoLLFBD.conf -i [your path]/zllcr/pieces -o [your path]/zllcr
./mergeTrees.py -c ../0LFBDSearchRegions/run_0LFBDSR.conf -i [your path]/sr/pieces -o [your path]/sr
./mergeTrees.py -c ../0LFBDSearchRegions/run_0LFBDLepCR.conf -i [your path]/lepcr/pieces -o [your path]/lepcr
(3) Rename and link trees:
./renameT2fbd.sh [your path]

***Note that all trees need to be in the same directory to make datacards***

Naming conventions:
  - all trees need to have the same ending (defaults are set to _tree.root)
  - signal points: model_mStop_mLSP (e.g. T2tt_800_450_tree.root)
  - backgrounds: backgroundName (e.g. ttbarplusw_tree.root)
  - control regions: sampleName_controlRegion (e.g. ttbarplusw_onelepcr_tree.root)

Background MC need to be combined in the following files:
  ttbarplusw: ttbar-mg, wjets-ht, tW, ttW
  znunu: znunu
  qcd: qcd

Control region MC need to be combined in the following files:
  gjets_photoncr: gjets, qcd-fake, qcd-frag
  ttbarplusw_onelepcr: ttbar-mg, wjets-ht, tW, ttW
  qcd_qcdcr: qcd
  nonqcd_qcdcr: ttbarplusw, znunu

Z->ll CR:
  ttbar: ttbar-mg, ttW, tW
  zll  : dyjetstoll-madgraph, tZq, ttZ

Control region data:
  data_photoncr: singlepho-2015d
  data_onelepcr: htmht
  data_qcdcr: htmht

