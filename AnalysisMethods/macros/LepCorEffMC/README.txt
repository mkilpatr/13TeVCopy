SUS Data/MC Lepton Scale Factors 
[0] https://twiki.cern.ch/twiki/bin/viewauth/CMS/SUSLeptonSF
[1] https://github.com/UCSBSusy/13TeVAnalysis/tree/74X_prod_v2/AnalysisMethods/macros/TnP

To run the lepton efficiencies, assuming we get the TnP efficiencies either from the
working group [0] or from the official TnP package Valentina adapted to our FW [1] (Last rebooted July 2016), one must:

0) Compare the current makeZeroLeptonSRTrees.C (or whatever's the current SR tree maker) to makeZeroLeptonLepCorEffMCTrees.C, and perform any needed updates.
1) Run this to make four sets of ttbarplusw trees. Before each run change the bools indicated 
by 'changeme'. One bool is to toggle SR/CR lep configs (zl_search_set and zl_lepton_set,
respectively) and the other to toggle Iso/Id trees (relaxed configs applied to lep
selections). Use ttbar and w+jets samples. See below.
2) Merge each set of trees and feed their location to getLepCorEffs.C. This runs over
the four combinations and produces the root files to drop in the data directory, as 
well as plots (svg/pdf) for each. To quickly check which regions are ~ 1, try:
root -l <root>.root
.ls
<hist>.Draw("texte");
< highlight a subregion on the axis to deoverlap numbers >

For step (2):
<change macro to id=true, sr=true>
./process.py -p ../0LepSearchRegions -m makeZeroLeptonLepCorEffMCTrees.C -c plotting/run_ttbarplusw.conf -t condor -o ./ntuples/LepEff/80X-22062016/id/sr/pieces/ -s submitidsr
./submitidsr.sh

<change macro to id=true, sr=false>
./process.py -p ../0LepSearchRegions -m makeZeroLeptonLepCorEffMCTrees.C -c plotting/run_ttbarplusw.conf -t condor -o ./ntuples/LepEff/80X-22062016/id/cr/pieces -s submitidcr
./submitidcr.sh

<change macro to id=false, sr=true>
./process.py -p ../0LepSearchRegions -m makeZeroLeptonLepCorEffMCTrees.C -c plotting/run_ttbarplusw.conf -t condor -o ./ntuples/LepEff/80X-22062016/iso/sr/pieces -s submitisosr
./submitisosr.sh

<change macro to id=false, sr=false>
./process.py -p ../0LepSearchRegions -m makeZeroLeptonLepCorEffMCTrees.C -c plotting/run_ttbarplusw.conf -t condor -o ./ntuples/LepEff/80X-22062016/iso/cr/pieces -s submitisocr
./submitisocr.sh

cd /eos/uscms/store/user/apatters/ntuples/LepEff/80X-22062016

cd id/sr
<if madgraph>
hadd -f ttbar-mg_tree.root pieces/ttbar-mg_*
hadd -f wjets-ht_tree.root pieces/wjets-ht_*
hadd -f ttbarplusw-mg_tree.root ttbar-mg_tree.root wjets-ht_tree.root

<if powheg>
hadd -f ttbar-powheg-ext_tree.root pieces/ttbar-powheg-ext_*
hadd -f wjets-ht_tree.root pieces/wjets-ht_*
hadd -f ttbarplusw_tree.root ttbar-powheg-ext_tree.root wjets-ht_tree.root

cd ../../id/cr
hadd -f ttbar-mg_tree.root pieces/ttbar-mg_* 
hadd -f wjets-ht_tree.root pieces/wjets-ht_*
hadd -f ttbarplusw-mg_tree.root ttbar-mg_tree.root wjets-ht_tree.root

hadd -f ttbar-powheg-ext_tree.root pieces/ttbar-powheg-ext_*
hadd -f wjets-ht_tree.root pieces/wjets-ht_*
hadd -f ttbarplusw_tree.root ttbar-powheg-ext_tree.root wjets-ht_tree.root

cd ../../iso/sr
hadd -f ttbar-mg_tree.root pieces/ttbar-mg_* 
hadd -f wjets-ht_tree.root pieces/wjets-ht_*
hadd -f ttbarplusw-mg_tree.root ttbar-mg_tree.root wjets-ht_tree.root

hadd -f ttbar-powheg-ext_tree.root pieces/ttbar-powheg-ext_*
hadd -f wjets-ht_tree.root pieces/wjets-ht_*
hadd -f ttbarplusw_tree.root ttbar-powheg-ext_tree.root wjets-ht_tree.root

cd ../../iso/cr
hadd -f ttbar-mg_tree.root pieces/ttbar-mg_* 
hadd -f wjets-ht_tree.root pieces/wjets-ht_*
hadd -f ttbarplusw-mg_tree.root ttbar-mg_tree.root wjets-ht_tree.root

hadd -f ttbar-powheg-ext_tree.root pieces/ttbar-powheg-ext_*
hadd -f wjets-ht_tree.root pieces/wjets-ht_*
hadd -f ttbarplusw_tree.root ttbar-powheg-ext_tree.root wjets-ht_tree.root

