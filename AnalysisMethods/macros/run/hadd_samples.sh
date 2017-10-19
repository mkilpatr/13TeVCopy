#!/bin/bash
cmsenv
mkdir original_files
mv *.root original_files
cd original_files
hadd met_tree.root met_*
mv met_tree.root ../
hadd tW_tree.root tW_*
mv tW_tree.root ../
hadd ttW_tree.root ttW_*
mv ttW_tree.root ../
hadd ttZ_tree.root ttZ_*
mv ttZ_tree.root ../
hadd ttbar_tree.root ttbar-mg_*
mv ttbar_tree.root ../
hadd wjets_tree.root wjets-ht_*
mv wjets_tree.root ../
hadd znunu_tree.root znunu_*
mv znunu_tree.root ../
cd ..
hadd ttOther_tree.root tW_tree.root ttW_tree.root ttZ_tree.root
hadd ttbarplusw_tree.root ttbar_tree.root wjets_tree.root tW_tree.root ttW_tree.root
hadd nonQCD_tree.root ttbar_tree.root wjets_tree.root tW_tree.root ttW_tree.root znunu_tree.root
hadd remaining_nonQCD_tree.root znunu_tree.root
rm hadd_samples.sh
