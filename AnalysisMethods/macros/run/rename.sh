#!/bin/bash

workdir=$1

cd $workdir

# sr
cd sr/
hadd diboson_tree.root ww_tree.root wz_tree.root zz_tree.root tWZ_tree.root ttH_tree.root
ln -s ../qcd/* ./
cd ..

# photoncr
cd photoncr/
hadd singlepho_tree.root singlephoton_tree.root jetht_tree.root
hadd photon_tree.root gjets_tree.root qcd-fake_tree.root qcd-frag_tree.root ttg_tree.root
cd ..

# zllcr
cd zllcr/
hadd doublelep_tree.root doubleeg_tree.root doublemu_tree.root singleel_tree.root singlemu_tree.root
hadd z-soup_tree.root dyll_tree.root ttZ_tree.root wz_tree.root zz_tree.root
hadd t-soup_tree.root ttbar_tree.root ttW_tree.root tW_tree.root ww_tree.root
cd ..

exit 0

